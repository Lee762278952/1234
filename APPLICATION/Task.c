#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "task.h"
#include "includes.h"
#include "lcd.h"
#include "sdram.h"
#include "touch.h"
#include "w25qxx.h"
#include "sys_malloc.h"
#include "exfuns.h"
#include "piclib.h"
#include "text.h"
#include "ff.h"
#include "sdio_sdcard.h"
#include "ltdc.h"
#include "fattester.h"
#include "usb_func.h"
#include "sys_application.h" 
#include "pyinput.h"
#include "FN6110R.h"
#include "wifi.h"
#include "sys_network.h"
#include "lwip_comm.h"
#include "rtc.h"
#include "power.h"
#include "sysdebug.h"
#include "app_ui.h"

#include "sys_malloc.h"

#if ApolloDev
#include "pcf8574.h"
#endif


//UCOSIII中以下优先级用户程序不能使用
//优先级0：中断服务服务管理任务 OS_IntQTask()
//优先级1：时钟节拍任务 OS_TickTask()
//优先级2：定时任务 OS_TmrTask()
//优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
//优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()
/****************************分配任务信息********************************/

//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小	
#define START_STK_SIZE 		512
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数


//任务优先级
#define SYS_INIT_TASK_PRIO		4
//任务堆栈大小	
#define SYS_INIT_STK_SIZE 		512
//任务控制块
OS_TCB SYS_INITTaskTCB;
//任务堆栈	
CPU_STK SYS_INIT_TASK_STK[SYS_INIT_STK_SIZE];
//声明任务函数
void SYS_INIT_task(void *p_arg);


//任务优先级
#define HTTP_REQUEST_TASK_PRIO		4
//任务堆栈大小	
#define HTTP_REQUEST_STK_SIZE 		512
//任务控制块
OS_TCB HTTP_REQUEST_TaskTCB;
//任务堆栈	
CPU_STK HTTP_REQUEST_TASK_STK[HTTP_REQUEST_STK_SIZE];
//声明任务函数
void Http_Request_TaskThread(void *p_arg);

//任务优先级
#define FUNCFRAMEWORK_TASK_PRIO		5
//任务堆栈大小	
#define FUNCFRAMEWORK_STK_SIZE 		512
//任务控制块
OS_TCB FUNCFRAMEWORK_TaskTCB;
//任务堆栈	
CPU_STK FUNCFRAMEWORK_TASK_STK[FUNCFRAMEWORK_STK_SIZE];
//声明任务函数
void FuncFramework_TaskThread(void *p_arg);


//任务优先级
#define DISPLAYINSIDE_TASK_PRIO		6
//任务堆栈大小	
#define DISPLAYINSIDE_STK_SIZE 		512
//任务控制块
OS_TCB DISPLAYINSIDE_TaskTCB;
//任务堆栈	
CPU_STK DISPLAYINSIDE_TASK_STK[DISPLAYINSIDE_STK_SIZE];
//声明任务函数
void DisplayInside_TaskThread(void *p_arg);


//任务优先级
#define MAINPROCESS_TASK_PRIO		7
//任务堆栈大小	
#define MAINPROCESS_STK_SIZE 		512
//任务控制块
OS_TCB MAINPROCESS_TaskTCB;
//任务堆栈	
CPU_STK MAINPROCESS_TASK_STK[MAINPROCESS_STK_SIZE];
//声明任务函数
void MainProcess_TaskThread(void *p_arg);




//任务优先级
#define NETWORK_TASK_PRIO		8
//任务堆栈大小	
#define NETWORK_STK_SIZE 		512
//任务控制块
OS_TCB NETWORK_TaskTCB;
//任务堆栈	
CPU_STK NETWORK_TASK_STK[NETWORK_STK_SIZE];
//声明任务函数
void NetWork_TaskThread(void *p_arg);



//任务优先级
#define WIFISTAMANAGE_TASK_PRIO		10
//任务堆栈大小	
#define WIFISTAMANAGE_STK_SIZE 		512
//任务控制块
OS_TCB WIFISTAMANAGE_TaskTCB;
//任务堆栈	
CPU_STK WIFISTAMANAGE_TASK_STK[WIFISTAMANAGE_STK_SIZE];
//声明任务函数
void WIFI_StaManage_TaskThread(void *p_arg);

//任务优先级
#define TP_TASK_PRIO		3
//任务堆栈大小	
#define TP_STK_SIZE 		256
//任务控制块
OS_TCB TP_TaskTCB;
//任务堆栈	
CPU_STK TP_TASK_STK[TP_STK_SIZE];
//声明任务函数
void TP_TaskThread(void *p_arg);


//任务优先级
#define DISPLAYOUTSIDE_TASK_PRIO		13
//任务堆栈大小	
#define DISPLAYOUTSIDE_STK_SIZE 		512
//任务控制块
OS_TCB DISPLAYOUTSIDE_TaskTCB;
//任务堆栈	
CPU_STK DISPLAYOUTSIDE_TASK_STK[DISPLAYOUTSIDE_STK_SIZE];
//声明任务函数
void DisplayOutside_TaskThread(void *p_arg);


//任务优先级
#define LWIPSTAMANAGE_TASK_PRIO		14
//任务堆栈大小	
#define LWIPSTAMANAGE_STK_SIZE 		512
//任务控制块
OS_TCB LWIPSTAMANAGE_TaskTCB;
//任务堆栈	
CPU_STK LWIPSTAMANAGE_TASK_STK[LWIPSTAMANAGE_STK_SIZE];
//声明任务函数
void LWIP_StaManage_TaskThread(void *p_arg);



//测试专用线程
#if Test_task

//任务优先级
#define TEST_TASK_PRIO		20
//任务堆栈大小	
#define TEST_STK_SIZE 		512
//任务控制块
OS_TCB TESTTaskTCB;
//任务堆栈	
CPU_STK TEST_TASK_STK[TEST_STK_SIZE];
//声明任务函数
void TEST_task(void *p_arg);

#endif


/***********************系统参数*******************************/
_sys_para_class *SYS;
//定时器
OS_TMR SYS_Timer_1S;
OS_TMR SYS_Timer_500MS;
OS_TMR SYS_Timer_100MS;


//信号量
OS_MUTEX	LCDDisplay_MUTEX;//屏幕控制信号量 用于内外屏不能同时调用驱动函数
OS_MUTEX NetWork_MUTEX;//网络资源信号量


//默认文件路径
//const u8 Def_PicPath[51][30] = {
//{"BACKGROUND/bg1.jpg"},
//{"BACKGROUND/bg2.jpg"},
//{"BACKGROUND/bg3.jpg"},
//{"ICON/icon1.jpg"},
//{"ICON/icon2.jpg"},
//{"ICON/icon3.jpg"},
//{"ICON/icon4.jpg"},
//{"ICON/icon5.jpg"},
//{"ICON/icon6.jpg"},
//{"ICON/picon1.jpg"},
//{"ICON/picon2.jpg"},
//{"ICON/picon3.jpg"},
//{"ICON/picon4.jpg"},
//{"ICON/picon5.jpg"},
//{"ICON/picon6.jpg"},
//{"ICON/sicon1.jpg"},
//{"ICON/sicon2.jpg"},
//{"ICON/sicon3.jpg"},
//{"ICON/sicon4.jpg"},
//{"ICON/sicon5.jpg"},
//{"ICON/sicon6.jpg"},
//{"ICON/sicon7.jpg"},
//{"ICON/sicon8.jpg"},
//{"ICON/spicon1.jpg"},
//{"ICON/spicon2.jpg"},
//{"ICON/spicon3.jpg"},
//{"ICON/spicon4.jpg"},
//{"ICON/spicon5.jpg"},
//{"ICON/spicon6.jpg"},
//{"ICON/spicon7.jpg"},
//{"ICON/spicon8.jpg"},
//{"ICON/oicon1.jpg"},
//{"ICON/oicon2.jpg"},
//{"ICON/oicon3.jpg"},
//{"ICON/oicon4.jpg"},
//{"ICON/oicon5.jpg"},
//{"ICON/opicon1.jpg"},
//{"ICON/opicon2.jpg"},
//{"ICON/opicon3.jpg"},
//{"ICON/opicon4.jpg"},
//{"ICON/opicon5.jpg"},
//{"ICON/micon1.jpg"},
//{"ICON/micon2.jpg"},
//{"ICON/micon3.jpg"},
//{"ICON/micon4.jpg"},
//{"ICON/mpicon1.jpg"},
//{"ICON/mpicon2.jpg"},
//{"ICON/mpicon3.jpg"},
//{"ICON/mpicon4.jpg"},
//{"ICON/calendar.jpg"},
//{"NAMEPIC/obg1.jpg"},
//};

const u8 Def_PicPath[32][30] = {
{"BACKGROUND/background.jpg"},
{"ICON/baocun_h.jpg"},
{"ICON/baocun_n.jpg"},
{"ICON/dxfw_h.jpg"},
{"ICON/dxfw_n.jpg"},
{"ICON/fanhui_n.jpg"},
{"ICON/fasong_h.jpg"},
{"ICON/fasong_n.jpg"},
{"ICON/grxx_h.jpg"},
{"ICON/grxx_n.jpg"},
{"ICON/hjfw_h.jpg"},
{"ICON/hjfw_n.jpg"},
{"ICON/hyxx_h.jpg"},
{"ICON/hyxx_n.jpg"},
{"ICON/icon_off_n.jpg"},
{"ICON/icon_on_n.jpg"},
{"ICON/img_bi_n.jpg"},
{"ICON/img_cha_n.jpg"},
{"ICON/img_fwy_n.jpg"},
{"ICON/img_hua_n.jpg"},
{"ICON/img_huatong_n.jpg"},
{"ICON/img_kafei_n.jpg"},
{"ICON/img_shui_n.jpg"},
{"ICON/img_zhi_n.jpg"},
{"ICON/kzd_n.jpg"},
{"ICON/mpkj_n.jpg"},
{"ICON/qiehuan_h.jpg"},
{"ICON/qiehuan_n.jpg"},
{"ICON/xtsz_h.jpg"},
{"ICON/xtsz_n.jpg"},
{"ICON/xuanze_h.jpg"},
{"ICON/xuanze_n.jpg"},
};


//背景图片路径 （前三个跟默认路径相同 ，默认路径是默认存在的3个图片路径）
//const u8 BG_PicPath[6][30] = {
//	{"BACKGROUND/bg1.jpg"},
//	{"BACKGROUND/bg2.jpg"},
//	{"BACKGROUND/bg3.jpg"},
//	{"BACKGROUND/bg4.jpg"},
//	{"BACKGROUND/bg5.jpg"},
//	{"BACKGROUND/bg6.jpg"}
//};

const u16 ICON_Location[6][4] = {
//主页图标坐标
{ICON1_Xs,ICON1_Xe,ICON1_Ys,ICON1_Ye},
{ICON2_Xs,ICON2_Xe,ICON2_Ys,ICON2_Ye},
{ICON3_Xs,ICON3_Xe,ICON3_Ys,ICON3_Ye},
{ICON4_Xs,ICON4_Xe,ICON4_Ys,ICON4_Ye},
{ICON5_Xs,ICON5_Xe,ICON5_Ys,ICON5_Ye},
{ICON6_Xs,ICON6_Xe,ICON6_Ys,ICON6_Ye},
};

const u16 SICON_Location[8][4] = {
//呼叫服务图标坐标
{SICON1_Xs,SICON1_Xe,SICON1_Ys,SICON1_Ye},
{SICON2_Xs,SICON2_Xe,SICON2_Ys,SICON2_Ye},
{SICON3_Xs,SICON3_Xe,SICON3_Ys,SICON3_Ye},
{SICON4_Xs,SICON4_Xe,SICON4_Ys,SICON4_Ye},
{SICON5_Xs,SICON5_Xe,SICON5_Ys,SICON5_Ye},
{SICON6_Xs,SICON6_Xe,SICON6_Ys,SICON6_Ye},
{SICON7_Xs,SICON7_Xe,SICON7_Ys,SICON7_Ye},
{SICON8_Xs,SICON8_Xe,SICON8_Ys,SICON8_Ye},
};


const u16 MICON_Location[4][4] = {
//呼叫服务图标坐标
{MICON1_Xs,MICON1_Xe,MICON1_Ys,MICON1_Ye},
{MICON2_Xs,MICON2_Xe,MICON2_Ys,MICON2_Ye},
{MICON3_Xs,MICON3_Xe,MICON3_Ys,MICON3_Ye},
{MICON4_Xs,MICON4_Xe,MICON4_Ys,MICON4_Ye},
};

const u16 OICON_Location[5][4] = {
//呼叫服务图标坐标
{OICON1_Xs,OICON1_Xe,OICON1_Ys,OICON1_Ye},
{OICON2_Xs,OICON2_Xe,OICON2_Ys,OICON2_Ye},
{OICON3_Xs,OICON3_Xe,OICON3_Ys,OICON3_Ye},
{OICON4_Xs,OICON4_Xe,OICON4_Ys,OICON4_Ye},
{OICON5_Xs,OICON5_Xe,OICON5_Ys,OICON5_Ye},
};

const u16 ColorSet_para[58] = 
{ 0x0000,0x2000,0x4000,0x6000,0x8000,0xA000,0xC000,0xE000,
	0xE100,0xE200,0xE300,0xE400,0xE500,0xE600,0xE700,
	0xC700,0xA700,0x8700,0x6700,0x4700,0x2700,0x0700,
	0x0704,0x0708,0x070C,0x0710,0x0714,0x0718,0x071C,
	0x061C,0x051C,0x041C,0x031C,0x021C,0x011C,0x001C,
	0x201C,0x401C,0x601C,0x801C,0xA01C,0xC01C,0xE01C,
	0xE01C,0xE018,0xE014,0xE010,0xE00C,0xE008,0xE004,
	0xE104,0xE208,0xE30C,0xE410,0xE514,0xE618,0xE71C,0xFFFF
};

//主页字段
const u8 Main_Word[6][10] = 
{
	{"本地改名"},{"会议信息"},{"呼叫服务"},{"短信收发"},{"显示人名"},{"系统设置"}
};

//呼叫服务字段
const u8 CallService_Word[8][10] = 
{
	{"咖啡 "},{"茶 "},{"花 "},{"服务员 "},
	{"麦克风 "},{"笔 "},{"纸 "},{"水 "},
};

//呼叫服务字段
const u8 Option_Word[5][10] = 
{
	{"地址设置"},{"更换壁纸"},{"亮度调整"},{"系统状态"},{"WIFI设置"}
};
/************************************************************/


