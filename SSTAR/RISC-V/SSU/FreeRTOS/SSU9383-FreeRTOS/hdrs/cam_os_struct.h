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

#if defined(__KERNEL__)
#define CAM_OS_LINUX_KERNEL
#endif

#ifdef CAM_OS_RTK
#include "sys_MsWrapper_cus_os_util.h"
#include "sys_MsWrapper_cus_os_sem.h"
#include "sys_MsWrapper_cus_os_timer.h"

#define CAM_OS_MUTEX_SIZE       CAM_OS_ALIGN_UP(sizeof(u32) + sizeof(Ms_Mutex_t), 4) / sizeof(u32)
#define CAM_OS_TSEM_SIZE        CAM_OS_ALIGN_UP(sizeof(u32) + sizeof(Ms_DynSemaphor_t), 4) / sizeof(u32)
#define CAM_OS_RWTSEM_SIZE      CAM_OS_ALIGN_UP(sizeof(u32) + sizeof(u32) + sizeof(Ms_Mutex_t) + sizeof(Ms_DynSemaphor_t), 4) / sizeof(u32)
#define CAM_OS_SPINLOCK_SIZE    CAM_OS_ALIGN_UP(sizeof(Ms_Spinlock_t), 4) / sizeof(u32)
#define CAM_OS_TIMER_SIZE       CAM_OS_ALIGN_UP(sizeof(size_t*), 4) / sizeof(u32)
#define CAM_OS_MEMCACHE_SIZE    CAM_OS_ALIGN_UP(sizeof(u8) + sizeof(u32), 4) / sizeof(u32)
#define CAM_OS_IDR_SIZE         CAM_OS_ALIGN_UP(sizeof(void**) + sizeof(u32*) + sizeof(u32), 4) / sizeof(u32)
#define CAM_OS_CPUMASK_SIZE     CAM_OS_ALIGN_UP(sizeof(u32), 4) / sizeof(u32)

#elif defined(CAM_OS_LINUX_KERNEL)

#include <linux/mutex.h>
#include <linux/semaphore.h>
#include <linux/rwsem.h>
#include <linux/completion.h>
#include <linux/spinlock_types.h>
#include <linux/timer.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>
#include <linux/slab_def.h>
#include <linux/idr.h>
#include <linux/version.h>

#define CAM_OS_MUTEX_SIZE       CAM_OS_ALIGN_UP(sizeof(u32) + sizeof(struct mutex), 4) / sizeof(u32)
#define CAM_OS_TSEM_SIZE        CAM_OS_ALIGN_UP(sizeof(u32) + sizeof(struct semaphore), 4) / sizeof(u32)
#define CAM_OS_RWTSEM_SIZE      CAM_OS_ALIGN_UP(sizeof(u32) + sizeof(struct rw_semaphore), 4) / sizeof(u32)
#define CAM_OS_SPINLOCK_SIZE    CAM_OS_ALIGN_UP(sizeof(u32) + sizeof(spinlock_t) + sizeof(unsigned long), 4) / sizeof(u32)
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
#define CAM_OS_TIMER_SIZE       CAM_OS_ALIGN_UP(sizeof(struct timer_list), 4) / sizeof(u32)
#else
#define CAM_OS_TIMER_SIZE       CAM_OS_ALIGN_UP(sizeof(struct timer_list) + sizeof(void*) + sizeof(void*), 4) / sizeof(u32)
#endif
#define CAM_OS_MEMCACHE_SIZE    CAM_OS_ALIGN_UP(sizeof(struct kmem_cache*) + 16, 4) / sizeof(u32)
#define CAM_OS_IDR_SIZE         CAM_OS_ALIGN_UP(sizeof(struct idr), 4) / sizeof(u32)
#define CAM_OS_CPUMASK_SIZE     CAM_OS_ALIGN_UP(sizeof(struct cpumask), 4) / sizeof(u32)

#else

#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define CAM_OS_MUTEX_SIZE       CAM_OS_ALIGN_UP(sizeof(u32) + sizeof(pthread_mutex_t), 4) / sizeof(u32)
#define CAM_OS_TSEM_SIZE        CAM_OS_ALIGN_UP(sizeof(u32) + sizeof(sem_t), 4) / sizeof(u32)
#define CAM_OS_RWTSEM_SIZE      CAM_OS_ALIGN_UP(sizeof(u32) + sizeof(pthread_rwlock_t), 4) / sizeof(u32)
#define CAM_OS_SPINLOCK_SIZE    CAM_OS_ALIGN_UP(sizeof(u32) + sizeof(pthread_spinlock_t), 4) / sizeof(u32)
#define CAM_OS_TIMER_SIZE       CAM_OS_ALIGN_UP(sizeof(timer_t), 4) / sizeof(u32)
#define CAM_OS_MEMCACHE_SIZE    0
#define CAM_OS_IDR_SIZE         CAM_OS_ALIGN_UP(sizeof(void **) + sizeof(unsigned long *) + sizeof(unsigned long), 4) / sizeof(u32)
#define CAM_OS_CPUMASK_SIZE     0

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__CAM_OS_STRUCT_H__
