#include "serial.h"

///通信串口数据队列长度
#define BOARD_UART_QUEUE_LEN    32
///通信串口数据缓冲区
unsigned char board_uart_rx_buf[BOARD_UART_QUEUE_LEN] = {0};

util_queue_t board_rx_queue = {0,board_uart_rx_buf,0,0,BOARD_UART_QUEUE_LEN};

///串口接收队列
util_queue_t * rx_queue_list = {&board_rx_queue};

/**
 * @brief 由串口接收中断调用
 * @param serial_index  串口序号
 * @param data          收到的数据
 * @return              (未使用)
 */
int serial_rx_handle(int data)
{
//    if(serial_index >= SERIAL_INDEX_MAX)
//    {
//        return 0;
//    }
    queue_put(rx_queue_list,(data&0x0ff));
    return 0;
}