//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,设置默认的时间片长度s
	OSSchedRoundRobinCfg(DEF_ENABLED,10,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//进入临界区

#if Test_task
	OSTaskCreate(	(OS_TCB 	* )&TESTTaskTCB,		
								(CPU_CHAR	* )"TEST task", 		
                (OS_TASK_PTR )TEST_task, 			
                (void		* )0,					
                (OS_PRIO	  )TEST_TASK_PRIO,     
                (CPU_STK   * )&TEST_TASK_STK[0],	
                (CPU_STK_SIZE)TEST_STK_SIZE/10,	
                (CPU_STK_SIZE)TEST_STK_SIZE,		
                (OS_MSG_QTY  )0,					
                (OS_TICK	  )0,					
                (void   	* )0,					
                (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP,
                (OS_ERR 	* )&err);	
#endif
	
	//创建设备系统初始化任务
	OSTaskCreate(	(OS_TCB 	* )&SYS_INITTaskTCB,		
								(CPU_CHAR	* )"System init task", 		
                (OS_TASK_PTR )SYS_INIT_task, 			
                (void		* )0,					
                (OS_PRIO	  )SYS_INIT_TASK_PRIO,     
                (CPU_STK   * )&SYS_INIT_TASK_STK[0],	
                (CPU_STK_SIZE)SYS_INIT_STK_SIZE/10,	
                (CPU_STK_SIZE)SYS_INIT_STK_SIZE,		
                (OS_MSG_QTY  )0,					
                (OS_TICK	  )0,					
                (void   	* )0,					
                (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP,
                (OS_ERR 	* )&err);	
								
								
		//创建触摸屏任务		
	OSTaskCreate(	(OS_TCB 	* )&TP_TaskTCB,		
						(CPU_CHAR	* )"TP_task", 		
						(OS_TASK_PTR )TP_TaskThread, 			
						(void		* )0,					
						(OS_PRIO	  )TP_TASK_PRIO,     
						(CPU_STK   * )&TP_TASK_STK[0],	
						(CPU_STK_SIZE)TP_STK_SIZE/10,	
						(CPU_STK_SIZE)TP_STK_SIZE,		
						(OS_MSG_QTY  )0,			
						(OS_TICK	  )0,					
						(void   	* )0,					
						(OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP,
						(OS_ERR 	* )&err);	
						
	//创建显示屏（内屏）任务		
	OSTaskCreate(	(OS_TCB 	* )&DISPLAYINSIDE_TaskTCB,		
						(CPU_CHAR	* )"DISPLAYINSIDE_task", 		
						(OS_TASK_PTR )DisplayInside_TaskThread, 			
						(void		* )0,					
						(OS_PRIO	  )DISPLAYINSIDE_TASK_PRIO,     
						(CPU_STK   * )&DISPLAYINSIDE_TASK_STK[0],	
						(CPU_STK_SIZE)DISPLAYINSIDE_STK_SIZE/10,	
						(CPU_STK_SIZE)DISPLAYINSIDE_STK_SIZE,		
						(OS_MSG_QTY  )0,			
						(OS_TICK	  )0,					
						(void   	* )0,					
						(OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP,
						(OS_ERR 	* )&err);	
						
	//创建显示屏（外屏）任务		
	OSTaskCreate(	(OS_TCB 	* )&DISPLAYOUTSIDE_TaskTCB,		
						(CPU_CHAR	* )"DISPLAYOUTSIDE_task", 		
						(OS_TASK_PTR )DisplayOutside_TaskThread, 			
						(void		* )0,					
						(OS_PRIO	  )DISPLAYOUTSIDE_TASK_PRIO,     
						(CPU_STK   * )&DISPLAYOUTSIDE_TASK_STK[0],	
						(CPU_STK_SIZE)DISPLAYOUTSIDE_STK_SIZE/10,	
						(CPU_STK_SIZE)DISPLAYOUTSIDE_STK_SIZE,		
						(OS_MSG_QTY  )0,			
						(OS_TICK	  )0,					
						(void   	* )0,					
						(OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP,
						(OS_ERR 	* )&err);	
						
	//主处理任务		
	OSTaskCreate(	(OS_TCB 	* )&MAINPROCESS_TaskTCB,		
						(CPU_CHAR	* )"MAINPROCESS_task", 		
						(OS_TASK_PTR )MainProcess_TaskThread, 			
						(void		* )0,					
						(OS_PRIO	  )MAINPROCESS_TASK_PRIO,     
						(CPU_STK   * )&MAINPROCESS_TASK_STK[0],	
						(CPU_STK_SIZE)MAINPROCESS_STK_SIZE/10,	
						(CPU_STK_SIZE)MAINPROCESS_STK_SIZE,		
						(OS_MSG_QTY  )0,			
						(OS_TICK	  )0,					
						(void   	* )0,					
						(OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP,
						(OS_ERR 	* )&err);	
						
  //功能弹框线程
	OSTaskCreate(	(OS_TCB 	* )&FUNCFRAMEWORK_TaskTCB,		
						(CPU_CHAR	* )"FUNCFRAMEWORK_Task", 		
						(OS_TASK_PTR )FuncFramework_TaskThread, 			
						(void		* )0,					
						(OS_PRIO	  )FUNCFRAMEWORK_TASK_PRIO,     
						(CPU_STK   * )&FUNCFRAMEWORK_TASK_STK[0],	
						(CPU_STK_SIZE)FUNCFRAMEWORK_STK_SIZE/10,	
						(CPU_STK_SIZE)FUNCFRAMEWORK_STK_SIZE,		
						(OS_MSG_QTY  )0,			
						(OS_TICK	  )0,					
						(void   	* )0,					
						(OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP,
						(OS_ERR 	* )&err);	
	
	//WIFI模块状态管理线程
	OSTaskCreate(	(OS_TCB 	* )&WIFISTAMANAGE_TaskTCB,		
						(CPU_CHAR	* )"WIFISTAMANAGE_Task", 		
						(OS_TASK_PTR )WIFI_StaManage_TaskThread, 			
						(void		* )0,					
						(OS_PRIO	  )WIFISTAMANAGE_TASK_PRIO,     
						(CPU_STK   * )&WIFISTAMANAGE_TASK_STK[0],	
						(CPU_STK_SIZE)WIFISTAMANAGE_STK_SIZE/10,	
						(CPU_STK_SIZE)WIFISTAMANAGE_STK_SIZE,		
						(OS_MSG_QTY  )0,			
						(OS_TICK	  )0,					
						(void   	* )0,					
						(OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP,
						(OS_ERR 	* )&err);
						
	//LWIP状态管理线程
	OSTaskCreate(	(OS_TCB 	* )&LWIPSTAMANAGE_TaskTCB,		
						(CPU_CHAR	* )"LWIPSTAMANAGE_Task", 		
						(OS_TASK_PTR )LWIP_StaManage_TaskThread, 			
						(void		* )0,					
						(OS_PRIO	  )LWIPSTAMANAGE_TASK_PRIO,     
						(CPU_STK   * )&LWIPSTAMANAGE_TASK_STK[0],	
						(CPU_STK_SIZE)LWIPSTAMANAGE_STK_SIZE/10,	
						(CPU_STK_SIZE)LWIPSTAMANAGE_STK_SIZE,		
						(OS_MSG_QTY  )0,			
						(OS_TICK	  )0,					
						(void   	* )0,					
						(OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP,
						(OS_ERR 	* )&err);
						
	//网络功能线程
	OSTaskCreate(	(OS_TCB 	* )&NETWORK_TaskTCB,		
						(CPU_CHAR	* )"NETWORK_Task", 		
						(OS_TASK_PTR )NetWork_TaskThread, 			
						(void		* )0,					
						(OS_PRIO	  )NETWORK_TASK_PRIO,     
						(CPU_STK   * )&NETWORK_TASK_STK[0],	
						(CPU_STK_SIZE)NETWORK_STK_SIZE/10,	
						(CPU_STK_SIZE)NETWORK_STK_SIZE,		
						(OS_MSG_QTY  )0,			
						(OS_TICK	  )0,					
						(void   	* )0,					
						(OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP,
						(OS_ERR 	* )&err);
						
	//HTTP请求功能线程
	OSTaskCreate(	(OS_TCB 	* )&HTTP_REQUEST_TaskTCB,		
						(CPU_CHAR	* )"HTTP_REQUEST_Task", 		
						(OS_TASK_PTR )Http_Request_TaskThread, 			
						(void		* )0,					
						(OS_PRIO	  )HTTP_REQUEST_TASK_PRIO,     
						(CPU_STK   * )&HTTP_REQUEST_TASK_STK[0],	
						(CPU_STK_SIZE)HTTP_REQUEST_STK_SIZE/10,	
						(CPU_STK_SIZE)HTTP_REQUEST_STK_SIZE,		
						(OS_MSG_QTY  )0,			
						(OS_TICK	  )0,					
						(void   	* )0,					
						(OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP,
						(OS_ERR 	* )&err);
		
	//创建显示屏控制信号量
	OSMutexCreate((OS_MUTEX*	)&LCDDisplay_MUTEX,
							 (CPU_CHAR*	)"LCDDisplay_MUTEX",
                (OS_ERR*		)&err);
	
//	//创建WIFI控制信号量
//	OSMutexCreate((OS_MUTEX*	)&WIFIControl_MUTEX,
//							 (CPU_CHAR*	)"WIFIControl_MUTEX",
//                (OS_ERR*		)&err);
//						
//	//创建LWIP控制信号量
//	OSMutexCreate((OS_MUTEX*	)&LWIPControl_MUTEX,
//							 (CPU_CHAR*	)"LWIPControl_MUTEX",
//                (OS_ERR*		)&err);
	//创建WIFI控制信号量
	OSMutexCreate((OS_MUTEX*	)&NetWork_MUTEX,
							 (CPU_CHAR*	)"NetWork_MUTEX",
                (OS_ERR*		)&err);

						
						
	//系统软件定时器			1S		
	OSTmrCreate((OS_TMR		*)&SYS_Timer_1S,		//定时器1
                (CPU_CHAR	*)"SYS_Timer_1S",		//定时器名字
                (OS_TICK	 )0,		
                (OS_TICK	 )100,          //1S
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //周期模式
                (OS_TMR_CALLBACK_PTR)SYS_Timer_1S_Callback,//定时器1回调函数
                (void	    *)0,			//参数为0
                (OS_ERR	    *)&err);		//返回的错误码		
								
//系统软件定时器			500ms		
	OSTmrCreate((OS_TMR		*)&SYS_Timer_500MS,		//定时器2
                (CPU_CHAR	*)"SYS_Timer_500MS",		//定时器名字
                (OS_TICK	 )0,		
                (OS_TICK	 )50,          //500ms
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //周期模式
                (OS_TMR_CALLBACK_PTR)SYS_Timer_500MS_Callback,//定时器1回调函数
                (void	    *)0,			//参数为0
                (OS_ERR	    *)&err);		//返回的错误码										

//系统软件定时器			100ms		
	OSTmrCreate((OS_TMR		*)&SYS_Timer_100MS,		//定时器2
                (CPU_CHAR	*)"SYS_Timer_100MS",		//定时器名字
                (OS_TICK	 )0,		
                (OS_TICK	 )10,          //100ms
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //周期模式
                (OS_TMR_CALLBACK_PTR)SYS_Timer_100MS_Callback,//定时器1回调函数
                (void	    *)0,			//参数为0
                (OS_ERR	    *)&err);		//返回的错误码										


								
				 

	OS_CRITICAL_EXIT();	//退出临界区			
	OSTmrStart(&SYS_Timer_500MS,&err);								
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务			 
}


/*********************测试任务函数**********************************/
#if Test_task


void TEST_task(void *p_arg)
{
	OS_ERR err;
	
	OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);
	OSTimeDlyHMSM(0,0,3,50,OS_OPT_TIME_HMSM_STRICT,&err);
	debug("TEST task start ...\r\n");
	UART1.Rx_Complete = False;
//	Print_MEM_POOL(SRAMEX);
//	Print_MEM_POOL(SRAMIN);
//	debug("SRAMIN usage rate = %u%%\r\n",my_mem_perused(SRAMIN)/10);
//	debug("SRAMEX usage rate = %u%%\r\n",my_mem_perused(SRAMEX)/10);
	while(1)
	{
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
		if(UART1.Rx_Complete)
		{
			UART1.Rx_Complete = False;
//			printf("u1:%s\r\n",UART1.Rx_Buffer);
			
			Print_MEM_POOL(SRAMEX);
			Print_MEM_POOL(SRAMIN);
			debug("SRAMIN usage rate = %u%%\r\n",my_mem_perused(SRAMIN)/10);
			debug("SRAMEX usage rate = %u%%\r\n",my_mem_perused(SRAMEX)/10);
		}
		
	}
}	
#endif


/*************设备系统初始函数********************************/
void SYS_INIT_task(void *p_arg)
{
	OS_ERR err;
//	CPU_SR_ALLOC();
	
	u8 res;
//	u16 i,j;
	u8(*fcpymsg)(u8*pname,u8 pct,u8 mode);
	u8 *Spath,*Tpath;
//	u8 *Ipath,*welword;
//	OS_CRITICAL_ENTER();	//进入临界区
	
	_BOOL Font_updata = False,UI_updata = False;

	SDRAM_Init();                   //初始化SDRAM
	LCD.initialization();                     //初始化LCD
	tp_dev.init();				    //触摸屏初始化 
	RTC_Init();
	my_mem_init(SRAMIN);		//初始化内部内存池
	my_mem_init(SRAMEX);		//初始化外部内存池
	my_mem_init(SRAMCCM);		//初始化CCM内存池 
	exfuns_init();				//为fatfs相关变量申请内存 
#if ApolloDev
	PCF8574_Init(); 
#endif

  SYS_MALLOC(SYS,SRAMIN,sizeof(_sys_para_class));

	GetCompileDateTime(SYS->CompileDate,SYS->CompileTime);
	SYS->MCU_UID = *(vu32*)(0x1FFF7A10);
	
	printf("\r\n\r\nTS-8209A is starting...\r\nMCU UID:%d\r\nCode compile date:%s\r\nCode compile time:%s\r\nCode version:%s\r\n\r\n",\
					SYS->MCU_UID,SYS->CompileDate,SYS->CompileTime,Version);

	W25QXX_Init();				//初始化W25Q256
	res = TEXT.font_check();
	if(res) 	{ debug("font check fail : %d \r\n",res);    SYS->Font_INIT = False; }    //字库检测
	else			{ debug("font check sucess\r\n");						SYS->Font_INIT = True;  }
	
	

#ifdef SD_CARD_ENABLE
	res = f_mount(fs[0],"0:",1); 		//挂载SD卡 
	
	if(res) 	{ debug("SD card filesystem mount fail : %d \r\n",res);	SYS->SD_CARD_INIT = False; }
	else	  	{	debug("SD card filesystem mount sucess \r\n");				SYS->SD_CARD_INIT = True;  }
#endif
	res = f_mount(fs[1],"1:",1); 		//挂载FLASH.
	if(res) 	{	debug("FLASH filesystem mount fail : %d \r\n",res);		SYS->FLASH_INIT = False; }
	else	  	{	debug("FLASH filesystem mount sucess \r\n");					SYS->FLASH_INIT = True;  }
	


	res = mf_FatfsCheck();
	if(res)		{	debug("Fatfs check fail : %d \r\n",res);		SYS->Fatfs_INIT = False; }
	else 			{	debug("Fatfs check sucess\r\n");						SYS->Fatfs_INIT = True;  }
	
	
	W25QXX_Read(&res,FatfsUDFlag_addr,1);
	if(res != FatfsUDFlag) SYS->Fatfs_INIT = False;
		/**************手动设置升级标志位***********************/
	
	W25QXX_Read(&res,UIUpdataFlag_addr,1);
	if(res != UIUpdataFlag)
	{
		UI_updata = True;
		res = UIUpdataFlag;
		W25QXX_Write(&res,UIUpdataFlag_addr,1); //标志位马上回写，无论是否完成升级，表示只进行一次升级
	}
	
	W25QXX_Read(&res,FontUpdataFlag_addr,1);
	if(res != FontUpdataFlag)
	{
		Font_updata = True;
		res = FontUpdataFlag;
		W25QXX_Write(&res,FontUpdataFlag_addr,1);
	}
	
//文件系统、字库、FLASH初始化失败
	if((!(SYS->Fatfs_INIT && SYS->FLASH_INIT && SYS->Font_INIT)) || Font_updata ||  UI_updata ) 
	{
		OTG_ENABLE  = 1;
		CHARGING_DISABLE = 1;
		res = 1;
		LCD.BackLightSet(100);
		POINT_COLOR=WHITE;
		
		
		
		if(!SYS->FLASH_INIT) 
		{
			if(!SYS->Font_INIT) TEXT.show_str_mid(1,res,(u8 *)"FLASH initialization failed,Please check the device.",16,1);
			else							 TEXT.show_str_mid(1,res,"储存设备初始化失败请检查设备",24,1);
			res += 30;
			mf_fmkfs("1:",0,512);
			while(1)
			{
				HAL_NVIC_SystemReset();//重启函数HAL_NVIC_SystemReset();
				OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);	
				debug("reboot\r\n");
			}
		}
		
		if(!SYS->Font_INIT)
		{
			TEXT.show_str_mid(1,res,"Font check failed,please insert the usb dise for update the font.",16,1);
			res += 30;
		}
		
	   if(!SYS->Fatfs_INIT) 
		{
			if(!SYS->Font_INIT) TEXT.show_str_mid(1,res,"File system initialization failed,please insert the usb dise for update the FS.",16,1);
			else							 TEXT.show_str_mid(1,res,"文件系统初始化失败请插入U盘更新文件系统",24,1);
		}
		
		
		if(!SYS->Font_INIT) TEXT.show_str_mid(1,240,"Please insert the usb dise!",24,800);
		else 								TEXT.show_str_mid(1,240,"请插入U盘",24,800);
		
		
		USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_cb);

		res = 0;
		while(!usb_connect)
		{
			USBH_Process(&USB_OTG_Core, &USB_Host);
			OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_HMSM_STRICT,&err);
			res ++;
			if(res > 250 )
			{
				if(USB_Host.gState >= HOST_ENUMERATION)
				{
					LCD.Fill(1,240,800,270,BLACK);
					res = 0;
					USB_Host.gState = HOST_IDLE;
					OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
					USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_cb);
					if(!SYS->Font_INIT) TEXT.show_str_mid(1,240,"Reading the usb dise!",24,800);
					else 								TEXT.show_str_mid(1,240,"正在读取...",24,800);					
				}
			}
		}
		debug("res = %d\r\n",res);
		res = f_mount(fs[3],"3:",1);
		debug("USB mount %d\r\n",res);
		LCD.Fill(1,240,800,270,BLACK);
		if(!SYS->Font_INIT) TEXT.show_str_mid(1,240,"It's connected the usb dise!",24,800);
		else 								TEXT.show_str_mid(1,240,"已连接U盘",24,800);
		
		LCD.Clear(BLACK);
		if(!SYS->Font_INIT)
		{
			
			TEXT.unigbk_updata();
			debug("unigbk_updata\r\n");
			TEXT.font_library_updata_fon(Font24);
			debug("Font24\r\n");
			TEXT.font_library_updata_fon(Font32);
			debug("Font32\r\n");
			TEXT.font_library_updata_fon(Font64);
			debug("Font64\r\n");
			res = FontUDFlag;
			W25QXX_Write(&res,FontUDFlag_addr,1);
		}
		
		LCD.Clear(BLACK);
		
		if(!SYS->Fatfs_INIT)
		{
//			Spath = mymalloc(SRAMIN,50);
//			Tpath = mymalloc(SRAMIN,50);
			SYS_MALLOC(Spath,SRAMIN,50);
			SYS_MALLOC(Tpath,SRAMIN,50);
			
			if(!(Spath && Tpath))	debug("malloc spath or tpath fail\r\n");
			
			TEXT.show_str_mid(1,240,"正在初始化文件系统",24,800);
			debug("initing system file\r\n");
			res = mf_fmkfs("1:",0,512);
			if(res) debug("formatting fail :%d\r\n",res);
			res=f_mkdir((const TCHAR*)"1:/BACKGROUND");//如果文件夹已经存在,就不创建.如果不存在就创建新的文件夹.
			res=f_mkdir((const TCHAR*)"1:/ICON");
			res=f_mkdir((const TCHAR*)"1:/NAMEPIC");
			
//			OS_CRITICAL_ENTER();	//进入临界区
			for(res = 0;res < 32;res ++)
			{	
				strcpy((char*)Spath,"3:/");
				strcat((char*)Spath,(const char*)Def_PicPath[res]);
				strcpy((char*)Tpath,"1:/");
				strcat((char*)Tpath,(const char*)Def_PicPath[res]);
				if(exf_copy(fcpymsg,Spath,Tpath,0,0,0)) debug("file %s copy fail\r\n",Tpath);
				LCD.Fill(1,280,800,300,BLACK);
				TEXT.show_str_mid(1,280,Tpath,16,800);
				debug("%s\r\n",Tpath);		
			}
//			OS_CRITICAL_EXIT();	
			
			LCD.Fill(1,240,800,270,BLACK);
			TEXT.show_str_mid(1,240,"初始化文件系统完成",24,800);
			debug("ini system file complete\r\n");
			res = FatfsUDFlag;
			W25QXX_Write(&res,FatfsUDFlag_addr,1);
			
//			myfree(SRAMIN,Spath);
//			myfree(SRAMIN,Tpath);
			
			SYS_FREE(Spath,SRAMIN);
			SYS_FREE(Tpath,SRAMIN);
			
			OSTimeDlyHMSM(0,0,1,500,OS_OPT_TIME_HMSM_STRICT,&err);
		}
		HAL_NVIC_SystemReset();
	}
		
	/*****************系统参数初始化*************************************/
	SYSPara_SaveNLoad(Load,0);
	W25QXX_Read(&res,SYSParaUDFlag_addr,1);
	if(res != SYSParaUDFlag)
	{
		debug("System parameter initialization!!\r\n");
		strcpy((char*)SYS->Name_Person,"电子桌牌");
		strcpy((char*)SYS->Name_Position,"您好");
		strcpy((char*)SYS->Name_Company,"欢迎使用");
		SYS->Name_PerColor = YELLOW;
		SYS->Name_PosColor = YELLOW;
		SYS->Name_ComColor = YELLOW;
		SYS->OuterBG_Color = RED;

		SYS->OuterBG_JPG = True;
		SYSPara_SaveNLoad(Save,0);
		res = SYSParaUDFlag;
		W25QXX_Write(&res,SYSParaUDFlag_addr,1);
	}

	
	if(SYS->Device_UID < 1 || SYS->Device_UID > MaxDeviceID) 
	{
		SYS->Device_UID = 1;
		SYSPara_SaveNLoad(Save,1);
	}
	SYS->Menu.Page = MainPage; //设置显示
	
	SYS->WIFI_SignalSt = 0;
//	SYS->ElectricQua = 0;
	SYS->Luminance = 50;
	SYS->O_Luminance = 75;
	
	SYS->SerID = 0;
	SYS->MeetingID = 0;
	SYS->Device_ShutDown = False;
	
//	if(!strstr((const char*)SYS->InnerJPG_Path,BG_Path))
//	{
		strcpy((char*)SYS->InnerJPG_Path,"1:/BACKGROUND/background.jpg");
		debug("Set default background path %s\r\n",SYS->InnerJPG_Path);
		SYSPara_SaveNLoad(Save,3);
//	}
	/*********************************************************************/
	
		
	LCD.SelectScreen(ScreenOutside);
	UI.Config();
	LCD.SelectScreen(ScreenInside);
//	
	DeviceSta_Get(1);
	
//	GetCompileDateTime(StrDateTime);
//	debug("GetDateTime:%s\r\n",StrDateTime);
	
	//发送信号量启动任务
	OSTaskSemPost(&TP_TaskTCB,OS_OPT_POST_NONE, &err);
	OSTaskSemPost(&DISPLAYINSIDE_TaskTCB,OS_OPT_POST_NONE, &err);
	OSTaskSemPost(&MAINPROCESS_TaskTCB,OS_OPT_POST_NONE, &err);
	OSTaskSemPost(&DISPLAYOUTSIDE_TaskTCB,OS_OPT_POST_NONE, &err);
	OSTaskSemPost(&WIFISTAMANAGE_TaskTCB,OS_OPT_POST_NONE, &err);
	OSTaskSemPost(&NETWORK_TaskTCB,OS_OPT_POST_NONE, &err);
	OSTaskSemPost(&LWIPSTAMANAGE_TaskTCB,OS_OPT_POST_NONE, &err);
	OSTaskSemPost(&FUNCFRAMEWORK_TaskTCB,OS_OPT_POST_NONE, &err);
	OSTaskSemPost(&HTTP_REQUEST_TaskTCB,OS_OPT_POST_NONE, &err);
	
#if Test_task
	OSTaskSemPost(&TESTTaskTCB,OS_OPT_POST_NONE,&err);
#endif
	//开启软件定时器
		OSTmrStart(&SYS_Timer_1S,&err);	 
		OSTmrStart(&SYS_Timer_100MS,&err);	
	
	OS_TaskSuspend((OS_TCB*)&SYS_INITTaskTCB,&err);		//挂起初始化任务

}
/***********************************************************/


/*******************触摸屏扫描任务**************************/
void TP_TaskThread(void *p_arg)
{
	OS_ERR err;
	
	_tp_sta Sta_temp;
	u16 CoX_temp,CoY_temp;
//	u8 backlight_temp;
	

	SYS->TP.StaUpdata = False;
	SYS->TP.CoUpdata = False;
	SYS->TP.Status = Loosen;
	SYS->TP.Coordinate_X = 0xFFFF;
	SYS->TP.Coordinate_Y = 0xFFFF;
	SYS->TP.BeforeCo_X = 0xFFFF;
	SYS->TP.BeforeCo_Y = 0xFFFF;
	
	Sta_temp = SYS->TP.Status;
	CoX_temp = SYS->TP.Coordinate_X;
	CoY_temp = SYS->TP.Coordinate_Y;
	
	SYS->dim_bl = 0;
	
	OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);

	debug("TP_TaskThread start ...\r\n");
	
	while(1)
	{
		OSTimeDlyHMSM(0,0,0,80,OS_OPT_TIME_HMSM_STRICT,&err);
		
			tp_dev.scan(0);
			if(((tp_dev.sta)&0x1F) != 1)
			{
				tp_dev.sta &= 0xE0;
				SYS->TP.Status = Loosen;
				SYS->TP.Coordinate_X = 0xFFFF;
				SYS->TP.Coordinate_Y = 0xFFFF;
			}
			else if(((tp_dev.sta) & 0x1F) == 1)
			{
				if(tp_dev.x[0]<LCD.width && tp_dev.x[0]>0 && tp_dev.y[0]<LCD.height && tp_dev.y[0] > 0)
				{	
//					if(tp_dev.x[0] != CoX_temp || tp_dev.y[0] != CoY_temp){
						SYS->dim_bl = 0;	
						SYS->TP.Coordinate_X = tp_dev.x[0];
						SYS->TP.Coordinate_Y = tp_dev.y[0];
						SYS->TP.Status = Press;
						LCD.BackLightSet(SYS->Luminance);
//					}
					
				}
			}	
			
			if(SYS->TP.Status != Sta_temp)  SYS->TP.StaUpdata = True;
			else  SYS->TP.StaUpdata = False;
			if(SYS->TP.Coordinate_X != CoX_temp || SYS->TP.Coordinate_Y != CoY_temp) SYS->TP.CoUpdata = True;
			else  SYS->TP.CoUpdata = False;
			
#if TP_DEBUG
			if(SYS->TP.StaUpdata || SYS->TP.CoUpdata) 
			{
				debug("SYS->TP.StaUpdata:%s   SYS->TP.CoUpdata:%s\r\n",SYS->TP.StaUpdata?"true":"false",SYS->TP.CoUpdata?"true":"false");
//				debug("x:%d   y:%d\r\n",SYS->TP.Coordinate_X,SYS->TP.Coordinate_Y);
//				debug("CoX_temp:%d   CoY_temp:%d\r\n",CoX_temp,CoY_temp);
			}
#endif	
			
						Sta_temp = SYS->TP.Status;
						CoX_temp = SYS->TP.Coordinate_X;
						CoY_temp = SYS->TP.Coordinate_Y;			

	}
}
/**********************************************************************/

/*******************屏幕（内屏）显示任务**************************/
void DisplayInside_TaskThread(void *p_arg)
{
	OS_ERR err;
	
	CPU_SR_ALLOC();

	u8 res = 0,ButtonO_temp=0;
	char *IDstr_temp = NULL;
	u16 *page_cache = NULL;
	u16 i,j;
	
	char time_str[25] = {'\0'};
	char weekday_str[5] = {'\0'};
	

	SYS->Menu.Page = MainPage;
	
	OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);
	debug("DisplayInside_TaskThread start...\r\n");
	
	OSMutexPend (&LCDDisplay_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);
	UI.SetBackGround(&Main_BackGround,0,0,800,480);
	UI.SetTextBox(&MainPageHeading,(char*)SYS->Name_Person,0,0);
//	UI.SetFuncBlock(&Calendar,NULL);
//	UI.SetTextBox(&CalendarDate,SYS->TimeDate.YearStr,Calendar.Position_X,Calendar.Position_Y);
//	UI.SetTextBox(&CalendarDate,SYS->TimeDate.MonthDateStr,Calendar.Position_X,Calendar.Position_Y+50);
//	UI.SetTextBox(&CalendarDate,SYS->TimeDate.WeekDayStrCN,Calendar.Position_X,Calendar.Position_Y+100);
	UI.SetFuncBlock(&StatusBar,NULL);
	UI.SetButton(&Button_Rename,UNCLICK);
	UI.SetButton(&Button_MeetMsg,UNCLICK);
	UI.SetButton(&Button_Service,UNCLICK);
	UI.SetButton(&Button_SMS,UNCLICK);
	UI.SetButton(&Button_ShowName,UNCLICK);
	UI.SetButton(&Button_Setting,UNCLICK);
	
	UI.BacklightGradient(0,SYS->Luminance,5);
	SYS->DisplayInside_SET = 1;
	OSMutexPost(&LCDDisplay_MUTEX,OS_OPT_POST_NONE,&err);
	

	while(1)
	{
		OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);
		if(SYS->TP.StaUpdata || SYS->TP.CoUpdata)
		{
			OSMutexPend (&LCDDisplay_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);
			if(LCD.LCD_Sel != ScreenInside) LCD.SelectScreen(ScreenInside);
			
			switch(SYS->Menu.Page)
			{
				case MainPage:
				{
					SYS->MenuOperat.Page = SYS->Menu.Page;
					res = UI.ButtonLibTouch(MainButtonLib,SYS->TP.Coordinate_X,SYS->TP.Coordinate_Y,4,6);
					if(res)
					{
						SYS->Menu.Page = (_menu_page)(res+1);
						SYS->Menu.PageSta[Lay1] = 0;
						SYS->Menu.PageUpdata = True;
					}
					
				}break;
				case Rename:
				{
					//返回键
					SYS->MenuOperat.Page = SYS->Menu.Page;
					if(UI.ButtonTouch(&ButtonM_Back,SYS->TP.Coordinate_X,SYS->TP.Coordinate_Y,0)){
								SYS->MenuOperat.PageSta[Lay1]= 0xFF;
								SYS->MenuOperat.PageUpdata = True;
								
								SYS->Menu.Page = MainPage;
								SYS->Menu.PageSta[Lay1] = 0;
								SYS->Menu.PageUpdata = True;
					}
					
					//本地改名功能按钮
					res = UI.ButtonLibTouch(Button_RenameLib,SYS->TP.Coordinate_X,SYS->TP.Coordinate_Y,4,9);
					if(res>=1 && res <= 9)
					{
						SYS->MenuOperat.PageSta[Lay1] = res;
						SYS->MenuOperat.PageUpdata = True;
					}
				}break;
				case MeetInfo:
				{
					//返回键
					SYS->MenuOperat.Page = SYS->Menu.Page;
					if(UI.ButtonTouch(&ButtonM_Back,SYS->TP.Coordinate_X,SYS->TP.Coordinate_Y,0)){
								SYS->MenuOperat.PageSta[Lay1]= 0xFF;
								SYS->MenuOperat.PageUpdata = True;
								
								SYS->Menu.Page = MainPage;
								SYS->Menu.PageSta[Lay1] = 0;
								SYS->Menu.PageUpdata = True;
					}
					
/*					res = UI.ButtonLibTouch(Button_M_Lib,SYS->TP.Coordinate_X,SYS->TP.Coordinate_Y,4,4);
					if(res)
					{
						switch(res)
						{
							case 1:
							case 4:
							{
								SYS->MenuOperat.PageSta[Lay1]= 0xFF;
								SYS->MenuOperat.PageUpdata = True;
								
								SYS->Menu.Page = MainPage;
								SYS->Menu.PageSta[Lay1] = 0;
								SYS->Menu.PageUpdata = True;
							}break;
							case 2:
							{
								SYS->MenuOperat.PageSta[Lay1]= 0xF2;
								SYS->MenuOperat.PageUpdata = True;
								
								SYS->Menu.Page = ShowName;
								SYS->Menu.PageSta[Lay1] = 0;
								SYS->Menu.PageUpdata = True;
							}break;
							case 3:
							{
								SYS->MenuOperat.PageSta[Lay1]= 0xF3;
								SYS->MenuOperat.PageUpdata = True;
								
								SYS->Menu.Page = MeetInfo;
								SYS->Menu.PageSta[Lay1] = 0;
								SYS->Menu.PageUpdata = True;

							}break;
							default:break;
						}
						break;
					}*/
				}break;
				case CallService:
				{
					//返回键
					SYS->MenuOperat.Page = SYS->Menu.Page;
					if(UI.ButtonTouch(&ButtonM_Back,SYS->TP.Coordinate_X,SYS->TP.Coordinate_Y,0)){
								SYS->MenuOperat.PageSta[Lay1]= 0xFF;
								SYS->MenuOperat.PageUpdata = True;
								
								SYS->Menu.Page = MainPage;
								SYS->Menu.PageSta[Lay1] = 0;
								SYS->Menu.PageUpdata = True;
					}
/*					res = UI.ButtonLibTouch(Button_M_Lib,SYS->TP.Coordinate_X,SYS->TP.Coordinate_Y,4,4);
					if(res)
					{
						switch(res)
						{
							case 1:
							case 4:
							{
								SYS->MenuOperat.PageSta[Lay1]= 0xFF;
								SYS->MenuOperat.PageUpdata = True;
								
								SYS->Menu.Page = MainPage;
								SYS->Menu.PageSta[Lay1] = 0;
								SYS->Menu.PageUpdata = True;
							}break;
							case 2:
							{
								SYS->MenuOperat.PageSta[Lay1]= 0xF2;
								SYS->MenuOperat.PageUpdata = True;
								
								SYS->Menu.Page = ShowName;
								SYS->Menu.PageSta[Lay1] = 0;
								SYS->Menu.PageUpdata = True;
							}break;
							case 3:
							{
								SYS->MenuOperat.PageSta[Lay1]= 0xF3;
								SYS->MenuOperat.PageUpdata = True;
								
								SYS->Menu.Page = MeetInfo;
								SYS->Menu.PageSta[Lay1] = 0;
								SYS->Menu.PageUpdata = True;

							}break;
							default:break;
						}
						break;
					}*/
					
					//服务选择
					res = UI.ButtonLibTouch(ServiceButtonLib,SYS->TP.Coordinate_X,SYS->TP.Coordinate_Y,3,8);
					if(res >= 1 && res <= 8)
					{
						SYS->MenuOperat.PageSta[Lay1] = res;
						SYS->MenuOperat.PageUpdata = True;
						break;
					}
//					else if(res == 10)
//					{
//						SYS->MenuOperat.PageSta[Lay1] = res;
////						SYS->MenuOperat.PageUpdata = T;
//						for(i=0;i<8;i++)
//						{
//							if(!ServiceButtonLib[i]->Sta)  SYS->MenuOperat.Para |= 1<<i;
//						}
//						SYS->MenuOperat.PageUpdata = True;
//					}
				}break;
				case SMS:
				{
					//返回键
					SYS->MenuOperat.Page = SYS->Menu.Page;
					if(UI.ButtonTouch(&ButtonM_Back,SYS->TP.Coordinate_X,SYS->TP.Coordinate_Y,0)){
								SYS->MenuOperat.PageSta[Lay1]= 0xFF;
								SYS->MenuOperat.PageUpdata = True;
								
								SYS->Menu.Page = MainPage;
								SYS->Menu.PageSta[Lay1] = 0;
								SYS->Menu.PageUpdata = True;
					}

					
					//短信
					res = UI.ButtonLibTouch(Button_SMSLib,SYS->TP.Coordinate_X,SYS->TP.Coordinate_Y,4,3);
					if(res>=1&&res<=3)
					{
						SYS->MenuOperat.PageSta[Lay1] = res;
						SYS->MenuOperat.PageUpdata = True;
						break;
					}
				}break;
				case ShowName:
				{
					SYS->Menu.Page = MainPage;
					SYS->Menu.PageSta[Lay1] = 0;
					SYS->Menu.PageUpdata = True;
					UI.SetBackGround(&Main_BackGround,0,0,800,480);
				}break;
				case Setting:
				{
					//返回键
					SYS->MenuOperat.Page = SYS->Menu.Page;
					if(UI.ButtonTouch(&ButtonM_Back,SYS->TP.Coordinate_X,SYS->TP.Coordinate_Y,0)){
								SYS->MenuOperat.PageSta[Lay1]= 0xFF;
								SYS->MenuOperat.PageUpdata = True;
								
								SYS->Menu.Page = MainPage;
								SYS->Menu.PageSta[Lay1] = 0;
								SYS->Menu.PageUpdata = True;
					}
/*					res = UI.ButtonLibTouch(Button_M_Lib,SYS->TP.Coordinate_X,SYS->TP.Coordinate_Y,4,4);
					if(res)
					{
						switch(res)
						{
							case 1:
							case 4:
							{
								SYS->MenuOperat.PageSta[Lay1]= 0xFF;
								SYS->MenuOperat.PageUpdata = True;
								
								SYS->Menu.Page = MainPage;
								SYS->Menu.PageSta[Lay1] = 0;
								SYS->Menu.PageUpdata = True;
							}break;
							case 2:
							{
								SYS->MenuOperat.PageSta[Lay1]= 0xF2;
								SYS->MenuOperat.PageUpdata = True;
								
								SYS->Menu.Page = ShowName;
								SYS->Menu.PageSta[Lay1] = 0;
								SYS->Menu.PageUpdata = True;
							}break;
							case 3:
							{
								SYS->MenuOperat.PageSta[Lay1]= 0xF3;
								SYS->MenuOperat.PageUpdata = True;
								
								SYS->Menu.Page = MeetInfo;
								SYS->Menu.PageSta[Lay1] = 0;
								SYS->Menu.PageUpdata = True;

							}break;
							default:break;
						}
						break;
					}*/
					
					//设置菜单分页选择
/*					res = UI.ButtonLibTouch(OptionButtonLib,SYS->TP.Coordinate_X,SYS->TP.Coordinate_Y,2,3);
					if(res)
					{
						if(res != ButtonO_temp)
						{
							if(ButtonO_temp != 0 && ButtonO_temp <= 3) UI.SetButton(OptionButtonLib[ButtonO_temp-1],UNCLICK);
							ButtonO_temp = res;
							
							SYS->Menu.Page = Setting;
							SYS->Menu.PageSta[Lay1] = res;
							SYS->Menu.PageUpdata = True;
							
							if(SYS->Menu.PageSta[Lay1] == 2)
							{
								SYS->MenuOperat.PageSta[Lay1] = 1;
								SYS->MenuOperat.PageUpdata = True;
							}
						}
						break;
					}*/
					
					//设置分页功能
					switch(SYS->Menu.PageSta[Lay1])
					{
						case 1:
						{
							res = UI.ButtonLibTouch(OptionAddressSet_ButtonLib,SYS->TP.Coordinate_X,SYS->TP.Coordinate_Y,4,4);
							if(res>=1 && res<=4)
							{
								SYS->MenuOperat.PageSta[Lay1] = res;
								SYS->MenuOperat.PageUpdata = True;
							}
							
							if(UI.ButtonTouch(&Button_Option_DHCP,SYS->TP.Coordinate_X,SYS->TP.Coordinate_Y,3))
							{
								SYS->MenuOperat.PageSta[Lay1] = 5;
								SYS->MenuOperat.PageUpdata = True;
								SYS->MenuOperat.Para = !Button_Option_DHCP.Sta;
							}
							
						}break;
//						case 2:
//						{
////							SYS->MenuOperat.PageSta[Lay1] = 1;
////							SYS->Menu.OperatUpdata = True;
//						}break;
						case 2:
						{
							UI.SliderTouch(&INScreenBrightness,SYS->TP.Coordinate_X,SYS->TP.Coordinate_Y,1,&SYS->Luminance);
//							UI.SliderTouch(&OUTScreenBrightness,SYS->TP.Coordinate_X,SYS->TP.Coordinate_Y,1,&SYS->O_Luminance);
							
						}break;
//						case 4:
//						{
//							
//						}break;
						case 3:
						{
							res = UI.ButtonLibTouch(OptionWIFISet_ButtonLib,SYS->TP.Coordinate_X,SYS->TP.Coordinate_Y,4,2);
							if(res>=1 && res<=2)
							{
								SYS->MenuOperat.PageSta[Lay1] = res;
								SYS->MenuOperat.PageUpdata = True;
							}
						}break;
						default:break;
						
					}
				}break;
				case Updating:
				{	
				}break;
				default:break;
			}
//			while(SYS->TP.Status == Press) OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);
			SYS->TP.StaUpdata = False;
			SYS->TP.CoUpdata = False;
			OSMutexPost(&LCDDisplay_MUTEX,OS_OPT_POST_NONE,&err);
		}
		
		//页面更新
		if(SYS->Menu.PageUpdata)
		{
			OSMutexPend (&LCDDisplay_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);
			if(LCD.LCD_Sel != ScreenInside) LCD.SelectScreen(ScreenInside);
			
			switch(SYS->Menu.Page)
			{
				case MainPage:
				{
					SYS->DisplayInside_SET = 1;
					UI.SetBackGround(&Main_BackGround,0,20,800,480);
					UI.SetTextBox(&MainPageHeading,(char*)SYS->Name_Person,0,0);
					
//					UI.SetFuncBlock(&Calendar,NULL);
//					UI.SetTextBox(&CalendarDate,SYS->TimeDate.YearStr,Calendar.Position_X,Calendar.Position_Y);
//					UI.SetTextBox(&CalendarDate,SYS->TimeDate.MonthDateStr,Calendar.Position_X,Calendar.Position_Y+50);
//					UI.SetTextBox(&CalendarDate,SYS->TimeDate.WeekDayStrCN,Calendar.Position_X,Calendar.Position_Y+100);
					
//					UI.SetFuncBlock(&StatusBar,NULL);
					
				  UI.SetButton(&Button_Rename,UNCLICK);
					UI.SetButton(&Button_MeetMsg,UNCLICK);
					UI.SetButton(&Button_Service,UNCLICK);
					UI.SetButton(&Button_SMS,UNCLICK);
					UI.SetButton(&Button_ShowName,UNCLICK);
					UI.SetButton(&Button_Setting,UNCLICK);
				}break;
				case Rename:
				{
					SYS->DisplayInside_SET = 1;
					UI.SetBackGround(&Main_BackGround,0,20,800,480);
					
					UI.SetTextBox(&SecPageHeading,"本地改名",NULL,NULL);
					
					UI.SetFuncBlock(&Rename_Blackboard,NULL);
					
					UI.SetTextBox(&Rename_InfoHeading,"姓名",76,121);
					UI.SetTextBox(&Rename_InfoHeading,"单位",76,192);
					UI.SetTextBox(&Rename_InfoHeading,"职位",76,261);
					
					UI.SetButton(&Button_Rename_PerName,UNCLICK);
					UI.SetButton(&Button_Rename_PerCom,UNCLICK);
					UI.SetButton(&Button_Rename_PerPos,UNCLICK);
					
					UI.SetTextBox(&Rename_InfoHeading,"颜色",582,122);
					UI.SetTextBox(&Rename_InfoHeading,"颜色",582,192);
					UI.SetTextBox(&Rename_InfoHeading,"颜色",582,261);
					UI.SetTextBox(&Rename_SetBGColor,"背景颜色",535,331);
					
					Button_Rename_PerNameColor.MainColor = SYS->Name_PerColor;
					Button_Rename_PerNameColor.MainColorPress = SYS->Name_PerColor;
					Button_Rename_PerComColor.MainColor = SYS->Name_ComColor;
					Button_Rename_PerComColor.MainColorPress = SYS->Name_ComColor;
					Button_Rename_PerPosColor.MainColor = SYS->Name_PosColor;
					Button_Rename_PerPosColor.MainColorPress = SYS->Name_PosColor;
					Button_Rename_SetBackGroundColor.MainColor = SYS->OuterBG_Color;
					Button_Rename_SetBackGroundColor.MainColorPress = SYS->OuterBG_Color;
					
					UI.SetButton(&Button_Rename_PerNameColor,UNCLICK);
					UI.SetButton(&Button_Rename_PerComColor,UNCLICK);
					UI.SetButton(&Button_Rename_PerPosColor,UNCLICK);
					UI.SetButton(&Button_Rename_SetBackGroundColor,UNCLICK);
					
					UI.SetButton(&Button_Rename_SetPicNameCar,UNCLICK);
					UI.SetButton(&Button_Rename_Confirm,UNCLICK);
					
//					UI.SetButton(&ButtonM_Home,UNCLICK);
//					UI.SetButton(&ButtonM_ShowName,UNCLICK);
//					UI.SetButton(&ButtonM_SysMsg,UNCLICK);
					UI.SetButton(&ButtonM_Back,UNCLICK);
				}break;
				case MeetInfo:
				{
					UI.SetBackGround(&Main_BackGround,0,20,800,480);
					SYS->DisplayInside_SET = 1;
					UI.SetTextBox(&SecPageHeading,"会议信息",NULL,NULL);
//					UI.SetFuncBlock(&Msg_Blackboard,NULL);
					
					if(SYS->RecMsgContNum > 0) 
					{
						if(SYS->RecMsgList[SYS->RecMsgContNum-1]->RecMsg_ID == 0)
						{
							UI.SetTextBox(&MeetMsg_RecMsgTitle,"系统通知",NULL,NULL);
						}
						else
						{
							if(SYS->UserList) 
							{
								for(i=0;i<SYS->UserNum;i++)
								{
									if(SYS->UserList[i]._lstUserID == SYS->RecMsgList[SYS->RecMsgContNum-1]->RecMsg_ID)
									{
										UI.SetTextBox(&MeetMsg_RecMsgTitle,SYS->UserList[i]._lstUserName,NULL,NULL);
										break;
									}
								}
								
								if(i == SYS->UserNum)
								{
									SYS->GetUserList = True;
									sprintf(IDstr_temp,"设备编号=%d",SYS->RecMsgList[SYS->RecMsgContNum-1]->RecMsg_ID);
									UI.SetTextBox(&MeetMsg_RecMsgTitle,IDstr_temp,NULL,NULL);
								}
							}
							else 
							{
								SYS->GetUserList = True;
								sprintf(IDstr_temp,"设备编号=%d",SYS->RecMsgList[SYS->RecMsgContNum-1]->RecMsg_ID);
								UI.SetTextBox(&MeetMsg_RecMsgTitle,IDstr_temp,NULL,NULL);
							}
						}
	
						UI.SetTextBox(&MeetMsg_RecMsgContent,(char*)SYS->RecMsgList[SYS->RecMsgContNum-1]->RecMsgCont,NULL,NULL);			
						SYS->RecMsgContNum --;
					}
					else
					{
						
						UI.SetTextBox(&MeetMsg_MeetingTopic,(char*)SYS->MeetingName,NULL,NULL);
						UI.SetTextBox(&MeetMsg_MeetingTitle,(char*)SYS->MeetingSlogan,NULL,NULL);
						UI.SetTextBox(&MeetMsg_MeetingContent,(char*)SYS->MeetingContent,NULL,NULL);
						UI.SetTextBox(&MeetMsg_MeetingSTime,(char*)SYS->MeetingStartTime,NULL,NULL);
						UI.SetTextBox(&MeetMsg_MeetingETime,(char*)SYS->MeetingEndTime,NULL,NULL);
					}
	
//					UI.SetButton(&ButtonM_Home,UNCLICK);
//					UI.SetButton(&ButtonM_ShowName,UNCLICK);
//					UI.SetButton(&ButtonM_SysMsg,UNCLICK);
					UI.SetButton(&ButtonM_Back,UNCLICK);
				}break;
				case CallService:
				{
					UI.SetBackGround(&Main_BackGround,0,20,800,480);
					SYS->DisplayInside_SET = 1;
//					UI.SetFuncBlock(&StatusBar,NULL);
					UI.SetTextBox(&SecPageHeading,"呼叫服务",NULL,NULL);
					
					
//					UI.SetButton(&ButtonS_ExtraSer,UNCLICK);
//					UI.SetButton(&ButtonS_Confirm,UNCLICK);
					
//					UI.SetButton(&ButtonM_Home,UNCLICK);
//					UI.SetButton(&ButtonM_ShowName,UNCLICK);
//					UI.SetButton(&ButtonM_SysMsg,UNCLICK);
					UI.SetButton(&ButtonM_Back,UNCLICK);
					
					UI.SetButton(&ButtonS_Coffee,UNCLICK);
					UI.SetButton(&ButtonS_Tea,UNCLICK);
					UI.SetButton(&ButtonS_Flower,UNCLICK);
					UI.SetButton(&ButtonS_Waiter,UNCLICK);
					UI.SetButton(&ButtonS_Microphone,UNCLICK);
					UI.SetButton(&ButtonS_Pen,UNCLICK);
					UI.SetButton(&ButtonS_Paper,UNCLICK);
					UI.SetButton(&ButtonS_Water,UNCLICK);
					
//					if(SYS->ExtraSer == True)
//					{
//						CallService_ExtraSer.FontColor = BLACK;
//						UI.SetTextBox(&CallService_ExtraSer,(char*)SYS->ExtraSerCont,NULL,NULL);
//					}
//					else
//					{
//						CallService_ExtraSer.FontColor = LGRAY;
//						UI.SetTextBox(&CallService_ExtraSer,"请点击输入其他服务需求",NULL,NULL);
//					}
				}break;
				case SMS:
				{
					switch(SYS->Menu.PageSta[Lay1])
					{
						case 0:
						{
							UI.SetBackGround(&Main_BackGround,0,20,800,480);
							SYS->DisplayInside_SET = 1;
//							UI.SetFuncBlock(&StatusBar,NULL);
							UI.SetTextBox(&SecPageHeading,"短信服务",NULL,NULL);
//							UI.SetFuncBlock(&Msg_Blackboard,NULL);
							
							UI.SetButton(&Button_SMS_Addressee,UNCLICK);
							UI.SetButton(&Button_SMS_Content,UNCLICK);
							UI.SetButton(&Button_SMS_Send,UNCLICK);
							UI.SetButton(&Button_SMS_Choice,UNCLICK);
							
//							UI.SetButton(&ButtonM_Home,UNCLICK);
//							UI.SetButton(&ButtonM_ShowName,UNCLICK);
//							UI.SetButton(&ButtonM_SysMsg,UNCLICK);
							UI.SetButton(&ButtonM_Back,UNCLICK);
						}break;			
						case 1:
						{
							UI.SetButton(&Button_SMS_Addressee,UNCLICK);
							UI.SetTextBox(&SMS_AddresseeName,SYS->UserList[SYS->FuncFrame_temp]._lstUserName,NULL,NULL);
						}break;			
						case 2:
						{
							UI.SetButton(&Button_SMS_Content,UNCLICK);
							UI.SetTextBox(&SMS_Content,(char*)SYS->MsgCont,NULL,NULL);
						}break;
					}
				}break;
				case ShowName:
				{
					UI.SetBackGround(&Main_BackGround,0,20,800,480);
//					UI.SetFuncBlock(&StatusBar,NULL);
					
//					page_cache = mymalloc(SRAMEX,800*480*2);
					SYS_MALLOC(page_cache,SRAMEX,800*480*2);
					
					if(!page_cache) debug("malloc page_cache fail...\r\n");
					else
					{
						LCD.SelectScreen(ScreenOutside);
						for(j=0;j<480;j++)
						{
							for(i=0;i<800;i++) page_cache[j*800+i] = LCD.ReadPoint((u16)(i*1.28),(u16)(j*1.25));
						}
						
						LCD.SelectScreen(ScreenInside);
						OS_CRITICAL_ENTER();	//进入临界区
						LCD.Color_Fill(0,0,800-1,480-1,page_cache);
						OS_CRITICAL_EXIT();	//退出临界区
//						myfree(SRAMEX,page_cache);
						SYS_FREE(page_cache,SRAMEX);
					}
					

				}break;
				case Setting:
				{
					switch(SYS->Menu.PageSta[Lay1])
					{
						case 0:
						{
							UI.SetBackGround(&Main_BackGround,0,20,800,480);
							UI.SetTextBox(&SecPageHeading,"系统设置",NULL,NULL);
							SYS->DisplayInside_SET = 1;
//							UI.SetFuncBlock(&StatusBar,NULL);
							
//							UI.SetButton(&ButtonM_Home,UNCLICK);
//							UI.SetButton(&ButtonM_ShowName,UNCLICK);
//							UI.SetButton(&ButtonM_SysMsg,UNCLICK);
							UI.SetButton(&ButtonM_Back,UNCLICK);
							
//							UI.SetButton(&ButtonO_AddressSet,UNCLICK);
//							UI.SetButton(&ButtonO_BackGroundSet,UNCLICK);
//							UI.SetButton(&ButtonO_LightSet,UNCLICK);
//							UI.SetButton(&ButtonO_SystemSta,UNCLICK);
//							UI.SetButton(&ButtonO_WIFISet,UNCLICK);
							
							
							ButtonO_temp = 0;
						}break;
						
						case 1:
						{
							UI.SetBackGround(&Main_BackGround,0,240,800,430);
							
	//						IDstr_temp = mymalloc(SRAMIN,5);
							SYS_MALLOC(IDstr_temp,SRAMIN,5);
							
							sprintf(IDstr_temp,"%d",SYS->Device_UID);
							UI.SetTextBox(&Option_DeviceID,IDstr_temp,NULL,NULL);
							UI.SetTextBox(&Option_ServerIP,(char*)SYS->TCP_IPadd,NULL,NULL);
							UI.SetTextBox(&Option_ServerPort,(char*)SYS->TCP_Port,NULL,NULL);
							UI.SetTextBox(&Option_LocalIP,(char*)SYS->Device_IP,NULL,NULL);
							UI.SetTextBox(&Option_DHCP,NULL,NULL,NULL);
							
							UI.SetButton(&Button_Option_SetDeviceID,UNCLICK);
							UI.SetButton(&Button_Option_SetServerIP,UNCLICK);
							UI.SetButton(&Button_Option_SetServerPort,UNCLICK);
							UI.SetButton(&Button_Option_SetLocalIP,UNCLICK);
							
							if(SYS->DHCP_Enable) UI.SetButton(&Button_Option_DHCP,CLICK);
							else UI.SetButton(&Button_Option_DHCP,UNCLICK);
							
//							myfree(SRAMIN,IDstr_temp);
							SYS_FREE(IDstr_temp,SRAMIN);
						}break;
						
//						case 2:
//						{
//							UI.SetBackGround(&Main_BackGround,0,240,800,430);
//						}break;
						
						case 2:
						{
							UI.SetBackGround(&Main_BackGround,0,240,800,430);
							
							UI.SetSlider(&INScreenBrightness,SYS->Luminance);
//							UI.SetSlider(&OUTScreenBrightness,SYS->O_Luminance);
						}break;
						
//						case 4:
//						{
//							SYS_MALLOC(sprinbuf,SRAMIN,20);
//							UI.SetBackGround(&Main_BackGround,0,240,800,430);
//							
//							UI.SetTextBox(&Option_DeviceMac,(char*)SYS->Device_Mac,NULL,NULL);
//							UI.SetTextBox(&Option_CodeVersion,(char*)SYS->CompileDate,NULL,NULL);
//							
//							sprintf(sprinbuf,"%%%d",OSStatTaskCPUUsage/100);
//							UI.SetTextBox(&Option_MCUUsage,(char*)sprinbuf,NULL,NULL);
//							
//							sprintf(sprinbuf,"%%%d",100-mf_showfree("1:",1)*100/mf_showfree("1:",0));
//							UI.SetTextBox(&Option_FLASHUsage,(char*)sprinbuf,NULL,NULL);
//							
//							sprintf(sprinbuf,"-%ddb",SYS->WIFI_SignalSt);
//							UI.SetTextBox(&Option_WIFISignal,(char*)sprinbuf,NULL,NULL);
//							
//							SYS_FREE(sprinbuf,SRAMIN);
//						}break;
						
						case 3:
						{
							UI.SetBackGround(&Main_BackGround,0,240,800,430);
							
							UI.SetTextBox(&Option_SSID,(char*)SYS->WIFI_SSID,NULL,NULL);
							UI.SetTextBox(&Option_WIFIPwd,(char*)SYS->WIFI_PWD,NULL,NULL);
							
							UI.SetButton(&Button_Option_SSID,UNCLICK);
							UI.SetButton(&Button_Option_WIFIPwd,UNCLICK);
							
						}break;

					}
				}break;
				case Updating:
				{
					UI.SetBackGround(&Main_BackGround,0,20,800,480);
					SYS->DisplayInside_SET = 1;
					switch(SYS->Menu.PageSta[Lay1])
					{
						case 1:
						{
							UI.SetTextBox(&Updating_Prompt,"数据正在升级...",NULL,NULL);
						}break;
						case 2:
						{
							UI.SetTextBox(&Updating_Prompt,"升级已完成 设备将重新启动",NULL,NULL);
						}break;
					}
				}break;
				default:break;
			}
			OSMutexPost(&LCDDisplay_MUTEX,OS_OPT_POST_NONE,&err);
			SYS->Menu.PageUpdata = False;
		}
		
		if(SYS->DisplayInside_SET == 1 && SYS->Menu.Page != ShowName)
		{
			OSMutexPend (&LCDDisplay_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);
			UI.SetFuncBlock(&StatusBar,NULL);
			SYS->DisplayInside_SET = 0;
			memset(time_str,'\0',25);
			sprintf(time_str,"20%02d-%02d-%02d %02d:%02d ",SYS->TimeDate.Year,SYS->TimeDate.Month,SYS->TimeDate.Date,SYS->TimeDate.Hours,SYS->TimeDate.Minutes);
			switch(SYS->TimeDate.WeekDay)
			{
				case 1:sprintf(weekday_str,"Mon");break;
				case 2:sprintf(weekday_str,"Tue");break;
				case 3:sprintf(weekday_str,"Wed");break;
				case 4:sprintf(weekday_str,"Thur");break;
				case 5:sprintf(weekday_str,"Fri");break;
				case 6:sprintf(weekday_str,"Sat");break;
				case 7:sprintf(weekday_str,"Sun");break;
			}
			strcat(time_str,weekday_str);
			UI.SetTextBox(&StatusBar_TimeDate,time_str,NULL,NULL);
			
			if(SYS->RecMsgContNum>0) TEXT.show_str_mid(600,0,(u8*)"MSG",16,1);
			
			if(GetSTA(WIFI.WIFI_STA,WSTA_Connect))
			{
				if(SYS->WIFI_SignalSt < 45) 	UI.SingleColorIconSet(&MicICON_WIFI4);
				else if(SYS->WIFI_SignalSt >= 45 && SYS->WIFI_SignalSt < 60) UI.SingleColorIconSet(&MicICON_WIFI3);
				else if(SYS->WIFI_SignalSt >= 60 && SYS->WIFI_SignalSt < 70) UI.SingleColorIconSet(&MicICON_WIFI2);
				else if(SYS->WIFI_SignalSt >= 70) UI.SingleColorIconSet(&MicICON_WIFI1);
			}
			
			if(NETWORK.LWIP_TransRdy || NETWORK.WIFI_TransRdy) UI.SingleColorIconSet(&MicICON_Link1);
			
			UI.SingleColorIconSet(&MicICON_Battery1);
			LCD.Fill(745,5,(745+(18*SYS->ElectricQua/100)),13,WHITE);
				
			if(SYS->Charging_EN == True) UI.SingleColorIconSet(&MicICON_Battery2);
				
			OSMutexPost(&LCDDisplay_MUTEX,OS_OPT_POST_NONE,&err);
		}
	}

}
/*****************************************************************************/

/*******************屏幕（外屏）显示任务**************************/
void DisplayOutside_TaskThread(void *p_arg)
{
	OS_ERR err;
	u8 res,lumina_temp;
	

	
	CPU_SR_ALLOC();
	
	SYS->DisplayOutside_SET = 0;
	
	OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);
	OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
	debug("DisplayOutside_TaskThread start...\r\n");
	
	OSMutexPend (&LCDDisplay_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);
	LCD.SelectScreen(ScreenOutside);
	piclib_init();
	if(SYS->OuterBG_JPG) 
	{
		OS_CRITICAL_ENTER();	//进入临界区
		res = ai_load_picfile("1:/NAMEPIC/obg2.jpg",0,0,LCD.width,LCD.height,1);
		if(res)			
		{			
			debug("Display obg2.jpg fail,res=%d\r\n",res);
			res = ai_load_picfile("1:/NAMEPIC/obg1.jpg",0,0,LCD.width,LCD.height,1);
			if(res){
				SYS->OuterBG_JPG = False;
				debug("Display obg1.jpg fail,res=%d\r\n",res);
			}
		}
		OS_CRITICAL_EXIT();	//临界区
	}
	
	if(!SYS->OuterBG_JPG)
	{
			OS_CRITICAL_ENTER();	//进入临界区
			LCD.Fill(0,0,1024-1,600-1,SYS->OuterBG_Color);
			OS_CRITICAL_EXIT();	//临界区
			POINT_COLOR=SYS->Name_ComColor;
			TEXT.show_str_mid(5,2,SYS->Name_Company,64,1);
			POINT_COLOR=SYS->Name_PerColor;
			TEXT.show_str_mid(1,172,SYS->Name_Person,192,1024);
			POINT_COLOR=SYS->Name_PosColor;
			TEXT.show_str_mid(1,530,SYS->Name_Position,64,1024);
	}
	UI.BacklightGradient(0,SYS->O_Luminance,5);
	lumina_temp = SYS->O_Luminance;
	LCD.SelectScreen(ScreenInside);
	OSMutexPost(&LCDDisplay_MUTEX,OS_OPT_POST_NONE,&err);
	
	while(1)
	{
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);
		if(SYS->DisplayOutside_SET)
		{
			OSMutexPend (&LCDDisplay_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);
			LCD.SelectScreen(ScreenOutside);
			UI.BacklightGradient(SYS->O_Luminance,0,5);
			LCD.BackLightSet(0);
//			piclib_init();
			switch(SYS->DisplayOutside_SET)
			{
				case 1:
				{
					OS_CRITICAL_ENTER();	//进入临界区
					LCD.Fill(0,0,1024-1,600-1,SYS->OuterBG_Color);
					OS_CRITICAL_EXIT();	//临界区
					POINT_COLOR=SYS->Name_ComColor;
					TEXT.show_str_mid(5,2,SYS->Name_Company,64,1);
					POINT_COLOR=SYS->Name_PerColor;
					TEXT.show_str_mid(1,172,SYS->Name_Person,192,1024);
					POINT_COLOR=SYS->Name_PosColor;
					TEXT.show_str_mid(1,530,SYS->Name_Position,64,1024);
				}break;
				case 2:
				{
					res = ai_load_picfile("1:/NAMEPIC/obg2.jpg",0,0,LCD.width,LCD.height,1);
					if(res)			
					{			
						debug("Display obg2.jpg fail,res=%d\r\n",res);
						res = ai_load_picfile("1:/NAMEPIC/obg1.jpg",0,0,LCD.width,LCD.height,1);
						if(res)
							{
							SYS->OuterBG_JPG = False;
							debug("Display obg1.jpg fail,res=%d\r\n",res);
						}
					}
				}break;
			}
			SYS->DisplayOutside_SET = 0;
			UI.BacklightGradient(0,SYS->O_Luminance,5);
			LCD.SelectScreen(ScreenInside);
			OSMutexPost(&LCDDisplay_MUTEX,OS_OPT_POST_NONE,&err);
		}
		
		if(lumina_temp != SYS->O_Luminance)
		{
			OSMutexPend (&LCDDisplay_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);
			LCD.SelectScreen(ScreenOutside);
			UI.BacklightGradient(lumina_temp,SYS->O_Luminance,5);
			lumina_temp = SYS->O_Luminance;
			LCD.SelectScreen(ScreenInside);
			OSMutexPost(&LCDDisplay_MUTEX,OS_OPT_POST_NONE,&err);
		}
	}
	
	
}

/*****************************************************************/



/********************主处理任务******************************************/
void MainProcess_TaskThread(void *p_arg)
{
	OS_ERR err;
	

//	u8 i;
	u16 temp;
	
	
	OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);
	debug("MainProcess_TaskThread start...\r\n");
	
	SYS->Menu.PageUpdata = False;


	while(1)
	{
		OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);
		if(SYS->MenuOperat.PageUpdata)
		{
			
			switch(SYS->MenuOperat.Page)
			{
				
					case MainPage:
					{

					}break;
					case Rename:
					{
						switch(SYS->MenuOperat.PageSta[Lay1])
						{
							case 1:SYS->FuncInpLen = Name_PerLen-1;goto Info_T9_Input;//设置姓名
							case 2:SYS->FuncInpLen = Name_ComLen-1;goto Info_T9_Input;//设置公司名称
							case 3:SYS->FuncInpLen = Name_PosLen-1;goto Info_T9_Input;//设置职位
							Info_T9_Input:
							{
								SYS->FuncMode = 0;
								SYS->T9_ENABLE = True;
								while(SYS->T9_ENABLE)
								{
									OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
								}
								if(SYS->T9_UPDATA)
								{
									SYS->T9_UPDATA = False;
									switch(SYS->MenuOperat.PageSta[Lay1])
									{
										case 1:
										{
											if(T9.Wordoutput_len > Name_PerLen-1) T9.Wordoutput_len = Name_PerLen-1;
											memcpy(SYS->Name_Person,T9.Wordoutput,T9.Wordoutput_len);
											SYS->Name_Person[T9.Wordoutput_len] = '\0';
											debug("Name_Person = %s\r\n",SYS->Name_Person);
										}break;
										case 2:
										{
											if(T9.Wordoutput_len > Name_ComLen-1) T9.Wordoutput_len = Name_ComLen-1;
											memcpy(SYS->Name_Company,T9.Wordoutput,T9.Wordoutput_len);
											SYS->Name_Company[T9.Wordoutput_len] = '\0';
											debug("Name_Company = %s\r\n",SYS->Name_Company);
										}break;
										case 3:
										{
											if(T9.Wordoutput_len > Name_PosLen-1) T9.Wordoutput_len = Name_PosLen-1;
											memcpy(SYS->Name_Position,T9.Wordoutput,T9.Wordoutput_len);
											SYS->Name_Position[T9.Wordoutput_len] = '\0';
											debug("Name_Position= %s\r\n",SYS->Name_Position);
										}break;
										default:break;
									}
									memset(T9.Wordoutput,'\0',T9.Wordoutput_len);
//									myfree(SRAMIN,T9.Wordoutput);
									SYS_FREE(T9.Wordoutput,SRAMIN);
									
									T9.Wordoutput_len = 0;
									SYS->Menu.PageSta[Lay1] = 0;
									SYS->Menu.PageUpdata = True;
								}
							}break;
							case 4://printf("Rename person color\r\n");break;
							case 5://printf("Rename company color\r\n");break;
							case 6://printf("Rename position color\r\n");break;
							case 7://printf("Rename Set BackGround Color\r\n");break;
							{
								switch(SYS->MenuOperat.PageSta[Lay1])
								{
									case 4:SYS->FuncFrame_temp = SYS->Name_PerColor;break;
									case 5:SYS->FuncFrame_temp = SYS->Name_ComColor;break;
									case 6:SYS->FuncFrame_temp = SYS->Name_PosColor;break;
									case 7:SYS->FuncFrame_temp = SYS->OuterBG_Color;;break;
									default:break;
								}
								SYS->ColorSet_ENABLE = True;
								while(SYS->ColorSet_ENABLE)
								{
									OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
								}
								if(SYS->ColorSet_UPDATA)
								{
									switch(SYS->MenuOperat.PageSta[Lay1])
									{
									case 4: SYS->Name_PerColor = SYS->FuncFrame_temp;;break;
									case 5: SYS->Name_ComColor = SYS->FuncFrame_temp;;break;
									case 6: SYS->Name_PosColor = SYS->FuncFrame_temp;;break;
									case 7: SYS->OuterBG_Color = SYS->FuncFrame_temp;break;
									default:break;
									}
									SYS->Menu.PageUpdata = True;
									SYS->FuncFrame_temp = 0;
								}
							}break;
							
							case 8://printf("Rename Set Pic Name Car\r\n");
							{
								SYS->OuterBG_JPG = True;
								SYSPara_SaveNLoad(Save,3);
								SYS->DisplayOutside_SET = 2;
							}break;
							case 9://printf("Rename Confirm\r\n")
							{
//								SYS->Menu.PageSta[Lay1] = 12;
//								SYS->Menu.PageUpdata = True;
								SYS->DisplayOutside_SET = 1;
								SYS->OuterBG_JPG = False;
								SYSPara_SaveNLoad(Save,2);
								SYSPara_SaveNLoad(Save,3);
							}break;
							default:break;
						}
					}break;
					case MeetInfo:
					{
						
					}break;
					case CallService:
					{
						switch(SYS->MenuOperat.PageSta[Lay1])
						{
							case 1:
							case 2:
							case 3:
							case 4:
							case 5:
							case 6:
							case 7:
							case 8:
							{
								if(SYS->DeviceRegister == False)
								{
//									SYS->FuncFrame_buf = mymalloc(SRAMIN,30);
									SYS_MALLOC(SYS->FuncFrame_buf,SRAMIN,30);
									SYS->FuncFrame_temp = 2;
									strcpy((char*)SYS->FuncFrame_buf,"抱歉,设备未注册.");
									SYS->WARNING_ENABLE = True;
									while(SYS->WARNING_ENABLE) OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
									SYS->Menu.Page = MainPage;
									SYS->Menu.PageUpdata = True;
									break;
								}
								else
								{
									SYS_MALLOC(SYS->FuncFrame_buf,SRAMIN,30);
									SYS_MALLOC(SYS->ServCont,SRAMIN,50);
									SYS->FuncFrame_temp = 2;
									strcpy((char*)SYS->FuncFrame_buf,"选择：");
									strcat((char*)SYS->FuncFrame_buf,(const char *)CallService_Word[SYS->MenuOperat.PageSta[Lay1]-1]);
									strcat((char*)SYS->FuncFrame_buf,"?");
									SYS->INQUIRY_ENABLE = True;
									while(SYS->INQUIRY_ENABLE) OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
									if(SYS->INQUIRY_ENABLE_UPDATA && SYS->FuncFrame_temp == 2){
										strcpy((char*)SYS->ServCont,(const char *)CallService_Word[SYS->MenuOperat.PageSta[Lay1]-1]);
										SYS->ReqService = True;
//									i = 0;
//										while(SYS->ReqService)
//										{
//											OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); 
//											if(i++>10) break;
//										}
									}
										SYS->Menu.Page = MainPage;
										SYS->Menu.PageUpdata = True;
									
									break;
								}

								
							}
								
//							case 9://printf("CallService input other need\r\n");
//							{
////								if(SYS->ExtraSerCont == NULL) SYS->ExtraSerCont = mymalloc(SRAMIN,50);
//								if(SYS->DeviceRegister == False)
//								{
//									SYS->FuncFrame_buf = mymalloc(SRAMIN,30);
//									SYS->FuncFrame_temp = 2;
//									strcpy((char*)SYS->FuncFrame_buf,"抱歉,设备未注册.");
//									SYS->WARNING_ENABLE = True;
//									while(SYS->WARNING_ENABLE) OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
//									SYS->Menu.Page = MainPage;
//									SYS->Menu.PageUpdata = True;
//									break;
//								}
//								else
//								{
//									SYS_MALLOC(SYS->ExtraSerCont,SRAMIN,50);
//									SYS->T9_ENABLE = True;
//									while(SYS->T9_ENABLE)
//									{
//										OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
//									}
//									if(SYS->T9_UPDATA)
//									{
//										SYS->T9_UPDATA = False;
//										if(T9.Wordoutput_len > 50) T9.Wordoutput_len = 50;
//										memcpy(SYS->ExtraSerCont,T9.Wordoutput,T9.Wordoutput_len);
//										SYS->ExtraSerCont[T9.Wordoutput_len] = '\0';
//										debug("ExtraSerCont = %s\r\n",SYS->ExtraSerCont);
//	//									SetSTA(SYS->Menu.PageSta[Lay1],(0x01 << res));
//	//									SYS->Menu.PageUpdata = True; 
//										if(T9.Wordoutput_len > 0) 
//										{
//											SYS->ExtraSer = True;
//											T9.Wordoutput_len = 0;
//										}
//										SYS->Menu.PageUpdata = True; 
//										
//									}
//									else SYS->ExtraSer = False;
//								}
//							}break;
//							case 10://printf("CallService Confirm : server = %d \r\n",SYS->Menu.OperatPara);
//							{
////								if(!SYS->ServCont)SYS->ServCont = mymalloc(SRAMIN,10000);
//								SYS_MALLOC(SYS->ServCont,SRAMIN,200);
////								memset(SYS->ServCont,'\0',200);
//								
//								SYS->SerID = SYS->MenuOperat.Para;
//								
//								if(SYS->DeviceRegister == False)
//								{
//									SYS->FuncFrame_buf = mymalloc(SRAMIN,30);
//									SYS->FuncFrame_temp = 2;
//									strcpy((char*)SYS->FuncFrame_buf,"抱歉,设备未注册.");
//									SYS->WARNING_ENABLE = True;
//									while(SYS->WARNING_ENABLE) OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
//									SYS->Menu.Page = MainPage;
//									SYS->Menu.PageUpdata = True;
//									break;
//								}
//								else
//								{
//									if(SYS->SerID != 0 || SYS->ExtraSer == True)
//									{
//										for(i=0;i<8;i++)
//										{
//											if(GetSTA(SYS->SerID,(0x01 << i))) strcat((char*)SYS->ServCont,(const char *)CallService_Word[i]);
//										}
//										if(SYS->ExtraSer && SYS->ExtraSerCont != NULL) strcat((char*)SYS->ServCont,(const char*)SYS->ExtraSerCont);
//										SYS->ReqService = True;
//										i = 0;
//										while(SYS->ReqService)
//										{
//											OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); 
//											if(i++>10) break;
//										}
//										
//										debug("Send Service sucess:%s\r\n",SYS->ServCont);
//										SYS->FuncFrame_buf = mymalloc(SRAMIN,30);
//										SYS->FuncFrame_temp = 2;
//										strcpy((char*)SYS->FuncFrame_buf,"已发送请求");
//										SYS->WARNING_ENABLE = True;
//										while(SYS->WARNING_ENABLE) OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
//	//									SYS->Menu.PageSta[Lay1] = 0x01;
//									}
////									else
////									{
////										debug("Service empty\r\n");
////										SYS->FuncFrame_buf = mymalloc(SRAMIN,30);
////										SYS->FuncFrame_temp = 2;
////										strcpy((char*)SYS->FuncFrame_buf,"请输入请求");
////										SYS->WARNING_ENABLE = True;
////										while(SYS->WARNING_ENABLE) OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
////										SYS->Menu.Page = MainPage;
////										SYS->Menu.PageUpdata = True;
////										break;
////									}
//									SYS->ExtraSer = False; 
//									SYS->SerID = 0;
//									SYS_FREE(SYS->ServCont,SRAMIN);
//									SYS_FREE(SYS->ExtraSerCont,SRAMIN);
//									SYS->Menu.PageUpdata = True;
//								}
//							}break;
							case 0xFF:
							case 0xF2:
							case 0xF3:	
							{
								SYS->ExtraSer = False; 
								SYS->SerID = 0;
								SYS_FREE(SYS->ServCont,SRAMIN);
								SYS_FREE(SYS->ExtraSerCont,SRAMIN);
							}break;
							default:break;
						}
					}break;
					case SMS:
					{
						if(SYS->MenuOperat.PageSta[Lay1]>=1 && SYS->MenuOperat.PageSta[Lay1]<=3)
						{
							if(SYS->DeviceRegister == False)
							{
								SYS->FuncFrame_buf = mymalloc(SRAMIN,30);
								SYS->FuncFrame_temp = 2;
								strcpy((char*)SYS->FuncFrame_buf,"抱歉,设备未注册.");
								SYS->WARNING_ENABLE = True;
								while(SYS->WARNING_ENABLE) OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
								SYS->Menu.Page = MainPage;
								SYS->Menu.PageUpdata = True;
								break;
							}
							else
							{
									switch(SYS->MenuOperat.PageSta[Lay1])
									{
										case 1://printf("SMS Addressee\r\n");
										{
											SYS->AddresseeSel_ENABLE = True;
											while(SYS->AddresseeSel_ENABLE)  OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
											if(SYS->AddresseeSel_UPDATA)
											{
												SYS->AddresseeSel_UPDATA = False;
												SYS->MsgReceive_ID = SYS->UserList[SYS->FuncFrame_temp]._lstUserID;
												SYS->Menu.PageSta[Lay1] = 1;
												SYS->Menu.PageUpdata = True;
											}
										}break;
										case 2://printf("SMS Content \r\n");
										{
											SYS_MALLOC(SYS->MsgCont,SRAMIN,200);
											SYS->FuncMode = 0;
											SYS->T9_ENABLE = True;
											while(SYS->T9_ENABLE)
											{
												OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
											}
											if(SYS->T9_UPDATA)
											{
												SYS->T9_UPDATA = False;
												if(T9.Wordoutput_len > 100) T9.Wordoutput_len = 100;
												memcpy(SYS->MsgCont,T9.Wordoutput,T9.Wordoutput_len);
												SYS->MsgCont[T9.Wordoutput_len] = '\0';
												debug("Len = %d,MsgCont = %s\r\n",T9.Wordoutput_len,SYS->MsgCont);
												SYS->Menu.PageSta[Lay1]=2;
												SYS->Menu.PageUpdata = True;
											}
										}break;
										case 3://printf("SMS Send \r\n");
										{
											if(SYS->MsgCont != NULL && strlen((const char *)SYS->MsgCont) != 0 && SYS->MsgReceive_ID != 0)
											{
												SYS->SendMassage = True;
												while(SYS->SendMassage) OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
													
												SYS->FuncFrame_buf = mymalloc(SRAMIN,30);
												SYS->FuncFrame_temp = 2;
												strcpy((char*)SYS->FuncFrame_buf,"已发送");
												SYS->WARNING_ENABLE = True;
												while(SYS->WARNING_ENABLE) OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);										
											
												SYS->MsgReceive_ID = 0;
												SYS_FREE(SYS->MsgCont,SRAMIN);
												SYS->Menu.PageSta[Lay1] = 0;
												SYS->Menu.PageUpdata = True;
											}
											else 
											{

												SYS->FuncFrame_buf = mymalloc(SRAMIN,30);
												SYS->FuncFrame_temp = 2;
												strcpy((char*)SYS->FuncFrame_buf,"短信收件人及内容不能为空");
												SYS->WARNING_ENABLE = True;
												while(SYS->WARNING_ENABLE) OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);

												debug("send msg error:msg:%s ID:%d\r\n",SYS->MsgCont,SYS->MsgReceive_ID);
											}

										}break;
										default:break;
									}
							}
						}
						else
						{
							switch(SYS->MenuOperat.PageSta[Lay1])
							{
								case 0xFF:
								case 0xF2:
								case 0xF3:	
								{
									SYS->MsgReceive_ID = 0;
									SYS_FREE(SYS->MsgCont,SRAMIN);
		//								memset(SYS->MsgCont,'\0',200);
		//								myfree(SRAMIN,SYS->MsgCont);
		//								SYS->MsgCont = NULL;
								}break;
								default:break;
							}
						}
					}break;
					case ShowName:break;
					case Setting:
					{
						switch(SYS->Menu.PageSta[Lay1])
						{
							case 1:
							{
								switch(SYS->MenuOperat.PageSta[Lay1])
								{
									case 1:
									case 2:
									case 3:
									case 4:
									{
										SYS->FuncMode = 2;
										SYS->T9_ENABLE = True;
										while(SYS->T9_ENABLE)
										{
											OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
										}
										if(SYS->T9_UPDATA)
										{
											SYS->T9_UPDATA = False;
											switch(SYS->MenuOperat.PageSta[Lay1])
											{
												case 1:
												{
													if(IS_NUM(T9.Wordoutput)) 
													{
														temp =  atoi((char*)T9.Wordoutput);
														if(temp > 0 && temp <= MaxDeviceID) SYS->Device_UID = atoi((char*)T9.Wordoutput);	
														
														SYS->DeviceRegister = False;
														debug("Device_UID = %d\r\n", SYS->Device_UID);
														SYSPara_SaveNLoad(Save,1);
													}
												}break;
												
												case 2:
												{
													if(IS_IPaddr(T9.Wordoutput)) 
													{
														strcpy((char*)SYS->TCP_IPadd,(char*)T9.Wordoutput);
														WIFI.WIFI_STA = 0x01;
														WIFI.WIFI_STA_Expect = 0x0F;
														LWIP.NetConnect_Rdy = False;
														strcpy((char*)NETWORK.Current_IPadd,(char*)SYS->TCP_IPadd);
														debug("TCP_IPadd = %s\r\n", SYS->TCP_IPadd);
														SYSPara_SaveNLoad(Save,4);
													}
													else debug("set serip fail:%s\r\n",T9.Wordoutput);
												}break;
												case 3:
												{
													if(IS_NUM(T9.Wordoutput))
													{
														strcpy((char*)SYS->TCP_Port,(char*)T9.Wordoutput);
														strcpy((char*)NETWORK.Current_Port,(char*)SYS->TCP_Port);
														WIFI.WIFI_STA = 0x01;
														WIFI.WIFI_STA_Expect = 0x0F;
														LWIP.NetConnect_Rdy = False;
														debug("TCP_Port = %s\r\n", SYS->TCP_Port);
														SYSPara_SaveNLoad(Save,4);
													}
												}break;
												case 4:
												{
													if(IS_IPaddr(T9.Wordoutput)) 
													{
														strcpy((char*)SYS->Device_IP,(char*)T9.Wordoutput);
														strcpy((char*)NETWORK.Current_DeviceIP,(char*)SYS->Device_IP);
														SYS->DHCP_Enable = False;		
														WIFI.WIFI_STA = 0x01;
														WIFI.WIFI_STA_Expect = 0x0F;
														LWIP.NetConnect_Rdy = False;
														debug("Device_IP = %s\r\n", SYS->Device_IP);
														SYSPara_SaveNLoad(Save,4);
													}
												}break;
											}
											SYS->DeviceRegister = False;
//											myfree(SRAMIN,T9.Wordoutput);
											SYS_FREE(T9.Wordoutput,SRAMIN);
											T9.Wordoutput = NULL;
										}	
									}break;
									case 5:
									{
										WIFI.WIFI_STA = 0x01;
										WIFI.WIFI_STA_Expect = 0x0F;
										LWIP.NetConnect_Rdy = False;
										SYS->DHCP_Enable = (_BOOL)SYS->MenuOperat.Para;
										SYSPara_SaveNLoad(Save,4);
									}break;
									default:break;
								}
								SYS->Menu.PageSta[Lay1] = 1;
								SYS->Menu.PageUpdata = True;
							}break;
							
//							case 2:
//							{
//								if(SYS->MenuOperat.PageSta[Lay1] == 1) 
//								{
//									SYS->BACKGROUND_Set_ENABLE = True;
//									while(SYS->BACKGROUND_Set_ENABLE) OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
//									if(SYS->BACKGROUND_Set_UPDATA) 
//									{
//										SYSPara_SaveNLoad(Save,3);
//										HAL_NVIC_SystemReset();
//									}
//									SYS->Menu.PageSta[Lay1] = 2;
//									SYS->Menu.Page = Setting;
//									SYS->Menu.PageUpdata = True;
//								}
//							}break;
							
							case 3:
							{
								switch(SYS->MenuOperat.PageSta[Lay1])
								{
									case 1://printf("Setting Set WIFI SSID\r\n");
									{
										SYS->WIFI_SSID_Get_ENABLE = True;
										while(SYS->WIFI_SSID_Get_ENABLE) 
										{
											OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
										}
										
										if(SYS->WIFI_SSID_Get_UPDATA)
										{
											strcpy((char*)SYS->WIFI_SSID,SYS->WIFI_List[SYS->FuncFrame_temp]._lstWIFISSID);
											strcpy((char*)WIFI.Current_SSID ,(char*)SYS->WIFI_SSID);
											WIFI.WIFI_STA = 0x01;
											WIFI.WIFI_STA_Expect = 0x0F;
											SYSPara_SaveNLoad(Save,4);
//											myfree(SRAMIN,SYS->WIFI_List);
											SYS_FREE(SYS->WIFI_List,SRAMIN);
										}
										SYS->Menu.PageSta[Lay1] = 3;
										SYS->Menu.PageUpdata = True;
									}break;
									case 2://printf("Setting Set WIFI PWD\r\n");
									{
										SYS->FuncMode = 1;
										SYS->T9_ENABLE = True;
										while(SYS->T9_ENABLE)
										{
											OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
										}
										if(SYS->T9_UPDATA)
										{
											SYS->T9_UPDATA = False;
											if(T9.Wordoutput_len > 30) T9.Wordoutput_len = 30;
											memcpy(SYS->WIFI_PWD,T9.Wordoutput,T9.Wordoutput_len);
											SYS->WIFI_PWD[T9.Wordoutput_len] = '\0';
											strcpy((char*)WIFI.Current_PWD,(char*)SYS->WIFI_PWD);
											WIFI.WIFI_STA = 0x01;
											WIFI.WIFI_STA_Expect = 0x0F;
											SYSPara_SaveNLoad(Save,4);
//											myfree(SRAMIN,T9.Wordoutput);
											SYS_FREE(T9.Wordoutput,SRAMIN);
										}
										SYS->Menu.PageSta[Lay1] = 3;
										SYS->Menu.PageUpdata = True;
									}break;
									default:break;
								}
							}break;
							default:break;
						}
					}break;
					case Updating:break;
					default:break;
			}
			SYS->MenuOperat.PageSta[Lay1] = 0;
			SYS->MenuOperat.PageUpdata = False;
		}
		
		if(SYS->HttpGeting == True) //HTTP下载处理
		{
			SYS->Menu.Page = Updating;
			SYS->Menu.PageSta[Lay1] = 1;
			SYS->Menu.PageUpdata = True;
			OSTmrStop(&SYS_Timer_1S,OS_OPT_TMR_NONE,NULL,&err);
			OSTmrStop(&SYS_Timer_100MS,OS_OPT_TMR_NONE,NULL,&err);
			while(SYS->HttpGeting) OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
			if(SYS->BGnameUpdata) SYS->OuterBG_JPG = True;
			SYSPara_SaveNLoad(Save,0);
			SYS->Menu.Page = Updating;
			SYS->Menu.PageSta[Lay1] = 2;
			SYS->Menu.PageUpdata = True;
			OSTimeDlyHMSM(0,0,5,0,OS_OPT_TIME_HMSM_STRICT,&err);
			HAL_NVIC_SystemReset();
		}
		
		if(SYS->Device_ShutDown == True) //关机
		{
			LCD.BackLightSet(0);
			LCD.SelectScreen(ScreenOutside);
			LCD.BackLightSet(0);
			SYSPara_SaveNLoad(Save,0);
			POWER_ENABLE = 0;
			while(1);
//			HAL_NVIC_SystemReset();
		}
	}
}
	

