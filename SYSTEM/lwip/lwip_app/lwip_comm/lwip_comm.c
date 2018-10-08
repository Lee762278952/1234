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


//lwip���ƽṹ�� 
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



extern u32 memp_get_memorysize(void);	//��memp.c���涨��
extern u8_t *memp_memory;				//��memp.c���涨��.
extern u8_t *ram_heap;					//��mem.c���涨��.


//u32 TCPTimer=0;			//TCP��ѯ��ʱ��
//u32 ARPTimer=0;			//ARP��ѯ��ʱ��
//u32 LWIP.LocalTime;		//lwip����ʱ�������,��λ:ms

#if LWIP_DHCP
u32 DHCPfineTimer=0;	//DHCP��ϸ�����ʱ��
u32 DHCPcoarseTimer=0;	//DHCP�ֲڴ����ʱ��
#endif

//lwip��mem��memp���ڴ�����
//����ֵ:0,�ɹ�;
//    ����,ʧ��
u8 lwip_comm_mem_malloc(void)
{
	u32 mempsize;
	u32 ramheapsize; 
	
	lwip_comm_mem_free();
	mempsize=memp_get_memorysize();			//�õ�memp_memory�����С
//	memp_memory=mymalloc(SRAMIN,mempsize);	//Ϊmemp_memory�����ڴ�
	SYS_MALLOC(memp_memory,SRAMIN,mempsize);
	
	ramheapsize=LWIP_MEM_ALIGN_SIZE(MEM_SIZE)+2*LWIP_MEM_ALIGN_SIZE(4*3)+MEM_ALIGNMENT;//�õ�ram heap��С
//	ram_heap=mymalloc(SRAMIN,ramheapsize);	//Ϊram_heap�����ڴ� 
	SYS_MALLOC(ram_heap,SRAMIN,ramheapsize);
	if(!(u32)&memp_memory||!(u32)&ram_heap)//������ʧ�ܵ�
	{
		lwip_comm_mem_free();
		return 1;
	}
	return 0;	
}
//lwip��mem��memp�ڴ��ͷ�
void lwip_comm_mem_free(void)
{ 	
//	myfree(SRAMIN,memp_memory);
//	myfree(SRAMIN,ram_heap);
	SYS_FREE(memp_memory,SRAMIN);
	SYS_FREE(ram_heap,SRAMIN);
	
//	memp_memory = NULL;
//	ram_heap = NULL;
}
//lwip Ĭ��IP����
//lwipx:lwip���ƽṹ��ָ��
void lwip_comm_default_ip_set(_lwip_dev *lwipx)
{
	u32 sn0;
	sn0=*(vu32*)(0x1FFF7A10);//��ȡSTM32��ΨһID��ǰ24λ��ΪMAC��ַ�����ֽ�
	//Ĭ��Զ��IPΪ:192.168.1.100
//	lwipx->ServerIP[0]=172;	
//	lwipx->ServerIP[1]=16;
//	lwipx->ServerIP[2]=4;
//	lwipx->ServerIP[3]=220;
	//MAC��ַ����(�����ֽڹ̶�Ϊ:2.0.0,�����ֽ���STM32ΨһID)
	lwipx->MAC[0]=2;//�����ֽ�(IEEE��֮Ϊ��֯ΨһID,OUI)��ַ�̶�Ϊ:2.0.0
	lwipx->MAC[1]=0;
	lwipx->MAC[2]=0;
	lwipx->MAC[3]=(sn0>>16)&0XFF;//�����ֽ���STM32��ΨһID
	lwipx->MAC[4]=(sn0>>8)&0XFFF;
	lwipx->MAC[5]=sn0&0XFF; 
//	//Ĭ�ϱ���IPΪ:192.168.1.30
//	lwipx->LocalIP[0]=192;	
//	lwipx->LocalIP[1]=168;
//	lwipx->LocalIP[2]=1;
//	lwipx->LocalIP[3]=30;
	//Ĭ����������:255.255.255.0
	lwipx->NetMask[0]=255;	
	lwipx->NetMask[1]=255;
	lwipx->NetMask[2]=255;
	lwipx->NetMask[3]=0;
	//Ĭ������:192.168.1.1
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


//LWIP��ʼ��(LWIP������ʱ��ʹ��)
//����ֵ:0,�ɹ�
//      1,�ڴ����
//      2,LAN8720��ʼ��ʧ��
//      3,�������ʧ��.
u8 lwip_comm_init(void)
{
    u8 retry=0;
	struct netif *Netif_Init_Flag;		//����netif_add()����ʱ�ķ���ֵ,�����ж������ʼ���Ƿ�ɹ�
	struct ip_addr ipaddr;  			//ip��ַ
	struct ip_addr netmask; 			//��������
	struct ip_addr gw;      			//Ĭ������ 

	if(ETH_Mem_Malloc())return 1;		//�ڴ�����ʧ��
	if(lwip_comm_mem_malloc())return 2;	//�ڴ�����ʧ��
	lwip_comm_default_ip_set(&LWIP);	//����Ĭ��IP����
	while(LAN8720_Init())		        //��ʼ��LAN8720,���ʧ�ܵĻ�������1��
	{
			retry++;
			if(retry>2) {retry=0;return 3;} //LAN8720��ʼ��ʧ��
	}

	lwip_init();						//��ʼ��LWIP�ں�

	

	//#if LWIP_DHCP		//ʹ�ö�̬IP
	if(LWIP.DHCP)
	{
		ipaddr.addr = 0;
		netmask.addr = 0;
		gw.addr = 0;
	}
	else			//ʹ�þ�̬IP
	{
		IP4_ADDR(&ipaddr,LWIP.LocalIP[0],LWIP.LocalIP[1],LWIP.LocalIP[2],LWIP.LocalIP[3]);
		IP4_ADDR(&netmask,LWIP.NetMask[0],LWIP.NetMask[1] ,LWIP.NetMask[2],LWIP.NetMask[3]);
		IP4_ADDR(&gw,LWIP.GateWay[0],LWIP.GateWay[1],LWIP.GateWay[2],LWIP.GateWay[3]);
	//	printf("����en��MAC��ַΪ:................%d.%d.%d.%d.%d.%d\r\n",LWIP.mac[0],LWIP.mac[1],LWIP.mac[2],LWIP.mac[3],LWIP.mac[4],LWIP.mac[5]);
	//	printf("��̬IP��ַ........................%d.%d.%d.%d\r\n",LWIP.LocalIP[0],LWIP.LocalIP[1],LWIP.LocalIP[2],LWIP.LocalIP[3]);
	//	printf("��������..........................%d.%d.%d.%d\r\n",LWIP.NetMask[0],LWIP.NetMask[1],LWIP.NetMask[2],LWIP.NetMask[3]);
	//	printf("Ĭ������..........................%d.%d.%d.%d\r\n",LWIP.GateWay[0],LWIP.GateWay[1],LWIP.GateWay[2],LWIP.GateWay[3]);
	}
	Netif_Init_Flag=netif_add(&LWIP.lwip_netif,&ipaddr,&netmask,&gw,NULL,&ethernetif_init,&ethernet_input);//�������б������һ������
	
	if(LWIP.DHCP)
	{
//	LWIP.DHCP_STA=0;	//DHCP���Ϊ0
	 dhcp_start(&LWIP.lwip_netif);	//����DHCP����
	}
	else LWIP.DHCP_STA = 0xFF;
	
	if(Netif_Init_Flag==NULL)return 4;//�������ʧ�� 
	else//������ӳɹ���,����netifΪĬ��ֵ,���Ҵ�netif����
	{
		netif_set_default(&LWIP.lwip_netif); //����netifΪĬ������
		netif_set_up(&LWIP.lwip_netif);		//��netif����
	}
	return 0;//����OK.
}   

//�����յ����ݺ���� 
void lwip_pkt_handle(void)
{
  //�����绺�����ж�ȡ���յ������ݰ������䷢�͸�LWIP���� 
 ethernetif_input(&LWIP.lwip_netif);
}

//LWIP��ѯ����
void lwip_periodic_handle(void)
{
   sys_check_timeouts();	
	if(LWIP.DHCP)
	{
		//ÿ500ms����һ��dhcp_fine_tmr()
		if (LWIP.LocalTime - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)
		{
			DHCPfineTimer =  LWIP.LocalTime;
			dhcp_fine_tmr();
			if ((LWIP.DHCP_STA != 2)&&(LWIP.DHCP_STA != 0XFF))
			{ 
				lwip_dhcp_process_handle();  //DHCP����
			}
		}

		//ÿ60sִ��һ��DHCP�ֲڴ���
		if (LWIP.LocalTime - DHCPcoarseTimer >= DHCP_COARSE_TIMER_MSECS)
		{
			DHCPcoarseTimer =  LWIP.LocalTime;
			dhcp_coarse_tmr();
		}  
	}
}


//���ʹ����DHCP
#if LWIP_DHCP

//DHCP��������
void lwip_dhcp_process_handle(void)
{
	u32 ip=0,netmask=0,gw=0;
	switch(LWIP.DHCP_STA)
	{
		case 0: 	//����DHCP
			dhcp_start(&LWIP.lwip_netif);
			LWIP.DHCP_STA = 1;		//�ȴ�ͨ��DHCP��ȡ���ĵ�ַ
			printf("Looking for DHCP server.....\r\n");  
			break;
		case 1:		//�ȴ���ȡ��IP��ַ
		{
			ip=LWIP.lwip_netif.ip_addr.addr;		//��ȡ��IP��ַ
			netmask=LWIP.lwip_netif.netmask.addr;//��ȡ��������
			gw=LWIP.lwip_netif.gw.addr;			//��ȡĬ������ 
			
			if(ip!=0)			//��ȷ��ȡ��IP��ַ��ʱ��
			{
				LWIP.DHCP_STA=2;	//DHCP�ɹ�
				
				//������ͨ��DHCP��ȡ����IP��ַ
				LWIP.LocalIP[3]=(uint8_t)(ip>>24); 
				LWIP.LocalIP[2]=(uint8_t)(ip>>16);
				LWIP.LocalIP[1]=(uint8_t)(ip>>8);
				LWIP.LocalIP[0]=(uint8_t)(ip);
//				
				//����ͨ��DHCP��ȡ�������������ַ
				LWIP.NetMask[3]=(uint8_t)(netmask>>24);
				LWIP.NetMask[2]=(uint8_t)(netmask>>16);
				LWIP.NetMask[1]=(uint8_t)(netmask>>8);
				LWIP.NetMask[0]=(uint8_t)(netmask);
//				
				//������ͨ��DHCP��ȡ����Ĭ������
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
			else if(LWIP.lwip_netif.dhcp->tries>LWIP_MAX_DHCP_TRIES) //ͨ��DHCP�����ȡIP��ַʧ��,�ҳ�������Դ���
			{
				LWIP.DHCP_STA=0XFF;//DHCP��ʱʧ��.
				printf("DHCP fail !!\r\n");
				//ʹ�þ�̬IP��ַ
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

//opt: 1=exģʽ 2=��ͨģʽ
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

























