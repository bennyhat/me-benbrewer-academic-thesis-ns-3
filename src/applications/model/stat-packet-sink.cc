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
 * Author:  Benjamin Brewer <bb127@my.bristol.ac.uk>
 */
#include "ns3/address.h"
#include "ns3/address-utils.h"
#include "ns3/log.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/node.h"
#include "ns3/socket.h"
#include "ns3/udp-socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/udp-socket-factory.h"
#include "stat-packet-sink.h"

using namespace std;

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("StatPacketSink");
NS_OBJECT_ENSURE_REGISTERED (StatPacketSink);

TypeId 
StatPacketSink::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::StatPacketSink")
    .SetParent<PacketSink> ()
    .AddConstructor<StatPacketSink> ()
    .AddAttribute ("Window", "The time window over which to average received rate",
                   TimeValue (),
                   MakeTimeAccessor (&StatPacketSink::m_averageWindow),
                   MakeTimeChecker ())
  ;
  return tid;
}

StatPacketSink::StatPacketSink () :
    m_averageWindow (-1.0),
    m_averageWindowJitter(0.0)
{
  NS_LOG_FUNCTION (this);
}

StatPacketSink::~StatPacketSink()
{
  NS_LOG_FUNCTION (this);
}

void StatPacketSink::StartApplication ()
{
  NS_LOG_FUNCTION (this);
  // Create the socket if not already
  if (!m_socket)
    {
      m_socket = Socket::CreateSocket (GetNode (), m_tid);
      m_socket->Bind (m_local);
      m_socket->Listen ();
      m_socket->ShutdownSend ();
      if (addressUtils::IsMulticast (m_local))
        {
          Ptr<UdpSocket> udpSocket = DynamicCast<UdpSocket> (m_socket);
          if (udpSocket)
            {
              // equivalent to setsockopt (MCAST_JOIN_GROUP)
              udpSocket->MulticastJoinGroup (0, m_local);
            }
          else
            {
              NS_FATAL_ERROR ("Error: joining multicast on a non-UDP socket");
            }
        }
    }

  m_socket->SetRecvCallback (MakeCallback (&StatPacketSink::HandleRead, this));
  m_socket->SetAcceptCallback (
    MakeNullCallback<bool, Ptr<Socket>, const Address &> (),
    MakeCallback (&StatPacketSink::HandleAccept, this));
  m_socket->SetCloseCallbacks (
    MakeCallback (&StatPacketSink::HandlePeerClose, this),
    MakeCallback (&StatPacketSink::HandlePeerError, this));

  // schedule the window averaging to occur
  if (!m_averageWindow.IsNegative())
    m_nextEvent = Simulator::Schedule (m_averageWindow,&StatPacketSink::RecordRate,this);
}

void
StatPacketSink::StopApplication ()     // Called at time specified by Stop
{
  NS_LOG_FUNCTION (this);
  Simulator::Cancel(m_nextEvent); // cancel the next average window
  while(!m_socketList.empty ()) //these are accepted sockets, close them
    {
      Ptr<Socket> acceptedSocket = m_socketList.front ();
      m_socketList.pop_front ();
      acceptedSocket->Close ();
    }
  if (m_socket)
    {
      m_socket->Close ();
      m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
    }
}

void
StatPacketSink::HandleRead (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);
  Ptr<Packet> packet;
  Address from;
  while ((packet = socket->RecvFrom (from)))
    {
      if (packet->GetSize () == 0)
        { //EOF
          break;
        }
      m_totalRx += packet->GetSize ();
      RecordJitter (packet);  // record the jitter
      if (InetSocketAddress::IsMatchingType (from))
        {
          NS_LOG_INFO ("At time " << Simulator::Now ().GetSeconds ()
                       << "s packet sink received "
                       <<  packet->GetSize () << " bytes from "
                       << InetSocketAddress::ConvertFrom(from).GetIpv4 ()
                       << " port " << InetSocketAddress::ConvertFrom (from).GetPort ()
                       << " total Rx " << m_totalRx << " bytes");
        }
      else if (Inet6SocketAddress::IsMatchingType (from))
        {
          NS_LOG_INFO ("At time " << Simulator::Now ().GetSeconds ()
                       << "s packet sink received "
                       <<  packet->GetSize () << " bytes from "
                       << Inet6SocketAddress::ConvertFrom(from).GetIpv6 ()
                       << " port " << Inet6SocketAddress::ConvertFrom (from).GetPort ()
                       << " total Rx " << m_totalRx << " bytes");
        }
      m_rxTrace (packet, from);
    }
}

// records the rate taken as total received in the window * 8 over the window
void
StatPacketSink::RecordRate (void)
{
  m_recordedRates.push_back (((double) m_totalRx * 8)/m_averageWindow.GetSeconds());
  m_totalRx = 0; // reset the total received

  m_recordedJitter.push_back (m_averageWindowJitter);
  m_averageWindowJitter = 0.0;  // reset jitter average

  // reschedule
  if (!m_averageWindow.IsNegative())
    m_nextEvent = Simulator::Schedule (m_averageWindow,&StatPacketSink::RecordRate,this);
}
// record the jitter as this packet time - the last and then average it in
void
StatPacketSink::RecordJitter (Ptr<Packet> packet)
{
  if (m_averageWindowJitter == 0.0)
    {
      m_averageWindowJitter = (Simulator::Now () - m_lastPacketTime).GetMicroSeconds ();
    }
  else
    {
      // .5 .5 2 sample average - might be more accurate if it were not recursive
      m_averageWindowJitter = 0.5 * (m_averageWindowJitter
          + (Simulator::Now () - m_lastPacketTime).GetMicroSeconds ());
    }
  m_lastPacketTime = Simulator::Now ();
}
std::list<double>
StatPacketSink::GetRecordedRates (void)
{
  return m_recordedRates;
}
std::list<double>
StatPacketSink::GetRecordedJitter (void)
{
  return m_recordedJitter;
}

} // Namespace ns3
