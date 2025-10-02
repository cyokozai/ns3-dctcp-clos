FROM fedora:latest

ENV NS3_VERSION=ns-3.41

WORKDIR /app
COPY . /app

# パッケージのインストール
RUN dnf update --assumeyes && \
    dnf install --assumeyes \
    gcc-c++ \
    cmake \
    ccache \
    ninja-build \
    git \
    python3 \
    gdb \
    pkgconf \
    gsl-devel \
    sqlite-devel \
    libxml2-devel \
    openssl-devel \
    which \
    make

# NS-3のダウンロードとセットアップ
RUN mkdir -p /opt && cd /opt && \
    git clone --depth 1 --branch ${NS3_VERSION} https://gitlab.com/nsnam/ns-3-dev.git ns-3

# NS-3の設定（メモリ使用量を削減するため最適化レベルを調整）
RUN cd /opt/ns-3 && \
    cmake -S . -B build -G Ninja \
      -DNS3_WARNINGS_AS_ERRORS=OFF \
      -DNS3_EXAMPLES=OFF \
      -DNS3_TESTS=OFF \
      -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DCMAKE_CXX_FLAGS="-O2 -g" \
      -DCMAKE_C_FLAGS="-O2 -g"

# NS-3のビルド（並列度を制限してメモリ不足を防ぐ）
RUN cd /opt/ns-3 && \
    cmake --build build --parallel 2

# NS-3のインストール
RUN cd /opt/ns-3 && \
    cmake --install build

# アプリケーションのビルド
RUN cmake -S /app -B /app/build -G Ninja -DCMAKE_BUILD_TYPE=Release && \
    cmake --build /app/build --parallel 2

ENTRYPOINT ["bash"]