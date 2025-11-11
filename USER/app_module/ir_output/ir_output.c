#include "ir_output.h"

uint8_t ir_send_count = 0;         //发送数据次数

uint8_t ir_sent_bite = 0;          //发送数据长度

uint32_t tick_num;

uint32_t light_twinkle_time_get = 0 ;

static uint64_t ir_code_conver_dock(uint8_t ir_code) ;

typedef enum
{
    IR_OUTPUT_LEFT    	   = 0,    //发送左边信号
    IR_OUTPUT_LEFT_MIDDLE  = 1,    //发送左中信号
	  IR_OUTPUT_RIGHT_MIDDLE = 2,    //发送右中信号
	  IR_OUTPUT_RIGHT		   = 3,	   //发送右边信号
}IR_OUTPUT_E;


///定义IR信号发送顺序变量: (左+右) -> (左中+右中) -> 近卫信号
IR_OUTPUT_E ir_output_step = IR_OUTPUT_LEFT_MIDDLE;
///定义IR信息
ir_info_t ir_info = {IR_CODE_SEND_MODE_BACK,0x00};



uint32_t timer_ms(void)
{
	return tick_num;
}

uint32_t timer_elapsed(uint32_t num)
{
	return (tick_num - num);
}


void ir_output_init(void)
{
    /*38KHz/50%红外载波输出*/
    tim_ir_pwm_cfg (1266,0);     
    /*GPIO初始化*/
    gpio_output_cfg(CW_GPIOB,GPIO_PIN_5);        //左红外信号
    gpio_output_cfg(CW_GPIOB,GPIO_PIN_6);        //左中红外信号
    gpio_output_cfg(CW_GPIOA,GPIO_PIN_1);        //右中红外信号
    gpio_output_cfg(CW_GPIOA,GPIO_PIN_2);        //右中红外信号
}

uint8_t get_ir_enable_flag(void)
{
    return ir_info.enable;
}

///////////////////////////////////////////////////////////////////////////////////

void set_ir_enable_flag(uint8_t flag,uint8_t count )
{
	ir_send_count = count ;
	ir_info.enable = flag;
}

extern WORK_MODE_FULL_GO_STEP FULL_GO_STEP ;


void SysTick_Handler(void)
{
	static uint8_t index = 0;
  tick_num++;
	{
		if(light_twinkle_time == 0)
		{
      light_twinkle_time_get = 0;
			index = 0;
		}
		else
		{
			switch(index)                        //闪烁
			{
				case 0 :
				{
					 light_twinkle_time_get = light_twinkle_time;
					 index = 1;
				}break ;
				case 1:
				{
					 light_twinkle_time_get--;
					 LED_display_light_twinkle(light_show_mode);        //0:白灯   ；1:红灯
					 if(light_twinkle_time_get == 0)
					 {
						 index = 0;
						 set_light_twinkle_time(0);
					 }
				}break ;
			}
		}
    }
}


uint64_t cnt_us=0; 
uint64_t timer_us(void)
{
	return cnt_us;
}

uint64_t timer_elapsed_us(uint64_t num)
{
	return (cnt_us - num);
}


void set_ir_sent_bite(uint8_t count )
{
   ir_sent_bite = count;
}



//红外发送数据缓冲区
uint8_t sent_buff[13] = {0};

uint64_t ir1 = 0;
uint64_t ir2 = 0;
uint64_t ir3 = 0;
uint64_t ir4 = 0;

