ARG BUILD_FROM=ghcr.io/home-assistant/base:3.24
FROM $BUILD_FROM

# Setup base
RUN apk add --no-cache socat

# Copy data
COPY rootfs /
RUN chmod a+x /run.sh

CMD [ "/run.sh" ]
