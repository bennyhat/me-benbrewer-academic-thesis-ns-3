################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/spectrum/helper/adhoc-aloha-noack-ideal-phy-helper.cc \
../src/spectrum/helper/spectrum-analyzer-helper.cc \
../src/spectrum/helper/spectrum-helper.cc \
../src/spectrum/helper/waveform-generator-helper.cc 

OBJS += \
./src/spectrum/helper/adhoc-aloha-noack-ideal-phy-helper.o \
./src/spectrum/helper/spectrum-analyzer-helper.o \
./src/spectrum/helper/spectrum-helper.o \
./src/spectrum/helper/waveform-generator-helper.o 

CC_DEPS += \
./src/spectrum/helper/adhoc-aloha-noack-ideal-phy-helper.d \
./src/spectrum/helper/spectrum-analyzer-helper.d \
./src/spectrum/helper/spectrum-helper.d \
./src/spectrum/helper/waveform-generator-helper.d 


# Each subdirectory must supply rules for building sources it contributes
src/spectrum/helper/%.o: ../src/spectrum/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

