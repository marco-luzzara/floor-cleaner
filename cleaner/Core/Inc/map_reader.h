#ifndef INC_MAP_READER_H_
#define INC_MAP_READER_H_

#include "types/map.h"

/*
 * @brief reads the map from the usart
 * @param huart point to the usart object
 * @param row_count pointer to the variable that will contain the number of rows
 * @param column_count pointer to the variable that will contain the number of columns
 * @retval the matrix representing the map
 */
void initialize_map(UART_HandleTypeDef *huart, MapInfo* mapInfo);

#endif /* INC_MAP_READER_H_ */
