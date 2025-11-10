/// @file util_queue.c
/// @brief 工具类模块 - 队列
/// 将 队列中的数据长度 改为通过读写指针计算得到，在单写单取的情形下，可以不用锁中断；
///
/// 读写指针之间必须相差一个数，所以队列的有效长度为 queue_len -1  , 也就是数据达到 queue_len -1 就应该认为已满。
///

#include "util_queue_interface.h"

/**
 * @fn void queue_reset(util_queue_t*)
 * @brief 队列重置
 *
 * @param queue 队列
 */
void queue_reset(util_queue_t * queue)
{
    queue->read_ptr = 0;
    queue->write_ptr = 0;
}

/**
 * @fn void queue_init(util_queue_t*, u8*, u16)
 * @brief 初始化队列结构体，设置缓存和缓存大小
 *
 * @param queue         队列
 * @param queue_buffer  缓存
 * @param queue_length  缓存大小
 */
void queue_init(util_queue_t * queue, u8 * queue_buffer, u16 queue_length)
{
    queue->data_buffer = queue_buffer;
    queue->queue_len = queue_length;
    queue_reset(queue);
}

/**
 * @fn U16 queue_get_datalength(util_queue_t*)
 * @brief 获取队列中数据数量
 *
 * @param queue 队列
 * @return  数据数量
 */
U16 queue_get_datalength(util_queue_t * queue)
{
    //循环数组作为队列，数据长度通过读写指针计算得到
    if (queue->write_ptr == queue->read_ptr)
    {
        return 0;
    }
    else if (queue->write_ptr > queue->read_ptr)
    {
        //写指针在读指针前面
        return queue->write_ptr - queue->read_ptr;
    }
    else
    {
        //写指针在读指针后面
        return queue->write_ptr + (queue->queue_len - queue->read_ptr);
    }
}

/**
 * @fn U8 queue_is_full(util_queue_t*)
 * @brief 队列是否已满
 *
 * @param queue 队列
 * @return 1 已满；0 未满
 */
U8 queue_is_full(util_queue_t * queue)
{
    //循环数组作为队列，通过读写指针计算判断
    if (queue->write_ptr == queue->read_ptr)
    {
        return 0;
    }
    else if (queue->write_ptr > queue->read_ptr)
    {
        //写指针在读指针前面
        return (queue->write_ptr - queue->read_ptr) >= (queue->queue_len - 1) ?
               1 : 0;
    }
    else
    {
        //写指针在读指针后面
        return (queue->read_ptr - queue->write_ptr) < 2 ? 1 : 0;
    }
}

/**
 * @fn U8 queue_is_empty(util_queue_t*)
 * @brief 队列是否为空
 *
 * @param queue 队列
 * @return 1 为空；0 不为空
 */
U8 queue_is_empty(util_queue_t * queue)
{
    return (queue->write_ptr == queue->read_ptr) ? 1 : 0;
}

/**
 * @fn U16 queue_get_space(util_queue_t*)
 * @brief 获取队列中剩余空间
 *
 * @param queue 队列
 * @return  剩余空间
 */
U16 queue_get_space(util_queue_t * queue)
{
    return (queue->queue_len - 1 - queue_get_datalength(queue));
}

/**
 * @fn void queue_put(util_queue_t*, U8)
 * @brief 队列中加入数据
 *
 * @param queue 队列
 * @param value 数据
 */
void queue_put(util_queue_t * queue, U8 value)
{
    //检查是否写满
//    if (queue_is_full(queue) == 1)
//    {
//        return;
//    }
    queue->data_buffer[queue->write_ptr] = value;
    queue->write_ptr++;
    if (queue->write_ptr >= queue->queue_len)
    {
        queue->write_ptr = 0;
    }
}

/**
 * @fn U8 queue_get(util_queue_t*)
 * @brief 队列中取出数据
 *
 * @param queue 队列
 * @return  数据
 */
U8 queue_get(util_queue_t * queue)
{
    U8 data = 0;

    //防止未初始化
//    if (queue->queue_len < 1)
//    {
//        return 0;
//    }
//    if (queue_is_empty(queue) == 1)
//    {
//        return 0;
//    }
    data = queue->data_buffer[queue->read_ptr];
    queue->read_ptr++;
    if (queue->read_ptr >= queue->queue_len)
    {
        queue->read_ptr = 0;
    }
    return data;
}

