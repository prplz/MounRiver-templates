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

/**
 *
 * @file    cus_os.h
 *
 * @brief   This file defines the OS semaphore interface for customer layer
 *          where the implementation might different between platforms.
 *          The OS related interfaces includes memory allocation, task inter-
 *          communication.
 *          ** Note **
 *          The file is new created, new add related functions should be
 *          added here!!
 *
 * @author  Xavier Fan
 * @version $Id: cus_os.h 15678 2008-10-29 06:59:18Z xavier.fan $
 *
 */

#ifndef __SYS_MSWRAPPER_CUS_OS_SEM_H__
#define __SYS_MSWRAPPER_CUS_OS_SEM_H__

/*--------------------------------------------------------------------------*/
/* INCLUDE FILE                                                             */
/*--------------------------------------------------------------------------*/

#include "sys_MsWrapper_cus_os_type.h"
#include "sys_MsWrapper_cus_os_util.h"
#include "sys_MsWrapper_cus_os_atomic.h"
#if defined(RTOS_KERNEL_USE_RTK)
#include "sys_rtk_semaphores.h"
#elif defined(RTOS_KERNEL_USE_FREERTOS)
#include "FreeRTOS.h"
#include "semphr.h"
#endif

/*--------------------------------------------------------------------------*/
/* PREPROCESSOR CONSTANT DEFINITION                                         */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* PREPROCESSOR MACRO DEFINITION                                            */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* TYPE DEFINITION                                                          */
/*--------------------------------------------------------------------------*/

#if defined(RTOS_KERNEL_USE_RTK)
typedef rtk_Mutex_t         Ms_Mutex_t;
typedef rtk_Dyn_Semaphor_t  Ms_DynSemaphor_t;
#elif defined(RTOS_KERNEL_USE_FREERTOS)
typedef struct
{
    SemaphoreHandle_t   handle;
#if (configSUPPORT_STATIC_ALLOCATION == 1)
    StaticSemaphore_t   buf;
#endif
} Ms_Mutex_t;

typedef struct
{
    MsAtomic_t  wait_cnt;
    SemaphoreHandle_t   handle;
#if (configSUPPORT_STATIC_ALLOCATION == 1)
    StaticSemaphore_t   buf;
#endif
} Ms_DynSemaphor_t;
#endif

/*--------------------------------------------------------------------------*/
/* GLOBAL VARIABLE DEFINITION                                               */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* GLOBAL FUNCTION PROTOTYPE                                                */
/*--------------------------------------------------------------------------*/
/** \addtogroup SenMan
 *  @{
 */

/**
 * \fn Ms_ErrCode_e MsCreateDynSem (Ms_DynSemaphor_t *PtrSem, u32 rtkCounterInitialValue)
 * \brief The MsCreateDynSem primitive creates a dynamic semaphore. After initialization, the initial valuie and current velue set to rtkCounterInitialValue. The scheduler is not activated
 * \param[in] PtrSem The dynamic semaphore handle
 * \param[in] rtkCounterInitialValue Initial number and current number of tokens owned by the semaphore
 * \return Return True if the dynamic semaphore is created successfully
 */
Ms_ErrCode_e MsCreateDynSem (Ms_DynSemaphor_t *PtrSem, u32 rtkCounterInitialValue);

/**
 * \fn Ms_ErrCode_e MsCreateDynSemExtend (Ms_DynSemaphor_t *PtrSem, u32 rtkCounterInitialValue, u32 rtkCounterCurrentValue)
 * \brief The MsCreateDynSem primitive creates a dynamic semaphore. After initialization, the initial valuie and current velue set to rtkCounterInitialValue. The scheduler is not activated
 * \param[in] PtrSem The dynamic semaphore handle
 * \param[in] rtkCounterInitialValue Initial number of tokens owned by the semaphore
 * \param[in] rtkCounterCurrentValue Current number of tokens owned by the semaphore
 * \return Return True if the dynamic semaphore is created successfully
 */
