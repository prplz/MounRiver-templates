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


#include <cam_os_wrapper.h>
#include <irqs.h>
#include <region_substract.h>
#include <drv_camclk_Api.h>
#ifdef CONFIG_GPIO_SUPPORT
#include <mdrv_gpio.h>
#endif
#ifdef CONFIG_SYSDESC_SUPPORT
#include <drv_sysdesc.h>
#endif
#include "drv_mspi.h"
#include "hal_mspi.h"
#include "hal_mspireg.h"
#include "hal_mspi_cfg.h"
#include "initcall.h"

#define mspi_dbg 0
#if mspi_dbg == 1
#define mspi_dbgmsg(args...) CamOsPrintf("[MSPI] : " args)
#else
#define mspi_dbgmsg(args...) \
    do                       \
    {                        \
    } while (0)
#endif
#define mspi_errmsg(fmt, ...)  CamOsPrintf("[MSPI] error : " fmt, ##__VA_ARGS__)
#define mspi_warnmsg(fmt, ...) CamOsPrintf("[MSPI] warning : " fmt, ##__VA_ARGS__)


#define SSTAR_SPI_MODE_BITS (SPI_CPOL | SPI_CPHA | SPI_CS_HIGH | SPI_3WIRE | SPI_LSB_FIRST)

#define MSPI_SPEED_DEFAULT  (6000000)

#ifdef CONFIG_ARM64
#define READ_WORD(_reg)        (*(volatile u16 *)(u64)(_reg))
#define WRITE_WORD(_reg, _val) ((*(volatile u16 *)(u64)(_reg)) = (u16)(_val))
#define WRITE_WORD_MASK(_reg, _val, _mask) \
    ((*(volatile u16 *)(u64)(_reg)) = ((*(volatile u16 *)(u64)(_reg)) & ~(_mask)) | ((u16)(_val) & (_mask)))
#else
#define READ_WORD(_reg)        (*(volatile u16 *)(u32)(_reg))
#define WRITE_WORD(_reg, _val) ((*(volatile u16 *)(u32)(_reg)) = (u16)(_val))
#define WRITE_WORD_MASK(_reg, _val, _mask) \
    ((*(volatile u16 *)(u32)(_reg)) = ((*(volatile u16 *)(u32)(_reg)) & ~(_mask)) | ((u16)(_val) & (_mask)))
#endif

struct mspi_clk
{
    u8  clk_src;
    u8  clk_div;
    u32 clk_rate;
};

struct mspi_clk_tbl
{
    u32 *            clk_src_tbl;
    u32              clk_src_tbl_sz;
    u32 *            clk_div_tbl;
    u32              clk_div_tbl_sz;
    u32              clk_cfg_tbl_sz;
    struct mspi_clk *clk_cfg_tbl;
};

struct sstar_mspi
{
    u8        group;
    u16       padmux;
    u8        inited;
    int       irq_num;
    char      irq_name[20];
    int       len;
    const u8 *tx_buf;
    u8 *      rx_buf;

    u16 mode;
    u16 cs_num;
    s16 bus_num;
    u32 use_dma;
    u32 xfer_dma;
    u32 mosi_pad;
    u32 mosi_mode;
    u32 _4to3mode;
    u32 bits_per_word;
    u32 speed_hz;
    u32 max_speed_hz;

    u32          word_size;
    CamOsTsem_t  tsem_id;

#ifdef CONFIG_CAM_CLK
    void *camclk;
#endif

    struct mspi_hal         hal;
    struct mspi_cfg_t       *cfg;
    struct mspi_clk_tbl     mspi_clk_tbl;
};

static u32 sstar_spi_clk_div_tbl[] = MSPI_CLK_DIV_VAL;

static struct sstar_mspi sstar_mspi_ctrl[SSTAR_MSPI_BUS_MAX];

static struct sstar_mspi* spi_get_ctrl(u8 bus_num)
{
    if(bus_num >= SSTAR_MSPI_BUS_MAX)
    {
        return NULL;
    }
    return &sstar_mspi_ctrl[bus_num];
}

