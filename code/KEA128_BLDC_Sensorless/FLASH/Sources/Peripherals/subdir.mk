################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Peripherals/acmp.c" \
"../Sources/Peripherals/adc.c" \
"../Sources/Peripherals/ftm.c" \
"../Sources/Peripherals/gpio.c" \
"../Sources/Peripherals/ics.c" \
"../Sources/Peripherals/nvic.c" \
"../Sources/Peripherals/osc.c" \
"../Sources/Peripherals/pit.c" \
"../Sources/Peripherals/rtc.c" \
"../Sources/Peripherals/sim.c" \
"../Sources/Peripherals/spi.c" \
"../Sources/Peripherals/uart.c" \

C_SRCS += \
../Sources/Peripherals/acmp.c \
../Sources/Peripherals/adc.c \
../Sources/Peripherals/ftm.c \
../Sources/Peripherals/gpio.c \
../Sources/Peripherals/ics.c \
../Sources/Peripherals/nvic.c \
../Sources/Peripherals/osc.c \
../Sources/Peripherals/pit.c \
../Sources/Peripherals/rtc.c \
../Sources/Peripherals/sim.c \
../Sources/Peripherals/spi.c \
../Sources/Peripherals/uart.c \

OBJS += \
./Sources/Peripherals/acmp.o \
./Sources/Peripherals/adc.o \
./Sources/Peripherals/ftm.o \
./Sources/Peripherals/gpio.o \
./Sources/Peripherals/ics.o \
./Sources/Peripherals/nvic.o \
./Sources/Peripherals/osc.o \
./Sources/Peripherals/pit.o \
./Sources/Peripherals/rtc.o \
./Sources/Peripherals/sim.o \
./Sources/Peripherals/spi.o \
./Sources/Peripherals/uart.o \

C_DEPS += \
./Sources/Peripherals/acmp.d \
./Sources/Peripherals/adc.d \
./Sources/Peripherals/ftm.d \
./Sources/Peripherals/gpio.d \
./Sources/Peripherals/ics.d \
./Sources/Peripherals/nvic.d \
./Sources/Peripherals/osc.d \
./Sources/Peripherals/pit.d \
./Sources/Peripherals/rtc.d \
./Sources/Peripherals/sim.d \
./Sources/Peripherals/spi.d \
./Sources/Peripherals/uart.d \

OBJS_QUOTED += \
"./Sources/Peripherals/acmp.o" \
"./Sources/Peripherals/adc.o" \
"./Sources/Peripherals/ftm.o" \
"./Sources/Peripherals/gpio.o" \
"./Sources/Peripherals/ics.o" \
"./Sources/Peripherals/nvic.o" \
"./Sources/Peripherals/osc.o" \
"./Sources/Peripherals/pit.o" \
"./Sources/Peripherals/rtc.o" \
"./Sources/Peripherals/sim.o" \
"./Sources/Peripherals/spi.o" \
"./Sources/Peripherals/uart.o" \

C_DEPS_QUOTED += \
"./Sources/Peripherals/acmp.d" \
"./Sources/Peripherals/adc.d" \
"./Sources/Peripherals/ftm.d" \
"./Sources/Peripherals/gpio.d" \
"./Sources/Peripherals/ics.d" \
"./Sources/Peripherals/nvic.d" \
"./Sources/Peripherals/osc.d" \
"./Sources/Peripherals/pit.d" \
"./Sources/Peripherals/rtc.d" \
"./Sources/Peripherals/sim.d" \
"./Sources/Peripherals/spi.d" \
"./Sources/Peripherals/uart.d" \

OBJS_OS_FORMAT += \
./Sources/Peripherals/acmp.o \
./Sources/Peripherals/adc.o \
./Sources/Peripherals/ftm.o \
./Sources/Peripherals/gpio.o \
./Sources/Peripherals/ics.o \
./Sources/Peripherals/nvic.o \
./Sources/Peripherals/osc.o \
./Sources/Peripherals/pit.o \
./Sources/Peripherals/rtc.o \
./Sources/Peripherals/sim.o \
./Sources/Peripherals/spi.o \
./Sources/Peripherals/uart.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/Peripherals/acmp.o: ../Sources/Peripherals/acmp.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Peripherals/acmp.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Peripherals/acmp.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Peripherals/adc.o: ../Sources/Peripherals/adc.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Peripherals/adc.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Peripherals/adc.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Peripherals/ftm.o: ../Sources/Peripherals/ftm.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Peripherals/ftm.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Peripherals/ftm.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Peripherals/gpio.o: ../Sources/Peripherals/gpio.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Peripherals/gpio.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Peripherals/gpio.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Peripherals/ics.o: ../Sources/Peripherals/ics.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Peripherals/ics.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Peripherals/ics.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Peripherals/nvic.o: ../Sources/Peripherals/nvic.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Peripherals/nvic.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Peripherals/nvic.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Peripherals/osc.o: ../Sources/Peripherals/osc.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Peripherals/osc.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Peripherals/osc.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Peripherals/pit.o: ../Sources/Peripherals/pit.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Peripherals/pit.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Peripherals/pit.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Peripherals/rtc.o: ../Sources/Peripherals/rtc.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Peripherals/rtc.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Peripherals/rtc.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Peripherals/sim.o: ../Sources/Peripherals/sim.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Peripherals/sim.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Peripherals/sim.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Peripherals/spi.o: ../Sources/Peripherals/spi.c
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Peripherals/spi.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Peripherals/spi.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Peripherals/uart.o: ../Sources/Peripherals/uart.c
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Peripherals/uart.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Peripherals/uart.o"
	@echo 'Finished building: $<'
	@echo ' '


