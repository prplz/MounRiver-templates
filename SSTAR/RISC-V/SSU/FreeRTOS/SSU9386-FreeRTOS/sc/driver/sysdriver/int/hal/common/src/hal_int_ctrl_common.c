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

/*=============================================================*/
// Include files
/*=============================================================*/

#include "sys_MsWrapper_cus_os_int_pub.h"
#ifdef CONFIG_CPU_ARCH_RISCV
#include "sys_riscv_clic.h"
#include "sys_riscv_copro.h"
#else
#include "sys_arm_gic.h"
#include "sys_arm_copro.h"
#endif
#include "sys_sys_isw_uart.h"
#include "sys_arch_timer.h"
#include "cam_os_wrapper.h"
#include "drv_dualos.h"
#include "hal_int_ctrl_pub.h"
#if defined (RTOS_KERNEL_USE_RTK)
#include "sys_rtk_hp.h"
#endif
#if defined(CONFIG_STACK_DIAGONSE)
#include "sys_sys_dbg.h"
#endif
#include "sys_swla.h"
#include "kernel.h"
#include "ms_platform.h"

/*=============================================================*/
// Macro definition
/*=============================================================*/

#define INT_MAX_NUMBER                      (GIC_SGI_NR + GIC_PPI_NR + GIC_SPI_ARM_INTERNAL_NR + GIC_SPI_MS_IRQ_NR + GIC_SPI_MS_FIQ_NR)

#define MAX_INT_NUM_PER_HOST_CTLR           (64)

#define INT_CONTROLER_1                     (0)
#define INT_CONTROLER_2                     (1)

#define INT_MAX_NAME_LEN                    (16)

#define INTC_NEST_IRQ_LIMIT                 (10)

#define INTC_FLAG_FIQ                       (0x0001)
#define INTC_FLAG_LEVEL_DETECT              (0x0002)
#define INTC_FLAG_PRE_CLEAR                 (0x0004)
#define INTC_FLAG_POST_CLEAR                (0x0008)
#define INTC_FLAG_NOT_SHARE                 (0x0010)

#define INTC_HASHTBL_CNT_BIT_SHIFT          (4) // 2^4

#if defined(CONFIG_SMP_SUPPORT) && (CONFIG_CORE_NUM > 1)
#define INTC_LOADING_BALANCE_EN             (0)
#else
#define INTC_LOADING_BALANCE_EN             (0)
#endif

#define INTC_DBG_EN                         (0)

#if (INTC_DBG_EN)
#define INTC_DEBUG(...)                     CamOsPrintf(__VA_ARGS__)
#else
#define INTC_DEBUG(...)
#endif
#define INTC_ERROR(...)                     CamOsPrintf(KERN_EMERG __VA_ARGS__)

#if (CONFIG_MS_INT_CTRL_CNT == 2)

#define INTC_GET_MS_INT_CTRL_ID(int, x)     do {(x) = (int >= MAX_INT_NUM_PER_HOST_CTLR) ? (INT_CONTROLER_2) : (INT_CONTROLER_1);} while(0)

#define INTC_READ_FIQ_MASK0_REG(c ,x)       do {BYTE_ENABLE_BUG_WORKAROUND; (x) = (c == INT_CONTROLER_2) ? (g_pKeIrqControl2->u32_Fiq_Mask0) : (g_pKeIrqControl->u32_Fiq_Mask0);} while(0)
#define INTC_READ_FIQ_MASK1_REG(c ,x)       do {BYTE_ENABLE_BUG_WORKAROUND; (x) = (c == INT_CONTROLER_2) ? (g_pKeIrqControl2->u32_Fiq_Mask1) : (g_pKeIrqControl->u32_Fiq_Mask1);} while(0)
#define INTC_READ_IRQ_MASK0_REG(c ,x)       do {BYTE_ENABLE_BUG_WORKAROUND; (x) = (c == INT_CONTROLER_2) ? (g_pKeIrqControl2->u32_Irq_Mask0) : (g_pKeIrqControl->u32_Irq_Mask0);} while(0)
#define INTC_READ_IRQ_MASK1_REG(c ,x)       do {BYTE_ENABLE_BUG_WORKAROUND; (x) = (c == INT_CONTROLER_2) ? (g_pKeIrqControl2->u32_Irq_Mask1) : (g_pKeIrqControl->u32_Irq_Mask1);} while(0)
#define INTC_READ_IRQ_MASK2_REG(c ,x)       do {BYTE_ENABLE_BUG_WORKAROUND; (x) = (c == INT_CONTROLER_2) ? (g_pKeIrqControl2->u32_Irq_Mask2) : (g_pKeIrqControl->u32_Irq_Mask2);} while(0)
#define INTC_READ_IRQ_MASK3_REG(c ,x)       do {BYTE_ENABLE_BUG_WORKAROUND; (x) = (c == INT_CONTROLER_2) ? (g_pKeIrqControl2->u32_Irq_Mask3) : (g_pKeIrqControl->u32_Irq_Mask3);} while(0)

#define INTC_READ_FIQ_STATUS0_REG(c ,x)     do {BYTE_ENABLE_BUG_WORKAROUND; (x) = (c == INT_CONTROLER_2) ? (g_pKeIrqControl2->u32_Fiq_Status0) : (g_pKeIrqControl->u32_Fiq_Status0);} while(0)
#define INTC_READ_FIQ_STATUS1_REG(c ,x)     do {BYTE_ENABLE_BUG_WORKAROUND; (x) = (c == INT_CONTROLER_2) ? (g_pKeIrqControl2->u32_Fiq_Status1) : (g_pKeIrqControl->u32_Fiq_Status1);} while(0)
#define INTC_READ_IRQ_STATUS0_REG(c ,x)     do {BYTE_ENABLE_BUG_WORKAROUND; (x) = (c == INT_CONTROLER_2) ? (g_pKeIrqControl2->u32_Irq_Status0) : (g_pKeIrqControl->u32_Irq_Status0);} while(0)
#define INTC_READ_IRQ_STATUS1_REG(c ,x)     do {BYTE_ENABLE_BUG_WORKAROUND; (x) = (c == INT_CONTROLER_2) ? (g_pKeIrqControl2->u32_Irq_Status1) : (g_pKeIrqControl->u32_Irq_Status1);} while(0)
#define INTC_READ_IRQ_STATUS2_REG(c ,x)     do {BYTE_ENABLE_BUG_WORKAROUND; (x) = (c == INT_CONTROLER_2) ? (g_pKeIrqControl2->u32_Irq_Status2) : (g_pKeIrqControl->u32_Irq_Status2);} while(0)
#define INTC_READ_IRQ_STATUS3_REG(c ,x)     do {BYTE_ENABLE_BUG_WORKAROUND; (x) = (c == INT_CONTROLER_2) ? (g_pKeIrqControl2->u32_Irq_Status3) : (g_pKeIrqControl->u32_Irq_Status3);} while(0)

#define INTC_WRITE_FIQ_MASK0_REG(c, x)      do {if (c == INT_CONTROLER_2) g_pKeIrqControl2->u32_Fiq_Mask0 = (x); else g_pKeIrqControl->u32_Fiq_Mask0 = (x);} while(0)
#define INTC_WRITE_FIQ_MASK1_REG(c, x)      do {if (c == INT_CONTROLER_2) g_pKeIrqControl2->u32_Fiq_Mask1 = (x); else g_pKeIrqControl->u32_Fiq_Mask1 = (x);} while(0)
#define INTC_WRITE_IRQ_MASK0_REG(c, x)      do {if (c == INT_CONTROLER_2) g_pKeIrqControl2->u32_Irq_Mask0 = (x); else g_pKeIrqControl->u32_Irq_Mask0 = (x);} while(0)
#define INTC_WRITE_IRQ_MASK1_REG(c, x)      do {if (c == INT_CONTROLER_2) g_pKeIrqControl2->u32_Irq_Mask1 = (x); else g_pKeIrqControl->u32_Irq_Mask1 = (x);} while(0)
#define INTC_WRITE_IRQ_MASK2_REG(c, x)      do {if (c == INT_CONTROLER_2) g_pKeIrqControl2->u32_Irq_Mask2 = (x); else g_pKeIrqControl->u32_Irq_Mask2 = (x);} while(0)
#define INTC_WRITE_IRQ_MASK3_REG(c, x)      do {if (c == INT_CONTROLER_2) g_pKeIrqControl2->u32_Irq_Mask3 = (x); else g_pKeIrqControl->u32_Irq_Mask3 = (x);} while(0)

#define INTC_CLR_INT_FIQ0_REG(c, mask)      do {if (c == INT_CONTROLER_2) g_pKeIrqControl2->u32_Fiq_Status0 = mask; else g_pKeIrqControl->u32_Fiq_Status0 = mask;} while(0)
#define INTC_CLR_INT_FIQ1_REG(c, mask)      do {if (c == INT_CONTROLER_2) g_pKeIrqControl2->u32_Fiq_Status1 = mask; else g_pKeIrqControl->u32_Fiq_Status1 = mask;} while(0)
#define INTC_CLR_INT_FIQ2_REG(c, mask)      do {if (c == INT_CONTROLER_2) g_pKeIrqControl2->u32_Fiq_Status2 = mask; else g_pKeIrqControl->u32_Fiq_Status2 = mask;} while(0)
#define INTC_CLR_INT_FIQ3_REG(c, mask)      do {if (c == INT_CONTROLER_2) g_pKeIrqControl2->u32_Fiq_Status3 = mask; else g_pKeIrqControl->u32_Fiq_Status3 = mask;} while(0)

