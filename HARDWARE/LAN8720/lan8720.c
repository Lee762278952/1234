#include "lan8720.h"
#include "lwip_comm.h"
#include "delay.h"
#include "sys_malloc.h"
#include "includes.h"
#include "sysdebug.h"

#if ApolloDev
#include "pcf8574.h"
#endif

ETH_HandleTypeDef ETH_Handler;      //以太网句柄

ETH_DMADescTypeDef *DMARxDscrTab;	//以太网DMA接收描述符数据结构体指针
ETH_DMADescTypeDef *DMATxDscrTab;	//以太网DMA发送描述符数据结构体指针 
uint8_t *Rx_Buff; 					//以太网底层驱动接收buffers指针 
uint8_t *Tx_Buff; 					//以太网底层驱动发送buffers指针
  
//LAN8720初始化
//返回值:0,成功;
//    其他,失败
u8 LAN8720_Init(void)
{   
		GPIO_InitTypeDef GPIO_Initure;	
    u8 macaddress[6];

#if !ApolloDev	
    __HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOA时钟
//	
    GPIO_Initure.Pin=GPIO_PIN_3; //PA3
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_NOPULL;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
#endif
    
//    INTX_DISABLE();  	//关闭所有中断，复位过程不能被打断！
#if ApolloDev
    PCF8574_WriteBit(ETH_RESET_IO,1);       //硬件复位
#else
	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);	//拉低 
#endif
    delay_ms(100);
	
#if ApolloDev	
    PCF8574_WriteBit(ETH_RESET_IO,0);       //复位结束
#else
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);	//拉高
#endif
    delay_ms(20);
 //   INTX_ENABLE();                          //开启所有中断  

    macaddress[0]=LWIP.MAC[0]; 
	macaddress[1]=LWIP.MAC[1]; 
	macaddress[2]=LWIP.MAC[2];
	macaddress[3]=LWIP.MAC[3];   
	macaddress[4]=LWIP.MAC[4];
	macaddress[5]=LWIP.MAC[5];
        
	ETH_Handler.Instance=ETH;
    ETH_Handler.Init.AutoNegotiation=ETH_AUTONEGOTIATION_ENABLE;//使能自协商模式 
    ETH_Handler.Init.Speed=ETH_SPEED_100M;//速度100M,如果开启了自协商模式，此配置就无效
    ETH_Handler.Init.DuplexMode=ETH_MODE_FULLDUPLEX;//全双工模式，如果开启了自协商模式，此配置就无效
    ETH_Handler.Init.PhyAddress=LAN8720_PHY_ADDRESS;//LAN8720地址  
    ETH_Handler.Init.MACAddr=macaddress;            //MAC地址  
    ETH_Handler.Init.RxMode=ETH_RXINTERRUPT_MODE;   //轮训接收模式 
    ETH_Handler.Init.ChecksumMode=ETH_CHECKSUM_BY_HARDWARE;//硬件帧校验  
    ETH_Handler.Init.MediaInterface=ETH_MEDIA_INTERFACE_RMII;//RMII接口  
    if(HAL_ETH_Init(&ETH_Handler)==HAL_OK)
    {
//			debug("PHY init success!PHY Speed :%x Link:%d\r\n",LAN8720_Get_Speed(),LAN8720_LinkCheck());
        return 0;   //成功
    }
    else 
		{
//			debug("PHY init fail!PHY Speed :%x Link:%d\r\n",LAN8720_Get_Speed(),LAN8720_LinkCheck());
			return 1;  //失败
		}
}

//取消Lan8270初始化
void LAN8270_DeInit(void)
{
	HAL_ETH_DeInit(&ETH_Handler);
}

//ETH底层驱动，时钟使能，引脚配置
//此函数会被HAL_ETH_Init()调用
//heth:以太网句柄
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_ETH_CLK_ENABLE();             //开启ETH时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
	__HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
    __HAL_RCC_GPIOG_CLK_ENABLE();			//开启GPIOG时钟
    
    /*网络引脚设置 RMII接口 
    ETH_MDIO -------------------------> PA2
    ETH_MDC --------------------------> PC1
    ETH_RMII_REF_CLK------------------> PA1
    ETH_RMII_CRS_DV ------------------> PA7
    ETH_RMII_RXD0 --------------------> PC4
    ETH_RMII_RXD1 --------------------> PC5
    ETH_RMII_TX_EN -------------------> PB11
    ETH_RMII_TXD0 --------------------> PG13
    ETH_RMII_TXD1 --------------------> PG14
    ETH_RESET-------------------------> PCF8574扩展IO*/
    
    //PA1,2,7
    GPIO_Initure.Pin=GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_7; 
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //推挽复用
    GPIO_Initure.Pull=GPIO_NOPULL;              //不带上下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;         //高速
    GPIO_Initure.Alternate=GPIO_AF11_ETH;       //复用为ETH功能
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);         //初始化
    
    //PB11
    GPIO_Initure.Pin=GPIO_PIN_11;               //PB11
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);         //始化
    
    //PC1,4,5
    GPIO_Initure.Pin=GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5; //PC1,4,5
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);         //初始化
	
    //PG13,14
    GPIO_Initure.Pin=GPIO_PIN_13|GPIO_PIN_14;   //PG13,14
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);         //初始化
    
    HAL_NVIC_SetPriority(ETH_IRQn,0,0);         //网络中断优先级应该高一点
    HAL_NVIC_EnableIRQ(ETH_IRQn);
}
//读取PHY寄存器值
u32 LAN8720_ReadPHY(u16 reg)
{
    u32 regval;
    HAL_ETH_ReadPHYRegister(&ETH_Handler,reg,&regval);
    return regval;
}
//向LAN8720指定寄存器写入值
//reg:要写入的寄存器
//value:要写入的值
void LAN8720_WritePHY(u16 reg,u16 value)
{
    u32 temp=value;
    HAL_ETH_ReadPHYRegister(&ETH_Handler,reg,&temp);
}
//得到8720的速度模式
//返回值:
//001:10M半双工
//101:10M全双工
//010:100M半双工
//110:100M全双工
//其他:错误.
u8 LAN8720_Get_Speed(void)
{
	u8 speed;
	speed=((LAN8720_ReadPHY(31)&0x1C)>>2); //从LAN8720的31号寄存器中读取网络速度和双工模式
	return speed;
}

