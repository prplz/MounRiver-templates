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

#ifndef __SYS_SYS_CORE_H__
#define __SYS_SYS_CORE_H__

#include "vm_types.ht"
#ifdef CONFIG_CPU_ARCH_RISCV
#include "riscv_core.h"
#include "sys_riscv_copro.h"
#include "sys_riscv_core.h"
#include "sys_riscv_cache_mmu.h"
#else
#include "sys_arm_copro.h"
#include "sys_arm_core.h"
#include "sys_arm_cache_mmu.h"
#endif

void sys_cache_mmu_enable(void);
void sys_cache_mmu_disable(void);
int sys_cache_mmu_setup(void* va, u64 pa, unsigned long size, u32 flag);
void sys_cache_mmu_reset(void);
void sys_cache_mmu_reset_range(void* va, unsigned long size);
void sys_cache_mmu_sync_tlb(void);
u64 sys_cache_mmu_va_to_pa(void* va);
u32 sys_cache_mmu_query(u32 id);

void sys_cache_mmu_clean_data_cache(void);

void sys_flush_data_cache_buffer(u32 addr, s32 size);
void sys_cache_mmu_flush_data_cache(void);

void sys_Invalidate_data_cache_buffer(u32 addr, s32 size);
void sys_Invalidate_data_cache(void);
void sys_Invalidate_inst_cache(void);

#endif // __SYS_SYS_CORE_H__
