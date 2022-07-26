#include "UART_menu.h"


uint8_t strRx[2]; // 一次性接收2字节数据，对应于16位宽寄存器
uint8_t cntRx = 0; // 接收计数
uint8_t state = 0; // 运行状态标志位：0- 开机，选择寄存器  1- 输入16位数据  2- 判断数据类型 3-
uint8_t reg_state = 0;  // 寄存器选择状态标志位： 0- 没有选择寄存器 1-选定了寄存器
uint16_t reg_num =0; // 选定的寄存器编号
uint16_t counter = 0;  // data_in的写入计数
uint8_t Rxdata_BUF[Rxdata_MAX_RECV_LEN]; 		//接收缓冲,最大UART4_MAX_RECV_LEN字节

	int i;
  uint16_t buffer_cnt;				//传输数据量
	unsigned int data_address;			//FPGA中数据区域起始地址:若设置前16个位置为寄存器，则data_address=16
	unsigned int FPGA_offset;				//FPGA RAM的地址偏移量
	uint16_t buffer[3000];	//输入数组
	
// 开机，提示选择寄存器操作
void OnePage()
{
  esp32_send_cmd("AT+CIPSEND=0,540","OK",50);	
	esp32_send_data("          Choose Which Register To Set          \r\n",10);//50
	esp32_send_data("/------ Total:     *01  |  Phase_1:  *02 ------/\r\n",10);//50
	esp32_send_data("/------ Phase_2:   *03  |  Phase_3:  *04 ------/\r\n",10);//50
	esp32_send_data("/------ Phase_4:   *05  |  Phase_5:  *06 ------/\r\n",10);//50
	esp32_send_data("/------ Phase_6:   *07  |  Phase_7:  *08 ------/\r\n",10);//50
	esp32_send_data("/------ Phase_8:   *09  |  Phase_9:  *11 ------/\r\n",10);//50
	esp32_send_data("/------ Phase_10:  *12  |  Phase_11: *13 ------/\r\n",10);//50
	esp32_send_data("/------ Phase_12:  *14  |  Ramp:     *15 ------/\r\n",10);//50
	esp32_send_data("/------ Freq:      *16  |  Normal:   *17 ------/\r\n",10);//50
	esp32_send_data("/------ Status:    *18  |  Mode:     *19 ------/\r\n",10);//50
	esp32_send_data("/------ Data_in:   *21  |                ------/\r\n",10);//50
	esp32_send_cmd("\r\n","SEND OK",10);
}

// 提示数据输入
void TwoPage()
{
	esp32_send_cmd("AT+CIPSEND=0,14","OK",50);	
	esp32_send_data("Load data now:",50);
}

// 提示数据重新输入
void ThreePage()
{
	while(esp32_send_cmd("AT+CIPSEND=0,23","OK",50));	
	esp32_send_data("Wrong Reg_num! Reload data now:",50);
}

// 提示数据重新输入
void FourPage()
{
	extern uint16_t counter;
	//Wrong! Reload data now:
	u3_printf(" Already %d data. Reload data now:\r\n",counter);
}

