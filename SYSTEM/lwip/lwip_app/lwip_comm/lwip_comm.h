#ifndef _LWIP_COMM_H
#define _LWIP_COMM_H 
#include "tcp_client_demo.h" 
#include "sys.h"
   
 
#define LWIP_MAX_DHCP_TRIES		4   //DHCP������������Դ���
   




//lwip���ƽṹ��
typedef struct __lwip_dev
{
	_BOOL NetConnect_Rdy;
	_BOOL DHCP;
	vu8 DHCP_STA;	
						//dhcp״̬ 
					//0,δ��ȡDHCP��ַ;
					//1,����DHCP��ȡ״̬
					//2,�ɹ���ȡDHCP��ַ
					//0XFF,IP��ȡʧ��	
	u8 MAC[6];      //MAC��ַ
	u8 ServerIP[4];	//Զ������IP��ַ 
	u16 ServerPort;
	u8 LocalIP[4];       //����IP��ַ
	u8 NetMask[4]; 	//��������
	u8 GateWay[4]; 	//Ĭ�����ص�IP��ַ
	
	u32 LocalTime;
	
	struct netif lwip_netif;				//����һ��ȫ�ֵ�����ӿ�
	
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

extern _lwip_dev LWIP;	//lwip���ƽṹ��

//extern u32 lwip_localtime;


#endif













