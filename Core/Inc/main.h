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
#include "stm32f1xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BUZZ_Pin GPIO_PIN_5
#define BUZZ_GPIO_Port GPIOE
#define K3_Pin GPIO_PIN_0
#define K3_GPIO_Port GPIOC
#define K3_EXTI_IRQn EXTI0_IRQn
#define K2_Pin GPIO_PIN_1
#define K2_GPIO_Port GPIOC
#define K2_EXTI_IRQn EXTI1_IRQn
#define K1_Pin GPIO_PIN_2
#define K1_GPIO_Port GPIOC
#define K1_EXTI_IRQn EXTI2_IRQn
#define A_Pin GPIO_PIN_8
#define A_GPIO_Port GPIOE
#define H_Pin GPIO_PIN_15
#define H_GPIO_Port GPIOE
#define LED_LOCK_Pin GPIO_PIN_3
#define LED_LOCK_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
