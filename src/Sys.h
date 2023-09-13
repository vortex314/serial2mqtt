#ifndef SYS_H
#define SYS_H

#include "errno.h"
#include "stdint.h"
//#include <string.h>

//#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#if defined(__GNUC__) && !defined(__ALIGN_END) /* GNU Compiler */
#define __ALIGN_END __attribute__((aligned(4)))
#define __ALIGN_BEGIN
#else
#ifndef __ALIGN_END
#define __ALIGN_END
#if defined(__CC_ARM) /* ARM Compiler */
#define __ALIGN_BEGIN __align(4)
#elif defined(__ICCARM__) /* IAR Compiler */
#define __ALIGN_BEGIN
#elif defined(__TASKING__) /* TASKING Compiler */
#define __ALIGN_BEGIN __align(4)
#endif /* __CC_ARM */
#endif
#endif /* __GNUC__ */
//#else
//  #define __ALIGN_BEGIN
//  #define __ALIGN_END
//#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

#define ZERO(x) memset(&(x), 0, sizeof(x))
#define BZERO(x) ::memset(&x, 0, sizeof(x))
#define STRINGIFY(X) #X
#define S(X) STRINGIFY(X)

#ifdef __cplusplus
class Sys {
    static char _hostname[30];
    static uint64_t _boot_time;

  public:
    static uint64_t _upTime;
    static void init();
    static uint64_t micros();
    static uint64_t millis();
    static uint32_t sec();
    static uint64_t now();
    static void setNow(uint64_t time);
    static void warn(int erc, const char* s);
    static void interruptEnable();
    static void interruptDisable();
    static void delayUs(uint32_t delay);
    static void delay(uint32_t msec);
    static void tick();
    static void hostname(const char* hn);
    static void setHostname(const char* hn);
    static const char* hostname();
    static uint32_t getFreeHeap();
    static uint32_t getSerialId();
    static const char* getProcessor();
    static const char* getBuild();
    static const char* board();
    static const char* cpu();

  protected:
  private:
};
#endif
#ifdef __cplusplus
extern "C" {
#endif
uint64_t SysMillis();
#ifdef __cplusplus
}
#endif

#endif // SYS_H
