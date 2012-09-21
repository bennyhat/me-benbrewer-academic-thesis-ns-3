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
 * Description: This is the class that implements the TXOP behaviour
 *      --NOTE-- there is a bug where TXOP is not reset after a busy time is thrown
 *      during SIFS. This is very rare, but must be fixed
 */
#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/pointer.h"
#include "ns3/simulator.h"

#include "edca-txop-cfb-n.h"
#include "mac-low.h"
#include "dcf-manager.h"
#include "mac-tx-middle.h"
#include "wifi-mac-trailer.h"
#include "wifi-mac.h"
#include "random-stream.h"
#include "wifi-mac-queue.h"
#include "msdu-aggregator.h"
#include "mgt-headers.h"
#include "qos-blocked-destinations.h"

NS_LOG_COMPONENT_DEFINE ("EdcaTxopCfbN");

#undef NS_LOG_APPEND_CONTEXT
#define NS_LOG_APPEND_CONTEXT if (m_low != 0) { std::clog << Simulator::Now() << " [mac=" << m_low->GetAddress () << "] "; }

namespace ns3 {

class EdcaTxopCfbN::Dcf : public DcfState
{
public:
  Dcf (EdcaTxopCfbN * txop)
    : m_txop (txop)
  {
  }
private:
  virtual void DoNotifyAccessGranted (void)
  {
    m_txop->NotifyAccessGranted ();
  }
  virtual void DoNotifyInternalCollision (void)
  {
    m_txop->NotifyInternalCollision ();
  }
  virtual void DoNotifyCollision (void)
  {
    m_txop->NotifyCollision ();
  }
  virtual void DoNotifyChannelSwitching (void)
  {
    m_txop->NotifyChannelSwitching ();
  }
  EdcaTxopCfbN *m_txop;
};

class EdcaTxopCfbN::TransmissionListener : public MacLowTransmissionListener
{
public:
  TransmissionListener (EdcaTxopCfbN * txop)
    : MacLowTransmissionListener (),
      m_txop (txop) {
  }

  virtual ~TransmissionListener () {}

  virtual void GotCts (double snr, WifiMode txMode)
  {
    m_txop->GotCts (snr, txMode);
  }
  virtual void MissedCts (void)
  {
    m_txop->MissedCts ();
  }
  virtual void GotAck (double snr, WifiMode txMode)
  {
    m_txop->GotAck (snr, txMode);
  }
  virtual void MissedAck (void)
  {
    m_txop->MissedAck ();
  }
  virtual void GotBlockAck (const CtrlBAckResponseHeader *blockAck, Mac48Address source)
  {
    m_txop->GotBlockAck (blockAck, source);
  }
  virtual void MissedBlockAck (void)
  {
    m_txop->MissedBlockAck ();
  }
  virtual void StartNext (void)
  {
    m_txop->StartNext ();
  }
  virtual void Cancel (void)
  {
    m_txop->Cancel ();
  }

private:
  EdcaTxopCfbN *m_txop;
};

class EdcaTxopCfbN::BlockAckEventListener : public MacLowBlockAckEventListener
{
public:
  BlockAckEventListener (EdcaTxopCfbN * txop)
    : MacLowBlockAckEventListener (),
      m_txop (txop) {
  }
  virtual ~BlockAckEventListener () {}

  virtual void BlockAckInactivityTimeout (Mac48Address address, uint8_t tid)
  {
    m_txop->SendDelbaFrame (address, tid, false);
  }

private:
  EdcaTxopCfbN *m_txop;
};

NS_OBJECT_ENSURE_REGISTERED (EdcaTxopCfbN);

TypeId
EdcaTxopCfbN::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EdcaTxopCfbN")
    .SetParent (ns3::Dcf::GetTypeId ())
    .AddConstructor<EdcaTxopCfbN> ()
  ;
  return tid;
}

EdcaTxopCfbN::EdcaTxopCfbN ()
{
  NS_LOG_FUNCTION (this);
  m_cfbStarted = Time (0);
  m_cfbElapsed = Time (0);
  m_currentDuration = Time (0);
  m_defaultAifs = 0;
}

EdcaTxopCfbN::~EdcaTxopCfbN ()
{
  NS_LOG_FUNCTION (this);
}

void
EdcaTxopCfbN::GotCts (double snr, WifiMode txMode)
{
  NS_LOG_FUNCTION (this << snr << txMode);
  NS_LOG_DEBUG ("got cts");
}

