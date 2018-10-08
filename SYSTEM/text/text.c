#include "punctuation.h"
#include "sys.h" 
#include "w25qxx.h"
#include "lcd.h"
#include "text.h"	
#include "string.h"												    
#include "usart.h"	
#include "ff.h"
#include "sys_malloc.h"
#include "exfuns.h"
#include "includes.h"


/********************
UNIGBK = 174344  ~175104 = 171kb
24*24  = 487296  ~487424 = 476kb
32*32  = 866304   866304 = 846kb
64*64  = 3465216 3465216 =3384kb

total  = 4993160 4994048 =4877kb
******************/
 
#define UNIGBKaddr 1024*1024*8
#define Font24addr UNIGBKaddr + 1024*171
#define Font32addr Font24addr + 1024*476
#define Font64addr Font32addr + 1024*846

#define UNIGBKsize 174344
#define Font24size 487296
#define Font32size 866304
#define Font64size 3465216

#define upd_SD_Card		"0:/"
#define upd_USB_disk	"3:/"

#define upd_select	upd_USB_disk  //ѡ������;�� Ĭ����U������
 
 
void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size);			//�õ����ֵĵ�����
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode);					//��ָ��λ����ʾһ������
void Show_FontEX(u16 x,u16 y,u8 *font,u16 size,u8 mode);
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u16 size,u8 mode);	//��ָ��λ����ʾһ���ַ��� 
void Show_Str_Mid(u16 x,u16 y,u8*str,u16 size,u16 len);
char *Font_NumToStr(u32 Num);

u8 Font_library_updata_TXT(_font_updata ft);
u8 Font_library_updata_FON(_font_updata ft);
u8 UNIGBK_updata(void);
u8 Font_Check(void);
 
_text_class TEXT = {
 
	.get_hzmat = Get_HzMat,
	.show_font = Show_Font,
	.show_str	 = Show_Str,
	.show_str_mid = Show_Str_Mid,
	.font_library_updata_txt = Font_library_updata_TXT,
	.font_library_updata_fon = Font_library_updata_FON,
	.unigbk_updata = UNIGBK_updata,
	.font_check = Font_Check,
	.NumToStr = Font_NumToStr,
};
 
 
 
 
 
 
 
//code �ַ�ָ�뿪ʼ
//���ֿ��в��ҳ���ģ
//code �ַ����Ŀ�ʼ��ַ,GBK��
//mat  ���ݴ�ŵ�ַ (size/8+((size%8)?1:0))*(size) bytes��С	
//size:�����С
void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size)
{		    
	unsigned char qh,ql;
	u16 i;					  
	unsigned long foffset; 
	u16 csize=(size/8+((size%8)?1:0))*(size);//�õ�����һ���ַ���Ӧ������ռ���ֽ���	 
	qh=*code;
	ql=*(++code);
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//�� ���ú���
	{   		    
	    for(i=0;i<csize;i++)*mat++=0x00;//�������
	    return; //��������
	}   
  if(qh == 0xA1 || qh == 0xA3)
	{
		for(i=0;i<PuncNum;i++)
		{
			if(qh == punc_list[i][0] && ql == punc_list[i][1])
			{
				switch(size)
				{
					case 24:
						memcpy(mat,punc_24[i],72);
						break;
					case 32:
						memcpy(mat,punc_32[i],128);
						break;
					case 64:
						memcpy(mat,punc_64[i],512);
						break;;
					default:
						memset(mat,0,512);
						break;
				}
			}
		}
	}
	else
	{
		qh-=0xB0; 
		ql-=0xA1;
		
		foffset=((unsigned long)(qh * 94)+ql)*csize;	//�õ��ֿ��е��ֽ�ƫ���� 
		switch(size)
		{

			case 24:
				W25QXX_Read(mat,foffset+Font24addr,csize);
				break;
			case 32:
				W25QXX_Read(mat,foffset+Font32addr,csize);
				break;
			case 64:
				W25QXX_Read(mat,foffset+Font64addr,csize);
				break;
				
		}     
	}	
}  
//��ʾһ��ָ����С�ĺ���
//x,y :���ֵ�����
//font:����GBK��
//size:�����С
//mode:0,������ʾ,1,������ʾ	   
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode)
{
	u8 temp,t1;
	u16 t;
	u16 x0 = x;
	u8 dzk[512];   
	u16 csize=(size/8+((size%8)?1:0))*(size);			//�õ�����һ���ַ���Ӧ������ռ���ֽ���	 
	if(size!=24&&size!=32&&size!=64)return;	//��֧�ֵ�size
	Get_HzMat(font,dzk,size);	//�õ���Ӧ��С�ĵ������� 
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];			//�õ��������� 
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LCD.Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD.Fast_DrawPoint(x,y,BACK_COLOR); 
			temp<<=1;

			x++;
			if((x-x0)==size)
			{
				x = x0;
				y++;
				break;
			}
		}  	 
	}  
}

