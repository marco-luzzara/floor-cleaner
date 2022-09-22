/*
 * driving.c
 *
 *  Created on: Aug 20, 2022
 *      Author: maluz
 */

#include <math.h>
#include "types/map.h"
#include "stm32l0xx_hal.h"
#include "types/cleaner.h"
#include "types/utils.h"

// TODO: set based on motor rpm
int millis_to_turn = 1000;
int millis_to_drive = 1000;

static bool find_cleaner(const MapInfo* mapInfo, CleanerInfo* cleanerInfo);
static bool find_first_around_cell(const MapPosition* start,
																	 const MapInfo* mapInfo,
																	 MapPosition* target,
																	 bool (*condition)(const MapInfo*, const MapPosition*));
static void find_initial_position(const MapInfo* mapInfo,
																  const MapPosition* start_position,
																	const MapPosition* nearest_unavailable,
																  MapPosition* target_position);

int start_drive(const MapInfo* mapInfo,
								const MotorsInfo* motorsInfo,
								const CleanComponentsInfo* cleanComponentsInfo) {
	CleanerInfo cleanerInfo;
	bool cleaner_found = find_cleaner(mapInfo, &cleanerInfo);

	if (!cleaner_found)
		return 1;

	MapPosition target_start_position;
	MapPosition nearest_unavailable;
	bool are_any_unavailable = find_first_around_cell(&cleanerInfo.position, mapInfo, &nearest_unavailable,
			lambda(bool, (const MapInfo* mapInfo, const MapPosition* cur_cell), {
				return mapInfo->map[cur_cell->row][cur_cell->col] == UNAVAILABLE;
			}));

	find_initial_position(mapInfo, &cleanerInfo.position,
			are_any_unavailable ? &nearest_unavailable : NULL,
			&target_start_position);

	return 0;
}

static void find_initial_position(const MapInfo* mapInfo,
																  const MapPosition* start_position,
																	const MapPosition* nearest_unavailable,
																  MapPosition* target_position) {
	// there is no unavailable cell, all must be clea
	if (nearest_unavailable == NULL) {
		//find_first_around_cell(start, mapInfo, target, condition)
	}
}

static bool visit_boundary_for_search(const MapInfo* mapInfo,
																			uint16_t start_index,
																			uint16_t excl_end_index,
																			MapPosition (*position_generator)(uint16_t),
																			bool (*condition)(const MapInfo*, const MapPosition*),
																			MapPosition* target) {
	for (uint16_t i = start_index; i < excl_end_index; i++) {
		MapPosition cur_cell = position_generator(i);
		if (condition(mapInfo, &cur_cell)) {
			*target = cur_cell;
			return true;
		}
	}

	return false;
}

static bool find_first_around_cell(const MapPosition* start,
																	 const MapInfo* mapInfo,
																	 MapPosition* target,
																	 bool (*condition)(const MapInfo*, const MapPosition*)) {
	uint16_t max_radius = max(
			max(start->row, mapInfo->row_count - start->row - 1),
			max(start->col, mapInfo->column_count - start->col- 1));

	for (uint16_t radius = 1; radius <= max_radius; radius++) {
		uint16_t up_limit = max(0, start->row - radius);
		uint16_t right_limit = min(mapInfo->column_count - 1, start->col + radius);
		uint16_t down_limit = min(mapInfo->row_count - 1, start->row + radius);
		uint16_t left_limit = max(0, start->col - radius);

		bool is_cell_found = false;
		// visiting the up row
		if (start->row - radius >= 0) {
			is_cell_found = visit_boundary_for_search(mapInfo, left_limit, right_limit,
					lambda(MapPosition, (uint16_t i), {
						MapPosition cur_cell; cur_cell.row = up_limit; cur_cell.col = i;
						return cur_cell;
					}),
					condition, target);
		}

		// visiting the right column
		if (!is_cell_found && start->col + radius < mapInfo->column_count) {
			is_cell_found = visit_boundary_for_search(mapInfo, up_limit, down_limit,
					lambda(MapPosition, (uint16_t i), {
						MapPosition cur_cell; cur_cell.row = i; cur_cell.col = right_limit;
						return cur_cell;
					}),
					condition, target);
		}

		// visiting the down row
		if (!is_cell_found && start->row + radius < mapInfo->row_count)
			is_cell_found = visit_boundary_for_search(mapInfo, left_limit + 1, right_limit + 1,
					lambda(MapPosition, (uint16_t i), {
						MapPosition cur_cell; cur_cell.row = down_limit; cur_cell.col = i;
						return cur_cell;
					}),
					condition, target);

		// visiting the left column
		if (!is_cell_found && start->col - radius >= 0)
			is_cell_found = visit_boundary_for_search(mapInfo, up_limit + 1, down_limit + 1,
					lambda(MapPosition, (uint16_t i), {
						MapPosition cur_cell; cur_cell.row = i; cur_cell.col = left_limit;
						return cur_cell;
					}),
					condition, target);

		if (is_cell_found)
			return true;
	}

	return false;
}

