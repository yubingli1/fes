/**
  ******************************************************************************
  * File Name          : TIM.h
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __tim_H
#define __tim_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "usart.h"
#include "UART_menu.h"
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
void TIM3_init(void);
void TIM3_Input_capture(void);
void TIM9_Input_capture(void);
void TIM7_IRQ(void);
void TIM6_IRQ(void);

extern uint32_t TIM3_CH1_dval;
extern uint16_t sec_cnt;
//extern uint16_t tim_data[1];
/* USER CODE END Private defines */

void MX_TIM3_Init(void);
void MX_TIM6_Init(void);
void MX_TIM7_Init(void);
void MX_TIM9_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ tim_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