/********************弹框功能任务******************************************/
void FuncFramework_TaskThread(void *p_arg)
{
		OS_ERR err;
	CPU_SR_ALLOC();
	
	_tp_sta CurrentTPsta = Loosen;
	u8 keytemp;
	u8 res,len/*,num*/;
	u16 i,j;
	u16 *page_cache;
	u16 Output_num,temp;
	u8  *Output_buf/*,*Ipath*/;
	char *wtemp;

	
	p_arg = p_arg;
	OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);
	debug("FuncFramework_TaskThread start...\r\n");
	
	while(1)
	{
		OSTimeDlyHMSM(0,0,0,300,OS_OPT_TIME_HMSM_STRICT,&err);
		//T9输入法
		if(SYS->T9_ENABLE)
		{
			OSMutexPend (&LCDDisplay_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);
			debug("T9_Input ENABLE...\r\n");
			
			Output_num = 0;
			temp = 0;
			T9.WordPage = 0;
		
			if(SYS->FuncMode == 1) T9.InpMode = 1;
			else if(!SYS->FuncMode) T9.InpMode = 0;
			
			if(SYS->FuncInpLen == 0)
				SYS->FuncInpLen = 16;
//			Output_buf = mymalloc(SRAMIN,500);
//			page_cache = mymalloc(SRAMEX,800*480*2);
			SYS_MALLOC(Output_buf,SRAMIN,500);
			SYS_MALLOC(page_cache,SRAMEX,800*480*2);
			
			if(!Output_buf || !page_cache) SYS->T9_ENABLE = False;
			else
			{
				OS_CRITICAL_ENTER();	//进入临界区
				for(j=0;j<480;j++)
				{
					for(i=0;i<800;i++)
					{
						page_cache[800*j+i] = LCD.ReadPoint(i,j);
					}
				}
				memset(Output_buf,'\0',500);
				LCD.SelectScreen(ScreenInside);
				py_load_ui(0);
				OS_CRITICAL_EXIT();	//退出临界区	
			}
			
			while(SYS->T9_ENABLE)
			{
				OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);
				
				if(SYS->TP.Status != CurrentTPsta)
				{
					SYS->TP.StaUpdata = False;
					SYS->TP.CoUpdata = False;
					CurrentTPsta = SYS->TP.Status;
					if(CurrentTPsta == Press)
					{
						if(SYS->TP.Coordinate_X > 0 && SYS->TP.Coordinate_X < 800 && SYS->TP.Coordinate_Y > 279 && SYS->TP.Coordinate_Y < 480)
						{
							T9.KeyPress = py_get_keynum(SYS->TP.Coordinate_X,SYS->TP.Coordinate_Y);
//							debug("keyboard = %d\r\n",T9.KeyPress);
							if(T9.KeyPress >= 1 && T9.KeyPress <= 12)
							{

								keytemp = T9.KeyPress;
								OS_CRITICAL_ENTER();	//进入临界区
								py_set_ui(keytemp,1);		
								OS_CRITICAL_EXIT();	//退出临界区	
								if(keytemp == 4 && T9.inputlen == 0) 
								{
									if(Output_num)
									{
										if(Output_buf[Output_num-1] >= 0x20 && Output_buf[Output_num-1] <= 0x7A) Output_num -= 1;
										else Output_num -= 2;
										Output_buf[Output_num] = '\0';
										debug("%s\r\n",Output_buf);
									}
								}
								if(keytemp == 12 && Output_num < SYS->FuncInpLen)
								{
									if(SYS->FuncMode == 2 || SYS->FuncMode == 1)
									{
										Output_buf[Output_num++] = '0';
									}
									else Output_buf[Output_num++] = ' ';
									
									
//									Output_buf[Output_num++] = ' ';
								}
								
								if(!SYS->FuncMode) //SYS->FuncMode == 0 输入法开放全部输入方式
								{
									if(!T9.InpMode) py_get_word(T9.KeyPress);
									else en_num_get_word(T9.KeyPress);
								}
								else if(SYS->FuncMode == 1)  //非中文输入
								{
									en_num_get_word(T9.KeyPress);
								}
								else if(SYS->FuncMode == 2 && Output_num < SYS->FuncInpLen) //纯数字
								{
									if(keytemp != 4 && keytemp != 8 && keytemp != 12) Output_buf[Output_num++] = keytemp - (keytemp)/4 + '0';
									else if( keytemp == 8) Output_buf[Output_num++] = '.';
//									else if( keytemp == 12) Output_buf[Output_num++] = '0';
								}

								T9.WordPage = 0;
							}
							else if(T9.KeyPress >= 21 && T9.KeyPress <= 29 && (T9.KeyPress-20) <= T9.result_num)//拼音选择
							{
								if(!T9.InpMode)
								{
									keytemp = T9.KeyPress;
									OS_CRITICAL_ENTER();	//进入临界区
									py_set_ui(keytemp,1);		
									OS_CRITICAL_EXIT();	//退出临界区	
									
									T9.WordPage = 0;
									T9.WordOrdinal = T9.KeyPress - 21;
								}
							}
							else if(T9.KeyPress == 31 || T9.KeyPress == 32)//翻页
							{
								if(!T9.InpMode)
								{
									keytemp = T9.KeyPress;
									OS_CRITICAL_ENTER();	//进入临界区
									py_set_ui(keytemp,1);		
									OS_CRITICAL_EXIT();	//退出临界区	
									if((T9.WordPage+1)*10 < T9.resultword_num && T9.KeyPress == 32)	//判断还有没得翻页
									{
										T9.WordPage += 1;
									}
									else if(T9.WordPage > 0 && T9.KeyPress == 31)
									{
										T9.WordPage -= 1;
									}
								}
							}
							else if(T9.KeyPress >= 41 && T9.KeyPress <= 50)
							{
								if(T9.result_num)
								{
									if(T9.WordPage*10+(T9.KeyPress - 41) < T9.resultword_num)  //判断按下位置是否超过该拼音对应的文字总数
									{
										keytemp = T9.KeyPress;
										OS_CRITICAL_ENTER();	//进入临界区
										py_set_ui(keytemp,1);		
										OS_CRITICAL_EXIT();	//退出临界区	
										
										if(Output_num < SYS->FuncInpLen)
										{
											if(!T9.InpMode)
											{
												res = T9.pymb[T9.WordOrdinal]->pymb[(T9.WordPage*10+keytemp-41)*2];
												if(res >= 0x20 && res <= 0x7A) Output_buf[Output_num++]   = T9.pymb[T9.WordOrdinal]->pymb[(T9.WordPage*10+keytemp-41)];
												else
												{
													Output_buf[Output_num++]   = T9.pymb[T9.WordOrdinal]->pymb[(T9.WordPage*10+keytemp-41)*2];
													Output_buf[Output_num++] = T9.pymb[T9.WordOrdinal]->pymb[(T9.WordPage*10+keytemp-41)*2+1];
												}
											}
											else
											{
												Output_buf[Output_num++]  = T9.input_en_num[keytemp-41];
											}
										}
										
										
//										debug("%s \r\n",Output_buf);
										
										T9.inputlen = 0;;
										memset(T9.inputstr,'\0',7);
										T9.WordPage = 0;
										T9.result_num = 0;
									}
								}
							}
							else if(T9.KeyPress >= 51 && T9.KeyPress <= 53)
							{
								keytemp = T9.KeyPress;
								OS_CRITICAL_ENTER();	//进入临界区
								py_set_ui(keytemp,1);		
								OS_CRITICAL_EXIT();	//退出临界区	
								
								if(keytemp == 52)
								{
									T9.inputlen = 0;                   //相关变量清零
									memset(T9.inputstr,'\0',7);
									T9.WordPage = 0;
									T9.result_num = 0;
									keytemp = 0;
									
									SYS->T9_ENABLE = False;
								}									
								else if(keytemp == 51)
								{
//									T9.Wordoutput = mymalloc(SRAMIN,Output_num+1);
									SYS_MALLOC(T9.Wordoutput,SRAMIN,Output_num+1);
									
									memset(T9.Wordoutput,'\0',Output_num+1);
									memcpy(T9.Wordoutput,Output_buf,Output_num);
									T9.Wordoutput_len = Output_num;
									debug("T9 output = %s\r\n",T9.Wordoutput);
									
									T9.inputlen = 0;
									memset(T9.inputstr,'\0',7);
									T9.WordPage = 0;
									T9.result_num = 0;
									keytemp = 0;
									
									SYS->T9_UPDATA = True;
									SYS->T9_ENABLE = False;
								}
								else if(keytemp == 53)
								{
									if(SYS->FuncMode == 0)
									{
										T9.InpMode = !T9.InpMode;
										py_get_word(8);
									}
								}
							}
						}
					}
					else  
					{	
						if(keytemp)
						{
							OS_CRITICAL_ENTER();	//进入临界区
							py_set_ui(keytemp,0);
							OS_CRITICAL_EXIT();	//退出临界区
							keytemp = 0;
							
							
							if(!T9.InpMode) T9.resultword_num  = strlen((const char *)T9.pymb[T9.WordOrdinal]->pymb)/2;
							else  T9.resultword_num  = strlen((const char*)T9.input_en_num);
						
							if(T9.result_num)
							{
								OS_CRITICAL_ENTER();	//进入临界区
								py_load_ui(1);
								OS_CRITICAL_EXIT();	//退出临界区
								
								if(!T9.InpMode)
								{
									if(T9.result_num > 9) T9.result_num = 9;
									for(res = 0;res<T9.result_num;res++)
									{
										POINT_COLOR=BLACK;
										TEXT.show_str_mid(0+(res%3)*71,320+(res/3)*56,T9.pymb[res]->py,24,71);
									}

									POINT_COLOR=BLACK;
									for(j=0;j<10;j++)
									{
										i = T9.WordPage*10+j;
										if(i>T9.resultword_num) break;
										if(T9.pymb[T9.WordOrdinal]->pymb[0] >= 0x20 && T9.pymb[T9.WordOrdinal]->pymb[0]<= 0x7A) LCD.ShowChar(0+j*60+30,280,T9.pymb[T9.WordOrdinal]->pymb[i],24,1);
										else TEXT.show_font(0+j*60+30,280,&T9.pymb[T9.WordOrdinal]->pymb[i*2],24,1);
									
									}
								}
								else
								{
									POINT_COLOR=BLACK;
									for(j=0;j<10;j++)
									{
										i = T9.WordPage*10+j;
										if(i>=T9.resultword_num) break;
										LCD.ShowChar(0+j*60+30,280,T9.input_en_num[i],24,1);

									}
								}
								
							}	
							else 	
							{
								OS_CRITICAL_ENTER();	//进入临界区
								py_load_ui(1);
								OS_CRITICAL_EXIT();	//退出临界区
							}
							
							if(temp != Output_num)
							{
								
								OS_CRITICAL_ENTER();	//进入临界区
								py_load_ui(2);
								temp = Output_num;
								POINT_COLOR=BLACK;
								TEXT.show_str(40,50,720,240,Output_buf,24,1);
								OS_CRITICAL_EXIT();	//退出临界区
							}
						}

					}		
				}
				
			}
			OS_CRITICAL_ENTER();	//进入临界区
			SYS->FuncInpLen = 0;
			LCD.Color_Fill(0,0,800-1,480-1,page_cache);
			
			debug("T9_Input DISABLE...\r\n");
//			myfree(SRAMEX,page_cache);
//			memset(Output_buf,'\0',500);
			SYS_FREE(page_cache,SRAMEX);
			SYS_FREE(Output_buf,SRAMIN);
			
//			myfree(SRAMIN,Output_buf);
			OS_CRITICAL_EXIT();	//退出临界区
			OSMutexPost(&LCDDisplay_MUTEX,OS_OPT_POST_NONE,&err);
//				debug("T9 output = %s\r\n",T9.Wordoutput);
		}
		
		else if(SYS->ColorSet_ENABLE)
		{
		
			OSMutexPend (&LCDDisplay_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);
			debug("ColorSet ENABLE...\r\n");
			
//			page_cache = mymalloc(SRAMEX,400*360*2);
			SYS_MALLOC(page_cache,SRAMEX,400*360*2);
			
			if(!page_cache) SYS->ColorSet_ENABLE = False;
			else
			{
				OS_CRITICAL_ENTER();	//进入临界区
				for(j=70;j<430;j++)
				{
					for(i=200;i<600;i++)
					{
						page_cache[400*(j-70)+(i-200)] = LCD.ReadPoint(i,j);
					}
				}
				LCD.SelectScreen(ScreenInside);
				LCD.Fill(200,100,600-1,390-1,LGRAY);
				LCD.Fill(200,70,600-1,100-1,BLUE);
				LCD.Fill(230,150,350,220,SYS->FuncFrame_temp);
				LCD.Fill(200,390,400,430-1,MGRAY);
				LCD.Fill(400,390,600-1,430-1,BLUE);
				
				POINT_COLOR=WHITE;
				TEXT.show_str_mid(200,70+3,"选择颜色",24,400);
				TEXT.show_str_mid(370,160,"已选颜色",24,1);
				TEXT.show_str_mid(200,390+8,"取消",24,200);
				TEXT.show_str_mid(400,390+8,"确定",24,200);
				for(res = 0;res < 58;res++)
				{

					LCD.Fill(res*6+226,240,(res+1)*6+226,380,ColorSet_para[res]);
				}
				
				OS_CRITICAL_EXIT();	//退出临界区	
			}
			
			temp = SYS->FuncFrame_temp;
			while(SYS->ColorSet_ENABLE)
			{
				OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);
				if(SYS->TP.Status == Press && SYS->TP.Coordinate_X > 226 && SYS->TP.Coordinate_X < 574 && SYS->TP.Coordinate_Y > 240 && SYS->TP.Coordinate_Y < 380)
				{				
					SYS->TP.StaUpdata = False;
					SYS->TP.CoUpdata = False;
					i = LCD.ReadPoint(SYS->TP.Coordinate_X,SYS->TP.Coordinate_Y);
					if(temp != i)
					{
						OS_CRITICAL_ENTER();	//进入临界区
						temp = i;
						LCD.Fill(230,150,350,220,temp);
						OS_CRITICAL_EXIT();	//退出临界区
					}
				}
				else if(SYS->TP.Status == Press && SYS->TP.Coordinate_X > 200 && SYS->TP.Coordinate_X < 400 && SYS->TP.Coordinate_Y > 390 && SYS->TP.Coordinate_Y < 430)
				{
					SYS->TP.StaUpdata = False;
					SYS->TP.CoUpdata = False;
					temp = 0;
					SYS->ColorSet_ENABLE = False;
					SYS->ColorSet_UPDATA = False;
				}
				else if(SYS->TP.Status == Press && SYS->TP.Coordinate_X > 400 && SYS->TP.Coordinate_X < 600 && SYS->TP.Coordinate_Y > 390 && SYS->TP.Coordinate_Y < 430)
				{
					
					SYS->TP.StaUpdata = False;
					SYS->TP.CoUpdata = False;
					SYS->FuncFrame_temp = temp;
					SYS->ColorSet_UPDATA = True;
					temp = 0;
					SYS->ColorSet_ENABLE = False;
					debug("Color = %x\r\n",SYS->FuncFrame_temp);
				}
			}
			
			OS_CRITICAL_ENTER();	//进入临界区
			LCD.Color_Fill(200,70,600-1,430-1,page_cache);
			OSMutexPost(&LCDDisplay_MUTEX,OS_OPT_POST_NONE,&err);
			debug("ColorSet DISABLE...\r\n");
