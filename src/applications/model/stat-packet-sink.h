/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2012 Benjamin Brewer
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
 * Author:  Benjamin Brewer <bb1267@my.bristol.ac.uk>
 */

#ifndef STAT_PACKET_SINK_H
#define STAT_PACKET_SINK_H

#include "packet-sink.h"

namespace ns3 {

class StatPacketSink : public PacketSink
{
public:
  static TypeId GetTypeId (void);
  StatPacketSink ();

  virtual ~StatPacketSink ();

  virtual std::list<double> GetRecordedRates (void);
  virtual std::list<double> GetRecordedJitter (void);
protected:
  virtual void RecordRate (void);
  virtual void RecordJitter (Ptr<Packet> packet);
  virtual void StartApplication (void);
  virtual void StopApplication (void);
  virtual void HandleRead (Ptr<Socket> socket);

  Time m_averageWindow;
  Time m_lastPacketTime;
  double m_averageWindowJitter;
  std::list<double> m_recordedRates;
  std::list<double> m_recordedJitter;

  EventId m_nextEvent;
};

} // namespace ns3

#endif /* STAT_PACKET_SINK_H */

