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

#include "cam_drv_i2c.h"
#include "drv_iic.h"

s32 CamI2cOpen(tI2cHandle *pHandle, u8 nPortNum)
{
#if defined(__RTK_OS__)
    pHandle->nPortNum = nPortNum;

#elif defined(__KERNEL__)
    pHandle->pAdapter = (void *)i2c_get_adapter(nPortNum);

#endif
    return 0;
}

s32 CamI2cTransfer(tI2cHandle *pHandle, tI2cMsg *pMsg, u32 nMsgNum)
{
#if defined(__RTK_OS__)
    extern s32 i2c_transfer(u8 u8Port, tI2cMsg *pmsg, s32 length);
    return i2c_transfer(pHandle->nPortNum, pMsg, nMsgNum);

#elif defined(__KERNEL__)
    return i2c_transfer((struct i2c_adapter *)pHandle->pAdapter, pMsg, nMsgNum);

#endif
}

s32 CamI2cClose(tI2cHandle *pHandle)
{
#if defined(__RTK_OS__)
    pHandle->nPortNum = (-1);

#elif defined(__KERNEL__)

#endif
    return 0;
}

s32 CamI2cSetClk(tI2cHandle *pHandle, u32 clk)
{
    //ToDo
#if defined(__RTK_OS__)
    sstar_i2c_set_speed(pHandle->nPortNum, clk);
#else
#endif
    return 0;
}

