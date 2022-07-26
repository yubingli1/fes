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


//IO方向设置
#define SDA_IN() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}//PB9 输入模式
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9 输出模式
#define SCL_OUT() {GPIOB->MODER&=~(3<<(6*2));GPIOB->MODER|=1<<6*2;} //PB6 输出模式
//IO操作
#define IIC_SCL   PBout(6) //SCL
#define IIC_SDA   PBout(9) //SDA
#define READ_SDA  PBin(9)  //输入SDA

//IIC所有操作函数	 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
uint8_t IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号


void IIC_Send_Byte(uint8_t txd);			//IIC发送
void IIC_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite);		//指定地址写入
uint8_t IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_ReadOneByte(uint16_t ReadAddr);							//指定地址读取一个字节

void IIC_Send_Two_Byte(uint16_t two_txd);	
void IIC_WriteTwoByte(uint16_t Two_WriteAddr,uint16_t Two_DataToWrite);//指定地址写入
uint16_t IIC_ReadTwoByte(uint16_t Two_ReadAddr);//指定地址读取
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
