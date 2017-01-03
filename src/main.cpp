#include <iostream>

/*
 * DEVICE <-> Gateway
 *
 * -> mqtt.connect,host,port,will.topic,will.message,will.qos,will.retain,clientId,user,password
 * <- mqtt.connack,error
 *
 * -> mqtt.publish,topic,message,qos,retain
 * <- mqtt.pubcomp
 *
 * -> mqtt.subscribe
 * <- mqtt.suback
 *
 * -> mqtt.unsubscribe
 * <- mqtt.unsuback
 *
 * -> mqtt.pingreq
 * <- mqtt.pingresp
 *
 * -> mqtt.disconnect
 */

using namespace std;
#include <cstdlib>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "Str.h"
#include <time.h>
//#include "Timer.h"
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
//#include "Sequence.h"
#include <unistd.h>
//#include "Thread.h"
// #include "Sequence.h"
#include "Serial.h"
#include <Actor.h>
#include <time.h>
#define TIMER_TICK 1000

#include <CborQueue.h>
#include <EventBus.h>
#include <Log.h>
#include <MqttGtw.h>
#include <SlipStream.h>

EventBus eb(10240, 1024);

struct {
    const char* host;
    uint16_t port;
    const char* prefix;
    uint32_t baudrate;
    const char* device;
    LogManager::LogLevel logLevel;

} context = { "pi3.local", 1883, "limero", 115200, "/dev/ttyACM0", LogManager::LOG_DEBUG };
Cbor mqttConfig(200);

Serial serial("/dev/ttyACM1");

//_______________________________________________________________________________________
//
// simulates RTOS generating events into queue : Timer::TICK,Serial::RXD,Serial::CONNECTED,...
//_______________________________________________________________________________________

void poller(int serialFd, int tcpFd, uint64_t sleepTill) {
    Cbor cbor(1024);
    Bytes bytes(1024);
    uint8_t buffer[1024];
    fd_set rfds;
    fd_set wfds;
    fd_set efds;
    struct timeval tv;
    int retval;
    uint64_t start = Sys::millis();
    //    uint64_t delta=1000;
    if(serialFd == 0 && tcpFd == 0) {
        usleep(1000);
        //        eb.publish(H("sys"),H("tick"));
    } else {

        // Wait up to 1000 msec.
        uint64_t delta = 5000;
        if(sleepTill > Sys::millis()) {
            delta = sleepTill - Sys::millis();
        } else {
            DEBUG(" now : %llu  next timeout : %llu  ", Sys::millis(), sleepTill);
            delta = 0;
        }
        tv.tv_sec = delta / 1000;
        tv.tv_usec = (delta * 1000) % 1000000;

        // Watch serialFd and tcpFd  to see when it has input.
        FD_ZERO(&rfds);
        FD_ZERO(&wfds);
        FD_ZERO(&efds);
        if(serialFd)
            FD_SET(serialFd, &rfds);
        if(tcpFd)
            FD_SET(tcpFd, &rfds);
        if(serialFd)
            FD_SET(serialFd, &efds);
        if(tcpFd)
            FD_SET(tcpFd, &efds);
        int maxFd = serialFd < tcpFd ? tcpFd : serialFd;
        maxFd += 1;

        start = Sys::millis();

        retval = select(maxFd, &rfds, NULL, &efds, &tv);

        uint64_t waitTime = Sys::millis() - start;
        if(waitTime == 0) {
            TRACE(" waited %ld/%ld msec.", waitTime, delta);
        }

        if(retval < 0) {
            WARN(" select() : error : %s (%d)", strerror(errno), errno);
        } else if(retval > 0) { // one of the fd was set
            if(FD_ISSET(serialFd, &rfds)) {
                int size = ::read(serialFd, buffer, sizeof(buffer));
                if(size > 0) {
                    //                   LOGF(" rxd Serial : %d ", size);
                    Str str(size * 3);
                    for(int i = 0; i < size; i++)
                        str.appendHex(buffer[i]);
                    //                    DEBUG("%s",str.c_str());
                    //					fprintf(stdout, "%s\n", str.c_str());
                    TRACE(" rxd [%d] : %s", size, str.c_str());
                    for(int i = 0; i < size; i++)
                        bytes.write(buffer[i]);
                    eb.event(H("serial"), H("rxd")).addKeyValue(H("data"), bytes);
                    eb.send();
                } else {
                    WARN("serial read error : %s (%d)", strerror(errno), errno);
                    eb.event(H("serial"), H("err"))
                    .addKeyValue(H("error"), errno)
                    .addKeyValue(H("error_msg"), strerror(errno));
                    eb.send();
                    serial.close();
                }
            }
            if(FD_ISSET(tcpFd, &rfds)) {
                ::read(tcpFd, buffer, sizeof(buffer)); // empty event pipe
                DEBUG(" wakeup ");
                // just return
                // eb.publish(H("tcp"),H("rxd"));
            }
            if(FD_ISSET(serialFd, &efds)) {
                WARN("serial  error : %s (%d)", strerror(errno), errno);
                eb.event(H("serial"), H("err"))
                .addKeyValue(H("error"), errno)
                .addKeyValue(H("error_msg"), strerror(errno));
                eb.send();
            }
            if(FD_ISSET(tcpFd, &efds)) {
                eb.event(H("tcp"), H("err"))
                .addKeyValue(H("error"), errno)
                .addKeyValue(H("error_msg"), strerror(errno));
                eb.send();
            }
        } else {
            TRACE(" timeout %llu", Sys::millis());
            // TODO publish TIMER_TICK
            //           eb.publish(H("sys"),H("tick"));
        }
    }
}

