#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"	 
#include "delay.h"	 
#include "ltdc.h"
#include "piclib.h"
#include <string.h>
//#include "font.h" 


#define  TIM3_psc		90-1
#define  TIM3_arr		500-1


//*****************************************/
//LCD驱动代码
//LCD1:STM32F429硬件RGB驱动(LTCD) 
//LCD2：FMC驱动SSD1963
//*****************************************/


void LCD_Init(void);													   	//初始化
void LCD_SelectScreen(_lcd_select sel);

void lcd1_DisplayOn(void);													//开显示
void lcd1_DisplayOff(void);													//关显示
void lcd1_Clear(u32 Color);	 												//清屏
void lcd1_DrawPoint(u16 x,u16 y);											//画点
u32  lcd1_ReadPoint(u16 x,u16 y); 											//读点 
u32 lcd2_FastReadPoint(u16 x,u16 y);
void lcd1_Fast_DrawPoint(u16 x,u16 y,u32 color);								//快速画点
void lcd1_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);				//填充指定颜色
void lcd1_Display_Dir(u8 dir);								//设置屏幕显示方向
void lcd1_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color);		   				//填充单色
void lcd1_BackLightSet(u8 pwm);

void lcd2_DisplayOn(void);
void lcd2_DisplayOff(void);
void lcd2_Clear(u32 color);
void lcd2_DrawPoint(u16 x,u16 y);
u32  lcd2_ReadPoint(u16 x,u16 y);
void lcd2_Fast_DrawPoint(u16 x,u16 y,u32 color);
void lcd2_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);
void lcd2_Display_Dir(u8 dir);								//设置屏幕显示方向
void lcd2_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color);		   				//填充单色
void lcd2_BackLightSet(u8 pwm);							//SSD1963 背光控制

void lcd2_WriteReg(u16 lcd_Reg, u16 lcd_RegValue);
u16  lcd2_ReadReg(u16 lcd_Reg);
void lcd2_WriteRAM_Prepare(void);
void lcd2_WriteRAM(u16 RGB_Code);
void lcd2_Scan_Dir(u8 dir);									//设置屏扫描方向
void lcd2_Set_Window(u16 sx,u16 sy,u16 width,u16 height);	//设置窗口	
void lcd2_SetCursor(u16 Xpos, u16 Ypos);										//lcd2设置光标


void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);						 			//画圆
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);							//画线
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);	   				//画矩形

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);						//显示一个字符
void LCD_ShowCharEX(u16 x,u16 y,u8 num,u8 size,u8 mode);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						//显示一个数字
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//显示 数字
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);		//显示一个字符串,12/16字体
void LCD_Darker(u16 sx,u16 sy,u16 ex,u16 ey,u8 coefficient);
//void LCD_MicroIcon(u16 sx,u16 sy,_microicon icon);


//u16 lcd2_framebuf[800][480] __attribute__((at(LCD2_FRAME_BUF_ADDR)));	//定义屏幕内部缓存


SRAM_HandleTypeDef SRAM_Handler;    //SRAM句柄(用于控制LCD)
//LCD的画笔颜色和背景色	   
vu32 POINT_COLOR=0xFF000000;		//画笔颜色
u32 BACK_COLOR =0xFFFFFFFF;  	//背景色 

//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev,lcd2dev;
//LCD功能对象
_lcd_class LCD = {
	.LCD_Sel	= LCD_1,
	.width 		= 1024,		//LCD 宽度
	.height 	= 600,			//LCD 高度
	.id 			= 0	,			//LCD ID
	.dir 			= 0	,	//横屏还是竖屏控制：0，竖屏；1，横屏。	
	.wramcmd 	= 0	,//开始写gram指令
	.setxcmd 	= 0	,//设置x坐标指令
	.setycmd 	= 0	,//设置y坐标指令 
	
	.initialization = LCD_Init,
	.SelectScreen 	= LCD_SelectScreen,
	
	.DisplayOn 			= lcd1_DisplayOn,										
	.DisplayOff			= lcd1_DisplayOff,												
	.Clear					= lcd1_Clear,									
	.DrawPoint 			= lcd1_DrawPoint,									
	.ReadPoint 			= lcd1_ReadPoint,
//	.FastReadPoint  = lcd1_ReadPoint,
	.Fast_DrawPoint = lcd1_Fast_DrawPoint,							
	.Color_Fill 		= lcd1_Color_Fill,
	.Display_Dir 		= lcd1_Display_Dir,
	.Fill 					= lcd1_Fill,
	.BackLightSet 	= lcd1_BackLightSet,	
	
	.WriteReg 				= lcd2_WriteReg,
	.ReadReg 					= lcd2_ReadReg,
	.WriteRAM_Prepare = lcd2_WriteRAM_Prepare,
	.WriteRAM 				= lcd2_WriteRAM,				
	.Scan_Dir 				= lcd2_Scan_Dir,						
	.Set_Window 			= lcd2_Set_Window,
	.SetCursor 				= lcd2_SetCursor,
	
	.Draw_Circle 	 = 	LCD_Draw_Circle,			 		
	.DrawLine			 = 	LCD_DrawLine,				
	.DrawRectangle =  LCD_DrawRectangle, 				

	.ShowChar 	= LCD_ShowChar,		
  .ShowCharEX = LCD_ShowCharEX,
	.ShowNum 		= LCD_ShowNum,				
	.ShowxNum 	= LCD_ShowxNum,		
	.ShowString = LCD_ShowString,
	.Darker     = LCD_Darker,
//	.MicroIcon =  LCD_MicroIcon,
};
	 
	 
//LCD2写寄存器函数
//regval:寄存器值
void lcd2_WR_REG(vu16 regval)
{   
	regval=regval;		//使用-O2优化的时候,必须插入的延时
	LCD2->LCD2_REG=regval;//写入要写的寄存器序号	 
}
//写LCD2数据
//data:要写入的值
void lcd2_WR_DATA(vu16 data)
{	  
	data=data;			//使用-O2优化的时候,必须插入的延时
	LCD2->LCD2_RAM=data;		 
}
//读LCD2数据
//返回值:读到的值
u16 lcd2_RD_DATA(void)
{
	vu16 ram;			//防止被优化
	ram=LCD2->LCD2_RAM;	
	return ram;	 
}					   
//LCD2写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
void lcd2_WriteReg(u16 LCD2_Reg,u16 LCD2_RegValue)
{	
	LCD2->LCD2_REG = LCD2_Reg;		//写入要写的寄存器序号	 
	LCD2->LCD2_RAM = LCD2_RegValue;//写入数据	    		 
}	   
//读寄存器
//LCD_Reg:寄存器地址
//返回值:读到的数据
u16 lcd2_ReadReg(u16 LCD2_Reg)
{										   
	lcd2_WR_REG(LCD2_Reg);		//写入要读的寄存器序号
	delay_us(5);		  
	return lcd2_RD_DATA();		//返回读到的值
}   
//开始写GRAM
void lcd2_WriteRAM_Prepare(void)
{
 	LCD2->LCD2_REG=lcd2dev.wramcmd;	  
}	 
//LCD写GRAM
//RGB_Code:颜色值
void lcd2_WriteRAM(u16 RGB_Code)
{							    
	LCD2->LCD2_RAM = RGB_Code;//写十六位GRAM
}
//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
u16 LCD_BGR2RGB(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
} 
//当mdk -O1时间优化时需要设置
//延时i
void opt_delay(u8 i)
{
	while(i--);
}