static void spi_interrupt(u32 irq, void *dev_id)
{
    struct sstar_mspi *sstar_spimst = (struct sstar_mspi *)dev_id;
    int                uDoneFlag    = 0;

    uDoneFlag = HAL_MSPI_CheckDone(&sstar_spimst->hal);

    if (uDoneFlag == 1)
    {
        HAL_MSPI_ClearDone(&sstar_spimst->hal);
        CamOsTsemUp(&sstar_spimst->tsem_id);
        mspi_dbgmsg("spi%d done\n", sstar_spimst->bus_num);
    }
}

static s32 spi_waitdone(struct mspi_hal *mspi)
{
    CamOsRet_e CamRet = CAM_OS_OK;
    struct sstar_mspi  *spimst = container_of(mspi, struct sstar_mspi, hal);

    CamRet = CamOsTsemTimedDown(&spimst->tsem_id, MSTAR_SPI_TIMEOUT_MS);
    if (CAM_OS_TIMEOUT == CamRet)
    {
        mspi_errmsg("mspi  CAM_OS_TIMEOUT!\n");
        return CAM_OS_TIMEOUT;
    }
    else if (CAM_OS_FAIL == CamRet)
    {
        mspi_errmsg("mspi  CAM_OS_FAIL!\n");
        return CAM_OS_FAIL;
    }
    else
    {
        mspi_dbgmsg("mspi DONE!\n");
    }

    return CamRet;
}

#ifndef CONFIG_PADMUX_SUPPORT
static s32 spi_padmux_set(struct sstar_mspi *spimst)
{
    struct mspi_cfg_t *cfg = spimst->cfg;
#ifndef CONFIG_GPIO_SUPPORT
    u8 i;
#endif

    if (!cfg->padmod)
        return SPI_INVALID_PADMUX;

#ifdef CONFIG_GPIO_SUPPORT

    if(MDrv_GPIO_PadGroupMode_Set(cfg->padmod))
        return SPI_PADMUX_SET_ERROR;
#else

    for (i = 0; i < cfg->regs->padmux_num; i++)
    {
        WRITE_WORD_MASK(cfg->regs->reg_padmod[i].bank_base + (cfg->regs->reg_padmod[i].reg_offset << 2),
                        cfg->padmod << cfg->regs->reg_padmod[i].bit_shift,
                        cfg->regs->reg_padmod[i].bit_mask);
    }
#endif
    return SPI_SUCCESS;
}
#endif

static void spi_clk_tbl_init(struct mspi_clk_tbl *clk_tbl)
{
    u8              i   = 0;
    u8              j   = 0;
    u32             clk = 0;
    struct mspi_clk temp;

    memset(&temp, 0, sizeof(struct mspi_clk));
    memset(clk_tbl->clk_cfg_tbl, 0, sizeof(struct mspi_clk) * clk_tbl->clk_cfg_tbl_sz);

    for (i = 0; i < clk_tbl->clk_src_tbl_sz; i++)
    {
        for (j = 0; j < clk_tbl->clk_div_tbl_sz; j++)
        {
            clk                                                            = clk_tbl->clk_src_tbl[i];
            clk_tbl->clk_cfg_tbl[j + clk_tbl->clk_div_tbl_sz * i].clk_src  = i;
            clk_tbl->clk_cfg_tbl[j + clk_tbl->clk_div_tbl_sz * i].clk_div  = j;
            clk_tbl->clk_cfg_tbl[j + clk_tbl->clk_div_tbl_sz * i].clk_rate = clk / clk_tbl->clk_div_tbl[j];
        }
    }

    for (i = 0; i < clk_tbl->clk_cfg_tbl_sz; i++)
    {
        for (j = i; j < clk_tbl->clk_cfg_tbl_sz; j++)
        {
            if (clk_tbl->clk_cfg_tbl[i].clk_rate > clk_tbl->clk_cfg_tbl[j].clk_rate)
            {
                memcpy(&temp, &clk_tbl->clk_cfg_tbl[i], sizeof(struct mspi_clk));

                memcpy(&clk_tbl->clk_cfg_tbl[i], &clk_tbl->clk_cfg_tbl[j], sizeof(struct mspi_clk));

                memcpy(&clk_tbl->clk_cfg_tbl[j], &temp, sizeof(struct mspi_clk));
            }
        }
    }

    for (i = 0; i < clk_tbl->clk_cfg_tbl_sz; i++)
    {
        mspi_dbgmsg("clk_cfg_tbl[%d].clk_cfg  = %d\n", i, clk_tbl->clk_cfg_tbl[i].clk_src);
        mspi_dbgmsg("clk_cfg_tbl[%d].clk_div  = %d\n", i, clk_tbl->clk_cfg_tbl[i].clk_div);
        mspi_dbgmsg("clk_cfg_tbl[%d].clk_rate = %d\n", i, clk_tbl->clk_cfg_tbl[i].clk_rate);
    }
}

