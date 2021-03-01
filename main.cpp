#include <stdio.h>
#include <Log.h>
#include <Config.h>
#include <Serial2Mqtt.h>
#include <thread>

#define DEFAULT_CONFIG "serial2mqtt.json"

void commandArguments(JsonObject, int argc, char **argv);

Log logger(2048);
//Config config;
#define MAX_PORT 20


Serial2Mqtt serial2mqtt[MAX_PORT];

void SetThreadName(std::thread *thread, const char *threadName)
{
    auto handle = thread->native_handle();
    pthread_setname_np(handle, threadName);
}

#include <LogFile.h>
LogFile logFile("wiringMqtt", 5, 2000000);

int main(int argc, char **argv)
{

    std::thread threads[MAX_PORT];
    StaticJsonDocument<10240> jsonDoc;

    Sys::init();
    INFO("build : " __DATE__ " " __TIME__);
    char cwd[256];
    getcwd(cwd, sizeof(cwd));
    INFO("current directory : %s", cwd);
    if (argc > 1)
    {
        INFO("loading config file : %s", argv[1]);
        config.loadFile(argv[1]);
    }
    else
    {
        INFO("load default config : %s", DEFAULT_CONFIG);
        config.loadFile(DEFAULT_CONFIG);
    }
    commandArguments(config.root(), argc, argv);

    JsonObject logConfig = config.root()["log"];
    std::string level = logConfig["level"] | "I";
    logger.setLogLevel(level[0]);
    if (logConfig["file"])
    {
        std::string prefix = logConfig["file"];
        logFile.prefix(prefix.c_str());
        logger.writer(
            [](char *line, unsigned int length) { logFile.append(line, length); });
        if (logConfig["console"])
        {
            bool consoleOn = logConfig["console"];
            logFile.console(consoleOn);
        }
    }
    INFO(" joystick2mqtt started. Build : %s ", __DATE__ " " __TIME__);

    config.setNameSpace("serial");
    JsonArray ports = config.root()["serial"]["ports"].as<JsonArray>();
    //	JsonArray		ports = jsonDoc.as<JsonArray>();

    if (ports.isNull())
    {
        jsonDoc.add("/dev/ttyUSB0");
        ports = jsonDoc.as<JsonArray>();
    }

    for (uint32_t i = 0; i < ports.size(); i++)
    {
        std::string port = ports[i];
        INFO("configuring port : %s", port.c_str());
        serial2mqtt[i].setSerialPort(port.c_str());
        serial2mqtt[i].setConfig(config);
        serial2mqtt[i].init();
    }

    for (uint32_t i = 0; i < ports.size(); i++)
    {
        threads[i] = std::thread([=]() {
            INFO("starting thread %d", i);
            serial2mqtt[i].run();
        });
        SetThreadName(&threads[i], serial2mqtt[i].getSerialPortShort().c_str());
    }

    sleep(UINT32_MAX); // UINT32_MAX to sleep 'forever'
    exit(0);

    return 0;
}

void commandArguments(JsonObject config, int argc, char **argv) {
  int opt;

  while ((opt = getopt(argc, argv, "f:m:l:v:")) != -1) {
    switch (opt) {
      case 'm':
        config["mqtt"]["connection"] = optarg;
        break;
      case 'f':
        config["configFile"] = optarg;
        break;
      case 'v': {
        std::string logLevel;
        logLevel += optarg[0];
        config["log"]["level"] = logLevel;
        break;
      }
      case 'l':
        config["log"]["file"] = optarg;
        break;
      default: /* '?' */
        fprintf(stderr,
                "Usage: %s [-v(TDIWE)] [-f configFile] [-l logFile] [-m "
                "mqtt_connection]\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }
  }
}
