#include <stdlib.h>
#include <types/map.h>
#include "stm32l0xx_hal.h"

/*
 * @brief read a single char from uart
 */
static char read_char(UART_HandleTypeDef *huart) {
	char c;
	HAL_UART_Receive(huart, (uint8_t*)&c, 1, HAL_MAX_DELAY);

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

	uint8_t** map = (uint8_t**)malloc(row_count * sizeof(uint8_t*));

	for (uint16_t r = 0; r < row_count; r++)
	{
		map[r] = (uint8_t*)malloc(column_count * sizeof(uint8_t));
		for (uint16_t c = 0; c < column_count; c++) {
			char cell[] = { '\0', '\0' };
			cell[0] = read_char(huart);
			map[r][c] = (uint8_t) atoi(cell);
		}
	}

	// should read flow terminator '&'
	read_char(huart);

	mapInfo->row_count = row_count;
	mapInfo->column_count = column_count;
	mapInfo->map = map;
}










