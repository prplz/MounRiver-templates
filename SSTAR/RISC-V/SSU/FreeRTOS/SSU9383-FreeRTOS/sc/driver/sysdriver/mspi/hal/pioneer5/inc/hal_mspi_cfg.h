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
#include <camclk_id.h>
#include <gpio.h>

#ifdef CONFIG_ARM64
#define READ_WORD(_reg)        (*(volatile u16 *)(u64)(_reg))
#define WRITE_WORD(_reg, _val) ((*(volatile u16 *)(u64)(_reg)) = (u16)(_val))
#define WRITE_WORD_MASK(_reg, _val, _mask)    \
     ((*(volatile u16 *)(u64)(_reg)) = \
          ((*(volatile u16 *)(u64)(_reg)) & ~(_mask)) | ((u16)(_val) & (_mask)))
#else
#define READ_WORD(_reg)        (*(volatile u16 *)(u32)(_reg))
#define WRITE_WORD(_reg, _val) ((*(volatile u16 *)(u32)(_reg)) = (u16)(_val))
#define WRITE_WORD_MASK(_reg, _val, _mask) \
     ((*(volatile u16 *)(u32)(_reg)) = ((*(volatile u16 *)(u32)(_reg)) & ~(_mask)) | ((u16)(_val) & (_mask)))
#endif


#define MSTAR_SPI_TIMEOUT_MS 30000

#define SYSDESC_DEV_NO_DEFINE   0xFFFF

struct clk_info
{
    u32 rate;
    u16 value;
};

struct reg_t{
    u64 bank_base;
    u8  reg_offset;
    u8  bit_shift;
    u32 bit_mask;
};

struct mspi_reg_t
{
#ifndef CONFIG_SYSDESC_SUPPORT
    u64          bank_base;
#endif

#ifndef CONFIG_CAM_CLK
    struct reg_t reg_clkgen;
#endif

#ifndef CONFIG_PADMUX_SUPPORT
    u8 padmux_num;
    struct reg_t reg_padmod[];
#endif
};

struct mspi_cfg_t
{
#ifndef CONFIG_PADMUX_SUPPORT
    u16  padmod;
#endif
#ifdef CONFIG_SYSDESC_SUPPORT
    u16  dev_id;
#else
    u8   enable;
    u8   cs_num;
    int  irq_num;
    u32  usedma;
    u32  clk_out_mode;
    bool _4to3_mode;
#ifdef CONFIG_GPIO_SUPPORT
    u16  pad_mosi;
    u16  cs_ext_num;
    u32  cs_ext[16];
#endif
#ifdef CONFIG_CAM_CLK
    u16  camclk_id;
#endif
#endif

#if !defined(CONFIG_SYSDESC_SUPPORT) || !defined(CONFIG_CAM_CLK) || !defined(CONFIG_PADMUX_SUPPORT)
    struct mspi_reg_t *regs;
#endif

};

#if !defined(CONFIG_SYSDESC_SUPPORT) || !defined(CONFIG_CAM_CLK) || !defined(CONFIG_PADMUX_SUPPORT)
// mspi0
struct mspi_reg_t mspi0_regs =
{
#ifndef CONFIG_SYSDESC_SUPPORT
    .bank_base = 0x2222000,
#endif

#ifndef CONFIG_CAM_CLK
    .reg_clkgen =
    {
        .bank_base  = 0x2207000,
        .reg_offset = 0x33,
        .bit_shift  = 0,
        .bit_mask   = (BIT4 | BIT3 | BIT2 | BIT1 | BIT0)
    },
#endif

#ifndef CONFIG_PADMUX_SUPPORT
    .padmux_num = 2,
    .reg_padmod =
    {
        [0] =
        {
            .bank_base  = 0x2207800,
            .reg_offset = 0x68,
            .bit_shift  = 0,
            .bit_mask   = (BIT1 | BIT0)
        },

        [1] =
        {
            .bank_base  = 0x2207800,
            .reg_offset = 0x70,
            .bit_shift  = 4,
            .bit_mask   = (BIT5 | BIT4)
        }
    }
#endif

};

