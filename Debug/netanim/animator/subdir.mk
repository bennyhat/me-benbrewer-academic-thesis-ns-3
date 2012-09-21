################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../netanim/animator/animatormode.cpp \
../netanim/animator/animatorscene.cpp \
../netanim/animator/animatorview.cpp \
../netanim/animator/animlink.cpp \
../netanim/animator/animnode.cpp \
../netanim/animator/animpacket.cpp \
../netanim/animator/animxmlparser.cpp \
../netanim/animator/linkupdatemanager.cpp \
../netanim/animator/main.cpp \
../netanim/animator/mobilitymanager.cpp \
../netanim/animator/nodepositionstatisticsdialog.cpp \
../netanim/animator/nodetrajectorydialog.cpp \
../netanim/animator/nodeupdatemanager.cpp \
../netanim/animator/packetstatisticsdialog.cpp 

OBJS += \
./netanim/animator/animatormode.o \
./netanim/animator/animatorscene.o \
./netanim/animator/animatorview.o \
./netanim/animator/animlink.o \
./netanim/animator/animnode.o \
./netanim/animator/animpacket.o \
./netanim/animator/animxmlparser.o \
./netanim/animator/linkupdatemanager.o \
./netanim/animator/main.o \
./netanim/animator/mobilitymanager.o \
./netanim/animator/nodepositionstatisticsdialog.o \
./netanim/animator/nodetrajectorydialog.o \
./netanim/animator/nodeupdatemanager.o \
./netanim/animator/packetstatisticsdialog.o 

CPP_DEPS += \
./netanim/animator/animatormode.d \
./netanim/animator/animatorscene.d \
./netanim/animator/animatorview.d \
./netanim/animator/animlink.d \
./netanim/animator/animnode.d \
./netanim/animator/animpacket.d \
./netanim/animator/animxmlparser.d \
./netanim/animator/linkupdatemanager.d \
./netanim/animator/main.d \
./netanim/animator/mobilitymanager.d \
./netanim/animator/nodepositionstatisticsdialog.d \
./netanim/animator/nodetrajectorydialog.d \
./netanim/animator/nodeupdatemanager.d \
./netanim/animator/packetstatisticsdialog.d 


# Each subdirectory must supply rules for building sources it contributes
netanim/animator/%.o: ../netanim/animator/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