/*
 * @brief find the current position of the cleaner, if present. direction is assumed UP
 */
static bool find_cleaner(const MapInfo* mapInfo, CleanerInfo* cleanerInfo) {
	for (int r = 0; r < mapInfo->row_count; r++) {
		for (int c = 0; c < mapInfo->column_count; c++) {
			if (mapInfo->map[r][c] == CLEANER_POS) {
				cleanerInfo->direction = UP;
				cleanerInfo->position.row = r;
				cleanerInfo->position.col = c;

				return true;
			}
		}
	}

	return false;
}


int drive_forward(const int* obstacle_found, MotorsInfo* motorsInfo) {
	// modified from HAL_Delay(Delay)
	HAL_GPIO_WritePin(&motorsInfo->antiClockwise_left_GPIOType, motorsInfo->antiClockwise_left_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(&motorsInfo->clockwise_right_GPIOType, motorsInfo->clockwise_right_pin, GPIO_PIN_SET);
  uint32_t tickstart = HAL_GetTick();
  uint32_t wait = millis_to_drive;
  uint8_t undo_drive = 0;
  uint32_t undo_delay;

  while((undo_delay = HAL_GetTick() - tickstart) < wait)
  {
  	if (*obstacle_found == 1) {
  		undo_drive = 1;
  		break;
  	}
  }

  HAL_GPIO_WritePin(&motorsInfo->antiClockwise_left_GPIOType, motorsInfo->antiClockwise_left_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(&motorsInfo->clockwise_right_GPIOType, motorsInfo->clockwise_right_pin, GPIO_PIN_RESET);

  if (undo_drive == 0)
  	return 0;

  // drive backward because of obstacle
  HAL_GPIO_WritePin(&motorsInfo->clockwise_left_GPIOType, motorsInfo->clockwise_left_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(&motorsInfo->antiClockwise_right_GPIOType, motorsInfo->antiClockwise_right_pin, GPIO_PIN_SET);

	// TODO: what if an obstacle is found while going back
  HAL_Delay(undo_delay);

  HAL_GPIO_WritePin(&motorsInfo->clockwise_left_GPIOType, motorsInfo->clockwise_left_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(&motorsInfo->antiClockwise_right_GPIOType, motorsInfo->antiClockwise_right_pin, GPIO_PIN_RESET);

	return 1;
}

void turn_left(MotorsInfo* motorsInfo) {
	HAL_GPIO_WritePin(&motorsInfo->clockwise_left_GPIOType, motorsInfo->clockwise_left_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(&motorsInfo->clockwise_right_GPIOType, motorsInfo->clockwise_right_pin, GPIO_PIN_SET);

	HAL_Delay(millis_to_turn);

	HAL_GPIO_WritePin(&motorsInfo->clockwise_left_GPIOType, motorsInfo->clockwise_left_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(&motorsInfo->clockwise_right_GPIOType, motorsInfo->clockwise_right_pin, GPIO_PIN_RESET);
}

void turn_right(MotorsInfo* motorsInfo) {
	HAL_GPIO_WritePin(&motorsInfo->antiClockwise_left_GPIOType, motorsInfo->antiClockwise_left_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(&motorsInfo->antiClockwise_right_GPIOType, motorsInfo->antiClockwise_right_pin, GPIO_PIN_SET);

	HAL_Delay(millis_to_turn);

	HAL_GPIO_WritePin(&motorsInfo->antiClockwise_left_GPIOType, motorsInfo->antiClockwise_left_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(&motorsInfo->antiClockwise_right_GPIOType, motorsInfo->antiClockwise_right_pin, GPIO_PIN_RESET);
}