//void ATIM_IRQHandler(void)
//{
//		static uint16_t index = 0;
//		static uint16_t time_cnt = 0;
//		static uint8_t a = 0;
//		static uint64_t cnt_time_100us = 0;
//		cnt_us++;
//    if (ATIM_GetITStatus(ATIM_STATE_UIF))
//    {
//        ATIM_ClearITPendingBit(ATIM_STATE_UIF);
////		if(charge_complate == 0)
//				{
//					if(light_twinkle_time == 0)
//					{
//						if(get_base_work_mode() == WORK_MODE_IDEL && (FULL_GO_STEP == WORK_MODE_FULL_GO_IDEL))       //&&charge_complate==0
//						{
//							if(dust_bag_state == DUST_BAG_STATE_UNSTALL)
//							{
//								set_light_twinkle_time(0XFFFFFFFF);
//							}
//							else
//							{
//								if(get_robot_at_dock_state() == ROBOT_STATE_AT_DOCK)
//								{
//									 led_breath(); 
//								}
//								else
//								{
//									 LED_ON(); 
//								}
//							}
//						}
//						else
//						{
//							 LED_OFF();	
//						}
//						}
//					}		
//		if((get_ir_enable_flag() != 1))
//		{
//			ir_detect_capture();
//			IR_MIDDLE_LEFT_OFF;
//			IR_MIDDLE_RIGHT_OFF;
//			time_cnt = 0;
//			ir_output_step = IR_OUTPUT_LEFT_MIDDLE;
//			return;
//		}		
//      time_cnt ++;
//      if(ir_info.mode == IR_CODE_SEND_MODE_BACK)
//      {
//        /*未使能发送引导码功能则不执行以下代码*/
//        if(time_cnt <= 280)
//        {
//            if(time_cnt == 1)
//            {
//                if(ir_output_step == IR_OUTPUT_LEFT)
//                {
//                    ir1 = ir_code_conver_dock(0x50);  
//                }
//                else if(ir_output_step == IR_OUTPUT_LEFT_MIDDLE)
//                {
//                    ir1 = ir_code_conver_dock(0x44); 
//                }
//                else if(ir_output_step == IR_OUTPUT_RIGHT_MIDDLE)
//                {
//                    ir1 = ir_code_conver_dock(0x42);			
//                }   
//                else if(ir_output_step == IR_OUTPUT_RIGHT)
//                {
//                    ir1 = ir_code_conver_dock(0x48);				
//                }
//            }
//#if 1
//            {
//                switch(ir_output_step)
//                {
////                case IR_OUTPUT_LEFT:
////                    a = ir1 & 0x01;
////                    if(a > 0)
////                        IR_LEFT_OFF;
////                    else
////                        IR_LEFT_ON;
////                    if((time_cnt%7) == 0)
////                    {
////                        ir1 >>= 1;
////                    }
////                    break;

////                case IR_OUTPUT_LEFT_MIDDLE:
////                    a = ir1 & 0x01;
////                    if(a > 0)
////                        IR_MIDDLE_LEFT_OFF;
////                    else
////                        IR_MIDDLE_LEFT_ON;
////                    if((time_cnt%7) == 0)
////                    {
////                        ir1 >>= 1;
////                    }
////                    break;
////										
//                case IR_OUTPUT_RIGHT_MIDDLE:
//                    a = ir1 & 0x01;
//                    if(a > 0)
//                        IR_MIDDLE_RIGHT_OFF;
//                    else
//                        IR_MIDDLE_RIGHT_ON;
//                    if((time_cnt%7) == 0)
//                    {
//                        ir1 >>= 1;
//                    }
//                    break;
//////										
////                case IR_OUTPUT_RIGHT:
////                    a = ir1 & 0x01;
////                    if(a > 0)
////                        IR_RIGHT_OFF;
////                    else
////                        IR_RIGHT_ON;
////                    if((time_cnt%7) == 0)
////                    {
////                        ir1 >>= 1;
////                    }
////                    break;