static u32 spi_set_clock(struct sstar_mspi *spimst, u32 clock)
{
    u8 i = 0;
#ifdef CONFIG_CAM_CLK
    u8                   cam_on;
    CAMCLK_Set_Attribute stSetCfg;
#else

#endif
    struct mspi_clk_tbl *clk_tbl = &spimst->mspi_clk_tbl;

    for (i = 0; i < clk_tbl->clk_cfg_tbl_sz; i++)
    {
        if (clock < clk_tbl->clk_cfg_tbl[i].clk_rate)
        {
            break;
        }
    }

    if (i >= 1)
    {
        i--;
    }
    else
    {
        return 0;
    }

#ifdef CONFIG_CAM_CLK
    if (CamClkGetOnOff(spimst->camclk, &cam_on))
    {
        mspi_errmsg("cam clk 0 on get fail\n");
        return 0;
    }
    if (!cam_on)
    {
        if (CamClkSetOnOff(spimst->camclk, 1))
        {
            mspi_errmsg("cam clk 0 on set fail\n");
            return 0;
        }
    }
    CAMCLK_SETRATE_ROUNDUP(stSetCfg, clk_tbl->clk_src_tbl[clk_tbl->clk_cfg_tbl[i].clk_src]);
    if (CamClkAttrSet(spimst->camclk, &stSetCfg))
    {
        mspi_errmsg("cam clk 0 rate set fail\n");
        return 0;
    }
#else
    WRITE_WORD_MASK((mspi_cfg[spimst->group].regs->reg_clkgen.bank_base + (mspi_cfg[spimst->group].regs->reg_clkgen.reg_offset << 2)),
                    spi_src_clk[clk_tbl->clk_cfg_tbl[i].clk_src].value << mspi_cfg[spimst->group].regs->reg_clkgen.bit_shift,
                    mspi_cfg[spimst->group].regs->reg_clkgen.bit_mask);
#endif

    HAL_MSPI_SetDivClk(&spimst->hal, clk_tbl->clk_cfg_tbl[i].clk_div);

    mspi_dbgmsg("calc config  : %04d\n", clk_tbl->clk_cfg_tbl[i].clk_src);
    mspi_dbgmsg("calc div     : %04d\n", clk_tbl->clk_cfg_tbl[i].clk_div);
    mspi_dbgmsg("calc rate    : %d\n", clk_tbl->clk_cfg_tbl[i].clk_rate);

    return clk_tbl->clk_cfg_tbl[i].clk_rate;
}

s32 spi_setup(struct spi_setup *setup)
{
    s32 ret = 0;
    u32 new_clock = 0;
    struct sstar_mspi *sstar_spimst = spi_get_ctrl(setup->bus_num);

    mspi_dbgmsg("setup channel : %d\n", sstar_spimst->bus_num);

    if(setup->mode & ~SSTAR_SPI_MODE_BITS)
    {
        return SPI_INVALID_MODE;
    }

    if (sstar_spimst->mode != setup->mode)
    {
        sstar_spimst->mode = setup->mode;

        ret = HAL_MSPI_SetMode(&sstar_spimst->hal, sstar_spimst->mode & (SPI_CPHA | SPI_CPOL));
        if (ret)
        {
            return SPI_INVALID_TRF_MODE;
        }

        HAL_MSPI_SetLSB(&sstar_spimst->hal, (sstar_spimst->mode & SPI_LSB_FIRST) >> 3);
        HAL_MSPI_ChipSelect(&sstar_spimst->hal, ((setup->mode & SPI_CS_HIGH) == SPI_CS_HIGH), setup->chip_select);
        HAL_MSPI_Set3WireMode(&sstar_spimst->hal, (sstar_spimst->mode & SPI_3WIRE));

        mspi_dbgmsg("setup mode : 0x%x\n", sstar_spimst->mode);
    }

    if (setup->max_speed_hz > sstar_spimst->max_speed_hz)
    {
        setup->max_speed_hz = sstar_spimst->max_speed_hz;
    }

    if (sstar_spimst->speed_hz != setup->max_speed_hz)
    {
        new_clock = spi_set_clock(sstar_spimst, setup->max_speed_hz);
        if (new_clock > 0)
        {
            setup->max_speed_hz    = new_clock;
            sstar_spimst->speed_hz = setup->max_speed_hz;
        }
        else
        {
            return SPI_INVALID_SPEED;
        }

        mspi_dbgmsg("setup speed  : %d\n", sstar_spimst->speed_hz);
    }

    return SPI_SUCCESS;
}

