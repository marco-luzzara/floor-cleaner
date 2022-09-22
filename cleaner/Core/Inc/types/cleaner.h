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

typedef struct MotorsInfo {
  GPIO_TypeDef clockwise_left_GPIOType;
  uint16_t clockwise_left_pin;
  GPIO_TypeDef antiClockwise_left_GPIOType;
  uint16_t antiClockwise_left_pin;

  GPIO_TypeDef clockwise_right_GPIOType;
	uint16_t clockwise_right_pin;
	GPIO_TypeDef antiClockwise_right_GPIOType;
	uint16_t antiClockwise_right_pin;
} MotorsInfo;

typedef struct CleanComponentsInfo {
  GPIO_TypeDef vacuum_GPIOType;
  uint16_t vacuum_pin;
} CleanComponentsInfo;

#endif /* INC_TYPES_CLEANER_H_ */
