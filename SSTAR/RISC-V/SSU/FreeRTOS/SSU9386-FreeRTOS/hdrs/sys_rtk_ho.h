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

/****************************************************************************/
/*  File    : rtk.ho                                                        */
/*--------------------------------------------------------------------------*/
/*  Scope   : RTK exports                                                   */
/*                                                                          */
/*  Contents:                                                               */
/*                                                                          */
/*                                                                          */
/*  Historical :                                                            */
/****************************************************************************/

/*!
 *   \file sys_rtk_ho.h
 *
 *   \brief public rtk include file
 *
 *   this file defines the public interface of RTK
 *
 */

#ifndef __SYS_RTK_HO_H__
#define __SYS_RTK_HO_H__


#include <stdarg.h>
#include "sys_rtk_vmrtkho.h"
#include "sys_rtk_config.h"

#define __REMOTE_RTK__
#define __RTK_DIAGNOSE__

#ifdef __RTK_DATA__
#define RtkDefExtern
#else
#define RtkDefExtern    extern
#endif

/******* Return status parameters *******/

/*!
 *      \brief Rtk error codes. These values are returned by most of the RTK functions
 */
typedef enum {
  RTK_OK= 10,                         ///< No error - the function call was successfull
  RTK_NOT_RELEASED= 11,               ///< The bloc was not released
  RTK_UNIT_NOAVAIL= 21,               ///< Not available
  RTK_NO_MESSAGE  = 33,               ///< No message found
  RTK_NON_SATISFACTORY_POOL=   40,    ///< No memory pool found (-deprecated-)
  RTK_NO_MEMORY_AVAIL      =   41,    ///< Not enough memory
  RTK_CLUSTER_OUT_OF_RANGE =   42,    ///<
  RTK_TIMER_NOT_FOUND      =   43,    ///< Invalid timer id
  RTK_MEM_OVERFLOW         =   44,    ///<
  RTK_NULL_RELEASE         =   45,    ///< Try to release a NULL pointer
  RTK_NOT_HEAP_RELEASE     =   46,    ///< Memory zone is not in heap
  RTK_NOT_MEMORY_RELEASE   =   47,     ///< Memory zone is not in pool memory (-deprecated-)
  RTK_MAILBOX_FULL   =   48,     ///< Mailbox is full (-deprecated-)
  RTK_TIMER_EXHAUSTED   =   49,     ///< Timer is unavailable (-deprecated-)
  RTK_MAILBOX_NOT_EXIST = 50,     ///< Mailbox is in initial state since dynamic task is deleted
  RTK_STACK_ADDRESS_ERROR = (-1),
  RTK_ENTRY_POINTER_ERROR = (-2),
  RTK_PRIORITY_ERROR = (-3),
  RTK_TASK_EXHAUSTED = (-4),
  RTK_TASK_ID_ERROR = (-5),
  RTK_TASK_NOT_EXISTED = (-6),
  RTK_TASK_ALREADY_CREATED = (-7),
} _rtk_ErrCode_e;

typedef u8 rtk_ErrCode_e;
/*!
 *    \brief memory area type
 */
typedef enum {
  RTK_GEN_NOT_DYNAMIC_MEMORY,    ///< Not a dynamic memory area
  RTK_GEN_HEAP_STACK,            ///< Stack's Heap zone
  RTK_GEN_POOL_STACK,            ///< Stack'sPool memory area (-deprecated-)
  RTK_GEN_HEAP_CUS,              ///< Customer's heap zone
  RTK_GEN_POOL_CUS,              ///< Customer's pool area (-deprecated-)
  RTK_GEN_HEAP_OPEN_AT           ///< Open AT heap zone
}rtk_MemoryAreaType_e;

/****** Received messages *********************/

/* Even though this value is mostly used in rtk_Task_t.TypeOfObj
   there are parts of the cause where it must be a one-bit boolean
   (see TypeOfEnv in Rtk10TimerExtract() for instance).
   So we use RTK_OBJ_MESSAGE for both mail-boxes and semaphores
   since a task cannot be waiting on both at the same time
   but both may be interrupted by the expiration of a timer.
*/

/*!
 *  \brief type of an object
 */
typedef enum {
  RTK_OBJ_MESSAGE =                      0,  ///< Object is a message
  RTK_OBJ_TIMER =                        1,   ///< Object is a timer
  RTK_OBJ_EVENT=                        2 ///< Object is an event
} rtk_ObjectType_e;

