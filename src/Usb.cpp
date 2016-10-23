#include "Usb.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <cstring>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/serial.h>
#include <sys/ioctl.h>

#include <EventBus.h>

typedef struct {
	uint32_t baudrate;
	uint32_t symbol;
} BAUDRATE;

BAUDRATE BAUDRATES[] = { { 50, B50 }, { 75, B75 }, { 110, B110 }, { 134, B134 }, //
		{ 150, B150 }, { 200, B200 }, { 300, B300 }, { 600, B600 }, { 1200,
				B1200 }, //
		{ 1800, B1800 }, { 2400, B2400 }, { 4800, B4800 }, { 9600, B9600 }, //
		{ 19200, B19200 }, { 38400, B38400 }, { 57600, B57600 }, //
		{ 115200, B115200 }, { 230400, B230400 }, { 460800, B460800 }, //
		{ 500000, B500000 }, { 576000, B576000 }, { 921600, B921600 }, //
		{ 1000000, B1000000 }, { 1152000, B1152000 }, { 1500000, B1500000 }, //
		{ 2000000, B2000000 }, { 2500000, B2500000 }, { 3000000, B3000000 }, //
		{ 3500000, B3500000 }, { 4000000, B4000000 } };

Usb::Usb(const char* device) :
		Actor("USB"), BufferedByteStream(256) {
	_device = device;
	_fd = 0;
	_isComplete = false;
}

void Usb::setDevice(const char* device) {
	_device = device;
}

void Usb::setBaudrate(uint32_t baudrate) {
	_baudrate = baudrate;
}

uint32_t baudSymbol(uint32_t value) {
	for (uint32_t i = 0; i < sizeof(BAUDRATES) / sizeof(BAUDRATE); i++)
		if (BAUDRATES[i].baudrate == value)
			return BAUDRATES[i].symbol;
	LOGF("connect: baudrate %s  not found, default to 115200.", value);
	return B115200;
}

void Usb::flush() {
	Bytes bytes(1024);
	while (_txd.hasData()) {
		bytes.write(_txd.read());
	}
	send(bytes);
}

Erc Usb::open() {
	struct termios options;
	LOGF( "Connecting to %s ....", _device);

	_fd = ::open(_device, O_EXCL | O_RDWR | O_NOCTTY | O_NDELAY);

	if (_fd == -1) {
		LOGF("connect: Unable to open %s errno : %d : %s ",
				_device, errno, strerror(errno));
		_fd = 0;
		return errno;
	}
	fcntl(_fd, F_SETFL, FNDELAY);

	tcgetattr(_fd, &options);
	cfsetispeed(&options, baudSymbol(_baudrate));
	cfsetospeed(&options, baudSymbol(_baudrate));

	options.c_cflag |= (CLOCAL | CREAD);
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	options.c_cflag &= ~CRTSCTS; /* Disable hardware flow control */

	options.c_lflag &= ~(ICANON | ECHO | ISIG);
	cfmakeraw(&options);

	tcsetattr(_fd, TCSANOW, &options);
	LOGF("set baudrate to ", _baudrate);
	LOGF( "open %s succeeded.", _device);
	return E_OK;
}

void Usb::logStats() {
	struct serial_icounter_struct counters;
	int rc = ::ioctl(_fd, TIOCGICOUNT, &counters);
	if (rc < 0) {
		LOGF("ioctl(TIOCGICOUNT) failed errno : %d : %s ",
				errno, strerror(errno));
		close();
		return;
	}
	LOGF(" overrun : %d", counters.buf_overrun);
}

Erc Usb::close() {
	_isComplete = false;
	::close(_fd);
	_fd = 0;
	eb.publish(H("usb.closed"));
	return E_OK;
}

Erc Usb::send(Bytes& bytes) {
	uint32_t count = ::write(_fd, bytes.data(), bytes.length());
	if (count != bytes.length()) {
		close();
		LOGF("send() failed errno : %d : %s ", errno, strerror(errno));
		return E_AGAIN;
	}
	return E_OK;
}

uint8_t Usb::read() {
	uint8_t b;
	int erc = ::read(_fd, &b, 1);
	if (erc < 0) {
		LOGF(" read() : %d %s", errno, strerror(errno));
		close();
		return 0;
	}
	return b;
}

#include <sys/ioctl.h>
bool Usb::hasData() {
	int count;
	int rc = ioctl(_fd, FIONREAD, (char *) &count);
	if (rc < 0) {
		LOGF(" ioctl() : %d %s", errno, strerror(errno));
		close();
		return 0;
	}
	return count == 0 ? false : true;
}

void Usb::onEvent(Cbor& cbor) {
}

int Usb::fd() {
	return _fd;
}

