#include <EventBus.h>

const char* hash2string(uint32_t hash)
{
    switch(hash)
    {
	case 0 :  return "type";
case H("clean_session") : return "clean_session";
case H("client_id") : return "client_id";
case H("data") : return "data";
case H("error") : return "error";
case H("error_detail") : return "error_detail";
case H("error_message") : return "error_message";
case H("host") : return "host";
case H("keep_alive") : return "keep_alive";
case H("link.ping") : return "link.ping";
case H("link.pong") : return "link.pong";
case H("message") : return "message";
case H("mqtt.connack") : return "mqtt.connack";
case H("mqtt.connect") : return "mqtt.connect";
case H("mqtt.disconnack") : return "mqtt.disconnack";
case H("mqtt.disconnect") : return "mqtt.disconnect";
case H("mqtt.puback") : return "mqtt.puback";
case H("mqtt.publish") : return "mqtt.publish";
case H("mqtt.published") : return "mqtt.published";
case H("mqtt.send") : return "mqtt.send";
case H("mqtt.suback") : return "mqtt.suback";
case H("mqtt.subscribe") : return "mqtt.subscribe";
case H("password") : return "password";
case H("port") : return "port";
case H("qos") : return "qos";
case H("retain") : return "retain";
case H("retained") : return "retained";
case H("sys.tick") : return "sys.tick";
case H("tcp.err") : return "tcp.err";
case H("tcp.rxd") : return "tcp.rxd";
case H("timeout") : return "timeout";
case H("topic") : return "topic";
case H("usb.err") : return "usb.err";
case H("usb.rxd") : return "usb.rxd";
case H("user") : return "user";
case H("will_message") : return "will_message";
case H("will_qos") : return "will_qos";
case H("will_retain") : return "will_retain";
case H("will_topic") : return "will_topic";
default :
        return "UNDEFINED";
    }
}
