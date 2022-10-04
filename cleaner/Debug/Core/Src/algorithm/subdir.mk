################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/algorithm/AStar.c \
../Core/Src/algorithm/shortest_path.c 

OBJS += \
./Core/Src/algorithm/AStar.o \
./Core/Src/algorithm/shortest_path.o 

C_DEPS += \
./Core/Src/algorithm/AStar.d \
./Core/Src/algorithm/shortest_path.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/algorithm/%.o: ../Core/Src/algorithm/%.c Core/Src/algorithm/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L053xx -c -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-algorithm

clean-Core-2f-Src-2f-algorithm:
	-$(RM) ./Core/Src/algorithm/AStar.d ./Core/Src/algorithm/AStar.o ./Core/Src/algorithm/shortest_path.d ./Core/Src/algorithm/shortest_path.o

.PHONY: clean-Core-2f-Src-2f-algorithm

