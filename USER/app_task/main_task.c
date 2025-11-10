#include "main_task.h"
#include "base_event.h"

#define COMMAND_ID_LEN  2 
#define COMMAND_LEN     2 
#define DEVISE_SN_LEN  16
#define OTA_SIZE_LEN    4
#define PAGE_NUM_LEN    2
#define DATA_LEN        16           //3
uint8_t recvive_ok_flag = 0;        //接收成功标志

uint8_t recv_len ;

uint8_t check_sum1 = 0;            //消息体校验和

uint8_t recv_data = 0xff;
uint8_t board_data_info = 0;
uint8_t data_index = 0;
uint16_t message_id = 0;


uint8_t recv_data_buf[32] = {0x0}; 



void Station_send_ack(uint8_t ack,uint8_t cmd1)
{
	sent_buff[0] = 0x69;
	sent_buff[1] = 0x96;
	sent_buff[2] = 0x14;
	sent_buff[8] = 0x03;           //长度
	sent_buff[6] = cmd1 ;
	sent_buff[9] = check_sum(sent_buff,9);
	sent_buff[12] = ack ;
	sent_buff[10]=	check_sum(sent_buff+11,2) ;
	set_ir_sent_bite(13);
  set_ir_send_mode(IR_CODE_SEND_MODE_INFO);	
	if(ota_ok == 1)
	{
       set_ir_enable_flag(1,6);		
	}
	else
	{
       set_ir_enable_flag(1,2);	
	}	
}


void Station_send_verson(uint8_t cmd1)
{
	sent_buff[0] = 0x69;
	sent_buff[1] = 0x96;
	sent_buff[2] = 0x14;
	sent_buff[8] = 0x03;           //长度
	sent_buff[6] = cmd1 ;
	sent_buff[9] = check_sum(sent_buff,9);
	sent_buff[12] = version[1] ;
	sent_buff[10]=	check_sum(sent_buff+11,2) ;
	set_ir_sent_bite(13);
    set_ir_send_mode(IR_CODE_SEND_MODE_INFO);	
    set_ir_enable_flag(1,5);		
}


