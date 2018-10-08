#include "wifi.h"
#include "string.h"
#include "sys_malloc.h"
#include "w25qxx.h"
#include "sysdebug.h"

static const u8 _AT_PPP[] = "+++";
static const u8 _AT_HEAD[] = "AT+";
static const u8 _TEST[] = "TEST";
static const u8 _STAID[] = "STAID=";
static const u8 _STAPWD[] = "STAPWD=";
static const u8 _CONNECT[] = "CONNECT";
static const u8 _MODE[] = "MODE=";
static const u8 _TCPCLIENT[] = "TCPCLIENT=";
static const u8 _EXIT[] = "EXIT";
static const u8 _SCAN[] = "SCAN=";
static const u8 _MAC[] = "MAC";
static const u8 _IPINFO[] = "IPINFO=";
static const u8 _CR_LF[] = "\r\n";
static const u8 _INQUIRY[] = "=?";
static const u8 _BLANKSPACE[] = " ";

//WIFI模块回调指令 WIFI_CALLBACK_PROCESS函数调用
static const u8 _OK[] = "OK";
static const u8 _ERROR[] = "ERROR";
static const u8 _FAIL[] = "FAIL";
static const u8 _CB_IP[] = "IP";
static const u8 _CB_PORT[] = "PORT";
static const u8 _CB_STATUS[] = "STATUS";
static const u8 _CB_SSID[] = "SSID";
static const u8 _CB_PWD[] = "PWD";
static const u8 _TCP_READY[] = "transfer pipe ready";
static const u8 _TCP_RST[] = "tcp client restart";
static const u8 _RESTART[] = "os restart";
static const u8 _RECONNECT[] = "station reconnect";
static const u8 _ITCL[] = "ITCL";
static const u8 _CB_CHANNEL[] = "CHANNEL";
//下面这两个均表示wifi信号字段，因为6110厂家将SIGNAL写成SIGHAL，因此多加一个字段做兼容处理，防止厂家后续改回来后不兼容
static const u8 _CB_SIGNAL[] = "SIGNAL";
static const u8 _CB_SIGHAL[] = "SIGHAL";
static const u8 _CB_COLON[] = ":";


static void WIFI_Config(void);
static void WIFI_TRANSMIT_SET(u8 mode);
static void WIFI_MODE_SET(_wifi_mode mode);
static void WIFI_TEST(void);
static void WIFI_SSID_SET(u8 * ssid);
static void WIFI_PWD_SET(u8 *pwd);
static void WIFI_CONNECT_FUNC(u8 func);
static void WIFI_TCPCLINET_SET(u8 *ip,u8 *port,u8 func);
static _wifi_callback WIFI_CALLBACK_PROCESS(u8 * value_1,u8 *value_2,u8 *value_3);
static _BOOL WIFI_TRANSMIT_SEND(u8 *sendbuf,u32 len);
static void WIFI_DISABLE_SET(void);
static void WIFI_ENABLE_SET(void);
static void WIFI_SCAN_AP(u8 opt,u8 *ssid);
static void WIFI_GET_MAC(void);
static _BOOL WIFI_TRANSMIT_RECEIVE(void);
static _BOOL WIFI_UARTBUF_SET(u8 opt);
static void WIFI_STAIP_HANDLE(u8 opt,u8 *IP,u8 mode);


_wifi_class WIFI =
{
	.WIFI_STA = 0,
	.WIFI_STA_Expect = 0x0F,
	
	

	.Config = WIFI_Config,
	.Mode_Set = WIFI_MODE_SET,
	.TransmitSet = WIFI_TRANSMIT_SET,
	.Test = WIFI_TEST,
	.SSID_Set = WIFI_SSID_SET,
	.PWD_Set = WIFI_PWD_SET,
	.Connect_Func = WIFI_CONNECT_FUNC,
	.TCPClinet_Set = WIFI_TCPCLINET_SET,
	.CallBack_Process = WIFI_CALLBACK_PROCESS,
	.Transmit_Send = WIFI_TRANSMIT_SEND,
	.Disable_Set = WIFI_DISABLE_SET,
	.Enable_Set = WIFI_ENABLE_SET,
	.Scan_AP = WIFI_SCAN_AP,
	.Get_Mac = WIFI_GET_MAC,
	.Transmit_Receive = WIFI_TRANSMIT_RECEIVE,
	.UartBuf_Set = WIFI_UARTBUF_SET,
	.StaIP_Handle = WIFI_STAIP_HANDLE,
};


