#include "UART_menu.h"


uint8_t strRx[2]; // һ���Խ���2�ֽ����ݣ���Ӧ��16λ��Ĵ���
uint8_t cntRx = 0; // ���ռ���
uint8_t state = 0; // ����״̬��־λ��0- ������ѡ��Ĵ���  1- ����16λ����  2- �ж��������� 3-
uint8_t reg_state = 0;  // �Ĵ���ѡ��״̬��־λ�� 0- û��ѡ��Ĵ��� 1-ѡ���˼Ĵ���
uint16_t reg_num =0; // ѡ���ļĴ������
uint16_t counter = 0;  // data_in��д�����
uint8_t Rxdata_BUF[Rxdata_MAX_RECV_LEN]; 		//���ջ���,���UART4_MAX_RECV_LEN�ֽ�

	int i;
  uint16_t buffer_cnt;				//����������
	unsigned int data_address;			//FPGA������������ʼ��ַ:������ǰ16��λ��Ϊ�Ĵ�������data_address=16
	unsigned int FPGA_offset;				//FPGA RAM�ĵ�ַƫ����
	uint16_t buffer[3000];	//��������
	
// ��������ʾѡ��Ĵ�������
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

// ��ʾ��������
void TwoPage()
{
	esp32_send_cmd("AT+CIPSEND=0,14","OK",50);	
	esp32_send_data("Load data now:",50);
}

// ��ʾ������������
void ThreePage()
{
	while(esp32_send_cmd("AT+CIPSEND=0,23","OK",50));	
	esp32_send_data("Wrong Reg_num! Reload data now:",50);
}

// ��ʾ������������
void FourPage()
{
	extern uint16_t counter;
	//Wrong! Reload data now:
	u3_printf(" Already %d data. Reload data now:\r\n",counter);
}

// ������������
void UART_menu(void)
{
	extern uint16_t counter;

	switch(state)
	{
		case 0: // ����
		{
			OnePage();
			state++;
			TwoPage(); // �ȴ���������
		} break;

		case 1:
		{
			// �ģ��յȴ�
		} break;

		case 2:
		{
			// ��������
			if (strRx[0]=='*') // ����Ϊ �Ĵ���ѡ����
			{
					reg_state = 1;
					reg_num = (short) (strRx[1]<<8 | (strRx[2])) ; 
				  printf("Reg_num is: %d\r\n",reg_num);
				  printf("reg_state id: %d\r\n",reg_state);
					state=1;
					TwoPage(); // �ȴ���������
			}
			else
			{
				state=1; // ��Ч����
				ThreePage(); // �ȴ���������
			}

		} break;


		case 3:
		{
			// ����Ӧ�Ĵ�����ֵ
			switch(reg_num)
			{
				case '1':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ; //total
					 FPGA_offset=0;		//FPGAд���ַ
					 buffer_cnt=1;								//д�����ݸ���
	  	     HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL�ⷽʽд��
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					 reg_state=0;
					
				}break;
				
				case '2':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ; //total
					 FPGA_offset=1;		//FPGAд���ַ
					 buffer_cnt=1;								//д�����ݸ���
	  	     HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL�ⷽʽд��
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					 reg_state=0;
					
				}break;
				case '3':
				{
					buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//phase
					FPGA_offset=2;		//FPGAд���ַ
					buffer_cnt=1;								//д�����ݸ���
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL�ⷽʽд��
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;
				case '4':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//ramp
					 FPGA_offset=3;		//FPGAд���ַ
					 buffer_cnt=1;								//д�����ݸ���
	  	     HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL�ⷽʽд��
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					 reg_state=0;
				}break;
				case '5':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//freq
					FPGA_offset=4;		//FPGAд���ַ
					buffer_cnt=1;								//д�����ݸ���
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL�ⷽʽд��
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;
				case '6':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//normal
					FPGA_offset=5;		//FPGAд���ַ
					buffer_cnt=1;								//д�����ݸ���
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL�ⷽʽд��
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;
				case '7':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//status
					FPGA_offset=6;		//FPGAд���ַ
					buffer_cnt=1;								//д�����ݸ���
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL�ⷽʽд��
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;
				case '8':
				{
					buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//mode
					FPGA_offset=7;		//FPGAд���ַ
					buffer_cnt=1;								//д�����ݸ���
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL�ⷽʽд��
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;

				case '9':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ; //total
					 FPGA_offset=8;		//FPGAд���ַ
					 buffer_cnt=1;								//д�����ݸ���
	  	     HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL�ⷽʽд��
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					 reg_state=0;
					
				}break;
				
				case '11':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ; //total
					 FPGA_offset=9;		//FPGAд���ַ
					 buffer_cnt=1;								//д�����ݸ���
	  	     HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL�ⷽʽд��
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					 reg_state=0;
					
				}break;
				case '12':
				{
					buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//phase
					FPGA_offset=10;		//FPGAд���ַ
					buffer_cnt=1;								//д�����ݸ���
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL�ⷽʽд��
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;
				case '13':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//ramp
					 FPGA_offset=11;		//FPGAд���ַ
					 buffer_cnt=1;								//д�����ݸ���
	  	     HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL�ⷽʽд��
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					 reg_state=0;
				}break;
				case '14':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//freq
					FPGA_offset=12;		//FPGAд���ַ
					buffer_cnt=1;								//д�����ݸ���
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL�ⷽʽд��
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;
				case '15':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//normal
					FPGA_offset=13;		//FPGAд���ַ
					buffer_cnt=1;								//д�����ݸ���
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL�ⷽʽд��
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;
				case '16':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//status
					FPGA_offset=14;		//FPGAд���ַ
					buffer_cnt=1;								//д�����ݸ���
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL�ⷽʽд��
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;
				case '17':
				{
					buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//mode
					FPGA_offset=15;		//FPGAд���ַ
					buffer_cnt=1;								//д�����ݸ���
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL�ⷽʽд��
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;
								
				case '18':
				{
					 buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//status
					FPGA_offset=16;		//FPGAд���ַ
					buffer_cnt=1;								//д�����ݸ���
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL�ⷽʽд��
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;
				case '19':
				{
					buffer[0] = (short) (strRx[0]<<8 | (strRx[1])) ;//mode
					FPGA_offset=17;		//FPGAд���ַ
					buffer_cnt=1;								//д�����ݸ���
	  	    HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL�ⷽʽд��
						while(esp32_send_cmd("AT+CIPSEND=0,12","OK",50));	
	          esp32_send_data("FMC success!",50);
					reg_state=0;
				}break;
				
			 default:
			 {
				 	ThreePage(); // �ȴ���������
				  reg_state=0;
					state=1; // ��Ч����
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
				FPGA_offset=18;		//FPGAд���ַ
				buffer_cnt=1000;								//д�����ݸ���
	  	  HAL_SRAM_Write_16b(&hsram1,((uint32_t *)(0x60000000 + (FPGA_offset<<1))),(uint16_t *)buffer,buffer_cnt);	//HAL�ⷽʽд��
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



