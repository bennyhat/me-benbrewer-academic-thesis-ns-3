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

#ifndef VLAN_HEADER_H
#define VLAN_HEADER_H

#include "ns3/header.h"
#include <string>
#include "ns3/mac48-address.h"

namespace ns3 {

class VlanHeader : public Header
{
public:

  /**
   * \brief Construct a null ethernet header
   * By default, does not add or remove an ethernet preamble
   */
  VlanHeader ();

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual uint32_t GetHeaderSize (void) const;
  virtual void Print (std::ostream &os) const;

  virtual uint8_t GetUserPriority (void);
  virtual uint16_t GetEtherType (void);
  virtual uint16_t GetVlanId (void);
private:
  static const int LENGTH_SIZE = 4;   /// size of the whole vlan part of the header in bytes

  uint8_t m_userPriority; // only 3 bits but it's an eight bit shifted to the right
  uint16_t m_etherType;
  uint16_t m_vlanId;
};

} // namespace ns3


#endif /* VLAN_HEADER_H */
