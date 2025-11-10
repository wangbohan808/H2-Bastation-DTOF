#ifndef PWM_DATA_OUTPUT_H
#define PWM_DATA_OUTPUT_H

#include "main.h"

typedef enum
{
    COMM_DATA_VACUUM_CLEANING_START  = 1,    //开始集尘
    COMM_DATA_VACUUM_CLEANING_FINISH = 2,    //集尘完成
    COMM_DATA_DUST_BAG_UNSTALL       = 3,    //尘袋不在位
    COMM_DATA_DUST_BAG_FULL          = 4,    //尘袋满
    COMM_DATA_AC_FAN_ABNORMAL        = 5,    //风机异常
}COMM_DATA_E;

extern void charge_ctrl_init(void);             //充电控制初始化

//extern void charge_ctrl_on(void);               //打开充电

extern void pwm_data_output(COMM_DATA_E data);  //极片信号发送

//void set_charge_state( uint8_t state) ;

//uint8_t get_charge_state(void) ;

void charge_on(void) ;

void charge_off(void) ;

#endif