#define	RTK_FORCE_SUSPEND				(0xFFFFFFFF)
#define	RTK_FORCE_ASSERT				(0xFFFFFFFE)

#if defined(__TIME_SLICE_SCHEDULE__)
#define RTK_SCHEDULE_TIMER_ID           (0xFFFFFFFE)
#endif

#ifdef __REMOTE_RTK__

/****** Type of remote mailboxes *************/

#define RTK_REMOTE_MBX                         0x01
#define RTK_LOCAL_MBX                          0x02
#endif

/*********************** some timer values ***************************/
/*!
 *   \brief Rtk tick time (integer approximation)
 */
#define RTK_TICK_TIME         VM_RTK_TICK_TIME
/*!
 *   \brief Rtk tick time (float value)
 */
#define RTK_TICK_TIME_REAL    VM_RTK_TICK_TIME_REAL
/*!
 *   \brief TDMA frame delay
 */
#define RTK_TDMA_FRAME_DELAY  4.615
/*!
 *   \brief macro to convert milli seconds to ticks
 */
#define RTK_MS_TO_TICK(MsT)   VM_RTK_MS_TO_TICK(MsT)
#define RTK_MS_TO_TICK_64(MsT)   VM_RTK_MS_TO_TICK_64(MsT)
/*!
 *   \brief macro to convert seconds to ticks
 */
#define RTK_S_TO_TICK(SecT)   VM_RTK_S_TO_TICK(SecT)
/*!
 *   \brief macro to convert minuts to ticks
 */
#define RTK_MN_TO_TICK(MnT)   VM_RTK_MN_TO_TICK(MnT)
/*!
 *   \brief macro to convert Ticks to s
 */
#define RTK_TICK_TO_S(_tick_)   VM_RTK_TICK_TO_S(_tick_)
/*!
 *   \brief macro to convert Ticks to ms
 */
#define RTK_TICK_TO_MS(_tick_)   VM_RTK_TICK_TO_MS(_tick_)
/*!
 *   \brief Rtk tick time (float value)
 */
#define RTK_TICK_TIME_REAL    VM_RTK_TICK_TIME_REAL
/*!
 *   \brief macro to convert TDMA frame to ticks
 */
#define RTK_FRAME_TO_TICK(F)  VM_RTK_FRAME_TO_TICK(F)
/*!
 *   \brief macro to convert ticks to TDMA frame
 */
#define RTK_TICK_TO_FRAME(T)  VM_RTK_TICK_TO_FRAME(T)
/*!
 *   \brief maximum value for an RTK timer
 */
#define RTK_MAXTIMER_VALUE      VM_RTK_MAXTIMER_VALUE

/******* Structure returned by Receive and TestReceive *******/

/*!
 *  \brief object structure ( as returned by \ref RtkReceive functions )
 */
typedef struct ObjDescriptorStructure {
  u8 TypeOfObj;      ///< type of object (\ref rtk_ObjectType_e)
  u8 Bid;            ///< Bid
  union  {
    u32   TimerID;   ///< Identifier of a timer object
    void  *Message;  ///< Message content of a message object
  }ReceivedObj;      ///< Received object content
} rtk_ObjDesc_t;


struct rtk_KernelInit;
struct rtk_TaskInit;
struct TaskStructure;

/******* Envelop Structure *******/

/*!
 *  \brief enveloppe structure
 */
typedef struct EnvStructure {
	struct EnvStructure *PtrNextEnv;               ///< Pointer to next enveloppe of an  envelopped linked array
	union  {
		u32      TimerID;                                 ///< Timer identifier
		u8       *PtrMessageObj;                    ///< Pointer to a message
		struct   TaskStructure  *PtrTaskObj;  ///< Pointer to a Task structure
		void     *PtrQueueObj;                       ///< Pointer to a queue
		struct   FlagStructure   *PtrFlagObj;   ///< Pointer to a flag structure
		struct   MutexStructure   *PtrMutexObj; ///< Pointer to a mutex structure
	}PtrObject;                                                ///< object's content
	u32 lr;                                                         /// caller function
	u32 flag;                                                     /// additional debug info
}rtk_Envelop_t;

/******* Structure for Queue management ***********/

/*!
 *   \brief queue structure
 */
typedef struct {
  rtk_Envelop_t  *First; ///< First element of the queue
  rtk_Envelop_t *Last;   ///< Last element of the queue
  u32 count;
} rtk_Queue_t;

/*!
 *   \brief used with \ref RTK2_DISABLE to store the it mask
 */
