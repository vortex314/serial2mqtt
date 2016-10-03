#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include "MQTTAsync.h"
#include <Cbor.h>
#include <EventBus.h>


#define ADDRESS     "tcp://limero.ddns.net:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "stm32/alive"
#define PAYLOAD     "true"
#define QOS         1
#define TIMEOUT     10000L

class MqttClient
{
private:
    static MQTTAsync_token deliveredtoken;
    static MQTTAsync client;
    static int disc_finished ;
    static int subscribed ;
    static int finished ;
    static Str connection;
    static Str clientId;
    static Str user;
    static Str password;
    static Str willTopic;
    static Str willMessage;
    static int willQos;
    static bool willRetain;

public:

    static void onConnectionLost(void *context, char *cause);
    static int onMessage(void *context, char *topicName, int topicLen, MQTTAsync_message *message);
    static void onDisconnect(void* context, MQTTAsync_successData* response);
    static void onSubscribe(void* context, MQTTAsync_successData* response);
    static void onSubscribeFailure(void* context, MQTTAsync_failureData* response);
    static void onConnectFailure(void* context, MQTTAsync_failureData* response);
    static void onConnect(void* context, MQTTAsync_successData* response);
    static Erc connect(Str& connection,Str& willTopic,Str& willMessage,int willQos);
    static Erc disconnect();
    static void onSend(void* context, MQTTAsync_successData* response);
    static Erc publish(Str& topic,Bytes& message,int qos=0,bool retain=false);
    static Erc subscribe(Str& topic,int qos=0);

};




#endif // MQTTCLIENT_H
