#ifndef __UART_menu_H
#define __UART_menu_H

#ifdef __cplusplus
 extern "C" {
 
#endif

#include "string.h"
#include "stdio.h"
#include "usart.h"
#include "dma.h"
#include "fmc.h"
#include "esp32.h"

#define Rxdata_MAX_RECV_LEN		3000//最大接收缓存字节数

extern  uint8_t strRx[2]; // 一次性接收2字节数据，对应于16位宽寄存器
extern  uint8_t cntRx; // 接收计数
extern  uint8_t state; // 运行状态标志位：0- 开机，选择寄存器  1- 输入16位数据  2- 判断数据类型 3-
extern  uint8_t reg_state;  // 寄存器选择状态标志位： 0- 没有选择寄存器 1-选定了寄存器
extern  uint16_t reg_num; // 选定的寄存器编号
extern  uint8_t Rxdata_BUF[Rxdata_MAX_RECV_LEN]; 		//接收缓冲,最大UART4_MAX_RECV_LEN字节
extern  uint16_t counter;

extern int i;
extern	uint16_t buffer_cnt;				//传输数据量
extern	unsigned int data_address;			//FPGA中数据区域起始地址:若设置前16个位置为寄存器，则data_address=16
extern	unsigned int FPGA_offset;				//FPGA RAM的地址偏移量
extern	uint16_t buffer[3000];	//输入数组

void UART_menu(void);
void OnePage(void);
void TwoPage(void);
void ThreePage(void);
void FourPage(void);
		
	
#ifdef __cplusplus
}
#endif

#endif /* __UART_menu_H */

/**
  * @}
  */
