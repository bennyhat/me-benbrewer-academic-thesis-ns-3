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
 */
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/dsdv-helper.h"
#include "ns3/netanim-module.h"
#include "ns3/click-internet-stack-helper.h"
#include "ns3/click-node-container.h"
#include <iostream>
#include <cmath>
#include <limits>
#include <map>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ScenarioSpecificInterference");

class ScenarioSpecificInterference
{
public:
  ScenarioSpecificInterference ();
  void CaseRun (uint32_t nWifis,
                uint32_t nSinks,
                uint32_t nSources,
                double totalTime,
                std::string rate,
                double dataStart,
                std::string CSVfileName,
                std::string topoFileName,
                std::string pairFileName,
                std::string startFileName,
                std::string durationFileName,
                std::string animationFileName,
                std::string clickFile
                );

private:
  uint32_t m_nWifis;
  uint32_t m_nSinks;
  uint32_t m_nSources;
  double m_totalTime;
  std::string m_rate;
  double m_dataStart;
  std::string m_CSVfileName;
  std::string m_topoFile;
  std::string m_pairFile;
  std::string m_startFile;
  std::string m_durationFile;
  std::string m_clickFile;

  ClickNodeContainer nodes;
  NetDeviceContainer devices;
  Ipv4InterfaceContainer interfaces;

  std::list <Ptr<StatPacketSink> > m_sinkApps;
  std::list <Ptr<OnOffApplication> > m_srcApps;

private:
  void CreateFile ();
  void CreateNodes ();
  void CreateDevices ();
  void InstallInternetStack ();
  void InstallApplications ();
  void SetupMobility ();
  void CheckThroughput ();
  void SendInterference (void);

};

int
main(int argc, char** argv)
{
  ScenarioSpecificInterference test;
  uint32_t nWifis = 25;
  uint32_t nSinks = 5;
  uint32_t nSources = 5;

  double totalTime = 420.0;
  double dataStart = 120.0;

  uint32_t seedNumber = 1;
  uint32_t runNumber = 1;

  std::string rate ("10Mbps");
  std::string packetSize ("1000");

  std::string appl = "all";
  std::string CSVfileName = "rates.csv";
  std::string topoFileName = "";
  std::string pairFileName = "";
  std::string startFileName = "";
  std::string durationFileName = "";
  std::string animationFileName = "";
  std::string clickFile = "";

  CommandLine cmd;
  cmd.AddValue ("nWifis", "Number of wifi nodes[Default:25]", nWifis);
  cmd.AddValue ("nSinks", "Number of wifi sink nodes[Default:5]", nSinks);
  cmd.AddValue ("nSources", "Number of wifi source nodes[Default:5]", nSources);
  cmd.AddValue ("totalTime", "Total Simulation time[Default:300]", totalTime);
  cmd.AddValue ("rate", "CBR traffic rate[Default:25Mbps]", rate);
  cmd.AddValue ("packetSize", "CBR packet size in bytes[Default:1000]", packetSize);
  cmd.AddValue ("dataStart", "Time at which nodes start to transmit data[Default=120.0]", dataStart);
  cmd.AddValue ("CSVfileName", "The name of the CSV output file name[Default:DsdvManetExample.csv]", CSVfileName);
  cmd.AddValue ("topoFile", "The name of the topology file (simple CSV) to use [Default: none]", topoFileName);
  cmd.AddValue ("pairFile", "The name of the topology file (simple CSV) to use [Default: none]", pairFileName);
  cmd.AddValue ("startFile", "The name of the topology file (simple CSV) to use [Default: none]", startFileName);
  cmd.AddValue ("durationFile", "The name of the topology file (simple CSV) to use [Default: none]", durationFileName);
  cmd.AddValue ("animFile", "The name of the topology file (simple CSV) to use [Default: none]", animationFileName);
  cmd.AddValue ("seed", "The seed number [Default=1]", seedNumber);
  cmd.AddValue ("run", "The run number [Default=1]", runNumber);
  cmd.AddValue ("clickFile", "The click file", clickFile);
  cmd.Parse (argc, argv);

  Config::SetDefault ("ns3::OnOffApplication::PacketSize", StringValue (packetSize));
  Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue (rate));
  Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode", StringValue ("OfdmRate6Mbps"));
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2000"));

  SeedManager::SetSeed (seedNumber);
  SeedManager::SetRun (runNumber);

  test = ScenarioSpecificInterference ();
  test.CaseRun (nWifis, nSinks, nSources, totalTime, rate, dataStart, CSVfileName, topoFileName, pairFileName, startFileName, durationFileName, animationFileName, clickFile);

  return 0;
}

