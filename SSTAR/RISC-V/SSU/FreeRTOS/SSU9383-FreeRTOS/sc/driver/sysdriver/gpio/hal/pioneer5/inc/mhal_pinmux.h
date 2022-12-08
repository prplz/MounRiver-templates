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

#ifndef __MHAL_PINMUX_H__
#define __MHAL_PINMUX_H__

#include "vm_types.ht"

extern S32   HalPadSetVal(U32 u32PadID, U32 u32Mode);
extern S32   HalPadGetVal(U32 u32PadID, U32* u32Mode);
extern S32   HalPadCheckVal(U32 u32PadID, U32 u32Mode);
extern S32   HalPadSetMode(U32 u32Mode);
extern U8    HalPadCheckInfoCount(void);
extern void* HalPadCheckInfoGet(U8 u8Index);
extern U8    HalPadModeToVal(U8* pu8Mode, U8* u8Index);
extern void  HalGPIOSetVol(U32 u32Group, U32 u32Mode);
extern U32*  HalPadModeToPadIndex(U32 u32Mode);

#endif // __MHAL_PINMUX_H__
