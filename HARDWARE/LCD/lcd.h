#ifndef __LCD_H
#define __LCD_H		
#include "sys.h"	 
#include "stdlib.h" 


//ʹ��NOR/SRAM�� Bank1.sector4,��ַλHADDR[27,26]=11 A18��Ϊ�������������� 
//ע������ʱSTM32�ڲ�������һλ����!  			    
#define LCD2_BASE        ((u32)(0x60000000 | 0x0007FFFE))
#define LCD2             ((LCD_TypeDef *) LCD2_BASE)
//////////////////////////////////////////////////////////////////////////////////
	 
//LCD2�ֱ�������
#define SSD_HOR_RESOLUTION		800		//LCDˮƽ�ֱ���
#define SSD_VER_RESOLUTION		480		//LCD��ֱ�ֱ���
//LCD2������������
#define SSD_HOR_PULSE_WIDTH		1		//ˮƽ����
#define SSD_HOR_BACK_PORCH		46		//ˮƽǰ��
#define SSD_HOR_FRONT_PORCH		210		//ˮƽ����

#define SSD_VER_PULSE_WIDTH		1		//��ֱ����
#define SSD_VER_BACK_PORCH		23		//��ֱǰ��
#define SSD_VER_FRONT_PORCH		22		//��ֱǰ��

#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)


//ɨ�跽����
#define L2R_U2D  0 		//������,���ϵ���
#define L2R_D2U  1 		//������,���µ���
#define R2L_U2D  2 		//���ҵ���,���ϵ���
#define R2L_D2U  3 		//���ҵ���,���µ���

#define U2D_L2R  4 		//���ϵ���,������
#define U2D_R2L  5 		//���ϵ���,���ҵ���
#define D2U_L2R  6 		//���µ���,������
#define D2U_R2L  7		//���µ���,���ҵ���	 

#define DFT_SCAN_DIR  L2R_U2D  //Ĭ�ϵ�ɨ�跽��

//������ɫ
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
#define BROWN 					 0XBC40 //��ɫ
#define BRRED 					 0XFC07 //�غ�ɫ
#define GRAY  					 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 		 0XC618 //ǳ��ɫ
#define MGRAY						 0xA534 //�л�ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

//LCD����	PB5	
#define LCD_LED     PBout(5)  


//LCD��Ҫ������
typedef struct  
{		 	 
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID
	u8  dir;			//���������������ƣ�0��������1��������	
	u16	wramcmd;		//��ʼдgramָ��
	u16 setxcmd;		//����x����ָ��
	u16 setycmd;		//����y����ָ�� 
}_lcd_dev; 	 

typedef enum 
{
	LCD_1 = 0,//LCD_1 = 1024*600 ������Ļ
	LCD_2 = 1,//LCD_2 = 800*480  ������Ļ
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


//LCD������
typedef struct  
{		 	
	_lcd_select LCD_Sel;
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID
	u8  dir;			//���������������ƣ�0��������1��������	
	u16	wramcmd;		//��ʼдgramָ��
	u16 setxcmd;		//����x����ָ��
	u16 setycmd;		//����y����ָ�� 
	
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
	
	void (*Draw_Circle)(u16 ,u16 ,u8 );						 			//��Բ
	void (*DrawLine)(u16 , u16 , u16 , u16 );							//����
	void (*DrawRectangle)(u16 , u16 , u16 , u16 );	   				//������

	void (*ShowChar)(u16 ,u16 ,u8 ,u8 ,u8 );						//��ʾһ���ַ�
	void (*ShowCharEX)(u16 ,u16 ,u8 ,u8 ,u8 );						//��ʾһ���ַ�
	void (*ShowNum)(u16 ,u16 ,u32 ,u8 ,u8 );  						//��ʾһ������
	void (*ShowxNum)(u16 ,u16 ,u32 ,u8 ,u8 ,u8 );				//��ʾ ����
	void (*ShowString)(u16 ,u16 ,u16 ,u16 ,u8 ,u8 *);		//��ʾһ���ַ���,12/16����
	void (*Darker)(u16,u16,u16,u16,u8);
	void (*MicroIcon)(u16 ,u16 ,_microicon );
	
}_lcd_class; 

extern _lcd_class LCD;
//LCD����
//extern _lcd_dev lcddev,lcd2dev;	//����LCD��Ҫ����
//LCD�Ļ�����ɫ�ͱ���ɫ	   
extern vu32  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern u32  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ

 	    
//LCD2��ַ�ṹ��
typedef struct
{
	vu16 LCD2_REG;
	vu16 LCD2_RAM;
} LCD_TypeDef;

	    															  


			   						   																			 


#endif  
	 
	 



