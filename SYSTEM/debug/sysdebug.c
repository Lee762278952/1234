#include "sysdebug.h"
#include "includes.h"
#include <stdio.h>  
#include <stdarg.h>  

u32 sys_debug_time=0;


void debug(const char *cmd, ...)  
{  
	CPU_SR_ALLOC();

	OS_CRITICAL_ENTER();	//�����ٽ���
	printf("[%d] ",sys_debug_time);  
	va_list args;       //����һ��va_list���͵ı������������浥������  
	va_start(args,cmd); //ʹargsָ��ɱ�����ĵ�һ������  
	vprintf(cmd,args);  //������vprintf�ȴ�V��  
	va_end(args);       //�����ɱ�����Ļ�ȡ  
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

        //��ȡ�������ڡ�ʱ��
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


