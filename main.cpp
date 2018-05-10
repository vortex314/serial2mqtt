#include <stdio.h>
#include <Log.h>
#include <Config.h>
#include <Serial2Mqtt.h>
#include <thread>

void overrideConfig(Config& config,int argc, char **argv);


Log logger(2048);
Config config;
#define MAX_PORT	20

Serial2Mqtt serial2mqtt[MAX_PORT];

int main(int argc, char **argv)
{

    std::thread threads[MAX_PORT];


    Sys::init();
    INFO("version : " __DATE__ " " __TIME__ "\n");
    INFO(" argv[0] : %s ",argv[0]);
    config.setFile("serial2mqtt.json");
    config.load();
    overrideConfig(config,argc,argv);
    config.setNameSpace("serial");
    json ports =  config.getJson("ports",json::parse("[\"/dev/ttyUSB0\"]"));


    for(uint32_t i=0; i<ports.size(); i++) {
        std::string port=ports[i];
        INFO(" configuring port : %s",port.c_str());
        serial2mqtt[i].setSerialPort(port.c_str());
        serial2mqtt[i].setConfig(config);
        serial2mqtt[i].init();
    }

    for(uint32_t i=0; i<ports.size(); i++) {
        threads[i] = std::thread([=]() {
            INFO(" starting thread %d",i);
            serial2mqtt[i].run();
        });
    }


    sleep(100000000);

    return 0;
}


void overrideConfig(Config& config,int argc, char **argv)
{
    int  opt;

    while ((opt = getopt(argc, argv, "f:m:")) != -1) {
        switch (opt) {
        case 'm':
            config.setNameSpace("mqtt");
            config.set("host",optarg);
            break;
        case 'f':
            config.setFile(optarg);
            config.load();
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s [-f configFile] [-m mqttHost]\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
    }
}
