#ifndef IR_OUTPUT_H
#define IR_OUTPUT_H

#include "main.h"



#define IR_LEFT_ON              set_gpio_level(CW_GPIOB,GPIO_PIN_5,1)         //左信号B0
#define IR_LEFT_OFF             set_gpio_level(CW_GPIOB,GPIO_PIN_5,0)

#define IR_RIGHT_ON             set_gpio_level(CW_GPIOA,GPIO_PIN_2,1)         //右信号A3
#define IR_RIGHT_OFF            set_gpio_level(CW_GPIOA,GPIO_PIN_2,0)   

#define IR_MIDDLE_LEFT_ON       set_gpio_level(CW_GPIOB,GPIO_PIN_6,1)         //左中B1
#define IR_MIDDLE_LEFT_OFF      set_gpio_level(CW_GPIOB,GPIO_PIN_6,0)

#define IR_MIDDLE_RIGHT_ON      set_gpio_level(CW_GPIOA,GPIO_PIN_1,1)         //右中A2
#define IR_MIDDLE_RIGHT_OFF     set_gpio_level(CW_GPIOA,GPIO_PIN_1,0)

extern uint8_t sent_buff[13] ;


typedef enum
{
    IR_CODE_SEND_MODE_BACK = 0,                         //发射红外引导码模式
    IR_CODE_SEND_MODE_INFO = 1,                         //发送通讯码模式
}IR_CODE_SEND_MODE_E;

typedef struct 
{
    IR_CODE_SEND_MODE_E mode;                           //模式
//    uint32_t code;                                       //码值
    uint8_t enable;                                     //使能
}ir_info_t;

extern void ir_output_init(void);                       //IR信号输出初始化

extern uint8_t get_ir_enable_flag(void);                //获取IR信号使能标志

//extern void set_ir_enable_flag(uint8_t flag);           //设置IR信号使能标志

extern void set_ir_send_mode(IR_CODE_SEND_MODE_E mode);

IR_CODE_SEND_MODE_E get_ir_send_mode(void);

uint32_t get_sys_100us_count(void);

uint32_t get_sys_us_interval(uint32_t time_cnt);

uint8_t ir_sent_buf(uint8_t data[],uint8_t len);

//void set_ir_info_code (uint16_t code) ;

void set_ir_enable_flag(uint8_t flag,uint8_t count ) ;  //设置IR信号使能标志

void set_ir_sent_bite(uint8_t count ) ;

extern ir_info_t ir_info ;

extern uint64_t cnt_us ;

uint32_t timer_elapsed(uint32_t num) ;

uint32_t timer_ms(void) ;

#endif

