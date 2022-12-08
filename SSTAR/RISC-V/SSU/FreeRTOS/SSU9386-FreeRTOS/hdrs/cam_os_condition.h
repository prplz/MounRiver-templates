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


#ifndef __CAM_OS_CONDITION_H__
#define __CAM_OS_CONDITION_H__

#include "cam_os_wrapper.h"

#if defined(__KERNEL__)
#define CAM_OS_LINUX_KERNEL
#endif

#if defined(CAM_OS_RTK)
#include "sys_rtk_ho.h"
#include "sys_MsWrapper_cus_os_sem.h"
#include "sys_MsWrapper_cus_os_util.h"

typedef struct
{
    struct CamOsListHead_t tListHead;
    Ms_Spinlock_t          flags;
} CamOsCondition_t;

typedef struct
{
    struct CamOsListHead_t tList;
    Ms_DynSemaphor_t       tDynSem;
} CamOsConditionSem_t;
#elif defined(CAM_OS_LINUX_KERNEL)
#include "linux/wait.h"
typedef wait_queue_head_t   CamOsCondition_t;
#else   // CAM_OS_LINUX_USER
#include "pthread.h"
typedef struct
{
    pthread_mutex_t tMutex;
    pthread_cond_t  tCondition;
} CamOsCondition_t;
#endif

#if defined(CAM_OS_RTK)
#define CamOsConditionInit(ptCondition)                                     \
({                                                                          \
    MsInitSpinlock(&(ptCondition)->flags);                                  \
    CAM_OS_INIT_LIST_HEAD(&(ptCondition)->tListHead);                       \
})
#elif defined(CAM_OS_LINUX_KERNEL)
#define CamOsConditionInit(ptCondition)                                     \
    init_waitqueue_head(ptCondition);
#else   // CAM_OS_LINUX_USER
#define CamOsConditionInit(ptCondition)                                     \
({                                                                          \
    pthread_condattr_t cattr;                                               \
    pthread_condattr_init(&cattr);                                          \
    pthread_condattr_setclock(&cattr, CLOCK_MONOTONIC);                     \
    pthread_cond_init(ptCondition.tCondition, &cattr);                      \
    pthread_mutex_init(ptCondition.tMutex, NULL);                           \
})
#endif

#if defined(CAM_OS_RTK)
#define CamOsConditionDeinit(ptCondition)                                   \
({                                                                          \
    (void)ptCondition;                                                      \
})
#elif defined(CAM_OS_LINUX_KERNEL)
#define CamOsConditionDeinit(ptCondition)                                   \
({                                                                          \
    (void)ptCondition;                                                      \
})
#else   // CAM_OS_LINUX_USER
#define CamOsConditionDeinit(ptCondition)                                   \
({                                                                          \
    pthread_cond_destroy(ptCondition.tCondition);                           \
    pthread_mutex_destroy(ptCondition.tMutex);                              \
})
#endif

#if defined(CAM_OS_RTK)
#define CamOsConditionWakeUpAll(ptCondition)                                \
({                                                                          \
    CamOsConditionSem_t *ptSem;                                             \
    MsSpinLockIrqSave(&(ptCondition)->flags);                               \
    CAM_OS_LIST_FOR_EACH_ENTRY(ptSem, &(ptCondition)->tListHead, tList)     \
    {                                                                       \
        MsProduceDynSem(&(ptSem->tDynSem));                                 \
    }                                                                       \
    MsSpinUnlockIrqRestore(&(ptCondition)->flags);                          \
})
#elif defined(CAM_OS_LINUX_KERNEL)
#define CamOsConditionWakeUpAll(ptCondition)                                \
    wake_up_all(ptCondition);
#else   // CAM_OS_LINUX_USER
#define CamOsConditionWakeUpAll(ptCondition)                                \
({                                                                          \
    pthread_mutex_lock(ptCondition.tMutex);                                 \
    pthread_cond_broadcast(ptCondition.tCondition);                         \
    pthread_mutex_unlock(ptCondition.tMutex);                               \
})
#endif

#if defined(CAM_OS_RTK)
#define CamOsConditionWait(ptCondition, condition)                          \
({                                                                          \
    CamOsRet_e __eRet = CAM_OS_OK;                                          \
    CamOsConditionSem_t tWaitSem;                                           \
    CAM_OS_INIT_LIST_HEAD(&tWaitSem.tList);                                 \
    MsCreateDynSemExtend(&tWaitSem.tDynSem, CAM_OS_MAX_INT - 1, 0);         \
    MsSpinLockIrqSave(&(ptCondition)->flags);                               \
    CAM_OS_LIST_ADD_TAIL(&tWaitSem.tList, &(ptCondition)->tListHead);       \
    MsSpinUnlockIrqRestore(&(ptCondition)->flags);                          \
    while (!(condition))                                                    \
        MsConsumeDynSem(&tWaitSem.tDynSem);                                 \
    MsSpinLockIrqSave(&(ptCondition)->flags);                               \
    CAM_OS_LIST_DEL(&tWaitSem.tList);                                       \
    MsDestroyDynSem(&tWaitSem.tDynSem);                                     \
    MsSpinUnlockIrqRestore(&(ptCondition)->flags);                          \
    __eRet;                                                                 \
})
#elif defined(CAM_OS_LINUX_KERNEL)
#define CamOsConditionWait(ptCondition, condition)                          \
({                                                                          \
    CamOsRet_e __eRet = CAM_OS_OK;                                          \
    wait_event((*(ptCondition)), condition);                                \
    __eRet;                                                                 \
})
#else   // CAM_OS_LINUX_USER
#define CamOsConditionWait(ptCondition, condition)                          \
({                                                                          \
    CamOsRet_e __eRet = CAM_OS_OK;                                          \
    pthread_mutex_lock(ptCondition.tMutex);                                 \
    while (!(condition))                                                    \
        pthread_cond_wait(ptCondition.tCondition, ptCondition.tMutex);      \
    pthread_mutex_unlock(ptCondition.tMutex);                               \
    __eRet;                                                                 \
})
#endif

