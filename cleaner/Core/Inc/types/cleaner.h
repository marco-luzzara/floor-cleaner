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

/*
 * Input1		Input2		Spinning Direction
 * Low(0)		Low(0)		Motor OFF
 * High(1)	Low(0)		Forward = ClockWise
 * Low(0)		High(1)		Backward = AntiClockWise
 * High(1)	High(1)		Motor OFF
 *
 * For L298N:
 * - IN1 and IN2 are for left
 * - IN3 and IN4 are for right
 */
typedef struct MotorsInfo {
  GPIO_TypeDef left1_GPIOType;
  uint16_t left1_pin;
  GPIO_TypeDef left2_GPIOType;
  uint16_t left2_pin;

  GPIO_TypeDef right1_GPIOType;
	uint16_t right1_pin;
	GPIO_TypeDef right2_GPIOType;
	uint16_t right2_pin;
} MotorsInfo;

typedef struct CleanComponentsInfo {
  GPIO_TypeDef vacuum_GPIOType;
  uint16_t vacuum_pin;
} CleanComponentsInfo;

#endif /* INC_TYPES_CLEANER_H_ */
