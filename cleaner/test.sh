mkdir -p Tests

gcc "Core/Src/tests/driving_test.c" -std=gnu11 -g3 -DDEBUG -D__TESTING__ -DUSE_HAL_DRIVER -DSTM32L053xx -ICore/Src -ICore/Inc -IDrivers/STM32L0xx_HAL_Driver/Inc -IDrivers/STM32L0xx_HAL_Driver/Inc/Legacy -IDrivers/CMSIS/Device/ST/STM32L0xx/Include -IDrivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -lrt -lm -o "Tests/driving_test"

./Tests/driving_test
