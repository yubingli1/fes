/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
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
#include "gpio.h"
/* USER CODE BEGIN 0 */
#include "sys.h"
#include "Screen.h"
#include "delay.h"
#include "tim.h"
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

uint8_t  Key_in[7] = {0x5A,0XA5,0X04,0X80,0X03,0,0};
unsigned int Interface_Sec_addr[60] = {0x10,0x11,0x12,0x13,0x14,0x20,0x21,0x22,0x23,0x24,
																			 0x30,0x31,0x32,0x33,0x34,0x40,0x41,0x42,0x43,0x44,
                                       0x50,0x51,0x52,0x53,0x54,0x60,0x61,0x62,0x63,0x64,
																			 0x70,0x71,0x72,0x73,0x74,0x80,0x81,0x82,0x83,0x84,
                                       0x90,0x91,0x92,0x93,0x94,0xA0,0xA1,0xA2,0xA3,0xA4,
																			 0xB0,0xB1,0xB2,0xB3,0xB4,0xC0,0xC1,0xC2,0xC3,0xC4};
unsigned int Interface_third_addr[12] = {0x15,0x25,0x35,0x45,0x55,0x65,0x75,0x85,0x95,0xA5,0xB5,0xC5};
uint8_t  Key_length = 7;
int Interface_flag = 0; //0:默认状态，主界面  1：通道选择界面  2：参数选择界面     确认SW6 : flag+1,   返回SW8 : flag-1
unsigned short int Interface_First = 23;  //通道选择界面 上SW4 : First-6,  左SW5 : First-1,  右SW7 : First+6, 下SW9 : First+6,
unsigned short int Interface_Sec = 35;    //参数选择界面 上SW4 : Sec-2,    左SW5 : Sec-1,    右SW7 : Sec+1,   下SW9 : Sec+2,
unsigned short int Interface_third = 97;  //输出强度选择界面 上SW4 : Third-1,    左SW5 : Third-4,    右SW7 : Third+4,   下SW9 : Third+1,

int CANCEL_flag=0;
int	TIM_flag=0;			 														 
uint16_t Key_addr;
uint16_t Key_data[1];
uint8_t  Key_cnt;

uint16_t tim_data[1] = {0};
uint16_t tim_addr = 0;
uint8_t tim_cnt = 0;
uint8_t  tim_in[8] = {0x5A,0XA5,0X05,0X82,0X00,0,0,0};
uint8_t  tim_length = 8;

unsigned int tim_offset_init[72]={0x10,0x11,0x12,0x13,0x14,0x20,0x21,0x22,0x23,0x24,
                                    0x30,0x31,0x32,0x33,0x34,0x40,0x41,0x42,0x43,0x44, 
                                    0x50,0x61,0x62,0x63,0x54,0x60,0x61,0x62,0x63,0x64, 
                                    0x70,0x71,0x72,0x73,0x74,0x80,0x81,0x82,0x83,0x84, 
                                    0x90,0x91,0x92,0x93,0x94,0xA0,0xA1,0xA2,0xA3,0xA4, 
                                    0xB0,0xB1,0xB2,0xB3,0xB4,0xC0,0xC1,0xC2,0xC3,0xC4,
                                    0x15,0x25,0x35,0x45,0x55,0x65,0x75,0x85,0x95,0xA5,0xb5,0xc5};

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);

  /**/
  LL_GPIO_ResetOutputPin(GPIOE, FAN_EN_Pin|ESP32_nEN_Pin|Insert_Pin|nEMG_Status_Pin
                          |nQON_Pin);

  /**/
  LL_GPIO_ResetOutputPin(GPIOC, Key_Enc_Pin|Key_C0_Pin|Key_C1_Pin|Key_C2_Pin
                          |LED_1_Pin|FLASH_WP_Pin);

  /**/
  LL_GPIO_ResetOutputPin(GPIOB, FPGA_nRST_Pin|LED_5_Pin|nINT_Pin);

  /**/
  LL_GPIO_ResetOutputPin(GPIOD, LED_4_Pin|LED_3_Pin|LED_2_Pin|HV_Power_nEN_Pin);

  /**/
  LL_GPIO_ResetOutputPin(GPIOA, CAN_S_Pin|MCU_SWDIO_Pin|MCU_SWDCK_Pin|FLASH_nRESET_Pin);

  /**/
  GPIO_InitStruct.Pin = FAN_EN_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(FAN_EN_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = ESP32_nEN_Pin|Insert_Pin|nEMG_Status_Pin|nQON_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = Key_Enc_Pin|Key_C2_Pin|LED_1_Pin|FLASH_WP_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = Key_R3_Pin|Key_R2_Pin|Key_R1_Pin|Key_R0_Pin
                          |SPI_nCS_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = Key_C0_Pin|Key_C1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = FPGA_nRST_Pin|LED_5_Pin|nINT_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = Screen_Busy_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(Screen_Busy_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LED_4_Pin|LED_3_Pin|LED_2_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = CAN_S_Pin|MCU_SWDIO_Pin|MCU_SWDCK_Pin|FLASH_nRESET_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = HV_Power_nEN_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(HV_Power_nEN_GPIO_Port, &GPIO_InitStruct);

  /**/
  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB, LL_SYSCFG_EXTI_LINE10);

  /**/
  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB, LL_SYSCFG_EXTI_LINE12);

  /**/
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_10;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /**/
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_12;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /**/
  LL_GPIO_SetPinPull(Key_HOME_GPIO_Port, Key_HOME_Pin, LL_GPIO_PULL_NO);

  /**/
  LL_GPIO_SetPinPull(Key_CANCEL_GPIO_Port, Key_CANCEL_Pin, LL_GPIO_PULL_NO);

  /**/
  LL_GPIO_SetPinMode(Key_HOME_GPIO_Port, Key_HOME_Pin, LL_GPIO_MODE_INPUT);

  /**/
  LL_GPIO_SetPinMode(Key_CANCEL_GPIO_Port, Key_CANCEL_Pin, LL_GPIO_MODE_INPUT);

  /* EXTI interrupt init*/
  NVIC_SetPriority(EXTI15_10_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1, 0));
  NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 2 */


