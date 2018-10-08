#ifndef _WIFI_H
#define _WIFI_H
#include "sys.h"
#include "FN6110R.h"

#define CommandDebug 0

#define  WSTA_Launch		0x01
#define  WSTA_Mode			0x02
#define  WSTA_Connect		0x04
#define  WSTA_Tcp				0x08

#define WIFI_UartBuf_LEN    	UART4_REC_LEN
#define WIFI_UartBuf_LEN_Ex		500*1024

//#define WIFI.CallBack_Process()  WIFI.CallBack_Process(NULL,NULL,NULL)

typedef enum
{
	station = 1,
	softap = 2,
	smart = 3,
}_wifi_mode;

typedef enum
{
	W_NULL = 0,
	W_OK,
	W_ERROR,
	W_FAIL,
	W_TCP_READY,
	W_TCP_RST,
	W_START,
	W_RECONNECT,
	W_SERVER,//�������·���Ϣ
	
}_wifi_callback;
 

typedef struct
{
	u8 WIFI_STA; 
	u8 WIFI_STA_Expect;//wifi״̬����ֵ��ͨ����������ֵ����ʹ����������ֵȥά��WIFIģ���״̬
	/***************************************************
	 WIFI_STA:
		bit 0:  0=ģ��δ����		1=ģ��������
		bit 1:  0=͸��ģʽ   	1=����ģʽ
		bit 2:	0=WIFIδ����		1=WIFI������
		bit 3:  0=TCPδ����   	1=TCP������
	     
		....retention
	****************************************************/
	_BOOL CommandMode;
	
	u8 *TCP_Rxbuf;
	u32 ReceiveLen;
//	u8 *TCP_Txbuf;
	
	u8 Current_SSID[30];
	u8 Current_PWD[30];
//	u8 Current_IPadd[20];
//	u8 Current_Port[5];
//	u8 Current_DeviceIP[20];
	
	void (*Config)(void);
	void (*Mode_Set)(_wifi_mode );
	void (*TransmitSet)(u8 );
	void (*Test)(void);
	void (*SSID_Set)(u8 *);
	void (*PWD_Set)(u8 *);
	void (*Connect_Func)(u8 );
	void (*TCPClinet_Set)(u8 *,u8 *,u8 );
	_wifi_callback (*CallBack_Process)(u8 *,u8 *,u8 *);
	_BOOL (*Transmit_Send)(u8 *,u32);
	void (*Disable_Set)(void);
  void (*Enable_Set)(void);
	void (*Scan_AP)(u8 ,u8 *);
	void (*Get_Mac)(void);
	_BOOL (*Transmit_Receive)(void);
	_BOOL (*UartBuf_Set)(u8 );
	void 	(*StaIP_Handle)(u8,u8*,u8);
	
}_wifi_class;

extern _wifi_class WIFI;



#endif
