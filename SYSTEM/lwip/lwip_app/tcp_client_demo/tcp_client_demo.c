#include "tcp_client_demo.h" 
#include "delay.h"
#include "usart.h"
#include "led.h"
//#include "key.h"
#include "lcd.h"
#include "sys_malloc.h"
#include "stdio.h"
#include "string.h" 
#include "includes.h"


static TIM_HandleTypeDef TIM2_Handler;      //定时器句柄 
static u8 TCP_recv_Timeout;
 
//void tcp_client_set_remoteip(void);
//void tcp_client_test(void);//TCP Client测试函数
err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err);
err_t tcp_client_recv(void *arg,struct tcp_pcb *tpcb,struct pbuf *p,err_t err);
err_t tcp_client_usersent(struct tcp_pcb *tpcb);
void tcp_client_error(void *arg,err_t err);
err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb);
err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
void tcp_client_senddata(struct tcp_pcb *tpcb, struct tcp_client_struct * es);
void tcp_client_connection_close(struct tcp_pcb *tpcb, struct tcp_client_struct * es );
err_t tcpclient_transmit_send(u8 *sendbuf,u32 len);
void TIM2_INIT(void);
void tcp_clinet_init(void);
_BOOL tcpclient_transmit_receive(void);
 
 //TCP功能
_tcpclient_class TCPCLIEN = 
{
	.Rx_Complete = False,
	.Rx_Receiving = False,
	.Rx_BufLen = 0,
	
	
	.Connect = tcp_client_connected,
	.Recv = tcpclient_transmit_receive,
	.Send = tcpclient_transmit_send,
	.Error = tcp_client_error,
	.Poll = tcp_client_poll,
	.ConnectionClose = tcp_client_connection_close,
	.Init = tcp_clinet_init,
};


void tcp_clinet_init(void)
{
	TIM2_INIT();
}

//lwIP TCP连接建立后调用回调函数
err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
	struct tcp_client_struct *es=NULL;  
	if(err==ERR_OK)   
	{
		es=(struct tcp_client_struct*)mem_malloc(sizeof(struct tcp_client_struct));  //申请内存
		if(es) //内存申请成功
		{
 			es->state=ES_TCPCLIENT_CONNECTED;//状态为连接成功
			es->pcb=tpcb;  
			es->p=NULL; 
			tcp_arg(tpcb,es);        			//使用es更新tpcb的callback_arg
			tcp_recv(tpcb,tcp_client_recv);  	//初始化LwIP的tcp_recv回调功能   
			tcp_err(tpcb,tcp_client_error); 	//初始化tcp_err()回调函数
			tcp_sent(tpcb,tcp_client_sent);		//初始化LwIP的tcp_sent回调功能
			tcp_poll(tpcb,tcp_client_poll,1); 	//初始化LwIP的tcp_poll回调功能 
 			TCPCLIEN.TcpClient_STA|=1<<5; 				//标记连接到服务器了
			err=ERR_OK;
		}else
		{ 
			tcp_client_connection_close(tpcb,es);//关闭连接
			err=ERR_MEM;	//返回内存分配错误
		}
	}else
	{
		tcp_client_connection_close(tpcb,0);//关闭连接
	}
	return err;
}
    
