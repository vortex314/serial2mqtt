#include "MqttClient.h"
#include "MQTTAsync.h"
#include <Cbor.h>
#include <EventBus.h>

#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#define HOST "test.mosquitto.org"

MqttClient* mqtt;


void MqttClient::onEvent(Cbor& cbor)
{
    uint32_t cmd;
    if (cbor.getKeyValue(EB_REQUEST, cmd))
    {
        cbor.getKeyValue(EB_SRC,_lastSrc);
        if (cmd == H("connect"))
            connect(cbor);
        else if (cmd == H("subscribe"))
            subscribe(cbor);
        else if (cmd == H("publish"))
            publish(cbor);
        else if (cmd == H("disconnect"))
            disconnect(cbor);
    }
}

MqttClient::MqttClient() : Actor("MqttClient"),
    _host(40), _port(1883), _clientId(30), _user(20), _password(20), _willTopic(
        30), _willMessage(30), _willQos(0), _willRetain(false), _keepAlive(
            20), _cleanSession(1), _connected(false),_msgid(0)
{
}

MqttClient::~MqttClient()
{
    ::close(_fd[0]);
    ::close(_fd[1]);
}

void MqttClient::wakeup()
{
    if ( write(_fd[1],"W",1) < 1 )
    {
        LOGF("Failed to write pipe: %s (%d)", strerror(errno), errno);
    }
}

int MqttClient::fd()
{
    return _fd[0];
}

void MqttClient::setup()
{
    _host = HOST;
    _clientId = CLIENTID;
    _user = "";
    _password = "";
    _willTopic = _clientId;
    _willTopic.append("/system/alive");
    _willMessage = "true";
    if (pipe(_fd) < 0)        LOGF("Failed to create pipe: %s (%d)", strerror(errno), errno);

    if (fcntl(_fd[0], F_SETFL, O_NONBLOCK) < 0)
        LOGF("Failed to set pipe non-blocking mode: %s (%d)", strerror(errno), errno);
}


