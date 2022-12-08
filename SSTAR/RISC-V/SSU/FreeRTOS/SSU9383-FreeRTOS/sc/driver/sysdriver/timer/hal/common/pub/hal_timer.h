/* SigmaStar trade secret */
/* Copyright (c) [2019~2020] SigmaStar Technology.
All rights reserved.

Unless otherwise stipulated in writing, any and all information contained
herein regardless in any format shall remain the sole proprietary of
SigmaStar and be kept in strict confidence
(SigmaStar Confidential Information) by the recipient.
Any unauthorized act including without limitation unauthorized disclosure,
copying, use, reproduction, sale, distribution, modification, disassembling,
reverse engineering and compiling of the contents of SigmaStar Confidential
Information is unlawful and strictly prohibited. SigmaStar hereby reserves the
rights to any and all damages, losses, costs and expenses resulting therefrom.
*/


#ifndef __HAL_TIMER_H__
#define __HAL_TIMER_H__

/*=============================================================*/
// Include files
/*=============================================================*/

#include "kernel.h"
#include "kernel_systimer.h"
#include "drv_timer_api.h"

/*=============================================================*/
// Enumeration
/*=============================================================*/

typedef enum
{
    HAL_TIMER0 = 0,
    HAL_TIMER1,
    HAL_TIMER2,
    HAL_TIMER_CNT
} HalTimerId_e;

/*=============================================================*/
// Macro definition
/*=============================================================*/

#define TIMER_WDTIMER_0_TICK        (0x3940000) //5s

#define HAL_OSTICK_MAX_COUNT        0xFFFFFFFF
#define HAL_GLOBAL_TIMER_MAX_COUNT  0xFFFFFFFF
#define HAL_STDATIMER_MAX_COUNT     0xFFFFFFFF

#define STDA_CLK_N                  12      // us
#define STDA_CLK_D                  1
#define GLOBAL_CLK_N                12000   // ms
#define GLOBAL_CLK_D                1
#define OSTICK_CLK_N                12000   // ms
#define OSTICK_CLK_D                1

/*=============================================================*/
// Global function definition
/*=============================================================*/

void HalTimerIsrReg(u32 u32TimerId, PfnTimerIntcISR pfnTimerISR);
void HalTimerIsrRel(u32 u32TimerId);

void HalTimerDisableIntr(u32 u32TimerId);
void HalTimerEnableIntr(u32 u32TimerId);
void HalTimerClearIntr(u32 u32TimerId);
u32 HalTimerGetIntrStatus(u32 u32TimerId);
void HalTimerStop(u32 u32TimerId);
void HalTimerStart(u32 u32TimerId, u32 u32MaxTick);
u32 HalTimerGetTimeoutLength(u32 u32TimerId);
u32 HalTimerGetCount(u32 u32TimerId);

#endif //__HAL_TIMER_H__
