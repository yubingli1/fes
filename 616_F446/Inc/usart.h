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
#include "stdio.h"      //������Ҫ��ͷ�ļ�
#include "stdarg.h"			//������Ҫ��ͷ�ļ� 
#include "string.h"     //������Ҫ��ͷ�ļ�
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define USART1_REC_LEN  			600  	//�����������ֽ��� 600
#define USART1_SEND_LEN  			600  	//�����������ֽ��� 600
//#define USART1_TXBUFF_SIZE    256    //���崮��1 ���ͻ�������С 256�ֽ�	  
//#define USART1_RXBUFF_SIZE    256    //���崮��2 ���ջ�������С 256�ֽ�
#define UART4_MAX_RECV_LEN		3000					//�����ջ����ֽ���
#define UART4_MAX_SEND_LEN		3000					//����ͻ����ֽ���
#define UART4_RX_EN 			1					//0,������;1,����.

#define UART5_MAX_RECV_LEN		3000					//�����ջ����ֽ���
#define UART5_MAX_SEND_LEN		3000					//����ͻ����ֽ���
#define UART5_RX_EN 			1					//0,������;1,����.

#define DATA_MAX_RECV_LEN		600					//�����ջ����ֽ���
#define Screen_MAX_RECV_LEN		600					//����ͻ����ֽ���


typedef __IO uint16_t vu16;


extern uint8_t  UART4_RX_BUF[UART4_MAX_RECV_LEN]; 		//���ջ���,���UART4_MAX_RECV_LEN�ֽ�
extern uint8_t  UART4_TX_BUF[UART4_MAX_SEND_LEN]; 		//���ͻ���,���UART4_MAX_SEND_LEN�ֽ�
extern vu16 UART4_RX_STA;   						//��������״̬
extern uint16_t UART4_cnt;

extern uint8_t  UART5_RX_BUF[UART5_MAX_RECV_LEN]; 		//���ջ���,���UART4_MAX_RECV_LEN�ֽ�
extern uint8_t  UART5_TX_BUF[UART5_MAX_SEND_LEN]; 		//���ͻ���,���UART4_MAX_SEND_LEN�ֽ�
extern vu16 UART5_RX_STA;   						//��������״̬
extern uint16_t UART5_cnt;

extern uint8_t  USART1_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint8_t  USART1_TX_BUF[USART1_SEND_LEN]; //���ͻ���,���USART_SEND_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint16_t USART1_RX_STA;         		//����״̬���	
	

extern uint8_t  DATA_RX_BUF[DATA_MAX_RECV_LEN];//�̼������ݽ��ջ���,���DATA_MAX_RECV_LEN�ֽ�
extern uint16_t DATA_RX_STA;   						//���մ̼�������״̬


extern uint8_t  Screen_RX_BUF[Screen_MAX_RECV_LEN]; 		//���������ݽ��ջ���,���DATA_MAX_RECV_LEN�ֽ�
extern uint16_t Screen_RX_STA;   						//���մ���������״̬
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
