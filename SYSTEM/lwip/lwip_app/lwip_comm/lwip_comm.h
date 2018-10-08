#ifndef _LWIP_COMM_H
#define _LWIP_COMM_H 
#include "tcp_client_demo.h" 
#include "sys.h"
   
 
#define LWIP_MAX_DHCP_TRIES		4   //DHCP服务器最大重试次数
   




//lwip控制结构体
typedef struct __lwip_dev
{
	_BOOL NetConnect_Rdy;
	_BOOL DHCP;
	vu8 DHCP_STA;	
						//dhcp状态 
					//0,未获取DHCP地址;
					//1,进入DHCP获取状态
					//2,成功获取DHCP地址
					//0XFF,IP获取失败	
	u8 MAC[6];      //MAC地址
	u8 ServerIP[4];	//远端主机IP地址 
	u16 ServerPort;
	u8 LocalIP[4];       //本机IP地址
	u8 NetMask[4]; 	//子网掩码
	u8 GateWay[4]; 	//默认网关的IP地址
	
	u32 LocalTime;
	
	struct netif lwip_netif;				//定义一个全局的网络接口
	
//	_tcpclient_class TCPCLIEN;
	

	void (*PeriodicHandle)(void);
	void (*SetPara)(struct __lwip_dev *);
	u8 (*MEM_Malloc)(void);
	void (*MEM_Free)(void);
	u8 (*Init)(void);
	_BOOL (*NETLinkCheck)(void);
  void (*DeInit)(void);
  _BOOL (*Buf_Set)(u8 );

}_lwip_dev;

extern _lwip_dev LWIP;	//lwip控制结构体

//extern u32 lwip_localtime;


#endif