void
EdcaTxopCfbN::MissedCts (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("missed cts");
  if (!NeedRtsRetransmission ())
    {
      NS_LOG_DEBUG ("Cts Fail");
      m_stationManager->ReportFinalRtsFailed (m_currentHdr.GetAddr1 (), &m_currentHdr);
      if (!m_txFailedCallback.IsNull ())
        {
          m_txFailedCallback (m_currentHdr);
        }
      // to reset the dcf.
      m_currentPacket = 0;
      m_dcf->ResetCw (); // reset call
    }
  else
    {
      m_dcf->UpdateFailedCw ();
    }
  ResetCfb (); //TODO just back out for now
  RestartAccessIfNeeded ();
}

void
EdcaTxopCfbN::GotAck (double snr, WifiMode txMode)
{
  NS_LOG_FUNCTION (this << snr << txMode);
  if (!NeedFragmentation ()
      || IsLastFragment ()
      || m_currentHdr.IsQosAmsdu ())
    {
      NS_LOG_DEBUG ("got ack. tx done.");
      if (!m_txOkCallback.IsNull ())
        {
          m_txOkCallback (m_currentHdr);
        }

      if (m_currentHdr.IsAction ())
        {
          WifiActionHeader actionHdr;
          Ptr<Packet> p = m_currentPacket->Copy ();
          p->RemoveHeader (actionHdr);
          if (actionHdr.GetCategory () == WifiActionHeader::BLOCK_ACK
              && actionHdr.GetAction ().blockAck == WifiActionHeader::BLOCK_ACK_DELBA)
            {
              MgtDelBaHeader delBa;
              p->PeekHeader (delBa);
              if (delBa.IsByOriginator ())
                {
                  m_baManager->TearDownBlockAck (m_currentHdr.GetAddr1 (), delBa.GetTid ());
                }
              else
                {
                  m_low->DestroyBlockAckAgreement (m_currentHdr.GetAddr1 (), delBa.GetTid ());
                }
            }
        }

      m_dcf->ResetCw ();
      CheckCfb(); // check if we should continue or back off
      m_currentPacket = 0;
      RestartAccessIfNeeded ();
    }
  else
    {
      NS_LOG_DEBUG ("got ack. tx not done, size=" << m_currentPacket->GetSize ());
    }
}

void
EdcaTxopCfbN::MissedAck (void)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("missed ack");
  if (!NeedDataRetransmission ())
    {
      NS_LOG_DEBUG ("Ack Fail");
      m_stationManager->ReportFinalDataFailed (m_currentHdr.GetAddr1 (), &m_currentHdr);
      if (!m_txFailedCallback.IsNull ())
        {
          m_txFailedCallback (m_currentHdr);
        }
      // to reset the dcf.
      m_currentPacket = 0;
      m_dcf->ResetCw ();
    }
  else
    {
      NS_LOG_DEBUG ("Retransmit");
      m_currentHdr.SetRetry ();
      m_dcf->UpdateFailedCw ();
    }
  ResetCfb (); // back out of TXOP
  RestartAccessIfNeeded ();
}

