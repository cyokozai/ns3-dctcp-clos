#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

struct SimulationConfig {
  double durationSec = 5.0;
  std::string linkDataRate = "10Mbps";
  std::string linkDelay = "2ms";
  uint32_t mtuBytes = 1500;
  bool enablePcap = true;
  uint16_t sinkPort = 5000;
  uint32_t sendSize = 1448;
  double senderStartSec = 0.5;
};

int main(int argc, char** argv) {
  CommandLine cmd;
  SimulationConfig cfg;
  cmd.AddValue("duration", "Simulation duration in seconds", cfg.durationSec);
  cmd.AddValue("rate", "Link data rate (e.g., 10Mbps)", cfg.linkDataRate);
  cmd.AddValue("delay", "Link delay (e.g., 2ms)", cfg.linkDelay);
  cmd.AddValue("mtu", "Link MTU in bytes", cfg.mtuBytes);
  cmd.AddValue("pcap", "Enable pcap tracing", cfg.enablePcap);
  cmd.AddValue("sendSize", "TCP app send size in bytes", cfg.sendSize);
  cmd.AddValue("senderStart", "Sender app start time (sec)", cfg.senderStartSec);
  cmd.Parse(argc, argv);

  Time::SetResolution(Time::NS);
  LogComponentEnable("BulkSendApplication", LOG_LEVEL_INFO);
  LogComponentEnable("PacketSink", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create(2);

  PointToPointHelper p2p;
  p2p.SetDeviceAttribute("DataRate", StringValue(cfg.linkDataRate));
  p2p.SetChannelAttribute("Delay", StringValue(cfg.linkDelay));
  p2p.SetDeviceAttribute("Mtu", UintegerValue(cfg.mtuBytes));

  NetDeviceContainer devices = p2p.Install(nodes);

  InternetStackHelper internet;
  internet.Install(nodes);

  Ipv4AddressHelper ipv4;
  ipv4.SetBase("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer ifaces = ipv4.Assign(devices);

  Address sinkAddress(InetSocketAddress(ifaces.GetAddress(1), cfg.sinkPort));

  PacketSinkHelper sinkHelper("ns3::TcpSocketFactory",
                              InetSocketAddress(Ipv4Address::GetAny(), cfg.sinkPort));
  ApplicationContainer sinkApp = sinkHelper.Install(nodes.Get(1));
  sinkApp.Start(Seconds(0.0));
  sinkApp.Stop(Seconds(cfg.durationSec));

  BulkSendHelper bulkSender("ns3::TcpSocketFactory", sinkAddress);
  bulkSender.SetAttribute("MaxBytes", UintegerValue(0));
  bulkSender.SetAttribute("SendSize", UintegerValue(cfg.sendSize));
  ApplicationContainer senderApp = bulkSender.Install(nodes.Get(0));
  senderApp.Start(Seconds(cfg.senderStartSec));
  senderApp.Stop(Seconds(cfg.durationSec - 0.1));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables();

  if (cfg.enablePcap) {
    p2p.EnablePcapAll("tcp-two-nodes");
  }

  Simulator::Stop(Seconds(cfg.durationSec));
  Simulator::Run();

  uint64_t totalRx = DynamicCast<PacketSink>(sinkApp.Get(0))->GetTotalRx();
  std::cout << "Total Bytes Received: " << totalRx << std::endl;

  Simulator::Destroy();
  return 0;
}