//			myfree(SRAMEX,page_cache);
			SYS_FREE(page_cache,SRAMEX);
			OS_CRITICAL_EXIT();	//退出临界区
		}
		
		else if(SYS->WordSizeSet_ENABLE)
		{		
			debug("WordSizeSet ENABLE...\r\n");
			
//			page_cache = mymalloc(SRAMEX,40*200*2);
			SYS_MALLOC(page_cache,SRAMEX,40*200*2);
			
			if(!page_cache) SYS->WordSizeSet_ENABLE = False;
			else
			{
				OS_CRITICAL_ENTER();	//进入临界区
				for(j=220;j<260;j++)
				{
					for(i=300;i<500;i++)
					{
						page_cache[200*(j-220)+(i-300)] = LCD.ReadPoint(i,j);
					}
				}
				LCD.SelectScreen(ScreenInside);
				LCD.Fill(300,220,500-1,260-1,BLUE);

				
				POINT_COLOR=WHITE;

				for(i=1;i<=5;i++)
					LCD.ShowNum(300+(i-1)*40+8,220+8,i,1,24);			
				
				OS_CRITICAL_EXIT();	//退出临界区	
			}
//			OSTimeDlyHMSM(0,0,0,300,OS_OPT_TIME_HMSM_STRICT,&err); //延时为了弹框不闪退
			while(SYS->WordSizeSet_ENABLE)
			{
				OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);
				if(SYS->TP.Status != CurrentTPsta)
				{
					SYS->TP.StaUpdata = False;
					SYS->TP.CoUpdata = False;
					CurrentTPsta = SYS->TP.Status;
					
					if(CurrentTPsta == Press)
					{
						if(SYS->TP.Status == Press && SYS->TP.Coordinate_X > 300 && SYS->TP.Coordinate_X < 340 && SYS->TP.Coordinate_Y > 220 && SYS->TP.Coordinate_Y < 260)		temp = 1;
						else if(SYS->TP.Status == Press && SYS->TP.Coordinate_X > 340 && SYS->TP.Coordinate_X < 380 && SYS->TP.Coordinate_Y > 220 && SYS->TP.Coordinate_Y < 260)temp = 2;
						else if(SYS->TP.Status == Press && SYS->TP.Coordinate_X > 380 && SYS->TP.Coordinate_X < 420 && SYS->TP.Coordinate_Y > 220 && SYS->TP.Coordinate_Y < 260)temp = 3;
						else if(SYS->TP.Status == Press && SYS->TP.Coordinate_X > 420 && SYS->TP.Coordinate_X < 460 && SYS->TP.Coordinate_Y > 220 && SYS->TP.Coordinate_Y < 260)temp = 4;
						else if(SYS->TP.Status == Press && SYS->TP.Coordinate_X > 460 && SYS->TP.Coordinate_X < 500 && SYS->TP.Coordinate_Y > 220 && SYS->TP.Coordinate_Y < 260)temp = 5;
						else temp = 0;
					}
					else
					{
						if(temp)
						{
							SYS->FuncFrame_temp = temp;
							temp = 0;
							SYS->WordSizeSet_UPDATA = True;
							SYS->WordSizeSet_ENABLE = False;
						}
						else
						{
							temp = 0;
							SYS->WordSizeSet_UPDATA = False;
							SYS->WordSizeSet_ENABLE = False;
						}
					}
				}
			}
			
			OS_CRITICAL_ENTER();	//进入临界区
			LCD.Color_Fill(300,220,500-1,260-1,page_cache);

			debug("WordSizeSet DISABLE...\r\n");
