#include "sys.h"
#include "usart.h"
#include "pymb.h"
#include "string.h"
#include "lcd.h"
#include "text.h"

#include "pyinput.h"

//数字表
const u8* kbd_tbl[12]={"1","2","3","<-","4","5","6","","7","8","9","0"};
//字符表
const u8* kbs_tbl[12]={",.?","ABC","DEF","del","GHI","JKL","MNO","./clr","PQRS","TUV","WXYZ","_"};

const u8* kbp_tbl[2] ={"-","+"};

const u8* kbc_tbl[2] ={"确定","取消"};

const u8* kbm_tbl[2] = {"CN","EN"};


//输入法功能
_t9_class T9 = {
	.KeyPress = 0,
	.inputlen = 0,
	.result_num = 0,
	.resultword_num = 0,
	.WordOrdinal = 0,
	.WordPage = 0,
	
	.getpymb = get_pymb,	//字符串到码表获取函数
};

void en_num_get_word(u8 key)
{
	switch(key)
	{
		case 1:
		{
			T9.input_en_num[0] = '.';
			T9.input_en_num[1] = ',';
			T9.input_en_num[2] = '?';
			T9.input_en_num[3] = '!';
			T9.input_en_num[4] = '_';
			T9.input_en_num[5] = '-';
			T9.input_en_num[6] = '1';
			T9.input_en_num[7] = '\0';
		}break;
		case 2:
		{
			T9.input_en_num[0] = 'A';
			T9.input_en_num[1] = 'B';
			T9.input_en_num[2] = 'C';
			T9.input_en_num[3] = 'a';
			T9.input_en_num[4] = 'b';
			T9.input_en_num[5] = 'c';
			T9.input_en_num[6] = '2';
			T9.input_en_num[7] = '\0';
		}break;
		case 3:
		{
			T9.input_en_num[0] = 'D';
			T9.input_en_num[1] = 'E';
			T9.input_en_num[2] = 'F';
			T9.input_en_num[3] = 'd';
			T9.input_en_num[4] = 'e';
			T9.input_en_num[5] = 'f';
			T9.input_en_num[6] = '3';
			T9.input_en_num[7] = '\0';
		}break;
		case 4:
		{
			memset(T9.input_en_num,'\0',10);
		}break;
		case 5:
		{
			T9.input_en_num[0] = 'G';
			T9.input_en_num[1] = 'H';
			T9.input_en_num[2] = 'I';
			T9.input_en_num[3] = 'g';
			T9.input_en_num[4] = 'h';
			T9.input_en_num[5] = 'i';
			T9.input_en_num[6] = '4';
			T9.input_en_num[7] = '\0';
		}break;
		case 6:
		{
			T9.input_en_num[0] = 'J';
			T9.input_en_num[1] = 'K';
			T9.input_en_num[2] = 'L';
			T9.input_en_num[3] = 'j';
			T9.input_en_num[4] = 'k';
			T9.input_en_num[5] = 'l';
			T9.input_en_num[6] = '5';
			T9.input_en_num[7] = '\0';
		}break;
		case 7:
		{
			T9.input_en_num[0] = 'M';
			T9.input_en_num[1] = 'N';
			T9.input_en_num[2] = 'O';
			T9.input_en_num[3] = 'm';
			T9.input_en_num[4] = 'n';
			T9.input_en_num[5] = 'o';
			T9.input_en_num[6] = '6';
			T9.input_en_num[7] = '\0';
		}break;
		case 8:break;
		case 9:
		{
			T9.input_en_num[0] = 'P';
			T9.input_en_num[1] = 'Q';
			T9.input_en_num[2] = 'R';
			T9.input_en_num[3] = 'S';
			T9.input_en_num[4] = 'p';
			T9.input_en_num[5] = 'q';
			T9.input_en_num[6] = 'r';
			T9.input_en_num[7] = 's';
			T9.input_en_num[8] = '7';
			T9.input_en_num[9] = '\0';
		}break;
		case 10:
		{
			T9.input_en_num[0] = 'T';
			T9.input_en_num[1] = 'U';
			T9.input_en_num[2] = 'V';
			T9.input_en_num[3] = 't';
			T9.input_en_num[4] = 'u';
			T9.input_en_num[5] = 'v';
			T9.input_en_num[6] = '8';
			T9.input_en_num[7] = '\0';
		}break;
		case 11:
		{
			T9.input_en_num[0] = 'W';
			T9.input_en_num[1] = 'X';
			T9.input_en_num[2] = 'Y';
			T9.input_en_num[3] = 'Z';
			T9.input_en_num[4] = 'w';
			T9.input_en_num[5] = 'x';
			T9.input_en_num[6] = 'y';
			T9.input_en_num[7] = 'z';
			T9.input_en_num[8] = '9';
			T9.input_en_num[9] = '\0';
		}break;
		case 12:break;
	}
	T9.result_num = 1;
}
void py_get_word(u8 key)
{
//	u8 i;
//	if(key == 1)
//	{
//		T9.WordOrdinal = 0;
//		T9.pymb[T9.WordOrdinal]->pymb[0] = '.';
//		T9.pymb[T9.WordOrdinal]->pymb[1] = ',';
//		T9.pymb[T9.WordOrdinal]->pymb[2] = '?';
//		T9.pymb[T9.WordOrdinal]->pymb[3] = '!';
//		T9.pymb[T9.WordOrdinal]->pymb[4] = '_';
//		T9.pymb[T9.WordOrdinal]->pymb[5] = '-';
//		T9.pymb[T9.WordOrdinal]->pymb[6] = '1';
//		T9.result_num = 1;
//		T9.inputstr[0]==NULL;
//	}
	
	if(key >= 2 && key <= 3)
	{
		T9.inputstr[T9.inputlen] = key + '0';
		if(T9.inputlen<7) T9.inputlen++;
	}
	else if(key >= 5 && key <= 7)
	{
		T9.inputstr[T9.inputlen] = key-1 + '0';
		if(T9.inputlen<7) T9.inputlen++;
	}
	else if(key >= 9 && key <= 11)
	{
		T9.inputstr[T9.inputlen] = key-2 + '0';
		if(T9.inputlen<7) T9.inputlen++;
	}
	else if(key == 4)
	{
		if(T9.inputlen)T9.inputlen--;
		T9.inputstr[T9.inputlen]='\0';//添加结束符
	}
	else if(key == 8)
	{
		if(T9.inputlen) T9.inputlen = 0;;
		memset(T9.inputstr,'\0',7);
	}
	if(T9.inputstr[0]!=NULL)
	{
		key=T9.getpymb(T9.inputstr);	//得到匹配的结果数
		if(key)//有部分匹配/完全匹配的结果
		{
			T9.WordOrdinal = 0;
			if(key&0X80)		   	//是部分匹配
			{
				T9.inputlen=key&0X7F;	//有效匹配位数
				T9.pymb[0]->py[T9.inputlen] = '\0';
//				T9.inputstr[T9.inputlen]='\0';//不匹配的位数去掉
//				if(T9.inputlen>1)
//				{
//					key = T9.getpymb(T9.inputstr);//重新获取完全匹配字符数
//					T9.result_num = key&0X7F;	
//					
//				}
				T9.result_num = 1;
			} 
			else							//全部匹配
			{
							T9.result_num=key&0X7F;	//总匹配结果
			}			
		}
		else 						//没有任何匹配
		{				   	
			T9.inputlen--;
			T9.inputstr[T9.inputlen]='\0';
		}
	}
	else
	{
//		cur_index=0;
		T9.result_num=0;

	}
			
//	for(i = 1;i<=T9.result_num;i++)
//	{
//		py_show_result(i);
//	}
}


