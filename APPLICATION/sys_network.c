#include "sys_network.h"
#include "includes.h"
#include "sys_malloc.h"
#include "wifi.h"
#include "W25Qxx.h"
#include "ff.h"
#include "fattester.h"
#include "lwip_comm.h"
#include <stdlib.h>
#include "sysdebug.h"
#include "lwip/dhcp.h"
#include "tjpgd.h"


//static _BOOL NETWORK.WIFI_TransRdy;//WIFI透传模式就绪
//static _wifi_callback CallBack_sta;
//extern OS_MUTEX WIFIControl_MUTEX;//WIFI控制信号量
//extern OS_MUTEX LWIPControl_MUTEX;//LWIP控制信号量
extern OS_MUTEX NetWork_MUTEX;

_BOOL NetWork_TransmitEnable = True;

static u8 network_head[24] = {0x49,0x54,0x43,0x4C,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

void NetWork_Format(u8 *buf,u8 field);
void JSON_CreatORComplete(u8 *json_buf,u8 opt);
void JSON_AddString(u8 *json_buf,u8 *key,u8 *value);
void JSON_AddNumber(u8 *json_buf,u8 *key,u16 num);
	
//_BOOL JSON_GetValueStr(u8 *json_buf,const char *key,char *value);
//u8 JSON_GetValueNum(const u8 *json_buf,const char *key);
_BOOL JSON_GetStr(u8 *json_buf,const char *key,char *value,u16 buflen);
_BOOL JSON_GetNum(const u8 *json_buf,const char *key,u16 *num);
_BOOL JSON_GetArr(u8 *json_buf,const char *key,char *value,u16 buflen);
_BOOL URL_GetIPnPORT(u8 *url,u8 *ip,u8 *port);
_BOOL NetWork_TransmitSend(u8 *sendbuf , u32 len);
_BOOL NetWork_TransmitReceive(void);	
	
_network_class NETWORK =
{
	.Mode = NET_AUTO,
	.WIFI_TransRdy = False,
	.LWIP_TransRdy = False,
	
	.Transmit_Send = NetWork_TransmitSend,
	.Transmit_Receive = NetWork_TransmitReceive,
};


	
void NetWork_TaskThread(void *p_arg)
{
	OS_ERR err;
	
	u8 *getContentStr;
	u16 getDevID,getNum;
	
	u8 *getURL;
	
	u8 res,getCmdEnum;
	u8 *NetWork_TxBuf = NULL;
//	u8 *NetWork_Rxbuf;
//	OS_MUTEX *network_mutex_temp;//信号量选择临时变量
	
	u8 wait_time=0;
	
	u16 i_devnum,j_devnum;
	char  c_devnum[5];
	
	OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);
	debug("NetWork_TaskThread start...\r\n");
	
	SYS->DeviceRegister = False;
	
	while(1)
	{
		OSTimeDlyHMSM(0,0,0,250,OS_OPT_TIME_HMSM_STRICT,&err);
		while(NETWORK.WIFI_TransRdy || NETWORK.LWIP_TransRdy)
		{
//			if(SYS->ServerCommand == True)
			OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);
//			if(NETWORK.WIFI_TransRdy)	network_mutex_temp = &WIFIControl_MUTEX;
//			else if(NETWORK.LWIP_TransRdy) network_mutex_temp = &LWIPControl_MUTEX; 
			
			OSMutexPend(&NetWork_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);
			
			if(!SYS->DeviceRegister)//如果设备未注册
			{
				if(!wait_time)
				{
//					NetWork_TxBuf = mymalloc(SRAMIN,120);
//						memset(NetWork_TxBuf,'\0',120);
					SYS_MALLOC(NetWork_TxBuf,SRAMIN,120);
					

					NetWork_Format(NetWork_TxBuf+24, REQ_TS_DEVICE_REG);
					network_head[8] = strlen((char*)NetWork_TxBuf+24)+24;
					memcpy(NetWork_TxBuf,network_head,24);
					
//					NetWork_TransmitSend(network_head,24);
					NetWork_TransmitSend(NetWork_TxBuf,network_head[8]);
//					myfree(SRAMIN,NetWork_TxBuf);
					SYS_FREE(NetWork_TxBuf,SRAMIN);
					wait_time = 80;
				}
				wait_time--;

#if NetWork_Test
				SYS->DeviceRegister = True;	
#endif
			}
			else
			{		
				//获取用户信息
				if(SYS->GetUserinfo && NetWork_TransmitEnable)
				{
//					NetWork_TxBuf = mymalloc(SRAMIN,100);
					NetWork_TransmitEnable = False;
					SYS_MALLOC(NetWork_TxBuf,SRAMIN,100);
					
					memset(NetWork_TxBuf,'\0',100);
					NetWork_Format(NetWork_TxBuf+24, REQ_TS_GET_USERINFO);
					network_head[8] = strlen((char*)NetWork_TxBuf+24)+24;
					memcpy(NetWork_TxBuf,network_head,24);
					
//					NetWork_TransmitSend(network_head,24);
					NetWork_TransmitSend(NetWork_TxBuf,network_head[8]);					
//					myfree(SRAMIN,NetWork_TxBuf);
					SYS_FREE(NetWork_TxBuf,SRAMIN);
					
					SYS->heartbeat_Timer = 0;
					SYS->HeartBeat = False;
					
#if NetWork_Test					
					SYS->GetUserinfo = False;
#endif
				}
				
				//获取会场用户列表
				else if(SYS->GetUserList && NetWork_TransmitEnable)
				{
//					NetWork_TxBuf = mymalloc(SRAMIN,100);
//					memset(NetWork_TxBuf,'\0',100);
					NetWork_TransmitEnable = False;
					SYS_MALLOC(NetWork_TxBuf,SRAMIN,100);
					
					NetWork_Format(NetWork_TxBuf+24, REQ_TS_GET_USERLIST);
					network_head[8] = strlen((char*)NetWork_TxBuf+24)+24;
					memcpy(NetWork_TxBuf,network_head,24);
					
//					NetWork_TransmitSend(network_head,24);
					NetWork_TransmitSend(NetWork_TxBuf,network_head[8]);					
//					myfree(SRAMIN,NetWork_TxBuf);
					SYS_FREE(NetWork_TxBuf,SRAMIN);
					
					SYS->heartbeat_Timer = 0;
					SYS->HeartBeat = False;
				
#if NetWork_Test					
					SYS->GetUserList = False;
#endif
				}
				
				//请求会议服务
				else if(SYS->ReqService && NetWork_TransmitEnable)
				{
//					NetWork_TxBuf = mymalloc(SRAMIN,300);
//					memset(NetWork_TxBuf,'\0',300);
					NetWork_TransmitEnable = False;
					SYS_MALLOC(NetWork_TxBuf,SRAMIN,300);
					
					NetWork_Format(NetWork_TxBuf+24, EVT_TS_REQSERVICE);
					network_head[8] = strlen((char*)NetWork_TxBuf+24)+24;
					memcpy(NetWork_TxBuf,network_head,24);
					
//					NetWork_TransmitSend(network_head,24);
					NetWork_TransmitSend(NetWork_TxBuf,network_head[8]);					
//					myfree(SRAMIN,NetWork_TxBuf);
					SYS_FREE(NetWork_TxBuf,SRAMIN);
					
					SYS->heartbeat_Timer = 0;
					SYS->HeartBeat = False;
					
#if NetWork_Test
					SYS->ReqService = False;
#endif
				}
				
				//短信发送
				else if(SYS->SendMassage && NetWork_TransmitEnable)
				{
//					NetWork_TxBuf = mymalloc(SRAMIN,600);
//					memset(NetWork_TxBuf,'\0',600);
					NetWork_TransmitEnable = False;
					SYS_MALLOC(NetWork_TxBuf,SRAMIN,600);
					
					NetWork_Format(NetWork_TxBuf+24, EVT_TS_SENDMSG);
					network_head[8] = strlen((char*)NetWork_TxBuf+24)+24;
					memcpy(NetWork_TxBuf,network_head,24);
					
//					NetWork_TransmitSend(network_head,24);
					NetWork_TransmitSend(NetWork_TxBuf,network_head[8]);					
//					myfree(SRAMIN,NetWork_TxBuf);
					SYS_FREE(NetWork_TxBuf,SRAMIN);
					
					SYS->heartbeat_Timer = 0;
					SYS->HeartBeat = False;
					
					SYS->SendMassage = False;

				}
				
				else if(SYS->HeartBeat && NetWork_TransmitEnable)//如果到时间HeartBeat
				{
//					NetWork_TxBuf = mymalloc(SRAMIN,150);
//					memset(NetWork_TxBuf,'\0',150);
					NetWork_TransmitEnable = False;
					SYS_MALLOC(NetWork_TxBuf,SRAMIN,150);
					
					NetWork_Format(NetWork_TxBuf+24, REQ_HEARTBEAT);
					network_head[8] = strlen((char*)NetWork_TxBuf+24)+24;
					memcpy(NetWork_TxBuf,network_head,24);
					
//					NetWork_TransmitSend(network_head,24);
					NetWork_TransmitSend(NetWork_TxBuf,network_head[8]);
//					myfree(SRAMIN,NetWork_TxBuf);
					SYS_FREE(NetWork_TxBuf,SRAMIN);
					
					SYS->HeartBeat = False;
				}
			}
			
			if(SYS->ServerCommand)//接收处理逻辑
			{
				
//				getContentStr = mymalloc(SRAMIN,200);
				SYS_MALLOC(getContentStr,SRAMIN,200);


				if(!JSON_GetNum(NETWORK.TransmitBuf,iDeviceID,&getDevID)) getDevID = 0xFFFF;
//  			debug("getDevID=%d\r\n",getDevID);
//				debug("%s\r\n",NETWORK.TransmitBuf);
 
				if(getDevID == SYS->Device_UID || getDevID == 0)
				{
					if(!JSON_GetNum(NETWORK.TransmitBuf,iCmdEnum,(u16*)&getCmdEnum)) getCmdEnum = 0;
					debug("DevID=%d Cmd=%d\r\n",getDevID,getCmdEnum);
			
					if(SYS->DeviceRegister == False && getCmdEnum != RSP_TS_DEVICE_REG) {	getCmdEnum = 0;debug("device is not register\r\n");}
					switch(getCmdEnum)
					{
						case RSP_HEARTBEAT:break;//心跳反馈
						case RSP_TS_DEVICE_REG://注册反馈
						{
							JSON_GetNum(NETWORK.TransmitBuf,iResult,&getNum);
							if(getNum == 200) //注册成功返回200
							{
								SYS->DeviceRegister = True;
								JSON_GetStr(NETWORK.TransmitBuf,strErrorMsg,(char*)getContentStr,15);
								SYS->TimeStamp = atol((char*)getContentStr);
								SYS->TimeSet = True;
								debug("Registration success ,server return timestamp=%d\r\n",SYS->TimeStamp);
							}
							else if(getNum == 400)
							{
								SYS->DeviceRegister = False;
							}
							else SYS->DeviceRegister = False;
						}break;		
						case EVT_TS_MEETINGINFO://会议信息
						{
							JSON_GetNum(NETWORK.TransmitBuf,iMeetingID,&SYS->MeetingID);
							JSON_GetStr(NETWORK.TransmitBuf,strName,(char*)SYS->MeetingName,sizeof(SYS->MeetingName));
							JSON_GetStr(NETWORK.TransmitBuf,strSlogan,(char*)SYS->MeetingSlogan,sizeof(SYS->MeetingSlogan));
							JSON_GetStr(NETWORK.TransmitBuf,strContent,(char*)SYS->MeetingContent,sizeof(SYS->MeetingContent));
							JSON_GetStr(NETWORK.TransmitBuf,strStartTime,(char*)SYS->MeetingStartTime,sizeof(SYS->MeetingStartTime));
							JSON_GetStr(NETWORK.TransmitBuf,strEndTime,(char*)SYS->MeetingEndTime,sizeof(SYS->MeetingEndTime));
							

							debug("MeetingInfo:id:%d\r\nname:%s\r\nslogan:%s\r\ncontent:%s\r\nstime:%s\r\netime:%s\r\n",SYS->MeetingID,SYS->MeetingName,SYS->MeetingSlogan,SYS->MeetingContent,SYS->MeetingStartTime,SYS->MeetingEndTime);

						}break; 		
						case EVT_TS_MEETING_BEGIN:break; //会议开始	
						case EVT_TS_MEETING_END:break;	//会议结束
						case EVT_TS_QUIT_MEETING:break;	//离开会议
						case RSP_TS_GET_USERINFO:				//用户信息
						{
//							myfree(SRAMIN,SYS->BGnameURL);
//							myfree(SRAMIN,SYS->BGpicURL);
//							myfree(SRAMIN,SYS->BG_IP);
//							myfree(SRAMIN,SYS->BG_Port);
							SYS_FREE(SYS->BGnameURL,SRAMIN);
							SYS_FREE(SYS->BGpicURL,SRAMIN);
							SYS_FREE(SYS->BG_IP,SRAMIN);
							SYS_FREE(SYS->BG_Port,SRAMIN);
														
							JSON_GetStr(NETWORK.TransmitBuf,strUserName,(char*)SYS->Name_Person,sizeof(SYS->Name_Person));
							JSON_GetStr(NETWORK.TransmitBuf,strCompany,(char*)SYS->Name_Company,sizeof(SYS->Name_Company));
							JSON_GetStr(NETWORK.TransmitBuf,strPosition,(char*)SYS->Name_Position,sizeof(SYS->Name_Position));
//							SYSPara_SaveNLoad(Save,2);
							
//							getURL = mymalloc(SRAMIN,100);
//							memset(getURL,'\0',100);
							SYS_MALLOC(getURL,SRAMIN,100);
							
							JSON_GetStr(NETWORK.TransmitBuf,strNameplateUrl,(char*)getURL,100);
							res = strlen((char*)getURL) + 1;
							if(res > 20)
							{
//								SYS->BGnameURL = mymalloc(SRAMIN,res+1);
//								memset(SYS->BGnameURL,'\0',res+1);
								SYS_MALLOC(SYS->BGnameURL,SRAMIN,res);
								strcpy((char*)SYS->BGnameURL,(char*)getURL);
								memset(getURL,'\0',100);
							}
							
							JSON_GetStr(NETWORK.TransmitBuf,strNameplateBGUrl,(char*)getURL,100);
							res = strlen((char*)getURL) + 1;
							if(res > 20)
							{
//								SYS->BGpicURL = mymalloc(SRAMIN,res);
//								memset(SYS->BGpicURL,'\0',res);
								SYS_MALLOC(SYS->BGpicURL,SRAMIN,res);
								strcpy((char*)SYS->BGpicURL,(char*)getURL);
								memset(getURL,'\0',100);
							}
							
							if(SYS->BGnameURL != NULL || SYS->BGpicURL != NULL)
							{
								SYS->HttpGeting = True;
								SYS->BGnameUpdata = False;
								SYS->BGpicUpdata = False;
							}
							
							debug("UserInfo:\r\nname:%s\r\ncompany:%s\r\npos:%s\r\nbgname:%s\r\nbgpic:%s\r\n",SYS->Name_Person,SYS->Name_Company,SYS->Name_Position,SYS->BGnameURL,SYS->BGpicURL);
											
//							myfree(SRAMIN,getURL);
							SYS_FREE(getURL,SRAMIN);
							SYS->GetUserinfo = False;	
						}break; 
						case RSP_TS_GET_USERLIST://用户列表
						{
//							myfree(SRAMIN,SYS->UserList);
//							myfree(SRAMIN,getContentStr);
							SYS_FREE(SYS->UserList,SRAMIN);
							SYS_FREE(getContentStr,SRAMIN);
							
							JSON_GetNum(NETWORK.TransmitBuf,iUserNum,&getNum);
							debug("iUserNum:%d\r\n",getNum);
							
//							SYS->UserList = mymalloc(SRAMIN,getNum*sizeof(_user_lst));			
//							getContentStr = mymalloc(SRAMIN,getNum*30);
							SYS_MALLOC(SYS->UserList,SRAMIN,getNum*sizeof(_user_lst));
							SYS_MALLOC(getContentStr,SRAMIN,getNum*30);
							
							JSON_GetArr(NETWORK.TransmitBuf,lstDevice,(char*)getContentStr,getNum*20);
							debug("lstDevice:%s\r\n",getContentStr);
							j_devnum = 0;
							for(i_devnum=0;i_devnum<MaxDeviceID;i_devnum++)
							{
								sprintf(c_devnum,"%d",i_devnum);
								if(JSON_GetStr(getContentStr,c_devnum,SYS->UserList[j_devnum]._lstUserName,18))
								{
									SYS->UserList[j_devnum]._lstUserID = i_devnum;
									debug("ID:%d name:%s\r\n",SYS->UserList[j_devnum]._lstUserID,SYS->UserList[j_devnum]._lstUserName);
									j_devnum++;
								}
								if(j_devnum >= getNum) break;
							}	
							SYS->UserNum = getNum;
							SYS->GetUserList = False;			
						}break; 
						case EVT_TS_REQSERVICE_ACK:
						{
							SYS->ReqService = False;
							SYS->FuncFrame_temp = 3;
							SYS_MALLOC(SYS->FuncFrame_buf,SRAMIN,30);
							memset(SYS->FuncFrame_buf,'\0',30);
							strcpy((char*)SYS->FuncFrame_buf,"已发送请求");
							SYS->WARNING_ENABLE = True;
							while(SYS->WARNING_ENABLE) OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
						}break; 
						case EVT_TS_CENTERCONTROL:
						{
							JSON_GetNum(NETWORK.TransmitBuf,iControlType,&getNum); 
							debug("iControlType:%d\r\n",getNum);
							switch(getNum)
							{
								case 1:break;
								case 2:SYS->Menu.Page = MeetInfo;break;
								case 3:SYS->Menu.Page = ShowName;break;
								case 10:SYS->Device_ShutDown = True;break;
								default:break;
							}
							SYS->Menu.PageSta[Lay1] = 0;
							SYS->Menu.PageUpdata = True;
							
						}break; 	
						case EVT_TS_USERSTATUS:break; 
						case EVT_TS_NAMEPLATE_UPDATE:SYS->GetUserinfo = True;break; 	
						case EVT_TS_SENDMSG: //短信接收
						{
//							debug("NETWORK.TransmitBuf:\r\n%s\r\n",NETWORK.TransmitBuf);
//							SYS->RecMsgList[SYS->RecMsgContNum] = mymalloc(SRAMIN,210); 
							SYS_MALLOC(SYS->RecMsgList[SYS->RecMsgContNum],SRAMIN,210);
							
							JSON_GetStr(NETWORK.TransmitBuf,strContent,(char*)SYS->RecMsgList[SYS->RecMsgContNum]->RecMsgCont,200);
							JSON_GetNum(NETWORK.TransmitBuf,iReceiverID,&SYS->RecMsgList[SYS->RecMsgContNum]->RecMsg_ID);
							
							debug("SYS->RecMsgContNum = %d\r\n%d:\r\n%s\r\n",SYS->RecMsgContNum,SYS->RecMsgList[SYS->RecMsgContNum]->RecMsg_ID,SYS->RecMsgList[SYS->RecMsgContNum]->RecMsgCont);
							SYS->RecMsgContNum ++;
							if(SYS->RecMsgContNum >= 10) SYS->RecMsgContNum = 9;
						}break;
						default:break;
					}
				}
				else
				{
					debug("Network content:%s\r\n",NETWORK.TransmitBuf);
				}
//				myfree(SRAMEX,NETWORK.TransmitBuf);	
//				myfree(SRAMIN,getContentStr);				
				SYS_FREE(NETWORK.TransmitBuf,SRAMEX);
				SYS_FREE(getContentStr,SRAMIN);
				
				SYS->ServerCommand = False;				
			}
			
			OSMutexPost(&NetWork_MUTEX,OS_OPT_POST_NONE,&err);//释放信号量						
		}
	}
}


