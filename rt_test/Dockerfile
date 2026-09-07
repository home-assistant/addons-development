ARG BUILD_FROM=ghcr.io/home-assistant/base:3.24
FROM $BUILD_FROM AS builder

# Setup base

WORKDIR /usr/src

RUN \
    set -x \ 
    && apk add --no-cache \
        bash-completion \
        pulseaudio-utils \
        alsa-plugins-pulse \
        bluez \
        git \
        libuv \
        mosquitto-clients \
        nano \
        openssh \
        pwgen \
        tmux \
        vim \
    \
    && apk add --no-cache --virtual .build-dependencies \
        bsd-compat-headers \
        build-base \
        linux-headers \
        cmake \
        json-c-dev \
        libuv-dev \
        openssl-dev \
        zlib-dev \
        paho-mqtt-c-dev

# Build libwebsockets

ARG LIBWEBSOCKETS_VERSION=4.3.5

RUN \
    set -x \
    && git clone --branch "v${LIBWEBSOCKETS_VERSION}" --depth=1 \
        https://github.com/warmcat/libwebsockets.git \
    \
    && cd libwebsockets \
    && mkdir -p build \
    && cd build \
    && cmake .. \
        -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
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

# Build ttyd

ARG TTYD_VERSION=1.7.3

RUN \
    set -x \
    && git clone --branch main --single-branch \
        https://github.com/tsl0922/ttyd \
    && cd ttyd \
    && git checkout "${TTYD_VERSION}" \
    \
    && mkdir -p build \
    && cd build \
    && cmake .. \
        -DCMAKE_BUILD_TYPE=MinSizeRel \
        -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
        -DCMAKE_INSTALL_PREFIX=/usr/local \
        -DCMAKE_VERBOSE_MAKEFILE=TRUE \
    && make \
    && make DESTDIR=./install install

# Build mqtt-rt-test

COPY build /usr/src/mqtt-rt-test/

RUN \
    set -x \
    && cd /usr/src/mqtt-rt-test \
    && mkdir -p build \
    && cd build \
    && cmake .. \
    && make

# Combine binaries for the app image

FROM $BUILD_FROM

RUN \
    set -x \ 
    && apk add --no-cache \
        bash-completion \
        libuv \
        mosquitto-clients \
        tmux \
        vim \
        json-c \
        paho-mqtt-c \
    \
    && sed -i "s/ash/bash/" /etc/passwd

COPY --from=builder /usr/src/libwebsockets/build/install /
COPY --from=builder /usr/src/ttyd/build/install /
COPY --from=builder /usr/src/mqtt-rt-test/build/mqtt-rt-test /usr/local/bin/

RUN ldconfig /

WORKDIR /root

# Copy data
COPY rootfs /