void LCD_SelectScreen(_lcd_select sel)
{
	if(!sel)
	{
		LCD.LCD_Sel = LCD_1;
		LCD.width 	= lcddev.width;		
		LCD.height 	= lcddev.height;			
		LCD.id 			= lcddev.id;			
		LCD.dir 		= lcddev.dir;	
		
		LCD.DisplayOn 			= lcd1_DisplayOn;										
		LCD.DisplayOff			= lcd1_DisplayOff;												
		LCD.Clear						= lcd1_Clear;									
		LCD.DrawPoint 			= lcd1_DrawPoint;									
		LCD.ReadPoint 			= lcd1_ReadPoint;
//		LCD.FastReadPoint   = lcd1_ReadPoint;
		LCD.Fast_DrawPoint 	= lcd1_Fast_DrawPoint;							
		LCD.Color_Fill 			= lcd1_Color_Fill;
		LCD.Display_Dir 		= lcd1_Display_Dir;
		LCD.Fill 						= lcd1_Fill;
		LCD.BackLightSet 	  = lcd1_BackLightSet;
	}
	else
	{
		LCD.LCD_Sel = LCD_2;
		LCD.width 	= lcd2dev.width;		
		LCD.height 	= lcd2dev.height;			
		LCD.id 			= lcd2dev.id;			
		LCD.dir 		= lcd2dev.dir;	
		LCD.wramcmd = lcd2dev.wramcmd;
		LCD.setxcmd = lcd2dev.setxcmd;
		LCD.setycmd = lcd2dev.setycmd;
		
		LCD.DisplayOn 			= lcd2_DisplayOn;										
		LCD.DisplayOff			= lcd2_DisplayOff;												
		LCD.Clear						= lcd2_Clear;									
		LCD.DrawPoint 			= lcd2_DrawPoint;									
		LCD.ReadPoint 			= lcd2_ReadPoint;
//		LCD.FastReadPoint   = lcd2_FastReadPoint;
		LCD.Fast_DrawPoint 	= lcd2_Fast_DrawPoint;							
		LCD.Color_Fill 			= lcd2_Color_Fill;
		LCD.Display_Dir 		= lcd2_Display_Dir;
		LCD.Fill 						= lcd2_Fill;
		LCD.BackLightSet 	  = lcd2_BackLightSet;
	}
	piclib_init();
}

/*************************************************************/
//读取个某点的颜色值	 
//x,y:坐标
//返回值:此点的颜色
/***********************************************************/
//LCD取点
u32 lcd1_ReadPoint(u16 x,u16 y)
{
	if(x>=lcddev.width||y>=lcddev.height)return 0;	//超过了范围,直接返回
	if(lcdltdc.pwidth!=0)							//如果是RGB屏
	{
		return LTDC_Read_Point(x,y);
	}
	else return 0;
}	

//LCD2取点
u32 lcd2_ReadPoint(u16 x,u16 y)
{
 	u16 r=0;
//		u16 g=0,b=0;
	if(x>=lcd2dev.width||y>=lcd2dev.height)return 0;	//超过了范围,直接返回
	lcd2_SetCursor(x,y);	    
	/*if(lcd2dev.id==0X9341||lcd2dev.id==0X5310||lcd2dev.id==0X1963)*/
	lcd2_WR_REG(0X2E);//9341/3510/1963 发送读GRAM指令
//	else if(lcd2dev.id==0X5510)lcd2_WR_REG(0X2E00);	//5510 发送读GRAM指令
 	r=lcd2_RD_DATA();								//dummy Read	   
//	if(lcd2dev.id==0X1963)
	return r;					//1963直接读就可以 
//	opt_delay(2);	  
// 	r=lcd2_RD_DATA();  		  						//实际坐标颜色
//	//9341/NT35310/NT35510要分2次读出 
//	opt_delay(2);	  
//	b=lcd2_RD_DATA(); 
//	g=r&0XFF;		//对于9341/5310/5510,第一次读取的是RG的值,R在前,G在后,各占8位
//	g<<=8; 
//	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));	//ILI9341/NT35310/NT35510需要公式转换一下
}	

////LCD2快速取点
//u32 lcd2_FastReadPoint(u16 x,u16 y)
//{
//	if(x>=lcd2dev.width||y>=lcd2dev.height)return 0;	//超过了范围,直接返回
//	return lcd2_framebuf[x][y];
//}	
/*************************************************************/

