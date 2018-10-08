#include "sys_application.h"
#include "includes.h"
#include "lcd.h"
#include "w25qxx.h"
#include "sys_malloc.h"
#include "sys_network.h"
#include "lwip_comm.h"
#include "rtc.h"
#include "text.h"
#include "power.h"
#include <time.h>
#include "sysdebug.h"
#include "piclib.h"




//u16 timer1;
//static u8 heartbeat_Timer;

/*************************ϵͳ��ʱ�� 1S***********************************/
static u8 min_t=0;
u8 ShutDown_t=0;
u16 wifista_t=0;
void SYS_Timer_1S_Callback(void)
{
	sys_debug_time++;
//	printf("SYS_Timer_1S start...\r\n");
	//��Ļ�����Զ��µ�
	if(SYS->dim_bl < ScreenDimTime)	SYS->dim_bl++;
	if(SYS->dim_bl == ScreenDimTime)
	{
		LCD.BackLightSet(20);
		debug("backlight dim\r\n");
		SYS->dim_bl++;
		
		debug("SRAMIN usage rate = %d%%\r\n",my_mem_perused(SRAMIN)/10);
		debug("SRAMEX usage rate = %d%%\r\n",my_mem_perused(SRAMEX)/10);
	}

	if(NETWORK.WIFI_TransRdy || NETWORK.LWIP_TransRdy)
	{
		SYS->heartbeat_Timer ++;

		if(SYS->heartbeat_Timer >= HeartBeatTime)
		{
			SYS->HeartBeat = True;
			SYS->heartbeat_Timer = 0;
		}
	}
	else SYS->heartbeat_Timer = 0;

	min_t++;	
	if(min_t >= 60 || DeviceSta_Get(0))
	{
		SYS->DisplayInside_SET = 1;
		debug("20%02d-%02d-%02d %02d:%02d Ele:%d%% wifi:-%ddb CPU:%d%%\r\n",SYS->TimeDate.Year,SYS->TimeDate.Month,SYS->TimeDate.Date,SYS->TimeDate.Hours,SYS->TimeDate.Minutes,SYS->ElectricQua,SYS->WIFI_SignalSt,OSStatTaskCPUUsage/100);
		min_t = 0;
	}

	
}
/*************************ϵͳ��ʱ�� 500ms***********************************/
static OS_ERR err_t500;
void SYS_Timer_500MS_Callback(void)
{
	#if !ApolloDev

		//�ػ�
	if(POWER_CHECK) ShutDown_t++;
	else if(ShutDown_t != 0) ShutDown_t = 0;
	if(ShutDown_t > 4) 
	{
		SYS->Device_ShutDown = True;
		LCD.BackLightSet(0);
		LCD.SelectScreen(ScreenOutside);
		LCD.BackLightSet(0);
    OSTimeDlyHMSM(0,0,5,0,OS_OPT_TIME_HMSM_STRICT,&err_t500);
		POWER_ENABLE = 0;
		while(1);
	}

#endif	
}
/*************************ϵͳ��ʱ�� 100ms***********************************/
extern _BOOL NetWork_TransmitEnable;
u8 TransmitTimer;
void SYS_Timer_100MS_Callback(void)
{
	//500ms �ͷ�һ�����紫��
	if(NetWork_TransmitEnable == False)
	{
		TransmitTimer++;
		if(TransmitTimer >= 10)
		{
//			printf("NetWork_TransmitEnable Ready!\r\n");
			NetWork_TransmitEnable = True;
			TransmitTimer = 0;
		}
	}
	
	//LWIP��ʱ��
	LWIP.LocalTime += 100;

}
/****************************************************************************/


