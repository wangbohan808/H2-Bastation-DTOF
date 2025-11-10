#include "adc_cfg.h"


void adc_capture_cfg_jcb(void)
{
	 	//打开GPIO时钟
    __SYSCTRL_GPIOA_CLK_ENABLE();
    //打开ADC时钟
    __SYSCTRL_ADC_CLK_ENABLE();
    ADC_WatchdogTypeDef ADC_WdtStructure = {0};

    //set PA02 as AIN2 INPUT
    PA04_ANALOG_ENABLE();       //充电电流检测   ADC_IN4
		PB02_ANALOG_ENABLE();       //机器人在位     ADC_IN9
		PB03_ANALOG_ENABLE();       //过零检测       ADC_IN10
		
		    //ADC模拟看门狗通道初始化
    ADC_WdtStructure.ADC_InitStruct.ADC_ClkDiv = ADC_Clk_Div8;
    ADC_WdtStructure.ADC_InitStruct.ADC_ConvertMode = ADC_ConvertMode_Continuous;
    ADC_WdtStructure.ADC_InitStruct.ADC_SQREns = ADC_SqrEns0to2;
    ADC_WdtStructure.ADC_InitStruct.ADC_IN0.ADC_InputChannel = ADC_InputCH10;
    ADC_WdtStructure.ADC_InitStruct.ADC_IN0.ADC_SampTime = ADC_SampTime390Clk;
		ADC_WdtStructure.ADC_InitStruct.ADC_IN1.ADC_InputChannel = ADC_InputCH4;
    ADC_WdtStructure.ADC_InitStruct.ADC_IN1.ADC_SampTime = ADC_SampTime390Clk;
		ADC_WdtStructure.ADC_InitStruct.ADC_IN2.ADC_InputChannel = ADC_InputCH9;
    ADC_WdtStructure.ADC_InitStruct.ADC_IN2.ADC_SampTime = ADC_SampTime390Clk;
    ADC_WdtStructure.ADC_WatchdogCHx = ADC_WATCHDOG_IN10;    // 模拟看门狗监控的是输入通道
    ADC_WdtStructure.ADC_WatchdogOverHighIrq = ENABLE;
    ADC_WdtStructure.ADC_WatchdogUnderLowIrq = ENABLE;
    ADC_WdtStructure.ADC_WatchdogVth = 0x0F00;
    ADC_WdtStructure.ADC_WatchdogVtl = 0x0000;
    
    ADC_WatchdogInit(&ADC_WdtStructure);
		
		ADC_ITConfig(ADC_IT_AWDH|ADC_IT_AWDL, ENABLE);
    ADC_ClearITPendingAll();

    NVIC_EnableIRQ(ADC_IRQn);
    //ADC使能
    ADC_Enable();
    ADC_SoftwareStartConvCmd(ENABLE);
    
}