ScenarioSpecificInterference::ScenarioSpecificInterference ()
{
}

void
ScenarioSpecificInterference::CheckThroughput ()
{
  std::ofstream out (m_CSVfileName.c_str (), std::ios::app);
  std::ifstream pairFile ( m_pairFile.c_str() );
  std::string pair;

  for (std::list<Ptr<StatPacketSink> >::iterator i = m_sinkApps.begin (); i != m_sinkApps.end (); i++)
    {
      std::getline ( pairFile, pair, ',' );
      std::getline ( pairFile, pair);
      uint32_t sink = atoi ( pair.c_str());

      std::list<double> rateList = static_cast<Ptr<StatPacketSink> >(*i)->GetRecordedRates();
      std::list<double> jitterList = static_cast<Ptr<StatPacketSink> >(*i)->GetRecordedJitter();
      for (std::list<double>::iterator k = rateList.begin (); k != rateList.end (); k++)
        {
          out << sink << "," << *k << "," << jitterList.front() << "," << static_cast<Ptr<StatPacketSink> >(*i)->GetTotalRx() << "," << m_srcApps.front()->GetTotalBytes() << "\n";
          jitterList.pop_front();
        }
      m_srcApps.pop_front();
    }

  out.close ();
}

void
ScenarioSpecificInterference::CaseRun (uint32_t nWifis, uint32_t nSinks, uint32_t nSources, double totalTime, std::string rate,
                           double dataStart, std::string CSVfileName, std::string topoFileName,
                           std::string pairFileName,std::string startFileName,std::string durationFileName,
                           std::string animationFileName, std::string clickFile)
{
  m_nWifis = nWifis;
  m_nSinks = nSinks;
  m_nSources = nSources;
  m_totalTime = totalTime;
  m_rate = rate;
  m_dataStart = dataStart;
  m_CSVfileName = CSVfileName;
  m_topoFile = topoFileName;
  m_pairFile = pairFileName;
  m_startFile = startFileName;
  m_durationFile = durationFileName;
  m_clickFile = clickFile;

  CreateFile ();
  CreateNodes ();
  CreateDevices ();
  SetupMobility ();
  InstallInternetStack ();
  InstallApplications ();

  std::cout << "\nStarting simulation for " << m_totalTime << " s ...\n";

  Simulator::Schedule (Seconds(m_totalTime),&ScenarioSpecificInterference::CheckThroughput,this);
  Simulator::Schedule (Seconds(200),&ScenarioSpecificInterference::SendInterference,this);

  if (!animationFileName.empty())
    {
      AnimationInterface anim (animationFileName.c_str());
      anim.EnablePacketMetadata(false);
      anim.SetMobilityPollInterval(Seconds(100));
    }

  Simulator::Stop (Seconds (m_totalTime));
  Simulator::Run ();
  Simulator::Destroy ();
}

void
ScenarioSpecificInterference::CreateFile ()
{
  std::ofstream out (m_CSVfileName.c_str ());
  out << "Sink ID," <<
  "ReceiveRate," <<
  "ReceiveDelay," <<
  "BytesReceived," <<
  "BytesSent" <<
  std::endl;
  out.close ();
}

void
ScenarioSpecificInterference::CreateNodes ()
{
  std::cout << "Creating " << (unsigned) m_nWifis << " nodes.\n";
  nodes.Create (m_nWifis);
  NS_ASSERT_MSG (m_nWifis > m_nSinks, "Sinks must be less or equal to the number of nodes in network");
  NS_ASSERT_MSG (m_nSources == m_nSinks, "Sinks must be equal to the number of sources");
}

void
ScenarioSpecificInterference::SetupMobility ()
{
  NS_ASSERT_MSG (!m_topoFile.empty(), "Must provide a topology file");
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> nodePositionList = CreateObject<ListPositionAllocator> ();

  std::ifstream topoFile ( m_topoFile.c_str() );
  std::string value;
  for (uint32_t i=0;i<m_nWifis;i++)
  {
      std::getline ( topoFile, value, ',' );
      double xPos = atof ( value.c_str() );
      std::getline ( topoFile, value, ',' );
      double yPos = atof ( value.c_str() );
      std::getline ( topoFile, value);
      double zPos = atof ( value.c_str() );
      nodePositionList->Add (Vector (xPos, yPos, zPos));
  }
  topoFile.close ();

  mobility.SetPositionAllocator(nodePositionList);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);
}

