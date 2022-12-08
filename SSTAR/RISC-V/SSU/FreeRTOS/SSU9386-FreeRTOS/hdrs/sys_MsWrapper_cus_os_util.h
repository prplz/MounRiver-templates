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

#ifndef __SYS_MSWRAPPER_CUS_OS_UTIL_H__
#define __SYS_MSWRAPPER_CUS_OS_UTIL_H__

/*------------------------------------------------------------------------------
    Include Files
-------------------------------------------------------------------------------*/

#include "sys_MsWrapper_cus_os_type.h"
#include "sys_MsWrapper_cus_os_int_pub.h"

#include "drv_timer_api.h"
#include "sys_arch_timer.h"
#ifdef CONFIG_CPU_ARCH_RISCV
#include "sys_riscv_spinlock.h"
#else
#include "sys_arm_spinlock.h"
#endif
#if defined(RTOS_KERNEL_USE_RTK)
#include "sys_rtk_ho.h"
#endif

/*------------------------------------------------------------------------------
    EXTERN definition
-------------------------------------------------------------------------------*/

#define MS_WRAPPER_MIN(a, b)        (((a)<(b)) ? (a) : (b))
#define MS_WRAPPER_MAX(a, b)        (((a)>(b)) ? (a)  : (b))

#if defined(RTOS_KERNEL_USE_RTK)
#define MS_MAX_TASKS                RTK_MAX_TASKS
#else
#define MS_MAX_TASKS                256
#endif

#define CUR_HW_CORE_ID              MsOsGetHwCoreId()
#define CUR_CORE_ID                 MsOsGetCoreId()
#define INVALID_CORE_ID             0xFFFFFFFF

extern const u32 m_u32CpuLogicalMap[];
extern const u32 m_u32CpuHwMap[];

#define HW_TO_LOGICAL_CORE_ID(x)    m_u32CpuLogicalMap[x]
#define LOGICAL_TO_HW_CORE_ID(x)    m_u32CpuHwMap[x]

#if defined(RTOS_KERNEL_USE_RTK)
#define USER_TO_HW_TICK_OSTICK(x)   DrvTimerUserToHwTickOstick(x)
#define HW_TO_USER_TICK_OSTICK(x)   DrvTimerHwToUserTickOstick(x)
#ifdef CONFIG_USE_ARCH_TIMER
#define MS_TO_HW_TICK_OSTICK(x)     arch_timer_ms_to_tick(x)
#define HW_TICK_TO_MS_OSTICK(x)     arch_timer_tick_to_ms(x)
#else
#define MS_TO_HW_TICK_OSTICK(x)     DrvTimerMs2Ostick(x)
#define HW_TICK_TO_MS_OSTICK(x)     DrvTimerOstick2Ms(x)
#endif
#endif // defined(RTOS_KERNEL_USE_RTK)

/*------------------------------------------------------------------------------
    Enumeration
-------------------------------------------------------------------------------*/
/*!
 *      \brief MS error codes. These values are returned by most of the MS functions
 */
typedef enum
{
    MS_OK                           = 10,       ///< No error - the function call was successfull
    MS_NOT_RELEASED                 = 11,       ///< The bloc was not released
    MS_UNIT_NOAVAIL                 = 21,       ///< Not available
    MS_NO_MESSAGE                   = 33,       ///< No message found
    MS_INVALID_MAILBOX_ID           = 34,       ///< invalid mailbox id
    MS_INVALID_MAILBOX_OWNER        = 35,       ///< mailbox is not owned by current task
    MS_INVALID_MAILBOX_THRESHOLD    = 36,       ///< invalid mailbox threshold
    MS_MAILBOX_OVER_THRESHOLD       = 37,       ///< mailbox reach threshold
    MS_NON_SATISFACTORY_POOL        = 40,       ///< No memory pool found (-deprecated-)
    MS_NO_MEMORY_AVAIL              = 41,       ///< Not enough memory
    MS_CLUSTER_OUT_OF_RANGE         = 42,
    MS_TIMER_NOT_FOUND              = 43,       ///< Invalid timer id
    MS_MEM_OVERFLOW                 = 44,
    MS_NULL_RELEASE                 = 45,       ///< Try to release a NULL pointer
    MS_NOT_HEAP_RELEASE             = 46,       ///< Memory zone is not in heap
    MS_NOT_MEMORY_RELEASE           = 47,       ///< Memory zone is not in pool memory (-deprecated-)
    MS_MAILBOX_FULL                 = 48,       ///< Mailbox is full (-deprecated-)
    MS_TIMER_EXHAUSTED              = 49,       ///< Timer is unavailable (-deprecated-)
    MS_MAILBOX_NOT_EXIST            = 50,       ///< Mailbox is in initial state since dynamic task is deleted
    MS_STACK_ADDRESS_ERROR          = (-1),
    MS_ENTRY_POINTER_ERROR          = (-2),
    MS_PRIORITY_ERROR               = (-3),
    MS_TASK_EXHAUSTED               = (-4),
    MS_TASK_ID_ERROR                = (-5),
    MS_TASK_NOT_EXISTED             = (-6),
    MS_TASK_ALREADY_CREATED         = (-7),
    MS_COARSE_TIMER_ID_ERROR        = (-9),
    MS_COARSE_TIMER_START_FAILED    = (-10),
} Ms_ErrCode_e;

