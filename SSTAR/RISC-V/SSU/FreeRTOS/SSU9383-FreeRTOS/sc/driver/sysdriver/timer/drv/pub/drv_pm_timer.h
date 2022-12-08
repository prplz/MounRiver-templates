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

#ifndef __DRV_PM_TIMER_H__
#define __DRV_PM_TIMER_H__

/*=============================================================*/
// Include files
/*=============================================================*/

#include "drv_timer_api.h"

/*=============================================================*/
// Enumeration
/*=============================================================*/

typedef enum
{
    DRV_TIMER0 = 0,
    DRV_TIMER1,
    DRV_TIMER2,
    DRV_TIMER_CNT
} DrvTimerId_e;

/*=============================================================*/
// Macro definition
/*=============================================================*/

#define DRV_TIMER_TICK_MAX_COUNT    (0xFFFFFFFF)

/*=============================================================*/
// Global function definition
/*=============================================================*/

void DrvTimerRegisterIsr(u32 u32TimerId, PfnTimerIntcISR pfnTimerISR);
void DrvTimerReleaseIsr(u32 u32TimerId);
void DrvTimerDisableIntr(u32 u32TimerId);
void DrvTimerEnableIntr(u32 u32TimerId);
void DrvTimerClearIntr(u32 u32TimerId);
u32 DrvTimerGetIntrStatus(u32 u32TimerId);
void DrvTimerStop(u32 u32TimerId);
void DrvTimerStart(u32 u32TimerId, u32 u32MaxTick);
u32 DrvTimerGetTimeoutLength(u32 u32TimerId);
u32 DrvTimerGetCount(u32 u32TimerId);

#endif //__DRV_PM_TIMER_H__