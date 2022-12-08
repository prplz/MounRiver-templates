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


#ifndef __MHAL_MSPI_H__
#define __MHAL_MSPI_H__

#define MSPI_MAX_SUPPORT_BITS 16

struct mspi_hal
{
    u64 mspi_base;
    u32 pad_ctrl;
    u32 clk_out_mode;
    u32 cs_num;
    u32 cs_ext[16];
    u32 bits_per_word;
    s32 (*calbak_mspi_waitdone)(struct mspi_hal *mspi);
};
typedef enum
{
    E_MSPI_CHL0,
    E_MSPI_CHL1,
    E_MSPI_CHL2,
    E_MSPI_CHL3,
    E_MSPI_MAX,
} MSPI_CHL_e;

typedef enum
{
    E_MSPI_MODE0, // CPOL = 0,CPHA =0
    E_MSPI_MODE1, // CPOL = 0,CPHA =1
    E_MSPI_MODE2, // CPOL = 1,CPHA =0
    E_MSPI_MODE3, // CPOL = 1,CPHA =1
    E_MSPI_MODE_MAX,
} MSPI_Mode_Config_e;

typedef enum
{
    E_MSPI_BIT_MSB_FIRST,
    E_MSPI_BIT_LSB_FIRST,
} MSPI_BitSeq_e;

u8   HAL_MSPI_CheckDmaMode(u8 u8Channel);
u8   HAL_MSPI_Config(struct mspi_hal *mspi);
u16  HAL_MSPI_CheckDone(struct mspi_hal *mspi);
void HAL_MSPI_ClearDone(struct mspi_hal *mspi);
u8   HAL_MSPI_SetLSB(struct mspi_hal *mspi, u8 enable);
u8   HAL_MSPI_Set3WireMode(struct mspi_hal *mspi, u8 enable);
u8   HAL_MSPI_SetDivClk(struct mspi_hal *mspi, u8 div);
u8   HAL_MSPI_SetMode(struct mspi_hal *mspi, MSPI_Mode_Config_e eMode);
void HAL_MSPI_ChipSelect(struct mspi_hal *mspi, u8 Enable, u8 eSelect);
u8   HAL_MSPI_Read(u8 u8Channel, struct mspi_hal *mspi, u8 *pData, u16 u16Size);
u8   HAL_MSPI_Write(u8 u8Channel, struct mspi_hal *mspi, u8 *pData, u16 u16Size);
u8   HAL_MSPI_DMA_Write(u8 u8Channel, struct mspi_hal *mspi, u8 *pData, u32 u32Size);
u8   HAL_MSPI_DMA_Read(u8 u8Channel, struct mspi_hal *mspi, u8 *pData, u32 u32Size);
u8   HAL_MSPI_FullDuplex(u8 u8Channel, struct mspi_hal *mspi, u8 *rx_buff, u8 *tx_buff, u16 u16Size);
u8   HAL_MSPI_SET_FRAMECFG(struct mspi_hal *mspi, int bits_per_word);

#endif
