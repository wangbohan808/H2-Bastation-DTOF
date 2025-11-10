#include "task_data_process.h"
#include "app_config.h"
#include "log.h"
#include "ir_capture.h"
#include "vacuum_ctrl.h"


uint8_t io_mode_change = 0;

uint8_t get_io_mode_change(void)
{
    return io_mode_change ;
}