static void WIFI_Config(void)
{
	
	FN6110R_INIT();	
	
}
//透传、命令模式设置 
//mode : 1=命令模式  0=透传模式
static void WIFI_TRANSMIT_SET(u8 mode)
{
	u8 Command[10] = {'\0'};
	if(mode)
	{
		strcpy((char *)Command,(char *)_AT_PPP);
	}
	else
	{
		strcpy((char *)Command,(char *)_AT_HEAD);
		strcat((char *)Command,(char *)_EXIT);
		strcat((char *)Command,(char *)_CR_LF);
	}
	U4.SendBuffer(Command,strlen((char *)Command));
#if CommandDebug
	printf("Command:");
	UART1.SendBuffer(Command,strlen((char *)Command));
#endif
}

static void WIFI_MODE_SET(_wifi_mode mode)
{
	u8 Command[10] = {'\0'};
	char c_mode;

	if(mode < 1 || mode > 3) return;
	c_mode = mode + '0';
	strcpy((char *)Command,(char *)_AT_HEAD);
	strcat((char *)Command,(char *)_MODE);
	strncat((char *)Command,(char *)&c_mode,1);
	strcat((char *)Command,(char *)_CR_LF);
	
	U4.SendBuffer(Command,strlen((char *)Command));
#if CommandDebug
	printf("Command:");
	UART1.SendBuffer(Command,strlen((char *)Command));
#endif
}

static void WIFI_TEST(void)
{
	u8 Command[10] = {'\0'};
	
	strcpy((char *)Command,(char *)_AT_HEAD);
	strcat((char *)Command,(char *)_TEST);
	strcat((char *)Command,(char *)_CR_LF);
	
	U4.SendBuffer(Command,strlen((char *)Command));
#if CommandDebug
	printf("Command:");
	UART1.SendBuffer(Command,strlen((char *)Command));
#endif
}


static void WIFI_SSID_SET(u8 * ssid)
{
	u8 Command[50] = {'\0'};
	
	if( strlen((char *)ssid) > 30) return;
	strcpy((char *)Command,(char *)_AT_HEAD);
	strcat((char *)Command,(char *)_STAID);
	strcat((char *)Command,(char *)ssid);
	strcat((char *)Command,(char *)_CR_LF);
	
	U4.SendBuffer(Command,strlen((char *)Command));
#if CommandDebug
	printf("Command:");
	UART1.SendBuffer(Command,strlen((char *)Command));
#endif	
}

static void WIFI_PWD_SET(u8 *pwd)
{
	u8 Command[50] = {'\0'};
	
	if( strlen((char *)pwd) > 30) return;
	strcpy((char *)Command,(char *)_AT_HEAD);
	strcat((char *)Command,(char *)_STAPWD);
	strcat((char *)Command,(char *)pwd);
	strcat((char *)Command,(char *)_CR_LF);
	
	U4.SendBuffer(Command,strlen((char *)Command));
#if CommandDebug
	printf("Command:");
	UART1.SendBuffer(Command,strlen((char *)Command));
#endif	
}

//wifi连接指令
//func : 1=启动连接 0=查询连接状态
static void WIFI_CONNECT_FUNC(u8 func)
{
	u8 Command[20] = {'\0'};
	if(func)
	{
		strcpy((char *)Command,(char *)_AT_HEAD);
		strcat((char *)Command,(char *)_CONNECT);
		strcat((char *)Command,(char *)_CR_LF);
	}
	else
	{
		strcpy((char *)Command,(char *)_AT_HEAD);
		strcat((char *)Command,(char *)_CONNECT);
		strcat((char *)Command,(char *)_INQUIRY);
		strcat((char *)Command,(char *)_CR_LF);
	}
	
	U4.SendBuffer(Command,strlen((char *)Command));
#if CommandDebug
	printf("Command:");
	UART1.SendBuffer(Command,strlen((char *)Command));
#endif
}

//WIFI TCP连接
//ip:ip地址  port：端口
//func ： 1=启动tcp连接 0=查询连接状态
static void WIFI_TCPCLINET_SET(u8 *ip,u8 *port,u8 func)
{
	u8 Command[40] = {'\0'};
	
	if(func)
	{
//		if(strlen((char *)ip) > 15 || strlen((char *)ip) < 7) return;
//		if(strlen((char *)port) > 5 || strlen((char *)port) < 2) return;
		
		strcpy((char *)Command,(char *)_AT_HEAD);
		strcat((char *)Command,(char *)_TCPCLIENT);
		strcat((char *)Command,(char *)"1,");
		strcat((char *)Command,(char *)ip);
		strcat((char *)Command,(char *)",");
		strcat((char *)Command,(char *)port);
		strcat((char *)Command,(char *)_CR_LF);
	}
	else
	{
		strcpy((char *)Command,(char *)_AT_HEAD);
		strcat((char *)Command,(char *)_TCPCLIENT);
		strncat((char *)Command,(char *)"?",1);
		strcat((char *)Command,(char *)_CR_LF);
	}
	
	U4.SendBuffer(Command,strlen((char *)Command));
#if CommandDebug
	printf("Command:");
	UART1.SendBuffer(Command,strlen((char *)Command));
#endif
}

