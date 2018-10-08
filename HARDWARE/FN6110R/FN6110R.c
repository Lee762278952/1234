#include "FN6110R.h"
#include "includes.h"
#include "sys_malloc.h"
#include "wifi.h"





static u8 UART4_Timeout;//���ڽ��ճ�ʱ��ʱ��

UART_HandleTypeDef UART4_Handler; //UART���
TIM_HandleTypeDef TIM5_Handler;      //��ʱ����� 

static void uart4_init(u32 bound);
static void UART4_SendBuffer(u8 *buf,u32 len);

_uart_class U4 =
{
	.Rx_Complete = False,
	.Rx_Receiving = False,	
	.Rx_BufLen = 0,
	.BaudRate = FN6110R_BaudRate,
	
	.INIT = uart4_init,
	.SendBuffer = UART4_SendBuffer,
};

void FN6110R_INIT(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	U4.INIT(U4.BaudRate);

	GPIO_Initure.Pin=GPIO_PIN_12;			//PC12
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;		//�����������
	GPIO_Initure.Pull=GPIO_PULLDOWN;			//����
	GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
	HAL_GPIO_Init(GPIOC,&GPIO_Initure);	   	//��ʼ��PC12
	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET);

}




//��ʼ��IO ����1 
//bound:������
static void uart4_init(u32 bound)
{	
//	GPIO_InitTypeDef GPIO_Initure;
	
	//UART ��ʼ������
	UART4_Handler.Instance=UART4;					    //UART4
	UART4_Handler.Init.BaudRate=bound;				    //������
	UART4_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART4_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART4_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART4_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART4_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART4_Handler);					    //HAL_UART_Init()��ʹ��UART4
	

	
//	U4.Rx_Buffer  = mymalloc(SRAMIN,UART4_REC_LEN);
//	U4.Rx_Buffer = mymalloc(SRAMEX,UART4_REC_LEN);
//	memset(U4.Rx_Buffer,'\0',UART4_REC_LEN);
	U4.Max_RxBufLen = UART4_REC_LEN;
	SYS_MALLOC(U4.Rx_Buffer,SRAMEX,U4.Max_RxBufLen);
	
		  //����4����ר�ö�ʱ��
	TIM5_Handler.Instance=TIM5;                          //ͨ�ö�ʱ��5
	TIM5_Handler.Init.Prescaler=Timer5_FrequencyDiv;      //��Ƶϵ��
	TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
	TIM5_Handler.Init.Period=Timer5_AutoLoading;         //�Զ�װ��ֵ
	TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
	HAL_TIM_Base_Init(&TIM5_Handler);
	

	
//	HAL_TIM_Base_Start_IT(&TIM5_Handler);

//	HAL_UART_Receive_IT(&UART4_Handler, (u8 *)aRxBuffer, 1);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������

}



//����4�жϷ������
void UART4_IRQHandler(void)                	
{ 
	CPU_SR_ALLOC();
	u8 Res;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
		OS_CRITICAL_ENTER();	//�����ٽ���
		if(U4.Rx_Receiving == False)  //���ڽ��յ���Ϣ�򿪶�ʱ��
		{
			U4.Rx_Receiving = True;
			U4.Rx_Complete = False;
			U4.Rx_BufLen = 0;
			HAL_TIM_Base_Start_IT(&TIM5_Handler);
//			printf("U4 get\r\n");
		}
		
		
		
//		if((__HAL_UART_GET_FLAG(&UART4_Handler,UART_FLAG_ORE)!=RESET))
		if(UART4->SR & UART_FLAG_ORE)
		{
			debug("Err:UART_FLAG_ORE != RESET\r\n");
			Res=UART4->DR;
		}
		
//		if((__HAL_UART_GET_FLAG(&UART4_Handler,UART_FLAG_RXNE)!=RESET))  
		if(UART4->SR & USART_SR_RXNE)//���յ�����
		{
//      HAL_UART_Receive(&UART4_Handler,&Res,1,1000); 
			Res=UART4->DR;
			
			if(U4.Rx_BufLen < U4.Max_RxBufLen)
			{
				U4.Rx_Buffer[U4.Rx_BufLen] = Res;
				U4.Rx_BufLen++;
				UART4_Timeout = 0;
			}
			else 
			{
				debug("Err:UART4 receiving overflow\r\n");
				__HAL_UART_CLEAR_FLAG(&UART4_Handler,UART_FLAG_ORE);
				WIFI.Disable_Set();
//				HAL_NVIC_SystemReset();
//				return;
			}
		}
//		HAL_UART_IRQHandler(&UART4_Handler);	
		OS_CRITICAL_EXIT();	
	
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
} 


static void UART4_SendBuffer(u8 *buf,u32 len)
{
	u16 i;
	
	for(i=0;i<len;i++)
	{
		HAL_UART_Transmit(&UART4_Handler,&buf[i],1,1000);	//���ͽ��յ�������
		while(__HAL_UART_GET_FLAG(&UART4_Handler,UART_FLAG_TC)!=SET);	
	}		
}



//��ʱ��5�жϷ�����

void TIM5_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	if(__HAL_TIM_GET_FLAG(&TIM5_Handler, TIM_FLAG_UPDATE) != RESET)
  {
		if(__HAL_TIM_GET_IT_SOURCE(&TIM5_Handler, TIM_IT_UPDATE) !=RESET)
    {
			__HAL_TIM_CLEAR_IT(&TIM5_Handler, TIM_IT_UPDATE);
			UART4_Timeout++;
			if(UART4_Timeout >= 10)
			{
				U4.Rx_Complete = True;
				U4.Rx_Receiving = False;
				UART4_Timeout = 0;
				HAL_TIM_Base_Stop_IT(&TIM5_Handler);
//				printf("UART4 Timeout\r\n");
			}
		}
    
  }
	
	HAL_TIM_IRQHandler(&TIM5_Handler);
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
}
