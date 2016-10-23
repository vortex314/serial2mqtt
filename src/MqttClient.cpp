#include "MqttClient.h"
#include "MQTTAsync.h"
#include <Cbor.h>
#include <EventBus.h>

#define HOST "test.mosquitto.org"

MqttClient Mqtt;

void MqttClient::router(Cbor& cbor) {
	uint32_t cmd;
	if (cbor.getKeyValue((uint16_t) 0, cmd)) {
		if (cmd == H("mqtt.connect"))
			Mqtt.connect(cbor);
		else if (cmd == H("mqtt.subscribe"))
			Mqtt.subscribe(cbor);
		else if (cmd == H("mqtt.publish"))
			Mqtt.publish(cbor);
		else if (cmd == H("mqtt.disconnect"))
			Mqtt.disconnect(cbor);
	}
}

MqttClient::MqttClient() :
		_host(40), _port(1883), _clientId(30), _user(20), _password(20), _willTopic(
				30), _willMessage(30), _willQos(0), _willRetain(false), _keepAlive(
				20), _cleanSession(1), _connected(false),_msgid(0) {
}

MqttClient::~MqttClient() {

}

void MqttClient::init() {
	_host = HOST;
	_clientId = CLIENTID;
	_user = "";
	_password = "";
	_willTopic = _clientId;
	_willTopic.append("/system/alive");
	_willMessage = "true";
}

void MqttClient::loadConfig(Cbor& cbor) {
	cbor.getKeyValue(H("host"), _host);
	cbor.getKeyValue(H("port"), _port);
	cbor.getKeyValue(H("client_id"), _clientId);
	cbor.getKeyValue(H("user"), _user);
	cbor.getKeyValue(H("password"), _password);
	cbor.getKeyValue(H("keep_alive"), _keepAlive);
	cbor.getKeyValue(H("clean_session"), _cleanSession);
	cbor.getKeyValue(H("will_topic"), _willTopic);
	cbor.getKeyValue(H("will_message"), _willMessage);
	cbor.getKeyValue(H("will_qos"), _willQos);
	cbor.getKeyValue(H("will_retain"), _willRetain);
}

void MqttClient::connect(Cbor& cbor) {
	init();
	loadConfig(cbor);

	if (_connected) {
		eb.publish(H("mqtt.connack"));
		return;
	}

	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	MQTTAsync_willOptions will_opts = MQTTAsync_willOptions_initializer;
//        MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
//        MQTTAsync_token token;
	int rc = 0;

	Str connection(30);
	connection = "tcp://";
	connection.append(_host).append(':').append(_port);
	MQTTAsync_create(&_client, connection.c_str(), _clientId.c_str(),
			MQTTCLIENT_PERSISTENCE_NONE, NULL);

	MQTTAsync_setCallbacks(_client, NULL, onConnectionLost, onMessage, NULL);

	conn_opts.keepAliveInterval = _keepAlive;
	conn_opts.cleansession = 1;
	conn_opts.onSuccess = onConnect;
	conn_opts.onFailure = onConnectFailure;
	conn_opts.context = _client;
	will_opts.message = _willMessage.c_str();
	will_opts.topicName = _willTopic.c_str();
	will_opts.qos = _willQos;
	will_opts.retained = _willRetain;
	conn_opts.will = &will_opts;
	if ((rc = MQTTAsync_connect(_client, &conn_opts)) != MQTTASYNC_SUCCESS) {
		LOGF("Failed to start connect, return code %d", rc);
		Cbor resp(20);
		resp.addKeyValue(H("error_detail"), rc);
		resp.addKeyValue(H("error"), (uint32_t) EINVAL);
		eb.publish(H("mqtt.connack"), resp);
	}
}

void MqttClient::onConnect(void* context, MQTTAsync_successData* response) {
	Mqtt._connected = true;
	LOGF("Successful connection");
	eb.publish(H("mqtt.connack"));
}

void MqttClient::onConnectionLost(void *context, char *cause) {
	Mqtt._connected = false;
	Cbor resp(20);
	resp.addKeyValue(H("error_message"),
			cause == 0 ? " unknown cause " : cause);
	resp.addKeyValue(H("error"), (uint32_t) EINVAL);
	eb.publish(H("mqtt.connack"), resp);
	LOGF("Connection lost, cause : %s ", cause==0 ? " unknown cause " : cause);
	Cbor cbor(1);
//	Mqtt.connect(cbor); // connect with default values , previously loaded
}

void MqttClient::onConnectFailure(void* context,
		MQTTAsync_failureData* response) {
	Mqtt._connected = false;
	LOGF("Connect failed, rc %d", response ? response->code : 0);
	Cbor cbor(100);
	cbor.addKeyValue(H("error_detail"), response ? response->code : 0);
	cbor.addKeyValue(H("error"), ECONNABORTED);
	eb.publish(H("mqtt.connack"), cbor);
}

