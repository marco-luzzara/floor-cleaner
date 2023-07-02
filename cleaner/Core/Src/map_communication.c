#include <stdlib.h>
#include <types/map.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "stm32l0xx_hal.h"

static void read_from_UART(UART_HandleTypeDef* huart, uint8_t* buffer, int buffer_length) {
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

/*
 * The following commands correspond to the real-time communication of the
 * cleaned cells
 */

static char buf[100];

static void send_command(UART_HandleTypeDef *huart, const char* raw_command) {
	if (huart->hdmatx->State == HAL_DMA_STATE_BUSY) {
		while(huart->hdmatx->State != HAL_DMA_STATE_READY)
			HAL_DMA_PollForTransfer(huart->hdmatx, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
	}

	uint8_t current_command_size = strlen(raw_command);
	uint8_t final_command_size = current_command_size + 1; // 1 is for the \n

	memset(buf, 0, sizeof(buf));
	strcat(buf, raw_command);
	buf[current_command_size] = '\n';

	HAL_UART_Transmit_DMA(huart, (uint8_t *) buf, final_command_size);
}

void send_start_command(UART_HandleTypeDef *huart) {
	const char* command_id = "START{}";
	send_command(huart, command_id);
}

void send_end_command(UART_HandleTypeDef *huart, uint8_t return_code) {
	const size_t BUFFER_LENGTH = 40;
	char command_id[BUFFER_LENGTH];

	snprintf(command_id, BUFFER_LENGTH, "END{'ret_code':%hu}", return_code);
	send_command(huart, command_id);
}

void send_new_cleaner_position_command(UART_HandleTypeDef *huart, uint16_t r, uint16_t c, bool cleaning_enabled) {
	const size_t BUFFER_LENGTH = 80;
	char command_id[BUFFER_LENGTH];

	snprintf(command_id, BUFFER_LENGTH, "MOVE{'r':%hu,'c':%hu,'cleaning_enabled':%s}",
			r, c, cleaning_enabled == true ? "True" : "False");
	send_command(huart, command_id);
}

void send_obstacle_command(UART_HandleTypeDef *huart, uint16_t r, uint16_t c) {
	const size_t BUFFER_LENGTH = 64;
	char command_id[BUFFER_LENGTH];

	snprintf(command_id, BUFFER_LENGTH, "OBSTACLE{'r':%hu,'c':%hu}", r, c);
	send_command(huart, command_id);
}







