################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../utils/bench-packets.cc \
../utils/bench-simulator.cc \
../utils/print-introspected-doxygen.cc \
../utils/test-runner.cc 

OBJS += \
./utils/bench-packets.o \
./utils/bench-simulator.o \
./utils/print-introspected-doxygen.o \
./utils/test-runner.o 

CC_DEPS += \
./utils/bench-packets.d \
./utils/bench-simulator.d \
./utils/print-introspected-doxygen.d \
./utils/test-runner.d 


# Each subdirectory must supply rules for building sources it contributes
utils/%.o: ../utils/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

