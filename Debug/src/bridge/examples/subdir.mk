################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/bridge/examples/csma-bridge-one-hop.cc \
../src/bridge/examples/csma-bridge.cc 

OBJS += \
./src/bridge/examples/csma-bridge-one-hop.o \
./src/bridge/examples/csma-bridge.o 

CC_DEPS += \
./src/bridge/examples/csma-bridge-one-hop.d \
./src/bridge/examples/csma-bridge.d 


# Each subdirectory must supply rules for building sources it contributes
src/bridge/examples/%.o: ../src/bridge/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


