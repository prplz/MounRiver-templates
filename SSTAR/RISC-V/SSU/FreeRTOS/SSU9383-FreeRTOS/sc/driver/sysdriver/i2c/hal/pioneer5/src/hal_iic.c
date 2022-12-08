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

#include <iic_os.h>
#include "hal_iic.h"
#include "hal_iic_reg.h"

#ifdef CONFIG_ARM64
#define READ_WORD(_reg)        (*(volatile u16 *)(u64)(_reg))
#define WRITE_WORD(_reg, _val) ((*(volatile u16 *)(u64)(_reg)) = (u16)(_val))
#define WRITE_WORD_MASK(_reg, _val, _mask) \
    ((*(volatile u16 *)(u64)(_reg)) = ((*(volatile u16 *)(u64)(_reg)) & ~(_mask)) | ((u16)(_val) & (_mask)))

#define WRITE_BYTE(_reg, _val) ((*(volatile unsigned char *)((u64)_reg)) = (u8)(_val))
#define READ_BYTE(_reg)        (*(volatile unsigned char *)((u64)_reg))
#else
#define READ_WORD(_reg)        (*(volatile u16 *)(u32)(_reg))
#define WRITE_WORD(_reg, _val) ((*(volatile u16 *)(u32)(_reg)) = (u16)(_val))
#define WRITE_WORD_MASK(_reg, _val, _mask) \
    ((*(volatile u16 *)(u32)(_reg)) = ((*(volatile u16 *)(u32)(_reg)) & ~(_mask)) | ((u16)(_val) & (_mask)))

#define WRITE_BYTE(_reg, _val) ((*(volatile unsigned char *)((u32)_reg)) = (u8)(_val))
#define READ_BYTE(_reg)        (*(volatile unsigned char *)((u32)_reg))
#endif

#define I2C_READ_REG(reg)                  READ_WORD(para_i2c_base->u64BankBase + ((reg) << 2))
#define I2C_WRITE_REG(reg, val)            WRITE_WORD(para_i2c_base->u64BankBase + ((reg) << 2), (val))
#define I2C_WRITE_REG_MASK(reg, val, mask) WRITE_WORD_MASK(para_i2c_base->u64BankBase + ((reg) << 2), val, mask)

#define I2C_WRITE_BYTE_REG(reg, val) WRITE_BYTE(para_i2c_base->u64BankBase + (reg), (val))
#define I2C_READ_BYTE_REG(reg)       READ_BYTE(para_i2c_base->u64BankBase + (reg))

#define NUMBER_TRANSFER_RETRY (50000)
#define MIIC_POLL_RETRY_TIME  (10000000)