void LWIP_StaManage_TaskThread(void *p_arg)
 {
	OS_ERR err;
//	CPU_SR_ALLOC();
	
	
	u8 retry_t;
//	u16 buflen;
	_BOOL lwip_init_done = False;
	_BOOL tcppcb_new_done = False;
	u16 check_t;
//	_BOOL NetConnect_Rdy = False;
	struct ip_addr rmtipaddr;  	//远端ip地址

	
	OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);
	OSTimeDlyHMSM(0,0,2,0,OS_OPT_TIME_HMSM_STRICT,&err);
	debug("LWIP_StaManage_TaskThread start...\r\n");

	
	NETWORK.LWIP_TransRdy = False;
 
	LWIP.DHCP_STA = 0;
	LWIP.LocalTime = 0;


	TCPCLIEN.Init();
//	TCPCLIEN.TcpPcb=tcp_new();
	
	IPaddr_atoi(NETWORK.Current_IPadd,LWIP.ServerIP);
	IPaddr_atoi(NETWORK.Current_DeviceIP,LWIP.LocalIP);
	LWIP.ServerPort = atoi((char*)NETWORK.Current_Port);
	
	while(1)
	{
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
		if((NETWORK.Mode == NET_AUTO && NETWORK.WIFI_TransRdy == False) || NETWORK.Mode == NET_LWIP)
		{
			retry_t = 0;
			while(LWIP.NetConnect_Rdy == False) 
			{				
				if(!((NETWORK.Mode == NET_AUTO && NETWORK.WIFI_TransRdy == False) || NETWORK.Mode == NET_LWIP)) break;
//   			OSMutexPend (&LWIPControl_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);//请求信号量
				OSMutexPend (&NetWork_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);//请求信号量
				if(lwip_init_done == True)
				{
					debug("LWIP restar\r\n");
					TCPCLIEN.TcpClient_STA = 0x00;
					LWIP.DHCP_STA = 0x00;
					LWIP.DeInit();
					lwip_init_done = False;	
				}

				LWIP.DHCP = SYS->DHCP_Enable;
				IPaddr_atoi(NETWORK.Current_IPadd,LWIP.ServerIP);
				IPaddr_atoi(NETWORK.Current_DeviceIP,LWIP.LocalIP);
				LWIP.ServerPort = atoi((char*)NETWORK.Current_Port);
				
				if(!LWIP.Init()) 
				{
					lwip_init_done = True;
					LWIP.NetConnect_Rdy = True;
					retry_t = 0;
				}
				debug("LWIP connect: times = %d\r\n",retry_t);
				retry_t++;
				if(retry_t > 50) retry_t = 50;
//				OSMutexPost(&LWIPControl_MUTEX,OS_OPT_POST_NONE,&err);
				OSMutexPost(&NetWork_MUTEX,OS_OPT_POST_NONE,&err);
				OSTimeDlyHMSM(0,0,0,(500*retry_t),OS_OPT_TIME_HMSM_STRICT,&err);
			}
					
			if(LWIP.NetConnect_Rdy == True)
			{
			debug("ServerIP:%d.%d.%d.%d LocalIP:%d.%d.%d.%d ServerPort:%d\r\n",
				LWIP.ServerIP[0],LWIP.ServerIP[1],LWIP.ServerIP[2],LWIP.ServerIP[3],
				LWIP.LocalIP[0],LWIP.LocalIP[1],LWIP.LocalIP[2],LWIP.LocalIP[3],
				LWIP.ServerPort);
			}


			while(LWIP.NetConnect_Rdy == True) 
			{
				if(!((NETWORK.Mode == NET_AUTO && NETWORK.WIFI_TransRdy == False) || NETWORK.Mode == NET_LWIP)) break;
				if(TCPCLIEN.Rx_Buffer == NULL) 
				{
//					TCPCLIEN.Rx_Buffer = mymalloc(SRAMEX,TCP_CLIENT_RX_BUFSIZE);
					SYS_MALLOC(TCPCLIEN.Rx_Buffer,SRAMEX,TCP_CLIENT_RX_BUFSIZE);

					TCPCLIEN.BufferSize = TCP_CLIENT_RX_BUFSIZE;
				}
				OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
//				LWIP.LocalTime += 50;
//				OSMutexPend (&LWIPControl_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);
				OSMutexPend (&NetWork_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);//请求信号量
				
				LWIP.PeriodicHandle();
				if(LWIP.DHCP_STA == 2 || LWIP.DHCP_STA == 0xFF)
				{
					if(TCPCLIEN.TcpClient_STA&1<<6)//是否收到数据?
					{
						if(strstr((char *)TCPCLIEN.Rx_Buffer,"ITCL"))
						{
//							NETWORK.TransmitBuf = mymalloc(SRAMEX,TCPCLIEN.Rx_BufLen+1);
//							memset(NETWORK.TransmitBuf,'\0',TCPCLIEN.Rx_BufLen+1);
							SYS_MALLOC(NETWORK.TransmitBuf,SRAMEX,TCPCLIEN.Rx_BufLen+1);
							
							memcpy(NETWORK.TransmitBuf,TCPCLIEN.Rx_Buffer+24,TCPCLIEN.Rx_BufLen-24);
							SYS->ServerCommand = True;
						}
						memset(TCPCLIEN.Rx_Buffer,'\0',TCPCLIEN.Rx_BufLen);
						TCPCLIEN.Rx_BufLen = 0;
						TCPCLIEN.TcpClient_STA&=~(1<<6);//标记数据已经被处理了.
						TCPCLIEN.Rx_Complete = False;
						check_t = 0;
					}
					if(TCPCLIEN.TcpClient_STA&1<<5)//是否连接上?
					{
						if(NETWORK.LWIP_TransRdy == False) 
						{
							check_t = 0;
							NETWORK.LWIP_TransRdy = True;
							debug("LWIP Transmit is ready\r\n");
						}
						check_t++;
						if(check_t > 900) //90S
						{
							TCPCLIEN.TcpClient_STA = 0;
//							NETWORK.LWIP_TransRdy = False;
							if(tcppcb_new_done) 
							{
								TCPCLIEN.ConnectionClose(TCPCLIEN.TcpPcb,0);//关闭连接
								tcppcb_new_done = False;
							}
						}
					}
					else if((TCPCLIEN.TcpClient_STA&1<<5)==0)
					{
						if(LWIP.LocalTime % 500)
						{
							if(NETWORK.LWIP_TransRdy == True)
							{
								NETWORK.LWIP_TransRdy = False;
								debug("TCP Disconnected\r\n"); 
							}
								
							if(tcppcb_new_done) 
							{
								TCPCLIEN.ConnectionClose(TCPCLIEN.TcpPcb,0);//关闭连接
								tcppcb_new_done = False;
							}
													
							TCPCLIEN.TcpPcb=tcp_new();	//创建一个新的pcb
							tcppcb_new_done = True;
							IP4_ADDR(&rmtipaddr,LWIP.ServerIP[0],LWIP.ServerIP[1],LWIP.ServerIP[2],LWIP.ServerIP[3]); 		
							if(TCPCLIEN.TcpPcb)			//创建成功
							{ 
								tcp_connect(TCPCLIEN.TcpPcb,&rmtipaddr,LWIP.ServerPort,TCPCLIEN.Connect);//连接到目的地址的指定端口上,当连接成功后回调tcp_client_connected()函数
							}
						}
					} 	
				}
				
//				OSMutexPost(&LWIPControl_MUTEX,OS_OPT_POST_NONE,&err);
				OSMutexPost(&NetWork_MUTEX,OS_OPT_POST_NONE,&err);
				
				if((LWIP.LocalTime % 1000) && !LWIP.NETLinkCheck())
				{
					debug("LWIP disconnect\r\n");
					if(TCPCLIEN.TcpPcb != NULL) 
					{
						TCPCLIEN.ConnectionClose(TCPCLIEN.TcpPcb,0);
						TCPCLIEN.TcpPcb = NULL;
						tcppcb_new_done = False;
					}
					TCPCLIEN.TcpClient_STA &= ~(1<<5);
					LWIP.DHCP_STA = 1;
					NETWORK.LWIP_TransRdy = False;
					LWIP.NetConnect_Rdy = False;
					while(!LWIP.NETLinkCheck()) OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
					debug("LWIP reconnect\r\n");
				}
			}
			if(tcppcb_new_done) 
			{
				TCPCLIEN.ConnectionClose(TCPCLIEN.TcpPcb,0);//关闭连接
				tcppcb_new_done = False;
			}
			NETWORK.LWIP_TransRdy = False;
		}
		else
		{
			if(NETWORK.LWIP_TransRdy == True) NETWORK.LWIP_TransRdy = False;
							
			OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
			if(TCPCLIEN.Rx_Buffer != NULL) 
			{
//				myfree(SRAMEX,TCPCLIEN.Rx_Buffer);
				SYS_FREE(TCPCLIEN.Rx_Buffer,SRAMEX);
			}
		}
	}
}
	
