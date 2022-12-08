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

#ifndef __KERNEL_H__
#define __KERNEL_H__

#include "vm_types.ht"
#include "cpu_mem_map.hc"

#ifdef  __cplusplus
extern "C" {
#endif

#include "kernel_itctrl.h"
#include "kernel_axi.h"
#include "kernel_systimer.h"
#include "kernel_csi.h"
#include "kernel_vif.h"
#include "kernel_bdma.h"
#include "kernel_rtc.h"
#include "kernel_sar_adc.h"
#include "kernel_spi.h"

#ifdef  __cplusplus
}
#endif

#endif //__KERNEL_H__
