#include "lwip_comm.h" 
#include "lan8720.h" 
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/init.h"
#include "ethernetif.h" 
#include "lwip/timers.h"
#include "lwip/tcp_impl.h"
#include "lwip/ip_frag.h"
#include "lwip/tcpip.h" 
#include "lwip/timers.h"
#include "sys_malloc.h"
#include "delay.h"
#include "usart.h" 
#include <stdio.h>

#include "sysdebug.h"
 
#if ApolloDev
#include "pcf8574.h"
#endif



void lwip_pkt_handle(void);
void lwip_periodic_handle(void);
	
void lwip_comm_default_ip_set(_lwip_dev *lwipx);
u8 lwip_comm_mem_malloc(void);
void lwip_comm_mem_free(void);
u8 lwip_comm_init(void);
void lwip_dhcp_process_handle(void);
void lwip_deinit(void);
static _BOOL LWIP_TCPBUF_SET(u8 opt);


//lwip控制结构体 
_lwip_dev LWIP = 
{
	.DHCP_STA = 0,
	.LocalTime = 0,
	

	
	.PeriodicHandle = lwip_periodic_handle,
	.SetPara = lwip_comm_default_ip_set,
	.MEM_Malloc = lwip_comm_mem_malloc,
	.MEM_Free = lwip_comm_mem_free,
	.Init = lwip_comm_init,
	.NETLinkCheck = LAN8720_LinkCheck,
	.DeInit = lwip_deinit,
	.Buf_Set = LWIP_TCPBUF_SET,
};



extern u32 memp_get_memorysize(void);	//在memp.c里面定义
extern u8_t *memp_memory;				//在memp.c里面定义.
extern u8_t *ram_heap;					//在mem.c里面定义.


//u32 TCPTimer=0;			//TCP查询计时器
//u32 ARPTimer=0;			//ARP查询计时器
//u32 LWIP.LocalTime;		//lwip本地时间计数器,单位:ms

#if LWIP_DHCP
u32 DHCPfineTimer=0;	//DHCP精细处理计时器
u32 DHCPcoarseTimer=0;	//DHCP粗糙处理计时器
#endif

//lwip中mem和memp的内存申请
//返回值:0,成功;
//    其他,失败
u8 lwip_comm_mem_malloc(void)
{
	u32 mempsize;
	u32 ramheapsize; 
	
	lwip_comm_mem_free();
	mempsize=memp_get_memorysize();			//得到memp_memory数组大小
//	memp_memory=mymalloc(SRAMIN,mempsize);	//为memp_memory申请内存
	SYS_MALLOC(memp_memory,SRAMIN,mempsize);
	
	ramheapsize=LWIP_MEM_ALIGN_SIZE(MEM_SIZE)+2*LWIP_MEM_ALIGN_SIZE(4*3)+MEM_ALIGNMENT;//得到ram heap大小
//	ram_heap=mymalloc(SRAMIN,ramheapsize);	//为ram_heap申请内存 
	SYS_MALLOC(ram_heap,SRAMIN,ramheapsize);
	if(!(u32)&memp_memory||!(u32)&ram_heap)//有申请失败的
	{
		lwip_comm_mem_free();
		return 1;
	}
	return 0;	
}
//lwip中mem和memp内存释放
void lwip_comm_mem_free(void)
{ 	
//	myfree(SRAMIN,memp_memory);
//	myfree(SRAMIN,ram_heap);
	SYS_FREE(memp_memory,SRAMIN);
	SYS_FREE(ram_heap,SRAMIN);
	
//	memp_memory = NULL;
//	ram_heap = NULL;
}
//lwip 默认IP设置
//lwipx:lwip控制结构体指针
void lwip_comm_default_ip_set(_lwip_dev *lwipx)
{
	u32 sn0;
	sn0=*(vu32*)(0x1FFF7A10);//获取STM32的唯一ID的前24位作为MAC地址后三字节
	//默认远端IP为:192.168.1.100
//	lwipx->ServerIP[0]=172;	
//	lwipx->ServerIP[1]=16;
//	lwipx->ServerIP[2]=4;
//	lwipx->ServerIP[3]=220;
	//MAC地址设置(高三字节固定为:2.0.0,低三字节用STM32唯一ID)
	lwipx->MAC[0]=2;//高三字节(IEEE称之为组织唯一ID,OUI)地址固定为:2.0.0
	lwipx->MAC[1]=0;
	lwipx->MAC[2]=0;
	lwipx->MAC[3]=(sn0>>16)&0XFF;//低三字节用STM32的唯一ID
	lwipx->MAC[4]=(sn0>>8)&0XFFF;
	lwipx->MAC[5]=sn0&0XFF; 
//	//默认本地IP为:192.168.1.30
//	lwipx->LocalIP[0]=192;	
//	lwipx->LocalIP[1]=168;
//	lwipx->LocalIP[2]=1;
//	lwipx->LocalIP[3]=30;
	//默认子网掩码:255.255.255.0
	lwipx->NetMask[0]=255;	
	lwipx->NetMask[1]=255;
	lwipx->NetMask[2]=255;
	lwipx->NetMask[3]=0;
	//默认网关:192.168.1.1
	lwipx->GateWay[0]=lwipx->LocalIP[0];	
	lwipx->GateWay[1]=lwipx->LocalIP[1];
	lwipx->GateWay[2]=lwipx->LocalIP[2];
	lwipx->GateWay[3]=1;	
	
//	lwipx->ServerPort = 1117;

} 

