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

#endif /* INC_DRIVING_H_ */
