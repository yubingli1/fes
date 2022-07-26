/**
  ******************************************************************************
  * File Name          : USART.c
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

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
/* USER CODE BEGIN 0 */
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (uint8_t) ch;      
	return ch;
}
#endif 

 uint8_t    USART1_RX_BUF[USART1_REC_LEN]; 		//���ջ���,���USART1_MAX_RECV_LEN�ֽ�
 uint8_t    USART1_TX_BUF[USART1_SEND_LEN]; 		//���ͻ���,���USART1_MAX_SEND_LEN�ֽ�
 uint16_t   USART1_RX_STA=0;
 uint8_t    USART1_cnt = 0;

 uint8_t Send_Endone = 0x0d;
 uint8_t Send_Endtwo = 0x0a;

 uint8_t  UART4_RX_BUF[UART4_MAX_RECV_LEN]; 		//���ջ���,���UART4_MAX_RECV_LEN�ֽ�
 uint8_t  UART4_TX_BUF[UART4_MAX_SEND_LEN]; 		//���ͻ���,���UART4_MAX_SEND_LEN�ֽ�
 vu16 UART4_RX_STA=0; 

 uint8_t  UART5_RX_BUF[UART5_MAX_RECV_LEN]; 		//���ջ���,���UART5_MAX_RECV_LEN�ֽ�
 uint8_t  UART5_TX_BUF[UART5_MAX_SEND_LEN]; 		//���ͻ���,���UART5_MAX_SEND_LEN�ֽ�
 vu16 UART5_RX_STA=0; 

 uint8_t  DATA_RX_BUF[DATA_MAX_RECV_LEN]; 		//���ݽ��ջ���,���DATA_MAX_RECV_LEN�ֽ�
 uint16_t DATA_RX_STA=0;
 
 uint8_t  Screen_RX_BUF[Screen_MAX_RECV_LEN]; 		//���ݽ��ջ���,���DATA_MAX_RECV_LEN�ֽ�
 uint16_t Screen_RX_STA=0;
 
uint16_t UART4_cnt = 0;
uint16_t UART5_cnt = 0;
uint8_t rx_cnt = 0;

/* USER CODE END 0 */

/* UART4 init function */
void MX_UART4_Init(void)
{
  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART4);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**UART4 GPIO Configuration
  PA0-WKUP   ------> UART4_TX
  PA1   ------> UART4_RX
  */
  GPIO_InitStruct.Pin = UART4_TX_Pin|UART4_RX_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* UART4 interrupt Init */
  NVIC_SetPriority(UART4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1, 0));
  NVIC_EnableIRQ(UART4_IRQn);

  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(UART4, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(UART4);
  LL_USART_Enable(UART4);

}
/* UART5 init function */
void MX_UART5_Init(void)
{
  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART5);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
  /**UART5 GPIO Configuration
  PC12   ------> UART5_TX
  PD2   ------> UART5_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_12;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* UART5 interrupt Init */
  NVIC_SetPriority(UART5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(UART5_IRQn);

  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(UART5, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(UART5);
  LL_USART_Enable(UART5);

}
/* USART1 init function */

void MX_USART1_UART_Init(void)
{
  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**USART1 GPIO Configuration
  PA9   ------> USART1_TX
  PA10   ------> USART1_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9|LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART1 interrupt Init */
  NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5, 0));
  NVIC_EnableIRQ(USART1_IRQn);

  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);

}

