/*
 * map_position.h
 *
 *  Created on: Sep 20, 2022
 *      Author: maluz
 */

#ifndef INC_TYPES_MAP_TYPES_H_
#define INC_TYPES_MAP_TYPES_H_

typedef struct MapPosition {
  uint16_t x;
  uint16_t y;
} MapPosition;

typedef struct MapInfo {
	uint8_t** map;
	uint16_t row_count;
	uint16_t column_count;
} MapInfo;

#endif /* INC_TYPES_MAP_TYPES_H_ */
