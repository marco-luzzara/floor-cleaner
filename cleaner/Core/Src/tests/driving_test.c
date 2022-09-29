/*
 * driving_test.c
 *
 *  Created on: Sep 28, 2022
 *      Author: maluz
 */
#ifdef __TESTING__

#include "minunit.h"
#include "driving.h"

void initialize_mapInfo(int row_count, int column_count, MapInfo* mapInfo) {
	mapInfo->row_count = row_count;
	mapInfo->column_count = column_count;

	CellType map[row_count][column_count];

	for (int r = 0; r < row_count; r++)
		for (int c = 0; c < column_count; c++)
			map[r][c] = TO_CLEAN;

	mapInfo->map = map;
}

void start_drive_wrapper(MapInfo* mapInfo) {
	start_drive(mapInfo, NULL, NULL, NULL);
}

MU_TEST(test_check) {
	MapInfo mapInfo;
	initialize_mapInfo(3, 3, &mapInfo);
	start_drive_wrapper(&mapInfo);
}

MU_TEST_SUITE(test_suite) {
	MU_RUN_TEST(test_check);
}

int main(int argc, char *argv[]) {
	MU_RUN_SUITE(test_suite);
	MU_REPORT();

	return MU_EXIT_CODE;
}

#endif
