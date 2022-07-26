/**
  ******************************************************************************
  * File Name          : TIM.c
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

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */
uint32_t TIM3_CH1_dval = 0;

#include "gpio.h"
#include "Screen.h"
/* USER CODE END 0 */

/* TIM3 init function */
void MX_TIM3_Init(void)
{
  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  /**TIM3 GPIO Configuration
  PC6   ------> TIM3_CH1
  PC7   ------> TIM3_CH2
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* TIM3 interrupt Init */
  NVIC_SetPriority(TIM3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),2, 0));
  NVIC_EnableIRQ(TIM3_IRQn);

  TIM_InitStruct.Prescaler = 720;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 1000;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM3, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM3);
  LL_TIM_SetTriggerOutput(TIM3, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM3);
  LL_TIM_IC_SetActiveInput(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI);
  LL_TIM_IC_SetPrescaler(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_ICPSC_DIV1);
  LL_TIM_IC_SetFilter(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_IC_FILTER_FDIV1);
  LL_TIM_IC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING);
  LL_TIM_IC_SetActiveInput(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_ACTIVEINPUT_DIRECTTI);
  LL_TIM_IC_SetPrescaler(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_ICPSC_DIV1);
  LL_TIM_IC_SetFilter(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_IC_FILTER_FDIV1);
  LL_TIM_IC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_IC_POLARITY_RISING);

}
/* TIM6 init function */
void MX_TIM6_Init(void)
{
  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);

  /* TIM6 interrupt Init */
  NVIC_SetPriority(TIM6_DAC_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),4, 0));
  NVIC_EnableIRQ(TIM6_DAC_IRQn);

  TIM_InitStruct.Prescaler = 4999;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 7199;
  LL_TIM_Init(TIM6, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM6);
  LL_TIM_SetTriggerOutput(TIM6, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM6);

}
/* TIM7 init function */
void MX_TIM7_Init(void)
{
  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM7);

  /* TIM7 interrupt Init */
  NVIC_SetPriority(TIM7_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),3, 0));
  NVIC_EnableIRQ(TIM7_IRQn);

  TIM_InitStruct.Prescaler = 720-1;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	//TIM_InitStruct.Autoreload = 1000; //定时10ms
  TIM_InitStruct.Autoreload = 50000-1; //定时500ms
  LL_TIM_Init(TIM7, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM7);
  LL_TIM_SetTriggerOutput(TIM7, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM7);

}
/* TIM9 init function */
void MX_TIM9_Init(void)
{
  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM9);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
  /**TIM9 GPIO Configuration
  PE5   ------> TIM9_CH1
  PE6   ------> TIM9_CH2
  */
  GPIO_InitStruct.Pin = TIM9_CH1_Pin|LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_3;
  LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* TIM9 interrupt Init */
  NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),6, 0));
  NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);

  TIM_InitStruct.Prescaler = 720;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 1000;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM9, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM9);
  LL_TIM_IC_SetActiveInput(TIM9, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI);
  LL_TIM_IC_SetPrescaler(TIM9, LL_TIM_CHANNEL_CH1, LL_TIM_ICPSC_DIV1);
  LL_TIM_IC_SetFilter(TIM9, LL_TIM_CHANNEL_CH1, LL_TIM_IC_FILTER_FDIV1);
  LL_TIM_IC_SetPolarity(TIM9, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING);
  LL_TIM_IC_SetActiveInput(TIM9, LL_TIM_CHANNEL_CH2, LL_TIM_ACTIVEINPUT_DIRECTTI);
  LL_TIM_IC_SetPrescaler(TIM9, LL_TIM_CHANNEL_CH2, LL_TIM_ICPSC_DIV1);
  LL_TIM_IC_SetFilter(TIM9, LL_TIM_CHANNEL_CH2, LL_TIM_IC_FILTER_FDIV1);
  LL_TIM_IC_SetPolarity(TIM9, LL_TIM_CHANNEL_CH2, LL_TIM_IC_POLARITY_RISING);

}

/* USER CODE BEGIN 1 */

void TIM3_init(void){
	
  LL_TIM_ClearFlag_CC1(TIM3);
	LL_TIM_EnableIT_CC1(TIM3); 
	LL_TIM_CC_EnableChannel(TIM3,LL_TIM_CHANNEL_CH1); 
	LL_TIM_ClearFlag_CC2(TIM3);
	LL_TIM_EnableIT_CC2(TIM3);	
	LL_TIM_CC_EnableChannel(TIM3,LL_TIM_CHANNEL_CH2);
	LL_TIM_EnableCounter(TIM3);
	LL_TIM_SetCounter(TIM3,0);
	//LL_GPIO_AF_EnableRemap_TIM3();
	
}

	
																						
void TIM3_Input_capture(void) //上升沿捕捉
{
	 if(LL_TIM_IsActiveFlag_CC1(TIM3) == SET){
		 if((TIM_flag!=0) || (Vaule_12_flag==1))  {
			 			if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_7)== RESET)
						TIM3_CH1_dval++;
			 else
						TIM3_CH1_dval--;
		 }
		else TIM3_CH1_dval=0;
	 }
	  
	 LL_TIM_ClearFlag_CC1(TIM3);
	 LL_TIM_ClearFlag_CC2(TIM3);
}


extern vu16 UART5_RX_STA;
extern uint16_t Screen_RX_STA;

void TIM6_IRQ(void)   //uart5
{
 if (LL_TIM_IsActiveFlag_UPDATE(TIM6) == SET)//是更新中断
	{	 			   
		UART5_RX_STA|=1<<15;	//标记接收完成
		Screen_RX_STA|=1<<15;	//标记接收完成
		LL_TIM_ClearFlag_UPDATE(TIM6);  //清除TIM6更新中断标志    
		LL_TIM_DisableCounter(TIM6);  //关闭TIM6 
	}
	
	
}



extern vu16 UART4_RX_STA;
extern uint16_t DATA_RX_STA;

//void TIM7_IRQ(void)  // ESP32 500ms
//{
// if (LL_TIM_IsActiveFlag_UPDATE(TIM7) == SET)//是更新中断
//	{	 			   
////		UART4_RX_STA|=1<<15;	//标记接收完成
////		DATA_RX_STA|=1<<15;	//标记接收完成
//		//LL_TIM_ClearFlag_UPDATE(TIM7);  //清除TIM7更新中断标志    
//		//LL_TIM_DisableCounter(TIM7);  //关闭TIM7 
//	}

//}

uint16_t sec_cnt = 1;

void TIM7_IRQ(void)  // 定时 500ms
{
 if (LL_TIM_IsActiveFlag_UPDATE(TIM7) == SET)//是更新中断
	{	 			   
		sec_cnt--;
		if(sec_cnt<0) sec_cnt=0;
		
		LL_TIM_ClearFlag_UPDATE(TIM7);  //清除TIM7更新中断标志    
		
	}

}


void TIM9_init(void){
	
  LL_TIM_ClearFlag_CC1(TIM9);
	LL_TIM_EnableIT_CC1(TIM9); 
	LL_TIM_CC_EnableChannel(TIM9,LL_TIM_CHANNEL_CH1); 
	LL_TIM_EnableCounter(TIM9);
	LL_TIM_SetCounter(TIM9,0);
	//LL_GPIO_AF_EnableRemap_TIM3();
	
}
	
void TIM9_Input_capture(void) 
{

	if(LL_TIM_IsActiveFlag_CC1(TIM9) == SET)
	{
		  LL_TIM_ClearFlag_CC1(TIM9);
			//TIM9_CH1_dval++;
		}

}





/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
