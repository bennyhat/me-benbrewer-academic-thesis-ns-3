################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/flow-monitor/model/flow-classifier.cc \
../src/flow-monitor/model/flow-monitor.cc \
../src/flow-monitor/model/flow-probe.cc \
../src/flow-monitor/model/histogram.cc \
../src/flow-monitor/model/ipv4-flow-classifier.cc \
../src/flow-monitor/model/ipv4-flow-probe.cc 

OBJS += \
./src/flow-monitor/model/flow-classifier.o \
./src/flow-monitor/model/flow-monitor.o \
./src/flow-monitor/model/flow-probe.o \
./src/flow-monitor/model/histogram.o \
./src/flow-monitor/model/ipv4-flow-classifier.o \
./src/flow-monitor/model/ipv4-flow-probe.o 

CC_DEPS += \
./src/flow-monitor/model/flow-classifier.d \
./src/flow-monitor/model/flow-monitor.d \
./src/flow-monitor/model/flow-probe.d \
./src/flow-monitor/model/histogram.d \
./src/flow-monitor/model/ipv4-flow-classifier.d \
./src/flow-monitor/model/ipv4-flow-probe.d 


# Each subdirectory must supply rules for building sources it contributes
src/flow-monitor/model/%.o: ../src/flow-monitor/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