typedef enum
{
    MS_CORE_READY = 0x1,
    MS_CORE_SUSPEND = 0x2,
    MS_CORE_OFFLINE = 0x4,
    MS_CORE_INVALID = 0xFF
} Ms_CoreStatus_e;

#if defined(CONFIG_CPU_HOTPLUG_SUPPORT)
typedef enum
{
    MS_HOTPLUG_CORE_STATE_NONE      = 0x0,
    MS_HOTPLUG_CORE_DOWN_INT_START  = (1 << 0),
    MS_HOTPLUG_CORE_DOWN_TASK_START = (1 << 1),
    MS_HOTPLUG_CORE_DOWN_MMU_START  = (1 << 2),
    MS_HOTPLUG_CORE_DOWN_END        = (1 << 3),
    MS_HOTPLUG_CORE_UP_INT_START    = (1 << 8),
    MS_HOTPLUG_CORE_UP_TASK_START   = (1 << 9),
    MS_HOTPLUG_CORE_UP_END          = (1 << 10)
} Ms_HotplugCoreStatus_e;

typedef enum
{
    MS_HOTPLUG_CORE_OFFLINE = 0x0,
    MS_HOTPLUG_CORE_ONLINE
} Ms_HotplugCoreAction_e;
#endif

/*------------------------------------------------------------------------------
    Data Type
-------------------------------------------------------------------------------*/

typedef void* MsThread;

typedef struct
{
    u32 nPriority;      /* From 1(lowest) to 99(highest), use OS default priority if set 0 */
    u32 nStackSize;     /* If nStackSize is zero, use OS default value */
    char *szName;
    struct {
        u32 nRuntime;
        u32 nDeadline;
    } Sched;
} MsThreadAttrb_t, *pMsThreadAttrb;

typedef struct
{
    void *top;
    void *bottom;
    void *sp;
} MsContextStackInfo_t;

typedef struct
{
    u32         u32Flag;
    spinlock_t  tLock;
} Ms_Spinlock_t;

/*------------------------------------------------------------------------------
    Global Function
-------------------------------------------------------------------------------*/

/** \addtogroup TaskMan1
 *  @{
 */

MsOsErr_e MsScheduleTask(u32 timeout_ms);

MsOsErr_e MsWakeUpTask(MsThread tThread);

/**
 * \fn u32 MsCurrTask(void)
 * \brief The MsCurrTask primitive returns the task id of current task
 * \return The ID of current task
 */
u32 MsCurrTask(void);

/**
 * \fn MsTaskId_e MsGetTaskId(MsThread tThread)
 * \brief The MsGetTaskId primitive returns the task id of a task handle
 * \param[in] tThread The Task handle
 * \return The ID of current task
 */
u32 MsGetTaskId(MsThread tThread);

/**
 * \fn void* MsGetTaskHandleByName(const char * name)
 * \brief The MsGetTaskHandleByName primitive returns the task handle by the specified task name
 * \param[in] name The task name
 * \return The task handle of the specified task name
 */
void* MsGetTaskHandleByName(const char * name);