//显示结果.
//index:0,表示没有一个匹配的结果.清空之前的显示
//   其他,索引号	
void py_show_result(u8 index)
{  
	if(index)
	{
		printf("\r\n拼音:%s\r\n",T9.pymb[index-1]->py);	//串口输出拼音
		printf("结果:%s\r\n",T9.pymb[index-1]->pymb);	//串口输出结果
	}
}	

//加载键盘界面
void py_load_ui(u8 mode)
{
	u8 i,j;


	if(mode == 0)
	{
		LCD.Fill(0,280,800-1,310-1,WHITE);
		LCD.Fill(0,310,800-1,480-1,LGRAY);
		POINT_COLOR=GRAY;
		LCD.DrawLine(0,311,800-1,311);
		
		for(j=0;j<3;j++)
		{
			for(i = 0;i<4;i++)
			{
	//			if(j == 3 && (i == 0 || i == 2)) continue;
				LCD.Fill(KB_MstartX+i*(KB_MsizeX+KB_MspacX),KB_MstartY+j*KB_MsizeY+(j+1)*KB_MspacY,KB_MstartX+(i+1)*(KB_MsizeX+KB_MspacX)-KB_MspacX,KB_MstartY+(j+1)*(KB_MsizeY+KB_MspacY),WHITE);
			}
		}

		
		POINT_COLOR=BLACK;
		for(j=0;j<3;j++)
		{
			for(i=0;i<4;i++)
			{
		//		if(i == 9) continue;
				TEXT.show_str_mid(KB_MstartX+i*(KB_MsizeX+KB_MspacX),KB_MstartY+4+(KB_MsizeY+KB_MspacY)*j,(u8*)kbd_tbl[4*j+i],16,KB_MsizeX);		
				TEXT.show_str_mid(KB_MstartX+i*(KB_MsizeX+KB_MspacX),KB_MstartY+(KB_MsizeY+KB_MspacY)/2+(KB_MsizeY+KB_MspacY)*j-3,(u8*)kbs_tbl[4*j+i],24,KB_MsizeX);	
			}			
		}
		
		LCD.Fill(715,313,790,353,WHITE);
		LCD.Fill(715,356,790,426,WHITE);
		LCD.Fill(715,429,790,469,WHITE);
		TEXT.show_str_mid(715,379,(u8*)kbc_tbl[0],24,75);	
		TEXT.show_str_mid(715,437,(u8*)kbc_tbl[1],24,75);	
		
		TEXT.show_str_mid(715,315,(u8*)kbm_tbl[T9.InpMode],24,50);
		POINT_COLOR=MGRAY;		
		TEXT.show_str_mid(765,335,(u8*)kbm_tbl[!T9.InpMode],16,25);	
		POINT_COLOR=BLACK;
	}
	if(mode == 0 || mode == 1)
	{
		LCD.Fill(0,280,800-1,310-1,WHITE);
		LCD.Fill(0,312,214,480-1,LGRAY);
		POINT_COLOR=MGRAY;
		LCD.DrawLine(KB_LstartX+10,KB_LstartY+KB_LsizeY,KB_LstartX+3*KB_LsizeX-10,KB_LstartY+KB_LsizeY);
		LCD.DrawLine(KB_LstartX+10,KB_LstartY+2*KB_LsizeY,KB_LstartX+3*KB_LsizeX-10,KB_LstartY+2*KB_LsizeY);
		LCD.DrawLine(KB_LstartX+KB_LsizeX,KB_LstartY+10,KB_LstartX+KB_LsizeX,KB_LstartY+3*KB_LsizeY);
		LCD.DrawLine(KB_LstartX+2*KB_LsizeX,KB_LstartY+10,KB_LstartX+2*KB_LsizeX,KB_LstartY+3*KB_LsizeY);

		
		LCD.DrawLine(650,280,650,310);
		LCD.DrawLine(720,280,720,310);
		TEXT.show_str_mid(650,280,(u8*)kbp_tbl[0],24,70);
		TEXT.show_str_mid(650+70,280,(u8*)kbp_tbl[1],24,70);
	}
	
	if(mode == 0 || mode == 2)
	{
		POINT_COLOR=LGRAY;
		LCD.Fill(30,20,770-1,260-1,WHITE);
		TEXT.show_str_mid(30,20,"输入框",24,740);
		
	}
}


