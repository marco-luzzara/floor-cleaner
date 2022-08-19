#include <stdlib.h>
#include "stm32l0xx_hal.h"

/*
 * @brief read a single char from uart
 */
char read_char(UART_HandleTypeDef *huart) {
	char c;
	HAL_UART_Receive(huart, (uint8_t*)&c, 1, HAL_MAX_DELAY);

	return c;
}

/*
 * @brief read characters until '|' is found. returns the int representing
 * the sequence of chars
 */
int read_number(UART_HandleTypeDef *huart) {
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

uint8_t** get_map(UART_HandleTypeDef *huart, int* row_count, int* column_count) {
	char c;
	do {
		c = read_char(huart);
	} while (c != '&');

	*row_count = read_number(huart);
	*column_count = read_number(huart);

	uint8_t** map = (uint8_t**)malloc(*row_count * sizeof(uint8_t*));

	for (int r = 0; r < *row_count; r++)
	{
		map[r] = (uint8_t*)malloc(*column_count * sizeof(uint8_t));
		for (int c = 0; c < *column_count; c++) {
			char cell[] = { '\0', '\0' };
			cell[0] = read_char(huart);
			map[r][c] = (uint8_t) atoi(cell);
		}
	}

	// should read flow terminator '&'
	read_char(huart);

	return map;
}