/*_______________________________________________________________________________

 loadOptions  role :
 - parse commandline otions
 h : host of mqtt server
 p : port
 d : the serial device "/dev/ttyACM*"
 b : the baudrate set ( only usefull for a serial2serial box or a real serial port )
 ________________________________________________________________________________*/

#include "Log.h"
#include "Tcp.h"

void loadOptions(int argc, char* argv[]) {
    int c;
    while((c = getopt(argc, argv, "h:p:d:b:l:m:")) != -1)
        switch(c) {
        case 'h':
            mqttConfig.addKeyValue(H("host"), optarg);
            break;
        case 'p':
            mqttConfig.addKeyValue(H("host"), atoi(optarg));
            break;
        case 'm':
            context.prefix = optarg;
            break;
        case 'd':
            context.device = optarg;
            break;
        case 'b':
            context.baudrate = atoi(optarg);
            break;
        case 'l':
            if(strcmp(optarg, "DEBUG") == 0)
                context.logLevel = LogManager::LOG_DEBUG;
            if(strcmp(optarg, "INFO") == 0)
                context.logLevel = LogManager::LOG_INFO;
            if(strcmp(optarg, "WARN") == 0)
                context.logLevel = LogManager::LOG_WARN;
            if(strcmp(optarg, "ERROR") == 0)
                context.logLevel = LogManager::LOG_ERROR;
            if(strcmp(optarg, "FATAL") == 0)
                context.logLevel = LogManager::LOG_FATAL;
            break;
        case '?':
            if(optopt == 'c')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if(isprint(optopt))
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
            return;
        default:
            abort();
            break;
        }
}

#include <execinfo.h>
#include <signal.h>

