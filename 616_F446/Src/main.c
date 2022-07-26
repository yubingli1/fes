/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sys.h"
#include "delay.h"
#include "UART_menu.h"
#include "esp32.h"
#include "Screen.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	uint8_t Led_flag = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_FMC_Init();
  MX_USART1_UART_Init();
  MX_TIM7_Init();
  MX_CAN1_Init();
  MX_UART5_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();
  MX_TIM9_Init();
  MX_SPI1_Init();
  MX_UART4_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  delay_init(72);               	//��ʼ����ʱ����
	LL_USART_EnableIT_RXNE(USART1);   
	LL_USART_EnableIT_PE(USART1);  
	//LL_USART_EnableIT_RXNE(UART4);   
	//LL_USART_EnableIT_PE(UART4);
	LL_USART_EnableIT_RXNE(UART5);   
	LL_USART_EnableIT_PE(UART5);

	SCL_OUT();    //IIC��ʼ��ƽ����
	SDA_OUT();
	LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_6);
	LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_9);
	BQ_25792_init();   //��ع����ʼ��
	
	LL_GPIO_SetOutputPin(ESP32_nEN_GPIO_Port,ESP32_nEN_Pin);       //ESP32��λ
	LL_GPIO_SetOutputPin(FPGA_nRST_GPIO_Port,FPGA_nRST_Pin);       //FPGA��λ
	LL_GPIO_SetOutputPin(HV_Power_nEN_GPIO_Port,HV_Power_nEN_Pin); //Power��λ
	LL_GPIO_ResetOutputPin(GPIOE,FAN_EN_Pin);                      //���ȸ�λ
	LL_GPIO_SetOutputPin(GPIOE,FAN_EN_Pin);                        //��������
	
	LL_GPIO_SetOutputPin(GPIOC,LED_1_Pin); //���󣬹��ϣ�������ָʾ��   //��ʱLED1��LED2��ϣ�����LED2 �̵ƣ���־��Դ����
	LL_GPIO_ResetOutputPin(GPIOD,LED_2_Pin); //����������ָʾ
	LL_GPIO_SetOutputPin(GPIOD,LED_3_Pin);
	
	
	LL_GPIO_SetOutputPin(GPIOD,LED_4_Pin); //�������������У���������ʱ���ڵ���ɫ״̬����־FES���ڳ�ʼ��״̬
	LL_GPIO_SetOutputPin(GPIOD,LED_5_Pin);
	
	
	
	LL_GPIO_SetOutputPin(GPIOC,Key_C0_Pin); //����ɨ���ʼ��
	LL_GPIO_SetOutputPin(GPIOC,Key_C1_Pin);
	LL_GPIO_SetOutputPin(GPIOC,Key_C2_Pin);
	
	TIM3_init();       //����TIM3��ʼ��
	delay_ms(4000);
	Screen_init();     //�̼���������ʼ��
	


	uint8_t FES_out = 0;
	uint8_t FES_Cnt = 6;  //���ڼ��� ������ *6
	uint8_t FES_Flag = 0; //Ĭ�Ͽ�ʼ��Ϣ״̬
	uint8_t Relax_Flag = 1;
	extern  uint16_t sec_cnt;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)                                                           
  {
		//UART_menu();
		
//		if(sec_cnt==0){
//			if(Relax_Flag){  //��Ϣ��������ʼ��̼�
//				FES_Flag = 1;
//				Relax_Flag = 0;
//				sec_cnt = 480;
//				FES_Cnt--;
//				printf("FES_Cnt = %d",FES_Cnt);
//			}
//			else{ //��̼����� ��ʼ��Ϣ
//				FES_Flag = 0;
//				Relax_Flag = 1;
//				sec_cnt = 120;
//			}
//		}
		if(Start_flag){
			ADC_Value();
	    LL_GPIO_ResetOutputPin(GPIOC,LED_1_Pin); //���󣬹��ϣ�������ָʾ��  LED1 LED2ͬʱ�������Ƶƣ���־FES��ʼ���
	    LL_GPIO_ResetOutputPin(GPIOD,LED_2_Pin); //����������ָʾ
			LL_GPIO_SetOutputPin(GPIOD,LED_4_Pin); //�������������У������� ����LED5 ��ɫ�� ��־FES��ʼ���
			Led_flag = 0;
			}
		else {
			if(Led_flag==50){
				Led_flag = 0;
			  LED_4 = ~LED_4; //��˸̬ ��־FES׼������
		    LL_GPIO_SetOutputPin(GPIOC,LED_1_Pin); //���󣬹��ϣ�������ָʾ��  ������LED2 �̵� ��־FES����׼��״̬
      	LL_GPIO_ResetOutputPin(GPIOD,LED_2_Pin); //����������ָʾ
			}
			else Led_flag++;
		}
		
		Screen_fmc_fpga();	 //��Ļɨ��
		Screen_current_value();  //���ɨ��
		if(FPGA_RESET>500) {
			printf("FPGA_RESET = %d \r\n",FPGA_RESET);
			Screen_RESET_init();
			FPGA_RESET = 0;
		}
		
//    Key_Scan();  //����ɨ��
		

		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
	
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3);
  LL_PWR_DisableOverDriveMode();
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_16, 144, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_SetSystemCoreClock(72000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
  LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
