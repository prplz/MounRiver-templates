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

#ifndef __SYS_RISCV_CACHE_MMU_H__
#define __SYS_RISCV_CACHE_MMU_H__

#include "vm_types.ht"
#include "sys_MsWrapper_cus_os_int_pub.h"

/************************************************************************
* Constant
************************************************************************/

//------------------------------------------------------------------------------
// Macro Define : Memory Map Region Relative
//------------------------------------------------------------------------------

#define MMU_MMAP_REGION(va, pa, sz, attr)    {(va), (pa), (sz), (attr)}
#define MMU_MMAP_REGION_FLAT(addr, sz, attr) MMU_MMAP_REGION(addr, addr, sz, attr)

//------------------------------------------------------------------------------
// Macro Define : Memory Attribute Relative
//------------------------------------------------------------------------------

#define MEM_TYPE_UNKNOWN            ~0U

#define MEM_TYPE_MASK               U(0x7)
#define MEM_TYPE(_attr)             ((_attr) & MEM_TYPE_MASK)

#define MEM_TYPE_DEVICE             U(0)
#define MEM_TYPE_NON_CACHEABLE      U(1)
#define MEM_TYPE_MEMORY             U(2)

#define MEM_TYPE_PERM_SHIFT         U(3)
#define MEM_TYPE_SEC_SHIFT          U(4)
#define MEM_TYPE_EXECUTE_SHIFT      U(5)
#define MEM_TYPE_DOMAIN_SHIFT       U(6) // For Non-LPAE only

#define MEM_TYPE_RO                 (U(0) << MEM_TYPE_PERM_SHIFT)
#define MEM_TYPE_RW                 (U(1) << MEM_TYPE_PERM_SHIFT)

#define MEM_TYPE_SECURE             (U(0) << MEM_TYPE_SEC_SHIFT)
#define MEM_TYPE_NS                 (U(1) << MEM_TYPE_SEC_SHIFT)

#define MEM_TYPE_EXECUTE            (U(0) << MEM_TYPE_EXECUTE_SHIFT)
#define MEM_TYPE_EXECUTE_NEVER      (U(1) << MEM_TYPE_EXECUTE_SHIFT)

#define MEM_TYPE_DOMAIN_NO_ACCESS   (U(0) << MEM_TYPE_DOMAIN_SHIFT)
#define MEM_TYPE_DOMAIN_CLIENT      (U(1) << MEM_TYPE_DOMAIN_SHIFT)

#define MEM_TYPE_CODE               (MEM_TYPE_MEMORY | MEM_TYPE_RO | MEM_TYPE_EXECUTE)
#define MEM_TYPE_RO_DATA            (MEM_TYPE_MEMORY | MEM_TYPE_RO | MEM_TYPE_EXECUTE_NEVER)

#define DATA_CACHE_ENTRY_SIZE       64 // 64 bytes per data cache entry
#define DATA_CACHE_ENTRY_SHIFT      6  // 64 bytes per data cache entry

/************************************************************************
* Enumeration
************************************************************************/

/*!
 *   \brief error code to specify the result of mmu related function calls.
 */
typedef enum
{
    ERR_MMU_NO_ERROR = 0,               ///< There was no error
    ERR_MMU_MODIFICATION_NOT_ALLOWED,   ///< The modifications are not allowed
    ERR_MMU_INVALID_SEGMENT,            ///< The specified segment is not valid
    ERR_MMU_INVALID_SIZE,               ///< The specified segment size is not valid
    ERR_MMU_INVALID_ATTRIBUTTION,       ///< The specified attribution is not valid
    ERR_MMU_FAILED,                     ///< The function call failed
    ERR_MMU_INVALID_PAGE_TALE           ///< The page table address is 0
} mmu_errorCode_e;

typedef enum
{
    MMU_INFO_ADDR_ALIGNMENT = 0,
    MMU_INFO_SIZE_ALIGNMENT,
    MMU_INFO_MAX_CNT
} mmu_query_info_e;

typedef enum
{
    MMU_RANGE0 = 0,
    MMU_RANGE1,
    MMU_RANGE2,
    MMU_RANGE3
} mmu_range_e;

typedef enum
{
    MMU_CACHE_CACHEABLE = 0,
    MMU_CACHE_WRITECOMBINE,
    MMU_CACHE_UNCACHEABLE
} mmu_cache_e;

/************************************************************************
* Functions
************************************************************************/

void arch_cache_mmu_flush_data_cache(void);
void arch_cache_mmu_clean_data_cache(void);

void __invalidate_data_cache(void);
void __invalidate_inst_cache(void);
void __drain_write_buffer(void);

u8 arch_cache_flush_invalid(u32 u32Flush, u32 u32Invalid, u32 u32AllRange);
u8 arch_cache_set_range(u32 u32Start, u32 u32End);
u8 arch_cache_get_range(u32* pu32Start, u32* pu32End);
u8 arch_icache_flush_invalid(void);

void arch_cache_mmu_range_configure(mmu_range_e eRange, u32 u32Start, u32 u32End,
                                    mmu_cache_e eCache);
void arch_cache_mmu_range_enable(mmu_range_e eRange);
void arch_cache_mmu_range_disable(mmu_range_e eRange);
void arch_cache_mmu_init(void);

// Call by sys_core.c
void arch_cache_mmu_enable(void);
void arch_cache_mmu_disable(void);
int arch_cache_mmu_setup(void* va, u64 pa, unsigned long size, u32 option);
void arch_cache_mmu_reset(void);
void arch_cache_mmu_reset_range(void* va, unsigned long size);
void arch_cache_mmu_sync_tlb(void);
u64 arch_cache_mmu_va_to_pa(void* va);
u32 arch_cache_mmu_query(u32 query_id);

u64 arch_cache_mmu_phy_to_miu(u64 phyaddr);
u64 arch_cache_mmu_miu_to_phy(u64 miuaddr);

#endif // __SYS_RISCV_CACHE_MMU_H__
