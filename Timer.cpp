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
    if (msec > 0) {
	_active=true;
	_interval=msec;
	_expiresOn=Sys::millis()+_interval;
    } else {
	_active=false;
    }
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
    if (msec > 0) {
	_active=true;
	_expiresOn=Sys::millis()+msec;
    } else {
	_active=false;
    }
    return *this;
}

Timer& Timer::doThis(TimerHandler action){
    _action=action;
    return *this;
}

void Timer::check(){
//    INFO("%lld %lld",_expiresOn,Sys::millis());
    if ( _expiresOn < Sys::millis() && _active ) {
        if ( _repeat ) {
            _expiresOn=Sys::millis()+_interval;
        } else {
            _active=false;
        }
        _action();
    }
}