void WIFI_StaManage_TaskThread(void *p_arg)
{
	OS_ERR err;
	
	u8 res,setmode = 0;
//	u8 Dlytime=1,time_init=1;
	
	u8 set_try_times;
	_BOOL set_success;
	_BOOL model_reset;
	
	
	u16 time_check=0;
	u16 time_signal_check=0;
  _wifi_callback cb_sta;
	SYS->HeartBeat = False;
	
	
	OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);
	debug("WIFI_StaManage_TaskThread start...\r\n");
	
	WIFI_Initialization();
	NETWORK.WIFI_TransRdy = False;
	WIFI.WIFI_STA_Expect = 0x0F;
	while(1)
	{	

		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
		if((NETWORK.Mode == NET_AUTO && NETWORK.LWIP_TransRdy == False) || NETWORK.Mode == NET_WIFI ) 
		{
//			time_init = 1;
			model_reset = False;
			while(WIFI.WIFI_STA != WIFI.WIFI_STA_Expect) 
			{
				if(!((NETWORK.Mode == NET_AUTO && NETWORK.LWIP_TransRdy == False) || NETWORK.Mode == NET_WIFI) )  break;
				if(U4.Rx_Buffer == NULL)
				{
//					U4.Rx_Buffer = mymalloc(SRAMEX,UART4_REC_LEN);
					SYS_MALLOC(U4.Rx_Buffer,SRAMEX,UART4_REC_LEN);

				}
				
//				debug("set wifi STA = %x expect = %x\r\n",WIFI.WIFI_STA,WIFI.WIFI_STA_Expect);

				//重启模块
				if(GetSTA(WIFI.WIFI_STA,WSTA_Launch) != GetSTA(WIFI.WIFI_STA_Expect,WSTA_Launch) || model_reset)
				{
	//				SetSTA(WIFI.WIFI_STA,WSTA_Launch);
					OSMutexPend (&NetWork_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);//请求信号量
					debug("Restart wifi...(%x:%x)\r\n",WIFI.WIFI_STA,WIFI.WIFI_STA_Expect);
					
					if(!NetWork_WIFI_Control(Restart))
					{
						WIFI.WIFI_STA = 0x00;
						model_reset = True;
						debug("fail!\r\n");
					}
					else 
					{
						WIFI.WIFI_STA = 0x01;
						model_reset = False;
						debug("Restart success!\r\n");
					}
					OSMutexPost(&NetWork_MUTEX,OS_OPT_POST_NONE,&err);
					
				}
				
				//设置模式（命令 || 透传）
				if(GetSTA(WIFI.WIFI_STA,WSTA_Mode) != GetSTA(WIFI.WIFI_STA_Expect,WSTA_Mode) && !model_reset )//
				{
					set_try_times = 0;
					set_success = False;
					while(!set_success && !model_reset)
					{
						OSMutexPend (&NetWork_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);//请求信号量
						if(GetSTA(WIFI.WIFI_STA_Expect,WSTA_Mode))
						{
							debug("Setting the WiFi func module as the command mode,try times %d (%x:%x)\r\n",set_try_times,WIFI.WIFI_STA,WIFI.WIFI_STA_Expect);
							if(NetWork_WIFI_Control(ModeCom) != 1)
							{
								if(!NetWork_WIFI_Control(Ask))  
								{
									if(NetWork_WIFI_Control(ModeCom) == 1)  SetSTA(WIFI.WIFI_STA,WSTA_Mode);
								}
								else 
								{
									SetSTA(WIFI.WIFI_STA,WSTA_Mode);
									set_success = True;
								}
							}
							else
							{
								SetSTA(WIFI.WIFI_STA,WSTA_Mode);
								set_success = True;
							}
							
							
							if(!setmode && set_success) 
							{
								debug("Get the WiFi module MAC address\r\n");
								NetWork_WIFI_Control(GetMac);
								NetWork_WIFI_Control(StationSet);
								setmode = 1;
							}
						}
						else
						{
							debug("Setting the WiFi func module as the transmission mode,try times %d (%x:%x)\r\n",set_try_times,WIFI.WIFI_STA,WIFI.WIFI_STA_Expect);
							if(!NetWork_WIFI_Control(ModeTran)) 
							{
								if(NetWork_WIFI_Control(ModeCom) == 1) 
								{
									if(NetWork_WIFI_Control(ModeTran))
									{
										ResetSTA(WIFI.WIFI_STA,WSTA_Mode);
										set_success = True;
									}
								}
							}
							else 
							{
								ResetSTA(WIFI.WIFI_STA,WSTA_Mode);
								set_success = True;
							}
						}
						
						if(set_success) debug("Setting success!\r\n");
						else 
						{
							set_try_times++;
							if(set_try_times >= 8) 
							{
								model_reset = True;
								debug("Setting the WiFi func module timeout..\r\n");
							}
						}
						OSMutexPost(&NetWork_MUTEX,OS_OPT_POST_NONE,&err);
						OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);
					}
				}

				//连接AP
				if((GetSTA(WIFI.WIFI_STA,WSTA_Connect) != GetSTA(WIFI.WIFI_STA_Expect,WSTA_Connect)) && !model_reset)
				{
					set_try_times = 0;
					set_success = False;
					
					while(!set_success && !model_reset && WIFI.WIFI_STA == 0x03)
					{
						OSMutexPend (&NetWork_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);//请求信号量
						debug("Connect to AP,try times %d (%x:%x)\r\n",set_try_times,WIFI.WIFI_STA,WIFI.WIFI_STA_Expect);
						if(SYS->DHCP_Enable) NetWork_WIFI_Control(SetDhcp);
					  else NetWork_WIFI_Control(LocalIp);
						
						res = NetWork_WIFI_Control(ConnectSta);
						
						if(res == 1) 
						{
							 NetWork_WIFI_Control(ApSignal);
							SetSTA(WIFI.WIFI_STA,WSTA_Connect);
							set_success = True;
						}
						else if(res == 4 || res == 5)
						{
							res = NetWork_WIFI_Control(ConnectSta);//二次判断
							
							if(res == 4 || res == 5)
							{
								debug("ssid or password different\r\n");
								if(!NetWork_WIFI_Control(SsidSet)) debug("SSID set fail\r\n");
								if(!NetWork_WIFI_Control(PwdSet)) debug("PWD set fail\r\n");
								if(!NetWork_WIFI_Control(ConnectEn))debug("STA connect fail\r\n");
								else SetSTA(WIFI.WIFI_STA,WSTA_Connect);
							}
						}
						else 
						{
							ResetSTA(WIFI.WIFI_STA,WSTA_Connect);
							NetWork_WIFI_Control(ConnectEn);
						}
	//					NetWork_WIFI_Control(GetIp);
						if(set_success) debug("Connect AP \'%s\' success(signal=%d) \r\n",WIFI.Current_SSID,SYS->WIFI_SignalSt);
						else 
						{
							set_try_times++;
							if(set_try_times >= 28) 
							{
								model_reset = True;
								debug("Connect to AP timeout..\r\n");
							}
						}
						OSMutexPost(&NetWork_MUTEX,OS_OPT_POST_NONE,&err);
						OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
					}
					
				}
				
				//连接TCP
				if((GetSTA(WIFI.WIFI_STA,WSTA_Tcp) != GetSTA(WIFI.WIFI_STA_Expect,WSTA_Tcp)) && !model_reset)
				{
					set_try_times = 0;
					set_success = False;
					
					while(!set_success && !model_reset && WIFI.WIFI_STA == 0x07)
					{
						OSMutexPend (&NetWork_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);//请求信号量
						debug("Connect to the TCP server,try times %d (%x:%x)\r\n",set_try_times,WIFI.WIFI_STA,WIFI.WIFI_STA_Expect);
						res = NetWork_WIFI_Control(TcpSta);

						if(res == 1)
						{
							SetSTA(WIFI.WIFI_STA,WSTA_Tcp);
							set_success = True;
						}
						else if(res == 4)
						{
							res = NetWork_WIFI_Control(TcpSta); //二次判断
							if(res == 4)
							{
								debug("TCP para different\r\n");
								if(!NetWork_WIFI_Control(TcpSet)) 
								{
									debug("TCP set fail\r\n"); 
									ResetSTA(WIFI.WIFI_STA,WSTA_Tcp);
								}
							}
						}
						else
						{	
							ResetSTA(WIFI.WIFI_STA,WSTA_Tcp);
							NetWork_WIFI_Control(TcpSet);
						}
						
						if(set_success)
						{
							debug("Connect to TCP success!\r\n");
							NetWork_WIFI_Control(ApSignal);
						}
						else 
						{
							set_try_times++;
							if(set_try_times >= 36) 
							{
								model_reset = True;
								debug("Connect to TCP timeout..\r\n");
								NetWork_WIFI_Control(ApSignal);
							}
						}
						OSMutexPost(&NetWork_MUTEX,OS_OPT_POST_NONE,&err);
						OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);
					}
				}
				
//				if(time_init <= 20) Dlytime = 0;
//				else Dlytime = 2;

				
				if(model_reset)
				{		
					WIFI.WIFI_STA = 0x00;
					WIFI.WIFI_STA_Expect = 0x0F;
					WIFI.Disable_Set();
//					OSMutexPost(&NetWork_MUTEX,OS_OPT_POST_NONE,&err);
					OSTimeDlyHMSM(0,0,5,0,OS_OPT_TIME_HMSM_STRICT,&err);
				}
				else
				{
//					OSMutexPost(&NetWork_MUTEX,OS_OPT_POST_NONE,&err);
					OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
				}
				
			}

			
			while(WIFI.WIFI_STA == WIFI.WIFI_STA_Expect) 
			{
				if(!((NETWORK.Mode == NET_AUTO && NETWORK.LWIP_TransRdy == False) || NETWORK.Mode == NET_WIFI) )  break;
				OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
				OSMutexPend (&NetWork_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);//请求信号量
							
				if(time_check >= 600 || WIFI.CommandMode) //60s(100ms*600)复查一次连接状态 无数据状态 
				{		
					NETWORK.WIFI_TransRdy = False;
					SetSTA(WIFI.WIFI_STA_Expect,WSTA_Mode);
					if(GetSTA(WIFI.WIFI_STA,WSTA_Mode))
					{
						if(time_check >= 600)
						{
							res = NetWork_WIFI_Control(TcpSta);
							debug("Detect the TCP link..\r\n");
							if(res == 1) 
							{
								SetSTA(WIFI.WIFI_STA,WSTA_Tcp);
							}
							else
							{
								ResetSTA(WIFI.WIFI_STA,WSTA_Tcp);	
								debug("Detect abnormal!!!\r\n");
							}
							time_check = 1;
//							SYS->DeviceRegister = False;
//							SYS->HeartBeat = False;
//							SYS->SendMassage = False;
//							SYS->ReqService = False;
//							SYS->GetUserList = False;
//							SYS->GetUserinfo = False;
						}
					}
				}
				else if(time_signal_check >= 450){
					SetSTA(WIFI.WIFI_STA_Expect,WSTA_Mode);
					if(GetSTA(WIFI.WIFI_STA,WSTA_Mode)){
						NetWork_WIFI_Control(ApSignal);
						debug("Detect normal,wifi signal:%d\r\n",SYS->WIFI_SignalSt);
						time_signal_check = 1;
					}	
				}
				else 
				{
					if(GetSTA(WIFI.WIFI_STA,WSTA_Mode)) ResetSTA(WIFI.WIFI_STA_Expect,WSTA_Mode);
					else
					{
						if(NETWORK.WIFI_TransRdy == False && WIFI.WIFI_STA == WIFI.WIFI_STA_Expect)
						{
							debug("WIFI transmit ready!\r\n");
							NETWORK.WIFI_TransRdy = True;
						}
						
						if(SYS->ServerCommand == False) cb_sta = WIFI.CallBack_Process(NULL,NULL,NULL);
						if(cb_sta)
						{
							switch(cb_sta)
							{
								case W_TCP_RST:ResetSTA(WIFI.WIFI_STA,WSTA_Tcp);debug("The \'TCP\' is disconnected\r\n");WIFI.WIFI_STA_Expect = 0x0F;break;
								case W_RECONNECT:ResetSTA(WIFI.WIFI_STA,WSTA_Connect);debug("The \'AP\' link is disconnected\r\n");WIFI.WIFI_STA_Expect = 0x0F;break;
								case W_SERVER:
								{
									WIFI.ReceiveLen -= 24;
//									NETWORK.TransmitBuf = mymalloc(SRAMEX,WIFI.ReceiveLen+1);
//									memset(NETWORK.TransmitBuf,'\0',WIFI.ReceiveLen+1);
									SYS_MALLOC(NETWORK.TransmitBuf,SRAMEX,WIFI.ReceiveLen+1);

									
									memcpy(NETWORK.TransmitBuf,WIFI.TCP_Rxbuf+24,WIFI.ReceiveLen);
									SYS->ServerCommand = True;
									time_check = 0;
									WIFI.ReceiveLen = 0;
//									myfree(SRAMEX,WIFI.TCP_Rxbuf);
									SYS_FREE(WIFI.TCP_Rxbuf,SRAMEX);
									break;
								}
								default:debug("cb_sta = %d\r\n",cb_sta);break;
							}
							cb_sta = W_NULL;
						}		
						time_check++;
						time_signal_check++;
					}
				}
				OSMutexPost(&NetWork_MUTEX,OS_OPT_POST_NONE,&err);
			}