void lwip_deinit(void)
{

	if(LWIP.DHCP)
	{
//		dhcp_release(&LWIP.lwip_netif);
		dhcp_stop(&LWIP.lwip_netif);
		dhcp_cleanup(&LWIP.lwip_netif);
	}
		netif_set_down(&LWIP.lwip_netif);
	netif_remove(&LWIP.lwip_netif);
	LAN8270_DeInit();
	sys_restart_timeouts();
	
}


//LWIP初始化(LWIP启动的时候使用)
//返回值:0,成功
//      1,内存错误
//      2,LAN8720初始化失败
//      3,网卡添加失败.
u8 lwip_comm_init(void)
{
    u8 retry=0;
	struct netif *Netif_Init_Flag;		//调用netif_add()函数时的返回值,用于判断网络初始化是否成功
	struct ip_addr ipaddr;  			//ip地址
	struct ip_addr netmask; 			//子网掩码
	struct ip_addr gw;      			//默认网关 

	if(ETH_Mem_Malloc())return 1;		//内存申请失败
	if(lwip_comm_mem_malloc())return 2;	//内存申请失败
	lwip_comm_default_ip_set(&LWIP);	//设置默认IP等信
	while(LAN8720_Init())		        //初始化LAN8720,如果失败的话就重试1次
	{
			retry++;
			if(retry>2) {retry=0;return 3;} //LAN8720初始化失败
	}

	lwip_init();						//初始化LWIP内核

	

	//#if LWIP_DHCP		//使用动态IP
	if(LWIP.DHCP)
	{
		ipaddr.addr = 0;
		netmask.addr = 0;
		gw.addr = 0;
	}
	else			//使用静态IP
	{
		IP4_ADDR(&ipaddr,LWIP.LocalIP[0],LWIP.LocalIP[1],LWIP.LocalIP[2],LWIP.LocalIP[3]);
		IP4_ADDR(&netmask,LWIP.NetMask[0],LWIP.NetMask[1] ,LWIP.NetMask[2],LWIP.NetMask[3]);
		IP4_ADDR(&gw,LWIP.GateWay[0],LWIP.GateWay[1],LWIP.GateWay[2],LWIP.GateWay[3]);
	//	printf("网卡en的MAC地址为:................%d.%d.%d.%d.%d.%d\r\n",LWIP.mac[0],LWIP.mac[1],LWIP.mac[2],LWIP.mac[3],LWIP.mac[4],LWIP.mac[5]);
	//	printf("静态IP地址........................%d.%d.%d.%d\r\n",LWIP.LocalIP[0],LWIP.LocalIP[1],LWIP.LocalIP[2],LWIP.LocalIP[3]);
	//	printf("子网掩码..........................%d.%d.%d.%d\r\n",LWIP.NetMask[0],LWIP.NetMask[1],LWIP.NetMask[2],LWIP.NetMask[3]);
	//	printf("默认网关..........................%d.%d.%d.%d\r\n",LWIP.GateWay[0],LWIP.GateWay[1],LWIP.GateWay[2],LWIP.GateWay[3]);
	}
	Netif_Init_Flag=netif_add(&LWIP.lwip_netif,&ipaddr,&netmask,&gw,NULL,&ethernetif_init,&ethernet_input);//向网卡列表中添加一个网口
	
	if(LWIP.DHCP)
	{
//	LWIP.DHCP_STA=0;	//DHCP标记为0
	 dhcp_start(&LWIP.lwip_netif);	//开启DHCP服务
	}
	else LWIP.DHCP_STA = 0xFF;
	
	if(Netif_Init_Flag==NULL)return 4;//网卡添加失败 
	else//网口添加成功后,设置netif为默认值,并且打开netif网口
	{
		netif_set_default(&LWIP.lwip_netif); //设置netif为默认网口
		netif_set_up(&LWIP.lwip_netif);		//打开netif网口
	}
	return 0;//操作OK.
}   