void Show_FontEX(u16 x,u16 y,u8 *font,u16 size,u8 mode)
{
	u8 temp,t1,i,j;
	u16 t;
	u16 x0 = x;
	u8 dzk[512];   
	u8 multi;
	u16 csize=512;			//�õ�����һ���ַ���Ӧ������ռ���ֽ���	 
	if(size!=128&&size!=192&&size!=256)return;	//��֧�ֵ�size
	multi = size/64;
	Get_HzMat(font,dzk,64);	//�õ���Ӧ��С�ĵ������� 
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];			//�õ��������� 
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)
			{
				for(j=0;j<multi;j++)
				{
					for(i=0;i<multi;i++)
					{
						LCD.Fast_DrawPoint(x+i,y+j,POINT_COLOR);
					}
				}
				
			}
			else if(mode==0)LCD.Fast_DrawPoint(x,y,BACK_COLOR); 
			temp<<=1;

			x += multi;
			if((x-x0)==size)
			{
				x = x0;
				y += multi;
				break;
			}
		}  	 
	}  
}



//��ָ��λ�ÿ�ʼ��ʾһ���ַ���	    
//֧���Զ�����
//(x,y):��ʼ����
//width,height:����
//str  :�ַ���
//size :�����С
//mode:0,�ǵ��ӷ�ʽ;1,���ӷ�ʽ    	   		   
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u16 size,u8 mode)
{					
	u16 x0=x;
	u16 y0=y;							  	  
    u8 bHz=0;     //�ַ���������  	    				    				  	  
    while(*str!=0)//����δ����
    { 
        if(!bHz)
        {
	        if(*str>0x80)bHz=1;//���� 
	        else              //�ַ�
	        {      
            if(x>(x0+width-size/2))//����
						{				   
							y+=size;
							x=x0;	   
						}							    
		        if(y>(y0+height-size))break;//Խ�緵��      
		        if(*str==13)//���з���
		        {         
		            y+=size;
								x=x0;
		            str++; 
		        }  
		        else 
						{
							if(size<=64) LCD.ShowChar(x,y,*str,size,mode);//��Ч����д�� 
							else LCD.ShowCharEX(x,y,*str,size,mode);
						}
						str++; 
		        x+=size/2; //�ַ�,Ϊȫ�ֵ�һ�� 
	        }
        }
				else//���� 
        {     
            bHz=0;//�к��ֿ�    
           if(x>(x0+width-size))//����
					{	    
						y+=size;
						x=x0;		  
					}
	        if(y>(y0+height-size))break;//Խ�緵��  		
					if(size <= 64)	Show_Font(x,y,str,size,mode); //��ʾ�������,������ʾ 
	        else if(size > 64 && size <= 256) Show_FontEX(x,y,str,size,mode);
	        str+=2; 
	        x+=size;//��һ������ƫ��	    
        }						 
    }   
}  			 		 
//��ָ����ȵ��м���ʾ�ַ���
//����ַ����ȳ�����len,����Show_Str��ʾ
//len:ָ��Ҫ��ʾ�Ŀ��			  
void Show_Str_Mid(u16 x,u16 y,u8*str,u16 size,u16 len)
{
	u16 strlenth=0;
   	strlenth=strlen((const char*)str);
	strlenth*=size/2;
	if(strlenth>len)Show_Str(x,y,LCD.width,LCD.height,str,size,1);
	else
	{
		strlenth=(len-strlenth)/2;
	    Show_Str(strlenth+x,y,LCD.width,LCD.height,str,size,1);
	}
}   


