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
#include "usb_debugger.h"
#include "config.h"
#include "data_structs.h"
#include "pinout.h"
#include "IMU.h"
#include "CU.h"
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
void MCU_startingProtocol(void);
void MCU_updateTelemetry(void);
void MCU_trasmitTelemetry(void);
void MCU_saveTelemetry(void);
void MCU_endingProtocol(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IMU_Latch_Pin GPIO_PIN_1
#define IMU_Latch_GPIO_Port GPIOC
#define CU_Latch_Pin GPIO_PIN_4
#define CU_Latch_GPIO_Port GPIOC
#define IMU_nOE_Pin GPIO_PIN_5
#define IMU_nOE_GPIO_Port GPIOC
#define MCU_LED_Pin GPIO_PIN_10
#define MCU_LED_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
