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


static TIM_HandleTypeDef TIM2_Handler;      //��ʱ����� 
static u8 TCP_recv_Timeout;
 
//void tcp_client_set_remoteip(void);
//void tcp_client_test(void);//TCP Client���Ժ���
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
 
 //TCP����
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

//lwIP TCP���ӽ�������ûص�����
err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
	struct tcp_client_struct *es=NULL;  
	if(err==ERR_OK)   
	{
		es=(struct tcp_client_struct*)mem_malloc(sizeof(struct tcp_client_struct));  //�����ڴ�
		if(es) //�ڴ�����ɹ�
		{
 			es->state=ES_TCPCLIENT_CONNECTED;//״̬Ϊ���ӳɹ�
			es->pcb=tpcb;  
			es->p=NULL; 
			tcp_arg(tpcb,es);        			//ʹ��es����tpcb��callback_arg
			tcp_recv(tpcb,tcp_client_recv);  	//��ʼ��LwIP��tcp_recv�ص�����   
			tcp_err(tpcb,tcp_client_error); 	//��ʼ��tcp_err()�ص�����
			tcp_sent(tpcb,tcp_client_sent);		//��ʼ��LwIP��tcp_sent�ص�����
			tcp_poll(tpcb,tcp_client_poll,1); 	//��ʼ��LwIP��tcp_poll�ص����� 
 			TCPCLIEN.TcpClient_STA|=1<<5; 				//������ӵ���������
			err=ERR_OK;
		}else
		{ 
			tcp_client_connection_close(tpcb,es);//�ر�����
			err=ERR_MEM;	//�����ڴ�������
		}
	}else
	{
		tcp_client_connection_close(tpcb,0);//�ر�����
	}
	return err;
}
    
//lwIP tcp_recv()�����Ļص�����
//TCP_CLIENT���ݽ���
err_t tcp_client_recv(void *arg,struct tcp_pcb *tpcb,struct pbuf *p,err_t err)
{ 
//	u32 data_len = 0;
	struct pbuf *q;
	struct tcp_client_struct *es;
	err_t ret_err; 
	LWIP_ASSERT("arg != NULL",arg != NULL);
	es=(struct tcp_client_struct *)arg; 
	if(p==NULL)//����ӷ��������յ��յ�����֡�͹ر�����
	{
		es->state=ES_TCPCLIENT_CLOSING;//��Ҫ�ر�TCP ������ 
 		es->p=p; 
		ret_err=ERR_OK;
	}else if(err!= ERR_OK)//�����յ�һ���ǿյ�����֡,����err!=ERR_OK
	{ 
		if(p)pbuf_free(p);//�ͷŽ���pbuf
		ret_err=err;
	}else if(es->state==ES_TCPCLIENT_CONNECTED)	//����������״̬ʱ
	{
		if(p!=NULL)//����������״̬���ҽ��յ������ݲ�Ϊ��ʱ
		{
			if(TCPCLIEN.Rx_Receiving == False)  //���ڽ��յ���Ϣ�򿪶�ʱ��
			{
				TCPCLIEN.Rx_Receiving = True;
				TCPCLIEN.Rx_Complete = False;
				TCPCLIEN.Rx_BufLen = 0;
				memset(TCPCLIEN.Rx_Buffer,'\0',TCPCLIEN.BufferSize);
				HAL_TIM_Base_Start_IT(&TIM2_Handler);
	//			printf("U4 get\r\n");
			}
			TCP_recv_Timeout = 0;
//			memset(TCPCLIEN.Rx_Buffer,0,TCP_CLIENT_RX_BUFSIZE);  //���ݽ��ջ���������
			for(q=p;q!=NULL;q=q->next)  //����������pbuf����
			{
				//�ж�Ҫ������TCP_CLIENT_RX_BUFSIZE�е������Ƿ����TCP_CLIENT_RX_BUFSIZE��ʣ��ռ䣬�������
				//�Ļ���ֻ����TCP_CLIENT_RX_BUFSIZE��ʣ�೤�ȵ����ݣ�����Ļ��Ϳ������е�����
				if(q->len > (TCPCLIEN.BufferSize-TCPCLIEN.Rx_BufLen)) memcpy(TCPCLIEN.Rx_Buffer+TCPCLIEN.Rx_BufLen,q->payload,(TCPCLIEN.BufferSize-TCPCLIEN.Rx_BufLen));//��������
				else memcpy(TCPCLIEN.Rx_Buffer+TCPCLIEN.Rx_BufLen,q->payload,q->len);
//				printf("len:%d\r\nstr:\r\n%s\r\n",q->len,q->payload);
				TCPCLIEN.Rx_BufLen += q->len;  	
				if(TCPCLIEN.Rx_BufLen > TCPCLIEN.BufferSize) break; //����TCP�ͻ��˽�������,����	
			}
//		
 			tcp_recved(tpcb,p->tot_len);//���ڻ�ȡ��������,֪ͨLWIP���Ի�ȡ��������
			pbuf_free(p);  	//�ͷ��ڴ�
			ret_err=ERR_OK;
		}
	}else  //���յ����ݵ��������Ѿ��ر�,
	{ 
		tcp_recved(tpcb,p->tot_len);//���ڻ�ȡ��������,֪ͨLWIP���Ի�ȡ��������
		es->p=NULL;
		pbuf_free(p); //�ͷ��ڴ�
		ret_err=ERR_OK;
	}
	return ret_err;
}