/***************************************************************************************************************************************/
/***************************************************************************************************************************************/
/***********************************************            键盘扫描            ********************************************************/
/***************************************************************************************************************************************/
/***************************************************************************************************************************************/



char Read_Key(void)
{

	LL_GPIO_ResetOutputPin(GPIOC,Key_C0_Pin); //按列依次置低扫描
	LL_GPIO_SetOutputPin(GPIOC,Key_C1_Pin);
	LL_GPIO_SetOutputPin(GPIOC,Key_C2_Pin);
	if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_3)==RESET)
	{
		LL_mDelay(10);
		if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_3)==RESET)
		return 1;
	}
	if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_2)==RESET)
	{
		LL_mDelay(10);
		if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_2)==RESET)
		return 2;
	}
	if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_1)==RESET)
	{
		LL_mDelay(10);
		if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_1)==RESET)
		return 3;
	}
	if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_0)==RESET)
	{
		LL_mDelay(10);
		if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_0)==RESET)
		return 4;
	}
	
	
	LL_GPIO_SetOutputPin(GPIOC,Key_C0_Pin); 
	LL_GPIO_ResetOutputPin(GPIOC,Key_C1_Pin);//按列依次置低扫描
	LL_GPIO_SetOutputPin(GPIOC,Key_C2_Pin);
	if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_0)==RESET)
	{
		LL_mDelay(10);
		if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_0)==RESET)
		return 5;      //开始
	}
	if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_1)==RESET)
	{
		LL_mDelay(10);
		if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_1)==RESET)
		return 6;    //暂停
	}
	if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_2)==RESET)
	{
		LL_mDelay(10);
		if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_2)==RESET)
		return 7;   //结束
	}
	if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_3)==RESET)
	{
		LL_mDelay(10);
		if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_3)==RESET){
			while(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_3)==RESET)
		  return 8;
		}			//确定
	}




	LL_GPIO_SetOutputPin(GPIOC,Key_C0_Pin); 
	LL_GPIO_SetOutputPin(GPIOC,Key_C1_Pin);
	LL_GPIO_ResetOutputPin(GPIOC,Key_C2_Pin);//按列依次置低扫描
	if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_0)==RESET)
	{
		LL_mDelay(10);
		if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_0)==RESET)
		return 9;    //上
	}
	if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_1)==RESET)
	{
		LL_mDelay(10);
		if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_1)==RESET)
		return 10;   //左
	}
	if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_2)==RESET)
	{
		LL_mDelay(10);
		if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_2)==RESET)
		return 11;   //右
	}
	if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_3)==RESET)
	{
		LL_mDelay(10);
		if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_3)==RESET)
		return 12;   //下
	}

	
}


/***************************************************************************************************************************************/
/***************************************************************************************************************************************/
/***********************************************        飞梭输入刷新处理        ********************************************************/
/***************************************************************************************************************************************/
/***************************************************************************************************************************************/



