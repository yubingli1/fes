/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
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
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "stdio.h"      //包含需要的头文件
#include "stdarg.h"			//包含需要的头文件 
#include "string.h"     //包含需要的头文件
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define USART1_REC_LEN  			600  	//定义最大接收字节数 600
#define USART1_SEND_LEN  			600  	//定义最大接收字节数 600
//#define USART1_TXBUFF_SIZE    256    //定义串口1 发送缓冲区大小 256字节	  
//#define USART1_RXBUFF_SIZE    256    //定义串口2 接收缓冲区大小 256字节
#define UART4_MAX_RECV_LEN		3000					//最大接收缓存字节数
#define UART4_MAX_SEND_LEN		3000					//最大发送缓存字节数
#define UART4_RX_EN 			1					//0,不接收;1,接收.

#define UART5_MAX_RECV_LEN		3000					//最大接收缓存字节数
#define UART5_MAX_SEND_LEN		3000					//最大发送缓存字节数
#define UART5_RX_EN 			1					//0,不接收;1,接收.

#define DATA_MAX_RECV_LEN		600					//最大接收缓存字节数
#define Screen_MAX_RECV_LEN		600					//最大发送缓存字节数


typedef __IO uint16_t vu16;


extern uint8_t  UART4_RX_BUF[UART4_MAX_RECV_LEN]; 		//接收缓冲,最大UART4_MAX_RECV_LEN字节
extern uint8_t  UART4_TX_BUF[UART4_MAX_SEND_LEN]; 		//发送缓冲,最大UART4_MAX_SEND_LEN字节
extern vu16 UART4_RX_STA;   						//接收数据状态
extern uint16_t UART4_cnt;

extern uint8_t  UART5_RX_BUF[UART5_MAX_RECV_LEN]; 		//接收缓冲,最大UART4_MAX_RECV_LEN字节
extern uint8_t  UART5_TX_BUF[UART5_MAX_SEND_LEN]; 		//发送缓冲,最大UART4_MAX_SEND_LEN字节
extern vu16 UART5_RX_STA;   						//接收数据状态
extern uint16_t UART5_cnt;

extern uint8_t  USART1_RX_BUF[USART1_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern uint8_t  USART1_TX_BUF[USART1_SEND_LEN]; //发送缓冲,最大USART_SEND_LEN个字节.末字节为换行符 
extern uint16_t USART1_RX_STA;         		//接收状态标记	
	

extern uint8_t  DATA_RX_BUF[DATA_MAX_RECV_LEN];//刺激器数据接收缓冲,最大DATA_MAX_RECV_LEN字节
extern uint16_t DATA_RX_STA;   						//接收刺激器数据状态


extern uint8_t  Screen_RX_BUF[Screen_MAX_RECV_LEN]; 		//触摸屏数据接收缓冲,最大DATA_MAX_RECV_LEN字节
extern uint16_t Screen_RX_STA;   						//接收触摸屏数据状态
/* USER CODE END Private defines */

void MX_UART4_Init(void);
void MX_UART5_Init(void);
void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void USART1_RxIdleCallback(void);
void UART1_Send(uint8_t uart_in[],uint8_t length);
void UART4_RxIdleCallback(void);
void UART5_RxIdleCallback(void);
void UART5_Send(uint8_t uart5_in[],uint8_t length);
void u3_printf(char* fmt,...);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
