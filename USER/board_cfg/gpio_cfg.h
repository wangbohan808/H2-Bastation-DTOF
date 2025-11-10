#ifndef GPIO_CFG_H
#define GPIO_CFG_H

#include "pcb_config.h"
#include "main.h"

extern void set_gpio_level(GPIO_TypeDef* GPIOx, uint16_t pin,uint8_t level);

extern void gpio_output_cfg(GPIO_TypeDef* GPIOx, uint16_t pin);

extern void gpio_input_cfg(GPIO_TypeDef* GPIOx, uint16_t pin);

#endif

