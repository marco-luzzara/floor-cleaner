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

typedef enum __attribute__ ((__packed__)) CellType {
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

bool is_row_valid(const MapInfo* mapInfo, uint16_t row);
bool is_column_valid(const MapInfo* mapInfo, uint16_t column);
bool is_cell_valid(const MapInfo* mapInfo, const MapPosition* cell);
bool are_MapPositions_equal(const MapPosition* mp1, const MapPosition* mp2);

#endif /* INC_TYPES_MAP_H_ */
