#include "task_base_state_process.h"
#include "base_state.h"
#include "ir_output.h"
#include "pwm_data_output.h"
#include "base_event.h"
#include "main.h"
#include "vacuum_ctrl.h" 

void task_base_state_process(void)
{
    /*检测主机是否离开了集尘宝,若离开了集尘宝,则切换状态*/
    if(get_robot_at_dock_state() == ROBOT_STATE_NOT_AT_DOCK)
    {
        base_state= BASE_STATE_IDLE ;
    }  
	switch(base_state)
	{
	case BASE_STATE_IDLE:
		/*发送红外引导码*/
	{   
		/*把红外发码模式设置为: 发送红外引导码模式*/
		set_ir_send_mode(IR_CODE_SEND_MODE_BACK);
		/*使能红外发码*/
		ir_info.enable = 1;
	}
	if(get_robot_at_dock_state() == ROBOT_STATE_AT_DOCK)
	{
		/*检测到主机回到充电座上,打开充电*/
		base_state = BASE_STATE_CHARGING ;
	}
	break;

	case BASE_STATE_CHARGING:
		/*如果接收到主机集尘命令 && 距离上一次集尘完成时间超过 x mins*/
		if(get_base_event(BASE_EVENT_VACUUM_RM_STAR))
		{
//			if((get_time_for_dust_finish() > 5 * 100))  //|| is_already_vacuum == 0
			{
				if(dust_bag_state == DUST_BAG_STATE_UNSTALL)
				{
					sent_buff[0]=0x0b;
					sent_buff[1]=0x01;
					sent_buff[2]=0x0c;
				}
				else if(dust_bag_time >216000*1000)
				{
					 sent_buff[0]=0x0b;
					 sent_buff[1]=0x00;
					 sent_buff[2]=0x0b;
				}
				else
				{
					sent_buff[0]=0x0e;
					sent_buff[1]=0x02;
					sent_buff[2]=0x10;
					need_duty = 1 ;	
				}	
                set_ir_sent_bite(3);
				/*把发码模式配置成通讯模式*/
				set_ir_send_mode(IR_CODE_SEND_MODE_INFO);
				/*切换状态*/				
				/*使能红外发码*/
				set_ir_enable_flag(1,5);
			}
		}
		else if (get_base_event(BASE_EVENT_CHARGE_ON))
		{
		    charge_on_flag = 1 ;
			sent_buff[0]=0xba;
			sent_buff[1]=0x01;
			sent_buff[2]=0xbb;	
            set_ir_sent_bite(3);
			/*把发码模式配置成通讯模式*/
			set_ir_send_mode(IR_CODE_SEND_MODE_INFO);
			/*切换状态*/				
			/*使能红外发码*/
			set_ir_enable_flag(1,8);
		}
		else
		{
			if(get_ir_enable_flag() == 1&&ir_info.mode == IR_CODE_SEND_MODE_BACK)
			{   
				/*使能红外发码*/
				ir_info.enable = 0;      //充电不发码
			}
		}
		break;    
	}
}



















