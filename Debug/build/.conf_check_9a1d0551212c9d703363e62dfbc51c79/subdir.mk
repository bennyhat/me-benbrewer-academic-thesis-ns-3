################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../build/.conf_check_9a1d0551212c9d703363e62dfbc51c79/test.cpp 

OBJS += \
./build/.conf_check_9a1d0551212c9d703363e62dfbc51c79/test.o 

CPP_DEPS += \
./build/.conf_check_9a1d0551212c9d703363e62dfbc51c79/test.d 


# Each subdirectory must supply rules for building sources it contributes
build/.conf_check_9a1d0551212c9d703363e62dfbc51c79/%.o: ../build/.conf_check_9a1d0551212c9d703363e62dfbc51c79/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


