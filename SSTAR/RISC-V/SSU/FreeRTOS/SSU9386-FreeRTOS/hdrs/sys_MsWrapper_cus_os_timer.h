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

#ifndef __SYS_MSWRAPPER_CUS_OS_TIMER_H__
#define __SYS_MSWRAPPER_CUS_OS_TIMER_H__

/*------------------------------------------------------------------------------
    Include Files
-------------------------------------------------------------------------------*/

#include "sys_MsWrapper_cus_os_type.h"
#if defined(RTOS_KERNEL_USE_FREERTOS)
#include "FreeRTOS.h"
#include "timers.h"
#endif

/*------------------------------------------------------------------------------
    Constant
-------------------------------------------------------------------------------*/

#define TIMER_MBX       36

/*------------------------------------------------------------------------------
    Data Type
-------------------------------------------------------------------------------*/

#if defined(RTOS_KERNEL_USE_RTK)
typedef u32             MsTimerId_e;
#elif defined(RTOS_KERNEL_USE_FREERTOS)
typedef TimerHandle_t   MsTimerId_e;
#endif

#if defined(RTOS_KERNEL_USE_RTK)
typedef struct MsCbTimer_t_
{
    MsTimerId_e eTimerID;
    u32 UserData;
    void (*pfCB)(MsTimerId_e eTimerID, u32 UserData);
} MsCbTimer_t;
#endif

typedef struct
{
    MsTimerId_e eTimerID;
    void (*pfnCallback)(unsigned long);
    void *pDataPtr;
#if defined(RTOS_KERNEL_USE_FREERTOS) && (configSUPPORT_STATIC_ALLOCATION == 1)
    StaticTimer_t buf;
#endif
} MsTimerExt_t;

/*------------------------------------------------------------------------------
    Global Function
-------------------------------------------------------------------------------*/
/** \addtogroup TimerMan
 *  @{
 */

MsOsErr_e MsTimerExtInit(MsTimerExt_t *ptHandle);
u32 MsTimerExtDelete(MsTimerExt_t *ptHandle);
MsOsErr_e MsTimerExtAdd(MsTimerExt_t *ptHandle, u32 nMsec, void *pDataPtr,
                            void (*pfnFunc)(unsigned long nDataAddr));
MsOsErr_e MsTimerExtModify(MsTimerExt_t *ptHandle, u32 nMsec);

#endif  /* __SYS_MSWRAPPER_CUS_OS_TIMER_H__ */