//lwIP tcp_recv()函数的回调函数
//TCP_CLIENT数据接收
err_t tcp_client_recv(void *arg,struct tcp_pcb *tpcb,struct pbuf *p,err_t err)
{ 
//	u32 data_len = 0;
	struct pbuf *q;
	struct tcp_client_struct *es;
	err_t ret_err; 
	LWIP_ASSERT("arg != NULL",arg != NULL);
	es=(struct tcp_client_struct *)arg; 
	if(p==NULL)//如果从服务器接收到空的数据帧就关闭连接
	{
		es->state=ES_TCPCLIENT_CLOSING;//需要关闭TCP 连接了 
 		es->p=p; 
		ret_err=ERR_OK;
	}else if(err!= ERR_OK)//当接收到一个非空的数据帧,但是err!=ERR_OK
	{ 
		if(p)pbuf_free(p);//释放接收pbuf
		ret_err=err;
	}else if(es->state==ES_TCPCLIENT_CONNECTED)	//当处于连接状态时
	{
		if(p!=NULL)//当处于连接状态并且接收到的数据不为空时
		{
			if(TCPCLIEN.Rx_Receiving == False)  //串口接收到信息打开定时器
			{
				TCPCLIEN.Rx_Receiving = True;
				TCPCLIEN.Rx_Complete = False;
				TCPCLIEN.Rx_BufLen = 0;
				memset(TCPCLIEN.Rx_Buffer,'\0',TCPCLIEN.BufferSize);
				HAL_TIM_Base_Start_IT(&TIM2_Handler);
	//			printf("U4 get\r\n");
			}
			TCP_recv_Timeout = 0;
//			memset(TCPCLIEN.Rx_Buffer,0,TCP_CLIENT_RX_BUFSIZE);  //数据接收缓冲区清零
			for(q=p;q!=NULL;q=q->next)  //遍历完整个pbuf链表
			{
				//判断要拷贝到TCP_CLIENT_RX_BUFSIZE中的数据是否大于TCP_CLIENT_RX_BUFSIZE的剩余空间，如果大于
				//的话就只拷贝TCP_CLIENT_RX_BUFSIZE中剩余长度的数据，否则的话就拷贝所有的数据
				if(q->len > (TCPCLIEN.BufferSize-TCPCLIEN.Rx_BufLen)) memcpy(TCPCLIEN.Rx_Buffer+TCPCLIEN.Rx_BufLen,q->payload,(TCPCLIEN.BufferSize-TCPCLIEN.Rx_BufLen));//拷贝数据
				else memcpy(TCPCLIEN.Rx_Buffer+TCPCLIEN.Rx_BufLen,q->payload,q->len);
//				printf("len:%d\r\nstr:\r\n%s\r\n",q->len,q->payload);
				TCPCLIEN.Rx_BufLen += q->len;  	
				if(TCPCLIEN.Rx_BufLen > TCPCLIEN.BufferSize) break; //超出TCP客户端接收数组,跳出	
			}
//		
 			tcp_recved(tpcb,p->tot_len);//用于获取接收数据,通知LWIP可以获取更多数据
			pbuf_free(p);  	//释放内存
			ret_err=ERR_OK;
		}
	}else  //接收到数据但是连接已经关闭,
	{ 
		tcp_recved(tpcb,p->tot_len);//用于获取接收数据,通知LWIP可以获取更多数据
		es->p=NULL;
		pbuf_free(p); //释放内存
		ret_err=ERR_OK;
	}
	return ret_err;
}

//lwIP tcp_err函数的回调函数
void tcp_client_error(void *arg,err_t err)
{  
	//这里我们不做任何处理
} 


//TCP发数据函数
err_t tcpclient_transmit_send(u8 *sendbuf,u32 len)
{
	err_t ret_err;
	
//	printf("len = %d\r\ntcpclient->sendbuf:%s\r\n",len,sendbuf);
	TCPCLIEN.Tx_BufLen = len;
//	if(TCPCLIEN.Tx_Buffer != NULL) myfree(SRAMIN,TCPCLIEN.Tx_Buffer);
	
//	TCPCLIEN.Tx_Buffer = mymalloc(SRAMIN,TCPCLIEN.Tx_BufLen+1);
//	memset(TCPCLIEN.Tx_Buffer,'\0',TCPCLIEN.Tx_BufLen+1);
	SYS_MALLOC(TCPCLIEN.Tx_Buffer,SRAMIN,TCPCLIEN.Tx_BufLen+1);

	memcpy(TCPCLIEN.Tx_Buffer,sendbuf,TCPCLIEN.Tx_BufLen);
//	printf("tcpclient->TCPCLIEN.Tx_Buffer:%s\r\n",TCPCLIEN.Tx_Buffer);
	ret_err = tcp_client_usersent(TCPCLIEN.TcpPcb);
	
//	myfree(SRAMIN,TCPCLIEN.Tx_Buffer);
	SYS_FREE(TCPCLIEN.Tx_Buffer,SRAMIN);

	return ret_err;
}

_BOOL tcpclient_transmit_receive(void)
{
	if(TCPCLIEN.Rx_Complete)
	{
		
		return True;
	}
	else return False;
}

