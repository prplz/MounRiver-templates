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

#include "vm_types.ht"
#include "drv_timer_api.h"
#include "hal_timer.h"
#include "sys_sys_bootinit.h"
#ifdef CONFIG_CPU_ARCH_RISCV
#include "sys_riscv_arch_timer.h"
#else
#include "sys_arm_arch_timer.h"
#endif

/*=============================================================*/
// Constant
/*=============================================================*/

#define DRV_OS_TICK_TIMER   HAL_TIMER0
#define DRV_STDA_TIMER      HAL_TIMER1
#define DRV_GLOBAL_TIMER    HAL_TIMER2

/*=============================================================*/
// Global variables
/*=============================================================*/

static u32 m_u32OsTickTimerInited = 0;

/*=============================================================*/
// Global function definition
/*=============================================================*/

static void _DrvTimerUDelay(u32 us, int cond)
{
    u32 c0 = 0;
    u32 period = 0;
    u32 count = 0;

    period = (u32)(us*STDA_CLK_N/STDA_CLK_D);

    c0 = (u32)HalTimerGetCount(HAL_TIMER1);

    while(cond)
    {
        u32 c1 = (u32)HalTimerGetCount(HAL_TIMER1);

        if (c1 < c0)
        {
            count += (c1 + (HAL_STDATIMER_MAX_COUNT - c0));
        }
        else
        {
            count += (c1 - c0);
        }

        c0 = c1;

        if (count >= period)
        {
            break;
        }
    }
}

void DrvTimerUdelay(u32 us)
{
    _DrvTimerUDelay(us, 1);
}

/**
 * @brief   To Delay in micro-seconds with condition
 *          cond : To delay "us" micro-seconds if "cond" is true
 *          us : micro-seconds to delay
 */
void DrvTimerUdelayCond(s32 cond, u32 us)
{
    _DrvTimerUDelay(us, cond);
}

//////////////////////////////////////////////////////

u32 DrvTimerGetOstickMaxCount(void)
{
    return HAL_OSTICK_MAX_COUNT;
}

void DrvTimerOstickStop(void)
{
    HalTimerStop(DRV_OS_TICK_TIMER);
}

/**
 * @brief   To set the timeout length OS_TICK and run
 *          An OSTICK timeout interrupt will issue after timeout, if the OSTICK interrupt
 *          mask is clear. The OSTICK counter will maintain at timeout value until the next
 *          OSTICK clock.
 */
void DrvTimerOstickRun(u32 tick)
{
    HalTimerStart(DRV_OS_TICK_TIMER, tick);
}

u32 DrvTimerOstickGetTimeoutLength(void)
{
    return HalTimerGetTimeoutLength(DRV_OS_TICK_TIMER);
}

u32 DrvTimerOstickGetCount(void)
{
    return HalTimerGetCount(DRV_OS_TICK_TIMER);
}

void DrvTimerOstickIntClear(void)
{
    HalTimerClearIntr(DRV_OS_TICK_TIMER);
}

u32 DrvTimerOstickIntFlag(void)
{
    return HalTimerGetIntrStatus(DRV_OS_TICK_TIMER);
}

void DrvTimerOsTickIsrReg(PfnTimerIntcISR pfnOstickTimerISR)
{
    if (m_u32OsTickTimerInited == 1)
    {
        // print error log.
        return;
    }
    m_u32OsTickTimerInited = 1;

    HalTimerIsrReg(DRV_OS_TICK_TIMER, pfnOstickTimerISR);
}

void DrvTimerOsTickIsrRel(void)
{
    if (m_u32OsTickTimerInited == 0)
    {
        // print error log.
        return;
    }
    m_u32OsTickTimerInited = 0;

    HalTimerIsrRel(DRV_OS_TICK_TIMER);
}

u32 DrvTimerMs2Ostick(u32 ms)
{
    return (((ms)*OSTICK_CLK_N+OSTICK_CLK_D-1)/OSTICK_CLK_D);
}

u32 DrvTimerOstick2Ms(u32 tick)
{
    return (((tick)*OSTICK_CLK_D)/OSTICK_CLK_N);
}

u32 DrvTimerOstick2Us(u32 tick)
{
    return (((tick)*OSTICK_CLK_D*1000)/OSTICK_CLK_N);
}

u32 DrvTimerStdaTimerGetTick(void)
{
    return HalTimerGetCount(DRV_STDA_TIMER);
}