//opt : 1=扫描特定ap 0=扫描全部AP
static void WIFI_SCAN_AP(u8 opt,u8 *ssid)
{
	u8 Command[30] = {'\0'};
	if(opt)
	{
		strcpy((char *)Command,(char *)_AT_HEAD);
		strcat((char *)Command,(char *)_SCAN);
		strcat((char *)Command,(char *) ssid);
		strcat((char *)Command,(char *)_CR_LF);	
	}
	else
	{
		strcpy((char *)Command,(char *)_AT_HEAD);
		strcat((char *)Command,(char *)_SCAN);
		strcat((char *)Command,(char *) "?");
		strcat((char *)Command,(char *)_CR_LF);	
	}
	U4.SendBuffer(Command,strlen((char *)Command));
#if CommandDebug
	printf("Command:");
	UART1.SendBuffer(Command,strlen((char *)Command));
#endif
}

static void WIFI_GET_MAC(void)
{
	u8 Command[20] = {'\0'};

	strcpy((char *)Command,(char *)_AT_HEAD);
	strcat((char *)Command,(char *)_MAC);
	strcat((char *)Command,(char *)_INQUIRY);
	strcat((char *)Command,(char *)_CR_LF);	

	U4.SendBuffer(Command,strlen((char *)Command));
#if CommandDebug
	printf("Command:");
	UART1.SendBuffer(Command,strlen((char *)Command));
#endif
}

//opt: 0 = 获取当前STAIP 1 = 设置STAIP
//mode: 0 = 使用DHCP  1 = 使能固定IP（IP值有效）
static void WIFI_STAIP_HANDLE(u8 opt,u8 *IP,u8 mode)
{
	u8 Command[30] = {'\0'};

	if(opt)
	{
		if(mode)
		{
			strcpy((char *)Command,(char *)_AT_HEAD);
			strcat((char *)Command,(char *)_IPINFO);
			strcat((char *)Command,(char *)"1,");
			strcat((char *)Command,(char *)"1,");
			strcat((char *)Command,(char *)IP);
			strcat((char *)Command,(char *)_CR_LF);
		}
		else
		{
			strcpy((char *)Command,(char *)_AT_HEAD);
			strcat((char *)Command,(char *)_IPINFO);
			strcat((char *)Command,(char *)"1,");
			strcat((char *)Command,(char *)"0");
			strcat((char *)Command,(char *)_CR_LF);
		}
	}
	else
	{
		strcpy((char *)Command,(char *)_AT_HEAD);
		strcat((char *)Command,(char *)_IPINFO);
		strcat((char *)Command,(char *)"1,");
		strcat((char *)Command,(char *)"?");
		strcat((char *)Command,(char *)_CR_LF);
	}

	U4.SendBuffer(Command,strlen((char *)Command));
#if CommandDebug
	printf("Command:");
	UART1.SendBuffer(Command,strlen((char *)Command));
#endif
}

