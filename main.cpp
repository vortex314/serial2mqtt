#include <stdio.h>
#include <Log.h>
#include <Config.h>
#include <Serial2Mqtt.h>
#include <thread>

Log logger(2048);
Config config;
#define MAX_PORT	2

    Serial2Mqtt serial2mqtt[MAX_PORT];

int main(int argc, char **argv)
{

    std::thread threads[MAX_PORT];
    
    
    Sys::init();
    INFO("version : " __DATE__ " " __TIME__ "\n");
    config.setFile("/home/lieven/workspace/serial2mqtt/config.json");
    config.load();
    uint64_t upTime=Sys::millis();
    config.setNameSpace("system");
    config.set("upTime",upTime);
    config.set("pid",getpid());


    config.setNameSpace("serial");

    for(int i=0; i<MAX_PORT; i++) {
        std::string port="/dev/ttyUSB"+std::to_string(i);
        INFO(" configuring port : %s",port.c_str());
        config.set("port",port.c_str());
        serial2mqtt[i].setSerialPort(port.c_str());
        serial2mqtt[i].setConfig(config);
        serial2mqtt[i].init();
    }

    config.save();

    for(int i=0; i<MAX_PORT; i++) {
        threads[i] = std::thread([=]() {
            INFO(" starting thread %d",i);
            serial2mqtt[i].run();
            INFO(" stopping thread %d",i);
        });
    }


    sleep(100000000);

    return 0;
}
