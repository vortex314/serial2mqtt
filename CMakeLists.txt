cmake_minimum_required(VERSION 3.13.0)
project(serial2mqtt VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -g ")

find_package(OpenSSL REQUIRED)
add_definitions(-DLINUX -std=c++11)
add_executable(serial2mqtt)

target_sources(serial2mqtt PRIVATE 
    src/Serial2Mqtt.cpp 
    src/LogFile.cpp 
    src/Sys.cpp 
    src/Timer.cpp 
    src/main.cpp 
    src/Config.cpp 
    src/Bytes.cpp 
    src/Log.cpp 
    src/Str.cpp
    src/CircBuf.cpp 
    src/Sema.cpp 
    ) 

# added paho.mqtt.c/src for paho-mqtt-c header files
include_directories(src  
    ArduinoJson/src 
    paho.mqtt.c/src
    )

#Where to find the lib "libpaho-mqtt3as.a", libssl and libcrypto
link_directories(${CMAKE_SOURCE_DIR}/paho.mqtt.c/src /usr/local/lib)

# added libpaho-mqtt3as.a, libssl.a and libcrypto.a as we build in static mode
target_link_libraries(serial2mqtt 
    libpaho-mqtt3as.a
    -l:libssl.a
    -l:libcrypto.a
    -lpthread 
    -lrt 
    -pthread 
    -latomic )
