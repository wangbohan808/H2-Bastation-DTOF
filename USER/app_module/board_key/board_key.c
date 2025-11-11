#include "board_key.h"
#include "vacuum_ctrl.h"
#include "base_event.h"
#include "adc_capture.h"
#include "base_state.h"
#include "ir_capture.h"
#include "lighting_ctrl.h"

typedef enum
{
    KYE_PRESS_NULL       = 0,  /*按键空闲*/
    KEY_PRESS_SHORT_UP   = 1,  /*按键短按*/
    KEY_PRESS_LONG_UP    = 2,  /*按键长按*/
    KEY_PRESS_DOUBLE_UP  = 3,  /*双击*/
    KEY_PRESS_TRIPLE_UP  = 4,  /*三击*/
    KEY_PRESS_HOLD_UP    = 5,  /*按键持续按下*/
}KEY_VALUE_E;


/*按键连续按下计数*/
 unsigned int key_press_on_time[BOARD_KEY_USED_NUM];
/*按键连续抬起计数*/
 unsigned int key_press_off_time[BOARD_KEY_USED_NUM];

key_io_info_t key_io_info_table[BOARD_KEY_USED_NUM] = 
{
    {CW_GPIOB,GPIO_PIN_7},
};


/*
 *@brief: 本地按键信号检测,在10ms定时任务中执行
*/
void key_press_detect(void)
{
	for (unsigned char i = 0; i < BOARD_KEY_USED_NUM; i++)
	{
		if(GET_INPUT_IO_LEVEL(key_io_info_table[i].GPIOx,key_io_info_table[i].io_pin) == BOARD_KEY_PRESS_IO_LEVEL)
		{
			key_press_on_time[i] ++;
			key_press_off_time[i] = 0;
		}
		else
		{
			if(key_press_on_time[i] > 0)
			{			
				key_press_off_time[i] ++;
				
				if(key_press_off_time[i] > KEY_MULTI_PRESS_MAX_INTERVAL)
				{
					if(key_press_on_time[i] > 2500)
					{
						set_need_duty(1);  	
					}
					
					key_press_off_time[i] = 0;
					key_press_on_time[i] = 0;
				}
			}
		}
	}    
}
