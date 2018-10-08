#ifndef __PYINPUT_H
#define __PYINPUT_H
#include "sys.h"
 
//�����̰����ߴ� 
#define 	KB_MsizeX 	 		120
#define 	KB_MsizeY 			 50
//�����̰������
#define 	KB_MspacX  			5
#define 	KB_MspacY  			3
//�����̿�ʼλ��
#define   KB_MstartX  		215
#define   KB_MstartY	 		310
//�����̰����ߴ�
#define 	KB_LsizeX 	 		70
#define 	KB_LsizeY 			54
//�����̿�ʼλ��
#define   KB_LstartX  		0
#define   KB_LstartY	 		310

#define   Nextpage       "+"
#define   Prevpage			 "-"

#define MAX_MATCH_PYMB 	10	//���ƥ����

//ƴ�������ƴ���Ķ�Ӧ��
typedef struct
{
  u8 *py_input;//������ַ���
  u8 *py;	   //��Ӧ��ƴ��
  u8 *pymb;	   //���
}py_index;

//���뷨������
typedef struct
{
	u8 KeyPress;
	u8 inputstr[7];
	u8 inputlen;
	u8 result_num;
	u8 resultword_num;
	u8 WordOrdinal;
	u8 WordPage;
	u8 InpMode; //ģʽ 0 = �������� CN 1 = Ӣ���������� EN
	
	u8 *Wordoutput;
	u8 Wordoutput_len;
	
	u8(*getpymb)(u8 *instr);			//�ַ���������ȡ����
  py_index *pymb[MAX_MATCH_PYMB];	//�����λ��
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