void
ScenarioSpecificInterference::CreateDevices ()
{
  QosWifiMacHelper wifiMac = QosWifiMacHelper::Default ();
  wifiMac.SetType ("ns3::AdhocTxopWifiMac");

  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss ("ns3::LogDistancePropagationLossModel");
  Ptr<YansWifiChannel> channel = wifiChannel.Create ();
  wifiPhy.SetChannel (channel);
  WifiHelper wifi;
  wifi.SetStandard (WIFI_PHY_STANDARD_80211a);
  wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
  devices = wifi.Install (wifiPhy, wifiMac, nodes);
}

void
ScenarioSpecificInterference::SendInterference (void)
{
  Ptr<WifiNetDevice> dev = devices.Get (5)->GetObject<WifiNetDevice>();
  Ptr<AdhocTxopWifiMac> mac = dev->GetMac()->GetObject<AdhocTxopWifiMac>();
  Ptr<SchedMacLow> macLow = mac->GetMacLow()->GetObject<SchedMacLow>();
  macLow->ReduceSignalStrength(0.0078125);
}

void
ScenarioSpecificInterference::InstallInternetStack ()
{
  NS_ASSERT_MSG (!m_clickFile.empty(), "Must provide a click file");
  ClickInternetStackHelper click;
  click.SetClickFile (nodes, m_clickFile.c_str());
  click.SetRoutingTableElement (nodes, "rt_routingprotocol");
  click.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  interfaces = address.Assign (devices);
}

void
ScenarioSpecificInterference::InstallApplications ()
{
  NS_ASSERT_MSG (!m_pairFile.empty(), "Must provide a pair file");
  NS_ASSERT_MSG (!m_startFile.empty(), "Must provide a start time file");
  NS_ASSERT_MSG (!m_durationFile.empty(), "Must provide a duration time file");

  // read in pair file (uniform)
  std::ifstream pairFile ( m_pairFile.c_str() );
  std::ifstream startFile ( m_startFile.c_str() );
  std::ifstream durationFile ( m_durationFile.c_str() );
  std::string pair;
  std::string tmpTime;

  for (uint32_t i=0;i<m_nSinks;i++)
  {
      // actual pair info
      std::getline ( pairFile, pair, ',' );
      uint32_t source = atoi ( pair.c_str());
      std::getline ( pairFile, pair);
      uint32_t sink = atoi ( pair.c_str());

      std::getline ( startFile, tmpTime);
      double start = atof (tmpTime.c_str());
      std::getline ( durationFile, tmpTime);
      double duration = atof (tmpTime.c_str());

      // setup apps on source
      Ptr<OnOffApplication> onOffApp = CreateObjectWithAttributes<OnOffApplication> (
          "Remote",AddressValue (InetSocketAddress (interfaces.GetAddress (sink), 10)),
          "DataRate", DataRateValue (DataRate (m_rate)),
          "StartTime", TimeValue(Seconds (m_dataStart + start)),
          "StopTime", TimeValue(Seconds (m_dataStart + start + duration)),
          "OnTime",RandomVariableValue(ConstantVariable (duration)),
          "OffTime",RandomVariableValue(ConstantVariable (0))
          );
      nodes.Get (source)->AddApplication (onOffApp);
      m_srcApps.push_back(onOffApp);

      // setup apps on sink
      Ptr<StatPacketSink> packetSinkApp = CreateObjectWithAttributes<StatPacketSink> (
          "Local",AddressValue (InetSocketAddress (interfaces.GetAddress (sink), 10)),
          "Window",TimeValue(Seconds(0.1)),
          "StartTime",TimeValue(Seconds (m_dataStart + start)),
          "StopTime",TimeValue(Seconds(m_dataStart + start + duration))
          );
      nodes.Get (sink)->AddApplication (packetSinkApp);

      m_sinkApps.push_back (packetSinkApp);
  }
  pairFile.close ();
  startFile.close ();
  durationFile.close ();
}

