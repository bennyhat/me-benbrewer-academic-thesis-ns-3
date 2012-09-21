################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/bridge/model/bridge-channel.cc \
../src/bridge/model/bridge-net-device.cc 

OBJS += \
./src/bridge/model/bridge-channel.o \
./src/bridge/model/bridge-net-device.o 

CC_DEPS += \
./src/bridge/model/bridge-channel.d \
./src/bridge/model/bridge-net-device.d 


# Each subdirectory must supply rules for building sources it contributes
src/bridge/model/%.o: ../src/bridge/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


