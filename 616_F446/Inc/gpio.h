/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for
  *                      the gpio
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
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "usart.h"
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define LED_1 PCout(9)    	//LED1
#define LED_2 PDout(13)   	//LED2
#define LED_4 PDout(11)   	//LED4


#define Key_R0 PCout(3)   	//Key_R0_Pin 输入
#define Key_R1 PCout(2)   	//Key_R1_Pin
#define Key_R2 PCout(1)   	//Key_R2_Pin
#define Key_R3 PCout(0)   	//Key_R3_Pin

#define Key_C0 PCout(4)   	//Key_C0_Pin 输出
#define Key_C1 PCout(5)   	//Key_C1_Pin
#define Key_C2 PCout(8)   	//Key_C2_Pin


extern int Interface_flag;

extern  uint8_t  Key_in[7];
extern  uint8_t  Key_length ;
extern  unsigned short int Interface_First;  //通道选择界面 上SW4 : First-6,  左SW5 : First-1,  右SW7 : First+6, 下SW9 : First+6,
extern  unsigned short int Interface_Sec;
extern  unsigned short int Interface_third;
extern int CANCEL_flag;
extern int TIM_flag;
/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
char Read_Key(void);
void Key_Scan(void);
void KEY_CANCEL_RxIdleCallback(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