static s32 spi_start_transfer(struct spi_setup *setup, struct spi_tfr *tfr)
{
    s32 ret;
    struct sstar_mspi *sstar_spimst = spi_get_ctrl(setup->bus_num);

    if(!sstar_spimst)
    {
        return SPI_INVALID_BUS;
    }

    sstar_spimst->tx_buf = tfr->tx_buf;
    sstar_spimst->rx_buf = tfr->rx_buf;
    sstar_spimst->len    = tfr->len;

    HAL_MSPI_ChipSelect(&sstar_spimst->hal, !((sstar_spimst->mode & SPI_CS_HIGH) == SPI_CS_HIGH), setup->chip_select);

    /*
     which I/O buffers are used ... each spi_transfer wraps a
     buffer for each transfer direction, supporting full duplex
     (two pointers, maybe the same one in both cases) and half
     duplex (one pointer is NULL) transfers;
    */

    if (sstar_spimst->tx_buf != NULL && sstar_spimst->rx_buf != NULL)
    {
        HAL_MSPI_FullDuplex(setup->bus_num, &sstar_spimst->hal, (u8 *)sstar_spimst->rx_buf,
                            (u8 *)sstar_spimst->tx_buf, (u16)sstar_spimst->len);
    }
    else if (sstar_spimst->tx_buf != NULL)
    {
        if (sstar_spimst->xfer_dma)
        {
            ret = HAL_MSPI_DMA_Write(setup->bus_num, &sstar_spimst->hal, (u8 *)sstar_spimst->tx_buf,
                                     (u32)sstar_spimst->len);
            if (ret)
            {
                return SPI_DMA_WRITE_ERROR;
            }
        }
        else
        {
            ret = HAL_MSPI_Write(setup->bus_num, &sstar_spimst->hal, (u8 *)sstar_spimst->tx_buf,
                                 (u32)sstar_spimst->len);
            if (ret)
            {
                return SPI_BUFFER_WRITE_ERROR;
            }
        }
    }
    else if (sstar_spimst->rx_buf != NULL)
    {
        if (sstar_spimst->_4to3mode)
        {
#ifdef CONFIG_GPIO_SUPPORT
            MDrv_GPIO_PadVal_Set(sstar_spimst->mosi_pad, PINMUX_FOR_GPIO_MODE);
            MDrv_GPIO_Pad_Odn(sstar_spimst->mosi_pad);
#endif
        }
        if (sstar_spimst->xfer_dma)
        {
            ret = HAL_MSPI_DMA_Read(setup->bus_num, &sstar_spimst->hal, (u8 *)sstar_spimst->rx_buf,
                                    (u32)sstar_spimst->len);
            if (ret)
            {
                return SPI_DMA_READ_ERROR;
            }
        }
        else
        {
            ret = HAL_MSPI_Read(setup->bus_num, &sstar_spimst->hal, (u8 *)sstar_spimst->rx_buf,
                                (u32)sstar_spimst->len);
            if (ret)
            {
                return SPI_BUFFER_READ_ERROR;
            }
        }
        if (sstar_spimst->_4to3mode)
        {
#ifdef CONFIG_GPIO_SUPPORT
            MDrv_GPIO_PadVal_Set(sstar_spimst->mosi_pad, sstar_spimst->mosi_mode);
#endif
        }
    }

    return SPI_SUCCESS;
}

