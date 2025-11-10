#include "task_base_mode_process.h"
#include "base_state.h"
#include "vacuum_ctrl.h"
#include "ir_output.h"
#include "pwm_data_output.h"
#include "vacuum_ctrl.h"
#include "task_base_self_det.h"
#include "task_base_state_process.h"
#include "base_event.h"
#include "ir_capture.h"
#include "lighting_ctrl.h"
#include "app_config.h"
#include "test_data_process.h"

uint8_t ir_test_step = 0;
uint32_t ir_sent_time = 0;
#define  IR_SENT_TIME   5
static void base_mode_full_go_process(void);
WORK_MODE_FULL_GO_STEP FULL_GO_STEP = WORK_MODE_FULL_GO_IDEL ;        //进入厂测模式 的步骤
//uint32_t enter_full_go_time = 0;                                      //进入第一步厂测模式的时间
TEST_FULL_GO TEST_FULL_GO_SEC = TEST_IDEL ;

uint32_t bin_crc_data = 0;


uint32_t FLASH_ReadByte(uint32_t address)
{
	return *(__IO uint8_t*)address; 
}

uint32_t crc32_update(uint32_t crc, const uint32_t start_addr_bin, uint32_t len) {
    crc = ~crc;  // 初始值取反（符合标准CRC32约定）
    for(int i =0 ;i<len;i++)
	{
        crc ^= FLASH_ReadByte(start_addr_bin+i);
        for(int i=0; i<8; i++) {
            crc = (crc & 1) ? (crc >> 1) ^ 0xEDB88320 : crc >> 1;
        }
    }
    return ~crc;  // 最终结果再次取反
}

void base_work_enter_full_go_step(void)
{
    static uint32_t time_cnt = 0;
    switch((u8)FULL_GO_STEP)
    {
         case WORK_MODE_FULL_GO_FIRST:
         { 				   
            if(get_ir_enable_flag())
            {
                 time_cnt = timer_ms();
            } 
            else
            {								
                if(get_base_event(BASE_EVENT_TEST_START1))
                {
                       FULL_GO_STEP = WORK_MODE_FULL_GO_SECOND ;
                       time_cnt = timer_ms();                    
                       /*把发码模式配置成通讯模式*/
					   sent_buff[0]=0x99;
					   sent_buff[1]=version[0];
					   sent_buff[2]=sent_buff[0]+sent_buff[1];
					   set_ir_sent_bite(3);
					   set_ir_enable_flag(1,3);
                }
                if(timer_elapsed(time_cnt)>10*1000)           //超时
                {
                     FULL_GO_STEP = WORK_MODE_FULL_GO_IDEL ;
                }
            }
         }break ;
         case WORK_MODE_FULL_GO_SECOND:
         {
			if(get_ir_enable_flag())
			{
			    time_cnt = timer_ms();
			} 
			else
			{							
				if(get_base_event(BASE_EVENT_TEST_START2))
				{
					  FULL_GO_STEP = WORK_MODE_FULL_GO_END ;
					  time_cnt = timer_ms();
					  /*把发码模式配置成通讯模式*/
					  sent_buff[0]=0x77;
					  sent_buff[1]=version[1];
					  sent_buff[2]=sent_buff[0]+sent_buff[1];					
					  set_ir_enable_flag(1,3);
					  set_ir_sent_bite(3);								
				}
				if(timer_elapsed(time_cnt)>10*1000)
				{
					  FULL_GO_STEP = WORK_MODE_FULL_GO_IDEL ;
				}
			}					     
         }break ;
         case WORK_MODE_FULL_GO_END:
         {
            if(get_ir_enable_flag() == 0)
            {
				  FULL_GO_STEP = WORK_MODE_FULL_GO_IDEL ;
				  set_base_work_mode(WORK_MODE_FULL_GO);
//				  TEST_FULL_GO_SEC = TEST_IDEL ;
				  set_light_twinkle_time(0);
				  LED_OFF();
				  TEST_FULL_GO_SEC = TEST_IR ;
				  ir_test_step = 0 ;
            }					      
         }break;
     }
}
uint16_t cmd = 0;
uint32_t timer_cnt = 0;
uint8_t sent_ok = 1;
uint8_t charge_complate ;
uint32_t charge_complate_time = 0;


