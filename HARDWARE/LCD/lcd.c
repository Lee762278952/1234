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
//LCD��������
//LCD1:STM32F429Ӳ��RGB����(LTCD) 
//LCD2��FMC����SSD1963
//*****************************************/


void LCD_Init(void);													   	//��ʼ��
void LCD_SelectScreen(_lcd_select sel);

void lcd1_DisplayOn(void);													//����ʾ
void lcd1_DisplayOff(void);													//����ʾ
void lcd1_Clear(u32 Color);	 												//����
void lcd1_DrawPoint(u16 x,u16 y);											//����
u32  lcd1_ReadPoint(u16 x,u16 y); 											//���� 
u32 lcd2_FastReadPoint(u16 x,u16 y);
void lcd1_Fast_DrawPoint(u16 x,u16 y,u32 color);								//���ٻ���
void lcd1_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);				//���ָ����ɫ
void lcd1_Display_Dir(u8 dir);								//������Ļ��ʾ����
void lcd1_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color);		   				//��䵥ɫ
void lcd1_BackLightSet(u8 pwm);

void lcd2_DisplayOn(void);
void lcd2_DisplayOff(void);
void lcd2_Clear(u32 color);
void lcd2_DrawPoint(u16 x,u16 y);
u32  lcd2_ReadPoint(u16 x,u16 y);
void lcd2_Fast_DrawPoint(u16 x,u16 y,u32 color);
void lcd2_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);
void lcd2_Display_Dir(u8 dir);								//������Ļ��ʾ����
void lcd2_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color);		   				//��䵥ɫ
void lcd2_BackLightSet(u8 pwm);							//SSD1963 �������

void lcd2_WriteReg(u16 lcd_Reg, u16 lcd_RegValue);
u16  lcd2_ReadReg(u16 lcd_Reg);
void lcd2_WriteRAM_Prepare(void);
void lcd2_WriteRAM(u16 RGB_Code);
void lcd2_Scan_Dir(u8 dir);									//������ɨ�跽��
void lcd2_Set_Window(u16 sx,u16 sy,u16 width,u16 height);	//���ô���	
void lcd2_SetCursor(u16 Xpos, u16 Ypos);										//lcd2���ù��


void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);						 			//��Բ
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);							//����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);	   				//������

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);						//��ʾһ���ַ�
void LCD_ShowCharEX(u16 x,u16 y,u8 num,u8 size,u8 mode);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						//��ʾһ������
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//��ʾ ����
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);		//��ʾһ���ַ���,12/16����
void LCD_Darker(u16 sx,u16 sy,u16 ex,u16 ey,u8 coefficient);
//void LCD_MicroIcon(u16 sx,u16 sy,_microicon icon);


//u16 lcd2_framebuf[800][480] __attribute__((at(LCD2_FRAME_BUF_ADDR)));	//������Ļ�ڲ�����


SRAM_HandleTypeDef SRAM_Handler;    //SRAM���(���ڿ���LCD)
//LCD�Ļ�����ɫ�ͱ���ɫ	   
vu32 POINT_COLOR=0xFF000000;		//������ɫ
u32 BACK_COLOR =0xFFFFFFFF;  	//����ɫ 

