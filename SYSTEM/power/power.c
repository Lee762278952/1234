#include "power.h"
#include "adc.h"


u8 PowerCheck(void)
{
	u16 adcx,pow_temp;
		
	adcx=Get_Adc_Average(ADC_CHANNEL_9,50);
	pow_temp = adcx*330/4096;
//	printf("power = %d adcx = %d\r\n",pow_temp,adcx);

	if(pow_temp >= 204) return 100;
	else if(pow_temp >= 198 && pow_temp < 204) return 90;
	else if(pow_temp >= 193 && pow_temp < 198) return 80;
	else if(pow_temp >= 188 && pow_temp < 193) return 70;
	else if(pow_temp >= 184 && pow_temp < 188) return 60;
	else if(pow_temp >= 181 && pow_temp < 184) return 50;
	else if(pow_temp >= 179 && pow_temp < 181) return 40;
	else if(pow_temp >= 178 && pow_temp < 179) return 30;
	else if(pow_temp >= 175 && pow_temp < 178) return 20;
	else if(pow_temp < 175) return 10;
	else return 0;
	
}

void Power_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_GPIOB_CLK_ENABLE();           //开启GPIOB时钟
	__HAL_RCC_GPIOD_CLK_ENABLE();           //开启GPIOB时钟
	
	GPIO_Initure.Pin=GPIO_PIN_0;           	//PB0
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//复用推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
	POWER_ENABLE = 1;	
	
	GPIO_Initure.Pin=GPIO_PIN_2;           	//PD2
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);
	
	GPIO_Initure.Pin=GPIO_PIN_3;           	//PD3
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);
		  
	OTG_ENABLE  = 0;
	CHARGING_DISABLE = 0;
	
	GPIO_Initure.Pin=GPIO_PIN_13;           	//PC13
	GPIO_Initure.Mode=GPIO_MODE_INPUT;  	//输入
	GPIO_Initure.Pull=GPIO_PULLDOWN;          //下拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
	HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	
	GPIO_Initure.Pin=GPIO_PIN_3;           	//PH3
	GPIO_Initure.Mode=GPIO_MODE_INPUT;  	//输入
	GPIO_Initure.Pull=GPIO_PULLUP;          //下拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
	HAL_GPIO_Init(GPIOH,&GPIO_Initure);
	
	ADC_Initialization();
}