//键盘按键按下
//0:松开 1:按下
void py_set_ui(u8 key,u8 sta)
{
	
	
	if(key >= 1 && key <= 12)
	{
		key -= 1;
		if(sta)
		{
			POINT_COLOR=BLACK;
			LCD.Fill(KB_MstartX+(key%4)*(KB_MsizeX+KB_MspacX),KB_MstartY+(key/4)*KB_MsizeY+((key/4)+1)*KB_MspacY,KB_MstartX+((key%4)+1)*(KB_MsizeX+KB_MspacX)-KB_MspacX,KB_MstartY+((key/4)+1)*KB_MsizeY+((key/4)+1)*KB_MspacY,GRAY);
			TEXT.show_str_mid(KB_MstartX+(key%4)*(KB_MsizeX+KB_MspacX),KB_MstartY+4+(KB_MsizeY+KB_MspacY)*(key/4),(u8*)kbd_tbl[key],16,KB_MsizeX);		
			TEXT.show_str_mid(KB_MstartX+(key%4)*(KB_MsizeX+KB_MspacX),KB_MstartY+(KB_MsizeY+KB_MspacY)/2+(KB_MsizeY+KB_MspacY)*(key/4)-3,(u8*)kbs_tbl[key],24,KB_MsizeX);
		}
		else
		{
			POINT_COLOR=BLACK;
			LCD.Fill(KB_MstartX+(key%4)*(KB_MsizeX+KB_MspacX),KB_MstartY+(key/4)*KB_MsizeY+((key/4)+1)*KB_MspacY,KB_MstartX+((key%4)+1)*(KB_MsizeX+KB_MspacX)-KB_MspacX,KB_MstartY+((key/4)+1)*KB_MsizeY+((key/4)+1)*KB_MspacY,WHITE);
			TEXT.show_str_mid(KB_MstartX+(key%4)*(KB_MsizeX+KB_MspacX),KB_MstartY+4+(KB_MsizeY+KB_MspacY)*(key/4),(u8*)kbd_tbl[key],16,KB_MsizeX);		
			TEXT.show_str_mid(KB_MstartX+(key%4)*(KB_MsizeX+KB_MspacX),KB_MstartY+(KB_MsizeY+KB_MspacY)/2+(KB_MsizeY+KB_MspacY)*(key/4)-3,(u8*)kbs_tbl[key],24,KB_MsizeX);
		}
	}
	
	else if(key >= 21 && key <= 29)
	{
		key -= 21;
		if(sta)
		{
			POINT_COLOR=BLACK;
			LCD.Fill(0+(key%3)*70,310+(key/3)*54,0+((key%3)+1)*70,310+((key/3)+1)*54,GRAY);
			TEXT.show_str_mid(0+(key%3)*71,320+(key/3)*56,T9.pymb[key]->py,24,71);
		}
		else
		{
			POINT_COLOR=BLACK;
			LCD.Fill(0+(key%3)*70,310+(key/3)*54,0+((key%3)+1)*70,310+((key/3)+1)*54,LGRAY);
			TEXT.show_str_mid(0+(key%3)*71,320+(key/3)*56,T9.pymb[key]->py,24,71);
		}
	}
	
	else if(key == 31 || key == 32)
	{
		key -= 31;
		if(sta)
		{
			POINT_COLOR=BLACK;
			LCD.Fill(650+key*70,280,650+(key+1)*70,310,GRAY);
			TEXT.show_str_mid(650+key*70,280,(u8*)kbp_tbl[key],24,70);
		}
		else
		{
			POINT_COLOR=BLACK;
			LCD.Fill(650+key*70,280,650+(key+1)*70,310,WHITE);
			TEXT.show_str_mid(650+key*70,280,(u8*)kbp_tbl[key],24,70);
		}
	}
	
	else if(key >= 41 && key <= 50)
	{
		key -= 41;
		if(sta)
		{
			POINT_COLOR=BLACK;
			LCD.Fill(15+key*60,280,15+(key+1)*60,310,GRAY);
			if(!T9.InpMode)TEXT.show_font(0+key*60+30,280,&T9.pymb[T9.WordOrdinal]->pymb[(T9.WordPage*10+key)*2],24,1);
		  else LCD.ShowChar(0+key*60+30,280,T9.input_en_num[key],24,1);
		}
		else
		{
			POINT_COLOR=BLACK;
			LCD.Fill(15+key*60,280,15+(key+1)*60,310,WHITE);
			if(!T9.InpMode)TEXT.show_font(0+key*60+30,280,&T9.pymb[T9.WordOrdinal]->pymb[(T9.WordPage*10+key)*2],24,1);
			else LCD.ShowChar(0+key*60+30,280,T9.input_en_num[key],24,1);
		}
	}
	
	else if(key >= 51 && key <= 53)
	{
		key -= 51;
		if(sta)
		{
			POINT_COLOR=BLACK;
			if(key == 0)
			{
				LCD.Fill(715,356,790,426,GRAY);
				TEXT.show_str_mid(715,379,(u8*)kbc_tbl[0],24,75);	
			}
			else if(key == 1)
			{
				LCD.Fill(715,429,790,469,GRAY);
				TEXT.show_str_mid(715,437,(u8*)kbc_tbl[1],24,75);
			}
			else if(key == 2)
			{
				LCD.Fill(715,313,790,353,GRAY);
				POINT_COLOR=BLACK;
				TEXT.show_str_mid(715,315,(u8*)kbm_tbl[T9.InpMode],24,50);
				POINT_COLOR=MGRAY;		
				TEXT.show_str_mid(765,335,(u8*)kbm_tbl[!T9.InpMode],16,25);	
				POINT_COLOR=BLACK;
			}
		}
		else
		{
			POINT_COLOR=BLACK;
			
			if(key == 0)
			{
				LCD.Fill(715,356,790,426,WHITE);
				TEXT.show_str_mid(715,379,(u8*)kbc_tbl[0],24,75);	
			}
			else if(key == 1)
			{
				LCD.Fill(715,429,790,469,WHITE);
				TEXT.show_str_mid(715,437,(u8*)kbc_tbl[1],24,75);
			}
			else if(key == 2)
			{
				LCD.Fill(715,313,790,353,WHITE);
				POINT_COLOR=BLACK;
				TEXT.show_str_mid(715,315,(u8*)kbm_tbl[T9.InpMode],24,50);
				POINT_COLOR=MGRAY;		
				TEXT.show_str_mid(765,335,(u8*)kbm_tbl[!T9.InpMode],16,25);	
				POINT_COLOR=BLACK;
			}
		}
	}
	
	 					   
}

