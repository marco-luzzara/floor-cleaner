#ifndef INC_DRIVING_H_
#define INC_DRIVING_H_

#include <stdbool.h>
#include "types/map.h"
#include "types/cleaner.h"

/**
 * @brief starts to drive
 * @retval 0 - the cleaner completed the cleaning process
 * 				 1 - cleaner not found
 * 				 2 - start position cannot be reached
 */
int start_drive(MapInfo* mapInfo,
								bool* obstacle_found,
								MotorsInfo* motorsInfo,
								CleanComponentsInfo* cleanComponentsInfo);

/**
 * @brief signals that the cleaner has finished its work
 */
//void end_drive();

#endif /* INC_DRIVING_H_ */
