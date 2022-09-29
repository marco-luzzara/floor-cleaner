SHELL=/bin/bash
CC = gcc
CFLAGS = -std=gnu11 -g3 -DDEBUG -D__TESTING__ -DUSE_HAL_DRIVER -DSTM32L053xx -ICore/Inc -IDrivers/STM32L0xx_HAL_Driver/Inc -IDrivers/STM32L0xx_HAL_Driver/Inc/Legacy -IDrivers/CMSIS/Device/ST/STM32L0xx/Include -IDrivers/CMSIS/Include -O2 -ffunction-sections -fdata-sections -Wall
TEST_DIR = Tests
BUILD_DIR = $(TEST_DIR)/build
SRC_DIR = Core/Src
DRIVING_LIB_SRC = $(SRC_DIR)/driving.c $(wildcard $(SRC_DIR)/algorithm/*.c) $(SRC_DIR)/types/map.c
DRIVING_LIB_OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(DRIVING_LIB_SRC))

.PHONY: all run clean

all: run

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@
	
$(TEST_DIR)/libdriving.a: $(DRIVING_LIB_OBJ)
	ar rcs $@ $^
	
$(TEST_DIR)/driving_test: $(SRC_DIR)/tests/driving_test.c $(TEST_DIR)/libdriving.a
	$(CC) $< $(CFLAGS) -ldriving -L$(TEST_DIR) -o $@
	
run: $(TEST_DIR)/driving_test
	./$<
	
clean:
	rm -r $(TEST_DIR)