/**
 * \fn Ms_ErrCode_e MsChangeTaskPriority(MsThread tThread, u32 nPriority)
 * \brief The MsChangeTaskPriority primitive is used to run-time change the priority of the given task.
 * When it is called, besides the priority of the desired task control block changes, the service will lock the system and rechain the priority waiting list for semaphore and flag. The priority change takes immediate effect upon API return.
 * The service will trigger rescheduling, only when the given new priority is higher than old one and the state of that task is RTK_READY or RTK_SUSPENDED
 * \param[in] tThread The Task handle
 * \param[in] nPriority The new task priority
 * \return MS_OK is returned if the action is done successfully; otherwise, kernel will assert
 */
Ms_ErrCode_e MsChangeTaskPriority(MsThread tThread, u32 nPriority);

/**
 * \fn Ms_ErrCode_e MsChangeTaskPriorityExtra(MsThread tThread, u32 nPriority)
 * \brief The MsChangeTaskPriorityExtra primitive is used to run-time change the priority of the given task.
 * When it is called, besides the priority of the desired task control block changes, the service will lock the system and rechain the priority waiting list for semaphore and flag. The priority change takes immediate effect upon API return.
 * The service will trigger rescheduling, only when the given new priority is higher than old one and the state of that task is RTK_READY or RTK_SUSPENDED
 * The nPriority will be set directly.
 * \param[in] tThread The Task handle
 * \param[in] nPriority The new task priority
 * \return MS_OK is returned if the action is done successfully; otherwise, kernel will assert
 */
Ms_ErrCode_e MsChangeTaskPriorityExtra(MsThread tThread, u32 nPriority);

/**
 * \fn Ms_ErrCode_e MsSetTaskName(MsThread tThread, const char *szTaskName)
 * \brief The MsSetTaskName primitive is used to run-time set the name of the given task.
 * \param[in] tThread The Task handle
 * \param[in] szTaskName The new task name
 * \return MS_OK is returned if the action is done successfully; otherwise, kernel will assert
 */
Ms_ErrCode_e MsSetTaskName(MsThread tThread, const char *szTaskName);

/**
 * \fn Ms_ErrCode_e MsGetTaskName(MsThread tThread, char *szTaskName, u32 nMaxLen)
 * \brief The MsGetTaskName primitive is used to get the name of the given task.
 * \param[in] tThread The Task handle
 * \param[out] szTaskName Used to return the task name
 * \param[in] nMaxLen Specifies the number of bytes available in szTaskName
 * \return MS_OK is returned if the action is done successfully; otherwise, kernel will assert
 */
Ms_ErrCode_e MsGetTaskName(MsThread tThread, char *szTaskName, u32 nMaxLen);

void MsSetTaskReadyToExit(MsThread tThread);
u8 MsIsCurTaskReadyToExit(void);

/**
 * \fn Ms_ErrCode_e MsCreateTask(MsThread *ptThread,
                                 MsThreadAttrb_t *ptAttrb,
                                 void *(*pfnStartRoutine)(void *),
                                 void *pArg);
 * \brief The MsCreateTask primitive is used for creating a dynamic task. Our system use mailbox as default inter-process communication and the corresponding mailbox ID of the created task is the same as the task ID
 * \param[out] ptThread: A successful call to CamOsThreadCreate() stores the handle
 *                       of the new thread.
 * \param[in] ptAttrb: Argument points to a CamOsThreadAttrb_t structure whose
 *                     contents are used at thread creation time to determine
 *                     thread priority, stack size and thread name. Thread
 *                     priority range from 1(lowest) to 99(highest), use OS
 *                     default priority if set 0.
 * \param[in] pfnStartRoutine(): The new thread starts execution by invoking it.
 * \param[in] pArg: It is passed as the sole argument of pfnStartRoutine().
 * \return MS_OK is returned if task is created successfully; otherwise, returns Ms_ErrCode_e
 */
Ms_ErrCode_e MsCreateTask(MsThread *ptThread,
                          MsThreadAttrb_t *ptAttrb,
                          void *(*pfnStartRoutine)(void *),
                          void *pArg);

/**
 * \fn Ms_ErrCode_e MsDeleteTask(MsThread tThread)
 * \brief The MsDeleteTask primitive is used for deleting a dynamic task
 * \param[in] tThread The Task handle
 * \return RTK_OK is returned if the dynamic task is deleted successfully; otherwise, returns error ID (Ms_ErrCode_e)
 */
