cmake_minimum_required(VERSION 3.13.0)
project(serial2mqtt VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -g ")
find_package(eclipse-paho-mqtt-c REQUIRED)
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
    
include_directories(src  
    ArduinoJson/src 
    )

target_link_libraries(serial2mqtt 
    eclipse-paho-mqtt-c::paho-mqtt3as-static
    -lpthread 
    -lrt 
    -pthread 
    -latomic )
    
 
    
