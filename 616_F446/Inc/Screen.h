#ifndef __SCREEN_H
#include "sys.h"
#include "stdio.h"	
#include "usart.h"
#include "main.h"
#include "dma.h"
#include "fmc.h"

#define S_Rxdata_MAX_RECV_LEN		3000//最大接收缓存字节数

 extern  uint16_t  Screen_addr;
 extern  uint32_t   FPGA_RESET;
 extern  uint16_t  Screen_data[3000];
 extern  uint16_t  Screen_data_now[99];
 extern	unsigned int Screen_FPGA_now_offset_init[99];				//FPGA RAM的地址偏移量
 extern uint8_t  UART5_TX_BUF[UART5_MAX_SEND_LEN]; 		//发送缓冲,最大UART5_MAX_SEND_LEN字节
 extern uint8_t  Screen_RX_BUF[Screen_MAX_RECV_LEN]; 
 extern	uint8_t Screen_cnt;				//传输数据量
 extern	unsigned int Screen_FPGA_offset;				//FPGA RAM的地址偏移量
 extern  uint16_t  Screen_data_init[84];
 extern	unsigned int Screen_FPGA_offset_init[84];				//FPGA RAM的地址偏移量
 extern	unsigned int ADC_CHn_FLAG;
 extern	uint8_t Vaule_12_flag;
 extern	uint8_t CHn_flag[12];
 extern uint8_t Start_flag;
 extern uint8_t  Vaule_12_flag;
//函数


void Screen_fmc_fpga(void);
void Screen_init(void);
void Screen_RESET_init(void);
void Screen_current_value(void);
void ADC_Value(void);
#endif

