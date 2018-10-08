#include "sysdebug.h"
#include "includes.h"
#include <stdio.h>  
#include <stdarg.h>  

u32 sys_debug_time=0;


void debug(const char *cmd, ...)  
{  
	CPU_SR_ALLOC();

	OS_CRITICAL_ENTER();	//进入临界区
	printf("[%d] ",sys_debug_time);  
	va_list args;       //定义一个va_list类型的变量，用来储存单个参数  
	va_start(args,cmd); //使args指向可变参数的第一个参数  
	vprintf(cmd,args);  //必须用vprintf等带V的  
	va_end(args);       //结束可变参数的获取  
	OS_CRITICAL_EXIT();	
}

u16 GetCompileDateTime(char *cDate,char *cTime)
{        
        const int  MONTH_PER_YEAR=12;
        const char szEnglishMonth[MONTH_PER_YEAR][4]={ "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
        char szTmpDate[40]={0};
        char szTmpTime[20]={0};
        char szMonth[4]={0};
        int iYear,iMonth,iDay,iHour,iMin,iSec;//,,

        //获取编译日期、时间
        sprintf(szTmpDate,"%s",__DATE__); //"Sep 18 2010"
        sprintf(szTmpTime,"%s",__TIME__);    //"10:59:19"
        
        sscanf(szTmpDate,"%s %d %d",szMonth,&iDay,&iYear);
        sscanf(szTmpTime,"%d:%d:%d",&iHour,&iMin,&iSec);
                
        for(int i=0;MONTH_PER_YEAR;i++)
        {
                if(strncmp(szMonth,szEnglishMonth[i],3)==0)
                {
                        iMonth=i+1;
                        break;
                }
        }

//				printf("%d,%d,%d,%d,%d,%d ",iYear,iMonth,iDay,iHour,iMin,iSec);
        sprintf(cDate,"%04d%02d%02d",iYear,iMonth,iDay);
				sprintf(cTime,"%02d%02d%02d",iHour,iMin,iSec);
        return 0;
}


