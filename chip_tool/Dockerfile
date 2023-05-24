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

FROM builder-base AS builder-ttyd

ARG LIBWEBSOCKETS_VERSION
ARG TTYD_VERSION

WORKDIR /usr/src

RUN \
    set -x \
    && apt-get install -y --no-install-recommends \
       cmake \
       libuv1-dev \
       libc-dev \
       libssl-dev \
       zlib1g-dev \
       libjson-c-dev \
    && git clone --branch "v${LIBWEBSOCKETS_VERSION}" --depth=1 \
        https://github.com/warmcat/libwebsockets \
    && cd libwebsockets \
    && mkdir -p build \
    && cd build \
    && cmake .. \
        -DCMAKE_BUILD_TYPE=MinSizeRel \
        -DCMAKE_INSTALL_PREFIX=/usr/local \
        -DCMAKE_VERBOSE_MAKEFILE=TRUE \
        -DLWS_IPV6=ON \
        -DLWS_STATIC_PIC=ON \
        -DLWS_UNIX_SOCK=ON \
        -DLWS_WITH_LIBUV=ON \
        -DLWS_WITH_SHARED=ON \
        -DLWS_WITHOUT_TESTAPPS=ON \
    && make \
    && make install \
    && make DESTDIR=./install install

RUN \
    cd /usr/src \
    && git clone --branch main --single-branch \
        https://github.com/tsl0922/ttyd \
    && cd ttyd \
    && git checkout "${TTYD_VERSION}" \
    && mkdir -p build \
    && cd build \
    && cmake .. \
        -DCMAKE_BUILD_TYPE=MinSizeRel \
        -DCMAKE_INSTALL_PREFIX=/usr/local \
        -DCMAKE_VERBOSE_MAKEFILE=TRUE \
    && make \
    && make DESTDIR=./install install

# Use amd64 stage to generate ZAP files (zap-cli is only available for amd64)
FROM --platform=linux/amd64 debian:bullseye AS builder-amd64-base

ARG CHIP_VERSION

# Set shell
SHELL ["/bin/bash", "-o", "pipefail", "-c"]

WORKDIR /usr/src

RUN \
    set -x \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
       git \
       ca-certificates \
       python3 \
    && git clone --depth 1 --no-single-branch -b master \
       https://github.com/project-chip/connectedhomeip \
    && cd connectedhomeip \
    && git fetch origin ${CHIP_VERSION} \
    && git checkout ${CHIP_VERSION} \
    && scripts/checkout_submodules.py --shallow --platform linux

RUN \
    set -x \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
       git \
       ca-certificates \
       python3 \
       python3-venv \
       python3-dev \
       python3-click \
       python3-coloredlogs \
       python3-pip \
       libgirepository1.0-dev \
       libuv1-dev \
       libprotobuf-dev \
       libdbus-1-dev \
       libglib2.0-dev \
       libreadline-dev \
    && cd connectedhomeip \
    && ./scripts/build/gn_bootstrap.sh \
    && ./scripts/run_in_build_env.sh "./scripts/codepregen.py ./zzz_pregenerated"


FROM builder-base AS builder-chip

ARG BUILD_ARCH

# Set shell
SHELL ["/bin/bash", "-o", "pipefail", "-c"]

WORKDIR /usr/src

COPY 0001-Use-data-as-platform-storage-location.patch /usr/src
COPY 0002-Use-data-as-storage-location-for-chip-tool.patch /usr/src

