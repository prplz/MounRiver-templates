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

#ifndef __SYS_RTK_CONFIG_H__
#define __SYS_RTK_CONFIG_H__

/*!
 *    \file sys_rtk_config.h
 *
 *    constants that defines the configuration of rtk
 *
 */

/*!
 *    \brief defines the maximum number of tasks
 *
 */
#define RTK_MAX_TASKS 200

/*!
 *    \brief max priority value ( all tasks priorities have to be lower or equal than this one )
 *
 */
#define RTK_MAX_TASK_PRIORITY RTK_MAX_TASKS

/*!
 *    \brief count of reserved system tasks
 */
#define RTK_MAX_SYSTEM_TASKS  40

/*!
 *    \brief count of reserved customer tasks
 */
#define RTK_MAX_CUSTOMER_TASKS (RTK_MAX_TASKS - RTK_MAX_SYSTEM_TASKS)

/*!
 *    \brief first customer task identifier
 */
#define RTK_FIRST_CUSTOMER_TASKID RTK_MAX_SYSTEM_TASKS

/*!
 *    \brief defines the maximum number of mailboxes
 *
 */
#define RTK_MAX_MAILBOXES 200

/*!
 *    \brief defines the maximum number of envelop
 *
 */
#define RTK_MAX_ENVEPOL 400

/*!
 *    \brief defines the maximum number of timer
 *
 */
#define RTK_MAX_TIMER 60

/*!
 *    \brief defines the maximum number of static semaphore
 *
 */
#define RTK_MAX_SEMAPHORE 8
#define TASK_RTK_SLEEP_SEM      0

/*!
 *    \brief first customer mailbox id
 *    each customer task is associated to a customer mailbox
 */
#define RTK_FIRST_CUSTOMER_MAILBOXID 40

#define RTK_MAX_SYSTEM_MBOXS		RTK_FIRST_CUSTOMER_MAILBOXID
#define RTK_MAX_CUSTOMER_MBOXS	(RTK_MAX_MAILBOXES - RTK_MAX_SYSTEM_MBOXS)

// check the current configuration

#if ((RTK_FIRST_CUSTOMER_MAILBOXID+RTK_MAX_CUSTOMER_TASKS) > RTK_MAX_MAILBOXES)

#error not enough mailboxes for the customer tasks

#endif


// length in bytes of the task state bitmap array
#define RTK_TASKS_BITMAP_LENGTH ((RTK_MAX_TASKS+7)/8)

#define RTK_MAX_TASK_TIMEOUT_DISABLE 	(0xFFFFFFFF)

#endif // __SYS_RTK_CONFIG_H__
