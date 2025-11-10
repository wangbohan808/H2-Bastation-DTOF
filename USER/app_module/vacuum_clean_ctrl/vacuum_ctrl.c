#include "vacuum_ctrl.h"
#include "base_event.h"


static void delay_us(uint16_t us);

/*此变量用于过零中断触发标志*/
uint8_t vacuum_zero_check_flg =  0; 
/*此变量用于设置集尘时间*/
uint8_t dust_absorption_time = 30;
/*此变量用于存储交流电的频率*/
uint8_t ac_frequency = 0;
/*此变量用于判断是否需要集尘*/
uint8_t need_duty = 0;
/*集尘完成时间,用于限定短时间内不重复集尘*/
uint32_t time_for_dust_finish = 0;

int raise_time3 = 0;
int raise_time4 = 0;
uint8_t filtering_flg = 0;
volatile uint8_t gFlagIrq;

void ADC_IRQHandler(void)
{
	static uint32_t time_cnt = 0;
    /* Clear ADC AWD pending interrupt bit */
//    ADC_INTFlag_Status_Clear(ADC_INT_FLAG_AWDG);
	ADC_GetITStatusAll(&gFlagIrq);
	ADC_ClearITPendingAll();
	if(ac_frequency == 0)
	{
		switch(filtering_flg)
		{
		case 0:
			raise_time3 = timer_ms();
			filtering_flg ++;
		  time_cnt = timer_ms();
			break;

		case 1:
			if(timer_elapsed(time_cnt) > 5)
			{
				time_cnt = timer_ms();
				raise_time4 = timer_elapsed(raise_time3);
				if(abs(raise_time4-10)<1)
				{
					ac_frequency = 50;
				}
				else if(abs(raise_time4-8)<1)
				{
					ac_frequency=60;
				}
				else
				{
					filtering_flg = 0;
				}
		    }
			break;
		}
	}
	else
	{
		if(filtering_flg == 1)
		{
				filtering_flg = 0;
				time_cnt = timer_ms();
				vacuum_zero_check_flg = 1 ;
		}
		else
		{
			if(timer_elapsed(time_cnt) > 5)
			{
				 time_cnt = timer_ms();
         vacuum_zero_check_flg = 1;			
			}
		}
	}	
}


#if 1
uint16_t rising_time = 600;
void dust_absorption_ctrl(uint8_t ctrl)
{
	  static uint32_t time_cnt = 0;
    static uint16_t zero_check_cnt = 0;
	  /*为了让用户能听清楚"开始集尘"语音再开始集尘,用此变量作为标识*/
	  static uint8_t collect_flag = 0;
    if(time_cnt < 1)
    {
        /*获取集尘开始时间 */
        time_cnt = timer_ms();
				if(ac_frequency == 50)
				{
						rising_time = 600 ;
				}
				else if(ac_frequency == 60)
				{
					 rising_time = 670 ;
				}
				vacuum_zero_check_flg = 0;
    }
    /*为了让用户听清楚“开始集尘语音,触发集尘后2s再开始集尘”*/
    if((collect_flag == 0 && timer_elapsed(time_cnt) > 3 * 1000)&&get_base_work_mode() != WORK_MODE_FULL_GO)
    {
        /*停止发送红外通讯码*/
        /*开始启动交流风机*/
        collect_flag = 1;
				vacuum_zero_check_flg = 0;
				time_cnt = timer_ms();
    }	  
    if(timer_elapsed(time_cnt) > (dust_absorption_time) * 1000 ||(dust_bag_state == DUST_BAG_STATE_UNSTALL&&get_base_work_mode() != WORK_MODE_FULL_GO))
//    if(timer_elapsed(time_cnt) > (dust_absorption_time) * 1000)
    {
        /*判断集尘超时,关闭集尘*/
        need_duty = 0 ;
        /*获取集尘完成的时间*/
        time_for_dust_finish = timer_ms();
        /*发布集尘完成消息*/
		if(get_base_work_mode() != WORK_MODE_FULL_GO)
		{
			sent_buff[0]=0x0e;
			sent_buff[1]=0x01;
			sent_buff[2]=0x0f;	
		}
		else           //产测模式集尘结束
		{
			sent_buff[0]=0x0b;
			sent_buff[1]=0x00;
			sent_buff[2]=0x0b;
			/*使能红外发码*/		
		}
		set_ir_sent_bite(3);	
		set_ir_enable_flag(1,3);      		
		set_ir_send_mode(IR_CODE_SEND_MODE_INFO);
  }	
//	/*实时检测机器人是否在位,如机器离开基站,立即停止集尘*/	
//	if(get_robot_at_dock_state() == ROBOT_STATE_NOT_AT_DOCK)
//	{
//		if(get_base_work_mode() != WORK_MODE_FULL_GO)
//		{
//			need_duty = 0;		
//		}
//	}
    if(need_duty < 1)
    {
		    dust_bag_time = dust_bag_time + timer_elapsed(time_cnt) ;
        /*重置各项参数*/
        time_cnt = 0;
		    collect_flag = 0;
        /*重置交流风机各项参数*/
        zero_check_cnt = 0;
		   /*停止启动交流风机*/
        set_gpio_level(CW_GPIOB,GPIO_PIN_4,0);
    }
#if 1
	if((ctrl == 1 && collect_flag == 1) || (get_base_work_mode() == WORK_MODE_FULL_GO && ctrl == 1))
    {
        /*检测到了零点*/
        if(vacuum_zero_check_flg == 1)
        {
            vacuum_zero_check_flg = 0;
            zero_check_cnt ++;
            /*缓启动,防止交流风机刚启动功率就非常大*/
			if((zero_check_cnt % 2) == 0) 
			{
				if(rising_time >215)             //3.9
				{
					if(ac_frequency == 50)
					{
					    rising_time -= 3;
					}
					else if(ac_frequency == 60)
					{
					     rising_time -= 2;
					}
				}
				else
				{
					rising_time = 215;
				}
			}
			if(rising_time > 215)
			{
				delay_10us(rising_time);				
				/*给交流设备输出一个脉冲信号*/
				set_gpio_level(CW_GPIOB,GPIO_PIN_4,1);
				delay_10us(10);
				set_gpio_level(CW_GPIOB,GPIO_PIN_4,0);	
			}
			else
			{
				set_gpio_level(CW_GPIOB,GPIO_PIN_4,1);
			}				
        }
#endif
    }
}
#endif




uint32_t get_time_for_dust_finish(void)
{
    return timer_elapsed(time_for_dust_finish);
}



void set_need_duty(uint8_t val)
{
	need_duty = val;
}

static void delay_us(uint16_t us)
{
    uint16_t i = 0;

    while(us--)
    {
        i = 4;			//4
        while(i--);
    }
}

void delay_10us(uint16_t us)
{
    while(us--)
    {
        delay_us(10);
    }
}
