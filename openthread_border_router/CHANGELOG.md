# Changelog

## 1.1.0

- Automatically flash firmware for Home Assistant SkyConnect and Yellow
- Update serial port defaults to match latest firmware builds

## 1.0.0

- Bump to OTBR POSIX version d83fee189a (2023-02-28 08:48:56 -0800)
- Remove Web UI via ingress (expose ports to use the Web UI, see documentation)
- Change vendor name to "Home Assistant" and product name to Silicon Labs
  Multiprotocol" (used in OTBR mDNS/DNS-SD announcments)
- Set default baudrate 115200 correctly
- Let the OTBR REST API listen on local interface only by default
- Fix REST API to correctly set the Connection HTTP header
- Fix REST API to return an HTTP compliant status line
- Add OTBR discovery support

## 0.3.0

- Bump to OTBR POSIX version 079bbce34a (2022-12-22 19:00:41 -0800)
- Add REST API with full active and pending dataset as well as state support
- Avoid start error in case multiple primary interfaces are returned
- Add fine grained OTBR log level control
- Fix service stop (finish) scripts

## 0.2.6

- Accept IPv6 forwarding explicitly (required for HAOS 9.x)
- Add egress firewall rules for forwarding if firewall is enabled

## 0.2.5

- Bump to OTBR POSIX version 110eb2507c (2022-11-24 14:36:14 -0800)

## 0.2.4

- Bump to OTBR POSIX version 0e15296792 (2022-11-07 12:33:00 +0100)

## 0.2.3

- Fix Firewall shutdown

## 0.2.2

- Bump to OTBR POSIX version 9fea68cfbe (2022-06-03 11:53:19 -0700)
- Use s6-overlay v3 style services

## 0.2.1

- Fix missing common script

## 0.2.0

- Support OpenThread Border Router firewall to avoid unnecessary traffic in the
  OpenThread network.

## 0.1.4

- Enable OpenThread diagnostic mode

## 0.1.3

- Fix startup without hardware flow control

## 0.1.2

- Bump OTBR to ot-br-posix git f8399eb08/openthread git 7dfde1f12

## 0.1.1

- Add baudrate and hardware flow control configurations

## 0.1.0

- initial version
