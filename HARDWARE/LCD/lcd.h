#ifndef __LCD_H
#define __LCD_H		
#include "sys.h"	 
#include "stdlib.h" 


//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A18作为数据命令区分线 
//注意设置时STM32内部会右移一位对其!  			    
#define LCD2_BASE        ((u32)(0x60000000 | 0x0007FFFE))
#define LCD2             ((LCD_TypeDef *) LCD2_BASE)
//////////////////////////////////////////////////////////////////////////////////
	 
//LCD2分辨率设置
#define SSD_HOR_RESOLUTION		800		//LCD水平分辨率
#define SSD_VER_RESOLUTION		480		//LCD垂直分辨率
//LCD2驱动参数设置
#define SSD_HOR_PULSE_WIDTH		1		//水平脉宽
#define SSD_HOR_BACK_PORCH		46		//水平前廊
#define SSD_HOR_FRONT_PORCH		210		//水平后廊

#define SSD_VER_PULSE_WIDTH		1		//垂直脉宽
#define SSD_VER_BACK_PORCH		23		//垂直前廊
#define SSD_VER_FRONT_PORCH		22		//垂直前廊

#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)


//扫描方向定义
#define L2R_U2D  0 		//从左到右,从上到下
#define L2R_D2U  1 		//从左到右,从下到上
#define R2L_U2D  2 		//从右到左,从上到下
#define R2L_D2U  3 		//从右到左,从下到上

#define U2D_L2R  4 		//从上到下,从左到右
#define U2D_R2L  5 		//从上到下,从右到左
#define D2U_L2R  6 		//从下到上,从左到右
#define D2U_R2L  7		//从下到上,从右到左	 

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         		 0x001F  
#define BRED             0XF81F
#define GRED 			 			 0XFFE0
#define GBLUE			 			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 					 0XBC40 //棕色
#define BRRED 					 0XFC07 //棕红色
#define GRAY  					 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 		 0XC618 //浅灰色
#define MGRAY						 0xA534 //中灰色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

//LCD背光	PB5	
#define LCD_LED     PBout(5)  


//LCD重要参数集
typedef struct  
{		 	 
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	wramcmd;		//开始写gram指令
	u16 setxcmd;		//设置x坐标指令
	u16 setycmd;		//设置y坐标指令 
}_lcd_dev; 	 

typedef enum 
{
	LCD_1 = 0,//LCD_1 = 1024*600 高清屏幕
	LCD_2 = 1,//LCD_2 = 800*480  标清屏幕
}_lcd_select;


typedef enum
{
	wifi_1=1,
	wifi_2,
	wifi_3,
	wifi_4,
	bat_1,
	bat_2,
	link,
}_microicon;


//LCD功能类
typedef struct  
{		 	
	_lcd_select LCD_Sel;
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	wramcmd;		//开始写gram指令
	u16 setxcmd;		//设置x坐标指令
	u16 setycmd;		//设置y坐标指令 
	
	void (*initialization)(void);
	void (*SelectScreen)(_lcd_select );
	
	void (*DisplayOn)(void);												
	void (*DisplayOff)(void);													
	void (*Clear)(u32);	 												
	void (*DrawPoint)(u16 ,u16 );										
	u32  (*ReadPoint)(u16 ,u16 ); 
//	u32  (*FastReadPoint)(u16 ,u16 ); 	
	void (*Fast_DrawPoint)(u16 ,u16 ,u32 );								
	void (*Color_Fill)(u16 ,u16 ,u16 ,u16 ,u16*);		
	void (*Display_Dir)(u8 );	
	void (*Fill)(u16 ,u16 ,u16 ,u16 ,u32 );
	
	void (*WriteReg)(u16 , u16 );
	u16  (*ReadReg)(u16 );
	void (*WriteRAM_Prepare)(void);
	void (*WriteRAM)(u16 );
	void (*BackLightSet)(u8 );						
	void (*Scan_Dir)(u8 );								
	void (*Set_Window)(u16 ,u16 ,u16 ,u16 );	
	void (*SetCursor)(u16 , u16 );	
	
	void (*Draw_Circle)(u16 ,u16 ,u8 );						 			//画圆
	void (*DrawLine)(u16 , u16 , u16 , u16 );							//画线
	void (*DrawRectangle)(u16 , u16 , u16 , u16 );	   				//画矩形

	void (*ShowChar)(u16 ,u16 ,u8 ,u8 ,u8 );						//显示一个字符
	void (*ShowCharEX)(u16 ,u16 ,u8 ,u8 ,u8 );						//显示一个字符
	void (*ShowNum)(u16 ,u16 ,u32 ,u8 ,u8 );  						//显示一个数字
	void (*ShowxNum)(u16 ,u16 ,u32 ,u8 ,u8 ,u8 );				//显示 数字
	void (*ShowString)(u16 ,u16 ,u16 ,u16 ,u8 ,u8 *);		//显示一个字符串,12/16字体
	void (*Darker)(u16,u16,u16,u16,u8);
	void (*MicroIcon)(u16 ,u16 ,_microicon );
	
}_lcd_class; 

extern _lcd_class LCD;
//LCD参数
//extern _lcd_dev lcddev,lcd2dev;	//管理LCD重要参数
//LCD的画笔颜色和背景色	   
extern vu32  POINT_COLOR;//默认红色    
extern u32  BACK_COLOR; //背景颜色.默认为白色

 	    
//LCD2地址结构体
typedef struct
{
	vu16 LCD2_REG;
	vu16 LCD2_RAM;
} LCD_TypeDef;

	    															  


			   						   																			 


#endif  
	 
	 



