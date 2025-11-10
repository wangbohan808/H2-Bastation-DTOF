#ifndef IR_CAPTURE_H
#define IR_CAPTURE_H

#include "main.h"
#include "stdlib.h"

/*红外解码方式:先高电平再低电平,单位100us*/
#define LEAD_CODE_L 30
#define LEAD_CODE_H 50

#define DATA_BIT_ONE_L 10        /*数据位"1"低电平持续时间*/
#define DATA_BIT_ONE_H 20        /*数据位"1"低电平持续时间*/
#define DATA_BIT_ZERO_L 10        /*数据位"0"低电平持续时间*/
#define DATA_BIT_ZERO_H 10        /*数据位"0"低电平持续时间*/

#define THRESHOLD_RANGE 2       /*接收头电平持续时间可变范围*/
#define BURR_RANGE      1       /*毛刺范围*/



typedef enum
{
    REMOTE_HEAD_CHECK  = 0,
    REMOTE_START_BIT_WAIT,
    REMOTE_START_BIT,
    REMOTE_STOP_BIT,
    REMOTE_BURR,
}DECODE_STATE_E;




#endif