void task_base_mode_process(void)
{  
	test_data_process(recv_data_buf,sizeof(recv_data_buf));
	if(get_base_work_mode() != WORK_MODE_IR_OTA ||message_id == 0x24 ||message_id == 0x18)
	{
		if(recvive_ok_flag == 1)
		{
			recvive_ok_flag = 0;
			timer_cnt = timer_ms();
			sent_ok = 0;
		}
		if(timer_elapsed(timer_cnt)>60 &&sent_ok == 0)
		{
			 sent_ok = 1;
			 if(message_id == 0)
			 {
				 uint32_t temp = 0;
				 cmd = recv_data_buf[0]<<8|recv_data_buf[1];
				 temp = ir_check_is_cmd(cmd);
				 if(temp != 0)
				 {
					 base_event |= (1 << (BASE_EVENT_E)temp);
				 }
		     }
			 else if(message_id == 0x18)
			 {
//			      Station_send_verson(message_id);
				  sent_buff[0] = 0x69;
				  sent_buff[1] = 0x96;
				  sent_buff[2] = 0x14;
				  sent_buff[8] = 0x03;           //长度
				  sent_buff[6] = message_id ;
				  sent_buff[9] = check_sum(sent_buff,9);
				  sent_buff[12] = version[1] ;
				  sent_buff[10]=	check_sum(sent_buff+11,2) ;
				  set_ir_sent_bite(13);
				  set_ir_send_mode(IR_CODE_SEND_MODE_INFO);	
				  set_ir_enable_flag(1,5);
				  page = 0;
				  timeout = 0;
				  last_number = 0;
				  count_data = 0;
//				  ota_reset();
			 }
			 else if(message_id == 0x24)
			 {
			      Station_send_ack(0,message_id);
				  bin_crc_data = recv_data_buf[14]<<24|recv_data_buf[15]<<16|recv_data_buf[16]<<8|recv_data_buf[17];
			 }
			 message_id = 0;
		}
    }
    switch(get_base_work_mode())
    {
        case WORK_MODE_IDEL:
        {
            if(get_base_event(BASE_EVENT_TEST_START))
            {						  
								FULL_GO_STEP = WORK_MODE_FULL_GO_FIRST ;
								set_ir_send_mode(IR_CODE_SEND_MODE_INFO);	
								sent_buff[0]=0xa6;
								sent_buff[1]=soft_version;
								sent_buff[2]=0xa6 + sent_buff[1];				
								set_ir_enable_flag(1,3);	
								set_ir_sent_bite(3);				
				        base_event = 0;
            }
            if(FULL_GO_STEP != WORK_MODE_FULL_GO_IDEL)
            {
				static uint32_t time_cnt = 0;
				switch((u8)FULL_GO_STEP)
				{
					 case WORK_MODE_FULL_GO_FIRST:
					 { 				   
						if(get_ir_enable_flag())
						{
							 time_cnt = timer_ms();
						} 
						else
						{								
							if(get_base_event(BASE_EVENT_TEST_START1))
							{
								   FULL_GO_STEP = WORK_MODE_FULL_GO_SECOND ;
								   time_cnt = timer_ms();                    
								   /*把发码模式配置成通讯模式*/
								   sent_buff[0]=0x99;
								   sent_buff[1]=version[0];
								   sent_buff[2]=sent_buff[0]+sent_buff[1];
								   set_ir_sent_bite(3);
								   set_ir_enable_flag(1,3);
							}
							if(timer_elapsed(time_cnt)>10*1000)           //超时
							{
								 FULL_GO_STEP = WORK_MODE_FULL_GO_IDEL ;
							}
						}
					 }break ;
					 case WORK_MODE_FULL_GO_SECOND:
					 {
						if(get_ir_enable_flag())
						{
							time_cnt = timer_ms();
						} 
						else
						{							
							if(get_base_event(BASE_EVENT_TEST_START2))
							{
								  FULL_GO_STEP = WORK_MODE_FULL_GO_END ;
								  time_cnt = timer_ms();
								  /*把发码模式配置成通讯模式*/
								  sent_buff[0]=0x77;
								  sent_buff[1]=version[1];
								  sent_buff[2]=sent_buff[0]+sent_buff[1];					
								  set_ir_enable_flag(1,3);
								  set_ir_sent_bite(3);								
							}
							if(timer_elapsed(time_cnt)>10*1000)
							{
								  FULL_GO_STEP = WORK_MODE_FULL_GO_IDEL ;
							}
						}					     
					 }break ;
					 case WORK_MODE_FULL_GO_END:
					 {
						if(get_ir_enable_flag() == 0)
						{
							  FULL_GO_STEP = WORK_MODE_FULL_GO_IDEL ;
							  set_base_work_mode(WORK_MODE_FULL_GO);
							  TEST_FULL_GO_SEC = TEST_IDEL ;
							  set_light_twinkle_time(0);
							  LED_OFF();
							  TEST_FULL_GO_SEC = TEST_IR ;
				              ir_test_step = 0 ;
						}					      
					 }break;
				 }
            }
            else 
            {
               task_base_state_process();  //设备状态处理
            }
        }
        break;

        case WORK_MODE_FULL_GO:
			set_ir_sent_bite(3);
            base_mode_full_go_process();
            break;

        case WORK_MODE_IR_OTA:
		{
			if(recvive_ok_flag == 1)
			{
                recvive_ok_flag = 0;
				timer_cnt = timer_ms();
				sent_ok = 0;
				if(message_id == 0x17)
				{
			        OTA_Process(recv_data_buf,sizeof(recv_data_buf));
				}		
			}
			if(timer_elapsed(timer_cnt)>90 &&sent_ok == 0)
			{
				sent_ok = 1;
				if(message_id == 0x16)
				{
					FLASH_UnlockAllPages();
					for(uint8_t i=0 ; i<25;i++)
					{
						FLASH_ErasePage(0x08004400+i*512);
					}
					FLASH_LockAllPages();
			        Station_send_ack(0,message_id);
					recv_ota_data_ok = 0;
					light_show_mode = 0;
					set_light_twinkle_time(0xffffffff);
				}
				else if(message_id == 0x17)
				{
				    Station_send_ack(6,message_id);
				}
				message_id = 0;
			}
			if(timer_elapsed(timer_cnt)>50*1000)   //接收超时
			{
				
				ota_ok = 0;
				set_base_work_mode(WORK_MODE_IDEL);
				set_light_twinkle_time(0);
				bin_crc_data = 0;
			}
			if(recv_ota_data_ok == 1)
			{
				switch(ota_ok)
				{
					case 1:
					{
						if(get_ir_enable_flag() == 1)
						{
							ota_ok =2;
						}
					}break ;
					case 2:
					{
						if(get_ir_enable_flag() == 0)           //bin文件校验
						{
                            if(crc32_update(0,start_add,Firmware_size) == bin_crc_data)   //bin文件校验正确
							{
							    ota_ok = 3 ;
							}
							else                                //bin文件校验错误
							{
								ota_ok = 0;
								set_base_work_mode(WORK_MODE_IDEL);
								set_light_twinkle_time(0);
								bin_crc_data = 0;
							}
						}
					}break;
					case 3:
					{
						 app_flag_write(0xffffffff,app_update_flag_addr) ;
						 app_flag_write1(0x87654321,ir_ota_addr);
						 NVIC_SystemReset();
					}break;
				}
		    }
		    if(timer_elapsed(timeout) > 3600*1000 && timeout!=0)
			{
				set_light_twinkle_time(0);
				set_base_work_mode(WORK_MODE_IDEL);
			}
		}break;
    }
}

