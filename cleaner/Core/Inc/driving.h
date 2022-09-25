#ifndef INC_DRIVING_H_
#define INC_DRIVING_H_

/**
 * @brief starts to drive
 * @retval 0 - the cleaner completed the cleaning process
 * 				 1 - cleaner not found
 */
int start_drive(const MapInfo* mapInfo, const MotorsInfo* motorsInfo);

/**
 * @brief signals that the cleaner has finished its work
 */
void end_drive();

/**
 * @brief move the cleaner of one unit
 * @retval 0 - the cleaner reached the destination
 * 				 1 - an obstacle has been found, the cleaner returned to the initial position
 */
bool drive_forward(const int* obstacle_found, MotorsInfo* motorsInfo);

/**
 * @brief rotate the cleaner by 90° anti-clockwise
 */
void turn_left(const MotorsInfo* motorsInfo);

/**
 * @brief rotate the cleaner by 90° clockwise
 */
void turn_right(const MotorsInfo* motorsInfo);

#endif /* INC_DRIVING_H_ */
