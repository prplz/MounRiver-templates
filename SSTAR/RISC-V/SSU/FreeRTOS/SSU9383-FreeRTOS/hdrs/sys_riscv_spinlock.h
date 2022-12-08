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

#ifndef __SYS_RISCV_SPINLOCK_H__
#define __SYS_RISCV_SPINLOCK_H__

#include "vm_types.ht"
#include "sys_sys_compiler.h"

///////////////////////////////////////////////

///////////////////////////////////////////////

typedef struct
{
} spinlock_t;

///////////////////////////////////////////////

#define SPINLOCK_INITIALIZER   {}

///////////////////////////////////////////////

ATTR_TARGET_ARM static inline void arch_spin_lock_init(spinlock_t *ptLock)
{
}

#endif // __SYS_RISCV_SPINLOCK_H__