#define INTC_READ_FIQ_POLARITY0_REG(c, x)   do {BYTE_ENABLE_BUG_WORKAROUND; (x) = (c == INT_CONTROLER_2) ? (g_pKeIrqControl2->u32_Fiq_Polarity0) : (g_pKeIrqControl->u32_Fiq_Polarity0);} while(0)
#define INTC_READ_FIQ_POLARITY1_REG(c, x)   do {BYTE_ENABLE_BUG_WORKAROUND; (x) = (c == INT_CONTROLER_2) ? (g_pKeIrqControl2->u32_Fiq_Polarity1) : (g_pKeIrqControl->u32_Fiq_Polarity1);} while(0)
#define INTC_READ_IRQ_POLARITY0_REG(c, x)   do {BYTE_ENABLE_BUG_WORKAROUND; (x) = (c == INT_CONTROLER_2) ? (g_pKeIrqControl2->u32_Irq_Polarity0) : (g_pKeIrqControl->u32_Irq_Polarity0);} while(0)
#define INTC_READ_IRQ_POLARITY1_REG(c, x)   do {BYTE_ENABLE_BUG_WORKAROUND; (x) = (c == INT_CONTROLER_2) ? (g_pKeIrqControl2->u32_Irq_Polarity1) : (g_pKeIrqControl->u32_Irq_Polarity1);} while(0)
#define INTC_READ_IRQ_POLARITY2_REG(c, x)   do {BYTE_ENABLE_BUG_WORKAROUND; (x) = (c == INT_CONTROLER_2) ? (g_pKeIrqControl2->u32_Irq_Polarity2) : (g_pKeIrqControl->u32_Irq_Polarity2);} while(0)
#define INTC_READ_IRQ_POLARITY3_REG(c, x)   do {BYTE_ENABLE_BUG_WORKAROUND; (x) = (c == INT_CONTROLER_2) ? (g_pKeIrqControl2->u32_Irq_Polarity3) : (g_pKeIrqControl->u32_Irq_Polarity3);} while(0)

#define INTC_WRITE_FIQ_POLARITY0_REG(c, x)  do {if (c == INT_CONTROLER_2) g_pKeIrqControl2->u32_Fiq_Polarity0 = (x); else g_pKeIrqControl->u32_Fiq_Polarity0 = (x);} while(0)
#define INTC_WRITE_FIQ_POLARITY1_REG(c, x)  do {if (c == INT_CONTROLER_2) g_pKeIrqControl2->u32_Fiq_Polarity1 = (x); else g_pKeIrqControl->u32_Fiq_Polarity1 = (x);} while(0)
#define INTC_WRITE_IRQ_POLARITY0_REG(c, x)  do {if (c == INT_CONTROLER_2) g_pKeIrqControl2->u32_Irq_Polarity0 = (x); else g_pKeIrqControl->u32_Irq_Polarity0 = (x);} while(0)
#define INTC_WRITE_IRQ_POLARITY1_REG(c, x)  do {if (c == INT_CONTROLER_2) g_pKeIrqControl2->u32_Irq_Polarity1 = (x); else g_pKeIrqControl->u32_Irq_Polarity1 = (x);} while(0)
#define INTC_WRITE_IRQ_POLARITY2_REG(c, x)  do {if (c == INT_CONTROLER_2) g_pKeIrqControl2->u32_Irq_Polarity2 = (x); else g_pKeIrqControl->u32_Irq_Polarity2 = (x);} while(0)
#define INTC_WRITE_IRQ_POLARITY3_REG(c, x)  do {if (c == INT_CONTROLER_2) g_pKeIrqControl2->u32_Irq_Polarity3 = (x); else g_pKeIrqControl->u32_Irq_Polarity3 = (x);} while(0)

#else

#define INTC_GET_MS_INT_CTRL_ID(int, x)     do {(x) = (INT_CONTROLER_1);} while(0)

#define INTC_READ_FIQ_MASK0_REG(c ,x)       do {(x) = (g_pKeIrqControl->u32_Fiq_Mask0);} while(0)
#define INTC_READ_FIQ_MASK1_REG(c ,x)       do {(x) = (g_pKeIrqControl->u32_Fiq_Mask1);} while(0)
#define INTC_READ_IRQ_MASK0_REG(c ,x)       do {(x) = (g_pKeIrqControl->u32_Irq_Mask0);} while(0)
#define INTC_READ_IRQ_MASK1_REG(c ,x)       do {(x) = (g_pKeIrqControl->u32_Irq_Mask1);} while(0)
#define INTC_READ_IRQ_MASK2_REG(c ,x)       do {(x) = (g_pKeIrqControl->u32_Irq_Mask2);} while(0)
#define INTC_READ_IRQ_MASK3_REG(c ,x)       do {(x) = (g_pKeIrqControl->u32_Irq_Mask3);} while(0)

#define INTC_READ_FIQ_STATUS0_REG(c ,x)     do {(x) = (g_pKeIrqControl->u32_Fiq_Status0);} while(0)
#define INTC_READ_FIQ_STATUS1_REG(c ,x)     do {(x) = (g_pKeIrqControl->u32_Fiq_Status1);} while(0)
#define INTC_READ_IRQ_STATUS0_REG(c ,x)     do {(x) = (g_pKeIrqControl->u32_Irq_Status0);} while(0)
#define INTC_READ_IRQ_STATUS1_REG(c ,x)     do {(x) = (g_pKeIrqControl->u32_Irq_Status1);} while(0)
#define INTC_READ_IRQ_STATUS2_REG(c ,x)     do {(x) = (g_pKeIrqControl->u32_Irq_Status2);} while(0)
#define INTC_READ_IRQ_STATUS3_REG(c ,x)     do {(x) = (g_pKeIrqControl->u32_Irq_Status3);} while(0)

#define INTC_WRITE_FIQ_MASK0_REG(c, x)      do {g_pKeIrqControl->u32_Fiq_Mask0 = (x);} while(0)
#define INTC_WRITE_FIQ_MASK1_REG(c, x)      do {g_pKeIrqControl->u32_Fiq_Mask1 = (x);} while(0)
#define INTC_WRITE_IRQ_MASK0_REG(c, x)      do {g_pKeIrqControl->u32_Irq_Mask0 = (x);} while(0)
#define INTC_WRITE_IRQ_MASK1_REG(c, x)      do {g_pKeIrqControl->u32_Irq_Mask1 = (x);} while(0)
#define INTC_WRITE_IRQ_MASK2_REG(c, x)      do {g_pKeIrqControl->u32_Irq_Mask2 = (x);} while(0)
#define INTC_WRITE_IRQ_MASK3_REG(c, x)      do {g_pKeIrqControl->u32_Irq_Mask3 = (x);} while(0)

#define INTC_CLR_INT_FIQ0_REG(c, mask)      do {g_pKeIrqControl->u32_Fiq_Status0 = mask;} while(0)
#define INTC_CLR_INT_FIQ1_REG(c, mask)      do {g_pKeIrqControl->u32_Fiq_Status1 = mask;} while(0)
#define INTC_CLR_INT_FIQ2_REG(c, mask)      do {g_pKeIrqControl->u32_Fiq_Status2 = mask;} while(0)
#define INTC_CLR_INT_FIQ3_REG(c, mask)      do {g_pKeIrqControl->u32_Fiq_Status3 = mask;} while(0)

#define INTC_READ_FIQ_POLARITY0_REG(c, x)   do {(x) = (g_pKeIrqControl->u32_Fiq_Polarity0);} while(0)
#define INTC_READ_FIQ_POLARITY1_REG(c, x)   do {(x) = (g_pKeIrqControl->u32_Fiq_Polarity1);} while(0)
#define INTC_READ_IRQ_POLARITY0_REG(c, x)   do {(x) = (g_pKeIrqControl->u32_Irq_Polarity0);} while(0)
#define INTC_READ_IRQ_POLARITY1_REG(c, x)   do {(x) = (g_pKeIrqControl->u32_Irq_Polarity1);} while(0)
#define INTC_READ_IRQ_POLARITY2_REG(c, x)   do {(x) = (g_pKeIrqControl->u32_Irq_Polarity2);} while(0)
#define INTC_READ_IRQ_POLARITY3_REG(c, x)   do {(x) = (g_pKeIrqControl->u32_Irq_Polarity3);} while(0)

#define INTC_WRITE_FIQ_POLARITY0_REG(c, x)  do {g_pKeIrqControl->u32_Fiq_Polarity0 = (x);} while(0)
#define INTC_WRITE_FIQ_POLARITY1_REG(c, x)  do {g_pKeIrqControl->u32_Fiq_Polarity1 = (x);} while(0)
#define INTC_WRITE_IRQ_POLARITY0_REG(c, x)  do {g_pKeIrqControl->u32_Irq_Polarity0 = (x);} while(0)
#define INTC_WRITE_IRQ_POLARITY1_REG(c, x)  do {g_pKeIrqControl->u32_Irq_Polarity1 = (x);} while(0)
#define INTC_WRITE_IRQ_POLARITY2_REG(c, x)  do {g_pKeIrqControl->u32_Irq_Polarity2 = (x);} while(0)
#define INTC_WRITE_IRQ_POLARITY3_REG(c, x)  do {g_pKeIrqControl->u32_Irq_Polarity3 = (x);} while(0)

#endif // CONFIG_MS_INT_CTRL_CNT

/*=============================================================*/
// Enum definition
/*=============================================================*/

typedef enum
{
    INT_MASK_STATUS_MASKED = 0,
    INT_MASK_STATUS_UNMASK = 1,
} IntMaskStatus_e;

/*=============================================================*/
// Data type definition
/*=============================================================*/

typedef struct IntrAttr_s
{
    struct CamOsHListNode_t tEntry;
    PfnIntcISR              pfnIsr;
    void*                   pvDevId;
    u16                     u16Flag;
    IntcPriority_e          ePriority;    /* Should be 8bit */
    #if defined(CONFIG_STR_SUPPORT) || defined(CONFIG_POWER_MANAG_SUPPORT)
    IntMaskStatus_e         eMaskStatus;
    #endif
    u32                     u32IntNum;
    u32                     u32Affinity;
    #if defined(CONFIG_CPU_HOTPLUG_SUPPORT)
    u32                     u32BackUpAffinity;
    #endif
    char                    pcName[INT_MAX_NAME_LEN];
    /* Run-time statistics */
    u32                     u32AccuExecCnt[CONFIG_CORE_NUM];
    u64                     u64AccuExecTick[CONFIG_CORE_NUM];
    u64                     u64MaxExecTick[CONFIG_CORE_NUM];
} IntrAttr_t;

/*=============================================================*/
// Variable definition
/*=============================================================*/