u8 Font_library_updata_TXT(_font_updata ft)
{
	u8 res = 0;
	u8 *buf,*ft_buf; 
	u8 *ft_path;
	u8 *ft_name;
	u8 percent = 0;
	
	u16 t;
	
	u32 tlen=0;
	u32 ft_size=0;
	u32 ft_addr=0;

	FIL *ft_file;
	UINT ft_br;

	POINT_COLOR = WHITE;
	LCD.SelectScreen(LCD_1);
	LCD.Clear(BLACK);
	Show_Str_Mid(360,280,"Font.txt library updataing!!",32,255);
	
	LCD.SelectScreen(LCD_2);
	LCD.Clear(BLACK);
	Show_Str_Mid(280,190,"Font.txt library updataing!!",24,255);
	LCD.Fill(200,260,600,265,GRAY);
	
	if(upd_select == upd_SD_Card)
	{
		res = f_mount(fs[0],"0:",1); 		 
		if(res) 
		{	
			printf("There if no SD card : %d \r\n ",res);
			return 0;
		}
	}
	else if(upd_select == upd_USB_disk)
	{
		res = f_mount(fs[3],"3:",1); 
		if(res)
		{			
			printf("There if no USB disk : %d \r\n ",res);
			return 0;
		}
	}
	else return 0;
	
//	buf 	 = (u8 *)mymalloc(SRAMIN,256);
//	ft_buf = (u8 *)mymalloc(SRAMIN,512);
//	ft_file= (FIL*)mymalloc(SRAMIN,sizeof(FIL));
//	ft_path= mymalloc(SRAMIN,50);
//	ft_name= mymalloc(SRAMIN,20);
	SYS_MALLOC(buf,SRAMIN,256);
	SYS_MALLOC(ft_buf,SRAMIN,512);
	SYS_MALLOC(ft_file,SRAMIN,sizeof(FIL));
	SYS_MALLOC(ft_path,SRAMIN,50);
	SYS_MALLOC(ft_name,SRAMIN,20);
	

	if(!buf || !ft_buf || !ft_file || !ft_path || !ft_name) 
	{
		printf("malloc fail\r\n");
		return 0;
	}
	
	strcpy((char*)ft_path,upd_select);
	strcat((char*)ft_path,"FONT_TXT/");
	
	switch(ft)
	{
		case Font24:	
		  strcpy((char*)ft_name,"24X24.txt");
			ft_addr = Font24addr;
			ft_size = Font24size;
			break;
		case Font32:
			strcpy((char*)ft_name,"32X32.txt");
			ft_addr = Font32addr;
			ft_size = Font32size;
			break;
		case Font64:
			strcpy((char*)ft_name,"64X64.txt");
			ft_addr = Font64addr;
			ft_size = Font64size;
			break;
	}

	Show_Str_Mid(350,150,ft_name,24,1);
	strcat((char*)ft_path,(const char*)ft_name);
	res=f_open(ft_file,(const TCHAR*)ft_path,FA_READ);//���ļ���
	if(res)
	{
		printf("Open %s file fail %d \r\n",ft_path,res);
		return 0;
	}
	
	while(!res)
	{

		res = f_read(ft_file,ft_buf,512,&ft_br);
		if(ft_br == 0) 
		{
//			myfree(SRAMIN,buf);
//			myfree(SRAMIN,ft_buf);
//			myfree(SRAMIN,ft_file);
//			myfree(SRAMIN,ft_path);
//			myfree(SRAMIN,ft_name);
//			buf = NULL;
//			ft_buf = NULL;
//			ft_file = NULL;
//			ft_path = NULL;
//			ft_name = NULL;
			SYS_FREE(buf,SRAMIN);
			SYS_FREE(ft_buf,SRAMIN);
			SYS_FREE(ft_file,SRAMIN);
			SYS_FREE(ft_path,SRAMIN);
			SYS_FREE(ft_name,SRAMIN);

			return 1;
		}
		
		if(!res)
		{
			for(t= 0;t < ft_br;t += 2)
			{
				if(ft_buf[t] >= '0' && ft_buf[t] <= '9') buf[t/2] |= ((ft_buf[t] - 0x30)<<4);
				else if(ft_buf[t] >= 'a' && ft_buf[t] <= 'f') buf[t/2] |= ((ft_buf[t] - 0x57)<<4);
				
				if(ft_buf[t+1] >= '0' && ft_buf[t+1] <= '9') buf[t/2] |= (ft_buf[t+1] - 0x30);
				else if(ft_buf[t+1] >= 'a' && ft_buf[t+1] <= 'f') buf[t/2] |= (ft_buf[t+1] - 0x57);
			}
			
			W25QXX_Write(buf,ft_addr+tlen,ft_br/2);
			tlen+=(ft_br/2);
			if(percent != tlen*100/ft_size)
			{
				percent = tlen*100/ft_size;
//				LCD.Fill(380,220,450,250,BLACK);
				POINT_COLOR = BLACK;
				LCD.ShowNum(390,220,percent-1,3,24);
				POINT_COLOR = WHITE;
				LCD.ShowNum(390,220,percent,3,24);
				LCD.ShowChar(430,220,'%',24,1);
				LCD.Fill(200+(percent-1)*4,260,200+percent*4,265,WHITE);
				
			}
//			printf("%d%% \r\n",tlen*100/ft_size);
			memset(buf,0,256);
		}
	}
	
//	myfree(SRAMIN,buf);
//	myfree(SRAMIN,ft_buf);
//	myfree(SRAMIN,ft_file);
//	myfree(SRAMIN,ft_path);
//	myfree(SRAMIN,ft_name);
//	buf = NULL;
//	ft_buf = NULL;
//	ft_file = NULL;
//	ft_path = NULL;
//	ft_name = NULL;
			SYS_FREE(buf,SRAMIN);
			SYS_FREE(ft_buf,SRAMIN);
			SYS_FREE(ft_file,SRAMIN);
			SYS_FREE(ft_path,SRAMIN);
			SYS_FREE(ft_name,SRAMIN);
	
	return 1;

}


