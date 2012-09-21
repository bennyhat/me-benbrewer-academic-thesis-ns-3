################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/core/examples/main-callback.cc \
../src/core/examples/main-ptr.cc \
../src/core/examples/main-random-variable.cc \
../src/core/examples/main-test-sync.cc \
../src/core/examples/sample-random-variable.cc \
../src/core/examples/sample-simulator.cc 

OBJS += \
./src/core/examples/main-callback.o \
./src/core/examples/main-ptr.o \
./src/core/examples/main-random-variable.o \
./src/core/examples/main-test-sync.o \
./src/core/examples/sample-random-variable.o \
./src/core/examples/sample-simulator.o 

CC_DEPS += \
./src/core/examples/main-callback.d \
./src/core/examples/main-ptr.d \
./src/core/examples/main-random-variable.d \
./src/core/examples/main-test-sync.d \
./src/core/examples/sample-random-variable.d \
./src/core/examples/sample-simulator.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/examples/%.o: ../src/core/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