volatile u32 m_u32InISRCtxt[CONFIG_CORE_NUM] = {0};
volatile u32 m_u32CurProcIntId[CONFIG_CORE_NUM] = {0};
static s32 m_s32IsrNestedLevel[CONFIG_CORE_NUM] = {0};

static CamOsSpinlock_t m_tIntSpinlock = {0};

static CAM_OS_DEFINE_HASHTABLE(gtIntrHashTbl, INTC_HASHTBL_CNT_BIT_SHIFT);

/*=============================================================*/
// Local function definition
/*=============================================================*/

static void _HalIntcSetPriority(u32 u32IntNum, IntcPriority_e ePriority)
{
    if (u32IntNum >= INT_MAX_NUMBER)
    {
        INTC_ERROR("[%s][%d] Invalid IntNum=%d\r\n", __FUNCTION__, __LINE__, u32IntNum);
        return;
    }

#ifndef CONFIG_CPU_ARCH_RISCV
#if defined(CONFIG_ARM_GIC_V3_SUPPORT)
    ePriority = ePriority << 2; // TBD : Shift 3 bits due to BPR
#else
    ePriority = ePriority << 3;
#endif

    arm_gic_irq_set_priority(u32IntNum, ePriority);
#endif
}

static void _HalIntcSetGroup(u32 u32IntNum)
{
    if (u32IntNum >= INT_MAX_NUMBER)
    {
        INTC_ERROR("[%s][%d] Invalid IntNum=%d\r\n", __FUNCTION__, __LINE__, u32IntNum);
        return;
    }

#ifndef CONFIG_CPU_ARCH_RISCV
    // For interrupt in the current world.
#if defined(CONFIG_ARM_GIC_V3_SUPPORT)
    #ifdef CONFIG_GIC_DUAL_SECURITY_SUPPORT
        #if defined(CONFIG_INT_GRP_G0_FIQ)
        arm_gic_irq_set_group(u32IntNum, GIC_INT_GRP_G0);
        #elif defined(CONFIG_INT_GRP_G1S)
        arm_gic_irq_set_group(u32IntNum, GIC_INT_GRP_G1S);
        #else // CONFIG_INT_GRP_G1NS
        arm_gic_irq_set_group(u32IntNum, GIC_INT_GRP_G1NS);
        #endif
    #else
        #if defined(CONFIG_INT_GRP_G0_FIQ)
        arm_gic_irq_set_group(u32IntNum, GIC_INT_GRP_G0);
        #else // CONFIG_INT_GRP_G1_IRQ
        arm_gic_irq_set_group(u32IntNum, GIC_INT_GRP_G1);
        #endif
    #endif
#else
    #if defined(CONFIG_INT_GRP_G0_FIQ)
    arm_gic_irq_set_group(u32IntNum, GIC_INT_GRP_G0);
    #else // CONFIG_INT_GRP_G1_IRQ
    arm_gic_irq_set_group(u32IntNum, GIC_INT_GRP_G1);
    #endif
#endif
#endif
}

static void _HalIntcSetTargetCores(u32 u32IntNum, u32 u32Affinity)
{
    if (u32IntNum >= INT_MAX_NUMBER)
    {
        INTC_ERROR("[%s][%d] Invalid IntNum=%d\r\n", __FUNCTION__, __LINE__, u32IntNum);
        return;
    }

#ifndef CONFIG_CPU_ARCH_RISCV
    if (u32Affinity == 0)
    {
    #if defined(CONFIG_ARM_GIC_V3_SUPPORT)
        #ifdef CONFIG_GIC_AFFINITY_ROUTING_SUPPORT
        arm_gic_irq_set_router(u32IntNum, (u32)arch_get_core_id());
        #else
        arm_gic_irq_set_target(u32IntNum, (u32)(1 << CUR_HW_CORE_ID));
        #endif
    #else
        arm_gic_irq_set_target(u32IntNum, (u32)(1 << CUR_HW_CORE_ID));
    #endif
    }
    else
    {
        u32 i = 0, u32HwAffinity = 0;

        // Transform logical affinity to HW affinity
        if (u32Affinity != INT_NO_AFFINITY)
        {
            for (i = 0; i < CONFIG_CORE_NUM; i++)
            {
                if (u32Affinity & (1 << i))
                {
                    if (LOGICAL_TO_HW_CORE_ID(i) != INVALID_CORE_ID)
                    {
                        u32HwAffinity |= (1 << LOGICAL_TO_HW_CORE_ID(i));
                    }
                }
            }
        }

        INTC_DEBUG(KERN_EMERG "[INT] SetTargetCores [%d] Affinity[%x][%x]\r\n", u32IntNum, u32Affinity, u32HwAffinity);

    #if defined(CONFIG_ARM_GIC_V3_SUPPORT)
        #ifdef CONFIG_GIC_AFFINITY_ROUTING_SUPPORT
        arm_gic_irq_set_router(u32IntNum, (u32)arch_get_core_id()); // TBD, if affinity set to INT_NO_AFFININT.
        #else
        arm_gic_irq_set_target(u32IntNum, u32HwAffinity);
        #endif
    #else
        arm_gic_irq_set_target(u32IntNum, u32HwAffinity);
    #endif
    }
#endif
}

static void _HalIntMaskInterrupt(IntNumber_e eIntNum, bool bHasLocked)
{
    MsIntMask_e eIntMask = 0;
    s16 ms_irq = 0;
    s16 ms_fiq = 0;
    u32 val = 0;
    u8  u8IntCtrler = INT_CONTROLER_1;
#if defined(CONFIG_STR_SUPPORT) || defined(CONFIG_POWER_MANAG_SUPPORT)
    IntrAttr_t *ptHashListNodeTmp = NULL;
    struct CamOsHListNode_t *n = NULL;
#endif

    u8IntCtrler = u8IntCtrler;

    if (eIntNum >= INT_MAX_NUMBER)
    {
        INTC_ERROR("[%s][%d] Invalid IntNum=%d\r\n", __FUNCTION__, __LINE__, eIntNum);
        return;
    }

    eIntMask = MsDisableInterrupt();

#ifndef CONFIG_CPU_ARCH_RISCV
    arm_gic_irq_disable(eIntNum);
#endif

#if defined(CONFIG_STR_SUPPORT) || defined(CONFIG_POWER_MANAG_SUPPORT)
    if (!bHasLocked)
        CamOsSpinLockIrqSave(&m_tIntSpinlock);

    CAM_OS_HASH_FOR_EACH_POSSIBLE_SAFE(gtIntrHashTbl, ptHashListNodeTmp, n, tEntry, eIntNum)
    {
        if (ptHashListNodeTmp->u32IntNum == eIntNum)
            ptHashListNodeTmp->eMaskStatus = INT_MASK_STATUS_MASKED;
    }

    if (!bHasLocked)
        CamOsSpinUnlockIrqRestore(&m_tIntSpinlock);
#endif

    if (eIntNum < GIC_SPI_MS_IRQ_START)
    {
        goto L_MaskInt;
    }

    ms_irq = eIntNum - GIC_SPI_MS_IRQ_START;
    ms_fiq = eIntNum - GIC_SPI_MS_FIQ_START;

    if (ms_fiq >= 0 && ms_fiq < GIC_SPI_MS_FIQ_NR)
    {
#ifdef CONFIG_CPU_ARCH_RISCV
        riscv_PLIC_disable_interrupt(EN_FIQ);
#endif
        INTC_GET_MS_INT_CTRL_ID(ms_fiq, u8IntCtrler);

        if (ms_fiq >= MAX_INT_NUM_PER_HOST_CTLR)
            ms_fiq -= MAX_INT_NUM_PER_HOST_CTLR;

        if ((ms_fiq/16) == 0)
        {
            INTC_READ_FIQ_MASK0_REG(u8IntCtrler, val);
            INTC_WRITE_FIQ_MASK0_REG(u8IntCtrler, val | (1 << (ms_fiq%16)) | ((1 << (ms_fiq%16)) << 16));
        }
        else
        {
            INTC_READ_FIQ_MASK1_REG(u8IntCtrler, val);
            INTC_WRITE_FIQ_MASK1_REG(u8IntCtrler, val | (1 << (ms_fiq%16)) | ((1 << (ms_fiq%16)) << 16));
        }
    }
    else if (ms_irq >= 0 && ms_irq < GIC_SPI_MS_IRQ_NR)
    {
#ifdef CONFIG_CPU_ARCH_RISCV
        riscv_PLIC_disable_interrupt(EN_IRQ);
#endif
        INTC_GET_MS_INT_CTRL_ID(ms_irq, u8IntCtrler);

        if (ms_irq >= MAX_INT_NUM_PER_HOST_CTLR)
            ms_irq -= MAX_INT_NUM_PER_HOST_CTLR;

        if ((ms_irq/16) == 0)
        {
            INTC_READ_IRQ_MASK0_REG(u8IntCtrler, val);
            INTC_WRITE_IRQ_MASK0_REG(u8IntCtrler, val | (1 << (ms_irq%16)) | ((1 << (ms_irq%16)) << 16));
        }
        else if ((ms_irq/16) == 1)
        {
            INTC_READ_IRQ_MASK1_REG(u8IntCtrler, val);
            INTC_WRITE_IRQ_MASK1_REG(u8IntCtrler, val | (1 << (ms_irq%16)) | ((1 << (ms_irq%16)) << 16));
        }
        else if ((ms_irq/16) == 2)
        {
            INTC_READ_IRQ_MASK2_REG(u8IntCtrler, val);
            INTC_WRITE_IRQ_MASK2_REG(u8IntCtrler, val | (1 << (ms_irq%16)) | ((1 << (ms_irq%16)) << 16));
        }
        else
        {
            INTC_READ_IRQ_MASK3_REG(u8IntCtrler, val);
            INTC_WRITE_IRQ_MASK3_REG(u8IntCtrler, val | (1 << (ms_irq%16)) | ((1 << (ms_irq%16)) << 16));
        }
    }
    else
    {
        INTC_DEBUG("[%s][%d] Invalid IntNum=%d\r\n", __FUNCTION__, __LINE__, eIntNum);
    }

L_MaskInt:

    MsEnableInterrupt(eIntMask);
}

