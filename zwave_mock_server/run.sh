#!/usr/bin/with-contenv bashio

echo
echo "##################################"
echo "     Z-Wave JS Mock Server"
echo "##################################"
echo
echo "Detected mock files in directory:"
echo
ls /config
echo
echo

# send discovery info to Home Assistant
declare ha_config
ha_config=$(\
    bashio::var.json \
        host "$(hostname)" \
        port "^3000" \
)
if bashio::discovery "zwave_js" "${ha_config}" > /dev/null; then
    bashio::log.info "Successfully send discovery information to Home Assistant."
else
    bashio::log.error "Discovery message to Home Assistant failed!"
fi

# start mock driver + zwave-server
bashio::log.info ""
bashio::log.info "Starting Z-Wave JS Mock Driver + Z-Wave Server..."
node /usr/local/bin/mock-server -c /config && \
node /usr/local/bin/zwave-server tcp://127.0.0.1:5555