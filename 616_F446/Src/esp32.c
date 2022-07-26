#include "esp32.h"
#include "string.h"
#include "usart.h"
#include "sys.h" 
#include "UART_menu.h"
#include "main.h"

//ESP32ģ���PC����͸��ģʽ
void esp32_start_trans(void)
{
	uint8_t client=0;
	uint8_t wifi=0;
	uint8_t *A1 ="192.168.4.2";
	uint8_t *A2 ="CONNECT";
	
	while(esp32_send_cmd("AT+RESTORE","ready",100));
	LL_mDelay(10000);         //��ʱ1S�ȴ������ɹ�
	
	while(esp32_send_cmd("AT","OK",50));
	while(esp32_send_cmd("AT+CWMODE=3","OK",50));//���ù���ģʽ 1��stationģʽ   2��APģʽ  3������ AP+stationģʽ
	while(esp32_send_cmd("AT+CWSAP=\"ESP322\",\"lihuiying\",11,2","OK",100));
	while(esp32_send_cmd("AT+RST","ready",100));//��Wifiģ������
	LL_mDelay(5000);         //��ʱ3S
	
	printf("Please Contact WIFI!\r\n");//���յ��ڴ���Ӧ����,����WIFI
	while(wifi==0){
	if(UART4_RX_STA&0X8000)
	{
		if(esp32_check_cmd(A1))
			{
				wifi=1;
				printf("ack:%s\r\n",(uint8_t*)A1);
				break;
			}
			UART4_RX_STA=0;
			UART4_cnt = 0;			
	}
	
}
  while(esp32_send_cmd("AT+CIPMUX=1","OK",50));//=0����·����ģʽ     =1����·����ģʽ
  while(esp32_send_cmd("AT+CIPSERVER=1,8089","OK",50));
  LL_mDelay(5000);         //��ʱ3S���ȴ�����

	printf("Please Contact TCP Server!\r\n");//���յ��ڴ���Ӧ�������ͻ�������
  
	while(client==0){
	if(UART4_RX_STA&0X8000)
	{
		if(esp32_check_cmd(A2))
			{
				client=1;
				printf("ack:%s\r\n",(uint8_t*)A2);
				break;
			}
			UART4_RX_STA=0;
			UART4_cnt = 0;			
	} 

	} 
	
}


//��ESP32��������
//cmd:���͵������ַ���;ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��;waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����);1,����ʧ��
uint8_t esp32_send_cmd(uint8_t *cmd,uint8_t *ack,uint16_t waittime)
{
	uint8_t res=0; 
	UART4_RX_STA=0;
	UART4_cnt = 0;
	u3_printf("%s\r\n",cmd);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			LL_mDelay(100);
			if(UART4_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(esp32_check_cmd(ack))
				{
					printf("ack:%s\r\n",(uint8_t*)ack);
					break;//�õ���Ч���� 
				}
					UART4_RX_STA=0;
				  UART4_cnt = 0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 


//ESP32�����,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����;����,�ڴ�Ӧ������λ��(str��λ��)
uint8_t* esp32_check_cmd(uint8_t *str)
{
	char *strx=0;
	if(UART4_RX_STA&0X8000)		//���յ�һ��������
	{ 
		UART4_RX_BUF[UART4_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)UART4_RX_BUF,(const char*)str);
	} 
	return (uint8_t*)strx;
}


//��ESP32��������
//cmd:���͵������ַ���;waittime:�ȴ�ʱ��
//����ֵ:�������ݺ󣬷������ķ�����֤��
uint8_t* esp32_send_data(uint8_t *cmd,uint16_t waittime)
{
	char temp[5];
	char *ack=temp;
	UART4_RX_STA=0;
	DATA_RX_STA=0;
	UART4_cnt = 0;
	u3_printf("%s",cmd);	//��������
	if(waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			LL_mDelay(10);
			if(UART4_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				UART4_RX_BUF[UART4_RX_STA&0X7FFF]=0;//��ӽ�����
				ack=(char*)UART4_RX_BUF;
				printf("ack:%s\r\n",(uint8_t*)ack);
				UART4_RX_STA=0;
				DATA_RX_STA=0;
				UART4_cnt = 0;
				break;//�õ���Ч���� 
			} 
		}
	}
	return (uint8_t*)ack;
} 


//ESP32�������ݲ�ͨ��FMC������FPGA

void esp32_fmc_fpga(void)
{
	  int j = 0;
	  int k = 0;
  	int recieve_data = 0;
	  
		if(DATA_RX_STA&0X8000)		//���յ�һ��������
		{ 	
				UART4_RX_STA=0;//��־λ����
			  DATA_RX_STA=0;  
		
				for(int i=0; i<(UART4_cnt-2); i++){
				
					if(recieve_data){						
						Rxdata_BUF[j] = DATA_RX_BUF[i];
					  if(((j+1)%2) ==0){
							buffer[k] = (short) (Rxdata_BUF[j-1]<<8 | (Rxdata_BUF[j]));
							k++;
					   	}
						j++;
					}
					else{
						if(DATA_RX_BUF[i] == ':'){
									recieve_data = 1;
								} 
						LL_USART_TransmitData8(USART1,DATA_RX_BUF[i]); //�����ѽ�����Ч����
				  	while(LL_USART_IsActiveFlag_TXE(USART1)==0);				
				   	}
														
				}
				 
				printf("\r\n");
					for(cntRx=0;cntRx<=2;cntRx++){
						strRx[cntRx]=Rxdata_BUF[cntRx];
						LL_USART_TransmitData8(USART1,strRx[cntRx]); //�����ѽ�������
				  	while(LL_USART_IsActiveFlag_TXE(USART1)==0);	
						}
				printf("\r\n");
				cntRx=0;
				
				
				// ���ֽ����ݽ�����ϣ���ת��һ״̬
				if((reg_state ==1) &&(reg_num!= '21')) //����data_in ����Ĵ���ֻ�踳ֵһ��
				{
					state =3;
				}
				else if ((reg_state ==1) && (reg_num== '21')) // data_in������ֵ
				{
					state =4;

				}
				else
				{
					state =2; // �ж��������ݵ�����
				}

				if(UART4_cnt < 100){//��������
					 for(int i=0; i<600;i++){ 
							 buffer[i] = 0;
							 }				
						}
         UART4_cnt = 0;
					
				for(int i=0; i<600;i++){
					DATA_RX_BUF[i] = 0;
					Rxdata_BUF[i] = 0;
					}
		
		
	}
} 


