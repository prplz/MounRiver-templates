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


#include <irqs.h>
#include <cam_os_wrapper.h>
#include <drv_camclk_Api.h>
#ifdef CONFIG_SYSDESC_SUPPORT
#include <device_id.h>
#include <property_id.h>
#include <drv_sysdesc.h>
#endif
#ifdef CONFIG_GPIO_SUPPORT
#include <mdrv_gpio.h>
#endif
#include "hal_iic.h"
#include "hal_iic_reg.h"
#include "hal_iic_cfg.h"
#include "drv_iic.h"
/*********debug mesg*********/

//#define DMSG_I2C_DRIVER_DEBUG
#define dmsg_i2c_drverr(fmt, ...)                                         \
    do                                                                    \
    {                                                                     \
        CamOsPrintf("[drv_i2c_err] <%s>, " fmt, __func__, ##__VA_ARGS__); \
    } while (0)
#ifdef DMSG_I2C_DRIVER_DEBUG
#define dmsg_i2c_drvwarn(fmt, ...)                                         \
    do                                                                     \
    {                                                                      \
        CamOsPrintf("[drv_i2c_warn] <%s>, " fmt, __func__, ##__VA_ARGS__); \
    } while (0)
#else
#define dmsg_i2c_drvwarn(fmt, ...)
#endif


/*******************************************************************/
/*                    value type                                   */
/*******************************************************************/
typedef struct _sstar_iic
{
    u32                u32IrqNum;
    u8                 u8IrqName[20];
    u8                 u8Padmod;
    ST_HAL_I2C_BASE    stHalCtrl;
    CamOsTsem_t        stTsemID;
    CamOsMutex_t       stMutexID;
    #ifdef CONFIG_SYSDESC_SUPPORT
    unsigned short     u16DeviceId;
    #endif
    #ifdef CONFIG_CAM_CLK
    void              *pvI2cClk;
    #endif
    const struct i2c_config  *pstConfig;
} ST_I2C_CTRL;

/*******************************************************************/
/*                    global value definition                      */
/*******************************************************************/
static ST_I2C_CTRL  *aI2CHandle[I2C_BUSNUM] = {0};

#define MIIC_DMA_TSEM_TIMEOUT (10000) // millseconds

/*******************************************************************/
/*                    function definition                          */
/*******************************************************************/

static s32 sstar_i2c_ctrl_set(u8 Group);
#ifndef CONFIG_CAM_CLK
static s32 sstar_i2c_srclk_en(void *para_i2c_base, bool enable);
#endif
static s32 sstar_i2c_padmod_set(ST_I2C_CTRL *para_i2c_ctrl);


/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
#define I2C_DMA_BUF_LEN  (4096)
s32 sstar_i2c_init(u8 Group)
{
    u8               u8Enable = 1;
    s32              s32Ret = 0;
    ST_HAL_DMA_ADDR *pstDmaAddr = NULL;
    ST_I2C_CTRL     *pstI2cCtrl = NULL;

    dmsg_i2c_drvwarn("for debug\n");

    if(Group > I2C_BUSNUM)
    {
        s32Ret = -1;
        dmsg_i2c_drverr("i2c-%d group greater than max bus number\n", Group);
        goto out;
    }
#ifdef CONFIG_SYSDESC_SUPPORT
    if(stI2cConfig[Group].dev_id == SYSDESC_DEV_NO_DEFINE)
    {
        u8Enable = 0;
    }
#ifdef SYSDESC_PRO_status_u8
    if (E_SYS_DESC_PASS != MDrv_SysDesc_Read_U8(stI2cConfig[Group].dev_id, SYSDESC_PRO_status_u8, &u8Enable))
    {
        u8Enable = 0;
    }
#else
    u8Enable = 0;
#endif
#else
    if(!stI2cConfig[Group].enable)
    {
        u8Enable = 0;
    }
#endif

    if(!u8Enable)
    {
        s32Ret = 0;
        goto out;
    }

    s32Ret = sstar_i2c_ctrl_set(Group);
    if (s32Ret < 0)
    {
        dmsg_i2c_drverr("i2c-%d driver config set failed\n", Group);
        goto out;
    }

    pstI2cCtrl = aI2CHandle[Group];

    pstDmaAddr = &pstI2cCtrl->stHalCtrl.stI2cDmaCtrl.stDmaMiuAdr;

    if (pstI2cCtrl->stHalCtrl.u32EnDma)
    {
        pstDmaAddr->pu8DmaAdrVirtu = (u8 *)CamOsMemAlloc(I2C_DMA_BUF_LEN);
        pstDmaAddr->u64DmaAdrPhy   = (u64)CamOsMemVirtToPhys((void *)pstDmaAddr->pu8DmaAdrVirtu);
        pstDmaAddr->u64DmaAdrMiu   = (u64)(u32)CamOsMemPhysToMiu(pstDmaAddr->u64DmaAdrPhy);
        dmsg_i2c_drvwarn("<<<<<<<<<<<<<<<IIC DMA VIRT ADDR IS %p, PHYS ADDR IS %#llx, MIU ADDR IS: %#llx\n",
                        pstDmaAddr->pu8DmaAdrVirtu, pstDmaAddr->u64DmaAdrPhy,
                             pstDmaAddr->u64DmaAdrMiu);
    }
    s32Ret = HAL_I2C_Init(&pstI2cCtrl->stHalCtrl);
    sstar_i2c_padmod_set(pstI2cCtrl);
out:
    return s32Ret;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
s32 sstar_i2c_deinit(u8 Group)
{
    ST_HAL_DMA_ADDR *pstDmaAddr;
    ST_I2C_CTRL *    pstI2cCtrl = aI2CHandle[Group];

    pstDmaAddr = &pstI2cCtrl->stHalCtrl.stI2cDmaCtrl.stDmaMiuAdr;
    if (pstI2cCtrl->stHalCtrl.u32EnDma)
    {
        CamOsMemRelease(pstDmaAddr->pu8DmaAdrVirtu);
        pstDmaAddr->u64DmaAdrMiu = 0;
        pstDmaAddr->u64DmaAdrPhy = 0;
    }
    CamOsTsemDeinit(&pstI2cCtrl->stTsemID);

    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
s32 sstar_i2c_master_xfer(u8 Group, struct i2c_msg *para_msg, s32 para_num)
{
    s32             s32Ret;
    s32             l_num;
    ST_I2C_CTRL *   pstI2cCtrl = aI2CHandle[Group];
    struct i2c_msg *pstI2cMsg  = para_msg;

    if (!pstI2cCtrl)
    {
        s32Ret = sstar_i2c_init(Group);
        if (s32Ret)
        {
            dmsg_i2c_drverr("can not transfer i2c-%d,init failed\n", Group);
            return -1;
        }
        else
        {
            pstI2cCtrl = aI2CHandle[Group];
        }
    }

#ifdef CONFIG_CAM_CLK
    CamClkSetOnOff(pstI2cCtrl->pvI2cClk, 1);
#else
    sstar_i2c_srclk_en(&pstI2cCtrl->stHalCtrl, true);
#endif

    CamOsMutexLock(&pstI2cCtrl->stMutexID);

    HAL_I2C_RstSet(&pstI2cCtrl->stHalCtrl, 1);
    HAL_I2C_RstSet(&pstI2cCtrl->stHalCtrl, 0);
    for (l_num = 0; l_num < para_num; l_num++, pstI2cMsg++)
    {
        dmsg_i2c_drvwarn("para number from user is : %d,flags is : 0x%x\n", para_num, pstI2cMsg->flags);

        if (pstI2cCtrl->stHalCtrl.u32EnDma)
        {
            if (!(pstI2cMsg->flags & CAM_I2C_STOP_BEFORE_RESTART) && (para_num > 1))
            {
                HAL_I2C_DmaStopFmt(&pstI2cCtrl->stHalCtrl, ((pstI2cMsg->flags) & CAM_I2C_RD));
            }
            else
            {
                HAL_I2C_DmaStopFmt(&pstI2cCtrl->stHalCtrl, 1);
            }
        }

        if ((pstI2cMsg->flags) & CAM_I2C_RD)
        {
            dmsg_i2c_drvwarn("IN READ \n");
            s32Ret |= HAL_I2C_Read(&pstI2cCtrl->stHalCtrl, pstI2cMsg->addr, pstI2cMsg->buf, (u32)pstI2cMsg->len);
        }
        else
        {
            dmsg_i2c_drvwarn("IN WRITE \n");
            s32Ret |= HAL_I2C_Write(&pstI2cCtrl->stHalCtrl, pstI2cMsg->addr, pstI2cMsg->buf, (u32)pstI2cMsg->len);
        }

        if (pstI2cMsg->flags & CAM_I2C_STOP_BEFORE_RESTART)
            HAL_I2C_Release(&pstI2cCtrl->stHalCtrl);

        if (s32Ret)
        {
            break;
        }
    }

    HAL_I2C_Release(&pstI2cCtrl->stHalCtrl);
    CamOsMutexUnlock(&pstI2cCtrl->stMutexID);

#ifdef CONFIG_CAM_CLK
    CamClkSetOnOff(pstI2cCtrl->pvI2cClk, 0);
#else
    sstar_i2c_srclk_en(&pstI2cCtrl->stHalCtrl, false);
#endif
    if (s32Ret)
    {
        dmsg_i2c_drverr("ERR xfer \n");
        para_num = 0;
    }
    else
    {
        dmsg_i2c_drvwarn("OK return xfer\n");
    }
    return para_num;
}

static void drv_i2c_interrupt(u32 nIrq, void *pI2cCtrl)
{
    s32            s32Ret;
    ST_I2C_CTRL *  pstI2cCtrl;

    pstI2cCtrl = (ST_I2C_CTRL *)pI2cCtrl;

    s32Ret = HAL_I2C_DmaTrDone(&pstI2cCtrl->stHalCtrl, true);
    if (s32Ret)
    {
        CamOsTsemUp(&(pstI2cCtrl->stTsemID));
    }

    //return 0;
}

#ifdef CONFIG_CAM_CLK
/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 sstar_i2c_set_srclk(void *para_i2c_base, u32 u32SrcClk)
{
    s32                  s32Ret = 0;
    u8                   u8CamclkId;
    CAMCLK_Set_Attribute stSetCfg;
    char                 camclk_name[32];
    ST_I2C_CTRL         *pstI2cCtrl;
    ST_HAL_I2C_BASE     *pstI2cBase;

    pstI2cBase = (ST_HAL_I2C_BASE *)para_i2c_base;

    if ((!pstI2cBase) || (pstI2cBase->u32Group > I2C_BUSNUM))
    {
        dmsg_i2c_drverr("parameter illegal\n");
        s32Ret = -1;
        goto err_out;
    }

    pstI2cCtrl = aI2CHandle[pstI2cBase->u32Group];

    if (!snprintf(camclk_name, sizeof(camclk_name), "i2c%d_camclk", pstI2cBase->u32Group))
    {
        dmsg_i2c_drverr("camclk name reformat failed, group:%d!\n", pstI2cBase->u32Group);
        s32Ret = -1;
        goto err_out;
    }
#ifdef CONFIG_SYSDESC_SUPPORT
#ifdef SYSDESC_PRO_camclk_u16
    MDrv_SysDesc_Read_U8_Array(pstI2cCtrl->u16DeviceId, SYSDESC_PRO_camclk_u16, &u8CamclkId, 1);
#else
    u8CamclkId = CAMCLK_VOID;
#endif
    if (!u8CamclkId)
    {
        dmsg_i2c_drverr( "Fail to get clk!\n");
        s32Ret = -1;
        goto err_out;
    }
#else
    u8CamclkId = aI2CHandle.pstConfig->camclk_id;
#endif
    if (CamClkRegister((u8 *)camclk_name, (u32)u8CamclkId, &pstI2cCtrl->pvI2cClk) == CAMCLK_RET_OK)
    {
        CAMCLK_SETRATE_ROUNDUP(stSetCfg, u32SrcClk);
        CamClkAttrSet(pstI2cCtrl->pvI2cClk, &stSetCfg);
        //CamClkSetOnOff(pstI2cCtrl->pvI2cClk, 0);
    }

err_out:
    return s32Ret;
}

#else
/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 sstar_i2c_set_srclk(void *para_i2c_base, u32 u32SrcClk)
{
    s32 s32Ret = 0;
    u8  u8ClkgenSel = CLKGEN_I2C_SEL_12M;
    ST_HAL_I2C_BASE *pstI2cBase = (ST_HAL_I2C_BASE *)para_i2c_base;
    const struct reg_t  *stClkgenMsg = NULL;
    ST_I2C_CTRL     *pstI2cCtrl = NULL;

    dmsg_i2c_drvwarn("i2c\n");

    if ((!pstI2cBase) || (pstI2cBase->u32Group > I2C_BUSNUM))
    {
        dmsg_i2c_drverr("parameter illegal\n");
        s32Ret = -1;
        goto err_out;
    }
    pstI2cCtrl  = aI2CHandle[pstI2cBase->u32Group];
    if (!pstI2cCtrl)
    {
        s32Ret = -1;
        goto err_out;
    }
    stClkgenMsg = &(pstI2cCtrl->pstConfig->pst_reg->reg_clkgen);
    dmsg_i2c_drvwarn("stClkgenMsg: %p\n", stClkgenMsg);
    switch (u32SrcClk)
    {
        case DRV_I2C_SRCCLK_12M:
            u8ClkgenSel = CLKGEN_I2C_SEL_12M;
            break;
        case DRV_I2C_SRCCLK_54M:
            u8ClkgenSel = CLKGEN_I2C_SEL_54M;
            break;
        case DRV_I2C_SRCCLK_72M:
            u8ClkgenSel = CLKGEN_I2C_SEL_72M;
            break;
        default :
            dmsg_i2c_drverr("i2c source clk not support\n");
            s32Ret = -1;
            break;
    }

    if (!s32Ret)
    {
        WRITE_WORD_MASK((stClkgenMsg->bank_base + (stClkgenMsg->reg_offset << 2)),
                        u8ClkgenSel << stClkgenMsg->bit_shift, stClkgenMsg->bit_mask);
    }

err_out:
    return s32Ret;

}
static s32 sstar_i2c_srclk_en(void *para_i2c_base, bool enable)
{
    s32 s32Ret = 0;
    ST_HAL_I2C_BASE *pstI2cBase = (ST_HAL_I2C_BASE *)para_i2c_base;
    struct reg_t *stClkgenMsg = NULL;
    ST_I2C_CTRL     *pstI2cCtrl = NULL;

    dmsg_i2c_drvwarn("i2c\n");

    if ((!pstI2cBase) || (pstI2cBase->u32Group > I2C_BUSNUM))
    {
        dmsg_i2c_drverr("parameter illegal\n");
        s32Ret = -1;
        goto err_out;
    }

    pstI2cCtrl  = aI2CHandle[pstI2cBase->u32Group];
    stClkgenMsg = &pstI2cCtrl->pstConfig->pst_reg->reg_clkgen;

    if (enable)
    {
        WRITE_WORD_MASK((stClkgenMsg->bank_base + (stClkgenMsg->reg_offset << 2)),
                        (0 << stClkgenMsg->bit_shift), 0x01 << stClkgenMsg->bit_shift);
    }
    else
    {
        WRITE_WORD_MASK((stClkgenMsg->bank_base + (stClkgenMsg->reg_offset << 2)),
                        (1 << stClkgenMsg->bit_shift), 0x01 << stClkgenMsg->bit_shift);
    }
err_out:
    return s32Ret;
}
#endif


#if defined(CONFIG_PADMUX_SUPPORT)
static s32 sstar_i2c_padmod_set(ST_I2C_CTRL *para_i2c_ctrl)
{
    dmsg_i2c_drvwarn("please set padmod in xx.sys or xx-padmux.c\n");
    return 0;
}

#else
static s32 sstar_i2c_padmod_set(ST_I2C_CTRL *para_i2c_ctrl)
{
    ST_I2C_CTRL *pstI2cCtrl    = para_i2c_ctrl;

    #ifdef CONFIG_GPIO_SUPPORT
    MDrv_GPIO_PadGroupMode_Set(pstI2cCtrl->u8Padmod);
    #else
    struct reg_t *padmux_reg = &para_i2c_ctrl->pstConfig->pst_reg->reg_padmod;

    WRITE_WORD_MASK(padmux_reg->bank_base + (padmux_reg->reg_offset << 2),
                            pstI2cCtrl->u8Padmod << padmux_reg->bit_shift,
                            padmux_reg->bit_mask);
    #endif
    return 0;
}
#endif

s32 sstar_i2c_dma_callback(void *para_i2c_base)
{
    ST_HAL_I2C_BASE *pstI2cBase = (ST_HAL_I2C_BASE *)para_i2c_base;
    ST_I2C_CTRL *pstI2cCtrl     = NULL;//aI2CHandle[pstI2cHal->u8Group];
    CamOsRet_e CamRet           = CAM_OS_OK;

    pstI2cCtrl = aI2CHandle[pstI2cBase->u32Group];
    HAL_I2C_SetTriggerReg(para_i2c_base);

    CamRet = CamOsTsemTimedDown(&pstI2cCtrl->stTsemID, MIIC_DMA_TSEM_TIMEOUT);
    if (CAM_OS_TIMEOUT == CamRet)
    {
        dmsg_i2c_drverr("i2c-%d DMA TIMEOUT!\n", pstI2cBase->u32Group);
        return -E_ERR_TIMEOUT;
    }
    else if (CAM_OS_FAIL == CamRet)
    {
        dmsg_i2c_drverr("i2c-%d DMA CAM_OS_FAIL!\n", pstI2cBase->u32Group);
        return -E_ERR;
    }
    else
    {
        dmsg_i2c_drvwarn("i2c-%d DMA DONE!\n", pstI2cBase->u32Group);
    }

    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 sstar_i2c_ctrl_set(u8 Group)
{
    ST_I2C_CTRL *    pstI2cCtrl   = NULL;
    u8               u8I2cGoup    = 0;
    s32              s32Ret = 0;
#ifdef CONFIG_SYSDESC_SUPPORT
    u32              u32BankBase  = 0;
    u8               u8IrqNum     = 0;
    u32              u32Speed     = 0;
    u16              u16tHD       = 0;
    u16              u16tSU       = 0;
    u16              u16tStart    = 0;
    u16              u16tStop     = 0;
    u8               u8Padmod     = 0;
    u8               u8EnDma      = 0;
    u8               u8PushPull   = 0;
    u8               u8OenCnt     = 0;
#endif
    dmsg_i2c_drvwarn("i2c-%d\n", Group);
    if (I2C_BUSNUM <= Group)
    {
        dmsg_i2c_drverr("do not support i2c-%d\n", Group);
        return -1;
    }

    u8I2cGoup = Group;

    pstI2cCtrl = (ST_I2C_CTRL *)CamOsMemAlloc(sizeof(ST_I2C_CTRL));
    if (!pstI2cCtrl)
    {
        dmsg_i2c_drverr("CamOsMemAlloc failed, group:%d!\n", u8I2cGoup);
        s32Ret = -1;
        goto err_out;
    }
    else
    {
        aI2CHandle[u8I2cGoup] = pstI2cCtrl;
    }

    dmsg_i2c_drvwarn("i2c-%d CamOsMemAlloc success!\n", u8I2cGoup);

    /*master mode and base addr*/
    pstI2cCtrl->stHalCtrl.u32Group                    = u8I2cGoup;

    pstI2cCtrl->pstConfig = &stI2cConfig[u8I2cGoup];
    dmsg_i2c_drvwarn("i2c-%d, pstI2cCtrl->pstConfig = 0x%x\n", u8I2cGoup, pstI2cCtrl->pstConfig);
#ifdef CONFIG_SYSDESC_SUPPORT
    pstI2cCtrl->u16DeviceId = stI2cConfig[u8I2cGoup].dev_id;

#ifdef SYSDESC_PRO_reg_u32_u16
    s32Ret = MDrv_SysDesc_Read_U32_Array(pstI2cCtrl->u16DeviceId, SYSDESC_PRO_reg_u32_u16, &u32BankBase, 1);
#else
    s32Ret = E_SYS_DESC_PROPERTY_ID_ERROR;
#endif
    if (s32Ret)
    {
        dmsg_i2c_drverr("can`t get BankBase property in SysDesc\n");
        goto err_out;
    }
    pstI2cCtrl->stHalCtrl.u64BankBase = (u64)u32BankBase;

#ifdef SYSDESC_PRO_interrupts_u8
    s32Ret = MDrv_SysDesc_Read_U8_Array(pstI2cCtrl->u16DeviceId, SYSDESC_PRO_interrupts_u8, &u8IrqNum, 1);
#else
    s32Ret = E_SYS_DESC_PROPERTY_ID_ERROR;
#endif
    if (s32Ret)
    {
        dmsg_i2c_drverr("can`t get interrupt property in SysDesc\n");
        goto err_out;
    }
    pstI2cCtrl->u32IrqNum = (u32)u8IrqNum;

#ifdef SYSDESC_PRO_speed_u32
    s32Ret = MDrv_SysDesc_Read_U32_Array(pstI2cCtrl->u16DeviceId, SYSDESC_PRO_speed_u32, &u32Speed, 1);
    if (s32Ret)
    {
        dmsg_i2c_drvwarn("can`t get speed property in SysDesc\n");
        u32Speed = I2C_DEFAULT_SPEED;
    }
#else
    dmsg_i2c_drvwarn("can`t get speed property in SysDesc\n");
    u32Speed = I2C_DEFAULT_SPEED;
#endif
    pstI2cCtrl->stHalCtrl.u32Speed = u32Speed;

#ifdef SYSDESC_PRO_padmux_u8
    s32Ret = MDrv_SysDesc_Read_U8_Array(pstI2cCtrl->u16DeviceId, SYSDESC_PRO_padmux_u8, &u8Padmod, 1);
    if (s32Ret)
    {
        dmsg_i2c_drvwarn("can`t get padmod property in SysDesc\n");
        u8Padmod = 0;
    }
#else
    dmsg_i2c_drvwarn("can`t get padmod property in SysDesc\n");
    u8Padmod = 0;
#endif
    pstI2cCtrl->u8Padmod = u8Padmod;

#ifdef SYSDESC_PRO_dma_u8
    s32Ret = MDrv_SysDesc_Read_U8_Array(pstI2cCtrl->u16DeviceId, SYSDESC_PRO_dma_u8, &u8EnDma, 1);
    if (s32Ret)
    {
        dmsg_i2c_drvwarn("can`t get dma property in SysDesc, set 0\n");
        u8EnDma = 0;
    }
#else
    dmsg_i2c_drvwarn("can`t get dma property in SysDesc, set 0\n");
    u8EnDma = 0;
#endif
    pstI2cCtrl->stHalCtrl.u32EnDma = u8EnDma;

#ifdef SYSDESC_PRO_pushen_u8
    s32Ret = MDrv_SysDesc_Read_U8_Array(pstI2cCtrl->u16DeviceId, SYSDESC_PRO_pushen_u8, &u8PushPull, 1);
    if (s32Ret)
    {
        dmsg_i2c_drvwarn("can`t get pushpull property in SysDesc,set 0\n");
        u8PushPull = 0;
    }
#else
    dmsg_i2c_drvwarn("can`t get pushpull property in SysDesc,set 0\n");
    u8PushPull = 0;
#endif
    pstI2cCtrl->stHalCtrl.u8PushPull = u8PushPull;

#ifdef SYSDESC_PRO_oencnt_u8
    s32Ret = MDrv_SysDesc_Read_U8_Array(pstI2cCtrl->u16DeviceId, SYSDESC_PRO_oencnt_u8, &u8OenCnt, 1);
    if (s32Ret)
    {
        dmsg_i2c_drvwarn("can`t get dma property in SysDesc, set 0\n");
        u8OenCnt = 0;
    }
#else
    dmsg_i2c_drvwarn("can`t get dma property in SysDesc, set 0\n");
    u8OenCnt = 0;
#endif
    pstI2cCtrl->stHalCtrl.u8OenCnt = u8OenCnt;

#ifdef SYSDESC_PRO_thd_u16
    s32Ret = MDrv_SysDesc_Read_U16_Array(pstI2cCtrl->u16DeviceId, SYSDESC_PRO_thd_u16, &u16tHD, 1);
    if (s32Ret)
    {
        dmsg_i2c_drvwarn("can`t get tHD property in SysDesc, set 0\n");
        u16tHD = 0;
    }
#else
    dmsg_i2c_drvwarn("can`t get tHD property in SysDesc, set 0\n");
    u16tHD = 0;
#endif
    pstI2cCtrl->stHalCtrl.stI2cClkCnt.u16tHD = u16tHD;

#ifdef SYSDESC_PRO_tsu_u16
    s32Ret = MDrv_SysDesc_Read_U16_Array(pstI2cCtrl->u16DeviceId, SYSDESC_PRO_tsu_u16, &u16tSU, 1);
    if (s32Ret)
    {
        dmsg_i2c_drvwarn("can`t get tSU property in SysDesc, set 0\n");
        u16tSU = 0;
    }
#else
    dmsg_i2c_drvwarn("can`t get tSU property in SysDesc, set 0\n");
    u16tSU = 0;
#endif
    pstI2cCtrl->stHalCtrl.stI2cClkCnt.u16tSU = u16tSU;

#ifdef SYSDESC_PRO_start_u16
    s32Ret = MDrv_SysDesc_Read_U16_Array(pstI2cCtrl->u16DeviceId, SYSDESC_PRO_start_u16, &u16tStart, 1);
    if (s32Ret)
    {
        dmsg_i2c_drvwarn("can`t get t-start property in SysDesc, set 0\n");
        u16tStart = 0;
    }
#else
    dmsg_i2c_drvwarn("can`t get t-start property in SysDesc, set 0\n");
    u16tStart = 0;
#endif
    pstI2cCtrl->stHalCtrl.stI2cClkCnt.u16CntForStart = u16tStart;

#ifdef SYSDESC_PRO_stop_u16
    s32Ret = MDrv_SysDesc_Read_U16_Array(pstI2cCtrl->u16DeviceId, SYSDESC_PRO_stop_u16, &u16tStop, 1);
    if (s32Ret)
    {
        dmsg_i2c_drvwarn("can`t get t-start property in SysDesc, set 0\n");
        u16tStop = 0;
    }
#else
    dmsg_i2c_drvwarn("can`t get t-start property in SysDesc, set 0\n");
    u16tStop = 0;
#endif
    pstI2cCtrl->stHalCtrl.stI2cClkCnt.u16CntForStp = u16tStop;

#else
    pstI2cCtrl->stHalCtrl.u64BankBase                = (u64)pstI2cCtrl->pstConfig->pst_reg->i2c_bank_base;
    dmsg_i2c_drvwarn("i2c-%d, u64BankBase = 0x%x\n", u8I2cGoup, pstI2cCtrl->stHalCtrl.u64BankBase);
    pstI2cCtrl->stHalCtrl.u32Group                    = Group;
    pstI2cCtrl->u8Padmod                             = (u8)pstI2cCtrl->pstConfig->padmod;
    pstI2cCtrl->u32IrqNum                            = (u32)pstI2cCtrl->pstConfig->irq;
    pstI2cCtrl->stHalCtrl.u8PushPull                 = (u8)pstI2cCtrl->pstConfig->push_pull;
    pstI2cCtrl->stHalCtrl.u8OenCnt                   = (u8)pstI2cCtrl->pstConfig->oen_cnt;
    pstI2cCtrl->stHalCtrl.u32EnDma                    = (u8)pstI2cCtrl->pstConfig->dma_en;
    pstI2cCtrl->stHalCtrl.u32Speed                   = (u32)pstI2cCtrl->pstConfig->speed;
    pstI2cCtrl->stHalCtrl.stI2cClkCnt.u16tHD         = (u16)pstI2cCtrl->pstConfig->t_hd;
    pstI2cCtrl->stHalCtrl.stI2cClkCnt.u16tSU         = (u16)pstI2cCtrl->pstConfig->t_su;
    pstI2cCtrl->stHalCtrl.stI2cClkCnt.u16CntForStart = (u16)pstI2cCtrl->pstConfig->t_start;
    pstI2cCtrl->stHalCtrl.stI2cClkCnt.u16CntForStp   = (u16)pstI2cCtrl->pstConfig->t_stop;
#endif

    pstI2cCtrl->stHalCtrl.calbak_i2c_set_srcclk      = sstar_i2c_set_srclk;
    pstI2cCtrl->stHalCtrl.calbak_i2c_dma_callback    = sstar_i2c_dma_callback;

    if (pstI2cCtrl->stHalCtrl.u32EnDma)
    {

        if (!snprintf((char *)pstI2cCtrl->u8IrqName, sizeof(pstI2cCtrl->u8IrqName), "i2c%d_Isr", u8I2cGoup))
        {
            dmsg_i2c_drverr("find irq reformat failed, group:%d!\n", u8I2cGoup);
            s32Ret = -1;
            goto err_out;
        }

        s32Ret = CamOsIrqRequest(pstI2cCtrl->u32IrqNum, drv_i2c_interrupt, "i2c%d_Isr", (void *)pstI2cCtrl);
        if (s32Ret == 0)
        {
            dmsg_i2c_drvwarn("i2c-%d irq registered sucess\n", u8I2cGoup);
        }
        else
        {
            dmsg_i2c_drverr("i2c-%d irq register failed,use polling\n", u8I2cGoup);
            s32Ret = -1;
            goto err_irq;
        }
    }
    CamOsTsemInit(&pstI2cCtrl->stTsemID, 0);
    return 0;
err_irq:
    CamOsIrqFree(pstI2cCtrl->u32IrqNum, (void *)pstI2cCtrl);
err_out:
    return s32Ret;
}

s32 sstar_i2c_set_speed(u8 Group, u32 speed)
{
    ST_I2C_CTRL *pstI2cCtrl = aI2CHandle[Group];

    if (!pstI2cCtrl)
    {
        dmsg_i2c_drverr("i2c-%d did not initial ever,set speed failed\n", Group);
        return -1;
    }
    pstI2cCtrl->stHalCtrl.u32Speed = speed;
    HAL_I2C_SrclkSelect(&pstI2cCtrl->stHalCtrl);
    HAL_I2C_CntSet(&pstI2cCtrl->stHalCtrl);

    return 0;
}
/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
#ifdef CONFIG_POWER_MANAG_SUPPORT
static s32 sstar_i2c_remove_srclk(ST_I2C_CTRL *I2cCtrl)
{

#ifdef CONFIG_CAM_CLK
    CamClkSetOnOff(I2cCtrl->pvI2cClk, 0);
    CamClkUnregister(I2cCtrl->pvI2cClk);
    I2cCtrl->pvI2cClk = NULL;
#else
    sstar_i2c_srclk_en(&I2cCtrl->stHalCtrl, false);
#endif
    return 0;
}

s32 sstar_i2c_suspend(void)
{
    ST_I2C_CTRL *pstI2cCtrl;
    u8 i;

    for (i = 0; i < I2C_BUSNUM; i++)
    {
        if (aI2CHandle[i])
        {
            pstI2cCtrl = aI2CHandle[i];
            CamOsIrqFree(pstI2cCtrl->u32IrqNum, (void *)pstI2cCtrl);
            CamOsTsemDeinit(&pstI2cCtrl->stHalCtrl.bDmaDetctMod);
            sstar_i2c_remove_srclk(pstI2cCtrl);
        }
    }
    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
s32 sstar_i2c_resume(void)
{
    ST_I2C_CTRL *pstI2cCtrl;
    s32          s32Ret;
    u8 i;

    for (i = 0; i < I2C_BUSNUM; i++)
    {
        if (aI2CHandle[i])
        {
            s32Ret = sstar_i2c_init(i);
            if (s32Ret)
            {
                dmsg_i2c_drverr("resume init i2c-%d failed\n", i);
            }
        }
    }

    return 0;
}

#endif // end #ifdef CONFIG_PM_SLEEP
