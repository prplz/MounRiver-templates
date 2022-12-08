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


#ifndef _DRV_SYSDESC_H_
#define _DRV_SYSDESC_H_

#include "device_id.h"
#include "property_id.h"

typedef enum
{
    E_SYS_DESC_PASS,
    E_SYS_DESC_MAGIC_ERROR,
    E_SYS_DESC_VERSION_ERROR,
    E_SYS_DESC_DEVICE_ID_ERROR,
    E_SYS_DESC_PROPERTY_ID_ERROR,
    E_SYS_DESC_OVERFLOW,
    E_SYS_DESC_CONTENT_ERROR,
    E_SYS_DESC_TOTALSIZE_ERROR,
    E_SYS_DESC_PROPERTY_EMPTY,
    E_SYS_DESC_CONTENT_LEN_ERROR,
    E_SYS_DESC_ELEMENT_COUNT_ERROR,
} enumSysDescErrCode;

enumSysDescErrCode MDrv_SysDesc_GetElemsOfSize(unsigned short u16DeviceID, unsigned short u16PropertyID, unsigned short *pu16GetData);
enumSysDescErrCode MDrv_SysDesc_GetContentOfLen(unsigned short u16DeviceID, unsigned short u16PropertyID, unsigned short *pu16GetData);
enumSysDescErrCode MDrv_SysDesc_GetElementCount(unsigned short u16DeviceID, unsigned short u16PropertyID, unsigned short *pu16Quotient, unsigned short *pu16Remainder);
enumSysDescErrCode MDrv_SysDesc_Read_U8(unsigned short u16DeviceID, unsigned short u16PropertyID, unsigned char *pu8GetData);
enumSysDescErrCode MDrv_SysDesc_Read_U16(unsigned short u16DeviceID, unsigned short u16PropertyID, unsigned short *pu16GetData);
enumSysDescErrCode MDrv_SysDesc_Read_U32(unsigned short u16DeviceID, unsigned short u16PropertyID, unsigned int *pu32GetData);
enumSysDescErrCode MDrv_SysDesc_Read_U64(unsigned short u16DeviceID, unsigned short u16PropertyID, unsigned long long *pu64GetData);
enumSysDescErrCode MDrv_SysDesc_Read_U8_Array(unsigned short u16DeviceID, unsigned short u16PropertyID, unsigned char *pu8GetData, unsigned short u16Length);
enumSysDescErrCode MDrv_SysDesc_Read_U16_Array(unsigned short u16DeviceID, unsigned short u16PropertyID, unsigned short *pu16GetData, unsigned short u16Length);
enumSysDescErrCode MDrv_SysDesc_Read_U32_Array(unsigned short u16DeviceID, unsigned short u16PropertyID, unsigned int *pu32GetData, unsigned short u16Length);
enumSysDescErrCode MDrv_SysDesc_Read_U64_Array(unsigned short u16DeviceID, unsigned short u16PropertyID, unsigned long long *pu64GetData, unsigned short u16Length);
enumSysDescErrCode MDrv_SysDesc_Read_MultiTypes(unsigned short u16DeviceID, unsigned short u16PropertyID, void *pvGetData, unsigned short u16DataTypeSize, unsigned short u16Length);
enumSysDescErrCode MDrv_SysDesc_Read_String(unsigned short u16DeviceID, unsigned short u16PropertyID, const char *pGetString, unsigned short u16Length);
#endif

