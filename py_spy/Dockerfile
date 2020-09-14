ARG BUILD_FROM
FROM $BUILD_FROM

ARG BUILD_ARCH

WORKDIR /usr/src

# Setup py-spy
ARG PY_SPY_VERSION
RUN \
    apk add --no-cache --virtual .build-deps \
        build-base \
        git \
        cargo \
        libunwind-static \
    && git clone --depth 1 -b v${PY_SPY_VERSION} https://github.com/benfred/py-spy \
    && cd py-spy \
    && pip3 install . \
    && apk del .build-deps \
    && rm -rf /usr/src/*

# Copy data
COPY rootfs /

WORKDIR /