//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev,lcd2dev;
//LCD���ܶ���
_lcd_class LCD = {
	.LCD_Sel	= LCD_1,
	.width 		= 1024,		//LCD ���
	.height 	= 600,			//LCD �߶�
	.id 			= 0	,			//LCD ID
	.dir 			= 0	,	//���������������ƣ�0��������1��������	
	.wramcmd 	= 0	,//��ʼдgramָ��
	.setxcmd 	= 0	,//����x����ָ��
	.setycmd 	= 0	,//����y����ָ�� 
	
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
	 
	 
//LCD2д�Ĵ�������
//regval:�Ĵ���ֵ
void lcd2_WR_REG(vu16 regval)
{   
	regval=regval;		//ʹ��-O2�Ż���ʱ��,����������ʱ
	LCD2->LCD2_REG=regval;//д��Ҫд�ļĴ������	 
}
//дLCD2����
//data:Ҫд���ֵ
void lcd2_WR_DATA(vu16 data)
{	  
	data=data;			//ʹ��-O2�Ż���ʱ��,����������ʱ
	LCD2->LCD2_RAM=data;		 
}
//��LCD2����
//����ֵ:������ֵ
u16 lcd2_RD_DATA(void)
{
	vu16 ram;			//��ֹ���Ż�
	ram=LCD2->LCD2_RAM;	
	return ram;	 
}					   
//LCD2д�Ĵ���
//LCD_Reg:�Ĵ�����ַ
//LCD_RegValue:Ҫд�������
void lcd2_WriteReg(u16 LCD2_Reg,u16 LCD2_RegValue)
{	
	LCD2->LCD2_REG = LCD2_Reg;		//д��Ҫд�ļĴ������	 
	LCD2->LCD2_RAM = LCD2_RegValue;//д������	    		 
}	   
//���Ĵ���
//LCD_Reg:�Ĵ�����ַ
//����ֵ:����������
u16 lcd2_ReadReg(u16 LCD2_Reg)
{										   
	lcd2_WR_REG(LCD2_Reg);		//д��Ҫ���ļĴ������
	delay_us(5);		  
	return lcd2_RD_DATA();		//���ض�����ֵ
}   
//��ʼдGRAM
void lcd2_WriteRAM_Prepare(void)
{
 	LCD2->LCD2_REG=lcd2dev.wramcmd;	  
}	 
//LCDдGRAM
//RGB_Code:��ɫֵ
void lcd2_WriteRAM(u16 RGB_Code)
{							    
	LCD2->LCD2_RAM = RGB_Code;//дʮ��λGRAM
}
//��ILI93xx����������ΪGBR��ʽ��������д���ʱ��ΪRGB��ʽ��
//ͨ���ú���ת��
//c:GBR��ʽ����ɫֵ
//����ֵ��RGB��ʽ����ɫֵ
u16 LCD_BGR2RGB(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
} 
//��mdk -O1ʱ���Ż�ʱ��Ҫ����
//��ʱi
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
//��ȡ��ĳ�����ɫֵ	 
//x,y:����
//����ֵ:�˵����ɫ
/***********************************************************/
//LCDȡ��
u32 lcd1_ReadPoint(u16 x,u16 y)
{
	if(x>=lcddev.width||y>=lcddev.height)return 0;	//�����˷�Χ,ֱ�ӷ���
	if(lcdltdc.pwidth!=0)							//�����RGB��
	{
		return LTDC_Read_Point(x,y);
	}
	else return 0;
}	

//LCD2ȡ��
u32 lcd2_ReadPoint(u16 x,u16 y)
{
 	u16 r=0;
//		u16 g=0,b=0;
	if(x>=lcd2dev.width||y>=lcd2dev.height)return 0;	//�����˷�Χ,ֱ�ӷ���
	lcd2_SetCursor(x,y);	    
	/*if(lcd2dev.id==0X9341||lcd2dev.id==0X5310||lcd2dev.id==0X1963)*/
	lcd2_WR_REG(0X2E);//9341/3510/1963 ���Ͷ�GRAMָ��
//	else if(lcd2dev.id==0X5510)lcd2_WR_REG(0X2E00);	//5510 ���Ͷ�GRAMָ��
 	r=lcd2_RD_DATA();								//dummy Read	   
//	if(lcd2dev.id==0X1963)
	return r;					//1963ֱ�Ӷ��Ϳ��� 
//	opt_delay(2);	  
// 	r=lcd2_RD_DATA();  		  						//ʵ��������ɫ
//	//9341/NT35310/NT35510Ҫ��2�ζ��� 
//	opt_delay(2);	  
//	b=lcd2_RD_DATA(); 
//	g=r&0XFF;		//����9341/5310/5510,��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
//	g<<=8; 
//	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));	//ILI9341/NT35310/NT35510��Ҫ��ʽת��һ��
}	

////LCD2����ȡ��
//u32 lcd2_FastReadPoint(u16 x,u16 y)
//{
//	if(x>=lcd2dev.width||y>=lcd2dev.height)return 0;	//�����˷�Χ,ֱ�ӷ���
//	return lcd2_framebuf[x][y];
//}	
/*************************************************************/

/*************************************************************/
//LCD������ʾ
void lcd1_DisplayOn(void)
{					   
	LTDC_Switch(1);//����LCD

}	 

//LCD2������ʾ
void lcd2_DisplayOn(void)
{					   
	if(lcd2dev.id==0X9341||lcd2dev.id==0X5310||lcd2dev.id==0X1963)lcd2_WR_REG(0X29);	//������ʾ
	else if(lcd2dev.id==0X5510)lcd2_WR_REG(0X2900);	//������ʾ
}	 
/************************************************************/


/*************************************************************/
//LCD�ر���ʾ
void lcd1_DisplayOff(void)
{	   
	LTDC_Switch(0);//�ر�LCD
} 

//LCD2�ر���ʾ
void lcd2_DisplayOff(void)
{	   
	if(lcd2dev.id==0X9341||lcd2dev.id==0X5310||lcd2dev.id==0X1963)lcd2_WR_REG(0X28);	//�ر���ʾ
	else if(lcd2dev.id==0X5510)lcd2_WR_REG(0X2800);	//�ر���ʾ
} 
/**************************************************************/


/***************************************************************/
//LCD2���ù��λ��(��RGB����Ч)
//Xpos:������
//Ypos:������
void lcd2_SetCursor(u16 Xpos, u16 Ypos)
{	  			 		
	if(lcd2dev.dir==0)//x������Ҫ�任
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
//����LCD���Զ�ɨ�跽��(��RGB����Ч)
//ע��:�����������ܻ��ܵ��˺������õ�Ӱ��(������9341),
//����,һ������ΪL2R_U2D����,�������Ϊ����ɨ�跽ʽ,���ܵ�����ʾ������.
//dir:0~7,����8������(���嶨���lcd.h)
//9341/5310/5510/1963��IC�Ѿ�ʵ�ʲ���	   	   
void lcd2_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u16 dirreg=0;
	u16 temp;  
	if((lcd2dev.dir==1&&lcd2dev.id!=0X1963)||(lcd2dev.dir==0&&lcd2dev.id==0X1963))//����ʱ����1963���ı�ɨ�跽������ʱ1963�ı䷽��
	{			   
		switch(dir)//����ת��
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
	if(lcd2dev.id==0x9341||lcd2dev.id==0X5310||lcd2dev.id==0X5510||lcd2dev.id==0X1963)//9341/5310/5510/1963,���⴦��
	{
		switch(dir)
		{
			case L2R_U2D://������,���ϵ���
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://������,���µ���
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://���ҵ���,���ϵ���
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://���ҵ���,���µ���
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://���ϵ���,������
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://���ϵ���,���ҵ���
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://���µ���,������
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://���µ���,���ҵ���
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		if(lcd2dev.id==0X5510)dirreg=0X3600;
		else dirreg=0X36;
 		if((lcd2dev.id!=0X5310)&&(lcd2dev.id!=0X5510)&&(lcd2dev.id!=0X1963))regval|=0X08;//5310/5510/1963����ҪBGR   
 		lcd2_WriteReg(dirreg,regval);
		if(lcd2dev.id!=0X1963)//1963�������괦��
		{
			if(regval&0X20)
			{
				if(lcd2dev.width<lcd2dev.height)//����X,Y
				{
					temp=lcd2dev.width;
					lcd2dev.width=lcd2dev.height;
					lcd2dev.height=temp;
				}
			}else  
			{
				if(lcd2dev.width>lcd2dev.height)//����X,Y
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
//LCD����
//x,y:����
//POINT_COLOR:�˵����ɫ
void lcd1_DrawPoint(u16 x,u16 y)
{
	if(lcdltdc.pwidth!=0)//�����RGB��
	{
		LTDC_Draw_Point(x,y,POINT_COLOR);
	}
}

//LCD2����
void lcd2_DrawPoint(u16 x,u16 y)
{
	lcd2_SetCursor(x,y);		//���ù��λ�� 
	lcd2_WriteRAM_Prepare();	//��ʼд��GRAM
	LCD2->LCD2_RAM=POINT_COLOR; 
//	lcd2_framebuf[x][y] = POINT_COLOR;
}
/***********************************************************/

/***********************************************************/
//���ٻ���
//x,y:����
//color:��ɫ
void lcd1_Fast_DrawPoint(u16 x,u16 y,u32 color)
{	   
	if(lcdltdc.pwidth!=0)//�����RGB��
	{
		LTDC_Draw_Point(x,y,color);
	}
}	 

//���ٻ���
//x,y:����
//color:��ɫ
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
//��������
//pwm:����ȼ�,0~100.Խ��Խ��.
void lcd1_BackLightSet(u8 pwm)
{	
	u16 compare;
	
	if(pwm > 100) return;
	compare = 500 - pwm*5;
	
	TIM3->CCR2 = compare; 
}

//lcd2
//SSD1963 ��������
//pwm:����ȼ�,0~100.Խ��Խ��.
void lcd2_BackLightSet(u8 pwm)
{	
	if(pwm > 100) return;
	lcd2_WR_REG(0xBE);	//����PWM���
	lcd2_WR_DATA(0x05);	//1����PWMƵ��
	lcd2_WR_DATA(pwm*2.55);//2����PWMռ�ձ�
	lcd2_WR_DATA(0x01);	//3����C
	lcd2_WR_DATA(0xFF);	//4����D
	lcd2_WR_DATA(0x00);	//5����E
	lcd2_WR_DATA(0x00);	//6����F
}
/***************************************************************/


/**************************************************************/
//����LCD��ʾ����
//dir:0,������1,����
void lcd1_Display_Dir(u8 dir)
{
    lcddev.dir=dir;         //����/����
	if(lcdltdc.pwidth!=0)   //�����RGB��
	{
		LTDC_Display_Dir(dir);
		lcddev.width=lcdltdc.width;
		lcddev.height=lcdltdc.height;
	}
}


//����LCD2��ʾ����
//dir:0,������1,����
void lcd2_Display_Dir(u8 dir)
{
  lcd2dev.dir=dir;         //����/����
	if(dir==0)			//����
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
			lcd2dev.wramcmd=0X2C;	//����д��GRAM��ָ�� 
			lcd2dev.setxcmd=0X2B;	//����дX����ָ��
			lcd2dev.setycmd=0X2A;	//����дY����ָ��
			lcd2dev.width=480;		//���ÿ��480
			lcd2dev.height=800;		//���ø߶�800  			
		}
	}
	else 				//����
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
			lcd2dev.wramcmd=0X2C;	//����д��GRAM��ָ�� 
			lcd2dev.setxcmd=0X2A;	//����дX����ָ��
			lcd2dev.setycmd=0X2B;	//����дY����ָ��
			lcd2dev.width=800;		//���ÿ��800
			lcd2dev.height=480;		//���ø߶�480  
		}
		if(lcd2dev.id==0X5310)
		{ 	 
			lcd2dev.width=480;
			lcd2dev.height=320; 			
		}
	} 
	lcd2_Scan_Dir(DFT_SCAN_DIR);	//Ĭ��ɨ�跽��
}
/**************************************************************/


/******************************************************************/
//LCD2���ô���(��RGB����Ч),���Զ����û������굽�������Ͻ�(sx,sy).
//sx,sy:������ʼ����(���Ͻ�)
//width,height:���ڿ�Ⱥ͸߶�,�������0!!
//�����С:width*height. 
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
	}else if(lcd2dev.id==0X1963)//1963�������⴦��
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

//SRAM�ײ�������ʱ��ʹ�ܣ����ŷ���
//�˺����ᱻHAL_SRAM_Init()����
//hsram:SRAM���
void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
    GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_FMC_CLK_ENABLE();				//ʹ��FMCʱ��
	__HAL_RCC_GPIOD_CLK_ENABLE();			//ʹ��GPIODʱ��
	__HAL_RCC_GPIOE_CLK_ENABLE();			//ʹ��GPIOEʱ��
	
	//��ʼ��PD0,1,4,5,7,8,9,10,13,14,15
	GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_8|\
                     GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_Initure.Mode=GPIO_MODE_AF_PP; 		//����
	GPIO_Initure.Pull=GPIO_PULLUP;			//����
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;		//����
	GPIO_Initure.Alternate=GPIO_AF12_FMC;	//����ΪFMC
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);
	
	//��ʼ��PE7,8,9,10,11,12,13,14,15
	GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|\
                     GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	HAL_GPIO_Init(GPIOE,&GPIO_Initure);
}
        
