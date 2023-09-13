/*
 * Log.h
 *
 *  Created on: Jul 3, 2016
 *      Author: lieven
 */

#ifndef LOG_H_
#define LOG_H_

#include <Sema.h>
#include <Sys.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <string>

#define myASSERT(xxx)                                                          \
    if (!(xxx)) {                                                              \
        WARN(" assertion " #xxx " failed.");                                   \
    };

extern std::string& string_format(std::string& str, const char* fmt, ...);
void bytesToHex(std::string& ret, uint8_t* input, uint32_t length,
                char sep = 0);

typedef void (*LogFunction)(char* start, uint32_t length);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
using cstr = const char* const;

static constexpr cstr past_last_slash(cstr str, cstr last_slash) {
    return *str == '\0' ? last_slash
                        : *str == '/' ? past_last_slash(str + 1, str + 1)
                                      : past_last_slash(str + 1, last_slash);
}

static constexpr cstr past_last_slash(cstr str) {
    return past_last_slash(str, str);
}
#define __SHORT_FILE__                                                         \
    ({                                                                         \
        constexpr cstr sf__{past_last_slash(__FILE__)};                        \
        sf__;                                                                  \
    })
//#pragma GCC diagnostic pop

class Log {
  public:
    typedef enum {
        LOG_TRACE = 0,
        LOG_DEBUG,
        LOG_INFO,
        LOG_WARN,
        LOG_ERROR,
        LOG_FATAL,
        LOG_NONE
    } LogLevel;
    static char _logLevel[7];
    std::string* _line;

  private:
    bool _enabled;
    LogFunction _logFunction;
    char _hostname[20];
    char _application[20];
    LogLevel _level;
#if !defined(ARDUINO)
    Sema& _sema;
#endif

  public:
    Log(uint32_t size);
    ~Log();
    bool enabled(LogLevel level);
    void setLogLevel(char l);
    void disable();
    void enable();
    void defaultOutput();
    void writer(LogFunction function);
    LogFunction writer();
    void printf(const char* fmt, ...);
    void log(char level, const char* file, uint32_t line, const char* function,
             const char* fmt, ...);

    void vprintf(const char* fmt, va_list args);
    const char* time();
    void host(const char* hostname);
    void location(const char* module, uint32_t line);
    void application(const char* applicationName);
    void flush();
    void level(LogLevel l);
    void logLevel();
    static void serialLog(char* start, uint32_t length);
    LogLevel level();
};

extern Log logger;
//#include <cstdio>

#define LOGF(fmt, ...)                                                         \
    logger.log(__FLE__, __SHORT_FILE__, __PRETTY_FUNCTION__, fmt, ##__VA_ARGS__)

/*
 #define LOGF(fmt, ...) \ logger.log(__FLE__, __LINE__, __PRETTY_FUNCTION__,
 fmt, ##__VA_ARGS__)
 */
#undef ASSERT
#define ASSERT(xxx)                                                            \
    if (!(xxx)) {                                                              \
        WARN(" ASSERT FAILED : %s", #xxx);                                     \
        while (1) {                                                            \
            Sys::delay(1000);                                                  \
        };                                                                     \
    }

#define INFO(fmt, ...)                                                         \
    if (logger.enabled(Log::LOG_INFO))                                         \
    logger.log('I', __SHORT_FILE__, __LINE__, __PRETTY_FUNCTION__, fmt,        \
               ##__VA_ARGS__)
#define ERROR(fmt, ...)                                                        \
    if (logger.enabled(Log::LOG_ERROR))                                        \
    logger.log('E', __SHORT_FILE__, __LINE__, __PRETTY_FUNCTION__, fmt,        \
               ##__VA_ARGS__)
#define WARN(fmt, ...)                                                         \
    if (logger.enabled(Log::LOG_WARN))                                         \
    logger.log('W', __SHORT_FILE__, __LINE__, __PRETTY_FUNCTION__, fmt,        \
               ##__VA_ARGS__)
#define FATAL(fmt, ...)                                                        \
    if (logger.enabled(Log::LOG_FATAL))                                        \
    logger.log('F', __SHORT_FILE__, __LINE__, __PRETTY_FUNCTION__, fmt,        \
               ##__VA_ARGS__)
#define DEBUG(fmt, ...)                                                        \
    if (logger.enabled(Log::LOG_DEBUG))                                        \
    logger.log('D', __SHORT_FILE__, __LINE__, __PRETTY_FUNCTION__, fmt,        \
               ##__VA_ARGS__)
#define TRACE(fmt, ...)                                                        \
    if (logger.enabled(Log::LOG_TRACE))                                        \
    logger.log('T', __SHORT_FILE__, __LINE__, __PRETTY_FUNCTION__, fmt,        \
               ##__VA_ARGS__)

#define __FLE__                                                                \
    (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1   \
                                      : __FILE__)

#endif /* LOG_H_ */
