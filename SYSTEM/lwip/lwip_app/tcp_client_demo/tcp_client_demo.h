#ifndef _TCP_CLIENT_H
#define _TCP_CLIENT_H
#include "sys.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "lwip/memp.h"
#include "lwip/mem.h"
#include "lwip_comm.h"
  

#define TCP_CLIENT_RX_BUFSIZE	10*1024	//定义tcp client最大接收数据长度
#define TCP_CLIENT_RX_BUFSIZE_Ex		500*1024 //EX接收长度（临时）

#define TCP_CLIENT_TX_BUFSIZE	500		//定义tcp client最大发送数据长度

#define 	Timer2_FrequencyDiv		(9000-1)
#define   Timer2_AutoLoading		(200-1)				//Timer2自动装载置  （10ms）



//tcp服务器连接状态
enum tcp_client_states
{
	ES_TCPCLIENT_NONE = 0,		//没有连接
	ES_TCPCLIENT_CONNECTED,		//连接到服务器了 
	ES_TCPCLIENT_CLOSING,		//关闭连接
}; 

//LWIP回调函数使用的结构体
struct tcp_client_struct
{
	u8 state;               //当前连接状
	struct tcp_pcb *pcb;    //指向当前的pcb
	struct pbuf *p;         //指向接收/或传输的pbuf
}; 

//TCP功能类
typedef struct 
{
	_BOOL Rx_Complete;
	_BOOL Rx_Receiving;
	
	
	u8 TcpClient_STA;
	//TCP Client接收数据缓冲区
	u8 *Rx_Buffer;	
	//TCP服务器接收数据长度
  u32 Rx_BufLen;
	
	//TCP服务器发送数据内容
	u8 *Tx_Buffer;
	//TCP服务器发送数据长度
	u32 Tx_BufLen;
	
	u32 BufferSize;
	
	struct tcp_pcb *TcpPcb;  	//定义一个TCP服务器控制块
//	void (*SetPara)(void);
  err_t (*Connect)(void *,struct tcp_pcb *,err_t);
	_BOOL (*Recv)(void);
	err_t (*Send)(u8 *,u32 );	
	void (*Error)(void *,err_t);
	err_t (*Poll)(void *, struct tcp_pcb *);
	void (*ConnectionClose)(struct tcp_pcb *, struct tcp_client_struct * );
	void (*Init)(void);

		 
}_tcpclient_class;

extern _tcpclient_class TCPCLIEN;





//extern _tcpclient_class TCPCLIENT;
//extern u8 *tcp_client_recvbuf;	
//extern u8 *tcp_client_sendbuf;
//extern u8 tcp_client_flag;	 



#endif
