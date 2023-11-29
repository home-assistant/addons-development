ARG BUILD_FROM
ARG CP210X_CFG_COMMIT

FROM $BUILD_FROM AS builder

# hadolint ignore=SC2086
RUN \
    set -x \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
        git \
        build-essential \
        libusb-1.0-0-dev \
    && cd /usr/src \
    && git clone --depth=1 -b master https://github.com/irrwisch1/cp210x-cfg \
    && cd cp210x-cfg \
    && git fetch origin ${CP210X_CFG_COMMIT} \
    && git checkout ${CP210X_CFG_COMMIT} \
    && make \
    && chmod 755 ./cp210x-cfg

FROM $BUILD_FROM
COPY rootfs /
COPY --from=builder /usr/src/cp210x-cfg/cp210x-cfg /usr/bin/cp210x-cfg
RUN \
    set -x \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
        libusb-1.0-0 \
        udev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /
ENTRYPOINT ["/init"]