static void _HalIntUnmaskInterrupt(IntNumber_e eIntNum, bool bHasLocked)
{
    MsIntMask_e eIntMask = 0;
    s16 ms_irq = 0;
    s16 ms_fiq = 0;
    u32 val = 0;
    u8  u8IntCtrler = INT_CONTROLER_1;
#if defined(CONFIG_STR_SUPPORT) || defined(CONFIG_POWER_MANAG_SUPPORT)
    IntrAttr_t *ptHashListNodeTmp = NULL;
    struct CamOsHListNode_t *n = NULL;
#endif

    u8IntCtrler = u8IntCtrler;

    if (eIntNum >= INT_MAX_NUMBER)
    {
        INTC_ERROR("[%s][%d] Invalid IntNum=%d\r\n", __FUNCTION__, __LINE__, eIntNum);
        return;
    }

    eIntMask = MsDisableInterrupt();

#ifndef CONFIG_CPU_ARCH_RISCV
    arm_gic_irq_enable(eIntNum);
#endif

#if defined(CONFIG_STR_SUPPORT) || defined(CONFIG_POWER_MANAG_SUPPORT)
    if (!bHasLocked)
        CamOsSpinLockIrqSave(&m_tIntSpinlock);

    CAM_OS_HASH_FOR_EACH_POSSIBLE_SAFE(gtIntrHashTbl, ptHashListNodeTmp, n, tEntry, eIntNum)
    {
        if (ptHashListNodeTmp->u32IntNum == eIntNum)
            ptHashListNodeTmp->eMaskStatus = INT_MASK_STATUS_UNMASK;
    }

    if (!bHasLocked)
        CamOsSpinUnlockIrqRestore(&m_tIntSpinlock);
#endif

    if (eIntNum < GIC_SPI_MS_IRQ_START)
    {
        goto L_UnMaskInt;
    }

    ms_irq = eIntNum - GIC_SPI_MS_IRQ_START;
    ms_fiq = eIntNum - GIC_SPI_MS_FIQ_START;

    if (ms_fiq >= 0 && ms_fiq < GIC_SPI_MS_FIQ_NR)
    {
#ifdef CONFIG_CPU_ARCH_RISCV
        riscv_PLIC_enable_interrupt(EN_FIQ);
#endif
        INTC_GET_MS_INT_CTRL_ID(ms_fiq, u8IntCtrler);

        if (ms_fiq >= MAX_INT_NUM_PER_HOST_CTLR)
            ms_fiq -= MAX_INT_NUM_PER_HOST_CTLR;

        if ((ms_fiq/16) == 0)
        {
            INTC_READ_FIQ_MASK0_REG(u8IntCtrler, val);
            INTC_WRITE_FIQ_MASK0_REG(u8IntCtrler, (val & ~(1 << (ms_fiq%16))) | ((1 << (ms_fiq%16)) << 16));
        }
        else
        {
            INTC_READ_FIQ_MASK1_REG(u8IntCtrler, val);
            INTC_WRITE_FIQ_MASK1_REG(u8IntCtrler, (val & ~(1 << (ms_fiq%16))) | ((1 << (ms_fiq%16)) << 16));
        }
    }
    else if (ms_irq >= 0 && ms_irq < GIC_SPI_MS_IRQ_NR)
    {
#ifdef CONFIG_CPU_ARCH_RISCV
        riscv_PLIC_enable_interrupt(EN_IRQ);
#endif
        INTC_GET_MS_INT_CTRL_ID(ms_irq, u8IntCtrler);

        if (ms_irq >= MAX_INT_NUM_PER_HOST_CTLR)
            ms_irq -= MAX_INT_NUM_PER_HOST_CTLR;

        if ((ms_irq/16) == 0)
        {
            INTC_READ_IRQ_MASK0_REG(u8IntCtrler, val);
            INTC_WRITE_IRQ_MASK0_REG(u8IntCtrler, (val & ~(1 << (ms_irq%16))) | ((1 << (ms_irq%16)) << 16));
        }
        else if ((ms_irq/16) == 1)
        {
            INTC_READ_IRQ_MASK1_REG(u8IntCtrler, val);
            INTC_WRITE_IRQ_MASK1_REG(u8IntCtrler, (val & ~(1 << (ms_irq%16))) | ((1 << (ms_irq%16)) << 16));
        }
        else if ((ms_irq/16) == 2)
        {
            INTC_READ_IRQ_MASK2_REG(u8IntCtrler, val);
            INTC_WRITE_IRQ_MASK2_REG(u8IntCtrler, (val & ~(1 << (ms_irq%16))) | ((1 << (ms_irq%16)) << 16));
        }
        else
        {
            INTC_READ_IRQ_MASK3_REG(u8IntCtrler, val);
            INTC_WRITE_IRQ_MASK3_REG(u8IntCtrler, (val & ~(1 << (ms_irq%16))) | ((1 << (ms_irq%16)) << 16));
        }
    }
    else
    {
         INTC_DEBUG("[%s][%d] Invalid eIntNum=%d\r\n", __FUNCTION__, __LINE__, eIntNum);
    }

L_UnMaskInt:

    MsEnableInterrupt(eIntMask);
}

/*=============================================================*/
// Global function definition
/*=============================================================*/

void HalIntFreeIsr(u32 u32IntNum, void* pvDevId)
{
    struct CamOsHListNode_t *n = NULL;
    IntrAttr_t *ptHashListNodeTmp = NULL;
    u8 u8ItemNum = 0;
    u8 u8Del = 0;

    if (u32IntNum >= INT_MAX_NUMBER)
    {
        INTC_ERROR("[%s][%d] Invalid IntNum=%d\r\n", __FUNCTION__, __LINE__, u32IntNum);
        CamOsPanic("[INT_ERR] Invalid interrupt parameters\r\n");
    }

    CamOsSpinLockIrqSave(&m_tIntSpinlock);

    CAM_OS_HASH_FOR_EACH_POSSIBLE_SAFE(gtIntrHashTbl, ptHashListNodeTmp, n, tEntry, u32IntNum)
    {
        if (ptHashListNodeTmp->u32IntNum == u32IntNum)
        {
            u8ItemNum++;
            if (ptHashListNodeTmp->pvDevId == pvDevId)
            {
                u8Del++;
                CAM_OS_HASH_DEL(&ptHashListNodeTmp->tEntry);
                CamOsMemRelease((void *)ptHashListNodeTmp);
                break;
            }
        }
    }

    CamOsSpinUnlockIrqRestore(&m_tIntSpinlock);

    if ((u8ItemNum == 0) || (u8Del == 0)) // IntNum err or pvDevId isn't match
    {
        INTC_ERROR("[%s][%d] Invalid ISR free in IntNum=%d, DevId=0x%08X\r\n", __FUNCTION__, __LINE__, u32IntNum, pvDevId);
        CamOsPanic("[INT_ERR] Invalid interrupt parameters\r\n");
    }

    if (u8ItemNum == 1) // Last ISR was be removed, need mask interrupt
    {
        HalIntMaskInterrupt(u32IntNum);

#ifndef CONFIG_CPU_ARCH_RISCV
#ifdef __ENABLE_DUALOS__
        // Set group to G1/G1NS for non-secure world use.
        #if defined(CONFIG_ARM_GIC_V3_SUPPORT)
        arm_gic_irq_set_group(u32IntNum, GIC_INT_GRP_G1NS);
        #else
        arm_gic_irq_set_group(u32IntNum, GIC_INT_GRP_G1);
        #endif
#endif
#endif
    }
}

void HalIntInitIntController(void)
{
    static bool bInited = 0;

    if (!bInited)
    {
        CamOsSpinInit(&m_tIntSpinlock);
    }
    bInited = 1;

#ifdef CONFIG_CPU_ARCH_RISCV
    riscv_PLIC_disable_interrupt(EN_CCIF);
    riscv_PLIC_disable_interrupt(EN_FIQ);
    riscv_PLIC_disable_interrupt(EN_IRQ);

    /* Set default priority to the lowest, only support 32 levels */
    riscv_PLIC_set_priority(EN_CCIF, EN_PRIORITY4);
    riscv_PLIC_set_priority(EN_FIQ, EN_PRIORITY4);
    riscv_PLIC_set_priority(EN_IRQ, EN_PRIORITY4);

    /* set interrupt priority mask level = 15, if the interrupt priority is larger than this value, it will be sent to CPU*/
    riscv_PLIC_set_threshold(EN_PRIORITY0);

    riscv_PLIC_set_ISR((isr_pointer)HalIntCommonHandler, EN_CCIF);
    riscv_PLIC_set_ISR((isr_pointer)HalIntCommonHandler, EN_FIQ);
    riscv_PLIC_set_ISR((isr_pointer)HalIntCommonHandler, EN_IRQ);
#else
    if (CUR_CORE_ID == CONFIG_BOOT_CORE_ID)
        arm_gic_init(GIC_INIT_TYPE_BOOT_CORE_INIT);
    else
        arm_gic_init(GIC_INIT_TYPE_NON_BOOT_CORE_INIT);
#endif
}