typedef  u32  rtk_MaskImage_t;
/*!
 *   \brief used with \ref RTK2_DISABLE to store the it mask
 */
typedef  rtk_MaskImage_t rtk_ItMask_t;

/*!
 *   \brief timer identifier type
 */
typedef u32 rtk_TimerId_t;
/*!
 *   \brief task identifier type
 */
typedef u8 rtk_TaskId_t;

/*!
 *  \brief mailbox identifier type
 */
typedef u8 rtk_MailboxId_t;

/*!
 *  \brief semaphore identifier type
 */
typedef u16 rtk_SemId_t;


/* ------------------------------------------------------------------------*/
/*              C RTK prototype interface functions                        */
/* ------------------------------------------------------------------------*/

  /*!
   *   \fn bool RtkMailboxStatus (rtk_MailboxId_t MboxNo, u32 *limit, u32 * max, u32 * current)
   *
   *   \get rtk mailbox status information
   *
   *   \param MboxNo the mailbox identifier
   *   \param limit  the limitation number of this mailbox
   *   \param max    the max unprocessed number of this mailbox
   *   \param current the current messge number of this mailbox
   *
   *   \return TRUE if MboxNo is correct
   *   \return FALSE if MboxNo is invalid or error occurs
   *
   */
  extern bool RtkMailboxStatus (rtk_MailboxId_t MboxNo, u32 *limit, u32 * max, u32 * current);

  /*!
   *    \fn void RTK2_ENABLE(rtk_ItMask_t itmask)
   *
   *    \brief enable back interruptions using the specified mask (\ref RTK2_DISABLE )
   *
   *    \param itmask the interruptions mask to restore
   *
   *    \return void
   */
  void           RTK2_ENABLE(rtk_ItMask_t itmask);

#define RTK2_ENABLE_K(itmask) RTK2_ENABLE(itmask)

  /*!
   *    \fn rtk_ItMask_t   RTK2_DISABLE()
   *
   *    \brief disable interruptions and return previous interruptions mask
   *
   *    \return the interruptions mask to use with \ref RTK2_ENABLE to restore interruptions
   */
  rtk_ItMask_t   RTK2_DISABLE(void);

#define RTK2_DISABLE_K RTK2_DISABLE

  /*!
   *    \fn rtk_ItMask_t   RTK2_DISABLE_IRQ()
   *
   *    \brief disable interruptions (only IRQ, not FIQ) and return previous interruptions mask
   *
   *    \return the interruptions mask to use with \ref RTK2_ENABLE to restore interruptions
   */
  rtk_ItMask_t   RTK2_DISABLE_IRQ(void);
void RTK2_ENABLE_IRQ(rtk_ItMask_t itmask);
  //////////////////////////////////////////////////////////////////////////
  //  QUEUE FUNCTIONS
  //////////////////////////////////////////////////////////////////////////

  /*!
   *    \fn rtk_ErrCode_e RtkQueue(rtk_Queue_t *Q, void *E);
   *
   *    \brief Enqueue an item at the end of the specified queue
   *    \param Q a \ref rtk_Queue_t pointer defining the queue
   *    \param E the element to enqueue
   *    \return RTKOK
   *
   */
  rtk_ErrCode_e  RtkQueue( rtk_Queue_t *, void * );

  /*!
   *   \fn rtk_ErrCode_e RtkQueueFirst(rtk_Queue_t *Q, void *E);
   *
   *   \brief Enqueue an item at the beginning of the queue
   *   \param Q a \ref  rtk_Queue_t pointer that specifies the queue
   *   \param E a pointer to the element to insert.
   *
   *   \return RTK_OK
   */
  rtk_ErrCode_e RtkQueueFirst(rtk_Queue_t *Q, void *E);

  /*!
   *    \fn void * RtkDeQueue(rtk_Queue_t *Q);
   *    \brief Remove the first element of a queue
   *
   *    \param Q a \ref rtk_Queue_t pointer that specifies the queue
   *    \return  a pointer to the element that was removed from the queue
   */
  void      *RtkDeQueue( rtk_Queue_t * );

  /*!
   *   \fn rtk_ErrCode_e  RtkDeleteQueue(rtk_Queue_t *Q,void *E_REF);
   *
   *   \brief remove the specified element from a queue
   *
   *   \param Q a \ref rtk_Queue_t pointer that defines the queue
   *   \param E_REF a pointer to the item to be removed
   *   \return \li RTK_OK if the element was succesfully removed
   *           \li FALSE  if the element does not belong to the queue
   */
  rtk_ErrCode_e        RtkDeleteQueue ( rtk_Queue_t *, void * );

  /*!
   *    \fn void RtkInitQueue(rtk_Queue_t* Q);
   *
   *    \brief Init the given queue, if not NULL
   *    \param Q a \ref rtk_Queue_t pointer defining the queue
   *    \return none
   *
   */
