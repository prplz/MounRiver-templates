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

#ifndef __SYS_RTK_VMRTKHO_H__
#define __SYS_RTK_VMRTKHO_H__

#include "drv_timer_api.h"

/****************************************************************************/
/*  OS constants                                                            */
/****************************************************************************/

/* Return values */
#define VM_RTK_OK                               10

/****************************************************************************/
/*  OS macro                                                                */
/****************************************************************************/

#if defined(__RTK_USER_TICK_4DOT6MS__)
#define HAL_RTK_TICK_TIME           5
#define HAL_RTK_TICK_TIME_REAL      4.638
#define HAL_RTK_MS_TO_TICK(MsT)     ((u32)((MsT) >= HAL_RTK_TICK_TIME ? (((MsT)* 221) / 1024) : 1))
#define HAL_RTK_MS_TO_TICK_64(MsT)  ((u64)((MsT) >= HAL_RTK_TICK_TIME ? div64_ul(((MsT)* 221), 1024) : 1))
#define HAL_RTK_S_TO_TICK(SecT)     ((u32)(((SecT)* 55189) >> 8))
#define HAL_RTK_MN_TO_TICK(MnT)     ((u32)(((MnT)* 206959) >> 4))
#define HAL_RTK_TICK_TO_MS(T)       ((u32)(((u64)(T)* 303955) >> 16))
#define HAL_RTK_TICK_TO_S(T)        ((u32)(((u64)(T)* 19) >> 12))
#define HAL_RTK_MAXTIMER_VALUE      0x3FFFFFFF
#elif defined(__RTK_USER_TICK_15MS__)
#define HAL_RTK_TICK_TIME           15
#define HAL_RTK_TICK_TIME_REAL      (15.0)
#define HAL_RTK_MS_TO_TICK(MsT)     ((u32)((MsT) >= HAL_RTK_TICK_TIME) ? (((MsT)+(HAL_RTK_TICK_TIME-1) )/HAL_RTK_TICK_TIME) : 1)
#define HAL_RTK_S_TO_TICK(SecT)     HAL_RTK_MS_TO_TICK((SecT*60))
#define HAL_RTK_MN_TO_TICK(MnT)     HAL_RTK_MS_TO_TICK((MnT*3600))
#define HAL_RTK_TICK_TO_S(T)        ((u32)((T)* HAL_RTK_TICK_TIME)/1000)
#define HAL_RTK_TICK_TO_MS(T)       ((u32)((T)* HAL_RTK_TICK_TIME))
#define HAL_RTK_MAXTIMER_VALUE      -1L
#else
#define HAL_RTK_TICK_TIME           1
#define HAL_RTK_TICK_TIME_REAL      (1)
#define HAL_RTK_MS_TO_TICK(MsT)     ((u32)MsT)
#define HAL_RTK_MS_TO_TICK_64(MsT)  ((u64)MsT)
#define HAL_RTK_S_TO_TICK(SecT)     HAL_RTK_MS_TO_TICK((SecT*1000))
#define HAL_RTK_MN_TO_TICK(MnT)     HAL_RTK_MS_TO_TICK((MnT*60000))
#define HAL_RTK_TICK_TO_S(T)        ((u32)((T)* HAL_RTK_TICK_TIME)/1000)
#define HAL_RTK_TICK_TO_MS(T)       ((u32)((T)* HAL_RTK_TICK_TIME))
#define HAL_RTK_MAXTIMER_VALUE      -1L
#endif

#define VM_RTK_TICK_TIME            HAL_RTK_TICK_TIME
#define VM_RTK_TICK_TIME_REAL       HAL_RTK_TICK_TIME_REAL
#define VM_RTK_MS_TO_TICK(MsT)      HAL_RTK_MS_TO_TICK(MsT)
#define VM_RTK_MS_TO_TICK_64(MsT)   HAL_RTK_MS_TO_TICK_64(MsT)
#define VM_RTK_S_TO_TICK(SecT)      HAL_RTK_S_TO_TICK(SecT)
#define VM_RTK_MN_TO_TICK(MnT)      HAL_RTK_MN_TO_TICK(MnT)
#define VM_RTK_TICK_TO_MS(T)        HAL_RTK_TICK_TO_MS(T)
#define VM_RTK_TICK_TO_S(T)         HAL_RTK_TICK_TO_S(T)
#define VM_RTK_MAXTIMER_VALUE       HAL_RTK_MAXTIMER_VALUE

#endif  /* __SYS_RTK_VMRTKHO_H__ */
