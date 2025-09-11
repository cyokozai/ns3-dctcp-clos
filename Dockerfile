FROM fedora:latest

ENV NS3_VERSION=ns-3.41

WORKDIR /app
COPY . /app

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
    make && \
    mkdir -p /opt && cd /opt && \
    git clone --depth 1 --branch ${NS3_VERSION} https://gitlab.com/nsnam/ns-3-dev.git ns-3 && \
    cmake -S /opt/ns-3 -B /opt/ns-3/build -G Ninja \
      -DNS3_WARNINGS_AS_ERRORS=OFF \
      -DNS3_EXAMPLES=OFF \
      -DNS3_TESTS=OFF \
      -DCMAKE_BUILD_TYPE=Release && \
    cmake --build /opt/ns-3/build && \
    cmake --install /opt/ns-3/build && \
    cmake -S /app -B /app/build -G Ninja -DCMAKE_BUILD_TYPE=Release && \
    cmake --build /app/build

ENTRYPOINT ["bash"]