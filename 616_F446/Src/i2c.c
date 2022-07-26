/**
  ******************************************************************************
  * File Name          : I2C.c
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

/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* USER CODE BEGIN 0 */
#include "delay.h"
#include "usart.h"

/* USER CODE END 0 */

I2C_HandleTypeDef hi2c1;

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();

    /* I2C1 interrupt Init */
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_9);

    /* I2C1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_6);//IIC_SCL=1;
	LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_9);//IIC_SDA=1;	 
	delay_us(4);
 	LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_9);//IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_6);//IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
	
}
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_6);//IIC_SCL=0;
	LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_9);//IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_6);//IIC_SCL=1;
	LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_9);//IIC_SDA=1;	发送I2C总线结束信号
	delay_us(4);		
  
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_9);//IIC_SDA=1;
	delay_us(4);	   
	LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_6);//IIC_SCL=1;
	delay_us(4);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_6);//IC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_6);//IIC_SCL=0;
	SDA_OUT();
	LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_9);//IIC_SDA=0;
	delay_us(4);
	LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_6);//IIC_SCL=1;
	delay_us(4);
	LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_6);//IIC_SCL=0;
	delay_us(1);	
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_6);//IIC_SCL=0;
	SDA_OUT();
	LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_9);//IIC_SDA=1;
	delay_us(4);
	LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_6);//IIC_SCL=1;
	delay_us(4);
	LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_6);//IIC_SCL=0;
	delay_us(1);	
}					 		

/***************************************************读写8位数据***************************************************/

//IIC发送
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	  SDA_OUT(); 	
    LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_6);//IIC_SCL=0;//拉低时钟开始数据传输

    for(t=0;t<8;t++)
	{              
			IIC_SDA=(txd&0x80)>>7;
			txd<<=1; 	  
			delay_us(4);   
			LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_6);//IIC_SCL=1;
			delay_us(4); 
			LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_6);//IIC_SCL=0;	
			
    }	 
} 	    

void IIC_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite){
	
	IIC_Start();  
	IIC_Send_Byte(0XD6+((WriteAddr/256)<<1)); //发送器件地址0XD6,写数据
	IIC_Wait_Ack(); 
	IIC_Send_Byte(WriteAddr%256); 
	IIC_Wait_Ack(); 
	IIC_Send_Byte(DataToWrite); //发送字节 
	IIC_Wait_Ack(); 
	IIC_Stop();//产生一个停止条件
	delay_ms(2);
	//printf("IIC_WriteOneByte! \r\n");
}


//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_6);//IIC_SCL=0; 
        delay_us(4);
		    LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_6);//IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(4); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}