void
EdcaTxopCfbN::NotifyAccessGranted (void)
{
  NS_LOG_FUNCTION (this);
  if (m_currentPacket == 0)
    {
      if (m_queue->IsEmpty () && !m_baManager->HasPackets ())
        {
          NS_LOG_DEBUG ("queue is empty");
          return;
        }
      if (m_baManager->HasBar (m_currentBar))
        {
          SendBlockAckRequest (m_currentBar);
          return;
        }
      /* check if packets need retransmission are stored in BlockAckManager */
      m_currentPacket = m_baManager->GetNextPacket (m_currentHdr);
      if (m_currentPacket == 0)
        {
          if (m_queue->PeekFirstAvailable (&m_currentHdr, m_currentPacketTimestamp, m_qosBlockedDestinations) == 0)
            {
              NS_LOG_DEBUG ("no available packets in the queue");
              return;
            }
          if (m_currentHdr.IsQosData () && !m_currentHdr.GetAddr1 ().IsBroadcast ()
              && m_blockAckThreshold > 0
              && !m_baManager->ExistsAgreement (m_currentHdr.GetAddr1 (), m_currentHdr.GetQosTid ())
              && SetupBlockAckIfNeeded ())
            {
              return;
            }
          m_currentPacket = m_queue->DequeueFirstAvailable (&m_currentHdr, m_currentPacketTimestamp, m_qosBlockedDestinations);
          NS_ASSERT (m_currentPacket != 0);

          uint16_t sequence = m_txMiddle->GetNextSequenceNumberfor (&m_currentHdr);
          m_currentHdr.SetSequenceNumber (sequence);
          m_currentHdr.SetFragmentNumber (0);
          m_currentHdr.SetNoMoreFragments ();
          m_currentHdr.SetNoRetry ();
          m_fragmentNumber = 0;
          NS_LOG_DEBUG ("dequeued size=" << m_currentPacket->GetSize () <<
                        ", to=" << m_currentHdr.GetAddr1 () <<
                        ", seq=" << m_currentHdr.GetSequenceControl ());
          if (m_currentHdr.IsQosData () && !m_currentHdr.GetAddr1 ().IsBroadcast ())
            {
              VerifyBlockAck ();
            }
        }
    }
  MacLowTransmissionParameters params;
  params.DisableOverrideDurationId ();
  if (m_currentHdr.GetAddr1 ().IsGroup ())
    {
      params.DisableRts ();
      params.DisableAck ();
      params.DisableNextData ();
      m_low->StartTransmission (m_currentPacket,
                                &m_currentHdr,
                                params,
                                m_transmissionListener);

      m_currentPacket = 0;
      m_dcf->ResetCw ();
      m_dcf->StartBackoffNow (m_rng->GetNext (0, m_dcf->GetCw ()));
      StartAccessIfNeeded ();
      NS_LOG_DEBUG ("tx broadcast");
    }
  else if (m_currentHdr.GetType () == WIFI_MAC_CTL_BACKREQ)
    {
      SendBlockAckRequest (m_currentBar);
    }
  else
    {
      if (m_currentHdr.IsQosData () && m_currentHdr.IsQosBlockAck ())
        {
          params.DisableAck ();
        }
      else
        {
          params.EnableAck ();
        }
      if (NeedFragmentation () && ((m_currentHdr.IsQosData ()
                                    && !m_currentHdr.IsQosAmsdu ())
                                   || m_currentHdr.IsData ())
          && (m_blockAckThreshold == 0
              || m_blockAckType == BASIC_BLOCK_ACK))
        {
          //With COMPRESSED_BLOCK_ACK fragmentation must be avoided.
          params.DisableRts ();
          WifiMacHeader hdr;
          Ptr<Packet> fragment = GetFragmentPacket (&hdr);
          if (IsLastFragment ())
            {
              NS_LOG_DEBUG ("fragmenting last fragment size=" << fragment->GetSize ());
              params.DisableNextData ();
            }
          else
            {
              NS_LOG_DEBUG ("fragmenting size=" << fragment->GetSize ());
              params.EnableNextData (GetNextFragmentSize ());
            }
          m_low->StartTransmission (fragment, &hdr, params,
                                    m_transmissionListener);
        }
      else
        {
          WifiMacHeader peekedHdr;
          if (m_currentHdr.IsQosData ()
              && m_queue->PeekByTidAndAddress (&peekedHdr, m_currentHdr.GetQosTid (),
                                               WifiMacHeader::ADDR1, m_currentHdr.GetAddr1 ())
              && !m_currentHdr.GetAddr1 ().IsBroadcast ()
              && m_aggregator != 0 && !m_currentHdr.IsRetry ())
            {
              /* here is performed aggregation */
              Ptr<Packet> currentAggregatedPacket = Create<Packet> ();
              m_aggregator->Aggregate (m_currentPacket, currentAggregatedPacket,
                                       MapSrcAddressForAggregation (peekedHdr),
                                       MapDestAddressForAggregation (peekedHdr));
              bool aggregated = false;
              bool isAmsdu = false;
              Ptr<const Packet> peekedPacket = m_queue->PeekByTidAndAddress (&peekedHdr, m_currentHdr.GetQosTid (),
                                                                             WifiMacHeader::ADDR1,
                                                                             m_currentHdr.GetAddr1 ());
              while (peekedPacket != 0)
                {
                  aggregated = m_aggregator->Aggregate (peekedPacket, currentAggregatedPacket,
                                                        MapSrcAddressForAggregation (peekedHdr),
                                                        MapDestAddressForAggregation (peekedHdr));
                  if (aggregated)
                    {
                      isAmsdu = true;
                      m_queue->Remove (peekedPacket);
                    }
                  else
                    {
                      break;
                    }
                  peekedPacket = m_queue->PeekByTidAndAddress (&peekedHdr, m_currentHdr.GetQosTid (),
                                                               WifiMacHeader::ADDR1, m_currentHdr.GetAddr1 ());
                }
              if (isAmsdu)
                {
                  m_currentHdr.SetQosAmsdu ();
                  m_currentHdr.SetAddr3 (m_low->GetBssid ());
                  m_currentPacket = currentAggregatedPacket;
                  currentAggregatedPacket = 0;
                  NS_LOG_DEBUG ("tx unicast A-MSDU");
                }
            }
          if (NeedRts ())
            {
              params.EnableRts ();
              NS_LOG_DEBUG ("tx unicast rts");
            }
          else
            {
              params.DisableRts ();
              NS_LOG_DEBUG ("tx unicast");
            }
          params.DisableNextData ();
          ContinueCfbIfNeeded (params); // only starting or continuing TXOP for unicast
          m_low->StartTransmission (m_currentPacket, &m_currentHdr,
              params, m_transmissionListener);
          CompleteTx ();
        }
    }
}

