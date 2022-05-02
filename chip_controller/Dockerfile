ARG BUILD_FROM
FROM $BUILD_FROM AS builder

ARG LIBWEBSOCKETS_VERSION
ARG TTYD_VERSION

COPY cmake-remove-duplicate-LwsCheckRequirements.patch /usr/src

WORKDIR /usr/src

RUN \
    set -x \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
       git \
       patch \
       gcc g++ \
       make \
       cmake \
       libuv1-dev \
       libc-dev \
       libssl-dev \
       zlib1g-dev \
       libjson-c-dev \
    && git clone --branch "v${LIBWEBSOCKETS_VERSION}" --depth=1 \
        https://github.com/warmcat/libwebsockets \
    && cd libwebsockets \
    && patch -p 1 < /usr/src/cmake-remove-duplicate-LwsCheckRequirements.patch \
    && mkdir -p build \
    && cd build \
    && cmake .. \
        -DCMAKE_BUILD_TYPE=MinSizeRel \
        -DCMAKE_INSTALL_PREFIX=/usr/local \
        -DCMAKE_VERBOSE_MAKEFILE=TRUE \
        -DLWS_IPV6=ON \
        -DLWS_STATIC_PIC=ON \
        -DLWS_UNIX_SOCK=OFF \
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

FROM $BUILD_FROM
ARG BUILD_ARCH

ARG CHIP_VERSION=5df932897db4658504309446322ad09cbf7aaf2c

# Set shell
SHELL ["/bin/bash", "-o", "pipefail", "-c"]

COPY 0001-linux-Use-data-as-default-storage-location.patch /usr/src

WORKDIR /usr/src

# Install and build CHIP Python Controller
RUN \
    set -x \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
       git \
       gcc g++ \
       pkg-config \
       libssl-dev \
       libdbus-1-dev \
       libglib2.0-dev \
       libavahi-client-dev \
       ninja-build \
       python3-venv \
       python3-dev \
       python3-pip \
       unzip \
       libgirepository1.0-dev \
       libcairo2-dev \
       libuv1 \
       openssl \
       zlib1g \
       libjson-c5 \
    && git clone --depth 1 -b master \
       https://github.com/project-chip/connectedhomeip \
    && cd connectedhomeip \
    && git fetch origin ${CHIP_VERSION} \
    && git checkout ${CHIP_VERSION} \
    && git submodule update --init \
    && patch -p1 < /usr/src/0001-linux-Use-data-as-default-storage-location.patch \
    && source scripts/activate.sh \
    && scripts/build_python.sh -m minimal -i separate
# && apt-get purge -y --auto-remove \
#       git \
#       python3-dev \
#    && rm -rf /var/lib/apt/lists/* \
#    && rm -rf /usr/src/*

COPY --from=builder /usr/src/libwebsockets/build/install /
COPY --from=builder /usr/src/ttyd/build/install /

COPY rootfs /