//			myfree(SRAMEX,page_cache);
			SYS_FREE(page_cache,SRAMEX);
			OS_CRITICAL_EXIT();	//退出临界区
		}
		else if(SYS->AddresseeSel_ENABLE)
		{
			OSMutexPend (&LCDDisplay_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);
			debug("AddresseeSel ENABLE...\r\n");
			
			Output_num = 0;
			temp = 0;
			res =0;
//			Output_buf = mymalloc(SRAMIN,30);
//			page_cache = mymalloc(SRAMEX,300*380*2);
			SYS_MALLOC(page_cache,SRAMEX,300*380*2);
			
			if(!page_cache) SYS->AddresseeSel_ENABLE = False;
			else
			{
				LCD.SelectScreen(ScreenInside);
				OS_CRITICAL_ENTER();	//进入临界区
				for(j=60;j<440;j++)
				{
					for(i=250;i<550;i++)
					{
						page_cache[300*(j-60)+(i-250)] = LCD.ReadPoint(i,j);
					}
				}
				
				
//				LCD.Color_Fill(250+1,60+1,550-1,440-1,SYS->bg_cache);
				LCD.Fill(250+1,60+1,550-1,440-1,LIGHTBLUE);
				LCD.Fill(250+10,60+10,550-10,440-10,LGRAY);
				
				OS_CRITICAL_EXIT();	//退出临界区	
			}
			

			temp = 0;
			POINT_COLOR=BLACK;
			TEXT.show_str_mid(250+10,250,"正在获取用户列表...",24,280);
			while(!SYS->UserList) 
			{
				SYS->GetUserList = True;
				OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
				temp ++;
				if(temp > 20) break;
			}
			
			
			if(!SYS->UserList) 
			{
				LCD.Fill(250+10,60+10,550-10,440-10,LGRAY);
				TEXT.show_str_mid(250,250,"获取用户列表失败",24,280);
				OSTimeDlyHMSM(0,0,2,0,OS_OPT_TIME_HMSM_STRICT,&err);
				SYS->AddresseeSel_ENABLE = False;
			}
			else
			{

				LCD.Fill(250+10,60+10,550-10,440-10,LGRAY);
				for(temp = 0;temp<8;temp++)
				{
					POINT_COLOR=MGRAY;
					LCD.DrawLine(250+15,temp*35+110,550-15,temp*35+110);
					if(temp < SYS->UserNum)
					{
						POINT_COLOR=BLACK;
						TEXT.show_str_mid(250+30,temp*35+110-29,(u8*)SYS->UserList[temp]._lstUserName,24,1);
						LCD.ShowNum(500,temp*35+110-29,SYS->UserList[temp]._lstUserID,4,16);
					}				
				}
				POINT_COLOR=GRAY;
				TEXT.show_str_mid(250+20,440-10-25,"上一页",24,1);
				if(SYS->UserNum > 8 ) POINT_COLOR=BLACK;
				TEXT.show_str_mid(458,440-10-25,"下一页",24,1);
			}
			
			while(SYS->AddresseeSel_ENABLE)
			{
				OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);
					
				if(SYS->TP.Status != CurrentTPsta)
				{
					SYS->TP.StaUpdata = False;
					SYS->TP.CoUpdata = False;
					CurrentTPsta = SYS->TP.Status;
					
					if(CurrentTPsta == Press)
					{
						if(SYS->TP.Coordinate_X > 260 && SYS->TP.Coordinate_X < 540 && SYS->TP.Coordinate_Y > 75 && SYS->TP.Coordinate_Y < 110)		temp = 1;
						else if(SYS->TP.Coordinate_X > 260 && SYS->TP.Coordinate_X < 540 && SYS->TP.Coordinate_Y > 110 && SYS->TP.Coordinate_Y < 145)temp = 2;
						else if(SYS->TP.Coordinate_X > 260 && SYS->TP.Coordinate_X < 540 && SYS->TP.Coordinate_Y > 145 && SYS->TP.Coordinate_Y < 180)temp = 3;
						else if(SYS->TP.Coordinate_X > 260 && SYS->TP.Coordinate_X < 540 && SYS->TP.Coordinate_Y > 180 && SYS->TP.Coordinate_Y < 215)temp = 4;
						else if(SYS->TP.Coordinate_X > 260 && SYS->TP.Coordinate_X < 540 && SYS->TP.Coordinate_Y > 215 && SYS->TP.Coordinate_Y < 250)temp = 5;
						else if(SYS->TP.Coordinate_X > 260 && SYS->TP.Coordinate_X < 540 && SYS->TP.Coordinate_Y > 250 && SYS->TP.Coordinate_Y < 285)temp = 6;
						else if(SYS->TP.Coordinate_X > 260 && SYS->TP.Coordinate_X < 540 && SYS->TP.Coordinate_Y > 285 && SYS->TP.Coordinate_Y < 320)temp = 7;
						else if(SYS->TP.Coordinate_X > 260 && SYS->TP.Coordinate_X < 540 && SYS->TP.Coordinate_Y > 320 && SYS->TP.Coordinate_Y < 355)temp = 8;
							
						else if(SYS->TP.Coordinate_X > 240 && SYS->TP.Coordinate_X < 350 && SYS->TP.Coordinate_Y > 380 && SYS->TP.Coordinate_Y < 440)temp = 9;
						else if(SYS->TP.Coordinate_X > 438 && SYS->TP.Coordinate_X < 550 && SYS->TP.Coordinate_Y > 380 && SYS->TP.Coordinate_Y < 440)temp = 10;				
						else temp = 0;
						
						if(temp >= 1 && temp <= 8)
						{
							if((res * 8 + temp) <= SYS->UserNum)  
							{
								SYS->FuncFrame_temp = res * 8 + temp -1;
								debug("select = %s ID = %d\r\n",SYS->UserList[SYS->FuncFrame_temp]._lstUserName,SYS->UserList[SYS->FuncFrame_temp]._lstUserID );
								SYS->AddresseeSel_UPDATA = True;
								SYS->AddresseeSel_ENABLE = False;
							}
						}
						else if(temp == 9)
						{
							if(res) res -= 1;
						}
						else if(temp == 10 )
						{
							if(((res+1)*8) < SYS->UserNum) res += 1;
						}
						
						if(temp)
						{
							LCD.Fill(250+10,60+10,550-10,440-10,LGRAY);
							for(temp = 0;temp<8;temp++)
							{
								POINT_COLOR=MGRAY;
								LCD.DrawLine(250+15,temp*35+110,550-15,temp*35+110);
								if(res*8+temp < SYS->UserNum)
								{
									POINT_COLOR=BLACK;
									TEXT.show_str_mid(250+30,temp*35+110-29,(u8*)SYS->UserList[res * 8 +temp]._lstUserName,24,1);
									LCD.ShowNum(500,temp*35+110-29,SYS->UserList[res * 8 +temp]._lstUserID,4,16);
								}				
							}
													
							if(res)  POINT_COLOR=BLACK;
							else  POINT_COLOR=GRAY;
							TEXT.show_str_mid(250+20,440-10-25,"上一页",24,1);
							
							if((res+1)*8 < SYS->UserNum) POINT_COLOR=BLACK;
							else POINT_COLOR=GRAY;
							TEXT.show_str_mid(458,440-10-25,"下一页",24,1);
						}
						else 
						{
							SYS->AddresseeSel_UPDATA = False;
							SYS->AddresseeSel_ENABLE = False;
						}

					}
					else
					{
						SYS->TP.StaUpdata = False;
						SYS->TP.CoUpdata = False;						
					}
				}
				
			}
			OS_CRITICAL_ENTER();	//进入临界区
			LCD.Color_Fill(250,60,550-1,440-1,page_cache);
			OSMutexPost(&LCDDisplay_MUTEX,OS_OPT_POST_NONE,&err);
			debug("AddresseeSel DISABLE...\r\n");
