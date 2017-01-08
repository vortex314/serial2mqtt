

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
#define __uid_t_defined
#include <iostream>
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
Uid uid(100);

Str str(300);
void logCbor(Cbor& cbor) {
    cbor.offset(0);
    uint32_t key;
    str.clear();
    Cbor::PackType ct;
    cbor.offset(0);
    while (cbor.hasData()) {
        cbor.get(key);
            if ( uid.label(key))
                str.append('"').append(uid.label(key)).append("\":");
            else
                str.append('"').append(key).append("\":");
        if (key == EB_DST || key == EB_SRC || key == EB_REQUEST || key==EB_REPLY || key==EB_EVENT ) {
            cbor.get(key);
            if ( uid.label(key))
                str.append('"').append(uid.label(key)).append("\"");
            else
                str.append('"').append(key).append("\"");

        } else {
            ct = cbor.tokenToString(str);
            if (ct == Cbor::P_BREAK || ct == Cbor::P_ERROR)
                break;
        }
        if (cbor.hasData())
            str << ",";
    };
    LOGF("%s", str.c_str());
}

struct
{
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

void poller(int serialFd, int tcpFd, uint64_t sleepTill)
{
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
    if(serialFd == 0 && tcpFd == 0)
    {
        usleep(1000);
        //        eb.publish(H("sys"),H("tick"));
    }
    else
    {

        // Wait up to 1000 msec.
        uint64_t delta = 5000;
        if(sleepTill > Sys::millis())
        {
            delta = sleepTill - Sys::millis();
        }
        else
        {
            DEBUG(" now : %llu  next timeout : %llu  ", Sys::millis(), sleepTill);
            for (Actor* cursor=Actor::first(); cursor; cursor=cursor->next())
            {
                if ( cursor->nextTimeout()==sleepTill )
                {
                    LOGF(" awaiting ACtor : %s",cursor->name());
                    break;
                }
            }
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
        if(waitTime == 0)
        {
            TRACE(" waited %ld/%ld msec.", waitTime, delta);
        }

        if(retval < 0)
        {
            WARN(" select() : error : %s (%d)", strerror(errno), errno);
        }
        else if(retval > 0)     // one of the fd was set
        {
            if(FD_ISSET(serialFd, &rfds))
            {
                int size = ::read(serialFd, buffer, sizeof(buffer));
                if(size > 0)
                {
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
                }
                else
                {
                    WARN("serial read error : %s (%d)", strerror(errno), errno);
                    eb.event(H("serial"), H("err"))
                    .addKeyValue(H("error"), errno)
                    .addKeyValue(H("error_msg"), strerror(errno));
                    eb.send();
                    serial.close();
                }
            }
            if(FD_ISSET(tcpFd, &rfds))
            {
                ::read(tcpFd, buffer, sizeof(buffer)); // empty event pipe
//               DEBUG(" wakeup ");
                // just return
                // eb.publish(H("tcp"),H("rxd"));
            }
            if(FD_ISSET(serialFd, &efds))
            {
                WARN("serial  error : %s (%d)", strerror(errno), errno);
                eb.event(H("serial"), H("err"))
                .addKeyValue(H("error"), errno)
                .addKeyValue(H("error_msg"), strerror(errno));
                eb.send();
            }
            if(FD_ISSET(tcpFd, &efds))
            {
                eb.event(H("tcp"), H("err"))
                .addKeyValue(H("error"), errno)
                .addKeyValue(H("error_msg"), strerror(errno));
                eb.send();
            }
        }
        else
        {
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

void loadOptions(int argc, char* argv[])
{
    int c;
    while((c = getopt(argc, argv, "h:p:d:b:l:m:")) != -1)
        switch(c)
        {
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

void SignalHandler(int signal_number)
{
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

void interceptAllSignals()
{
    signal(SIGFPE, SignalHandler);
    signal(SIGILL, SignalHandler);
    signal(SIGINT, SignalHandler);
    signal(SIGSEGV, SignalHandler);
    signal(SIGTERM, SignalHandler);
}

#include "Json.h"

#define EB_ROUTE H("Router")

uint16_t h(Str& str)
{
    uint32_t hh = FNV_OFFSET;
    str.offset(0);
    while(str.hasData())
    {
        hh = hh * FNV_PRIME;
        hh = hh ^ str.read();
    }
    return hh & 0xFFFF;
}
extern void logCbor(Cbor&);
extern char* hash2string(uint32_t h);
//_______________________________________________________________________________________________________________
//

/*
  if ( isRequest(actor->id(),H("status"))) {
        eb.reply()
        .addKeyValue(H("state"),actor->_state)
        .addKeyValue(H("timeout"),actor->_timeout)
        .addKeyValue(H("id"),actor->_id)
        .addKeyValue(H("line"),actor->_ptLine);
        */


//_______________________________________________________________________________________________________________
//
class Logger : public Actor
{

public:
    Logger()
        : Actor("Logger")
    {
    }
    void setup()
    {
    }
	void init(){
		
	}
#define CNT 100
    void onEvent(Cbor& msg)
    {
        if(eb.isRequest(0,H("log")))
        {
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
        }
        else eb.defaultHandler(this,msg);
    }
};

class Relay : public Actor
{
public:
    Relay():Actor("Relay")
    {
    }
    void setup()
    {
        eb.onDst(H("Relay")).subscribe(this);
    }
	void init(){
		
	}
    void onEvent(Cbor& msg)
    {
        eb.defaultHandler(this,msg);
    }
};

Relay relay;

Logger logger;
//_______________________________________________________________________________________________________________
//
class Sonar : public Actor
{
    uint32_t _counter;

public:
    Sonar()
        : Actor("Sonar")
    {
        _counter++;
    }

    void setup()
    {
        timeout(2000);
    }
	void init(){
		
	}

    void onEvent(Cbor& msg)
    {
        PT_BEGIN();
        timeout(2000);
        eb.request(H("Router"), H("subscribe"), H("Sonar")).addKeyValue(H("name"), "Sonar");
        eb.send();
        PT_YIELD_UNTIL(timeout());
        goto PINGING;

PINGING :
        {
            while(true)
            {
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

void slipSend(Cbor& cbor)
{
    slip.send(cbor);
}

int main(int argc, char* argv[])
{

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
    eb.onAny().subscribe([](Cbor& cbor)
    {
        if(Log.level() <= LogManager::LOG_DEBUG)
            logCbor(cbor);
    });
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
/*    eb.onRequest(H("mqtt"), H("publish")).subscribe(&mqttGtw, (MethodHandler)&MqttGtw::publish);
    eb.onRequest(H("mqtt"), H("subscribe")).subscribe(&mqttGtw, (MethodHandler)&MqttGtw::subscribe);
    eb.onRequest(H("mqtt"), H("connected")).subscribe(&mqttGtw, (MethodHandler)&MqttGtw::isConnected);
    eb.onRequest(H("mqtt")).subscribe(&mqttGtw); */


//   router.setup();
//   eb.onRemote().subscribe(&router, (MethodHandler)&Router::onPublish);

//    eb.onEvent(H("mqtt"), H("published")).subscribe(&router, (MethodHandler)&Router::onPublished);
//    eb.onEvent(H("mqtt"), H("disconnected")).subscribe(&router, (MethodHandler)&Router::onEvent);

//    echo.setup();
    //    eb.onRequest(H("Router"),H("subscribe")).subscribe(&router,(MethodHandler)&Router::onSubscribe);

    eb.onEvent(slip.id(), H("rxd")).subscribe([](Cbor& msg)   // put SLIP messages on EB
    {
        Cbor data(0);
        if(msg.mapKeyValue(H("data"), data))
        {
            eb.publish(data);
        }
    });
    eb.onSrc(H("mqtt")).subscribe(slipSend); // put some EB messages on SLIP
    eb.onRequest(H("Logger")).subscribe(&logger);

//    sonar.setup();
//    relay.setup();
    // push some downstream for test purpose



    while(1)
    {
        poller(serial.fd(), mqttGtw.fd(), Actor::lowestTimeout());
        eb.eventLoop();
    }
}
/*,
H"dst"),H("src"),H("request"),H("reply"),H("event"),H("error"),H("Actor"),H("bootTime"),H("clientId"),H("connect"),H("connected"),H("data"),H("disconnect"),H("disconnected"),H("#dst"),
H("#dst_device"),H("error"),H("error_msg"),H("#event"),H("#from"),H("host"),H("hostname"),H("id"),H("init"),H("keep_alive"),H("line"),H("message"),H("motor"),H("mqtt"),H("now"),
H("prefix"),H("props"),H("publish"),H("published"),H("register"),H("Relay"),H("#reply"),H("#request"),H("reset"),H("Router"),H("rxd"),H("set"),H("setup"),H("#src"),H("#src_device"),H("state"),
H("status"),H("subscribe"),H("system"),H("timeout"),H("topic"),H("upTime"),H("will_message"),H("will_topic"),
*/