################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../build/.conf_check_b164aaefc1269589e15986e29a28fe64/test.cpp 

OBJS += \
./build/.conf_check_b164aaefc1269589e15986e29a28fe64/test.o 

CPP_DEPS += \
./build/.conf_check_b164aaefc1269589e15986e29a28fe64/test.d 


# Each subdirectory must supply rules for building sources it contributes
build/.conf_check_b164aaefc1269589e15986e29a28fe64/%.o: ../build/.conf_check_b164aaefc1269589e15986e29a28fe64/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


