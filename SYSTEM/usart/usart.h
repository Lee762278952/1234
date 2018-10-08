#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"	

  	
			
typedef struct  
{		 	
	_BOOL Rx_Complete;		//��ɽ���
	_BOOL Rx_Receiving;  //���ڽ��ձ�־λ	
	
	u8  *Rx_Buffer;
	u32	Rx_BufLen;
	u32 BaudRate;
	u32 Max_RxBufLen;
	
	void (*INIT)(u32);
	void (*SendBuffer)(u8 *,u32);
	
}_uart_class; 
			
extern _uart_class UART1;
			



#endif
