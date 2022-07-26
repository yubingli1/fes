#include "Screen.h"
#include "string.h"
#include "usart.h"
#include "sys.h" 
#include "main.h"
#include "tim.h"
#include "fmc.h"
#include "adc.h"
#include "stm32f4xx_hal.h"
//触摸屏接收数据并通过FMC发送至FPGA



/***********************************************************屏幕初始化，写入默认设定参数***********************************************************/

	 uint16_t Screen_addr = 0;
   uint16_t Screen_data[3000];
   uint8_t  Screen_cnt;
	 uint8_t  uart5_in[8] = {0x5A,0XA5,0X05,0X82,0X00,0,0,0};
   uint8_t  length = 8;
	 unsigned short int data[60];
	 unsigned short int FPGA_data[1];
   
	 
	 unsigned int Screen_FPGA_offset;				//FPGA RAM的地址偏移量
	 unsigned int Read_FPGA_offset;				//FPGA RAM的地址偏移量
   unsigned int  Read_cnt;
	 
   uint16_t Screen_data_init[84]={50,50,50,50,50,50,50,50,50,50,50,50,
	                                0,0,0,0,0,0,0,0,0,0,0,0,
	                                3000,3000,3000,3000,3000,3000,3000,3000,3000,3000,3000,3000,
                                  1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
																  500,500,500,500,500,500,500,500,500,500,500,500,
		                              0,0,0,0,0,0,0,0,0,0,0,0, 
		                              0,0,0,0,0,0,0,0,0,0,0,0,  
	                                };
	unsigned int Screen_FPGA_offset_init[84]={0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80,0x90,0xA0,0xB0,0xC0,
	                                          0x11,0x21,0x31,0x41,0x51,0x61,0x71,0x81,0x91,0xA1,0xB1,0xC1,
		                                        0x12,0x22,0x32,0x42,0x52,0x62,0x72,0x82,0x92,0xA2,0xB2,0xC2,
		                                        0x13,0x23,0x33,0x43,0x53,0x63,0x73,0x83,0x93,0xA3,0xB3,0xC3,
		                                        0x14,0x24,0x34,0x44,0x54,0x64,0x74,0x84,0x94,0xA4,0xB4,0xC4,
		                                        0x15,0x25,0x35,0x45,0x55,0x65,0x75,0x85,0x95,0xA5,0xB5,0xC5,
	                                          0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC};

	uint16_t Screen_data_now[99]={50,0,3000,1000,500, 50,0,3000,1000,500,
                                50,0,3000,1000,500, 50,0,3000,1000,500,
                                50,0,3000,1000,500, 50,0,3000,1000,500,
                                50,0,3000,1000,500, 50,0,3000,1000,500,
                                50,0,3000,1000,500, 50,0,3000,1000,500,
                                50,0,3000,1000,500, 50,0,3000,1000,500,
                                0,0,0,0,0,0,0,0,0,0,0,0,
															  0,0,0,0,0,0,0,0,0,0,0,0,
		                            0,0,0,0,0,0,0,0,0,0,0,0,
															  0,0,0};

 unsigned int Screen_FPGA_now_offset_init[99]={0x10,0x11,0x12,0x13,0x14,0x20,0x21,0x22,0x23,0x24,
	                                             0x30,0x31,0x32,0x33,0x34,0x40,0x41,0x42,0x43,0x44,
		                                           0x50,0x51,0x52,0x53,0x54,0x60,0x61,0x62,0x63,0x64,
		                                           0x70,0x71,0x72,0x73,0x74,0x80,0x81,0x82,0x83,0x84,
		                                           0x90,0x91,0x92,0x93,0x94,0xA0,0xA1,0xA2,0xA3,0xA4,
		                                           0xB0,0xB1,0xB2,0xB3,0xB4,0xC0,0xC1,0xC2,0xC3,0xC4,
	                                             0x15,0x25,0x35,0x45,0x55,0x65,0x75,0x85,0x95,0xA5,0xB5,0xC5,  //输出强度
	                                             0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,  //通道开启情况
	                                             0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,
                                               0xD0,0xD1,0xD2};//D0开始/暂停  D1波形数据,脉宽  D2脉宽
		
void Screen_init(void)
{ 
	Screen_cnt = 1;
  for(int i=0;i<84;i++){
		
		Screen_FPGA_offset = Screen_FPGA_offset_init[i];
		data[0] = Screen_data_init[i];
		uart5_in[5] = Screen_FPGA_offset;
		uart5_in[6] = data[0]>>8;
		uart5_in[7] = data[0];
	  HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)data,Screen_cnt);	//HAL库方式读取
		UART5_Send(uart5_in,length);
  }
	
}