u8 Font_library_updata_FON(_font_updata ft)
{
	u8 res = 0;
	u8 *ft_buf; 
	u8 *ft_path;
	u8 *ft_name;
	u8 percent = 0;
	
//	u16 t;
	
	u32 tlen=0;
	u32 ft_size=0;
	u32 ft_addr=0;

	FIL *ft_file;
	UINT ft_br;

	POINT_COLOR = WHITE;
	LCD.SelectScreen(LCD_1);
	LCD.Clear(BLACK);
	Show_Str_Mid(360,280,"Font.fon library updataing!!",32,255);
	
	LCD.SelectScreen(LCD_2);
	LCD.Clear(BLACK);
	Show_Str_Mid(280,190,"Font.fon library updataing!!",24,255);
	LCD.Fill(200,260,600,265,GRAY);
	
	if(upd_select == upd_SD_Card)
	{
		res = f_mount(fs[0],"0:",1); 		 
		if(res) 
		{	
			printf("There if no SD card : %d \r\n ",res);
			return 0;
		}
	}
	else if(upd_select == upd_USB_disk)
	{
		res = f_mount(fs[3],"3:",1); 
		if(res)
		{			
			printf("There if no USB disk : %d \r\n ",res);
			return 0;
		}
	}
	else return 0;
	
//	buf 	 = (u8 *)mymalloc(SRAMIN,256);
//	ft_buf = (u8 *)mymalloc(SRAMIN,512);
//	ft_file= (FIL*)mymalloc(SRAMIN,sizeof(FIL));
//	ft_path= mymalloc(SRAMIN,50);
//	ft_name= mymalloc(SRAMIN,20);
//	SYS_MALLOC(buf,SRAMIN,256);
	SYS_MALLOC(ft_buf,SRAMIN,512);
	SYS_MALLOC(ft_file,SRAMIN,sizeof(FIL));
	SYS_MALLOC(ft_path,SRAMIN,50);
	SYS_MALLOC(ft_name,SRAMIN,20);
	

	if(!ft_buf || !ft_file || !ft_path || !ft_name) 
	{
		printf("malloc fail\r\n");
		return 0;
	}
	
	strcpy((char*)ft_path,upd_select);
	strcat((char*)ft_path,"FONT/");
	
	switch(ft)
	{
		case Font24:	
		  strcpy((char*)ft_name,"24X24.FON");
			ft_addr = Font24addr;
			ft_size = Font24size;
			break;
		case Font32:
			strcpy((char*)ft_name,"32X32.FON");
			ft_addr = Font32addr;
			ft_size = Font32size;
			break;
		case Font64:
			strcpy((char*)ft_name,"64X64.FON");
			ft_addr = Font64addr;
			ft_size = Font64size;
			break;
	}

	Show_Str_Mid(350,150,ft_name,24,1);
	strcat((char*)ft_path,(const char*)ft_name);
	res=f_open(ft_file,(const TCHAR*)ft_path,FA_READ);//���ļ���
	if(res)
	{
		printf("Open %s file fail %d \r\n",ft_path,res);
		return 0;
	}
	
	while(!res)
	{

		res = f_read(ft_file,ft_buf,512,&ft_br);
		if(ft_br == 0) 
		{

//			SYS_FREE(buf,SRAMIN);
			SYS_FREE(ft_buf,SRAMIN);
			SYS_FREE(ft_file,SRAMIN);
			SYS_FREE(ft_path,SRAMIN);
			SYS_FREE(ft_name,SRAMIN);

			return 1;
		}
		
		if(!res)
		{
			
			W25QXX_Write(ft_buf,ft_addr+tlen,ft_br);
			tlen+=ft_br;
			if(percent != tlen*100/ft_size)
			{
				percent = tlen*100/ft_size;
//				LCD.Fill(380,220,450,250,BLACK);
				POINT_COLOR = BLACK;
				LCD.ShowNum(390,220,percent-1,3,24);
				POINT_COLOR = WHITE;
				LCD.ShowNum(390,220,percent,3,24);
				LCD.ShowChar(430,220,'%',24,1);
				LCD.Fill(200+(percent-1)*4,260,200+percent*4,265,WHITE);
				
			}
//			printf("%d%% \r\n",tlen*100/ft_size);
			memset(ft_buf,0,512);

		}
	}

//		SYS_FREE(buf,SRAMIN);
			SYS_FREE(ft_buf,SRAMIN);
			SYS_FREE(ft_file,SRAMIN);
			SYS_FREE(ft_path,SRAMIN);
			SYS_FREE(ft_name,SRAMIN);
	
	return 1;

}