//��ʼ��lcd
//�ó�ʼ���������Գ�ʼ�������ͺŵ�LCD(�����.c�ļ���ǰ�������)
void LCD_Init(void)
{ 	  
	GPIO_InitTypeDef GPIO_Initure;
	FMC_NORSRAM_TimingTypeDef FSMC_ReadWriteTim;
	FMC_NORSRAM_TimingTypeDef FSMC_WriteTim;
	
	TIM_HandleTypeDef TIM3_Handler;         //��ʱ��3PWM��� 
	TIM_OC_InitTypeDef TIM3_CH2Handler;	    //��ʱ��3ͨ��4���
	
	
//	lcddev.id=LTDC_PanelID_Read();	//����Ƿ���RGB������
	lcddev.id  = 0X7016;
	if(lcddev.id!=0)
	{
		LTDC_Init();			    //ID����,˵����RGB������.
		
		lcddev.width=lcdltdc.pwidth;
		lcddev.height=lcdltdc.pheight;
	}
	
	
	__HAL_RCC_TIM3_CLK_ENABLE();			//ʹ�ܶ�ʱ��3
	
	__HAL_RCC_GPIOB_CLK_ENABLE();			//����GPIOBʱ��

	
	GPIO_Initure.Pin=GPIO_PIN_5;           	//PB1
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//�����������
	GPIO_Initure.Pull=GPIO_PULLUP;          //����
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
	GPIO_Initure.Alternate= GPIO_AF2_TIM3;	//PB5����ΪTIM3_CH2
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
	GPIO_Initure.Pin=GPIO_PIN_6;           	//PB1
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//�����������
	GPIO_Initure.Pull=GPIO_PULLUP;          //����
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
	PBout(6) = 1;

	
	TIM3_Handler.Instance=TIM3;            //��ʱ��3
	TIM3_Handler.Init.Prescaler=TIM3_psc;       //��ʱ����Ƶ
	TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//���ϼ���ģʽ
	TIM3_Handler.Init.Period=TIM3_arr;          //�Զ���װ��ֵ
	TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_PWM_Init(&TIM3_Handler);       //��ʼ��PWM
	
	TIM3_CH2Handler.OCMode=TIM_OCMODE_PWM1; //ģʽѡ��PWM1
	TIM3_CH2Handler.Pulse=TIM3_arr/2;            //���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%
	TIM3_CH2Handler.OCPolarity=TIM_OCPOLARITY_LOW; //����Ƚϼ���Ϊ�� 
	HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH2Handler,TIM_CHANNEL_2);//����TIM3ͨ��4

	HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_2);//����PWMͨ��2
	
	SRAM_Handler.Instance=FMC_NORSRAM_DEVICE;        //BANK1
	SRAM_Handler.Extended=FMC_NORSRAM_EXTENDED_DEVICE;       
	
	SRAM_Handler.Init.NSBank=FMC_NORSRAM_BANK1;     //ʹ��NE1
	SRAM_Handler.Init.DataAddressMux=FMC_DATA_ADDRESS_MUX_DISABLE;  //������������
	SRAM_Handler.Init.MemoryType=FMC_MEMORY_TYPE_SRAM;              //SRAM
	SRAM_Handler.Init.MemoryDataWidth=FMC_NORSRAM_MEM_BUS_WIDTH_16; //16λ���ݿ��
	SRAM_Handler.Init.BurstAccessMode=FMC_BURST_ACCESS_MODE_DISABLE; //�Ƿ�ʹ��ͻ������,����ͬ��ͻ���洢����Ч,�˴�δ�õ�
	SRAM_Handler.Init.WaitSignalPolarity=FMC_WAIT_SIGNAL_POLARITY_LOW;//�ȴ��źŵļ���,����ͻ��ģʽ����������
	SRAM_Handler.Init.WaitSignalActive=FMC_WAIT_TIMING_BEFORE_WS;   //�洢�����ڵȴ�����֮ǰ��һ��ʱ�����ڻ��ǵȴ������ڼ�ʹ��NWAIT
	SRAM_Handler.Init.WriteOperation=FMC_WRITE_OPERATION_ENABLE;    //�洢��дʹ��
	SRAM_Handler.Init.WaitSignal=FMC_WAIT_SIGNAL_DISABLE;           //�ȴ�ʹ��λ,�˴�δ�õ�
	SRAM_Handler.Init.ExtendedMode=FMC_EXTENDED_MODE_ENABLE;        //��дʹ�ò�ͬ��ʱ��
	SRAM_Handler.Init.AsynchronousWait=FMC_ASYNCHRONOUS_WAIT_DISABLE;//�Ƿ�ʹ��ͬ������ģʽ�µĵȴ��ź�,�˴�δ�õ�
	SRAM_Handler.Init.WriteBurst=FMC_WRITE_BURST_DISABLE;           //��ֹͻ��д
	SRAM_Handler.Init.ContinuousClock=FMC_CONTINUOUS_CLOCK_SYNC_ASYNC;
	
	//FSMC��ʱ����ƼĴ���
	FSMC_ReadWriteTim.AddressSetupTime=0x0F;        //��ַ����ʱ��(ADDSET)Ϊ15��HCLK 1/180M*15=5.5ns*15=82.5ns
	FSMC_ReadWriteTim.AddressHoldTime=0x00;
	FSMC_ReadWriteTim.DataSetupTime=0x46;           //���ݱ���ʱ��(DATAST)Ϊ70��HCLK	=5.5*70=385ns
	FSMC_ReadWriteTim.AccessMode=FMC_ACCESS_MODE_A; //ģʽA
	//FSMCдʱ����ƼĴ���
	FSMC_WriteTim.AddressSetupTime=0x0F;            //��ַ����ʱ��(ADDSET)Ϊ15��HCLK=82.5ns
	FSMC_WriteTim.AddressHoldTime=0x00;
	FSMC_WriteTim.DataSetupTime=0x0F;               //���ݱ���ʱ��(DATAST)Ϊ5.5ns*15��HCLK=82.5ns
	FSMC_WriteTim.AccessMode=FMC_ACCESS_MODE_A;     //ģʽA
	HAL_SRAM_Init(&SRAM_Handler,&FSMC_ReadWriteTim,&FSMC_WriteTim);		
	delay_ms(50); // delay 50 ms  
	