static _BOOL net_temp;
static u8 msgnum_temp;
_BOOL DeviceSta_Get(u8 opt)
{
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
	struct tm *time = NULL;

	time_t stamp;
	u16 pow;
	_BOOL ret = False;
	
	if(SYS->TimeSet)
	{
//		time = mymalloc(SRAMIN,50);
//		SYS_MALLOC(time,SRAMIN,500);
//		Print_MEM_POOL(SRAMIN);
		stamp = SYS->TimeStamp;
		time = localtime(&stamp);
	  debug("Set \'RTC\' time:%d-%d-%d %d:%d:%d  %d  \r\n",time->tm_year-100,time->tm_mon+1,time->tm_mday,time->tm_hour+8,time->tm_min,time->tm_sec,(time->tm_wday==0)?7:time->tm_wday); 
		
		RTC_Set_Date(time->tm_year-100,time->tm_mon+1,time->tm_mday,(time->tm_wday==0)?7:time->tm_wday);	//��������
		RTC_Set_Time(time->tm_hour+8,time->tm_min,time->tm_sec,0);	        //����ʱ�� ,����ʵ��ʱ���޸�
			                    
		HAL_RTCEx_BKUPWrite(&RTC_Handler,RTC_BKP_DR0,0X5050);//����Ѿ���ʼ������
		
		SYS->TimeSet = False;
//		myfree(SRAMIN,time);
//		SYS_FREE(time,SRAMIN);

		
		ret = True;
	}
	
	HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStruct,RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BIN);
	
	SYS->TimeDate.Year = RTC_DateStruct.Year;
	SYS->TimeDate.Month = RTC_DateStruct.Month;
	SYS->TimeDate.Date = RTC_DateStruct.Date;
	SYS->TimeDate.WeekDay = RTC_DateStruct.WeekDay;
	SYS->TimeDate.Hours = RTC_TimeStruct.Hours;
	SYS->TimeDate.Minutes = RTC_TimeStruct.Minutes;
	SYS->TimeDate.Seconds = RTC_TimeStruct.Seconds;
	
	if(opt == 1 || ret == True)
	{
		sprintf(SYS->TimeDate.YearStr,"20%02d��",SYS->TimeDate.Year);
		sprintf(SYS->TimeDate.MonthDateStr,"%02d��%02d��",SYS->TimeDate.Month,SYS->TimeDate.Date);
		switch(SYS->TimeDate.WeekDay)
		{
			case 1:sprintf(SYS->TimeDate.WeekDayStrCN,"����һ");break;
			case 2:sprintf(SYS->TimeDate.WeekDayStrCN,"���ڶ�");break;
			case 3:sprintf(SYS->TimeDate.WeekDayStrCN,"������");break;
			case 4:sprintf(SYS->TimeDate.WeekDayStrCN,"������");break;
			case 5:sprintf(SYS->TimeDate.WeekDayStrCN,"������");break;
			case 6:sprintf(SYS->TimeDate.WeekDayStrCN,"������");break;
			case 7:sprintf(SYS->TimeDate.WeekDayStrCN,"������");break;
		}
	}
  
	
	pow = PowerCheck();
	if(SYS->Charging_EN == False && (pow < SYS->ElectricQua || SYS->ElectricQua == 0)) 
	{
		SYS->ElectricQua = pow;
		ret = True;
	}
	else if(SYS->Charging_EN == True && (pow > SYS->ElectricQua || SYS->ElectricQua == 0))
	{
		SYS->ElectricQua = pow;
		ret = True;
	}
	
	if(CHG_CHECK == 1 && SYS->Charging_EN == False) 
	{
		SYS->Charging_EN = True;
		ret = True;
	}
	else if(CHG_CHECK == 0 && SYS->Charging_EN == True)
	{
		SYS->Charging_EN = False;
		ret = True;
	}
	
	if(net_temp != (NETWORK.LWIP_TransRdy || NETWORK.WIFI_TransRdy)) 
	{
		net_temp = (_BOOL)(NETWORK.LWIP_TransRdy || NETWORK.WIFI_TransRdy);
		ret = True;
	}
	
	if(SYS->RecMsgContNum != msgnum_temp)
	{
		msgnum_temp = SYS->RecMsgContNum;
		ret = True;
	}

	return ret;
}


