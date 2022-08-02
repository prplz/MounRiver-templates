#ifndef _CH32V_PWM_H_
#define _CH32V_PWM_H_

#include <CH32V103.h>

#ifndef PWM_DUTY_MAX
#define PWM_DUTY_MAX		100
#endif

typedef enum
{
	TIM1_CH1 = 0,
	TIM1_CH2,    //1
	TIM1_CH3,    //2
	TIM1_CH4,    //3
    
	TIM2_CH1,    //4
    TIM2_CH2,    //5
    TIM2_CH3,    //6
    TIM2_CH4,    //7
    
    TIM3_CH1,    //8
    TIM3_CH2,    //9
    TIM3_CH3,    //10
    TIM3_CH4,    //11

	TIM4_CH1,    //12
    TIM4_CH2,    //13
    TIM4_CH3,    //14
    TIM4_CH4,    //15
} PWM_CHANNEL_Name;

static const uint32_t GET_PWM_TIM[16]={
    TIM1_BASE,  
    TIM1_BASE,  
    TIM1_BASE,  
    TIM1_BASE,   

    TIM2_BASE,  
    TIM2_BASE,  
    TIM2_BASE,  
    TIM2_BASE,

    TIM3_BASE,  
    TIM3_BASE,  
    TIM3_BASE,  
    TIM3_BASE, 

    TIM4_BASE,  
    TIM4_BASE,  
    TIM4_BASE,  
    TIM4_BASE
};
//========================================================================
// 描述: PWM初始化.
// 参数: 定时器通道、输出引脚、复用功能、频率、占空比（0-100）%
// 返回: none.
/*
引脚分布如下：
TIM1_CH1 PA8
TIM1_CH2 PA9
TIM1_CH3 PA10
TIM1_CH4 PA11

TIM2_CH1 PA0 (PA15)
TIM2_CH2 PA1 (PB3)
TIM2_CH3 PA2 (PB10)
TIM2_CH4 PA3 (PB11)

TIM3_CH1 PA6 (PB4) (PC6)
TIM3_CH2 PA7 (PB5) (PC7)
TIM3_CH3 PB0       (PC8)
TIM3_CH4 PB1       (PC9)

TIM4_CH1 PB6 
TIM4_CH2 PB7 
TIM4_CH3 PB8       
TIM4_CH4 PB9    
注意同一组PWM频率一样，占空比可以不一样，注意复用功能组的选择，成对使用，不能单独引脚设置。   
*/
//========================================================================

void _pwm(TIM_TypeDef* TIMx,uint16_t channel,uint16_t arr,uint16_t psc,uint16_t match_temp)
{
    TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_Period = arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIMx, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = match_temp;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    

    //ch1
    if((channel == 0) || (channel == 4) || (channel == 8) || (channel == 12))
    {
        TIM_OC1Init( TIMx, &TIM_OCInitStructure );
        TIM_OC1PreloadConfig( TIMx, TIM_OCPreload_Disable );
    }
    //ch2
    else if((channel == 1) || (channel == 5) || (channel == 9) || (channel == 13))
    {
        TIM_OC2Init( TIMx, &TIM_OCInitStructure );
        TIM_OC2PreloadConfig( TIMx, TIM_OCPreload_Disable );
    }
    //ch3
    else if((channel == 2) || (channel == 6) || (channel == 10) || (channel == 14))
    {
        TIM_OC3Init( TIMx, &TIM_OCInitStructure );
        TIM_OC3PreloadConfig( TIMx, TIM_OCPreload_Disable );
    }
    //ch4
    else if((channel == 3) || (channel == 7) || (channel == 11) || (channel == 15))
    {
        TIM_OC4Init( TIMx, &TIM_OCInitStructure );
        TIM_OC4PreloadConfig( TIMx, TIM_OCPreload_Disable );
    }
	
	TIM_CtrlPWMOutputs(TIMx, ENABLE );
	TIM_ARRPreloadConfig( TIMx, ENABLE );
	TIM_Cmd( TIMx, ENABLE );
}
void PWM_Init(PWM_CHANNEL_Name channel, uint8_t pin, uint32_t freq, uint16_t duty)
{
    u16 arr,psc;

    


    if(duty > PWM_DUTY_MAX)	
	{
		duty = PWM_DUTY_MAX;
	}

    TIM_TypeDef* TIMx = (TIM_TypeDef*) GET_PWM_TIM[channel];

	if(TIMx == TIM1)
	{
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1, ENABLE );
	}
	else if(TIMx == TIM2)
	{
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );
	}
	else if(TIMx == TIM3)
	{
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );
	}
	else if(TIMx == TIM4)
	{
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE );
	}

    pinMode(pin,GPIO_Mode_AF_PP);


    
    if((pin == PA15) || (pin == PB3))
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE );
        GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);
        /*
            TIM2_CH1      (PA15)
            TIM2_CH2      (PB3)
            TIM2_CH3 PA2 
            TIM2_CH4 PA3 
        */
    }
    else if((pin == PB10) || (pin == PB11))
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE );
        GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2,ENABLE);
        /*
            TIM2_CH1 PA0 
            TIM2_CH2 PA1 
            TIM2_CH3     (PB10)
            TIM2_CH4     (PB11)
        */
    }
        // RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE );
        // GPIO_PinRemapConfig(GPIO_FullRemap_TIM2,ENABLE);
        /*
            TIM2_CH1     (PA15) 
            TIM2_CH2     (PB3) 
            TIM2_CH3     (PB10)
            TIM2_CH4     (PB11)
        */

    else if((pin == PB4) || (pin == PB5))
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE );
        GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
        /*
            TIM3_CH1      (PB4) 
            TIM3_CH2      (PB5) 
            TIM3_CH3 PB0       
            TIM3_CH4 PB1       
        */
    }
    else if((pin == PC6) || (pin == PC7) || (pin == PC8) || (pin == PC9))
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE );
        GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
        /*
            TIM3_CH1           (PC6)
            TIM3_CH2           (PC7)
            TIM3_CH3           (PC8)
            TIM3_CH4           (PC9)
        */
    }
    if(channel<3)duty=PWM_DUTY_MAX-duty;
    uint16_t freq_div = ((SystemCoreClock / freq) >> 16);						
	uint16_t period_temp = (SystemCoreClock / freq / (freq_div+1));	
	uint16_t match_temp = (uint16_t)(period_temp*((float)duty/PWM_DUTY_MAX));	
	
   
	arr=period_temp;
	psc=freq_div;	
    _pwm(TIMx,channel,arr,psc,match_temp);

}

