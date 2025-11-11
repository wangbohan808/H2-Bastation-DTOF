#ifndef BOARD_KEY_H
#define BOARD_KEY_H

#include "app_config.h"
#include "gpio_cfg.h"
#include "lighting_ctrl.h"

/*配置按键个数*/
#define BOARD_KEY_USED_NUM              1
/*配置按键短按时长,单位:10ms*/
#define KEY_PRESS_SHOT_TIME             4
/*配置按键长按时长,单位:10ms*/
#define KEY_PRESS_LONG_TIME             200
/*配置按键按下保持时间*/
#define KEY_PRESS_HOLD_TIME             400
/*连续短按最大时间间隔,单位:10ms*/
#define KEY_MULTI_PRESS_MAX_INTERVAL    400    
/*按键按下时的io电平*/
#define BOARD_KEY_PRESS_IO_LEVEL        0  

#define GET_INPUT_IO_LEVEL(gpiox,pinx)         GPIO_ReadPin(gpiox,pinx)

#define RAMFUNC		__attribute__((section(".ramcode")))

typedef struct 
{
    GPIO_TypeDef* GPIOx;    /*按键输入io对应的gpio组*/
    unsigned short io_pin;  
}key_io_info_t;

extern key_io_info_t key_io_info_table[BOARD_KEY_USED_NUM];

extern void board_key_init(void);

extern void key_press_detect(void);

void set_key_init(uint8_t val) ;

uint8_t get_key_init(void ) ;


#endif

