################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/mobility/examples/main-grid-topology.cc \
../src/mobility/examples/main-random-topology.cc \
../src/mobility/examples/main-random-walk.cc 

OBJS += \
./src/mobility/examples/main-grid-topology.o \
./src/mobility/examples/main-random-topology.o \
./src/mobility/examples/main-random-walk.o 

CC_DEPS += \
./src/mobility/examples/main-grid-topology.d \
./src/mobility/examples/main-random-topology.d \
./src/mobility/examples/main-random-walk.d 


# Each subdirectory must supply rules for building sources it contributes
src/mobility/examples/%.o: ../src/mobility/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


