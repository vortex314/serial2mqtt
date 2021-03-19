
#include "Serial2Mqtt.h"

typedef struct
{
	uint32_t baudrate;
	uint32_t symbol;
} BAUDRATE;

BAUDRATE BAUDRATE_TABLE[] = {{50, B50}, {75, B75}, {110, B110}, {134, B134}, //
							 {150, B150},
							 {200, B200},
							 {300, B300},
							 {600, B600},
							 {1200, B1200},
							 {1800, B1800},
							 {2400, B2400},
							 {4800, B4800},
							 {9600, B9600}, //
							 {19200, B19200},
							 {38400, B38400},
							 {57600, B57600}, //
							 {115200, B115200},
							 {230400, B230400},
							 {460800, B460800}, //
							 {500000, B500000},
							 {576000, B576000},
							 {921600, B921600}, //
							 {1000000, B1000000},
							 {1152000, B1152000},
							 {1500000, B1500000}, //
							 {2000000, B2000000},
							 {2500000, B2500000},
							 {3000000, B3000000}, //
							 {3500000, B3500000},
							 {4000000, B4000000}};

const char *signalString[] = {"PIPE_ERROR",
							  "SELECT_ERROR",
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
							  "TIMEOUT"};

const char *cmdString[] = {"MQTT-SUB", "MQTT-PUB", "MQTT-CONN", "MQTT-DISC"};

void logRaw(const std::string line)
{
	logger.writer()((char *)line.c_str(), line.size());
}

#define USB() logger.application(_serialPort.c_str())

Serial2Mqtt::Serial2Mqtt()
	: _serialBuffer(10240), _jsonDocument()
{
	_mqttConnectionState = MS_DISCONNECTED;
}

Serial2Mqtt::~Serial2Mqtt() {}

void Serial2Mqtt::setConfig(Config config) { _config = config; }

int baudSymbol(uint32_t br)
{
	for (uint32_t i = 0; i < sizeof(BAUDRATE_TABLE) / sizeof(BAUDRATE); i++)
		if (BAUDRATE_TABLE[i].baudrate == br)
			return BAUDRATE_TABLE[i].symbol;
	INFO("connect: baudrate %d  not found, default to 115200.", br);
	return B115200;
}

void Serial2Mqtt::setSerialPort(string port)
{
	_serialPort = port;
	if (_serialPort.find("/dev/tty") == 0)
	{
		_serialPortShort = _serialPort.substr(8);
	}
	else
	{
		size_t pos = _serialPort.rfind('/');
		if (pos == string::npos)
		{
			_serialPortShort = _serialPort;
		}
		else
		{
			_serialPortShort = _serialPort.substr(pos + 1);
		}
	}
}

static bool mqttFilterValidate(const char *filter)
{
	switch (filter[0])
	{
	case '\0':
		return false;
	case '#':
		return filter[1] == '\0';
	case '+':
		if (filter[1] == '\0')
			return true;
		if (filter[1] != '/')
			return false;
		break;
	}

	for (;;)
	{
		++filter;
		switch (filter[0])
		{
		case '\0':
			return true;
		case '#':
			return filter[-1] == '/' && filter[1] == '\0';
		case '+':
			if (filter[-1] != '/')
				return false;
			if (filter[1] == '\0')
				return true;
			if (filter[1] != '/')
				return false;
			break;
		default:
			break;
		}
	}
}

