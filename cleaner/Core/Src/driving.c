/*
 * driving.c
 *
 *  Created on: Aug 20, 2022
 *      Author: maluz
 */

#include <math.h>
#include <assert.h>
#include "types/map.h"
#include "stm32l0xx_hal.h"
#include "types/cleaner.h"
#include "types/utils.h"
#include "algorithm/shortest_path.h"

// TODO: set based on motor rpm
int millis_to_turn = 1000;
int millis_to_drive = 1000;

static bool drive_forward(CleanerInfo* cleanerInfo, bool* obstacle_found, MotorsInfo* motorsInfo);
static void turn_left(CleanerInfo* cleanerInfo, MotorsInfo* motorsInfo);
static void turn_right(CleanerInfo* cleanerInfo, MotorsInfo* motorsInfo);
static void enable_cleaning(CleanComponentsInfo* cleanComponentsInfo);
static void disable_cleaning(CleanComponentsInfo* cleanComponentsInfo);
static bool initialize_cleaner(MapInfo* mapInfo, CleanerInfo* cleanerInfo);
static bool find_first_around_cell(const MapPosition* start,
																	 const MapInfo* mapInfo,
																	 MapPosition* target,
																	 bool (*condition)(const MapInfo*, const MapPosition*));
static bool find_start_position(const MapInfo* mapInfo,
																  MapPosition* target_position);
static bool find_next_cell_while_cleaning(const CleanerInfo* cleanerInfo,
																					const MapInfo* mapInfo,
																					MapPosition* next_cell);
static bool move_cleaner_to(MapInfo* mapInfo,
														bool* obstacle_found,
														MotorsInfo* motorsInfo,
														CleanerInfo* cleanerInfo,
														const MapPosition* target_position);
static bool move_cleaner_to_adjacent_position(MapInfo* mapInfo,
																							bool* obstacle_found,
																						  MotorsInfo* motorsInfo,
																						  CleanerInfo* cleanerInfo,
																						  MapPosition* target_position);

int start_drive(MapInfo* mapInfo,
								bool* obstacle_found,
								MotorsInfo* motorsInfo,
								CleanComponentsInfo* cleanComponentsInfo) {
	CleanerInfo cleanerInfo;
	bool is_cleaner_found = initialize_cleaner(mapInfo, &cleanerInfo);
	if (!is_cleaner_found)
		return 1;

	MapPosition start_position;
	bool is_start_cell_found = find_start_position(mapInfo, &start_position);
	if (!is_start_cell_found)
		return 2;

	bool can_move_to_start_position = move_cleaner_to(mapInfo,
																										obstacle_found,
																										motorsInfo,
																										&cleanerInfo,
																										&start_position);

	if (!can_move_to_start_position)
		return 3;
	mapInfo->map[start_position.row][start_position.col] = ALREADY_CLEANED;

	bool is_cleaning_ongoing = true;
	while (is_cleaning_ongoing) {
		MapPosition next_cell;
		bool is_cell_available = find_next_cell_while_cleaning(&cleanerInfo, mapInfo, &next_cell);

		if (is_cell_available) {
			enable_cleaning(cleanComponentsInfo);
			move_cleaner_to_adjacent_position(mapInfo, obstacle_found, motorsInfo, &cleanerInfo, &next_cell);
			disable_cleaning(cleanComponentsInfo);

			mapInfo->map[next_cell.row][next_cell.col] = ALREADY_CLEANED;
		}
		else {
			is_cleaning_ongoing = find_first_around_cell(&cleanerInfo.position, mapInfo, &next_cell,
					lambda(bool, (const MapInfo* inner_mapInfo, const MapPosition* target_pos), {
						return inner_mapInfo->map[target_pos->row][target_pos->col] == TO_CLEAN;
					}));

			if (is_cleaning_ongoing) {
				bool can_reach_next_cell = move_cleaner_to(mapInfo, obstacle_found, motorsInfo, &cleanerInfo, &next_cell);
				if (!can_reach_next_cell)
					return 4;
				mapInfo->map[next_cell.row][next_cell.col] = ALREADY_CLEANED;
			}
		}
	}

	return 0;
}

/*
 * move cleaner to a non-adjacent non-UNAVAILABLE position using a shortest path algorithm.
 * If an obstacle is found during the road, the shortest path is recomputed.
 * this method returns false only when there is no path to the specified target position
 * Precondition: the path is made of adjacent positions.
 */
