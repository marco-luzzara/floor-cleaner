/*
 * cleaner.h
 *
 *  Created on: Sep 21, 2022
 *      Author: maluz
 */

#ifndef INC_TYPES_CLEANER_H_
#define INC_TYPES_CLEANER_H_

#include "types/map.h"
#include "types/utils.h"

typedef enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
} Direction;

typedef struct CleanerInfo {
	MapPosition position;
	Direction direction;
} CleanerInfo;

#endif /* INC_TYPES_CLEANER_H_ */
