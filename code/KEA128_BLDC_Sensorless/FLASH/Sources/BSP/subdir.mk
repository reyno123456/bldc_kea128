################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/BSP/MC33937_routines.c" \

C_SRCS += \
../Sources/BSP/MC33937_routines.c \

OBJS += \
./Sources/BSP/MC33937_routines.o \

C_DEPS += \
./Sources/BSP/MC33937_routines.d \

OBJS_QUOTED += \
"./Sources/BSP/MC33937_routines.o" \

C_DEPS_QUOTED += \
"./Sources/BSP/MC33937_routines.d" \

OBJS_OS_FORMAT += \
./Sources/BSP/MC33937_routines.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/BSP/MC33937_routines.o: ../Sources/BSP/MC33937_routines.c
	@echo 'Building file: $<'
	@echo 'Executing target #16 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/BSP/MC33937_routines.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/BSP/MC33937_routines.o"
	@echo 'Finished building: $<'
	@echo ' '