void
EdcaTxopCfbN::ContinueCfbIfNeeded (MacLowTransmissionParameters &a_params)
{
  // check if current packet is burstable
  if (IsBurstable (m_currentHdr, m_currentPacket))
    {
      // if so
      // check if next packet is burstable
      NS_LOG_DEBUG ("checking queue");
      if (!m_queue->IsEmpty())
        {
          WifiMacHeader hdr;
          MacLowTransmissionParameters params;
          Ptr<const Packet> pkt = GetNextPacket (hdr,params);

          if (IsBurstable (hdr, pkt))
            {
              // if so
              // check if they won't exceed TXOP limit
              m_currentDuration = m_low->CalculateTransmissionTime (m_currentPacket, &m_currentHdr, a_params);
              Time nextDuration = m_low->CalculateTransmissionTime (pkt, &hdr, params);
              Time totalDuration = m_cfbElapsed + m_currentDuration + m_low->GetSifs() + nextDuration;

              if (totalDuration.GetMicroSeconds() < (m_currentHdr.GetQosTxopLimit() * 32)) // TODO - get this in mus
                {
                  // if so
                  // set additional duration of packet to this one plus next one
                  a_params.EnableOverrideDurationId (nextDuration);

                  // set the cfb start time
                  m_cfbStarted = Simulator::Now();

                  NS_LOG_DEBUG (Simulator::Now () << "continuing CFB, this frame will take - " << m_currentDuration);

                  return;
                }
              NS_LOG_DEBUG (Simulator::Now () << "exiting CFB, the total duration would have been - " << totalDuration);
            }
        }
    }
  NS_LOG_DEBUG (Simulator::Now () << "exiting CFB, for a variety of reasons");

  // just in case there are any doubts
  m_currentDuration = Time (0);
  m_cfbElapsed = Time (0);
  m_cfbStarted = Time (0);
}

void
EdcaTxopCfbN::CheckCfb ()
{
  // check if CFB timer is set
  if (!m_cfbStarted.IsZero())
    {
      NS_LOG_DEBUG (Simulator::Now () << "checking CFB on ACK reception, time elapsed - "
          << m_cfbElapsed
          << " time started "
          << m_cfbStarted);

      // if so
      Time elapsed = MicroSeconds (Simulator::Now ().GetMicroSeconds() - m_cfbStarted.GetMicroSeconds());
      m_cfbElapsed += elapsed + m_low->GetSifs();           // set CFB time elapsed

      WifiMacHeader hdr;
      MacLowTransmissionParameters params;
      Ptr<const Packet> pkt = GetNextPacket (hdr,params);

      Time nextDuration = m_low->CalculateTransmissionTime (pkt, &hdr, params);
      Time totalDuration = m_cfbElapsed + nextDuration;

      // TODO - there are lots of ambiguities in the standard on this part
      //        the biggy is just making sure TXOP is not surpassed.
      //        however, there does appear to be some leeway as far as
      //        sending an RTS/CTS to protect second packets that have a lowered PHY rate
      //        at this point. May need to analytically examine the probability of wrap around
      if (totalDuration.GetMicroSeconds() < (m_currentHdr.GetQosTxopLimit() * 32))
        {
          NS_LOG_DEBUG (Simulator::Now () << "continuing SIFS for CFB, time elapsed - " << m_cfbElapsed);
          ContinueCfb ();
          return;
        }
    }

  // default
  ResetCfb ();
}

