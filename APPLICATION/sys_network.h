#ifndef _NETWORK_H
#define _NETWORK_H
#include "sys.h"
#include "wifi.h"

#define NET_WIFI_DEBUG  		0
#define NetWork_Test 				0



//�����ֶ� ���������������
#define iCmdEnum 				"iCmdEnum"
//�豸ID ����Ϣ��Ҫ����
#define iDeviceID 			"iDeviceID"

//����
#define iElectric 			"iElectric"
#define iLuminance 			"iLuminance"
#define iWifi 					"iWifi"

//�豸ע�� MAC�ַ���C->S
#define strMac 					"strMac"
//ע�ᷴ��S->C
#define iResult 				"iResult"
#define strErrorMsg		 	"strErrorMsg"
//�·�������ϢS->C
#define iMeetingID 			"iMeetingID"
#define strName 				"strName"
#define strSlogan 			"strSlogan"
#define strContent 			"strContent"
#define strStartTime 		"strStartTime"
#define strEndTime 			"strEndTime"
//�����û���Ϣ ��������������ͼƬURL
#define iChairman 				"iChairman"
#define iSecretary 				"iSecretary"
#define strUserName 			"strUserName"
#define strCompany 				"strCompany"
#define strPosition 			"strPosition"
#define strFormat 				"strFormat"
#define strWelcome 				"strWelcome"
#define strNameplateUrl 	"strNameplateUrl"
#define strNameplateBGUrl "strNameplateBGUrl"

//�û��б�����
#define iUserNum 				"iUserNum"
#define lstDevice 			"lstDevice"
#define strUserName 		"strUserName"
//�������
#define iServiceID 			"iServiceID"
//����Ϣ
#define iSenderID 			"iSenderID"
#define iMsgType 				"iMsgType"
#define iContentType 		"iContentType"
#define lstRecvDeviceID "lstRecvDeviceID"
#define iReceiverID 		"iReceiverID"
//���п���
#define iControlType 		"iControlType"

//C->S������
#define REQ_HEARTBEAT 					101
#define REQ_TS_DEVICE_REG 			201
#define REQ_TS_GET_USERINFO 		207
#define REQ_TS_GET_USERLIST 		209
#define EVT_TS_REQSERVICE 			211
//S->C������
#define RSP_HEARTBEAT 					102
#define RSP_TS_DEVICE_REG 			202
#define EVT_TS_MEETINGINFO 			203
#define EVT_TS_MEETING_BEGIN 		204
#define EVT_TS_MEETING_END			205
#define EVT_TS_QUIT_MEETING 		206
#define RSP_TS_GET_USERINFO 		208
#define RSP_TS_GET_USERLIST 		210
#define EVT_TS_REQSERVICE_ACK 	212
#define EVT_TS_CENTERCONTROL 		214
#define EVT_TS_USERSTATUS 			215
#define EVT_TS_NAMEPLATE_UPDATE 216
//C->S->C������
#define EVT_TS_SENDMSG 					213

typedef enum
{
	Restart = 1,
	Ask,
	ModeCom,
	ConnectSta,
	TcpSta,
	StationSet,
	SsidSet,
	PwdSet,
	TcpSet,
	ConnectEn,
	ModeTran,
	ApSignal,
	GetMac,
	GetIp,
	LocalIp,
	SetDhcp,
	
}_wifi_control;

typedef enum
{
	NET_AUTO = 0,
	NET_LWIP,
	NET_WIFI,
	
}_network_mode;

typedef struct
{
	_network_mode Mode;
	
	_BOOL WIFI_TransRdy;
	_BOOL LWIP_TransRdy;
	
	_BOOL WIFI_STA_UPdata;
	
	u8 *TransmitBuf;
	u32 TransmitBufLen;
	
	_BOOL (*Transmit_Send)(u8 *,u32);
	_BOOL (*Transmit_Receive)(void);
	
	u8 Current_IPadd[20];
	u8 Current_Port[5];
	u8 Current_DeviceIP[20];
	
}_network_class;

extern _network_class NETWORK;


u8 NetWork_WIFI_Control(_wifi_control opt);
void WIFI_Initialization(void);
//void WIFI_StaManage_TaskThread(void *p_arg);
_BOOL IS_Port(u8 *port);
_BOOL IS_IPaddr(u8 *ip);
_BOOL IS_NUM(u8 *num);
_BOOL IPaddr_atoi(u8 *ip_str,u8 *ip_num);
#endif

