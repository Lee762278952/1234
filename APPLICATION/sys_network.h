#ifndef _NETWORK_H
#define _NETWORK_H
#include "sys.h"
#include "wifi.h"

#define NET_WIFI_DEBUG  		0
#define NetWork_Test 				0



//命令字段 后面跟命令字内容
#define iCmdEnum 				"iCmdEnum"
//设备ID 发消息需要包含
#define iDeviceID 			"iDeviceID"

//心跳
#define iElectric 			"iElectric"
#define iLuminance 			"iLuminance"
#define iWifi 					"iWifi"

//设备注册 MAC字符串C->S
#define strMac 					"strMac"
//注册反馈S->C
#define iResult 				"iResult"
#define strErrorMsg		 	"strErrorMsg"
//下发会议消息S->C
#define iMeetingID 			"iMeetingID"
#define strName 				"strName"
#define strSlogan 			"strSlogan"
#define strContent 			"strContent"
#define strStartTime 		"strStartTime"
#define strEndTime 			"strEndTime"
//返回用户信息 包含该名牌外屏图片URL
#define iChairman 				"iChairman"
#define iSecretary 				"iSecretary"
#define strUserName 			"strUserName"
#define strCompany 				"strCompany"
#define strPosition 			"strPosition"
#define strFormat 				"strFormat"
#define strWelcome 				"strWelcome"
#define strNameplateUrl 	"strNameplateUrl"
#define strNameplateBGUrl "strNameplateBGUrl"

//用户列表内容
#define iUserNum 				"iUserNum"
#define lstDevice 			"lstDevice"
#define strUserName 		"strUserName"
//请求服务
#define iServiceID 			"iServiceID"
//短消息
#define iSenderID 			"iSenderID"
#define iMsgType 				"iMsgType"
#define iContentType 		"iContentType"
#define lstRecvDeviceID "lstRecvDeviceID"
#define iReceiverID 		"iReceiverID"
//集中控制
#define iControlType 		"iControlType"

//C->S命令字
#define REQ_HEARTBEAT 					101
#define REQ_TS_DEVICE_REG 			201
#define REQ_TS_GET_USERINFO 		207
#define REQ_TS_GET_USERLIST 		209
#define EVT_TS_REQSERVICE 			211
//S->C命令字
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
//C->S->C命令字
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