//			myfree(SRAMEX,page_cache);
			SYS_FREE(page_cache,SRAMEX);
			OS_CRITICAL_EXIT();	//退出临界区
		}
		
		else if(SYS->WIFI_SSID_Get_ENABLE)
		{

			debug("WIFI_SSID_Get ENABLE...\r\n");
			
//			page_cache = mymalloc(SRAMEX,300*380*2);
//			SYS->WIFI_List = mymalloc(SRAMIN,500);
			
			SYS_MALLOC(page_cache,SRAMEX,300*380*2);
			SYS_MALLOC(SYS->WIFI_List,SRAMIN,500);
			SYS->WIFI_SSID_Get_UPDATA = False;
			
			if(!page_cache || !SYS->WIFI_List) SYS->AddresseeSel_ENABLE = False;
			else
			{
				OS_CRITICAL_ENTER();	//进入临界区
				OSMutexPend (&LCDDisplay_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err); //请求屏幕控制资源信号量
				LCD.SelectScreen(ScreenInside);
				for(j=60;j<440;j++)
				{
					for(i=250;i<550;i++)
					{
						page_cache[300*(j-60)+(i-250)] = LCD.ReadPoint(i,j);
					}
				}

				
//				LCD.Color_Fill(250+1,60+1,550-1,440-1,SYS->bg_cache);
				LCD.Fill(250+1,60+1,550-1,440-1,LIGHTBLUE);
				LCD.Fill(250+10,60+10,550-10,440-10,LGRAY);
				POINT_COLOR=BLACK;
				TEXT.show_str_mid(250+10,250,"正在获取WIFI列表...",24,280);
				OS_CRITICAL_EXIT();	//退出临界区	
			}
			
			OSMutexPend (&NetWork_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);//请求WIFI控制资源信号量
			
			if(SYS->WIFI_SSID_Get_ENABLE)
			{
//				WIFI.CommandMode = True;	
				i = 0;
				while(!GetSTA(WIFI.WIFI_STA,WSTA_Mode))
				{
					debug("Setting the WiFi func module as the command mode in \"wifi ssid get()\",try times %d (%x:%x)\r\n",i,WIFI.WIFI_STA,WIFI.WIFI_STA_Expect);
					if(NetWork_WIFI_Control(ModeCom) != 1)
					{
						if(!NetWork_WIFI_Control(Ask))  
						{
							if(NetWork_WIFI_Control(ModeCom) == 1)  SetSTA(WIFI.WIFI_STA,WSTA_Mode);
						}
						else 
						{
							SetSTA(WIFI.WIFI_STA,WSTA_Mode);
						}
					}
					else
					{
						SetSTA(WIFI.WIFI_STA,WSTA_Mode);
					}
					i++;
					if(i>10) 
					{
						SYS->WIFI_SSID_Get_ENABLE = False;
						break;
					}
					OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
				}
			}

			
			if(SYS->WIFI_SSID_Get_ENABLE)
			{
				
				
				j=0;
				while(SYS->WIFI_SSID_Get_ENABLE)
				{
					WIFI.Transmit_Send("AT+SCAN=?\r\n",strlen("AT+SCAN=?\r\n"));	

					i = 0;
					while(!WIFI.Transmit_Receive())
					{
						OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
						i++;
						if(i>100) 
						{
							debug("wifi ssid get timeout\r\n");
							break;
						}
					}
					
	//			}
	//			
	//			if(SYS->WIFI_SSID_Get_ENABLE)
	//			{
					i = 0;
					if(WIFI.TCP_Rxbuf)
					{
						wtemp = (char *)WIFI.TCP_Rxbuf;
//						debug("ssid get %s\r\n\r\n",WIFI.TCP_Rxbuf);
						for(i = 0 ; i < 11 ; i++)
						{
							wtemp = strstr((char *)wtemp,(char *)"SSID");
							if(wtemp == NULL) break;
							
							memset(SYS->WIFI_List[i]._lstWIFISIGNAL,'\0',5);
							memset(SYS->WIFI_List[i]._lstWIFISSID,'\0',30);
						
							wtemp += 5;
							len = strstr((char *)wtemp,(char *)" ") - wtemp;
							strncpy((char *)SYS->WIFI_List[i]._lstWIFISSID,wtemp,len);
							SYS->WIFI_List[i]._lstWIFISSID[len] = '\0';
							
							if(strstr((char *)WIFI.TCP_Rxbuf,(char *)"SIGHAL")) wtemp = strstr((char *)wtemp,(char *)"SIGHAL");
							else if(strstr((char *)WIFI.TCP_Rxbuf,(char *)"SIGNAL")) wtemp = strstr((char *)wtemp,(char *)"SIGNAL");
							
							wtemp += 8;
							len = strstr((char *)wtemp,(char *)" ") - wtemp;
							strncpy((char *)SYS->WIFI_List[i]._lstWIFISIGNAL,wtemp,len);
							SYS->WIFI_List[i]._lstWIFISIGNAL[len] = '\0';

							
							debug("SSID = %s SIGNAL =  %s\r\n",SYS->WIFI_List[i]._lstWIFISSID,SYS->WIFI_List[i]._lstWIFISIGNAL);
						}
//						myfree(SRAMEX,WIFI.TCP_Rxbuf);
						SYS_FREE(WIFI.TCP_Rxbuf,SRAMEX);
					}
					
					SYS->WIFI_ListNum = i;
					if(SYS->WIFI_ListNum)
					{
						OS_CRITICAL_ENTER();	//进入临界区
						LCD.Fill(250+10,60+10,550-10,440-10,LGRAY);
						for(i = 0;i<SYS->WIFI_ListNum;i++)
						{
							POINT_COLOR=MGRAY;
							LCD.DrawLine(250+15,i*30+110,550-15,i*30+110);

							POINT_COLOR=BLACK;
							TEXT.show_str_mid(250+30,i*30+110-29,(u8*)SYS->WIFI_List[i]._lstWIFISSID,24,1);
							TEXT.show_str_mid(500,i*30+110-29,(u8*)SYS->WIFI_List[i]._lstWIFISIGNAL,16,1);
											
						}
						OS_CRITICAL_EXIT();	//临界区
						break;
					}
					else 
					{
						debug("ssid get fail,retry times:%d\r\n",j);
						j++;			
						if(j>5)
						{
							SYS->WIFI_SSID_Get_ENABLE = False;
//							break;
						}
					}
				}
			}
			WIFI.CommandMode = False;
			OSMutexPost(&NetWork_MUTEX,OS_OPT_POST_NONE,&err);//释放信号量	
			
			while(SYS->WIFI_SSID_Get_ENABLE)
			{

				OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);
				
				if(SYS->TP.Status != CurrentTPsta)
				{
					SYS->TP.StaUpdata = False;
					SYS->TP.CoUpdata = False;
					CurrentTPsta = SYS->TP.Status;					
					if(CurrentTPsta == Press)
					{
						if(SYS->TP.Coordinate_X > 260 && SYS->TP.Coordinate_X < 540 && SYS->TP.Coordinate_Y > 75 && SYS->TP.Coordinate_Y < 110)		temp = 1;
						else if(SYS->TP.Coordinate_X > 260 && SYS->TP.Coordinate_X < 540 && SYS->TP.Coordinate_Y > 110 && SYS->TP.Coordinate_Y < 140)temp = 2;
						else if(SYS->TP.Coordinate_X > 260 && SYS->TP.Coordinate_X < 540 && SYS->TP.Coordinate_Y > 140 && SYS->TP.Coordinate_Y < 170)temp = 3;
						else if(SYS->TP.Coordinate_X > 260 && SYS->TP.Coordinate_X < 540 && SYS->TP.Coordinate_Y > 170 && SYS->TP.Coordinate_Y < 200)temp = 4;
						else if(SYS->TP.Coordinate_X > 260 && SYS->TP.Coordinate_X < 540 && SYS->TP.Coordinate_Y > 200 && SYS->TP.Coordinate_Y < 230)temp = 5;
						else if(SYS->TP.Coordinate_X > 260 && SYS->TP.Coordinate_X < 540 && SYS->TP.Coordinate_Y > 230 && SYS->TP.Coordinate_Y < 260)temp = 6;
						else if(SYS->TP.Coordinate_X > 260 && SYS->TP.Coordinate_X < 540 && SYS->TP.Coordinate_Y > 260 && SYS->TP.Coordinate_Y < 290)temp = 7;
						else if(SYS->TP.Coordinate_X > 260 && SYS->TP.Coordinate_X < 540 && SYS->TP.Coordinate_Y > 290 && SYS->TP.Coordinate_Y < 320)temp = 8;
							
						else if(SYS->TP.Coordinate_X > 260 && SYS->TP.Coordinate_X < 540 && SYS->TP.Coordinate_Y > 320 && SYS->TP.Coordinate_Y < 350)temp = 9;
						else if(SYS->TP.Coordinate_X > 260 && SYS->TP.Coordinate_X < 540 && SYS->TP.Coordinate_Y > 350 && SYS->TP.Coordinate_Y < 380)temp = 10;	
						else if(SYS->TP.Coordinate_X > 260 && SYS->TP.Coordinate_X < 540 && SYS->TP.Coordinate_Y > 380 && SYS->TP.Coordinate_Y < 410)temp = 11;	
						else if(SYS->TP.Coordinate_X < 250 || SYS->TP.Coordinate_X > 550 || SYS->TP.Coordinate_Y < 60 || SYS->TP.Coordinate_Y > 450) temp = 0;
							
						
						if(temp) 
						{
//							strcpy((char*)SYS->FuncFrame_temp ,SYS->WIFI_List[temp-1]._lstWIFISSID);
							SYS->FuncFrame_temp = temp-1;
							SYS->WIFI_SSID_Get_UPDATA = True;
						}
						
						SYS->WIFI_SSID_Get_ENABLE = False;
						
					}
				}			
			}
			OS_CRITICAL_ENTER();	//进入临界区
			LCD.Color_Fill(250,60,550-1,440-1,page_cache);
			OSMutexPost(&LCDDisplay_MUTEX,OS_OPT_POST_NONE,&err);
			debug("WIFI_SSID_Get DISABLE...\r\n");
