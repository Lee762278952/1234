#ifndef __POWER_H__
#define __POWER_H__	  
#include "sys.h" 

#define POWER_ENABLE 					PBout(0)   
#define OTG_ENABLE		 				PDout(2)   
#define CHARGING_DISABLE 		 	PDout(3) 

#define POWER_CHECK           PCin(13)  
#define CHG_CHECK							PHin(3)

u8 PowerCheck(void);
void Power_Init(void);

#endif