void Serial2Mqtt::init()
{
	_startTime = Sys::millis();
	_config.setNameSpace("programmer");
	_config.get("file", _binFile, "image.binary");
	_config.get("exec", _programCommand, "echo no command defined ");

	_config.setNameSpace("serial");
	_config.get("baudrate", _serialBaudrate, 115200);
	std::string protocol;
	_config.get("protocol", protocol, "jsonObject");
	_protocol = JSON_OBJECT;
	if (protocol == "jsonObject")
		_protocol = JSON_OBJECT;
	else if (protocol == "jsonArray")
		_protocol = JSON_ARRAY;
	else if (protocol == "protobuf")
		_protocol = PROTOBUF;

	JsonObject conf = _config.root();
	_logProtocol = conf["log"]["protocol"] | false;
	_logDebug = conf["log"]["debug"] | true;
	_logUseColors = conf["log"]["useColors"] | true;
	_logMqtt = conf["log"]["mqtt"] | false;
	_logProgram = conf["log"]["program"] | false;

	if (_logUseColors == false)
	{
		_colorTxd = "";
		_colorRxd = "";
		_colorDebug = "";
		_colorDefault = "";
	}

	std::string crc;
	_config.get("crc", crc, "off");
	_crc = CRC_OFF;
	if (crc == "on")
		_crc = CRC_ON;
	else if (crc == "off")
		_crc = CRC_OFF;
	_config.get("reconnectInterval", _serialReconnectInterval, 5000);
	_config.get("idleTimeout", _serialIdleTimeout, 5000);
	_config.get("silentInterval", _serialSilentInterval, 0);

	_config.setNameSpace("mqtt");
	_config.get("connection", _mqttConnection, "tcp://test.mosquitto.org:1883");
	_config.get("keepAliveInterval", _mqttKeepAliveInterval, 5);
	_config.get("willMessage", _mqttWillMessage, "false");
	_config.get("willQos", _mqttWillQos, 0);
	_config.get("willRetained", _mqttWillRetained, false);
	_serial2mqttDevice = Sys::hostname();
	_serial2mqttDevice += "." + _serialPortShort;
	_mqttDevice = _serial2mqttDevice;
	_mqttSubscribedTo = "dst/" + _serial2mqttDevice + "/#";
	_mqttProgrammerTopic = "dst/" + _serial2mqttDevice + "/serial2mqtt/flash";
	_config.get("clientId", _mqttClientId, (_mqttDevice + std::to_string(getpid())).c_str());
	std::string willTopicDefault = "src/" + _serial2mqttDevice + "/serial2mqtt/alive";
	_config.get("willTopic", _mqttWillTopic, willTopicDefault.c_str());
	_config.get("user", _mqttUser, "");
	_config.get("password", _mqttPassword, "");
	_config.get("reconnectInterval", _mqttReconnectInterval, 2000);
	_config.get("publishInterval", _mqttPublishInterval, 1000);
	_config.get("localPersistenceDir", _mqttLocalPersistenceDir, "");
	JsonArray filters = config.root()["mqtt"]["localPersistenceFilters"].as<JsonArray>();
	if (!filters.isNull())
	{
		for (JsonArray::iterator it = filters.begin(); it != filters.end(); ++it)
		{
			const char *filter = it->as<const char *>();
			if (mqttFilterValidate(filter))
			{
				_mqttLocalPersistenceFilters.push_back(filter);
			}
			else
			{
				WARN("localPersistenceFilter %s invalid, skipping", filter);
			}
		}
	}
	if (_mqttLocalPersistenceDir.length() > 0)
	{
		_mqttLocalPersistenceFile = _mqttLocalPersistenceDir + "/local_persistence." + _serialPortShort;
	}

	if (pipe(_signalFd) < 0)
	{
		WARN("Failed to create pipe: %s (%d)", strerror(errno), errno);
	}

	if (fcntl(_signalFd[0], F_SETFL, O_NONBLOCK) < 0)
	{
		WARN("Failed to set pipe non-blocking mode: %s (%d)", strerror(errno), errno);
	}
}

void Serial2Mqtt::threadFunction(void *pv) { run(); }

void Serial2Mqtt::run()
{
	string line;
	Timer mqttConnectTimer;
	Timer serialConnectTimer;
	Timer mqttPublishTimer;
	Timer serialTimer;
	Bytes nullmsg(NULL, 0);
	if (_mqttLocalPersistenceFile.length() > 0)
	{
		ifstream smf(_mqttLocalPersistenceFile);
		DynamicJsonDocument doc(1 * 1024 * 1024);
		DeserializationError error = deserializeJson(doc, smf);
		if (error == DeserializationError::Ok)
		{
			if (!doc.is<JsonObject>())
			{
				error = DeserializationError::InvalidInput;
			}
		}
		if (error == DeserializationError::Ok)
		{
			for (JsonPair p : doc.as<JsonObject>())
			{
				if (!p.value().is<string>())
				{
					error = DeserializationError::InvalidInput;
					break;
				}
				string message = p.value().as<string>();
				Bytes data(reinterpret_cast<uint8_t *>(const_cast<char *>(message.c_str())), message.length());
				_mqttLocalPersistenceMessages.emplace(p.key().c_str(), data);
			}
		}
		if (error == DeserializationError::Ok)
		{
			INFO("%d entries loaded from local persistence file %s", _mqttLocalPersistenceMessages.size(), _mqttLocalPersistenceFile.c_str());
		}
		else
		{
			_mqttLocalPersistenceMessages.clear();
			ERROR("unable to load local persistence file %s", _mqttLocalPersistenceFile.c_str());
		}
	}
	mqttConnectTimer.atInterval(_mqttReconnectInterval).doThis([this]() {
		if (_mqttConnectionState != MS_CONNECTING)
		{
			mqttConnect();
		}
	});
	serialConnectTimer.atInterval(_serialReconnectInterval).doThis([this, &serialTimer]() {
		if (!_serialConnected)
		{
			serialConnect();
			serialTimer.atDelta(_serialIdleTimeout);
		}
	});
	mqttPublishTimer.atInterval(_mqttPublishInterval).doThis([this]() {
		std::string sUpTime = std::to_string((Sys::millis() - _startTime) / 1000);
		mqttPublish("src/" + _serial2mqttDevice + "/serial2mqtt/alive", "true", 0, 0);
		mqttPublish("src/" + _serial2mqttDevice + "/system/upTime", sUpTime, 0, 0);
		mqttPublish("src/" + _serial2mqttDevice + "/serial2mqtt/device", _mqttDevice, 0, 0);
		reportStatus("src/" + _serial2mqttDevice + "/serial2mqtt/status");
	});
	serialTimer.atDelta(_serialIdleTimeout).doThis([this, &serialTimer]() {
		if (_serialConnected)
		{
			WARN("disconnecting serial no new data received in %d msec", _serialIdleTimeout);
			serialDisconnect();
			serialConnect();
			serialTimer.atDelta(_serialIdleTimeout);
		}
	});
	if (_mqttConnectionState != MS_CONNECTING)
		mqttConnect();
	if (serialConnect() != E_OK && _serialReconnectInterval < 1)
	{
		WARN("serial reconnection disabled, exiting now");
		exit(1);
	}
	while (true)
	{
		while (true)
		{
			Signal s = waitSignal(1000);

			DEBUG("signal = %s", signalString[s]);
			mqttConnectTimer.check();
			serialTimer.check();
			mqttPublishTimer.check();
			serialConnectTimer.check();
			switch (s)
			{
			case TIMEOUT:
			{
				if (!_serialConnected)
				{
					//						serialConnect();
				}
				break;
			}
			case SERIAL_RXD:
			{
				serialRxd();
				while (serialGetLine(line))
				{
					serialTimer.atDelta(_serialIdleTimeout);
					serialHandleLine(line);
					line.clear();
					serialRxd();
				}
				break;
			}
			case SERIAL_ERROR:
			{
				INFO("Serial error occured, disconnecting");
				serialDisconnect();
				break;
			}
			case MQTT_CONNECT_SUCCESS:
			{
				INFO("MQTT_CONNECT_SUCCESS");
				if (_serialConnected)
					serialPublish(CONNECT, _mqttDevice, nullmsg, 0, false);
				mqttConnectionState(MS_CONNECTED);
				mqttSubscribe(_mqttSubscribedTo);
				break;
			}
			case MQTT_CONNECT_FAIL:
			{
				WARN("MQTT_CONNECT_FAIL");
				mqttConnectionState(MS_DISCONNECTED);
				break;
			}
			case MQTT_DISCONNECTED:
			{
				WARN("MQTT_DISCONNECTED");
				serialPublish(DISCONNECT, "", nullmsg, 0, false);
				mqttConnectionState(MS_DISCONNECTED);
				break;
			}
			case MQTT_SUBSCRIBE_SUCCESS:
			{
				INFO("MQTT_SUBSCRIBE_SUCCESS");
				break;
			}
			case MQTT_SUBSCRIBE_FAIL:
			{
				WARN("MQTT_SUBSCRIBE_FAIL");
				mqttDisconnect();
				break;
			}
			case MQTT_ERROR:
			{
				WARN("MQTT_ERROR");
				break;
			}
			case PIPE_ERROR:
			{
				WARN("PIPE_ERROR");
				break;
			}
			case MQTT_PUBLISH_SUCCESS:
			{
				break;
			}
			case MQTT_MESSAGE_RECEIVED:
			{
				break;
			}
			default:
			{
				WARN("received signal [%d] %s", s, signalString[s]);
			}
			}
		}
	}
	WARN("exited run loop !!");
}

