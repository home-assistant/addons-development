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

CMD [ "/run.sh" ]


