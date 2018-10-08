#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"	

  	
			
typedef struct  
{		 	
	_BOOL Rx_Complete;		//完成接收
	_BOOL Rx_Receiving;  //正在接收标志位	
	
	u8  *Rx_Buffer;
	u32	Rx_BufLen;
	u32 BaudRate;
	u32 Max_RxBufLen;
	
	void (*INIT)(u32);
	void (*SendBuffer)(u8 *,u32);
	
}_uart_class; 
			
extern _uart_class UART1;
			



#endif
