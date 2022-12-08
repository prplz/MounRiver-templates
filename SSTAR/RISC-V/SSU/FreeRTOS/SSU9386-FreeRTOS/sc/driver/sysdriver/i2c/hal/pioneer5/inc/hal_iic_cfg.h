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

#ifdef CONFIG_GPIO_SUPPORT
#include <padmux.h>
#endif

#define CLKGEN_I2C_SEL_12M (0x8)
#define CLKGEN_I2C_SEL_54M (0x4)
#define CLKGEN_I2C_SEL_72M (0x0)

#define I2C_DEFAULT_SPEED  (200000)

#define READ_WORD(_reg)        (*(volatile u16 *)(u32)(_reg))
#define WRITE_WORD(_reg, _val) ((*(volatile u16 *)(u32)(_reg)) = (u16)(_val))
#define WRITE_WORD_MASK(_reg, _val, _mask)    \
    ((*(volatile u16 *)(u32)(_reg)) = \
         ((*(volatile u16 *)(u32)(_reg)) & ~(_mask)) | ((u16)(_val) & (_mask)))

#define SYSDESC_DEV_NO_DEFINE   0xFFFF

struct reg_t{
    u64 bank_base;
    u8  reg_offset;
    u8  bit_shift;
    u32 bit_mask;
};

struct i2c_reg_t{
#ifndef CONFIG_SYSDESC_SUPPORT
    u64          i2c_bank_base;
#endif

#ifndef CONFIG_CAM_CLK
    struct reg_t reg_clkgen;
#endif

    struct reg_t reg_padmod;
};

struct i2c_config{
#ifdef CONFIG_SYSDESC_SUPPORT
    u16  dev_id;
#else
    u8   enable;
    u32  irq;
    u32  speed;
    u16  t_hd;
    u16  t_su;
    u16  t_start;
    u16  t_stop;
    u8   push_pull;
    u8   oen_cnt;
    u8   dma_en;
    u16  padmod;
#ifdef CONFIG_CAM_CLK
    u16  camclk_id;
#endif
#endif
#if !defined(CONFIG_SYSDESC_SUPPORT) || !defined(CONFIG_CAM_CLK) || !defined(CONFIG_PADMUX_SUPPORT)
    struct i2c_reg_t *pst_reg;
#endif
};// end of struct i2c_config


#if !defined(CONFIG_SYSDESC_SUPPORT) || !defined(CONFIG_CAM_CLK) || !defined(CONFIG_PADMUX_SUPPORT)
//I2C-0
struct i2c_reg_t I2c0Reg =
{
#ifndef CONFIG_SYSDESC_SUPPORT
    .i2c_bank_base = 0x2222800,
#endif

#ifndef CONFIG_CAM_CLK
    .reg_clkgen = {
        .bank_base  = 0x2207000,
        .reg_offset = 0x37,
        .bit_shift  = 0,
        .bit_mask   = (BIT3 | BIT2 | BIT1 | BIT0)
    },
#endif

    .reg_padmod = {
        .bank_base  = 0x2207800,
        .reg_offset = 0x6F,
        .bit_shift  = 0,
        .bit_mask   = (BIT2 | BIT1 | BIT0)
    },
};

    //I2C-1
struct i2c_reg_t I2c1Reg =
{
#ifndef CONFIG_SYSDESC_SUPPORT
    .i2c_bank_base = 0x2222A00,
#endif

#ifndef CONFIG_CAM_CLK
    .reg_clkgen = {
        .bank_base  = 0x2207000,
        .reg_offset = 0x37,
        .bit_shift  = 8,
        .bit_mask   = (BIT11 | BIT10 | BIT9 | BIT8)
    },
#endif

    .reg_padmod = {
        .bank_base  = 0x2207800,
        .reg_offset = 0x53,
        .bit_shift  = 0,
        .bit_mask   = (BIT2 | BIT1 | BIT0)
    },
};

    //I2C-2
struct i2c_reg_t I2c2Reg =
{
#ifndef CONFIG_SYSDESC_SUPPORT
    .i2c_bank_base = 0x2222C00,
#endif

#ifndef CONFIG_CAM_CLK
    .reg_clkgen = {
        .bank_base  = 0x2207000,
        .reg_offset = 0x37,
        .bit_shift  = 12,
        .bit_mask   = (BIT15 | BIT14 | BIT13 | BIT12)
    },
#endif

    .reg_padmod = {
        .bank_base  = 0x2207800,
        .reg_offset = 0x6F,
        .bit_shift  = 8,
        .bit_mask   = (BIT9 | BIT8)
    },
};

    //I2C-3
