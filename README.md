# serial2mqtt
# WORK IN PROGRESS !! Major rework
<script src="https://cdnjs.cloudflare.com/ajax/libs/js-sequence-diagrams/1.0.6/sequence-diagram-min.js" integrity="sha256-XQQMbGaLPASU6qUHPt5k45JchBlcJebnuBdnWfW7T6I=" crossorigin="anonymous"></script>

MQTT for all micro-controllers ! The purpose is to offer MQTT publisher/subscriber functionality to all small micro controllers. Those with just a UART or USB interface.
Example : a cheap STM32 board on ebay.
![Afbeeldingsresultaat voor ebay stm32](http://www.rogerclark.net/wp-content/uploads/2014/11/STM32Mini-300x300.jpg)
This program will act as a full MQTT Client gateway and make integration as simple as possible.
This was created because Ethernet or WiFi is still absent in most ( cheap ) controllers .
Also the concept behind is that a central PC or Raspberry PI can act as the intelligent mind behind commodity components.

![enter image description here](http://drive.google.com/uc?export=view&id=1rGeHOaMEGLJJqxFsd5fnaAE7N1DHoJUI)

Arduino Sample program to communicate with the serial2mqtt  gateway

   

    class Mqtt {
      public:
        static void publish(int qos, bool retain, String topic, String message ) {
          Serial.printf("M,%d,%d,%s,%s\n", qos, retain, topic.c_str(), message.c_str());
        }
    };
    int i = 0;
    void loop() {
      delay(100);                       // wait for a 0.1 second
      String  message=String(i++);
      Mqtt::publish(0, false, "src/arduino1/system/upTime", message);
      Mqtt::publish(0,false,"src/arduino1/system/host","arduino1");
    }

## Working assumptions
- Topic Names 
--The design will take into account some assumptions about topic names and tree-structure to make it simple to use.
Structure topic to and from  device :
-- dst/DEVICE/SERVICE/PROPERTY
-- src/DEVICE/SERVICE/PROPERTY
-- if DEVICE is not known yet the serial2mqtt will subscribe to the dst/PORT/prog , where PORT is for example ttyUSB0
- Serial messages will be **BINARY** or **ASCII**
-- BINARY format will be CBOR encoded in a SLIP envelope
-- ASCII will be text delimited by newlines
- The serial2mqtt should be able to reset the device ( hard reset )
- The serial2mqtt should be able to program new code into the device
- Through the same communication, debugging logs can be handled without disturbing the mqtt flow
- the serial2mqtt establishes the client MQTT link and subscribes to dst/DEVICE/#
- when there is a big delay on the serial2mqtt serial input, it will stop the TCP MQTT connection
- serial2mqtt is event driven and not blocking
- one instance of serial2mqtt should be able to handle different serial ports
- USB devices coming and going should be tracked by serial2mqtt
- Configuration can be command line and config file driven ( JSON )
- serial2mqtt should be able to program the device through the serial interface, for this purpose a third party app will be launched with the concerned serial port as argument.

# Protocol
## ASCII TEXT
- `M,PUB,<qos>,<retain>,<topic>,<message>`
## BINARY CBOR SLIP
    <END><SLIP ENCODED MESSAGE><END>
    <SLIP ENCODED MESSAGE> == <'M'><"PUB">,<qos Integer><retain boolean><topic string><message binary><CRC integer>
 ## CONNECTION SETUP
```mermaid
sequenceDiagram
µC->> serial2mqtt: M,PUB,0,0,src/drive/motor/rpm,1234
Note right of serial2mqtt: connects to mqtt broker at first message.
serial2mqtt-->>MQTT Broker: CONNECT(broker,port)
MQTT Broker --x serial2mqtt: CONNACK
serial2mqtt-->>MQTT Broker: SUBSCRIBE("dst/drive/#")
serial2mqtt-->>MQTT Broker: PUBLISH("src/drive/motor/rpm",1000,0,false)
µC->> serial2mqtt: M,PUB,0,0,src/drive/motor/temp,20
serial2mqtt-->>MQTT Broker:  PUBLISH("src/drive/motor/temp",20,0,false)
Note right of µC: no more messages after 5 sec, serial2mqtt disconnects.
serial2mqtt-->>MQTT Broker: DISCONNECT
```
# Programming through serial2mqtt
A command line utility will send a single mqtt request to the serial2mqtt gateway to program the microcontroller.
```mermaid
sequenceDiagram
µC->> serial2mqtt: M,PUB,0,0,src/drive/motor/rpm,1234
serial2mqtt->>MQTT Broker:  PUBLISH("src/drive/motor/temp",20,0,false)
programmer CLI -x MQTT Broker: PUBLISH("dst/drive/prog/flash",flash image binary)
MQTT Broker -x serial2mqtt : PUBLISH
activate serial2mqtt
serial2mqtt -x µC : program flash image
deactivate serial2mqtt
```
# Logging through serial2mqtt
The micrcontroller will also log to the central logging system 
