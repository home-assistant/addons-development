# Home Assistant Add-on: OpenThread Border Router

## Installation

Follow these steps to get the add-on installed on your system:

1. Navigate in your Home Assistant frontend to **Settings** -> **Add-ons, Backup & Supervisor** -> **Add-on Store**.
2. Click on the top right menu and "Repository"
3. Add "https://github.com/home-assistant/addons-development" to add the "Home Assistant Add-on Repository for Development" repository.
4. Find the "OpenThread Border Router" add-on and click it.
5. Click on the "INSTALL" button.

## How to use

You will need a 802.15.4 capable radio supported by OpenThread. The website
[openthread.io maintains a list of supported platforms][openthread-platforms].
One of the cheapest but still well documented Radios for development is the
Nordic Semiconductor [nRF52840 Dongle][nordic-nrf52840-dongle]. The Dongle needs
a recent version of the OpenThread OCP firmare.
[This article][nordic-nrf52840-dongle-install] outlines the steps to install the
RCP firmware for the nRF52840 Dongle.

The add-on also has successfully tested with Silicon Labs `ot-rcp` firmware.
For Yellow, you can find this firmware at [github.com/NabuCasa/silabs-firmware](https://github.com/NabuCasa/silabs-firmware).

Once the firmware is loaded follow the following steps:

1. Select the correct `device` in the add-on configuration tab and press `Save`.
2. Start the add-on.

### OpenThread Border Router

This add-on makes your Home Assistant installation an OpenThread BorderRouter
(OTBR). There is no integration in Home Assistant Core at this point.

The add-on comes with the OTBR web fronted provided by the OpenThread reference
implementation.

Follow the guides at [openthread.io](https://openthread.io), e.g. the [OpenThread
Commissioner](https://openthread.io/guides/commissioner) guide.

## Configuration

Add-on configuration:

| Configuration      | Description                                            |
|--------------------|--------------------------------------------------------|
| device (mandatory) | Serial port where the OpenThread RCP Radio is attached |
| otbr_debug         | Start OpenThread BorderRouter Agent with debug log     |

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
[issue]: https://github.com/home-assistant/addons-development/issues
[openthread-platforms]: https://openthread.io/platforms
[nordic-nrf52840-dongle]: https://www.nordicsemi.com/Products/Development-hardware/nrf52840-dongle
[nordic-nrf52840-dongle-install]: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/matter/openthread_rcp_nrf_dongle.html

