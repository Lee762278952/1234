#ifndef __SYS_MALLOC_H
#define __SYS_MALLOC_H
#include "sys.h" 
#include "malloc.h"

#define Malloc_Debug 							1

#define Malloc_Debug_Success 			0
#define Malloc_Debug_Fail 				1
#define Malloc_Debug_Warning 			1


#define MAX_MemoryPool_Size  			100
#define MemoryPool_NUM						2 //目前只支持SRAMIN和SRAMEX
#define Pname_MaxLen   						30
#define Fname_MaxLen   						20

#define SYS_MALLOC(p,memx,size) 	sys_mem_malloc((void**)(&(p)),memx,size,(#p),__FUNCTION__)
#define SYS_FREE(p,memx)     			sys_mem_free((void**)(&(p)),memx,(#p))


typedef struct
{
	_BOOL Enable;
	char PointerName[Pname_MaxLen];
	char FunctionName[Fname_MaxLen];
	u32 PointerAddress;
	u32 Size;
	
}_memory_record;

extern _BOOL sys_mem_malloc(void *(*mem_pointer),u8 memx,u32 size,char *mem_pointer_name,const char *function_name);
extern _BOOL sys_mem_free(void *(*mem_pointer),u8 memx,char *mem_pointer_name);
extern void Print_MEM_POOL(u8 memx);
#endif