void Serial2Mqtt::signal(uint8_t m)
{
	if (write(_signalFd[1], (void *)&m, 1) < 1)
	{
		INFO("Failed to write pipe: %s (%d)", strerror(errno), errno);
	}
	//	INFO("signal '%c'",m);
}

Serial2Mqtt::Signal Serial2Mqtt::waitSignal(uint32_t timeout)
{
	Signal returnSignal = TIMEOUT;
	uint8_t buffer;
	fd_set rfds;
	fd_set wfds;
	fd_set efds;
	struct timeval tv;
	int retval;
	//    uint64_t delta=1000;

	// Wait up to 1000 msec.
	uint64_t delta = timeout;

	tv.tv_sec = 1;
	tv.tv_usec = (delta * 1000) % 1000000;

	// Watch serialFd and tcpFd  to see when it has input.
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&efds);
	if (_serialConnected)
	{
		FD_SET(_serialFd, &rfds);
		FD_SET(_serialFd, &efds);
	}
	if (_signalFd[0])
	{
		FD_SET(_signalFd[0], &rfds);
		FD_SET(_signalFd[0], &efds);
	}
	int maxFd = _serialFd < _signalFd[0] ? _signalFd[0] : _serialFd;
	maxFd += 1;

	retval = select(maxFd, &rfds, NULL, &efds, &tv);

	if (retval < 0)
	{
		WARN("select() : error : %s (%d)", strerror(errno), errno);
		returnSignal = SELECT_ERROR;
	}
	else if (retval > 0)
	{ // one of the fd was set
		if (FD_ISSET(_serialFd, &rfds))
		{
			returnSignal = SERIAL_RXD;
		}
		if (FD_ISSET(_signalFd[0], &rfds))
		{
			::read(_signalFd[0], &buffer, 1); // read 1 event
			returnSignal = (Signal)buffer;
		}
		if (FD_ISSET(_serialFd, &efds))
		{
			WARN("serial  error : %s (%d)", strerror(errno), errno);
			returnSignal = SERIAL_ERROR;
		}
		if (FD_ISSET(_signalFd[0], &efds))
		{
			WARN("pipe  error : %s (%d)", strerror(errno), errno);
			returnSignal = PIPE_ERROR;
		}
	}
	else
	{

		TRACE("timeout %llu", Sys::millis());
		returnSignal = TIMEOUT;
	}

	return (Signal)returnSignal;
}

