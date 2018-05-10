#include "Serial2Mqtt.h"

using std::string;
using std::vector;

typedef struct {
    uint32_t baudrate;
    uint32_t symbol;
} BAUDRATE;

BAUDRATE BAUDRATE_TABLE[] = { { 50, B50 }, { 75, B75 }, { 110, B110 }, { 134, B134 }, //
    { 150, B150 }, { 200, B200 }, { 300, B300 }, { 600, B600 }, {	1200,	B1200	},
    { 1800, B1800 }, { 2400, B2400 }, { 4800, B4800 }, { 9600, B9600 }, //
    { 19200, B19200 }, { 38400, B38400 }, { 57600, B57600 }, //
    { 115200, B115200 }, { 230400, B230400 }, { 460800, B460800 }, //
    { 500000, B500000 }, { 576000, B576000 }, { 921600, B921600 }, //
    { 1000000, B1000000 }, { 1152000, B1152000 }, { 1500000, B1500000 }, //
    { 2000000, B2000000 }, { 2500000, B2500000 }, { 3000000, B3000000 }, //
    { 3500000, B3500000 }, { 4000000, B4000000 }
};

const char* signalString[]= {   "PIPE_ERROR",
                                "SERIAL_CONNECT",
                                "SERIAL_DISCONNECT",
                                "SERIAL_RXD",
                                "SERIAL_ERROR",
                                "MQTT_CONNECT_SUCCESS",
                                "MQTT_CONNECT_FAIL",
                                "MQTT_SUBSCRIBE_SUCCESS",
                                "MQTT_SUBSCRIBE_FAIL",
                                "MQTT_PUBLISH_SUCCESS",
                                "MQTT_PUBLISH_FAIL",
                                "MQTT_DISCONNECTED",
                                "MQTT_MESSAGE_RECEIVED",
                                "MQTT_ERROR",
                                "TIMEOUT"
                            } ;

Serial2Mqtt::Serial2Mqtt() : _serialBuffer(10240)
{
}

Serial2Mqtt::~Serial2Mqtt()
{
}

void Serial2Mqtt::setConfig(Config config)
{
    _config=config;
}

int baudSymbol(uint32_t br)
{
    for (uint32_t i = 0; i < sizeof(BAUDRATE_TABLE) / sizeof(BAUDRATE); i++)
        if (BAUDRATE_TABLE[i].baudrate == br)
            return BAUDRATE_TABLE[i].symbol;
    INFO("connect: baudrate %d  not found, default to 115200.", br);
    return B115200;
}

void Serial2Mqtt::setSerialPort(std::string port)
{
    _serialPort=port;
    _serialPortShort = _serialPort.substr(8,_serialPort.length()-8);
    _serial2mqttDevice = Sys::hostname();
    _serial2mqttDevice += "."+_serialPortShort;
    _mqttDevice = _serial2mqttDevice;
    _mqttSubscribedTo = "dst/"+_serial2mqttDevice+"/#";
    _mqttProgrammerTopic = "dst/"+_serial2mqttDevice+"/serial2mqtt/flash";
    _mqttClientId = _mqttDevice+std::to_string(getpid());
    _config.get("willTopic",_mqttWillTopic,"src/"+_serial2mqttDevice+"/serial2mqtt/alive");
}

void Serial2Mqtt::init()
{
    _config.setNameSpace("programmer");
    _config.get("file",_binFile,"image.binary");
    _config.get("exec",_programCommand,"echo no command defined ");
    
    _config.setNameSpace("serial");
    _config.get("baudrate",_serialBaudrate,115200);
    
    _config.setNameSpace("mqtt");
    _config.get("port",_mqttPort,1883);
    _config.get("host",_mqttHost,"test.mosquitto.org");
    _config.get("keepAliveInterval",_mqttKeepAliveInterval,5);
    _config.get("willMessage",_mqttWillMessage,"false");
    _mqttWillQos=0;
    _mqttWillRetained=false;


    if (pipe(_signalFd) < 0)        INFO("Failed to create pipe: %s (%d)", strerror(errno), errno);

    if (fcntl(_signalFd[0], F_SETFL, O_NONBLOCK) < 0)
        INFO("Failed to set pipe non-blocking mode: %s (%d)", strerror(errno), errno);
}

