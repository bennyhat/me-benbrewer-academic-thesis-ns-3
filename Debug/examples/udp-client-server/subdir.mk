################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../examples/udp-client-server/udp-client-server.cc \
../examples/udp-client-server/udp-trace-client-server.cc 

OBJS += \
./examples/udp-client-server/udp-client-server.o \
./examples/udp-client-server/udp-trace-client-server.o 

CC_DEPS += \
./examples/udp-client-server/udp-client-server.d \
./examples/udp-client-server/udp-trace-client-server.d 


# Each subdirectory must supply rules for building sources it contributes
examples/udp-client-server/%.o: ../examples/udp-client-server/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


