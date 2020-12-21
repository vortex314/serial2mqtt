# Configuring serial2mqtt
Most of the serial2mqtt options can be set from its configuration file. The configuration is in JSON format, and unless specified, located in the current directory named `serial2mqtt.json`. The whole file is a JSON object, built from the following groups:
* `serial` for serial port related parameters,
* `mqtt` for MQTT related parameters, and
* `programmer`

## The `serial` group
The serial group describes how to access the serial port, and how to communicate with the MCU over it. These are the supported options:
* `ports` array describing the serial ports, default value is `/dev/ttyUSB0`. See *Serial port device format* for details.
* `baudrate` serial baud rate, default value is `115200`.
* `protocol` specifies how to communicate with the MCU, valid values are `jsonObject` (default) and `jsonArray`.
* `reconnectInterval` is the time interval between loosing the serial connection and the reconnection starts. It is specified in milliseconds, the default value is `5000`. You can disable serial reconnection with `0` value, in this case serial2mqtt will exit upon serial disconnection.
* `idleTimeout` is the maximal time interval without communication. Upon exceeding this limit, the serial connection will be closed and reopened. It is specified in milliseconds, the default value is `5000`. You can disable this functionality with `0` (can be useful during development and testing, but not recommented in production environment).

### Serial port device format
Serial ports can be local (traditional serial ports, USB dongles, bluetooth etc. accessible by a character device) or remote. Remote serial ports are serial ports on a remote machine, terminal server etc. shared over the network, e.g. this setup [when the MCU is directly connected to an OpenWRT router](https://github.com/vortex314/serial2mqtt/issues/8).

If the serial port name begins with `tcp://`, it will be treated as a tcp address. The following address formats are supported:
* `tcp://127.0.0.1:1111` (using IPv4 addresses)
* `tcp://[::1]:1111` (using IPv6 addresses)
* `tcp://localhost:1111` (using domain names)

Using the domain name format, the code tries every address record returned by the resolver.

Other names will be treated as a regular serial port specified by its character device.
