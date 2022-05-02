ARG BUILD_FROM
FROM $BUILD_FROM

ARG BUILD_ARCH

ARG CHIP_VERSION=5df932897db4658504309446322ad09cbf7aaf2c

# Set shell
SHELL ["/bin/bash", "-o", "pipefail", "-c"]

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
    && git clone --depth 1 -b master \
       https://github.com/project-chip/connectedhomeip \
    && cd connectedhomeip \
    && git fetch origin ${CHIP_VERSION} \
    && git checkout ${CHIP_VERSION} \
    && git submodule update --init \
    && source scripts/activate.sh \
    && scripts/build_python.sh -m minimal -i separate
# && apt-get purge -y --auto-remove \
#       git \
#       python3-dev \
#    && rm -rf /var/lib/apt/lists/* \
#    && rm -rf /usr/src/*

COPY rootfs /
