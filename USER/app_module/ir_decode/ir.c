/// @file ir.c
/// @brief 红外信号解析处理

#include "ir.h"
#include "base_event.h"
#include "ir_output.h"



typedef enum
{
    RESERVE                  = 0X0,
    IR_CMD_VACUUM_START      = 0xb200,      //开始集尘
    IR_CMD_TEST_START        = 0XA55A,      //进入测试模式
	
	IR_CMD_TEST_START1       = 0X8888,      //进入测试模式
	IR_CMD_TEST_START2       = 0X6666,      //进入测试模式  
    IR_CMD_TEST_VACUUM_ON    = 0X0101,      //开启风机测试
	IR_CMD_TEST_VACUUM_ON_15 = 0X0102,
	
    IR_CMD_TEST_VACUUM_OFF   = 0Xffff,      //关闭风机测试
    IR_CMD_TEST_DUSTBUG      = 0X0200,      //开机集尘测试
	IR_CMD_TEST_LED_ON       = 0X0301,      //闪灯测试
	IR_CMD_TEST_CURRENT      = 0X1000,      //充电电流
	
	IR_CMD_TEST_IR_OK        = 0X0401,
	IR_CMD_TEST_IR_ERR       = 0X0402,
    IR_CMD_TEST_END          = 0XABBA,       //退出
	IR_CMD_CHARGE_COMPLETE   = 0X0164,
	
	IR_CMD_VACUUM_RM_STAR    = 0X0e00,
	IR_CMD_CHARGE_ON         = 0Xba00,
}IR_CDM_E;

typedef struct 
{
    IR_CDM_E cmd;
}ir_cmd_info_t;


//红外通讯命令表
const ir_cmd_info_t ir_cmd_table[IR_CMD_COUNT] = {
    {RESERVE},
    {IR_CMD_VACUUM_START},
    {IR_CMD_TEST_START},
	{IR_CMD_TEST_START1},
	
	{IR_CMD_TEST_START2},
    {IR_CMD_TEST_VACUUM_ON},
	{IR_CMD_TEST_VACUUM_ON_15},
    {IR_CMD_TEST_VACUUM_OFF},
	
    {IR_CMD_TEST_DUSTBUG},
	{IR_CMD_TEST_LED_ON},
	{IR_CMD_TEST_CURRENT},	
	{IR_CMD_TEST_IR_OK},
	
	{IR_CMD_TEST_IR_ERR},
    {IR_CMD_TEST_END},
	{IR_CMD_CHARGE_COMPLETE},
	{IR_CMD_VACUUM_RM_STAR},
	{IR_CMD_CHARGE_ON},
};


//检查是否为指令表中的指令
uint8_t ir_check_is_cmd(uint16_t ir_code)
{
    for(uint8_t i=0;i<IR_CMD_COUNT;i++)
    {
        if(ir_code == ir_cmd_table[i].cmd)
        {
            return i;
        }
    }
    return 0;
}

/**
 * @fn void ir_rx_decode(void)
 * @brief 这个函数中，将红外信号从队列中取出，依次调用已配置的解码器 ir_f_decoder_to_use[] 进行解码
 *
 */
void ir_rx_decode(void)
{
    //是否成功解码
    uint8_t value = 0;
    //解码结果值
    uint16_t code_result = 0;
    //队列中数据数量
    uint8_t data_length_temp = 0;
	  data_length_temp = queue_get_datalength(&ir_rx_queue);
	while (data_length_temp > 0)
	{					  
		data_length_temp--;
		//从队列里取出一个字节 ，含 8 个位数据
		value = queue_get(&ir_rx_queue);
		//顺序调用注册的解码函数，解析红外信号    2
		if(ir_decoder_dock_io_level_process(value, &code_result))
		{
			//最后一个收到的码值，赋给 ir_current_code		
			//解出一个码值，后面的解码器不再执行
			data_length_temp = 0;  //解出一个码值退出本次循环	
			break;
		}
	}
}