static bool move_cleaner_to(MapInfo* mapInfo,
														bool* obstacle_found,
														MotorsInfo* motorsInfo,
														CleanerInfo* cleanerInfo,
														const MapPosition* target_position) {
	assert(mapInfo->map[target_position->row][target_position->col] != UNAVAILABLE);

	while (!are_MapPositions_equal(&cleanerInfo->position, target_position)) {
		MapPosition* path; size_t path_length;
		bool does_path_exist = find_best_path(&cleanerInfo->position, target_position, mapInfo, &path, &path_length);
		if (!does_path_exist)
			return false;

		bool is_move_successful;
		// the loop starts from 1 because the path includes the current position at index 0
		for (size_t i = 1; i < path_length; i++) {
			is_move_successful = move_cleaner_to_adjacent_position(mapInfo,
																														 obstacle_found,
																														 motorsInfo,
																														 cleanerInfo,
																														 &path[i]);
			if (!is_move_successful)
				break;
		}

		free(path);
	}

	return true;
}

/*
 * @brief move the cleaner to the next adjacent position that is compliant
 * with the cleaning logic. the cleaner will find the next position TO_CLEAN
 * after an UNAVAILABLE or ALREADY_CLEANED position, visiting the 4 directions
 * clockwise. The reason is that the cleaner moves next to the perimeter in a
 * clockwise way.
 */
static bool move_cleaner_to_adjacent_position(MapInfo* mapInfo,
																							bool* obstacle_found,
																						  MotorsInfo* motorsInfo,
																						  CleanerInfo* cleanerInfo,
																						  MapPosition* target_position) {
	assert(mapInfo->map[target_position->row][target_position->col] != UNAVAILABLE);

	MapPosition* cleaner_pos = &cleanerInfo->position;
	Direction current_dir = cleanerInfo->direction;
	Direction target_dir;

	// is the target position on the top
	if (target_position->row == cleaner_pos->row - 1)
		target_dir = UP;
	// is the target position on the right
	else if (target_position->col == cleaner_pos->col + 1)
		target_dir = RIGHT;
	// is the target position on the bottom
	else if (target_position->row == cleaner_pos->row + 1)
		target_dir = DOWN;
	// is the target position on the left (target_position->col == cleaner_pos->col - 1))
	else
		target_dir = LEFT;

	// this is the most common situation because the cleaner goes around the perimeter clockwise
	if ((current_dir == UP && target_dir == RIGHT) ||
		  (current_dir == RIGHT && target_dir == DOWN) ||
		  (current_dir == DOWN && target_dir == LEFT) ||
		  (current_dir == LEFT && target_dir == UP)) {
		turn_right(cleanerInfo, motorsInfo);
	}
	else if (current_dir != target_dir) {
		while (cleanerInfo->direction != target_dir)
			turn_left(cleanerInfo, motorsInfo);
	}

	// now the direction is correct
	bool is_move_successful = drive_forward(cleanerInfo, obstacle_found, motorsInfo);
	if (!is_move_successful) // an obstacle has been found
		mapInfo->map[target_position->row][target_position->col] = UNAVAILABLE;

	return is_move_successful;
}

/*
 * @brief find the next cell along the perimeter while cleaning. The cleaner always drives on
 * the perimeter drawn by the area not already cleaned. the cleaner direction is clockwise,
 * so the perimeter wall is always on its left.
 * Precondition: there is at least one unavailable or already cleaned cell around
 */
