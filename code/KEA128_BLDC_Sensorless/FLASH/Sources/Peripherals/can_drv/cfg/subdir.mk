################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Peripherals/can_drv/cfg/Can_cfg.c" \

C_SRCS += \
../Sources/Peripherals/can_drv/cfg/Can_cfg.c \

OBJS += \
./Sources/Peripherals/can_drv/cfg/Can_cfg.o \

C_DEPS += \
./Sources/Peripherals/can_drv/cfg/Can_cfg.d \

OBJS_QUOTED += \
"./Sources/Peripherals/can_drv/cfg/Can_cfg.o" \

C_DEPS_QUOTED += \
"./Sources/Peripherals/can_drv/cfg/Can_cfg.d" \

OBJS_OS_FORMAT += \
./Sources/Peripherals/can_drv/cfg/Can_cfg.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/Peripherals/can_drv/cfg/Can_cfg.o: ../Sources/Peripherals/can_drv/cfg/Can_cfg.c
	@echo 'Building file: $<'
	@echo 'Executing target #13 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Peripherals/can_drv/cfg/Can_cfg.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Peripherals/can_drv/cfg/Can_cfg.o"
	@echo 'Finished building: $<'
	@echo ' '


