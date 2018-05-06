#ifndef TIMER_H
#define TIMER_H
#include <functional>
#include <stdint.h>
#include <Sys.h>
#include <Log.h>
#define TimerHandler std::function<void ()>
class Timer
{
    uint32_t _interval;
    uint64_t _expiresOn;
    bool _repeat;
    bool _active;
    TimerHandler _action;
    
public:
    Timer();
    ~Timer();
    Timer& atInterval(uint32_t msec);
    Timer& atExpiry(uint64_t msec);
    Timer& doThis(TimerHandler);
    void check();
};

#endif // TIMER_H
