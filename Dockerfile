# simulation/Dockerfile
FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential \
    g++ \
    python3 \
    python3-dev \
    git \
    wget \
    unzip \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /home
RUN wget https://www.nsnam.org/releases/ns-allinone-3.41.tar.bz2 && \
    tar xjf ns-allinone-3.41.tar.bz2 && \
    rm ns-allinone-3.41.tar.bz2 && \
    mkdir ns-allinone-3.41 && \
    mkdir ns-allinone-3.41/ns-3.41 && \
    cd ns-allinone-3.41/ns-3.41 && \
    ./ns3 configure --build-profile=optimized --enable-examples --enable-tests && \
    ./ns3 build

WORKDIR /home/simulation
COPY src/ .

CMD ["/ns-allinone-3.41/ns-3.41/ns3", "run", "clos-dctcp-simulation"]