/***************************************************************************************/
/*****************************    function definition    *******************************/
/***************************************************************************************/

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_GetRegFlag(ST_HAL_I2C_BASE *para_i2c_base)
{
    s32 s32Ret;

    s32Ret = I2C_READ_REG(REG_I2C_INT_FLAG);
    s32Ret = s32Ret & REG_MASK_INT_FLAG;

    return s32Ret;
}
s32 HAL_I2C_ClearInt(ST_HAL_I2C_BASE *para_i2c_base)
{
    u16 u16Retry = 0;

    I2C_WRITE_REG_MASK(REG_I2C_INT_FLAG, true, REG_MASK_INT_FLAG);

    for (u16Retry = 0; u16Retry < NUMBER_TRANSFER_RETRY; u16Retry++)
    {
        if (!HAL_I2C_GetRegFlag(para_i2c_base))
        {
            return 0;
        }
    }

    return -1;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static inline s32 HAL_I2C_DetDmaTrDone(ST_HAL_I2C_BASE *para_i2c_base)
{
    s32 s32Ret;

    s32Ret = I2C_READ_REG(REG_I2C_DMA_TRANSFER_DONE);
    s32Ret &= REG_MASK_DMA_TRSFER_DONE;

    return s32Ret;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
s32 HAL_I2C_RstSet(ST_HAL_I2C_BASE *para_i2c_base, bool para_rst)
{
    I2C_WRITE_REG_MASK(REG_I2C_CONFIG, para_rst, REG_MASK_CFG_RST);

    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_DmaCfgIntSet(ST_HAL_I2C_BASE *para_i2c_base, bool para_disenable)
{
    I2C_WRITE_REG_MASK(REG_I2C_DMA_CONFIG, para_disenable << SHIFT_BIT2, REG_MASK_DMACFG_INTFLAG);

    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
s32 HAL_I2C_DmaCfgSofRstSet(ST_HAL_I2C_BASE *para_i2c_base, bool para_disenable)
{
    I2C_WRITE_REG_MASK(REG_I2C_DMA_RESERVED, para_disenable, REG_MASK_DMA_RESET);
    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_DmaCfgMiuRst(ST_HAL_I2C_BASE *para_i2c_base, bool para_disenable)
{
    I2C_WRITE_REG_MASK(REG_I2C_DMA_CONFIG, para_disenable << SHIFT_BIT3, REG_MASK_DMACFG_MIURST);

    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_DmaAdrModSet(ST_HAL_I2C_BASE *para_i2c_base, const EN_HAL_DMA_ADRMODE para_adr_mode)
{
    I2C_WRITE_REG_MASK(REG_I2C_DMA_10BITMODE, para_adr_mode << SHIFT_BIT10, REG_MASK_DMABITMODE);

    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_DmaMiuPriSet(ST_HAL_I2C_BASE *para_i2c_base, const EN_HAL_DMA_MIUPRIORITY para_miu_pri)
{
    I2C_WRITE_REG_MASK(REG_I2C_DMA_CONFIG, para_miu_pri << SHIFT_BIT4, REG_MASK_DMACFG_MIUPRI);

    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_DmaMiuChnalSet(ST_HAL_I2C_BASE *para_i2c_base, const EN_HAL_DMA_MIUCHANNEL para_miu_chanl)
{
    I2C_WRITE_REG_MASK(REG_I2C_DMA_MIUCHANL_SEL, para_miu_chanl << SHIFT_BIT7, REG_MASK_DMA_MIUCHANLSEL);

    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_DmaMiuAddrSet(ST_HAL_I2C_BASE *para_i2c_base, const u64 para_miu_addr)
{
    I2C_WRITE_REG(REG_I2C_BUFDRAMADDR_LOW, (u16)(para_miu_addr & 0x0000FFFF));
    I2C_WRITE_REG(REG_I2C_BUFDRAMADDR_HIGH, ((u16)((para_miu_addr & 0xFFFF0000) >> 16)));
    I2C_WRITE_REG_MASK(REG_I2C_BUFDRAMADDR_MSB, ((u16)((para_miu_addr & 0xF00000000) >> 32)), REG_MASK_IIC_ADDR_MSB);

    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
u32 HAL_I2C_GetDmaTc(ST_HAL_I2C_BASE *para_i2c_base)
{
    u32 u32Ret;
    u32Ret = I2C_READ_REG(REG_I2C_DMA_TC_2CH);
    u32Ret |= (I2C_READ_REG(REG_I2C_DMA_TC_2DH) << 16);
    return u32Ret;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
s32 HAL_I2C_SetTriggerReg(ST_HAL_I2C_BASE *para_i2c_base)
{
    I2C_WRITE_REG_MASK(REG_I2C_DMA_TRIGGER, true, REG_MASK_DMATRIG);
    return 0;
}
static s32 HAL_I2C_DmaTransfer(ST_HAL_I2C_BASE *para_i2c_base)
{
    s32 s32Ret         = 0;
    u32 u32WaitDmaDone = MIIC_POLL_RETRY_TIME;

    if (para_i2c_base->calbak_i2c_dma_callback)
    {
        s32Ret = para_i2c_base->calbak_i2c_dma_callback(para_i2c_base);
        if (s32Ret)
        {
            return -E_ERR;
        }
    }
    else
    {
        while (u32WaitDmaDone--)
        {
            s32Ret = HAL_I2C_DetDmaTrDone(para_i2c_base);
            if (s32Ret)
            {
                HAL_I2C_DmaTrDone(para_i2c_base, true);
                break;
            }
            if (1 == u32WaitDmaDone)
            {
                dmsg_i2c_halerr("i2c-%d dma read polling timeout!\n", para_i2c_base->u32Group);
                return -E_ERR_TIMEOUT;
            }
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
static s32 HAL_I2C_ForStpCntSet(ST_HAL_I2C_BASE *para_i2c_base, u16 para_cnt)
{
    I2C_WRITE_REG(REG_I2C_CNT_FOR_STOP, para_cnt);

    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_HiPerCntSet(ST_HAL_I2C_BASE *para_i2c_base, u16 para_cnt)
{
    I2C_WRITE_REG(REG_I2C_CNT_HIGH_PERIOD, para_cnt);

    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_LowPerCntSet(ST_HAL_I2C_BASE *para_i2c_base, u16 para_cnt)
{
    I2C_WRITE_REG(REG_I2C_CNT_LOW_PERIOD, para_cnt);

    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_BtwnFlEdgCntSet(ST_HAL_I2C_BASE *para_i2c_base, u16 para_cnt)
{
    I2C_WRITE_REG(REG_I2C_CNT_BTWN_FALEDGE, para_cnt);

    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_ForStarCntSet(ST_HAL_I2C_BASE *para_i2c_base, u16 para_cnt)
{
    I2C_WRITE_REG(REG_I2C_CNT_FOR_START, para_cnt);

    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_LtchTimCntSet(ST_HAL_I2C_BASE *para_i2c_base, u16 para_cnt)
{
    I2C_WRITE_REG(REG_I2C_CNT_DATA_LATCHTIM, para_cnt);

    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_TimoutOcrCntSet(ST_HAL_I2C_BASE *para_i2c_base, u16 para_cnt)
{
    I2C_WRITE_REG(REG_I2C_CNT_DELYTIMOUT_INTOCUR, para_cnt);

    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
s32 HAL_I2C_DmaTrDone(ST_HAL_I2C_BASE *para_i2c_base, bool para_boolval)
{
    s32 s32Ret;
    s32Ret = HAL_I2C_DetDmaTrDone(para_i2c_base);
    if (s32Ret)
    {
        I2C_WRITE_REG_MASK(REG_I2C_DMA_TRANSFER_DONE, para_boolval, REG_MASK_DMA_TRSFER_DONE);
        return 1;
    }
    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_SetCmdData(ST_HAL_I2C_BASE *para_i2c_base, u16 para_data)
{
    I2C_WRITE_REG(REG_I2C_DMA_CMD_DATA_25H, para_data);

    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:para_boolwr = 1/READ; para_boolwr = 0/WRITE
//*return:
/****************************************************/
static s32 HAL_I2C_SetCmdLen(ST_HAL_I2C_BASE *para_i2c_base, u16 para_len)
{
    I2C_WRITE_REG_MASK(REG_I2C_DMA_CMD_LEN_29H, para_len, REG_MASK_DMA_CMDLEN);

    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:para_boolwr = 1/READ; para_boolwr = 0/WRITE
//*return:
/****************************************************/
static s32 HAL_I2C_SetDmaDataLen(ST_HAL_I2C_BASE *para_i2c_base, u32 para_len)
{
    I2C_WRITE_REG(REG_I2C_TRANS_DATA_LEN_LOW, (u16)(para_len & 0x0000FFFF));
    dmsg_i2c_halwarn("i2c-%d set data len : %d\n", para_i2c_base->u32Group, para_len);
    I2C_WRITE_REG(REG_I2C_TRANS_DATA_LEN_HIGH, (u16)((para_len & 0xFFFF0000) >> 16));

    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:para_boolwr = 1/READ; para_boolwr = 0/WRITE
//*return:
/****************************************************/
static s32 HAL_I2C_TrsfmtSet(ST_HAL_I2C_BASE *para_i2c_base, bool para_boolwr)
{
    I2C_WRITE_REG_MASK(REG_I2C_RDWR_FORMAT, para_boolwr << SHIFT_BIT6, REG_MASK_RDWR_FMT);

    return 0;
}

s32 HAL_I2C_DmaStopFmt(ST_HAL_I2C_BASE *para_i2c_base, bool para_stop)
{
    if (para_stop)
    {
        I2C_WRITE_REG_MASK(REG_I2C_STOP_FORMAT_DISABLE, 0 << 5, REG_MASK_STPFMT_DIS);
    }
    else
    {
        I2C_WRITE_REG_MASK(REG_I2C_STOP_FORMAT_DISABLE, 1 << 5, REG_MASK_STPFMT_DIS);
    }
    return 0;
}
/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_DmaSlvAddrSet(ST_HAL_I2C_BASE *para_i2c_base, u16 para_slvadr)
{
    if (E_DMA_ADDRMODE_10BIT == para_i2c_base->stI2cDmaCtrl.enDmaAdrMod)
    {
        I2C_WRITE_REG_MASK(REG_I2C_DMA_SLAVEADDR, para_slvadr & REG_MASK_DMASLVADDR_10BIT, REG_MASK_DMASLVADDR_10BIT);
    }
    else
    {
        I2C_WRITE_REG_MASK(REG_I2C_DMA_SLAVEADDR, para_slvadr & REG_MASK_DMASLVADDR_NORM, REG_MASK_DMASLVADDR_10BIT);
    }

    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_TranCmdStart(ST_HAL_I2C_BASE *para_i2c_base)
{
    I2C_WRITE_BYTE_REG((REG_I2C_STAR_STOP << 2), REG_MASK_CMD_START);

    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_TranCmdStop(ST_HAL_I2C_BASE *para_i2c_base)
{
    I2C_WRITE_BYTE_REG(((REG_I2C_STAR_STOP << 2) + 1), REG_MASK_CMD_STOP);

    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static inline s32 HAL_I2C_DetStartInt(ST_HAL_I2C_BASE *para_i2c_base)
{
    s32 s32Ret;

    s32Ret = I2C_READ_REG(REG_I2C_INT_STATUS) & REG_MASK_INTSTU_START;
    s32Ret = s32Ret >> SHIFT_BIT8;

    return s32Ret;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static inline s32 HAL_I2C_DetStopInt(ST_HAL_I2C_BASE *para_i2c_base)
{
    s32 s32Ret;

    s32Ret = I2C_READ_REG(REG_I2C_INT_STATUS) & REG_MASK_INTSTU_STOP;
    s32Ret = s32Ret >> SHIFT_BIT9;

    return s32Ret;
}

static inline s32 HAL_I2C_DetIdle(ST_HAL_I2C_BASE *para_i2c_base)
{
    s32 s32Ret;

    s32Ret = I2C_READ_REG(REG_I2C_INT_STATUS) & REG_MASK_INTSTU_STATE;

    return s32Ret;
}
/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static inline s32 HAL_I2C_DetRxDoneInt(ST_HAL_I2C_BASE *para_i2c_base)
{
    s32 s32Ret;

    s32Ret = I2C_READ_REG(REG_I2C_INT_STATUS) & REG_MASK_INTSTU_RXDONE;
    s32Ret = s32Ret >> SHIFT_BIT10;

    return s32Ret;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static inline s32 HAL_I2C_DetTxDoneInt(ST_HAL_I2C_BASE *para_i2c_base)
{
    s32 s32Ret;

    s32Ret = I2C_READ_REG(REG_I2C_INT_STATUS) & REG_MASK_INTSTU_TXDONE;
    s32Ret = s32Ret >> SHIFT_BIT11;

    return s32Ret;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static inline u16 HAL_I2C_GetSlvAck(ST_HAL_I2C_BASE *para_i2c_base)
{
    return (I2C_READ_BYTE_REG((REG_I2C_WRITE_DATA << 2) + 1) & 0x01);
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_ReadTrigger(ST_HAL_I2C_BASE *para_i2c_base, const u8 ack)
{
    // miic offset h0003 bit8(reg_rdata_en) will take effect by the action write register
    // rather than value 1 in this bit
    // so wo move reg_ack_bit here or it`ll trigger twice
    // and the seconed write trigger will cover the last read action whenever the last read done or not
    // then cause some unexpected clk bit

    I2C_WRITE_BYTE_REG(((REG_I2C_READ_DATA << 2) + 1), (1 | (ack << SHIFT_BIT1)));

    return 0;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_SendByte(ST_HAL_I2C_BASE *para_i2c_base, const u8 para_val)
{
    s32 s32Ret = 0;
    u16 u16Retry;

    I2C_WRITE_BYTE_REG((REG_I2C_WRITE_DATA << 2), para_val);
    for (u16Retry = 0; u16Retry < NUMBER_TRANSFER_RETRY; u16Retry++)
    {
        s32Ret |= (HAL_I2C_GetRegFlag(para_i2c_base) && HAL_I2C_DetTxDoneInt(para_i2c_base));
        if (s32Ret)
        {
            s32Ret |= HAL_I2C_ClearInt(para_i2c_base);
            u16Retry = 0;
            break;
        }
    }
    if (u16Retry >= NUMBER_TRANSFER_RETRY)
    {
        dmsg_i2c_halerr("i2c-%d err tx done timeout\n", para_i2c_base->u32Group);
        return -E_ERR_RETRY;
    }
    for (u16Retry = 0; u16Retry < NUMBER_TRANSFER_RETRY; u16Retry++) // ask for ack
    {
        if (!HAL_I2C_GetSlvAck(para_i2c_base))
        {
            HAL_I2C_ClearInt(para_i2c_base);
            dmsg_i2c_halwarn("i2c-%d get an ack\n", para_i2c_base->u32Group);
            return 0;
        }
    }
    HAL_I2C_ClearInt(para_i2c_base);
    dmsg_i2c_halerr("i2c-%d no ack!\n", para_i2c_base->u32Group);
    return -E_ERR_RETRY;
}
/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_SendBuf(ST_HAL_I2C_BASE *para_i2c_base, u8 *para_pdata, u32 para_length)
{
    s32 s32Ret = 0;
    u16 i;
    for (i = 0; i < para_length; i++)
    {
        s32Ret |= HAL_I2C_SendByte(para_i2c_base, *(para_pdata + i));
        if (s32Ret < 0) // send byte
        {
            return s32Ret;
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
static int HAL_I2C_RecvByte(ST_HAL_I2C_BASE *para_i2c_base, u8 *Readbuf, const u8 ack)
{
    u16 u16Retry;
    s32 s32Ret = 0;

    s32Ret |= HAL_I2C_ReadTrigger(para_i2c_base, ack);

    for (u16Retry = 0; u16Retry < NUMBER_TRANSFER_RETRY; u16Retry++)
    {
        s32Ret |= (HAL_I2C_GetRegFlag(para_i2c_base) && HAL_I2C_DetRxDoneInt(para_i2c_base));
        if (s32Ret)
        {
            HAL_I2C_ClearInt(para_i2c_base);
            u16Retry = 0;
            break;
        }
    }
    if (u16Retry >= NUMBER_TRANSFER_RETRY)
    {
        dmsg_i2c_halerr("i2c-%d recv byte timeout err\n", para_i2c_base->u32Group);
        s32Ret = -E_ERR_RETRY;
        goto OUT;
    }
    *Readbuf = (u8)(I2C_READ_BYTE_REG(REG_I2C_READ_DATA << 2));
    s32Ret   = 0;
OUT:
    HAL_I2C_ClearInt(para_i2c_base);
    return s32Ret;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return: recv byte number
/****************************************************/
static s32 HAL_I2C_RecvBuf(ST_HAL_I2C_BASE *para_i2c_base, u8 *para_pdata, u32 para_length)
{
    u16 i;
    u8  ack;
    for (i = 0; i < para_length; i++)
    {
        if (i == (para_length - 1))
        {
            dmsg_i2c_halwarn("i2c-%d return nack\n", para_i2c_base->u32Group);
            ack = 0x01;
        }
        else
        {
            ack = 0x00;
            dmsg_i2c_halwarn("i2c-%d return ack\n", para_i2c_base->u32Group);
        }
        if (HAL_I2C_RecvByte(para_i2c_base, (para_pdata + i), ack))
        {
            dmsg_i2c_halerr("i2c-%d receive buffer error\n", para_i2c_base->u32Group);
            break;
        }
    }
    return i;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_DmaWrite(ST_HAL_I2C_BASE *para_i2c_base, u16 para_slvadr, u8 *para_pdata, u32 para_len)
{
    s32 s32Ret = 0;
    u8 *pu8DmaViraddr;
    u32 u32DmaTc;

    // set read/write cmd
    s32Ret |= HAL_I2C_TrsfmtSet(para_i2c_base, false); // set write
    // set slave addr
    s32Ret |= HAL_I2C_DmaSlvAddrSet(para_i2c_base, para_slvadr); // set dma slave address
    // set cmd data
    s32Ret |= HAL_I2C_SetCmdData(para_i2c_base, 0x0000);
    // set cmd len
    s32Ret |= HAL_I2C_SetCmdLen(para_i2c_base, 0x0);
    // set miu addr
    /*done in HAL_I2C_DmaSetup*/
    // set data length
    s32Ret |= HAL_I2C_SetDmaDataLen(para_i2c_base, para_len);
    // reset and reset miu
    s32Ret |= HAL_I2C_DmaCfgMiuRst(para_i2c_base, true);
    s32Ret |= HAL_I2C_DmaCfgMiuRst(para_i2c_base, false);
    // copy data to miu addr
    pu8DmaViraddr = para_i2c_base->stI2cDmaCtrl.stDmaMiuAdr.pu8DmaAdrVirtu;
    memcpy(pu8DmaViraddr, para_pdata, para_len);
    i2c_os_cache_flush(pu8DmaViraddr, para_len);
    //  set enable dma interrupt
    s32Ret |= HAL_I2C_DmaCfgIntSet(para_i2c_base, true);
    // set clear done
    s32Ret |= HAL_I2C_DmaTrDone(para_i2c_base, true);

    // set trigger
    s32Ret |= HAL_I2C_DmaTransfer(para_i2c_base);
    if (s32Ret)
    {
        dmsg_i2c_halerr("i2c-%d, trigger dma write failed\n", para_i2c_base->u32Group);
        return -E_ERR_WRITE;
    }

    u32DmaTc = HAL_I2C_GetDmaTc(para_i2c_base);

    if (u32DmaTc != para_len)
    {
        dmsg_i2c_halerr("i2c-%d err dma write transform len:%d != para_len:%d\n", para_i2c_base->u32Group, u32DmaTc,
                        para_len);
        return -E_ERR_WRITE;
    }
    dmsg_i2c_halwarn("DMA WRITE SUCCESS DONE\n");
    return 0;
} // end HAL_I2C_DmaWrite()
s32 HAL_I2C_DmaAsyncWrite(ST_HAL_I2C_BASE *para_i2c_base, u16 para_slvadr, u8 *para_pdata, u32 para_len)
{
    s32 s32Ret = 0;
    u8 *pu8DmaViraddr;

    // set read/write cmd
    s32Ret |= HAL_I2C_TrsfmtSet(para_i2c_base, false); // set write
    // set slave addr
    s32Ret |= HAL_I2C_DmaSlvAddrSet(para_i2c_base, para_slvadr); // set dma slave address
    // set cmd data
    s32Ret |= HAL_I2C_SetCmdData(para_i2c_base, 0x0000);
    // set cmd len
    s32Ret |= HAL_I2C_SetCmdLen(para_i2c_base, 0x0);
    // set miu addr
    /*done in HAL_I2C_DmaSetup*/
    // set data length
    s32Ret |= HAL_I2C_SetDmaDataLen(para_i2c_base, para_len);
    // reset and reset miu
    s32Ret |= HAL_I2C_DmaCfgMiuRst(para_i2c_base, true);
    s32Ret |= HAL_I2C_DmaCfgMiuRst(para_i2c_base, false);
    // copy data to miu addr
    pu8DmaViraddr = para_i2c_base->stI2cDmaCtrl.stDmaMiuAdr.pu8DmaAdrVirtu;
    memcpy(pu8DmaViraddr, para_pdata, para_len);
    i2c_os_cache_flush(pu8DmaViraddr, para_len);
    //  set enable dma interrupt
    s32Ret |= HAL_I2C_DmaCfgIntSet(para_i2c_base, true);
    // set clear done
    s32Ret |= HAL_I2C_DmaTrDone(para_i2c_base, true);

    // set trigger
    s32Ret |= HAL_I2C_DmaTransfer(para_i2c_base);

    return 0;
} // end HAL_I2C_DmaWrite()

static s32 HAL_I2C_WnGrpLenSet(ST_HAL_I2C_BASE *para_i2c_base, u8 para_grplen)
{
    I2C_WRITE_REG_MASK(REG_I2C_WN_GROUP_LEN, para_grplen, REG_MASK_WN_GROUP_LEN);
    return 0;
}

static s32 HAL_I2C_WnModeEn(ST_HAL_I2C_BASE *para_i2c_base, bool para_en)
{
    I2C_WRITE_REG_MASK(REG_I2C_WN_MODE, para_en, REG_MASK_WN_MODE_EN);
    return 0;
}
static s32 HAL_I2C_WnWaitCntSet(ST_HAL_I2C_BASE *para_i2c_base, u16 para_waitcnt)
{
    I2C_WRITE_REG(REG_I2C_WN_WAIT_CNT, para_waitcnt);
    return 0;
}
s32 HAL_I2C_WNModeClr(ST_HAL_I2C_BASE *para_i2c_base)
{
    s32 s32Ret = 0;
    s32Ret |= HAL_I2C_WnGrpLenSet(para_i2c_base, 0);
    s32Ret |= HAL_I2C_WnModeEn(para_i2c_base, false);

    return s32Ret;
}
s32 HAL_I2C_DmaWNWrite(ST_HAL_I2C_BASE *para_i2c_base, u16 para_slvadr, u8 *para_pdata, u32 para_len, u8 para_wnlen,
                       u16 para_waitcnt)
{
    s32 s32Ret = 0;
    u8 *pu8DmaViraddr;
    u32 u32DmaTrsCnt;

    s32Ret |= HAL_I2C_DmaCfgSofRstSet(para_i2c_base, true);  // dma reset
    s32Ret |= HAL_I2C_DmaCfgMiuRst(para_i2c_base, true);     // miu reset
    s32Ret |= HAL_I2C_DmaCfgSofRstSet(para_i2c_base, false); // dma not reset
    s32Ret |= HAL_I2C_DmaCfgMiuRst(para_i2c_base, false);    // miu not reset

    s32Ret |= HAL_I2C_TrsfmtSet(para_i2c_base, false); // set write
    s32Ret |= HAL_I2C_WnGrpLenSet(para_i2c_base, para_wnlen);
    s32Ret |= HAL_I2C_WnModeEn(para_i2c_base, true);             // enable wn mode
    s32Ret |= HAL_I2C_WnWaitCntSet(para_i2c_base, para_waitcnt); // weather set wait counter
    s32Ret |= HAL_I2C_DmaSlvAddrSet(para_i2c_base, para_slvadr); // set dma slave address
    // set cmd data
    s32Ret |= HAL_I2C_SetCmdData(para_i2c_base, 0x0000);
    // set cmd len
    s32Ret |= HAL_I2C_SetCmdLen(para_i2c_base, 0x0);
    // set data length
    s32Ret |= HAL_I2C_SetDmaDataLen(para_i2c_base, para_len);
    s32Ret |= HAL_I2C_DmaTrDone(para_i2c_base, true);
    s32Ret |= HAL_I2C_DmaCfgIntSet(para_i2c_base, true);
    pu8DmaViraddr = para_i2c_base->stI2cDmaCtrl.stDmaMiuAdr.pu8DmaAdrVirtu;
    memcpy(pu8DmaViraddr, para_pdata, para_len);
    i2c_os_cache_flush(pu8DmaViraddr, para_len);

    s32Ret |= HAL_I2C_DmaTransfer(para_i2c_base);

    u32DmaTrsCnt = HAL_I2C_GetDmaTc(para_i2c_base);
    if (u32DmaTrsCnt != para_len)
    {
        dmsg_i2c_halerr("i2c-%d err dma write transform len:%d != para_len:%d\n", para_i2c_base->u32Group, u32DmaTrsCnt,
                        para_len);
        return -E_ERR_WRITE;
    }

    return 0;
}

s32 HAL_I2C_DmaWNAsyncWrite(ST_HAL_I2C_BASE *para_i2c_base, u16 para_slvadr, u8 *para_pdata, u32 para_len,
                            u8 para_wnlen, u16 para_waitcnt)
{
    s32 s32Ret = 0;
    u8 *pu8DmaViraddr;

    s32Ret |= HAL_I2C_DmaCfgSofRstSet(para_i2c_base, true);  // dma reset
    s32Ret |= HAL_I2C_DmaCfgMiuRst(para_i2c_base, true);     // miu reset
    s32Ret |= HAL_I2C_DmaCfgSofRstSet(para_i2c_base, false); // dma not reset
    s32Ret |= HAL_I2C_DmaCfgMiuRst(para_i2c_base, false);    // miu not reset

    s32Ret |= HAL_I2C_TrsfmtSet(para_i2c_base, false); // set write
    s32Ret |= HAL_I2C_WnGrpLenSet(para_i2c_base, para_wnlen);
    s32Ret |= HAL_I2C_WnModeEn(para_i2c_base, true);             // enable wn mode
    s32Ret |= HAL_I2C_WnWaitCntSet(para_i2c_base, para_waitcnt); // weather set wait counter
    s32Ret |= HAL_I2C_DmaSlvAddrSet(para_i2c_base, para_slvadr); // set dma slave address
    // set cmd data
    s32Ret |= HAL_I2C_SetCmdData(para_i2c_base, 0x0000);
    // set cmd len
    s32Ret |= HAL_I2C_SetCmdLen(para_i2c_base, 0x0);
    // set data length
    s32Ret |= HAL_I2C_SetDmaDataLen(para_i2c_base, para_len);
    s32Ret |= HAL_I2C_DmaTrDone(para_i2c_base, true);
    s32Ret |= HAL_I2C_DmaCfgIntSet(para_i2c_base, true);
    pu8DmaViraddr = para_i2c_base->stI2cDmaCtrl.stDmaMiuAdr.pu8DmaAdrVirtu;
    memcpy(pu8DmaViraddr, para_pdata, para_len);
    i2c_os_cache_flush(pu8DmaViraddr, para_len);

    s32Ret |= HAL_I2C_DmaTransfer(para_i2c_base);

    return 0;
}
static s32 HAL_I2C_NormalWrite(ST_HAL_I2C_BASE *para_i2c_base, u16 para_slvadr, u8 *para_pdata, u32 para_len)
{
    s32 s32Ret   = 0;
    u16 u16Retry = 0;
    // reset iic
    // 1.send start
    HAL_I2C_ClearInt(para_i2c_base);
    s32Ret |= HAL_I2C_TranCmdStart(para_i2c_base); // do we need clear start bit??
    for (u16Retry = 0; u16Retry < NUMBER_TRANSFER_RETRY; u16Retry++)
    {
        if (HAL_I2C_GetRegFlag(para_i2c_base))
        {
            HAL_I2C_ClearInt(para_i2c_base);
            u16Retry = 0;
            break;
        }
    }
    if (u16Retry >= NUMBER_TRANSFER_RETRY)
    {
        s32Ret = -E_ERR_RETRY;
        dmsg_i2c_halerr("i2c-%d write start err!\n", para_i2c_base->u32Group);
        goto err_rtn;
    }
    // 2.send slave addr << 1 | w_bit
    s32Ret |= HAL_I2C_SendByte(para_i2c_base, para_slvadr);
    if (s32Ret < 0)
    {
        dmsg_i2c_halerr("i2c-%d send slave addr|write = 0x%04x nack err %d\n", para_i2c_base->u32Group, para_slvadr,
                        s32Ret);
        goto err_rtn;
    }

    // 3.send data
    s32Ret |= HAL_I2C_SendBuf(para_i2c_base, para_pdata, para_len);
    if (s32Ret < 0)
    {
        dmsg_i2c_halerr("i2c-%d write send data err %d\n", para_i2c_base->u32Group, s32Ret);
        goto err_rtn;
    }
    dmsg_i2c_halwarn("FIFO MODE WRITE SUCCESS DONE\n");
    return 0;
err_rtn:
    return s32Ret;
}
/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_DmaRead(ST_HAL_I2C_BASE *para_i2c_base, u16 para_slvadr, u8 *para_pdata, u32 para_len)
{
    s32 s32Ret = 0;
    u8 *pu8DmaViraddr;
    u32 u32DmaTc;

    pu8DmaViraddr = para_i2c_base->stI2cDmaCtrl.stDmaMiuAdr.pu8DmaAdrVirtu;

    // set read/write cmd
    s32Ret |= HAL_I2C_TrsfmtSet(para_i2c_base, true); // set read
    // set slave addr
    s32Ret |= HAL_I2C_DmaSlvAddrSet(para_i2c_base, para_slvadr); // set dma slave address
    // set cmd data
    s32Ret |= HAL_I2C_SetCmdData(para_i2c_base, 0x0000);
    // set cmd len
    s32Ret |= HAL_I2C_SetCmdLen(para_i2c_base, 0x00);
    // set miu addr
    /*done in HAL_I2C_DmaSetup*/
    // set data length
    s32Ret |= HAL_I2C_SetDmaDataLen(para_i2c_base, para_len);
    // reset and reset miu

    s32Ret |= HAL_I2C_DmaCfgMiuRst(para_i2c_base, true);
    s32Ret |= HAL_I2C_DmaCfgMiuRst(para_i2c_base, false);
    // set enable dma interrupt
    s32Ret |= HAL_I2C_DmaCfgIntSet(para_i2c_base, true);
    // set clear done
    s32Ret |= HAL_I2C_DmaTrDone(para_i2c_base, true);

    // set trigger
    s32Ret |= HAL_I2C_DmaTransfer(para_i2c_base);
    if (s32Ret)
    {
        dmsg_i2c_halerr("i2c-%d, trigger dma read failed\n", para_i2c_base->u32Group);
        return -E_ERR_READ;
    }

    u32DmaTc = HAL_I2C_GetDmaTc(para_i2c_base);

    if (u32DmaTc != para_len)
    {
        dmsg_i2c_halerr("i2c-%d dma read transform len:%d != para_len:%d err\n", para_i2c_base->u32Group, u32DmaTc,
                        para_len);
        return -E_ERR_READ;
    }
    i2c_os_cache_invalidate(pu8DmaViraddr, para_len);
    memcpy(para_pdata, pu8DmaViraddr, para_len);
    dmsg_i2c_halwarn("DMA READ SUCCESS DONE\n");
    return 0;
}
s32 HAL_I2C_DmaAsyncRead(ST_HAL_I2C_BASE *para_i2c_base, u16 para_slvadr, u8 *para_pdata, u32 para_len)
{
    s32 s32Ret = 0;

    // set read/write cmd
    s32Ret |= HAL_I2C_TrsfmtSet(para_i2c_base, true); // set read
    // set slave addr
    s32Ret |= HAL_I2C_DmaSlvAddrSet(para_i2c_base, para_slvadr); // set dma slave address
    // set cmd data
    s32Ret |= HAL_I2C_SetCmdData(para_i2c_base, 0x0000);
    // set cmd len
    s32Ret |= HAL_I2C_SetCmdLen(para_i2c_base, 0x00);
    // set miu addr
    /*done in HAL_I2C_DmaSetup*/
    // set data length
    s32Ret |= HAL_I2C_SetDmaDataLen(para_i2c_base, para_len);
    // reset and reset miu

    s32Ret |= HAL_I2C_DmaCfgMiuRst(para_i2c_base, true);
    s32Ret |= HAL_I2C_DmaCfgMiuRst(para_i2c_base, false);
    // set enable dma interrupt
    s32Ret |= HAL_I2C_DmaCfgIntSet(para_i2c_base, true);
    // set clear done
    s32Ret |= HAL_I2C_DmaTrDone(para_i2c_base, true);

    // set trigger
    s32Ret |= HAL_I2C_DmaTransfer(para_i2c_base);

    return 0;
}

static s32 HAL_I2C_NormalRead(ST_HAL_I2C_BASE *para_i2c_base, u16 para_slvadr, u8 *para_pdata, u32 para_len)
{
    s32 s32Ret   = 0;
    u16 u16Retry = 0;

    // reset iic
    HAL_I2C_ClearInt(para_i2c_base);
    I2C_DELAY_N_US(1);
    // 1.send start
    s32Ret |= HAL_I2C_TranCmdStart(para_i2c_base);
    for (u16Retry = 0; u16Retry < NUMBER_TRANSFER_RETRY; u16Retry++)
    {
        if (HAL_I2C_GetRegFlag(para_i2c_base))
        {
            HAL_I2C_ClearInt(para_i2c_base);
            u16Retry = 0;
            break;
        }
    }
    if (u16Retry >= NUMBER_TRANSFER_RETRY)
    {
        s32Ret = -E_ERR_RETRY;
        dmsg_i2c_halerr("i2c-%d start err %d\n", para_i2c_base->u32Group, s32Ret);
        goto err_rtn;
    }

    // 2.send slave addr <<  1| r_bit
    s32Ret |= HAL_I2C_SendByte(para_i2c_base, para_slvadr);
    if (s32Ret)
    {
        dmsg_i2c_halerr("i2c-%d send slave addr|read = 0x%04x nack from slave err %d\n", para_i2c_base, para_slvadr,
                        s32Ret);
        goto err_rtn;
    }

    // 3.recv data
    s32Ret |= HAL_I2C_RecvBuf(para_i2c_base, para_pdata, para_len);
    if (para_len != s32Ret)
    {
        dmsg_i2c_halerr("i2c-%d read data err %d\n", para_i2c_base->u32Group, s32Ret);
        goto err_rtn;
    }
    dmsg_i2c_halwarn("NORMAL READ SUCCESS DONE\n");
    return 0;
err_rtn:
    return s32Ret;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_tSUtHDSet(ST_HAL_I2C_BASE *para_i2c_base)
{
    I2C_WRITE_REG_MASK(REG_I2C_CNT_TSUTHD_OPEN, REG_MASK_I2C_CNT_TSUTDH_OPEN, REG_MASK_I2C_CNT_TSUTDH_OPEN);
    I2C_WRITE_REG(REG_I2C_CNT_TSU_SET, para_i2c_base->stI2cClkCnt.u16tSU);
    I2C_WRITE_REG(REG_I2C_CNT_THD_SET, para_i2c_base->stI2cClkCnt.u16tHD);
    return 0;
}
/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
s32 HAL_I2C_CntSet(ST_HAL_I2C_BASE *para_i2c_base)
{
    s32 s32Ret = 0;

    s32Ret |= HAL_I2C_ForStpCntSet(para_i2c_base, para_i2c_base->stI2cClkCnt.u16CntForStp);
    s32Ret |= HAL_I2C_HiPerCntSet(para_i2c_base, para_i2c_base->stI2cClkCnt.u16CntHighPerid);
    s32Ret |= HAL_I2C_LowPerCntSet(para_i2c_base, para_i2c_base->stI2cClkCnt.u16CntLowPerid);
    s32Ret |= HAL_I2C_BtwnFlEdgCntSet(para_i2c_base, para_i2c_base->stI2cClkCnt.u16CntBtwnFalEdg);
    s32Ret |= HAL_I2C_ForStarCntSet(para_i2c_base, para_i2c_base->stI2cClkCnt.u16CntForStart);
    s32Ret |= HAL_I2C_LtchTimCntSet(para_i2c_base, para_i2c_base->stI2cClkCnt.u16CntDataLatchTim);
    s32Ret |= HAL_I2C_TimoutOcrCntSet(para_i2c_base, para_i2c_base->stI2cClkCnt.u16CntTimoutIntrDly);
    if (para_i2c_base->stI2cClkCnt.u16tSU | para_i2c_base->stI2cClkCnt.u16tHD)
    {
        s32Ret |= HAL_I2C_tSUtHDSet(para_i2c_base);
    }

    return s32Ret;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_DmaSetup(ST_HAL_I2C_BASE *para_i2c_base)
{
    s32 s32Ret = 0;
    s32Ret |= HAL_I2C_DmaTrDone(para_i2c_base, true);
    s32Ret |= HAL_I2C_DmaAdrModSet(para_i2c_base, para_i2c_base->stI2cDmaCtrl.enDmaAdrMod);

    s32Ret |= HAL_I2C_DmaMiuPriSet(para_i2c_base, para_i2c_base->stI2cDmaCtrl.enDmaMiuPri);

    s32Ret |= HAL_I2C_DmaMiuChnalSet(para_i2c_base, para_i2c_base->stI2cDmaCtrl.enDmaMiuChanel);

    s32Ret |= HAL_I2C_DmaMiuAddrSet(para_i2c_base, para_i2c_base->stI2cDmaCtrl.stDmaMiuAdr.u64DmaAdrMiu);
    I2C_WRITE_REG_MASK(REG_I2C_DMA_CONFIG, 0, REG_MASK_DMACFG_SOFTRST);
    // set interrupt occure after miu move data done completely
    I2C_WRITE_REG_MASK(REG_I2C_DMA_RESERVED, BIT4 | BIT5 | BIT7, REG_MASK_LASTDONE);

    return s32Ret;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_MstSetup(ST_HAL_I2C_BASE *para_i2c_base)
{
    s32 s32Ret = 0;

    // 1.clear interrupt
    s32Ret |= HAL_I2C_ClearInt(para_i2c_base);
    // 2.reset master
    s32Ret |= HAL_I2C_RstSet(para_i2c_base, true);
    s32Ret |= HAL_I2C_RstSet(para_i2c_base, false);
    // 3.config set
    I2C_WRITE_REG(REG_I2C_CONFIG, para_i2c_base->u8I2cConfig);
    if ((HAL_I2CONFIG_OEN_PUSH_EN & para_i2c_base->u8I2cConfig) && para_i2c_base->u8OenCnt)
    {
        I2C_WRITE_REG_MASK(REG_I2C_CNT_TSUTHD_OPEN, BIT6, BIT6);
    }

    s32Ret |= HAL_I2C_DmaCfgSofRstSet(para_i2c_base, true);
    s32Ret |= HAL_I2C_DmaCfgMiuRst(para_i2c_base, true);
    s32Ret |= HAL_I2C_DmaCfgSofRstSet(para_i2c_base, false);
    s32Ret |= HAL_I2C_DmaCfgMiuRst(para_i2c_base, false);
    return s32Ret;
}

s32 HAL_I2C_Release(ST_HAL_I2C_BASE *para_i2c_base)
{
    s32 s32Ret = 0;

    if (0 == para_i2c_base->u32EnDma)
    {
        HAL_I2C_ClearInt(para_i2c_base);
        s32Ret |= HAL_I2C_TranCmdStop(para_i2c_base);

        while (HAL_I2C_DetIdle(para_i2c_base) && (0 == HAL_I2C_GetRegFlag(para_i2c_base)))
            ;

        HAL_I2C_ClearInt(para_i2c_base);
    }

    return s32Ret;
}
/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
s32 HAL_I2C_Write(ST_HAL_I2C_BASE *para_i2c_base, u16 para_slvadr, u8 *para_pdata, u32 para_len)
{
    s32 s32Ret = 0;
    u8  u8SlvAddr;

    u8SlvAddr = (u8)((para_slvadr & 0x00FF) << 1);
    u8SlvAddr &= I2C_SET_WRITEBIT_INDATA;
    if (para_i2c_base->u32EnDma)
    {
        s32Ret |= HAL_I2C_DmaWrite(para_i2c_base, para_slvadr, para_pdata, para_len);
    }
    else
    {
        s32Ret |= HAL_I2C_NormalWrite(para_i2c_base, u8SlvAddr, para_pdata, para_len);
    }

    if (s32Ret)
    {
        s32Ret = -E_ERR_WRITE;
        dmsg_i2c_halwarn("i2c-%d write err\n", para_i2c_base->u32Group);
    }

    return s32Ret;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
s32 HAL_I2C_Read(ST_HAL_I2C_BASE *para_i2c_base, u16 para_slvadr, u8 *para_pdata, u32 para_len)
{
    s32 s32Ret = 0;
    u8  u8SlvAddr;

    u8SlvAddr = (u8)((para_slvadr & 0x00FF) << 1);
    u8SlvAddr |= I2C_SET_READBIT_INDATA;

    if (para_i2c_base->u32EnDma)
    {
        s32Ret |= HAL_I2C_DmaRead(para_i2c_base, para_slvadr, para_pdata, para_len);
    }
    else
    {
        s32Ret |= HAL_I2C_NormalRead(para_i2c_base, u8SlvAddr, para_pdata, para_len);
    }
    if (s32Ret)
    {
        s32Ret = -E_ERR_READ;
        dmsg_i2c_halwarn("i2c-%d read err\n", para_i2c_base->u32Group);
    }
    return s32Ret;
}

/****************************************************/
//*func:
//*description:
//*parameter:
//*return:
/****************************************************/
static s32 HAL_I2C_CalcCnt(ST_HAL_I2C_BASE *paraI2cBase, u32 paraSrcClk, EN_I2C_SPEED_MODE paraSpeedMode_e)
{
    u16 u16LPeridCnt, u16HPeridCnt, u16PeridCnt;
    u32 u32StartCnt = 0;
    u32 u32StopCnt  = 0;
    u8  u8OenSuppl  = 0;

    u16PeridCnt = (paraSrcClk / paraI2cBase->u32Speed);
    switch (paraSpeedMode_e)
    {
        case E_I2C_SPEED_MODE_NORMAL:
            u16PeridCnt -= DRV_I2C_CNT_DEC_12M;
            u16HPeridCnt = (u16PeridCnt / 2);
            u16LPeridCnt = u16PeridCnt - u16HPeridCnt;
            u8OenSuppl   = OEN_WAIT_PUSHPULL_12M;
            break;
        case E_I2C_SPEED_MODE_HIGH:
            u16PeridCnt -= DRV_I2C_CNT_DEC_54M;
            u16HPeridCnt = (u16PeridCnt / 2);
            u16LPeridCnt = u16PeridCnt - u16HPeridCnt;
            u8OenSuppl   = OEN_WAIT_PUSHPULL_54M;
            break;
        case E_I2C_SPEED_MODE_ULTRA:
            u16PeridCnt -= DRV_I2C_CNT_DEC_72M;
            u16HPeridCnt = (u16PeridCnt / 2);
            u16LPeridCnt = u16PeridCnt - u16HPeridCnt;
            u8OenSuppl   = OEN_WAIT_PUSHPULL_72M;
            break;
        default:
            u16LPeridCnt = 0;
            u16HPeridCnt = 0;
            dmsg_i2c_halerr("i2c-%d mode err\n", paraI2cBase->u32Group);
            break;
    } // end of switch(EN_SpeedMode)

    // why we need this calculate,see in hal_iic.h for OEN_WAIT_PUSHPULL_12M
    if ((HAL_I2CONFIG_OEN_PUSH_EN & paraI2cBase->u8I2cConfig) && paraI2cBase->u8OenCnt)
    {
        u16LPeridCnt -= u8OenSuppl;
        u16HPeridCnt -= u8OenSuppl;
    }
    paraI2cBase->stI2cClkCnt.u16CntLowPerid     = u16LPeridCnt;
    paraI2cBase->stI2cClkCnt.u16CntHighPerid    = u16HPeridCnt;
    paraI2cBase->stI2cClkCnt.u16CntBtwnFalEdg   = u16LPeridCnt / 2;
    paraI2cBase->stI2cClkCnt.u16CntDataLatchTim = u16HPeridCnt / 2;

    if (paraI2cBase->u32Speed >= 250000)
    {
        u32StartCnt = u16PeridCnt * (paraI2cBase->u32Speed / 250000) + 1;
        u32StopCnt  = u16PeridCnt * (paraI2cBase->u32Speed / 250000) + 1;
    }
    else
    {
        u32StartCnt = u16PeridCnt / (250000 / paraI2cBase->u32Speed);
        u32StopCnt  = u16PeridCnt / (250000 / paraI2cBase->u32Speed);
    }
    if (0 == paraI2cBase->stI2cClkCnt.u16CntForStart)
    {
        paraI2cBase->stI2cClkCnt.u16CntForStart = (u32StartCnt >= (u32)0xFFFF) ? 0xFFFF : (u16)u32StartCnt;
    }
    if (0 == paraI2cBase->stI2cClkCnt.u16CntForStp)
    {
        paraI2cBase->stI2cClkCnt.u16CntForStp = (u32StopCnt >= (u32)0xFFFF) ? 0xFFFF : (u16)u32StopCnt;
    }

    if (0 == paraI2cBase->stI2cClkCnt.u16tHD)
        paraI2cBase->stI2cClkCnt.u16tHD = paraI2cBase->stI2cClkCnt.u16CntHighPerid;

    if (0 == paraI2cBase->stI2cClkCnt.u16tSU)
        paraI2cBase->stI2cClkCnt.u16tSU = paraI2cBase->stI2cClkCnt.u16CntHighPerid;

    return 0;
}
s32 HAL_I2C_SrclkSelect(ST_HAL_I2C_BASE *para_i2c_base)
{
    u32               u32SrcClk, u32Speed;
    s32               s32Ret = 0;
    EN_I2C_SPEED_MODE EN_SpeedMode;

    u32Speed = para_i2c_base->u32Speed;
    u32Speed = (u32Speed / 1000) * 1000;

    // select source clock
    if (u32Speed <= DRV_I2C_SPEED_200KHZ)
    {
        EN_SpeedMode = E_I2C_SPEED_MODE_NORMAL;
        u32SrcClk    = DRV_I2C_SRCCLK_12M;
    }
    else if (u32Speed <= DRV_I2C_SPEED_700KHZ)
    {
        EN_SpeedMode = E_I2C_SPEED_MODE_HIGH;
        u32SrcClk    = DRV_I2C_SRCCLK_54M;
    }
    else if (u32Speed < DRV_I2C_SPEED_1500KHZ)
    {
        EN_SpeedMode = E_I2C_SPEED_MODE_ULTRA;
        u32SrcClk    = DRV_I2C_SRCCLK_72M;
    }
    else
    {
        EN_SpeedMode = E_I2C_SPEED_MODE_HIGH;
        u32SrcClk    = DRV_I2C_SRCCLK_72M;
        u32Speed     = DRV_I2C_SPEED_1500KHZ;
    }
    para_i2c_base->u32Speed = u32Speed;

    if (para_i2c_base->calbak_i2c_set_srcclk)
    {
        s32Ret = para_i2c_base->calbak_i2c_set_srcclk(para_i2c_base, u32SrcClk);
        if (s32Ret)
        {
            dmsg_i2c_halerr("i2c-%d clock callback func err\n", para_i2c_base->u32Group);
            s32Ret = -E_ERR_SRCCLK_SETUP;
            goto err_set;
        }
    }
    else
    {
        return -E_ERR;
    }
    HAL_I2C_CalcCnt(para_i2c_base, u32SrcClk, EN_SpeedMode);

err_set:
    return s32Ret;
}
s32 HAL_I2C_Init(ST_HAL_I2C_BASE *para_i2c_base)
{
    s32 s32Ret = 0;

    /*reg miic config*/
    para_i2c_base->u8I2cConfig &= 0x00;
    para_i2c_base->u8I2cConfig |= (HAL_I2CONFIG_CLKSTRE_EN | HAL_I2CONFIG_FILTER_EN);
    if (para_i2c_base->u8PushPull)
    {
        para_i2c_base->u8I2cConfig |= HAL_I2CONFIG_OEN_PUSH_EN;
    }
    if (para_i2c_base->u32EnDma)
    {
        para_i2c_base->u8I2cConfig |= HAL_I2CONFIG_DMA_EN | HAL_I2CONFIG_INT_EN | HAL_I2CONFIG_TIMOUT_INT_EN;
    }
    /*reg cnt default val*/
    para_i2c_base->stI2cClkCnt.u16CntBtwnFalEdg    = 5;
    para_i2c_base->stI2cClkCnt.u16CntDataLatchTim  = 5;
    para_i2c_base->stI2cClkCnt.u16CntHighPerid     = 25;
    para_i2c_base->stI2cClkCnt.u16CntLowPerid      = 27;
    para_i2c_base->stI2cClkCnt.u16CntTimoutIntrDly = 0xff;
    /*dma confing*/
    para_i2c_base->stI2cDmaCtrl.enDmaAdrMod    = E_DMA_ADDRMODE_NORMAL;
    para_i2c_base->stI2cDmaCtrl.enDmaMiuChanel = E_DMA_MIU_CHANNEL0;
    para_i2c_base->stI2cDmaCtrl.enDmaMiuPri    = E_DMA_MIUPRI_LOW;
    para_i2c_base->stI2cDmaCtrl.bDmaCfgEnIntr  = true;
    /*set mode of detecting dma done*/
    /*selcet source clk*/
    s32Ret = HAL_I2C_SrclkSelect(para_i2c_base);

    s32Ret |= HAL_I2C_MstSetup(para_i2c_base);
    s32Ret |= HAL_I2C_CntSet(para_i2c_base);
    if (para_i2c_base->u32EnDma)
        s32Ret |= HAL_I2C_DmaSetup(para_i2c_base);

    return s32Ret;
}
