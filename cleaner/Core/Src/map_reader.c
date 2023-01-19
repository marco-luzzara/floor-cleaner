#include <stdlib.h>
#include <types/map.h>
#include <stdbool.h>
#include "stm32l0xx_hal.h"

void read_from_UART(UART_HandleTypeDef* huart, uint8_t* buffer, int buffer_length) {
	HAL_UART_Receive(huart, buffer, buffer_length, HAL_MAX_DELAY);
}

/*
 * @brief read a single char from uart
 */
static char read_char(UART_HandleTypeDef *huart) {
	char c;
	read_from_UART(huart, (uint8_t*)&c, 1);

	return c;
}

/*
 * @brief read characters until '|' is found. returns the int representing
 * the sequence of chars
 */
static int read_number(UART_HandleTypeDef *huart) {
	char num[] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };
	uint8_t index = 0;
	char token;

	do {
		token = read_char(huart);
		if (token != '|') {
			num[index] = token;
			index++;
		}
	} while (token != '|');

	return atoi(num);
}

void initialize_map(UART_HandleTypeDef *huart, MapInfo* mapInfo) {
	char c;
	do {
		c = read_char(huart);
	} while (c != '&');

	int row_count = read_number(huart);
	int column_count = read_number(huart);

	// the matrix is allocated as a single array with all the memory needed.
	// in this way the DMA can place all the received bytes directly on the matrix.
	// map_rows contains the pointer to the other rows
	CellType** map_rows = (CellType**)malloc(row_count * sizeof(CellType*));

	int map_size = row_count * column_count;
	CellType* map = (CellType*)malloc(map_size * sizeof(CellType));

	for (int r = 0; r < row_count; r++) {
		map_rows[r] = &map[r * column_count];
	}

	read_from_UART(huart, map, map_size);

	// should read flow terminator '&'
	read_char(huart);

	mapInfo->row_count = row_count;
	mapInfo->column_count = column_count;
	mapInfo->map = map_rows;
}










