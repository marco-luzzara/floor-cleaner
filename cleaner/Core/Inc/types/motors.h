/*
 * motors.h
 *
 *  Created on: Sep 13, 2022
 *      Author: maluz
 */

#ifndef INC_TYPES_MOTORS_H_
#define INC_TYPES_MOTORS_H_

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

#endif /* INC_TYPES_MOTORS_H_ */
