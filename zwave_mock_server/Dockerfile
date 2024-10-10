ARG BUILD_FROM
FROM $BUILD_FROM

ARG ZWAVEJS_SERVER_VERSION
ARG ZWAVEJS_VERSION

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

COPY run.sh /
RUN chmod a+x /run.sh

# include default mock files
COPY default_mock_files /default_mock_files

# Symlink @zwave-js/* packages and other packages,
# which zwave-js server depends on without specifying it in package.json
# hadolint disable=SC2155
RUN export NODE_PATH="$( npm root -g )" \
    && cd "$NODE_PATH/zwave-js/node_modules/@zwave-js" \
    && for dir in * ; do ln -s "${PWD}/$dir" "$NODE_PATH/@zwave-js/$dir" ; done \
    && ln -s "$NODE_PATH/zwave-js/node_modules/winston-transport" "$NODE_PATH/winston-transport" \
    && ln -s "$NODE_PATH/zwave-js/node_modules/triple-beam" "$NODE_PATH/triple-beam" \
    && ln -s "$NODE_PATH/zwave-js/node_modules/alcalzone-shared" "$NODE_PATH/alcalzone-shared"

CMD [ "/run.sh" ]


