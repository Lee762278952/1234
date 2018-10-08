#ifndef __SYS_APPLICATION_H__
#define __SYS_APPLICATION_H__
#include "sys.h"


void SYS_Timer_1S_Callback(void);
void SYS_Timer_100MS_Callback(void);
void SYS_Timer_500MS_Callback(void);
void UART1_REC_Timer_Callback(void);
void UART4_REC_Timer_Callback(void);
//void DeviceSta_Display(void);
_BOOL DeviceSta_Get(u8 opt);
void SYSPara_SaveNLoad(u8 operation,u8 section);
//void Calendar_Display(void);
#endif
