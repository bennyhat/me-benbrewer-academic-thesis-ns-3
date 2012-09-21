################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../scratch/dsdv-manet-test-non-dsdvmobiles.cc \
../scratch/exponential-generator.cc \
../scratch/scenario-random-udp.cc \
../scratch/scenario-specific-udp.cc \
../scratch/scenario-txop-demo.cc \
../scratch/scratch-simulator.cc \
../scratch/sr2-test.cc \
../scratch/stats-voice-txop-udp.cc 

OBJS += \
./scratch/dsdv-manet-test-non-dsdvmobiles.o \
./scratch/exponential-generator.o \
./scratch/scenario-random-udp.o \
./scratch/scenario-specific-udp.o \
./scratch/scenario-txop-demo.o \
./scratch/scratch-simulator.o \
./scratch/sr2-test.o \
./scratch/stats-voice-txop-udp.o 

CC_DEPS += \
./scratch/dsdv-manet-test-non-dsdvmobiles.d \
./scratch/exponential-generator.d \
./scratch/scenario-random-udp.d \
./scratch/scenario-specific-udp.d \
./scratch/scenario-txop-demo.d \
./scratch/scratch-simulator.d \
./scratch/sr2-test.d \
./scratch/stats-voice-txop-udp.d 


# Each subdirectory must supply rules for building sources it contributes
scratch/%.o: ../scratch/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


