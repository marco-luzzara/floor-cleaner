/*
 * @brief reads the map from the usart
 * @param huart point to the usart object
 * @param row_count pointer to the variable that will contain the number of rows
 * @param column_count pointer to the variable that will contain the number of columns
 * @retval the matrix representing the map
 */
uint8_t** get_map(UART_HandleTypeDef *huart, int* row_count, int* column_count);
