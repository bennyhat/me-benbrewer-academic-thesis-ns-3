################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/tools/test/average-test-suite.cc \
../src/tools/test/event-garbage-collector-test-suite.cc 

OBJS += \
./src/tools/test/average-test-suite.o \
./src/tools/test/event-garbage-collector-test-suite.o 

CC_DEPS += \
./src/tools/test/average-test-suite.d \
./src/tools/test/event-garbage-collector-test-suite.d 


# Each subdirectory must supply rules for building sources it contributes
src/tools/test/%.o: ../src/tools/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


