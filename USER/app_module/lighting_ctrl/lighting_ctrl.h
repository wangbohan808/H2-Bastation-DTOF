#ifndef LIGHTING_CTRL_H
#define LIGHTING_CTRL_H

#include "gpio_cfg.h"



/*
LIGHT_CTRL_ENABLE 对应的版本
    0：无按键，单灯
		1：有按键，RGB
		2：有按键，单灯
*/

extern void lighting_ctrl_init(void);           //LED初始化函数

void LED_ON(void);

void set_light_twinkle_time(uint32_t time) ;

//uint32_t get_light_twinkle_time(void) ;

void LED_OFF(void) ;

void LED_display_light_twinkle(uint8_t led) ;

void  led_breath(void) ;

void led_set_pwm(uint8_t duty) ;

extern uint8_t light_show_mode ;

extern uint32_t light_twinkle_time;

void led_set_pwm(uint8_t duty); //100*100 =10000us =10ms 

#endif