#ifndef NULL
#define NULL 0
#endif
__inline static void RtkInitQueue(rtk_Queue_t* Q)
{
    if ( Q != NULL )
    {
        Q->First=Q->Last = (rtk_Envelop_t*)NULL;
	 Q->count = 0;
    }
}

  void      RtkConnectTimerInterrupt _PROTO(( void ** ));
  void      RtkDisconnectTimerInterrupt _PROTO(( void ** ));

  u32	  RtkTaskMarkerCall _PROTO((void *Fct, u8 NbPar, ...));
  void      RtkDefaultHook _PROTO((u8 Mbox, void *Msg, bool IsLocal));


  //////////////////////////////////////////////////////////////////////////
  //  MISC FUNCTIONS
  //////////////////////////////////////////////////////////////////////////

  u16       RtkGetVersion _PROTO((void));
  u8        RtkInitTask _PROTO((struct rtk_TaskInit *PtrTaskInfo));

#ifdef __REMOTE_RTK__
  void      RtkRestartTasks _PROTO((void));
#endif
  u8        RtkChangeTaskPriority _PROTO((u8 rtkTaskNumber,u8 rtkNewPriority));
  u8        RtkSetTaskName _PROTO((u8 rtkTaskNumber,const char *szTaskName));
  u8        RtkGetTaskName _PROTO((u8 rtkTaskNumber,char *szTaskName, u32 nMaxLen));
  void      RtkSuspendTaskTreat(u32 i_u32_NbTask, va_list i_s_args );
  void      RtkResumeTaskTreat(u32 i_u32_NbTask, va_list i_s_args );
  u8        RtkInitMailBox _PROTO((u8 MboxNo));
  void      RtkSuspendTask _PROTO((u32 NbTask, ...));
  void      RtkResumeTask _PROTO((u32 NbTask, ...));

#if defined (__RTK_OSTICK_ENHANCE__)
  void RtkConnectGlobalTimerInterrupt _PROTO((void **));
  void RtkDisconnectGlobalTimerInterrupt _PROTO((void **)) ;
#endif

#if defined(__TIME_SLICE_SCHEDULE__)
  u8 RtkChangeTaskTimeSlice(u8 uTaskId, u32 uTimeSlice);
  u8 RtkSelfYield(void);
#endif

#ifdef __RTK_DIAGNOSE__
  void      RtkSetOwner _PROTO(( u8 , void * ));
#else
#define   RtkSetOwner(x, y)
#endif

  u8 RtkGetTaskStack( u8 rtkTaskNumber, void **PtrStackBase, u32 *StackSize );
  u8 RtkGetTaskState( u8 rtkTaskNumber, u32 *pPC, u32 *pState);
  u8 RtkSetTaskState( u8 rtkTaskNumber, u32 PC, u32 state);
  u8 RtkRegisterUserOwnCallStack(bool val);

  /* ------------------- to be provided par user ----------------------------- */

typedef enum
{
	RTK_NON_TASK_STACK_TYPE_MARKER,
	RTK_NON_TASK_STACK_TYPE_FIQ,
	RTK_NON_TASK_STACK_TYPE_IRQ,
	RTK_NON_TASK_STACK_TYPE_SUP,
	RTK_NON_TASK_STACK_TYPE_MAX
} rtk_non_task_stack_type_e;

u32 RtkScanTaskStackDepth(u8 taskId, void* pStkInfo);
/* ------------------------------------------------------------------------*/
/*              EXPORTERTED variables                                      */
/* ------------------------------------------------------------------------*/

#define RTK_ALL_TASK_LOCAL 0xffffffff
#define RTK_ALL_MBX_LOCAL  0xaaaaaaaa

#define RTK_ALL_STACK_TASK 0xe03fffff  /* customer tasks are excluded id[22-28] */

#define MBX_STATE_NUM (RTK_MAX_TASKS/16+(RTK_MAX_TASKS%16?1:0))

/* implementation/declaration of some data */
#ifdef __RTK_DATA__

