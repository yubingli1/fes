#ifndef __SCREEN_H
#include "sys.h"
#include "stdio.h"	
#include "usart.h"
#include "main.h"
#include "dma.h"
#include "fmc.h"

#define S_Rxdata_MAX_RECV_LEN		3000//�����ջ����ֽ���

 extern  uint16_t  Screen_addr;
 extern  uint32_t   FPGA_RESET;
 extern  uint16_t  Screen_data[3000];
 extern  uint16_t  Screen_data_now[99];
 extern	unsigned int Screen_FPGA_now_offset_init[99];				//FPGA RAM�ĵ�ַƫ����
 extern uint8_t  UART5_TX_BUF[UART5_MAX_SEND_LEN]; 		//���ͻ���,���UART5_MAX_SEND_LEN�ֽ�
 extern uint8_t  Screen_RX_BUF[Screen_MAX_RECV_LEN]; 
 extern	uint8_t Screen_cnt;				//����������
 extern	unsigned int Screen_FPGA_offset;				//FPGA RAM�ĵ�ַƫ����
 extern  uint16_t  Screen_data_init[84];
 extern	unsigned int Screen_FPGA_offset_init[84];				//FPGA RAM�ĵ�ַƫ����
 extern	unsigned int ADC_CHn_FLAG;
 extern	uint8_t Vaule_12_flag;
 extern	uint8_t CHn_flag[12];
 extern uint8_t Start_flag;
 extern uint8_t  Vaule_12_flag;
//����


void Screen_fmc_fpga(void);
void Screen_init(void);
void Screen_RESET_init(void);
void Screen_current_value(void);
void ADC_Value(void);
#endif

