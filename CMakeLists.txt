cmake_minimum_required(VERSION 3.13.0)
project(serial2mqtt VERSION 0.1.0)

# include(CTest)
# enable_testing()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -g ")
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


    # set(CPACK_PROJECT_NAME ${PROJECT_NAME})
    # set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
    set(GCC_FLAGS "-DLINUX")
    # include(CPack)
    
    add_definitions(${GCC_FLAGS})
    
    include_directories(src  
            ArduinoJson/src 
            paho.mqtt.c/src  
            )

    link_directories(paho.mqtt.c/src /home/lieven/workspace/serial2mqtt/paho.mqtt.c/src)

    target_link_libraries(serial2mqtt 
            -L/home/lieven/workspace/serial2mqtt/paho.mqtt.c/src
            -l:libpaho-mqtt3a.a
            -lpthread 
            -lrt 
            -pthread 
            -latomic )
    
 
    