void SignalHandler(int signal_number) {
    void* array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:%s \n", signal_number, strsignal(signal_number));
    fprintf(stderr, "Error: errno %d:%s \n", errno, strerror(errno));
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

void interceptAllSignals() {
    signal(SIGFPE, SignalHandler);
    signal(SIGILL, SignalHandler);
    signal(SIGINT, SignalHandler);
    signal(SIGSEGV, SignalHandler);
    signal(SIGTERM, SignalHandler);
}

#include "Json.h"
#include "MqttClient.h"

#define EB_ROUTE H("Router")

uint16_t h(Str& str) {
    uint32_t hh = FNV_OFFSET;
    str.offset(0);
    while(str.hasData()) {
        hh = hh * FNV_PRIME;
        hh = hh ^ str.read();
    }
    return hh & 0xFFFF;
}
extern void logCbor(Cbor&);
extern char* hash2string(uint32_t h);
//_______________________________________________________________________________________________________________
//
class Id {
    char* _name;
    uint16_t _id;
    static Id* _first;
    Id* _next;
public:
    Id(const char* name,uint16_t id) {
        _name = (char*)malloc(strlen(name));
        strcpy(_name,name);
        _id=id;
        _next=0;
        if (first() == 0) {
            setFirst(this);
        } else {
            last()->setNext(this);
        }
    }
    Id* last() {
        Id* cursor = first();
        while (cursor->_next) {
            cursor = cursor->next();
        }
        return cursor;
    }

    static Id* first() {
        return Id::_first;
    }
    Id* next() {
        return _next;
    }
    void setNext(Id* a) {
        _next = a;
    }
    static void setFirst(Id* f) {
        _first = f;
    }

    static char* findById(uint16_t h) {
        char *ptr=0;
        ptr=hash2string(h);
        if ( ptr ) return ptr;
        if ( first()==0) return 0;
        for (Id* cur=first(); cur; cur=cur->next()) {
            if ( cur->_id == h ) return cur->_name;
        }
        return 0;
    }

};
/*
  if ( isRequest(actor->id(),H("status"))) {
        eb.reply()
        .addKeyValue(H("state"),actor->_state)
        .addKeyValue(H("timeout"),actor->_timeout)
        .addKeyValue(H("id"),actor->_id)
        .addKeyValue(H("line"),actor->_ptLine);
        */
Id* Id::_first=0;
//_______________________________________________________________________________________________________________
//
class Router : public Actor {
    Str _topic;
    Json _message;
    Actor* _actor;
    Str _name;

public:
    Router()
        : Actor("Router")
        , _topic(30)
        , _message(100)
        , _name(20) {
    }
    void setup() {
        timeout(2000);
        eb.onDst(id()).subscribe(this);
    }
#define CNT 100
    bool addHeader(Json& json, Cbor& cbor, uint16_t key) {
        uint16_t v;
        if(cbor.getKeyValue(key, v)) {
            json.addKey(hash2string(key));
            json.add(Id::findById(v));
            return true;
        }
        return false;
    }

    bool addTopic(Str& topic, Cbor& cbor, uint16_t key) {
        uint16_t v;
        if(cbor.getKeyValue(key, v)) {
            if(topic.length())
                topic.append('/');
            char* nm = Id::findById(v);
            if ( nm )topic.append(nm);
            else topic.append(v);
            return true;
        }
        return false;
    }

    bool isHeaderField(uint16_t key) {
        if(key == EB_SRC || key == EB_DST || key == EB_REQUEST || key == EB_REPLY || key == EB_EVENT)
            return true;
        return false;
    }
    int nextHash(Str& str) {
        Str field(20);
        while(str.hasData()) {
            uint8_t ch = str.read();
            if(ch == '/')
                break;
            field.append((char)ch);
        }
        if(field.length() == 0)
            return 0;
        uint16_t hsh=h(field);
        if ( Id::findById(hsh)==0)  new Id(field.c_str(),hsh);
        return hsh;
    }

    void jsonToCbor(Cbor& cbor, Json& json) {
        if(json.parse() != E_OK)
            return;
        Str key(10), value(20);
        json.rewind();
        while(true) {
            if(json.get(key)) {
                cbor.addKey(h(key));
                if(key.startsWith("#")) { // ENUM hash field
                    json.get(value);
                    cbor.add(h(value));
                } else if(key.startsWith("$")) { // HEX string field
                    Bytes bytes(100);
                    json.get(bytes);
                } else if ( json.getType() == Json::JSON_STRING ) {
                    Str str(100);
                    json.get(str);
                    cbor.add(str);
                } else if(json.getType() == Json::JSON_NUMBER) {
                    int64_t d;
                    json.get(d);
                    cbor.add(d);
                } else if ( json.getType() == Json::JSON_BOOL ) {
                    bool flag;
                    json.get(flag);
                    cbor.add(flag);
                } else {
                    cbor.addNull(); // just to avoid map index lost
                }
            } else
                break;
        }
    }

    void ebToMqtt(Str& topic, Json& json, Cbor& cbor) {
        topic.clear();
        json.clear();
        if(cbor.gotoKey(EB_REQUEST)) {
            addTopic(topic, cbor, EB_DST);
            topic.append("/request");
            addTopic(topic, cbor, EB_REQUEST);
            addTopic(topic, cbor, EB_SRC);
        }
        if(cbor.gotoKey(EB_REPLY)) {
            addTopic(topic, cbor, EB_SRC);
            topic.append("/reply");
            addTopic(topic, cbor, EB_DST);
            addTopic(topic, cbor, EB_REPLY);
        }
        if(cbor.gotoKey(EB_EVENT)) {
            addTopic(topic, cbor, EB_SRC);
            topic.append("/event");
            addTopic(topic, cbor, EB_EVENT);
        }
        Cbor::PackType type;
        //        Cbor::CborVariant variant;
        cbor.offset(0);
        uint16_t key;
        while(cbor.hasData()) {
            if(cbor.get(key)) {
                if(isHeaderField(key)) {
                    cbor.skipToken();
                } else {
                    char* name=hash2string(key);
                    if ( name)
                        json.addKey(name);
                    else {
                        Str str(10);
                        str.append(key);
                        json.addKey(str.c_str());
                    }
                    type = cbor.tokenToString(json);
                }
            }
        }
    }
    void onEvent(Cbor& msg) {
        PT_BEGIN();
DISCONNECTED : {
            while(true) {
                timeout(2000);
                eb.request(H("mqtt"), H("connected"), H("Router"));
                eb.send();
                PT_YIELD_UNTIL(timeout() || eb.isReply(H("mqtt"), H("connected")));
                if(eb.isReplyCorrect(H("mqtt"), H("connected"))) {
                    goto CONNECTED;
                }
                timeout(2000);
                PT_YIELD_UNTIL(timeout());
            }
        }
CONNECTED : {
            for(_actor = Actor::first(); _actor; _actor = _actor->next()) {
                _name = _actor->name();

                _topic = _name;
                _topic += "/request/#";
                eb.request(H("mqtt"), H("subscribe"), H("Router")).addKeyValue(H("topic"), _topic);
                eb.send();
                timeout(1000);
                PT_YIELD_UNTIL(eb.isReplyCorrect(H("mqtt"), H("subscribe")) || timeout());
                if(timeout())
                    goto DISCONNECTED;
                _topic = _name;
                _topic += "/reply/#";
                eb.request(H("mqtt"), H("subscribe"), H("Router")).addKeyValue(H("topic"), _topic);
                eb.send();
                timeout(1000);
                PT_YIELD_UNTIL(eb.isReplyCorrect(H("mqtt"), H("subscribe")) || timeout());
                if(timeout())
                    goto DISCONNECTED;
            }
            goto SLEEPING;
        }
SLEEPING : {
            while(true) {
                for(_actor = Actor::first(); _actor; _actor = _actor->next()) {
                    _name = _actor->name();
                    _topic = _name;
                    _topic += "/event";
                    _topic += "/alive";
                    _message.clear();
                    _message.add(true);
                    eb.request(H("mqtt"), H("publish"), H("Router"))
                    .addKeyValue(H("topic"), _topic)
                    .addKeyValue(H("message"), _message);
                    eb.send();
                    timeout(2000);
                    PT_YIELD_UNTIL(eb.isReplyCorrect(H("mqtt"), H("publish")) || timeout());
                    if(timeout())
                        goto DISCONNECTED;
                    timeout(5000);
                    PT_YIELD_UNTIL(timeout());
                }
                goto IDLE;
            }
        }
IDLE : {
            while(true) {
                goto DISCONNECTED;
                timeout(5000);
                PT_YIELD_UNTIL(timeout() || eb.isEvent(H("mqtt"),H("disconnected")));
                if ( eb.isEvent(H("mqtt"),H("disconnected"))) goto DISCONNECTED;
            }

        }
        PT_END();
    }
    void onPublish(Cbor& msg) {

        ebToMqtt(_topic, _message, msg);
        eb.request(H("mqtt"), H("publish"), H("Router"))
        .addKeyValue(H("topic"), _topic)
        .addKeyValue(H("message"), _message);
        eb.send();
    }

    void onPublished(Cbor& msg) {
        if(msg.getKeyValue(H("topic"), _topic) && msg.getKeyValue(H("message"), (Bytes&)_message)) {
            uint16_t field[4];
            int i = 0;
            uint16_t v;
            _topic.offset(0);
            while((v = nextHash(_topic)) && i < 4) {
                field[i++] = v;
            }
            if(field[1] == EB_REQUEST) {
                Cbor& req = eb.request(field[0], field[2], field[3]);
                jsonToCbor(req, _message);
                eb.send();
            } else if(field[1] == EB_REPLY) {
                Cbor& repl = eb.reply(field[0], field[2], field[3]);
                jsonToCbor(repl, _message);
                eb.send();
            } else if(field[1] == EB_EVENT) {
                Cbor& ev = eb.event(field[0], field[2]);
                jsonToCbor(ev, _message);
                eb.send();
            } else {
                WARN(" unknown topic : %s ",_topic.c_str());
            }
        }
    }
};

//Router router;

//_______________________________________________________________________________________________________________
//
class Echo : public Actor {
    uint32_t _counter;

public:
    Echo()
        : Actor("Echo") {
        _counter++;
    }
    void setup() {
        eb.onDst(id()).subscribe(this);
    }
#define CNT 100
    void onEvent(Cbor& msg) {
        static uint32_t start;
        if ( eb.isRequest(id(),H("ping"))) {
            if(_counter % CNT == 0) {
                INFO(" request-reply %d msg/sec", (_counter * 1000) / (Sys::millis() - start));
                start = Sys::millis();
                _counter = 0;
            }

            Cbor& repl = eb.reply();
            uint32_t value;
            if(msg.getKeyValue(H("nr"), value)) {
                repl.addKeyValue(H("nr"), value);
            }
            eb.send();
            _counter++;
        } else eb.defaultHandler(this);
    }
};

Echo echo;
//_______________________________________________________________________________________________________________
//
class Logger : public Actor {

public:
    Logger()
        : Actor("Logger") {
    }
    void setup() {
    }
#define CNT 100
    void onEvent(Cbor& msg) {
        if(eb.isRequest(0,H("log"))) {
            Str host(20);
            uint64_t time;
            Str object(20);
            Str method(20);
            Str line(100);
            host = "<host>";
            time = 0;
            object = "<object>";
            method = "<method>";
            line = "-";
            msg.getKeyValue(H("time"), time);
            msg.getKeyValue(H("host"), host);
            msg.getKeyValue(H("object"), object);
            msg.getKeyValue(H("method"), method);
            msg.getKeyValue(H("line"), line);
            fprintf(stderr, "%s \n", line.c_str());
            //        fprintf(stderr,"%llu | %s | %s:%s %s
            //        \n",time,host.c_str(),object.c_str(),method.c_str(),line.c_str());
        } else eb.defaultHandler(this);
    }
};

class Relay : public Actor {
public:
    Relay():Actor("Relay") {
    }
    void setup() {
        eb.onDst(H("Relay")).subscribe(this);
    }
    void onEvent(Cbor& msg) {
        eb.defaultHandler(this);
    }
};

Relay relay;

Logger logger;
//_______________________________________________________________________________________________________________
//
class Sonar : public Actor {
    uint32_t _counter;

public:
    Sonar()
        : Actor("Sonar") {
        _counter++;
    }

    void setup() {
        timeout(2000);
    }

    void onEvent(Cbor& msg) {
        PT_BEGIN();
        timeout(2000);
        eb.request(H("Router"), H("subscribe"), H("Sonar")).addKeyValue(H("name"), "Sonar");
        eb.send();
        PT_YIELD_UNTIL(timeout());
        goto PINGING;

PINGING : {
            while(true) {
                timeout(2000);
                PT_YIELD_UNTIL(timeout());
//               eb.request(H("Echo"), H("ping"), H("Sonar")).addKeyValue(H("nr"), _counter++);
//               eb.send();
//               PT_YIELD_UNTIL(timeout());
            }
        }

        PT_END();
    }
};

Sonar sonar;

//_______________________________________________________________________________________________________________
//

//_______________________________________________________________________________________________________________
//
uint16_t tester = H("Tester");
uint16_t uin = H("uint32_t");

SlipStream slip(1024, serial);

// MqttClient Mqtt;
MqttGtw mqttGtw;

void slipSend(Cbor& cbor) {
    slip.send(cbor);
}

int main(int argc, char* argv[]) {

    INFO("Start %s version : %s %s", argv[0], __DATE__, __TIME__);
    LOGF(" H('sys') : %d   H('timeout')=%d", H("sys"), H("timeout"));
    LOGF(" EB_REQUEST %d EB_DST %d EB_SRC %d ",EB_REQUEST,EB_DST,EB_SRC);
    static_assert(H("timeout") == 16294, " timout hash incorrect");
    Str tim("timeout");
    if(h(tim) != 16294)
        exit(-1);

    loadOptions(argc, argv);
    Log.level(context.logLevel);
    //    Mqtt.loadConfig(mqttConfig);
    mqttGtw.loadConfig(mqttConfig);
    interceptAllSignals();

    serial.setDevice(context.device);
    serial.setBaudrate(context.baudrate);
    serial.id(H("serial"));
    serial.setup();

    slip.src(serial.id());
    slip.id(H("slip"));
    slip.setup();

    //    Mqtt.setup();
    mqttGtw.setup();
    eb.onRequest(H("mqtt"), H("publish")).subscribe(&mqttGtw, (MethodHandler)&MqttGtw::publish);
    eb.onRequest(H("mqtt"), H("subscribe")).subscribe(&mqttGtw, (MethodHandler)&MqttGtw::subscribe);
    eb.onRequest(H("mqtt"), H("connected")).subscribe(&mqttGtw, (MethodHandler)&MqttGtw::connected);


 //   router.setup();
 //   eb.onRemote().subscribe(&router, (MethodHandler)&Router::onPublish);

//    eb.onEvent(H("mqtt"), H("published")).subscribe(&router, (MethodHandler)&Router::onPublished);
//    eb.onEvent(H("mqtt"), H("disconnected")).subscribe(&router, (MethodHandler)&Router::onEvent);

    echo.setup();
    //    eb.onRequest(H("Router"),H("subscribe")).subscribe(&router,(MethodHandler)&Router::onSubscribe);

    eb.onEvent(slip.id(), H("rxd")).subscribe([](Cbor& msg) { // put SLIP messages on EB
        Cbor data(0);
        if(msg.mapKeyValue(H("data"), data)) {
            eb.publish(data);
        }
    });
    eb.onSrc(H("mqtt")).subscribe(slipSend); // put some EB messages on SLIP
    eb.onRequest(H("link")).subscribe(slipSend);
    eb.onReply(0, H("ping")).subscribe(slipSend);

    eb.onRequest(H("mqtt")).subscribe(&mqttGtw);
    eb.onRequest(H("Logger")).subscribe(&logger);

    sonar.setup();
    relay.setup();
    // push some downstream for test purpose

    eb.onAny().subscribe([](Cbor& cbor) {
        if(Log.level() <= LogManager::LOG_DEBUG)
            logCbor(cbor);
    });

    while(1) {
        poller(serial.fd(), mqttGtw.fd(), Actor::lowestTimeout());
        eb.eventLoop();
    }
}