void Serial2Mqtt::reportStatus(const std::string& topic)
{
	_jsonDocument.clear();
	JsonObject msg = _jsonDocument.to<JsonObject>();

	msg["Uptime"] = std::to_string((Sys::millis() - _startTime) / 1000);
	msg["SerialIsConnected"] = _serialConnected;
	msg["SerialConnectionCount"] = _serialConnectionCount;
	msg["SerialConnectionErrors"] = _serialConnectionErrors;
	msg["SerialPublishMessagesReceived"] = _serialPublishMessagesReceived;
	msg["SerialSubscribeMessagesReceived"] = _serialSubscribeMessagesReceived;
	msg["SerialUnknownMessagesReceived"] = _serialUnknownMessagesReceived;
	msg["SerialLogMessagesReceived"] = _serialLogMessagesReceived;
	msg["SerialMessagesSent"] = _serialMessagesSent;

	std::string s;
	serializeJson(msg, s);
	mqttPublish("src/" + _serial2mqttDevice + "/serial2mqtt/status", s, 0, 0);
}

Erc Serial2Mqtt::serialConnect()
{
	struct termios options;

	INFO("Connecting to '%s' ....", _serialPort.c_str());
	++_serialConnectionCount;
	++_serialConnectionErrors;

	if (_serialPort.find("tcp://") == 0)
	{
		char host[_serialPort.length()];
		char port[_serialPort.length()];
		char term;
		struct addrinfo hints = {.ai_family = AF_UNSPEC, .ai_socktype = SOCK_STREAM}, *addresses, *ai;
		int err;

		if (sscanf(_serialPort.c_str() + 6, "[%[^]]]:%s %c", host, port, &term) == 2)
		{
			// IPv6 numeric format, e.g.: tcp://[::1]:1111
		}
		else if (sscanf(_serialPort.c_str() + 6, "%[^:]:%s %c", host, port, &term) == 2)
		{
			// IPv4 numeric format or name, e.g.: tcp://foo.bar:1111
		}
		else
		{
			ERROR("unable to parse tcp address '%s'", _serialPort.c_str());
			return EINVAL;
		}
		if ((err = getaddrinfo(host, port, &hints, &addresses)) != 0)
		{
			ERROR("unable to resolve tcp address '%s': %s", _serialPort.c_str(), gai_strerror(err));
			return EINVAL;
		}
		for (ai = addresses; ai != NULL; ai = ai->ai_next)
		{
			_serialFd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
			if (_serialFd < 0)
			{
				continue;
			}
			if (connect(_serialFd, ai->ai_addr, ai->ai_addrlen) == 0)
			{
				INFO("open '%s' succeeded.fd=%d", _serialPort.c_str(), _serialFd);
				fcntl(_serialFd, F_SETFL, fcntl(_serialFd, F_GETFL, 0) | FNDELAY);
				break;
			}
			err = errno;
			close(_serialFd);
		}
		freeaddrinfo(addresses);
		if (ai == NULL)
		{
			_serialFd = 0;
			ERROR("connect: '%s' errno : %d : %s", _serialPort.c_str(), err, strerror(err));
			return err;
		}
	}
	else
	{
		_serialFd = ::open(_serialPort.c_str(), O_EXCL | O_RDWR | O_NOCTTY | O_NDELAY);

		if (_serialFd == -1)
		{
			ERROR("connect: Unable to open '%s' errno : %d : %s", _serialPort.c_str(), errno, strerror(errno));
			_serialFd = 0;
			return errno;
		}

		INFO("open '%s' succeeded.fd=%d", _serialPort.c_str(), _serialFd);

		//	fcntl(_serialFd, F_SETFL, FNDELAY);
		INFO("set baudrate to %d", _serialBaudrate);

		if (tcgetattr(_serialFd, &options) < 0)
			ERROR("tcgetattr() failed '%s' errno : %d : %s", _serialPort.c_str(), errno, strerror(errno));

		options.c_cflag |= (CLOCAL | CREAD);
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		options.c_cflag &= ~CSIZE;
		options.c_cflag &= ~HUPCL; // avoid DTR drop at close time
		options.c_cflag |= CS8;
		options.c_cflag &= ~CRTSCTS; /* Disable hardware flow control */

		//	options.c_lflag &= ~(ECHO | ISIG); // no echo, signal
		options.c_lflag = 0;	   // wait full line, was ICANON
		options.c_cc[VEOL] = '\n'; // add an additional EOL symbol
		options.c_cc[VTIME] = 0;
		options.c_cc[VMIN] = 1;
		options.c_iflag |= IGNCR;							  // ignore carriage return
		options.c_oflag &= ~(ONLCR | OCRNL | ONOCR | ONLRET); // our output is CR-LF terminated properly
		//    cfmakeraw(&options);
		if (cfsetispeed(&options, baudSymbol(_serialBaudrate)) < 0)
		{
			ERROR("cfsetispeed() failed '%s' errno : %d : %s", _serialPort.c_str(), errno, strerror(errno));
		}
		if (cfsetospeed(&options, baudSymbol(_serialBaudrate)) < 0)
		{
			ERROR("cfsetospeed() failed '%s' errno : %d : %s", _serialPort.c_str(), errno, strerror(errno));
		}

		if (tcsetattr(_serialFd, TCSANOW, &options) < 0)
		{
			ERROR("tcsetattr() failed '%s' errno : %d : %s", _serialPort.c_str(), errno, strerror(errno));
		}

		/*    int status;
		    if ( ioctl(_serialFd, TIOCMGET,&status )<0)
			ERROR("ioctl()<0 '%s' errno : %d : %s",_serialPort.c_str(), errno, strerror(errno));
		    status |= TIOCM_DTR | TIOCM_RTS;
		    if ( ioctl( _serialFd, TIOCMSET, &status )<0)
			ERROR("ioctl()<0 '%s' errno : %d : %s",_serialPort.c_str(), errno, strerror(errno));
		*/

		if (_serialSilentInterval > 0)
		{
			INFO("silent interval started for %lu msec", _serialSilentInterval);
			Sys::delay(_serialSilentInterval);
			tcflush(_serialFd, TCIOFLUSH);
		}
	}
	_serialConnected = true;
	--_serialConnectionErrors;

	if (_mqttConnectionState == MS_CONNECTED)
	{
		Bytes nullmsg(NULL, 0);
		serialPublish(CONNECT, _mqttDevice, nullmsg, 0, false);
	}
	for (map<string, Bytes>::iterator lpmi = _mqttLocalPersistenceMessages.begin(); lpmi != _mqttLocalPersistenceMessages.end(); ++lpmi)
	{
		serialPublish(PUBLISH, lpmi->first, lpmi->second, 0, true);
	}

	return E_OK;
}