//LWIP数据发送，用户应用程序调用此函数来发送数据
//tpcb:TCP控制块
//返回值:0，成功；其他，失败
err_t tcp_client_usersent(struct tcp_pcb *tpcb)
{
    err_t ret_err;
	struct tcp_client_struct *es; 
	es=tpcb->callback_arg;
	if(es!=NULL)  //连接处于空闲可以发送数据
	{
        es->p=pbuf_alloc(PBUF_TRANSPORT, TCPCLIEN.Tx_BufLen ,PBUF_POOL);	//申请内存 
        pbuf_take(es->p,(char*)TCPCLIEN.Tx_Buffer,TCPCLIEN.Tx_BufLen);	//将tcp_client_sentbuf[]中的数据拷贝到es->p_tx中
        tcp_client_senddata(tpcb,es);//将tcp_client_sentbuf[]里面复制给pbuf的数据发送出去
        TCPCLIEN.TcpClient_STA&=~(1<<7);	//清除数据发送标志
        if(es->p)pbuf_free(es->p);	//释放内存
		ret_err=ERR_OK;
	}else
	{ 
		tcp_abort(tpcb);//终止连接,删除pcb控制块
		ret_err=ERR_ABRT;
	}
	return ret_err;
}
//lwIP tcp_poll的回调函数
err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb)
{
	err_t ret_err;
	struct tcp_client_struct *es; 
	es=(struct tcp_client_struct*)arg;
    if(es->state==ES_TCPCLIENT_CLOSING)         //连接断开
	{ 
 		tcp_client_connection_close(tpcb,es);   //关闭TCP连接
	} 
	ret_err=ERR_OK;
    return ret_err;
} 
//lwIP tcp_sent的回调函数(当从远端主机接收到ACK信号后发送数据)
err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	struct tcp_client_struct *es;
	LWIP_UNUSED_ARG(len);
	es=(struct tcp_client_struct*)arg;
	if(es->p)tcp_client_senddata(tpcb,es);//发送数据
	return ERR_OK;
}
//此函数用来发送数据
void tcp_client_senddata(struct tcp_pcb *tpcb, struct tcp_client_struct * es)
{
	struct pbuf *ptr; 
 	err_t wr_err=ERR_OK;
	while((wr_err==ERR_OK)&&es->p&&(es->p->len<=tcp_sndbuf(tpcb)))
	{
		ptr=es->p;
		wr_err=tcp_write(tpcb,ptr->payload,ptr->len,1); //将要发送的数据加入到发送缓冲队列中
		if(wr_err==ERR_OK)
		{  
			es->p=ptr->next;			//指向下一个pbuf
			if(es->p)pbuf_ref(es->p);	//pbuf的ref加一
			pbuf_free(ptr);				//释放ptr 
		}else if(wr_err==ERR_MEM)es->p=ptr;
        tcp_output(tpcb);		        //将发送缓冲队列中的数据立即发送出去
	} 	
} 
//关闭与服务器的连接
void tcp_client_connection_close(struct tcp_pcb *tpcb, struct tcp_client_struct * es)
{
	//移除回调
	tcp_abort(tpcb);//终止连接,删除pcb控制块
	tcp_arg(tpcb,NULL);  
	tcp_recv(tpcb,NULL);
	tcp_sent(tpcb,NULL);
	tcp_err(tpcb,NULL);
	tcp_poll(tpcb,NULL,0);  
	if(es)mem_free(es); 
	TCPCLIEN.TcpClient_STA&=~(1<<5);//标记连接断开了
}


//TCP接收专用定时器
void TIM2_INIT(void)
{	
	TIM2_Handler.Instance=TIM2;                          //通用定时器4
	TIM2_Handler.Init.Prescaler=Timer2_FrequencyDiv;      //分频系数
	TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
	TIM2_Handler.Init.Period=Timer2_AutoLoading;         //自动装载值
	TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
	HAL_TIM_Base_Init(&TIM2_Handler);
}


//定时器5中断服务函数

void TIM2_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif
	if(__HAL_TIM_GET_FLAG(&TIM2_Handler, TIM_FLAG_UPDATE) != RESET)
  {
		if(__HAL_TIM_GET_IT_SOURCE(&TIM2_Handler, TIM_IT_UPDATE) !=RESET)
    {
			__HAL_TIM_CLEAR_IT(&TIM2_Handler, TIM_IT_UPDATE);
			TCP_recv_Timeout++;
			if(TCP_recv_Timeout >= 30)
			{
					TCPCLIEN.TcpClient_STA|=1<<6;		//标记接收到数据了
				TCPCLIEN.Rx_Complete = True;
				TCPCLIEN.Rx_Receiving = False;
				TCP_recv_Timeout = 0;
				HAL_TIM_Base_Stop_IT(&TIM2_Handler);
//				printf("UART4 Timeout\r\n");
			}
		}
    
  }
	
	HAL_TIM_IRQHandler(&TIM2_Handler);
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
}