void Serial2Mqtt::threadFunction(void* pv)
{
    run();
}

void Serial2Mqtt::run()
{
    std::string line;
    Timer mqttTimer;
    Timer serialTimer;

    mqttTimer.atInterval(5000).doThis([this]() {
        if ( !_mqttConnected) {
            mqttConnect();
        } else {
            mqttPublish("src/"+_serial2mqttDevice+"/serial2mqtt/alive","true",0,0);
            mqttPublish("src/"+_serial2mqttDevice+"/serial2mqtt/device",_mqttDevice,0,0);
        }
    });
    serialTimer.atDelta(5000).doThis([this]() {
        if ( _serialConnected ) {
            serialDisconnect();
            WARN(" disconecting serial no new data received in %d msec",5000);
        }

    });
    mqttConnect();

    while(true) {
        while(true) {
            Signal s = waitSignal(1000);
            DEBUG("signal = %s",signalString[s]);
            mqttTimer.check();
            serialTimer.check();
            switch(s) {
            case TIMEOUT: {
                if ( !_serialConnected) {
                    serialConnect();
                }
                break;
            }
            case SERIAL_RXD : {
                serialRxd();
                if ( serialGetLine(line)) {
                    serialTimer.atDelta(5000);
                    serialHandleLine(line);
                    line.clear();
                }
                break;
            }
            case SERIAL_ERROR : {
                serialDisconnect();
                break;
            }
            case MQTT_CONNECT_SUCCESS : {
                INFO("MQTT_CONNECT_SUCCESS %s ",_serialPortShort.c_str());
                _mqttConnected=true;
                mqttSubscribe(_mqttSubscribedTo);
                break;
            }
            case MQTT_CONNECT_FAIL : {
                WARN("MQTT_CONNECT_FAIL %s ",_serialPortShort.c_str());
                _mqttConnected=false;
                break;
            }
            case MQTT_DISCONNECTED: {
                WARN("MQTT_DISCONNECTED %s ",_serialPortShort.c_str());
                _mqttConnected=false;
                break;
            }
            case MQTT_SUBSCRIBE_SUCCESS: {
                INFO("MQTT_SUBSCRIBE_SUCCESS %s ",_serialPortShort.c_str());
                break;
            }
            case MQTT_SUBSCRIBE_FAIL: {
                WARN("MQTT_SUBSCRIBE_FAIL %s ",_serialPortShort.c_str());
                mqttDisconnect();
                break;
            }
            case MQTT_ERROR : {
                WARN("MQTT_ERROR %s ",_serialPortShort.c_str());
                break;
            }
            case PIPE_ERROR : {
                WARN("PIPE_ERROR %s ",_serialPortShort.c_str());
                break;
            }
            case MQTT_PUBLISH_SUCCESS: {
                break;
            }
            case MQTT_MESSAGE_RECEIVED: {
                break;
            }
            default: {
                WARN("received signal '%c' for %s ",s,_serialPortShort.c_str());
            }
            }
        }
    }
}


void Serial2Mqtt::signal(uint8_t m)
{
    if ( write(_signalFd[1],(void*)&m,1) < 1 ) {
        INFO("Failed to write pipe: %s (%d)", strerror(errno), errno);
    }
//	INFO(" signal '%c' ",m);
}


