#include "usart.h"
#include "delay.h"
#include "sys_malloc.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用os,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os 使用	  
#endif

//V1.0修改说明 
////////////////////////////////////////////////////////////////////////////////// 	  
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 

//#if EN_USART1_RX   //如果使能了接收

#define 	Timer4_FrequencyDiv		(9000-1)
#define   Timer4_AutoLoading		(100-1)				//Timer4自动装载置  （10ms）

#define USART1_REC_LEN  			200  	//定义最大接收字节数 200


static u8 UART1_Timeout;//串口接收超时计时器

UART_HandleTypeDef UART1_Handler; //UART句柄
TIM_HandleTypeDef TIM4_Handler;      //定时器句柄 

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

//初始化IO 串口1 
//bound:波特率
static void uart_init(u32 bound)
{	
	//UART 初始化设置
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //波特率
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()会使能UART1
	
//	UART1.Rx_Buffer  = mymalloc(SRAMIN,USART1_REC_LEN);
	SYS_MALLOC(UART1.Rx_Buffer,SRAMIN,USART1_REC_LEN);

		  //串口1接收专用定时器
	TIM4_Handler.Instance=TIM4;                          //通用定时器3
	TIM4_Handler.Init.Prescaler=Timer4_FrequencyDiv;      //分频系数
	TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
	TIM4_Handler.Init.Period=Timer4_AutoLoading;         //自动装载值
	TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
	HAL_TIM_Base_Init(&TIM4_Handler);
	

	
	HAL_TIM_Base_Start_IT(&TIM4_Handler);

//	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, 1);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量

}

//UART底层初始化，时钟使能，引脚配置，中断配置
//此函数会被HAL_UART_Init()调用
//huart:串口句柄

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
		__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
	
		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//高速
		GPIO_Initure.Alternate=GPIO_AF7_USART1;	//复用为USART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA10
		__HAL_UART_DISABLE_IT(huart,UART_IT_TC);
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//开启接收中断
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//使能USART1中断通道
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//抢占优先级3，子优先级3

	}
	else if(huart->Instance==UART4)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();			//使能GPIOC时钟
		__HAL_RCC_UART4_CLK_ENABLE();			//使能UART4时钟
		
		GPIO_Initure.Pin=GPIO_PIN_10;			//PC10
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//高速
		GPIO_Initure.Alternate=GPIO_AF8_UART4;	//复用为UART4
		HAL_GPIO_Init(GPIOC,&GPIO_Initure);	   	

		GPIO_Initure.Pin=GPIO_PIN_11;			//PC11
		HAL_GPIO_Init(GPIOC,&GPIO_Initure);	   	
		__HAL_UART_DISABLE_IT(huart,UART_IT_TC);
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//开启接收中断
		HAL_NVIC_EnableIRQ(UART4_IRQn);				//使能UART4中断通道
		HAL_NVIC_SetPriority(UART4_IRQn,1,1);			//抢占优先级3，子优先级3
	}
}

//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  if(htim->Instance==TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();            //使能TIM4时钟
		HAL_NVIC_SetPriority(TIM4_IRQn,3,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM4_IRQn);          //开启ITM3中断   
	}
	else if(htim->Instance==TIM5)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();            //使能TIM5时钟
		HAL_NVIC_SetPriority(TIM5_IRQn,3,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM5_IRQn);          //开启ITM3中断   
	}
	else if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM5时钟
		HAL_NVIC_SetPriority(TIM2_IRQn,3,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM3中断   
	}
}




//串口1中断服务程序
void USART1_IRQHandler(void)                	
{ 
	u8 Res;
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif
		if(!UART1.Rx_Receiving)  //串口接收到信息打开定时器
		{
			UART1.Rx_Receiving = True;
			UART1.Rx_Complete = False;
			UART1.Rx_BufLen = 0;
			memset(UART1.Rx_Buffer,'\0',USART1_REC_LEN);
			HAL_TIM_Base_Start_IT(&TIM4_Handler);
		}
		
		if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
      HAL_UART_Receive(&UART1_Handler,&Res,1,1000); 

			UART1.Rx_Buffer[UART1.Rx_BufLen] = Res;
			UART1.Rx_BufLen++;

			UART1_Timeout = 0;
		}
		HAL_UART_IRQHandler(&UART1_Handler);	
	
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
} 


static void USART1_SendBuffer(u8 *buf,u32 len)
{
	u32 i;
	
	for(i=0;i<len;i++)
	{
		HAL_UART_Transmit(&UART1_Handler,&buf[i],1,1000);	//发送接收到的数据
		while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);	
	}		
}



//定时器4中断服务函数

void TIM4_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS	 	//使用OS
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
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
}




