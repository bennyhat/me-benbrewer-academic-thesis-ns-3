################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/wifi/helper/adhoc-txop-wifi-mac.cc \
../src/wifi/helper/athstats-helper.cc \
../src/wifi/helper/edca-txop-cfb-n.cc \
../src/wifi/helper/nqos-wifi-mac-helper.cc \
../src/wifi/helper/qos-txop-wifi-mac-helper.cc \
../src/wifi/helper/qos-wifi-mac-helper.cc \
../src/wifi/helper/wifi-helper.cc \
../src/wifi/helper/yans-wifi-helper.cc 

OBJS += \
./src/wifi/helper/adhoc-txop-wifi-mac.o \
./src/wifi/helper/athstats-helper.o \
./src/wifi/helper/edca-txop-cfb-n.o \
./src/wifi/helper/nqos-wifi-mac-helper.o \
./src/wifi/helper/qos-txop-wifi-mac-helper.o \
./src/wifi/helper/qos-wifi-mac-helper.o \
./src/wifi/helper/wifi-helper.o \
./src/wifi/helper/yans-wifi-helper.o 

CC_DEPS += \
./src/wifi/helper/adhoc-txop-wifi-mac.d \
./src/wifi/helper/athstats-helper.d \
./src/wifi/helper/edca-txop-cfb-n.d \
./src/wifi/helper/nqos-wifi-mac-helper.d \
./src/wifi/helper/qos-txop-wifi-mac-helper.d \
./src/wifi/helper/qos-wifi-mac-helper.d \
./src/wifi/helper/wifi-helper.d \
./src/wifi/helper/yans-wifi-helper.d 


# Each subdirectory must supply rules for building sources it contributes
src/wifi/helper/%.o: ../src/wifi/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


