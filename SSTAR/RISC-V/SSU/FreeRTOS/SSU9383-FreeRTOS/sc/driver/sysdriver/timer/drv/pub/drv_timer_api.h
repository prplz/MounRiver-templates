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

#ifndef __DRV_TIMER_API_H__
#define __DRV_TIMER_API_H__

#include "vm_types.ht"

#if defined(RTOS_KERNEL_USE_RTK)
#include "sys_rtk_vmrtkho.h"
#endif

/*=============================================================*/
// Typedef
/*=============================================================*/

typedef void (*PfnTimerIntcISR)(void);

/*=============================================================*/
// Global function definition
/*=============================================================*/

void DrvSysTimerInit(void);
void DrvTimerUdelay(u32 us);
void DrvTimerUdelayCond(s32 cond, u32 us);

u32 DrvTimerGetOstickMaxCount(void);
void DrvTimerOstickStop(void);
void DrvTimerOstickRun(u32 tick);
u32 DrvTimerOstickGetTimeoutLength(void);
u32 DrvTimerOstickGetCount(void);
void DrvTimerOstickIntClear(void);
u32 DrvTimerOstickIntFlag(void);
void DrvTimerOsTickIsrReg(PfnTimerIntcISR pfnOstickTimerISR);
void DrvTimerOsTickIsrRel(void);
u32 DrvTimerMs2Ostick(u32 ms);
u32 DrvTimerOstick2Ms(u32 tick);
u32 DrvTimerOstick2Us(u32 tick);

u32 DrvTimerStdaTimerTick2Us(u32 tick);
u32 DrvTimerStdaTimerGetTick(void);

u32 DrvTimerGetGlobalTimerMaxCount(void);
u32 DrvTimerGlobalTimerTick2Ms(u32 tick);
void DrvTimerGlobalTimerStop(void);
void DrvTimerGlobalTimerRun(u32 tick);
u32 DrvTimerGlobalTimerGetTick(void);
u32 DrvTimerGlobalTimerGetTimeoutLength();
void DrvTimerGlobalTimerIntClear(void);
u32 DrvTimerGlobalTimerIntFlag(void);
void DrvTimerGlobalTimerIntEnable(void);
void DrvTimerGlobalTimerIntDisable(void);
void DrvTimerGlobalTimerIsrReg(PfnTimerIntcISR pfnTimeSoftTimerISR);
void DrvTimerGlobalTimerIsrRel(void);

#if defined(RTOS_KERNEL_USE_RTK)
u32 DrvTimerHwToUserTickGlobal(u32 x);
u32 DrvTimerUserToHwTickOstick(u32 x);
u32 DrvTimerHwToUserTickOstick(u32 x);
#endif

#endif /* __DRV_TIMER_API_H__ */