/*************************************************************/
//LCD开启显示
void lcd1_DisplayOn(void)
{					   
	LTDC_Switch(1);//开启LCD

}	 

//LCD2开启显示
void lcd2_DisplayOn(void)
{					   
	if(lcd2dev.id==0X9341||lcd2dev.id==0X5310||lcd2dev.id==0X1963)lcd2_WR_REG(0X29);	//开启显示
	else if(lcd2dev.id==0X5510)lcd2_WR_REG(0X2900);	//开启显示
}	 
/************************************************************/


/*************************************************************/
//LCD关闭显示
void lcd1_DisplayOff(void)
{	   
	LTDC_Switch(0);//关闭LCD
} 

//LCD2关闭显示
void lcd2_DisplayOff(void)
{	   
	if(lcd2dev.id==0X9341||lcd2dev.id==0X5310||lcd2dev.id==0X1963)lcd2_WR_REG(0X28);	//关闭显示
	else if(lcd2dev.id==0X5510)lcd2_WR_REG(0X2800);	//关闭显示
} 
/**************************************************************/


/***************************************************************/
//LCD2设置光标位置(对RGB屏无效)
//Xpos:横坐标
//Ypos:纵坐标
void lcd2_SetCursor(u16 Xpos, u16 Ypos)
{	  			 		
	if(lcd2dev.dir==0)//x坐标需要变换
	{
		Xpos=lcd2dev.width-1-Xpos;
		lcd2_WR_REG(lcd2dev.setxcmd); 
		lcd2_WR_DATA(0);lcd2_WR_DATA(0); 		
		lcd2_WR_DATA(Xpos>>8);lcd2_WR_DATA(Xpos&0XFF);		 	 
	}else
	{
		lcd2_WR_REG(lcd2dev.setxcmd); 
		lcd2_WR_DATA(Xpos>>8);lcd2_WR_DATA(Xpos&0XFF); 		
		lcd2_WR_DATA((lcd2dev.width-1)>>8);lcd2_WR_DATA((lcd2dev.width-1)&0XFF);		 	 			
	}	
	lcd2_WR_REG(lcd2dev.setycmd); 
	lcd2_WR_DATA(Ypos>>8);lcd2_WR_DATA(Ypos&0XFF); 		
	lcd2_WR_DATA((lcd2dev.height-1)>>8);lcd2_WR_DATA((lcd2dev.height-1)&0XFF); 			 		
		
	
}
/*******************************************************************/

/********************************************************************/
//设置LCD的自动扫描方向(对RGB屏无效)
//注意:其他函数可能会受到此函数设置的影响(尤其是9341),
//所以,一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.
//dir:0~7,代表8个方向(具体定义见lcd.h)
//9341/5310/5510/1963等IC已经实际测试	   	   
void lcd2_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u16 dirreg=0;
	u16 temp;  
	if((lcd2dev.dir==1&&lcd2dev.id!=0X1963)||(lcd2dev.dir==0&&lcd2dev.id==0X1963))//横屏时，对1963不改变扫描方向！竖屏时1963改变方向
	{			   
		switch(dir)//方向转换
		{
			case 0:dir=6;break;
			case 1:dir=7;break;
			case 2:dir=4;break;
			case 3:dir=5;break;
			case 4:dir=1;break;
			case 5:dir=0;break;
			case 6:dir=3;break;
			case 7:dir=2;break;	     
		}
	} 
	if(lcd2dev.id==0x9341||lcd2dev.id==0X5310||lcd2dev.id==0X5510||lcd2dev.id==0X1963)//9341/5310/5510/1963,特殊处理
	{
		switch(dir)
		{
			case L2R_U2D://从左到右,从上到下
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://从左到右,从下到上
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://从右到左,从上到下
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://从右到左,从下到上
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://从上到下,从左到右
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://从上到下,从右到左
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://从下到上,从左到右
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://从下到上,从右到左
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		if(lcd2dev.id==0X5510)dirreg=0X3600;
		else dirreg=0X36;
 		if((lcd2dev.id!=0X5310)&&(lcd2dev.id!=0X5510)&&(lcd2dev.id!=0X1963))regval|=0X08;//5310/5510/1963不需要BGR   
 		lcd2_WriteReg(dirreg,regval);
		if(lcd2dev.id!=0X1963)//1963不做坐标处理
		{
			if(regval&0X20)
			{
				if(lcd2dev.width<lcd2dev.height)//交换X,Y
				{
					temp=lcd2dev.width;
					lcd2dev.width=lcd2dev.height;
					lcd2dev.height=temp;
				}
			}else  
			{
				if(lcd2dev.width>lcd2dev.height)//交换X,Y
				{
					temp=lcd2dev.width;
					lcd2dev.width=lcd2dev.height;
					lcd2dev.height=temp;
				}
			}  
		}
		if(lcd2dev.id==0X5510)
		{
			lcd2_WR_REG(lcd2dev.setxcmd);lcd2_WR_DATA(0); 
			lcd2_WR_REG(lcd2dev.setxcmd+1);lcd2_WR_DATA(0); 
			lcd2_WR_REG(lcd2dev.setxcmd+2);lcd2_WR_DATA((lcd2dev.width-1)>>8); 
			lcd2_WR_REG(lcd2dev.setxcmd+3);lcd2_WR_DATA((lcd2dev.width-1)&0XFF); 
			lcd2_WR_REG(lcd2dev.setycmd);lcd2_WR_DATA(0); 
			lcd2_WR_REG(lcd2dev.setycmd+1);lcd2_WR_DATA(0); 
			lcd2_WR_REG(lcd2dev.setycmd+2);lcd2_WR_DATA((lcd2dev.height-1)>>8); 
			lcd2_WR_REG(lcd2dev.setycmd+3);lcd2_WR_DATA((lcd2dev.height-1)&0XFF);
		}else
		{
			lcd2_WR_REG(lcd2dev.setxcmd); 
			lcd2_WR_DATA(0);lcd2_WR_DATA(0);
			lcd2_WR_DATA((lcd2dev.width-1)>>8);lcd2_WR_DATA((lcd2dev.width-1)&0XFF);
			lcd2_WR_REG(lcd2dev.setycmd); 
			lcd2_WR_DATA(0);lcd2_WR_DATA(0);
			lcd2_WR_DATA((lcd2dev.height-1)>>8);lcd2_WR_DATA((lcd2dev.height-1)&0XFF);  
		}
  	} 
}    
/********************************************************************/


