################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/network/helper/application-container.cc \
../src/network/helper/click-node-container.cc \
../src/network/helper/net-device-container.cc \
../src/network/helper/node-container.cc \
../src/network/helper/packet-socket-helper.cc \
../src/network/helper/trace-helper.cc 

OBJS += \
./src/network/helper/application-container.o \
./src/network/helper/click-node-container.o \
./src/network/helper/net-device-container.o \
./src/network/helper/node-container.o \
./src/network/helper/packet-socket-helper.o \
./src/network/helper/trace-helper.o 

CC_DEPS += \
./src/network/helper/application-container.d \
./src/network/helper/click-node-container.d \
./src/network/helper/net-device-container.d \
./src/network/helper/node-container.d \
./src/network/helper/packet-socket-helper.d \
./src/network/helper/trace-helper.d 


# Each subdirectory must supply rules for building sources it contributes
src/network/helper/%.o: ../src/network/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


