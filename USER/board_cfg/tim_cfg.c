#include "tim_cfg.h"


///IR-38K红外载波输出初始化
void tim_ir_pwm_cfg(uint16_t arr,uint16_t psc)
{

	  __SYSCTRL_GPIOA_CLK_ENABLE();
	  GTIM_InitTypeDef GTIM_InitStruct = {0};
    GTIM_OCModeCfgTypeDef GTIM_OCModeCfgStruct = {DISABLE,DISABLE,0};

    __SYSCTRL_GTIM1_CLK_ENABLE();
    
    PA05_DIGTAL_ENABLE();
    PA05_DIR_OUTPUT();
    PA05_PUSHPULL_ENABLE();
    PA05_AFx_GTIM1CH2();

    GTIM_InitStruct.AlignMode = GTIM_ALIGN_MODE_EDGE;
    GTIM_InitStruct.ARRBuffState = GTIM_ARR_BUFF_EN;
    GTIM_InitStruct.Direction = GTIM_DIRECTION_UP;
    GTIM_InitStruct.EventOption = GTIM_EVENT_NORMAL;
    GTIM_InitStruct.Prescaler = psc;
    GTIM_InitStruct.PulseMode = GTIM_PULSE_MODE_DIS;
    GTIM_InitStruct.ReloadValue = arr;
    GTIM_InitStruct.UpdateOption = GTIM_UPDATE_DIS;
    GTIM_TimeBaseInit(CW_GTIM1, &GTIM_InitStruct);
    
    
    GTIM_OCModeCfgStruct.FastMode = DISABLE;
    GTIM_OCModeCfgStruct.OCMode = GTIM_OC_MODE_PWM1;
    GTIM_OCModeCfgStruct.OCPolarity = GTIM_OC_POLAR_NONINVERT;
    GTIM_OCModeCfgStruct.PreloadState = DISABLE;
    GTIM_OC2ModeCfg(CW_GTIM1, &GTIM_OCModeCfgStruct);
    
    GTIM_SetCompare2(CW_GTIM1, arr/2);
    GTIM_OC2Cmd(CW_GTIM1, ENABLE);
    GTIM_Cmd(CW_GTIM1, ENABLE);

}

void NVIC_ConfigurationATIM(void)
{
    __disable_irq();
    NVIC_EnableIRQ(ATIM_IRQn);
    __enable_irq();
}


///CHARGE-PWM配置(arr:(1200-2) psc:(8000-1))
void tim_charge_pwm_cfg(uint16_t arr,uint16_t psc)
{
		__SYSCTRL_ATIM_CLK_ENABLE();
		NVIC_ConfigurationATIM() ;
		ATIM_InitTypeDef ATIM_InitStruct = {DISABLE,0};

		ATIM_InitStruct.BufferState = ENABLE;               //使能缓存寄存器    
		ATIM_InitStruct.CounterAlignedMode = ATIM_COUNT_ALIGN_MODE_EDGE;    //边沿对齐
		ATIM_InitStruct.CounterDirection = ATIM_COUNTING_UP;        //向上计数；
		ATIM_InitStruct.CounterOPMode = ATIM_OP_MODE_REPETITIVE;    //连续运行模式   
		ATIM_InitStruct.Prescaler = psc;                   // 1分频
		ATIM_InitStruct.ReloadValue = arr;                 // 重载周期9+1
		ATIM_InitStruct.RepetitionCounter = 0;              // 重复周期0

		ATIM_Init(&ATIM_InitStruct);
		ATIM_ITConfig(ATIM_IT_UIE, ENABLE);              // 有重复计数器溢出产生进入中断
		
		ATIM_Cmd(ENABLE);
	
}






