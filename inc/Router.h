/*
 * Router.h
 *
 *  Created on: 3-jan.-2017
 *      Author: lieven2
 */

#ifndef ROUTER_H_
#define ROUTER_H_

#include <Json.h>
#include <Cbor.h>
#include <EventBus.h>
#include <Uid.h>


//_______________________________________________________________________________________________________________
//
class Router: public Actor {
	Str _topic;
	Json _message;
	Actor* _actor;
	Str _name;

public:
	Router();
	void setup();
	bool addHeader(Json& json, Cbor& cbor, uint16_t key);
	bool addTopic(Str& topic, Cbor& cbor, uint16_t key);
	bool isHeaderField(uint16_t key);
	int nextHash(Str& str);
	void jsonToCbor(Cbor& cbor, Json& json);
	void cborToMqtt(Str& topic, Json& json, Cbor& cbor);
	void onEvent(Cbor& msg);
	void ebToMqtt(Cbor& msg);
	void mqttToEb(Cbor& msg);
};

#endif /* ROUTER_H_ */