/* USER CODE BEGIN 1 */
void USART1_RxIdleCallback(void)
{
	  uint8_t Res;
	
		if(LL_USART_IsActiveFlag_RXNE(USART1) && LL_USART_IsEnabledIT_RXNE(USART1))
		{
		Res =LL_USART_ReceiveData8(USART1);	//��ȡ���յ�������		
		if((USART1_RX_STA&0x8000)==0)//����δ���
			{
			if(USART1_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)USART1_RX_STA=0;//���մ���,���¿�ʼ
				else USART1_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART1_RX_STA|=0x4000;
				else
					{
					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
					USART1_RX_STA++;
					if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}
			LL_USART_EnableIT_RXNE(USART1);																//�ٴο������ڽ����жϣ��Է��ж�λ�����㵼���жϱ��ر�	
		}
		
}  



void UART1_Send(uint8_t uart_in[],uint8_t length){
	for(int i=0; i<length; i++){
		LL_USART_TransmitData8(USART1,uart_in[i]);
		while(LL_USART_IsActiveFlag_TXE(USART1)==0);
	}
//	while(LL_USART_IsActiveFlag_TC(USART1) == 0);
//	LL_USART_TransmitData8(USART1,Send_Endone);
//	while(LL_USART_IsActiveFlag_TC(USART1) == 0);
//	LL_USART_TransmitData8(USART1,Send_Endtwo);
//	while(LL_USART_IsActiveFlag_TC(USART1) == 0);	
}



void UART4_RxIdleCallback(void)
{
	uint8_t res;	 	
	//
	
	if(LL_USART_IsActiveFlag_RXNE(UART4) && LL_USART_IsEnabledIT_RXNE(UART4))//���յ�����
	{	 

		res =LL_USART_ReceiveData8(UART4);	

		if((UART4_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
			{ 
				if(UART4_RX_STA<UART4_MAX_RECV_LEN)	//�����Խ�������
				{
					LL_TIM_SetCounter(TIM7,0);//���������  
					
					if(UART4_RX_STA==0)				//ʹ�ܶ�ʱ��7���ж� 
					{
						LL_TIM_EnableIT_UPDATE(TIM7);
						LL_TIM_EnableCounter(TIM7);// <==> TIM_Cmd(TIM7,ENABLE);//ʹ�ܶ�ʱ��7
					}
					
					UART4_RX_BUF[UART4_RX_STA++]=res;	//��¼���յ���ֵ
					DATA_RX_BUF[DATA_RX_STA++]=res; //��¼�̼�������
					UART4_cnt++;
					
				}else 
				{
					UART4_RX_STA|=1<<15;				//ǿ�Ʊ�ǽ������
					DATA_RX_STA|=1<<15;
				} 
			}
			
			LL_USART_EnableIT_RXNE(UART4);																//�ٴο������ڽ����жϣ��Է��ж�λ�����㵼���жϱ��ر�	
	
	}
  

}   


void UART5_RxIdleCallback(void)
{
	uint8_t res;	 	
	//
	
	if(LL_USART_IsActiveFlag_RXNE(UART5) && LL_USART_IsEnabledIT_RXNE(UART5))//���յ�����
	{	 

		res =LL_USART_ReceiveData8(UART5);	
		
		if((UART5_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
			{ 
				if(UART5_RX_STA<UART5_MAX_RECV_LEN)	//�����Խ�������
				{
					LL_TIM_SetCounter(TIM6,0);//���������  
					
					if(UART5_RX_STA==0)				//ʹ�ܶ�ʱ��6���ж� 
					{
						LL_TIM_EnableIT_UPDATE(TIM6);
						LL_TIM_EnableCounter(TIM6);// <==> TIM_Cmd(TIM6,ENABLE);//ʹ�ܶ�ʱ��6
					}
					
					UART5_RX_BUF[UART5_RX_STA++]=res;	//��¼���յ���ֵ
					Screen_RX_BUF[Screen_RX_STA++]=res; //��¼����
					UART5_cnt++;
					
				}else 
				{
					UART5_RX_STA|=1<<15;				//ǿ�Ʊ�ǽ������
					Screen_RX_STA|=1<<15;
				} 
			}
			
			LL_USART_EnableIT_RXNE(UART5);		//�ٴο������ڽ����жϣ��Է��ж�λ�����㵼���жϱ��ر�	
	
	}
  

}   

void UART5_Send(uint8_t uart5_in[],uint8_t length){
	for(int i=0; i<length; i++){
		LL_USART_TransmitData8(UART5,uart5_in[i]);
		while(LL_USART_IsActiveFlag_TXE(UART5)==0);
	}
}


//����3,printf ����
void u3_printf(char* fmt,...)  
{  
	uint16_t i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)UART4_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)UART4_TX_BUF);		//�˴η������ݵĳ���
	for(j=0;j<i;j++)							//ѭ����������
	{
		UART4->DR = UART4_TX_BUF[j];
    while((UART4->SR&0X40)==0);
	} 
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
