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
#ifndef SCHED_MAC_LOW_H
#define SCHED_MAC_LOW_H

#include "mac-low.h"

namespace ns3 {

class SchedMacLow : public MacLow
{
public:
  SchedMacLow ();
  virtual ~SchedMacLow ();

  virtual void ReceiveOk (Ptr<Packet> packet, double rxSnr, WifiMode txMode, WifiPreamble preamble);
  virtual void ReduceSignalStrength (double howMuch);
protected:
  double m_signalReduction;
};

} // namespace ns3

#endif /* SCHED_MAC_LOW_H */
