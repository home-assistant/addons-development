#!/usr/bin/with-contenv bashio
set -e

declare device
declare device_usb_path
declare device_attrs
declare device_busnum
declare device_devnum

device=$(bashio::config 'device')
device_usb_path=$(udevadm info -q path -n "$device")
device_attrs=$(udevadm info -q property -a -p "$device_usb_path" | grep -F 'ATTRS{')
device_busnum=$(echo "$device_attrs" | grep busnum | head -n 1 | awk -F '"' '{ print $2 }')
device_devnum=$(echo "$device_attrs" | grep devnum | head -n 1 | awk -F '"' '{ print $2 }')

if [ -z "$device_busnum" ] || [ -z "$device_devnum" ]; then
	bashio::exit.nok "Could not find USB device or bus numbers for $device"
fi

cp210x-cfg -N "$(bashio::config 'advanced.product')" -C "$(bashio::config 'advanced.manufacturer')" -d "$device_busnum.$device_devnum"

bashio::log.green '-------------------------------------------------------------------'
bashio::log.green 'Programming successful. Unplug your SkyConnect and plug it back in!'
bashio::log.green '-------------------------------------------------------------------'