//			NETWORK.WIFI_TransRdy = False;
		}
		else
		{
			if(U4.Rx_Buffer) 
			{
//				myfree(SRAMEX,U4.Rx_Buffer);
				SYS_FREE(U4.Rx_Buffer,SRAMEX);
			}
			if(NETWORK.WIFI_TransRdy || WIFI.WIFI_STA)
			{
				WIFI.WIFI_STA = 0;
				WIFI.WIFI_STA_Expect = 0x0F;
				NETWORK.WIFI_TransRdy = False;
			}
			WIFI.Disable_Set();
			OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
		}
	}
}



void Http_Request_TaskThread(void *p_arg)
{
	OS_ERR err;
//	CPU_SR_ALLOC();
	
	_BOOL BGname,BGpic;
	u8 *http_url = NULL;
	u8 len,res,link_t,download_t,ram_temp = 0;
	u32 pic_size,i,jpg_h,jpg_e,file_bw;
//	OS_MUTEX *network_mutex_temp;//信号量选择临时变量
	u16 wait_time = 0;
	
	DIR fsdir;
	FIL *ft_file = NULL;
	
	OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);
	debug("Http_Request_TaskThread start...\r\n");
	

	
//	ft_file= (FIL*)mymalloc(SRAMIN,sizeof(FIL));
	SYS_MALLOC(ft_file,SRAMIN,sizeof(FIL));
	
	while(1)
	{
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
		if(SYS->HttpGeting)
		{
			OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
			debug("Http_Request ENABLE...\r\n");
			
//			SYS->BG_IP = mymalloc(SRAMIN,20);
//			memset(SYS->BG_IP,'\0',20);
//			SYS->BG_Port = mymalloc(SRAMIN,10);
//			memset(SYS->BG_Port,'\0',10);
			SYS_MALLOC(SYS->BG_IP,SRAMIN,20);
			SYS_MALLOC(SYS->BG_Port,SRAMIN,10);
			
			if(SYS->BGnameURL) BGname = True;
			if(SYS->BGpicURL) BGpic = True;
			
			
			if(!URL_GetIPnPORT(SYS->BGnameURL,SYS->BG_IP,SYS->BG_Port))  BGname = False;
			if(!URL_GetIPnPORT(SYS->BGpicURL,SYS->BG_IP,SYS->BG_Port)) BGpic = False;
			debug("BG_IP:%s\r\nBG_Port:%s\r\n",SYS->BG_IP,SYS->BG_Port);
			debug("BGname = %d\r\nBGpic = %d\r\n",BGname,BGpic);
			
			strcpy((char*)NETWORK.Current_IPadd,(char*)SYS->BG_IP);
			strcpy((char*)NETWORK.Current_Port,(char*)SYS->BG_Port);

			WIFI.WIFI_STA = 0x00;
			WIFI.WIFI_STA_Expect = 0x0F; //重连接
			NETWORK.WIFI_TransRdy = False;
			NETWORK.LWIP_TransRdy = False;
			LWIP.NetConnect_Rdy = False;//重连接
			OSTimeDlyHMSM(0,0,0,300,OS_OPT_TIME_HMSM_STRICT,&err);

			link_t = 0;
			download_t = 0;
			while(SYS->HttpGeting)
			{
				OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
				link_t++;
				if(link_t > 100) 
				{
					debug("HttpGeting link timeout\r\n");
					SYS->HttpGeting = False;
				}
				
				if(BGname || BGpic)
				{
					startdownload:
					while(NETWORK.WIFI_TransRdy || NETWORK.LWIP_TransRdy)
					{
//						debug("WIFI_TransRdy = %s  LWIP_TransRdy = %s\r\n",(NETWORK.WIFI_TransRdy ? "true":"false"),(NETWORK.LWIP_TransRdy ? "true":"false"));
						link_t = 0;
						download_t++;
						if(download_t > 10) 
						{
							BGname = False;
							BGpic = False;
							break;
						}
						OSTimeDlyHMSM(0,0,0,300,OS_OPT_TIME_HMSM_STRICT,&err);
						if(BGname)
						{
							if(NETWORK.WIFI_TransRdy) 
							{
	//							network_mutex_temp = &WIFIControl_MUTEX;	
								WIFI.UartBuf_Set(1);
								ram_temp = 1;
							}
							else if(NETWORK.LWIP_TransRdy)
							{
	//							network_mutex_temp = &LWIPControl_MUTEX;
								LWIP.Buf_Set(1);
								ram_temp = 2;
							}
							
							OSMutexPend(&NetWork_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);//请求信号量
							
							debug("BG namepic start\r\n");
							
							if(f_opendir(&fsdir,"1:/NAMEPIC")) 
							{
								debug("mkdir\r\n");
								res = f_mkdir("1:/NAMEPIC");
								if(res) debug("mkdir = %d\r\n",res);
							}
							
							res = f_open(ft_file,"1:/NAMEPIC/obg2.jpg",FA_WRITE);
							if(!res)
							{
								f_close(ft_file);
	//							debug("delete old file\r\n");
								res = f_unlink("1:/NAMEPIC/obg2.jpg");
								if(res) debug("delete fail:%d\r\n",res);
								else debug("delete 1:/NAMEPIC/obg2.jpg\r\n");
							}
							res = f_open(ft_file,"1:/NAMEPIC/obg2.jpg",FA_WRITE|FA_CREATE_NEW);
							if(res) debug("CREATE file fail:%d\r\n",res);

							debug("NamePic download and save start...\r\n");
							
							len = strlen((char*)SYS->BGnameURL)+15;
	//						http_url = mymalloc(SRAMIN,len);
	//							memset(http_url,'\0',len);
							SYS_MALLOC(http_url,SRAMIN,len);					
							
							strcpy((char*)http_url,"GET ");
							strcat((char*)http_url,(char*)SYS->BGnameURL);
							strcat((char*)http_url,"\r\n\r\n");
							
	//						debug("%s",http_url);
							NetWork_TransmitSend(http_url,strlen((char *)http_url));
							
	//						OS_CRITICAL_ENTER();	//进入临界区
							while(!NetWork_TransmitReceive()){
								OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
								wait_time++;
								if(wait_time > 100){
									wait_time = 0;
									debug("NetWork Transmit Receive fail\r\n");
									goto startdownload;
								}
							} 
	//						OS_CRITICAL_EXIT();	
							
	//	   				UART1.SendBuffer(NETWORK.TransmitBuf ,NETWORK.TransmitBufLen);
							jpg_h = 0xFFFF;
							jpg_e = 0;
							for(i=1;i<NETWORK.TransmitBufLen;i++)
							{
								if(NETWORK.TransmitBuf[i] == 0xD8 && NETWORK.TransmitBuf[i-1] == 0xFF)
								{
									jpg_h = i - 1;
									break;
								}
							}
	//						for(i=NETWORK.TransmitBufLen-1;i>jpg_h;i--)
	//						{
	//							if(NETWORK.TransmitBuf[i] == 0xD9 && NETWORK.TransmitBuf[i-1] == 0xFF) jpg_e = i;
	//							break;
	//						}
							for(i=1;i<NETWORK.TransmitBufLen;i++)
							{
								if(NETWORK.TransmitBuf[i] == 0xD9 && NETWORK.TransmitBuf[i-1] == 0xFF) jpg_e = i;
							}
							if(jpg_h == 0xFFFF || jpg_e == 0) 
							{
								debug("Request err:%d->%d\r\n",jpg_h,jpg_e);
								WIFI.WIFI_STA = 0x00;
								WIFI.WIFI_STA_Expect = 0x0F; //重连接
								LWIP.NetConnect_Rdy = False;//重连接
							}
							else
							{
								pic_size = jpg_e - jpg_h + 1;
								debug("Download NamePic finish size:%d\r\n",pic_size);
								
								//debug图片内容
								debug("Len=%d jpg_h=%d jpg_e=%d\r\n",NETWORK.TransmitBufLen,jpg_h,jpg_e);
								debug("Image data:\r\n");
								for(i=jpg_h;i<jpg_h+10;i++) printf("0x%02x,",NETWORK.TransmitBuf[i]);
								printf(".....");
								for(i=jpg_e-10;i<jpg_e+1;i++) printf("0x%02x,",NETWORK.TransmitBuf[i]);
								debug("\r\nImage data end\r\n\r\n");
	//							debug("The remaining data:\r\n");
	//							for(i=jpg_e+1;i<NETWORK.TransmitBufLen;i++)
	//							{
	//								printf("0x%02x,",NETWORK.TransmitBuf[i]);
	//							}
	//							debug("\r\nThe remaining data end\r\n\r\n");
								
								
		//						debug("jpg_h:%d\r\n")
								res = f_write(ft_file,NETWORK.TransmitBuf+jpg_h,pic_size,&file_bw);
								if(res) debug("Save NamePic error res:%d bw:%d\r\n",res,file_bw);
								else  debug("Save NamePic finish \r\n");
								f_close(ft_file);
								if(!whether_jpg((const u8*)"1:/NAMEPIC/obg2.jpg"))
								{
									BGname = False;
									SYS->BGnameUpdata = True;
								}
								else debug("open obg2.jpg fail,file not jpg!\r\n");
							}
							
							if(ram_temp == 1) WIFI.UartBuf_Set(0);
							else if(ram_temp == 2) LWIP.Buf_Set(0);
							
							OSMutexPost(&NetWork_MUTEX,OS_OPT_POST_NONE,&err);//释放信号量		

							NETWORK.TransmitBufLen = 0;
	//						myfree(SRAMEX,NETWORK.TransmitBuf);
	//						myfree(SRAMIN,http_url);
							SYS_FREE(NETWORK.TransmitBuf,SRAMEX);
							SYS_FREE(http_url,SRAMIN);
							
							WIFI.WIFI_STA = 0x00;
							WIFI.WIFI_STA_Expect = 0x0F; //重连接
							LWIP.NetConnect_Rdy = False;//重连接
							break;
						}
						else if(BGpic) //背景图片
						{	BGpic = False;/*
							if(NETWORK.WIFI_TransRdy) 
							{
	//							network_mutex_temp = &WIFIControl_MUTEX;	
								WIFI.UartBuf_Set(1);
								ram_temp = 1;
							}
							else if(NETWORK.LWIP_TransRdy)
							{
	//							network_mutex_temp = &LWIPControl_MUTEX;
								LWIP.Buf_Set(1);
								ram_temp = 2;
							}
							
							OSMutexPend(&NetWork_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);//请求信号量
							
							debug("BGpic start\r\n");
							
							if(f_opendir(&fsdir,"1:/BACKGROUND")) 
							{
								debug("open dir fail\r\n");
	//							res = f_mkdir("1:/NAMEPIC");
	//							if(res) debug("mkdir = %d\r\n",res);
							}
							
							res = f_open(ft_file,"1:/BACKGROUND/bg4.jpg",FA_WRITE);
							if(!res)
							{
								f_close(ft_file);
	//							debug("delete old file\r\n");
								res = f_unlink("1:/BACKGROUND/bg4.jpg");
								if(res) debug("delete fail:%d\r\n",res);

							}
							res = f_open(ft_file,"1:/BACKGROUND/bg4.jpg",FA_WRITE|FA_CREATE_NEW);
							if(res) debug("CREATE file fail:%d\r\n",res);

							debug("BG Pic download and save start...\r\n");
							
							len = strlen((char*)SYS->BGpicURL)+15;
	//						http_url = mymalloc(SRAMIN,len);
	//						memset(http_url,'\0',len);
							SYS_MALLOC(http_url,SRAMIN,len);
							
							strcpy((char*)http_url,"GET ");
							strcat((char*)http_url,(char*)SYS->BGpicURL);
							strcat((char*)http_url,"\r\n\r\n");
							
	//						debug("%s",http_url);
							NetWork_TransmitSend(http_url,strlen((char *)http_url));
							
							while(!NetWork_TransmitReceive()) OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err);
							
							
	//	   				UART1.SendBuffer(NETWORK.TransmitBuf ,NETWORK.TransmitBufLen);
							jpg_h = 0xFFFF;
							jpg_e = 0;
							for(i=1;i<NETWORK.TransmitBufLen;i++)
							{
								if(NETWORK.TransmitBuf[i] == 0xD8 && NETWORK.TransmitBuf[i-1] == 0xFF)
								{
									jpg_h = i - 1;
									break;
								}
							}
	//						for(i=NETWORK.TransmitBufLen-1;i>jpg_h;i--)
	//						{
	//							if(NETWORK.TransmitBuf[i] == 0xD9 && NETWORK.TransmitBuf[i-1] == 0xFF) jpg_e = i;
	//							break;
	//						}
							for(i=1;i<NETWORK.TransmitBufLen;i++)
							{
								if(NETWORK.TransmitBuf[i] == 0xD9 && NETWORK.TransmitBuf[i-1] == 0xFF) jpg_e = i;
							}
							if(jpg_h == 0xFFFF || jpg_e == 0) 
							{
								debug("Request err:%d->%d\r\n",jpg_h,jpg_e);
							}
							else
							{
								pic_size = jpg_e - jpg_h + 1;
								debug("Download BG Pic finish size:%d\r\n",pic_size);
								
								//debug图片内容
								debug("Len=%d jpg_h=%d jpg_e=%d\r\n",NETWORK.TransmitBufLen,jpg_h,jpg_e);
								debug("Image data:\r\n");
								for(i=jpg_h;i<jpg_h+10;i++) printf("0x%02x,",NETWORK.TransmitBuf[i]);
								printf(".....");
								for(i=jpg_e-10;i<jpg_e+1;i++) printf("0x%02x,",NETWORK.TransmitBuf[i]);
								debug("\r\nImage data end\r\n\r\n");
	//							debug("The remaining data:\r\n");
	//							for(i=jpg_e+1;i<NETWORK.TransmitBufLen;i++)
	//							{
	//								printf("0x%02x,",NETWORK.TransmitBuf[i]);
	//							}
	//							debug("\r\nThe remaining data end\r\n\r\n");
								
								
								res = f_write(ft_file,NETWORK.TransmitBuf+jpg_h,pic_size,&file_bw);
								if(res) debug("Save NamePic finish res:%d bw:%d\r\n",res,file_bw);
								else  debug("Save BGPic finish \r\n");
								f_close(ft_file);
								
								if(!whether_jpg((const u8*)"1:/BACKGROUND/bg4.jpg"))
								{
									BGpic = False;
									SYS->BGpicUpdata = True;
								}
								else debug("open bg4.jpg fail,file not jpg!\r\n");
							}
							
							if(ram_temp == 1) WIFI.UartBuf_Set(0);
							else if(ram_temp == 2) LWIP.Buf_Set(0);
							OSMutexPost(&NetWork_MUTEX,OS_OPT_POST_NONE,&err);//释放信号量		

							NETWORK.TransmitBufLen = 0;
	//						myfree(SRAMEX,NETWORK.TransmitBuf);
	//						myfree(SRAMIN,http_url);
							SYS_FREE(NETWORK.TransmitBuf,SRAMEX);
							SYS_FREE(http_url,SRAMIN);
							
							WIFI.WIFI_STA = 0x00;
							WIFI.WIFI_STA_Expect = 0x0F; //重连接
							LWIP.NetConnect_Rdy = False;//重连接
							break;*/
						}

					}
				}
				else 
				{
					SYS->HttpGeting = False;
					NETWORK.WIFI_TransRdy = False;
					NETWORK.LWIP_TransRdy = False;
				}
			}
			

//			WIFI.UartBuf_Set(0);
//			LWIP.Buf_Set(0);
			memcpy(NETWORK.Current_IPadd,SYS->TCP_IPadd,30);
			memcpy(NETWORK.Current_Port,SYS->TCP_Port,5);
			WIFI.WIFI_STA = 0x05;
			WIFI.WIFI_STA_Expect = 0x0F;
			LWIP.NetConnect_Rdy = False;

			debug("Http_Request DISABLE...\r\n");
		}
	
	}
}