#if defined(CONFIG_STR_SUPPORT) || defined(CONFIG_POWER_MANAG_SUPPORT)
void HalIntResumeIntController(void)
{
    u32 u32IntNum = 0;
    u32 u32PrevIntNum = INT_MAX_NUMBER;
    IntrAttr_t *ptHashListNodeTmp = NULL;
    IntMaskStatus_e eTmpMaskStatus = INT_MASK_STATUS_MASKED;
    int bkt = 0;
    const u32 u32CurCoreID = CUR_CORE_ID;

#ifndef CONFIG_CPU_ARCH_RISCV
    if (u32CurCoreID == CONFIG_BOOT_CORE_ID)
        arm_gic_init(GIC_INIT_TYPE_STR_RESUME_BOOT_CORE_REINIT);
    else
        arm_gic_init(GIC_INIT_TYPE_STR_RESUME_NON_BOOT_CORE_REINIT);
#endif

    // Restore the previous interrupt settings
    CamOsSpinLockIrqSave(&m_tIntSpinlock);

    CAM_OS_HASH_FOR_EACH(gtIntrHashTbl, bkt, ptHashListNodeTmp, tEntry)
    {
        u32IntNum = ptHashListNodeTmp->u32IntNum;

        if (ptHashListNodeTmp->u32Affinity & (1 << u32CurCoreID))
        {
            if ((u32IntNum >= 0) && (u32IntNum != u32PrevIntNum)) // EROY CHECK : Why check (u32IntNum != u32PrevIntNum)?
            {
                if (u32IntNum >= INT_MAX_NUMBER)
                {
                    INTC_ERROR("[%s][%d] Invalid IntNum=%d\r\n", __FUNCTION__, __LINE__, u32IntNum);
                    CamOsPanic("[INT_ERR] Invalid interrupt parameters-1\r\n");
                }
                else if (ptHashListNodeTmp->ePriority >= INTC_PRIORITY_MAX)
                {
                    INTC_ERROR("[%s][%d] Invalid Priority=%d, IntNum=%d\r\n", __FUNCTION__, __LINE__, ptHashListNodeTmp->ePriority, u32IntNum);
                    CamOsPanic("[INT_ERR] Invalid interrupt parameters-2\r\n");
                }

                eTmpMaskStatus = ptHashListNodeTmp->eMaskStatus;

                // Mask interrupt
                _HalIntMaskInterrupt(u32IntNum, TRUE);

                // Configure the interrupt controler
                _HalIntcSetPriority(u32IntNum, ptHashListNodeTmp->ePriority);
                _HalIntcSetGroup(u32IntNum);
                _HalIntcSetTargetCores(u32IntNum, ptHashListNodeTmp->u32Affinity);

                // Unmask interrupt if necessary
                if (eTmpMaskStatus == INT_MASK_STATUS_UNMASK)
                {
                    //ForceUartSendTrace(KERN_EMERG "Resume core[%d] interrupt[%d]\r\n", u32CurCoreID, u32IntNum);
                    _HalIntUnmaskInterrupt(u32IntNum, TRUE);
                }
                u32PrevIntNum = u32IntNum;
            }
        }
    }

    CamOsSpinUnlockIrqRestore(&m_tIntSpinlock);
}
#endif

s32 HalIntInitInterrupt(IntInitParam_u *puParam, IntNumber_e eIntNum)
{
    struct CamOsHListNode_t *n = NULL;
    IntcParam_t *ptIntcParam = &(puParam->intc);
    IntrAttr_t *ptHashListNode = NULL;
    IntrAttr_t *ptHashListNodeTmp = NULL;
    const u32 u32CurCoreID = CUR_CORE_ID;

    // Check parameters
    if (eIntNum >= INT_MAX_NUMBER)
    {
        INTC_ERROR("[%s][%d] Invalid IntNum=%d\r\n", __FUNCTION__, __LINE__, eIntNum);
        CamOsPanic("[INT_ERR] Invalid interrupt parameters-1\r\n");
    }
    else if (ptIntcParam == NULL)
    {
        INTC_ERROR("[%s][%d] Invalid ptIntcParam=NULL, IntNum=%d\r\n", __FUNCTION__, __LINE__, eIntNum);
        CamOsPanic("[INT_ERR] Invalid interrupt parameters-2\r\n");
    }
    else if (ptIntcParam->ePriority >= INTC_PRIORITY_MAX)
    {
        INTC_ERROR("[%s][%d] Invalid Priority=%d, IntNum=%d\r\n", __FUNCTION__, __LINE__, ptIntcParam->ePriority, eIntNum);
        CamOsPanic("[INT_ERR] Invalid interrupt parameters-3\r\n");
    }
    else if (ptIntcParam->pfnIsr == NULL)
    {
        INTC_ERROR("[%s][%d] Invalid pfnIsr=NULL, IntNum=%d\r\n", __FUNCTION__, __LINE__, eIntNum);
        CamOsPanic("[INT_ERR] Invalid interrupt parameters-4\r\n");
    }

    // Fill the control table
    ptHashListNode = (IntrAttr_t *)CamOsMemCalloc(1, sizeof(IntrAttr_t));
    if (ptHashListNode == NULL)
    {
        CamOsPanic("[INT_ERR] CamOsMemCalloc error\r\n");
    }

    ptHashListNode->u16Flag = 0x0000;

    if (ptIntcParam->pDevId == NULL)
    {
        ptHashListNode->u16Flag |= INTC_FLAG_NOT_SHARE;  // It means that multi ISR callback isn't supported
    }
    ptHashListNode->pfnIsr = ptIntcParam->pfnIsr;
    ptHashListNode->pvDevId = ptIntcParam->pDevId;
    ptHashListNode->u16Flag |= INTC_FLAG_POST_CLEAR;
    ptHashListNode->ePriority = ptIntcParam->ePriority;
    #if defined(CONFIG_STR_SUPPORT) || defined(CONFIG_POWER_MANAG_SUPPORT)
    ptHashListNode->eMaskStatus = INT_MASK_STATUS_MASKED;
    #endif
    ptHashListNode->u32IntNum = eIntNum;

    if (eIntNum >= 0 && eIntNum < (GIC_SGI_NR + GIC_PPI_NR))
    {
        // SGI and PPI, register for each core
        ptHashListNode->u32Affinity = (1 << u32CurCoreID);
    }
    else if (eIntNum < INT_MAX_NUMBER)
    {
        // SPI, register once
        if (ptIntcParam->u32Affinity != 0)
        {
            ptHashListNode->u32Affinity = ptIntcParam->u32Affinity;
        }
        else
        {
            ptHashListNode->u32Affinity = (1 << u32CurCoreID);
        }

        #if defined(CONFIG_SMP_SUPPORT) && (CONFIG_CORE_NUM > 1)
        #if (INTC_LOADING_BALANCE_EN)
        ptHashListNode->u32Affinity = INT_NO_AFFINITY;
        #else // Let boot core handle most of SPI interrupts.
        ptHashListNode->u32Affinity = (1 << CONFIG_BOOT_CORE_ID);
        #endif
        #endif
    }

    #if defined(CONFIG_CPU_HOTPLUG_SUPPORT)
    ptHashListNode->u32BackUpAffinity = ptHashListNode->u32Affinity;
    #endif

    if (ptIntcParam->pcName)
    {
        memcpy(ptHashListNode->pcName, ptIntcParam->pcName, INT_MAX_NAME_LEN);
    }

    CamOsSpinLockIrqSave(&m_tIntSpinlock);

    // Check exist node in list
    CAM_OS_HASH_FOR_EACH_POSSIBLE_SAFE(gtIntrHashTbl, ptHashListNodeTmp, n, tEntry, eIntNum)
    {
        if (ptHashListNodeTmp->u32Affinity & (1 << u32CurCoreID))
        {
            if (ptHashListNodeTmp->u32IntNum == eIntNum)
            {
                if (ptHashListNodeTmp->u16Flag & INTC_FLAG_NOT_SHARE)
                {
                    INTC_ERROR("[%s][%d] Interrupt can't share, Flag=0x%x, IntNum=%d\r\n", __FUNCTION__, __LINE__, ptHashListNodeTmp->u16Flag, eIntNum);
                    CamOsPanic("[INT_ERR] Invalid interrupt parameters-5\r\n");
                }

                if ((ptHashListNodeTmp->pfnIsr == ptIntcParam->pfnIsr) &&
                    (ptHashListNodeTmp->pvDevId == ptIntcParam->pDevId))
                {
                    INTC_ERROR("[%s][%d] ISR and DevId are repeated, IntNum=%d\r\n", __FUNCTION__, __LINE__, eIntNum);
                    CamOsPanic("[INT_ERR] Invalid interrupt parameters-6\r\n");
                }

                // Writeback u16Flag and ePriority attribute from exist node
                ptHashListNode->u16Flag = ptHashListNodeTmp->u16Flag;
                ptHashListNode->ePriority = ptHashListNodeTmp->ePriority;
            }
        }
    }
    CAM_OS_HASH_ADD(gtIntrHashTbl, &ptHashListNode->tEntry, ptHashListNode->u32IntNum);

    CamOsSpinUnlockIrqRestore(&m_tIntSpinlock);

    // Mask interrupt
    HalIntMaskInterrupt(eIntNum);

    // Configure the interrupt controler
    _HalIntcSetPriority(eIntNum, ptHashListNode->ePriority);
    _HalIntcSetGroup(eIntNum);
    _HalIntcSetTargetCores(eIntNum, ptHashListNode->u32Affinity);

    return 0;
}

void HalIntMaskInterrupt(IntNumber_e eIntNum)
{
    return _HalIntMaskInterrupt(eIntNum, FALSE);
}

void HalIntUnmaskInterrupt(IntNumber_e eIntNum)
{
    return _HalIntUnmaskInterrupt(eIntNum, FALSE);
}

