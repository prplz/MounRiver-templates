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


/******************************************************************/
/*  File    : RTKSRTUC.HT                                         */
/*-------------------------------------------------------------   */
/*  Scope   :  Internal RTK Structures definitions                */
/*                                                                */
/*  Contents:                                                     */
/*                                                                */
/******************************************************************/

#ifndef __SYS_RTK_HT_H__
#define __SYS_RTK_HT_H__

#include "sys_rtk_inconfig.h"
#if defined(CONFIG_VFP_SUPPORT)
#if defined(CONFIG_VFP_LAZY_SWITCH_SUPPORT)
#include "sys_arm_vfp.h"
#endif
#endif

typedef struct CallbackTimer_t_
{
	u32 UserData;
	void (*pfCB)(u32 TimerID, u32 UserData);
} CallbackTimer_t;

typedef struct ObjDescEx_t_
{
  u8 TypeOfObj;      ///< type of object (\ref rtk_ObjectType_e)
  u8 bid;
  u32 TimerID;
  CallbackTimer_t cbtimer;
} ObjDescEx_t;

/******* Timer Structure context *******/

typedef struct TimerStructure
{
    struct TimerStructure *NextPtrTimer;
    u64		target_ts; //For ARM GTimer
    u32    Counter;
    u32    sem;
    u32    TimerID;
    u8     MailBoxNo;
    u16     SemId;
    bool   m_bool_StrictTimer;
	void   *flag_obj;
	u32    flag_timer;
#ifdef __RTK_TIMER_PERIODIC__
    bool   bCyclic;
    u32    saveCounter;
#endif
	CallbackTimer_t cbtimer;
} rtk_Timer_t;


/******* System register *******/
typedef struct RegStructure
{
    void           *SP;
    void           *PC;
    void           *TopOfStack;
    u16            StackSize;
} rtk_Register_t ;

#define RTK_FLAG_GUARD (0x47414C46) //"FLAG"
#define RTK_FLAG_INITIALIZED (0x54494E49) //"INIT"
#define RTK_FLAG_DESTROY (0x74656C64) //"dlet"
#define RTK_FLAG_TIMER (0x726D6974) //"timr"

typedef enum {
  RTK_FLAG_WAITMODE_AND = (1<<0),             ///< wake up if all bits specified in the mask are set in the flag.
  RTK_FLAG_WAITMODE_OR = (1<<1),              ///< wake up if any bits specified in the mask are set in the flag.
  RTK_FLAG_WAITMODE_CLR = (1<<2)              ///< clear all bits in the flag when the condition is met. Typically,only the bits that are set in the pattern parameter are cleared.
} _rtk_flag_mode_e;

typedef u8 rtk_flag_mode_t;
typedef s32 rtk_flag_value_t;
typedef u32 rtk_tick_count_t;

/******* Structure d'un flag *******/
typedef struct FlagStructure
{
	s32 FlagGuard;
    rtk_flag_value_t FlagValue;
    rtk_Envelop_t *PtrFirstTask;
	s32 FlagState;
	u8 FlagId;
} rtk_Flag_t ;

typedef struct FlagWaitInfoStructure
{
	rtk_Flag_t *wait_PtrFlag;
	u32 awaken_address;
	rtk_flag_value_t wait_pattern;
	rtk_flag_value_t flag_return_value;
	rtk_flag_mode_t wait_mode;
} RtkFlagWait_info_t ;

/******* Structure d'un Mutex *******/
typedef enum
{
    PRIO_INVR_NONE = 0,
    PRIO_INVR_INHERIT,
    PRIO_INVR_CEILING
}rtk_Prio_Invr_e;

typedef struct MutexStructure
{
    bool bLocked;
    u8 uMutexCount;
    struct TaskStructure* pTaskOwner;       // Owner
    rtk_Envelop_t* PtrTaskQueue;            // Queue
    rtk_Prio_Invr_e protocol;

    // Elements for priority inversion
    u8 original_priority;
    bool priority_inherited;
} rtk_Mutex_t ;

/*****************************************************************/
/*        !!!!!!!!!!!!!!!!! BEWARE !!!!!!!!!!!!                  */
/*                                                               */
/* if the offset of MarkerSP, Marked or Id is changed, update it */
/* in rtk2util.asm the corresponding value                       */
/*                                                               */
/*****************************************************************/


#define STACK_POOL_GUARD_PATTERN (0x78787878)
#define CUSTO_POOL_GUARD_PATTERN (0x56565656)
#define DEBUG_POOL_GUARD_PATTERN (0x34343434)
#define NB_POOLS_MONITORED (16)

#define RTK_TASK_NAME_CHAR_LEN	(16)