_BOOL NetWork_TransmitSend(u8 *sendbuf , u32 len)
{
	err_t ret_err;

//	u32 i;
//	debug("NetWork->sendbuf:\r\n  ");
//	for(i = 0;i<len;i++) printf(" %x",sendbuf[i]);
//	printf("\r\n");
	
	
	if(NETWORK.LWIP_TransRdy) 
	{
		ret_err = TCPCLIEN.Send(sendbuf,len);
		
		if(ret_err == 0) return True;
		else return False;
	}
	else if(NETWORK.WIFI_TransRdy)
	{
		return WIFI.Transmit_Send(sendbuf,len);		
	}
	
	else return False;
	
}
_BOOL NetWork_TransmitReceive(void)
{
	if(NETWORK.LWIP_TransRdy) 
	{
		if(TCPCLIEN.Recv())
		{
//			NETWORK.TransmitBuf = mymalloc(SRAMEX,TCPCLIEN.Rx_BufLen+1);
//			memset(NETWORK.TransmitBuf,'\0',TCPCLIEN.Rx_BufLen+1);
			SYS_MALLOC(NETWORK.TransmitBuf,SRAMEX,TCPCLIEN.Rx_BufLen+1);
			
			memcpy(NETWORK.TransmitBuf,TCPCLIEN.Rx_Buffer,TCPCLIEN.Rx_BufLen);	
			memset(TCPCLIEN.Rx_Buffer,'\0',TCPCLIEN.Rx_BufLen);
			NETWORK.TransmitBufLen = TCPCLIEN.Rx_BufLen;
			TCPCLIEN.Rx_BufLen = 0;
			TCPCLIEN.TcpClient_STA&=~(1<<6);//标记数据已经被处理了.
			TCPCLIEN.Rx_Complete = False;
			
			return True;
		}
		else return False;
	}
	else if(NETWORK.WIFI_TransRdy)
	{
		if(WIFI.Transmit_Receive())
		{
//			NETWORK.TransmitBuf = mymalloc(SRAMEX,WIFI.ReceiveLen+1);
//			memset(NETWORK.TransmitBuf,'\0',WIFI.ReceiveLen+1);
			SYS_MALLOC(NETWORK.TransmitBuf,SRAMEX,WIFI.ReceiveLen+1);
			
			memcpy(NETWORK.TransmitBuf,WIFI.TCP_Rxbuf,WIFI.ReceiveLen);
			NETWORK.TransmitBufLen = WIFI.ReceiveLen;
			WIFI.ReceiveLen = 0;
//			myfree(SRAMEX,WIFI.TCP_Rxbuf);
			SYS_FREE(WIFI.TCP_Rxbuf,SRAMEX);
			
			return True;
		}
		else return False;
	}
	else return False;
	
}