//			myfree(SRAMEX,page_cache);
			SYS_FREE(page_cache,SRAMEX);
			OS_CRITICAL_EXIT();	//退出临界区
			
		}
		else if(SYS->WARNING_ENABLE)
		{
			debug("WARNING NOTICE ENABLE...\r\n");
			
//			page_cache = mymalloc(SRAMEX,300*120*2);
			SYS_MALLOC(page_cache,SRAMEX,300*120*2);
			
			if(!page_cache) SYS->WARNING_ENABLE = False;
			else
			{
				OS_CRITICAL_ENTER();	//进入临界区
				OSMutexPend (&LCDDisplay_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);
				LCD.SelectScreen(ScreenInside);
				for(j=180;j<300;j++)
				{
					for(i=250;i<550;i++)
					{
						page_cache[300*(j-180)+(i-250)] = LCD.ReadPoint(i,j);
					}
				}
				
				LCD.Darker(250,180,550,300,20);
				POINT_COLOR=WHITE;
				TEXT.show_str_mid(250+5,180+5,"提示",24,290);
				TEXT.show_str_mid(250+5,180+40,SYS->FuncFrame_buf,24,290);
				POINT_COLOR=BLACK;
				OS_CRITICAL_EXIT();	//退出临界区	
				
				OSTimeDlyHMSM(0,0,SYS->FuncFrame_temp,0,OS_OPT_TIME_HMSM_STRICT,&err);
				SYS->WARNING_ENABLE = False; 
			}
			OS_CRITICAL_ENTER();	//进入临界区
			LCD.Color_Fill(250,180,550-1,300-1,page_cache);
			OSMutexPost(&LCDDisplay_MUTEX,OS_OPT_POST_NONE,&err);
			debug("WARNING NOTICE DISABLE...\r\n");
//			myfree(SRAMEX,page_cache);
//			myfree(SRAMIN,SYS->FuncFrame_buf);
			SYS_FREE(page_cache,SRAMEX);
			SYS_FREE(SYS->FuncFrame_buf,SRAMIN);
			OS_CRITICAL_EXIT();	//退出临界区

		}
		else if(SYS->INQUIRY_ENABLE)
		{
			debug("INQUIRY ENABLE...\r\n");
			
//			page_cache = mymalloc(SRAMEX,300*120*2);
			SYS_MALLOC(page_cache,SRAMEX,300*120*2);
			
			if(!page_cache) SYS->INQUIRY_ENABLE = False;
			else
			{
				OS_CRITICAL_ENTER();	//进入临界区
				OSMutexPend (&LCDDisplay_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);
				LCD.SelectScreen(ScreenInside);
				for(j=180;j<300;j++)
				{
					for(i=250;i<550;i++)
					{
						page_cache[300*(j-180)+(i-250)] = LCD.ReadPoint(i,j);
					}
				}
				SYS->INQUIRY_ENABLE_UPDATA = False;
				LCD.Fill(250,180,550-1,300-1,BLACK);
				
				LCD.Fill(250,250,400-1,300-1,LGRAY);
				LCD.Fill(400-1,250,550-1,300-1,BLUE);
				POINT_COLOR=WHITE;
				TEXT.show_str_mid(250+5,180+5,"确定选择服务",24,290);
				TEXT.show_str_mid(250+5,180+40,SYS->FuncFrame_buf,24,290);
				TEXT.show_str_mid(250+5,250+5,"取 消",24,150);
				TEXT.show_str_mid(400+5,250+5,"确 定",24,150);
				POINT_COLOR=BLACK;
				OS_CRITICAL_EXIT();	//退出临界区	
				
				while(SYS->INQUIRY_ENABLE){
					OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);
					
				if(SYS->TP.Status != CurrentTPsta)
				{
					SYS->TP.StaUpdata = False;
					SYS->TP.CoUpdata = False;
					CurrentTPsta = SYS->TP.Status;	
					temp = 0;
					if(CurrentTPsta == Press)
					{
						if(SYS->TP.Coordinate_X > 250 && SYS->TP.Coordinate_X < 400 && SYS->TP.Coordinate_Y > 250 && SYS->TP.Coordinate_Y < 300)		temp = 1;
						else if(SYS->TP.Coordinate_X > 400 && SYS->TP.Coordinate_X < 550 && SYS->TP.Coordinate_Y > 250 && SYS->TP.Coordinate_Y < 300)temp = 2;
	
							
						
						if(temp) 
						{
//							strcpy((char*)SYS->FuncFrame_temp ,SYS->WIFI_List[temp-1]._lstWIFISSID);
							SYS->FuncFrame_temp = temp;
							SYS->INQUIRY_ENABLE_UPDATA = True;
							SYS->INQUIRY_ENABLE = False;
						}
						
						
						
					}
				}		
					
				}
			}
			OS_CRITICAL_ENTER();	//进入临界区
			LCD.Color_Fill(250,180,550-1,300-1,page_cache);
			OSMutexPost(&LCDDisplay_MUTEX,OS_OPT_POST_NONE,&err);
			debug("WARNING NOTICE DISABLE...\r\n");