u8 UNIGBK_updata(void)
{
	u8 res = 0;
	u8 *ft_buf; 
	u8 *ft_path;
	u8 percent = 0;
	
	u32 tlen=0;


	FIL *ft_file;
	UINT ft_br;

	POINT_COLOR = WHITE;
	LCD.SelectScreen(LCD_1);
	LCD.Clear(BLACK);
	Show_Str_Mid(360,280,"UNIGBK updataing!!",32,255);
	
	LCD.SelectScreen(LCD_2);
	LCD.Clear(BLACK);
	Show_Str_Mid(280,190,"UNIGBK updataing!!",24,255);
	LCD.Fill(200,260,600,265,GRAY);
	
//	res = f_mount(fs[0],"0:",1); 		 
//	if(res) printf("There if no SD card : %d \r\n ",res);
	
	if(upd_select == upd_SD_Card)
	{
		res = f_mount(fs[0],"0:",1); 		 
		if(res) 
		{	
			printf("There if no SD card : %d \r\n ",res);
			return 0;
		}
	}
	else if(upd_select == upd_USB_disk)
	{
		res = f_mount(fs[3],"3:",1); 
		if(res)
		{			
			printf("There if no USB disk : %d \r\n ",res);
			return 0;
		}
	}
	else return 0;
	
//	ft_buf = (u8 *)mymalloc(SRAMIN,512);
//	ft_file= (FIL*)mymalloc(SRAMIN,sizeof(FIL));
//	ft_path= mymalloc(SRAMIN,50);
	SYS_MALLOC(ft_buf,SRAMIN,512);
	SYS_MALLOC(ft_file,SRAMIN,sizeof(FIL));
	SYS_MALLOC(ft_path,SRAMIN,50);


	if( !ft_buf || !ft_file || !ft_path) 
	{
		printf("malloc fail\r\n");
		return 0;
	}
	
	strcpy((char*)ft_path,upd_select);
	strcat((char*)ft_path,"FONT/UNIGBK.BIN");


	res=f_open(ft_file,(const TCHAR*)ft_path,FA_READ);//���ļ���
	if(res)
	{
		printf("Open %s file fail %d \r\n",ft_path,res);
		return 0;
	}
	
	while(!res)
	{

		res = f_read(ft_file,ft_buf,512,&ft_br);
		if(ft_br == 0) 
		{
			SYS_FREE(ft_buf,SRAMIN);
			SYS_FREE(ft_file,SRAMIN);
			SYS_FREE(ft_path,SRAMIN);

			return 1;
		}
		
		if(!res)
		{
			W25QXX_Write(ft_buf,UNIGBKaddr+tlen,ft_br);
			tlen+=(ft_br);
			if(percent != tlen*100/UNIGBKsize)
			{
				percent = tlen*100/UNIGBKsize;
//				LCD.Fill(390,220,430,250,BLACK);
				POINT_COLOR = BLACK;
				LCD.ShowNum(390,220,percent-1,3,24);
				POINT_COLOR = WHITE;
				LCD.ShowNum(390,220,percent,3,24);
				LCD.ShowChar(430,220,'%',24,1);
				LCD.Fill(200+(percent-1)*4,260,200+percent*4,265,WHITE);
				
			}

			memset(ft_buf,0,512);
		}
	}
	

//	myfree(SRAMIN,ft_buf);
//	myfree(SRAMIN,ft_file);
//	myfree(SRAMIN,ft_path);
//	ft_buf = NULL;
//	ft_file = NULL;
//	ft_path = NULL;
			SYS_FREE(ft_buf,SRAMIN);
			SYS_FREE(ft_file,SRAMIN);
			SYS_FREE(ft_path,SRAMIN);


	return 1;
}


u8 Font_Check(void)
{
	u8 res;
	
	W25QXX_Read(&res,FontUDFlag_addr,1);
	
	if(res != FontUDFlag)
	{
		printf("Font Check error:read = %d\r\n",res);
		res = 1;
	}
	else res = 0;
	
	return res;
}

char *Font_NumToStr(u32 Num)
{
	static char Str[10] = {'\0'};
	
	sprintf(Str,"%d",Num);
	
	return Str;
}


















		  