u8 NetWork_WIFI_Control(_wifi_control opt)
{
	OS_ERR err;
	//参数读取buf
//	u8 I_buf[30]={'\0'},P_buf[30]={'\0'},S_buf[5]={'\0'};
	u8 *vBuf_1=NULL,*vBuf_2=NULL,*vBuf_3=NULL;
	
	u8 timeout = 0;
	_wifi_callback res;
	u8 retemp;

	switch(opt)
	{
		case Restart://WIFI重置
		{
			WIFI.Disable_Set();
			OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err);
			WIFI.Enable_Set();
			do
			{
				res = WIFI.CallBack_Process(NULL,NULL,NULL);
				timeout ++;
				if(timeout > 25)
				{
#if NET_WIFI_DEBUG
					debug("Wifi restart timeout\r\n");
#endif
					return 0;
				}
				OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
			}
			while(!res);
			
			if(res == W_START) return 1;
			else return 0;
		}
		
		case Ask://WIFI询问
		{
			WIFI.Test();
			do
			{
				res = WIFI.CallBack_Process(NULL,NULL,NULL);
				timeout ++;
				if(timeout > 5)
				{
#if NET_WIFI_DEBUG
					debug("AT+TEST timeout\r\n");
#endif
					return 0;
				}
				OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
			}
			while(!res);
			
			switch(res)
			{
				case W_OK: 
#if NET_WIFI_DEBUG
					debug("AT+TEST OK\r\n");
#endif				
				return 1;
				case W_ERROR:
#if NET_WIFI_DEBUG					
				debug("AT+TEST ERR\r\n");
#endif				
				return 0;
				default:return res;
			}
		}
		
		case ModeCom://进入命令模式
		{
			WIFI.TransmitSet(1);
			do
			{
				res = WIFI.CallBack_Process(NULL,NULL,NULL);
				timeout ++;
				if(timeout > 5)
				{
#if NET_WIFI_DEBUG					
					debug("+++ timeout\r\n");
#endif					
					return 0;
				}
				OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
			}
			while(!res);
			if(res == W_OK)
			{ 
#if NET_WIFI_DEBUG
				debug("+++ OK\r\n"); 
#endif				
				return 1;
			}
			else 	return 2;	
		}
		
		case ConnectSta://查询WIFI连接状态
		{
//			vBuf_1 = mymalloc(SRAMIN,30);//SSID
//			vBuf_2 = mymalloc(SRAMIN,30);//PWD
//			vBuf_3 = mymalloc(SRAMIN,5);//STATUS
			SYS_MALLOC(vBuf_1,SRAMIN,50);//SSID
			SYS_MALLOC(vBuf_2,SRAMIN,50);//PWD
			SYS_MALLOC(vBuf_3,SRAMIN,50);//STATUS
			
			WIFI.Connect_Func(0);
			do
			{
				res = WIFI.CallBack_Process(vBuf_1,vBuf_2,vBuf_3);
				timeout ++;
				if(timeout > 10)
				{
#if NET_WIFI_DEBUG
					debug("AT+CONNECT timeout\r\n");
#endif				
//					myfree(SRAMIN,vBuf_1);
//					myfree(SRAMIN,vBuf_2);
//					myfree(SRAMIN,vBuf_3);
					SYS_FREE(vBuf_1,SRAMIN);
					SYS_FREE(vBuf_2,SRAMIN);
					SYS_FREE(vBuf_3,SRAMIN);
					
					return 0;
				}
				OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
			}
			while(!res);
			if(res == W_OK)
			{
#if NET_WIFI_DEBUG
				debug("AT+CONNECT OK\r\n%s\r\n%s\r\n%s\r\n",vBuf_1,vBuf_2,vBuf_3);
#endif				
				if(!(strcmp((char *)WIFI.Current_SSID,(char *)vBuf_1) || strcmp((char *)WIFI.Current_PWD,(char *)vBuf_2))) 
				{
					if(strstr((char *)vBuf_3,"YES")) retemp = 1;
					else if(strstr((char *)vBuf_3,"NO")) retemp =  2;
					else retemp =  3;
				}
				else retemp =  4;
			}
			else if(res == W_FAIL) retemp = 5;
			else retemp = 0x0F;
//			myfree(SRAMIN,vBuf_1);
//			myfree(SRAMIN,vBuf_2);
//			myfree(SRAMIN,vBuf_3);
				SYS_FREE(vBuf_1,SRAMIN);
				SYS_FREE(vBuf_2,SRAMIN);
				SYS_FREE(vBuf_3,SRAMIN);
		}return retemp;
		
		case TcpSta://查询TCP连接状态
		{
//			debug("Check the TCP state...\r\n");
//			vBuf_1 = mymalloc(SRAMIN,30);//IP
//			vBuf_2 = mymalloc(SRAMIN,30);//PORT
//			vBuf_3 = mymalloc(SRAMIN,5);//STATUS
			SYS_MALLOC(vBuf_1,SRAMIN,50);//IP
			SYS_MALLOC(vBuf_2,SRAMIN,50);//PORT
			SYS_MALLOC(vBuf_3,SRAMIN,50);//STATUS
			
			WIFI.TCPClinet_Set(NULL,NULL,0);
			do
			{
				res = WIFI.CallBack_Process(vBuf_1,vBuf_2,vBuf_3);
				timeout ++;
				if(timeout > 15)
				{
#if NET_WIFI_DEBUG
					debug("AT+TCP timeout\r\n");
#endif					
//					myfree(SRAMIN,vBuf_1);
//					myfree(SRAMIN,vBuf_2);
//					myfree(SRAMIN,vBuf_3);
					SYS_FREE(vBuf_1,SRAMIN);
					SYS_FREE(vBuf_2,SRAMIN);
					SYS_FREE(vBuf_3,SRAMIN);
					return 0;
				}
				OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
			}
			while(!res);
			if(res == W_OK)
			{
#if NET_WIFI_DEBUG
				debug("AT+TCPClinet OK: %s:%s sta=%s\r\n",vBuf_1,vBuf_2,vBuf_3);
#endif				
				if(!(strcmp((char *)NETWORK.Current_IPadd,(char *)vBuf_1) || strcmp((char *)NETWORK.Current_Port,(char *)vBuf_2)))
				{
					if(strstr((char *)vBuf_3,"YES")) retemp =  1;
					else if(strstr((char *)vBuf_3,"NO")) retemp =  2;
					else retemp =  3;
				}
				else retemp =  4;
				
			}
//				myfree(SRAMIN,vBuf_1);
//				myfree(SRAMIN,vBuf_2);
//				myfree(SRAMIN,vBuf_3);
					SYS_FREE(vBuf_1,SRAMIN);
					SYS_FREE(vBuf_2,SRAMIN);
					SYS_FREE(vBuf_3,SRAMIN);
		}return retemp;
		
		case StationSet://WIFI模式 （设置为station）
		{
			WIFI.Mode_Set(station);
			do
			{
				res = WIFI.CallBack_Process(NULL,NULL,NULL);
				timeout ++;
				if(timeout > 5) 
				{
#if NET_WIFI_DEBUG
					debug("mode set timeout\r\n");
#endif					
					return 0;
				}
				OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
			}
			while(!res);
			if(res == W_OK) return 1;
			else return 0;		
		}
		
		case SsidSet://SSID设置
		{
//			strcpyWIFI.Current_SSID
			WIFI.SSID_Set(WIFI.Current_SSID);
			do
			{
				res = WIFI.CallBack_Process(NULL,NULL,NULL);
				timeout ++;
				if(timeout > 10) 
				{
#if NET_WIFI_DEBUG
					debug("SSID set timeout\r\n");
#endif					
					return 0;
				}
				OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
			}
			while(!res);
			if(res == W_OK) return 1;
			else return 0;		
		}
		
		case PwdSet://密码设置
		{
			WIFI.PWD_Set(WIFI.Current_PWD);
			do
			{
				res = WIFI.CallBack_Process(NULL,NULL,NULL);
				timeout ++;
				if(timeout > 10)
				{
#if NET_WIFI_DEBUG					
					debug("PWD set timeout\r\n");
#endif	
					return 0;
				}
				OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
			}
			while(!res);
			if(res == W_OK) return 1;
			else return 0;		
		}
		
		case TcpSet://TCP设置
		{
			WIFI.TCPClinet_Set(NETWORK.Current_IPadd,NETWORK.Current_Port,1);
			do
			{
				res = WIFI.CallBack_Process(NULL,NULL,NULL);
				timeout ++;
				if(timeout > 30) 
				{
#if NET_WIFI_DEBUG					
					debug("TCPCLINET set timeout\r\n");
#endif
					return 0;
				}
				OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
			}
			while(!res);
			if(res == W_OK) return 1;
			else return 0;		
		}
		
		case ConnectEn://WIFI启动连接
		{
			WIFI.Connect_Func(1);
			do
			{
				res = WIFI.CallBack_Process(NULL,NULL,NULL);
				timeout ++;
				if(timeout > 30)
				{
#if NET_WIFI_DEBUG					
					debug("STA CONNECT timeout\r\n");
#endif
					return 0;
				}
				OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
			}
			while(!res);
			if(res == W_OK) return 1;
			else return 0;		
		}
		
		case ModeTran://进入透传模式
		{
			WIFI.TransmitSet(0);
			do
			{
				res = WIFI.CallBack_Process(NULL,NULL,NULL);
				timeout ++;
				if(timeout > 5)
				{
#if NET_WIFI_DEBUG					
					debug("AT+EXIT timeout\r\n");
#endif
					return 0;
				}
				OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
			}
			while(!res);
			if(res == W_OK) return 1;
			else return 0;		
		}
		
		case ApSignal://AP扫描(信号获取)
		{
//			vBuf_1 = mymalloc(SRAMIN,30);//SSID
//			vBuf_2 = mymalloc(SRAMIN,5);//SIGNAL
//			vBuf_3 = mymalloc(SRAMIN,10);//CHANNEL
			SYS_MALLOC(vBuf_1,SRAMIN,50);//IP
			SYS_MALLOC(vBuf_2,SRAMIN,50);//PORT
			SYS_MALLOC(vBuf_3,SRAMIN,50);//STATUS
			WIFI.Scan_AP(1,WIFI.Current_SSID);
			do
			{
				res = WIFI.CallBack_Process(vBuf_1,vBuf_2,vBuf_3);
				timeout ++;
				if(timeout > 15)
				{
#if NET_WIFI_DEBUG					
					debug("Scan AP timeout\r\n");
#endif
//					myfree(SRAMIN,vBuf_1);
//					myfree(SRAMIN,vBuf_2);
//					myfree(SRAMIN,vBuf_3);
					SYS_FREE(vBuf_1,SRAMIN);
					SYS_FREE(vBuf_2,SRAMIN);
					SYS_FREE(vBuf_3,SRAMIN);
					return 0;
				}
				OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
			}
			while(!res);
			if(res == W_OK)
			{
#if NET_WIFI_DEBUG
				debug("Scan AP OK\r\n%s\r\n%s\r\n%s\r\n",vBuf_1,vBuf_2,vBuf_3);
#endif				
				if(!strcmp((char *)WIFI.Current_SSID,(char *)vBuf_1))
				{
//					debug("signal = %d",atoi((char*)vBuf_2));
					SYS->WIFI_SignalSt = atoi((char*)vBuf_2);
#if NET_WIFI_DEBUG
				debug("Scan AP signal = %d\r\n",SYS->WIFI_SignalSt);
#endif	
				}
				else retemp =  4;
				
			}
//			myfree(SRAMIN,vBuf_1);
//			myfree(SRAMIN,vBuf_2);
//			myfree(SRAMIN,vBuf_3);			
			SYS_FREE(vBuf_1,SRAMIN);
			SYS_FREE(vBuf_2,SRAMIN);
			SYS_FREE(vBuf_3,SRAMIN);
		}return retemp;
		
		case GetMac://查询WIFI MAC
		{
//			vBuf_1 = mymalloc(SRAMIN,30);//MAC
//			memset(SYS->Device_Mac,'\0',20);
			SYS_MALLOC(vBuf_1,SRAMIN,50);//MAC
			
			WIFI.Get_Mac();
			do
			{
				res = WIFI.CallBack_Process(vBuf_1,NULL,NULL);
				timeout ++;
				if(timeout > 5)
				{
#if NET_WIFI_DEBUG
					debug("AT+MAC timeout\r\n");
#endif				
//					myfree(SRAMIN,vBuf_1);
					SYS_FREE(vBuf_1,SRAMIN);

					return 0;
				}
				OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
			}
			while(!res);
			if(res == W_OK)
			{
#if NET_WIFI_DEBUG
				debug("AT+MAC OK\r\n%s\r\n",vBuf_1);
#endif				
				memset(SYS->Device_Mac,'\0',20);
				strncpy((char*)SYS->Device_Mac,(char*)vBuf_1,17);
//				debug("Device_Mac = %s\r\n",SYS->Device_Mac);
				
			}
			else retemp = 0x0F;
//			myfree(SRAMIN,vBuf_1);
			SYS_FREE(vBuf_1,SRAMIN);

		}return retemp;
		
		case GetIp: //获取STA IP
		{
//			vBuf_1 = mymalloc(SRAMIN,30);//IP
//						memset(vBuf_1,'\0',30);
			SYS_MALLOC(vBuf_1,SRAMIN,50);//IP
			
			WIFI.StaIP_Handle(0,NULL,1);
			
			do
			{
				res = WIFI.CallBack_Process(vBuf_1,NULL,NULL);
				timeout ++;
				if(timeout > 5)
				{
#if NET_WIFI_DEBUG
					debug("AT+IPINFO timeout\r\n");
#endif				
//					myfree(SRAMIN,vBuf_1);
					SYS_FREE(vBuf_1,SRAMIN);

					return 0;
				}
				OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
			}
			while(!res);
			if(res == W_OK)
			{
#if NET_WIFI_DEBUG
				debug("AT+IPINFO OK\r\n%s\r\n",vBuf_1);
#endif				

//				memset(SYS->Device_IP,'\0',20);
//				strcpy((char*)NETWORK.Current_DeviceIP,(char*)vBuf_1);
				return 1;
//				if(strcmp((char*)WIFI.Current_DeviceIP,(char*)SYS->Device_IP)) return 1;
//				else return 2;
				
			}
			else retemp = 0;
//			myfree(SRAMIN,vBuf_1);
			SYS_FREE(vBuf_1,SRAMIN);

		}
		
		case LocalIp:  //设置本地IP
		{
			WIFI.StaIP_Handle(1,SYS->Device_IP,1);
			do
			{
				res = WIFI.CallBack_Process(NULL,NULL,NULL);
				timeout ++;
				if(timeout > 5)
				{
#if NET_WIFI_DEBUG
					debug("AT+SETIP timeout\r\n");
#endif				
					return 0;
				}
				OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
			}
			while(!res);
			if(res == W_OK)
			{
#if NET_WIFI_DEBUG
				debug("AT+IPINFO OK\r\n%s\r\n",vBuf_1);
#endif								
			}
			else retemp = 0x0F;

		}return retemp;	
		
		case SetDhcp:  //设置DHCP
		{
			WIFI.StaIP_Handle(1,NULL,0);
			do
			{
				res = WIFI.CallBack_Process(NULL,NULL,NULL);
				timeout ++;
				if(timeout > 5)
				{
#if NET_WIFI_DEBUG
					debug("SET DHCP timeout\r\n");
#endif				
					return 0;
				}
				OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
			}
			while(!res);
			if(res == W_OK)
			{
#if NET_WIFI_DEBUG
				debug("SET DHCP OK\r\n");
#endif								
			}
			else retemp = 0x0F;

		}return retemp;
		
		
		
		default:return 0;
	}
	
	
}