typedef struct TaskStructure
{
    u8             State;
    u8             Priority;
    u8             TypeOfObj;
    u8             Id;
    union
    {
        u32      ObjParameter;
        void     *PtrMessage;
    } PendingObj;
    rtk_Register_t Reg;
    struct TaskStructure *PtrNextPrio;
    rtk_Timer_t   *PtrTimer;
    u32    MarkerSP;
    bool   Marked;
  u8 bitmapIndex;
  u8 bitmapMask;
#ifdef RTK_EQUAL_PRIORITY_TASK
  u8 IndexOfBitmapIdx2task;
#endif
  rtk_SemId_t WaitSemID;
  void *WaitDynSem;
  bool   userOwnCallStack;
	RtkFlagWait_info_t flag_wait_info;
#ifdef RTK_EQUAL_PRIORITY_TASK
    struct TaskStructure *PtrNextSamePrio;
#endif

    rtk_Envelop_t* pOwnedMutex;
    rtk_Mutex_t  * pWaitMutex;

    u32 userData;

    char TaskName[RTK_TASK_NAME_CHAR_LEN];
    u32 scheduledOutTime;

    u64 execTicksAccumulated;

#if defined(CONFIG_VFP_SUPPORT)
#if defined(CONFIG_VFP_LAZY_SWITCH_SUPPORT)
    union vfp_state vfpstate;
#endif
#endif

#if defined(__TIME_SLICE_SCHEDULE__)
    u32 ScheduleInTime;
    u32 TimeSliceMax;
    u32 TimeSliceLeft;
#endif
} rtk_Task_t;

#define LEN_rtk_Task_t sizeof (rtk_Task_t)

/******* Structure d'un Semaphore *******/

typedef struct SemStructure
{
    s16         CurrentValue;
    s16         InitValue;
    rtk_Envelop_t *PtrFirstTask;
} rtk_Semaphor_t ;

/******* Structure d'une BAL *******/

typedef struct MBoxStructure
{
    rtk_Envelop_t *PtrFirstTask;          /* first task which is waiting to receive this mailbox */
    rtk_Envelop_t *PtrLastTask;           /* last task which is waiting to receive this mailbox */
    rtk_Envelop_t *PtrLastObj;             /* last object which is hung on this mailbox */
    rtk_Envelop_t *PtrObjQue;              /* first object which is hung on this mailbox */
#ifdef RTK_MAILBOX_THRESHOLD_CHECK
    u8              Threshold;  //a value of 1~99. it's used to determine msg can be sent or not if calling MsSendWithThreshold
    u8              padding1;
    u8              padding2;
    u8              padding3;
#endif
} rtk_Mailbox_t;

/******* Structure used for InitKernel *******/

typedef struct rtk_KernelInit
{

} rtk_KernelInit_t;


/******* Structure used for InitTask *******/

typedef struct rtk_TaskInit
{
    u8  rtkTaskNumber;
    u8  rtkTaskPriority;
    void (*rtkStartAdress) _PROTO((void));
    u16  rtkTaskStackLength;
    void *rtkTaskStack;
    void *rtkTopOfStack;
    char rtkTaskName[16];
    u32 userData;
#if defined(__TIME_SLICE_SCHEDULE__)
    u32 TimeSliceLeft;
    u32 TimeSliceMax;
#endif
} rtk_TaskInit_t;

/* Storage structure for heap statistics */

typedef struct {
   u16 MbxSrc;
   u16 MbxDst;
   u16 Length;
   u16 TypMsg;
   u32 FN;
} rtk_MessageSave_t;

typedef struct
{
	void*	TopOfStack;
 	u16		StackSize;
	u32		MaxUsage;
    char	TaskName[RTK_TASK_NAME_CHAR_LEN];
} rtk_StackInfo_t;

#define RTK_MBOX_HISTORY_NOT_RECEIVED		0xFF
#define RTK_MBOX_HISTORY_IRQ_SENT				0xFD
#define RTK_MBOX_HISTORY_FIQ_SENT				0xFC

extern rtk_MessageSave_t MessageSave[];
extern u8 MessageSaveIndex;

/* Structure of the Message */
/*****************************************/
/*    Message structure                  */
/*****************************************/

/* Header Part of the message */
typedef struct
{
    u8  MbxSrc;
    u8  ModuleID;
    u16  MbxDst;
    s16  Length;
    u16  TypMsg;
} vm_msgHeader_t;

struct vm_msg
{
    vm_msgHeader_t   Header;
};

/**
 * \brief message opaque type
 * \see vm_msgt.ht
 */
typedef struct vm_msg vm_msg_t;

#endif          /* __SYS_RTK_HT_H__ */

