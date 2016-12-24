################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/MqttClient.cpp \
../src/Sys.cpp \
../src/Tcp.cpp \
../src/Usb.cpp \
../src/keys.cpp \
../src/main.cpp 

OBJS += \
./src/MqttClient.o \
./src/Sys.o \
./src/Tcp.o \
./src/Usb.o \
./src/keys.o \
./src/main.o 

CPP_DEPS += \
./src/MqttClient.d \
./src/Sys.d \
./src/Tcp.d \
./src/Usb.d \
./src/keys.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -I"/home/lieven/linux/Common" -I"/home/lieven/linux/paho/src" -I"/home/lieven/linux/serial2mqtt/inc" -I"/home/lieven/linux/Common/inc" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


