################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/BLDCSensorless.c" \

C_SRCS += \
../Sources/BLDCSensorless.c \

OBJS += \
./Sources/BLDCSensorless.o \

C_DEPS += \
./Sources/BLDCSensorless.d \

OBJS_QUOTED += \
"./Sources/BLDCSensorless.o" \

C_DEPS_QUOTED += \
"./Sources/BLDCSensorless.d" \

OBJS_OS_FORMAT += \
./Sources/BLDCSensorless.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/BLDCSensorless.o: ../Sources/BLDCSensorless.c
	@echo 'Building file: $<'
	@echo 'Executing target #17 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/BLDCSensorless.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/BLDCSensorless.o"
	@echo 'Finished building: $<'
	@echo ' '