void MqttClient::loadConfig(Cbor& cbor)
{
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

void MqttClient::connect(Cbor& cbor)
{
    loadConfig(cbor);

    if (_connected)
    {
        eb.reply(_lastSrc,H("connect"),H("mqtt")).addKeyValue(EB_ERROR,E_OK);
        eb.send();
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

    MQTTAsync_setCallbacks(_client, this, onConnectionLost, onMessage, NULL); //TODO add ondelivery

    conn_opts.keepAliveInterval = _keepAlive;
    conn_opts.cleansession = 1;
    conn_opts.onSuccess = onConnect;
    conn_opts.onFailure = onConnectFailure;
    conn_opts.context = this;
    will_opts.message = _willMessage.c_str();
    will_opts.topicName = _willTopic.c_str();
    will_opts.qos = _willQos;
    will_opts.retained = _willRetain;
    conn_opts.will = &will_opts;
    if ((rc = MQTTAsync_connect(_client, &conn_opts)) != MQTTASYNC_SUCCESS)
    {
        LOGF("Failed to start connect, return code %d", rc);
        eb.reply(_lastSrc,H("connect"),H("mqtt")).addKeyValue(H("error"), (uint32_t) EINVAL).addKeyValue(H("error_detail"), rc);
        eb.send();
    }
}

void MqttClient::onConnect(void* context, MQTTAsync_successData* response)
{
    MqttClient* mq = (MqttClient*)context;
    mq->_connected = true;
    LOGF("Successful connection");
    eb.reply(mq->_lastSrc,H("connect"),H("mqtt")).addKeyValue(EB_ERROR, (uint32_t) E_OK);
    eb.send();
    mq->wakeup();
}


void MqttClient::onConnectFailure(void* context,
                                  MQTTAsync_failureData* response)
{
    MqttClient* mq = (MqttClient*)context;
    mq->_connected = false;
    LOGF("Connect failed, rc %d", response ? response->code : 0);
    eb.reply(mq->_lastSrc,H("mqtt"),H("connect")).addKeyValue(H("error_detail"), response ? response->code : 0).addKeyValue(EB_ERROR, ECONNABORTED);
    eb.send();
    mq->wakeup();
}

void MqttClient::onConnectionLost(void *context, char *cause)
{
    MqttClient* mq = (MqttClient*)context;
    LOGF("Connection lost, cause : %s ", cause==0 ? " unknown cause " : cause);
    mq->_connected = false;
    eb.event(H("mqtt"),H("disconnected"))
    .addKeyValue(H("error_detail"),
                 cause == 0 ? " unknown cause " : cause)
    .addKeyValue(EB_ERROR, (uint32_t) EINVAL);
    eb.send();
    mq->wakeup();

}


int MqttClient::onMessage(void *context, char *topicName, int topicLen,
                          MQTTAsync_message *message)
{
    MqttClient* mq = (MqttClient*)context;
    Bytes bytes((uint8_t*) message->payload, message->payloadlen);
    eb.event(H("mqtt"),H("published")).addKeyValue(H("topic"), topicName).addKeyValue(H("message"), bytes).addKeyValue(
        H("qos"), message->qos).addKeyValue(H("retained"),
                                            message->retained);
    eb.send();
    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(topicName);
    mq->wakeup();
    return 1;
}



void MqttClient::disconnect(Cbor& cbor)
{
    _connected = false;

    MQTTAsync_disconnectOptions disc_opts =
        MQTTAsync_disconnectOptions_initializer;
    disc_opts.onSuccess = onDisconnect;
    disc_opts.context=this;
    int rc = 0;
    if ((rc = MQTTAsync_disconnect(Mqtt._client, &disc_opts))
            != MQTTASYNC_SUCCESS)
    {
        LOGF("Failed to start disconnect, return code %d", rc);
        eb.reply(_lastSrc,H("disconnect"),H("mqtt")).addKeyValue(H("error_detail"), "disconnect fail").addKeyValue(H("error"), ENOTCONN);
        eb.send();
    }
    MQTTAsync_destroy(&_client);
}

void MqttClient::onDisconnect(void* context, MQTTAsync_successData* response)
{
    MqttClient* mq = (MqttClient*)context;
    mq->_connected = false;

    LOGF("Successful disconnection");
    eb.reply(Mqtt._lastSrc,H("mqtt"),H("disconnect")).addKeyValue(EB_ERROR, E_OK);
    eb.send();
    mq->wakeup();
}

void MqttClient::publish(Cbor& cbor)
{
    if (false)
    {
        eb.reply().addKeyValue(H("error_detail"),"not connected").addKeyValue(H("error"), (uint32_t) ENOTCONN);
        eb.send();
        return;
    }
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
    Str topic(60);
    Bytes message(1024);
    int qos = 0;
    bool retain=false;
    if ( cbor.getKeyValue(H("topic"), topic)  &&     cbor.getKeyValue(H("message"), message))
    {
        cbor.getKeyValue(H("qos"), qos);
        cbor.getKeyValue(H("retain"), retain);

        int rc = E_OK;
        opts.onSuccess = MqttClient::onPublishSuccess;
        opts.onFailure = MqttClient::onPublishFailure;
        opts.context = this;

        pubmsg.payload = message.data();
        pubmsg.payloadlen = message.length();
        pubmsg.qos = qos;
        pubmsg.retained = retain;
        pubmsg.msgid = _msgid++;

        if ((rc = MQTTAsync_sendMessage(Mqtt._client, topic.c_str(), &pubmsg, &opts))
                != MQTTASYNC_SUCCESS)
        {
            LOGF("Failed MQTTAsync_sendMessage() =  %d", rc);
            eb.reply().addKeyValue(H("error_detail"), rc).addKeyValue(H("error"), (uint32_t) EAGAIN);
            eb.send();
        }
    } else {

        eb.reply().addKeyValue(H("error_detail"),strerror(EINVAL)).addKeyValue(H("error"), (uint32_t) EINVAL);
        eb.send();
    }
}

void MqttClient::onPublishSuccess(void* context,
                                  MQTTAsync_successData* response)
{
    MqttClient* mq = (MqttClient*)context;

//	LOGF("Message with token value %d delivery confirmed", response->token);
    eb.reply(mq->_lastSrc,H("publish"),H("mqtt")).addKeyValue(H("error"),E_OK);
    eb.send();
    mq->wakeup();
}

void MqttClient::onPublishFailure(void* context,
                                  MQTTAsync_failureData* response)
{
    MqttClient* mq = (MqttClient*)context;
    LOGF("Publish failed, rc %d", response ? response->code : 0);

    eb.reply(mq->_lastSrc,H("publish"),H("mqtt"))
    .addKeyValue(H("error"), response ? response->code : 0)
    .addKeyValue(H("error_detail"),
                 response->message ? "no detail" : response->message);
    eb.send();
    mq->wakeup();
}

void MqttClient::subscribe(Cbor& cbor)
{
    if (!_connected)
    {
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
            != MQTTASYNC_SUCCESS)
    {
        LOGF("Failed to start subscribe, return code %d", rc);
        eb.reply(_lastSrc,H("subscribe"),H("mqtt")).addKeyValue(H("error"), (uint32_t) EAGAIN).addKeyValue(H("error_detail"), rc);;
        eb.send();
    }
}

void MqttClient::onSubscribe(void* context, MQTTAsync_successData* response)
{
    MqttClient* mq = (MqttClient*)context;
    LOGF("Subscribe succeeded");
    eb.reply(Mqtt._lastSrc,H("subscribe"),H("mqtt")).addKeyValue(H("error"), (uint32_t) E_OK);
    eb.send();
    mq->wakeup();
}

void MqttClient::onSubscribeFailure(void* context,
                                    MQTTAsync_failureData* response)
{
    MqttClient* mq = (MqttClient*)context;
    LOGF("Subscribe failed, rc %d", response ? response->code : 0);
    eb.reply(mq->_lastSrc,H("subscribe"),H("mqtt")).addKeyValue(H("error"), response ? response->code : 0);
    eb.send();
    mq->wakeup();
    mq->wakeup();
}