void Screen_RESET_init(void)
{ 
  for(int i=0;i<96;i++){
		
		Screen_FPGA_offset = Screen_FPGA_now_offset_init[i];
		data[0] = Screen_data_now[i];
		uart5_in[5] = Screen_FPGA_offset;
		uart5_in[6] = data[0]>>8;
		uart5_in[7] = data[0];
	  HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)data,Screen_cnt);	//HAL库方式读取
		UART5_Send(uart5_in,length);
		printf("Screen_data_now[%d] = %x  \r\n",i,Screen_data_now[i]);
  }
	  for(int i=98;i>95;i--){
		
		Screen_FPGA_offset = Screen_FPGA_now_offset_init[i];
		data[0] = Screen_data_now[i];
		uart5_in[5] = Screen_FPGA_offset;
		uart5_in[6] = data[0]>>8;
		uart5_in[7] = data[0];
	  HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)data,Screen_cnt);	//HAL库方式读取
		UART5_Send(uart5_in,length);
		printf("Screen_data_now[%d] = %x  \r\n",i,Screen_data_now[i]);
  }
}



/**********************************************************阻抗检测***********************************************************/

uint8_t Fpga_data_cnt = 38;
uint32_t FPGA_RESET = 0;
unsigned int FPGA_Screen_offset_init[24]={0x16,0x26,0x36,0x46,0x56,0x66,0x76,0x86,0x96,0xA6,0xB6,0xC6,
																				  0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC};
unsigned  short int FPGA_Screen[12];
long int Calculate_next;	
unsigned  long  int Calculate_data[12];															
unsigned  short int Read_FPGA_data[38];
unsigned int ADC_CHn_FLAG;
																					
void Screen_current_value(void){
	
	  int k=0;
	  int Screen_update = 0;
		Read_cnt = 1;
	  FPGA_data[0] = 0;
	  Read_FPGA_offset = 0;
	  
    //读取28个数据，进行除法运算并发送至上位机，CHn_wave  /  CHn_wave_out ,ADC通道切换
		//HAL_SRAM_Read_16b(&hsram1,((uint32_t *)(0x60000000 + (Read_FPGA_offset<<1))),(uint16_t *) Read_FPGA_data,Read_cnt);

		for(int j=0;j<Fpga_data_cnt;j++)
		{
		
			HAL_SRAM_Read_16b(&hsram1,((uint32_t *)(0x60000000 + (Read_FPGA_offset<<1))),(uint16_t *)FPGA_data,Read_cnt);		
			Read_FPGA_data[j] = FPGA_data[0];
			Read_FPGA_offset++;
			//printf("Read_FPGA_data[%d] = %x \r\n",j,FPGA_data[0]);
		}		
	 for(int j=0;j<Fpga_data_cnt;j++)
		{
			if(((j%3) ==1) && (j<37)){
			Calculate_next = (Read_FPGA_data[j+1]<<16) + (Read_FPGA_data[j]);
			FPGA_Screen[k] = (uint16_t)((uint32_t)Read_FPGA_data[j-1] *100 / Calculate_next);
			k++;
		  Calculate_next = 0;
			//printf("Calculate_next = %lx \r\n",Calculate_next);
			}
	   
		}
		
		
	  ADC_CHn_FLAG = Read_FPGA_data[37];
		
		 
		if(Read_FPGA_data[36] == 12){
				Screen_update = 1;
			  FPGA_RESET=0;
			}
			else if(Start_flag==1 && Read_FPGA_data[36] == 11){
				Screen_update = 0;
				FPGA_RESET++;
			}
		else Screen_update = 0;
			
  	if(Screen_update){
			for(int L=0;L<12;L++){
				uart5_in[5] = FPGA_Screen_offset_init[L];
				uart5_in[6] = FPGA_Screen[L]>>8;
				uart5_in[7] = FPGA_Screen[L];
				UART5_Send(uart5_in,length);
			  
			}
			for(int M=0;M<12;M++){
				uart5_in[5] = FPGA_Screen_offset_init[M+12];
				uart5_in[6] = 0;
				uart5_in[7] = CHn_flag[M];
				UART5_Send(uart5_in,length);
				//printf("CHn_flag = %x \r\n",CHn_flag[M]);
			}
		}

		for(int m=0;m<Fpga_data_cnt;m++){
			FPGA_Screen[m] = 0;
			Read_FPGA_data[m] = 0;
		}
		
}






