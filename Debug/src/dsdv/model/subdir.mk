################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/dsdv/model/dsdv-packet-queue.cc \
../src/dsdv/model/dsdv-packet.cc \
../src/dsdv/model/dsdv-routing-protocol.cc \
../src/dsdv/model/dsdv-rtable.cc 

OBJS += \
./src/dsdv/model/dsdv-packet-queue.o \
./src/dsdv/model/dsdv-packet.o \
./src/dsdv/model/dsdv-routing-protocol.o \
./src/dsdv/model/dsdv-rtable.o 

CC_DEPS += \
./src/dsdv/model/dsdv-packet-queue.d \
./src/dsdv/model/dsdv-packet.d \
./src/dsdv/model/dsdv-routing-protocol.d \
./src/dsdv/model/dsdv-rtable.d 


# Each subdirectory must supply rules for building sources it contributes
src/dsdv/model/%.o: ../src/dsdv/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