//在指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
uint8_t IIC_ReadOneByte(uint16_t ReadAddr)
{				  
	uint8_t temp=0;	
	
  IIC_Start();  
	IIC_Send_Byte(0XD6+((ReadAddr/256)<<1));   //发送器件地址0XD6,写数据 
	IIC_Wait_Ack(); 
  IIC_Send_Byte(ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();	  
  delay_us(2);
	IIC_Start();  	 	   
	IIC_Send_Byte(0XD7);           //进入接收模式			   
	IIC_Wait_Ack();	 
	
  temp=IIC_Read_Byte(0);	
	
  IIC_Stop();//产生一个停止条件	 
  //printf("IIC_ReadOneByte! \r\n");	
	return temp;
}


/***************************************************读写16位数据***************************************************/

void IIC_WriteTwoByte(uint16_t Two_WriteAddr,uint16_t Two_DataToWrite){
	
	//printf("Two_WriteAddr = %d,    Two_DataToWrite = %d! \r\n",Two_WriteAddr,Two_DataToWrite);
	IIC_Start();  
	IIC_Send_Byte(0XD6+((Two_WriteAddr/256)<<1)); //发送器件地址0XD6,写数据
	IIC_Wait_Ack(); 
	IIC_Send_Byte(Two_WriteAddr%256); 
	IIC_Wait_Ack(); 
	IIC_Send_Byte(Two_DataToWrite/256); //发送字节。先高位再低位 
	IIC_Wait_Ack(); 
	IIC_Send_Byte(Two_DataToWrite%256); //发送字节。先高位再低位 
	IIC_Wait_Ack(); 
	IIC_Stop();//产生一个停止条件
	delay_ms(2);
	//printf("IIC_WriteTwoByte! \r\n");
}




//在指定地址读出2个数据
//Two_ReadAddr:开始读数的地址  
//返回值  :读到的数据

uint16_t IIC_ReadTwoByte(uint16_t Two_ReadAddr)
{				  
	uint8_t temp_H=0;	
	uint8_t temp_L=0;
	uint16_t temp=0;
  IIC_Start();  
	IIC_Send_Byte(0XD6+((Two_ReadAddr/256)<<1));   //发送器件地址0XD6,写数据 
	IIC_Wait_Ack(); 
  IIC_Send_Byte(Two_ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();	  
  delay_us(2);
	IIC_Start();  	 	   
	IIC_Send_Byte(0XD7);           //进入接收模式	先高位再低位		   
	IIC_Wait_Ack();	 
	
  temp_H=IIC_Read_Byte(1);
	temp_L=IIC_Read_Byte(0);
	
  IIC_Stop();//产生一个停止条件	 
	temp = ((unsigned int)temp_H<<8)|temp_L;
  //printf("IIC_ReadTwoByte! \r\n");
	//printf("temp = %d! \r\n",temp);
	return temp;
}






void BQ_25792_init(void){  //bq25792初始化，写工作电压和看门狗
	 
	uint8_t Vsysmin_data = 0;
	uint8_t Watchdog_data = 0;	
	uint8_t Charger_Control = 0;	
	uint16_t Charge_Current = 0;
	uint16_t Input_Current = 0;
	uint8_t Endac_data = 0;
	uint8_t En_Charge = 0;

	
	
	Vsysmin_data = IIC_ReadOneByte(0x00);
	Watchdog_data = IIC_ReadOneByte(0x10);	
	Charger_Control = IIC_ReadOneByte(0x14);
	Input_Current = IIC_ReadTwoByte(0x06);
	Charge_Current = IIC_ReadTwoByte(0x03);
	Endac_data = IIC_ReadOneByte(0X2E);
	En_Charge = IIC_ReadOneByte(0X0F);
	delay_ms(40);
	

// 9V -> 12V
	while(Vsysmin_data!=38){
		IIC_WriteOneByte(0,38); 
		Vsysmin_data = IIC_ReadOneByte(0x00);
	}
  	//printf("Vsysmin_data 0X00 is: %d  \r\n",Vsysmin_data);
	
	//Wstchdog禁用
	while(Watchdog_data!=144){
		IIC_WriteOneByte(16,144); 
		Watchdog_data = IIC_ReadOneByte(0x10);
	}
		//printf("Watchdog_data 0X10 is: %d  \r\n",Watchdog_data);	
	
//关闭充电
//	while(En_Charge!=130){
//		IIC_WriteOneByte(15,130); 
//		En_Charge = IIC_ReadOneByte(0x0F);
//	}
		//printf("En_Charge 0X0F is: %d  \r\n",En_Charge);	
	
//开启ADC
	while(Endac_data!=176){
		IIC_WriteOneByte(46,176); 
		Endac_data = IIC_ReadOneByte(0x2E);
	}
		//printf("Endac_data 0X2E is: %d  \r\n",Endac_data);
	
//Charger_Control
 while(Charger_Control!=20){
		IIC_WriteOneByte(20,20);
		Charger_Control = IIC_ReadOneByte(0x14);
			//printf("Charger_Control 0X14 is: %d  \r\n",Charger_Control);	
	}		
	
//IINDPM 3.3A
		while(Input_Current!=330){
		IIC_WriteTwoByte(6,330); 
		Input_Current = IIC_ReadTwoByte(0x06);
		
	}
		//printf("Input_Current 0X06 is: %d  \r\n",Input_Current);	
	
//5A
		while(Charge_Current!=500){
		IIC_WriteTwoByte(3,500); 
		Charge_Current = IIC_ReadTwoByte(0x03);
		
	}
		//printf("Charge_Current 0X03 is: %d  \r\n",Charge_Current);			
	
	
	delay_ms(4);
	delay_ms(4);
	//IIC_WriteOneByte(46,26);  //数值待确认
	//delay_ms(40);
	
	
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