//得到触摸屏的输入
//x,y:键盘坐标
//返回值：按键键值
// 0:按键位置坐标无效
// 1~12：主键盘位置
// 21~29：左侧副键盘位置
// 31~32：加减选择
// 41~50：文字选择
u8 py_get_keynum(u16 x,u16 y)
{
	u8 i,j;
	
	for(j=0;j<3;j++)										//主键盘区域
	{
		for(i = 0;i<4;i++)
		{
//			if(j == 3 && (i == 0 || i == 2)) continue;
			
			if(x > KB_MstartX+i*(KB_MsizeX+KB_MspacX) && 
				 x < KB_MstartX+(i+1)*(KB_MsizeX+KB_MspacX)-KB_MspacX && 
				 y > KB_MstartY+j*KB_MsizeY+(j+1)*KB_MspacY && 
				 y < KB_MstartY+(j+1)*KB_MsizeY+(j+1)*KB_MspacY) return 4*j+i+1;
		}
	}
	
	for(j=0;j<3;j++)													//副键盘区域
	{
		for(i = 0;i<3;i++)
		{
			if(x > KB_LstartX+i*KB_LsizeX && 
				 x < KB_LstartX+(i+1)*KB_LsizeX && 
				 y > KB_LstartY+j*KB_LsizeY && 
				 y < KB_LstartY+(j+1)*KB_LsizeY) return 3*j+i+21;
		}
	}
	
	for(i = 0;i<2;i++)											//翻页 + - 
	{
		if(x > 650 + i*70			&& 
			 x < 650 +(i+1)*70 && 
			 y > 280 					&& 
			 y < 310		)   return i+31;
	}
	
	for(i = 0;i< 10 ;i++)                   //字体选择区域
	{
		if(x > 15 + i*60			&& 
			 x < 15 +(i+1)*60  && 
			 y > 280 					&& 
			 y < 310		)   return i+41;
	}
	
//	for(i=0;i<2;i++)
//	{
//		if(x > 715				&& 
//			 x < 790      	&& 
//			 y > 313+i*77 	&& 
//			 y < 313+(i+1)*77		)   return i+51;
//	}
	if(x > 715 && x <790 && y > 313 && y < 353) return 53;
	else if(x > 715 && x <790 && y > 356 && y < 426) return 51;
  else if(x > 715 && x <790 && y > 429 && y < 469) return 52;

	return 0;
}