/*********************************************************************/
//LCD画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void lcd1_DrawPoint(u16 x,u16 y)
{
	if(lcdltdc.pwidth!=0)//如果是RGB屏
	{
		LTDC_Draw_Point(x,y,POINT_COLOR);
	}
}

//LCD2画点
void lcd2_DrawPoint(u16 x,u16 y)
{
	lcd2_SetCursor(x,y);		//设置光标位置 
	lcd2_WriteRAM_Prepare();	//开始写入GRAM
	LCD2->LCD2_RAM=POINT_COLOR; 
//	lcd2_framebuf[x][y] = POINT_COLOR;
}
/***********************************************************/

/***********************************************************/
//快速画点
//x,y:坐标
//color:颜色
void lcd1_Fast_DrawPoint(u16 x,u16 y,u32 color)
{	   
	if(lcdltdc.pwidth!=0)//如果是RGB屏
	{
		LTDC_Draw_Point(x,y,color);
	}
}	 

//快速画点
//x,y:坐标
//color:颜色
void lcd2_Fast_DrawPoint(u16 x,u16 y,u32 color)
{	   
	if(x > LCD.width || y > LCD.height) return;
	if(lcd2dev.id==0X9341||lcd2dev.id==0X5310)
	{
		lcd2_WR_REG(lcd2dev.setxcmd); 
		lcd2_WR_DATA(x>>8);lcd2_WR_DATA(x&0XFF);  			 
		lcd2_WR_REG(lcd2dev.setycmd); 
		lcd2_WR_DATA(y>>8);lcd2_WR_DATA(y&0XFF); 		 	 
	}else if(lcd2dev.id==0X5510)
	{
		lcd2_WR_REG(lcd2dev.setxcmd);lcd2_WR_DATA(x>>8);  
		lcd2_WR_REG(lcd2dev.setxcmd+1);lcd2_WR_DATA(x&0XFF);	  
		lcd2_WR_REG(lcd2dev.setycmd);lcd2_WR_DATA(y>>8);  
		lcd2_WR_REG(lcd2dev.setycmd+1);lcd2_WR_DATA(y&0XFF); 
	}
	else if(lcd2dev.id==0X1963)
	{
		if(lcd2dev.dir==0)x=lcd2dev.width-1-x;
		lcd2_WR_REG(lcd2dev.setxcmd); 
		lcd2_WR_DATA(x>>8);lcd2_WR_DATA(x&0XFF); 		
		lcd2_WR_DATA(x>>8);lcd2_WR_DATA(x&0XFF); 		
		lcd2_WR_REG(lcd2dev.setycmd); 
		lcd2_WR_DATA(y>>8);lcd2_WR_DATA(y&0XFF); 		
		lcd2_WR_DATA(y>>8);lcd2_WR_DATA(y&0XFF); 		
	}		 
	LCD2->LCD2_REG=lcd2dev.wramcmd; 
	LCD2->LCD2_RAM=color; 
//	lcd2_framebuf[x][y] = POINT_COLOR;
}	 
/************************************************************/

/*************************************************************/
//lcd1
//背光设置
//pwm:背光等级,0~100.越大越亮.
void lcd1_BackLightSet(u8 pwm)
{	
	u16 compare;
	
	if(pwm > 100) return;
	compare = 500 - pwm*5;
	
	TIM3->CCR2 = compare; 
}

//lcd2
//SSD1963 背光设置
//pwm:背光等级,0~100.越大越亮.
void lcd2_BackLightSet(u8 pwm)
{	
	if(pwm > 100) return;
	lcd2_WR_REG(0xBE);	//配置PWM输出
	lcd2_WR_DATA(0x05);	//1设置PWM频率
	lcd2_WR_DATA(pwm*2.55);//2设置PWM占空比
	lcd2_WR_DATA(0x01);	//3设置C
	lcd2_WR_DATA(0xFF);	//4设置D
	lcd2_WR_DATA(0x00);	//5设置E
	lcd2_WR_DATA(0x00);	//6设置F
}
/***************************************************************/


/**************************************************************/
//设置LCD显示方向
//dir:0,竖屏；1,横屏
void lcd1_Display_Dir(u8 dir)
{
    lcddev.dir=dir;         //横屏/竖屏
	if(lcdltdc.pwidth!=0)   //如果是RGB屏
	{
		LTDC_Display_Dir(dir);
		lcddev.width=lcdltdc.width;
		lcddev.height=lcdltdc.height;
	}
}


