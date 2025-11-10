#include "ir_decoder_io_level.h"
/// @file ir_decoder_dock.c
/// @brief ??????????????


remote_decode_t dock_decode;
//static remote_decode_t remote_decode[IR_POSITION_MAX];

///???????IO???????��??????8??
static uint8_t ir_rx_buffer[IR_QUEUE_LEN] = {0};

///?????????????
util_queue_t ir_rx_queue = {0};

///?????????IO???,??8bit???????
static uint8_t ir_working_byte;

///???IO????????
static uint8_t ir_capture_counter;


void ir_dock_resync_init(remote_decode_t * decode_p)
{
    decode_p->state = REMOTE_RESYNC;
    decode_p->timer = 0;
	decode_p->count = 0; 
	decode_p->decode_value = 0;
	decode_p->bits_count = 0;
}

/**
 * @fn uint16_t ir_dock_decode_init(uint16_t)
 * @brief ???????????
 *
 * @param instance ?????ID
 */
static void ir_dock_decode_init(uint16_t instance)
{
    ir_dock_resync_init(&dock_decode);
}


//#if(ENCODED_TYPE == ENCODED_TYPE_I)
/**
 * @fn int16_t ir_dock_decode_state(uint16_t, uint16_t)
 * @brief ????????????????????????,
 * ??????????????"1"?????????3ms,????????1ms; "0"?????????1ms,????????3ms
 *
 * @param instance ?????ID
 * @param ir_state ??????????????
 * @return -1:��??????????? 0:???????????
 */

static int16_t ir_dock_decode_state(uint16_t ir_state)
{
    remote_decode_t * decode_p;
    int16_t remoteStatus = -1;   
    decode_p = &dock_decode;
    decode_p->timer++;
    switch (decode_p->state)
    {
    case REMOTE_RESYNC: //????????		
		if(ir_state == 0)
		{
			ir_dock_resync_init(decode_p);
		}
		else if(decode_p->timer > DOCK_RESYNC_TICKS)//
		{
			decode_p->state = REMOTE_HEADER;
		}							
    break;

    case REMOTE_HEADER:
		if(ir_state == 0)
		{
			decode_p->count ++; //low 
			if(decode_p->count >= DOCK_HEADER_LOW)
			{
				decode_p->state = REMOTE_DATA_BIT_HIGH;
				decode_p->timer = 0;
				decode_p->count = 0;
			}						 
		}
        break;				
	//????????????			
    case REMOTE_DATA_BIT_HIGH:
		{
		    if(decode_p->count == 0 && ir_state == 1) //first high level
			{
				decode_p->timer = 1;					
			}
			if(ir_state == 1)
			{
				decode_p->count ++;
			}
			if(decode_p->timer >= 9-1) //decode_p->timer > DOCK_DATA_BIT_UNIT_TICK && ir_state == 0
			{
				decode_p->state = REMOTE_DATA_BIT_LOW;
				decode_p->timer = 0;
				decode_p->count = 0;
			}				
		}break;
			 
		case REMOTE_DATA_BIT_LOW:
		{
			if(decode_p->count == 0 && ir_state == 0) //first low level
			{
				decode_p->timer = 1;					
			}
			if(ir_state == 0)
			{
				decode_p->count ++;
			}
			if ((decode_p->timer >= 8) && ((decode_p->count < 1))) //error because low level too few
			{
				  remoteStatus = 0;
				  ir_dock_resync_init(decode_p);	
                  break;
			}
			if(decode_p->timer >= 8 && ir_state == 1 ) //receive high level again, one bit finish
			{
				decode_p->decode_value <<= 1;
                if (decode_p->count >= DOCK_DATA_BIT_VALUE_1_TICK)        
				{
                    decode_p->decode_value++;
                }				
				decode_p->bits_count ++;
				decode_p->state = REMOTE_DATA_BIT_HIGH; //to decode next bit
				decode_p->count = 0;
				decode_p->timer = 0;				 
				if(decode_p->bits_count >=8) //1 byte finish
				{
					queue_put(rx_queue_list,((decode_p->decode_value)&0x0ff));
					decode_p->bits_count = 0;
				}
			}
		}	
		break;
    }
    return (remoteStatus);
}

/**
 * @fn uint8_t ir_decoder_dock_io_level_process(IR_REMOT_POSITION_E, uint8_t, uint8_t*)
 * @brief ???????????????��???
 *
 * @param ir_pos        ???????????
 * @param bit_state_8   8��????????8????????
 * @param code_result   ???????
 * @return 1 ??????????0 ��?????????
 */

uint8_t ir_decoder_dock_io_level_process(uint8_t bit_state_8, uint16_t * code_result)
{
    uint8_t state = 0;
    int16_t result;
    
    for (u8 i = 0; i < 8; i++)
    {
        state = (bit_state_8 >> (7 - i)) & 0x01;		
        /*????????????????,?????????????????????????,??????????*/
        result = ir_dock_decode_state(state);
        if (result == 0 ) //dock_decode[index].decode_value
        {
			// adc[index]= dock_decode[index].decode_value;
            //???????????
			dock_decode.decode_value=0;
            return 1;
        }
    }
    return 0;
}


/**
 * @fn void ir_detect_capture(void)
 * @brief ??????????? ????4k??8k????��??��???
 *
 *  ??? 26us ~ 36 us
 */

void ir_detect_capture(void)
{
    uint8_t state;
    uint8_t index = 0;	
	state = GPIO_ReadPin(CW_GPIOA ,GPIO_PIN_3 );
	state ^= IS_LEVEL_INVERT;				
	ir_working_byte <<= 1;
	ir_working_byte |= state;
	ir_capture_counter++;
	if (ir_capture_counter >= 8)
	{
		if(!queue_is_full(&ir_rx_queue))
		{
			queue_put(&ir_rx_queue,ir_working_byte);
			ir_working_byte = index;
			ir_capture_counter = 0;
		}
	}
}


/**
 * @fn void ir_decoder_io_level_init(void)
 * @brief ??????????��????
 *
 */
void ir_decoder_io_level_init(void)
{
        ir_rx_queue.data_buffer = &(ir_rx_buffer[0]);
        ir_rx_queue.queue_len = IR_QUEUE_LEN;
        ir_dock_resync_init(&dock_decode);	
}

