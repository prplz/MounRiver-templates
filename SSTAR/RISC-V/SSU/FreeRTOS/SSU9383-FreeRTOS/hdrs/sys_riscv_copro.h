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

#ifndef __SYS_RISCV_COPRO_H__
#define __SYS_RISCV_COPRO_H__

#include "vm_types.ht"
#include "sys_sys_compiler.h"

//------------------------------------------------------------------------------
// Inline Functions
//------------------------------------------------------------------------------

ATTR_TARGET_ARM static inline void arch_local_int_disable(void)
{
    __asm__ __volatile__("csrci mstatus, 0x8\n"
                         : : : "memory", "cc");
}

ATTR_TARGET_ARM static inline void arch_local_int_enable(void)
{
    __asm__ __volatile__("csrsi mstatus, 0x8\n"
                         : : : "memory", "cc");
}

ATTR_TARGET_ARM static inline unsigned long arch_local_int_save(void)
{
    unsigned long flags;

    __asm__ __volatile__("csrrci %0, mstatus, 0x8\n"
                         : "=r" (flags) : : "memory", "cc");
    return flags;
}

ATTR_TARGET_ARM static inline void arch_local_int_restore(unsigned long flags)
{
    __asm__ __volatile__("csrw mstatus, %0" : : "r" (flags) : "memory", "cc");
}

// MPIDR
ATTR_TARGET_ARM static inline unsigned long arch_get_core_id(void)
{
    unsigned long ulRet = 0;

    return ulRet;
}

ATTR_TARGET_ARM static inline unsigned long arch_get_mcause(void)
{
    unsigned long ulRet;

    __asm__ __volatile__("csrr %0, mcause\n" : "=r" (ulRet) : : "memory", "cc");
    return ulRet;
}

ATTR_TARGET_ARM static inline unsigned long arch_get_sp(void)
{
    unsigned long ulRet;

    __asm__ __volatile__("mv %0, sp\n"
                         : "=r" (ulRet) : : "memory", "cc");
    return ulRet;
}

#ifdef CONFIG_SMP_SUPPORT
#define __ALT_SMP_ASM(smp, up)  \
    "9998: " smp "\n"           \
    " .pushsection \".alt.smp.init\", \"a\"\n" \
    " .align 2\n"               \
    " .long 9998b - .\n"        \
    " " up "\n"                 \
    " .popsection\n"
#else
#define __ALT_SMP_ASM(smp, up) up
#endif

ATTR_TARGET_ARM static inline void arch_prefetchw(const void *ptr)
{
}

#endif // __SYS_RISCV_COPRO_H__
