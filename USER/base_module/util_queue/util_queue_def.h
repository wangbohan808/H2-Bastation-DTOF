/// @file util_queue_def.h
/// @brief 工具类模块 - 队列

#ifndef UTIL_QUEUE_DEF_H
#define UTIL_QUEUE_DEF_H

#include "app_config.h"
#include "main.h"

typedef unsigned long U32;
typedef unsigned short U16;
typedef unsigned char U8;
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;


///队列结构体定义
typedef struct util_queue_t
{
    U8 is_lock_irq;     //读写时是否锁中断，保证多存和多取操作的同步；如果单存单取，可以不使能
    U8 * data_buffer;   //数据缓存
    U16 write_ptr;      //当前写地址
    U16 read_ptr;       //当前读地址
//    U16 data_length;    //数据长度
    U16 queue_len;      //队列缓存长度
} util_queue_t;

#endif

