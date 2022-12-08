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


#ifndef __DRV_BUS_I2C_H__
#define __DRV_BUS_I2C_H__

/*=============================================================*/
// Include files
/*=============================================================*/
#include "vm_types.ht"
#include "drv_i2c_common.h"
/*=============================================================*/
// Extern definition
/*=============================================================*/
//#define __USE_SW_I2C__

/*=============================================================*/
// Macro definition
/*=============================================================*/

/*=============================================================*/
// Data type definition
/*=============================================================*/

/*=============================================================*/
// Variable definition
/*=============================================================*/

/*=============================================================*/
// Global function definition
/*=============================================================*/
I2CErrorCode_e DrvI2cOpenHW(s32 *pHandle, I2COptions_t *pOption);
I2CErrorCode_e DrvI2cOpen(u32 nAddress, I2cClockSpeed_e eClkSpeed, bool bReStart, s32 *pHandle);
I2CErrorCode_e DrvI2cClose(s32 nHandle);
I2CErrorCode_e DrvI2cWrite(s32 nHandle, u8 nPortNum, void * pDataToWrite, u32 nNbBytes, bool bStopBit);
I2CErrorCode_e DrvI2cRead(s32 nHandle, u8 nPortNum, void * pDataToRead, u32 nNbBytes);
I2CErrorCode_e DrvI2cWriteAndRead(s32 nHandle, u8 nPortNum, void * pDataToWrite, u32 nNbBytesToWrite, void * pDataToRead, u32 nNbBytesToRead);
//I2CErrorCode_e DrvI2cOpenSW(u32 nAddress, I2cClockSpeed_e eClkSpeed, IoChipPadNum_e eScl, IoChipPadNum_e eSda, s32 *pHandle);
I2CErrorCode_e DrvI2cAddrWrite(s32 nHandle, u8 nPortNum, u16 nAddress, u16 nDataToWrite, bool bStopBit);
I2CErrorCode_e DrvI2cAddrRead(s32 nHandle, u8 nPortNum, u16 nAddress, u16 *pDataToRead, bool bStopBit);
I2CErrorCode_e DrvI2cSystemSuspend(void);
I2CErrorCode_e DrvI2cSystemResume(void);

#endif /* __DRV_BUS_I2C_H__*/

