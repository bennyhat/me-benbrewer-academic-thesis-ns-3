################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/aodv/model/aodv-dpd.cc \
../src/aodv/model/aodv-id-cache.cc \
../src/aodv/model/aodv-neighbor.cc \
../src/aodv/model/aodv-packet.cc \
../src/aodv/model/aodv-routing-protocol.cc \
../src/aodv/model/aodv-rqueue.cc \
../src/aodv/model/aodv-rtable.cc 

OBJS += \
./src/aodv/model/aodv-dpd.o \
./src/aodv/model/aodv-id-cache.o \
./src/aodv/model/aodv-neighbor.o \
./src/aodv/model/aodv-packet.o \
./src/aodv/model/aodv-routing-protocol.o \
./src/aodv/model/aodv-rqueue.o \
./src/aodv/model/aodv-rtable.o 

CC_DEPS += \
./src/aodv/model/aodv-dpd.d \
./src/aodv/model/aodv-id-cache.d \
./src/aodv/model/aodv-neighbor.d \
./src/aodv/model/aodv-packet.d \
./src/aodv/model/aodv-routing-protocol.d \
./src/aodv/model/aodv-rqueue.d \
./src/aodv/model/aodv-rtable.d 


# Each subdirectory must supply rules for building sources it contributes
src/aodv/model/%.o: ../src/aodv/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


