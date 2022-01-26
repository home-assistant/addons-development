# Home Assistant Add-on: Silicon Labs Multiprotocol

## Installation

Follow these steps to get the add-on installed on your system:

1. Navigate in your Home Assistant frontend to **Settings** -> **Add-ons, Backup & Supervisor** -> **Add-on Store**.
2. Find the "Silicon Labs Multiprotocol" add-on and click it.
3. Click on the "INSTALL" button.

## How to use

The add-on needs a Silicon Labs based wireless module accessible through a
serial port (like the module on Yellow or most USB based wireless adapters).

**NOTE:** The Silicon Labs Multiprotocol add-on needs the RCP Multi-PAN firmware
loaded on the Silicon Labs wireless controller. For Yellow, you can find this
firmware at [github.com/NabuCasa/silabs-firmware](https://github.com/NabuCasa/silabs-firmware).

Once the firmware is loaded follow the following staps:

1. Select the correct `device` in the add-on configuration tab and press `Save`.
2. Start the add-on.
3. Remember/copy the hostname of the add-on (e.g. `69c153b9-silabs-multiprotocol`).
4. Add the Zigbee Home Automation (ZHA) integration to Home Assitant Core
5. When asked for the Serial Device Path, choose `Enter Manually`.
6. Choose `EZSP` as Radio type.
7. As serial path, enter `socket://<hostname-from-above>:9999`.
8. Choose 115200 as port speed.
9. Press `Submit`. Adding ZHA should succeed and you should be able to use ZHA
   as if using any other supported radio type.

## Configuration

Add-on configuration:

```yaml
device: /dev/ttyUSB0
cpcd_trace: false
socat_trace: false
```

## Known issues and limitations

- No aarch64 support at this time
- Some devices seem to fail to join the network

## Support

Got questions?

You have several options to get them answered:

- The [Home Assistant Discord Chat Server][discord].
- The Home Assistant [Community Forum][forum].
- Join the [Reddit subreddit][reddit] in [/r/homeassistant][reddit]

In case you've found a bug, please [open an issue on our GitHub][issue].

[discord]: https://discord.gg/c5DvZ4e
[forum]: https://community.home-assistant.io
[reddit]: https://reddit.com/r/homeassistant
[issue]: https://github.com/home-assistant/addons/issues
