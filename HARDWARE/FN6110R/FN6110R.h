#ifndef _FN6110R_H
#define _FN6110R_H
#include "sys.h"
#include "usart.h"

#define 	CHIP_EN 	PCout(12)   

#define 	Timer5_FrequencyDiv		(9000-1)
#define   Timer5_AutoLoading		(200-1)				//Timer4×Ô¶¯×°ÔØÖÃ  £¨10ms£©
#define 	FN6110R_BaudRate  		115200
#define 	UART4_REC_LEN  				10*1024  

extern _uart_class U4;

void FN6110R_INIT(void);

#endif

