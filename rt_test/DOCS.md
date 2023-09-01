# Home Assistant Add-on: Real-Time Latency test

## How to use

Install Mosquitto or another MQTT broker. Then use an automation in Home
Assistant Core which sends back a message to the test utility.

The following automation can be used:

```yaml
alias: MQTT RT ping
description: "MQTT Real-Time latency test ping responder"
mode: single
trigger:
  - platform: state
    entity_id:
      - sensor.rt_test
    id: trigger
condition: []
action:
  - service: mqtt.publish
    data:
      qos: "0"
      retain: false
      topic: homeassistant/sensor/rt-test/response
      payload_template: "{{ trigger.to_state.state }}"
```

Then open the ingress Web-UI of the add-on and launch `mqtt-rt-test`:
```
Usage: mqtt-rt-test SERVER_URI USERNAME PASSWORD
```

Press Ctrl+C after a while to get a summary of the latencies measured.

Note: The add-on runs with real-time priority (FIFO scheduler, priority 90).
To change priority of other components of Home Assistant `chrt -f -p 90 <pid>`
from the OS shell can be used.

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

