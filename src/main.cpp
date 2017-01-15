

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
#include "main_labels.h"

EventBus eb(10240, 1024);
Uid uid(100);
Log logger(300);

Str str(300);
void logCbor(Cbor& cbor)
{
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

struct {
	const char* host;
	uint16_t port;
	const char* prefix;
	uint32_t baudrate;
	const char* device;
	Log::LogLevel logLevel;

} context = { "pi3.local", 1883, "limero", 115200, "/dev/ttyACM0", Log::LOG_DEBUG };
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
			for (Actor* cursor=Actor::first(); cursor; cursor=cursor->next()) {
				if ( cursor->nextTimeout()==sleepTill ) {
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
//               DEBUG(" wakeup ");
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

void loadOptions(int argc, char* argv[])
{
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
				context.logLevel = Log::LOG_DEBUG;
			if(strcmp(optarg, "INFO") == 0)
				context.logLevel = Log::LOG_INFO;
			if(strcmp(optarg, "WARN") == 0)
				context.logLevel = Log::LOG_WARN;
			if(strcmp(optarg, "ERROR") == 0)
				context.logLevel = Log::LOG_ERROR;
			if(strcmp(optarg, "FATAL") == 0)
				context.logLevel = Log::LOG_FATAL;
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


extern void logCbor(Cbor&);

//_______________________________________________________________________________________________________________
//

//_______________________________________________________________________________________________________________
//


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
	LOGF(" to serial port ");
	slip.send(cbor);
}

int main(int argc, char* argv[])
{

	INFO("Start %s version : %s %s", argv[0], __DATE__, __TIME__);
	static_assert(H("timeout") == 16294, " timout hash incorrect");


	eb.setup();
	uid.add(labels,LABEL_COUNT);
	loadOptions(argc, argv);
	logger.level(context.logLevel);
	//    Mqtt.loadConfig(mqttConfig);
	mqttGtw.loadConfig(mqttConfig);
	
	eb.onAny().subscribe([](Cbor& cbor) {
		if(logger.level() <= Log::LOG_DEBUG)
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
	mqttGtw.setup();
	
	eb.onEvent(slip.id(), H("rxd")).subscribe([](Cbor& msg) { // put SLIP messages on EB
		Cbor data(0);
		if(msg.mapKeyValue(H("data"), data)) {
			LOGF(" from serial port ");

			eb.publish(data);
		}
	});
	eb.onSrc(H("mqtt")).subscribe(slipSend); // put some EB messages on SLIP

	while(1) {
		poller(serial.fd(), mqttGtw.fd(), Actor::lowestTimeout());
		eb.eventLoop();
	}
}

