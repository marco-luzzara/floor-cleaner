/*
 * shortest_path.h
 *
 *  Created on: Sep 23, 2022
 *      Author: maluz
 */

#ifndef INC_ALGORITHM_SHORTEST_PATH_H_
#define INC_ALGORITHM_SHORTEST_PATH_H_

#include "types/map.h"
#include "algorithm/AStar.h"

bool find_best_path(const MapPosition* start,
										const MapPosition* end,
										const MapInfo* mapInfo,
										MapPosition* path[],
										size_t* path_size);

#endif /* INC_ALGORITHM_SHORTEST_PATH_H_ */
