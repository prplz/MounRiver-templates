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

#ifndef __SYS_RISCV_ARCH_TIMER_H__
#define __SYS_RISCV_ARCH_TIMER_H__

#include "vm_types.ht"

///////////////////////////////////////////////////

#define ARCH_TIMER_CTRL_ENABLE      (1 << 0)
#define ARCH_TIMER_CTRL_IT_MASK     (1 << 1)
#define ARCH_TIMER_CTRL_IT_STAT     (1 << 2)

///////////////////////////////////////////////////

typedef void (*PfnArchTimerIntcISR)(void);

///////////////////////////////////////////////////

static inline void _arch_timer_reg_write_ctrl(u32 val)
{
    //isb();
}

static inline u32 _arch_timer_reg_read_ctrl(void)
{
    u32 val = 0;

    return val;
}

static inline void arch_timer_write_compare_val(u64 u64Val)
{
    //isb();
}

static inline u64 arch_timer_read_compare_val(void)
{
    u64 u64Val = 0;

    return u64Val;
}

u64 arch_timer_get_counter(void);
u32 arch_timer_get_cntfrq(void);


static inline u32 arch_timer_get_counter_32bit(void)
{
    return (u32)arch_timer_get_counter();
}

static inline void arch_timer_enable(void)
{
    _arch_timer_reg_write_ctrl(_arch_timer_reg_read_ctrl() | ARCH_TIMER_CTRL_ENABLE);
}

static inline void arch_timer_disable(void)
{
    u32 ctrl = 0;

    ctrl = _arch_timer_reg_read_ctrl();
    ctrl &= ~(ARCH_TIMER_CTRL_ENABLE);
    _arch_timer_reg_write_ctrl(ctrl);
}

static inline void arch_timer_enable_int(void)
{
    u32 ctrl = 0;

    ctrl = _arch_timer_reg_read_ctrl();
    ctrl &= ~(ARCH_TIMER_CTRL_IT_MASK);
    _arch_timer_reg_write_ctrl(ctrl);
}

static inline void arch_timer_disable_int(void)
{
    _arch_timer_reg_write_ctrl(_arch_timer_reg_read_ctrl() | ARCH_TIMER_CTRL_IT_MASK);
}

static inline u32 arch_timer_get_int_flag(void)
{
    u32 ctrl = 0;

    ctrl = _arch_timer_reg_read_ctrl();
    return (ctrl & ARCH_TIMER_CTRL_IT_STAT) ? 1 : 0;
}

static inline void arch_timer_clear_int_flag(void)
{
    // The CNTP_CTL.ISTATUS bit is read only
}

///////////////////////////////////////////////////

void arch_timer_register_isr(PfnArchTimerIntcISR pfnTimerISR);
void arch_timer_release_isr(void);

u32 arch_timer_get_max_cnt_32bit(void);
u32 arch_timer_get_timeout_length(void);
u32 arch_timer_tick_to_ms(u32 tick);
u32 arch_timer_ms_to_tick(u32 ms);

#endif // __SYS_RISCV_ARCH_TIMER_H__
