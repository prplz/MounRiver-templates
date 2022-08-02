/*
CH32V定时器功能驱动
目前支持TIM2/3/4基本的定时功能，后续添加包括TIM1的输入捕获、输出比较、编码器等功能。

使用范例：
void myfun(void)
{
    
}
TIM_Init(TIM2,5000,myfun);

*/
#ifndef _CH32V_TIM_H_
#define _CH32V_TIM_H_ 

#include <CH32V103.h>
#include "ch32v10x_gpio.h"
typedef void (*voidFuncPtr)(void);
volatile static voidFuncPtr tim_fun[3] = { NULL };
//========================================================================
// 描述: TIM初始化.
// 参数: 定时器(TIM2/3/4)、周期（us）、回调函数
// 返回: none.
//========================================================================
void TIM_attachInterrupt(TIM_TypeDef* TIMx, uint32_t period, void (*userFunc)(void))
{
    // //初始化TIM NVIC，设置中断优先级分组
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

    //1.使能时钟，记录回调函数
    if(TIMx == TIM2)
	{
        tim_fun[0] = userFunc;
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );
        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;           //TIM2中断
        TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能TIM2中断，允许更新中断
	}
	else if(TIMx == TIM3)
	{
        tim_fun[1] = userFunc;
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );

        NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //TIM3中断 
        TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能TIM3中断，允许更新中断
	}
	else if(TIMx == TIM4)
	{
        tim_fun[2] = userFunc;
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE );
        NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;           //TIM4中断
        TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能TIM4中断，允许更新中断
	}

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //设置抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //设置响应优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //使能通道1中断
    NVIC_Init(&NVIC_InitStructure); //初始化NVIC

    //2.设置定时周期
    if (period > 50000) 
    {
        TIM_TimeBaseInitStructure.TIM_Prescaler = 7199;  	//预分频器设置
        TIM_TimeBaseInitStructure.TIM_Period = period / 100-1;  	//设定计数器自动重装值
    }
    else 
    {
        TIM_TimeBaseInitStructure.TIM_Prescaler = 71;   	//预分频器设置
        TIM_TimeBaseInitStructure.TIM_Period = period-1;  	//设定计数器自动重装值
    }

    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIMx, &TIM_TimeBaseInitStructure);

    TIM_Cmd( TIMx, ENABLE);
}

//========================================================================
// 描述: 更新周期
// 参数: 定时器、周期
// 返回: none.
//========================================================================
void TIM_Duty_Updata(TIM_TypeDef* TIMx, uint32_t period)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    //1.设置定时周期
    if (period > 50000) 
    {
        TIM_TimeBaseInitStructure.TIM_Prescaler = 7199;  	//预分频器设置
        TIM_TimeBaseInitStructure.TIM_Period = period / 100-1;  	//设定计数器自动重装值
    }
    else 
    {
        TIM_TimeBaseInitStructure.TIM_Prescaler = 71;   	//预分频器设置
        TIM_TimeBaseInitStructure.TIM_Period = period-1;  	//设定计数器自动重装值
    }
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIMx, &TIM_TimeBaseInitStructure);
}

//========================================================================
// 描述: 关闭定时器
// 参数: 定时器
// 返回: none.
//========================================================================
void TIM_Disable(TIM_TypeDef* TIMx)
{
	TIM_Cmd( TIMx, DISABLE);   //关闭定时器
}

//========================================================================
// 描述: 启动定时器
// 参数: 定时器
// 返回: none.
//========================================================================
void TIM_Enable(TIM_TypeDef* TIMx)
{
	TIM_Cmd( TIMx, ENABLE);   //使能定时器
}

//========================================================================
// 描述: 定时器解除占用
// 参数: 定时器
// 返回: none.
//========================================================================
void TIM_detachInterrupt(TIM_TypeDef* TIMx)
{
    if(TIMx == TIM2)
	{
        tim_fun[0] = NULL;
        RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, DISABLE );
	}
	else if(TIMx == TIM3)
	{
        tim_fun[1] = NULL;
        RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, DISABLE );
	}
	else if(TIMx == TIM4)
	{
        tim_fun[2] = NULL;
        RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, DISABLE );
	}
}

//========================================================================
// 描述: TIM中断函数.
// 参数: none.
// 返回: none.
//========================================================================

extern "C"{
    void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
    void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
    void TIM4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

    void TIM2_IRQHandler(void)
    {
        if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查TIM2中断是否发生。
        {

            TIM_ClearITPendingBit(TIM2,TIM_IT_Update);    //清除TIM2的中断挂起位。

            if(tim_fun[0]!=NULL)tim_fun[0]();
        }
    }

    void TIM3_IRQHandler(void)
    {
        if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查TIM3中断是否发生。
        {

            TIM_ClearITPendingBit(TIM3,TIM_IT_Update);    //清除TIM3的中断挂起位。
           
            if(tim_fun[1]!=NULL)tim_fun[1]();
        }
    }

    void TIM4_IRQHandler(void)
    {
        if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查TIM4中断是否发生。
        {

            TIM_ClearITPendingBit(TIM4,TIM_IT_Update);    //清除TIM4的中断挂起位。

            if(tim_fun[2]!=NULL)tim_fun[2]();
        }
    }

} // extern "C"

#endif
