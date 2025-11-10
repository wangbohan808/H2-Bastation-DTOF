/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 CW.
  * All rights reserved.</center></h2>
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "base_types.h"
#include "cw32l010.h"
#include "cw32l010_atim.h"
#include "cw32l010_gpio.h"
#include "cw32l010_sysctrl.h"
#include "interrupts_cw32l010.h"
#include "system_cw32l010.h"
#include "cw32l010_flash.h"
#include "cw32l010_iwdt.h"
#include "cw32l010_adc.h"
#include "cw32l010_btim.h"
#include "cw32l010_gtim.h"
#include "cw32l010_systick.h"
#include "util_queue_def.h"
#include "util_queue_interface.h"
#include "adc_cfg.h"
#include "gpio_cfg.h"
#include "adc_capture.h"
#include "ir_capture.h"
#include "iwdg_reset.h"
#include "main_task.h"
#include "lighting_ctrl.h"
#include "vacuum_ctrl.h"
#include "base_state.h"
#include "ir.h"
#include "ir_decoder_io_level.h"
#include "app_config.h"
#include "ir_output.h"
#include "pwm_data_output.h"
#include "main_task.h"
#include "board_key.h"
#include "ozone_ctrl.h"
#include "serial.h"
#include "tim_cfg.h"
#include "iwdg_reset.h"
#include "task_base_self_det.h"
#include "task_base_state_process.h"
#include "task_data_process.h"
#include "task_lighting_ctrl.h"
#include "task_base_mode_process.h"
#include "test_data_process.h"
#include "ir_ota.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
/* USER CODE END Private defines */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT CW *****END OF FILE****/
