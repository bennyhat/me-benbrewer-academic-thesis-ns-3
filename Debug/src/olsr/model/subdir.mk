################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/olsr/model/olsr-header.cc \
../src/olsr/model/olsr-routing-protocol.cc \
../src/olsr/model/olsr-state.cc 

OBJS += \
./src/olsr/model/olsr-header.o \
./src/olsr/model/olsr-routing-protocol.o \
./src/olsr/model/olsr-state.o 

CC_DEPS += \
./src/olsr/model/olsr-header.d \
./src/olsr/model/olsr-routing-protocol.d \
./src/olsr/model/olsr-state.d 


# Each subdirectory must supply rules for building sources it contributes
src/olsr/model/%.o: ../src/olsr/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