//获取网线连接状态
//return True = 网线连接  False = 网线断开
_BOOL LAN8720_LinkCheck(void)
{
	u32 phyreg;
	phyreg = LAN8720_ReadPHY(PHY_BSR);
	if((phyreg & PHY_LINKED_STATUS) != PHY_LINKED_STATUS) return False;
	else return True;
}


extern void lwip_pkt_handle(void);		//在lwip_comm.c里面定义
//中断服务函数
void ETH_IRQHandler(void)
{
    OSIntEnter(); 
    while(ETH_GetRxPktSize(ETH_Handler.RxDesc))   
    {
        lwip_pkt_handle();//处理以太网数据，即将数据提交给LWIP
    }
    //清除中断标志位
    __HAL_ETH_DMA_CLEAR_IT(&ETH_Handler,ETH_DMA_IT_R); 
    __HAL_ETH_DMA_CLEAR_IT(&ETH_Handler,ETH_DMA_IT_NIS); 
    OSIntExit();  
}

//获取接收到的帧长度
//DMARxDesc:接收DMA描述符
//返回值:接收到的帧长度
u32  ETH_GetRxPktSize(ETH_DMADescTypeDef *DMARxDesc)
{
    u32 frameLength = 0;
    if(((DMARxDesc->Status&ETH_DMARXDESC_OWN)==(uint32_t)RESET) &&
     ((DMARxDesc->Status&ETH_DMARXDESC_ES)==(uint32_t)RESET) &&
     ((DMARxDesc->Status&ETH_DMARXDESC_LS)!=(uint32_t)RESET)) 
    {
        frameLength=((DMARxDesc->Status&ETH_DMARXDESC_FL)>>ETH_DMARXDESC_FRAME_LENGTHSHIFT);
    }
    return frameLength;
}

//为ETH底层驱动申请内存
//返回值:0,正常
//    其他,失败
u8 ETH_Mem_Malloc(void)
{ 
	ETH_Mem_Free();
//	DMARxDscrTab=mymalloc(SRAMIN,ETH_RXBUFNB*sizeof(ETH_DMADescTypeDef));//申请内存
//	DMATxDscrTab=mymalloc(SRAMIN,ETH_TXBUFNB*sizeof(ETH_DMADescTypeDef));//申请内存  
//	Rx_Buff=mymalloc(SRAMIN,ETH_RX_BUF_SIZE*ETH_RXBUFNB);	//申请内存
//	Tx_Buff=mymalloc(SRAMIN,ETH_TX_BUF_SIZE*ETH_TXBUFNB);	//申请内存
	SYS_MALLOC(DMARxDscrTab,SRAMIN,ETH_RXBUFNB*sizeof(ETH_DMADescTypeDef));
	SYS_MALLOC(DMATxDscrTab,SRAMIN,ETH_TXBUFNB*sizeof(ETH_DMADescTypeDef));
	SYS_MALLOC(Rx_Buff,SRAMIN,ETH_RX_BUF_SIZE*ETH_RXBUFNB);
	SYS_MALLOC(Tx_Buff,SRAMIN,ETH_TX_BUF_SIZE*ETH_TXBUFNB);

	if(!(u32)&DMARxDscrTab||!(u32)&DMATxDscrTab||!(u32)&Rx_Buff||!(u32)&Tx_Buff)
	{
		ETH_Mem_Free();
		return 1;	//申请失败
	}	
	return 0;		//申请成功
}

//释放ETH 底层驱动申请的内存
void ETH_Mem_Free(void)
{ 
//	myfree(SRAMIN,DMARxDscrTab);//释放内存
//	myfree(SRAMIN,DMATxDscrTab);//释放内存
//	myfree(SRAMIN,Rx_Buff);		//释放内存
//	myfree(SRAMIN,Tx_Buff);		//释放内存  
	SYS_FREE(DMARxDscrTab,SRAMIN);
	SYS_FREE(DMATxDscrTab,SRAMIN);
	SYS_FREE(Rx_Buff,SRAMIN);
	SYS_FREE(Tx_Buff,SRAMIN);

}



