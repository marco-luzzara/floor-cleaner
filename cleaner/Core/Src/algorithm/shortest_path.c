/*
 * shortest_path.c
 *
 *  Created on: Sep 23, 2022
 *      Author: maluz
 */

#include <math.h>
#include "types/map.h"
#include "stm32l0xx_hal.h"
#include "algorithm/AStar.h"

static void path_node_neighbors(ASNeighborList neighbors, void *node, void *context)
{
	MapPosition* position = (MapPosition*) node;
	MapInfo* mapInfo = (MapInfo*) context;

	bool (*can_transit_on)(const MapPosition*) =
			lambda(bool, (const MapPosition* test_position), {
				return is_cell_valid(mapInfo, test_position) &&
							 (mapInfo->map[test_position->row][test_position->col] == ALREADY_CLEANED ||
									 mapInfo->map[test_position->row][test_position->col] == TO_CLEAN);
			});

	MapPosition up_position = { .row = position->row - 1, .col = position->col };
	if (can_transit_on(&up_position))
		ASNeighborListAdd(neighbors, &up_position, 1);

	MapPosition right_position = { .row = position->row, .col = position->col + 1 };
	if (can_transit_on(&right_position))
		ASNeighborListAdd(neighbors, &right_position, 1);

	MapPosition down_position = { .row = position->row + 1, .col = position->col };
	if (can_transit_on(&down_position))
		ASNeighborListAdd(neighbors, &down_position, 1);

	MapPosition left_position = { .row = position->row, .col = position->col - 1 };
	if (can_transit_on(&left_position))
		ASNeighborListAdd(neighbors, &left_position, 1);
}

static float path_node_heuristic(void *fromNode, void *toNode, void *context)
{
    MapPosition* from = (MapPosition*) fromNode;
    MapPosition* to = (MapPosition*) toNode;

    return fabs(from->row - to->row) + fabs(from->col - to->col);
}

bool find_best_path(const MapPosition* start,
										const MapPosition* end,
										const MapInfo* mapInfo,
										ASPath* path) {
	ASPathNodeSource path_node_source =
	{
	    sizeof(MapPosition),
	    &path_node_neighbors,
	    &path_node_heuristic,
	    NULL,
	    NULL
	};
	*path = ASPathCreate(&path_node_source, (void*) mapInfo, (void*) start, (void*) end);

	return ASPathGetCount(*path) > 0;
}
