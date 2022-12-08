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

#ifndef __KERNEL_SYSTIMER_H__
#define __KERNEL_SYSTIMER_H__

/************************************************/
/* SYS TIMER registers definition               */
/************************************************/

typedef struct KeInfinteTimer_t
{
    /* [0x00] */
  u16 reg_timer_en                  :1;
  u16 reg_timer_trig                :1;
  u16 rsvd1                         :6;
  u16 reg_timer_int_en	            :1;
  u16 rsvd2                         :7;
  u16                               :16;
  /* [0x04] */
  u16 reg_timer_hit                 :1;
  u16 rsvd3                         :15;
  u16                               :16;
  /* [0x08] */
  u16 reg_timer_max_low             :16;
  u16                               :16;
  /* [0x0C] */
  u16 reg_timer_max_high            :16;
  u16                               :16;
  /* [0x10] */
  u16 reg_timer_cap_low             :16;
  u16                               :16;
  /* [0x14] */
  u16 reg_timer_cap_high            :16;
  u16                               :16;
} KeInfinteTimer_t;

extern volatile KeInfinteTimer_t    * const g_ptKeInfinteTimer0;
extern volatile KeInfinteTimer_t    * const g_ptKeInfinteTimer1;
extern volatile KeInfinteTimer_t    * const g_ptKeInfinteTimer2;

#endif /*__KERNEL_SYSTIMER_H__*/
