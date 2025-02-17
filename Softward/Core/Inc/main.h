/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#define Key_Up_Pin GPIO_PIN_2
#define Key_Up_GPIO_Port GPIOC
#define Key_Down_Pin GPIO_PIN_3
#define Key_Down_GPIO_Port GPIOC
#define AIN2_Pin GPIO_PIN_1
#define AIN2_GPIO_Port GPIOA
#define AIN1_Pin GPIO_PIN_2
#define AIN1_GPIO_Port GPIOA
#define BIN1_Pin GPIO_PIN_7
#define BIN1_GPIO_Port GPIOE
#define BIN2_Pin GPIO_PIN_8
#define BIN2_GPIO_Port GPIOE
#define Key_No_Pin GPIO_PIN_12
#define Key_No_GPIO_Port GPIOB
#define Key_No_EXTI_IRQn EXTI15_10_IRQn
#define Key_Yes_Pin GPIO_PIN_13
#define Key_Yes_GPIO_Port GPIOB
#define Key_Yes_EXTI_IRQn EXTI15_10_IRQn
#define ST7789_CS_Pin GPIO_PIN_15
#define ST7789_CS_GPIO_Port GPIOA
#define ST7789_SCL_Pin GPIO_PIN_10
#define ST7789_SCL_GPIO_Port GPIOC
#define ST7789_DC_Pin GPIO_PIN_11
#define ST7789_DC_GPIO_Port GPIOC
#define ST7789_SDA_Pin GPIO_PIN_12
#define ST7789_SDA_GPIO_Port GPIOC
#define ST7789_BLK_Pin GPIO_PIN_5
#define ST7789_BLK_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
