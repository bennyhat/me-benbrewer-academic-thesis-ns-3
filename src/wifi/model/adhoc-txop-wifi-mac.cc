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
 */
#include "adhoc-txop-wifi-mac.h"

#include "ns3/pointer.h"
#include "ns3/log.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/llc-snap-header.h"
#include "ns3/vlan-header.h"

#include "qos-tag.h"
#include "mac-low.h"
#include "dcf-manager.h"
#include "mac-rx-middle.h"
#include "mac-tx-middle.h"
#include "msdu-aggregator.h"
#include "amsdu-subframe-header.h"
#include "mgt-headers.h"
#include "edca-txop-cfb-n.h"
#include "regular-wifi-mac.h"

NS_LOG_COMPONENT_DEFINE ("AdhocTxopWifiMac");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (AdhocTxopWifiMac);

TypeId
AdhocTxopWifiMac::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::AdhocTxopWifiMac")
    .SetParent<AdhocWifiMac> ()
    .AddConstructor<AdhocTxopWifiMac> ()
  ;
  return tid;
}

AdhocTxopWifiMac::AdhocTxopWifiMac ()
{
  NS_LOG_FUNCTION (this);

  // Let the lower layers know that we are acting in an IBSS
  SetTypeOfStation (ADHOC_STA);
}

AdhocTxopWifiMac::~AdhocTxopWifiMac ()
{
  NS_LOG_FUNCTION (this);
}

void
AdhocTxopWifiMac::Enqueue (Ptr<const Packet> packet, Mac48Address to)
{
  NS_LOG_FUNCTION (this << packet << to);
  if (m_stationManager->IsBrandNew (to))
    {
      // In ad hoc mode, we assume that every destination supports all
      // the rates we support.
      for (uint32_t i = 0; i < m_phy->GetNModes (); i++)
        {
          m_stationManager->AddSupportedMode (to, m_phy->GetMode (i));
        }
      m_stationManager->RecordDisassociated (to);
    }

  WifiMacHeader hdr;

  // If we are not a QoS STA then we definitely want to use AC_BE to
  // transmit the packet. A TID of zero will map to AC_BE (through \c
  // QosUtilsMapTidToAc()), so we use that as our default here.
  uint8_t tid = 0;

  // For now, a STA that supports QoS does not support non-QoS
  // associations, and vice versa. In future the STA model should fall
  // back to non-QoS if talking to a peer that is also non-QoS. At
  // that point there will need to be per-station QoS state maintained
  // by the association state machine, and consulted here.
  if (m_qosSupported)
    {
      hdr.SetType (WIFI_MAC_QOSDATA);
      hdr.SetQosAckPolicy (WifiMacHeader::NORMAL_ACK);
      hdr.SetQosNoEosp ();
      hdr.SetQosNoAmsdu ();

      // Fill in the QoS control field in the MAC header
      tid = QosUtilsGetTidForPacket (packet);

      // all tags above 7 are invalid
      if (tid >= 8)
        {
          tid = 0;
        }
      hdr.SetQosTid (tid);

      // TXOP sets for OFDM PHY
      if (tid == 6 || tid == 7)
        {
          hdr.SetQosTxopLimit (47); // by 32 us
        }
      else if (tid == 4 || tid == 5)
        {
          hdr.SetQosTxopLimit (94); // by 32 us
        }
      else
        {
          hdr.SetQosTxopLimit (0);
        }
    }
  else
    {
      hdr.SetTypeData ();
    }

  hdr.SetAddr1 (to);
  hdr.SetAddr2 (m_low->GetAddress ());
  hdr.SetAddr3 (GetBssid ());
  hdr.SetDsNotFrom ();
  hdr.SetDsNotTo ();

  if (m_qosSupported)
    {
      // Sanity check that the TID is valid
      NS_ASSERT (tid < 8);
      m_edca[QosUtilsMapTidToAc (tid)]->Queue (packet, hdr);
    }
  else
    {
      m_dca->Queue (packet, hdr);
    }
}
// added for debugging at some point
Ptr<MacLow>
AdhocTxopWifiMac::GetMacLow (void)
{
  return m_low;
}

} // namespace ns3
