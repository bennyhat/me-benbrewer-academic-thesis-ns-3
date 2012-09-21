################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../build/.conf_check_1d0477c52973e128d652160e9a5e4f87/test.cpp 

OBJS += \
./build/.conf_check_1d0477c52973e128d652160e9a5e4f87/test.o 

CPP_DEPS += \
./build/.conf_check_1d0477c52973e128d652160e9a5e4f87/test.d 


# Each subdirectory must supply rules for building sources it contributes
build/.conf_check_1d0477c52973e128d652160e9a5e4f87/%.o: ../build/.conf_check_1d0477c52973e128d652160e9a5e4f87/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


