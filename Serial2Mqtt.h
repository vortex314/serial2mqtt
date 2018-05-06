#ifndef SERIAL2MQTT_H
#define SERIAL2MQTT_H

#include "MQTTAsync.h"
#include "Config.h"
#include "CircBuf.h"
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <linux/serial.h>
#include <termios.h>
#include <asm-generic/ioctls.h>
#include <sys/ioctl.h>


class Serial2Mqtt
{

	typedef enum {
		SERIAL_DISCONNECTED,
		SERIAL_CONNECTED
	} SerialState;
	MQTTAsync_token _deliveredtoken;
	MQTTAsync _client;
	int _signalFd[2];   // pipe fd to wakeup in select
	// SERIAL
	std::string _serialPort;
	int _serialBaudrate;
	int _serialFd=0;
	bool _serialConnected=false;
	std::string _serialLine;
	CircBuf _serialBuffer;
	// MQTT

	std::string _mqttHost;
	std::string _mqttClientId;
	uint16_t _mqttPort;
	uint32_t _mqttKeepAliveInterval;
	std::string _mqttWillMessage;
	std::string _mqttWillTopic;
	uint16_t _mqttWillQos;
	bool _mqttWillRetained;
	std::string _mqttDevice;

	bool _mqttConnected=false;
	bool _mqttConnecting=false;
	std::string _mqttSubscribedTo;

	Config _config;

public:
	typedef enum {ERROR='E',
	              SERIAL_CONNECT='C',
	              SERIAL_DISCONNECT='D',
	              SERIAL_RXD='R',
	              MQTT_CONNECT_SUCCESS='c',
				  MQTT_CONNECT_FAIL='f',
				  MQTT_SUBSCRIBE_SUCCESS='s',
				  MQTT_SUBSCRIBE_FAIL='g',
				  MQTT_PUBLISH_SUCCESS='p',
				  MQTT_PUBLISH_FAIL='q',
	              MQTT_DISCONNECTED='d',
				  MQTT_MESSAGE_RECEIVED='r',
	              TIMEOUT='T'
	             } Signal;

	Serial2Mqtt();
	~Serial2Mqtt();
	void init();
	void run();
	void threadFunction(void*);
	void signal(uint8_t s);
	Signal waitSignal(uint32_t timeout);


	void setConfig(Config config);
	void setSerialPort(std::string port);
	Erc serialConnect();
	void serialDisconnect();
	void serialRxd();
	bool serialGetLine(std::string& line);
	void serialHandleLine(std::string& line);
	void serialPublish(std::string topic,Bytes message,int qos,bool retained);
//	void serialMqttPublish(std::string topic,Bytes message,int qos,bool retained);


	Erc mqttConnect();
	void mqttDisconnect();
	void mqttPublish(std::string topic,Bytes message,int qos,bool retained);
	void mqttSubscribe(std::string topic);

	static void onConnectionLost(void *context, char *cause);
	static int onMessage(void *context, char *topicName, int topicLen, MQTTAsync_message *message);
	static void onDisconnect(void* context, MQTTAsync_successData* response);
	static void onConnectFailure(void* context, MQTTAsync_failureData* response);
	static void onConnectSuccess(void* context, MQTTAsync_successData* response);
	static void onSubscribeSuccess(void* context, MQTTAsync_successData* response);
	static void onSubscribeFailure(void* context, MQTTAsync_failureData* response);
	static void onPublishSuccess(void* context, MQTTAsync_successData* response);
	static void onPublishFailure(void* context, MQTTAsync_failureData* response);


};

#endif // SERIAL2MQTT_H