static bool find_next_cell_while_cleaning(const CleanerInfo* cleanerInfo,
																					const MapInfo* mapInfo,
																					MapPosition* next_cell) {
	bool (*is_boundary)(const MapPosition*) =
			lambda(bool, (const MapPosition* position), {
				return !is_cell_valid(mapInfo, position) || mapInfo->map[position->row][position->col] == UNAVAILABLE ||
						mapInfo->map[position->row][position->col] == ALREADY_CLEANED;
			});

	const MapPosition* current_position = &cleanerInfo->position;
	bool boundaries[4];

	// is the boundary on the top?
	MapPosition top_boundary = { .row = current_position->row - 1, .col = current_position->col };
	boundaries[0] = is_boundary(&top_boundary);
	// is the boundary on the right?
	MapPosition right_boundary = { .row = current_position->row, .col = current_position->col + 1 };
	boundaries[1] = is_boundary(&right_boundary);
	// is the boundary on the bottom?
	MapPosition bottom_boundary = { .row = current_position->row + 1, .col = current_position->col };
	boundaries[2] = is_boundary(&bottom_boundary);
	// is the boundary on the left?
	MapPosition left_boundary = { .row = current_position->row, .col = current_position->col - 1 };
	boundaries[3] = is_boundary(&left_boundary);

	// cleaning around the cell is complete
	if (boundaries[0] && boundaries[1] && boundaries[2] && boundaries[3])
		return false;

	if (boundaries[0] && !boundaries[1])
		*next_cell = right_boundary;
	else if (boundaries[1] && !boundaries[2])
		*next_cell = bottom_boundary;
	else if (boundaries[2] && !boundaries[3])
		*next_cell = left_boundary;
	else
		*next_cell = top_boundary;

	return true;
}

/*
 * @brief the starting position is the cell 0,0 if TO_CLEAN or the nearest to it
 */