//	printf(" LCD2 ID:%x\r\n",lcd2dev.id); //��ӡLCD ID   


	lcd2dev.id = 0x1963;
	if(lcd2dev.id==0X1963)
	{
			lcd2_WR_REG(0xE2);		//Set PLL with OSC = 10MHz (hardware),	Multiplier N = 35, 250MHz < VCO < 800MHz = OSC*(N+1), VCO = 300MHz
			lcd2_WR_DATA(0x1D);		//����1 
			lcd2_WR_DATA(0x02);		//����2 Divider M = 2, PLL = 300/(M+1) = 100MHz
			lcd2_WR_DATA(0x04);		//����3 Validate M and N values   
			delay_us(100);
			lcd2_WR_REG(0xE0);		// Start PLL command
			lcd2_WR_DATA(0x01);		// enable PLL
			delay_ms(10);
			lcd2_WR_REG(0xE0);		// Start PLL command again
			lcd2_WR_DATA(0x03);		// now, use PLL output as system clock	
			delay_ms(12);  
			lcd2_WR_REG(0x01);		//��λ
			delay_ms(10);
			
			lcd2_WR_REG(0xE6);		//��������Ƶ��,33Mhz
			lcd2_WR_DATA(0x2F);
			lcd2_WR_DATA(0xFF);
			lcd2_WR_DATA(0xFF);

			lcd2_WR_REG(0xB0);		//����LCDģʽ
			lcd2_WR_DATA(0x20);		//24λģʽ
			lcd2_WR_DATA(0x00);		//TFT ģʽ 
	
			lcd2_WR_DATA((SSD_HOR_RESOLUTION-1)>>8);//����LCDˮƽ����
			lcd2_WR_DATA(SSD_HOR_RESOLUTION-1);		 
			lcd2_WR_DATA((SSD_VER_RESOLUTION-1)>>8);//����LCD��ֱ����
			lcd2_WR_DATA(SSD_VER_RESOLUTION-1);		 
			lcd2_WR_DATA(0x00);		//RGB���� 
			
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
			
			lcd2_WR_REG(0xF0);	//����SSD1963��CPU�ӿ�Ϊ16bit  
			lcd2_WR_DATA(0x03);	//16-bit(565 format) data for 16bpp 

			lcd2_WR_REG(0x29);	//������ʾ
			//����PWM���  ����ͨ��ռ�ձȿɵ� 
			lcd2_WR_REG(0xD0);	//�����Զ���ƽ��DBC
			lcd2_WR_DATA(0x00);	//disable
	
			lcd2_WR_REG(0xBE);	//����PWM���
			lcd2_WR_DATA(0x05);	//1����PWMƵ��
			lcd2_WR_DATA(0xFE);	//2����PWMռ�ձ�
			lcd2_WR_DATA(0x01);	//3����C
			lcd2_WR_DATA(0x00);	//4����D
			lcd2_WR_DATA(0x00);	//5����E 
			lcd2_WR_DATA(0x00);	//6����F 
			
			lcd2_WR_REG(0xB8);	//����GPIO����
			lcd2_WR_DATA(0x03);	//2��IO�����ó����
			lcd2_WR_DATA(0x01);	//GPIOʹ��������IO���� 
			lcd2_WR_REG(0xBA);
			lcd2_WR_DATA(0X01);	//GPIO[1:0]=01,����LCD����
			

				
	}
	if(lcd2dev.id==0X9341||lcd2dev.id==0X5310||lcd2dev.id==0X5510||lcd2dev.id==0X1963)//������⼸��IC,������WRʱ��Ϊ���
	{
		//��������дʱ����ƼĴ�����ʱ��   	 							    
        FSMC_WriteTim.AddressSetupTime=4;      
        FSMC_WriteTim.DataSetupTime=4;         
        FMC_NORSRAM_Extended_Timing_Init(SRAM_Handler.Extended,&FSMC_WriteTim,SRAM_Handler.Init.NSBank,SRAM_Handler.Init.ExtendedMode);
	}   	
	lcd1_Display_Dir(1);		//Ĭ��Ϊ����
	lcd2_Display_Dir(1);	
 
	lcd2_Clear(BLACK);
	lcd1_Clear(BLACK);
	
