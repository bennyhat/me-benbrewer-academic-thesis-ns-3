################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/buildings/test/building-position-allocator-test.cc \
../src/buildings/test/buildings-helper-test.cc \
../src/buildings/test/buildings-pathloss-test.cc \
../src/buildings/test/buildings-shadowing-test.cc 

OBJS += \
./src/buildings/test/building-position-allocator-test.o \
./src/buildings/test/buildings-helper-test.o \
./src/buildings/test/buildings-pathloss-test.o \
./src/buildings/test/buildings-shadowing-test.o 

CC_DEPS += \
./src/buildings/test/building-position-allocator-test.d \
./src/buildings/test/buildings-helper-test.d \
./src/buildings/test/buildings-pathloss-test.d \
./src/buildings/test/buildings-shadowing-test.d 


# Each subdirectory must supply rules for building sources it contributes
src/buildings/test/%.o: ../src/buildings/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

