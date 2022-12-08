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


#ifndef _RISCV_INTC_H
#define _RISCV_INTC_H

typedef int (*isr_pointer)(int);

// msip interrupt api
int riscv_msip_enable(char enable);
int riscv_msip_trig( );
int riscv_msip_clear( );
int riscv_msip_ISR( );
int riscv_msip_set_ISR(isr_pointer fun_pointer);

// PLIC interrupt api
typedef enum{
    EN_CCIF = 1,
    EN_FIQ  = 2,
    EN_IRQ  = 3,
    EN_INT4 = 4,
    EN_INT5 = 5,
    EN_INT6 = 6,
    EN_INT7 = 7,
    EN_INT8 = 8,
    EN_INT9 = 9,
    EN_INT10 = 10,
    EN_INT11 = 11,
    EN_INT12 = 12,
    EN_INT13 = 13,
    EN_INT14 = 14,
    EN_INT15 = 15,
    EN_INT_NUM = 16,
}EN_PLIC_TYPE;

typedef enum{
    EN_PRIORITY0 = 0,
    EN_PRIORITY1,
    EN_PRIORITY2,
    EN_PRIORITY3,
    EN_PRIORITY4,
    EN_PRIORITY5,
    EN_PRIORITY6,
    EN_PRIORITY7,
    EN_PRIORITY_MAX,
}EN_PLIC_PRIORITY;

int riscv_PLIC_init (char enable);
int riscv_PLIC_is_interrupt_enabled (EN_PLIC_TYPE interrupt);
int riscv_PLIC_enable_interrupt (EN_PLIC_TYPE interrupt);
int riscv_PLIC_disable_interrupt (EN_PLIC_TYPE interrupt);
int riscv_PLIC_set_threshold (EN_PLIC_PRIORITY threshold);
int riscv_PLIC_set_priority (EN_PLIC_TYPE interrupt, EN_PLIC_PRIORITY priority);
int riscv_PLIC_claim_interrupt(void);
int riscv_PLIC_complete_interrupt(EN_PLIC_TYPE interrupt);
int riscv_PLIC_ISR(void);
int riscv_PLIC_set_ISR(isr_pointer fun_pointer, EN_PLIC_TYPE interrupt);
int riscv_PLIC_enable_interrupt_nesting(char enable);

// mtimer interrupt api
int riscv_mtimer_enable(char enable);
int riscv_mtimer_set_cmp_counter(uint64_t counter);
int riscv_mtimer_set_run_counter(uint64_t counter);
uint64_t riscv_mtimer_get_cmp_counter();
int riscv_mtimer_ISR(void);
int riscv_mtimer_set_ISR(isr_pointer fun_pointer);
int riscv_mtimer_start(void);
int riscv_mtimer_stop(void);
uint64_t riscv_mtimer_get_run_counter(void);

int riscv_mcycle_start(void);
int riscv_mcycle_stop(void);
int riscv_mcycle_set_run_counter(uint64_t counter);
uint64_t riscv_mcycle_get_run_counter(void);
#endif
