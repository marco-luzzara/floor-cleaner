#ifndef INC_MAP_COMMUNICATION_H_
#define INC_MAP_COMMUNICATION_H_

#include "types/map.h"

/*
 * @brief reads the map from the usart
 * @param huart point to the usart object
 * @param row_count pointer to the variable that will contain the number of rows
 * @param column_count pointer to the variable that will contain the number of columns
 * @retval the matrix representing the map
 */
void initialize_map(UART_HandleTypeDef *huart, MapInfo* mapInfo);

void send_start_command(UART_HandleTypeDef *huart);

void send_end_command(UART_HandleTypeDef *huart);

void send_new_cleaner_position_command(UART_HandleTypeDef *huart, uint16_t r, uint16_t c);

void send_obstacle_command(UART_HandleTypeDef *huart, uint16_t r, uint16_t c);

#endif /* INC_MAP_COMMUNICATION_H_ */