Ms_ErrCode_e MsCreateDynSemExtend(Ms_DynSemaphor_t *PtrSem, u32 rtkCounterInitialValue, u32 rtkCounterCurrentValue);

/**
 * \fn void* MsDestroyDynSem (Ms_DynSemaphor_t *PtrSem)
 * \brief The MsDestroyDynSem primitive destroys a dynamic semaphore.
 * \param[in] PtrSem The dynamic semaphore handle that will be destroyed
 */
void MsDestroyDynSem (Ms_DynSemaphor_t *PtrSem);

/**
 * \fn Ms_ErrCode_e MsProduceDynSem(Ms_DynSemaphor_t *PtrSem)
 * \brief The MsProduceDynSem primitive adds a new token to the dynamic semaphore.If there are one or more tasks waiting on this semaphore, the highest priority task is de-queued and the state is changed to Ready. If there are no tasks waiting on this semaphore, the semaphore value is incremented by one unit and the calling task is re-activated
 * \param[in] PtrSem The dynamic semaphore handle
 * \return Return RTK_OK if produce semaphore token successfully.
 */
Ms_ErrCode_e MsProduceDynSem(Ms_DynSemaphor_t *PtrSem);

/**
 * \fn Ms_ErrCode_e MsProduceSafeDynSem(Ms_DynSemaphor_t *PtrSem, bool bProduceAll)
 * \brief The MsProduceDynSem primitive add tokens to the dynamic semaphore(not over init value).If there are one or more tasks waiting on this semaphore, all tasks are de-queued and the state is changed to Ready. If there are no tasks waiting on this semaphore, the semaphore value is set to init value unit and the calling task is re-activated
 * \param[in] PtrSem The dynamic semaphore handle
 * \param[in] bProduceAll TRUE mead produce to the init value
 * \return Return RTK_OK if produce semaphore token successfully.
 */
Ms_ErrCode_e MsProduceSafeDynSem(Ms_DynSemaphor_t *PtrSem, bool bProduceAll);

/**
 * \fn Ms_ErrCode_e MsConsumeDynSem(Ms_DynSemaphor_t *PtrSem)
 * \brief The MsConsumeDynSem primitive is used by a task to wait for a dynamic semaphore to be in the signaled state. If the semaphore referenced by a dynamic semaphore contains at least one token, the token counter is decremented and control is returned to the calling task. If the semaphore referenced by dynamic semaphore contains no tokens, the calling task is queued to the semaphore��s task queue
 * \param[in] PtrSem The dynamic semaphore handle
 * \return Return RTK_OK if concume dynamic semaphore token successfully.
 */
Ms_ErrCode_e MsConsumeDynSem(Ms_DynSemaphor_t *PtrSem);

/**
 * \fn Ms_ErrCode_e MsPollDynSem(Ms_DynSemaphor_t *PtrSem)
 * \brief The MsPollDynSem primitive is used by a task to test if a semaphore is in the signaled state. If the semaphore referenced by semID contains at least one token, the token counter is decremented and the primitive returns RTK_OK. If the semaphore referenced by semID contains no tokens, the primitive immediately returns RTK_UNIT_NOAVAIL (the token counter is not decremented). As the scheduler is never activated, this primitive can be called from a critical section
 * \param[in] PtrSem The dynamic semaphore handle
 * \return Return RTK_OK if concume SEM token successfully; otherwise, return RTK_UNIT_NOAVAIL
 */
Ms_ErrCode_e MsPollDynSem(Ms_DynSemaphor_t *PtrSem);

/**
 * \fn Ms_ErrCode_e MsConsumeAllDynSem(Ms_DynSemaphor_t *PtrSem)
 * \brief The MsConsumeDynSem primitive is used by a task to wait for a dynamic semaphore to be in the signaled state. The calling task consumes all tokens and is queued to the semaphore��s task queue
 * \param[in] PtrSem The dynamic semaphore handle
 * \return Return RTK_OK if concume dynamic semaphore token successfully.
 */
