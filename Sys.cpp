/*
 * Sys.cpp
 *
 *  Created on: May 15, 2016
 *      Author: lieven
 */

#include <Log.h>
#include <Sys.h>
#include <stdint.h>
#include <sys/time.h>
//#include <espressif/esp_wifi.h>

uint64_t Sys::_upTime;

#include <time.h>
#include <unistd.h>

uint64_t Sys::millis()   // time in msec since boot, only increasing
{
	struct timespec deadline;
	clock_gettime((int)CLOCK_MONOTONIC, &deadline);
	Sys::_upTime= deadline.tv_sec*1000 + deadline.tv_nsec/1000000;
	return _upTime;
}


void Sys::init()
{
	gethostname(_hostname,30);
}



void Sys::delay(uint32_t time)
{
	usleep(time*1000);
}

uint64_t Sys::now()
{
	return _boot_time+Sys::millis();
}

void Sys::setNow(uint64_t n)
{
	_boot_time = n-Sys::millis();
}

void Sys::hostname(const char* hostname)
{
	strncpy(_hostname, hostname,sizeof(_hostname));
}

const char* Sys::hostname()
{
	return _hostname;
}


uint32_t Sys::getSerialId()
{
	return 0xDEADBEEF;
}

const char* Sys::getProcessor()
{
	return "AMD64";
}
const char* Sys::getBuild()
{
	return __DATE__ " " __TIME__;
}



uint32_t Sys::getFreeHeap()
{
	return 123456789;
};

char Sys::_hostname[30];
uint64_t Sys::_boot_time = 0;


uint32_t Sys::sec()
{
	return millis()/1000;
}



void Sys::setHostname(const char *h)
{
	strncpy(_hostname, h, strlen(h) + 1);
}


extern "C" uint64_t SysMillis()
{
	return Sys::millis();
}
