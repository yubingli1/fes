/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_gpio.h"

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
#define FAN_EN_Pin LL_GPIO_PIN_2
#define FAN_EN_GPIO_Port GPIOE
#define ESP32_nEN_Pin LL_GPIO_PIN_3
#define ESP32_nEN_GPIO_Port GPIOE
#define Insert_Pin LL_GPIO_PIN_4
#define Insert_GPIO_Port GPIOE
#define TIM9_CH1_Pin LL_GPIO_PIN_5
#define TIM9_CH1_GPIO_Port GPIOE
#define Key_Enc_Pin LL_GPIO_PIN_13
#define Key_Enc_GPIO_Port GPIOC
#define Key_R3_Pin LL_GPIO_PIN_0
#define Key_R3_GPIO_Port GPIOC
#define Key_R2_Pin LL_GPIO_PIN_1
#define Key_R2_GPIO_Port GPIOC
#define Key_R1_Pin LL_GPIO_PIN_2
#define Key_R1_GPIO_Port GPIOC
#define Key_R0_Pin LL_GPIO_PIN_3
#define Key_R0_GPIO_Port GPIOC
#define UART4_TX_Pin LL_GPIO_PIN_0
#define UART4_TX_GPIO_Port GPIOA
#define UART4_RX_Pin LL_GPIO_PIN_1
#define UART4_RX_GPIO_Port GPIOA
#define Key_C0_Pin LL_GPIO_PIN_4
#define Key_C0_GPIO_Port GPIOC
#define Key_C1_Pin LL_GPIO_PIN_5
#define Key_C1_GPIO_Port GPIOC
#define Key_HOME_Pin LL_GPIO_PIN_10
#define Key_HOME_GPIO_Port GPIOB
#define Key_HOME_EXTI_IRQn EXTI15_10_IRQn
#define Key_CANCEL_Pin LL_GPIO_PIN_12
#define Key_CANCEL_GPIO_Port GPIOB
#define Key_CANCEL_EXTI_IRQn EXTI15_10_IRQn
#define FPGA_nRST_Pin LL_GPIO_PIN_13
#define FPGA_nRST_GPIO_Port GPIOB
#define LED_5_Pin LL_GPIO_PIN_14
#define LED_5_GPIO_Port GPIOB
#define Screen_Busy_Pin LL_GPIO_PIN_15
#define Screen_Busy_GPIO_Port GPIOB
#define LED_4_Pin LL_GPIO_PIN_11
#define LED_4_GPIO_Port GPIOD
#define LED_3_Pin LL_GPIO_PIN_12
#define LED_3_GPIO_Port GPIOD
#define LED_2_Pin LL_GPIO_PIN_13
#define LED_2_GPIO_Port GPIOD
#define Key_C2_Pin LL_GPIO_PIN_8
#define Key_C2_GPIO_Port GPIOC
#define LED_1_Pin LL_GPIO_PIN_9
#define LED_1_GPIO_Port GPIOC
#define CAN_S_Pin LL_GPIO_PIN_8
#define CAN_S_GPIO_Port GPIOA
#define MCU_SWDIO_Pin LL_GPIO_PIN_13
#define MCU_SWDIO_GPIO_Port GPIOA
#define MCU_SWDCK_Pin LL_GPIO_PIN_14
#define MCU_SWDCK_GPIO_Port GPIOA
#define FLASH_nRESET_Pin LL_GPIO_PIN_15
#define FLASH_nRESET_GPIO_Port GPIOA
#define FLASH_WP_Pin LL_GPIO_PIN_10
#define FLASH_WP_GPIO_Port GPIOC
#define SPI_nCS_Pin LL_GPIO_PIN_11
#define SPI_nCS_GPIO_Port GPIOC
#define HV_Power_nEN_Pin LL_GPIO_PIN_6
#define HV_Power_nEN_GPIO_Port GPIOD
#define nINT_Pin LL_GPIO_PIN_8
#define nINT_GPIO_Port GPIOB
#define nEMG_Status_Pin LL_GPIO_PIN_0
#define nEMG_Status_GPIO_Port GPIOE
#define nQON_Pin LL_GPIO_PIN_1
#define nQON_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