//比较两个字符串的匹配情况
//返回值:0xff,表示完全匹配.
//		 其他,匹配的字符数
u8 str_match(u8*str1,u8*str2)
{
	u8 i=0;
	while(1)
	{
		if(*str1!=*str2)break;		  //部分匹配
		if(*str1=='\0'){i=0XFF;break;}//完全匹配
		i++;
		str1++;
		str2++;
	}
	return i;//两个字符串相等
}

//获取匹配的拼音码表
//*strin,输入的字符串,形如:"726"
//**matchlist,输出的匹配表.
//返回值:[7],0,表示完全匹配；1，表示部分匹配（仅在没有完全匹配的时候才会出现）
//		 [6:0],完全匹配的时候，表示完全匹配的拼音个数
//			   部分匹配的时候，表示有效匹配的位数				    	 
u8 get_matched_pymb(u8 *strin,py_index **matchlist)
{
	py_index *bestmatch=0;//最佳匹配
	u16 pyindex_len=0;
	u16 i=0;
	u8 temp,mcnt=0,bmcnt=0;
	bestmatch=(py_index*)&py_index3[0];//默认为a的匹配
	pyindex_len=sizeof(py_index3)/sizeof(py_index3[0]);//得到py索引表的大小.
	for(i=0;i<pyindex_len;i++)
	{
		temp=str_match(strin,(u8*)py_index3[i].py_input);
		if(temp)
		{
			if(temp==0XFF)matchlist[mcnt++]=(py_index*)&py_index3[i];
			else if(temp>bmcnt)//找最佳匹配
			{
				bmcnt=temp;
			    bestmatch=(py_index*)&py_index3[i];//最好的匹配.
			}
		}
	}
	if(mcnt==0&&bmcnt)//没有完全匹配的结果,但是有部分匹配的结果
	{
		matchlist[0]=bestmatch;
		mcnt=bmcnt|0X80;		//返回部分匹配的有效位数
	}
	return mcnt;//返回匹配的个数
}

//得到拼音码表.
//str:输入字符串
//返回值:匹配个数.
u8 get_pymb(u8* str)
{
	return get_matched_pymb(str,T9.pymb);
}

void test_py(u8 *inputstr)
{
	u8 t=0;
	u8 i=0;
	t=T9.getpymb(inputstr);
	if(t&0X80)
	{
		printf("\r\n输入数字为:%s\r\n",inputstr);
		printf("部分匹配位数:%d\r\n",t&0X7F);
		printf("部分匹配结果:%s,%s\r\n",T9.pymb[0]->py,T9.pymb[0]->pymb);
	}else if(t)
	{
		printf("\r\n输入数字为:%s\r\n",inputstr);
		printf("完全匹配个数:%d\r\n",t);
		printf("完全匹配的结果:\r\n");
		for(i=0;i<t;i++)
		{
			printf("%s,%s\r\n",T9.pymb[i]->py,T9.pymb[i]->pymb);
		}
	}else printf("没有任何匹配结果！\r\n");
}
