////                    break;
////                default:break;
//                }
//            }
//#elif
//						
//#endif
//        }
//        else if(time_cnt == 281)
//        {
//            IR_LEFT_OFF;
//            IR_RIGHT_OFF;
//            IR_MIDDLE_LEFT_OFF;
//            IR_MIDDLE_RIGHT_OFF;
////            if(ir_output_step == IR_OUTPUT_RIGHT)
////                ir_output_step = IR_OUTPUT_LEFT;
////            else
////                ir_output_step += 1;
//	          if(ir_output_step == IR_OUTPUT_RIGHT_MIDDLE)
//                ir_output_step = IR_OUTPUT_RIGHT_MIDDLE;
//            else
//                ir_output_step = IR_OUTPUT_RIGHT_MIDDLE;
//        }
//        else if(time_cnt > 421-80)
//        {
//            time_cnt = 0;
//        }
//      }
//      /*发送通讯码值模式*/
//      else if(ir_info.mode == IR_CODE_SEND_MODE_INFO)
//      {
//           	switch(index)
//            {
//				case 0:
//				{
//					 cnt_time_100us=cnt_us;
//					 index=1;
//				}break;
//				case 1:
//				{
//					 if((cnt_us - cnt_time_100us)>=10)
//					 {
//						 cnt_time_100us=cnt_us;
//						 if(ir_sent_buf(sent_buff,ir_sent_bite))
//						 {
//							   index=2;
//							   cnt_time_100us=cnt_us;
//						 }
//					 }
//				}break;
//				case 2:
//				{
//					 IR_MIDDLE_LEFT_OFF;	
//					 IR_MIDDLE_RIGHT_OFF ;
//					 if((cnt_us - cnt_time_100us)>=300)				
//					 {
//						  index=0;							 
//					 }
//				}break;
//            }
//      }
//   }
//}