struct mspi_reg_t mspi1_regs =
{
#ifndef CONFIG_SYSDESC_SUPPORT
    .bank_base = 0x2222200,
#endif

#ifndef CONFIG_CAM_CLK
    .reg_clkgen =
    {
        .bank_base  = 0x2207000,
        .reg_offset = 0x33,
        .bit_shift  = 8,
        .bit_mask   = (BIT12 | BIT11 | BIT10 | BIT9 | BIT8)
    },
#endif

#ifndef CONFIG_PADMUX_SUPPORT
    .padmux_num = 2,
    .reg_padmod =
    {
        [0] =
        {
            .bank_base  = 0x2207800,
            .reg_offset = 0x68,
            .bit_shift  = 3,
            .bit_mask   = (BIT5 | BIT4 | BIT3)
        },

        [1] =
        {
            .bank_base  = 0x2207800,
            .reg_offset = 0x68,
            .bit_shift  = 6,
            .bit_mask   = (BIT7 | BIT6)
        }
    }
#endif

};

#endif

struct mspi_cfg_t mspi_cfg[] =
{
    // mspi0
    [0] =
    {
#ifdef CONFIG_SYSDESC_SUPPORT
#ifdef SYSDESC_DEV_mspi0
        .dev_id       = SYSDESC_DEV_mspi0,
#else
        .dev_id       = SYSDESC_DEV_NO_DEFINE,
#endif
#else
        .enable       = 1,
        .cs_num       = 2,
        .irq_num      = INT_IRQ_MSPI_0,
        .usedma       = 1,
        .clk_out_mode = 0,
        ._4to3_mode   = 0,
#ifdef CONFIG_GPIO_SUPPORT
        .cs_ext_num   = 2,
        .cs_ext       =
        {
            PAD_EMMC_CLK, PAD_EMMC_D1
        },
#endif

#ifndef CONFIG_PADMUX_SUPPORT
#ifdef CONFIG_GPIO_SUPPORT
        .padmod = PINMUX_FOR_SPI0_MODE_2,
#else
        .padmod = 2,
#endif
#endif


#ifdef CONFIG_CAM_CLK
        .camclk_id  = CAMCLK_mspi0,
#endif
#endif

#if !defined(CONFIG_SYSDESC_SUPPORT) || !defined(CONFIG_CAM_CLK) || !defined(CONFIG_PADMUX_SUPPORT)
        .regs = &mspi0_regs
#endif
    },

    // mspi1
    [1] =
    {
#ifdef CONFIG_SYSDESC_SUPPORT
#ifdef SYSDESC_DEV_mspi1
        .dev_id       = SYSDESC_DEV_mspi1,
#else
        .dev_id       = SYSDESC_DEV_NO_DEFINE,
#endif
#else
        .enable       = 1,
        .cs_num       = 2,
        .irq_num      = INT_IRQ_MSPI_1,
        .usedma       = 1,
        .clk_out_mode = 0,
        ._4to3_mode   = 0,
#ifdef CONFIG_GPIO_SUPPORT
        .cs_ext_num   = 2,
        .cs_ext       =
        {
            PAD_EMMC_CLK, PAD_EMMC_D1
        },
#endif

#ifndef CONFIG_PADMUX_SUPPORT
#ifdef CONFIG_GPIO_SUPPORT
        .padmod = PINMUX_FOR_SPI1_MODE_1,
#else
        .padmod = 1,
#endif
#endif

#ifdef CONFIG_CAM_CLK
        .camclk_id  = CAMCLK_mspi1,
#endif
#endif

#if !defined(CONFIG_SYSDESC_SUPPORT) || !defined(CONFIG_CAM_CLK) || !defined(CONFIG_PADMUX_SUPPORT)
        .regs = &mspi1_regs
#endif
    }
};


#define SSTAR_MSPI_BUS_MAX   (sizeof(mspi_cfg)/sizeof(mspi_cfg[0]))

#ifndef CONFIG_CAM_CLK
static struct clk_info spi_src_clk[] = {{108000000, (0<<2)}, {54000000, (1<<2)}, {12000000, (2<<2)}, {144000000, (3<<2)}};
#define SPI_SRC_CLK_SIZE (sizeof(spi_src_clk)/sizeof(spi_src_clk[0]))
#endif
