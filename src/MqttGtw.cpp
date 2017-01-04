#include "MqttGtw.h"
#include "MQTTAsync.h"
#include <Cbor.h>
#include <EventBus.h>

#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

 int wakeupPipe[2];
//--------------------------------------------------------------------------------------------------------
void MqttGtw::onEvent(Cbor& msg) {
    Cbor cbor(0);
    uint32_t error;
    PT_BEGIN()
    ;
CONNECTING: {
        while (true) {
            timeout(2000);
            PT_YIELD_UNTIL(timeout());

            timeout(UINT_LEAST32_MAX);
            connect(cbor);
            PT_YIELD_UNTIL(eb.isReply(id(),H("connect")));
            if ( msg.getKeyValue(EB_ERROR,error) && error==0) {
                goto CONNECTED;
            }
        };
    };
CONNECTED: {
        while(true) {
        timeout(5000);
        PT_YIELD_UNTIL(eb.isEvent(id(),H("closed") || eb.isEvent(id(),H("disconnected"))) || timeout() );
        if ( _connected==false) goto CONNECTING;
        if ( timeout() ) continue;
        goto CONNECTING;
        }
    }

    PT_END()
}
//--------------------------------------------------------------------------------------------------------
void MqttGtw::onActorRegister(Cbor& cbor) {

}
//--------------------------------------------------------------------------------------------------------
MqttGtw::MqttGtw() : Actor("mqtt"),
    _host(40), _port(1883), _clientId(30), _user(20), _password(20), _willTopic(
        30), _willMessage(30), _willQos(0), _willRetain(false), _keepAlive(
            20), _cleanSession(1), _connected(false),_msgid(0),_prefix(20) {
}
//--------------------------------------------------------------------------------------------------------
MqttGtw::~MqttGtw() {

}
//--------------------------------------------------------------------------------------------------------
void MqttGtw::wakeup() {
    if ( write(wakeupPipe[1],"W",1) < 1 ) {
        LOGF("Failed to write pipe: %s (%d)", strerror(errno), errno);
    }
    LOGF(" wakeup ");
}
//--------------------------------------------------------------------------------------------------------
int MqttGtw::fd() {
    return wakeupPipe[0];
}
//--------------------------------------------------------------------------------------------------------
void MqttGtw::setup() {
    _host = HOST;
    _clientId = CLIENTID;
    _user = "";
    _password = "";
    _prefix="limero";
    _willTopic = _prefix;
    _willTopic.append("/system/alive");
    _willMessage = "false";
    _keepAlive=120;
    timeout(2000);
    if (pipe(wakeupPipe) < 0)        LOGF("Failed to create pipe: %s (%d)", strerror(errno), errno);

    if (fcntl(wakeupPipe[0], F_SETFL, O_NONBLOCK) < 0)
        LOGF("Failed to set pipe non-blocking mode: %s (%d)", strerror(errno), errno);
}
//--------------------------------------------------------------------------------------------------------
void MqttGtw::loadConfig(Cbor& cbor) {
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
    cbor.getKeyValue(H("prefix"),_prefix);
}
//--------------------------------------------------------------------------------------------------------
void MqttGtw::connect(Cbor& cbor) {
    loadConfig(cbor);
    cbor.getKeyValue(EB_SRC,_lastSrc);

    if (_connected) {
        eb.reply(_lastSrc,H("connect"),id()).addKeyValue(EB_ERROR,E_OK);
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
    conn_opts.onSuccess = onConnectSuccess;
    conn_opts.onFailure = onConnectFailure;
    conn_opts.context = this;
    will_opts.message = _willMessage.c_str();
    will_opts.topicName = _willTopic.c_str();
    will_opts.qos = _willQos;
    will_opts.retained = _willRetain;
    conn_opts.will = &will_opts;
    if ((rc = MQTTAsync_connect(_client, &conn_opts)) != MQTTASYNC_SUCCESS) {
        LOGF("Failed to start connect, return code %d", rc);
        eb.reply(_lastSrc,H("connect"),id()).addKeyValue(H("error"), (uint32_t) EINVAL).addKeyValue(H("error_detail"), rc);
        eb.send();
    }
}
//--------------------------------------------------------------------------------------------------------
void MqttGtw::onConnectSuccess(void* context, MQTTAsync_successData* response) {
    MqttGtw* mq = (MqttGtw*)context;
    mq->_connected = true;
    INFO("Successful connection");
    eb.reply(mq->_lastSrc,H("connect"),mq->id()).addKeyValue(EB_ERROR, (uint32_t) E_OK);
    eb.send();
    mq->wakeup();
}
//--------------------------------------------------------------------------------------------------------
void MqttGtw::onConnectFailure(void* context,
                               MQTTAsync_failureData* response) {
    MqttGtw* mq = (MqttGtw*)context;
    mq->_connected = false;
    WARN("Connect failed, rc %d", response ? response->code : 0);
    eb.reply(mq->_lastSrc,mq->id(),H("connect")).addKeyValue(H("error_detail"), response ? response->code : 0).addKeyValue(EB_ERROR, ECONNABORTED);
    eb.send();
    mq->wakeup();
}
//--------------------------------------------------------------------------------------------------------
void MqttGtw::onConnectionLost(void *context, char *cause) {
    MqttGtw* mq = (MqttGtw*)context;
    WARN("Connection lost, cause : %s ", cause==0 ? " unknown cause " : cause);
    mq->_connected = false;
    eb.event(mq->id(),H("disconnected"))
    .addKeyValue(H("error_detail"),
                 cause == 0 ? " unknown cause " : cause)
    .addKeyValue(EB_ERROR, (uint32_t) EINVAL);
    eb.send();
    mq->wakeup();

}
#include <string.h>
//--------------------------------------------------------------------------------------------------------
int MqttGtw::onMessage(void *context, char *topicName, int topicLen,
                       MQTTAsync_message *message) {
    MqttGtw* mq = (MqttGtw*)context;
    Cbor msg((uint8_t*) message->payload, message->payloadlen);
    Str topic((uint8_t*)topicName,topicLen);
    eb.event(mq->id(),H("published")).addKeyValue(H("topic"), topicName).addKeyValue(H("message"), msg).addKeyValue(
        H("qos"), message->qos).addKeyValue(H("retained"),
                                            message->retained);
    eb.send();
    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(topicName);
    mq->wakeup();
    return 1;
}
//--------------------------------------------------------------------------------------------------------
void MqttGtw::isConnected(Cbor& cbor) {
    cbor.getKeyValue(EB_SRC,_lastSrc);
    if ( _connected ) {
        eb.reply().addKeyValue(H("error"),0);
        eb.send();
    } else {
        eb.reply().addKeyValue(H("error"),ENOTCONN);
        eb.send();
    }
}
//--------------------------------------------------------------------------------------------------------
void MqttGtw::disconnect(Cbor& cbor) {
    cbor.getKeyValue(EB_SRC,_lastSrc);
    _connected = false;
    INFO("disconnect()");

    MQTTAsync_disconnectOptions disc_opts =
        MQTTAsync_disconnectOptions_initializer;
    disc_opts.onSuccess = onDisconnect;
    disc_opts.context=this;
    int rc = 0;
    if ((rc = MQTTAsync_disconnect(_client, &disc_opts))
            != MQTTASYNC_SUCCESS) {
        LOGF("Failed to start disconnect, return code %d", rc);
        eb.reply(_lastSrc,H("disconnect"),id()).addKeyValue(H("error_detail"), "disconnect fail").addKeyValue(H("error"), ENOTCONN);
        eb.send();
    }
    MQTTAsync_destroy(&_client);
}
//--------------------------------------------------------------------------------------------------------
void MqttGtw::onDisconnect(void* context, MQTTAsync_successData* response) {
    MqttGtw* mq = (MqttGtw*)context;
    mq->_connected = false;

    INFO("Successful disconnection");
    eb.reply(mq->_lastSrc,mq->id(),H("disconnect")).addKeyValue(EB_ERROR, E_OK);
    eb.send();
    mq->wakeup();
}
//--------------------------------------------------------------------------------------------------------
void MqttGtw::publish(Cbor& cbor) {
    cbor.getKeyValue(EB_SRC,_lastSrc);
    if (false) {
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
    if ( cbor.getKeyValue(H("topic"), topic)  &&     cbor.getKeyValue(H("message"), message)) {
        cbor.getKeyValue(H("qos"), qos);
        cbor.getKeyValue(H("retain"), retain);

        int rc = E_OK;
        opts.onSuccess = MqttGtw::onPublishSuccess;
        opts.onFailure = MqttGtw::onPublishFailure;
        opts.context = this;

        pubmsg.payload = message.data();
        pubmsg.payloadlen = message.length();
        pubmsg.qos = qos;
        pubmsg.retained = retain;
        pubmsg.msgid = _msgid++;

        if ((rc = MQTTAsync_sendMessage(_client, topic.c_str(), &pubmsg, &opts))
                != MQTTASYNC_SUCCESS) {
            LOGF("Failed MQTTAsync_sendMessage() =  %d", rc);
            eb.reply().addKeyValue(H("error_detail"), rc).addKeyValue(H("error"), (uint32_t) EAGAIN);
            eb.send();
        }
    } else {

        eb.reply().addKeyValue(H("error_detail"),strerror(EINVAL)).addKeyValue(H("error"), (uint32_t) EINVAL);
        eb.send();
    }
}
//--------------------------------------------------------------------------------------------------------
void MqttGtw::onPublishSuccess(void* context,
                               MQTTAsync_successData* response) {
    MqttGtw* mq = (MqttGtw*)context;

//	LOGF("Message with token value %d delivery confirmed", response->token);
    eb.reply(mq->_lastSrc,H("publish"),mq->id()).addKeyValue(H("error"),E_OK);
    eb.send();
    mq->wakeup();
}
//--------------------------------------------------------------------------------------------------------
void MqttGtw::onPublishFailure(void* context,
                               MQTTAsync_failureData* response) {
    MqttGtw* mq = (MqttGtw*)context;
    WARN("Publish failed, rc %d", response ? response->code : 0);

    eb.reply(mq->_lastSrc,H("publish"),mq->id())
    .addKeyValue(H("error"), response ? response->code : 0)
    .addKeyValue(H("error_detail"),
                 response->message ? "no detail" : response->message);
    eb.send();
    mq->wakeup();
}
//--------------------------------------------------------------------------------------------------------
void MqttGtw::subscribe(Cbor& cbor) {
    cbor.getKeyValue(EB_SRC,_lastSrc);
    if (!_connected) {
        eb.reply().addKeyValue(H("error"), ENOTCONN);
        return;
    }
    Str topic(60);
    int qos = 0;
    cbor.getKeyValue(H("topic"), topic);
    cbor.getKeyValue(H("qos"), qos);
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    INFO("Subscribing to topic %s for client %s using QoS%d",
         topic.c_str(), CLIENTID, qos);
    opts.onSuccess = onSubscribeSuccess;
    opts.onFailure = onSubscribeFailure;
    opts.context = this;
    int rc = E_OK;

    if ((rc = MQTTAsync_subscribe(_client, topic.c_str(), qos, &opts))
            != MQTTASYNC_SUCCESS) {
        LOGF("Failed to start subscribe, return code %d", rc);
        eb.reply().addKeyValue(H("error"), EAGAIN).addKeyValue(H("error_detail"), rc);;
        eb.send();
    }
}
//--------------------------------------------------------------------------------------------------------
void MqttGtw::onSubscribeSuccess(void* context, MQTTAsync_successData* response) {
    MqttGtw* mq = (MqttGtw*)context;
    INFO("Subscribe succeeded");
    eb.reply(mq->_lastSrc,H("subscribe"),mq->id()).addKeyValue(H("error"),  E_OK);
    eb.send();
    mq->wakeup();
}
//--------------------------------------------------------------------------------------------------------
void MqttGtw::onSubscribeFailure(void* context,
                                 MQTTAsync_failureData* response) {
    MqttGtw* mq = (MqttGtw*)context;
    WARN("Subscribe failed, rc %d", response ? response->code : 0);
    eb.reply(mq->_lastSrc,H("subscribe"),mq->id()).addKeyValue(H("error"), response ? response->code : 0);
    eb.send();
    mq->wakeup();
}
//--------------------------------------------------------------------------------------------------------
