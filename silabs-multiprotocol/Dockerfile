ARG BUILD_FROM
FROM $BUILD_FROM

ARG BUILD_ARCH

ARG CPCD_VERSION=alpha/4.0.1.0
ARG GECKO_SDK_VERSION=v4.0.1
ARG SYSTEM_ARCH=arm32v7

# Set shell
SHELL ["/bin/bash", "-o", "pipefail", "-c"]

ENV BORDER_ROUTING=1
ENV BACKBONE_ROUTER=1
ENV OTBR_OPTIONS "-DOTBR_DBUS=OFF -DOT_MULTIPAN_RCP=ON -DOT_POSIX_CONFIG_RCP_BUS=CPC -DOT_LINK_RAW=1 -DOTBR_VENDOR_NAME=HomeAssistant -DOTBR_PRODUCT_NAME=OpenThreadBorderRouter"
ENV WEB_GUI=1
ENV DOCKER 1

COPY 0001-Avoid-writing-to-system-console.patch /usr/src
COPY 0001-web-set-text-html-Content-Type-for-html-files.patch /usr/src
COPY 0001-web-Use-relative-paths-in-xhr-requests.patch /usr/src

# Required and installed during build (script/bootstrap), could be removed
ENV OTBR_BUILD_DEPS build-essential ninja-build cmake wget ca-certificates \
  libreadline-dev libncurses-dev libcpputest-dev libdbus-1-dev libavahi-common-dev \
  libavahi-client-dev libboost-dev libboost-filesystem-dev libboost-system-dev \
  libnetfilter-queue-dev

WORKDIR /usr/src
RUN \
    set -x \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
       build-essential \
       cmake \
       git \
       git-lfs \
       libmbedtls-dev \
       libmbedtls12 \
       socat \
       iproute2 \
       lsb-release \
       sudo \
    && git clone --depth 1 -b ${CPCD_VERSION} \
       https://github.com/SiliconLabs/cpc-daemon.git \
    && mkdir cpc-daemon/build && cd cpc-daemon/build \
    && cmake ../ \
    && make \
    && make install \
    && cd /usr/src \
    && GIT_LFS_SKIP_SMUDGE=1 git clone --depth 1 -b ${GECKO_SDK_VERSION} \
       https://github.com/SiliconLabs/gecko_sdk.git \
    && cd gecko_sdk \
    && git lfs pull --include="*.png" \
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
    # Install npm/nodejs for WebUI manually to avoid systemd getting pulled in \
    && apt-get install -y --no-install-recommends \
       nodejs \
       npm \
    && cd /usr/src \
    && cd gecko_sdk/util/third_party/ot-br-posix \
    && chmod +x ./script/bootstrap \
    && ./script/bootstrap \
    && ln -s ../../../openthread/ third_party/openthread/repo \
    && chmod +x ./script/* \
    && patch -p1 < /usr/src/0001-Avoid-writing-to-system-console.patch \
    && patch -p1 < /usr/src/0001-web-set-text-html-Content-Type-for-html-files.patch \
    && patch -p1 < /usr/src/0001-web-Use-relative-paths-in-xhr-requests.patch \
    && ./script/setup \
    && apt-get purge -y --auto-remove \
       build-essential \
       git \
       git-lfs \
       libmbedtls-dev \
       ${OTBR_BUILD_DEPS} \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf /usr/src/*

RUN touch /accept_silabs_msla

COPY rootfs /
