#include "sys_malloc.h"
#include "sysdebug.h"
#include <string.h>

static _memory_record SRAMIN_MEN_POOL[MemoryPool_NUM][MAX_MemoryPool_Size];
//_memory_record SRAMEX_MEN_POOL[MAX_MemoryPool_Size];

static _memory_record *Find_MEM_POOL(u8 memx,u32 mem_pointer_addr);
static _BOOL Write_MEM_POOL(u8 memx,u32 size,char *p_name,const char *f_name,u32 p_address);

_BOOL sys_mem_malloc(void *(*mem_pointer),u8 memx,u32 size,char *mem_pointer_name,const char *function_name)
{
	void *p;
	char str[15],pname[Pname_MaxLen] = {'\0'},fname[Fname_MaxLen]= {'\0'};
	_memory_record *mem_pool_temp;
	
	strncpy(pname,mem_pointer_name,(strlen(mem_pointer_name)>=Pname_MaxLen ? Pname_MaxLen-1:strlen(mem_pointer_name)));
	strncpy(fname,function_name,(strlen(function_name)>=Fname_MaxLen ? Fname_MaxLen-1:strlen(function_name)));
	
	if(memx>1)
	{
#if Malloc_Debug && Malloc_Debug_Fail
		debug("Malloc fail: \'%s\' in Function \'%s()\'.memory block(memx = %d) unlawful\r\n",pname,fname,memx);
#endif
		*mem_pointer = NULL;
		return False;
	}
	
	
	if(*mem_pointer != NULL) //如果指针地址不为空，寻找内存池中该地址是否已经被注册
	{
#if Malloc_Debug && Malloc_Debug_Warning
		debug("Malloc warning: \'%s\' in Function \'%s()\'.The allocation pointer's addr is not \'NULL\'\r\n",pname,fname);
#endif
		
		//寻找内存池
		mem_pool_temp = Find_MEM_POOL(memx,(u32)*mem_pointer);
		
		if(mem_pool_temp == NULL) //如果内存池中没有已被注册的地址，将指针赋值为NULL
		{
			*mem_pointer = NULL;
		}
		else 
		{
			if(!strcmp(mem_pool_temp->FunctionName,fname) && !strcmp(mem_pool_temp->PointerName,pname)) //如果内存池中已经存在该地址的指针，比对指针名及申请内存的函数名
			{
				if(mem_pool_temp->Size == size) //如果大小一样 不需要重新分配 返回失败
				{
#if Malloc_Debug && Malloc_Debug_Fail
					debug("Malloc fail: \'%s\' in Function \'%s()\'.The memory of the pointer has been allocated the same size in this function\r\n",pname,fname);
#endif
					return False;
				}
				else //如果申请内存大小不同 释放原内存地址 重新分配
				{
					SYS_FREE((*mem_pointer),memx);
					debug("Malloc warning: \'%s\' in Function \'%s()\'.The pointer memory is not released (size=%d) and the system has reassigned memory\r\n",pname,fname,mem_pool_temp->Size);
				}
			}
			else *mem_pointer = NULL;
		}
	}
	
	p = mymalloc(memx,size);
	if(p == NULL) 
	{
#if Malloc_Debug && Malloc_Debug_Fail
		debug("Malloc fail: \'%s\' in Function \'%s()\'.Lack of memory space\r\n",pname,fname);
#endif
		*mem_pointer = NULL;
		return False;
	}
	*mem_pointer = p;
	sprintf(str,"%u",(u32)mem_pointer); //这个不能去掉

	if((u32)(*mem_pointer) == (u32)p)
	{
		Write_MEM_POOL(memx,size,pname,fname,(u32)*mem_pointer);
#if Malloc_Debug && Malloc_Debug_Success
		debug("Malloc success: \'%s\' in Function \'%s()\'.Allocated add = %u size = %d\r\n",pname,fname,(u32)*mem_pointer,size);
#endif
		memset(*mem_pointer,'\0',size);
		return True;
	}
	else
	{
#if Malloc_Debug && Malloc_Debug_Fail
		debug("Malloc fail: \'%s\' in Function \'%s()\'.Pointer cannot be initializedr,err_add=%s\r\n",pname,fname,str);
#endif
		myfree(memx,p);
		*mem_pointer = NULL;
		return False;
	}
	
}

