#include "gpio_cfg.h"

void set_gpio_level(GPIO_TypeDef* GPIOx, uint16_t pin,uint8_t level)
{
		GPIO_WritePin(GPIOx,pin,level);
}

void gpio_output_cfg(GPIO_TypeDef* GPIOx, uint16_t pin)
{
    GPIO_InitTypeDef GPIO_InitStructure;
		if(pin < GPIO_PIN_All)
		{
				/* Assign default value to GPIO_InitStructure structure */
				/* Select the GPIO pin to control */
				GPIO_InitStructure.Pins          = pin;
				/* Set pin mode to general push-pull output */
				GPIO_InitStructure.Mode    = GPIO_MODE_OUTPUT_PP;
				/* Set the pin drive current to 4MA*/
				GPIO_InitStructure.IT = GPIO_IT_NONE;
				/* Initialize GPIO */
				GPIO_Init(GPIOx, &GPIO_InitStructure);
		}  	
}

void gpio_input_cfg(GPIO_TypeDef* GPIOx, uint16_t pin)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	if(pin < GPIO_PIN_All)
	{		
			/* Select the GPIO pin to control */
			GPIO_InitStructure.Pins          = pin;
			/* Set pin mode to general push-pull output */
			GPIO_InitStructure.Mode    = GPIO_MODE_INPUT;
			/* Set the pin drive current to 4MA*/
			GPIO_InitStructure.IT = GPIO_IT_NONE;
			/* Initialize GPIO */
			GPIO_Init(GPIOx, &GPIO_InitStructure);
	}   
}





