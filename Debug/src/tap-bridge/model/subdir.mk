################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/tap-bridge/model/tap-bridge.cc \
../src/tap-bridge/model/tap-creator.cc \
../src/tap-bridge/model/tap-encode-decode.cc 

OBJS += \
./src/tap-bridge/model/tap-bridge.o \
./src/tap-bridge/model/tap-creator.o \
./src/tap-bridge/model/tap-encode-decode.o 

CC_DEPS += \
./src/tap-bridge/model/tap-bridge.d \
./src/tap-bridge/model/tap-creator.d \
./src/tap-bridge/model/tap-encode-decode.d 


# Each subdirectory must supply rules for building sources it contributes
src/tap-bridge/model/%.o: ../src/tap-bridge/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


