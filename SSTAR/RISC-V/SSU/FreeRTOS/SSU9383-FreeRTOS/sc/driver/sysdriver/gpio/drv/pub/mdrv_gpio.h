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

#ifndef _MDRV_GPIO_H_
#define _MDRV_GPIO_H_

#include "vm_types.ht"
#include "gpio.h"
#include "padmux.h"

typedef struct MDrv_PadCheck
{
    U16 base;
    U16 offset;
    U16 mask;
    U16 val;
    U16 regval;
} MDrv_PadCheckInfo;

typedef enum
{
    MDRV_PULL_UP = 0,
    MDRV_PULL_DOWN,
    MDRV_PULL_OFF,
} MDrv_PadPull;

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

void  MDrv_GPIO_Init(void);
void  MDrv_GPIO_Pad_Set(U8 u8IndexGPIO);
U8    MDrv_GPIO_PadGroupMode_Set(U32 u32PadMode);
U8    MDrv_GPIO_PadVal_Set(U8 u8IndexGPIO, U32 u32PadMode);
U8    MDrv_GPIO_PadVal_Get(U8 u8IndexGPIO, U32* u32PadMode);
void  MDrv_GPIO_VolVal_Set(U8 u8Group, U32 u32Mode);
U8    MDrv_GPIO_PadVal_Check(U8 u8IndexGPIO, U32 u32PadMode);
U8    MDrv_GPIO_Pad_Oen(U8 u8IndexGPIO);
U8    MDrv_GPIO_Pad_Odn(U8 u8IndexGPIO);
U8    MDrv_GPIO_Pad_Read(U8 u8IndexGPIO, U8* u8PadLevel);
U8    MDrv_GPIO_Pad_InOut(U8 u8IndexGPIO, U8* u8PadInOut);
U8    MDrv_GPIO_Pull_High(U8 u8IndexGPIO);
U8    MDrv_GPIO_Pull_Low(U8 u8IndexGPIO);
U8    MDrv_GPIO_Pull_Up(U8 u8IndexGPIO);
U8    MDrv_GPIO_Pull_Down(U8 u8IndexGPIO);
U8    MDrv_GPIO_Pull_Off(U8 u8IndexGPIO);
U8    MDrv_GPIO_Pull_Status(U8 u8IndexGPIO, U8* u8PullStatus);
U8    MDrv_GPIO_Set_High(U8 u8IndexGPIO);
U8    MDrv_GPIO_Set_Low(U8 u8IndexGPIO);
U8    MDrv_GPIO_Drv_Set(U8 u8IndexGPIO, U8 u8Level);
U8    MDrv_GPIO_Drv_Get(U8 u8IndexGPIO, U8* u8Level);
int   MDrv_GPIO_To_Irq(U8 u8IndexGPIO);
U8    MDrv_GPIO_Get_CheckCount(void);
void* MDrv_GPIO_Get_CheckInfo(U8 u8Index);
U8    MDrv_GPIO_NameToNum(U8* pu8Name, U8* GpioIndex);
U8    MDrv_GPIO_PadModeToVal(U8* pu8Mode, U8* u8Index);
U32*  MDrv_GPIO_PadModeToPadIndex(U32 u32Mode);

#endif // _MDRV_GPIO_H_