//设置LCD2显示方向
//dir:0,竖屏；1,横屏
void lcd2_Display_Dir(u8 dir)
{
  lcd2dev.dir=dir;         //横屏/竖屏
	if(dir==0)			//竖屏
	{
		lcd2dev.width=240;
		lcd2dev.height=320;
		if(lcd2dev.id==0X9341||lcd2dev.id==0X5310)
		{
			lcd2dev.wramcmd=0X2C;
	 		lcd2dev.setxcmd=0X2A;
			lcd2dev.setycmd=0X2B;  	 
			if(lcd2dev.id==0X5310)
			{
				lcd2dev.width=320;
				lcd2dev.height=480;
			}
		}else if(lcd2dev.id==0x5510)
		{
			lcd2dev.wramcmd=0X2C00;
	 		lcd2dev.setxcmd=0X2A00;
			lcd2dev.setycmd=0X2B00; 
			lcd2dev.width=480;
			lcd2dev.height=800;
		}else if(lcd2dev.id==0X1963)
		{
			lcd2dev.wramcmd=0X2C;	//设置写入GRAM的指令 
			lcd2dev.setxcmd=0X2B;	//设置写X坐标指令
			lcd2dev.setycmd=0X2A;	//设置写Y坐标指令
			lcd2dev.width=480;		//设置宽度480
			lcd2dev.height=800;		//设置高度800  			
		}
	}
	else 				//横屏
	{	  				
		lcd2dev.width=320;
		lcd2dev.height=240;
		if(lcd2dev.id==0X9341||lcd2dev.id==0X5310)
		{
			lcd2dev.wramcmd=0X2C;
	 		lcd2dev.setxcmd=0X2A;
			lcd2dev.setycmd=0X2B;  	 
		}else if(lcd2dev.id==0x5510)
		{
			lcd2dev.wramcmd=0X2C00;
	 		lcd2dev.setxcmd=0X2A00;
			lcd2dev.setycmd=0X2B00; 
			lcd2dev.width=800;
			lcd2dev.height=480;
		}else if(lcd2dev.id==0X1963)
		{
			lcd2dev.wramcmd=0X2C;	//设置写入GRAM的指令 
			lcd2dev.setxcmd=0X2A;	//设置写X坐标指令
			lcd2dev.setycmd=0X2B;	//设置写Y坐标指令
			lcd2dev.width=800;		//设置宽度800
			lcd2dev.height=480;		//设置高度480  
		}
		if(lcd2dev.id==0X5310)
		{ 	 
			lcd2dev.width=480;
			lcd2dev.height=320; 			
		}
	} 
	lcd2_Scan_Dir(DFT_SCAN_DIR);	//默认扫描方向
}
/**************************************************************/


/******************************************************************/
//LCD2设置窗口(对RGB屏无效),并自动设置画点坐标到窗口左上角(sx,sy).
//sx,sy:窗口起始坐标(左上角)
//width,height:窗口宽度和高度,必须大于0!!
//窗体大小:width*height. 
void lcd2_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{     
	u16 twidth,theight;
	twidth=sx+width-1;
	theight=sy+height-1;
	if(lcd2dev.id==0X9341||lcd2dev.id==0X5310||(lcd2dev.dir==1&&lcd2dev.id==0X1963))
	{
		lcd2_WR_REG(lcd2dev.setxcmd); 
		lcd2_WR_DATA(sx>>8); 
		lcd2_WR_DATA(sx&0XFF);	 
		lcd2_WR_DATA(twidth>>8); 
		lcd2_WR_DATA(twidth&0XFF);  
		lcd2_WR_REG(lcd2dev.setycmd); 
		lcd2_WR_DATA(sy>>8); 
		lcd2_WR_DATA(sy&0XFF); 
		lcd2_WR_DATA(theight>>8); 
		lcd2_WR_DATA(theight&0XFF); 
	}else if(lcd2dev.id==0X1963)//1963竖屏特殊处理
	{
		sx=lcd2dev.width-width-sx; 
		height=sy+height-1; 
		lcd2_WR_REG(lcd2dev.setxcmd); 
		lcd2_WR_DATA(sx>>8); 
		lcd2_WR_DATA(sx&0XFF);	 
		lcd2_WR_DATA((sx+width-1)>>8); 
		lcd2_WR_DATA((sx+width-1)&0XFF);  
		lcd2_WR_REG(lcd2dev.setycmd); 
		lcd2_WR_DATA(sy>>8); 
		lcd2_WR_DATA(sy&0XFF); 
		lcd2_WR_DATA(height>>8); 
		lcd2_WR_DATA(height&0XFF); 		
	}else if(lcd2dev.id==0X5510)
	{
		lcd2_WR_REG(lcd2dev.setxcmd);lcd2_WR_DATA(sx>>8);  
		lcd2_WR_REG(lcd2dev.setxcmd+1);lcd2_WR_DATA(sx&0XFF);	  
		lcd2_WR_REG(lcd2dev.setxcmd+2);lcd2_WR_DATA(twidth>>8);   
		lcd2_WR_REG(lcd2dev.setxcmd+3);lcd2_WR_DATA(twidth&0XFF);   
		lcd2_WR_REG(lcd2dev.setycmd);lcd2_WR_DATA(sy>>8);   
		lcd2_WR_REG(lcd2dev.setycmd+1);lcd2_WR_DATA(sy&0XFF);  
		lcd2_WR_REG(lcd2dev.setycmd+2);lcd2_WR_DATA(theight>>8);   
		lcd2_WR_REG(lcd2dev.setycmd+3);lcd2_WR_DATA(theight&0XFF);  
	} 
}
/**************************************************************/

//SRAM底层驱动，时钟使能，引脚分配
//此函数会被HAL_SRAM_Init()调用
//hsram:SRAM句柄
void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
    GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_FMC_CLK_ENABLE();				//使能FMC时钟
	__HAL_RCC_GPIOD_CLK_ENABLE();			//使能GPIOD时钟
	__HAL_RCC_GPIOE_CLK_ENABLE();			//使能GPIOE时钟
	
	//初始化PD0,1,4,5,7,8,9,10,13,14,15
	GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_8|\
                     GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_Initure.Mode=GPIO_MODE_AF_PP; 		//复用
	GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;		//高速
	GPIO_Initure.Alternate=GPIO_AF12_FMC;	//复用为FMC
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);
	
	//初始化PE7,8,9,10,11,12,13,14,15
	GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|\
                     GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	HAL_GPIO_Init(GPIOE,&GPIO_Initure);
}
        