static _wifi_callback WIFI_CALLBACK_PROCESS(u8 * value_1,u8 *value_2,u8 *value_3)
{
	char *temp;
	u8 len;
	_wifi_callback res;
	
	if(U4.Rx_Complete == True)
	{
#if CommandDebug
		printf("callback : %sbuflen=%d\r\n",U4.Rx_Buffer,U4.Rx_BufLen );
#endif
		if(strstr((char *)U4.Rx_Buffer,(char *)_OK))
		{		
			if(U4.Rx_BufLen > 4)
			{
				if(strstr((char *)U4.Rx_Buffer,(char *)_CB_SSID) && strstr((char *)U4.Rx_Buffer,(char *)_CB_PWD) && strstr((char *)U4.Rx_Buffer,(char *)_CB_STATUS))
				{
					if(value_1 != NULL && value_2 != NULL && value_3 != NULL)
					{
						temp = strstr((char *)U4.Rx_Buffer,(char *)_CB_SSID);
						temp += 5;
						len = strstr((char *)temp,(char *)_CR_LF) - temp;
						if(len>50) len = 50;
						strncpy((char *)value_1,temp,len);
						value_1[len] = '\0';
						
						temp = strstr((char *)U4.Rx_Buffer,(char *)_CB_PWD);
						temp += 4;
						len = strstr((char *)temp,(char *)_CR_LF) - temp;
						if(len>50) len = 50;
						strncpy((char *)value_2,temp,len);
						value_2[len] = '\0';
						
						temp = strstr((char *)U4.Rx_Buffer,(char *)_CB_STATUS);
						temp += 7;
						len = strstr((char *)temp,(char *)_CR_LF) - temp;
						if(len>50) len = 50;
						strncpy((char *)value_3,temp,len);
						value_3[len] = '\0';
					}
				}
				else if(strstr((char *)U4.Rx_Buffer,(char *)_CB_IP)) //包含IP字段
				{
					if(strstr((char *)U4.Rx_Buffer,(char *)_CB_PORT) && strstr((char *)U4.Rx_Buffer,(char *)_CB_STATUS)) //除IP外还有PORT和STATUS字段
					{
						if(value_1 != NULL && value_2 != NULL && value_3 != NULL)
						{
							temp = strstr((char *)U4.Rx_Buffer,(char *)_CB_IP);
							temp += 3;
							len = strstr((char *)temp,(char *)_BLANKSPACE) - temp;
							if(len>50) len = 50;
							strncpy((char *)value_1,temp,len);
							value_1[len] = '\0';
							
							temp = strstr((char *)U4.Rx_Buffer,(char *)_CB_PORT);
							temp += 5;
							len = strstr((char *)temp,(char *)_BLANKSPACE) - temp;
							if(len>50) len = 50;
							strncpy((char *)value_2,temp,len);
							value_2[len] = '\0';
							
							temp = strstr((char *)U4.Rx_Buffer,(char *)_CB_STATUS);
							temp += 7;
							len = strstr((char *)temp,(char *)_CR_LF) - temp;
							if(len>50) len = 50;
							strncpy((char *)value_3,temp,len);
							value_3[len] = '\0';
						}
					}
					else
					{
						if(value_1 != NULL)
						{
							temp = strstr((char *)U4.Rx_Buffer,(char *)_CB_IP);
							temp += 3;
							len = strstr((char *)temp,(char *)_CR_LF) - temp;
							if(len>50) len = 50;
							strncpy((char *)value_1,temp,len);
							value_1[len] = '\0';
						}
					}
				}
				else if(strstr((char *)U4.Rx_Buffer,(char *)_CB_SSID) && strstr((char *)U4.Rx_Buffer,(char *)_CB_CHANNEL) && 
								(strstr((char *)U4.Rx_Buffer,(char *)_CB_SIGNAL) || strstr((char *)U4.Rx_Buffer,(char *)_CB_SIGHAL)))//这里对字段内容做了一个或处理
				{
					if(value_1 != NULL && value_2 != NULL && value_3 != NULL)
					{
						temp = strstr((char *)U4.Rx_Buffer,(char *)_CB_SSID);
						temp += 5;
						len = strstr((char *)temp,(char *)_BLANKSPACE) - temp;
						if(len>50) len = 50;
						strncpy((char *)value_1,temp,len);
						value_1[len] = '\0';
						
						if(strstr((char *)U4.Rx_Buffer,(char *)_CB_SIGHAL)) temp = strstr((char *)U4.Rx_Buffer,(char *)_CB_SIGHAL);
						else if(strstr((char *)U4.Rx_Buffer,(char *)_CB_SIGNAL)) temp = strstr((char *)U4.Rx_Buffer,(char *)_CB_SIGNAL);
						
						temp += 8;
						len = strstr((char *)temp,(char *)_BLANKSPACE) - temp;
						if(len>50) len = 50;
						strncpy((char *)value_2,temp,len);
						value_2[len] = '\0';
						
						temp = strstr((char *)U4.Rx_Buffer,(char *)_CB_CHANNEL);
						temp += 8;
						len = strstr((char *)temp,(char *)_BLANKSPACE) - temp;
						if(len>50) len = 50;
						strncpy((char *)value_3,temp,len);
						value_3[len] = '\0';
					}
				}
				else if((strstr((char *)U4.Rx_Buffer,(char *)_CB_COLON) && strstr((char *)U4.Rx_Buffer+7,(char *)_CB_COLON)) || strstr((char *)U4.Rx_Buffer,(char *)_MAC)) //判断不止一个冒号或者包含MAC
				{
					if(value_1 != NULL)
					{
						temp = strstr((char *)U4.Rx_Buffer,(char *)_CB_COLON) - 2;
						len = 17;
						strncpy((char *)value_1,temp,len);
						value_1[len] = '\0';
					}
				}			
			}			
			res = W_OK;
		}
		else if(strstr((char *)U4.Rx_Buffer,(char *)_ITCL)) 
		{
			res = W_SERVER;
//			WIFI.TCP_Rxbuf = mymalloc(SRAMEX,U4.Rx_BufLen+1);
//			memset(WIFI.TCP_Rxbuf,'\0',U4.Rx_BufLen+1);
			SYS_MALLOC(WIFI.TCP_Rxbuf,SRAMEX,U4.Rx_BufLen+1);

			memcpy(WIFI.TCP_Rxbuf,U4.Rx_Buffer,U4.Rx_BufLen);
			WIFI.ReceiveLen = U4.Rx_BufLen;
		}
		else if(strstr((char *)U4.Rx_Buffer,(char *)_TCP_RST)) res = W_TCP_RST;
		else if(strstr((char *)U4.Rx_Buffer,(char *)_RECONNECT)) res = W_RECONNECT;
		else if(strstr((char *)U4.Rx_Buffer,(char *)_ERROR)) res = W_ERROR;
		else if(strstr((char *)U4.Rx_Buffer,(char *)_FAIL)) res = W_FAIL;
		else if(strstr((char *)U4.Rx_Buffer,(char *)_TCP_READY)) res = W_TCP_READY;	
		else if(strstr((char *)U4.Rx_Buffer,(char *)_RESTART)) res = W_START;	
		else res = W_NULL;
		
		memset(U4.Rx_Buffer,'\0',U4.Rx_BufLen);
		U4.Rx_Complete = False;
		U4.Rx_BufLen = 0;
		
//			printf("\r\n");
		return res;
	}
	else return W_NULL;
}
//透传发送
static _BOOL WIFI_TRANSMIT_SEND(u8 *sendbuf,u32 len)
{
	if(U4.Rx_Receiving == False)
	{
		U4.SendBuffer(sendbuf,len);
		return True;
	}
	else return False;
}

