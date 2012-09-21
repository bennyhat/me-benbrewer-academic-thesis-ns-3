################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/olsr/examples/olsr-hna.cc \
../src/olsr/examples/simple-point-to-point-olsr.cc 

OBJS += \
./src/olsr/examples/olsr-hna.o \
./src/olsr/examples/simple-point-to-point-olsr.o 

CC_DEPS += \
./src/olsr/examples/olsr-hna.d \
./src/olsr/examples/simple-point-to-point-olsr.d 


# Each subdirectory must supply rules for building sources it contributes
src/olsr/examples/%.o: ../src/olsr/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