int MqttClient::onMessage(void *context, char *topicName, int topicLen,
		MQTTAsync_message *message) {
	Bytes bytes((uint8_t*) message->payload, message->payloadlen);
	Cbor cbor(1000);
	cbor.addKeyValue(H("topic"), topicName).addKeyValue(H("message"), bytes).addKeyValue(
			H("qos"), message->qos).addKeyValue(H("retained"),
			message->retained);
	eb.publish(H("mqtt.published"), cbor);
	MQTTAsync_freeMessage(&message);
	MQTTAsync_free(topicName);
	return 1;
}

void MqttClient::onDisconnect(void* context, MQTTAsync_successData* response) {
	LOGF("Successful disconnection");
	eb.publish((uint16_t) H("mqtt.disconnack"));
}

void MqttClient::disconnect(Cbor& cbor) {
	MQTTAsync_disconnectOptions disc_opts =
			MQTTAsync_disconnectOptions_initializer;
	disc_opts.onSuccess = onDisconnect;
	int rc = 0;
	if ((rc = MQTTAsync_disconnect(Mqtt._client, &disc_opts))
			!= MQTTASYNC_SUCCESS) {
		LOGF("Failed to start disconnect, return code %d", rc);
		Cbor resp(20);
		resp.addKeyValue(H("error_message"), "disconnect fail");
		resp.addKeyValue(H("error"), ENOTCONN);
		eb.publish(H("mqtt.disconnack"), resp);

	}
	MQTTAsync_destroy(&Mqtt._client);
}

void MqttClient::publish(Cbor& cbor) {
	if (!_connected) {
		Cbor resp(20);
		resp.addKeyValue(H("error"), ENOTCONN);
		eb.publish(H("mqtt.puback"), resp);
	}
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
	Str topic(60);
	Bytes message(1024);
	int qos = 0;
	bool retain;
	cbor.getKeyValue(H("topic"), topic);
	cbor.getKeyValue(H("message"), message);
	cbor.getKeyValue(H("qos"), qos);
	cbor.getKeyValue(H("retain"), retain);

	int rc = E_OK;
	opts.onSuccess = MqttClient::onPublishSuccess;
	opts.onFailure = MqttClient::onPublishFailure;
	opts.context = _client;

	pubmsg.payload = message.data();
	pubmsg.payloadlen = message.length();
	pubmsg.qos = 1;
	pubmsg.retained = retain;
	pubmsg.msgid = _msgid++;

	if ((rc = MQTTAsync_sendMessage(Mqtt._client, topic.c_str(), &pubmsg, &opts))
			!= MQTTASYNC_SUCCESS) {
		LOGF("Failed to start sendMessage, return code %d", rc);
		Cbor resp(20);
		resp.addKeyValue(H("error_detail"), rc);
		resp.addKeyValue(H("error"), (uint32_t) EAGAIN);
		eb.publish(H("mqtt.puback"), resp);
	}
}

void MqttClient::onPublishSuccess(void* context,
		MQTTAsync_successData* response) {
//	LOGF("Message with token value %d delivery confirmed", response->token);
	eb.publish(H("mqtt.puback"));
}

void MqttClient::onPublishFailure(void* context,
		MQTTAsync_failureData* response) {
	LOGF("Publish failed, rc %d", response ? response->code : 0);
	Cbor cbor(100);

	cbor.addKeyValue(H("error"), response ? response->code : 0);
	cbor.addKeyValue(H("error_msg"),
			response->message ? "no detail" : response->message);
	eb.publish(H("mqtt.puback"), cbor);
}

void MqttClient::subscribe(Cbor& cbor) {
	if (!_connected) {
		Cbor resp(20);
		resp.addKeyValue(H("error"), ENOTCONN);
		eb.publish(H("mqtt.puback"), resp);
	}
	Str topic(60);
	int qos = 0;
	cbor.getKeyValue(H("topic"), topic);
	cbor.getKeyValue(H("qos"), qos);
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	LOGF("Subscribing to topic %s for client %s using QoS%d",
			topic.c_str(), CLIENTID, qos);
	opts.onSuccess = onSubscribe;
	opts.onFailure = onSubscribeFailure;
	opts.context = Mqtt._client;
	int rc = E_OK;

	if ((rc = MQTTAsync_subscribe(Mqtt._client, topic.c_str(), qos, &opts))
			!= MQTTASYNC_SUCCESS) {
		LOGF("Failed to start subscribe, return code %d", rc);
		Cbor resp(20);
		resp.addKeyValue(H("error_detail"), rc);
		resp.addKeyValue(H("error"), (uint32_t) EAGAIN);
		eb.publish(H("mqtt.suback"), resp);
	}
}

void MqttClient::onSubscribe(void* context, MQTTAsync_successData* response) {
	LOGF("Subscribe succeeded");
	eb.publish(H("mqtt.suback"));
}

void MqttClient::onSubscribeFailure(void* context,
		MQTTAsync_failureData* response) {
	LOGF("Subscribe failed, rc %d", response ? response->code : 0);
	Cbor cbor(100);
	cbor.addKeyValue(H("error"), response ? response->code : 0);
	eb.publish(H("mqtt.suback"), cbor);
}

