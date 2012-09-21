/*
 * exponential-generator.cc
 *
 *  Created on: 29 Aug 2012
 *      Author: ben
 */
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/random-variable.h"
#include <iostream>
#include <cmath>

using namespace ns3;

int
main(int argc, char** argv)
{
  double mean = 25.0;
  double upper = 0.0;
  std::string outFile = "duration.time";
  uint32_t runs = 100;
  uint32_t pairs = 10;

  CommandLine cmd;
  cmd.AddValue ("mean", "Mean number", mean);
  cmd.AddValue ("upper", "Upper limit", upper);
  cmd.AddValue ("runs", "Number of runs", runs);
  cmd.AddValue ("pairs", "Number of pairs", pairs);
  cmd.AddValue ("outFile", "Output file name", outFile);
  cmd.Parse (argc, argv);

  SeedManager::SetSeed (1);

  for (uint32_t i = 1; i <= runs; i++)
    {
      SeedManager::SetRun (i);
      ExponentialVariable distribution;
      if (upper != 0.0)
        {
          distribution = ExponentialVariable (mean,upper);
        }
      else
        {
          distribution = ExponentialVariable (mean);
        }
      std::ofstream out ((outFile + "-" + static_cast<ostringstream*>( &(ostringstream() << i))->str()
          + "-" + static_cast<ostringstream*>( &(ostringstream() << pairs))->str() + ".time").c_str (), std::ios::app);
      for (uint32_t j=0;j < pairs;j++)
        {
          out << distribution.GetValue() << "\n";
        }
      out.close ();
    }

  return 0;
}



