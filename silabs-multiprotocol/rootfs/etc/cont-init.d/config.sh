#!/usr/bin/with-contenv bashio
# ==============================================================================
# Generate Silicon Labs Multiprotocol configurations
# ==============================================================================

declare device
declare cpcd_trace

if ! bashio::config.has_value 'device'; then
    bashio::log.fatal "No serial port device set!"
    bashio::exit.nok
fi
device=$(bashio::config 'device')

if  ! bashio::config.has_value 'cpcd_trace'; then
    cpcd_trace="false"
else
    cpcd_trace=$(bashio::config 'cpcd_trace')
fi

bashio::log.info "Generating cpcd configuration."
bashio::var.json \
    device "${device}" \
    cpcd_trace "${cpcd_trace}" \
    | tempio \
        -template /usr/local/share/cpcd.conf \
        -out /usr/local/etc/cpcd.conf
