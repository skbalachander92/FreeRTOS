################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Third-party/FreeRTOS/org/Source/portable/MemMang/heap_4.c 

OBJS += \
./Third-party/FreeRTOS/org/Source/portable/MemMang/heap_4.o 

C_DEPS += \
./Third-party/FreeRTOS/org/Source/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
Third-party/FreeRTOS/org/Source/portable/MemMang/heap_4.o: ../Third-party/FreeRTOS/org/Source/portable/MemMang/heap_4.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F446xx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F4 -DSTM32F446RETx -DDEBUG -DNUCLEO_F446RE -c -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/CMSIS/core" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/Third-party/SEGGER/Config" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/Third-party/SEGGER/OS" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/Third-party/SEGGER/SEGGER" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/Config" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/Third-party/FreeRTOS/org/Source/include" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/Third-party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/CMSIS/device" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/inc" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/StdPeriph_Driver/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Third-party/FreeRTOS/org/Source/portable/MemMang/heap_4.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