//	LCD_LED=1;				//��������
	lcd2_BackLightSet(0);
	lcd1_BackLightSet(0);

	
	LCD.SelectScreen(LCD_2);
}  

/********************************************************/
//LCD��������
//color:Ҫ���������ɫ
void lcd1_Clear(u32 color)
{
	if(lcdltdc.pwidth!=0)	//�����RGB��
	{
		LTDC_Clear(color);
	}
}

//LCD2��������
//color:Ҫ���������ɫ
void lcd2_Clear(u32 color)
{
	u32 index=0;      
	u32 totalpoint=lcddev.width;
	
	totalpoint*=lcddev.height; 			//�õ��ܵ���
	lcd2_SetCursor(0x00,0x0000);			//���ù��λ�� 
	lcd2_WriteRAM_Prepare();     		//��ʼд��GRAM	 	  
	for(index=0;index<totalpoint;index++)
	{
		LCD2->LCD2_RAM=color;	
	}
//	memset(lcd2_framebuf,color,800*480);
}
/*********************************************************/

/*********************************************************/
//LCD��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
//**ע��** :FILL�������õ�ʱ���ر��Ǵ�Χ�����ɫ��ʱ����õ���ϵͳ����OS_CRITICAL_ENTER()�����ٽ�������ֹϵͳ���ô�ϳ����������ߺ������
void lcd1_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color)
{          

	LTDC_Fill(sx,sy,ex,ey,color);
	
}  

