################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: ReMutt_Control_Code.axf

# Tool invocations
ReMutt_Control_Code.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"X:\rmc\CMSIS_CORE_LPC177x_8x\Debug" -L"X:\rmc\CMSIS_DSPLIB_CM3\lib" -Xlinker -Map="ReMutt_Control_Code.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "ReMutt_Control_Code_Debug.ld" -o "ReMutt_Control_Code.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) ReMutt_Control_Code.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "ReMutt_Control_Code.axf"; arm-none-eabi-objcopy -O binary "ReMutt_Control_Code.axf" "ReMutt_Control_Code.bin" ; checksum -p LPC1788 -d "ReMutt_Control_Code.bin";  arm-none-eabi-objcopy -I binary "ReMutt_Control_Code.bin" -O ihex "ReMutt_Control_Code.hex";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
