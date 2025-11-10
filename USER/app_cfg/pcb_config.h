#ifndef PCB_CONFIG_H
#define PCB_CONFIG_H

#include "cw32l010.h"
#include "app_config.h"

///LOG_UART
#if DUST_CHARGE_HIGH_ENABLE
#define LOG_UARTx       UART2
#define LOG_PERIPH      RCC_APB_PERIPH_UART2
#define LOG_GPIO        CW_GPIOA
#define LOG_PERIPH_GPIO RCC_APB_PERIPH_IOPA
#define LOG_TX_PIN      GPIO_PIN_9
#define LOG_TX_AF       GPIO_AF1_UART2
#define LOG_RX_PIN      GPIO_PIN_7
#define LOG_RX_AF       GPIO_AF6_UART2

#elif CHARGE_ONLY_ENABLE
#define LOG_UARTx       UART1
#define LOG_PERIPH      RCC_APB_PERIPH_UART1
#define LOG_GPIO        CW_GPIOA
#define LOG_PERIPH_GPIO RCC_APB_PERIPH_IOPA
#define LOG_TX_PIN      GPIO_PIN_14
#define LOG_TX_AF       GPIO_AF2_UART1
#define LOG_RX_PIN      GPIO_PIN_12
#define LOG_RX_AF       GPIO_AF2_UART1
#endif


#endif

