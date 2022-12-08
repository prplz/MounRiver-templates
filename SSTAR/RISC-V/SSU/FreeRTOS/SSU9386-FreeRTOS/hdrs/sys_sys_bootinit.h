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
 * \file sys_sys_bootinit.h
 * \brief System boot init interface
*/
/****************************************************************************/

#ifndef __SYS_SYS_BOOTINIT_H__
#define __SYS_SYS_BOOTINIT_H__

#include "vm_types.ht"

/**
 * \brief macros
 */
#if defined(__arm)
#define __ONCE_CALL_ON_BOOT__          __attribute__((section(".bootcalloncesection")))
#define __RO_CODE_ON_BOOT__            __attribute__((section(".RoCodeOnBoot")))
#else
#define __ONCE_CALL_ON_BOOT__
#define __RO_CODE_ON_BOOT__
#endif

#endif  // __SYS_SYS_BOOTINIT_H__
