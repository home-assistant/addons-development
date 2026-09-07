ARG BUILD_FROM=ghcr.io/home-assistant/base:3.24
FROM $BUILD_FROM

ARG ZWAVEJS_SERVER_VERSION=3.1.0
ARG ZWAVEJS_VERSION=15.3.0

# install nodejs + yarn
RUN set -x \
    && apk add --no-cache \
    nodejs \
    npm \
    yarn

# install zwave-js
RUN set -x \
    && npm install -g \
    "zwave-js@${ZWAVEJS_VERSION}" \
    "@zwave-js/server@${ZWAVEJS_SERVER_VERSION}" \
    "sinon@17.0.1"

COPY rootfs /
RUN chmod a+x /run.sh

# Symlink @zwave-js/* packages and other packages,
# which zwave-js server depends on without specifying it in package.json
RUN NODE_PATH="$( npm root --quiet -g )" \
    && export NODE_PATH \
    && cd "$NODE_PATH/zwave-js/node_modules/@zwave-js" \
    && for dir in * ; do ln -s "${PWD}/$dir" "$NODE_PATH/@zwave-js/$dir" ; done \
    && ln -s "$NODE_PATH/zwave-js/node_modules/winston-transport" "$NODE_PATH/winston-transport" \
    && ln -s "$NODE_PATH/zwave-js/node_modules/triple-beam" "$NODE_PATH/triple-beam" \
    && ln -s "$NODE_PATH/zwave-js/node_modules/alcalzone-shared" "$NODE_PATH/alcalzone-shared"

CMD [ "/run.sh" ]
