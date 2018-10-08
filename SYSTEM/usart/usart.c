#include "usart.h"
#include "delay.h"
#include "sys_malloc.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��os,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os ʹ��	  
#endif

//V1.0�޸�˵�� 
////////////////////////////////////////////////////////////////////////////////// 	  
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 

//#if EN_USART1_RX   //���ʹ���˽���

#define 	Timer4_FrequencyDiv		(9000-1)
#define   Timer4_AutoLoading		(100-1)				//Timer4�Զ�װ����  ��10ms��

#define USART1_REC_LEN  			200  	//�����������ֽ��� 200


static u8 UART1_Timeout;//���ڽ��ճ�ʱ��ʱ��

UART_HandleTypeDef UART1_Handler; //UART���
TIM_HandleTypeDef TIM4_Handler;      //��ʱ����� 

static void uart_init(u32 bound);
static void USART1_SendBuffer(u8 *buf,u32 len);

_uart_class UART1 =
{
	.Rx_Complete = False,
	.Rx_Receiving = False,	
	.Rx_BufLen = 0,
	.BaudRate = 115200,
	
	.INIT = uart_init,
	.SendBuffer = USART1_SendBuffer,
};

//��ʼ��IO ����1 
//bound:������
static void uart_init(u32 bound)
{	
	//UART ��ʼ������
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //������
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()��ʹ��UART1
	
//	UART1.Rx_Buffer  = mymalloc(SRAMIN,USART1_REC_LEN);
	SYS_MALLOC(UART1.Rx_Buffer,SRAMIN,USART1_REC_LEN);

		  //����1����ר�ö�ʱ��
	TIM4_Handler.Instance=TIM4;                          //ͨ�ö�ʱ��3
	TIM4_Handler.Init.Prescaler=Timer4_FrequencyDiv;      //��Ƶϵ��
	TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
	TIM4_Handler.Init.Period=Timer4_AutoLoading;         //�Զ�װ��ֵ
	TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
	HAL_TIM_Base_Init(&TIM4_Handler);
	

	
	HAL_TIM_Base_Start_IT(&TIM4_Handler);

//	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, 1);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������

}

//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
//�˺����ᱻHAL_UART_Init()����
//huart:���ھ��

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART1ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate=GPIO_AF7_USART1;	//����ΪUSART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA10
		__HAL_UART_DISABLE_IT(huart,UART_IT_TC);
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//���������ж�
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//��ռ���ȼ�3�������ȼ�3

	}
	else if(huart->Instance==UART4)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();			//ʹ��GPIOCʱ��
		__HAL_RCC_UART4_CLK_ENABLE();			//ʹ��UART4ʱ��
		
		GPIO_Initure.Pin=GPIO_PIN_10;			//PC10
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate=GPIO_AF8_UART4;	//����ΪUART4
		HAL_GPIO_Init(GPIOC,&GPIO_Initure);	   	

		GPIO_Initure.Pin=GPIO_PIN_11;			//PC11
		HAL_GPIO_Init(GPIOC,&GPIO_Initure);	   	
		__HAL_UART_DISABLE_IT(huart,UART_IT_TC);
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//���������ж�
		HAL_NVIC_EnableIRQ(UART4_IRQn);				//ʹ��UART4�ж�ͨ��
		HAL_NVIC_SetPriority(UART4_IRQn,1,1);			//��ռ���ȼ�3�������ȼ�3
	}
}

//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  if(htim->Instance==TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();            //ʹ��TIM4ʱ��
		HAL_NVIC_SetPriority(TIM4_IRQn,3,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM4_IRQn);          //����ITM3�ж�   
	}
	else if(htim->Instance==TIM5)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();            //ʹ��TIM5ʱ��
		HAL_NVIC_SetPriority(TIM5_IRQn,3,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM5_IRQn);          //����ITM3�ж�   
	}
	else if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();            //ʹ��TIM5ʱ��
		HAL_NVIC_SetPriority(TIM2_IRQn,3,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //����ITM3�ж�   
	}
}




//����1�жϷ������
void USART1_IRQHandler(void)                	
{ 
	u8 Res;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
		if(!UART1.Rx_Receiving)  //���ڽ��յ���Ϣ�򿪶�ʱ��
		{
			UART1.Rx_Receiving = True;
			UART1.Rx_Complete = False;
			UART1.Rx_BufLen = 0;
			memset(UART1.Rx_Buffer,'\0',USART1_REC_LEN);
			HAL_TIM_Base_Start_IT(&TIM4_Handler);
		}
		
		if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
      HAL_UART_Receive(&UART1_Handler,&Res,1,1000); 

			UART1.Rx_Buffer[UART1.Rx_BufLen] = Res;
			UART1.Rx_BufLen++;

			UART1_Timeout = 0;
		}
		HAL_UART_IRQHandler(&UART1_Handler);	
	
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
} 


static void USART1_SendBuffer(u8 *buf,u32 len)
{
	u32 i;
	
	for(i=0;i<len;i++)
	{
		HAL_UART_Transmit(&UART1_Handler,&buf[i],1,1000);	//���ͽ��յ�������
		while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
	}		
}



//��ʱ��4�жϷ�����

void TIM4_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	if(__HAL_TIM_GET_FLAG(&TIM4_Handler, TIM_FLAG_UPDATE) != RESET)
  {
		if(__HAL_TIM_GET_IT_SOURCE(&TIM4_Handler, TIM_IT_UPDATE) !=RESET)
    {
			__HAL_TIM_CLEAR_IT(&TIM4_Handler, TIM_IT_UPDATE);
			UART1_Timeout++;
			if(UART1_Timeout >= 10)
			{
				UART1.Rx_Complete = True;
				UART1.Rx_Receiving = False;
				UART1_Timeout = 0;
				HAL_TIM_Base_Stop_IT(&TIM4_Handler);
	//			printf("Uart1 Timeout\r\n");
			}
		}
    
  }
	
	HAL_TIM_IRQHandler(&TIM4_Handler);
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
}