//初始化lcd
//该初始化函数可以初始化各种型号的LCD(详见本.c文件最前面的描述)
void LCD_Init(void)
{ 	  
	GPIO_InitTypeDef GPIO_Initure;
	FMC_NORSRAM_TimingTypeDef FSMC_ReadWriteTim;
	FMC_NORSRAM_TimingTypeDef FSMC_WriteTim;
	
	TIM_HandleTypeDef TIM3_Handler;         //定时器3PWM句柄 
	TIM_OC_InitTypeDef TIM3_CH2Handler;	    //定时器3通道4句柄
	
	
//	lcddev.id=LTDC_PanelID_Read();	//检查是否有RGB屏接入
	lcddev.id  = 0X7016;
	if(lcddev.id!=0)
	{
		LTDC_Init();			    //ID非零,说明有RGB屏接入.
		
		lcddev.width=lcdltdc.pwidth;
		lcddev.height=lcdltdc.pheight;
	}
	
	
	__HAL_RCC_TIM3_CLK_ENABLE();			//使能定时器3
	
	__HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟

	
	GPIO_Initure.Pin=GPIO_PIN_5;           	//PB1
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
	GPIO_Initure.Alternate= GPIO_AF2_TIM3;	//PB5复用为TIM3_CH2
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
	GPIO_Initure.Pin=GPIO_PIN_6;           	//PB1
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//复用推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
	PBout(6) = 1;

	
	TIM3_Handler.Instance=TIM3;            //定时器3
	TIM3_Handler.Init.Prescaler=TIM3_psc;       //定时器分频
	TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
	TIM3_Handler.Init.Period=TIM3_arr;          //自动重装载值
	TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_PWM_Init(&TIM3_Handler);       //初始化PWM
	
	TIM3_CH2Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
	TIM3_CH2Handler.Pulse=TIM3_arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
	TIM3_CH2Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低 
	HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH2Handler,TIM_CHANNEL_2);//配置TIM3通道4

	HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_2);//开启PWM通道2
	
	SRAM_Handler.Instance=FMC_NORSRAM_DEVICE;        //BANK1
	SRAM_Handler.Extended=FMC_NORSRAM_EXTENDED_DEVICE;       
	
	SRAM_Handler.Init.NSBank=FMC_NORSRAM_BANK1;     //使用NE1
	SRAM_Handler.Init.DataAddressMux=FMC_DATA_ADDRESS_MUX_DISABLE;  //不复用数据线
	SRAM_Handler.Init.MemoryType=FMC_MEMORY_TYPE_SRAM;              //SRAM
	SRAM_Handler.Init.MemoryDataWidth=FMC_NORSRAM_MEM_BUS_WIDTH_16; //16位数据宽度
	SRAM_Handler.Init.BurstAccessMode=FMC_BURST_ACCESS_MODE_DISABLE; //是否使能突发访问,仅对同步突发存储器有效,此处未用到
	SRAM_Handler.Init.WaitSignalPolarity=FMC_WAIT_SIGNAL_POLARITY_LOW;//等待信号的极性,仅在突发模式访问下有用
	SRAM_Handler.Init.WaitSignalActive=FMC_WAIT_TIMING_BEFORE_WS;   //存储器是在等待周期之前的一个时钟周期还是等待周期期间使能NWAIT
	SRAM_Handler.Init.WriteOperation=FMC_WRITE_OPERATION_ENABLE;    //存储器写使能
	SRAM_Handler.Init.WaitSignal=FMC_WAIT_SIGNAL_DISABLE;           //等待使能位,此处未用到
	SRAM_Handler.Init.ExtendedMode=FMC_EXTENDED_MODE_ENABLE;        //读写使用不同的时序
	SRAM_Handler.Init.AsynchronousWait=FMC_ASYNCHRONOUS_WAIT_DISABLE;//是否使能同步传输模式下的等待信号,此处未用到
	SRAM_Handler.Init.WriteBurst=FMC_WRITE_BURST_DISABLE;           //禁止突发写
	SRAM_Handler.Init.ContinuousClock=FMC_CONTINUOUS_CLOCK_SYNC_ASYNC;
	
	//FSMC读时序控制寄存器
	FSMC_ReadWriteTim.AddressSetupTime=0x0F;        //地址建立时间(ADDSET)为15个HCLK 1/180M*15=5.5ns*15=82.5ns
	FSMC_ReadWriteTim.AddressHoldTime=0x00;
	FSMC_ReadWriteTim.DataSetupTime=0x46;           //数据保存时间(DATAST)为70个HCLK	=5.5*70=385ns
	FSMC_ReadWriteTim.AccessMode=FMC_ACCESS_MODE_A; //模式A
	//FSMC写时序控制寄存器
	FSMC_WriteTim.AddressSetupTime=0x0F;            //地址建立时间(ADDSET)为15个HCLK=82.5ns
	FSMC_WriteTim.AddressHoldTime=0x00;
	FSMC_WriteTim.DataSetupTime=0x0F;               //数据保存时间(DATAST)为5.5ns*15个HCLK=82.5ns
	FSMC_WriteTim.AccessMode=FMC_ACCESS_MODE_A;     //模式A
	HAL_SRAM_Init(&SRAM_Handler,&FSMC_ReadWriteTim,&FSMC_WriteTim);		
	delay_ms(50); // delay 50 ms  
	
