# Home Assistant Add-on: SkyConnect CP2102N Programmer

## How to use
Point the add-on to the serial port of your defective SkyConnect. It will look like this:

```
/dev/serial/by-id/usb-Silicon_Labs_CP2102N_USB_to_UART_Bridge_Controller_c8bfd7a0a664a0d5c2b36b54b2d15a9e-if00-port0
```

Run the add-on, wait for it to complete programming (it will automatically stop). Then unplug the SkyConnect and plug it in again, it will now show up properly.