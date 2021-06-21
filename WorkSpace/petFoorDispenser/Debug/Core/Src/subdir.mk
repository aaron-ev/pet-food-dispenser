################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/fonts.c \
../Core/Src/it.c \
../Core/Src/lcd1602_i2c.c \
../Core/Src/main.c \
../Core/Src/msp.c \
../Core/Src/screenDriver.c \
../Core/Src/servo.c \
../Core/Src/ssd1306.c \
../Core/Src/syscalls.c \
../Core/Src/system_stm32f4xx.c 

OBJS += \
./Core/Src/fonts.o \
./Core/Src/it.o \
./Core/Src/lcd1602_i2c.o \
./Core/Src/main.o \
./Core/Src/msp.o \
./Core/Src/screenDriver.o \
./Core/Src/servo.o \
./Core/Src/ssd1306.o \
./Core/Src/syscalls.o \
./Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Core/Src/fonts.d \
./Core/Src/it.d \
./Core/Src/lcd1602_i2c.d \
./Core/Src/main.d \
./Core/Src/msp.d \
./Core/Src/screenDriver.d \
./Core/Src/servo.d \
./Core/Src/ssd1306.d \
./Core/Src/syscalls.d \
./Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F446xx -I"C:/Users/aaron/Desktop/git_dispenser/workSpace/petFoorDispenser/Core/Inc" -I"C:/Users/aaron/Desktop/git_dispenser/workSpace/petFoorDispenser/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/aaron/Desktop/git_dispenser/workSpace/petFoorDispenser/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/aaron/Desktop/git_dispenser/workSpace/petFoorDispenser/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/aaron/Desktop/git_dispenser/workSpace/petFoorDispenser/Drivers/CMSIS/Include" -I"C:/Users/aaron/Desktop/git_driverLCD16X2/driver"  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


