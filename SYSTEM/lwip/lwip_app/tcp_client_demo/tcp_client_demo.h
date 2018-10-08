#ifndef _TCP_CLIENT_H
#define _TCP_CLIENT_H
#include "sys.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "lwip/memp.h"
#include "lwip/mem.h"
#include "lwip_comm.h"
  

#define TCP_CLIENT_RX_BUFSIZE	10*1024	//����tcp client���������ݳ���
#define TCP_CLIENT_RX_BUFSIZE_Ex		500*1024 //EX���ճ��ȣ���ʱ��

#define TCP_CLIENT_TX_BUFSIZE	500		//����tcp client��������ݳ���

#define 	Timer2_FrequencyDiv		(9000-1)
#define   Timer2_AutoLoading		(200-1)				//Timer2�Զ�װ����  ��10ms��



//tcp����������״̬
enum tcp_client_states
{
	ES_TCPCLIENT_NONE = 0,		//û������
	ES_TCPCLIENT_CONNECTED,		//���ӵ��������� 
	ES_TCPCLIENT_CLOSING,		//�ر�����
}; 

//LWIP�ص�����ʹ�õĽṹ��
struct tcp_client_struct
{
	u8 state;               //��ǰ����״
	struct tcp_pcb *pcb;    //ָ��ǰ��pcb
	struct pbuf *p;         //ָ�����/�����pbuf
}; 

//TCP������
typedef struct 
{
	_BOOL Rx_Complete;
	_BOOL Rx_Receiving;
	
	
	u8 TcpClient_STA;
	//TCP Client�������ݻ�����
	u8 *Rx_Buffer;	
	//TCP�������������ݳ���
  u32 Rx_BufLen;
	
	//TCP������������������
	u8 *Tx_Buffer;
	//TCP�������������ݳ���
	u32 Tx_BufLen;
	
	u32 BufferSize;
	
	struct tcp_pcb *TcpPcb;  	//����һ��TCP���������ƿ�
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