void lcd2_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color)
{          
	u16 i,j;
	u16 height,width;
	
	width=ex-sx+1; 			//�õ����Ŀ��
	height=ey-sy+1;			//�߶� 
	for(i=0;i<height;i++)
	{
		lcd2_SetCursor(sx,sy+i);      				//���ù��λ�� 
		lcd2_WriteRAM_Prepare();     			//��ʼд��GRAM	  
		for(j=0;j<width;j++)
		{
			LCD2->LCD2_RAM=color;	//��ʾ��ɫ 	    
//			lcd2_framebuf[j][i] = color;
		}
	} 
//	memset(lcd2_framebuf,color,800*480);

	
}
/****************************************************************/


/***************************************************************/
//LCD��ָ�����������ָ����ɫ��			 
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void lcd1_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	if(lcdltdc.pwidth!=0)	//�����RGB��
	{
		LTDC_Color_Fill(sx,sy,ex,ey,color);
	}
}

//LCD2��ָ�����������ָ����ɫ��			 
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void lcd2_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;

	width=ex-sx+1; 			//�õ����Ŀ��
	height=ey-sy+1;			//�߶�
	for(i=0;i<height;i+=2)
	{
//		memcpy(&lcd2_framebuf[sy+i][sx],&color[i*width],width);
		lcd2_SetCursor(sx,sy+i);   	//���ù��λ�� 
		lcd2_WriteRAM_Prepare();     //��ʼд��GRAM
		for(j=0;j<width;j++)
		{
			LCD2->LCD2_RAM=color[i*width+j];//д������ 
//			lcd2_framebuf[j][i] = color[i*width+j];
		}
	}
	for(i=1;i<height;i+=2)
	{
//		memcpy(&lcd2_framebuf[sy+i][sx],&color[i*width],width);
		lcd2_SetCursor(sx,sy+i);   	//���ù��λ�� 
		lcd2_WriteRAM_Prepare();     //��ʼд��GRAM
		for(j=0;j<width;j++)
		{
			LCD2->LCD2_RAM=color[i*width+j];//д������ 
//			lcd2_framebuf[j][i] = color[i*width+j];
		}
	}
	
} 
/**********************************************************************/


