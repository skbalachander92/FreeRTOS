################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Third-party/SEGGER/Config/SEGGER_SYSVIEW_Config_FreeRTOS.c 

OBJS += \
./Third-party/SEGGER/Config/SEGGER_SYSVIEW_Config_FreeRTOS.o 

C_DEPS += \
./Third-party/SEGGER/Config/SEGGER_SYSVIEW_Config_FreeRTOS.d 


# Each subdirectory must supply rules for building sources it contributes
Third-party/SEGGER/Config/SEGGER_SYSVIEW_Config_FreeRTOS.o: ../Third-party/SEGGER/Config/SEGGER_SYSVIEW_Config_FreeRTOS.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F446xx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F4 -DSTM32F446RETx -DDEBUG -DNUCLEO_F446RE -c -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/002_LED_Button/CMSIS/core" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/002_LED_Button/Third-party/SEGGER/Config" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/002_LED_Button/Third-party/SEGGER/OS" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/002_LED_Button/Third-party/SEGGER/SEGGER" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/002_LED_Button/Config" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/002_LED_Button/Third-party/FreeRTOS/org/Source/include" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/002_LED_Button/Third-party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/002_LED_Button/CMSIS/device" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/002_LED_Button/inc" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/002_LED_Button/StdPeriph_Driver/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Third-party/SEGGER/Config/SEGGER_SYSVIEW_Config_FreeRTOS.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