void
EdcaTxopCfbN::ContinueCfb (void)
{
  m_dcf->SetAifsn (0);                                  // set AIFS to SIFS
}

void
EdcaTxopCfbN::NotifyCollision (void)
{
  NS_LOG_FUNCTION (this);
  ResetCfb ();  // back out
  RestartAccessIfNeeded ();
}

void
EdcaTxopCfbN::DoStart ()
{
  m_dcf->ResetCw ();
  m_dcf->StartBackoffNow (m_rng->GetNext (0, m_dcf->GetCw ()));
  ns3::Dcf::DoStart ();

  m_defaultAifs = this->GetAifsn (); // set the default AIFS
}

Ptr<const Packet>
EdcaTxopCfbN::GetNextPacket (WifiMacHeader &hdr, MacLowTransmissionParameters &params)
{
  Ptr<const Packet> pkt = m_queue->Peek (&hdr);
  params = DetermineParameters (hdr, pkt);
  return pkt;
}

MacLowTransmissionParameters
EdcaTxopCfbN::DetermineParameters (WifiMacHeader hdr, Ptr<const Packet> pkt)
{
  MacLowTransmissionParameters params;
  params.DisableOverrideDurationId ();
  if (hdr.GetAddr1 ().IsGroup ())
    {
      params.DisableRts ();
      params.DisableAck ();
      params.DisableNextData ();
    }
  else
    {
      if (hdr.IsQosData () && hdr.IsQosBlockAck ())
        {
          params.DisableAck ();
        }
      else
        {
          params.EnableAck ();
        }
      if (NeedFragmentation () && ((hdr.IsQosData () && !hdr.IsQosAmsdu ()) || hdr.IsData ())
                && (m_blockAckThreshold == 0 || m_blockAckType == BASIC_BLOCK_ACK))
        {
          params.DisableRts ();
          if (m_stationManager->IsLastFragment (hdr.GetAddr1 (), &hdr,
                pkt, m_fragmentNumber + 1))
            {
              params.DisableNextData ();
            }
          else
            {
              params.EnableNextData (GetNextFragmentSize ());
            }
        }
      else
        {
          if (m_stationManager->NeedRts (hdr.GetAddr1 (), &hdr, pkt))
            {
              params.EnableRts ();
              NS_LOG_DEBUG ("tx unicast rts");
            }
          else
            {
              params.DisableRts ();
              NS_LOG_DEBUG ("tx unicast");
            }
          params.DisableNextData ();
        }
      }

  return params;
}

void
EdcaTxopCfbN::ResetCfb ()
{
  NS_LOG_DEBUG (Simulator::Now () << "resetting CFB, time elapsed - " << m_cfbElapsed);
  m_cfbStarted = Time (0);                                             // set CFB timer to 0
  m_cfbElapsed = Time (0);                                             // set CFB elapsed to 0
  m_currentDuration = Time (0);
  m_dcf->SetAifsn (m_defaultAifs);                                    // set AIFS to Default AIFS
  m_dcf->StartBackoffNow (m_rng->GetNext (0, m_dcf->GetCw ()));       // back off
}

bool
EdcaTxopCfbN::IsBurstable (WifiMacHeader hdr, Ptr<const Packet> pkt)
{
  // very simple criteria - doesn't check for bcast, or mcast, but shouldn't be any
  //    for this research
  return       (hdr.IsQosData()
            &&  hdr.GetQosTxopLimit() != 0
            &&  !m_stationManager->NeedFragmentation (hdr.GetAddr1 (), &hdr, pkt));
}

void
EdcaTxopCfbN::Queue (Ptr<const Packet> packet, const WifiMacHeader &hdr)
{
  NS_LOG_FUNCTION (this << packet << &hdr);
  WifiMacTrailer fcs;
  uint32_t fullPacketSize = hdr.GetSerializedSize () + packet->GetSize () + fcs.GetSerializedSize ();
  m_stationManager->PrepareForQueue (hdr.GetAddr1 (), &hdr,
                                     packet, fullPacketSize);
  m_queue->Enqueue (packet, hdr);
  Simulator::Schedule (Seconds (0.0), &EdcaTxopCfbN::StartAccessIfNeeded, this);
}

} // namespace ns3
