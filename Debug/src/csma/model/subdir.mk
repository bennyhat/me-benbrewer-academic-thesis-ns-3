################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/csma/model/backoff.cc \
../src/csma/model/csma-channel.cc \
../src/csma/model/csma-net-device.cc 

OBJS += \
./src/csma/model/backoff.o \
./src/csma/model/csma-channel.o \
./src/csma/model/csma-net-device.o 

CC_DEPS += \
./src/csma/model/backoff.d \
./src/csma/model/csma-channel.d \
./src/csma/model/csma-net-device.d 


# Each subdirectory must supply rules for building sources it contributes
src/csma/model/%.o: ../src/csma/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


