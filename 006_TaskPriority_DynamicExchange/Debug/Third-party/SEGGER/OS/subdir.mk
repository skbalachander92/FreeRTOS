################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Third-party/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.c 

OBJS += \
./Third-party/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.o 

C_DEPS += \
./Third-party/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.d 


# Each subdirectory must supply rules for building sources it contributes
Third-party/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.o: ../Third-party/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F446xx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F4 -DSTM32F446RETx -DDEBUG -DNUCLEO_F446RE -c -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/006_TaskPriority_DynamicExchange/CMSIS/core" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/006_TaskPriority_DynamicExchange/Third-party/SEGGER/Config" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/006_TaskPriority_DynamicExchange/Third-party/SEGGER/OS" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/006_TaskPriority_DynamicExchange/Third-party/SEGGER/SEGGER" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/006_TaskPriority_DynamicExchange/Config" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/006_TaskPriority_DynamicExchange/Third-party/FreeRTOS/org/Source/include" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/006_TaskPriority_DynamicExchange/Third-party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/006_TaskPriority_DynamicExchange/CMSIS/device" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/006_TaskPriority_DynamicExchange/inc" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/006_TaskPriority_DynamicExchange/StdPeriph_Driver/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Third-party/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