//	printf(" LCD2 ID:%x\r\n",lcd2dev.id); //打印LCD ID   


	lcd2dev.id = 0x1963;
	if(lcd2dev.id==0X1963)
	{
			lcd2_WR_REG(0xE2);		//Set PLL with OSC = 10MHz (hardware),	Multiplier N = 35, 250MHz < VCO < 800MHz = OSC*(N+1), VCO = 300MHz
			lcd2_WR_DATA(0x1D);		//参数1 
			lcd2_WR_DATA(0x02);		//参数2 Divider M = 2, PLL = 300/(M+1) = 100MHz
			lcd2_WR_DATA(0x04);		//参数3 Validate M and N values   
			delay_us(100);
			lcd2_WR_REG(0xE0);		// Start PLL command
			lcd2_WR_DATA(0x01);		// enable PLL
			delay_ms(10);
			lcd2_WR_REG(0xE0);		// Start PLL command again
			lcd2_WR_DATA(0x03);		// now, use PLL output as system clock	
			delay_ms(12);  
			lcd2_WR_REG(0x01);		//软复位
			delay_ms(10);
			
			lcd2_WR_REG(0xE6);		//设置像素频率,33Mhz
			lcd2_WR_DATA(0x2F);
			lcd2_WR_DATA(0xFF);
			lcd2_WR_DATA(0xFF);

			lcd2_WR_REG(0xB0);		//设置LCD模式
			lcd2_WR_DATA(0x20);		//24位模式
			lcd2_WR_DATA(0x00);		//TFT 模式 
	
			lcd2_WR_DATA((SSD_HOR_RESOLUTION-1)>>8);//设置LCD水平像素
			lcd2_WR_DATA(SSD_HOR_RESOLUTION-1);		 
			lcd2_WR_DATA((SSD_VER_RESOLUTION-1)>>8);//设置LCD垂直像素
			lcd2_WR_DATA(SSD_VER_RESOLUTION-1);		 
			lcd2_WR_DATA(0x00);		//RGB序列 
			
			lcd2_WR_REG(0xB4);		//Set horizontal period
			lcd2_WR_DATA((SSD_HT-1)>>8);
			lcd2_WR_DATA(SSD_HT-1);
			lcd2_WR_DATA(SSD_HPS>>8);
			lcd2_WR_DATA(SSD_HPS);
			lcd2_WR_DATA(SSD_HOR_PULSE_WIDTH-1);
			lcd2_WR_DATA(0x00);
			lcd2_WR_DATA(0x00);
			lcd2_WR_DATA(0x00);
			lcd2_WR_REG(0xB6);		//Set vertical period
			lcd2_WR_DATA((SSD_VT-1)>>8);
			lcd2_WR_DATA(SSD_VT-1);
			lcd2_WR_DATA(SSD_VPS>>8);
			lcd2_WR_DATA(SSD_VPS);
			lcd2_WR_DATA(SSD_VER_FRONT_PORCH-1);
			lcd2_WR_DATA(0x00);
			lcd2_WR_DATA(0x00);
			
			lcd2_WR_REG(0xF0);	//设置SSD1963与CPU接口为16bit  
			lcd2_WR_DATA(0x03);	//16-bit(565 format) data for 16bpp 

			lcd2_WR_REG(0x29);	//开启显示
			//设置PWM输出  背光通过占空比可调 
			lcd2_WR_REG(0xD0);	//设置自动白平衡DBC
			lcd2_WR_DATA(0x00);	//disable
	
			lcd2_WR_REG(0xBE);	//配置PWM输出
			lcd2_WR_DATA(0x05);	//1设置PWM频率
			lcd2_WR_DATA(0xFE);	//2设置PWM占空比
			lcd2_WR_DATA(0x01);	//3设置C
			lcd2_WR_DATA(0x00);	//4设置D
			lcd2_WR_DATA(0x00);	//5设置E 
			lcd2_WR_DATA(0x00);	//6设置F 
			
			lcd2_WR_REG(0xB8);	//设置GPIO配置
			lcd2_WR_DATA(0x03);	//2个IO口设置成输出
			lcd2_WR_DATA(0x01);	//GPIO使用正常的IO功能 
			lcd2_WR_REG(0xBA);
			lcd2_WR_DATA(0X01);	//GPIO[1:0]=01,控制LCD方向
			

				
	}
	if(lcd2dev.id==0X9341||lcd2dev.id==0X5310||lcd2dev.id==0X5510||lcd2dev.id==0X1963)//如果是这几个IC,则设置WR时序为最快
	{
		//重新配置写时序控制寄存器的时序   	 							    
        FSMC_WriteTim.AddressSetupTime=4;      
        FSMC_WriteTim.DataSetupTime=4;         
        FMC_NORSRAM_Extended_Timing_Init(SRAM_Handler.Extended,&FSMC_WriteTim,SRAM_Handler.Init.NSBank,SRAM_Handler.Init.ExtendedMode);
	}   	
	lcd1_Display_Dir(1);		//默认为横屏
	lcd2_Display_Dir(1);	
 
	lcd2_Clear(BLACK);
	lcd1_Clear(BLACK);
	
//	LCD_LED=1;				//点亮背光
	lcd2_BackLightSet(0);
	lcd1_BackLightSet(0);

	
	LCD.SelectScreen(LCD_2);
}  

/********************************************************/
//LCD清屏函数
//color:要清屏的填充色
void lcd1_Clear(u32 color)
{
	if(lcdltdc.pwidth!=0)	//如果是RGB屏
	{
		LTDC_Clear(color);
	}
}

//LCD2清屏函数
//color:要清屏的填充色
void lcd2_Clear(u32 color)
{
	u32 index=0;      
	u32 totalpoint=lcddev.width;
	
	totalpoint*=lcddev.height; 			//得到总点数
	lcd2_SetCursor(0x00,0x0000);			//设置光标位置 
	lcd2_WriteRAM_Prepare();     		//开始写入GRAM	 	  
	for(index=0;index<totalpoint;index++)
	{
		LCD2->LCD2_RAM=color;	
	}
//	memset(lcd2_framebuf,color,800*480);
}
/*********************************************************/

/*********************************************************/
//LCD在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
//**注意** :FILL函数调用的时候，特别是大范围填充颜色的时候最好调用系统函数OS_CRITICAL_ENTER()进入临界区，防止系统调用打断出现闪屏或者黑屏情况
void lcd1_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color)
{          

	LTDC_Fill(sx,sy,ex,ey,color);
	
}  