# Copy sources with pregenerated ZAP code
COPY --from=builder-amd64-base /usr/src/connectedhomeip /usr/src/connectedhomeip

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
       libreadline-dev \
       libgirepository1.0-dev \
       ninja-build \
       python3-venv \
       python3-dev \
       python3-pip \
       python3-dbus \
       python3-click \
       python3-coloredlogs \
       python3-cryptography \
       unzip \
       ccache \
    && pwd && ls -la \
    && cd connectedhomeip \
    && patch -p1 < /usr/src/0001-Use-data-as-platform-storage-location.patch \
    && patch -p1 < /usr/src/0002-Use-data-as-storage-location-for-chip-tool.patch \
    && source scripts/bootstrap.sh \
    && if [ "${BUILD_ARCH}" = "amd64" ]; \
       then \
           EXAMPLE_PREFIX="linux-x64-"; \
       elif [ "${BUILD_ARCH}" = "aarch64" ]; \
       then \
           EXAMPLE_PREFIX="linux-arm64-"; \
       else \
           exit 1; \
       fi \
    && scripts/build/build_examples.py --target ${EXAMPLE_PREFIX}chip-tool \
                                       --pregen-dir ./zzz_pregenerated \
                                       --pw-command-launcher=ccache \
                                       build \
    && scripts/build/build_examples.py --target ${EXAMPLE_PREFIX}chip-cert \
                                       --pregen-dir ./zzz_pregenerated \
                                       --pw-command-launcher=ccache \
                                       build \
    && scripts/build/build_examples.py --target ${EXAMPLE_PREFIX}all-clusters-no-ble-no-wifi \
                                       --pregen-dir ./zzz_pregenerated \
                                       --pw-command-launcher=ccache \
                                       build \
    && scripts/build/build_examples.py --target ${EXAMPLE_PREFIX}bridge \
                                       --pregen-dir ./zzz_pregenerated \
                                       --pw-command-launcher=ccache \
                                       build \
    && scripts/build/build_examples.py --target ${EXAMPLE_PREFIX}light \
                                       --pregen-dir ./zzz_pregenerated \
                                       --pw-command-launcher=ccache \
                                       build \
    && scripts/build/build_examples.py --target ${EXAMPLE_PREFIX}lock \
                                       --pregen-dir ./zzz_pregenerated \
                                       --pw-command-launcher=ccache \
                                       build \
    && scripts/build/build_examples.py --target ${EXAMPLE_PREFIX}thermostat \
                                       --pregen-dir ./zzz_pregenerated \
                                       --pw-command-launcher=ccache \
                                       build \
    && scripts/build/build_examples.py --target ${EXAMPLE_PREFIX}tv-app \
                                       --pregen-dir ./zzz_pregenerated \
                                       --pw-command-launcher=ccache \
                                       build \
    && scripts/build/build_examples.py --target ${EXAMPLE_PREFIX}tv-casting-app \
                                       --pregen-dir ./zzz_pregenerated \
                                       --pw-command-launcher=ccache \
                                       build

FROM $BUILD_FROM

RUN \
    set -x \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
       libuv1 \
       openssl \
       zlib1g \
       libjson-c5 \
       libdbus-1-3 \
       libglib2.0 \
       tmux \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf /usr/src/*

COPY --from=builder-ttyd /usr/src/libwebsockets/build/install /
COPY --from=builder-ttyd /usr/src/ttyd/build/install /

RUN ldconfig

WORKDIR /root

COPY --from=builder-chip /usr/src/connectedhomeip/credentials /root/credentials
COPY --from=builder-chip /usr/src/connectedhomeip/out/linux-*-chip-tool/chip-tool /usr/local/bin
COPY --from=builder-chip /usr/src/connectedhomeip/out/linux-*-chip-cert/chip-cert /usr/local/bin
COPY --from=builder-chip /usr/src/connectedhomeip/out/linux-*-all-clusters-no-ble-no-wifi/address-resolve-tool /usr/local/bin
COPY --from=builder-chip /usr/src/connectedhomeip/out/linux-*-all-clusters-no-ble-no-wifi/chip-all-clusters-app /usr/local/bin
COPY --from=builder-chip /usr/src/connectedhomeip/out/linux-*-bridge/chip-bridge-app /usr/local/bin
COPY --from=builder-chip /usr/src/connectedhomeip/out/linux-*-light/chip-lighting-app /usr/local/bin
COPY --from=builder-chip /usr/src/connectedhomeip/out/linux-*-lock/chip-lock-app /usr/local/bin
COPY --from=builder-chip /usr/src/connectedhomeip/out/linux-*-thermostat/thermostat-app /usr/local/bin
COPY --from=builder-chip /usr/src/connectedhomeip/out/linux-*-tv-app/chip-tv-app /usr/local/bin
COPY --from=builder-chip /usr/src/connectedhomeip/out/linux-*-tv-casting-app/chip-tv-casting-app /usr/local/bin

COPY rootfs /
