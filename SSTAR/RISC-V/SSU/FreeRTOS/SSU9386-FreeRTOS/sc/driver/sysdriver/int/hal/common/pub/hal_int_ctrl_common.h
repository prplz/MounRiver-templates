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

#ifndef __HAL_INT_CTRL_COMMON_H__
#define __HAL_INT_CTRL_COMMON_H__

/*=============================================================*/
// Include files
/*=============================================================*/

#include "vm_types.ht"
#include "irqs.h"

/*=============================================================*/
// Macro definition
/*=============================================================*/

#define INTC_ISR_LIMIT              (4)

#define INTC_PRIORITY_0             0
#define INTC_PRIORITY_1             1
#define INTC_PRIORITY_2             2
#define INTC_PRIORITY_3             3
#define INTC_PRIORITY_4             4
#define INTC_PRIORITY_5             5
#define INTC_PRIORITY_6             6
#define INTC_PRIORITY_7             7
#define INTC_PRIORITY_8             8
#define INTC_PRIORITY_9             9
#define INTC_PRIORITY_10            10
#define INTC_PRIORITY_11            11
#define INTC_PRIORITY_12            12
#define INTC_PRIORITY_13            13
#define INTC_PRIORITY_14            14
#define INTC_PRIORITY_15            15

#if defined(RTOS_KERNEL_USE_FREERTOS) && defined(__FREERTOS_FIXME_PATCH__)
#define INTC_PRIORITY_MAX           (32)
#else
#define INTC_PRIORITY_MAX           (16)
#endif

#define INT_NO_AFFINITY             (0xFFFFFFFF)

#if defined(CONFIG_SMP_SUPPORT) && (CONFIG_CORE_NUM > 1)
#define IPI_SMP_TASK_YIELD          (0)
#define IPI_SMP_CORE_PANIC          (1)
#define IPI_SMP_CORE_PERF           (2)
#define IPI_SMP_DISABLE_INT         (3)
#define IPI_SMP_ENABLE_INT          (4)
#endif

/*=============================================================*/
// Typedef definition
/*=============================================================*/

typedef u32 IntNumber_e;
typedef u8  IntcPriority_e;

/**
 * Interrupt Service Routine type
 */
typedef void (*PfnIntcISR)(u32, void*);
typedef void (*PfnIntcMultiISR[INTC_ISR_LIMIT])(u32, void*);

/*=============================================================*/
// Enumeration definition
/*=============================================================*/

#ifdef CONFIG_CPU_ARCH_RISCV
/**
 * The interrupt line can be mapped on IRQ or FIQ
 */
typedef enum
{
    INTC_MAP_CCIF = 1,
    INTC_MAP_FIQ,
    INTC_MAP_IRQ,
    INTC_MAP_MAX
} IntcMap_e;
#else
/**
 * The interrupt line can be mapped on IRQ or FIQ
 */
typedef enum
{
    INTC_MAP_FIQ = 0,
    INTC_MAP_IRQ,
    INTC_MAP_MAX
} IntcMap_e;
#endif

/**
 * Polarity of the interrupt line
 */
typedef enum
{
    INTC_POLARITY_LOW = 0,
    INTC_POLARITY_HIGH,
    INTC_POLARITY_BOTH,
    INTC_POLARITY_MAX
} IntcPolarity_e;

/*=============================================================*/
// Structure definition
/*=============================================================*/

typedef struct
{
    IntcMap_e       eMap; // IRQ or FIQ
    IntcPriority_e  ePriority;
    PfnIntcISR      pfnIsr;
    void*           pDevId;
    char*           pcName;
    u32             u32Affinity;
} IntcParam_t;

typedef union
{
    IntcParam_t     intc;
} IntInitParam_u;

/*=============================================================*/
// Function prototype
/*=============================================================*/

void HalIntFreeIsr(u32 nIntcNumber, void* pDevId);

void HalIntInitIntController(void);
#if defined(CONFIG_STR_SUPPORT) || defined(CONFIG_POWER_MANAG_SUPPORT)
void HalIntResumeIntController(void);
#endif
s32 HalIntInitInterrupt(IntInitParam_u *puParam, IntNumber_e eIntNum);

void HalIntMaskInterrupt(IntNumber_e eIntNum);
void HalIntUnmaskInterrupt(IntNumber_e eIntNum);
bool HalIntIsMaskInterrupt(IntNumber_e eIntNum);
void HalIntClearInterrupt(IntNumber_e eIntNum);
void HalIntSetInterruptPolarity(IntcPolarity_e ePolarity, IntNumber_e eIntNum);

void HalIntSendSGI(u32 u32HwCoreID, u32 u32IntNum);
void HalIntShowInterruptStat(void);

u32 HalIntGetCurrentIrqNum(void);
#if defined(CONFIG_CPU_HOTPLUG_SUPPORT)
void HalIntCpuHotplug(u32 u32CoreID, u32 u32Online);
#endif

void HalIntCommonHandler(IntcMap_e eMap);

#endif // __HAL_INT_CTRL_COMMON_H__
