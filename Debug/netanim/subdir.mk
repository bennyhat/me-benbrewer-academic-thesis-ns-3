################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../netanim/animatormode.o \
../netanim/animatorscene.o \
../netanim/animatorview.o \
../netanim/animlink.o \
../netanim/animnode.o \
../netanim/animpacket.o \
../netanim/animxmlparser.o \
../netanim/linkupdatemanager.o \
../netanim/main.o \
../netanim/mobilitymanager.o \
../netanim/moc_animatormode.o \
../netanim/moc_animatorscene.o \
../netanim/netanim.o \
../netanim/nodepositionstatisticsdialog.o \
../netanim/nodetrajectorydialog.o \
../netanim/nodeupdatemanager.o \
../netanim/packetstatisticsdialog.o \
../netanim/qrc_animator.o \
../netanim/xdebug.o 

CPP_SRCS += \
../netanim/moc_animatormode.cpp \
../netanim/moc_animatorscene.cpp \
../netanim/qrc_animator.cpp 

OBJS += \
./netanim/moc_animatormode.o \
./netanim/moc_animatorscene.o \
./netanim/qrc_animator.o 

CPP_DEPS += \
./netanim/moc_animatormode.d \
./netanim/moc_animatorscene.d \
./netanim/qrc_animator.d 


# Each subdirectory must supply rules for building sources it contributes
netanim/%.o: ../netanim/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