static s32 spi_finish_transfer(struct spi_setup *setup, struct spi_tfr *tfr, bool cs_change)
{
    struct sstar_mspi *sstar_spimst = spi_get_ctrl(setup->bus_num);

    if (tfr->delay_usecs)
        CamOsUsDelay(tfr->delay_usecs);

    if (cs_change)
    {
        /* Clear TA flag */
        HAL_MSPI_ChipSelect(&sstar_spimst->hal, ((setup->mode & SPI_CS_HIGH) == SPI_CS_HIGH), setup->chip_select);
    }
    return 0;
}

s32 spi_transfer(struct spi_setup *setup, struct spi_tfr * tfr, u32 length)
{
    s32 ret;
    u32 tfr_idx;
    u8  cs_change;
    struct sstar_mspi *  sstar_spimst = spi_get_ctrl(setup->bus_num);

    for (tfr_idx = 0; tfr_idx < length; tfr_idx++, tfr++)
    {
        if (sstar_spimst->bits_per_word != tfr->bits_per_word)
        {
            if (sstar_spimst->bits_per_word != tfr->bits_per_word)
            {
                sstar_spimst->xfer_dma          = (tfr->bits_per_word % 8 == 0) ? sstar_spimst->use_dma : false;
                sstar_spimst->bits_per_word     = tfr->bits_per_word;
                sstar_spimst->hal.bits_per_word = tfr->bits_per_word;
                if (tfr->bits_per_word > MSPI_MAX_SUPPORT_BITS)
                {
                    return SPI_INVALID_WORD_BITS;
                }
                else if (tfr->bits_per_word > 8)
                {
                    sstar_spimst->word_size = 2;
                }
                else
                {
                    sstar_spimst->word_size = 1;
                }
                HAL_MSPI_SET_FRAMECFG(&sstar_spimst->hal, tfr->bits_per_word);

                mspi_dbgmsg("setup bits : %d\n", sstar_spimst->bits_per_word);
            }
        }

        if (tfr->len % sstar_spimst->word_size != 0)
        {
            return SPI_INVALID_TRANSFER_LENGTH;
        }

        ret = spi_start_transfer(setup, tfr);
        if (ret)
        {
            mspi_dbgmsg("start_transfer err\n");
            return ret;
        }

        cs_change = tfr->cs_change || (tfr_idx == (length - 1));

        mspi_dbgmsg("cs_change %d\n", cs_change);

        ret = spi_finish_transfer(setup, tfr, cs_change);
        if (ret)
        {
            mspi_dbgmsg("finish transfer err\n");
            return ret;
        }
    }
    return SPI_SUCCESS;
}

