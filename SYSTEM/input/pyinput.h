#ifndef __PYINPUT_H
#define __PYINPUT_H
#include "sys.h"
 
//主键盘按键尺寸 
#define 	KB_MsizeX 	 		120
#define 	KB_MsizeY 			 50
//主键盘按键间距
#define 	KB_MspacX  			5
#define 	KB_MspacY  			3
//主键盘开始位置
#define   KB_MstartX  		215
#define   KB_MstartY	 		310
//副键盘按键尺寸
#define 	KB_LsizeX 	 		70
#define 	KB_LsizeY 			54
//副键盘开始位置
#define   KB_LstartX  		0
#define   KB_LstartY	 		310

#define   Nextpage       "+"
#define   Prevpage			 "-"

#define MAX_MATCH_PYMB 	10	//最大匹配数

//拼音码表与拼音的对应表
typedef struct
{
  u8 *py_input;//输入的字符串
  u8 *py;	   //对应的拼音
  u8 *pymb;	   //码表
}py_index;

//输入法功能类
typedef struct
{
	u8 KeyPress;
	u8 inputstr[7];
	u8 inputlen;
	u8 result_num;
	u8 resultword_num;
	u8 WordOrdinal;
	u8 WordPage;
	u8 InpMode; //模式 0 = 中文输入 CN 1 = 英文数字输入 EN
	
	u8 *Wordoutput;
	u8 Wordoutput_len;
	
	u8(*getpymb)(u8 *instr);			//字符串到码表获取函数
  py_index *pymb[MAX_MATCH_PYMB];	//码表存放位置
	u8 input_en_num[10];
}_t9_class;

extern _t9_class T9;





void py_load_ui(u8 mode);
u8 py_get_keynum(u16 x,u16 y);
void py_set_ui(u8 key,u8 sta);
void py_get_word(u8 key);
void py_show_result(u8 index);
void en_num_get_word(u8 key);
//extern pyinput t9;
u8 str_match(u8*str1,u8*str2);
u8 get_matched_pymb(u8 *strin,py_index **matchlist);
u8 get_pymb(u8* str);
void test_py(u8 *inputstr);
#endif








