#include "ir_ota.h"

uint32_t Firmware_size = 0;        //固件大小
uint16_t Numbur = 0;               //固件包序号
uint8_t page =0 ;                  //写进去的页数
uint32_t data_buff[4] = {0};



#define FLASH_PAGE_SIZE 512 

//#define app_update_flag_addr 		0x0001000	//地址要超出APP的内容范围。demo的bin内容在0x080054F8截止
//#define ir_ota_addr                 0x0001004


void Set_Firmware_size(uint32_t size)
{
      Firmware_size = size ;
}

uint32_t Get_Firmware_size()
{
     return Firmware_size ;
}

void Set_Numbur(uint16_t num)
{
     Numbur = num ;
}




uint8_t check_sum(uint8_t * arr ,uint8_t len)
{
    uint8_t sum = 0;
	for(int i = 0; i <len; i++)
	{
	    sum += arr[i] ;
	}
	return sum ;
}


uint32_t start_add1;
int32_t app_flash_write(uint32_t *data ,uint32_t Flash_address,uint16_t count)
{
//        uint32_t i;
		start_add1 = Flash_address;
		FLASH_UnlockAllPages();
//		for(i=0;i< count ;i++)
//		{
////			 FLASH_Word_Program(start_add1+i*4, data[i]);
//				if(FLASH_EOP != FLASH_WriteWords(start_add1+i*4, data[i]))		//写
//				{
//						FLASH_LockAllPages();
//						return 1;
//				}
//		}
		FLASH_WriteWords(start_add1,data,count);
		FLASH_LockAllPages();
		
    return 0;
}

int32_t app_flag_write(uint32_t data ,uint32_t start_add2)
{
		FLASH_UnlockAllPages();

		FLASH_ErasePage(start_add2);			//写之前先擦一遍，每次擦512Byte
	
//		if (FLASH_EOP != FLASH_WriteWords(start_add2, data))		//写
//		{
//				FLASH_LockAllPages();
//				return 1;
//		}
		FLASH_WriteWords(start_add2,&data,1);
		FLASH_LockAllPages();
		
		return 0;
}

int32_t app_flag_write1(uint32_t data ,uint32_t start_add2)
{
		FLASH_UnlockAllPages();
	
//		if (FLASH_EOP != FLASH_WriteWords(start_add2, data))		//写
//		{
//				FLASH_LockAllPages();
//				return 1;
//		}
	 	FLASH_WriteWords(start_add2,&data,1);
		FLASH_LockAllPages();
		
		return 0;
}


uint32_t timeout = 0 ;
uint16_t last_number = 0;
#define RECV_LEN      16
#define PAGE_COUNT    32
uint16_t count_data = 0;
uint8_t last_len = 0;
uint8_t recv_ota_data_ok = 0;
void OTA_Process(uint8_t* recv_buff,uint8_t len )
{
	if(recv_buff[13]+recv_buff[14] ==0xff)     //序号正确
	{
		if(Numbur == last_number+1)
		{
			if(Firmware_size-(page*512+(count_data)*RECV_LEN)>RECV_LEN)
			{
				for(int i = 0; i<4 ;i++)
				{
				   data_buff[i] = recv_buff[15+i*4] |recv_buff[16+i*4]<<8 |recv_buff[17+i*4] <<16 |recv_buff[18+i*4]<<24;
				}
				app_flash_write(data_buff,start_add+page*FLASH_PAGE_SIZE+count_data*RECV_LEN,4);
		  }
			else              //最后一包数据
			{               //          12152               11776                   23*16  12144
				last_len = RECV_LEN-(Firmware_size-(page*512+count_data*RECV_LEN)) ;
				for(uint8_t i = 0;i<last_len;i++)
				{
				     recv_buff[15+(Firmware_size-(page*512+count_data*RECV_LEN))+i] = 0xff;
				}
				for(int i = 0; i<4 ;i++)
				{
				   data_buff[i] = recv_buff[15+i*4] |recv_buff[16+i*4]<<8 |recv_buff[17+i*4] <<16 |recv_buff[18+i*4]<<24;
				}
				app_flash_write(data_buff,start_add+page*FLASH_PAGE_SIZE+count_data*RECV_LEN,4);
				recv_ota_data_ok = 1;
			}
			last_number = Numbur;
			if(last_number == 255)
			{
			    last_number = 0;
			}
			count_data ++;
			if(count_data>PAGE_COUNT-1)
			{
			    count_data = 0;
				page++;
			}
        }
    }
}


