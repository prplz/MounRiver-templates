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

#include <stdio.h>
#include <stdlib.h>
#include "vm_types.ht"
#include "mdrv_types.h"
#include "mdrv_puse.h"
#include "gpio.h"
#include "padmux.h"
#include "drv_sysdesc.h"
#include "device_id.h"
#include "property_id.h"
#include "cam_os_wrapper.h"
#include "initcall.h"
#include "hal_padmux.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define PADINFO_ENABLE                  0

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if PADINFO_ENABLE
    #define PAD_PRINT(x, args...)           CamOsPrintf(x, ##args)
#else
    #define PAD_PRINT(x, args...)
#endif

#define PADINFO_NAME    "schematic"

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static int                      _nPad = 0;
static pad_info_t*              _pPadInfo = NULL;

//-------------------------------------------------------------------------------------------------
//  Implementation
//-------------------------------------------------------------------------------------------------
int mdrv_padmux_active(void)
{
    return (_pPadInfo) ? 1: 0;
}

int mdrv_padmux_getpad(int puse)
{
    int i;

    if (MDRV_PUSE_NA == puse)
        return PAD_UNKNOWN;

    for (i = 0; i < _nPad; i++)
    {
        if (_pPadInfo[i].u32Puse == puse)
            return _pPadInfo[i].u32PadId;
    }
    return PAD_UNKNOWN;
}

int mdrv_padmux_getmode(int puse)
{
    int i;

    if (MDRV_PUSE_NA == puse)
        return PINMUX_FOR_UNKNOWN_MODE;

    for (i = 0; i < _nPad; i++)
    {
        if (_pPadInfo[i].u32Puse == puse)
            return _pPadInfo[i].u32Mode;
    }
    return PINMUX_FOR_UNKNOWN_MODE;
}

int mdrv_padmux_getpuse(int IP_Index, int Channel_Index, int Pad_Index)
{
    int puse;
    puse = (IP_Index) + (Channel_Index << 8) + (Pad_Index);
    return puse;
}

void _mhal_padmux_size(int *PadmuxSize)
{
    *PadmuxSize = size_of_schematic / sizeof(pad_info_t);
}

void _mhal_padmux(pad_info_t *PadInfo)
{
    int i = 0;
    int nPadmuxSize = 0;
    _mhal_padmux_size(&nPadmuxSize);
    for(i=0; i < nPadmuxSize; i++)
    {
        *PadInfo ++ = schematic[i];
    }
}

extern int MDrv_GPIO_PadVal_Set(U8 u8IndexGPIO, U32 u32PadMode);
static int _mdrv_padmux_dts(void)
{
#ifdef SYSDESC_DEV_padmux
    enumSysDescErrCode ret;
    U16 u16elemSize;
    U16 u16contentOfLen;
    U16 u16elemCount;
    U16 u16Remainder;
    U8 i;
#if defined(SYSDESC_PRO_status_u8)
    if (E_SYS_DESC_PASS != MDrv_SysDesc_Read_U8_Array(SYSDESC_DEV_padmux, SYSDESC_PRO_status_u8, &i, 1) || !i)
    {
        PAD_PRINT("[%s][padmux] sysdesc no enable!\n", __func__);
        return -1;
    }
#else
    PAD_PRINT("[%s][padmux] sysdesc no enable!\n", __func__);
    return -1;
#endif

    ret = MDrv_SysDesc_GetElemsOfSize(SYSDESC_DEV_padmux,SYSDESC_PRO_schematic_u32_u32_u32,&u16elemSize);
    if(ret)
    {
        PAD_PRINT("[%s][%d] invalid dts of GetElemsOfSize,code:[%d]\n", __FUNCTION__, __LINE__, ret);
        return -1;
    }

    ret = MDrv_SysDesc_GetContentOfLen(SYSDESC_DEV_padmux,SYSDESC_PRO_schematic_u32_u32_u32,&u16contentOfLen);
    if(ret)
    {
        PAD_PRINT("[%s][%d] invalid dts of GetContentOfLen,code:[%d]\n", __FUNCTION__, __LINE__, ret);
        return -1;
    }

    ret = MDrv_SysDesc_GetElementCount(SYSDESC_DEV_padmux,SYSDESC_PRO_schematic_u32_u32_u32,&u16elemCount,&u16Remainder);
    if(ret)
    {
        PAD_PRINT("[%s][%d] invalid dts of GetElementCount,code:[%d]\n", __FUNCTION__, __LINE__, ret);
        return -1;
    }

    if (NULL == (_pPadInfo = malloc(u16elemCount*sizeof(pad_info_t))))
    {
        PAD_PRINT("[%s][%d] malloc fail\n", __FUNCTION__, __LINE__);
        return -1;
    }

    ret = MDrv_SysDesc_Read_MultiTypes(SYSDESC_DEV_padmux,SYSDESC_PRO_schematic_u32_u32_u32,_pPadInfo,u16elemSize,u16contentOfLen);
    if(ret)
    {
        PAD_PRINT("[%s][%d] of_property_read_u32_array fail,code:[%d]\n", __FUNCTION__, __LINE__,ret);
        free(_pPadInfo);
        _pPadInfo = NULL;
        return -1;
    }
    _nPad = u16elemCount;

#else
    _mhal_padmux_size(&_nPad);
    _pPadInfo = malloc(_nPad * sizeof(pad_info_t));
    _mhal_padmux(_pPadInfo);
#endif

#if 1
    if (_nPad > 0)
    {
        int i;
        PAD_PRINT("[%s][%d] *******************************\n", __FUNCTION__, __LINE__);
        for (i = 0; i < _nPad; i++)
        {
            PAD_PRINT("[%s][%d] (PadId, Mode, Puse) = (%d, 0x%02x, 0x%08x)\n", __FUNCTION__, __LINE__,
                _pPadInfo[i].u32PadId,
                _pPadInfo[i].u32Mode,
                _pPadInfo[i].u32Puse);
            MDrv_GPIO_PadVal_Set((U8)_pPadInfo[i].u32PadId & 0xFF, _pPadInfo[i].u32Mode);
        }
        PAD_PRINT("[%s][%d] *******************************\n", __FUNCTION__, __LINE__);
    }
#endif
    return 0;
}

int mdrv_padmux_exist(void)
{
    PAD_PRINT("%s\r\n", __FUNCTION__);
    if (_pPadInfo) {
        free(_pPadInfo);
        _pPadInfo = NULL;
    }
    return 0;
}

void mdrv_padmux_init(void)
{
    PAD_PRINT("%s\r\n", __FUNCTION__);
    _mdrv_padmux_dts();
}

rtos_device_initcall(mdrv_padmux_init);