void Serial2Mqtt::serialDisconnect()
{
	INFO("serial close(%d)", _serialFd);
	close(_serialFd);
	_serialFd = 0;
	_serialConnected = false;

	if (_serialReconnectInterval < 1)
	{
		WARN("serial reconnection disabled, exiting now");
		exit(1);
	}
}

void Serial2Mqtt::serialRxd()
{
	if (!_serialConnected)
		return;
	size_t space;
	char buffer[1024];
	int erc;
	while (_serialBuffer.size() < 1024) // just a magic limit to avoid memory overflow for nonsense
	{
		erc = read(_serialFd, buffer, min(space, sizeof(buffer)));
		if (erc > 0)
		{
			//			INFO("read() = %d bytes",erc);
			for (int i = 0; i < erc; i++)
				_serialBuffer.write(buffer[i]);
		}
		else if (erc < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				return;
			DEBUG("read returns %d => errno : %d = %s", erc, errno, strerror(errno));
			signal(SERIAL_ERROR);
		}
		else if (isatty(_serialFd))
		{
			// serial devices could return 0 upon no data available
			return;
		}
		else
		{
			// sockets return 0 upon reading from a closed connection
			DEBUG("EOF received on the serial connection");
			signal(SERIAL_ERROR);
			return;
		}
	}
}

bool Serial2Mqtt::serialGetLine(string &line)
{
	if (!_serialConnected)
		return false;
	while (_serialBuffer.hasData())
	{
		char ch = _serialBuffer.read();
		if (ch == '\n')
		{
			return true;
		}
		else if (ch == '\r')
		{
		}
		else
		{
			if (line.length() > 1024)
			{
				line.clear();
				WARN("buffer garbage  > 1024 flushed");
			}
			line += ch;
		}
	}
	return false;
}

std::vector<string> split(const string &text, char sep)
{
	std::vector<string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != string::npos)
	{
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(text.substr(start));
	return tokens;
}

unsigned short crc16(const unsigned char *data_p, unsigned char length)
{
	unsigned char x;
	unsigned short crc = 0xFFFF;

	while (length--)
	{
		x = crc >> 8 ^ *data_p++;
		x ^= x >> 4;
		crc = (crc << 8) ^ ((unsigned short)(x << 12)) ^ ((unsigned short)(x << 5)) ^ ((unsigned short)x);
	}
	return crc;
}
/*
	* extract CRC and verify in syntax => ["ABCD",2,..]
 */
bool Serial2Mqtt::checkCrc(std::string &line)
{
	if (line.length() < 9)
		return false;
	std::string crcStr = line.substr(2, 4);
	uint32_t crcFound;
	sscanf(crcStr.c_str(), "%4X", &crcFound);
	std::string line2 = line.substr(0, 2) + "0000" + line.substr(6, string::npos);
	uint32_t crcCalc = crc16((uint8_t *)line2.data(), line2.length());
	return crcCalc == crcFound;
}

void Serial2Mqtt::genCrc(std::string &line)
{
	uint32_t crcCalc = crc16((uint8_t *)line.data(), line.length());
	char hexCrc[5];
	sprintf(hexCrc, "%4.4X", crcCalc);
	std::string lineCopy = "[\"";
	lineCopy += hexCrc + line.substr(6, string::npos);
	line = lineCopy.c_str();
}

/*
 * JSON protocol : [CMD,TOPIC,MESSAGE,QOS,RETAIN,CRC]
 * CMD : 0:SUBSCRIBE,1:PUBLISH,2:CONNECT,3:DISCONNECT
 * subscribe : [0,"dst/myTopic","ABCD"]
 * publish : [1,"dst/topic1","message1",0,0,"ABCD"]
 * connect : [2,"device","","ABCD"]
 * disconnect : [3,"","","ABCD"]
 */

