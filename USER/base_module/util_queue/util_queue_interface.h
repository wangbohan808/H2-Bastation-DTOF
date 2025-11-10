/// @file util_queue_interface.h
/// @brief 工具类模块 - 队列 对外接口

#ifndef UTIL_QUEUE_INTERFACE_H
#define UTIL_QUEUE_INTERFACE_H

//#include "util_queue_def.h"
#include "main.h"

/**
 * @fn void queue_reset(util_queue_t*)
 * @brief 队列重置
 *
 * @param queue 队列
 */
extern void queue_reset(util_queue_t * queue);

/**
 * @fn void queue_init(util_queue_t*, u8*, u16)
 * @brief 初始化队列结构体，设置缓存和缓存大小
 *
 * @param queue         队列
 * @param queue_buffer  缓存
 * @param queue_length  缓存大小
 */
extern void queue_init(util_queue_t * queue, u8 * queue_buffer, u16 queue_length);

/**
 * @fn U8 queue_is_empty(util_queue_t*)
 * @brief 队列是否为空
 *
 * @param queue 队列
 * @return 1 为空；0 不为空
 */
extern U8 queue_is_empty(util_queue_t * queue);

/**
 * @fn U8 queue_is_full(util_queue_t*)
 * @brief 队列是否已满
 *
 * @param queue 队列
 * @return 1 已满；0 未满
 */
extern U8 queue_is_full(util_queue_t * queue);

/**
 * @fn U16 queue_get_datalength(util_queue_t*)
 * @brief 获取队列中数据数量
 *
 * @param queue 队列
 * @return  数据数量
 */
extern U16 queue_get_datalength(util_queue_t * queue);

/**
 * @fn U16 queue_get_space(util_queue_t*)
 * @brief 获取队列中剩余空间
 *
 * @param queue 队列
 * @return  剩余空间
 */
extern U16 queue_get_space(util_queue_t * queue);

/**
 * @fn void queue_put(util_queue_t*, U8)
 * @brief 队列中加入数据
 *
 * @param queue 队列
 * @param value 数据
 */
extern void queue_put(util_queue_t * queue, U8 value);

/**
 * @fn U8 queue_get(util_queue_t*)
 * @brief 队列中取出数据
 *
 * @param queue 队列
 * @return  数据
 */
extern U8 queue_get(util_queue_t * queue);

#endif
