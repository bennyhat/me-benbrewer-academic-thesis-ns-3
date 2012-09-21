################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../netanim/designer/abstractdesigneritem.cpp \
../netanim/designer/applicationswidget.cpp \
../netanim/designer/appmanager.cpp \
../netanim/designer/attributeipv4addrportwidget.cpp \
../netanim/designer/attributeprotocolwidget.cpp \
../netanim/designer/attributerandomvariablewidget.cpp \
../netanim/designer/designermode.cpp \
../netanim/designer/designernoderect.cpp \
../netanim/designer/designerp2plinkline.cpp \
../netanim/designer/designerscene.cpp \
../netanim/designer/designerview.cpp \
../netanim/designer/nodenetworkwidget.cpp \
../netanim/designer/nodepropertiestab.cpp \
../netanim/designer/p2plinkmanager.cpp 

OBJS += \
./netanim/designer/abstractdesigneritem.o \
./netanim/designer/applicationswidget.o \
./netanim/designer/appmanager.o \
./netanim/designer/attributeipv4addrportwidget.o \
./netanim/designer/attributeprotocolwidget.o \
./netanim/designer/attributerandomvariablewidget.o \
./netanim/designer/designermode.o \
./netanim/designer/designernoderect.o \
./netanim/designer/designerp2plinkline.o \
./netanim/designer/designerscene.o \
./netanim/designer/designerview.o \
./netanim/designer/nodenetworkwidget.o \
./netanim/designer/nodepropertiestab.o \
./netanim/designer/p2plinkmanager.o 

CPP_DEPS += \
./netanim/designer/abstractdesigneritem.d \
./netanim/designer/applicationswidget.d \
./netanim/designer/appmanager.d \
./netanim/designer/attributeipv4addrportwidget.d \
./netanim/designer/attributeprotocolwidget.d \
./netanim/designer/attributerandomvariablewidget.d \
./netanim/designer/designermode.d \
./netanim/designer/designernoderect.d \
./netanim/designer/designerp2plinkline.d \
./netanim/designer/designerscene.d \
./netanim/designer/designerview.d \
./netanim/designer/nodenetworkwidget.d \
./netanim/designer/nodepropertiestab.d \
./netanim/designer/p2plinkmanager.d 


# Each subdirectory must supply rules for building sources it contributes
netanim/designer/%.o: ../netanim/designer/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ben/workspace/ns-3.14/build" -I"/home/ben/workspace/click" -I/usr/include/python2.7 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


