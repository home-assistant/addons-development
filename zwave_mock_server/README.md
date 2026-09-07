## Mock Server for Z-Wave JS

Run a virtual Z-Wave controller from mock files, useful for debugging and development.
This app consists of the Z-Wave JS Driver itself (running in mock mode) which exposes a virtual serial port on TCP port 5555. You can connect to this fake Z-Wave stick by connecting to this port from the Z-Wave JS UI or Z-Wave JS Server app.

For convenience, this app also hosts the Z-Wave JS Server directly, making it an all-in-one package to quickly fire up a mocked Z-Wave network. Simply connect to the Z-Wave JS websocket server on port 3000 from the Home Assistant Z-Wave integration.

### Connect Home Assistant to the Mock Server

Within Home Assistant, add a new Integration., click Z-Wave.
Do not check the 'use default supervisor app' checkbox, which allows you to manually enter the URL.

Use this url: `http://77f1785d-zwave-mock-server:3000`

NOTE: If you want to connect from your development machine to the mock server running on another machine/instance, you need to open the port in the app settings for the mock server app and then you can connect to port 3000 of the host running the mock app.

### How to import mock/dump files ?

The mock/dump files are stored in the `addon_configs` folder.
Use either the Samba app, a Filebrowser app or the VS Code app to explore the files of the addon_configs share and look for a subfolder named zwave_mock_server. Place the dump files in that folder (named .js or .dump). Make sure to restart the Z-Wave JS Mock Server app if you placed new files in the folder.

### How to export mock/dump files ?

From within Z-Wave JS, open a Z-Wave node and click the Advanced button.
In the dialog, at 'Export json' click the 'DRIVER' button to make a Node dump in the format defined by the driver.
Rename the downloaded file so it gets the .dump extension.
