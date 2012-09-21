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
#ifndef QOS_TXOP_WIFI_MAC_HELPER_H
#define QOS_TXOP_WIFI_MAC_HELPER_H

#include "qos-wifi-mac-helper.h"

namespace ns3 {

/**
 * \brief create QoS-enabled MAC layers for a ns3::WifiNetDevice.
 *
 * This class can create MACs of type ns3::ApWifiMac, ns3::StaWifiMac,
 * and, ns3::AdhocWifiMac, with QosSupported attribute set to True.
 */
class QosTxopWifiMacHelper : public QosWifiMacHelper
{
public:
  /**
   * Create a QosWifiMacHelper that is used to make life easier when working
   * with Wifi devices using a QOS MAC layer.
   */
  QosTxopWifiMacHelper ();

  /**
   * \internal
   * Destroy a QosWifiMacHelper
   */
  virtual ~QosTxopWifiMacHelper ();

  /**
   * Create a mac helper in a default working state.
   */
  static QosTxopWifiMacHelper Default (void);

protected:
  virtual Ptr<WifiMac> Create (void) const;
  void Setup (Ptr<WifiMac> mac, enum AcIndex ac, std::string dcaAttrName) const;
};

} // namespace ns3

#endif /* QOS_TXOP_WIFI_MAC_HELPER_H */
