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

#ifndef __SYS_MSWRAPPER_CUS_OS_INT_PUB_H__
#define __SYS_MSWRAPPER_CUS_OS_INT_PUB_H__

/*=============================================================*/
// Include files
/*=============================================================*/

#if defined(RTOS_KERNEL_USE_RTK)
#include "sys_rtk_ho.h"
#endif
#include "vm_types.ht"

/*=============================================================*/
// Data type definition
/*=============================================================*/
#if defined(RTOS_KERNEL_USE_RTK)
typedef rtk_ItMask_t MsIntMask_e;
#else
typedef u32 MsIntMask_e;
#endif

/*=============================================================*/
// Macro definition
/*=============================================================*/

/** \addtogroup IntMan
 *  @{
 */

/**
 * \def MsDisableInterrupt()
 * \brief The MsDisableInterrupt primitive disable the interrupt and return the mask of disabled interrupts. The mask would be used in enable interrupt. This interface operates the CPU coprocessor IRQ bit and is defined as the kernel mode operation.
 *
 * e.g.
 *
 * MsIntMask_e Mask;
 *
 *      Mask = MsDisableInterrupt();   // disable interrupt
 *
 *      / * The period while the interrupt is disabled. * /
 *
 *      MsEnableInterrupt(Mask);       // enable interrupt
 * \return The mask of disabled interrupts
 */
MsIntMask_e MsDisableInterrupt(void);

/**
 * \def MsEnableInterrupt(eMask)
 * \brief The MsEnableInterrupt primitive enables the interrupt from the mask returned from MsDisableInterrupt primitive. This interface operates the CPU coprocessor IRQ bit and is defined as the kernel mode operation
 * \param[in] eMask The mask of interrupt to enable
 */
void MsEnableInterrupt(MsIntMask_e eMask);

/** @}*/

#endif // __SYS_MSWRAPPER_CUS_OS_INT_PUB_H__
