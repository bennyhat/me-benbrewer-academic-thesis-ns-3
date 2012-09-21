/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Benjamin Brewer
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Benjamin Brewer <bb1267@my.bristol.ac.uk>
 *
 * Description: adds the ability to reduce the input SNR by a constant factor
 */

#include "ns3/assert.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/tag.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/double.h"

#include "schedulable-mac-low.h"
#include "wifi-phy.h"
#include "wifi-mac-trailer.h"
#include "qos-utils.h"
#include "edca-txop-n.h"

NS_LOG_COMPONENT_DEFINE ("SchedMacLow");

#undef NS_LOG_APPEND_CONTEXT
#define NS_LOG_APPEND_CONTEXT std::clog << "[mac=" << m_self << "] "


namespace ns3 {

class SnrTag : public Tag
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual void Print (std::ostream &os) const;

  void Set (double snr);
  double Get (void) const;
private:
  double m_snr;
};

SchedMacLow::SchedMacLow () :
    m_signalReduction(0.0)
{
  NS_LOG_FUNCTION (this);
}

SchedMacLow::~SchedMacLow ()
{
  NS_LOG_FUNCTION (this);
}

void
SchedMacLow::ReduceSignalStrength (double howMuch)
{
  m_signalReduction = howMuch;
}

void
SchedMacLow::ReceiveOk (Ptr<Packet> packet, double rxSnr, WifiMode txMode, WifiPreamble preamble)
{
  NS_LOG_FUNCTION (this << packet << rxSnr << txMode << preamble);
  /* A packet is received from the PHY.
   * When we have handled this packet,
   * we handle any packet present in the
   * packet queue.
   */
  // BB - only change - we don't really care about the PHY and error rate, we're more interested in
  //     inducing a rate reduction

  if(m_signalReduction > 0.0)
    {
      NS_LOG_DEBUG (Simulator::Now() << " - reducing receive signal strength for " << m_self
          << " from " << rxSnr << " by a factor of " << m_signalReduction);
      rxSnr = rxSnr * m_signalReduction;
    }

  WifiMacHeader hdr;
  packet->RemoveHeader (hdr);

  bool isPrevNavZero = IsNavZero ();
  NS_LOG_DEBUG ("duration/id=" << hdr.GetDuration ());
  NotifyNav (hdr, txMode, preamble);
  if (hdr.IsRts ())
    {
      /* see section 9.2.5.7 802.11-1999
       * A STA that is addressed by an RTS frame shall transmit a CTS frame after a SIFS
       * period if the NAV at the STA receiving the RTS frame indicates that the medium is
       * idle. If the NAV at the STA receiving the RTS indicates the medium is not idle,
       * that STA shall not respond to the RTS frame.
       */
      if (isPrevNavZero
          && hdr.GetAddr1 () == m_self)
        {
          NS_LOG_DEBUG ("rx RTS from=" << hdr.GetAddr2 () << ", schedule CTS");
          NS_ASSERT (m_sendCtsEvent.IsExpired ());
          m_stationManager->ReportRxOk (hdr.GetAddr2 (), &hdr,
                                        rxSnr, txMode);
          m_sendCtsEvent = Simulator::Schedule (GetSifs (),
                                                &SchedMacLow::SendCtsAfterRts, this,
                                                hdr.GetAddr2 (),
                                                hdr.GetDuration (),
                                                txMode,
                                                rxSnr);
        }
      else
        {
          NS_LOG_DEBUG ("rx RTS from=" << hdr.GetAddr2 () << ", cannot schedule CTS");
        }
    }
  else if (hdr.IsCts ()
           && hdr.GetAddr1 () == m_self
           && m_ctsTimeoutEvent.IsRunning ()
           && m_currentPacket != 0)
    {
      NS_LOG_DEBUG ("receive cts from=" << m_currentHdr.GetAddr1 ());
      SnrTag tag;
      packet->RemovePacketTag (tag);
      m_stationManager->ReportRxOk (m_currentHdr.GetAddr1 (), &m_currentHdr,
                                    rxSnr, txMode);
      m_stationManager->ReportRtsOk (m_currentHdr.GetAddr1 (), &m_currentHdr,
                                     rxSnr, txMode, tag.Get ());

      m_ctsTimeoutEvent.Cancel ();
      NotifyCtsTimeoutResetNow ();
      m_listener->GotCts (rxSnr, txMode);
      NS_ASSERT (m_sendDataEvent.IsExpired ());
      m_sendDataEvent = Simulator::Schedule (GetSifs (),
                                             &SchedMacLow::SendDataAfterCts, this,
                                             hdr.GetAddr1 (),
                                             hdr.GetDuration (),
                                             txMode);
    }
  else if (hdr.IsAck ()
           && hdr.GetAddr1 () == m_self
           && (m_normalAckTimeoutEvent.IsRunning ()
               || m_fastAckTimeoutEvent.IsRunning ()
               || m_superFastAckTimeoutEvent.IsRunning ())
           && m_txParams.MustWaitAck ())
    {
      NS_LOG_DEBUG ("receive ack from=" << m_currentHdr.GetAddr1 ());
      SnrTag tag;
      packet->RemovePacketTag (tag);
      m_stationManager->ReportRxOk (m_currentHdr.GetAddr1 (), &m_currentHdr,
                                    rxSnr, txMode);
      m_stationManager->ReportDataOk (m_currentHdr.GetAddr1 (), &m_currentHdr,
                                      rxSnr, txMode, tag.Get ());
      bool gotAck = false;
      if (m_txParams.MustWaitNormalAck ()
          && m_normalAckTimeoutEvent.IsRunning ())
        {
          m_normalAckTimeoutEvent.Cancel ();
          NotifyAckTimeoutResetNow ();
          gotAck = true;
        }
      if (m_txParams.MustWaitFastAck ()
          && m_fastAckTimeoutEvent.IsRunning ())
        {
          m_fastAckTimeoutEvent.Cancel ();
          NotifyAckTimeoutResetNow ();
          gotAck = true;
        }
      if (gotAck)
        {
          m_listener->GotAck (rxSnr, txMode);
        }
      if (m_txParams.HasNextPacket ())
        {
          m_waitSifsEvent = Simulator::Schedule (GetSifs (),
                                                 &SchedMacLow::WaitSifsAfterEndTx, this);
        }
    }
  else if (hdr.IsBlockAck () && hdr.GetAddr1 () == m_self
           && (m_txParams.MustWaitBasicBlockAck () || m_txParams.MustWaitCompressedBlockAck ())
           && m_blockAckTimeoutEvent.IsRunning ())
    {
      NS_LOG_DEBUG ("got block ack from " << hdr.GetAddr2 ());
      CtrlBAckResponseHeader blockAck;
      packet->RemoveHeader (blockAck);
      m_blockAckTimeoutEvent.Cancel ();
      m_listener->GotBlockAck (&blockAck, hdr.GetAddr2 ());
    }
  else if (hdr.IsBlockAckReq () && hdr.GetAddr1 () == m_self)
    {
      CtrlBAckRequestHeader blockAckReq;
      packet->RemoveHeader (blockAckReq);
      if (!blockAckReq.IsMultiTid ())
        {
          uint8_t tid = blockAckReq.GetTidInfo ();
          AgreementsI it = m_bAckAgreements.find (std::make_pair (hdr.GetAddr2 (), tid));
          if (it != m_bAckAgreements.end ())
            {
              //Update block ack cache
              BlockAckCachesI i = m_bAckCaches.find (std::make_pair (hdr.GetAddr2 (), tid));
              NS_ASSERT (i != m_bAckCaches.end ());
              (*i).second.UpdateWithBlockAckReq (blockAckReq.GetStartingSequence ());

              NS_ASSERT (m_sendAckEvent.IsExpired ());
              /* See section 11.5.3 in IEEE802.11 for mean of this timer */
              ResetBlockAckInactivityTimerIfNeeded (it->second.first);
              if ((*it).second.first.IsImmediateBlockAck ())
                {
                  NS_LOG_DEBUG ("rx blockAckRequest/sendImmediateBlockAck from=" << hdr.GetAddr2 ());
                  m_sendAckEvent = Simulator::Schedule (GetSifs (),
                                                        &SchedMacLow::SendBlockAckAfterBlockAckRequest, this,
                                                        blockAckReq,
                                                        hdr.GetAddr2 (),
                                                        hdr.GetDuration (),
                                                        txMode);
                }
              else
                {
                  NS_FATAL_ERROR ("Delayed block ack not supported.");
                }
            }
          else
            {
              NS_LOG_DEBUG ("There's not a valid agreement for this block ack request.");
            }
        }
      else
        {
          NS_FATAL_ERROR ("Multi-tid block ack is not supported.");
        }
    }
  else if (hdr.IsCtl ())
    {
      NS_LOG_DEBUG ("rx drop " << hdr.GetTypeString ());
    }
  else if (hdr.GetAddr1 () == m_self)
    {
      m_stationManager->ReportRxOk (hdr.GetAddr2 (), &hdr,
                                    rxSnr, txMode);

      if (hdr.IsQosData () && StoreMpduIfNeeded (packet, hdr))
        {
          /* From section 9.10.4 in IEEE802.11:
             Upon the receipt of a QoS data frame from the originator for which
             the Block Ack agreement exists, the recipient shall buffer the MSDU
             regardless of the value of the Ack Policy subfield within the
             QoS Control field of the QoS data frame. */
          if (hdr.IsQosAck ())
            {
              AgreementsI it = m_bAckAgreements.find (std::make_pair (hdr.GetAddr2 (), hdr.GetQosTid ()));
              RxCompleteBufferedPacketsWithSmallerSequence (it->second.first.GetStartingSequence (),
                                                            hdr.GetAddr2 (), hdr.GetQosTid ());
              RxCompleteBufferedPacketsUntilFirstLost (hdr.GetAddr2 (), hdr.GetQosTid ());
              NS_ASSERT (m_sendAckEvent.IsExpired ());
              m_sendAckEvent = Simulator::Schedule (GetSifs (),
                                                    &SchedMacLow::SendAckAfterData, this,
                                                    hdr.GetAddr2 (),
                                                    hdr.GetDuration (),
                                                    txMode,
                                                    rxSnr);
            }
          else if (hdr.IsQosBlockAck ())
            {
              AgreementsI it = m_bAckAgreements.find (std::make_pair (hdr.GetAddr2 (), hdr.GetQosTid ()));
              /* See section 11.5.3 in IEEE802.11 for mean of this timer */
              ResetBlockAckInactivityTimerIfNeeded (it->second.first);
            }
          return;
        }
      else if (hdr.IsQosData () && hdr.IsQosBlockAck ())
        {
          /* This happens if a packet with ack policy Block Ack is received and a block ack
             agreement for that packet doesn't exist.

             From section 11.5.3 in IEEE802.11e:
             When a recipient does not have an active Block ack for a TID, but receives
             data MPDUs with the Ack Policy subfield set to Block Ack, it shall discard
             them and shall send a DELBA frame using the normal access
             mechanisms. */
          AcIndex ac = QosUtilsMapTidToAc (hdr.GetQosTid ());
          m_edcaListeners[ac]->BlockAckInactivityTimeout (hdr.GetAddr2 (), hdr.GetQosTid ());
          return;
        }
      else if (hdr.IsQosData () && hdr.IsQosNoAck ())
        {
          NS_LOG_DEBUG ("rx unicast/noAck from=" << hdr.GetAddr2 ());
        }
      else if (hdr.IsData () || hdr.IsMgt ())
        {
          NS_LOG_DEBUG ("rx unicast/sendAck from=" << hdr.GetAddr2 ());
          NS_ASSERT (m_sendAckEvent.IsExpired ());
          m_sendAckEvent = Simulator::Schedule (GetSifs (),
                                                &SchedMacLow::SendAckAfterData, this,
                                                hdr.GetAddr2 (),
                                                hdr.GetDuration (),
                                                txMode,
                                                rxSnr);
        }
      goto rxPacket;
    }
  else if (hdr.GetAddr1 ().IsGroup ())
    {
      if (hdr.IsData () || hdr.IsMgt ())
        {
          NS_LOG_DEBUG ("rx group from=" << hdr.GetAddr2 ());
          goto rxPacket;
        }
      else
        {
          // DROP
        }
    }
  else if (m_promisc)
    {
      NS_ASSERT (hdr.GetAddr1 () != m_self);
      if (hdr.IsData ())
        {
          goto rxPacket;
        }
    }
  else
    {
      //NS_LOG_DEBUG_VERBOSE ("rx not-for-me from %d", GetSource (packet));
    }
  return;
rxPacket:
  WifiMacTrailer fcs;
  packet->RemoveTrailer (fcs);
  m_rxCallback (packet, &hdr);
  return;
}

} // namespace ns3
