#ifndef __ESP32_H
//#define __ESP32_H
#include "sys.h"
#include "stdio.h"	
#include "usart.h"
#include "UART_menu.h"
#include "main.h"

//�û�����ATָ��
extern char restart[];
extern char cwmode[];
extern char cwlap[];
extern char cwjap[];
extern char cifsr[];
extern char cipmux[];
extern char cipstart[];
extern char cipsend[];
extern char cipserver[];
extern char cwlif[];
extern char cipstatus[];
extern char cipsto[];
extern char cipmode[];
extern char test[];
#define Rxdata_MAX_RECV_LEN		3000//�����ջ����ֽ���
 extern  uint8_t Rxdata_BUF[Rxdata_MAX_RECV_LEN]; 		//���ջ���,���UART4_MAX_RECV_LEN�ֽ�
//����

uint8_t esp32_send_cmd(uint8_t *cmd,uint8_t *ack,uint16_t waittime);
uint8_t* esp32_send_data(uint8_t *cmd,uint16_t waittime);
uint8_t* esp32_check_cmd(uint8_t *str);
void esp32_start_trans(void);
void esp32_fmc_fpga(void);
uint8_t esp32_quit_trans(void);	
#endif