void lcd2_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color)
{          
	u16 i,j;
	u16 height,width;
	
	width=ex-sx+1; 			//得到填充的宽度
	height=ey-sy+1;			//高度 
	for(i=0;i<height;i++)
	{
		lcd2_SetCursor(sx,sy+i);      				//设置光标位置 
		lcd2_WriteRAM_Prepare();     			//开始写入GRAM	  
		for(j=0;j<width;j++)
		{
			LCD2->LCD2_RAM=color;	//显示颜色 	    
//			lcd2_framebuf[j][i] = color;
		}
	} 
//	memset(lcd2_framebuf,color,800*480);

	
}
/****************************************************************/


/***************************************************************/
//LCD在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void lcd1_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	if(lcdltdc.pwidth!=0)	//如果是RGB屏
	{
		LTDC_Color_Fill(sx,sy,ex,ey,color);
	}
}

//LCD2在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void lcd2_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;

	width=ex-sx+1; 			//得到填充的宽度
	height=ey-sy+1;			//高度
	for(i=0;i<height;i+=2)
	{
//		memcpy(&lcd2_framebuf[sy+i][sx],&color[i*width],width);
		lcd2_SetCursor(sx,sy+i);   	//设置光标位置 
		lcd2_WriteRAM_Prepare();     //开始写入GRAM
		for(j=0;j<width;j++)
		{
			LCD2->LCD2_RAM=color[i*width+j];//写入数据 
//			lcd2_framebuf[j][i] = color[i*width+j];
		}
	}
	for(i=1;i<height;i+=2)
	{
//		memcpy(&lcd2_framebuf[sy+i][sx],&color[i*width],width);
		lcd2_SetCursor(sx,sy+i);   	//设置光标位置 
		lcd2_WriteRAM_Prepare();     //开始写入GRAM
		for(j=0;j<width;j++)
		{
			LCD2->LCD2_RAM=color[i*width+j];//写入数据 
//			lcd2_framebuf[j][i] = color[i*width+j];
		}
	}
	
} 
/**********************************************************************/


/**********************************************************************/
//LCD画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD.DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}   


/*******************************************************************/
//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD.DrawLine(x1,y1,x2,y1);
	LCD.DrawLine(x1,y1,x1,y2);
	LCD.DrawLine(x1,y2,x2,y2);
	LCD.DrawLine(x2,y1,x2,y2);
}
/*****************************************************************/


//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD.DrawPoint(x0+a,y0-b);             //5
 		LCD.DrawPoint(x0+b,y0-a);             //0           
		LCD.DrawPoint(x0+b,y0+a);             //4               
		LCD.DrawPoint(x0+a,y0+b);             //6 
		LCD.DrawPoint(x0-a,y0+b);             //1       
 		LCD.DrawPoint(x0-b,y0+a);             
		LCD.DrawPoint(x0-a,y0-b);             //2             
  	LCD.DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 	

/****************************************************************/
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24/32
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
  u16 temp,t1,t;
	u16 y0=y;
	u16 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else if(size==32)temp=asc2_3216[num][t];	//调用3216字体
		else if(size==64)temp=asc2_6432[num][t];//调用6432字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD.Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD.Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
}   

/****************************************************************/
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24/32
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowCharEX(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
  u16 temp,t1,t,i;
	u16 y0=y;
	u16 csize=256;		//得到字体一个字符对应点阵集所占的字节数	
	mode = mode;
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
		if(size==192)temp=asc2_6432[num][t]; 	 	//调用6432字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)
			{
				for(i=0;i<3;i++)
				{
					LCD.Fast_DrawPoint(x+i,y,POINT_COLOR);
					LCD.Fast_DrawPoint(x+i,y+1,POINT_COLOR);
					LCD.Fast_DrawPoint(x+i,y+2,POINT_COLOR);
				}
			}
			temp<<=1;
			y+=3;
			if(y>=lcddev.height)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x+=3;
				if(x>=lcddev.width)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
}   


//m^n函数
//返回值:m^n次方.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}

//显示区域颜色加深
//coefficient 1~99;
void LCD_Darker(u16 sx,u16 sy,u16 ex,u16 ey,u8 coefficient)
{
	u16 x_temp,y_temp,color;
	u16 RR,GG,BB;

	if(coefficient<1 || coefficient >99) return;
	
	for(y_temp = sy;y_temp < ey;y_temp++)
	{
		for(x_temp = sx;x_temp < ex;x_temp++)
		{
			color = LCD.ReadPoint(x_temp,y_temp);
			
			RR = color >> 11;
			GG = (color & 0x7FF) >> 5;
			BB = color & 0x1F;

			RR = (RR * coefficient / 100);
			GG = (GG * coefficient / 100);
			BB = (BB * coefficient / 100);

			POINT_COLOR = ((RR << 11) | (GG << 5) | (BB));
			LCD.DrawPoint(x_temp,y_temp);
		}
	}
	POINT_COLOR = BLACK;
}

//void LCD_MicroIcon(u16 sx,u16 sy,_microicon icon)
//{
//	u8 i,h,w,temp;
//	u8 *icon_tamp;
//	
//	switch(icon)
//	{
//		case wifi_1:icon_tamp = (u8*)gImage_microicon_wifi1;break;
//		case wifi_2:icon_tamp = (u8*)gImage_microicon_wifi2;break;
//		case wifi_3:icon_tamp = (u8*)gImage_microicon_wifi3;break;
//		case wifi_4:icon_tamp = (u8*)gImage_microicon_wifi4;break;
//		case bat_1:icon_tamp = (u8*)gImage_microicon_bat1;break;
//		case bat_2:icon_tamp = (u8*)gImage_microicon_bat2;break;
//		case link:icon_tamp = (u8*)gImage_microicon_link1;break;
//	}
//	
//	for(h=0;h<15;h++)
//	{   						
//		for(w=0;w<4;w++)
//		{
//			temp=icon_tamp[h*4+w];
//			for(i=0;i<8;i++)
//			{
//				if(temp&0x80)LCD.Fast_DrawPoint(sx+(w*8+i),sy+h,WHITE);
//				temp<<=1;
//			}
//		}
//	} 
//	 
//}