bool HalIntIsMaskInterrupt(IntNumber_e eIntNum)
{
    bool        bIsMask = FALSE;
    MsIntMask_e eIntMask = 0;
    u32         reg_value = 0;
    u32         mask = 0;
    s16         ms_irq = 0;
    s16         ms_fiq = 0;
    u8          u8IntCtrler = INT_CONTROLER_1;

    u8IntCtrler = u8IntCtrler;

    if (eIntNum >= INT_MAX_NUMBER)
    {
        INTC_ERROR("[%s][%d] Invalid eIntNum=%d\r\n", __FUNCTION__, __LINE__, eIntNum);
        return FALSE;
    }

    eIntMask = MsDisableInterrupt();

#ifndef CONFIG_CPU_ARCH_RISCV
    // Check GIC first
    reg_value = (bool)arm_gic_irq_is_enabled(eIntNum);

    if (reg_value == 0)
        bIsMask = TRUE;
#endif

    ms_irq = eIntNum - GIC_SPI_MS_IRQ_START;
    ms_fiq = eIntNum - GIC_SPI_MS_FIQ_START;

    // Check MS interrupt controller
    if (eIntNum > GIC_SPI_MS_IRQ_START && bIsMask == FALSE)
    {
        if (ms_fiq >= 0 && ms_fiq < GIC_SPI_MS_FIQ_NR)
        {
#ifdef CONFIG_CPU_ARCH_RISCV
            if(!riscv_PLIC_is_interrupt_enabled(EN_FIQ))
                bIsMask = TRUE;
#endif
            INTC_GET_MS_INT_CTRL_ID(ms_fiq, u8IntCtrler);

            if (ms_fiq >= MAX_INT_NUM_PER_HOST_CTLR)
                ms_fiq -= MAX_INT_NUM_PER_HOST_CTLR;

            mask = 1 << (ms_fiq % 16);

            if ((ms_fiq/16) == 0)
                INTC_READ_FIQ_MASK0_REG(u8IntCtrler, reg_value);
            else
                INTC_READ_FIQ_MASK1_REG(u8IntCtrler, reg_value);
        }
        else if (ms_irq >= 0 && ms_irq < GIC_SPI_MS_IRQ_NR)
        {
#ifdef CONFIG_CPU_ARCH_RISCV
            if(!riscv_PLIC_is_interrupt_enabled(EN_IRQ))
                bIsMask = TRUE;
#endif
            INTC_GET_MS_INT_CTRL_ID(ms_irq, u8IntCtrler);

            if (ms_irq >= MAX_INT_NUM_PER_HOST_CTLR)
                ms_irq -= MAX_INT_NUM_PER_HOST_CTLR;

            mask = 1 << (ms_irq % 16);

            if ((ms_irq/16) == 0)
                INTC_READ_IRQ_MASK0_REG(u8IntCtrler, reg_value);
            else if ((ms_irq/16) == 1)
                INTC_READ_IRQ_MASK1_REG(u8IntCtrler, reg_value);
            else if ((ms_irq/16) == 2)
                INTC_READ_IRQ_MASK2_REG(u8IntCtrler, reg_value);
            else
                INTC_READ_IRQ_MASK3_REG(u8IntCtrler, reg_value);
        }

        reg_value &= mask;

        if (reg_value != 0)
            bIsMask = TRUE;
    }

    MsEnableInterrupt(eIntMask);

    return bIsMask;
}

void HalIntClearInterrupt(IntNumber_e eIntNum)
{
    s16 ms_fiq = 0;
    u8  u8IntCtrler = INT_CONTROLER_1;

    u8IntCtrler = u8IntCtrler;

    /* NOTE: only clear if it is MS_FIQ */
    if (eIntNum < GIC_SPI_MS_FIQ_START)
    {
        return;
    }

    ms_fiq = eIntNum - GIC_SPI_MS_FIQ_START;

    if (ms_fiq >= 0 && ms_fiq < GIC_SPI_MS_FIQ_NR)
    {
        INTC_GET_MS_INT_CTRL_ID(ms_fiq, u8IntCtrler);

        if (ms_fiq >= MAX_INT_NUM_PER_HOST_CTLR)
            ms_fiq -= MAX_INT_NUM_PER_HOST_CTLR;

        switch (ms_fiq/16)
        {
        case 0:
            INTC_CLR_INT_FIQ0_REG(u8IntCtrler, 1 << (ms_fiq%16));
            break;
        case 1:
            INTC_CLR_INT_FIQ1_REG(u8IntCtrler, 1 << (ms_fiq%16));
            break;
        case 2:
            INTC_CLR_INT_FIQ2_REG(u8IntCtrler, 1 << (ms_fiq%16));
            break;
        case 3:
            INTC_CLR_INT_FIQ3_REG(u8IntCtrler, 1 << (ms_fiq%16));
            break;
        }
    }
    else
    {
        INTC_DEBUG("[%s][%d] Invalid IntNum=%d\r\n", __FUNCTION__, __LINE__, eIntNum);
    }
}

void HalIntSetInterruptPolarity(IntcPolarity_e ePolarity, IntNumber_e eIntNum)
{
    MsIntMask_e eIntMask = 0;
    s16         ms_irq = 0;
    s16         ms_fiq = 0;
    u32         val = 0;
    u8          u8IntCtrler = INT_CONTROLER_1;

    u8IntCtrler = u8IntCtrler;

    if (eIntNum < GIC_SPI_MS_IRQ_START)
    {
        INTC_ERROR("[%s][%d] Invalid IntNum=%d\r\n", __FUNCTION__, __LINE__, eIntNum);
        return;
    }

    eIntMask = MsDisableInterrupt();

    ms_irq = eIntNum - GIC_SPI_MS_IRQ_START;
    ms_fiq = eIntNum - GIC_SPI_MS_FIQ_START;

    if (ms_fiq >= 0 && ms_fiq < GIC_SPI_MS_FIQ_NR)
    {
        INTC_GET_MS_INT_CTRL_ID(ms_fiq, u8IntCtrler);

        if (ms_fiq >= MAX_INT_NUM_PER_HOST_CTLR)
            ms_fiq -= MAX_INT_NUM_PER_HOST_CTLR;

        if (ePolarity == INTC_POLARITY_LOW)
        {
            if ((ms_fiq/16) == 0)
            {
                INTC_READ_FIQ_POLARITY0_REG(u8IntCtrler, val);
                INTC_WRITE_FIQ_POLARITY0_REG(u8IntCtrler, val | (1 << (ms_fiq%16)) | ((1 << (ms_fiq%16)) << 16));
            }
            else
            {
                INTC_READ_FIQ_POLARITY1_REG(u8IntCtrler, val);
                INTC_WRITE_FIQ_POLARITY1_REG(u8IntCtrler, val | (1 << (ms_fiq%16)) | ((1 << (ms_fiq%16)) << 16));
            }
        }
        else
        {
            if ((ms_fiq/16) == 0)
            {
                INTC_READ_FIQ_POLARITY0_REG(u8IntCtrler, val);
                INTC_WRITE_FIQ_POLARITY0_REG(u8IntCtrler, (val & ~(1 << (ms_fiq%16))) | ((1 << (ms_fiq%16)) << 16));
            }
            else
            {
                INTC_READ_FIQ_POLARITY1_REG(u8IntCtrler, val);
                INTC_WRITE_FIQ_POLARITY1_REG(u8IntCtrler, (val & ~(1 << (ms_fiq%16))) | ((1 << (ms_fiq%16)) << 16));
            }
        }
    }
    else if (ms_irq >= 0 && ms_irq < GIC_SPI_MS_IRQ_NR)
    {
        INTC_GET_MS_INT_CTRL_ID(ms_irq, u8IntCtrler);

        if (ms_irq >= MAX_INT_NUM_PER_HOST_CTLR)
            ms_irq -= MAX_INT_NUM_PER_HOST_CTLR;

        if (ePolarity == INTC_POLARITY_LOW)
        {
            if ((ms_irq/16) == 0)
            {
                INTC_READ_IRQ_POLARITY0_REG(u8IntCtrler, val);
                INTC_WRITE_IRQ_POLARITY0_REG(u8IntCtrler, val | (1 << (ms_irq%16)) | ((1 << (ms_irq%16)) << 16));
            }
            else if ((ms_irq/16) == 1)
            {
                INTC_READ_IRQ_POLARITY1_REG(u8IntCtrler, val);
                INTC_WRITE_IRQ_POLARITY1_REG(u8IntCtrler, val | (1 << (ms_irq%16)) | ((1 << (ms_irq%16)) << 16));
            }
            else if ((ms_irq/16) == 2)
            {
                INTC_READ_IRQ_POLARITY2_REG(u8IntCtrler, val);
                INTC_WRITE_IRQ_POLARITY2_REG(u8IntCtrler, val | (1 << (ms_irq%16)) | ((1 << (ms_irq%16)) << 16));
            }
            else
            {
                INTC_READ_IRQ_POLARITY3_REG(u8IntCtrler, val);
                INTC_WRITE_IRQ_POLARITY3_REG(u8IntCtrler, val | (1 << (ms_irq%16)) | ((1 << (ms_irq%16)) << 16));
            }
        }
        else
        {
            if ((ms_irq/16) == 0)
            {
                INTC_READ_IRQ_POLARITY0_REG(u8IntCtrler, val);
                INTC_WRITE_IRQ_POLARITY0_REG(u8IntCtrler, (val & ~(1 << (ms_irq%16))) | ((1 << (ms_irq%16)) << 16));
            }
            else if ((ms_irq/16) == 1)
            {
                INTC_READ_IRQ_POLARITY1_REG(u8IntCtrler, val);
                INTC_WRITE_IRQ_POLARITY1_REG(u8IntCtrler, (val & ~(1 << (ms_irq%16))) | ((1 << (ms_irq%16)) << 16));
            }
            else if ((ms_irq/16) == 2)
            {
                INTC_READ_IRQ_POLARITY2_REG(u8IntCtrler, val);
                INTC_WRITE_IRQ_POLARITY2_REG(u8IntCtrler, (val & ~(1 << (ms_irq%16))) | ((1 << (ms_irq%16)) << 16));
            }
            else
            {
                INTC_READ_IRQ_POLARITY3_REG(u8IntCtrler, val);
                INTC_WRITE_IRQ_POLARITY3_REG(u8IntCtrler, (val & ~(1 << (ms_irq%16))) | ((1 << (ms_irq%16)) << 16));
            }
        }
    }
    else
    {
        INTC_DEBUG("[%s][%d] Invalid IntNum=%d\r\n", __FUNCTION__, __LINE__, eIntNum);
    }

    MsEnableInterrupt(eIntMask);
}

void HalIntSendSGI(u32 u32HwCoreID, u32 u32IntNum)
{
#ifndef CONFIG_CPU_ARCH_RISCV
    arm_gic_send_sgi(u32HwCoreID, u32IntNum);
#endif
}

u32 HalIntGetCurrentIrqNum(void)
{
    u32 u32IrqID = 0;
    MsIntMask_e eIntMask = MsDisableInterrupt();

    u32IrqID = m_u32CurProcIntId[CUR_CORE_ID];
    MsEnableInterrupt(eIntMask);

    return u32IrqID;
}

