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

#ifndef _MHAL_GPIO_H_
#define _MHAL_GPIO_H_

#include "vm_types.ht"

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

typedef enum
{
    MHAL_PULL_UP = 0,
    MHAL_PULL_DOWN,
    MHAL_PULL_OFF,
} MHal_PadPull;

extern void  MHal_GPIO_Init(void);
extern void  MHal_GPIO_Pad_Set(U8 u8IndexGPIO);
extern U8    MHal_GPIO_PadGroupMode_Set(U32 u32PadMode);
extern U8    MHal_GPIO_PadVal_Set(U8 u8IndexGPIO, U32 u32PadMode);
extern U8    MHal_GPIO_PadVal_Get(U8 u8IndexGPIO, U32 *u32PadMode);
extern void  MHal_GPIO_VolVal_Set(U8 u8Group, U32 u32Mode);
extern U8    MHal_GPIO_PadVal_Check(U8 u8IndexGPIO, U32 u32PadMode);
extern U8    MHal_GPIO_Pad_Oen(U8 u8IndexGPIO);
extern U8    MHal_GPIO_Pad_Odn(U8 u8IndexGPIO);
extern U8    MHal_GPIO_Pad_Level(U8 u8IndexGPIO, U8 *u8PadLevel);
extern U8    MHal_GPIO_Pad_InOut(U8 u8IndexGPIO, U8 *u8PadInOut);
extern U8    MHal_GPIO_Pull_High(U8 u8IndexGPIO);
extern U8    MHal_GPIO_Pull_Low(U8 u8IndexGPIO);
extern U8    MHal_GPIO_Pull_Up(U8 u8IndexGPIO);
extern U8    MHal_GPIO_Pull_Down(U8 u8IndexGPIO);
extern U8    MHal_GPIO_Pull_Off(U8 u8IndexGPIO);
extern U8    MHal_GPIO_Pull_status(U8 u8IndexGPIO, U8 *u8PullStatus);
extern U8    MHal_GPIO_Set_High(U8 u8IndexGPIO);
extern U8    MHal_GPIO_Set_Low(U8 u8IndexGPIO);
extern U8    MHal_GPIO_Drv_Set(U8 u8IndexGPIO, U8 u8Level);
extern U8    MHal_GPIO_Drv_Get(U8 u8IndexGPIO, U8 *u8level);
extern int   MHal_GPIO_To_Irq(U8 u8IndexGPIO);
extern U8    MHal_GPIO_Get_CheckCount(void);
extern void *MHal_GPIO_Get_CheckInfo(U8 u8Index);
extern U8    MHal_GPIO_NameToNum(U8 *pu8Name, U8 *GpioIndex);
extern U8    MHal_GPIO_PadModeToVal(U8 *pu8Mode, U8 *u8Index);
extern int   GPI_GPIO_To_Irq(U8 u8IndexGPIO);
extern U32 * MHal_GPIO_PadModeToPadIndex(U32 u32Mode);

#endif // _MHAL_GPIO_H_
