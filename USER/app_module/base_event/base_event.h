#ifndef BASE_EVENT_H
#define BASE_EVENT_H

#include "main.h"

typedef enum
{
    BASE_EVENT_RESERVE          = 0,        //预留
    BASE_EVENT_VACUUM_START     = 1,        //集尘开始
    BASE_EVENT_TEST_START       = 2,        //进入测试模式
	BASE_EVENT_TEST_START1      = 3,
	
	BASE_EVENT_TEST_START2      = 4,
    BASE_EVENT_TEST_VACUUN_START= 5,        //开启风机测试
	BASE_EVENT_TEST_VACUUN_START15 = 6,
    BASE_EVENT_TEST_VACUUN_END  = 7,        //结束风机测试
	
    BASE_EVENT_TEST_DUSTBUG     = 8,        //开启尘袋测试
	BASE_EVENT_TEST_LED_ON      = 9,
	BASE_EVENT_TEST_CURRENT     =10,
	BASE_EVENT_TEST_IR_OK       =11,
	
	BASE_EVENT_TEST_IR_ERR      =12,
    BASE_EVENT_TEST_END         =13,        //推出测试模式
	BASE_EVENT_CHARGE_COMPLETE  =14,        //充电完成
	BASE_EVENT_VACUUM_RM_STAR   =15,
	BASE_EVENT_CHARGE_ON        =16,
}BASE_EVENT_E;

extern void create_base_event(BASE_EVENT_E event);

extern void clear_base_event(BASE_EVENT_E event);

extern uint8_t get_base_event(BASE_EVENT_E event);

void clear_all_event(void) ;

uint8_t get_base_event_no_clear(BASE_EVENT_E event) ;

extern uint32_t base_event ;

#endif
