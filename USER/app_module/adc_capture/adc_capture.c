#include "adc_capture.h"

///adc采集初始化
void adc_get_data_init_jcb(void)
{
	adc_capture_cfg_jcb();
}

/*
机器人在位：  3    在： 1.06     不在： 4.4
充电电流  ：  1
尘袋气压  ：  2    
尘袋在位  ：  4
过零      ：  0

*/

///充电电流电压值获取
uint16_t get_changing_cur(void)
{	
	return CW_ADC->RESULT1 ;
}



