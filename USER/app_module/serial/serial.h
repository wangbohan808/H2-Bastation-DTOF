#ifndef SERIAL_H
#define SERIAL_H

#include "main.h"
//#include "util_queue_interface.h"
//#include "util_queue_def.h"


//extern util_queue_t board_rx_queue;

int serial_rx_handle(int data);

extern util_queue_t * rx_queue_list ;

#endif