void WIFI_Initialization(void)
{
//	OS_ERR err;
	u8 res;
	
	WIFI.Config();	
	WIFI.Disable_Set();

	SYS->ServerCommand = False;

	W25QXX_Read(&res,WIFI_InfoUDFlag_addr,1);
	if(res != WIFI_InfoUDFlag) 
	{
		debug("updata wifi info\r\n");
		memset(SYS->WIFI_SSID,'\0',30);
		memcpy(SYS->WIFI_SSID,"TS8209A",strlen("TS8209A"));
		memset(SYS->WIFI_PWD,'\0',30);
		memcpy(SYS->WIFI_PWD,"12345678",strlen("12345678"));
		
		memset(SYS->Device_IP,'\0',20);
		memcpy(SYS->Device_IP,"192.168.1.1",strlen("192.168.1.1"));
		
		SYS->DHCP_Enable = True;

		memset(SYS->TCP_IPadd,'\0',20);
		memset(SYS->TCP_Port,'\0',5);
#if 1	
		memcpy(SYS->TCP_IPadd,"172.16.4.220",strlen("172.16.4.220"));
		memcpy(SYS->TCP_Port,"1117",strlen("1117"));
#else
		memcpy(SYS->TCP_IPadd,"172.16.10.30",strlen("172.16.10.30"));
		memcpy(SYS->TCP_Port,"2340",strlen("2340"));
#endif	
		SYSPara_SaveNLoad(Save,4);
		res = WIFI_InfoUDFlag;
		W25QXX_Write(&res,WIFI_InfoUDFlag_addr,1);
	}
	
	memcpy(WIFI.Current_SSID,SYS->WIFI_SSID,30);
	memcpy(WIFI.Current_PWD,SYS->WIFI_PWD,30);
	memcpy(NETWORK.Current_IPadd,SYS->TCP_IPadd,20);
	memcpy(NETWORK.Current_Port,SYS->TCP_Port,5);
	memcpy(NETWORK.Current_DeviceIP,SYS->Device_IP,20);
}

