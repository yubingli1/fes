/**
  ******************************************************************************
  * File Name          : I2C.h
  * Description        : This file provides code for the configuration
  *                      of the I2C instances.
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
#ifndef __i2c_H
#define __i2c_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "sys.h"
/* USER CODE END Includes */

extern I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN Private defines */


//IO��������
#define SDA_IN() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}//PB9 ����ģʽ
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9 ���ģʽ
#define SCL_OUT() {GPIOB->MODER&=~(3<<(6*2));GPIOB->MODER|=1<<6*2;} //PB6 ���ģʽ
//IO����
#define IIC_SCL   PBout(6) //SCL
#define IIC_SDA   PBout(9) //SDA
#define READ_SDA  PBin(9)  //����SDA

//IIC���в�������	 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
uint8_t IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�


void IIC_Send_Byte(uint8_t txd);			//IIC����
void IIC_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite);		//ָ����ַд��
uint8_t IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_ReadOneByte(uint16_t ReadAddr);							//ָ����ַ��ȡһ���ֽ�

void IIC_Send_Two_Byte(uint16_t two_txd);	
void IIC_WriteTwoByte(uint16_t Two_WriteAddr,uint16_t Two_DataToWrite);//ָ����ַд��
uint16_t IIC_ReadTwoByte(uint16_t Two_ReadAddr);//ָ����ַ��ȡ
uint16_t IIC_Read_Two_Byte(unsigned char two_ack);





void BQ_25792_init(void);
	
/* USER CODE END Private defines */

void MX_I2C1_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ i2c_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
