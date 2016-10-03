#include "MqttClient.h"
#include "MQTTAsync.h"

#define ADDRESS     "tcp://limero.ddns.net:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "stm32/alive"
#define PAYLOAD     "true"
#define QOS         1
#define TIMEOUT     10000L

MQTTAsync MqttClient::client;
int MqttClient::disc_finished = 0;
int MqttClient::subscribed = 0;
int MqttClient::finished = 0;
int MqttClient::deliveredtoken = 0;
Str MqttClient::connection(30);
Str MqttClient::clientId;
Str MqttClient::user(30);
Str MqttClient::password(30);
Str MqttClient::willTopic(40);
Str MqttClient::willMessage(40);
int MqttClient::willQos = 0;
bool MqttClient::willRetain = false;

void MqttClient::onConnectionLost(void *context, char *cause) {
	MQTTAsync client = (MQTTAsync) context;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	int rc;
	LOGF("Connection lost, cause : %s ", cause);
	LOGF("Reconnecting...");
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS) {
		LOGF("Failed to start connect, return code %d", rc);
		finished = 1;
	}
}

int MqttClient::onMessage(void *context, char *topicName, int topicLen,
		MQTTAsync_message *message) {
	Bytes bytes((uint8_t*) message->payload, message->payloadlen);
	Cbor cbor(1000);
	cbor.addKeyValue(H("topic"), topicName).addKeyValue(H("message"), bytes).addKeyValue(
			H("qos"), message->qos).addKeyValue(H("retained"),
			message->retained);
	eb.publish(H("mqtt.publish"), cbor);
	MQTTAsync_freeMessage(&message);
	MQTTAsync_free(topicName);
	return 1;
}

void MqttClient::onDisconnect(void* context, MQTTAsync_successData* response) {
	LOGF("Successful disconnection");
	disc_finished = 1;
	eb.publish((uint16_t) H("mqtt.disconnected"));
}

void MqttClient::onSubscribe(void* context, MQTTAsync_successData* response) {
	LOGF("Subscribe succeeded");
	subscribed = 1;
	eb.publish(H("mqtt.suback"));
}

void MqttClient::onSubscribeFailure(void* context,
		MQTTAsync_failureData* response) {
	LOGF("Subscribe failed, rc %d", response ? response->code : 0);
	finished = 1;
	Cbor cbor(100);
	cbor.addKeyValue(H("error"), response ? response->code : 0);
	eb.publish(H("mqtt.error"));
}

void MqttClient::onConnectFailure(void* context,
		MQTTAsync_failureData* response) {
	LOGF("Connect failed, rc %d", response ? response->code : 0);
	finished = 1;
	Cbor cbor(100);
	cbor.addKeyValue(H("error"), response ? response->code : 0);
	eb.publish(H("mqtt.disconnected"), cbor);
}

void MqttClient::onConnect(void* context, MQTTAsync_successData* response) {
	LOGF("Successful connection");
	eb.publish(H("mqtt.connected"));
}

Erc MqttClient::connect(Str& connection, Str& willTopic, Str& willMessage,
		int willQos) {
	MqttClient::connection = connection;
	MqttClient::willTopic = willTopic;
	MqttClient::willMessage = willMessage;
	MqttClient::willQos = willQos;
	MqttClient::clientId = CLIENTID;
	MqttClient::user = "";
	MqttClient::password = "";

	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	MQTTAsync_willOptions will_opts = MQTTAsync_willOptions_initializer;
//        MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
//        MQTTAsync_token token;
	int rc = 0;

	MQTTAsync_create(&client, connection.c_str(), CLIENTID,
			MQTTCLIENT_PERSISTENCE_NONE, NULL);

	MQTTAsync_setCallbacks(client, NULL, onConnectionLost, onMessage, NULL);

	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	conn_opts.onSuccess = onConnect;
	conn_opts.onFailure = onConnectFailure;
	conn_opts.context = client;
	will_opts.message = willMessage.c_str();
	will_opts.topicName = willTopic.c_str();
	will_opts.qos = willQos;
	will_opts.retained = 0;
	conn_opts.will = &will_opts;
	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS) {
		LOGF("Failed to start connect, return code %d", rc);
	}
	return rc;
}

Erc MqttClient::disconnect() {
	MQTTAsync_disconnectOptions disc_opts =
			MQTTAsync_disconnectOptions_initializer;
	disc_opts.onSuccess = onDisconnect;
	int rc = 0;
	if ((rc = MQTTAsync_disconnect(client, &disc_opts)) != MQTTASYNC_SUCCESS) {
		LOGF("Failed to start disconnect, return code %d", rc);
	}
	MQTTAsync_destroy(&client);
	return rc;
}

void MqttClient::onSend(void* context, MQTTAsync_successData* response) {
	LOGF("Message with token value %d delivery confirmed", response->token);
	eb.publish(H("mqtt.send"));
}

Erc MqttClient::publish(Str& topic, Bytes& message, int qos, bool retain) {
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
	int rc = E_OK;
	opts.onSuccess = MqttClient::onSend;
	opts.context = client;

	pubmsg.payload = message.data();
	pubmsg.payloadlen = message.length();
	pubmsg.qos = qos;
	pubmsg.retained = retain;
	deliveredtoken = 0;

	if ((rc = MQTTAsync_sendMessage(client, topic.c_str(), &pubmsg, &opts))
			!= MQTTASYNC_SUCCESS) {
		printf("Failed to start sendMessage, return code %d", rc);
	}
	return rc;
}

Erc MqttClient::subscribe(Str& topic, int qos) {
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	LOGF("Subscribing to topic %s for client %s using QoS%d",
			topic.c_str(), CLIENTID, qos);
	opts.onSuccess = onSubscribe;
	opts.onFailure = onSubscribeFailure;
	opts.context = client;

	deliveredtoken = 0;
	int rc = E_OK;

	if ((rc = MQTTAsync_subscribe(client, topic.c_str(), qos, &opts))
			!= MQTTASYNC_SUCCESS) {
		LOGF("Failed to start subscribe, return code %d", rc);
	}
	return rc;
}