uint8_t test_vacuun = 0;
DUST_BAG_STATE_E dustbug_state = DUST_BAG_STATE_UNSTALL;
static void base_mode_full_go_process(void)
{
	  //风机测试
    if(get_base_event(BASE_EVENT_TEST_VACUUN_START) == 1)          //进入风机测试
    {
			dust_absorption_time = 2 ;
			test_vacuun = 1;
    }
	//进入整机风机测试
	if(get_base_event(BASE_EVENT_TEST_VACUUN_START15) == 1)          //进入风机测试
    {
		dust_absorption_time = 8 ;
		test_vacuun =1 ;
    }
	if(test_vacuun == 1)
	{
	    test_vacuun = 0;
		LED_OFF();
		set_light_twinkle_time(0);
        if((get_time_for_dust_finish() > 5 * 1000))
        {
			sent_buff[0]=0x66;
			sent_buff[1]=ac_frequency;
			sent_buff[2]=0x66+sent_buff[1];
            /*使能红外发码*/
            set_ir_enable_flag(1,3);
            TEST_FULL_GO_SEC = TEST_VACUUN_TEST ;
            /*使能红外发码*/
        }
	}
	//尘袋测试
    if(get_base_event(BASE_EVENT_TEST_DUSTBUG) == 1)
    {
		dustbug_state = dust_bag_state;
		sent_buff[0]=0x67;
		sent_buff[1]=0x02-dustbug_state;
		sent_buff[2]=0x67+sent_buff[1];
        /*使能红外发码*/
        set_ir_enable_flag(1,3);	
        TEST_FULL_GO_SEC = TEST_DUSTBUG_TEST ;
    }
	//闪灯测试
    if(get_base_event(BASE_EVENT_TEST_LED_ON) == 1)
    {
	    set_light_twinkle_time(1*1000);
		light_show_mode = 0;
		sent_buff[0]=0x68;
		sent_buff[1]=0x02;
		sent_buff[2]=0x6a;
        /*使能红外发码*/
        set_ir_enable_flag(1,3);
        TEST_FULL_GO_SEC = TEST_LED_ON_TEST ;
    }
		//产生测试结束事件
	if(get_base_event(BASE_EVENT_TEST_END) == 1)
    {	  
		  sent_buff[0]=0xcd;
		  sent_buff[1]=0xdc;
		  sent_buff[2]=0xa9;
		  set_ir_enable_flag(1,3);
		  set_base_work_mode(WORK_MODE_IDEL);
		  TEST_FULL_GO_SEC = TEST_IDEL ;
		  base_event = 0;
          light_show_mode =1;		
    }
    //产生电流测试事件
	if(get_base_event(BASE_EVENT_TEST_CURRENT) == 1)
    {		  
		  sent_buff[0]=(0xd<<4)+(get_changing_cur()>>8);
		  sent_buff[1]=get_changing_cur();
		  sent_buff[2]=sent_buff[0]+sent_buff[1];
		  set_ir_enable_flag(1,3); 			
    }
	if(get_base_event(BASE_EVENT_TEST_IR_ERR) == 1)
	{
	     TEST_FULL_GO_SEC = TEST_IR ;
		 ir_test_step = 0;
	}
	switch((u8)TEST_FULL_GO_SEC)
	{
		case TEST_IR :
		{
			switch(ir_test_step)
			{
				case 0:
				{
				     ir_sent_time = timer_ms ();
					 ir_test_step = 1;
					 ir_info.enable = 1;
					 set_ir_send_mode(IR_CODE_SEND_MODE_BACK);	
				}break ;
				case 1:
				{
					  ir_info.enable = 1;
					  set_ir_send_mode(IR_CODE_SEND_MODE_BACK);	
					  if(timer_elapsed(ir_sent_time)>10*100)
					  {
                            ir_info.enable = 0;
							ir_test_step = 0;										  
							set_ir_send_mode(IR_CODE_SEND_MODE_INFO);	
							sent_buff[0] = 0x64 ;
						    sent_buff[1] = 0x00 ;
						    sent_buff[2] = 0x64 ;
							set_ir_enable_flag(1,3);
						    TEST_FULL_GO_SEC = TEST_IDEL ;
					  }
				}break;
			}
		}break;		
		case TEST_VACUUN_TEST:
		{
			if(get_ir_enable_flag() == 0)
			{
				need_duty = 1 ;
				TEST_FULL_GO_SEC = TEST_IDEL ;						  
			}
		}break ;
		case TEST_DUSTBUG_TEST :
		{
			if(dustbug_state != dust_bag_state)
			{
				dustbug_state = dust_bag_state;
				set_ir_enable_flag(1,3);
				sent_buff[0]=0x67;
				sent_buff[1]=0x02-dustbug_state;
				sent_buff[2]=0x67+sent_buff[1];
				TEST_FULL_GO_SEC = TEST_IDEL ;
			}
		}break ;
		case TEST_LED_ON_TEST:
		{
		   if(light_twinkle_time == 0)
			 {
			 	   set_light_twinkle_time(1000);
		       light_show_mode = !light_show_mode;
			 }
		}break;
	}
}