static _BOOL WIFI_TRANSMIT_RECEIVE(void)
{
	if(U4.Rx_Complete == True)
	{
//		WIFI.TCP_Rxbuf = mymalloc(SRAMEX,U4.Rx_BufLen+5);
//		memset(WIFI.TCP_Rxbuf,'\0',U4.Rx_BufLen+5);
		SYS_MALLOC(WIFI.TCP_Rxbuf,SRAMEX,U4.Rx_BufLen+5);
		memcpy(WIFI.TCP_Rxbuf,U4.Rx_Buffer,U4.Rx_BufLen);
		WIFI.ReceiveLen = U4.Rx_BufLen;
		
		memset(U4.Rx_Buffer,'\0',U4.Rx_BufLen);
		U4.Rx_Complete = False;
		U4.Rx_BufLen = 0;
		return True;
	}
	else return False;
}

//opt: 1=ex模式 2=普通模式
static _BOOL WIFI_UARTBUF_SET(u8 opt)
{
	if(opt)//ex mode
	{
//		myfree(SRAMEX,U4.Rx_Buffer);
//		U4.Rx_Buffer = NULL;
//		U4.Rx_Buffer = mymalloc(SRAMEX,WIFI_UartBuf_LEN_Ex);
		SYS_FREE(U4.Rx_Buffer,SRAMEX);
		U4.Max_RxBufLen = WIFI_UartBuf_LEN_Ex;
		SYS_MALLOC(U4.Rx_Buffer,SRAMEX,WIFI_UartBuf_LEN_Ex);
		if(U4.Rx_Buffer == NULL)
		{
//			U4.Rx_Buffer = mymalloc(SRAMEX,WIFI_UartBuf_LEN);
			debug("malloc wifi u4 uartbuf fail\r\n");
			SYS_MALLOC(U4.Rx_Buffer,SRAMEX,WIFI_UartBuf_LEN);
			return False;
		}
		else return True;
	}
	else
	{
//		myfree(SRAMEX,U4.Rx_Buffer);
//		U4.Rx_Buffer = NULL;
//		U4.Rx_Buffer = mymalloc(SRAMEX,WIFI_UartBuf_LEN);
		SYS_FREE(U4.Rx_Buffer,SRAMEX);
		U4.Max_RxBufLen = WIFI_UartBuf_LEN;
		SYS_MALLOC(U4.Rx_Buffer,SRAMEX,WIFI_UartBuf_LEN);
		return True;
	}
	
}

static void WIFI_DISABLE_SET(void)
{
	CHIP_EN = 0;
}

static void WIFI_ENABLE_SET(void)
{
	CHIP_EN = 1;
}