void ATIM_IRQHandler(void)
{
		static uint16_t index = 0;
		static uint16_t time_cnt = 0;
		static uint8_t a = 0;
		static uint8_t b = 0;
		static uint8_t c = 0;
		static uint8_t d = 0;
		static uint64_t cnt_time_100us = 0;
		cnt_us++;
    if (ATIM_GetITStatus(ATIM_STATE_UIF))
    {
        ATIM_ClearITPendingBit(ATIM_STATE_UIF);
//		if(charge_complate == 0)
				{
					if(light_twinkle_time == 0)
					{
						if(get_base_work_mode() == WORK_MODE_IDEL && (FULL_GO_STEP == WORK_MODE_FULL_GO_IDEL))       //&&charge_complate==0
						{
							if(dust_bag_state == DUST_BAG_STATE_UNSTALL)
							{
								set_light_twinkle_time(0XFFFFFFFF);
							}
							else
							{
								if(get_robot_at_dock_state() == ROBOT_STATE_AT_DOCK)
								{
									 led_breath(); 
								}
								else
								{
									 LED_ON(); 
								}
							}
						}
						else
						{
							 LED_OFF();	
						}
						}
					}		
		if((get_ir_enable_flag() != 1))
		{
			ir_detect_capture();
			IR_MIDDLE_LEFT_OFF;
			IR_MIDDLE_RIGHT_OFF;
			time_cnt = 0;
			ir_output_step = IR_OUTPUT_LEFT_MIDDLE;
			return;
		}		
      time_cnt ++;
      if(ir_info.mode == IR_CODE_SEND_MODE_BACK)
      {
        /*未使能发送引导码功能则不执行以下代码*/
#if 0
        if(time_cnt <= 280)
        {
            if(time_cnt == 1)
            {
                if(ir_output_step == IR_OUTPUT_LEFT)
                {
                    ir1 = ir_code_conver_dock(0x50);  
                }
                else if(ir_output_step == IR_OUTPUT_LEFT_MIDDLE)
                {
                    ir1 = ir_code_conver_dock(0x44); 
                }
                else if(ir_output_step == IR_OUTPUT_RIGHT_MIDDLE)
                {
                    ir1 = ir_code_conver_dock(0x42);			
                }   
                else if(ir_output_step == IR_OUTPUT_RIGHT)
                {
                    ir1 = ir_code_conver_dock(0x48);				
                }
            }
            {
                switch(ir_output_step)
                {
                case IR_OUTPUT_LEFT:
                    a = ir1 & 0x01;
                    if(a > 0)
                        IR_LEFT_OFF;
                    else
                        IR_LEFT_ON;
                    if((time_cnt%7) == 0)
                    {
                        ir1 >>= 1;
                    }
                    break;

                case IR_OUTPUT_LEFT_MIDDLE:
                    a = ir1 & 0x01;
                    if(a > 0)
                        IR_MIDDLE_LEFT_OFF;
                    else
                        IR_MIDDLE_LEFT_ON;
                    if((time_cnt%7) == 0)
                    {
                        ir1 >>= 1;
                    }
                    break;
										
                case IR_OUTPUT_RIGHT_MIDDLE:
                    a = ir1 & 0x01;
                    if(a > 0)
                        IR_MIDDLE_RIGHT_OFF;
                    else
                        IR_MIDDLE_RIGHT_ON;
                    if((time_cnt%7) == 0)
                    {
                        ir1 >>= 1;
                    }
                    break;
										
                case IR_OUTPUT_RIGHT:
                    a = ir1 & 0x01;
                    if(a > 0)
                        IR_RIGHT_OFF;
                    else
                        IR_RIGHT_ON;
                    if((time_cnt%7) == 0)
                    {
                        ir1 >>= 1;
                    }
                    break;

                    break;
                default:break;
                }
            }
					}
        else if(time_cnt == 281)
        {
            IR_LEFT_OFF;
            IR_RIGHT_OFF;
            IR_MIDDLE_LEFT_OFF;
            IR_MIDDLE_RIGHT_OFF;
            if(ir_output_step == IR_OUTPUT_RIGHT)
                ir_output_step = IR_OUTPUT_LEFT;
            else
                ir_output_step += 1;
        }
        else if(time_cnt > 421-80)
        {
            time_cnt = 0;
        }
#elif 2
      if(time_cnt <= 280)
      {
					if(time_cnt == 1)
					{
							ir1 = ir_code_conver_dock(0x48);
							ir2 = ir_code_conver_dock(0x44); 
							ir3 = ir_code_conver_dock(0x42);
							ir4 = ir_code_conver_dock(0x50);
					}
					a = ir1 & 0x01;
					b = ir2 & 0x01;
					c = ir3 & 0x01;
					d = ir4 & 0x01;
					
					if(a > 0) IR_LEFT_OFF;
					else IR_LEFT_ON;
					
					if(b > 0) IR_MIDDLE_LEFT_OFF;
					else IR_MIDDLE_LEFT_ON;
					
					if(c > 0) IR_MIDDLE_RIGHT_OFF;
					else IR_MIDDLE_RIGHT_ON;
					
					if(d > 0) IR_RIGHT_OFF;
					else IR_RIGHT_ON;
								
					if((time_cnt%7) == 0)
					{
						ir1 >>= 1;
						ir2 >>= 1;
						ir3 >>= 1;
						ir4 >>= 1;
					}
        }
        else if(time_cnt == 281)
        {
            IR_LEFT_OFF;
            IR_RIGHT_OFF;
            IR_MIDDLE_LEFT_OFF;
            IR_MIDDLE_RIGHT_OFF;
        }
        else if(time_cnt > 460)
        {
            time_cnt = 0;
        }
#endif
      }
      /*发送通讯码值模式*/
      else if(ir_info.mode == IR_CODE_SEND_MODE_INFO)
      {
           	switch(index)
            {
				case 0:
				{
					 cnt_time_100us=cnt_us;
					 index=1;
				}break;
				case 1:
				{
					 if((cnt_us - cnt_time_100us)>=10)
					 {
						 cnt_time_100us=cnt_us;
						 if(ir_sent_buf(sent_buff,ir_sent_bite))
						 {
							   index=2;
							   cnt_time_100us=cnt_us;
						 }
					 }
				}break;
				case 2:
				{
					 IR_MIDDLE_LEFT_OFF;	
					 IR_MIDDLE_RIGHT_OFF ;
					 if((cnt_us - cnt_time_100us)>=300)				
					 {
						  index=0;							 
					 }
				}break;
            }
      }
   }
}


///设置红外发射码模式,ir_code仅在 mode 为 IR_CODE_SEND_MODE_INFO 时有效
void set_ir_send_mode(IR_CODE_SEND_MODE_E mode)
{
    ir_info.mode = mode;
}

