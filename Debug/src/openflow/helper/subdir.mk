################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/openflow/helper/openflow-switch-helper.cc 

OBJS += \
./src/openflow/helper/openflow-switch-helper.o 

CC_DEPS += \
./src/openflow/helper/openflow-switch-helper.d 


# Each subdirectory must supply rules for building sources it contributes
src/openflow/helper/%.o: ../src/openflow/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


