################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/click/examples/nsclick-raw-wlan.cc \
../src/click/examples/nsclick-routing.cc \
../src/click/examples/nsclick-simple-lan.cc \
../src/click/examples/nsclick-udp-client-server-csma.cc \
../src/click/examples/nsclick-udp-client-server-wifi.cc 

OBJS += \
./src/click/examples/nsclick-raw-wlan.o \
./src/click/examples/nsclick-routing.o \
./src/click/examples/nsclick-simple-lan.o \
./src/click/examples/nsclick-udp-client-server-csma.o \
./src/click/examples/nsclick-udp-client-server-wifi.o 

CC_DEPS += \
./src/click/examples/nsclick-raw-wlan.d \
./src/click/examples/nsclick-routing.d \
./src/click/examples/nsclick-simple-lan.d \
./src/click/examples/nsclick-udp-client-server-csma.d \
./src/click/examples/nsclick-udp-client-server-wifi.d 


# Each subdirectory must supply rules for building sources it contributes
src/click/examples/%.o: ../src/click/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