_BOOL sys_mem_free(void *(*mem_pointer),u8 memx,char *mem_pointer_name)
{
	u8 i;
	if(*mem_pointer == NULL) 
	{
#if Malloc_Debug && Malloc_Debug_Fail
		debug("MEM Free fail:Pointer \'%s\' memory release failure,the address is \'NULL\'\r\n",mem_pointer_name);
#endif
		return False;
	}
	for(i=0;i<MAX_MemoryPool_Size;i++)
	{
		if(SRAMIN_MEN_POOL[memx][i].Enable == True && SRAMIN_MEN_POOL[memx][i].PointerAddress == (u32)(*mem_pointer))
		{
			if(strcmp(SRAMIN_MEN_POOL[memx][i].PointerName,mem_pointer_name)) 
			{
#if Malloc_Debug && Malloc_Debug_Warning
				debug("MEM Free warning:The name of the pointer \'%s\' is different from the memory pool record \'%s\', but they have the same address\r\n",
								SRAMIN_MEN_POOL[memx][i].PointerName,mem_pointer_name);
#endif
			}
			memset(*mem_pointer,'\0',SRAMIN_MEN_POOL[memx][i].Size);
			myfree(memx,*mem_pointer);
			*mem_pointer = NULL;
#if Malloc_Debug && Malloc_Debug_Success
			debug("MEM Free success:The memory of the pointer \'%s\' is successfully released,release %d bytes of memory\r\n",SRAMIN_MEN_POOL[memx][i].PointerName,SRAMIN_MEN_POOL[memx][i].Size);
#endif			
			memset(SRAMIN_MEN_POOL[memx][i].FunctionName,'\0',Fname_MaxLen);
			memset(SRAMIN_MEN_POOL[memx][i].PointerName,'\0',Pname_MaxLen);
			SRAMIN_MEN_POOL[memx][i].PointerAddress = NULL;
			SRAMIN_MEN_POOL[memx][i].Size = 0;
			SRAMIN_MEN_POOL[memx][i].Enable = False;
			
			return True;
		}
	}
#if Malloc_Debug && Malloc_Debug_Fail
	debug("MEM Free fail:Pointer \'%s\' memory release failure,no pointer address (%u) was found\r\n",mem_pointer_name,(u32)*mem_pointer);
#endif
	return False;
}

static _memory_record *Find_MEM_POOL(u8 memx,u32 mem_pointer_addr)
{
	u8 i;
	for(i=0;i<MAX_MemoryPool_Size;i++)
	{
		if(SRAMIN_MEN_POOL[memx][i].Enable)
		{
			if(SRAMIN_MEN_POOL[memx][i].PointerAddress == mem_pointer_addr) return &SRAMIN_MEN_POOL[memx][i];
		}
	}
	return NULL;
}

static _BOOL Write_MEM_POOL(u8 memx,u32 size,char *p_name,const char *f_name,u32 p_address)
{
	u8 i;
	for(i=0;i<MAX_MemoryPool_Size;i++)
	{
		if(SRAMIN_MEN_POOL[memx][i].Enable == False)
		{
			strcpy(SRAMIN_MEN_POOL[memx][i].FunctionName,f_name);
			strcpy(SRAMIN_MEN_POOL[memx][i].PointerName,p_name);
			SRAMIN_MEN_POOL[memx][i].PointerAddress = p_address;
			SRAMIN_MEN_POOL[memx][i].Size = size;
			SRAMIN_MEN_POOL[memx][i].Enable = True;
			return True;
		}
	}
	return False;
}

void Print_MEM_POOL(u8 memx)
{
	u8 i;
	debug("Printing the memory pool table SRAM=\'%d\'\r\n",memx);
	for(i=0;i<MAX_MemoryPool_Size;i++)
	{
		if(SRAMIN_MEN_POOL[memx][i].Enable == True)
		{
			printf("    [%d]: %s	%s		%u	%d\r\n",i,	
										SRAMIN_MEN_POOL[memx][i].FunctionName,
										SRAMIN_MEN_POOL[memx][i].PointerName,
										SRAMIN_MEN_POOL[memx][i].PointerAddress,
										SRAMIN_MEN_POOL[memx][i].Size);
		}
	}
	debug("Finish...\r\n");
}
