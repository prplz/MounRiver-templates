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

#ifndef __KERNEL_ITCTRL_H__
#define __KERNEL_ITCTRL_H__

#include "cpu_mem_map.hc"

typedef struct KeIrqControl_s
{
    u32 u32_Fiq_Force0;     // force 15~0
    u32 u32_Fiq_Force1;     // force 31~16
    u32 u32_Fiq_Force2;     // force 47~32
    u32 u32_Fiq_Force3;     // force 63~48
    u32 u32_Fiq_Mask0;      // mask 15~0
    u32 u32_Fiq_Mask1;      // mask 31~16
    u32 u32_Fiq_Mask2;      // mask 47~32
    u32 u32_Fiq_Mask3;      // mask 63~48
    u32 u32_Fiq_Polarity0;  // polarity 15~0
    u32 u32_Fiq_Polarity1;  // polarity 31~16
    u32 u32_Fiq_Polarity2;  // polarity 47~32
    u32 u32_Fiq_Polarity3;  // polarity 63~48
    u32 u32_Fiq_Status0;    // status 15~0
    u32 u32_Fiq_Status1;    // status 31~16
    u32 u32_Fiq_Status2;    // status 47~32
    u32 u32_Fiq_Status3;    // status 63~48
    u32 u32_Irq_Force0;     // force 15~0
    u32 u32_Irq_Force1;     // force 31~16
    u32 u32_Irq_Force2;     // force 47~32
    u32 u32_Irq_Force3;     // force 63~48
    u32 u32_Irq_Mask0;      // mask 15~0
    u32 u32_Irq_Mask1;      // mask 31~16
    u32 u32_Irq_Mask2;      // mask 47~32
    u32 u32_Irq_Mask3;      // mask 63~48
    u32 u32_Irq_Polarity0;  // polarity 15~0
    u32 u32_Irq_Polarity1;  // polarity 31~16
    u32 u32_Irq_Polarity2;  // polarity 47~32
    u32 u32_Irq_Polarity3;  // polarity 63~48
    u32 u32_Irq_Status0;    // status 15~0
    u32 u32_Irq_Status1;    // status 31~16
    u32 u32_Irq_Status2;    // status 47~32
    u32 u32_Irq_Status3;    // status 63~48
} KeIrqControl_t;

extern volatile KeIrqControl_t* const g_pKeIrqControl;
extern volatile KeIrqControl_t* const g_pKeIrqControl2;

#define CONFIG_MS_INT_CTRL_CNT  2

#endif // __KERNEL_ITCTRL_H__
