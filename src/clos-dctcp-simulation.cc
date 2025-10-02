// ... (ヘッダファイルのインクルードなど)

// ノードにインストールするTCPをDCTCPに設定
Config::SetDefault("ns3::TcpL4Protocol::SocketType", StringValue("ns3::TcpDctcp"));

// ...

// PointToPointHelperを使用してリンクを作成
PointToPointHelper p2p;
p2p.SetDeviceAttribute("DataRate", StringValue("10Gbps"));
p2p.SetChannelAttribute("Delay", StringValue("10us"));

// キューにREDとECNを設定
// Thresholds in bytes for ECN marking
p2p.SetQueue("ns3::RedQueue",
             "Mode", StringValue("QUEUE_MODE_BYTES"),
             "MinTh", UintegerValue(10000), // 10KB
             "MaxTh", UintegerValue(10000), // 閾値を超えたら即マーキング
             "LinkBandwidth", StringValue("10Gbps"),
             "LinkDelay", StringValue("10us"),
             "EcnMode", StringValue("QUEUE_DISC_ECN")); // ECNを有効化

// ... (トポロジ構築とデバイスのインストール)