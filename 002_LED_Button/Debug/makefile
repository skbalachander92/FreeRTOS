################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include src/subdir.mk
-include Third-party/SEGGER/SEGGER/subdir.mk
-include Third-party/SEGGER/OS/subdir.mk
-include Third-party/SEGGER/Config/subdir.mk
-include Third-party/FreeRTOS/org/Source/portable/MemMang/subdir.mk
-include Third-party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Third-party/FreeRTOS/org/Source/subdir.mk
-include StdPeriph_Driver/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
002_LED_Button.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
002_LED_Button.list \

OBJCOPY_BIN += \
002_LED_Button.bin \


# All Target
all: 002_LED_Button.elf secondary-outputs

# Tool invocations
002_LED_Button.elf: $(OBJS) $(USER_OBJS) D:\WORKSPACE\stm32cubeIDE_Workspace\masteringRTOS\RTOS_MyWorkspace\002_LED_Button\STM32F446RETX_FLASH.ld
	arm-none-eabi-gcc -o "002_LED_Button.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"D:\WORKSPACE\stm32cubeIDE_Workspace\masteringRTOS\RTOS_MyWorkspace\002_LED_Button\STM32F446RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="002_LED_Button.map" -Wl,--gc-sections -static -specs=rdimon.specs -lc -lrdimon --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

002_LED_Button.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "002_LED_Button.list"
	@echo 'Finished building: $@'
	@echo ' '

002_LED_Button.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "002_LED_Button.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
