################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../inc/common.c \
../inc/gpio.c \
../inc/lcd.c \
../inc/rotary.c 

OBJS += \
./inc/common.o \
./inc/gpio.o \
./inc/lcd.o \
./inc/rotary.o 

C_DEPS += \
./inc/common.d \
./inc/gpio.d \
./inc/lcd.d \
./inc/rotary.d 


# Each subdirectory must supply rules for building sources it contributes
inc/%.o: ../inc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32L4 -DSTM32L476RGTx -DNUCLEO_L476RG -DDEBUG -DSTM32L476xx -DUSE_HAL_DRIVER -I"C:/Users/Eason/workspace/timegloves/HAL_Driver/Inc/Legacy" -I"C:/Users/Eason/workspace/timegloves/inc" -I"C:/Users/Eason/workspace/timegloves/CMSIS/device" -I"C:/Users/Eason/workspace/timegloves/CMSIS/core" -I"C:/Users/Eason/workspace/timegloves/HAL_Driver/Inc" -I"C:/Users/Eason/workspace/timegloves/Utilities/STM32L4xx_Nucleo" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


