#ifndef IR_OTA_H_
#define IR_OTA_H_

#include "main.h"

void test_data_process(uint8_t* recv_data_buf,uint8_t len) ;

uint8_t check_sum(uint8_t * arr ,uint8_t len) ;
int32_t app_flag_write1(uint32_t data ,uint32_t start_add2) ;
int32_t app_flag_write(uint32_t data ,uint32_t start_add2) ;

#define start_add 0x8004400


extern uint8_t page ;

extern uint8_t count ;

extern uint32_t timeout ;

void Set_Firmware_size(uint32_t size) ;

void Set_Numbur(uint16_t num) ;

void OTA_Process(uint8_t* recv_buff,uint8_t len ) ;

int32_t app_flash_write(uint32_t *data ,uint32_t Flash_address,uint16_t count) ;

#define app_update_flag_addr 		0x0000E08	//地址要超出APP的内容范围。demo的bin内容在0x080054F8截止
#define ir_ota_addr             0x0000E00

extern uint32_t Firmware_size ;

extern uint16_t Numbur ;

extern uint8_t recv_ota_data_ok ;

extern uint16_t last_number ;

extern uint16_t count_data ;

#endif