//��FLASH������ȡϵͳ�ؼ�����
//operation 1=Save  0=Load
//section  
void SYSPara_SaveNLoad(u8 operation,u8 section)
{
	u8 *FlashBuf = NULL;
	
//	FlashBuf = mymalloc(SRAMIN,100);
	SYS_MALLOC(FlashBuf,SRAMIN,100);
	
	if(!FlashBuf) return;
	
	memset(FlashBuf,'\0',100);
	
	if(operation)//SAVE
	{
		if(section == 0 || section == 1)
		{
			//�豸ID
			FlashBuf[0] = SYS->Device_UID >> 8;
			FlashBuf[1] = SYS->Device_UID & 0x00FF;
			W25QXX_Write(FlashBuf,Device_UID_addr,Device_UIDLen);
		}
		
		if(section == 0 || section == 2)
		{
			//����
			memcpy(FlashBuf,SYS->Name_Person,Name_PerLen);
			W25QXX_Write(FlashBuf,Name_Person_addr,Name_PerLen);
			//ְλ
			memcpy(FlashBuf,SYS->Name_Position,Name_PerLen);
			W25QXX_Write(FlashBuf,Name_Position_addr,Name_PosLen);
			//��λ
			memcpy(FlashBuf,SYS->Name_Company,Name_ComLen);
			W25QXX_Write(FlashBuf,Name_Company_addr,Name_ComLen);
			//������ɫ
			FlashBuf[0] = SYS->Name_PerColor >> 8;
			FlashBuf[1] = SYS->Name_PerColor & 0x00FF;
			W25QXX_Write(FlashBuf,Name_PerColor_addr,Name_PerColorLen);
			//ְλ��ɫ
			FlashBuf[0] = SYS->Name_PosColor >> 8;
			FlashBuf[1] = SYS->Name_PosColor & 0x00FF;
			W25QXX_Write(FlashBuf,Name_PosColor_addr,Name_PosColorLen);
			//��λ��ɫ
			FlashBuf[0] = SYS->Name_ComColor >> 8;
			FlashBuf[1] = SYS->Name_ComColor & 0x00FF;
			W25QXX_Write(FlashBuf,Name_ComColor_addr,Name_ComColorLen);
			//����������ɫ
			FlashBuf[0] = SYS->OuterBG_Color >> 8;
			FlashBuf[1] = SYS->OuterBG_Color & 0x00FF;
			W25QXX_Write(FlashBuf,OuterBG_Color_addr,OuterBG_ColorLen);
			//��������
			FlashBuf[0] = SYS->Name_PerSize;
			W25QXX_Write(FlashBuf,Name_PerSize_addr,Name_PerSizeLen);
			//ְλ����
			FlashBuf[0] = SYS->Name_PosSize;
			W25QXX_Write(FlashBuf,Name_PosSize_addr,Name_PosSizeLen);
			//��˾����
			FlashBuf[0] = SYS->Name_ComSize;
			W25QXX_Write(FlashBuf,Name_ComSize_addr,Name_ComSizeLen);
		}
		
		if(section == 0 || section == 3)
		{
			//��������·��
			memcpy(FlashBuf,SYS->InnerJPG_Path,InnerJPG_PathLen);
			W25QXX_Write(FlashBuf,InnerJPG_Path_addr,InnerJPG_PathLen);
			//��������·��
			memcpy(FlashBuf,SYS->OuterJPG_Path,OuterJPG_PathLen);
			W25QXX_Write(FlashBuf,OuterJPG_Path_addr,OuterJPG_PathLen);
			//������ʾ��ɫ��ͼƬ��־λ
			FlashBuf[0] = SYS->OuterBG_JPG;
			W25QXX_Write(FlashBuf,OuterBG_JPG_addr,OuterBG_JPGLen);
		}
		if(section == 0 || section == 4)
		{
			W25QXX_Write(FlashBuf,WIFI_SSID_addr,WIFI_SSIDLen);
			//SSID
			memcpy(FlashBuf,SYS->WIFI_SSID,WIFI_SSIDLen);
			W25QXX_Write(FlashBuf,WIFI_SSID_addr,WIFI_SSIDLen);
			//PASSWORD
			memcpy(FlashBuf,SYS->WIFI_PWD,WIFI_PWDLen);
			W25QXX_Write(FlashBuf,WIFI_PWD_addr,WIFI_PWDLen);
			//IP ADDRESS
			memcpy(FlashBuf,SYS->TCP_IPadd,TCP_IPaddLen);
			W25QXX_Write(FlashBuf,TCP_IPadd_addr,TCP_IPaddLen);
			//PORT
			memcpy(FlashBuf,SYS->TCP_Port,TCP_PortLen);
			W25QXX_Write(FlashBuf,TCP_Port_addr,TCP_PortLen);
			//STA IP
			memcpy(FlashBuf,SYS->Device_IP,Device_IPLen);
			W25QXX_Write(FlashBuf,Device_IP_addr,Device_IPLen);
			//DHCP
			FlashBuf[0] = SYS->DHCP_Enable;
			W25QXX_Write(FlashBuf,DHCP_Enable_addr,DHCP_EnableLen);
		}
		
	}
	else//Load
	{
		if(section == 0 || section == 1)
		{
			//�豸ID
			W25QXX_Read(FlashBuf,Device_UID_addr,2);
			SYS->Device_UID = ( FlashBuf[0] << 8 ) | FlashBuf[1];
		}
		if(section == 0 || section == 2)
		{
			W25QXX_Read(FlashBuf,Name_Person_addr,Name_PerLen);
			memcpy(SYS->Name_Person,FlashBuf,Name_PerLen);
			
			W25QXX_Read(FlashBuf,Name_Position_addr,Name_PosLen);
			memcpy(SYS->Name_Position,FlashBuf,Name_PosLen);
			
			W25QXX_Read(FlashBuf,Name_Company_addr,Name_ComLen);
			memcpy(SYS->Name_Company,FlashBuf,Name_ComLen);
			
			W25QXX_Read(FlashBuf,Name_PerColor_addr,Name_PerColorLen);
			SYS->Name_PerColor = ( FlashBuf[0] << 8 ) | FlashBuf[1];
			
			W25QXX_Read(FlashBuf,Name_PosColor_addr,Name_PosColorLen);
			SYS->Name_PosColor = ( FlashBuf[0] << 8 ) | FlashBuf[1];
			
			W25QXX_Read(FlashBuf,Name_ComColor_addr,Name_ComColorLen);
			SYS->Name_ComColor = ( FlashBuf[0] << 8 ) | FlashBuf[1];
			
			W25QXX_Read(FlashBuf,OuterBG_Color_addr,OuterBG_ColorLen);
			SYS->OuterBG_Color = ( FlashBuf[0] << 8 ) | FlashBuf[1];
			
			W25QXX_Read(FlashBuf,Name_PerSize_addr,Name_PerSizeLen);
			SYS->Name_PerSize = FlashBuf[0];
			
			W25QXX_Read(FlashBuf,Name_PosSize_addr,Name_PosSizeLen);
			SYS->Name_PosSize = FlashBuf[0];
			
			W25QXX_Read(FlashBuf,Name_ComSize_addr,Name_ComSizeLen);
			SYS->Name_ComSize = FlashBuf[0];
		}
		if(section == 0 || section == 3)
		{
			W25QXX_Read(FlashBuf,InnerJPG_Path_addr,InnerJPG_PathLen);
			memcpy(SYS->InnerJPG_Path,FlashBuf,30);
			
			W25QXX_Read(FlashBuf,OuterJPG_Path_addr,OuterJPG_PathLen);
			memcpy(SYS->OuterJPG_Path,FlashBuf,30);
			
			W25QXX_Read(FlashBuf,OuterBG_JPG_addr,OuterBG_JPGLen);
			SYS->OuterBG_JPG = (_BOOL)FlashBuf[0];
		}
		if(section == 0 || section == 4)
		{
			W25QXX_Read(FlashBuf,WIFI_SSID_addr,WIFI_SSIDLen);
			memcpy(SYS->WIFI_SSID,FlashBuf,WIFI_SSIDLen);
			
			W25QXX_Read(FlashBuf,WIFI_PWD_addr,WIFI_PWDLen);
			memcpy(SYS->WIFI_PWD,FlashBuf,WIFI_PWDLen);
			
			W25QXX_Read(FlashBuf,TCP_IPadd_addr,TCP_IPaddLen);
			memcpy(SYS->TCP_IPadd,FlashBuf,TCP_IPaddLen);
			
			W25QXX_Read(FlashBuf,TCP_Port_addr,TCP_PortLen);
			memcpy(SYS->TCP_Port,FlashBuf,TCP_PortLen);
			
			W25QXX_Read(FlashBuf,Device_IP_addr,Device_IPLen);
			memcpy(SYS->Device_IP,FlashBuf,Device_IPLen);
			
			W25QXX_Read(FlashBuf,DHCP_Enable_addr,DHCP_EnableLen);
			SYS->DHCP_Enable = (_BOOL)FlashBuf[0];
		}
	}
	
//	myfree(SRAMIN,FlashBuf);
	SYS_FREE(FlashBuf,SRAMIN);
}

