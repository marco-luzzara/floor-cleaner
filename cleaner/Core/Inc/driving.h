#ifndef INC_DRIVING_H_
#define INC_DRIVING_H_

/**
 * @brief starts to drive
 * @retval 0 - the cleaner completed the cleaning process
 * 				 1 - otherwise
 */
int start_drive(uint8_t** map, MotorsInfo* motorsInfo);

/**
 * @brief signals that the cleaner has finished its work
 */
void end_drive();

/**
 * @brief move the cleaner of one unit
 * @retval 0 - the cleaner reached the destination
 * 				 1 - an obstacle has been found, the cleaner returned to the initial position
 */
int drive_forward(int millisToDrive, int* obstacleFound, MotorsInfo* motorsInfo);

/**
 * @brief rotate the cleaner by 90° anti-clockwise
 */
void turn_left(MotorsInfo* motorsInfo);

/**
 * @brief rotate the cleaner by 90° clockwise
 */
void turn_right(MotorsInfo* motorsInfo);

#endif /* INC_DRIVING_H_ */
