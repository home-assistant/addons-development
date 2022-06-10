ARG BUILD_FROM
FROM $BUILD_FROM AS builder-base

# hadolint ignore=DL3009
RUN \
    set -x \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
       git \
       patch \
       gcc g++ \
       make \
       pkg-config

FROM builder-base AS builder-chip

ARG BUILD_ARCH=amd64
ARG CHIP_VERSION=55ab764beabaed8675bd60e6d18e84d545a733e0

# Set shell
SHELL ["/bin/bash", "-o", "pipefail", "-c"]

COPY 0001-Use-data-as-platform-storage-location.patch /usr/src
COPY 0001-Avoid-python_runner.py-deadlock.patch /usr/src

WORKDIR /usr/src

# Install and build CHIP Python Controller
# hadolint ignore=SC1091
RUN \
    set -x \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
       protobuf-compiler \
       libprotobuf-dev \
       gn \
       libssl-dev \
       libdbus-1-dev \
       libglib2.0-dev \
       ninja-build \
       python3-venv \
       python3-dev \
       python3-pip \
       python3-dbus \
       python3-cryptography \
       unzip \
       libgirepository1.0-dev \
       libcairo2-dev \
    && git clone --depth 1 -b master \
       https://github.com/project-chip/connectedhomeip \
    && cd connectedhomeip \
    && git fetch origin ${CHIP_VERSION} \
    && git checkout ${CHIP_VERSION} \
    && git submodule update --init \
    && patch -p1 < /usr/src/0001-Use-data-as-platform-storage-location.patch \
    && ( \
           cd third_party/pigweed/repo/ \
           && patch -p1 < /usr/src/0001-Avoid-python_runner.py-deadlock.patch \
       ) \
    && source scripts/no_cipd_bootstrap.sh \
    && scripts/build_python.sh -m minimal

FROM builder-base AS builder-matter-server

WORKDIR /usr/src

ARG MATTER_SERVER_VERSION=ac5545bcdfdf02a583ae44ebc0b2ab4c4e3686c5

RUN \
    set -x \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
       python3-venv \
       python3-dev \
       python3-pip \
    && git clone --depth 1 -b main \
       https://github.com/home-assistant-libs/python-matter-server \
    && cd python-matter-server \
    && git fetch origin ${MATTER_SERVER_VERSION} \
    && git checkout ${MATTER_SERVER_VERSION} \
    && pip wheel .

FROM $BUILD_FROM

RUN \
    set -x \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
       libuv1 \
       openssl \
       zlib1g \
       libjson-c5 \
       python3-venv \
       python3-pip \
       python3-gi \
       python3-gi-cairo \
       python3-dbus \
       python3-psutil \
       unzip \
       libcairo2 \
       gdb \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf /usr/src/*

WORKDIR /root

COPY --from=builder-chip /usr/src/connectedhomeip/credentials /root/credentials
COPY --from=builder-chip \
     /usr/src/connectedhomeip/out/python_lib/controller/python/chip-0.0-cp37-abi3-linux_*.whl \
     /tmp
COPY --from=builder-matter-server \
     /usr/src/python-matter-server/python_matter_server-0.0.1-py3-none-any.whl \
     /tmp

# hadolint ignore=DL3013
RUN \
    pip3 install --no-cache-dir /tmp/chip-0.0-cp37-abi3-linux_*.whl \
    && pip3 install --no-cache-dir /tmp/python_matter_server-0.0.1-py3-none-any.whl \
    && rm /tmp/*.whl

COPY rootfs /
