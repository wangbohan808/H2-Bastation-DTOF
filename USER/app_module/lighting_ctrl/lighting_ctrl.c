#include "lighting_ctrl.h"
#include "vacuum_ctrl.h"
#include "base_state.h"



void lighting_ctrl_init(void)
{
  gpio_output_cfg(CW_GPIOB,GPIO_PIN_1);
	set_gpio_level(CW_GPIOB,GPIO_PIN_1,1);            //红灯
}

void LED_ON(void)
{
    set_gpio_level(CW_GPIOB,GPIO_PIN_1,0);
//    CW_GPIOA->PBC= GPIO_PIN_4;	
}

void LED_OFF(void)
{
    set_gpio_level(CW_GPIOB,GPIO_PIN_1,1); 
//	CW_GPIOA->PBSC = GPIO_PIN_4;
}


void LED_display_light_twinkle(uint8_t led)
{
	static uint16_t dispaly_twinkle_count = 0;	
	dispaly_twinkle_count ++ ;
	if(dispaly_twinkle_count <500 )
	{
		    LED_ON();
	}			
	else if(dispaly_twinkle_count>=500&&dispaly_twinkle_count<=1000) 
	{
		 LED_OFF();
		 if(dispaly_twinkle_count == 1000)
		 {
			 dispaly_twinkle_count = 0;
		 }
	}
}

///*
//         暗--灭  ：1s   10ms 10个等级   100ms提升一个等级
//         
//*/
void led_set_pwm(uint8_t duty) //100*100 =10000us =10ms       
{
	static uint8_t cnt1 = 0;
	cnt1 ++;
	if(cnt1 < duty )
	{
		LED_ON() ;
	}
	else 
	{
		LED_OFF();
		if(cnt1 == 100 )
		{
		   cnt1 = 0;
		}
	}
}



//100 *10ms  =1000ms  =1s
void  led_breath(void)   //
{
    static uint8_t time = 0;
    static uint8_t cty = 0;
	  static uint8_t breath_mode = 0;
	  time ++;
	  if(breath_mode == 0)
		{
				if(time >= 250)
				{
					 time = 0;
					 cty ++;
					 if(cty >= 100)
					 {
					     breath_mode = 1;
					 }
				}
		}
		else
		{
		    if(time >= 250)
				{
					 time = 0;
					 cty --;
					 if(cty == 0)
					 {
					     breath_mode = 0;
					 }
				}
		}
		led_set_pwm(cty);
}





//等效闪烁时间，若时间为0，则不闪烁
uint32_t light_twinkle_time = 0;
uint8_t light_show_mode = 1;


void set_light_twinkle_time(uint32_t time)
{
    light_twinkle_time = time ;
}


