#include "base_event.h"

uint32_t base_event = BASE_EVENT_RESERVE;

void create_base_event(BASE_EVENT_E event)
{
    base_event |= (1 << event);
}

void clear_base_event(BASE_EVENT_E event)
{
    base_event &= ~(1 << event);
}

uint8_t get_base_event(BASE_EVENT_E event)
{
    unsigned char ret = 0;
    ret = ((base_event >> event) & 1);
    base_event &= ~(1 << event);	
    return ret;
}



void clear_all_event()
{
    base_event = 0;
}