Ms_ErrCode_e MsDeleteTask(MsThread tThread);

/**
 * \fn void MsSuspendTaskByName(const char * name)
 * \brief Suspend task by task name
 * \param[in] Name of The Task
 * \return void
 */
void MsSuspendTaskByName(const char * name);

/**
 * \fn void MsResumeTaskByName(const char * name)
 * \brief Resume task by task name
 * \param[in] Name of The Task
 * \return void
 */
void MsResumeTaskByName(const char * name);

Ms_ErrCode_e MsSelfYield(void);

/**
 * \ingroup UTIL
 * \fn MsOsErr_e MsSleep(u32 ms)
 * \brief The MsSleep primitive suspends the calling task for specified time
 * \param[in] ms The sleep time is milliseconds
 * \return Result of sleeping
 */
MsOsErr_e MsSleep(u32 ms);

/**
 * \fn u32 MsGetOsTick(void);
 * \brief The MsGetOsTick primitive returns the current count of tick
 * \return The current count of tick
 */
u32 MsGetOsTick(void);
u32 MsGetOsMs(void);
u32 MsGetOsHz(void);

void MsGetCurrTaskStack(void** start, void** top);
void MsDumpCurTaskInfo(u32 u32CurTaskSp);
void MsDumpAllTaskInfo(void);

void *MsGetCurrentTcb(void);
char *MsGetTcbStatus(void *tcb);
void MsGetTcbStackInfo(void *tcb, MsContextStackInfo_t *stackInfo);
char *MsGetTcbName(void *tcb);
unsigned long MsGetTcbTaskId(void *tcb);
unsigned long MsGetTcbTaskPriority(void *tcb);
void MsGetTcbGenrealRegInStack(void *tcb, u32 *regs, u32 regNum);
#if defined(CONFIG_VFP_SUPPORT)
#if defined(CONFIG_VFP_LAZY_SWITCH_SUPPORT)
union vfp_state *MsGetTcbVfpStatus(void *tcb);
#endif
#endif
void* MsForEachTcbByIdx(u32 idx);
u32 MsGetTaskAmount(void);

/////////////////////////////////////////////////////////////

MsIntMask_e MsDisableInterrupt(void);
void MsEnableInterrupt(MsIntMask_e eMask);
void MsDisablePreempt(void);
void MsEnablePreempt(void);
void MsSuspendScheduler(void);
void MsResumeScheduler(void);
void MsEnterRegionSaveIrq(unsigned long *flags);
void MsLeaveRegionRestoreIrq(unsigned long *flags);

Ms_ErrCode_e MsInitSpinlock(Ms_Spinlock_t* ptMsLock);
Ms_ErrCode_e MsSpinLock(Ms_Spinlock_t* ptMsLock);
Ms_ErrCode_e MsSpinUnlock(Ms_Spinlock_t* ptMsLock);
Ms_ErrCode_e MsSpinLockIrqSave(Ms_Spinlock_t* ptMsLock);
Ms_ErrCode_e MsSpinUnlockIrqRestore(Ms_Spinlock_t* ptMsLock);

bool MsIsKernelBootReady(void);
u32 MsRunInIsrContext(void);
void MsOsPanic(const char *szMessage);

u32 MsOsGetHwCoreId(void);
u32 MsOsGetCoreId(void);
void MsOsSetPossibleCpuMask(void);
u32 MsOsGetPossibleCpuMask(void);
u32 MsOsGetOnlineCpuMask(void);
int MsIsCoreReady(int iCoreID);
int MsIsCoreSuspend(int iCoreID);
int MsIsCoreOffline(int iCoreID);
int MsIsCoreInvalid(int iCoreID);
int MsGetCoreStatus(int iCoreID);
void MsSetCoreStatus(int iCoreID, int iStatus);

#if defined(CONFIG_CPU_HOTPLUG_SUPPORT)
u32 MsGetHotplugCoreStage(u32 u32CoreID);
void MsSetHotplugCoreStage(u32 u32CoreID, u32 u32State);
void MsClearHotplugCoreStage(u32 u32CoreID, u32 u32State);
void MsSetCoreOffline(u32 u32CoreID);
void MsSetCoreOnline(u32 u32CoreID);
#endif

#endif  /* __SYS_MSWRAPPER_CUS_OS_UTIL_H__ */
