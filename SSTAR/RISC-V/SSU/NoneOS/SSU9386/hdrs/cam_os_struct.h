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


#ifndef __CAM_OS_STRUCT_H__
#define __CAM_OS_STRUCT_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "cam_os_util.h"


#define CAM_OS_MUTEX_SIZE       (0)
#define CAM_OS_TSEM_SIZE        (0)
#define CAM_OS_RWTSEM_SIZE      (0)
#define CAM_OS_SPINLOCK_SIZE    (0)
#define CAM_OS_TIMER_SIZE       (0)
#define CAM_OS_MEMCACHE_SIZE    CAM_OS_ALIGN_UP(sizeof(u8) + sizeof(u32), 4) / sizeof(u32)
#define CAM_OS_IDR_SIZE         (0)
#define CAM_OS_CPUMASK_SIZE     (0)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__CAM_OS_STRUCT_H__