//当接收到数据后调用 
void lwip_pkt_handle(void)
{
  //从网络缓冲区中读取接收到的数据包并将其发送给LWIP处理 
 ethernetif_input(&LWIP.lwip_netif);
}

//LWIP轮询任务
void lwip_periodic_handle(void)
{
   sys_check_timeouts();	
	if(LWIP.DHCP)
	{
		//每500ms调用一次dhcp_fine_tmr()
		if (LWIP.LocalTime - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)
		{
			DHCPfineTimer =  LWIP.LocalTime;
			dhcp_fine_tmr();
			if ((LWIP.DHCP_STA != 2)&&(LWIP.DHCP_STA != 0XFF))
			{ 
				lwip_dhcp_process_handle();  //DHCP处理
			}
		}

		//每60s执行一次DHCP粗糙处理
		if (LWIP.LocalTime - DHCPcoarseTimer >= DHCP_COARSE_TIMER_MSECS)
		{
			DHCPcoarseTimer =  LWIP.LocalTime;
			dhcp_coarse_tmr();
		}  
	}
}


//如果使能了DHCP
#if LWIP_DHCP

//DHCP处理任务
void lwip_dhcp_process_handle(void)
{
	u32 ip=0,netmask=0,gw=0;
	switch(LWIP.DHCP_STA)
	{
		case 0: 	//开启DHCP
			dhcp_start(&LWIP.lwip_netif);
			LWIP.DHCP_STA = 1;		//等待通过DHCP获取到的地址
			printf("Looking for DHCP server.....\r\n");  
			break;
		case 1:		//等待获取到IP地址
		{
			ip=LWIP.lwip_netif.ip_addr.addr;		//读取新IP地址
			netmask=LWIP.lwip_netif.netmask.addr;//读取子网掩码
			gw=LWIP.lwip_netif.gw.addr;			//读取默认网关 
			
			if(ip!=0)			//正确获取到IP地址的时候
			{
				LWIP.DHCP_STA=2;	//DHCP成功
				
				//解析出通过DHCP获取到的IP地址
				LWIP.LocalIP[3]=(uint8_t)(ip>>24); 
				LWIP.LocalIP[2]=(uint8_t)(ip>>16);
				LWIP.LocalIP[1]=(uint8_t)(ip>>8);
				LWIP.LocalIP[0]=(uint8_t)(ip);
//				
				//解析通过DHCP获取到的子网掩码地址
				LWIP.NetMask[3]=(uint8_t)(netmask>>24);
				LWIP.NetMask[2]=(uint8_t)(netmask>>16);
				LWIP.NetMask[1]=(uint8_t)(netmask>>8);
				LWIP.NetMask[0]=(uint8_t)(netmask);
//				
				//解析出通过DHCP获取到的默认网关
				LWIP.GateWay[3]=(uint8_t)(gw>>24);
				LWIP.GateWay[2]=(uint8_t)(gw>>16);
				LWIP.GateWay[1]=(uint8_t)(gw>>8);
				LWIP.GateWay[0]=(uint8_t)(gw);
//				
			
				printf("DHCP sucess !!\r\n");			
				printf("MAC:%d.%d.%d.%d.%d.%d\r\n",LWIP.MAC[0],LWIP.MAC[1],LWIP.MAC[2],LWIP.MAC[3],LWIP.MAC[4],LWIP.MAC[5]);
				printf("DHCP get IP:%d.%d.%d.%d\r\n",LWIP.LocalIP[0],LWIP.LocalIP[1],LWIP.LocalIP[2],LWIP.LocalIP[3]);
				printf("DHCP get NetMask:%d.%d.%d.%d\r\n",LWIP.NetMask[0],LWIP.NetMask[1],LWIP.NetMask[2],LWIP.NetMask[3]);
				printf("DHCP get GateWay:%d.%d.%d.%d\r\n",LWIP.GateWay[0],LWIP.GateWay[1],LWIP.GateWay[2],LWIP.GateWay[3]);
			
			}
			else if(LWIP.lwip_netif.dhcp->tries>LWIP_MAX_DHCP_TRIES) //通过DHCP服务获取IP地址失败,且超过最大尝试次数
			{
				LWIP.DHCP_STA=0XFF;//DHCP超时失败.
				printf("DHCP fail !!\r\n");
				//使用静态IP地址
				IP4_ADDR(&(LWIP.lwip_netif.ip_addr),LWIP.LocalIP[0],LWIP.LocalIP[1],LWIP.LocalIP[2],LWIP.LocalIP[3]);
				IP4_ADDR(&(LWIP.lwip_netif.netmask),LWIP.NetMask[0],LWIP.NetMask[1],LWIP.NetMask[2],LWIP.NetMask[3]);
				IP4_ADDR(&(LWIP.lwip_netif.gw),LWIP.GateWay[0],LWIP.GateWay[1],LWIP.GateWay[2],LWIP.GateWay[3]);

				printf("MAC:%d.%d.%d.%d.%d.%d\r\n",LWIP.MAC[0],LWIP.MAC[1],LWIP.MAC[2],LWIP.MAC[3],LWIP.MAC[4],LWIP.MAC[5]);
				printf("static IP:%d.%d.%d.%d\r\n",LWIP.LocalIP[0],LWIP.LocalIP[1],LWIP.LocalIP[2],LWIP.LocalIP[3]);
				printf("NetMask:%d.%d.%d.%d\r\n",LWIP.NetMask[0],LWIP.NetMask[1],LWIP.NetMask[2],LWIP.NetMask[3]);
				printf("GateWay:%d.%d.%d.%d\r\n",LWIP.GateWay[0],LWIP.GateWay[1],LWIP.GateWay[2],LWIP.GateWay[3]);
				
			}
		}
		break;
		default : break;
	}
}
#endif 

