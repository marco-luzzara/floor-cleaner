/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define START_BUTTON_Pin GPIO_PIN_13
#define START_BUTTON_GPIO_Port GPIOC
#define MOTOR_1___IN1_Pin GPIO_PIN_0
#define MOTOR_1___IN1_GPIO_Port GPIOC
#define MOTOR_1___IN2_Pin GPIO_PIN_1
#define MOTOR_1___IN2_GPIO_Port GPIOC
#define MOTOR_2___IN3_Pin GPIO_PIN_2
#define MOTOR_2___IN3_GPIO_Port GPIOC
#define MOTOR_2___IN4_Pin GPIO_PIN_3
#define MOTOR_2___IN4_GPIO_Port GPIOC
#define MOTOR_1___Speed_Pin GPIO_PIN_0
#define MOTOR_1___Speed_GPIO_Port GPIOA
#define MOTOR_2___Speed_Pin GPIO_PIN_1
#define MOTOR_2___Speed_GPIO_Port GPIOA
#define DISTANCE_SENSOR_Pin GPIO_PIN_9
#define DISTANCE_SENSOR_GPIO_Port GPIOA
#define DISTANCE_SENSOR_EXTI_IRQn EXTI4_15_IRQn
#define VACUUM_Pin GPIO_PIN_10
#define VACUUM_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_12
#define BUZZER_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