u32 DrvTimerStdaTimerTick2Us(u32 tick)
{
    return (((tick)*STDA_CLK_D)/STDA_CLK_N);
}

u32 DrvTimerGetGlobalTimerMaxCount(void)
{
    return HAL_GLOBAL_TIMER_MAX_COUNT;
}

u32 DrvTimerGlobalTimerTick2Ms(u32 tick)
{
    return (((tick)*GLOBAL_CLK_D)/GLOBAL_CLK_N);
}

void DrvTimerGlobalTimerStop(void)
{
    HalTimerStop(DRV_GLOBAL_TIMER);
}

void DrvTimerGlobalTimerRun(u32 tick)
{
    HalTimerStart(DRV_GLOBAL_TIMER, tick);
}

u32 DrvTimerGlobalTimerGetTimeoutLength(void)
{
    return HalTimerGetTimeoutLength(DRV_GLOBAL_TIMER);
}

u32 DrvTimerGlobalTimerGetTick(void)
{
    return HalTimerGetCount(DRV_GLOBAL_TIMER);
}

void DrvTimerGlobalTimerIntClear(void)
{
    HalTimerClearIntr(DRV_GLOBAL_TIMER);
}

u32 DrvTimerGlobalTimerIntFlag(void)
{
    return HalTimerGetIntrStatus(DRV_GLOBAL_TIMER);
}

void DrvTimerGlobalTimerIntEnable(void)
{
    HalTimerEnableIntr(DRV_GLOBAL_TIMER);
}

void DrvTimerGlobalTimerIntDisable(void)
{
    HalTimerDisableIntr(DRV_GLOBAL_TIMER);
}

void DrvTimerGlobalTimerIsrReg(PfnTimerIntcISR pfnTimeSoftTimerISR)
{
    HalTimerIsrReg(DRV_GLOBAL_TIMER, pfnTimeSoftTimerISR);
}

void DrvTimerGlobalTimerIsrRel(void)
{
    HalTimerIsrRel(DRV_GLOBAL_TIMER);
}

/*=============================================================*/
// Global function definition
/*=============================================================*/

#ifdef CONFIG_USE_ARCH_TIMER
void DrvSysTimerInit(void)
{
    // OSTick
    arch_timer_disable_int();
    arch_timer_disable();
    arch_timer_clear_int_flag();
    arch_timer_enable_int();
}
#else
void DrvSysTimerInit(void)
{
    // UDelay Use
    HalTimerStart(DRV_STDA_TIMER, HAL_STDATIMER_MAX_COUNT);

    // OSTick
    HalTimerDisableIntr(DRV_OS_TICK_TIMER);
    HalTimerStop(DRV_OS_TICK_TIMER);
    HalTimerClearIntr(DRV_OS_TICK_TIMER);
    HalTimerEnableIntr(DRV_OS_TICK_TIMER);

    // Global
    HalTimerDisableIntr(DRV_GLOBAL_TIMER);
    HalTimerStop(DRV_GLOBAL_TIMER);
    HalTimerClearIntr(DRV_GLOBAL_TIMER);
    HalTimerStart(DRV_GLOBAL_TIMER, HAL_OSTICK_MAX_COUNT);
}
#endif

#if defined(RTOS_KERNEL_USE_RTK)
/**
 * @brief   To transfer the hardware level tick to user level - 32bits
 */
u32 DrvTimerHwToUserTickGlobal(u32 x)
{
#ifdef CONFIG_USE_ARCH_TIMER
    return VM_RTK_MS_TO_TICK(arch_timer_tick_to_ms(x));
#else
    return VM_RTK_MS_TO_TICK(DrvTimerGlobalTimerTick2Ms(x));
#endif
}
/**
 * @brief   To transfer the user level tick to hardware level
 */
u32 DrvTimerUserToHwTickOstick(u32 x)
{
#ifdef CONFIG_USE_ARCH_TIMER
    return arch_timer_ms_to_tick(VM_RTK_TICK_TO_MS(x));
#else
    return DrvTimerMs2Ostick(VM_RTK_TICK_TO_MS(x));
#endif
}

/**
 * @brief   To transfer the hardware level tick to user level
 */
u32 DrvTimerHwToUserTickOstick(u32 x)
{
#ifdef CONFIG_USE_ARCH_TIMER
    return VM_RTK_MS_TO_TICK(arch_timer_tick_to_ms(x));
#else
    return VM_RTK_MS_TO_TICK(DrvTimerOstick2Ms(x));
#endif
}
#endif
