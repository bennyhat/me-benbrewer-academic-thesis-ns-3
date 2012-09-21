################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/click/model/ipv4-click-routing.cc \
../src/click/model/ipv4-l3-click-protocol.cc 

OBJS += \
./src/click/model/ipv4-click-routing.o \
./src/click/model/ipv4-l3-click-protocol.o 

CC_DEPS += \
./src/click/model/ipv4-click-routing.d \
./src/click/model/ipv4-l3-click-protocol.d 


# Each subdirectory must supply rules for building sources it contributes
src/click/model/%.o: ../src/click/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


