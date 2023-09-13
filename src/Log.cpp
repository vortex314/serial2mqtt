/*
 * LOg.cpp
 *
 *  Created on: Jul 3, 2016
 *      Author: lieven
 */
#include <Log.h>
extern "C" {
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
}

char Log::_logLevel[7] = {'T', 'D', 'I', 'W', 'E', 'F', 'N'};

#ifdef ARDUINO
#include <Arduino.h>
#include <WString.h>
#endif

#ifdef STM32_OPENCM3
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#endif

#ifdef LM4F_OPENCM3
#include <libopencm3/lm4f/uart.h>
#endif

#ifdef ESP32_IDF
#endif
#ifdef ESP_OPEN_RTOS
#endif

#ifdef __linux__
#include <sys/prctl.h>
#endif

std::string& string_format(std::string& str, const char* fmt, ...) {
    int size = strlen(fmt) * 2 + 50; // Use a rubric appropriate for your code
    va_list ap;
    while (1) { // Maximum two passes on a POSIX system...
        ASSERT(size < 1024);
        str.resize(size);
        va_start(ap, fmt);
        int n = vsprintf((char*)str.data(), fmt, ap);
        va_end(ap);
        if (n > -1 && n < size) { // Everything worked
            str.resize(n);
            return str;
        }
        if (n > -1)       // Needed size returned
            size = n + 1; // For null char
        else
            size *= 2; // Guess at a larger size (OS specific)
    }
    return str;
}

void bytesToHex(std::string& ret, uint8_t* input, uint32_t length, char sep) {
    static const char characters[] = "0123456789ABCDEF";
    for (uint32_t i = 0; i < length; i++) {
        ret += (characters[input[i] >> 4]);
        ret += characters[input[i] & 0x0F];
        if (sep != 0)
            ret += sep;
    }
}

void Log::serialLog(char* start, uint32_t length) {
#ifdef ARDUINO
    Serial.write((const uint8_t*)start, length);
    Serial.write("\r\n");
#endif
#if defined(STM32_OPENCM3) || defined(LM4F_OPENCM3)
    *(start + length) = '\0';
    char* s = start;
    while (*s) {
        uart_send_blocking(0, *(s++));
    }
#endif
#if defined(__linux__) || defined(ESP_OPEN_RTOS) || defined(ESP8266_RTOS_SDK) || defined(ESP32_IDF) ||      \
    defined(__APPLE__)
    *(start + length) = '\0';
    fprintf(stdout, "%s\n", start);
    fflush(stdout);
#endif
}

Log::Log(uint32_t size)
    : _enabled(true), _logFunction(serialLog), _level(LOG_INFO),
      _sema(Sema::create()) {
    if (_line == 0) {
        _line = new std::string;
        _line->reserve(size);
    }
    _application[0] = 0;
    _hostname[0] = 0;
}

Log::~Log() {}

void Log::setLogLevel(char c) {
    for (uint32_t i = 0; i < sizeof(_logLevel); i++)
        if (_logLevel[i] == c) {
            _level = (Log::LogLevel)i;
            break;
        }
}

bool Log::enabled(LogLevel level) {
    if (level >= _level) {
        return true;
    }
    return false;
}

void Log::disable() { _enabled = false; }

void Log::enable() { _enabled = true; }

void Log::defaultOutput() { _logFunction = serialLog; }

void Log::writer(LogFunction function) { _logFunction = function; }

LogFunction Log::writer() { return _logFunction; }

#ifdef ESP8266
extern "C" {
#include <ets_sys.h>
};
#endif

void Log::application(const char* app) {
    strncpy(_application, app, sizeof(_application));
}

void Log::log(char level, const char* file, uint32_t lineNbr,
              const char* function, const char* fmt, ...) {
    _sema.wait();
    if (_line == 0) {
        ::printf("%s:%d %s:%u\n", __FILE__, __LINE__, file,
                 (unsigned int)lineNbr);
        //		_sema.release();
        return;
    }

    va_list args;
    va_start(args, fmt);
    static char logLine[256];
    vsnprintf(logLine, sizeof(logLine) - 1, fmt, args);
    va_end(args);
#ifdef __linux__
#ifdef __GLIBC__
    //	::snprintf(_application,sizeof(_application),"%X",(uint32_t)pthread_self());
    pthread_getname_np(pthread_self(), _application, sizeof(_application));
#else
    static_assert(sizeof(_application) >= 16, "PR_GET_NAME argument must be at least 16 bytes long");
    if (prctl(PR_GET_NAME, _application)) {
	    ::snprintf(_application, sizeof(_application), "%X", static_cast<uint32_t>(pthread_self()));
    }
#endif
#endif
#if defined(ESP32_IDF) || defined(ESP_OPEN_RTOS) || defined(ESP8266_RTOS_SDK)
    extern void* pxCurrentTCB;
    //	if ( _application[0]==0)
    ::snprintf(_application, sizeof(_application), "%X",
               (uint32_t)pxCurrentTCB);
#endif
    string_format(*_line, "%+10.10s %c | %8s | %s | %15s:%4d | %s",
                  _application, level, time(), Sys::hostname(), file, lineNbr,
                  logLine);
    logger.flush();
    _sema.release();
}

void Log::flush() {
    if (_logFunction)
        _logFunction((char*)_line->c_str(), _line->size());
    *_line = "";
}

void Log::level(LogLevel l) { _level = l; }

Log::LogLevel Log::level() { return _level; }
//---------------------------------------------------------------------------------------------

//_________________________________________ LINUX
//___________________________________________
//
#if defined(__linux__) || defined(__APPLE__)
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
//---------------------------------------------------------------------------------------------
const char* Log::time() {
    struct timeval tv;
    struct timezone tz;
    struct tm* tm;
    static char buffer[100];
    gettimeofday(&tv, &tz);
    tm = ::localtime(&tv.tv_sec);
    sprintf(buffer, "%02d:%02d:%02d.%03u ", tm->tm_hour, tm->tm_min, tm->tm_sec,
            (uint32_t)tv.tv_usec / 1000);
    return buffer;
}

extern const char* __progname;

#endif
//_________________________________________ EMBEDDED

#if defined(ESP32_IDF) || defined(ARDUINO)
const char* Log::time() {
    static char szTime[20];
    snprintf(szTime, sizeof(szTime), "%llu", Sys::millis());
    return szTime;
}
#endif

#if defined(ESP_OPEN_RTOS) || defined(ESP8266_RTOS_SDK) // doesn't support 64 bit printf
const char* Log::time() {
    static char szTime[20];
    snprintf(szTime, sizeof(szTime), "%d", (uint32_t)Sys::millis());
    return szTime;
}
#endif

#ifdef ARDUINO

#endif
