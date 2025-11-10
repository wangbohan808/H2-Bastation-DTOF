#ifndef BASE_STATE_H
#define BASE_STATE_H

#include "main.h"

typedef enum
{
    BASE_CONFIGURATION_LOW = 0,     //低配版基站
    BASE_CONFIGURATION_HIGH = 1,     //高配版基站
}BASE_CONFIGURATION_E;

typedef enum
{
    BASE_STATE_IDLE         = 0,      //设备空闲
    BASE_STATE_CHARGING     = 1,      //开始充电
} BASE_STATE_E;

typedef enum
{
    WORK_MODE_IDEL    = 0,     //空闲模式
    WORK_MODE_FULL_GO = 1,     //老化测试模式
    WORK_MODE_IR_OTA  = 2,     //OTA模式
}BASE_WORK_MODE_E;

typedef enum
{
    DUST_BAG_STATE_UNSTALL = 0,     //尘袋不在位
    DUST_BAG_STATE_STALL   = 1,     //尘袋在位
}DUST_BAG_STATE_E;

typedef enum
{
    ROBOT_STATE_NOT_AT_DOCK = 0,    //机器人不在位
    ROBOT_STATE_AT_DOCK     = 1,    //机器人在位
}ROBOT_AT_DOCK_STATE_E;

typedef enum
{
    DUST_BAG_FULL     = 0,          //尘袋满
    DUST_BAG_NOT_FULL = 1,          //尘袋未满
}DUST_BAG_FULL_STATE_E;

typedef enum
{
    CHARGE_STATE_NONE  = 0,       //未开启充电
    CHARGE_STATE_CHARGE = 1,      //开启充电
    CHARGE_STATE_ERROR  = 2,      //充电异常
}CHARGE_STATE_E;

typedef enum          //进入老化模式步骤
{
    WORK_MODE_FULL_GO_IDEL     = 0,
    WORK_MODE_FULL_GO_FIRST    = 1,     //步骤1
    WORK_MODE_FULL_GO_SECOND   = 2,     //步骤2
    WORK_MODE_FULL_GO_END      = 3,     //步骤3
}WORK_MODE_FULL_GO_STEP;


typedef enum          //常常模式测试项目 
{
    TEST_IDEL            = 0,
	TEST_IR              = 1,
    TEST_VACUUN_TEST     = 2,
    TEST_DUSTBUG_TEST    = 3,     
    TEST_LED_ON_TEST     = 4,     
	TEST_CURRENT_        = 5,
    TEST_END             = 6,	
}TEST_FULL_GO;


extern BASE_STATE_E get_base_state(void);                           //获取基站状态

extern void set_base_state(BASE_STATE_E state);                     //设置基站状态

extern BASE_WORK_MODE_E get_base_work_mode(void);                   //获取基站工作模式

extern void set_base_work_mode(BASE_WORK_MODE_E mode);              //设置基站工作模式

extern ROBOT_AT_DOCK_STATE_E get_robot_at_dock_state(void);         //获取主机在位状态

extern void set_robot_at_dock_state(ROBOT_AT_DOCK_STATE_E state);   //设置基站在位状态

extern DUST_BAG_STATE_E get_dust_bag_state(void);                   //获取尘袋在位情况

extern void set_duty_bag_state(DUST_BAG_STATE_E state);             //设置尘袋在位情况

extern DUST_BAG_FULL_STATE_E get_dust_bag_full_state(void);         //获取尘袋满状态

extern void set_dust_bag_full_state(DUST_BAG_FULL_STATE_E state);   //设置尘袋满状态

extern void set_base_change_state(CHARGE_STATE_E state);            //设置充电状态

extern CHARGE_STATE_E get_base_charge_state(void);                  //获取充电状态

uint32_t get_delay_time_cnt(void);

void set_delay_time_cnt(uint32_t val);

BASE_CONFIGURATION_E get_base_configuration(void);

void set_base_configuration(BASE_CONFIGURATION_E configuration);

extern DUST_BAG_STATE_E dust_bag_state ;

extern BASE_STATE_E base_state ;

extern BASE_WORK_MODE_E base_work_mode ;

extern ROBOT_AT_DOCK_STATE_E robot_at_dock_state ;


#endif

