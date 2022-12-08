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

#ifndef __SYS_RTK_INCONFIG_H__
#define __SYS_RTK_INCONFIG_H__
/**!
 *   \file rtkinternalconfig.h
 *
 *   \brief defines constants that controls internal behaviour of RTK
 *
 ****/


//=======================================================================
//  SCHEDULER OPTIONS



//=======================================================================
//  TASKS OPTIONS
#define RTK_EQUAL_PRIORITY_TASK


//=======================================================================
//  MAILBOXS OPTIONS
#define RTK_MAILBOX_THRESHOLD_CHECK


//=======================================================================
// DEBUGGING - BENCHMARKING OPTIONS

/**!
 *    \brief define the following flag to activate benchmark on the time
 *           to choose a new task to schedule to
 ***/
//#define __MEASURE_SCHEDULING_TIME__


#endif // __SYS_RTK_INCONFIG_H__

