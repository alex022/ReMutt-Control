################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/camera.c \
../src/clkpwr.c \
../src/cr_startup_lpc177x_8x.c \
../src/crp.c \
../src/fsr_circuit.c \
../src/global.c \
../src/gpio.c \
../src/main.c \
../src/pinsel.c \
../src/pwm.c \
../src/retarget.c \
../src/sdram.c \
../src/servo.c \
../src/ssp.c \
../src/stepper_motor.c \
../src/timer.c \
../src/uart.c \
../src/wifi.c 

OBJS += \
./src/camera.o \
./src/clkpwr.o \
./src/cr_startup_lpc177x_8x.o \
./src/crp.o \
./src/fsr_circuit.o \
./src/global.o \
./src/gpio.o \
./src/main.o \
./src/pinsel.o \
./src/pwm.o \
./src/retarget.o \
./src/sdram.o \
./src/servo.o \
./src/ssp.o \
./src/stepper_motor.o \
./src/timer.o \
./src/uart.o \
./src/wifi.o 

C_DEPS += \
./src/camera.d \
./src/clkpwr.d \
./src/cr_startup_lpc177x_8x.d \
./src/crp.d \
./src/fsr_circuit.d \
./src/global.d \
./src/gpio.d \
./src/main.d \
./src/pinsel.d \
./src/pwm.d \
./src/retarget.d \
./src/sdram.d \
./src/servo.d \
./src/ssp.d \
./src/stepper_motor.d \
./src/timer.d \
./src/uart.d \
./src/wifi.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC177x_8x -D__USE_CMSIS_DSPLIB=CMSIS_DSPLIB_CM3 -D__LPC177X_8X__ -I"X:\rmc\ReMutt_Control_Code\inc" -I"X:\rmc\CMSIS_CORE_LPC177x_8x\inc" -I"X:\rmc\CMSIS_DSPLIB_CM3\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/cr_startup_lpc177x_8x.o: ../src/cr_startup_lpc177x_8x.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC177x_8x -D__USE_CMSIS_DSPLIB=CMSIS_DSPLIB_CM3 -D__LPC177X_8X__ -I"X:\rmc\ReMutt_Control_Code\inc" -I"X:\rmc\CMSIS_CORE_LPC177x_8x\inc" -I"X:\rmc\CMSIS_DSPLIB_CM3\inc" -Os -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/cr_startup_lpc177x_8x.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


