/*
 * driving.c
 *
 *  Created on: Aug 20, 2022
 *      Author: maluz
 */

#include "stm32l0xx_hal.h"
#include "types/motors.h"
#include "types/map_types.h"

// TODO: set based on motor rpm
int millis_to_turn = 1000;
int millis_to_drive = 1000;

int start_drive(MapInfo* mapInfo, MotorsInfo* motorsInfo) {
	return 0;
}

MapPosition get_cleaner_position(MapInfo* mapInfo) {
	MapPosition mapPosition;
	return mapPosition;
}


int drive_forward(int* obstacle_found, MotorsInfo* motorsInfo) {
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
