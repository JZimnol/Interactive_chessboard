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
#include "stm32f4xx_hal.h"

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
#define RedLed_Pin GPIO_PIN_13
#define RedLed_GPIO_Port GPIOC
#define MUX_A_Pin GPIO_PIN_0
#define MUX_A_GPIO_Port GPIOB
#define MUX_B_Pin GPIO_PIN_1
#define MUX_B_GPIO_Port GPIOB
#define MUX_C_Pin GPIO_PIN_2
#define MUX_C_GPIO_Port GPIOB
#define SPI2_LATCH_Pin GPIO_PIN_14
#define SPI2_LATCH_GPIO_Port GPIOB
#define P2_changeModePlayer_Pin GPIO_PIN_3
#define P2_changeModePlayer_GPIO_Port GPIOB
#define P2_changeModePlayer_EXTI_IRQn EXTI3_IRQn
#define P1_changeModePlayer_Pin GPIO_PIN_4
#define P1_changeModePlayer_GPIO_Port GPIOB
#define P1_changeModePlayer_EXTI_IRQn EXTI4_IRQn
#define P1_forward_Pin GPIO_PIN_5
#define P1_forward_GPIO_Port GPIOB
#define P1_forward_EXTI_IRQn EXTI9_5_IRQn
#define P1_backward_Pin GPIO_PIN_6
#define P1_backward_GPIO_Port GPIOB
#define P1_backward_EXTI_IRQn EXTI9_5_IRQn
#define P2_forward_Pin GPIO_PIN_7
#define P2_forward_GPIO_Port GPIOB
#define P2_forward_EXTI_IRQn EXTI9_5_IRQn
#define P2_backward_Pin GPIO_PIN_8
#define P2_backward_GPIO_Port GPIOB
#define P2_backward_EXTI_IRQn EXTI9_5_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
