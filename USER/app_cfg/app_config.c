#include "app_config.h"
#include "interrupts_cw32l010.h"

char version[2] = {1,24};

uint8_t soft_version = 3;

void set_soft_version(uint8_t version)
{
     soft_version = version ;
}


uint8_t get_version()
{
   return soft_version ;
}



uint8_t get_version_sort1()
{
	return version[0];
}

uint8_t get_version_sort2()
{
	return version[1];
}








