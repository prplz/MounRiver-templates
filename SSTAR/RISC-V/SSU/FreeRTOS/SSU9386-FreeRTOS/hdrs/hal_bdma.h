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

#ifndef __HAL_BDMA_H__
#define __HAL_BDMA_H__

/*=============================================================*/
// Data type definition
/*=============================================================*/

#define BDMA_SUPPORT_DMABLIT

#ifdef CONFIG_PHYS_ADDR_T_64BIT
typedef u64 phys_addr_t;
#else
typedef u32 phys_addr_t;
#endif

typedef enum
{
  HAL_BDMA_TRIGGER = 0x1,
  HAL_BDMA_STOP    = 0x10
}HalBdmaCtrl_e;

typedef enum
{
    HAL_BDMA_PROC_DONE       = 0,
    HAL_BDMA_ERROR           = -1,
    HAL_BDMA_POLLING_TIMEOUT = -2
} HalBdmaErr_e;

typedef enum
{
    //BDMA0
    HAL_BDMA_CH0 = 0,
    HAL_BDMA_CH1,
    HAL_BDMA_CH2,
    HAL_BDMA_CH3,

    // BDMA2
    HAL_BDMA1_CH0,
    HAL_BDMA1_CH1,
    HAL_BDMA1_CH2,
    HAL_BDMA1_CH3,

    // BDMA3
    HAL_BDMA2_CH0,
    HAL_BDMA2_CH1,
    HAL_BDMA2_CH2,
    HAL_BDMA2_CH3,
    HAL_BDMA_CH_NUM
} HalBdmaCh_e;

typedef enum
{
    HAL_BDMA_MIU0_TO_MIU0 = 0x0,
    HAL_BDMA_MIU0_TO_MIU1,
    HAL_BDMA_MIU1_TO_MIU0,
    HAL_BDMA_MIU1_TO_MIU1,
    HAL_BDMA_MIU0_TO_IMI,
    HAL_BDMA_MIU1_TO_IMI,
    HAL_BDMA_IMI_TO_MIU0,
    HAL_BDMA_IMI_TO_MIU1,
    HAL_BDMA_IMI_TO_IMI,
    HAL_BDMA_MEM_TO_MIU0,
    HAL_BDMA_MEM_TO_MIU1,
    HAL_BDMA_MEM_TO_IMI,
    HAL_BDMA_SPI_TO_MIU0,
    HAL_BDMA_SPI_TO_MIU1,
    HAL_BDMA_SPI_TO_IMI,
    HAL_BDMA_MIU0_TO_SPI,
    HAL_BDMA_MSPI0_TO_MIU,
    HAL_BDMA_MIU_TO_MSPI0,
    HAL_BDMA_MSPI1_TO_MIU,
    HAL_BDMA_MIU_TO_MSPI1
} HalBdmaPathSel_e;

#define HAL_BDMA_MIU HAL_BDMA_MIU0_TO_MIU0
#define HAL_BDMA_IMI HAL_BDMA_MIU0_TO_MIU1
#define HAL_BDMA_SPI 0x5

typedef enum
{
    HAL_BDMA_DATA_BYTE_1  = 0x0,
    HAL_BDMA_DATA_BYTE_2  = 0x1,
    HAL_BDMA_DATA_BYTE_4  = 0x2,
    HAL_BDMA_DATA_BYTE_8  = 0x3,
    HAL_BDMA_DATA_BYTE_16 = 0x4
} HalBdmaDataWidth_e;

typedef enum
{
    HAL_BDMA_ADDR_INC = 0x0,
    HAL_BDMA_ADDR_DEC = 0x1
} HalBdmaAddrMode_e;

/*=============================================================*/
// Structure definition
/*=============================================================*/

typedef void (* HalBdmaTxCb)(u32);

typedef struct {
    u32                     u32SrcWidth;     ///< Width of source
    u32                     u32SrcOffset;    ///< Line-to-line offset of source
    u32                     u32DstWidth;     ///< Width of destination
    u32                     u32DstOffset;    ///< Line-to-line offset of destination
} HalBdmaLineOfst_t;

typedef struct
{
    bool               bIntMode;
    HalBdmaPathSel_e   ePathSel;
    HalBdmaDataWidth_e eSrcDataWidth;
    HalBdmaDataWidth_e eDstDataWidth;
    HalBdmaAddrMode_e  eDstAddrMode;
    u32                u32TxCount;
    u32                u32Pattern;
    phys_addr_t        pSrcAddr;
    phys_addr_t        pDstAddr;
    u32                bEnLineOfst;
    HalBdmaLineOfst_t *pstLineOfst;
    HalBdmaTxCb        pfTxCbFunc;
    void *             pTxCbParm;
} HalBdmaParam_t;

/*=============================================================*/
// Global function definition
/*=============================================================*/

HalBdmaErr_e HalBdma_Initialize(u8 u8DmaCh);
HalBdmaErr_e HalBdma_Transfer(u8 u8DmaCh, HalBdmaParam_t *ptBdmaParam);
HalBdmaErr_e HalBdma_WaitTransferDone(u8 u8DmaCh, HalBdmaParam_t *ptBdmaParam);

#endif // __HAL_BDMA_H__