void NetWork_Format(u8 *network_buf,u8 field)
{
	switch(field)
	{
		case REQ_HEARTBEAT://101心跳
		{
			JSON_CreatORComplete(network_buf,0);
			JSON_AddNumber(network_buf,(u8*)iCmdEnum,field);
			JSON_AddNumber(network_buf,(u8*)iDeviceID,SYS->Device_UID);		
			JSON_AddNumber(network_buf,(u8*)iElectric,SYS->ElectricQua);
			JSON_AddNumber(network_buf,(u8*)iLuminance,SYS->Luminance);
			JSON_AddNumber(network_buf,(u8*)iWifi,SYS->WIFI_SignalSt);
			JSON_CreatORComplete(network_buf,1);
			
		}break;
		case REQ_TS_DEVICE_REG://201
		{
			JSON_CreatORComplete(network_buf,0);
			JSON_AddNumber(network_buf,(u8*)iCmdEnum,field);
			JSON_AddNumber(network_buf,(u8*)iDeviceID,SYS->Device_UID);
			JSON_AddString(network_buf,(u8*)strMac,SYS->Device_Mac);
			JSON_CreatORComplete(network_buf,1);
			
		}break;
		case REQ_TS_GET_USERINFO:
		{
			JSON_CreatORComplete(network_buf,0);
			JSON_AddNumber(network_buf,(u8*)iCmdEnum,field);
			JSON_AddNumber(network_buf,(u8*)iDeviceID,SYS->Device_UID);
			JSON_CreatORComplete(network_buf,1);
		}break;
		case REQ_TS_GET_USERLIST:
		{
			JSON_CreatORComplete(network_buf,0);
			JSON_AddNumber(network_buf,(u8*)iCmdEnum,field);
			JSON_AddNumber(network_buf,(u8*)iDeviceID,SYS->Device_UID);
			JSON_CreatORComplete(network_buf,1);
		}break;
		case EVT_TS_REQSERVICE:
		{
			JSON_CreatORComplete(network_buf,0);
			JSON_AddNumber(network_buf,(u8*)iCmdEnum,field);
			JSON_AddNumber(network_buf,(u8*)iDeviceID,SYS->Device_UID);
			JSON_AddNumber(network_buf,(u8*)iServiceID,SYS->SerID);
			JSON_AddString(network_buf,(u8*)strContent,SYS->ServCont);
			JSON_CreatORComplete(network_buf,1);
		}break;
	  case EVT_TS_SENDMSG:
		{
			JSON_CreatORComplete(network_buf,0);
			JSON_AddNumber(network_buf,(u8*)iCmdEnum,field);
			JSON_AddNumber(network_buf,(u8*)iDeviceID,SYS->Device_UID);
			JSON_AddNumber(network_buf,(u8*)iMsgType,1);
			JSON_AddNumber(network_buf,(u8*)iContentType,0);
			JSON_AddString(network_buf,(u8*)strContent,SYS->MsgCont);
			JSON_AddNumber(network_buf,(u8*)lstRecvDeviceID,(u16)0);
			JSON_AddNumber(network_buf,(u8*)iReceiverID,SYS->MsgReceive_ID);
			JSON_CreatORComplete(network_buf,1);
		}break;
	}
	
}

//opt :0=开始 1=结束
void JSON_CreatORComplete(u8 *json_buf,u8 opt)
{
	if(opt)
	{
		strcat((char*)json_buf,"\"end\":0}");
	}
	else strcat((char*)json_buf,"{");
}

void JSON_AddString(u8 *json_buf,u8 *key,u8 *value)
{
	strcat((char*)json_buf,"\"");
	strcat((char*)json_buf,(char*)key);
	strcat((char*)json_buf,"\":\"");
	strcat((char*)json_buf,(char*)value);
	strcat((char*)json_buf,"\",");
	
}

void JSON_AddNumber(u8 *json_buf,u8 *key,u16 num)
{
	char num2char[5] = {'\0'};
	
	sprintf(num2char,"%d",num);
	
	strcat((char*)json_buf,"\"");
	strcat((char*)json_buf,(char*)key);
	strcat((char*)json_buf,"\":");
	strcat((char*)json_buf,num2char);
	strcat((char*)json_buf,",");
	
}

u16 JSON_GetValue(const u8 *json_buf,const char *key,char *value,u8 mode)
{
	char *temp_buf;
	u16 len;
	u32 add_temp1,add_temp2;
	u8 *temp_key;
	u8 keylen;
	
	keylen = strlen(key);
//	temp_key = mymalloc(SRAMIN,keylen+5);
	SYS_MALLOC(temp_key,SRAMIN,keylen+5);

	sprintf((char*)temp_key,"\"%s\"",key);

	
	temp_buf = strstr((char*)json_buf,(char*)temp_key);
	if(!temp_buf)
	{
//		myfree(SRAMIN,temp_key);
		SYS_FREE(temp_key,SRAMIN);
		return 0;
	}
	
	temp_buf += strlen((char*)key)+2;
	
	add_temp1 = (u32)strstr((char*)temp_buf,",");
	add_temp2 = (u32)strstr((char*)temp_buf,"}");
	
	if(mode) add_temp1 = 0;
	
	if(add_temp1 && add_temp2) len = (add_temp1 < add_temp2 ? strstr((char*)temp_buf,",") : strstr((char*)temp_buf,"}"))  - temp_buf - 1;
	else if(add_temp1 || add_temp2)
	{
		if(add_temp1) len = strstr((char*)temp_buf,",") - temp_buf - 1;
		if(add_temp2) len = strstr((char*)temp_buf,"}") - temp_buf - 1;
	}		
	else len = 0;
	
	temp_buf += 1;
	if(!strncmp(temp_buf,"\"",1) && !strncmp(temp_buf+len-1,"\"",1))
	{
		temp_buf += 1;
		len -= 2;
	}

	strncpy((char*)value,temp_buf,len);
//	memcpy(value,temp_buf,len);
	value[len] = '\0';
//	myfree(SRAMIN,temp_key);
	SYS_FREE(temp_key,SRAMIN);
	return len;
}

_BOOL JSON_GetStr(u8 *json_buf,const char *key,char *value,u16 buflen)
{
	u16 len_temp;
	u8 *Str_buf = NULL;
	_BOOL res;
	
//	Str_buf = mymalloc(SRAMIN,buflen+10);
//	memset(Str_buf,'\0',buflen+10);
	SYS_MALLOC(Str_buf,SRAMIN,buflen+10);


	
	len_temp = JSON_GetValue(json_buf,key,(char*)Str_buf,0);
	if(!len_temp) 
	{
		res = False;
//		debug("GetStr err:0\r\n");
	}
	else if(len_temp > buflen)
	{
		debug("GetStr err:%d\r\n",len_temp);
		res = False;
	}
	else  
	{
		memset(value,'\0',buflen);
		strncpy(value,(char*)Str_buf,len_temp); 
		res = True;
	}
	
//	myfree(SRAMIN,Str_buf);
	SYS_FREE(Str_buf,SRAMIN);
	return res;
	
}

_BOOL JSON_GetNum(const u8 *json_buf,const char *key,u16 *num)
{
  u16 len_temp;
	u8 Str_buf[10];
	u8 i;

	len_temp = JSON_GetValue(json_buf,key,(char*)Str_buf,0);
	if(!len_temp)
	{
		debug("GetNum err:0\r\n");
		return False;
	}
	else  
	{	
		for(i = 0;i < len_temp;i ++)
		{
			if(Str_buf[i]<'0' || Str_buf[i]>'9') 
			{
				debug("GetNum err:%x\r\n",Str_buf[i]);
				return False;
			}
		}
		*num = atoi((char*)Str_buf);
	}

	return True;
}

_BOOL JSON_GetArr(u8 *json_buf,const char *key,char *value,u16 buflen)
{
	u16 len_temp;
	u8 *Str_buf;
	_BOOL res;
	
//	Str_buf = mymalloc(SRAMIN,buflen+10);
//	memset(Str_buf,'\0',buflen+10);
	SYS_MALLOC(Str_buf,SRAMIN,buflen+10);

	
	len_temp = JSON_GetValue(json_buf,key,(char*)Str_buf,1);
	if(!len_temp) 
	{
		res = False;
		debug("GetArr err:0\r\n");
	}
	else if(len_temp > buflen)
	{
		debug("GetArr err:%d\r\n",len_temp);
		res = False;
	}
	else if(Str_buf[0] != '{')
	{
		debug("GetArr err:not arr\r\n");
		res = False;
	}
	else  
	{
		memset(value,'\0',buflen);
		strncpy(value,(char*)Str_buf,len_temp); 
		value[len_temp] = '}';
		value[len_temp+1] = '\0';
		res = True;
	}
	
//	myfree(SRAMIN,Str_buf);
	SYS_FREE(Str_buf,SRAMIN);
	return res;
	
}


/*******************URL->IP\PORT 相关函数********************************/
_BOOL IPaddr_atoi(u8 *ip_str,u8 *ip_num)
{
	u8 i,len,j=0,k=0;;
	u8 ip_temp[4] = {'\0'};
	
//	debug("IP_str:%s\r\n",ip_str);
	if(!IS_IPaddr(ip_str)) return False;
	len = strlen((char*)ip_str);
	
	for(i=0;i<len;i++)
	{
		if(ip_str[i] != '.')
		{
			ip_temp[j] = ip_str[i];
			j++;
		}
		else
		{
			ip_num[k] = atoi((char*)ip_temp);
			memset(ip_temp,'\0',4);
			j =0;
			k++;
		}
	}
	ip_num[k] = atoi((char*)ip_temp);
	
//	debug("IP:%d.%d.%d.%d\r\n",ip_num[0],ip_num[1],ip_num[2],ip_num[3]);
	
	return True;
}

_BOOL IS_IPaddr(u8 *ip)
{
	u8 len,p_num=0;
	signed char p0=0,p1=0,i;
	len = strlen((char*)ip);
	
	if(len < 7 || len > 15) return False;
	p0 = -1;
	for(i=0;i<len;i++)
	{
		if(ip[i] < '0' || ip[i] > '9')
		{
			if(ip[i] == '.') 
			{
				p_num++;
				p1 = i;
				if((p1 - p0 <= 1) || (p1 - p0 > 4)) return False;
				else p0 = p1;
			}
			else return False;
		}
	}
	if(p_num != 3) return False;
	
	return True;
}

_BOOL IS_Port(u8 *port)
{
	u8 len,i;
	u32 num;
	
	len = strlen((char*)port);
	
	if(len < 1 || len > 5) return False;
	
	for(i=0;i<len;i++)
	{
		if(port[i] < '0' || port[i] > '9') return False;
	}
	
	num = atol((char*)port);
	
	if(num == 0 || num > 65535) return False;
	
	return True;
}

_BOOL IS_NUM(u8 *num)
{
	u8 len,i;
	
	len = strlen((char*)num);
	
	for(i=0;i<len;i++)
	{
		if(num[i] < '0' || num[i] > '9')
		{
			return False;
		}
	}
	
	return True;
}

_BOOL URL_GetIPnPORT(u8 *url,u8 *ip,u8 *port)
{
	char *temp_buf;
	u8 len;
	
	temp_buf = strstr((char*)url,"http://");
	if(!temp_buf) return False;
	
	temp_buf += 7;
	len = strstr(temp_buf,":") - temp_buf;
	
	strncpy((char*)ip,temp_buf,len);
//	if(IS_IPaddr(ip))	debug("ip:%s\r\n",ip);
//	else memset((char*)ip,'\0',len);
	if(!IS_IPaddr(ip)) memset((char*)ip,'\0',len);
	
	temp_buf += (len+1);
	len = strstr(temp_buf,"/") - temp_buf;
	
	strncpy((char*)port,temp_buf,len);
//	if(IS_Port(port)) debug("port:%s\r\n",port);
//	else memset((char*)port,'\0',len);
	if(!IS_Port(port)) memset((char*)port,'\0',len);
		
	return True;
}