/**********************************************************************/
//LCD����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD.DrawPoint(uRow,uCol);//���� 
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
//������	  
//(x1,y1),(x2,y2):���εĶԽ�����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD.DrawLine(x1,y1,x2,y1);
	LCD.DrawLine(x1,y1,x1,y2);
	LCD.DrawLine(x1,y2,x2,y2);
	LCD.DrawLine(x2,y1,x2,y2);
}
/*****************************************************************/


//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
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
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 	

/****************************************************************/
//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24/32
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
  u16 temp,t1,t;
	u16 y0=y;
	u16 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
 	num=num-' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[num][t];	//����1608����
		else if(size==24)temp=asc2_2412[num][t];	//����2412����
		else if(size==32)temp=asc2_3216[num][t];	//����3216����
		else if(size==64)temp=asc2_6432[num][t];//����6432����
		else return;								//û�е��ֿ�
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD.Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD.Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//��������
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//��������
				break;
			}
		}  	 
	}  	    	   	 	  
}   

/****************************************************************/
//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24/32
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowCharEX(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
  u16 temp,t1,t,i;
	u16 y0=y;
	u16 csize=256;		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
	mode = mode;
 	num=num-' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	for(t=0;t<csize;t++)
	{   
		if(size==192)temp=asc2_6432[num][t]; 	 	//����6432����
		else return;								//û�е��ֿ�
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
			if(y>=lcddev.height)return;		//��������
			if((y-y0)==size)
			{
				y=y0;
				x+=3;
				if(x>=lcddev.width)return;	//��������
				break;
			}
		}  	 
	}  	    	   	 	  
}   


//m^n����
//����ֵ:m^n�η�.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
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
//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
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
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}

//��ʾ������ɫ����
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


