#ifndef _CH32V_EXTI_H_
#define _CH32V_EXTI_H_

#include <CH32V103.h>

typedef void (*voidFuncPtr)(void);
volatile static voidFuncPtr exti_fun[16] = {NULL};

//========================================================================
// 描述: 引脚中断初始化函数.
// 参数: pin：引脚; interrupt_type:触发类型; userFunc:回调函数.
// 返回: none.
//========================================================================
void Pin_attachInterrupt(uint8_t pin, EXTITrigger_TypeDef interrupt_type, void (*userFunc)(void))
{
    uint8_t EXT_LINEX;

    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    if (interrupt_type == EXTI_Trigger_Rising)
    {
        pinMode(pin, GPIO_Mode_IPD);
    }
    else if (interrupt_type == EXTI_Trigger_Falling)
    {
        pinMode(pin, GPIO_Mode_IPU);
    }
    else
    {
        pinMode(pin, GPIO_Mode_IN_FLOATING);
    }

    EXT_LINEX = pin % 16;
    GPIO_EXTILineConfig(pin / 16, pin % 16);
    EXTI_InitStructure.EXTI_Line = (1 << EXT_LINEX);
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = interrupt_type;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    if (EXT_LINEX < 5)
    {
        NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn + EXT_LINEX;
    }
    else if (EXT_LINEX < 10)
    {
        NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    }
    else
    {
        NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    }
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    exti_fun[EXT_LINEX] = userFunc;
}

//========================================================================
// 描述: 引脚中断处理函数.
// 参数: none.
// 返回: none.
//========================================================================

extern "C"
{
    void EXTI0_IRQHandler(void)
    {

        if (EXTI_GetITStatus(EXTI_Line0) != RESET)
        {
            exti_fun[0]();
            EXTI_ClearITPendingBit(EXTI_Line0);
        }
    }
    void EXTI1_IRQHandler(void)
    {
        if (EXTI_GetITStatus(EXTI_Line1) != RESET)
        {
            exti_fun[1]();
            EXTI_ClearITPendingBit(EXTI_Line1);
        }
    }
    void EXTI2_IRQHandler(void)
    {
        if (EXTI_GetITStatus(EXTI_Line2) != RESET)
        {
            exti_fun[2]();
            EXTI_ClearITPendingBit(EXTI_Line2);
        }
    }
    void EXTI3_IRQHandler(void)
    {

        if (EXTI_GetITStatus(EXTI_Line3) != RESET)
        {
            exti_fun[3]();
            EXTI_ClearITPendingBit(EXTI_Line3);
        }
    }
    void EXTI4_IRQHandler(void)
    {

        if (EXTI_GetITStatus(EXTI_Line4) != RESET)
        {
            exti_fun[4]();
            EXTI_ClearITPendingBit(EXTI_Line4);
        }
    }
    void EXTI9_5_IRQHandler(void)
    {

        if (EXTI_GetITStatus(EXTI_Line5) != RESET)
        {
            exti_fun[5]();
            EXTI_ClearITPendingBit(EXTI_Line5);
        }
        if (EXTI_GetITStatus(EXTI_Line6) != RESET)
        {
            exti_fun[6]();
            EXTI_ClearITPendingBit(EXTI_Line6);
        }
        if (EXTI_GetITStatus(EXTI_Line7) != RESET)
        {
            exti_fun[7]();
            EXTI_ClearITPendingBit(EXTI_Line7);
        }
        if (EXTI_GetITStatus(EXTI_Line8) != RESET)
        {
            exti_fun[8]();
            EXTI_ClearITPendingBit(EXTI_Line8);
        }
        if (EXTI_GetITStatus(EXTI_Line9) != RESET)
        {
            exti_fun[9]();
            EXTI_ClearITPendingBit(EXTI_Line9);
        }
    }
    void EXTI15_10_IRQHandler(void)
    {

        if (EXTI_GetITStatus(EXTI_Line10) != RESET)
        {
            exti_fun[10]();
            EXTI_ClearITPendingBit(EXTI_Line10);
        }
        if (EXTI_GetITStatus(EXTI_Line11) != RESET)
        {
            exti_fun[11]();
            EXTI_ClearITPendingBit(EXTI_Line11);
        }
        if (EXTI_GetITStatus(EXTI_Line12) != RESET)
        {
            exti_fun[12]();
            EXTI_ClearITPendingBit(EXTI_Line12);
        }
        if (EXTI_GetITStatus(EXTI_Line13) != RESET)
        {
            exti_fun[13]();
            EXTI_ClearITPendingBit(EXTI_Line13);
        }
        if (EXTI_GetITStatus(EXTI_Line14) != RESET)
        {
            exti_fun[14]();
            EXTI_ClearITPendingBit(EXTI_Line14);
        }
        if (EXTI_GetITStatus(EXTI_Line15) != RESET)
        {
            exti_fun[15]();
            EXTI_ClearITPendingBit(EXTI_Line15);
        }
    }
}

#endif //CH32V_exti.h