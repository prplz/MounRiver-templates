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

#ifndef _CAM_DRV_I2C_H_
#define _CAM_DRV_I2C_H_
#include <cam_os_wrapper.h>
#include "drv_iic.h"
#if defined(__RTK_OS__)
#include "vm_types.ht"

#elif defined(__KERNEL__)
#include <linux/i2c.h>
typedef struct i2c_msg tI2cMsg;
#endif

typedef struct {
    s32 nPortNum;
    void *pAdapter;
}tI2cHandle;

s32 CamI2cOpen(tI2cHandle *pHandle, u8 nPortNum);
s32 CamI2cTransfer(tI2cHandle *pHandle, tI2cMsg *pMsg, u32 nMsgNum);
s32 CamI2cClose(tI2cHandle *pHandle);
s32 CamI2cSetClk(tI2cHandle *pHandle, u32 clk);

#endif // _CAM_DRV_I2C_H_

