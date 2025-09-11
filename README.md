## ns-3 を Docker で動かして簡単な TCP 実験をする

### 概要

Docker 上で ns-3 (CMake 版) をビルド・インストールし、2 ノードの PointToPoint を用いた最小の TCP 実験を C++ で実行します。

### 事前準備

- Docker がインストールされていること

### ビルド

```bash
docker build -t ns3-sim:latest .
```

### 実行（サンプル）

インタラクティブに入って実行:

```bash
docker run --rm -it ns3-sim:latest
# /app にプロジェクトが配置済み。必要ならリビルド
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release && cmake --build build
./build/sim --duration=5 --rate=10Mbps --delay=2ms --mtu=1500 --pcap=1
```

1 コマンドで実行（非対話）:

```bash
docker run --rm ns3-sim:latest bash -lc "./build/sim --duration=5 --rate=10Mbps --delay=2ms --mtu=1500 --pcap=1 | cat"
```

### 出力

- 標準出力: 受信総バイト数 (Total Bytes Received)
- `pcap=1` の場合: `/app` 直下に `tcp-two-nodes-*.pcap` が生成され、Wireshark 等で解析可能

### コード概要

- `src/main.cpp`: 2 ノード (送信/受信) を P2P で接続。`BulkSendApplication` → `PacketSink` で TCP スループットを観測。
- `CMakeLists.txt`: ns-3 のモジュールを `pkg-config` でリンク
- `Dockerfile`: Fedora ベースで ns-3 ソースをクローンし、CMake でビルド・インストール後に本プロジェクトをビルド

### 実験パラメータ

- `--duration`: シミュレーション時間（秒）
- `--rate`: P2P リンク帯域（例: `10Mbps`）
- `--delay`: 片道遅延（例: `2ms`）
- `--mtu`: MTU（bytes）
- `--pcap`: pcap トレースの有効/無効（1/0）

### よくある質問

- 異なる TCP バリアントで試すには？
  - 例: CUBIC を用いる場合
    ```bash
    ./build/sim --duration=5 --rate=10Mbps --delay=2ms --mtu=1500 --pcap=1 --cmd='Config::SetDefault("ns3::TcpL4Protocol::SocketType", StringValue("ns3::TcpCubic"));'
    ```
  - もしくは `src/main.cpp` 内で `Config::SetDefault("ns3::TcpL4Protocol::SocketType", StringValue("ns3::TcpCubic"));` を追加してください。
