#include "Timer.h"

Timer::Timer()
{
}

Timer::~Timer()
{
}

Timer& Timer::atInterval(uint32_t msec)
{
_repeat=true;
_active=true;
_interval=msec;
_expiresOn=Sys::millis()+_interval;
return *this;
}
Timer& Timer::atExpiry(uint64_t msec)
{
_repeat=false;
_active=true;
_expiresOn=msec;
return *this;
}

Timer& Timer::atDelta(uint32_t msec)
{
_repeat=false;
_active=true;
_expiresOn=Sys::millis()+msec;
return *this;
}

Timer& Timer::doThis(TimerHandler action){
    _action=action;
    return *this;
}

void Timer::check(){
//    INFO(" %lld %lld ",_expiresOn,Sys::millis());
    if ( _expiresOn < Sys::millis() && _active ) {
        _action();
        if ( _repeat ) {
            _expiresOn=Sys::millis()+_interval;
        } else {
            _active=false;
        }
    }
}
