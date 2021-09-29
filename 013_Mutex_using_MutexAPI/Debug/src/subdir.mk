################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/system_stm32f4xx.c 

OBJS += \
./src/main.o \
./src/system_stm32f4xx.o 

C_DEPS += \
./src/main.d \
./src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/main.o: ../src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F446xx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F4 -DSTM32F446RETx -DDEBUG -DNUCLEO_F446RE -c -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/013_Mutex_using_MutexAPI/CMSIS/core" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/013_Mutex_using_MutexAPI/Third-party/SEGGER/Config" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/013_Mutex_using_MutexAPI/Third-party/SEGGER/OS" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/013_Mutex_using_MutexAPI/Third-party/SEGGER/SEGGER" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/013_Mutex_using_MutexAPI/Config" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/013_Mutex_using_MutexAPI/Third-party/FreeRTOS/org/Source/include" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/013_Mutex_using_MutexAPI/Third-party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/013_Mutex_using_MutexAPI/CMSIS/device" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/013_Mutex_using_MutexAPI/inc" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/013_Mutex_using_MutexAPI/StdPeriph_Driver/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/system_stm32f4xx.o: ../src/system_stm32f4xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F446xx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F4 -DSTM32F446RETx -DDEBUG -DNUCLEO_F446RE -c -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/013_Mutex_using_MutexAPI/CMSIS/core" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/013_Mutex_using_MutexAPI/Third-party/SEGGER/Config" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/013_Mutex_using_MutexAPI/Third-party/SEGGER/OS" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/013_Mutex_using_MutexAPI/Third-party/SEGGER/SEGGER" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/013_Mutex_using_MutexAPI/Config" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/013_Mutex_using_MutexAPI/Third-party/FreeRTOS/org/Source/include" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/013_Mutex_using_MutexAPI/Third-party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/013_Mutex_using_MutexAPI/CMSIS/device" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/013_Mutex_using_MutexAPI/inc" -I"D:/WORKSPACE/stm32cubeIDE_Workspace/masteringRTOS/RTOS_MyWorkspace/013_Mutex_using_MutexAPI/StdPeriph_Driver/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/system_stm32f4xx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