Serial2Mqtt::Signal Serial2Mqtt::waitSignal(uint32_t timeout)
{
    Signal returnSignal=TIMEOUT;
    Bytes bytes(1024);
    uint8_t buffer[1024];
    fd_set rfds;
    fd_set wfds;
    fd_set efds;
    struct timeval tv;
    int retval;
    uint64_t start = Sys::millis();
    //    uint64_t delta=1000;
    if( _serialFd == 0 ) {
        usleep(timeout*1000);
    } else {

        // Wait up to 1000 msec.
        uint64_t delta = timeout;

        tv.tv_sec = delta / 1000;
        tv.tv_usec = (delta * 1000) % 1000000;

        // Watch serialFd and tcpFd  to see when it has input.
        FD_ZERO(&rfds);
        FD_ZERO(&wfds);
        FD_ZERO(&efds);
        if(_serialFd) {
            FD_SET(_serialFd, &rfds);
            FD_SET(_serialFd, &efds);
        }
        if(_signalFd[0]) {
            FD_SET(_signalFd[0], &rfds);
            FD_SET(_signalFd[0], &efds);
        }
        int maxFd = _serialFd < _signalFd[0] ? _signalFd[0] : _serialFd;
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
            if(FD_ISSET(_serialFd, &rfds)) {
                return SERIAL_RXD;
            }
            if(FD_ISSET(_signalFd[0], &rfds)) {
                ::read(_signalFd[0], buffer,1); // read 1 event
                return (Signal)buffer[0];
            }
            if(FD_ISSET(_serialFd, &efds)) {
                WARN("serial  error : %s (%d)", strerror(errno), errno);
                return SERIAL_ERROR;
            }
            if(FD_ISSET(_signalFd[0], &efds)) {
                WARN("pipe  error : %s (%d)", strerror(errno), errno);
                return PIPE_ERROR;
            }
        } else {
            TRACE(" timeout %llu", Sys::millis());
            returnSignal=TIMEOUT;
            // TODO publish TIMER_TICK
            //           eb.publish(H("sys"),H("tick"));
        }
    }
    return (Signal)returnSignal;
}


Erc Serial2Mqtt::serialConnect()
{
    struct termios options;
    INFO( "Connecting to '%s' ....", _serialPort.c_str());

    _serialFd = ::open(_serialPort.c_str(), O_EXCL | O_RDWR | O_NOCTTY | O_NDELAY);

    if (_serialFd == -1) {
        ERROR("connect: Unable to open '%s' errno : %d : %s ",
              _serialPort.c_str(), errno, strerror(errno));
        _serialFd = 0;
        return errno;
    }
    INFO( "open '%s' succeeded.fd=%d", _serialPort.c_str(),_serialFd);


    fcntl(_serialFd, F_SETFL, FNDELAY);

    if ( tcgetattr(_serialFd, &options)<0)
        ERROR("tcgetattr() failed '%s' errno : %d : %s ",_serialPort.c_str(), errno, strerror(errno));
    if ( cfsetispeed(&options, baudSymbol(_serialBaudrate))<0)
        ERROR("cfsetispeed() failed '%s' errno : %d : %s ",_serialPort.c_str(), errno, strerror(errno));
    if ( cfsetospeed(&options, baudSymbol(_serialBaudrate))<0)
        ERROR("cfsetospeed() failed '%s' errno : %d : %s ",_serialPort.c_str(), errno, strerror(errno));
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag &= ~HUPCL;  // avoid DTR drop at close time
    options.c_cflag |= CS8;
    options.c_cflag &= ~CRTSCTS; /* Disable hardware flow control */

    options.c_lflag &= ~(ICANON | ECHO | ISIG);
    cfmakeraw(&options);

    if ( tcsetattr(_serialFd, TCSANOW, &options)<0)
        ERROR("tcsetattr() failed '%s' errno : %d : %s ",_serialPort.c_str(), errno, strerror(errno));
    INFO("set baudrate to %d ", _serialBaudrate);

    /*    int status;
        if ( ioctl(_serialFd, TIOCMGET,&status )<0)
            ERROR("ioctl()<0 '%s' errno : %d : %s ",_serialPort.c_str(), errno, strerror(errno));
        status |= TIOCM_DTR | TIOCM_RTS;
        if ( ioctl( _serialFd, TIOCMSET, &status )<0)
            ERROR("ioctl()<0 '%s' errno : %d : %s ",_serialPort.c_str(), errno, strerror(errno));
    */
    _serialConnected=true;
    return E_OK;
}