#if defined(CONFIG_CPU_HOTPLUG_SUPPORT)
void HalIntCpuHotplug(u32 u32CoreID, u32 u32Online)
{
    IntrAttr_t *ptHashListNodeTmp = NULL;
    int bkt = 0;

    CamOsSpinLockIrqSave(&m_tIntSpinlock);

    CAM_OS_HASH_FOR_EACH(gtIntrHashTbl, bkt, ptHashListNodeTmp, tEntry)
    {
        if (u32Online)
        {
            if (ptHashListNodeTmp->u32IntNum >= GIC_SPI_ARM_INTERNAL_START)
            {
                ptHashListNodeTmp->u32Affinity |= (1 << u32CoreID);
                ptHashListNodeTmp->u32Affinity &= ptHashListNodeTmp->u32BackUpAffinity;
                _HalIntcSetTargetCores(ptHashListNodeTmp->u32IntNum, ptHashListNodeTmp->u32Affinity);
            }
        }
        else
        {
            if (ptHashListNodeTmp->u32Affinity & (1 << u32CoreID))
            {
                if (ptHashListNodeTmp->u32IntNum >= GIC_SPI_ARM_INTERNAL_START)
                {
                    ptHashListNodeTmp->u32Affinity &= ~(1 << u32CoreID);
                    if ((ptHashListNodeTmp->u32Affinity & MsOsGetPossibleCpuMask()) == 0)
                    {
                        ptHashListNodeTmp->u32Affinity |= (1 << CONFIG_BOOT_CORE_ID);
                    }
                    _HalIntcSetTargetCores(ptHashListNodeTmp->u32IntNum, ptHashListNodeTmp->u32Affinity);
                }

                // Reset statistic information
                ptHashListNodeTmp->u32AccuExecCnt[u32CoreID] = 0;
                ptHashListNodeTmp->u64AccuExecTick[u32CoreID] = 0;
                ptHashListNodeTmp->u64MaxExecTick[u32CoreID] = 0;
            }
        }
    }

    CamOsSpinUnlockIrqRestore(&m_tIntSpinlock);
}
#endif

void HalIntShowInterruptStat(void)
{
    IntrAttr_t *ptHashListNodeTmp = NULL;
    u64 u64TotalIsrCnt[CONFIG_CORE_NUM] = {0};
    u32 u32PrevIntNum[CONFIG_CORE_NUM] = {0};
    int bkt = 0;
    int i = 0;

    CamOsSpinLockIrqSave(&m_tIntSpinlock);

    for (i = 0; i < CONFIG_CORE_NUM; i++)
    {
        if (!MsIsCoreReady(i))
        {
            continue;
        }

        u64TotalIsrCnt[i] = 0;
        u32PrevIntNum[i] = 0xFFFFFFFF;

        UartSendTrace("CORE [%d]\r\n", i);
        UartSendTrace("------------------------------------------------------------------------------------------------\r\n");
        UartSendTrace("  INT       Count     MaxTimeUs    AvgTimeUs    Handler       DevId         Affinity    Name\n");
        UartSendTrace("------------------------------------------------------------------------------------------------\r\n");

        CAM_OS_HASH_FOR_EACH(gtIntrHashTbl, bkt, ptHashListNodeTmp, tEntry)
        {
            if ((ptHashListNodeTmp->u32Affinity & (1 << i)) &&
                (ptHashListNodeTmp->u32IntNum >= 0) &&
                (ptHashListNodeTmp->pfnIsr))
            {
                UartSendTrace("  %3d  %10d    %10lld   %10lld    0x%08X    0x%08X    0x%08X  %s\r\n",
                                ptHashListNodeTmp->u32IntNum,
                                ptHashListNodeTmp->u32AccuExecCnt[i],
                                (ptHashListNodeTmp->u64MaxExecTick[i] * 1000000 / arch_timer_get_cntfrq()),
                                #if 0
                                ((ptHashListNodeTmp->u64AccuExecTick[i] * 1000000) / (ptHashListNodeTmp->u32AccuExecCnt[i] * arch_timer_get_cntfrq())),
                                #else // TBD
                                ((ptHashListNodeTmp->u64AccuExecTick[i]) / (ptHashListNodeTmp->u32AccuExecCnt[i] * 6)),
                                #endif
                                ptHashListNodeTmp->pfnIsr,
                                ptHashListNodeTmp->pvDevId,
                                ptHashListNodeTmp->u32Affinity,
                                ptHashListNodeTmp->pcName);
            }

            // EROY TBD, another core did not disable interrupt, the u32AccuExecCnt is still increased during the calculation.
            if (ptHashListNodeTmp->u32IntNum != u32PrevIntNum[i])
            {
                u64TotalIsrCnt[i] += ptHashListNodeTmp->u32AccuExecCnt[i];
                u32PrevIntNum[i] = ptHashListNodeTmp->u32IntNum;
            }
        }

        //UartSendTrace("Total  Core[%d][%10lld]\r\n", i, u64TotalIsrCnt[i]);
        UartSendTrace("------------------------------------------------------------------------------------------------\r\n\r\n");
    }

    CamOsSpinUnlockIrqRestore(&m_tIntSpinlock);

#ifdef __ENABLE_DUALOS__
    {
        extern volatile u64 gu64SmcRecvCnt;
        UartSendTrace("SMC    %10lld\n", gu64SmcRecvCnt);
    }
#endif
}

#ifdef CONFIG_CPU_ARCH_RISCV
/** HalIntcIsPending
* @brief        Check an interrupt is pending on INTC or not
*
* @param[in]    nIntcNumber         Interrupt number.
*
* @retval       bool                TRUE: interrup is pending , FALSE: interrup not pending.
*
*/
bool HalIntcIsPending(u32 nIntcNumber)
{
    u32 reg_value = 0;
    u32 mask;
    s16 ms_irq;
    s16 ms_fiq;
    u8  u8IntCtrler = INT_CONTROLER_1;

    ms_irq = nIntcNumber - GIC_HWIRQ_MS_START;
    ms_fiq = nIntcNumber - GIC_HWIRQ_MS_START - GIC_SPI_MS_IRQ_NR;

    if (nIntcNumber > GIC_HWIRQ_MS_START)
    {
        if (ms_fiq >= 0 && ms_fiq < GIC_SPI_MS_FIQ_NR )
        {
            INTC_GET_MS_INT_CTRL_ID(ms_fiq, u8IntCtrler);

            if (ms_fiq >= MAX_INT_NUM_PER_HOST_CTLR)
                ms_fiq -= MAX_INT_NUM_PER_HOST_CTLR;

            mask = 1 << (ms_fiq%16);
            if((ms_fiq/16) == 0)
                INTC_READ_FIQ_STATUS0_REG(u8IntCtrler, reg_value);
            else
                INTC_READ_FIQ_STATUS1_REG(u8IntCtrler, reg_value);
        }
        else if( ms_irq >=0 && ms_irq < GIC_SPI_MS_IRQ_NR )
        {
            INTC_GET_MS_INT_CTRL_ID(ms_irq, u8IntCtrler);

            if (ms_irq >= MAX_INT_NUM_PER_HOST_CTLR)
                ms_irq -= MAX_INT_NUM_PER_HOST_CTLR;

            mask = 1 << (ms_irq%16);
            if((ms_irq/16) == 0)
                INTC_READ_IRQ_STATUS0_REG(u8IntCtrler, reg_value);
            else if((ms_irq/16) == 1)
                INTC_READ_IRQ_STATUS1_REG(u8IntCtrler, reg_value);
            else if((ms_irq/16) == 2)
                INTC_READ_IRQ_STATUS2_REG(u8IntCtrler, reg_value);
            else
                INTC_READ_IRQ_STATUS3_REG(u8IntCtrler, reg_value);
        }
        reg_value &= mask;
    }

    return reg_value ? 1 : 0;
}

/** HalIntcGetPending
* @brief        Get fist pending interrupt number for specified type
*
* @retval       u32                pending interrupt number
*
*/
u32 HalIntcGetPending(IntcMap_e eMap)
{
    u16 u16RegValue = 0;
    u32 i;
    u32 u32Base;
    u32 u32Pending = 0xffffffff;
    u8  u8IntCtrler = INT_CONTROLER_1;
#if (CONFIG_MS_INT_CTRL_CNT == 2)
    u32 u32Loop = 2;
#else
    u32 u32Loop = 1;
#endif

    switch (eMap)
    {
    case INTC_MAP_FIQ:
    {
        u32Base = 0;
        INTC_READ_FIQ_STATUS0_REG(u8IntCtrler, u16RegValue);
        for (i = 0; i < 16; i++)
        {
            if (u16RegValue & (1 << i))
                break;
        }

        if (i < 16)
        {
            u32Pending = i + u32Base + GIC_SPI_MS_FIQ_START;
            break;
        }

        u32Base += 16;
        INTC_READ_FIQ_STATUS1_REG(u8IntCtrler, u16RegValue);
        for (i = 0; i < 16; i++)
        {
            if (u16RegValue & (1 << i))
                break;
        }

        if (i < 16)
        {
             u32Pending = i + u32Base + GIC_SPI_MS_FIQ_START;
             break;
        }
        break;
    }
    case INTC_MAP_IRQ:
    {
        u32Base = 0;
        do
        {
            INTC_READ_IRQ_STATUS0_REG(u8IntCtrler, u16RegValue);
            for (i = 0; i < 16; i++)
            {
                if (u16RegValue & (1 << i))
                    break;
            }

            if (i < 16)
            {
                u32Pending = i + u32Base + GIC_SPI_MS_IRQ_START;
                break;
            }

            u32Base += 16;
            INTC_READ_IRQ_STATUS1_REG(u8IntCtrler, u16RegValue);
            for (i = 0; i < 16; i++)
            {
                if (u16RegValue & (1 << i))
                    break;
            }

            if (i < 16)
            {
                u32Pending = i + u32Base + GIC_SPI_MS_IRQ_START;
                break;
            }

            u32Base += 16;
            INTC_READ_IRQ_STATUS2_REG(u8IntCtrler, u16RegValue);
            for (i = 0; i < 16; i++)
            {
                if (u16RegValue & (1 << i))
                    break;
            }

            if (i < 16)
            {
                u32Pending = i + u32Base + GIC_SPI_MS_IRQ_START;
                break;
            }

            u32Base += 16;
            INTC_READ_IRQ_STATUS3_REG(u8IntCtrler, u16RegValue);
            for (i = 0; i < 16; i++)
            {
                if (u16RegValue & (1 << i))
                    break;
            }

            if (i < 16)
            {
                u32Pending = i + u32Base + GIC_SPI_MS_IRQ_START;
                break;
            }

            u32Base += 16;
            u8IntCtrler = INT_CONTROLER_2;
            u32Loop = u32Loop - 1;
        } while (u32Loop);
        break;
    }
    default:
    {
        break;
    }
    }

    return u32Pending;
}
#endif

