#!/usr/bin/with-contenv bashio

echo
echo "##################################"
echo "     Z-Wave JS Mock Server"
echo "##################################"
echo

found_file=false
for file in /config/*; do
  # print all the files (not directories) in the addons_config folder
  if [ -f "$file" ]; then
    found_file=true
    echo "Detected mock file: $file"
  fi
done
# if addon_config folder is empty, copy all the default mock files
if ! $found_file; then
    echo
    echo "No mock files found in addons_config directory, using default files..."
    cp /default_mock_files/*.* /config
    echo
fi

# always copy/overwrite default mock files into subfolder for easy access
rm -rf /config/default_mock_files
mv -f /default_mock_files /config

# send discovery info to Home Assistant
echo
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
NODE_PATH="$( npm root --quiet -g )"
export NODE_PATH
bashio::log.info ""
bashio::log.info "Starting Z-Wave JS Mock Driver + Z-Wave Server..."
node /usr/local/bin/mock-server -c /config & \
node /usr/local/bin/zwave-server tcp://127.0.0.1:5555