IR_CODE_SEND_MODE_E get_ir_send_mode(void)
{
    return ir_info.mode;
}



uint32_t ir_code_conver_data(uint8_t data)       //0x00           100  10                1010 1010 1010 1010 //1a           0101 0101 0010 0101 0010
{
    uint32_t a=0;
	uint8_t i=0;
	for(i=0;i<8;i++)
	{
	    if(data&0x80)
		{
			a=((a<<3)|0x04);  //100
		}
        else
		{
			a=(a<<2)|0x02;    //10
		}
        data<<=1;				
	}
	return a;
}



uint8_t sent_head(void)  
{
	  uint8_t head_data=0xf8;      //11111000
	  static uint8_t i=0;
    if((head_data<< i)&0x80)
	  {
            IR_MIDDLE_LEFT_ON;
            IR_MIDDLE_RIGHT_ON;
	  }
	  else
	  {
            IR_MIDDLE_LEFT_OFF;
            IR_MIDDLE_RIGHT_OFF;		  
	  }
	  i++;
	  if(i>7)
	  {
		   i=0;
		   return 1;
	  }
	  return 0;
}


uint8_t sent_tail(void)        //11110000
{
	  uint8_t tail_data=0xf0;
	  static uint8_t i=0;
	  if((tail_data<<i)&0x80)
	  {
            IR_MIDDLE_LEFT_ON;
            IR_MIDDLE_RIGHT_ON;
	  }
	  else
	  {
            IR_MIDDLE_LEFT_OFF;
            IR_MIDDLE_RIGHT_OFF;
	  }
	  i++;
	  if(i>7)
	  {
		  i=0;
          IR_MIDDLE_LEFT_OFF;
          IR_MIDDLE_RIGHT_OFF;
		  return 1;
	  }
	  return 0;
}

uint8_t ir_sent_buf(uint8_t buf[],uint8_t len)
{
      static uint8_t index=0;
	  static uint32_t data=0;
	  static uint8_t i=0;
	  static uint8_t n=0;
	  uint8_t j=0;
      if(ir_send_count == 0)
      {
		  ir_info.enable = 0;
          return 0;
      }
	  switch(index)
	  {
		   case 0:            //发送数据头
		   {
			   if(sent_head())
			   {
				   index=1;
				   data=ir_code_conver_data(buf[0]);           //1a 00011010 //16-24
				   for(j=0;j<8;j++)
				   {
						if((buf[0]<<j)&0x80)
						{
							 n+=3;
						}
						else
						{
							 n+=2;
						}
					}
			  }				
		  }break;
		  case 1:               //发送数据
		  {
			  if(n)
			  {
				  n--;
				  if(data&(0x01<<n))
				  {
						IR_MIDDLE_LEFT_ON;
						IR_MIDDLE_RIGHT_ON;
				  }
		  		  else
				  {
					   IR_MIDDLE_LEFT_OFF;
                       IR_MIDDLE_RIGHT_OFF;
				  }
			  }
			  if(n==0)
			  {
				  i++;
				  if(i<=len-1)
				  {
					  for(j=0;j<8;j++)
					  {
						  if((buf[i]<<j)&0x80)
						  {
							  n+=3;
						  }
						  else
						  {
							  n+=2;
						  }
					  }
					  data=ir_code_conver_data(buf[i]);
				  }
				  else if(i>=len)
				  {
					  index=2;
				  }
			  }
			}break;
			case 2:
			{
			     if(sent_tail())         //发送数据尾
				 {
					 ir_send_count --;
				     index=0;
					 i=0;
					 return 1;
				 }
			}break;
		}
		return 0;
}


static uint64_t ir_code_conver_dock(uint8_t ir_code)
{
    uint8_t i = 0;
    uint64_t ret = 0;
    for(i=0;i<8;i++)
    {
        ret <<= 5;
        if(ir_code & 0x01)
            ret |= 0x10;
        else
            ret |= 0x1e;      
        ir_code >>= 1;
    }
    return ret;
}