/***********************************************************接收屏幕数据并发送至上位机 / 下位机***********************************************************/
/*                                                 */
/*						00 01 -> 00 0C 各通道开关配置        */
/*						00 21 -> 00 2C 三色灯                */
/*						00 D0          开始 / 暂停           */
/*						00 D1          波形数据,脉宽         */
/*						00 D2          脉宽                  */
/*						00 10 -> 00 C0 总周期   Total        */
/*						00 11 -> 00 C1 相位延迟 Phase        */
/*						00 12 -> 00 C2 正常输出 Normal       */
/*						00 13 -> 00 C3 斜升斜降 Ramp         */
/*						00 14 -> 00 C4 刺激频率 Freq         */
/*						00 15 -> 00 C5 电流强度 Ratio        */
/*								                                 */

uint8_t  Start_flag=0; //开始运行标志位
uint8_t  Vaule_12_flag=0;
uint8_t  CHn_flag[12];
uint8_t  Fes_Start = 1;

void Screen_fmc_fpga(void)
{
	
		if(Screen_RX_STA&0X8000)		//接收到一次数据了
		{ 	

				if((Screen_RX_BUF[0]==0x5A) && (Screen_RX_BUF[1]==0xA5)){
						Screen_addr = (short) (Screen_RX_BUF[5] | (Screen_RX_BUF[4])<<8) ;  //5A A5 06 83 00 10 01 04 D2
						Screen_data[0] = (short) (Screen_RX_BUF[8] | (Screen_RX_BUF[7])<<8) ;  // 00 10为寄存器地址  04 D2为数据
						}
		
		 // 给对应寄存器赋值
		
		switch(Screen_addr)
			{
/*						00 01 -> 00 0C 各通道开关配置        */
				case  0x01 :
				{
					 CHn_flag[0] = Screen_RX_BUF[8];
					 Screen_FPGA_offset=0x01;		//FPGA写入地址
					 Screen_cnt=1;								//写入数据个数
					 Screen_data_now[72] = Screen_data[0];
	  	     HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
					// printf("CHn_flag[0] = %x",CHn_flag[0]);
				}break;
				
				case  0x02 :
				{
					 CHn_flag[1] = Screen_RX_BUF[8];
					 Screen_FPGA_offset=0x02;		//FPGA写入地址
					 Screen_cnt=1;								//写入数据个数
					 Screen_data_now[73] = Screen_data[0];
	  	     HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
					
				}break;
				case  0x03 :
				{
					CHn_flag[2] = Screen_RX_BUF[8];
					Screen_FPGA_offset=0x03;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
				  Screen_data_now[74] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				case  0x04 :
				{
					CHn_flag[3] = Screen_RX_BUF[8];
					 Screen_FPGA_offset=0x04;		//FPGA写入地址
					 Screen_cnt=1;								//写入数据个数
					Screen_data_now[75] = Screen_data[0];
	  	     HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				
				case  0x05 :
				{
					CHn_flag[4] = Screen_RX_BUF[8];
					Screen_FPGA_offset=0x05;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[76] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				
				case  0x06 :
				{
					CHn_flag[5] = Screen_RX_BUF[8];
					Screen_FPGA_offset=0x06;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[77] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				
				case  0x07 :
				{
					CHn_flag[6] = Screen_RX_BUF[8];
					Screen_FPGA_offset=0x07;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[78] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入0;
				}break;
				
				case  0x08 :
				{
					CHn_flag[7] = Screen_RX_BUF[8];
					Screen_FPGA_offset=0x08;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[79] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0x09 :
				{
					 CHn_flag[8] = Screen_RX_BUF[8];
					 Screen_FPGA_offset=0x09;		//FPGA写入地址
					 Screen_cnt=1;								//写入数据个数
					Screen_data_now[80] = Screen_data[0];
	  	     HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
					
				}break;
				
				case  0x0A :
				{
					CHn_flag[9] = Screen_RX_BUF[8];
					 Screen_FPGA_offset=0X0A;		//FPGA写入地址
					 Screen_cnt=1;								//写入数据个数
					Screen_data_now[81] = Screen_data[0];
	  	     HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				
				}break;
				
				case  0x0B :
				{
					CHn_flag[10] = Screen_RX_BUF[8];
					 Screen_FPGA_offset=0x0B;		//FPGA写入地址
					 Screen_cnt=1;								//写入数据个数
					Screen_data_now[82] = Screen_data[0];
	  	     HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
					
				}break;
				
				case  0x0C :
				{
					CHn_flag[11] = Screen_RX_BUF[8];
					Screen_FPGA_offset=0x0C;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[83] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

/*00 D0开始 / 暂停*/
	
				case 0xd0:
				{
					 Screen_FPGA_offset=0xD0;		//FPGA写入地址
					 Screen_cnt=1;								//写入数据个数
					Screen_data_now[96] = Screen_data[0];
	  	     HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
					
				}break;

/*00 D1 波形数据*/

				case 0xd1: 
				{
					Screen_FPGA_offset=0xD1;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[97] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				
/*00 D2脉宽*/				
			  case 0xd2: 
				{
					Screen_FPGA_offset=0xD2;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[98] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;	
				
				
				
				case 0xf0:    //12通道输出电流强度联调
				{
					if(Screen_data[0]) Vaule_12_flag = 1;
					else Vaule_12_flag = 0;
					
					//printf("Vaule_12_flag = %d" ,Vaule_12_flag);
				}break;
				

/*						00 10 -> 00 C0 总周期   Total        */
/*						00 11 -> 00 C1 相位延迟 Phase        */
/*						00 12 -> 00 C2 正常输出 Normal       */
/*						00 13 -> 00 C3 斜升斜降 Ramp         */
/*						00 14 -> 00 C4 刺激频率 Freq         */
/*						00 15 -> 00 C5 电流强度 Ratio        */		
				
				case  0X10 :
				{
					Screen_FPGA_offset=0x10;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[0] = Screen_data[0];
					 //printf("Screen_data_now = %x",Screen_data_now[1]);
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入

				}break;
				
				case  0X11 :
				{
					Screen_FPGA_offset=0x11;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[1] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入

				}break;

				case  0X12 :
				{
					Screen_FPGA_offset=0x12;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[2] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0X13 :
				{
					Screen_FPGA_offset=0x13;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[3] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0X14 :
				{
					Screen_FPGA_offset=0x14;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[4] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				case  0X15 :
				{
					Screen_FPGA_offset=0x15;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[60] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;				
				
				
				case  0X20 :
				{
					Screen_FPGA_offset=0x20;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[5] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				case  0X21 :
				{
					Screen_FPGA_offset=0x21;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[6] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
								
				case  0X22 :
				{
					Screen_FPGA_offset=0x22;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[7] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				
				case  0X23 :
				{
					Screen_FPGA_offset=0x23;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[8] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				
				case  0X24 :
				{
					Screen_FPGA_offset=0x24;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[9] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				case  0X25 :
				{
					Screen_FPGA_offset=0x25;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[61] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;					
				
				
				case  0X30 :
				{
					Screen_FPGA_offset=0x30;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[10] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0X31 :
				{
					Screen_FPGA_offset=0x31;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[11] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0X32 :
				{
					Screen_FPGA_offset=0x32;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[12] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0X33 :
				{
					Screen_FPGA_offset=0x33;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[13] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0X34 :
				{
					Screen_FPGA_offset=0x34;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[14] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				case  0X35 :
				{
					Screen_FPGA_offset=0x35;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[62] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;	
				
				
				case  0X40 :
				{
					Screen_FPGA_offset=0x40;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[15] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				
				case  0X41 :
				{
					Screen_FPGA_offset=0x41;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[16] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				case  0X42 :
				{
					Screen_FPGA_offset=0x42;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[17] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
								
				case  0X43 :
				{
					Screen_FPGA_offset=0x43;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[18] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				
				case  0X44 :
				{
					Screen_FPGA_offset=0x44;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[19] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				case  0X45 :
				{
					Screen_FPGA_offset=0x45;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[63] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;					
				
				
				case  0X50 :
				{
					Screen_FPGA_offset=0x50;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[20] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				
				case  0X51 :
				{
					Screen_FPGA_offset=0x51;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[21] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0X52 :
				{
					Screen_FPGA_offset=0x52;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[22] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0X53 :
				{
					Screen_FPGA_offset=0x53;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[23] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0X54 :
				{
					Screen_FPGA_offset=0x54;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[24] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				case  0X55 :
				{
					Screen_FPGA_offset=0x55;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[64] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;	
				
				
				case  0X60 :
				{
					Screen_FPGA_offset=0x60;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[25] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0X61 :
				{
					Screen_FPGA_offset=0x61;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[26] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0X62 :
				{
					Screen_FPGA_offset=0x62;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[27] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0X63 :
				{
					Screen_FPGA_offset=0x63;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[28] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0X64 :
				{
					Screen_FPGA_offset=0x64;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[29] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0X65 :
				{
					Screen_FPGA_offset=0x65;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[65] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;	
				
				case  0X70 :
				{
					Screen_FPGA_offset=0x70;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[30] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				
				case  0X71 :
				{
					Screen_FPGA_offset=0x71;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[31] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0X72 :
				{
					Screen_FPGA_offset=0x72;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[32] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0X73 :
				{
					Screen_FPGA_offset=0x73;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[33] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0X74 :
				{
					Screen_FPGA_offset=0x74;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[34] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				case  0X75 :
				{
					Screen_FPGA_offset=0x75;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[66] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;					
				
				
				case  0X80 :
				{
					Screen_FPGA_offset=0x80;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[35] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				case  0X81 :
				{
					Screen_FPGA_offset=0x81;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[36] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
								
				case  0X82 :
				{
					Screen_FPGA_offset=0x82;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[37] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				
				case  0X83 :
				{
					Screen_FPGA_offset=0x83;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[38] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				
				case  0X84 :
				{
					Screen_FPGA_offset=0x84;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[39] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				case  0X85 :
				{
					Screen_FPGA_offset=0x85;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[67] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;					
				
				
				case  0X90 :
				{
					Screen_FPGA_offset=0x90;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[40] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0X91 :
				{
					Screen_FPGA_offset=0x91;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[41] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0X92 :
				{
					Screen_FPGA_offset=0x92;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[42] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0X93 :
				{
					Screen_FPGA_offset=0x93;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[43] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0X94 :
				{
					Screen_FPGA_offset=0x94;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[44] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				case  0X95 :
				{
					Screen_FPGA_offset=0x95;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[68] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;	
				
				
				case  0XA0 :
				{
					Screen_FPGA_offset=0xA0;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[45] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				
				case  0XA1 :
				{
					Screen_FPGA_offset=0xA1;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[46] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				case  0XA2 :
				{
					Screen_FPGA_offset=0xA2;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[47] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
								
				case  0XA3 :
				{
					Screen_FPGA_offset=0xA3;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[48] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				
				case  0XA4 :
				{
					Screen_FPGA_offset=0xA4;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[49] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				case  0XA5 :
				{
					Screen_FPGA_offset=0xA5;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[69] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;					
				
				
				case  0XB0 :
				{
					Screen_FPGA_offset=0xB0;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[50] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				
				case  0XB1 :
				{
					Screen_FPGA_offset=0xB1;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[51] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0XB2 :
				{
					Screen_FPGA_offset=0xB2;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[52] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0XB3 :
				{
					Screen_FPGA_offset=0xB3;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[53] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0XB4 :
				{
					Screen_FPGA_offset=0xB4;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[54] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				case  0XB5 :
				{
					Screen_FPGA_offset=0xB5;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[70] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;	
				
				
				case  0XC0 :
				{
					Screen_FPGA_offset=0xC0;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[55] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case  0XC1 :
				{
					Screen_FPGA_offset=0xC1;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[56] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case 0XC2:
				{
					Screen_FPGA_offset=0xC2;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[57] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case 0XC3:
				{
					Screen_FPGA_offset=0xC3;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[58] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;

				case 0XC4:
				{
					Screen_FPGA_offset=0xC4;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[59] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;
				case  0XC5 :
				{
					Screen_FPGA_offset=0xC5;		//FPGA写入地址
					Screen_cnt=1;								//写入数据个数
					Screen_data_now[71] = Screen_data[0];
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (Screen_FPGA_offset<<1))),(uint16_t *)Screen_data,Screen_cnt);	//HAL库方式写入
				}break;					
        //default : break;

			}
			 
		if(Screen_addr==0xd0) {
				if(Screen_data[0]==0xd1) {
						Start_flag =1;
						if(!Fes_Start) Fes_Start=1;		
						if(Fes_Start)
							{
								LL_TIM_EnableIT_UPDATE(TIM7);
								LL_TIM_EnableCounter(TIM7);// 使能定时器7
								Fes_Start=2;
							}
						}
				else if(Screen_data[0]==0xd0) {
						Fes_Start=0;
			      LL_TIM_DisableCounter(TIM7);
						Start_flag =0;
				    ADC_Value(); 
					}
				}
				
		UART5_cnt = 0;
		Screen_addr = 0;
		for(int i=0; i<600;i++){
				UART5_RX_BUF[i] = 0;
				Screen_RX_BUF[i] = 0;
				Screen_data[i] = 0;
			}				

			
        UART5_RX_STA=0;//标志位清零
				Screen_RX_STA =0;//标志位清零 
			
			
	}		
}




