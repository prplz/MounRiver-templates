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

/*------------------------------------------------------------------------------
    PROJECT:
    DESCRIPTION:
          System service functions

    HISTORY:
         <Date>     <Author>    <Modification Description>
       2009/04/07   Ralf Chen   Initial revision

-------------------------------------------------------------------------------*/


#ifndef __SYS_MSWRAPPER_CUS_OS_TYPE_H__
#define __SYS_MSWRAPPER_CUS_OS_TYPE_H__


/*------------------------------------------------------------------------------
    Include Files
-------------------------------------------------------------------------------*/

#include "vm_types.ht"
#if defined(RTOS_KERNEL_USE_FREERTOS)
#include "FreeRTOS.h"
#include "task.h"
#endif

/*------------------------------------------------------------------------------
    EXTERN definition
-------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
    Constant
-------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
    Data Type
-------------------------------------------------------------------------------*/

/* Task ID */
#if defined(RTOS_KERNEL_USE_RTK)
typedef u8 MsTaskId_e;
#elif defined(RTOS_KERNEL_USE_FREERTOS)
typedef TaskHandle_t MsTaskId_e;
#endif


/* Error code of execution */
typedef u8 MsOsErr_e;

/* Mailbox ID */
typedef u16 MsMailbox_e;

/* Semaphore ID */
typedef u16 MsSemId_e;

/*------------------------------------------------------------------------------
    Variable
-------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
    Macro
-------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
    Global Function
-------------------------------------------------------------------------------*/




#endif  /* __SYS_MSWRAPPER_CUS_OS_TYPE_H__ */


