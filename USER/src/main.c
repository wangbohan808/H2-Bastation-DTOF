 /*******************************************************************************
*
* 代码许可和免责信息
* 武汉芯源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由此
* 生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉芯源半
* 导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明示或暗
* 含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵权的保证
* 或条件。
* 无论何种情形，武汉芯源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
* 即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带的
* 或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金额的
* 损失。
* 某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些或
* 全部上述排除或限制可能并不适用于您。
*
*******************************************************************************/
/******************************************************************************
 * Include files
 ******************************************************************************/
#include "main.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
 
static void Hardware_init_jcb(void) ;

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  
 *         
 * @return int32_t
 */
 uint32_t KeyNum;
 
int32_t main(void)
{
	  SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV1);
    InitTick(48000000);
		Hardware_init_jcb();	
		while (1)
		{ 
		    ir_rx_decode();             //红外解码函数
				task_base_mode_process();   //模式处理              :正常模式，治具模式		
			
				if(need_duty == 1)
				{
					 dust_absorption_ctrl(1);
				}
			  task_base_self_detect(); 

				if(KeyNum != timer_ms())
				{
					KeyNum = timer_ms();
					key_press_detect();
				}	
				
		}
}



void SYSCTRL_Configuration(void)
{

	 REGBITS_SET(CW_SYSCTRL->AHBEN, (0x5A5A0000 |bv1));
   REGBITS_SET(CW_SYSCTRL->AHBEN, (0x5A5A0000 |bv5));
}



void iwdg_reset_init(void)
{
		__SYSCTRL_IWDT_CLK_ENABLE(); 
		IWDT_InitTypeDef IWDT_InitStruct ={0};
   
    IWDT_InitStruct.IWDT_ITState = ENABLE;
    IWDT_InitStruct.IWDT_OverFlowAction = IWDT_OVERFLOW_ACTION_RESET;
    IWDT_InitStruct.IWDT_Pause = IWDT_SLEEP_PAUSE ;
    IWDT_InitStruct.IWDT_Prescaler = IWDT_Prescaler_DIV512;     // 默认设置为最大分频，计时间隔为15.6ms
    IWDT_InitStruct.IWDT_WindowValue = 0xFFF;          // 关闭窗口看门狗的功能
    IWDT_InitStruct.IWDT_ReloadValue = IWDT_2_SECS;
    
    // IWDT的时钟为LSI，启动IWDT前LSI必须有效
    if (CW_SYSCTRL->CR1_f.LSIEN == 0)
    {
        SYSCTRL_LSI_Enable();
    }  
    IWDT_Init(&IWDT_InitStruct);
	  IWDT_Cmd();
    IWDT_Refresh();   
}


static void Hardware_init_jcb(void)
{
	SYSCTRL_Configuration() ;
#if 1
	/*芯片复位后x秒再把SWCLK复用为UART_TX,留有足够的时间能让芯片重新烧录代码*/
	__SYSCTRL_GPIOA_CLK_ENABLE();
	__SYSCTRL_GPIOB_CLK_ENABLE();
	tim_charge_pwm_cfg(100-1,48-1);                              //100us定时中断

	lighting_ctrl_init();   
	
#endif
	gpio_input_cfg(CW_GPIOA , GPIO_PIN_0); 
	
	/* 复位引脚重置为输出功能 */
	CW_SYSCTRL->CR2 =  CW_SYSCTRL->CR2 | (0x5A5A0000 | bv0);
	gpio_input_cfg(CW_GPIOB , GPIO_PIN_7);
	
	adc_capture_cfg_jcb();                                        //adc采集初始化
	
	gpio_input_cfg(CW_GPIOA,GPIO_PIN_3);                             //ir接收初始化
    ir_decoder_io_level_init();
	
	
	ir_output_init();                                             //ir输出初始化
	

	charge_ctrl_init();                                           //充电控制初始化
	charge_off();
	gpio_output_cfg(CW_GPIOB,GPIO_PIN_4);                           //吸尘功能初始化
	
	set_gpio_level(CW_GPIOB,GPIO_PIN_4,0);
	
  iwdg_reset_init();                                            //看门狗初始化  	
}


/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