struct i2c_reg_t I2c3Reg =
{
#ifndef CONFIG_SYSDESC_SUPPORT
    .i2c_bank_base = 0x2222E00,
#endif

#ifndef CONFIG_CAM_CLK
    .reg_clkgen = {
        .bank_base  = 0x2207000,
        .reg_offset = 0x39,
        .bit_shift  = 0,
        .bit_mask   = (BIT3 | BIT2 | BIT1 | BIT0)
    },
#endif

    .reg_padmod = {
        .bank_base  = 0x2207800,
        .reg_offset = 0x73,
        .bit_shift  = 0,
        .bit_mask   = (BIT1 | BIT0)
    },
};

    //I2C-4
struct i2c_reg_t I2c4Reg =
{
#ifndef CONFIG_SYSDESC_SUPPORT
    .i2c_bank_base = 0x2223000,
#endif

#ifndef CONFIG_CAM_CLK
    .reg_clkgen = {
        .bank_base  = 0x2207000,
        .reg_offset = 0x39,
        .bit_shift  = 8,
        .bit_mask   = (BIT11 | BIT10 | BIT9 | BIT8)
    },
#endif

    .reg_padmod = {
        .bank_base  = 0x2207800,
        .reg_offset = 0x73,
        .bit_shift  = 4,
        .bit_mask   = (BIT6 | BIT5 | BIT4)
    },
};

    //I2C-5
struct i2c_reg_t I2c5Reg =
{
#ifndef CONFIG_SYSDESC_SUPPORT
    .i2c_bank_base = 0x2223200,
#endif

#ifndef CONFIG_CAM_CLK
    .reg_clkgen = {
        .bank_base  = 0x2207000,
        .reg_offset = 0x39,
        .bit_shift  = 12,
        .bit_mask   = (BIT15 | BIT14 | BIT13 | BIT12)
    },
#endif

    .reg_padmod = {
        .bank_base  = 0x2207800,
        .reg_offset = 0x73,
        .bit_shift  = 8,
        .bit_mask   = (BIT10 | BIT9 | BIT8)
    },
};


#endif// end of(!CONFIG_SYSDESC_SUPPORT||!CONFIG_CAM_CLK||!CONFIG_PADMUX_SUPPORT)

