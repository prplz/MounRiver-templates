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

#ifndef __DRV_I2C_COMMON_H__
#define __DRV_I2C_COMMON_H__

#include "vm_types.ht"

/****************************************************************************/
/*  I2C general constants definitions                                       */
/****************************************************************************/

/**
 * @brief Functions return values constants
 */
typedef enum
{
    I2C_PROC_DONE                =  0,
    I2C_ERROR                    = -1,
    I2C_BAD_PARAMETER            = -2,
    I2C_ALREADY_USED             = -3,
    I2C_UNKNOWN_HANDLE           = -4,
    I2C_HANDLE_NOT_OPENED        = -5,
    I2C_NO_MORE_HANDLE_FREE      = -6,
    I2C_HAL_NOT_SUPPORT          = -7,
    I2C_DRV_NOT_SUPPORT          = -8,
    I2C_POLLING_TIMEOUT          = -9,
    I2C_MAS_GDMA_ERROR           = -10,
    I2C_MAS_GDMA_POLLING_TIMEOUT = -11
}I2CErrorCode_e;

typedef enum
{
    I2C_ONE_BYTE_DATA = 0x0,
    I2C_TWO_BYTE_DATA = 0x1,
    I2C_UNKNOWN_BYTE_DATA = 0x2
}I2CDataLen_e;

typedef enum
{
    I2C_ONE_BYTE_ADDR = 0x0,
    I2C_TWO_BYTE_ADDR = 0x1,
    I2C_UNKNOWN_BYTE_ADDR = 0x2
}I2CAddrLen_e;


typedef enum
{
    I2C_CTRL_MODE_HW = 0x0,
    I2C_CTRL_MODE_SW = 0x1,
    I2C_CTRL_MODE_UNKNOWN = 0x2
}I2CControlMode_e;

typedef enum
{
    I2C_SEN_TYPE_OTHER = 0x0,
    I2C_SEN_TYPE_OMNI = 0x1,
    I2C_SEN_TYPE_UNKNOWN = 0x2
}I2CSensorType_e;

typedef enum
{
    I2C_HIGHSPEED_MODE          = 0x01,   ///< 1 MHz
    I2C_FAST_MODE               = 0x02,   ///< 400 kHz
    I2C_SLOW_MODE               = 0x82    ///< 100 kHz
} I2cClockSpeed_e;

typedef struct{
    u8                  nSlaveID;        ///< device address for this session
    u16                 nSpeed;

    //for DBB I2C
    bool                bReStart;        ///< support restart or not
    I2cClockSpeed_e     eClockSpeed;

    //for I2C MAS
    I2CControlMode_e    eControlMode;
    I2CSensorType_e     eSensorType;
    I2CDataLen_e        eDataLen;
    I2CAddrLen_e        eAddrLen;
} I2COptions_t;

#endif /* __DRV_I2C_COMMON_H__*/

