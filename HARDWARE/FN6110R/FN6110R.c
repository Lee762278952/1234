#include "FN6110R.h"
#include "includes.h"
#include "sys_malloc.h"
#include "wifi.h"





static u8 UART4_Timeout;//串口接收超时计时器

UART_HandleTypeDef UART4_Handler; //UART句柄
TIM_HandleTypeDef TIM5_Handler;      //定时器句柄 

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
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;		//复用推挽输出
	GPIO_Initure.Pull=GPIO_PULLDOWN;			//上拉
	GPIO_Initure.Speed=GPIO_SPEED_FAST;		//高速
	HAL_GPIO_Init(GPIOC,&GPIO_Initure);	   	//初始化PC12
	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET);

}




//初始化IO 串口1 
//bound:波特率
static void uart4_init(u32 bound)
{	
//	GPIO_InitTypeDef GPIO_Initure;
	
	//UART 初始化设置
	UART4_Handler.Instance=UART4;					    //UART4
	UART4_Handler.Init.BaudRate=bound;				    //波特率
	UART4_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART4_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART4_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART4_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART4_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART4_Handler);					    //HAL_UART_Init()会使能UART4
	

	
//	U4.Rx_Buffer  = mymalloc(SRAMIN,UART4_REC_LEN);
//	U4.Rx_Buffer = mymalloc(SRAMEX,UART4_REC_LEN);
//	memset(U4.Rx_Buffer,'\0',UART4_REC_LEN);
	U4.Max_RxBufLen = UART4_REC_LEN;
	SYS_MALLOC(U4.Rx_Buffer,SRAMEX,U4.Max_RxBufLen);
	
		  //串口4接收专用定时器
	TIM5_Handler.Instance=TIM5;                          //通用定时器5
	TIM5_Handler.Init.Prescaler=Timer5_FrequencyDiv;      //分频系数
	TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
	TIM5_Handler.Init.Period=Timer5_AutoLoading;         //自动装载值
	TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
	HAL_TIM_Base_Init(&TIM5_Handler);
	

	
//	HAL_TIM_Base_Start_IT(&TIM5_Handler);

//	HAL_UART_Receive_IT(&UART4_Handler, (u8 *)aRxBuffer, 1);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量

}



//串口4中断服务程序
void UART4_IRQHandler(void)                	
{ 
	CPU_SR_ALLOC();
	u8 Res;
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif
		OS_CRITICAL_ENTER();	//进入临界区
		if(U4.Rx_Receiving == False)  //串口接收到信息打开定时器
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
		if(UART4->SR & USART_SR_RXNE)//接收到数据
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
	
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
} 


static void UART4_SendBuffer(u8 *buf,u32 len)
{
	u16 i;
	
	for(i=0;i<len;i++)
	{
		HAL_UART_Transmit(&UART4_Handler,&buf[i],1,1000);	//发送接收到的数据
		while(__HAL_UART_GET_FLAG(&UART4_Handler,UART_FLAG_TC)!=SET);	
	}		
}



//定时器5中断服务函数

void TIM5_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS	 	//使用OS
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
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
}
