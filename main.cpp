#include <stdio.h>
#include <Log.h>
#include <Config.h>
#include <Serial2Mqtt.h>
#include <thread>


#define DEFAULT_CONFIG "serial2mqtt.json"

void overrideConfig(Config& config,int argc, char **argv);


Log logger(2048);
//Config config;
#define MAX_PORT	20

std::string logFile="";
FILE* logFd=0;

void myLogFunction(char* s,uint32_t length)
{
    fprintf(logFd,"%s\n",s);
    fflush(logFd);
    fprintf(stdout,"%s\r\n",s);

}



Serial2Mqtt serial2mqtt[MAX_PORT];

void SetThreadName(std::thread* thread, const char* threadName)
{
    auto handle = thread->native_handle();
    pthread_setname_np(handle,threadName);
}

int main(int argc, char **argv)
{

    std::thread threads[MAX_PORT];
    StaticJsonDocument<10240> jsonDoc;

    Sys::init();
    INFO("build : " __DATE__ " " __TIME__ );
    char cwd[256];
    getcwd(cwd,sizeof(cwd));
    INFO("current directory : %s ",cwd);
    if ( argc > 1 ) {
        INFO(" loading config file : %s ",argv[1]);
        config.loadFile(argv[1]);
    } else {
        INFO(" load default config : %s",DEFAULT_CONFIG);
        config.loadFile(DEFAULT_CONFIG);
    }
    overrideConfig(config,argc,argv);
    if ( logFile.length()>0 ) {
        INFO(" logging to file %s ", logFile.c_str());
        logFd=fopen(logFile.c_str(),"w");
        if ( logFd==NULL ) {
            WARN(" open logfile %s failed : %d %s ",logFile.c_str(),errno, strerror(errno));
        } else {
            logger.writer(myLogFunction);
        }
    }

    config.setNameSpace("serial");
    JsonArray ports = config.root()["serial"]["ports"].as<JsonArray>() ;
//	JsonArray		ports = jsonDoc.as<JsonArray>();

    if ( ports.isNull() ) {
        jsonDoc.add("/dev/ttyUSB0");
        ports = jsonDoc.as<JsonArray>();
    }


    for(uint32_t i=0; i<ports.size(); i++) {
        std::string port=ports[i];
        INFO(" configuring port : %s",port.c_str());
        serial2mqtt[i].setSerialPort(port.c_str());
        serial2mqtt[i].setConfig(config);
        serial2mqtt[i].setLogFd(logFd);
        serial2mqtt[i].init();
    }

    for(uint32_t i=0; i<ports.size(); i++) {
        threads[i] = std::thread([=]() {
            INFO(" starting thread %d",i);
            serial2mqtt[i].run();
        });
        std::string port= ports[i];
        SetThreadName(&threads[i],port.substr(8).c_str());
    }


    sleep(UINT32_MAX); // UINT32_MAX to sleep 'forever'
    exit(0);

    return 0;
}


void overrideConfig(Config& config,int argc, char **argv)
{
    int  opt;

    while ((opt = getopt(argc, argv, "f:m:l:v:")) != -1) {
        switch (opt) {
        case 'm':
            config.setNameSpace("mqtt");
            config.set("host",optarg);
            break;
        case 'f':
            config.loadFile(optarg);
            break;
        case 'v': {
            char logLevel = optarg[0];
            logger.setLogLevel(logLevel);
            break;
        }
        case 'l':
            logFile=optarg;
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s [-v(TDIWE)] [-f configFile] [-l logFile] [-m mqttHost]\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
    }
}
