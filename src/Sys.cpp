#include "Sys.h"

#include <malloc.h>
#include <unistd.h> // gethostname
#include <string.h>
#include <time.h>


char Sys::_hostname[] ;

uint64_t Sys::_upTime=0;

void Sys::init(){
gethostname(_hostname,30);
}

uint64_t Sys::millis()   // time in msec since boot, only increasing
{
    struct timespec deadline;
    clock_gettime(CLOCK_MONOTONIC, &deadline);
    Sys::_upTime= deadline.tv_sec*1000 + deadline.tv_nsec/1000000;
    return _upTime;
}

void Sys::delay(uint32_t time){
usleep(time*1000);
}


