uint16_t time_cnt = 0;
uint8_t ota_ok = 0;
void test_data_process(uint8_t* recv_data_buf,uint8_t len)
{
    time_cnt++;
    if(board_data_info!=0 && queue_is_empty(rx_queue_list) && time_cnt>50000)
    {
        time_cnt = 0;
        data_index = 0;
        board_data_info = 0;
		recv_len = 0;
    }
    while(queue_is_empty(rx_queue_list) == 0)
    {
        time_cnt = 0;
        recv_data = queue_get(rx_queue_list);
		if( data_index >=32)
		{
		    data_index = 0;
		}
        switch(board_data_info)
        {
        case 0:
            /*byte0 verification: 0xa5*/
            if(recv_data == 0x69)              //ota相关指令
            {
				data_index = 0;
                recv_data_buf[data_index++] = recv_data;
                board_data_info ++;
//				recv_len = 0;
            }
			else                              //控制基站工作相关指令
			{
			    recv_data_buf[data_index++] = recv_data;
          board_data_info =15;
			}
            break;

        case 1:
            /*byte1 verification: 0x5a*/
            if(recv_data == 0x96)
            {
                recv_data_buf[data_index++] = recv_data;
                board_data_info ++;
            }
            else
            {
                data_index = 0;
                board_data_info = 0;
            }
            break;

        case 2:             //协议类型   0x14
            /*byte2 data_length*/
            recv_data_buf[data_index++] = recv_data;
            board_data_info ++;
            break;

        case 3:             //协议版本      0
            /*byte3 data_cmd*/
            recv_data_buf[data_index++] = recv_data;
            board_data_info ++;
            break;

        case 4:            //消息序号
            /*byte4 data*/
            recv_data_buf[data_index++] = recv_data;
            board_data_info ++;
            break;

        case 5:           //命令id
            /*byte4 data*/
            recv_data_buf[data_index++] = recv_data;
		    recv_len++ ;
		    if(recv_len >=COMMAND_ID_LEN)
			{
				recv_len = 0;
			    board_data_info ++;
				message_id = recv_data_buf[5]<<8|recv_data_buf[6];
			}
            break;

        case 6:          //消息长度
			recv_data_buf[data_index++] = recv_data;
		    recv_len++ ;
		    if(recv_len >=COMMAND_LEN)
			{
				recv_len = 0;
			    board_data_info ++;
			}
            break;
		case 7:         //消息头校验
		{
		    /*checksum and process data*/
            recv_data_buf[data_index++] = recv_data;
            if(check_sum(recv_data_buf,9) == recv_data_buf[data_index-1])      //头校验通过
            {
				board_data_info ++;
            }
            else
            {
				data_index = 0;
                board_data_info = 0;
            }   
		}break;
		case 8:        //消息体校验
		{
			check_sum1 = recv_data ;
		    recv_data_buf[data_index++] = recv_data;
            board_data_info ++;
		}break ;
		case 9:          //设备类型
		{
		     recv_data_buf[data_index++] = recv_data;
			 board_data_info ++;
		}break;
		case 10:
		{
		     recv_data_buf[data_index++] = recv_data;
			 if(recv_data == 0)    //ota类型
			 {
			     board_data_info ++;
				 ota_ok = 0;
			 }
			 else if(recv_data == 0x10)          //以32字节传输
			 {
			     board_data_info+=3;
				 ota_ok = 0;
			 }
			 else if(recv_data == 0x04)
			 {
                 recvive_ok_flag = 1 ;               //升级成功
				 ota_ok = 1;
				 data_index = 0;
                 board_data_info = 0;
			 }
			 else
			 {
			      data_index = 0;
                  board_data_info = 0;
				  ota_ok = 0;
			 }
		}break ;
		case 11:
		{
		     recv_data_buf[data_index++] = recv_data;
			 board_data_info ++ ;
		}break ;
		case 12:             //固件大小          
		{
			 recv_data_buf[data_index++] = recv_data;
		     recv_len ++ ;
			 if(recv_len>=OTA_SIZE_LEN)
			 {
				  if(check_sum(recv_data_buf+11,7) == check_sum1)        //6
				  {
                       recvive_ok_flag = 1;                              //接收到固件大小
					   if(message_id == 0x16)
					   {
						   timeout = timer_ms();
						   set_base_work_mode(WORK_MODE_IR_OTA);
						   Firmware_size = recv_data_buf[14]<<24|recv_data_buf[15]<<16|recv_data_buf[16]<<8|recv_data_buf[17] ;
					   }
				  }
				  recv_len = 0; 
			      data_index = 0;
                  board_data_info = 0;
			 }
		}break ;
		case 13:               //数据包序号及补码
		{
		     recv_data_buf[data_index++] = recv_data;
		     recv_len ++ ;
			 if(recv_len>=PAGE_NUM_LEN)
			 {
                 recv_len = 0;
				 board_data_info++ ;
				 Numbur = recv_data_buf[13] ;
			 }
		}break ;
		case 14:              //数据
		{
		     recv_data_buf[data_index++] = recv_data;
		     recv_len ++ ;
			 if(recv_len>=DATA_LEN)
			 {
                 recv_len = 0;
				 board_data_info = 0;
				 data_index = 0;
				 if(check_sum(recv_data_buf+11,20) == check_sum1)          //校验通过，写数据
				 {
				     recvive_ok_flag = 1;	                               //接收到数据
				 }
			 }
		}break ;
		case 15:
		{
			 recv_data_buf[data_index++] = recv_data;
             board_data_info++;
		}break ;
		case 16:
		{
			 recv_data_buf[data_index++] = recv_data;
		     data_index = 0;
             board_data_info =0;
			 if(((recv_data_buf[0]+recv_data_buf[1])&0xff) ==recv_data_buf[2])
			 {
				 recvive_ok_flag = 1 ;
         if(recv_data_buf[0] == 0x0e  && recv_data_buf[1] == 0x00)
				 {
	             set_light_twinkle_time(0);
  				     set_base_work_mode(WORK_MODE_IDEL);
				 }					 
		     }			 
			 return ;
		}break;
        }
    }            
}