void Serial2Mqtt::serialHandleLine(string &line)
{

	std::vector<string> token;
	_jsonDocument.clear();
	if (_protocol == JSON_ARRAY && line.length() > 2 && line[0] == '[' && line[line.length() - 1] == ']')
	{
		deserializeJson(_jsonDocument, line);
		if (_jsonDocument.is<JsonArray>())
		{
			if (_logProtocol)
			{
				logRaw(_colorRxd + line + _colorDefault);
			}
			JsonArray array = _jsonDocument.as<JsonArray>();
			int cmd = array[0];
			if (cmd == PUBLISH)
			{
				std::string topic = array[1];
				std::string message = array[2];
				uint32_t qos = 0;
				if (array.size() > 3)
					qos = array[3];
				bool retained = false;
				if (array.size() > 4)
					retained = array[4];
				mqttPublish(topic, message, qos, retained);
				++_serialPublishMessagesReceived;
				return;
			}
			else if (cmd == SUBSCRIBE)
			{
				std::string topic = array[1];
				mqttSubscribe(topic);
				++_serialSubscribeMessagesReceived;
				return;
			}
			else
			{
				WARN("invalid command from device : %d", cmd);
				++_serialUnknownMessagesReceived;
			}
		}
	}
	else if (_protocol == JSON_OBJECT && line.length() > 2 && line[0] == '{' && line[line.length() - 1] == '}')
	{
		if (_logProtocol)
		{
			logRaw(_colorRxd + line + _colorDefault);
		};
		deserializeJson(_jsonDocument, line);
		if (_jsonDocument.is<JsonObject>())
		{
			JsonObject json = _jsonDocument.as<JsonObject>();
			if (json.containsKey("cmd"))
			{
				string cmd = json["cmd"];
				if (cmd.compare("MQTT-PUB") == 0 && json.containsKey("topic") && json.containsKey("message"))
				{
					int qos = 0;
					bool retained = false;
					string topic = json["topic"];
					token = split(topic, '/');
					if (token[1].compare(_mqttDevice) != 0)
					{
						WARN("subscribed topic differ %s <> %s", token[1].c_str(), _mqttDevice.c_str());
						_mqttDevice = token[1];
						_mqttSubscribedTo = "dst/" + _mqttDevice + "/#";
						mqttSubscribe(_mqttSubscribedTo);
					}
					string message = json["message"];
					/*                    Bytes msg(1024);
					                    msg.append((uint8_t*)message.c_str(),message.length());*/
					mqttPublish(topic, message, qos, retained);
					++_serialPublishMessagesReceived;
					return;
				}
				else if (cmd.compare("MQTT-SUB") == 0 && json.containsKey("topic"))
				{
					string topic = json["topic"];
					mqttSubscribe(topic);
					++_serialSubscribeMessagesReceived;
					return;
				}
				else
				{
					WARN("invalid command from device : %s", line.c_str());
					++_serialUnknownMessagesReceived;
				}
			}
		}
	}
	if (_logDebug)
		logRaw(_colorDebug + line + _colorDefault);
	if (_logMqtt)
	{
		mqttPublish("src/" + _serial2mqttDevice + "/serial2mqtt/log", line, 0, false);
		++_serialLogMessagesReceived;
	}
}

void Serial2Mqtt::serialPublish(CMD command, string topic, Bytes message, int qos, bool retained)
{
	std::string line;

	if (_protocol == JSON_OBJECT)
	{
		string msg;
		msg.assign((const char *)message.data(), 0, message.length());
		_jsonDocument.clear();
		JsonObject out = _jsonDocument.to<JsonObject>();
		out["cmd"] = cmdString[command];
		out["topic"] = topic;
		out["message"] = msg.c_str();
		if (qos)
			out["qos"] = qos;
		if (retained)
			out["retained"] = retained;
		serializeJson(out, line);
	}
	else if (_protocol == JSON_ARRAY)
	{
		string msg;
		msg.assign((const char *)message.data(), 0, message.length());
		DynamicJsonDocument doc(2038);
		doc.add(command);
		doc.add(topic);
		doc.add(msg);
		if (qos)
			doc.add(qos);
		if (retained)
			doc.add(1);
		if (_crc == CRC_ON)
			doc.add("0000");
		serializeJson(doc, line);
		if (_crc == CRC_ON)
			genCrc(line);
	}
	else
	{
		WARN("invalid protocol found.");
	}
	serialTxd(line + "\r\n");
	DEBUG("To Serial %s : %s", _serialPort.c_str(), line.c_str());
}

void Serial2Mqtt::serialTxd(const string &line)
{
	const char *buf;
	size_t len;

	if (_logProtocol)
	{
		std::string str = _colorTxd;
		for (std::string::const_iterator it = line.cbegin(); it != line.cend(); ++it)
			if (*it != '\r')
				str.push_back(*it);
		str += _colorDefault;
		logRaw(str);
	}

	buf = line.c_str();
	len = line.length();
	while (len > 0)
	{
		int erc = write(_serialFd, buf, len);
		if (erc >= 0)
		{
			buf += erc;
			len -= erc;
		}
		else if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			tcdrain(_serialFd);
		}
		else
		{
			WARN("write(%d,.,%d) failed '%s' errno : %d : %s", _serialFd, len, _serialPort.c_str(), errno, strerror(errno));
			signal(SERIAL_ERROR);
			break;
		}
	}
	fsync(_serialFd);
	++_serialMessagesSent;
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
const char *mqttConnectionStates[] = {"MS_CONNECTED", "MS_DISCONNECTED", "MS_CONNECTING", "MS_DISCONNECTING"};
void Serial2Mqtt::mqttConnectionState(MqttConnectionState st)
{
	INFO("MQTT connection state %s => %s", mqttConnectionStates[_mqttConnectionState], mqttConnectionStates[st]);
	_mqttConnectionState = st;

	if (st == MS_DISCONNECTED && _mqttReconnectInterval < 1)
	{
		WARN("MQTT reconnection disabled, exiting now");
		exit(1);
	}
}
static bool mqttTopicMatch(const string &filter, const string &topic)
{
	int i, j;

	// this algorithm doesn't handle topic or filter problems
	i = 0;
	j = 0;
	for (;;)
	{
		switch (filter[i])
		{
		case '\0':
			return topic[j] == '\0';
		case '#':
			return true;
		case '+':
			i++;
			while (topic[j] != '\0' && topic[j] != '/')
				j++;
			break;
		default:
			if (filter[i] != topic[j])
				return false;
			i++;
			j++;
			break;
		}
	}
}

