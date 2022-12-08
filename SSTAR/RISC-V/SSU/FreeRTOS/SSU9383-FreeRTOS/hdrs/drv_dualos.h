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

#ifndef __DRV_DUALOS_H__
#define __DRV_DUALOS_H__

#include "cam_os_wrapper.h"
#include "sys_arch_timer.h"

#define ENABLE_NBLK_CALL    1

#define INTEROS_SC_L2R_HANDSHAKE            (0x800)
#define INTEROS_SC_L2R_RTK_CLI              (0x801)
#define INTEROS_SC_L2R_RTK_LOG              (0x802)
#define INTEROS_SC_L2R_SCR_FW_ENABLE        (0x803)
#define INTEROS_SC_L2R_SCR_FW_DISABLE       (0x804)
#define INTEROS_SC_L2R_RTK_ASSERT           (0x805)
#define INTEROS_SC_L2R_RSQ_INIT             (0x100)
#define INTEROS_SC_L2R_CALL                 (0x808)
#define INTEROS_SC_L2R_ACK_RTK_SIG_BLK      (0x80A)
#define INTEROS_SC_L2R_AMP_RTK_CPU_USAGE    (0x80B)
#define INTEROS_SC_L2R_RPMSG_HANDSHAKE      (0x900)
#define INTEROS_SC_L2R_RPMSG_NOTIFY         (0x901)

#define IPI_NR_RTOS_2_LINUX_CALL_REQ        8
#define IPI_NR_RTOS_2_LINUX_CALL_RESP       9
#define IPI_NR_LINUX_2_RTOS_CALL_REQ        10
#define IPI_NR_LINUX_2_RTOS_CALL_RESP       11
#define IPI_NR_RTOS_2_LINUX_NBLK_CALL_REQ   12
#define IPI_NR_RTOS_2_LINUX_RPMSG           13
#define IPI_NR_REROUTE_SMC                  14
#define IPI_NR_LINUX_2_RTOS_RPMSG           15

#define RTKINFO_FIQCNT(c)                   if (_rtk) _rtk->fiq_cnt++
#define RTKINFO_FFIQTS()                    if (_rtk) _rtk->ffiq_ts = (unsigned int)(arch_timer_get_counter() - epiod);
#define RTKINFO_LOADNSTS()                  if (_rtk) _rtk->ldns_ts = (unsigned int)(arch_timer_get_counter() - epiod);
#define RTKINFO_TTFF_ISP()                  if (_rtk && !_rtk->ttff_isp) _rtk->ttff_isp = (unsigned int)(arch_timer_get_counter() - epiod);
#define RTKINFO_TTFF_SCL()                  if (_rtk && !_rtk->ttff_scl) _rtk->ttff_scl = (unsigned int)(arch_timer_get_counter() - epiod);
#define RTKINFO_TTFF_MFE()                  if (_rtk && !_rtk->ttff_mfe) _rtk->ttff_mfe = (unsigned int)(arch_timer_get_counter() - epiod);

/* back to NS */
#define BACK_TO_NS()                        {asm volatile("smc #0");}

#define SHARE_BASE_SIZE                     (4096)
#define SHARE_LOG_SIZE                      (16 * 1024)
#define MAX_LINELEN                         (256)
#define SHARE_BLK_CALL_SIZE                 (6 * 1024)
#define SHARE_NBLK_CALL_SIZE                (6 * 1024)
#define MAX_CALLLEN                         (96)

/////////////////////////////////////////////////////////////

struct rlink_head {
    u64             next, prev;
    u64             nphys;      // next object physical address
    unsigned int    nsize;      // next object size
    unsigned int    reserved;
};

typedef struct {
    struct rlink_head   root;
    char                name[8];
    char                version[64];
    unsigned int        verid;
    unsigned int        size;
    unsigned int        fiq_cnt;
    unsigned int        ffiq_ts;
    unsigned int        ttff_isp;
    unsigned int        ttff_scl;
    unsigned int        ttff_mfe;
    unsigned int        ldns_ts;
    u64                 start_ts;
    u64                 lifet;
    u64                 spent;
    u64                 spent_hyp;                  // Restore secure/non-secure world ctx time (if CONFIG_ADV_HYP_PROF_SUPPORT enabled)
    u64                 spent_sc;                   // Execute FIQ or RPMSG call function time (if CONFIG_ADV_HYP_PROF_SUPPORT enabled)
    u64                 linux_idle_in_rtos_time;
    u64                 diff;
    unsigned int        linux_idle;
    u64                 syscall_cnt;
    // sbox must be 16-byte aligned
    unsigned char       sbox[1024];
} rtkinfo_t;

/////////////////////////////////////////////////////////////

extern rtkinfo_t    *_rtk;
extern u64          epiod;

/////////////////////////////////////////////////////////////

void DrvDualOsBootNsSuspend(void);
void DrvDualOsBootNsResume(void);
int is_linux_up(void);
int verify_linux_up(void);
int lh_clicmd(char* buf, int len);

unsigned int rsq_log(char *msg);
unsigned int rsq_log_flush_to_uart(void);
int is_rsq_log_buf_inited(void);
unsigned int rsq_nblk_call(u32 arg0, u32 arg1, u32 arg2, u32 arg3);
unsigned int rsq_blk_call(u32 arg0, u32 arg1, u32 arg2, u32 arg3);

s32 interos_sc_reg(u32 id, void *func, const char *name);
s32 interos_sc_dereg(u32 id, void *func);

#endif /* __DRV_DUALOS_H__ */
