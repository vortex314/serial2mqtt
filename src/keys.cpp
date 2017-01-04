#include <EventBus.h>

const char* hash2string(uint32_t hash)
{
    switch(hash)
    {
	case EB_DST :  return "dst";
    case EB_SRC :  return "src";
    case EB_EVENT :  return "event";
    case EB_REQUEST :  return "request";
    case EB_REPLY :  return "reply";
case H("Echo") : return "Echo";
case H("Logger") : return "Logger";
case H("Relay") : return "Relay";
case H("Router") : return "Router";
case H("Sonar") : return "Sonar";
case H("Tester") : return "Tester";
case H("clean_session") : return "clean_session";
case H("client_id") : return "client_id";
case H("closed") : return "closed";
case H("connect") : return "connect";
case H("connected") : return "connected";
case H("data") : return "data";
case H("disconnect") : return "disconnect";
case H("disconnected") : return "disconnected";
case H("err") : return "err";
case H("error") : return "error";
case H("error_detail") : return "error_detail";
case H("error_msg") : return "error_msg";
case H("host") : return "host";
case H("id") : return "id";
case H("keep_alive") : return "keep_alive";
case H("line") : return "line";
case H("log") : return "log";
case H("message") : return "message";
case H("method") : return "method";
case H("mqtt") : return "mqtt";
case H("name") : return "name";
case H("nr") : return "nr";
case H("object") : return "object";
case H("opened") : return "opened";
case H("password") : return "password";
case H("ping") : return "ping";
case H("port") : return "port";
case H("prefix") : return "prefix";
case H("publish") : return "publish";
case H("published") : return "published";
case H("qos") : return "qos";
case H("retain") : return "retain";
case H("retained") : return "retained";
case H("rxd") : return "rxd";
case H("serial") : return "serial";
case H("slip") : return "slip";
case H("state") : return "state";
case H("status") : return "status";
case H("subscribe") : return "subscribe";
case H("sys") : return "sys";
case H("tcp") : return "tcp";
case H("tick") : return "tick";
case H("time") : return "time";
case H("timeout") : return "timeout";
case H("topic") : return "topic";
case H("uint32_t") : return "uint32_t";
case H("user") : return "user";
case H("will_message") : return "will_message";
case H("will_qos") : return "will_qos";
case H("will_retain") : return "will_retain";
case H("will_topic") : return "will_topic";
default : {
		return 0;
        }
    }
}
