#ifndef MAIN_TASK_H
#define MAIN_TASK_H

#include "main.h"

extern uint16_t message_id ;

extern void app_main_task_jcb(void);

extern void app_main_task_cdz(void);

uint8_t Get_recvive_ok_flag(void) ;

extern uint8_t recv_data_buf[32] ; 

void Set_recvive_ok_flag(uint8_t flag) ;

void Station_send_ack(uint8_t ack,uint8_t cmd1) ;

void Station_send_verson(uint8_t cmd1) ;

extern uint8_t	ota_ok ;

extern uint8_t recvive_ok_flag ;


#endif

