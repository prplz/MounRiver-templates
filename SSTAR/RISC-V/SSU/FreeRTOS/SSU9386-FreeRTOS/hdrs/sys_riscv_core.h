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

#ifndef __SYS_RISCV_CORE_H__
#define __SYS_RISCV_CORE_H__

#include "vm_types.ht"

/************************************************************************
* Macro define
************************************************************************/

#define isb()       __asm__ __volatile__ ("fence" : : : "memory")
#define dsb()       __asm__ __volatile__ ("fence" : : : "memory")
#define dsbish()    __asm__ __volatile__ ("fence" : : : "memory")
#define smp_mb()    __asm__ __volatile__ ("fence" : : : "memory")

#define wfi()       __asm__ __volatile__ ("wfi" : : : "memory")
#define wfe()
#define sev()

#define ASM_INFINITE_LOOP() __asm__ __volatile__ ("j .")

/************************************************************************
* Inline function
************************************************************************/
static inline void dsb_sev(void)
{
    asm volatile ("fence" : : : "memory");
    sev();
}

#endif  // __SYS_RISCV_CORE_H__
