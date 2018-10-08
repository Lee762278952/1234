#ifndef _TASK_H
#define _TASK_H

#include "includes.h"

//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小	
#define START_STK_SIZE 		512
//任务控制块
extern OS_TCB StartTaskTCB;
//任务堆栈	
extern CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数


void start_task(void *p_arg);

#endif
