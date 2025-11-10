#ifndef _IR_DECODER_IO_LEVEL_H
#define _IR_DECODER_IO_LEVEL_H

#include "main.h"
#include "ir.h"
//#include "util_queue_interface.h"

/// @file ir_decoder_dock.h
/// @brief 红外解码器 - dock

///红外解码器最大数量
//#define IR_DECODE_FUNCTION_MAX_COUNT 2
///IR数据队列长度，如果信号采集在8K中断执行，解码在10ms中断执行，则需要大于 8000/(100 * 8) = 10
#define IR_QUEUE_LEN 128

typedef enum
{
      REMOTE_RESYNC,  //等待信号接收同步
	  REMOTE_HEADER,  //
	  REMOTE_DATA_BIT_HIGH, //
	  REMOTE_DATA_BIT_LOW,	 
} REMOTE_DECODE_ST_E;

typedef struct
{
    REMOTE_DECODE_ST_E state;
    unsigned char timer;           //一位编码的时间有效时间，用于超时判断
    unsigned char count;           //有效电平统计时间（主要是统计低电平时间，有的方案可能是统计高电平）
    unsigned char bits_count;      //数据码位数统计
    unsigned char decode_value;    //解码过程值
} remote_decode_t;



extern remote_decode_t dock_decode; 



/**
 * @fn uint8_t ir_decoder_dock_io_level_process(IR_REMOT_POSITION_E, uint8_t, uint16_t*)
 * @brief 本解码器的解码执行函数
 *
 * @param ir_pos        红外接收头序号
 * @param bit_state_8   8位数据，表示8个电平信号
 * @param code_result   解码结果值
 * @return 1 解码出数据；0 未解码出数据
 */
uint8_t ir_decoder_dock_io_level_process(uint8_t bit_state_8, uint16_t * code_result);

/**
 * @fn void ir_decoder_dock_init(void)
 * @brief 解码器初始化
 *
 */
extern void ir_decoder_dock_init(void);


/**
 * @fn uint8_t ir_decoder_remote_io_level_process(IR_REMOT_POSITION_E, uint8_t, uint8_t*)
 * @brief 本解码器的解码执行函数
 *
 * @param ir_pos        红外接收头序号
 * @param bit_state_8   8位数据，表示8个电平信号
 * @param code_result   解码结果值
 * @return 1 解码出数据；0 未解码出数据
 */
//uint8_t ir_decoder_remote_io_level_process(IR_REMOT_POSITION_E ir_pos, uint8_t bit_state_8, uint8_t * code_result);

/**
 * @fn void ir_decoder_remote_init(void)
 * @brief 解码器初始化
 *
 */
void ir_decoder_remote_init(void);

void ir_detect_capture(void);
/**
 * @fn void ir_decoder_io_level_init(void)
 * @brief 红外数据队列初始化
 *
 */
void ir_decoder_io_level_init(void);

extern util_queue_t ir_rx_queue ;

void ir_dock_resync_init(remote_decode_t * decode_p) ;


#endif
