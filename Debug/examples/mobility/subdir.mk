################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../examples/mobility/ns2-mobility-trace.cc 

OBJS += \
./examples/mobility/ns2-mobility-trace.o 

CC_DEPS += \
./examples/mobility/ns2-mobility-trace.d 


# Each subdirectory must supply rules for building sources it contributes
examples/mobility/%.o: ../examples/mobility/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