// 串口引导界面
void UART_menu(void)
{
	extern uint16_t counter;

	switch(state)
	{
		case 0: // 开机
		{
			OnePage();
			state++;
			TwoPage(); // 等待数据输入
		} break;

		case 1:
		{
			// 改，空等待
		} break;

		case 2:
		{
			// 处理数据
			if (strRx[0]=='*') // 数据为 寄存器选择择
			{
					reg_state = 1;
					reg_num = (short) (strRx[1]<<8 | (strRx[2])) ; 
				  printf("Reg_num is: %d\r\n",reg_num);
				  printf("reg_state id: %d\r\n",reg_state);
					state=1;
					TwoPage(); // 等待数据输入
			}
			else
			{
				state=1; // 无效数据
				ThreePage(); // 等待数据输入
			}

		} break;


		case 3:
		{
			// 给对应寄存器赋值
			switch(reg_num)
			{
				case '1':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ; //total
					 FPGA_offset=0;		//FPGA写入地址
					 buffer_cnt=1;								//写入数据个数
	  	     HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL库方式写入
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					 reg_state=0;
					
				}break;
				
				case '2':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ; //total
					 FPGA_offset=1;		//FPGA写入地址
					 buffer_cnt=1;								//写入数据个数
	  	     HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL库方式写入
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					 reg_state=0;
					
				}break;
				case '3':
				{
					buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//phase
					FPGA_offset=2;		//FPGA写入地址
					buffer_cnt=1;								//写入数据个数
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL库方式写入
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;
				case '4':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//ramp
					 FPGA_offset=3;		//FPGA写入地址
					 buffer_cnt=1;								//写入数据个数
	  	     HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL库方式写入
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					 reg_state=0;
				}break;
				case '5':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//freq
					FPGA_offset=4;		//FPGA写入地址
					buffer_cnt=1;								//写入数据个数
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL库方式写入
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;
				case '6':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//normal
					FPGA_offset=5;		//FPGA写入地址
					buffer_cnt=1;								//写入数据个数
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL库方式写入
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;
				case '7':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//status
					FPGA_offset=6;		//FPGA写入地址
					buffer_cnt=1;								//写入数据个数
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL库方式写入
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;
				case '8':
				{
					buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//mode
					FPGA_offset=7;		//FPGA写入地址
					buffer_cnt=1;								//写入数据个数
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL库方式写入
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;

				case '9':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ; //total
					 FPGA_offset=8;		//FPGA写入地址
					 buffer_cnt=1;								//写入数据个数
	  	     HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL库方式写入
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					 reg_state=0;
					
				}break;
				
				case '11':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ; //total
					 FPGA_offset=9;		//FPGA写入地址
					 buffer_cnt=1;								//写入数据个数
	  	     HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL库方式写入
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					 reg_state=0;
					
				}break;
				case '12':
				{
					buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//phase
					FPGA_offset=10;		//FPGA写入地址
					buffer_cnt=1;								//写入数据个数
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL库方式写入
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;
				case '13':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//ramp
					 FPGA_offset=11;		//FPGA写入地址
					 buffer_cnt=1;								//写入数据个数
	  	     HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL库方式写入
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					 reg_state=0;
				}break;
				case '14':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//freq
					FPGA_offset=12;		//FPGA写入地址
					buffer_cnt=1;								//写入数据个数
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL库方式写入
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;
				case '15':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//normal
					FPGA_offset=13;		//FPGA写入地址
					buffer_cnt=1;								//写入数据个数
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL库方式写入
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;
				case '16':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//status
					FPGA_offset=14;		//FPGA写入地址
					buffer_cnt=1;								//写入数据个数
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL库方式写入
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;
				case '17':
				{
					buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//mode
					FPGA_offset=15;		//FPGA写入地址
					buffer_cnt=1;								//写入数据个数
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL库方式写入
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;
								
				case '18':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//status
					FPGA_offset=16;		//FPGA写入地址
					buffer_cnt=1;								//写入数据个数
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL库方式写入
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;
				case '19':
				{
					buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//mode
					FPGA_offset=17;		//FPGA写入地址
					buffer_cnt=1;								//写入数据个数
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL库方式写入
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;
				
			 default:
			 {
				 	ThreePage(); // 等待数据输入
				  reg_state=0;
					state=1; // 无效数据
			 }
			 break;
			}
			state=0;
			reg_state=0;
		} break;

		case 4:
		{
				reg_state=0;
				state=0;
				FPGA_offset=18;		//FPGA写入地址
				buffer_cnt=1000;								//写入数据个数
	  	  HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL库方式写入
				while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
				esp32_send_data("FMC success!",50);

		}break;

		default:
		{
			reg_state=0;
			state=0;
			reg_num=0;
		}

		break;

}
}



