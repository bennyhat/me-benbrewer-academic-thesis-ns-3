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
 * Description: This really isn't useful either
 */
#include "qos-txop-wifi-mac-helper.h"
#include "ns3/msdu-aggregator.h"
#include "ns3/wifi-mac.h"
#include "ns3/edca-txop-cfb-n.h"
#include "ns3/pointer.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"

namespace ns3 {

QosTxopWifiMacHelper::QosTxopWifiMacHelper ()
{
}

QosTxopWifiMacHelper::~QosTxopWifiMacHelper ()
{
}

QosTxopWifiMacHelper
QosTxopWifiMacHelper::Default (void)
{
  QosTxopWifiMacHelper helper;

  // We're making QoS-enabled Wi-Fi MACs here, so we set the necessary
  // attribute. I've carefully positioned this here so that someone
  // who knows what they're doing can override with explicit
  // attributes.
  helper.SetType ("ns3::StaWifiMac",
                  "QosSupported", BooleanValue (true));

  return helper;
}

void
QosTxopWifiMacHelper::Setup (Ptr<WifiMac> mac, enum AcIndex ac, std::string dcaAttrName) const
{
  std::map<AcIndex, ObjectFactory>::const_iterator it = m_aggregators.find (ac);
  PointerValue ptr;
  mac->GetAttribute (dcaAttrName, ptr);
  Ptr<EdcaTxopCfbN> edca = ptr.Get<EdcaTxopCfbN> ();

  if (it != m_aggregators.end ())
    {
      ObjectFactory factory = it->second;
      Ptr<MsduAggregator> aggregator = factory.Create<MsduAggregator> ();
      edca->SetMsduAggregator (aggregator);
    }
  if (m_bAckThresholds.find (ac) != m_bAckThresholds.end ())
    {
      edca->SetBlockAckThreshold (m_bAckThresholds.find (ac)->second);
    }
  if (m_bAckInactivityTimeouts.find (ac) != m_bAckInactivityTimeouts.end ())
    {
      edca->SetBlockAckInactivityTimeout (m_bAckInactivityTimeouts.find (ac)->second);
    }
}

Ptr<WifiMac>
QosTxopWifiMacHelper::Create (void) const
{
  Ptr<WifiMac> mac = m_mac.Create<WifiMac> ();

  Setup (mac, AC_VO, "VO_EdcaTxopN");
  Setup (mac, AC_VI, "VI_EdcaTxopN");
  Setup (mac, AC_BE, "BE_EdcaTxopN");
  Setup (mac, AC_BK, "BK_EdcaTxopN");

  return mac;
}

} // namespace ns3