u32 rtk_MbxState[MBX_STATE_NUM] = {
#if (MBX_STATE_NUM==(5))
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL,	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL
#elif(MBX_STATE_NUM==(6))
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL,
	RTK_ALL_MBX_LOCAL
#elif(MBX_STATE_NUM==(7))
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL,
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL
#elif(MBX_STATE_NUM==(8))
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL,
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL
#elif(MBX_STATE_NUM==(9))
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL,
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL
#elif(MBX_STATE_NUM==(10))
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL,
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL
#elif(MBX_STATE_NUM==(11))
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL,
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL,
	RTK_ALL_MBX_LOCAL
#elif(MBX_STATE_NUM==(12))
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL,
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL,
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL
#elif(MBX_STATE_NUM==(13))
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL,
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL,
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL
#elif(MBX_STATE_NUM==(14))
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL,
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL,
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL
#elif(MBX_STATE_NUM==(15))
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL,
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL,
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL
#elif(MBX_STATE_NUM==(16))
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL,
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL,
	RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL, RTK_ALL_MBX_LOCAL,
	RTK_ALL_MBX_LOCAL
#else
#error "pls add new array!!"
#endif
	};

void (*rtk_MbxHook)( u8 Mbx, void *Msg, bool IsLocal ) = RtkDefaultHook;

#else

extern  u32 rtk_MbxState[MBX_STATE_NUM];

extern  void (*rtk_MbxHook)( u8 Mbx, void *Msg, bool IsLocal );
//extern void *rtk_OverwriteArea;
#endif


/******** QUEUE MANAGEMENT **********/

#define RtkLastQueue(Q,E)          (*E = Q->Last, Q->Last->PtrObject.PtrQueueObj)
#define RtkReplaceObjectQueue(E,P) ((rtk_Envelop_t *) (*E))->PtrObject.PtrQueueObj = (void*) P

#define RtkTopQueue(Q,E)						\
  (									\
   ((rtk_Queue_t*)Q) ? (						\
			(*E = (void*)((rtk_Queue_t*)Q)->First),		\
			(((rtk_Queue_t*)Q)->First) ? ((void *)(((rtk_Envelop_t*)(*E))->PtrObject.PtrQueueObj)): NULL \
			):NULL						\
   )

// How to use this macro can be found in A7.18 of "The C Program Language, 2nd Edition".
// The expression after "," means the return value of this macro.
#define RtkNextQueue(E)							\
  ((*E) ?								\
   ((((rtk_Envelop_t*) (*E))->PtrNextEnv ) ?				\
    (*E = (void*) ((rtk_Envelop_t*) (*E))->PtrNextEnv, (void *) (((rtk_Envelop_t*) (*E))->PtrObject.PtrQueueObj )) \
    : (*E = (void*) NULL, (void*) NULL))				\
   : (void*) NULL)

// Correct LINT Error 522, this macro is defined.
// RtkNextQueue_No_Return() does not have return value (used by hac1tim.c in L1c).
#define RtkNextQueue_No_Return(E)							\
  ((*E) ?								\
   ((((rtk_Envelop_t*) (*E))->PtrNextEnv ) ?				\
    (*E = (void*) ((rtk_Envelop_t*) (*E))->PtrNextEnv) \
    : (*E = (void*) NULL))				\
   : (void*) NULL)



#define RtkObjectQueue(E) ((*E) ? (((rtk_Envelop_t*) (*E))->PtrObject.PtrQueueObj) : ((void *) NULL ))


/************* MACRO INTERRUPT *******************/

#define RtkTraceCriticalSessionTiming()
#define RtkStartQbTiming(x)
#define RtkUpdateFrameLength(x)


/************* REMOTE MANAGEMENT MACRO *******************/
//#define RTK_GET_TSK_STATE(t)    ((rtk_TaskState>>t)&1)
#define RTK_GET_MBX_STATE(m)    ((rtk_MbxState[m >> 4] >> ((m & 15) << 1)) & 3)
#define RTK_SET_MBX_STATE(m, s) rtk_MbxState[m >> 4] =	\
    (rtk_MbxState[m >> 4] & ~(3 << ((m & 15) << 1))) |	\
    (s << ((m & 15) << 1))

RtkDefExtern    u16 huge        *rtk_CustHeapStart;
RtkDefExtern    u16 huge        *rtk_CustHeapEnd;

#include "sys_rtk_ht.h"

#define CONVERT_U8_U32 0xFF

#if defined(__IRAM_ALLOC__)
RtkDefExtern u32 ms_iramhandle;
#endif

/* ------------------- to be provided par user ----------------------------- */
#undef RtkDefExtern

#endif  /* __SYS_RTK_HO_H__ */

