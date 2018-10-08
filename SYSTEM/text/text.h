#ifndef __TEXT_H__
#define __TEXT_H__	  
#include "sys.h" 

typedef enum 
{
	Font24 = 1,
	Font32 = 2,
	Font64 = 3,
}_font_updata;
 					     

//文字功能类
typedef struct  
{		 	
	void (*get_hzmat)(u8 *,u8 *,u8 );			
	void (*show_font)(u16 ,u16 ,u8 *font,u8 size,u8 mode);					
	void (*show_str)(u16 ,u16 ,u16 ,u16 ,u8 *,u16 ,u8 );	
	void (*show_str_mid)(u16 ,u16 ,u8 *,u16 ,u16 );
	char *(*NumToStr)(u32 );

	u8 (*font_library_updata_txt)(_font_updata );
	u8 (*font_library_updata_fon)(_font_updata );
	u8 (*unigbk_updata)(void);
	u8 (*font_check)(void);
	
}_text_class; 


extern _text_class TEXT;


#endif

