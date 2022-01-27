ARG BUILD_FROM
FROM $BUILD_FROM

ARG BUILD_ARCH

ARG CPCD_VERSION=Alpha-4.0.0.0
ARG GECKO_SDK_VERSION=v4.0.0
ARG SYSTEM_ARCH=arm32v7

# Set shell
SHELL ["/bin/bash", "-o", "pipefail", "-c"]

WORKDIR /usr/src
RUN \
    set -x \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
       build-essential \
       cmake \
       git \
       libmbedtls-dev \
       libmbedtls12 \
       socat \
    && git clone --depth 1 -b ${CPCD_VERSION} \
       https://github.com/SiliconLabs/cpc-daemon.git \
    && mkdir cpc-daemon/build && cd cpc-daemon/build \
    && cmake ../ \
    && make \
    && make install \
    && cd /usr/src \
    && git clone --depth 1 -b ${GECKO_SDK_VERSION} \
       https://github.com/SiliconLabs/gecko_sdk.git \
    && cd gecko_sdk \
    && mkdir -p /usr/local/bin/ && mkdir -p /usr/local/etc/ \
    &&  if [ "${BUILD_ARCH}" = "armv7" ]; \
        then \
            cp "app/host/multiprotocol/zigbeed/multiprotocol-container/_artifacts/bin_arm32v7/zigbeed" /usr/local/bin/; \
        elif [ "${BUILD_ARCH}" = "aarch64" ]; \
        then \
            cp "app/host/multiprotocol/zigbeed/multiprotocol-container/_artifacts/bin_arm64v8/zigbeed" /usr/local/bin/; \
        else \
            exit 1; \
        fi \
    && chmod +x /usr/local/bin/zigbeed \
    && cp "app/host/multiprotocol/zigbeed/multiprotocol-container/_artifacts/etc/zigbeed.conf" /usr/local/etc/ \
    && apt-get purge -y --auto-remove \
       build-essential \
       git \
       libmbedtls-dev \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf /usr/src/*

RUN touch /accept_silabs_msla

COPY rootfs /
