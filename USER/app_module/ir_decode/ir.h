/// @file ir.h
/// @brief 红外信号解析处理

#ifndef IR_H
#define IR_H


#include "main.h"

///解码方式的宏定义，由此选择解码的方式
#define IR_DECODE_TYPE IR_DECODE_BY_IO_LEVEL

///红外接收头定义的表格

//extern uint16_t ir_current_code;
extern void ir_rx_decode(void);
extern uint8_t ir_check_is_cmd(uint16_t ir_code);

///////////////////////////线程解码的配置//////////////////////////////

#define IS_LEVEL_INVERT 1       //io电平是否取反,一般不需要改动

#define DOCK_RESYNC_TICKS       (20 * 2)   //空闲状态检测到高电平的时间
#define DOCK_HEADER_LOW         (25)       //前导码低电平30ms
#define DOCK_DATA_BIT_UNIT_TICK (8)        //data bit high level err tick
#define DOCK_DATA_BIT_VALUE_1_TICK  (12)  //

//#define DOCK_START_BIT_TICK     (3 * 2)    //第一位编码的等待时间，可以用于跳过前导码，如没有前导码，实际上可以不设置
//#define DOCK_DATA_BIT_TICK      (3/0.125)  //一位编码的有效时间         24             3
//#define DOCK_STOP_BIT_TICK      (14 * 2)   //一位编码最大超时时间
//#define DOCK_BIT_TIMEOUT_TICK   (18 * 2)   //等待一位编码开始的等待最长时间
//#define DOCK_BYTE_STOP_TICKS    (4 * 2)    //编码结束的等待时间，一般小于发码间隔
//#define DOCK_STOP_BIT_MIN_TICKS (2 * 2)    //位编码高电平的计算时间，一般在‘0’和‘1’编码的高电平时间取值
#define IR_CMD_COUNT             (16 + 1)           //IR信号指令

#endif


