################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/tests/driving_test.c 

OBJS += \
./Core/Src/tests/driving_test.o 

C_DEPS += \
./Core/Src/tests/driving_test.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/tests/%.o: ../Core/Src/tests/%.c Core/Src/tests/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L053xx -c -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-tests

clean-Core-2f-Src-2f-tests:
	-$(RM) ./Core/Src/tests/driving_test.d ./Core/Src/tests/driving_test.o

.PHONY: clean-Core-2f-Src-2f-tests

