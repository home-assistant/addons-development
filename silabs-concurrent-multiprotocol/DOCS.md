# Home Assistant App: Silicon Labs Concurrent Multiprotocol

## Installation

Follow these steps to get the app installed on your system:

1. Navigate in your Home Assistant frontend to **Settings** -> **Apps** -> **Install app**.
2. Find the "Silicon Labs Concurrent Multiprotocol" app and click it.
3. Click on the "INSTALL" button.

## How to use

The app needs a Silicon Labs based wireless module accessible through a
serial port (like the module on Yellow or most USB based wireless adapters).

**NOTE:** The Silicon Labs Multiprotocol app needs the Zigbee NCP and
OpenThread RCP firmware loaded on the Silicon Labs wireless controller. For
Yellow or SkyConnect, you can find this firmware at [github.com/NabuCasa/silabs-firmware](https://github.com/NabuCasa/silabs-firmware).

Once the firmware is loaded follow the following steps:

1. Select the correct `device` in the app configuration tab and press `Save`.
2. Start the app.

**NOTE:** the Web frontend is only accessible when OpenThread is enabled (see below).

### Zigbee

To use Zigbee with ZHA configure the Integration as follows:

1. Remember/copy the hostname of the app (e.g. `c8f00288-silabs-concurrent-multiprotocol`).
2. Add the Zigbee Home Automation (ZHA) integration to Home Assitant Core
3. When asked for the Serial Device Path, choose `Enter Manually`.
4. Choose `EZSP` as Radio type.
5. As serial path, enter `socket://<hostname-from-above>:9999`.
6. Port speed and flow control don't matter.
7. Press `Submit`. Adding ZHA should succeed and you should be able to use ZHA
   as if using any other supported radio type.

### OpenThread

At this point OpenThread support is experimental. This app makes your Home
Assistant installation an OpenThread BorderRouter (OTBR). There is no integration
in Home Assistant Core at this point.

To use the OTBR enable it in the Configuration tab and restart the app. With
that you will be able to access the OTBR web frontend. When joining or forming a
network make sure to use the same IEEE 802.15.4 channel for both OpenThread and
Zigbee, and *different* PAN IDs. This limitations stem from the fact that a single
radio is used.

Follow the guides at [openthread.io](https://openthread.io), e.g. the [OpenThread
Commissioner](https://openthread.io/guides/commissioner) guide.

## Configuration

App configuration:

| Configuration      | Description                                            |
|--------------------|--------------------------------------------------------|
| device (mandatory) | Serial sevice where the Silicon Labs radio is attached |
| baudrate           | Serial port baudrate (depends on firmware)   |
| flow_control       | If hardware flow control should be enabled (depends on firmware) |
| network_device     | Host and port where CPC daemon can find the Silicon Labs radio (takes precedence over device) |
| cpcd_trace         | Co-Processsor Communication tracing (trace in log)     |
| otbr_enable        | Enable OpenThread BorderRouter                         |
| otbr_debug         | Start OpenThread BorderRouter Agent with debug log     |

## Known issues and limitations

- ezspd is sometimes unable to connect to cpcd, leading to errors like:
  Connected to CPC daemon, endpoint 5: ERROR (errno 11)
  Workaround: Restart the app

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
