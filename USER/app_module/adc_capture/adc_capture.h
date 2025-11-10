#ifndef ADC_CAPTURE_H
#define ADC_CAPTURE_H

#include "main.h"

typedef enum
{
    ADC_VALUE_ROBOT         = 1,    //机器人在位检测
    ADC_VALUE_CHARGE_CUR    = 2,    //充电电流检测
    ADC_VALUE_DUST_LEVEL    = 3,    //尘满检测

    ADC_VALUE_MAX,
}ADC_VALUE_E;

extern uint16_t ADCConvertedValue[5];

extern void adc_get_data_init_jcb(void);            //adc数据采集初始化

extern void adc_get_data_init_cdz(void);

extern uint16_t get_changing_cur(void);         //获取主机充电电流检测值

void ADC_Config_And_Get_Data(void);

uint16_t get_changing_cur_wake(void) ;

#endif