void Serial2Mqtt::serialDisconnect()
{
    close(_serialFd);
    _serialConnected=false;
}

void Serial2Mqtt::serialRxd()
{
    char buffer[1024];
    int erc;
    while(true) {
        erc=read(_serialFd,buffer,sizeof(buffer)-2);
        if ( erc > 0 ) {
//			INFO(" read() = %d bytes",erc);
            for(int i=0; i<erc; i++)
                _serialBuffer.write(buffer[i]);
            if ( _serialBuffer.size()>1000 ) _serialBuffer.clear();
        } else {
            return;
        }

    }
}

bool Serial2Mqtt::serialGetLine(std::string& line)
{
    while(_serialBuffer.hasData()) {
        char ch = _serialBuffer.read();
        if ( ch=='\n' ) {
            return true;
        } else if ( ch =='\r' ) {

        } else {
            if ( line.length()>1024 ) {
                line.clear();
                WARN(" %s buffer garbage  > 1024 flushed ",_serialPortShort.c_str());
            }
            line += ch;
        }
    }
    return false;
}

std::vector<std::string> split(const std::string &text, char sep)
{
    std::vector<std::string> tokens;
    std::size_t start = 0, end = 0;
    while ((end = text.find(sep, start)) != std::string::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(text.substr(start));
    return tokens;
}


void Serial2Mqtt::serialHandleLine(std::string& line)
{
//    INFO(" RXD %d bytes ",line.length());
    std::vector<std::string> token;
    /*	char cLine[line.length()+1];
    	strcpy(cLine,line.c_str());
    	token = split(cLine,',');*/
    if ( line[0]=='{') {

        try {
            json args=json::parse(line);
            INFO("%s %s",_serialPortShort.c_str(),args.dump().c_str());
//	ParseCSV(line,token);
//	for(uint32_t i=0; i<token.size(); i++)
//		INFO(" token[%d] = %s",i,token[i].c_str());
            if ( args.find("cmd") != args.end() ) {
                std::string cmd = args["cmd"];
                if ( cmd.compare("MQTT-PUB")==0 ) {
                    int qos=0;
                    bool retained=false;
                    std::string topic=args["topic"];
                    token = split(topic,'/');
                    if ( token[1].compare(_mqttDevice)!=0 ) {
                        WARN(" subscribed topic differ %s <> %s ",token[1].c_str(),_mqttDevice.c_str());
                        _mqttDevice =  token[1];
                        _mqttSubscribedTo = "dst/"+_mqttDevice+"/#";
                        mqttSubscribe(_mqttSubscribedTo);
                    }
                    std::string message=args["message"];
                    /*                    Bytes msg(1024);
                                        msg.append((uint8_t*)message.c_str(),message.length());*/
                    mqttPublish(topic,message,qos,retained);
                }
            }
        } catch(...) { // JSON Object parse failure
//            INFO("PARSE FAILED : %s",line.c_str());
            mqttPublish("src/"+_serial2mqttDevice+"/serial2mqtt/log",line,0,false);
        }
    } else { // no JSON object
//        INFO("NO { FOUND : %s",line.c_str())
        mqttPublish("src/"+_serial2mqttDevice+"/serial2mqtt/log",line,0,false);
    }
}

void Serial2Mqtt::serialPublish(std::string topic,Bytes message,int qos,bool retained)
{
    std::string line;
    Str msg(1024);
    msg.write( message.data(),0,message.length());
    json out ;
    out["cmd"]="MQTT-PUB";
    out["topic"]=topic;
    out["message"]=msg.c_str();
    if ( qos ) out["qos"]=qos;
    if ( retained ) out["retained"]=retained;
    line = out.dump();
    int erc = write(_serialFd,line.c_str(),line.length());
    if ( erc < 0 ) {
        INFO("write() failed '%s' errno : %d : %s ",_serialPort.c_str(), errno, strerror(errno));
    }
    INFO(" TXD : %s ",line.c_str());
}

/*
 *
						@     @  @@@@@  @@@@@@@ @@@@@@@
						@@   @@ @     @    @       @
						@ @ @ @ @     @    @       @
						@  @  @ @     @    @       @
						@     @ @   @ @    @       @
						@     @ @    @     @       @
						@     @  @@@@ @    @       @

 *
 */

Erc Serial2Mqtt::mqttConnect()
{
    std::string connection;
    int rc;

    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    MQTTAsync_willOptions will_opts = MQTTAsync_willOptions_initializer;

    connection = "tcp://"+ _mqttHost+":";
    connection +=std::to_string(_mqttPort);
    INFO(" MQTT connecting %s ... for %s ",connection.c_str(),_serialPortShort.c_str());
    MQTTAsync_create(&_client, connection.c_str(), _mqttClientId.c_str(),
                     MQTTCLIENT_PERSISTENCE_NONE, NULL);

    MQTTAsync_setCallbacks(_client, this, onConnectionLost, onMessage, onDeliveryComplete); //TODO add ondelivery

    conn_opts.keepAliveInterval = _mqttKeepAliveInterval;
    conn_opts.cleansession = 1;
    conn_opts.onSuccess = onConnectSuccess;
    conn_opts.onFailure = onConnectFailure;
    conn_opts.context = this;
    will_opts.message = _mqttWillMessage.c_str();
    will_opts.topicName = _mqttWillTopic.c_str();
    will_opts.qos = _mqttWillQos;
    will_opts.retained = _mqttWillRetained;
    conn_opts.will = &will_opts;
    if ((rc = MQTTAsync_connect(_client, &conn_opts)) != MQTTASYNC_SUCCESS) {
        WARN("Failed to start connect, return code %d", rc);
        return E_NOT_FOUND;
    }
    return E_OK;
}

void Serial2Mqtt::mqttDisconnect()
{
    MQTTAsync_disconnectOptions disc_opts =
        MQTTAsync_disconnectOptions_initializer;
    disc_opts.onSuccess = onDisconnect;
    disc_opts.context=this;
    int rc = 0;
    if ((rc = MQTTAsync_disconnect(_client, &disc_opts))
        != MQTTASYNC_SUCCESS) {
        WARN("Failed to disconnect, return code %d", rc);
        return;
    }
    MQTTAsync_destroy(&_client);
    _mqttConnected=false;
}

void Serial2Mqtt::mqttSubscribe(std::string topic)
{
    int qos=0;
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    INFO("Subscribing to topic %s for client %s using QoS%d",
         topic.c_str(), _mqttClientId.c_str(), qos);
    opts.onSuccess = onSubscribeSuccess;
    opts.onFailure = onSubscribeFailure;
    opts.context = this;
    int rc = E_OK;

    if ((rc = MQTTAsync_subscribe(_client, topic.c_str(), qos, &opts))
        != MQTTASYNC_SUCCESS) {
        ERROR("Failed to start subscribe, return code %d", rc);
        signal(MQTT_SUBSCRIBE_FAIL);
    } else {
        INFO(" subscribe send ");
    }
}

void Serial2Mqtt::onConnectionLost(void *context, char *cause)
{
    Serial2Mqtt* me = (Serial2Mqtt*)context;
//   me->mqttDisconnect();
    me->signal(MQTT_DISCONNECTED);
}
int Serial2Mqtt::onMessage(void *context, char *topicName, int topicLen, MQTTAsync_message *message)
{
    Serial2Mqtt* me = (Serial2Mqtt*)context;
    Bytes msg((uint8_t*) message->payload, message->payloadlen);
    string topic(topicName,topicLen);
    
    if ( topic.compare(me->_mqttProgrammerTopic)==0 ) {
        INFO(" flash image received , saved to %s",me->_binFile.c_str())
        me->flashBin(msg);
    } else {
    me->serialPublish(topic,msg,message->qos,message->retained);
    }

    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(topicName);
    me->signal(MQTT_MESSAGE_RECEIVED);
    return 1;
}

void Serial2Mqtt::onDeliveryComplete(void* context, MQTTAsync_token response)
{
//    Serial2Mqtt* me = (Serial2Mqtt*)context;
}

void Serial2Mqtt::onDisconnect(void* context, MQTTAsync_successData* response)
{
    Serial2Mqtt* me = (Serial2Mqtt*)context;
    me->signal(MQTT_DISCONNECTED);
}

void Serial2Mqtt::onConnectFailure(void* context, MQTTAsync_failureData* response)
{
    Serial2Mqtt* me = (Serial2Mqtt*)context;
    me->signal(MQTT_CONNECT_FAIL);
}

void Serial2Mqtt::onConnectSuccess(void* context, MQTTAsync_successData* response)
{
    Serial2Mqtt* me = (Serial2Mqtt*)context;
    me->signal(MQTT_CONNECT_SUCCESS);
}

void Serial2Mqtt::onSubscribeSuccess(void* context, MQTTAsync_successData* response)
{
    Serial2Mqtt* me = (Serial2Mqtt*)context;
    me->signal(MQTT_SUBSCRIBE_SUCCESS);
}

void Serial2Mqtt::onSubscribeFailure(void* context, MQTTAsync_failureData* response)
{
    Serial2Mqtt* me = (Serial2Mqtt*)context;
    me->signal(MQTT_SUBSCRIBE_FAIL);
}

void Serial2Mqtt::mqttPublish(std::string topic,std::string message,int qos,bool retained)
{
    Str msg(message.length()+2);
    msg=message.c_str();
    mqttPublish(topic,msg,qos,retained);
}

void Serial2Mqtt::mqttPublish(std::string topic,Bytes message,int qos,bool retained)
{
    if ( !_mqttConnected) {
        INFO("mqttPublish waiting connect ");
        return;
    }
    qos=1;
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    MQTTAsync_message pubmsg = MQTTAsync_message_initializer;

//    INFO("mqttPublish %s",topic.c_str());

    int rc = E_OK;
    opts.onSuccess = onPublishSuccess;
    opts.onFailure = onPublishFailure;
    opts.context = this;

    pubmsg.payload = message.data();
    pubmsg.payloadlen = message.length();
    pubmsg.qos = qos;
    pubmsg.retained = retained;

    if ((rc = MQTTAsync_sendMessage(_client, topic.c_str(), &pubmsg, &opts))
        != MQTTASYNC_SUCCESS) {
        signal(MQTT_DISCONNECTED);
        ERROR("MQTTAsync_sendMessage failed.");
    }
}
void Serial2Mqtt::onPublishSuccess(void* context, MQTTAsync_successData* response)
{
    Serial2Mqtt* me = (Serial2Mqtt*)context;
    me->signal(MQTT_PUBLISH_SUCCESS);
}
void Serial2Mqtt::onPublishFailure(void* context, MQTTAsync_failureData* response)
{
    Serial2Mqtt* me = (Serial2Mqtt*)context;
    me->signal(MQTT_PUBLISH_FAIL);
}


void Serial2Mqtt::flashBin(Bytes& bytes){
    FILE *f = NULL;
	char *data = NULL;

	/* open in read binary mode */
	f = fopen(_binFile.c_str(),"w");
	if ( f!=NULL) {
        size_t rc= fwrite(bytes.data(),1,bytes.length(),f);
        if ( rc != bytes.length() ) {
            WARN(" file %s cannot be saved. ",_binFile.c_str());
        }
		fclose(f);
        popen(_programCommand.c_str(),"r");
	} else {
		WARN(" bin file %s not found.",_binFile.c_str());
		data = (char*)malloc(10);
		strcpy(data,"{}");
	}
}