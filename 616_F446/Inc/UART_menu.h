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

#define Rxdata_MAX_RECV_LEN		3000//�����ջ����ֽ���

extern  uint8_t strRx[2]; // һ���Խ���2�ֽ����ݣ���Ӧ��16λ��Ĵ���
extern  uint8_t cntRx; // ���ռ���
extern  uint8_t state; // ����״̬��־λ��0- ������ѡ��Ĵ���  1- ����16λ����  2- �ж��������� 3-
extern  uint8_t reg_state;  // �Ĵ���ѡ��״̬��־λ�� 0- û��ѡ��Ĵ��� 1-ѡ���˼Ĵ���
extern  uint16_t reg_num; // ѡ���ļĴ������
extern  uint8_t Rxdata_BUF[Rxdata_MAX_RECV_LEN]; 		//���ջ���,���UART4_MAX_RECV_LEN�ֽ�
extern  uint16_t counter;

extern int i;
extern	uint16_t buffer_cnt;				//����������
extern	unsigned int data_address;			//FPGA������������ʼ��ַ:������ǰ16��λ��Ϊ�Ĵ�������data_address=16
extern	unsigned int FPGA_offset;				//FPGA RAM�ĵ�ַƫ����
extern	uint16_t buffer[3000];	//��������

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