const struct i2c_config stI2cConfig[] = {

    //I2C0 config
    {
#ifdef CONFIG_SYSDESC_SUPPORT
#ifdef SYSDESC_DEV_i2cm0
        .dev_id = SYSDESC_DEV_i2cm0,
#else
        .dev_id = SYSDESC_DEV_NO_DEFINE,
#endif
#else
        .enable = 1,
        .speed = I2C_DEFAULT_SPEED,
        .irq = INT_IRQ_MIIC_0,
        .t_hd  = 0,
        .t_su    = 0,
        .t_start = 0,
        .t_stop  = 0,
        .push_pull = 1,
        .oen_cnt   = 0,
        .dma_en    = 0,
#ifdef CONFIG_GPIO_SUPPORT
        .padmod = PINMUX_FOR_I2C0_MODE_1,
#else
        .padmod = 7,
#endif
#ifdef CONFIG_CAM_CLK
        .camclk_id = CAMCLK_miic0,
#endif
#endif

#if !defined(CONFIG_SYSDESC_SUPPORT) || !defined(CONFIG_CAM_CLK) || !defined(CONFIG_PADMUX_SUPPORT)
        .pst_reg = &I2c0Reg,
#endif
    },

    //I2C1 config
    {
#ifdef CONFIG_SYSDESC_SUPPORT
#ifdef SYSDESC_DEV_i2cm1
        .dev_id = SYSDESC_DEV_i2cm1,
#else
        .dev_id = SYSDESC_DEV_NO_DEFINE,
#endif
#else
        .enable = 1,
        .speed = I2C_DEFAULT_SPEED,
        .irq = INT_IRQ_MIIC_0,
        .t_hd  = 0,
        .t_su    = 0,
        .t_start = 0,
        .t_stop  = 0,
        .push_pull = 1,
        .oen_cnt   = 0,
        .dma_en    = 0,
#ifdef CONFIG_GPIO_SUPPORT
        .padmod = PINMUX_FOR_I2C1_MODE_3,
#else
        .padmod = 3,
#endif
#ifdef CONFIG_CAM_CLK
        .camclk_id = CAMCLK_miic1,
#endif
#endif

#if !defined(CONFIG_SYSDESC_SUPPORT) || !defined(CONFIG_CAM_CLK) || !defined(CONFIG_PADMUX_SUPPORT)
        .pst_reg = &I2c1Reg,
#endif
    },

    //I2C2 config
    {
#ifdef CONFIG_SYSDESC_SUPPORT
#ifdef SYSDESC_DEV_i2cm2
        .dev_id = SYSDESC_DEV_i2cm2,
#else
        .dev_id = SYSDESC_DEV_NO_DEFINE,
#endif
#else
        .enable = 1,
        .speed = I2C_DEFAULT_SPEED,
        .irq = INT_IRQ_MIIC_0,
        .t_hd  = 0,
        .t_su    = 0,
        .t_start = 0,
        .t_stop  = 0,
        .push_pull = 1,
        .oen_cnt   = 0,
        .dma_en    = 0,
#ifdef CONFIG_GPIO_SUPPORT
#ifdef _RTK_22_LANE_SUPPORT_
        .padmod = PINMUX_FOR_I2C2_MODE_2,
#else
        .padmod = PINMUX_FOR_I2C2_MODE_1,
#endif
#else
#ifdef _RTK_22_LANE_SUPPORT_
        .padmod = 2,
#else
        .padmod = 5,
#endif
#endif
#ifdef CONFIG_CAM_CLK
        .camclk_id = CAMCLK_miic2,
#endif
#endif

#if !defined(CONFIG_SYSDESC_SUPPORT) || !defined(CONFIG_CAM_CLK) || !defined(CONFIG_PADMUX_SUPPORT)
        .pst_reg = &I2c2Reg,
#endif
    },
    //I2C3 config
    {
#ifdef CONFIG_SYSDESC_SUPPORT
#ifdef SYSDESC_DEV_i2cm3
        .dev_id = SYSDESC_DEV_i2cm3,
#else
        .dev_id = SYSDESC_DEV_NO_DEFINE,
#endif
#else
        .enable = 1,
        .speed = I2C_DEFAULT_SPEED,
        .irq = INT_IRQ_MIIC_0,
        .t_hd  = 0,
        .t_su    = 0,
        .t_start = 0,
        .t_stop  = 0,
        .push_pull = 1,
        .oen_cnt   = 0,
        .dma_en    = 0,

#ifdef CONFIG_GPIO_SUPPORT
        .padmod = 0,
#else
        .padmod = 0,
#endif
#ifdef CONFIG_CAM_CLK
        .camclk_id = CAMCLK_miic3,
#endif
#endif

#if !defined(CONFIG_SYSDESC_SUPPORT) || !defined(CONFIG_CAM_CLK) || !defined(CONFIG_PADMUX_SUPPORT)
        .pst_reg = &I2c3Reg,
#endif
    },

    //I2C4 config
    {
#ifdef CONFIG_SYSDESC_SUPPORT
#ifdef SYSDESC_DEV_i2cm4
        .dev_id = SYSDESC_DEV_i2cm4,
#else
        .dev_id = SYSDESC_DEV_NO_DEFINE,
#endif
#else
        .enable = 1,
        .speed = I2C_DEFAULT_SPEED,
        .irq = INT_IRQ_MIIC_0,
        .t_hd  = 0,
        .t_su    = 0,
        .t_start = 0,
        .t_stop  = 0,
        .push_pull = 1,
        .oen_cnt   = 0,
        .dma_en    = 0,

#ifdef CONFIG_GPIO_SUPPORT
        .padmod = PINMUX_FOR_I2C4_MODE_2,
#else
        .padmod = 2,
#endif
#ifdef CONFIG_CAM_CLK
        .camclk_id = CAMCLK_miic4,
#endif
#endif

#if !defined(CONFIG_SYSDESC_SUPPORT) || !defined(CONFIG_CAM_CLK) || !defined(CONFIG_PADMUX_SUPPORT)
        .pst_reg = &I2c4Reg,
#endif
    },

    //I2C5 config
    {
#ifdef CONFIG_SYSDESC_SUPPORT
#ifdef SYSDESC_DEV_i2cm5
        .dev_id = SYSDESC_DEV_i2cm5,
#else
        .dev_id = SYSDESC_DEV_NO_DEFINE,
#endif
#else
        .enable = 1,
        .speed = I2C_DEFAULT_SPEED,
        .irq = INT_IRQ_MIIC_0,
        .t_hd  = 0,
        .t_su    = 0,
        .t_start = 0,
        .t_stop  = 0,
        .push_pull = 1,
        .oen_cnt   = 0,
        .dma_en    = 0,

#ifdef CONFIG_GPIO_SUPPORT
        .padmod = PINMUX_FOR_I2C5_MODE_1,
#else
        .padmod = 1,
#endif
#ifdef CONFIG_CAM_CLK
        .camclk_id = CAMCLK_miic5,
#endif
#endif

#if !defined(CONFIG_SYSDESC_SUPPORT) || !defined(CONFIG_CAM_CLK) || !defined(CONFIG_PADMUX_SUPPORT)
        .pst_reg = &I2c5Reg,
#endif
    },

};

#if !defined(CONFIG_SYSDESC_SUPPORT) || !defined(CONFIG_CAM_CLK) || !defined(CONFIG_PADMUX_SUPPORT)
#define I2C_BUSNUM (sizeof(stI2cConfig)/(sizeof(struct i2c_config)))
#else
#define I2C_BUSNUM (6)
#endif
