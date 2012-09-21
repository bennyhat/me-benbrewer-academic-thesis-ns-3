################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/mesh/test/mesh-information-element-vector-test-suite.cc 

OBJS += \
./src/mesh/test/mesh-information-element-vector-test-suite.o 

CC_DEPS += \
./src/mesh/test/mesh-information-element-vector-test-suite.d 


# Each subdirectory must supply rules for building sources it contributes
src/mesh/test/%.o: ../src/mesh/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


