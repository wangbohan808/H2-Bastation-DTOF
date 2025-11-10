#include "task_base_self_det.h"
#include "adc_capture.h"
#include "base_state.h"
#include "app_config.h"
#include "pwm_data_output.h"
#include "stdlib.h"
#include "vacuum_ctrl.h"
#include "adc_capture.h"


/*基站状态自检:尘袋在位,主机在位,主机离座*/
/*
 *base_module_state[0] : 尘袋在位计数
 *base_module_state[1] : 主机在位计数
 *base_module_state[2] : 主机离座计数
 *base_module_state[3] : 充电异常计数
*/
uint8_t base_module_state = 0x0;
uint8_t base_module_state1 = 0x0;
uint32_t at_dock_time = 0; 
uint32_t dust_bag_time = 0 ;
uint32_t left_time = 0;
uint8_t at_dock = 9;
uint16_t erp_count = 0;
uint8_t charge_on_flag = 0;
uint64_t tim1_cnt = 0;
uint8_t last_state = 0;
uint8_t state = 0;
void task_base_self_detect(void)
{
    static uint32_t sys_time_cnt = 0;
	if(timer_elapsed(sys_time_cnt)<5)
	{
	    return ;
	}
    sys_time_cnt = timer_ms();
    /*尘袋在位检测*/
    if(GPIO_ReadPin(CW_GPIOA ,GPIO_PIN_0) == 1)
    {
				dust_bag_state= DUST_BAG_STATE_UNSTALL;
				dust_bag_time = 0;
    }
    else
    {
		    dust_bag_state = DUST_BAG_STATE_STALL; 
				set_light_twinkle_time(0);
    }
//		state = robot_at_dock_state ;
//		if(state != last_state)
//		{
//		    last_state = state ;
//			  if(state == ROBOT_STATE_AT_DOCK)
//				{
//				     need_duty = 1;
//				}
//		}
	/*充电电流检测*/
	if(1)            //没有进入低功耗
	{
    if(charge_on_flag == 0)
		{			
			if((CW_ADC->RESULT2&0xfff) <3650  && (CW_ADC->RESULT2&0xfff) >820) 
			{
				if(base_module_state1 < 50 )
				{
					base_module_state1 ++;
				}
			}
			else
			{
				base_module_state1 = 0;
			}
			if(base_module_state1 >= 50 && robot_at_dock_state == ROBOT_STATE_NOT_AT_DOCK)
			{
			    charge_on();
				robot_at_dock_state = ROBOT_STATE_AT_DOCK ;
				base_module_state = 0;
				at_dock = 0;			
				at_dock_time = timer_ms();
			}
			//充电电流检测
			if(timer_elapsed(at_dock_time)>5*1000)
			{			
				if(get_changing_cur() < (CHARGE_DETECT_CUR_ROBOT_NOT_AT_DOCK + CHARGE_DETECT_CUR_ERROR_RANGE))
				{
					if(base_module_state < 200 )
					{
						base_module_state ++;
					}
				} 
				else
				{
					base_module_state = 0; 
				}
			}
			if(base_module_state >= 200 &&robot_at_dock_state == ROBOT_STATE_AT_DOCK)
			{
				  robot_at_dock_state =ROBOT_STATE_NOT_AT_DOCK ;
				  ir_dock_resync_init(&dock_decode);
				  charge_off();
				  if(get_base_work_mode()!=WORK_MODE_FULL_GO)
				  {
					 set_base_work_mode(WORK_MODE_IDEL);
					 set_light_twinkle_time(0);               //4.8改
				  }
				  at_dock_time = 0;
				  left_time = timer_ms();
				  base_module_state1 = 0;
			}
	  }
		else 
		{
			charge_on();
			if(get_changing_cur() < (CHARGE_DETECT_CUR_ROBOT_NOT_AT_DOCK + CHARGE_DETECT_CUR_ERROR_RANGE))
			{
				if(base_module_state < 200 )
				{
					base_module_state ++;
				}
			} 
			else
			{
				base_module_state = 0; 
			}
			if(base_module_state >= 200)
			{
				  robot_at_dock_state =ROBOT_STATE_NOT_AT_DOCK ;
				  ir_dock_resync_init(&dock_decode);
				  if(get_base_work_mode()!=WORK_MODE_FULL_GO)
				  {
					 set_base_work_mode(WORK_MODE_IDEL);
					 set_light_twinkle_time(0);               //4.8改
				  }
				  at_dock_time = 0;
				  if(left_time == 0)
				  {
					  left_time = timer_ms();
				  }
			}
			else if(base_module_state <1)
			{
				robot_at_dock_state= ROBOT_STATE_AT_DOCK; 
				at_dock = 0;
				if(at_dock_time == 0)	
				{				
					at_dock_time = timer_ms();
				}
			}
		}
		if(at_dock_time!=0)
		{
			if(timer_elapsed(left_time)<30*1000)
			{
				if(timeout && get_base_work_mode()!=WORK_MODE_IR_OTA)
				{
					 set_base_work_mode(WORK_MODE_IR_OTA);
					 set_light_twinkle_time(0xffffffff);
					 ir_info.enable = 0; 
				}
			}
			left_time = 0;
		}
  }
	if(tim1_cnt !=cnt_us)
	{
		   tim1_cnt = cnt_us ;
		   IWDT_Refresh();   
	}
    /*检测充电电流超过安全阈值*/
}