void HalIntCommonHandler(IntcMap_e eMap)
{
    struct CamOsHListNode_t *n = NULL;
    IntrAttr_t *ptHashListNodeTmp = NULL;
    u8 u8RepeatCnt = 0;
    u32 u32PendingInt = 0;
    u64 u64ExecTick = 0;
    u16 u16TempFlag = 0;
    const u32 u32CurCoreID = CUR_CORE_ID;

#if defined(RTOS_KERNEL_USE_RTK)
    RtkRegionCntInc();
#endif

    do
    {
        switch (eMap)
        {
#ifdef CONFIG_CPU_ARCH_RISCV
        case INTC_MAP_IRQ:
            u32PendingInt = HalIntcGetPending(eMap);
            if (u32PendingInt >= GIC_SPI_MS_IRQ_END)
            {
                //FIXME: disable this warning as a workaround for unknown reason which trigger
                //       interrupt faster than bit setting in riu register.
                //INTC_ERROR("[%s][%d] No pending IRQ interrupt [%d][%d]\r\n", __FUNCTION__, __LINE__, u32PendingInt, eMap);
                return;
            }
            break;
        case INTC_MAP_FIQ:
            u32PendingInt = HalIntcGetPending(eMap);
            if (u32PendingInt >= GIC_SPI_MS_FIQ_END)
            {
                INTC_ERROR("[%s][%d] No pending FIQ interrupt [%d][%d]\r\n", __FUNCTION__, __LINE__, u32PendingInt, eMap);
                return;
            }
            break;
#elif defined(CONFIG_ARM_GIC_V3_SUPPORT)
        #if defined(CONFIG_INT_GRP_G0_FIQ)
        case INTC_MAP_FIQ:
            u32PendingInt = arm_gic_get_active_by_group(0); // G0 FIQ from mon_mode and secure world
            break;
        #elif defined(CONFIG_INT_GRP_G1S)
        case INTC_MAP_FIQ:
            u32PendingInt = arm_gic_get_active_by_group(1); // G1S FIQ from mon_mode
            break;
        case INTC_MAP_IRQ:
            u32PendingInt = arm_gic_get_active_by_group(1); // G1S IRQ from secure world
            break;
        #elif defined(CONFIG_INT_GRP_G1NS)
        case INTC_MAP_FIQ:
            u32PendingInt = arm_gic_get_active_by_group(1); // G1NS FIQ from mon_mode
            break;
        case INTC_MAP_IRQ:
            u32PendingInt = arm_gic_get_active_by_group(1); // G1NS IRQ from non-secure world
            break;
        #endif
#else
        #if defined(CONFIG_INT_GRP_G0_FIQ)
        case INTC_MAP_FIQ:
            u32PendingInt = arm_gic_get_active();
            break;
        #elif defined(CONFIG_INT_GRP_G1_IRQ)
        case INTC_MAP_IRQ:
            u32PendingInt = arm_gic_get_active();
            break;
        #endif
#endif
        default:
            INTC_ERROR("[%s][%d] Wrong interrupt type [%d][%d]\r\n", __FUNCTION__, __LINE__, u32PendingInt, eMap);
            CamOsPanic("[INT_ERR] Receive wrong interrupt type");
            break;
        }

        /* Check for pending interrupt */
        if (u32PendingInt >= INT_MAX_NUMBER)
        {
            //GICC_WRITEL(GICC_AEOIR, u32PendingInt);
            break;
        }

        m_u32CurProcIntId[u32CurCoreID] = u32PendingInt;

        #ifdef __ENABLE_DUALOS__
        RTKINFO_FIQCNT(++_fiqcnt);
        #endif

        m_s32IsrNestedLevel[u32CurCoreID]++;

#ifndef CONFIG_CPU_ARCH_RISCV
        arm_gic_irq_disable(u32PendingInt); // Enable irq in later HalIntUnmaskInterrupt()
#endif

        u8RepeatCnt = 0;

        m_u32InISRCtxt[u32CurCoreID] = 1;

        CamOsSpinLockIrqSave(&m_tIntSpinlock);

        #if defined(CONFIG_SYS_SWLA_SUPPORT)
        sys_swla_log_add_irq(u32PendingInt, SS_SWLA_LOG_START);
        #endif

        CAM_OS_HASH_FOR_EACH_POSSIBLE_SAFE(gtIntrHashTbl, ptHashListNodeTmp, n, tEntry, u32PendingInt)
        {
            if (ptHashListNodeTmp->u32Affinity & (1 << u32CurCoreID))
            {
                if (ptHashListNodeTmp->u32IntNum == u32PendingInt)
                {
                    u16TempFlag = ptHashListNodeTmp->u16Flag;

                    if (u8RepeatCnt == 0)
                    {
                        _HalIntMaskInterrupt(u32PendingInt, TRUE);
                        if (u16TempFlag & INTC_FLAG_PRE_CLEAR)
                        {
                            HalIntClearInterrupt(u32PendingInt);
                        }
                    }

                    ptHashListNodeTmp->u32AccuExecCnt[u32CurCoreID]++;

                    if (ptHashListNodeTmp->pfnIsr)
                    {
                        u64ExecTick = arch_timer_get_counter();
                        ptHashListNodeTmp->pfnIsr(ptHashListNodeTmp->u32IntNum, ptHashListNodeTmp->pvDevId);
                        u64ExecTick = arch_timer_get_counter() - u64ExecTick;

                        ptHashListNodeTmp->u64AccuExecTick[u32CurCoreID] += u64ExecTick;
                        if (u64ExecTick > ptHashListNodeTmp->u64MaxExecTick[u32CurCoreID])
                        {
                            ptHashListNodeTmp->u64MaxExecTick[u32CurCoreID] = u64ExecTick;
                        }
                    }

                    u8RepeatCnt++;
                }
            }
        }

        #if defined(CONFIG_SYS_SWLA_SUPPORT)
        sys_swla_log_add_irq(u32PendingInt, SS_SWLA_LOG_STOP);
        #endif

        CamOsSpinUnlockIrqRestore(&m_tIntSpinlock);

        m_u32InISRCtxt[u32CurCoreID] = 0;

        if (u8RepeatCnt > 0)
        {
            if (u16TempFlag & INTC_FLAG_POST_CLEAR)
            {
                HalIntClearInterrupt(u32PendingInt);
            }
            _HalIntUnmaskInterrupt(u32PendingInt, FALSE);
        }
        else
        {
            INTC_ERROR("[%s][%d] Wrong interrupt [%d] Map [%d] in core[%d]\r\n", __FUNCTION__, __LINE__, u32PendingInt, eMap, u32CurCoreID);
            CamOsPanic("[INT_ERR] Interrupt handler can't find out the interrupt number in list");
        }

        // Check ISR depth
        if (m_s32IsrNestedLevel[u32CurCoreID] >= INTC_NEST_IRQ_LIMIT)
        {
            INTC_ERROR("ISR too deep\r\n");
        }
        m_s32IsrNestedLevel[u32CurCoreID]--;

#ifndef CONFIG_CPU_ARCH_RISCV
        switch (eMap)
        {
#if defined(CONFIG_ARM_GIC_V3_SUPPORT)
        #if defined(CONFIG_INT_GRP_G0_FIQ)
        case INTC_MAP_FIQ:
            arm_gic_set_eoi_by_group(0, u32PendingInt);
            break;
        #elif defined(CONFIG_INT_GRP_G1S)
        case INTC_MAP_FIQ:
        case INTC_MAP_IRQ:
            arm_gic_set_eoi_by_group(1, u32PendingInt);
            break;
        #elif defined(CONFIG_INT_GRP_G1NS)
        case INTC_MAP_FIQ:
        case INTC_MAP_IRQ:
            arm_gic_set_eoi_by_group(1, u32PendingInt);
            break;
        #endif
#else
        case INTC_MAP_FIQ:
        case INTC_MAP_IRQ:
            arm_gic_set_eoi(u32PendingInt);
            break;
#endif
        default:
            break;
        }
#endif

#ifdef CONFIG_CPU_ARCH_RISCV
    } while (0);
#else
    } while (1);
#endif

#if defined(RTOS_KERNEL_USE_RTK)
    RtkRegionCntDec();
#endif

#if defined(CONFIG_CPU_HOTPLUG_SUPPORT)
    if (MsGetHotplugCoreStage(CUR_CORE_ID) & MS_HOTPLUG_CORE_DOWN_INT_START)
    {
        HalIntCpuHotplug(CUR_CORE_ID, MS_HOTPLUG_CORE_OFFLINE);
        MsClearHotplugCoreStage(CUR_CORE_ID, MS_HOTPLUG_CORE_DOWN_INT_START);
    }
    else if (MsGetHotplugCoreStage(CUR_CORE_ID) & MS_HOTPLUG_CORE_UP_INT_START)
    {
        HalIntCpuHotplug(CUR_CORE_ID, MS_HOTPLUG_CORE_ONLINE);
        MsClearHotplugCoreStage(CUR_CORE_ID, MS_HOTPLUG_CORE_UP_INT_START);
    }
#endif

#if defined(CONFIG_STACK_DIAGONSE)
    {
        static u32 m_u32IntStackTopPtr[CONFIG_CORE_NUM] = {0};

        // Check stack overflow
        if (m_u32IntStackTopPtr[CUR_CORE_ID] == 0)
        {
            SysDbgSP2CallStackId((u32)__builtin_frame_address(0), &m_u32IntStackTopPtr[CUR_CORE_ID], NULL);
        }
        else
        {
            if (*(u32 *)(m_u32IntStackTopPtr[CUR_CORE_ID]) != ARM_MODE_STACK_GUARD_PATTERN)
            {
                CamOsPanic("Interrupt Stack Overflow!");
            }
        }
    }
#endif
}