#if defined(CAM_OS_LINUX_KERNEL)
#define CamOsConditionTimedWait(ptCondition, condition, nMsec)              \
({                                                                          \
    CamOsRet_e __eRet = CAM_OS_OK;                                          \
    if (!wait_event_timeout((*(ptCondition)), condition,                    \
                               msecs_to_jiffies(nMsec)))                    \
    {                                                                       \
        __eRet = CAM_OS_TIMEOUT;                                            \
    }                                                                       \
    __eRet;                                                                 \
})
#elif defined(CAM_OS_RTK)
#define __CamOsConditionTimedWait(ptCondition, condition, timeout_ms)       \
({                                                                          \
    unsigned long __ret = timeout_ms;                                       \
    unsigned long long __target_time = CamOsGetTimeInMs() + timeout_ms;     \
    CamOsConditionSem_t tTWaitSem;                                          \
    CAM_OS_INIT_LIST_HEAD(&tTWaitSem.tList);                                \
    MsCreateDynSemExtend(&tTWaitSem.tDynSem, CAM_OS_MAX_INT - 1, 0);        \
    MsSpinLockIrqSave(&(ptCondition)->flags);                               \
    CAM_OS_LIST_ADD_TAIL(&tTWaitSem.tList, &(ptCondition)->tListHead);      \
    MsSpinUnlockIrqRestore(&(ptCondition)->flags);                          \
    while (!(condition))                                                    \
    {                                                                       \
        if (MS_NO_MESSAGE == MsConsumeDynSemDelay(&tTWaitSem.tDynSem, __ret)) {    \
            __ret = (condition);                                            \
            break;                                                          \
        }                                                                   \
        __ret = (unsigned long)(__target_time - CamOsGetTimeInMs());        \
        if (__ret > timeout_ms) {                                           \
            __ret = (condition);                                            \
            break;                                                          \
        }                                                                   \
    }                                                                       \
    MsSpinLockIrqSave(&(ptCondition)->flags);                               \
    CAM_OS_LIST_DEL(&tTWaitSem.tList);                                      \
    MsDestroyDynSem(&tTWaitSem.tDynSem);                                    \
    MsSpinUnlockIrqRestore(&(ptCondition)->flags);                          \
    __ret;                                                                  \
})

#define CamOsConditionTimedWait(ptCondition, condition, nMsec)              \
({                                                                          \
    CamOsRet_e __eRet = CAM_OS_OK;                                          \
    if(!__CamOsConditionTimedWait(ptCondition, condition, (nMsec&0xFFFFFFFF)))  \
    {                                                                       \
        __eRet = CAM_OS_TIMEOUT;                                            \
    }                                                                       \
    __eRet;                                                                 \
})
#else   // CAM_OS_LINUX_USER
#define __CamOsConditionTimedWait(ptCondition, condition, timeout_ms)       \
({                                                                          \
    int __ret = 1;                                                          \
    struct timespec max_wait;                                               \
    s64 nano_sec = 0;                                                       \
    clock_gettime(CLOCK_MONOTONIC, &max_wait);                              \
    nano_sec = (timeout_ms * 1000000LL) + max_wait.tv_nsec;                 \
    max_wait.tv_sec += (nano_sec / 1000000000LL);                           \
    max_wait.tv_nsec = nano_sec % 1000000000LL;                             \
    pthread_mutex_lock(ptCondition.tMutex);                                 \
    while (!(condition)) {                                                  \
        if (0 != pthread_cond_timedwait(ptCondition.tCondition,             \
                                        ptCondition.tMutex, &max_wait)) {   \
            __ret = (condition);                                            \
            break;                                                          \
        }                                                                   \
    }                                                                       \
    pthread_mutex_unlock(ptCondition.tMutex);                               \
    __ret;                                                                  \
})

#define CamOsConditionTimedWait(ptCondition, condition, nMsec)              \
({                                                                          \
    CamOsRet_e __eRet = CAM_OS_OK;                                          \
    if(!__CamOsConditionTimedWait(ptCondition, condition, nMsec))           \
    {                                                                       \
        __eRet = CAM_OS_TIMEOUT;                                            \
    }                                                                       \
    __eRet;                                                                 \
})
#endif

#endif //__CAM_OS_CONDITION_H__
