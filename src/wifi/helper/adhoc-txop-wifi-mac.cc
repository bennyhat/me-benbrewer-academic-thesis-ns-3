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
 * Description: This really serves no purpose. Probably can be removed. Leaving for
 *      example purposes though
 */
#include "adhoc-txop-wifi-mac.h"

#include "ns3/pointer.h"
#include "ns3/log.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/trace-source-accessor.h"

#include "qos-tag.h"
#include "mac-low.h"
#include "dcf-manager.h"
#include "mac-rx-middle.h"
#include "mac-tx-middle.h"
#include "msdu-aggregator.h"
#include "amsdu-subframe-header.h"
#include "mgt-headers.h"
#include "edca-txop-cfb-n.h"

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
AdhocTxopWifiMac::SetupEdcaQueue (enum AcIndex ac)
{
  NS_LOG_FUNCTION (this << ac);

  // Our caller shouldn't be attempting to setup a queue that is
  // already configured.
  NS_ASSERT (m_edca.find (ac) == m_edca.end ());

  Ptr<EdcaTxopCfbN> edca = CreateObject<EdcaTxopCfbN> ();
  edca->SetLow (m_low);
  edca->SetManager (m_dcfManager);
  edca->SetTxMiddle (m_txMiddle);
  edca->SetTxOkCallback (MakeCallback (&AdhocTxopWifiMac::TxOk, this));
  edca->SetTxFailedCallback (MakeCallback (&AdhocTxopWifiMac::TxFailed, this));
  edca->SetAccessCategory (ac);
  edca->CompleteConfig ();
  m_edca.insert (std::make_pair (ac, edca));
}

} // namespace ns3
