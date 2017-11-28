################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Peripherals/can_drv/Can.c" \
"../Sources/Peripherals/can_drv/Can_Isr.c" \

C_SRCS += \
../Sources/Peripherals/can_drv/Can.c \
../Sources/Peripherals/can_drv/Can_Isr.c \

OBJS += \
./Sources/Peripherals/can_drv/Can.o \
./Sources/Peripherals/can_drv/Can_Isr.o \

C_DEPS += \
./Sources/Peripherals/can_drv/Can.d \
./Sources/Peripherals/can_drv/Can_Isr.d \

OBJS_QUOTED += \
"./Sources/Peripherals/can_drv/Can.o" \
"./Sources/Peripherals/can_drv/Can_Isr.o" \

C_DEPS_QUOTED += \
"./Sources/Peripherals/can_drv/Can.d" \
"./Sources/Peripherals/can_drv/Can_Isr.d" \

OBJS_OS_FORMAT += \
./Sources/Peripherals/can_drv/Can.o \
./Sources/Peripherals/can_drv/Can_Isr.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/Peripherals/can_drv/Can.o: ../Sources/Peripherals/can_drv/Can.c
	@echo 'Building file: $<'
	@echo 'Executing target #14 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Peripherals/can_drv/Can.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Peripherals/can_drv/Can.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Peripherals/can_drv/Can_Isr.o: ../Sources/Peripherals/can_drv/Can_Isr.c
	@echo 'Building file: $<'
	@echo 'Executing target #15 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Peripherals/can_drv/Can_Isr.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Peripherals/can_drv/Can_Isr.o"
	@echo 'Finished building: $<'
	@echo ' '


