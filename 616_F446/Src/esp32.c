#include "esp32.h"
#include "string.h"
#include "usart.h"
#include "sys.h" 
#include "UART_menu.h"
#include "main.h"

//ESP32模块和PC进入透传模式
void esp32_start_trans(void)
{
	uint8_t client=0;
	uint8_t wifi=0;
	uint8_t *A1 ="192.168.4.2";
	uint8_t *A2 ="CONNECT";
	
	while(esp32_send_cmd("AT+RESTORE","ready",100));
	LL_mDelay(10000);         //延时1S等待重启成功
	
	while(esp32_send_cmd("AT","OK",50));
	while(esp32_send_cmd("AT+CWMODE=3","OK",50));//设置工作模式 1：station模式   2：AP模式  3：兼容 AP+station模式
	while(esp32_send_cmd("AT+CWSAP=\"ESP322\",\"lihuiying\",11,2","OK",100));
	while(esp32_send_cmd("AT+RST","ready",100));//让Wifi模块重启
	LL_mDelay(5000);         //延时3S
	
	printf("Please Contact WIFI!\r\n");//接收到期待的应答结果,连接WIFI
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
  while(esp32_send_cmd("AT+CIPMUX=1","OK",50));//=0：单路连接模式     =1：多路连接模式
  while(esp32_send_cmd("AT+CIPSERVER=1,8089","OK",50));
  LL_mDelay(5000);         //延时3S，等待连接

	printf("Please Contact TCP Server!\r\n");//接收到期待的应答结果，客户端连接
  
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


//向ESP32发送命令
//cmd:发送的命令字符串;ack:期待的应答结果,如果为空,则表示不需要等待应答;waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果);1,发送失败
uint8_t esp32_send_cmd(uint8_t *cmd,uint8_t *ack,uint16_t waittime)
{
	uint8_t res=0; 
	UART4_RX_STA=0;
	UART4_cnt = 0;
	u3_printf("%s\r\n",cmd);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			LL_mDelay(100);
			if(UART4_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(esp32_check_cmd(ack))
				{
					printf("ack:%s\r\n",(uint8_t*)ack);
					break;//得到有效数据 
				}
					UART4_RX_STA=0;
				  UART4_cnt = 0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 


//ESP32命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果;其他,期待应答结果的位置(str的位置)
uint8_t* esp32_check_cmd(uint8_t *str)
{
	char *strx=0;
	if(UART4_RX_STA&0X8000)		//接收到一次数据了
	{ 
		UART4_RX_BUF[UART4_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)UART4_RX_BUF,(const char*)str);
	} 
	return (uint8_t*)strx;
}


//向ESP32发送数据
//cmd:发送的命令字符串;waittime:等待时间
//返回值:发送数据后，服务器的返回验证码
uint8_t* esp32_send_data(uint8_t *cmd,uint16_t waittime)
{
	char temp[5];
	char *ack=temp;
	UART4_RX_STA=0;
	DATA_RX_STA=0;
	UART4_cnt = 0;
	u3_printf("%s",cmd);	//发送命令
	if(waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			LL_mDelay(10);
			if(UART4_RX_STA&0X8000)//接收到期待的应答结果
			{
				UART4_RX_BUF[UART4_RX_STA&0X7FFF]=0;//添加结束符
				ack=(char*)UART4_RX_BUF;
				printf("ack:%s\r\n",(uint8_t*)ack);
				UART4_RX_STA=0;
				DATA_RX_STA=0;
				UART4_cnt = 0;
				break;//得到有效数据 
			} 
		}
	}
	return (uint8_t*)ack;
} 


//ESP32接收数据并通过FMC发送至FPGA

void esp32_fmc_fpga(void)
{
	  int j = 0;
	  int k = 0;
  	int recieve_data = 0;
	  
		if(DATA_RX_STA&0X8000)		//接收到一次数据了
		{ 	
				UART4_RX_STA=0;//标志位清零
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
						LL_USART_TransmitData8(USART1,DATA_RX_BUF[i]); //发送已接收无效数据
				  	while(LL_USART_IsActiveFlag_TXE(USART1)==0);				
				   	}
														
				}
				 
				printf("\r\n");
					for(cntRx=0;cntRx<=2;cntRx++){
						strRx[cntRx]=Rxdata_BUF[cntRx];
						LL_USART_TransmitData8(USART1,strRx[cntRx]); //发送已接收数据
				  	while(LL_USART_IsActiveFlag_TXE(USART1)==0);	
						}
				printf("\r\n");
				cntRx=0;
				
				
				// 三字节数据接收完毕，跳转下一状态
				if((reg_state ==1) &&(reg_num!= '21')) //除了data_in 其余寄存器只需赋值一次
				{
					state =3;
				}
				else if ((reg_state ==1) && (reg_num== '21')) // data_in连续赋值
				{
					state =4;

				}
				else
				{
					state =2; // 判断输入数据的类型
				}

				if(UART4_cnt < 100){//数组清零
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