void TIM_Keyboard (void){
	
			if(TIM_flag==1){  //通道参数输入

		 tim_data[0] = Screen_data_now[Interface_Sec-35] + TIM3_CH1_dval;
		 tim_addr = tim_offset_init[Interface_Sec-35];
		 tim_cnt = 1;
		 HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (tim_addr<<1))),(uint16_t *)tim_data,tim_cnt);	//HAL库方式写入
		 tim_in[5] = tim_addr;
	 	 tim_in[6] = tim_data[0]>>8;
		 tim_in[7] = tim_data[0];
		 UART5_Send(tim_in,tim_length);
     Screen_data_now[Interface_Sec-35] = tim_data[0];
		 TIM3_CH1_dval=0;
		 //printf("tim_data = %d    tim_addr = %x ",tim_data[0],tim_addr);	
			 
		 }
		 
		 else if(TIM_flag==2){ //输出强度调节

		 tim_data[0] = Screen_data_now[Interface_third-37] + TIM3_CH1_dval;
		 tim_addr = tim_offset_init[Interface_third-37];
		 tim_cnt = 1;
		 HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (tim_addr<<1))),(uint16_t *)tim_data,tim_cnt);	//HAL库方式写入
		 tim_in[5] = tim_addr;
	 	 tim_in[6] = tim_data[0]>>8;
		 tim_in[7] = tim_data[0];
		 UART5_Send(tim_in,tim_length);
     Screen_data_now[Interface_third-37] = tim_data[0];  //-97+60
		 TIM3_CH1_dval=0;
		 //printf("tim_data = %d    tim_addr = %x ",tim_data[0],tim_addr);
			 
		 }
		 
		else if(Vaule_12_flag){ //十二通道联合写入

			 for(int i=0;i<12;i++){
			 tim_data[0] =  TIM3_CH1_dval;
			 tim_addr = tim_offset_init[i+60];
			 tim_cnt = 1;
			 tim_in[5] = tim_addr;
	 	   tim_in[6] = tim_data[0]>>8;
		   tim_in[7] = tim_data[0];
		   HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (tim_addr<<1))),(uint16_t *)tim_data,tim_cnt);	//HAL库方式写入
	     UART5_Send(tim_in,tim_length);
			 Screen_data_now[i+60] = tim_data[0];  //-97+60
			 //printf("tim_data = %d    tim_addr = %x ",tim_data[0],tim_addr);
        }			 
      }
	
	  else tim_data[0] = 0;
}



/***************************************************************************************************************************************/
/***************************************************************************************************************************************/
/***********************************************       键值 / 飞梭 处理         ********************************************************/
/***************************************************************************************************************************************/
/***************************************************************************************************************************************/

