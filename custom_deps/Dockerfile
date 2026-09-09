ARG BUILD_FROM=ghcr.io/home-assistant/base-python:3.14-alpine3.24
FROM $BUILD_FROM

# Setup base
RUN apk add --no-cache \
    gcc \
    g++ \
    make \
    cmake \
    git

# Copy data
COPY rootfs /
RUN chmod a+x /run.sh

CMD [ "/run.sh" ]
