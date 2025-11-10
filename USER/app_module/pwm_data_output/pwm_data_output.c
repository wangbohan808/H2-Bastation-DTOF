#include "pwm_data_output.h"

//充电控制初始化
void charge_ctrl_init(void)
{
    gpio_output_cfg(CW_GPIOA,GPIO_PIN_6);
    set_gpio_level(CW_GPIOA,GPIO_PIN_6,1);
}


void charge_on(void)
{
	  GPIO_WritePin(CW_GPIOA,GPIO_PIN_6,1);
}

void charge_off(void)
{
	  GPIO_WritePin(CW_GPIOA,GPIO_PIN_6,0);
}




