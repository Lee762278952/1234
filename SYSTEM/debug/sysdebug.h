#ifndef __SYSDEBUG_H
#define __SYSDEBUG_H
#include "sys.h"

#define ApolloDev 					0   //��������Ƿ��ڰ����޿���������

extern u32 sys_debug_time;

void debug(const char *cmd, ...);

u16 GetCompileDateTime(char *cDate,char *cTime);

#endif