void Key_Scan (void)
{
  uint8_t Key = 0;
	
  if(CANCEL_flag==1){      //返回键值处理
		
		if(Interface_flag==3)  {
				Key_in[6] = Interface_Sec;
				UART5_Send(Key_in,Key_length);
			  TIM_flag = 0;
			}
				
		else if(Interface_flag==1)  {
				Key_in[6] = Interface_First;
				UART5_Send(Key_in,Key_length);
			  TIM_flag = 0;
			}
			else if(Interface_flag==0){
				Key_in[6] = 1;
				UART5_Send(Key_in,Key_length);
			}
			CANCEL_flag=0;
	}	
	
	Key = Read_Key();
	TIM_Keyboard();
	
	switch(Key)   //键值处理
	{
		case 0x05: // R_SW1 开始运行
		{
		 Screen_FPGA_offset=0xD0;		
		 Screen_data[0]=0xD1;
		 Screen_cnt=1;		
     Start_flag=1;	
		 HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
			
		}break;
		case 0x06: // R_SW2 暂停运行
		{
		 Screen_FPGA_offset=0xD0;	
		 Screen_data[0]=0xD3;
		 Screen_cnt=1;		
     Start_flag=0;			
		 HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入			
			
			
		}break;
		case 0x07: // R_SW3结束运行
		{
		 Screen_FPGA_offset=0xD0;	
		 Screen_data[0]=0xD0;
		 Screen_cnt=1;
     Start_flag=0;			
		 HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + ( Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入		
   	}break;		

		case 8:  // R_SW6 确定
		{
 	  Interface_flag = Interface_flag + 1;
		if(Interface_flag>5) Interface_flag = 5;
		LL_mDelay(150);
	
		if(Interface_flag==1)  { //菜单选择
			Interface_First = 23;
			Interface_Sec = 35;
			Interface_third = 97;
			Key_in[6] = Interface_First;
		  UART5_Send(Key_in,Key_length);
		}
		
		
		else if(Interface_flag==2){//菜单选择
			Key_in[6] = Interface_Sec;
			Key_addr = Interface_Sec_addr[Interface_Sec-35];
			UART5_Send(Key_in,Key_length);
			
		}
		else if(Interface_flag==3){ //飞梭调节参数
			  TIM_flag = 1;
		}
    

		
		else if(Interface_flag==4){//菜单选择
			  Key_in[6] = Interface_third;
				Key_addr = Interface_third_addr[Interface_third-97];
		    UART5_Send(Key_in,Key_length);

		}
				
		else if(Interface_flag==5){ //飞梭调节输出强度
			  TIM_flag = 2;
		}
				
		
		}break;
		
		
		case 0x09: // R_SW4 上
		{
			LL_mDelay(150);
			TIM_flag = 0;
			if(Interface_flag==1){
				Interface_First = Interface_First-6;
				Interface_Sec = Interface_Sec-30;
				if(Interface_First<23) Interface_First = Interface_First+6;
				if(Interface_Sec<35) Interface_Sec = Interface_Sec+30;
				Key_in[6] = Interface_First;
				UART5_Send(Key_in,Key_length);
			}
			
			else if(Interface_flag==2){
				Interface_Sec = Interface_Sec-2;
				if(Interface_Sec<35) Interface_Sec = Interface_Sec+2;
				Key_addr = Interface_Sec_addr[Interface_Sec-35];
				Key_in[6] = Interface_Sec;
				UART5_Send(Key_in,Key_length);
			}

			else if(Interface_flag==4){
				Interface_third = Interface_third-1;
				if(Interface_third<97) Interface_third = Interface_third+1;
				Key_addr = Interface_third_addr[Interface_third-97];
				Key_in[6] = Interface_third;
				UART5_Send(Key_in,Key_length);
			}
			
			
			
		}break;
		
		
		case 0x0A: // R_SW5 左
		{
			LL_mDelay(150);
			TIM_flag = 0;
			if(Interface_flag==1){
				Interface_First = Interface_First-1;
				Interface_Sec = Interface_Sec-5;
				if(Interface_First<23) Interface_First = Interface_First+1;
				if(Interface_Sec<35) Interface_Sec = Interface_Sec+5;
				Key_in[6] = Interface_First;
				UART5_Send(Key_in,Key_length);
			}
			
			else if(Interface_flag==2){
				Interface_Sec = Interface_Sec-1;
				if(Interface_Sec<35) Interface_Sec = Interface_Sec+1;
				Key_addr = Interface_Sec_addr[Interface_Sec-35];
				Key_in[6] = Interface_Sec;
				UART5_Send(Key_in,Key_length);
			}
			
  		else if(Interface_flag==4){
				Interface_third = Interface_third-4;
				if(Interface_third<97) Interface_third = Interface_third+4;
				Key_addr = Interface_third_addr[Interface_third-97];
				Key_in[6] = Interface_third;
				UART5_Send(Key_in,Key_length);
			}	
		
   	}break;		
		
		
		case 0x0B: // R_SW7 右
		{
			LL_mDelay(150);
			TIM_flag = 0;
 		 if(Interface_flag==1){
				Interface_First = Interface_First+1;
			  Interface_Sec = Interface_Sec+5;
				if(Interface_First>34) Interface_First = Interface_First-1;
			  if(Interface_Sec>90) Interface_Sec = Interface_Sec-5;
			  Key_in[6] = Interface_First;
				UART5_Send(Key_in,Key_length);
			}
			
			else if(Interface_flag==2){
				Interface_Sec = Interface_Sec+1;
				if(Interface_Sec>94) Interface_Sec = Interface_Sec-1;
				Key_addr = Interface_Sec_addr[Interface_Sec-35];
				Key_in[6] = Interface_Sec;
				UART5_Send(Key_in,Key_length);
			}	

		else if(Interface_flag==4){
				Interface_third = Interface_third+4;
				if(Interface_third>108) Interface_third = Interface_third-4;
				Key_addr = Interface_third_addr[Interface_third-97];
				Key_in[6] = Interface_third;
				UART5_Send(Key_in,Key_length);
			}	
						
		}break;
		
		
		case 0x0C: // R_SW9 下
		{
			LL_mDelay(150);
			TIM_flag = 0;
  	 if(Interface_flag==1){
				Interface_First = Interface_First+6;
			  Interface_Sec = Interface_Sec+30;
				if(Interface_First>34) Interface_First = Interface_First-6;
			  if(Interface_Sec>90) Interface_Sec = Interface_Sec-30;
			  Key_in[6] = Interface_First;
				UART5_Send(Key_in,Key_length);
			}
			
			else if(Interface_flag==2){
				Interface_Sec = Interface_Sec+2;
				if(Interface_Sec>94) Interface_Sec = Interface_Sec-2;
				Key_addr = Interface_Sec_addr[Interface_Sec-35];
				Key_in[6] = Interface_Sec;
				UART5_Send(Key_in,Key_length);
			}	

		 else if(Interface_flag==4){
				Interface_third = Interface_third+1;
				if(Interface_third>108) Interface_third = Interface_third-1;
				Key_addr = Interface_third_addr[Interface_third-97];
				Key_in[6] = Interface_third;
				UART5_Send(Key_in,Key_length);
			}	
						
   	}break;		
		
		default :	 break;
		
	}
		Key = 0;  
    Key_addr = 0;	
}


	
																		




/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
