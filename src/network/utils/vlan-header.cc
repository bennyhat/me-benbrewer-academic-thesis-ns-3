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
 * Description: Very dirty class that allows for pulling out a VLAN header
 */

#include <iomanip>
#include <iostream>
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/header.h"
#include "address-utils.h"
#include "vlan-header.h"

NS_LOG_COMPONENT_DEFINE ("VlanHeader");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (VlanHeader);

VlanHeader::VlanHeader ()
:  m_userPriority (0),
   m_etherType (0),
   m_vlanId (0)
{
}

uint32_t 
VlanHeader::GetHeaderSize (void) const
{
  return GetSerializedSize ();
}


TypeId 
VlanHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::VlanHeader")
    .SetParent<Header> ()
    .AddConstructor<VlanHeader> ()
  ;
  return tid;
}
TypeId 
VlanHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t 
VlanHeader::GetSerializedSize (void) const
{
  return LENGTH_SIZE;
}

void
VlanHeader::Serialize (Buffer::Iterator start) const
{
  // TODO - this is pretty unusable ATM
}
uint32_t
VlanHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  m_userPriority = i.ReadU8();
  m_userPriority = m_userPriority >> 5; // not bothering with actualy VLAN ID - yep...
  i.Prev(); // step back one byte (UP stepped it one)
  m_vlanId = i.ReadNtohU16();
  m_vlanId = m_vlanId << 3;
  m_vlanId = m_vlanId >> 3;
  m_etherType = i.ReadNtohU16();

  return GetSerializedSize ();
}

uint8_t
VlanHeader::GetUserPriority (void)
{
  return m_userPriority;
}

uint16_t
VlanHeader::GetEtherType (void)
{
  return m_etherType;
}

uint16_t
VlanHeader::GetVlanId (void)
{
  return m_vlanId;
}

void
VlanHeader::Print (std::ostream &os) const
{
  //TODO - required to implement
}

} // namespace ns3
