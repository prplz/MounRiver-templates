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


#ifndef _HAL_IIC_H_
#define _HAL_IIC_H_

#include "cam_os_wrapper.h"

#define DRV_I2C_SRCCLK_72M (72000000)
#define DRV_I2C_SRCCLK_54M (54000000)
#define DRV_I2C_SRCCLK_12M (12000000)

#define DRV_I2C_CNT_DEC_12M (8)
#define DRV_I2C_CNT_DEC_54M (5)
#define DRV_I2C_CNT_DEC_72M (4)

#define DRV_I2C_SPEED_200KHZ  (200000)
#define DRV_I2C_SPEED_700KHZ  (700000)
#define DRV_I2C_SPEED_1500KHZ (1500000)

#define I2C_SET_WRITEBIT_INDATA (0xFE)
#define I2C_SET_READBIT_INDATA  (0x01)

#define I2C_DMA_TIMEOUT_MS (2000)

// if we open OEN enable,we set offset h0010 bit6,
// if we don`t set offset h0010 bit6,it SDA waveform is below
//       _
//      / |
// ____|  |___
// it we set offset h0010 bit6,it will take some time for waitting pull-up
// it will take some time between SDA push 1T between SDA low like below
//      __
//     |
// ____/('/' is the time)
//
#define OEN_WAIT_PUSHPULL_12M (3)
#define OEN_WAIT_PUSHPULL_54M (8)
#define OEN_WAIT_PUSHPULL_72M (11)

typedef enum _en_iic_hal_err
{
    E_ERR = 1,
    E_ERR_INIT,         // INIT ERR
    E_ERR_MST_SETUP,    // I2C MASTER SET UP ERR
    E_ERR_CNT_SETUP,    // COUNT SET UP ERR
    E_ERR_DMA_SETUP,    // DMA SET UP ERR
    E_ERR_SRCCLK_SETUP, // SET SOURCE CLK ERR
    E_ERR_WRITE,        // WRITE ERR
    E_ERR_READ,         // READ ERR
    E_ERR_PARAMETER,    // PARAMETER ERR
    E_ERR_TIMEOUT,      // TIMEOUT ERR
    E_ERR_RETRY,
} EN_I2C_HAL_ERRNUM;

typedef struct _hal_iic_clkcnt
{
    u16 u16CntForStp;
    u16 u16CntHighPerid;
    u16 u16CntLowPerid;
    u16 u16CntBtwnFalEdg;
    u16 u16CntForStart;
    u16 u16CntDataLatchTim;
    u16 u16CntTimoutIntrDly;
    u16 u16tSU;
    u16 u16tHD;

} ST_HAL_I2C_CLKCNT;

typedef enum _hal_dma_addr_mode
{
    E_DMA_ADDRMODE_NORMAL = 0,
    E_DMA_ADDRMODE_10BIT,
    E_DMA_ADDRMODE_MAX,
} EN_HAL_DMA_ADRMODE;

typedef enum _hal_dma_miu_priority
{
    E_DMA_MIUPRI_LOW = 0,
    E_DMA_MIUPRI_HIGH,
    E_DMA_MIUPRI_MAX,
} EN_HAL_DMA_MIUPRIORITY;

typedef enum _hal_dma_miu_channel
{
    E_DMA_MIU_CHANNEL0 = 0,
    E_DMA_MIU_CHANNEL1,
    E_DMA_MIU_MAX,
} EN_HAL_DMA_MIUCHANNEL;

typedef struct _hal_dma_addr
{
    u64 u64DmaAdrPhy;
    u8 *pu8DmaAdrVirtu;
    u64 u64DmaAdrMiu;
} ST_HAL_DMA_ADDR;

typedef struct _hal_dma_ctrl
{
    EN_HAL_DMA_ADRMODE     enDmaAdrMod;
    EN_HAL_DMA_MIUPRIORITY enDmaMiuPri;
    EN_HAL_DMA_MIUCHANNEL  enDmaMiuChanel;
    ST_HAL_DMA_ADDR        stDmaMiuAdr;
    bool                   bDmaCfgEnIntr;
} ST_HAL_DMA_CTRL;

typedef enum _i2c_speed_mode
{
    E_I2C_SPEED_MODE_NORMAL = 0,
    E_I2C_SPEED_MODE_HIGH,
    E_I2C_SPEED_MODE_ULTRA,
    E_I2C_SPEED_MODE_MAX,
} EN_I2C_SPEED_MODE;

typedef struct _hal_iic
{
    u64               u64BankBase;
    u32               u32EnDma;
    u32               u32Speed;
    u32               u32Group;
    u8                u8I2cConfig;
    u8                u8PushPull;
    u8                u8OenCnt;
    ST_HAL_I2C_CLKCNT stI2cClkCnt;
    ST_HAL_DMA_CTRL   stI2cDmaCtrl;
    s32 (*calbak_i2c_set_srcclk)(void *para_i2c_base, u32 para_src_clk);
    s32 (*calbak_i2c_dma_callback)(void *para_i2c_base);
} ST_HAL_I2C_BASE;
/******************************************************************************/
/***********************    function declaration    ***************************/
/******************************************************************************/
extern s32 HAL_I2C_SetTriggerReg(ST_HAL_I2C_BASE *para_i2c_base);
extern s32 HAL_I2C_DmaTrDone(ST_HAL_I2C_BASE *para_i2c_base, bool para_boolval);
extern s32 HAL_I2C_DmaStopFmt(ST_HAL_I2C_BASE *para_i2c_base, bool para_stop);
extern s32 HAL_I2C_RstSet(ST_HAL_I2C_BASE *para_i2c_base, bool para_rst);
extern s32 HAL_I2C_DmaCfgSofRstSet(ST_HAL_I2C_BASE *para_i2c_base, bool para_disenable);
extern s32 HAL_I2C_WNModeClr(ST_HAL_I2C_BASE *para_i2c_base);
extern s32 HAL_I2C_DmaWNWrite(ST_HAL_I2C_BASE *para_i2c_base, u16 para_slvadr, u8 *para_pdata, u32 para_len,
                              u8 para_wnlen, u16 para_waitcnt);
extern s32 HAL_I2C_DmaWNAsyncWrite(ST_HAL_I2C_BASE *para_i2c_base, u16 para_slvadr, u8 *para_pdata, u32 para_len,
                                   u8 para_wnlen, u16 para_waitcnt);
extern u32 HAL_I2C_GetDmaTc(ST_HAL_I2C_BASE *para_i2c_base);
extern s32 HAL_I2C_Write(ST_HAL_I2C_BASE *para_i2c_base, u16 para_slvadr, u8 *para_pdata, u32 para_len);
extern s32 HAL_I2C_Release(ST_HAL_I2C_BASE *para_i2c_base);
extern s32 HAL_I2C_Read(ST_HAL_I2C_BASE *para_i2c_base, u16 para_slvadr, u8 *para_pdata, u32 para_len);
extern s32 HAL_I2C_Init(ST_HAL_I2C_BASE *para_i2c_base);
extern s32 HAL_I2C_DmaAsyncWrite(ST_HAL_I2C_BASE *para_i2c_base, u16 para_slvadr, u8 *para_pdata, u32 para_len);
extern s32 HAL_I2C_DmaAsyncRead(ST_HAL_I2C_BASE *para_i2c_base, u16 para_slvadr, u8 *para_pdata, u32 para_len);
extern s32 HAL_I2C_SrclkSelect(ST_HAL_I2C_BASE *para_i2c_base);
extern s32 HAL_I2C_CntSet(ST_HAL_I2C_BASE *para_i2c_base);
#endif
