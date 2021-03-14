# Configuring serial2mqtt
Most of the serial2mqtt options can be set from its configuration file. The configuration is in JSON format, and unless specified, located in the current directory named `serial2mqtt.json`. The whole file is a JSON object, built from the following groups:
* `serial` for serial port related parameters,
* `mqtt` for MQTT related parameters,
* `log` for logging parameters, and
* `programmer`

## The `serial` group
The serial group describes how to access the serial port, and how to communicate with the MCU over it. These are the supported options:
* `ports` array describing the serial ports, default value is `/dev/ttyUSB0`. See *Serial port device format* for details.
* `baudrate` serial baud rate, default value is `115200`.
* `protocol` specifies how to communicate with the MCU, valid values are `jsonObject` (default) and `jsonArray`.
* `reconnectInterval` is the time interval between loosing the serial connection and the reconnection starts. It is specified in milliseconds, the default value is `5000`. You can disable serial reconnection with `0` value, in this case serial2mqtt will exit upon serial disconnection.
* `idleTimeout` is the maximal time interval without communication. Upon exceeding this limit, the serial connection will be closed and reopened. It is specified in milliseconds, the default value is `5000`. You can disable this functionality with `0` (can be useful during development and testing, but not recommented in production environment).
* `silentInterval` specifies the interval right after opening the serial device when all traffic will be discared. Its purpose is to let the bootloader run or avoid corrupted data appearing using USB ports. It is specified in milliseconds, the default value is `0` which disables this functionality.

### Serial port device format
Serial ports can be local (traditional serial ports, USB dongles, bluetooth etc. accessible by a character device) or remote. Remote serial ports are serial ports on a remote machine, terminal server etc. shared over the network, e.g. this setup [when the MCU is directly connected to an OpenWRT router](https://github.com/vortex314/serial2mqtt/issues/8).

If the serial port name begins with `tcp://`, it will be treated as a tcp address. The following address formats are supported:
* `tcp://127.0.0.1:1111` (using IPv4 addresses)
* `tcp://[::1]:1111` (using IPv6 addresses)
* `tcp://localhost:1111` (using domain names)

Using the domain name format, the code tries every address record returned by the resolver.

Other names will be treated as a regular serial port specified by its character device.

## The `mqtt` group
The mqtt group configures how to connect to the MQTT broker, and the extra functionality provided by serial2mqtt. These are the supported options:
* `connection` holds the MQTT broker address in URI format, default value is `tcp://test.mosquitto.org:1883`.
* `keepAliveInterval` is the MQTT keepalive interval parameter in seconds, default value is `5`.
* `willTopic` is the MQTT topic where the LWT message will be sent. Default value is `src/<hostname>.<serialPortShort>/serial2mqtt/alive`.
* `willMessage` is the message body of the LWT message sent to `willTopic`. Default value is `false`.
* `clientId` is the MQTT client identifier, must be unique on the broker. Default value is `<hostname>.<serialPortShort><process-id>`.
* `user` is the user for MQTT authentication. Default value is an empty string, which means no authentication.
* `password` is the password for MQTT authentication. Default value is an empty string.
* `reconnectInterval` is the time interval between loosing the MQTT broker connection and the reconnection start. It is specified in milliseconds, the default value is `2000`. You can disable MQTT broker reconnection with `0` value, in this case serial2mqtt will exit upon MQTT disconnection.
* `publishInterval` is the time interval that serial2mqtt publishes its uptime and alive messages. You can disable this functionality with `0` (can be useful during development and testing).
* `localPersistenceDir` is the directory where local persistence database will be stored. Default value is an empty string, which disables this functionality. See *Local persistence support* for details.
* `localPersistenceFilters` array contains the topic filters, see *Local persistence support* for details.

### Local persistence support
Local persistence is an useful, when:
* the MCU has no EEPROM to store its own configuration
* the configuration is too complex/big to store in the MCU
* the configuration changes are too frequent to wear out the EEPROM quickly
* the MCU developer is lazy to store the configuration locally

This feature implements something similar to the MQTT retained messages, but
* it is stored on the host running serial2mqtt so it is available during MQTT broker downtime
* no need to set the retain flag on the senders consistently. All of the messages will be saved matching one of the filters in the `localPersistenceFilters` array

The local persistence functionality is disabled by default. The user has to set the `localPersistenceDir` parameter in the mqtt group to enable it, and has to give filters in the `localPersistenceFilters` array. If it's active, all of the stored messages will be played back right after every serial connection establishment and after every subscribe request.

Here's an example:
```
{
    "mqtt": {
        "connection": "tcp://test.mosquitto.org:1883",
        "localPersistenceDir": "/var/serial2mqtt",
        "localPersistenceFilters": ["hvac/controller/+/set"]
```

## The `log` group
The log group configures how to handle logging:
* `protocol` controls whether the serial protocol logging is enabled or disabled, default value is `false`.
* `debug` controls whether debug logging is enabled or disabled, default value is `true`.
* `mqtt` controls whether MQTT related logging is enabled or disabled, default value is `false`.
* `program` controls whether the MCU firmware programming logging is enabled or disabled, default value is `false`.
* `useColors` controls whether the logging messages are colored or not. Default value is `true`.
* `console` log to stdout
* `file` file prefix for rotating logs 5 of each 2MB
