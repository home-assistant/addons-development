# Home Assistant Add-on: Matter Server (beta)

## Installation

Use the following steps to install this add-on.

1. Click the Home Assistant My button below to open the add-on page on your
   Home Assistant instance.

   [![Open this add-on in your Home Assistant instance.][addon-badge]][addon]


1. If it's the first time you install an Add-on from this repository, Home
   Assitant will ask you to add the 'addons-development' repository. Click
   "Add" to add the repository.
1. Click the "Install" button to install the add-on.

## How to use

Note: This add-on conflicts with the regular Matter Server add-on! Make sure
to stop the Matter Server (non-beta) add-on before starting this add-on!

Start the Matter Server add-on to make the WebSocket available to Home
Assistant Core. Install the [Matter integration][matter_integration]
in Home Assistant Core.


## Configuration

Add-on configuration:

| Configuration      | Description                                                 |
|--------------------|-------------------------------------------------------------|
| log_level          | Logging level of the Matter Server component.               |

## Support

Got questions?

You have several options to get them answered:

- The [Home Assistant Discord Chat Server][discord].
- The Home Assistant [Community Forum][forum].
- Join the [Reddit subreddit][reddit] in [/r/homeassistant][reddit]

In case you've found a bug, please [open an issue on our GitHub][issue].

[addon]: https://my.home-assistant.io/redirect/supervisor_addon/?addon=core_matter_server
[addon-badge]: https://my.home-assistant.io/badges/supervisor_addon.svg
[discord]: https://discord.gg/c5DvZ4e
[forum]: https://community.home-assistant.io
[reddit]: https://reddit.com/r/homeassistant
[issue]: https://github.com/home-assistant/addons/issues
[matter_server_repo]: https://github.com/home-assistant-libs/python-matter-server
[matter_integration]: https://www.home-assistant.io/integrations/matter/