//========================================================================
// 描述: 更新PWM占空比.
// 参数: 定时器通道、占空比（0-100）%
// 返回: none.
//========================================================================
void PWM_Duty_Updata (PWM_CHANNEL_Name channel, uint16_t duty)
{
	if(duty > PWM_DUTY_MAX)	
	{
		duty = PWM_DUTY_MAX;
	}
	
	TIM_TypeDef* TIMx = (TIM_TypeDef*) GET_PWM_TIM[channel];
															
	uint16_t period_temp = TIMx->ATRLR;											// 获取自动重装载值
	uint16_t match_temp = (uint16_t)(period_temp*((float)duty/PWM_DUTY_MAX));	// 计算占空比

	if((channel == 0) || (channel == 4) || (channel == 8) || (channel == 12) || (channel == 13) || (channel == 14))
	{
		TIMx->CH1CVR = match_temp;											    // 装载比较值
	}
	//CH2
	else if((channel == 1) || (channel == 5) || (channel == 9))
	{
		TIMx->CH2CVR = match_temp;											     // 装载比较值
	}
	//CH3
	else if((channel == 2) || (channel == 6) || (channel == 10))
	{       
		TIMx->CH3CVR = match_temp;											      // 装载比较值
	}
	//CH4
	else if((channel == 3) || (channel == 7) || (channel == 11))
	{
		TIMx->CH4CVR = match_temp;											      // 装载比较值
	}
}

//========================================================================
// 描述: 更新PWM频率.
// 参数: 定时器通道、频率、占空比（0-100）%
// 返回: none.
//========================================================================
void PWM_Frequency_Updata (PWM_CHANNEL_Name channel, uint32_t freq, uint16_t duty)
{
    u16 arr,psc;
    TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TypeDef* TIMx = (TIM_TypeDef*) GET_PWM_TIM[channel];
															
	uint16_t freq_div = ((SystemCoreClock / freq) >> 16);						
	uint16_t period_temp = (SystemCoreClock / freq / (freq_div+1));	
	uint16_t match_temp = (uint16_t)(period_temp*((float)duty/PWM_DUTY_MAX));	
	arr=period_temp;
	psc=freq_div;	

	TIM_TimeBaseInitStructure.TIM_Period = arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIMx, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = match_temp;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    //ch1
    if((channel == 0) || (channel == 4) || (channel == 8) || (channel == 12))
    {
        TIM_OC1Init( TIMx, &TIM_OCInitStructure );
        TIM_OC1PreloadConfig( TIMx, TIM_OCPreload_Disable );
    }
    //ch2
    else if((channel == 1) || (channel == 5) || (channel == 9) || (channel == 13))
    {
        TIM_OC2Init( TIMx, &TIM_OCInitStructure );
        TIM_OC2PreloadConfig( TIMx, TIM_OCPreload_Disable );
    }
    //ch3
    else if((channel == 2) || (channel == 6) || (channel == 10) || (channel == 14))
    {
        TIM_OC3Init( TIMx, &TIM_OCInitStructure );
        TIM_OC3PreloadConfig( TIMx, TIM_OCPreload_Disable );
    }
    //ch4
    else if((channel == 3) || (channel == 7) || (channel == 11) || (channel == 15))
    {
        TIM_OC4Init( TIMx, &TIM_OCInitStructure );
        TIM_OC4PreloadConfig( TIMx, TIM_OCPreload_Disable );
    }
	
	TIM_CtrlPWMOutputs(TIMx, ENABLE );
}

#endif