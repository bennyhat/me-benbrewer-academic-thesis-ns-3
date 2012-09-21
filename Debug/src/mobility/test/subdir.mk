################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/mobility/test/ns2-mobility-helper-test-suite.cc \
../src/mobility/test/steady-state-random-waypoint-mobility-model-test.cc \
../src/mobility/test/waypoint-mobility-model-test.cc 

OBJS += \
./src/mobility/test/ns2-mobility-helper-test-suite.o \
./src/mobility/test/steady-state-random-waypoint-mobility-model-test.o \
./src/mobility/test/waypoint-mobility-model-test.o 

CC_DEPS += \
./src/mobility/test/ns2-mobility-helper-test-suite.d \
./src/mobility/test/steady-state-random-waypoint-mobility-model-test.d \
./src/mobility/test/waypoint-mobility-model-test.d 


# Each subdirectory must supply rules for building sources it contributes
src/mobility/test/%.o: ../src/mobility/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


