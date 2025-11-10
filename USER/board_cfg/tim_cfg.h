#ifndef TIM_CFG_H
#define TIM_CFG_H

#include "main.h"

extern void tim_ir0_capture_cfg(void);                     //IR-PWM输入捕获采集配置

extern void tim_ir1_capture_cfg(void);              

extern void tim_charge_pwm_cfg(uint16_t arr,uint16_t psc);  //CHARGE-PWM输出配置

extern void tim_ir_pwm_cfg(uint16_t arr,uint16_t psc);      //IR-PWM输出配置

extern void tim_ir_ouput_cfg(void) ;

#endif

