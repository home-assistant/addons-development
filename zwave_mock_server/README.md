## Mock Server for Z-Wave JS

Run a virtual Z-Wave controller from mock files, useful for debugging and development.
This add-on consists of the Z-Wave JS Driver itself (running in mock mode) which exposes a virtual serial port on TCP port 5555. You can connect to this fake Z-Wave stick by connecting to this port from the Z-Wave JS UI or Z-Wave JS Server add-on.

For convenience, this add-on also hosts the Z-Wave JS Server directly, making it a all-in-one package to quickly fire up a mocked Z-Wave network. Simply connect to the Z-Wave JS websocket server fromm the Home Assistant Z-Wave integration.

### How to import mock/dump files ?

The mock/dump files are stored in the `addon_configs` folder.
Use either the Samba add-on, a Fileborwser add-on or the VS Code add-on to explore the files of the addon_configs share and look for a subfolder named zwave_mock_server. Place the dump files in that folder (named .js or .mock). Make sure to restart the Z-Wave JS Mock Server add-on if you placed new files in the folder.

### How to export mock/dump files ?

From within Z-Wave JS, open a Z-Wave node and click the Advanced button.
In the dialog, at 'Export json' click the 'DRIVER' button to make a Node dump in the format defined by the driver.
Rename the downloaded file so it gets the .mock extension.
