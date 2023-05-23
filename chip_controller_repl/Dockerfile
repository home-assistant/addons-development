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

WORKDIR /usr/src

FROM builder-base AS builder-python-dbus-wheel

RUN \
    set -x \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
       build-essential \
       python3-dev \
       python3-pip \
       libdbus-1-dev \
       libglib2.0-dev \
    && pip3 wheel dbus-python

FROM builder-base AS builder-ttyd

ARG LIBWEBSOCKETS_VERSION
ARG TTYD_VERSION

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

FROM $BUILD_FROM

ARG HOME_ASSISTANT_CHIP_VERSION

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
       python3-construct \
       unzip \
       libcairo2 \
       libdbus-1-3 \
       libglib2.0-0 \
       gdb \
       git \
    && git clone --depth 1 -b v1.0-branch \
       https://github.com/project-chip/connectedhomeip \
    && cp -r connectedhomeip/credentials /root/credentials \
    && rm -rf connectedhomeip \
    && apt-get purge -y --auto-remove \
       git \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf /usr/src/*

COPY --from=builder-ttyd /usr/src/libwebsockets/build/install /
COPY --from=builder-ttyd /usr/src/ttyd/build/install /

RUN ldconfig

WORKDIR /root

COPY --from=builder-python-dbus-wheel /usr/src/dbus_python-1.2.18-cp39-cp39-linux_*.whl /root

# hadolint ignore=DL3013
RUN \
    pip3 install \
       dbus_python-1.2.18-cp39-cp39-linux_*.whl \
    && pip3 install \
       home-assistant-chip-clusters==${HOME_ASSISTANT_CHIP_VERSION} \
       home-assistant-chip-core==${HOME_ASSISTANT_CHIP_VERSION} \
       home-assistant-chip-repl==${HOME_ASSISTANT_CHIP_VERSION} \
    && pip3 install --no-cache-dir jupyter-console

COPY rootfs /