static bool find_start_position(const MapInfo* mapInfo, MapPosition* target_position) {
	MapPosition up_left_corner = { .row = 0, .col = 0 };
	if (mapInfo->map[up_left_corner.row][up_left_corner.col] == TO_CLEAN) {
		*target_position = up_left_corner;
		return true;
	}

	bool is_cell_to_clean_found = find_first_around_cell(&up_left_corner, mapInfo, target_position,
			lambda(bool, (const MapInfo* mapInfo, const MapPosition* cur_cell), {
				return mapInfo->map[cur_cell->row][cur_cell->col] == TO_CLEAN;
			}));

	return is_cell_to_clean_found;
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

/*
 * @brief find the first cell that satisfies the condition function. the search is
 * performed all around the cell (excluding the cell itself): first the enclosing square with
 * edge length= 3 is visited, then the enclosing square with edge length = 5 and so on until
 * all the grid is visited.
 */
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
		if (is_row_valid(mapInfo, start->row - radius)) {
			is_cell_found = visit_boundary_for_search(mapInfo, left_limit, right_limit,
					lambda(MapPosition, (uint16_t i), {
						MapPosition cur_cell; cur_cell.row = up_limit; cur_cell.col = i;
						return cur_cell;
					}),
					condition, target);
		}

		// visiting the right column
		if (!is_cell_found && is_column_valid(mapInfo, start->col + radius)) {
			is_cell_found = visit_boundary_for_search(mapInfo, up_limit, down_limit,
					lambda(MapPosition, (uint16_t i), {
						MapPosition cur_cell; cur_cell.row = i; cur_cell.col = right_limit;
						return cur_cell;
					}),
					condition, target);
		}

		// visiting the down row
		if (!is_cell_found && is_row_valid(mapInfo, start->row + radius))
			is_cell_found = visit_boundary_for_search(mapInfo, left_limit + 1, right_limit + 1,
					lambda(MapPosition, (uint16_t i), {
						MapPosition cur_cell; cur_cell.row = down_limit; cur_cell.col = i;
						return cur_cell;
					}),
					condition, target);

		// visiting the left column
		if (!is_cell_found && is_column_valid(mapInfo, start->col - radius))
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
static bool initialize_cleaner(MapInfo* mapInfo, CleanerInfo* cleanerInfo) {
	for (int r = 0; r < mapInfo->row_count; r++) {
		for (int c = 0; c < mapInfo->column_count; c++) {
			if (mapInfo->map[r][c] == CLEANER_POS) {
				cleanerInfo->direction = UP;
				cleanerInfo->position.row = r;
				cleanerInfo->position.col = c;
				mapInfo->map[r][c] = TO_CLEAN;

				return true;
			}
		}
	}

	return false;
}

/**
 * @brief move the cleaner of one unit
 * @retval true - the cleaner reached the destination
 * 				 false - an obstacle has been found, the cleaner returned to the initial position
 */
static bool drive_forward(CleanerInfo* cleanerInfo, bool* obstacle_found, MotorsInfo* motorsInfo) {
	// reset obstacle_found to be sure the cleaner starts moving
	*obstacle_found = false;
	bool undo_drive = false;

#ifndef __TESTING__
	// modified from HAL_Delay(Delay)
	HAL_GPIO_WritePin(motorsInfo->left2_GPIOType, motorsInfo->left2_pin, GPIO_PIN_SET); // ACW
	HAL_GPIO_WritePin(motorsInfo->right1_GPIOType, motorsInfo->right1_pin, GPIO_PIN_SET); // CW
	// TODO: use optical encoder output to find out how many steps have been done
  uint32_t tickstart = HAL_GetTick();
  uint32_t wait = millis_to_drive;
  uint32_t undo_delay;

  while((undo_delay = HAL_GetTick() - tickstart) < wait)
  {
  	if (*obstacle_found) {
  		undo_drive = true;
  		break;
  	}
  }

  HAL_GPIO_WritePin(motorsInfo->left2_GPIOType, motorsInfo->left2_pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(motorsInfo->right1_GPIOType, motorsInfo->right1_pin, GPIO_PIN_RESET);
#endif

  if (!undo_drive) {
  	uint16_t current_row = cleanerInfo->position.row;
  	uint16_t current_col = cleanerInfo->position.col;

  	switch (cleanerInfo->direction) {
  	case UP: current_row--; break;
  	case RIGHT: current_col++; break;
  	case DOWN: current_row++; break;
  	case LEFT: current_col--; break;
  	}

		cleanerInfo->position.row = current_row;
		cleanerInfo->position.col = current_col;

  	return true;
  }

#ifndef __TESTING__
  // drive backward because of obstacle
  HAL_GPIO_WritePin(motorsInfo->left1_GPIOType, motorsInfo->left1_pin, GPIO_PIN_SET); // CW
	HAL_GPIO_WritePin(motorsInfo->right2_GPIOType, motorsInfo->right2_pin, GPIO_PIN_SET); // ACW

	// TODO: what if an obstacle is found while going back
  HAL_Delay(undo_delay);

  HAL_GPIO_WritePin(motorsInfo->left1_GPIOType, motorsInfo->left1_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(motorsInfo->right2_GPIOType, motorsInfo->right2_pin, GPIO_PIN_RESET);
#endif

	return false;
}

/**
 * @brief rotate the cleaner by 90° anti-clockwise
 */
static void turn_left(CleanerInfo* cleanerInfo, MotorsInfo* motorsInfo) {
#ifndef __TESTING__
	HAL_GPIO_WritePin(motorsInfo->left1_GPIOType, motorsInfo->left1_pin, GPIO_PIN_SET); // CW
	HAL_GPIO_WritePin(motorsInfo->right1_GPIOType, motorsInfo->right1_pin, GPIO_PIN_SET); // CW

	HAL_Delay(millis_to_turn);

	HAL_GPIO_WritePin(motorsInfo->left1_GPIOType, motorsInfo->left1_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(motorsInfo->right1_GPIOType, motorsInfo->right1_pin, GPIO_PIN_RESET);
#endif

	// adding 3 is like subtracting 1 with modulo 4
	cleanerInfo->direction = (cleanerInfo->direction + 3) % 4;
}

/**
 * @brief rotate the cleaner by 90° clockwise
 */
static void turn_right(CleanerInfo* cleanerInfo, MotorsInfo* motorsInfo) {
#ifndef __TESTING__
	HAL_GPIO_WritePin(motorsInfo->left2_GPIOType, motorsInfo->left2_pin, GPIO_PIN_SET); // ACW
	HAL_GPIO_WritePin(motorsInfo->right2_GPIOType, motorsInfo->right2_pin, GPIO_PIN_SET); // ACW

	HAL_Delay(millis_to_turn);

	HAL_GPIO_WritePin(motorsInfo->left2_GPIOType, motorsInfo->left2_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(motorsInfo->right2_GPIOType, motorsInfo->right2_pin, GPIO_PIN_RESET);
#endif

	cleanerInfo->direction = (cleanerInfo->direction + 1) % 4;
}

static void enable_cleaning(CleanComponentsInfo* cleanComponentsInfo) {
#ifndef __TESTING__
	HAL_GPIO_WritePin(cleanComponentsInfo->vacuum_GPIOType, cleanComponentsInfo->vacuum_pin, GPIO_PIN_SET);
#endif
}

static void disable_cleaning(CleanComponentsInfo* cleanComponentsInfo) {
#ifndef __TESTING__
	HAL_GPIO_WritePin(cleanComponentsInfo->vacuum_GPIOType, cleanComponentsInfo->vacuum_pin, GPIO_PIN_RESET);
#endif
}
