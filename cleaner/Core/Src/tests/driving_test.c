/*
 * driving_test.c
 *
 *  Created on: Sep 28, 2022
 *      Author: maluz
 */
#ifdef __TESTING__

#include <stdlib.h>
#include "minunit.h"
#include "driving.h"
#include "types/map.h"

// it is global because necessary for the teardown
MapInfo mapInfo;

void test_setup(void) {
}

void test_teardown(void) {
	for (int r = 0; r < mapInfo.row_count; r++)
		free(mapInfo.map[r]);

	free(mapInfo.map);
}

void initialize_mapInfo(int row_count, int column_count, const MapPosition* cleaner_pos) {
	mapInfo.row_count = row_count;
	mapInfo.column_count = column_count;

	CellType** map = (CellType**)malloc(row_count * sizeof(CellType*));

	for (int r = 0; r < row_count; r++) {
		map[r] = (CellType*)malloc(column_count * sizeof(CellType));
		for (int c = 0; c < column_count; c++) {
			map[r][c] = TO_CLEAN;
		}
	}
	map[cleaner_pos->row][cleaner_pos->col] = CLEANER_POS;
	//

	mapInfo.map = map;
}

void assert_map_cleaned() {
	for (int r = 0; r < mapInfo.row_count; r++) {
		for (int c = 0; c < mapInfo.column_count; c++) {
			if (mapInfo.map[r][c] != UNAVAILABLE)
				mu_assert_int_eq(ALREADY_CLEANED, mapInfo.map[r][c]);
		}
	}
}

void start_drive_wrapper() {
	bool dummy_obstacle_found;
	mu_assert_int_eq(0, start_drive(&mapInfo, &dummy_obstacle_found, NULL, NULL));
}

MU_TEST(test_basic_map_no_wall) {
	initialize_mapInfo(1, 3, &(MapPosition) { .row = 0, .col = 0 });
	start_drive_wrapper();

	assert_map_cleaned();
}

MU_TEST_SUITE(test_suite) {
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

	MU_RUN_TEST(test_basic_map_no_wall);
}

int main(int argc, char *argv[]) {
	MU_RUN_SUITE(test_suite);
	MU_REPORT();

	return MU_EXIT_CODE;
}

#endif
