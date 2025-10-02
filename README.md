## ns-3 を Docker で動かして簡単な TCP 実験をする

### 概要

Docker 上で ns-3 (CMake 版) をビルド・インストールし、IP Clos トポロジを用いた実験環境を実行します。

```planetext
ns3-dctcp-clos/
├── compose.yaml
├── simulation/
│   ├── Dockerfile
│   └── src/
│       └── clos-dctcp-simulation.cc  # ns-3シミュレーションスクリプト
└── plotting/
    ├── Dockerfile
    ├── requirements.txt
    └── src/
        └── plot-results.py           # Pythonプロットスクリプト
```