//			myfree(SRAMEX,page_cache);
//			myfree(SRAMIN,SYS->FuncFrame_buf);
			SYS_FREE(page_cache,SRAMEX);
			SYS_FREE(SYS->FuncFrame_buf,SRAMIN);
			OS_CRITICAL_EXIT();	//退出临界区

		}
//		else if(SYS->BACKGROUND_Set_ENABLE)
//		{
//			debug("BACKGROUND Set ENABLE...\r\n");
//			SYS->BACKGROUND_Set_UPDATA = False;
////			page_cache = mymalloc(SRAMEX,340*380*2);
////			Ipath = mymalloc(SRAMIN,50);    //为ICON路径申请内存
//			SYS_MALLOC(page_cache,SRAMEX,340*380*2);
//			SYS_MALLOC(Ipath,SRAMIN,50);
//			
//			if(!page_cache || !Ipath) SYS->BACKGROUND_Set_ENABLE = False;
//			else
//			{
//				OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
//				OS_CRITICAL_ENTER();	//进入临界区
//				OSMutexPend (&LCDDisplay_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);
//				LCD.SelectScreen(ScreenInside);
//				for(j=60;j<440;j++)
//				{
//					for(i=230;i<570;i++)
//					{
//						page_cache[340*(j-60)+(i-230)] = LCD.ReadPoint(i,j);
//					}
//				}

//				
////				LCD.Color_Fill(230+1,60+1,570-1,440-1,SYS->bg_cache);
//				LCD.Fill(230+1,60+1,570-1,440-1,LIGHTBLUE);
//				LCD.Fill(230+10,60+10,570-10,440-10,LGRAY);
//				POINT_COLOR=BLACK;

//				res = 0;
//				num = 0;
//				for(i=0;i<6;i++)
//				{
//					strcpy((char*)Ipath,"1:/");
//					strcat((char*)Ipath,(char*)BG_PicPath[i]);
//					if(!ai_load_picfile(Ipath,260+((i%2)?152:0),90+(i/2*110),128,75,1))
//					{
//						res |= (0x01<<i);
//						if(!strcmp((char*)SYS->InnerJPG_Path,(char*)Ipath))
//						{
//							POINT_COLOR=WHITE;
//							LCD.DrawRectangle(260+((i%2)?152:0)-3,90+(i/2*110)-3,388+((i%2)?152:0)+3,165+(i/2*110)+3);
//							LCD.DrawRectangle(260+((i%2)?152:0)-2,90+(i/2*110)-2,388+((i%2)?152:0)+2,165+(i/2*110)+2);
//							LCD.DrawRectangle(260+((i%2)?152:0)-1,90+(i/2*110)-1,388+((i%2)?152:0)+1,165+(i/2*110)+1);
//							num = i;
//						}
//					}
//					memset(Ipath,'\0',50);
//				}
//				POINT_COLOR=BLACK;
//				TEXT.show_str_mid(240,405,"取消",24,160);
//				TEXT.show_str_mid(400,405,"确定",24,160);
//				
//				debug("display res=%x\r\n",res);
//				OS_CRITICAL_EXIT();	//退出临界区	
//				
//				while(SYS->BACKGROUND_Set_ENABLE)
//				{
//					OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);
//						
//					if(SYS->TP.Status != CurrentTPsta)
//					{
//						SYS->TP.StaUpdata = False;
//						SYS->TP.CoUpdata = False;
//						CurrentTPsta = SYS->TP.Status;
//						
//						if(CurrentTPsta == Press)
//						{
//							if(SYS->TP.Coordinate_X > 260 && SYS->TP.Coordinate_X < 388 && SYS->TP.Coordinate_Y > 90 && SYS->TP.Coordinate_Y < 165)		temp = 1;
//							else if(SYS->TP.Coordinate_X > 412 && SYS->TP.Coordinate_X < 540 && SYS->TP.Coordinate_Y > 90 && SYS->TP.Coordinate_Y < 165)temp = 2;
//							else if(SYS->TP.Coordinate_X > 260 && SYS->TP.Coordinate_X < 388 && SYS->TP.Coordinate_Y > 200 && SYS->TP.Coordinate_Y < 275)temp = 3;
//							else if(SYS->TP.Coordinate_X > 412 && SYS->TP.Coordinate_X < 540 && SYS->TP.Coordinate_Y > 200 && SYS->TP.Coordinate_Y < 275)temp = 4;	
//							else if(SYS->TP.Coordinate_X > 260 && SYS->TP.Coordinate_X < 388 && SYS->TP.Coordinate_Y > 310 && SYS->TP.Coordinate_Y < 385)temp = 5;
//							else if(SYS->TP.Coordinate_X > 412 && SYS->TP.Coordinate_X < 540 && SYS->TP.Coordinate_Y > 310 && SYS->TP.Coordinate_Y < 385)temp = 6;
//							
//							else if(SYS->TP.Coordinate_X > 240 && SYS->TP.Coordinate_X < 400 && SYS->TP.Coordinate_Y > 400 && SYS->TP.Coordinate_Y < 430)temp = 7;
//							else if(SYS->TP.Coordinate_X > 400 && SYS->TP.Coordinate_X < 560 && SYS->TP.Coordinate_Y > 400 && SYS->TP.Coordinate_Y < 430)temp = 8;
//							
//							else if(SYS->TP.Coordinate_X < 230 || SYS->TP.Coordinate_X > 570 || SYS->TP.Coordinate_Y < 60 || SYS->TP.Coordinate_Y > 440) temp = 0;							
//							
//							if(temp >= 1 && temp <= 6 && (res & 1<<(temp-1)))
//							{
//								POINT_COLOR=LGRAY;
//								LCD.DrawRectangle(260+((num%2)?152:0)-3,90+(num/2*110)-3,388+((num%2)?152:0)+3,165+(num/2*110)+3);
//								LCD.DrawRectangle(260+((num%2)?152:0)-2,90+(num/2*110)-2,388+((num%2)?152:0)+2,165+(num/2*110)+2);
//								LCD.DrawRectangle(260+((num%2)?152:0)-1,90+(num/2*110)-1,388+((num%2)?152:0)+1,165+(num/2*110)+1);
//								num = temp-1;
//								POINT_COLOR=WHITE;
//								LCD.DrawRectangle(260+((num%2)?152:0)-3,90+(num/2*110)-3,388+((num%2)?152:0)+3,165+(num/2*110)+3);
//								LCD.DrawRectangle(260+((num%2)?152:0)-2,90+(num/2*110)-2,388+((num%2)?152:0)+2,165+(num/2*110)+2);
//								LCD.DrawRectangle(260+((num%2)?152:0)-1,90+(num/2*110)-1,388+((num%2)?152:0)+1,165+(num/2*110)+1);
//							}	
//							else if(temp == 8)
//							{
//								memset(SYS->InnerJPG_Path,'\0',30);
//								strcpy((char*)SYS->InnerJPG_Path,"1:/");
//								strcat((char*)SYS->InnerJPG_Path,(char*)BG_PicPath[num]);
//								SYS->BACKGROUND_Set_UPDATA = True;
//								SYS->BACKGROUND_Set_ENABLE = False;
//							}
//							else if(temp == 0 || temp == 7) SYS->BACKGROUND_Set_ENABLE = False;
//							
//						}
////					
//					}
//				}
//				OS_CRITICAL_ENTER();	//进入临界区
//				LCD.Color_Fill(230,60,570-1,440-1,page_cache);
//				OSMutexPost(&LCDDisplay_MUTEX,OS_OPT_POST_NONE,&err);
//				debug("BACKGROUND Set DISABLE...\r\n");
////				myfree(SRAMEX,page_cache);
////				myfree(SRAMIN,Ipath);
//				SYS_FREE(page_cache,SRAMEX);
//				SYS_FREE(Ipath,SRAMIN);
//				OS_CRITICAL_EXIT();	//退出临界区
//			}
//		}

	}
		
}						
/***************************************************************************/						


//void malloc_test1(void)
//{
//	u8 *oh_my_god,I_want_to_malloc_every_day_hahaha;
//	u16 *malloc_test_para;
//	
//	SYS_MALLOC(oh_my_god,SRAMIN,7890);
//	SYS_MALLOC(I_want_to_malloc_every_day_hahaha,SRAMEX,6666666);
//	SYS_MALLOC(malloc_test_para,SRAMEX,696699);
//}

//void malloc_testing_function_i_just_want_to_use_a_long_funcname_to_test(void)
//{
//	u8 *oh_my_god,i_just_want_to_use_a_long_value_name_to_test_yeaHAHAHA;
//	u16 *malloc_test_para1;
//	
//	SYS_MALLOC(oh_my_god,SRAMIN,6780);
//	SYS_MALLOC(i_just_want_to_use_a_long_value_name_to_test_yeaHAHAHA,SRAMEX,888888);
//	SYS_MALLOC(malloc_test_para1,SRAMEX,696699);
//}


		





