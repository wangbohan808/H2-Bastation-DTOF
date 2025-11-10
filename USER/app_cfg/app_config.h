#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include "cw32l010.h"


/**************************  基站配置: 充电桩/集尘宝(低配)/集尘宝(高配)  **************************/
/*充电桩*/
#define CHARGE_ONLY            1 
/*集尘宝(高配)*/                 
#define DUST_CHARGE_HIGH       2
/*集尘宝(低配)*/
#define DUST_CHARGE_LOW        3
#define BASE_STATION_MODE (DUST_CHARGE_HIGH)     

#define CHARGE_ONLY_ENABLE      (BASE_STATION_MODE == CHARGE_ONLY)
#define DUST_CHARGE_HIGH_ENABLE (BASE_STATION_MODE == DUST_CHARGE_HIGH)
#define DUST_CHARGE_LOW_ENABLE  (BASE_STATION_MODE == DUST_CHARGE_LOW)


/**************************  基站-主机通讯方式: 红外+极片/红外+红外  **************************/
/*红外+极片*/
#define COMMUNICATE_IR_POLE    1
/*红外+红外*/
#define COMMUNICATE_IR_IR      2
#define BASE_COMM_MODE (COMMUNICATE_IR_IR)

#define COMM_MODE_IR_POLE_ENABLE (BASE_COMM_MODE == COMMUNICATE_IR_POLE)
#define COMM_MODE_IR_IR_ENABLE   (BASE_COMM_MODE == COMMUNICATE_IR_IR)

/**************************  基站选配: 特殊灯效  **************************/
#define SPECIAL_LIGHTING_EFFECT_ENABLE 1

/**************************  基站选配: O3空气净化  **************************/
#define OZONE_AIR_PURIFICATION_ENABLE  1

/**************************  基站集尘时间间隔: 两次集尘时间间隔时间(单位:分钟)  **************************/
#define DUST_ABSORPTION_TIME_INTERVAL          1


/************************  定义systick定时器频率,不要随意更改此数值!!!  ************************/
#define SYSTICK_FRE_1MS                         1000



/**************************************  机器人在位检测  **************************************/
/*机器人在位检测: 机器人在位,ADC电压为3.2v*/
#define ROBOT_ON_DOCK_CUR                       2760
/*在位检测允许误差范围*/
#define ROBOT_ON_DOCK_ERROR_RANGE               250                     //     250



/**************************************  充电电流检测  **************************************/
/*机器人充电电流最大值*/
#define CHARGE_DETECT_CUR_MAX                   100
/*机器人离开基站时的充电电流*/
#define CHARGE_DETECT_CUR_ROBOT_NOT_AT_DOCK     0
/*充电电流检测允许误差范围*/  
#define CHARGE_DETECT_CUR_ERROR_RANGE           11     //30




void set_soft_version(uint8_t version) ;

uint8_t get_version(void) ;

uint8_t get_version_sort1(void) ;

uint8_t get_version_sort2(void) ;

extern char version[2] ;

extern uint8_t soft_version ;

#endif

