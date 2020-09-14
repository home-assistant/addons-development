ARG BUILD_FROM
FROM $BUILD_FROM

ARG BUILD_ARCH

# Setup py-spy
ARG PY_SPY_VERSION
RUN \
    apk add --no-cache --virtual .build-deps \
        cargo \
        libunwind-dev \
        libunwind-static \
    && cargo install py-spy v${PY_SPY_VERSION} \
    && apk del .build-deps

# Copy data
COPY rootfs /