Erc Serial2Mqtt::mqttConnect()
{
	int rc;
	if (_mqttConnectionState == MS_CONNECTING || _mqttConnectionState == MS_CONNECTED)
		return E_OK;

	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	MQTTAsync_willOptions will_opts = MQTTAsync_willOptions_initializer;

	INFO("MQTT connecting %s", _mqttConnection.c_str());
	mqttConnectionState(MS_CONNECTING);
	MQTTAsync_create(&_client, _mqttConnection.c_str(), _mqttClientId.c_str(), MQTTCLIENT_PERSISTENCE_NONE, NULL);

	MQTTAsync_setCallbacks(_client, this, onConnectionLost, onMessage, onDeliveryComplete); // TODO add ondelivery

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
	if (_mqttUser.length() > 0)
	{
		conn_opts.username = _mqttUser.c_str();
		conn_opts.password = _mqttPassword.c_str();
	}
	if ((rc = MQTTAsync_connect(_client, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
		WARN("Failed to start connect, return code %d", rc);
		return E_NOT_FOUND;
	}
	mqttConnectionState(MS_CONNECTING);
	return E_OK;
}

void Serial2Mqtt::mqttDisconnect()
{
	mqttConnectionState(MS_DISCONNECTING);
	MQTTAsync_disconnectOptions disc_opts = MQTTAsync_disconnectOptions_initializer;
	disc_opts.onSuccess = onDisconnect;
	disc_opts.context = this;
	int rc = 0;
	if ((rc = MQTTAsync_disconnect(_client, &disc_opts)) != MQTTASYNC_SUCCESS)
	{
		WARN("Failed to disconnect, return code %d", rc);
		return;
	}
	MQTTAsync_destroy(&_client);
	mqttConnectionState(MS_DISCONNECTED);
}

void Serial2Mqtt::mqttSubscribe(string topic)
{
	int qos = 0;
	for (map<string, Bytes>::iterator lpmi = _mqttLocalPersistenceMessages.begin(); lpmi != _mqttLocalPersistenceMessages.end(); ++lpmi)
	{
		if (mqttTopicMatch(topic, lpmi->first))
		{
			serialPublish(PUBLISH, lpmi->first, lpmi->second, 0, true);
		}
	}
	if (_mqttConnectionState != MS_CONNECTED)
		return;
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	INFO("Subscribing to topic %s for client %s using QoS%d", topic.c_str(), _mqttClientId.c_str(), qos);
	opts.onSuccess = onSubscribeSuccess;
	opts.onFailure = onSubscribeFailure;
	opts.context = this;
	int rc = E_OK;

	if ((rc = MQTTAsync_subscribe(_client, topic.c_str(), qos, &opts)) != MQTTASYNC_SUCCESS)
	{
		ERROR("Failed to start subscribe, return code %d", rc);
		signal(MQTT_SUBSCRIBE_FAIL);
	}
	else
	{
		if (_logMqtt)
			INFO("subscribe send");
	}
}

void Serial2Mqtt::onConnectionLost(void *context, char *cause)
{
	Serial2Mqtt *me = (Serial2Mqtt *)context;
	//   me->mqttDisconnect();
	me->signal(MQTT_DISCONNECTED);
}

int Serial2Mqtt::onMessage(void *context, char *topicName, int topicLen, MQTTAsync_message *message)
{
	Serial2Mqtt *me = (Serial2Mqtt *)context;
	Bytes msg((uint8_t *)message->payload, message->payloadlen);
	string topic(topicName, topicLen);
	string m((char *)message->payload, message->payloadlen);
	if (me->_logMqtt)
		INFO("MQTT RXD %s : %s", topic.c_str(), m.c_str());

	if (topic.compare(me->_mqttProgrammerTopic) == 0)
	{
		INFO("flash image received , saved to %s", me->_binFile.c_str());
		me->flashBin(msg);
	}
	else
	{

		for (vector<string>::iterator it = me->_mqttLocalPersistenceFilters.begin(); it != me->_mqttLocalPersistenceFilters.end(); ++it)
		{
			if (mqttTopicMatch(*it, topic))
			{
				bool modified = false;
				try
				{
					Bytes &b = me->_mqttLocalPersistenceMessages.at(topic);
					if (msg.length() > 0)
					{
						if (msg.length() != b.length() || memcmp(msg.data(), b.data(), b.length() != 0))
						{
							me->_mqttLocalPersistenceMessages.erase(topic);
							me->_mqttLocalPersistenceMessages.emplace(topic, msg);
							modified = true;
						}
					}
					else
					{
						me->_mqttLocalPersistenceMessages.erase(topic);
						modified = true;
					}
				}
				catch (...)
				{
					if (msg.length() > 0)
					{
						me->_mqttLocalPersistenceMessages.emplace(topic, msg);
						modified = true;
					}
				}
				if (modified && me->_mqttLocalPersistenceFile.length() > 0)
				{
					ofstream smf(me->_mqttLocalPersistenceFile);
					DynamicJsonDocument doc(1 * 1024 * 1024);
					string b;
					doc.to<JsonObject>();
					for (map<string, Bytes>::iterator lpmi = me->_mqttLocalPersistenceMessages.begin(); lpmi != me->_mqttLocalPersistenceMessages.end(); ++lpmi)
					{
						b.assign(reinterpret_cast<char *>(lpmi->second.data()), lpmi->second.length());
						doc[lpmi->first] = b;
					}
					serializeJson(doc, smf);
					INFO("%d entries saved to local persistence file %s", me->_mqttLocalPersistenceMessages.size(), me->_mqttLocalPersistenceFile.c_str());
				}
				break;
			}
		}
		me->serialPublish(PUBLISH, topic, msg, message->qos, message->retained);
	}

	MQTTAsync_freeMessage(&message);
	MQTTAsync_free(topicName);
	me->signal(MQTT_MESSAGE_RECEIVED);
	return 1;
}

void Serial2Mqtt::onDeliveryComplete(void *context, MQTTAsync_token response)
{
	//    Serial2Mqtt* me = (Serial2Mqtt*)context;
}

void Serial2Mqtt::onDisconnect(void *context, MQTTAsync_successData *response)
{
	Serial2Mqtt *me = (Serial2Mqtt *)context;
	me->signal(MQTT_DISCONNECTED);
}

void Serial2Mqtt::onConnectFailure(void *context, MQTTAsync_failureData *response)
{
	Serial2Mqtt *me = (Serial2Mqtt *)context;
	me->signal(MQTT_CONNECT_FAIL);
}

void Serial2Mqtt::onConnectSuccess(void *context, MQTTAsync_successData *response)
{
	Serial2Mqtt *me = (Serial2Mqtt *)context;
	me->signal(MQTT_CONNECT_SUCCESS);
}

void Serial2Mqtt::onSubscribeSuccess(void *context, MQTTAsync_successData *response)
{
	Serial2Mqtt *me = (Serial2Mqtt *)context;
	me->signal(MQTT_SUBSCRIBE_SUCCESS);
}

void Serial2Mqtt::onSubscribeFailure(void *context, MQTTAsync_failureData *response)
{
	Serial2Mqtt *me = (Serial2Mqtt *)context;
	me->signal(MQTT_SUBSCRIBE_FAIL);
}

void Serial2Mqtt::mqttPublish(string topic, string message, int qos, bool retained)
{
	Bytes msg(1024);
	if (_logMqtt)
		INFO("MQTT TXD : %s = %s", topic.c_str(), message.c_str());
	msg = message.c_str();
	mqttPublish(topic, msg, qos, retained);
}

void Serial2Mqtt::mqttPublish(string topic, Bytes message, int qos, bool retained)
{
	if (!_mqttConnectionState == MS_CONNECTED)
	{
		return;
	}
	qos = 1;
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;

	//   INFO("mqttPublish %s",topic.c_str());

	int rc = E_OK;
	opts.onSuccess = onPublishSuccess;
	opts.onFailure = onPublishFailure;
	opts.context = this;

	pubmsg.payload = message.data();
	pubmsg.payloadlen = message.length();
	pubmsg.qos = qos;
	pubmsg.retained = retained;

	if ((rc = MQTTAsync_sendMessage(_client, topic.c_str(), &pubmsg, &opts)) != MQTTASYNC_SUCCESS)
	{
		signal(MQTT_DISCONNECTED);
		ERROR("MQTTAsync_sendMessage failed.");
	}
}
void Serial2Mqtt::onPublishSuccess(void *context, MQTTAsync_successData *response)
{
	Serial2Mqtt *me = (Serial2Mqtt *)context;
	me->signal(MQTT_PUBLISH_SUCCESS);
}
void Serial2Mqtt::onPublishFailure(void *context, MQTTAsync_failureData *response)
{
	Serial2Mqtt *me = (Serial2Mqtt *)context;
	me->signal(MQTT_PUBLISH_FAIL);
}

void Serial2Mqtt::flashBin(Bytes &bytes)
{
	FILE *f = NULL;
	INFO("writing binary file %s", _binFile.c_str());
	/* open in read binary mode */
	f = fopen(_binFile.c_str(), "w");
	if (f != NULL)
	{
		size_t rc = fwrite(bytes.data(), 1, bytes.length(), f);
		if (rc != bytes.length())
		{
			WARN("file %s cannot be saved.", _binFile.c_str());
		}
		fclose(f);
		INFO("executing programming %s", _programCommand.c_str());
		FILE *outFd = popen(_programCommand.c_str(), "r");
		sleep(30);
		if (outFd == NULL)
		{
			WARN("stating '%s' failed.", _programCommand.c_str());
		}
		else
		{
			pclose(outFd);
		}
	}
	else
	{
		WARN("bin file %s not found.", _binFile.c_str());
	}
}