Ms_ErrCode_e MsConsumeAllDynSem(Ms_DynSemaphor_t *PtrSem);

/**
 * \fn Ms_ErrCode_e MsConsumeDynSemDelay(Ms_DynSemaphor_t *PtrSem, u32 timeout_ms)
 * \brief The MsConsumeDynSemDelay primitive is almost the same as MsConsumeDynSem primitive except the task at most wait timeout_ms time in millisecond, if task waits more than timeout_ms time, then task will be re-activated. For the task, there is no difference between timer expiration and a semaphore production
 * \param[in] PtrSem The dynamic semaphore handle
 * \param[in] timeout_ms The waiting time(millisecond) for a task to wait the semaphore
 * \return CUS_OS_NO_MESSAGE if timeout; otherwise, retuen RTK_OK.
 */
Ms_ErrCode_e MsConsumeDynSemDelay(Ms_DynSemaphor_t *PtrSem, u32 timeout_ms);

/**
 * \fn Ms_ErrCode_e MsConsumeAllDynSemDelay(Ms_DynSemaphor_t *PtrSem, u32 timeout_ms)
 * \brief The MsConsumeDynSemDelay primitive is almost the same as MsConsumeAllDynSem primitive except the task at most wait timeout_ms time in millisecond, if task waits more than timeout_ms time, then task will be re-activated. For the task, there is no difference between timer expiration and a semaphore production
 * \param[in] PtrSem The dynamic semaphore handle
 * \param[in] timeout_ms The waiting time(millisecond) for a task to wait the semaphore
 * \return CUS_OS_NO_MESSAGE if timeout; otherwise, retuen RTK_OK.
 */
Ms_ErrCode_e MsConsumeAllDynSemDelay(Ms_DynSemaphor_t *PtrSem, u32 timeout_ms);

/**
 * \fn Ms_ErrCode_e MsGetDynSemCount(Ms_DynSemaphor_t *PtrSem, s16* pSemCount)
 * \brief The MsGetDynSemCount primitive returns the current number of available tokerns in the semaphore
 * \param[in] PtrSem The dynamic semaphore handle
 * \param[out] pSemCount The current number of available tokerns in the semaphore
 * \return Always RTK_OK
 */
Ms_ErrCode_e MsGetDynSemCount(Ms_DynSemaphor_t *PtrSem, s16* pSemCount);

bool MsSemQueryService(void);

/**
 * \fn u8 MsInitMutex (Ms_Mutex_t* mutex)
 * \brief It initializes the given mutex control block
 * \param[in] mutex control block ptr
 * \return Always RTK_OK
 */
u8 MsInitMutex (Ms_Mutex_t* mutex);

/**
 * \fn u8 MsDeInitMutex (Ms_Mutex_t* mutex)
 * \brief It initializes the given mutex control block
 * \param[in] mutex control block ptr
 * \return Always RTK_OK
 */
u8 MsDeInitMutex(Ms_Mutex_t* mutex);

/**
 * \fn Ms_ErrCode_e MsMutexLock(Ms_Mutex_t* mutex)
 * \brief It locks the given mutex control block
 * \param[in] mutex control block ptr
 * \return Error code Ms_ErrCode_e
 */
Ms_ErrCode_e MsMutexLock(Ms_Mutex_t* mutex);
/**
 * \fn Ms_ErrCode_e MsMutexUnlock(Ms_Mutex_t* mutex)
 * \brief It unlocks the given mutex control block
 * \param[in] mutex control block ptr
 * \return Error code Ms_ErrCode_e
 */
Ms_ErrCode_e MsMutexUnlock(Ms_Mutex_t* mutex);
/**
 * \fn Ms_ErrCode_e MsMutexTryLock(Ms_Mutex_t* mutex)
 * \brief If mutex available, lock it, or return immediately
 * \param[in] mutex control block ptr
 * \return Error code Ms_ErrCode_e
 */
Ms_ErrCode_e MsMutexTryLock(Ms_Mutex_t* mutex);

bool MsMutexQueryService(void);

/** @}*/
#endif
