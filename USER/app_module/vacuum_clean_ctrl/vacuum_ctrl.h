#ifndef VACUUM_CTRL_H
#define VACUUM_CTRL_H

#include "main.h"

extern void dust_absorption_ctrl_init(void);

extern void dust_absorption_ctrl(uint8_t ctrl);         //集尘控制函数

extern void set_need_duty(uint8_t val);

uint32_t get_time_for_dust_finish(void);

void delay_10us(uint16_t us) ;

extern uint8_t ac_frequency ;

extern uint8_t dust_absorption_time ;

extern uint8_t need_duty ;

extern uint32_t time_for_dust_finish ;

uint8_t get_need_duty(void) ;


#endif