s32 spi_ctrl_init(u8 group)
{
    u32 i;
    s32 ret;
    u8  irq_num;
    u8  use_dma;
    u8  cs_num;
    u16 pad_mux;
    u8  enable = 1;
    u8  _4to3_mode;
#ifdef CONFIG_GPIO_SUPPORT
    u16 ext_cs_num;
#endif
    u32 clk_out_mode;

    u32 bank_base;
#ifdef CONFIG_CAM_CLK
    u16 camclk_id;
    u8  num_parents;
    u8  cam_name[32];
    CAMCLK_Get_Attribute stGetCfg;
#endif

    if(group >= SSTAR_MSPI_BUS_MAX)
        return SPI_INVALID_BUS;

#ifdef CONFIG_SYSDESC_SUPPORT
    if(mspi_cfg[group].dev_id == SYSDESC_DEV_NO_DEFINE)
        enable = 0;
#ifdef SYSDESC_PRO_status_u8
    if (E_SYS_DESC_PASS != MDrv_SysDesc_Read_U8(mspi_cfg[group].dev_id, SYSDESC_PRO_status_u8, &enable))
        enable = 0;
#else
    enable = 0;
#endif
#else
    if(!mspi_cfg[group].enable)
        enable = 0;
#endif

    if(!enable)
        return SPI_SUCCESS;

    if(sstar_mspi_ctrl[group].inited)
        return SPI_SUCCESS;

#ifdef CONFIG_SYSDESC_SUPPORT

#ifdef SYSDESC_PRO_reg_u32_u16
    ret = MDrv_SysDesc_Read_U32_Array(mspi_cfg[group].dev_id, SYSDESC_PRO_reg_u32_u16, &bank_base, 1);
    if (ret)
        return ret;
#else
    mspi_errmsg("mspi%d get reg property fail\n", group);
    return SPI_SYSDESC_REG_ERROR;
#endif

#ifdef SYSDESC_PRO_interrupts_u8
    ret = MDrv_SysDesc_Read_U8(mspi_cfg[group].dev_id, SYSDESC_PRO_interrupts_u8, &irq_num);
    if(ret)
        return ret;
#else
    mspi_errmsg("mspi%d get interrupts property fail\n", group);
    return SPI_SYSDESC_INTERRUPT_ERROR;
#endif

#ifdef SYSDESC_PRO_dma_u8
    ret = MDrv_SysDesc_Read_U8(mspi_cfg[group].dev_id, SYSDESC_PRO_dma_u8, &use_dma);
    if(ret)
        use_dma = 0;
#else
    use_dma = 0;
#endif

#ifdef SYSDESC_PRO_clk_out_mode_u32
    ret = MDrv_SysDesc_Read_U32(mspi_cfg[group].dev_id, SYSDESC_PRO_clk_out_mode_u32, &clk_out_mode);
    if(ret)
        clk_out_mode = 0;
#else
    clk_out_mode = 0;
#endif

#ifdef SYSDESC_PRO_cs_num_u8
    ret = MDrv_SysDesc_Read_U8(mspi_cfg[group].dev_id, SYSDESC_PRO_cs_num_u8, &cs_num);
    if(ret)
        cs_num = 1;
#else
    cs_num = 1;
#endif

#ifdef SYSDESC_PRO_cs_ext_u32
    ret = MDrv_SysDesc_GetContentOfLen(mspi_cfg[group].dev_id, SYSDESC_PRO_cs_ext_u32, &ext_cs_num);
    if(ret)
        ext_cs_num = 0;

    if(ext_cs_num)
    {
        mspi_dbgmsg("ext_cs_num %d\n", ext_cs_num);
        ret = MDrv_SysDesc_Read_U32_Array(mspi_cfg[group].dev_id, SYSDESC_PRO_cs_ext_u32, sstar_mspi_ctrl[group].hal.cs_ext, ext_cs_num/sizeof(u32));
        if(ret)
            return ret;
    }
#else
    ext_cs_num = 0;
#endif

#ifdef SYSDESC_PRO_4to3_mode_u8
    _4to3_mode = MDrv_SysDesc_Read_U8(mspi_cfg[group].dev_id, SYSDESC_PRO_4to3_mode_u8, &_4to3_mode);
    if(ret)
        _4to3_mode = 0;
#else
    _4to3_mode = 0;
#endif

#ifdef SYSDESC_PRO_pad_mux_u16
    ret = MDrv_SysDesc_Read_U16(mspi_cfg[group].dev_id, SYSDESC_PRO_pad_mux_u16, &pad_mux);
    if(ret)
        pad_mux = 0;
#else
    pad_mux = 0;
#endif

#else
    irq_num = mspi_cfg[group].irq_num;
    use_dma = mspi_cfg[group].usedma;
    bank_base = mspi_cfg[group].regs->bank_base;
    cs_num  = mspi_cfg[group].cs_num;
#ifdef CONFIG_GPIO_SUPPORT
    ext_cs_num = mspi_cfg[group].cs_ext_num;
    if(ext_cs_num > 16)
        return SPI_INVALID_EXT_CS_NUM;
    for (i = 0; i < ext_cs_num; i++)
        sstar_mspi_ctrl[group].hal.cs_ext[i] = mspi_cfg[group].cs_ext[i];
#endif
    clk_out_mode = mspi_cfg[group].clk_out_mode;
    _4to3_mode = mspi_cfg[group]._4to3_mode;
    pad_mux = mspi_cfg[group].padmod;
#endif

    sstar_mspi_ctrl[group].group = group;
    sstar_mspi_ctrl[group].irq_num = irq_num;
    sstar_mspi_ctrl[group].use_dma = use_dma;
    sstar_mspi_ctrl[group].hal.mspi_base = bank_base;
    sstar_mspi_ctrl[group].hal.cs_num = cs_num;
#ifdef CONFIG_GPIO_SUPPORT
    sstar_mspi_ctrl[group].cs_num = cs_num + ext_cs_num;
#else
    sstar_mspi_ctrl[group].cs_num = cs_num;
#endif
    sstar_mspi_ctrl[group].hal.clk_out_mode = clk_out_mode;
    sstar_mspi_ctrl[group]._4to3mode = _4to3_mode;
    sstar_mspi_ctrl[group].padmux = pad_mux;
    sstar_mspi_ctrl[group].hal.calbak_mspi_waitdone = spi_waitdone;
    sstar_mspi_ctrl[group].cfg = &mspi_cfg[group];

    if (!CamOsSnprintf(sstar_mspi_ctrl[group].irq_name, sizeof(sstar_mspi_ctrl[group].irq_name), "mspi%d_Isr", group))
    {
        mspi_errmsg("find irq reformat failed\n");
    }

    mspi_dbgmsg("group %d irq number %d\n", group, irq_num);

    ret = CamOsIrqRequest(sstar_mspi_ctrl[group].irq_num, spi_interrupt, sstar_mspi_ctrl[group].irq_name, (void *)&sstar_mspi_ctrl[group]);
    if (ret == 0)
    {
        mspi_dbgmsg("%s registered\n", sstar_mspi_ctrl[group].irq_name);
    }
    else
    {
        mspi_errmsg("%s register failed", sstar_mspi_ctrl[group].irq_name);
        return ret;
    }

#ifdef CONFIG_CAM_CLK
#ifdef CONFIG_SYSDESC_SUPPORT
    ret = MDrv_SysDesc_Read_U16(mspi_cfg[group].dev_id, SYSDESC_PRO_camclk_u16, &camclk_id);
    if(ret)
        return ret;
#else
    camclk_id = mspi_cfg[group].camclk_id£»
#endif
    ret = CamOsSnprintf((char*)cam_name, sizeof(cam_name), "mspi%d camclk", group);
    if(!ret)
    {
        mspi_errmsg("camclk name of mspi %d snprinf fail\n", group);
        return ret;
    }

    ret = CamClkRegister(cam_name, camclk_id, &sstar_mspi_ctrl[group].camclk);
    if (ret)
    {
        mspi_errmsg("register mspi %d camclk fail\n", group);
        return ret;
    }

    ret = CamClkAttrGet(sstar_mspi_ctrl[group].camclk, &stGetCfg);
    if (ret)
    {
        mspi_errmsg("get camclk att fail\n");
        return ret;
    }

    num_parents = stGetCfg.u32NodeCount;
    mspi_dbgmsg("parents n = %d\n", num_parents);
    sstar_mspi_ctrl[group].mspi_clk_tbl.clk_src_tbl_sz = num_parents;
    sstar_mspi_ctrl[group].mspi_clk_tbl.clk_src_tbl =
        CamOsMemAlloc(sizeof(u32) * sstar_mspi_ctrl[group].mspi_clk_tbl.clk_src_tbl_sz);
    if (!sstar_mspi_ctrl[group].mspi_clk_tbl.clk_src_tbl)
    {
        ret = -1;
        mspi_errmsg("malloc clk_src_tbl fail\n");
        goto err_src;
    }
    for (i = 0; i < num_parents; i++)
    {
        sstar_mspi_ctrl[group].mspi_clk_tbl.clk_src_tbl[i] = CamClkRateGet(stGetCfg.u32Parent[i]);
        mspi_dbgmsg("clk src %d = %d\n", i, sstar_mspi_ctrl[group].mspi_clk_tbl.clk_src_tbl[i]);
    }
#else
    sstar_mspi_ctrl[group].mspi_clk_tbl.clk_src_tbl_sz = SPI_SRC_CLK_SIZE;
    sstar_mspi_ctrl[group].mspi_clk_tbl.clk_src_tbl =
        CamOsMemAlloc(sizeof(u32) * sstar_mspi_ctrl[group].mspi_clk_tbl.clk_src_tbl_sz);
    if (!sstar_mspi_ctrl[group].mspi_clk_tbl.clk_src_tbl)
    {
        ret = -1;
        mspi_errmsg("malloc clk_src_tbl fail\n");
        goto err_src;
    }
    for (i = 0; i < SPI_SRC_CLK_SIZE; i++)
    {
        sstar_mspi_ctrl[group].mspi_clk_tbl.clk_src_tbl[i] = spi_src_clk[i].rate;
    }
#endif

    // clock table divide table cfg
    sstar_mspi_ctrl[group].mspi_clk_tbl.clk_div_tbl_sz = sizeof(sstar_spi_clk_div_tbl)/sizeof(u32);
    sstar_mspi_ctrl[group].mspi_clk_tbl.clk_div_tbl = sstar_spi_clk_div_tbl;

    for (i = 0; i < sizeof(sstar_spi_clk_div_tbl)/sizeof(u32); i++)
    {
        mspi_dbgmsg("clk div %d = %d\n", i, sstar_mspi_ctrl[group].mspi_clk_tbl.clk_div_tbl[i]);
    }

    // clock table config table malloc
    sstar_mspi_ctrl[group].mspi_clk_tbl.clk_cfg_tbl_sz =
        sstar_mspi_ctrl[group].mspi_clk_tbl.clk_div_tbl_sz * sstar_mspi_ctrl[group].mspi_clk_tbl.clk_src_tbl_sz;
    sstar_mspi_ctrl[group].mspi_clk_tbl.clk_cfg_tbl =
        CamOsMemAlloc(sizeof(struct mspi_clk) * sstar_mspi_ctrl[group].mspi_clk_tbl.clk_cfg_tbl_sz);
    if (!sstar_mspi_ctrl[group].mspi_clk_tbl.clk_cfg_tbl)
    {
        ret = -2;
        mspi_errmsg("malloc clk_cfg_tbl fail\n");
        goto err_cfg;
    }

    /* initialise the clock calc table for calc closest clock */
    spi_clk_tbl_init(&sstar_mspi_ctrl[group].mspi_clk_tbl);

    sstar_mspi_ctrl[group].max_speed_hz =
        sstar_mspi_ctrl[group].mspi_clk_tbl.clk_cfg_tbl[sstar_mspi_ctrl[group].mspi_clk_tbl.clk_cfg_tbl_sz - 1].clk_rate;
    mspi_dbgmsg("max_speed_hz = %d\n", sstar_mspi_ctrl[group].max_speed_hz);

    if (clk_out_mode)
    {
        if (!spi_set_clock(&sstar_mspi_ctrl[group], clk_out_mode))
        {
            goto err_cfg;
        }
    }

    /* initialise the hardware */
    ret = HAL_MSPI_Config(&sstar_mspi_ctrl[group].hal);
    if (ret)
    {
        ret = -3;
        mspi_errmsg("config mspi%d master: %d\n", group, ret);
        goto err_cfg;
    }

#ifndef CONFIG_PADMUX_SUPPORT
    ret = spi_padmux_set(&sstar_mspi_ctrl[group]);
    if (ret)
    {
        mspi_errmsg("mspi%d padmux set: %d\n", group, ret);
        goto err_cfg;
    }
#endif

    CamOsTsemInit(&sstar_mspi_ctrl[group].tsem_id, 0);

    sstar_mspi_ctrl[group].inited = 1;
    return 0;

err_cfg:
    CamOsMemRelease(sstar_mspi_ctrl[group].mspi_clk_tbl.clk_cfg_tbl);
err_src:
    CamOsMemRelease(sstar_mspi_ctrl[group].mspi_clk_tbl.clk_src_tbl);
    return ret;
}


static void sstar_spi_init(void)
{
    s32 ret;
    u8  group;
    for (group = 0; group < SSTAR_MSPI_BUS_MAX; group++)
    {
        ret = spi_ctrl_init(group);
        if(ret)
        {
            CamOsPrintf("[SPI] Controller %d init fail : %d\n", group, ret);
            continue;
        }
    }
}

rtos_application_initcall(sstar_spi_init, 0);

