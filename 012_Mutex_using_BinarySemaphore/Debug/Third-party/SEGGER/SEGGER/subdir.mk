################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Third-party/SEGGER/SEGGER/SEGGER_RTT.c \
../Third-party/SEGGER/SEGGER/SEGGER_SYSVIEW.c 

OBJS += \
./Third-party/SEGGER/SEGGER/SEGGER_RTT.o \
./Third-party/SEGGER/SEGGER/SEGGER_SYSVIEW.o 

C_DEPS += \
./Third-party/SEGGER/SEGGER/SEGGER_RTT.d \
./Third-party/SEGGER/SEGGER/SEGGER_SYSVIEW.d 


# Each subdirectory must supply rules for building sources it contributes
Third-party/SEGGER/SEGGER/SEGGER_RTT.o: ../Third-party/SEGGER/SEGGER/SEGGER_RTT.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F446xx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F4 -DSTM32F446RETx -DDEBUG -DNUCLEO_F446RE -c -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/CMSIS/core" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/Third-party/SEGGER/Config" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/Third-party/SEGGER/OS" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/Third-party/SEGGER/SEGGER" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/Config" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/Third-party/FreeRTOS/org/Source/include" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/Third-party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/CMSIS/device" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/inc" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/StdPeriph_Driver/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Third-party/SEGGER/SEGGER/SEGGER_RTT.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Third-party/SEGGER/SEGGER/SEGGER_SYSVIEW.o: ../Third-party/SEGGER/SEGGER/SEGGER_SYSVIEW.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F446xx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F4 -DSTM32F446RETx -DDEBUG -DNUCLEO_F446RE -c -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/CMSIS/core" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/Third-party/SEGGER/Config" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/Third-party/SEGGER/OS" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/Third-party/SEGGER/SEGGER" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/Config" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/Third-party/FreeRTOS/org/Source/include" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/Third-party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/CMSIS/device" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/inc" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/012_Mutex_using_BinarySemaphore/StdPeriph_Driver/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Third-party/SEGGER/SEGGER/SEGGER_SYSVIEW.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