//opt: 1=ex模式 2=普通模式
static _BOOL LWIP_TCPBUF_SET(u8 opt)
{
	if(opt)//ex mode
	{
//		myfree(SRAMEX,TCPCLIEN.Rx_Buffer);
//			TCPCLIEN.Rx_Buffer = NULL;
		SYS_FREE(TCPCLIEN.Rx_Buffer,SRAMEX);

//		TCPCLIEN.Rx_Buffer = mymalloc(SRAMEX,TCP_CLIENT_RX_BUFSIZE_Ex);
		SYS_MALLOC(TCPCLIEN.Rx_Buffer,SRAMEX,TCP_CLIENT_RX_BUFSIZE_Ex);

		TCPCLIEN.BufferSize = TCP_CLIENT_RX_BUFSIZE_Ex;
		if(TCPCLIEN.Rx_Buffer == NULL)
		{
			TCPCLIEN.BufferSize = TCP_CLIENT_RX_BUFSIZE;
//			TCPCLIEN.Rx_Buffer = mymalloc(SRAMEX,TCP_CLIENT_RX_BUFSIZE);
			SYS_MALLOC(TCPCLIEN.Rx_Buffer,SRAMEX,TCP_CLIENT_RX_BUFSIZE);
			return False;
		}
		else return True;
	}
	else
	{
		TCPCLIEN.BufferSize = TCP_CLIENT_RX_BUFSIZE;
//		myfree(SRAMEX,TCPCLIEN.Rx_Buffer);
		SYS_FREE(TCPCLIEN.Rx_Buffer,SRAMEX);

		TCPCLIEN.Rx_Buffer = NULL;
//		TCPCLIEN.Rx_Buffer = mymalloc(SRAMEX,TCP_CLIENT_RX_BUFSIZE);
		SYS_MALLOC(TCPCLIEN.Rx_Buffer,SRAMEX,TCP_CLIENT_RX_BUFSIZE);
		return True;
	}
	
}

