//lwIP tcp_err�����Ļص�����
void tcp_client_error(void *arg,err_t err)
{  
	//�������ǲ����κδ���
} 


//TCP�����ݺ���
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

//LWIP���ݷ��ͣ��û�Ӧ�ó�����ô˺�������������
//tpcb:TCP���ƿ�
//����ֵ:0���ɹ���������ʧ��
err_t tcp_client_usersent(struct tcp_pcb *tpcb)
{
    err_t ret_err;
	struct tcp_client_struct *es; 
	es=tpcb->callback_arg;
	if(es!=NULL)  //���Ӵ��ڿ��п��Է�������
	{
        es->p=pbuf_alloc(PBUF_TRANSPORT, TCPCLIEN.Tx_BufLen ,PBUF_POOL);	//�����ڴ� 
        pbuf_take(es->p,(char*)TCPCLIEN.Tx_Buffer,TCPCLIEN.Tx_BufLen);	//��tcp_client_sentbuf[]�е����ݿ�����es->p_tx��
        tcp_client_senddata(tpcb,es);//��tcp_client_sentbuf[]���渴�Ƹ�pbuf�����ݷ��ͳ�ȥ
        TCPCLIEN.TcpClient_STA&=~(1<<7);	//������ݷ��ͱ�־
        if(es->p)pbuf_free(es->p);	//�ͷ��ڴ�
		ret_err=ERR_OK;
	}else
	{ 
		tcp_abort(tpcb);//��ֹ����,ɾ��pcb���ƿ�
		ret_err=ERR_ABRT;
	}
	return ret_err;
}
//lwIP tcp_poll�Ļص�����
err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb)
{
	err_t ret_err;
	struct tcp_client_struct *es; 
	es=(struct tcp_client_struct*)arg;
    if(es->state==ES_TCPCLIENT_CLOSING)         //���ӶϿ�
	{ 
 		tcp_client_connection_close(tpcb,es);   //�ر�TCP����
	} 
	ret_err=ERR_OK;
    return ret_err;
} 
//lwIP tcp_sent�Ļص�����(����Զ���������յ�ACK�źź�������)
err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	struct tcp_client_struct *es;
	LWIP_UNUSED_ARG(len);
	es=(struct tcp_client_struct*)arg;
	if(es->p)tcp_client_senddata(tpcb,es);//��������
	return ERR_OK;
}
//�˺���������������
void tcp_client_senddata(struct tcp_pcb *tpcb, struct tcp_client_struct * es)
{
	struct pbuf *ptr; 
 	err_t wr_err=ERR_OK;
	while((wr_err==ERR_OK)&&es->p&&(es->p->len<=tcp_sndbuf(tpcb)))
	{
		ptr=es->p;
		wr_err=tcp_write(tpcb,ptr->payload,ptr->len,1); //��Ҫ���͵����ݼ��뵽���ͻ��������
		if(wr_err==ERR_OK)
		{  
			es->p=ptr->next;			//ָ����һ��pbuf
			if(es->p)pbuf_ref(es->p);	//pbuf��ref��һ
			pbuf_free(ptr);				//�ͷ�ptr 
		}else if(wr_err==ERR_MEM)es->p=ptr;
        tcp_output(tpcb);		        //�����ͻ�������е������������ͳ�ȥ
	} 	
} 
//�ر��������������
void tcp_client_connection_close(struct tcp_pcb *tpcb, struct tcp_client_struct * es)
{
	//�Ƴ��ص�
	tcp_abort(tpcb);//��ֹ����,ɾ��pcb���ƿ�
	tcp_arg(tpcb,NULL);  
	tcp_recv(tpcb,NULL);
	tcp_sent(tpcb,NULL);
	tcp_err(tpcb,NULL);
	tcp_poll(tpcb,NULL,0);  
	if(es)mem_free(es); 
	TCPCLIEN.TcpClient_STA&=~(1<<5);//������ӶϿ���
}


//TCP����ר�ö�ʱ��
void TIM2_INIT(void)
{	
	TIM2_Handler.Instance=TIM2;                          //ͨ�ö�ʱ��4
	TIM2_Handler.Init.Prescaler=Timer2_FrequencyDiv;      //��Ƶϵ��
	TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
	TIM2_Handler.Init.Period=Timer2_AutoLoading;         //�Զ�װ��ֵ
	TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
	HAL_TIM_Base_Init(&TIM2_Handler);
}


//��ʱ��5�жϷ�����

void TIM2_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
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
					TCPCLIEN.TcpClient_STA|=1<<6;		//��ǽ��յ�������
				TCPCLIEN.Rx_Complete = True;
				TCPCLIEN.Rx_Receiving = False;
				TCP_recv_Timeout = 0;
				HAL_TIM_Base_Stop_IT(&TIM2_Handler);
//				printf("UART4 Timeout\r\n");
			}
		}
    
  }
	
	HAL_TIM_IRQHandler(&TIM2_Handler);
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
}


