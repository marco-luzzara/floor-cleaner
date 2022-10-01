/*
 * map.c
 *
 *  Created on: Sep 29, 2022
 *      Author: maluz
 */

#include "types/map.h"

inline bool is_row_valid(const MapInfo* mapInfo, uint16_t row) {
	return row >= 0 && row < mapInfo->row_count;
}

inline bool is_column_valid(const MapInfo* mapInfo, uint16_t column) {
	return column >= 0 && column < mapInfo->column_count;
}

inline bool is_cell_valid(const MapInfo* mapInfo, const MapPosition* cell) {
	return is_row_valid(mapInfo, cell->row) && is_column_valid(mapInfo, cell->col);
}

inline bool are_MapPositions_equal(const MapPosition* mp1, const MapPosition* mp2) {
	return mp1->row == mp2->row && mp1->col == mp2->col;
}
