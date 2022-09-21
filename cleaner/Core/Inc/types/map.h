/*
 * map_position.h
 *
 *  Created on: Sep 20, 2022
 *      Author: maluz
 */

#ifndef INC_TYPES_MAP_H_
#define INC_TYPES_MAP_H_

#include <stdbool.h>
#include "stm32l0xx_hal.h"
#include "types/utils.h"

typedef enum CellType {
	UNAVAILABLE = 0,
	TO_CLEAN = 1,
	ALREADY_CLEANED = 2,
	CLEANER_POS = 3,
} CellType;

typedef struct MapPosition {
  uint16_t row;
  uint16_t col;
} MapPosition;

typedef struct MapInfo {
	uint8_t** map;
	uint16_t row_count;
	uint16_t column_count;
} MapInfo;

#endif /* INC_TYPES_MAP_H_ */
