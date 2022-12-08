/*
 * hal_ir_cfg.h- Sigmastar
 *
 * Copyright (c) [2019~2020] SigmaStar Technology.
 *
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.
 *
 */

#ifndef _HAL_IR_CFG_H_
#define _HAL_IR_CFG_H_

#include "hal_ir.h"
#include "padmux.h"
#ifdef CAM_OS_RTK
#include "irqs.h"
#endif
#ifdef  CONFIG_CAMCLK_SUPPORT
#include "camclk.h"
#endif
#ifdef CONFIG_SYSDESC_SUPPORT
#include "drv_sysdesc.h"
#endif

#define IR_GROUP 5
#define SYSDESC_DEV_IR_NODEF 0xFFFF

struct reg_res{
    u64 bank_base;
    u8  reg_offset;
    u8  bit_shift;
    u32 bit_mask;
};

struct ir_reg_res{
    u64            ir_bank_base;
    struct reg_res ir_reg_clkgen;
    struct reg_res ir_reg_padmod;
};

struct ir_config
{
    u8  enable;
    u16 dev_id;
    u32 irq;
    u32 rc_irq;
    u32 pad_mod;
    u32 clk_mod;
    u32 vendor_id;
    u32 product_id;
    u32 decode_mode;
    u32 header_code[2];
};

#ifdef CAM_OS_RTK
#if !defined(CONFIG_SYSDESC_SUPPORT) || !defined(CONFIG_CAMCLK_SUPPORT) || !defined(CONFIG_GPIO_SUPPORT)
struct ir_reg_res ir_reg[IR_GROUP] =
{
    {//IR0

#ifndef CONFIG_SYSDESC_SUPPORT
    .ir_bank_base = 0x2007A00,
#endif

#ifndef CONFIG_CAMCLK_SUPPORT
    .ir_reg_clkgen = {
        .bank_base  = 0X2001C00,
        .reg_offset = 0x21,
        .bit_shift  = 7,
        .bit_mask   = 0x3E0
    },
#endif

#ifndef CONFIG_GPIO_SUPPORT
    .ir_reg_padmod = {
        .bank_base  = 0x2007E00,
        .reg_offset = 0x62,
        .bit_shift  = 0,
        .bit_mask   = 0x3
    },
#endif

    },//end of ir_reg[0]

    {//IR1

#ifndef CONFIG_SYSDESC_SUPPORT
    .ir_bank_base = 0x2225800,
#endif

#ifndef CONFIG_CAMCLK_SUPPORT
    .ir_reg_clkgen = {
        .bank_base  = 0x2207000,
        .reg_offset = 0x21,
        .bit_shift  = 2,
        .bit_mask   = 0x1F
    },
#endif

#ifndef CONFIG_GPIO_SUPPORT
    .ir_reg_padmod = {
        .bank_base  = 0x2207800,
        .reg_offset = 0x24,
        .bit_shift  = 0,
        .bit_mask   = 0x3
    },
#endif

    },//end of ir_reg[1]

    {//IR2

#ifndef CONFIG_SYSDESC_SUPPORT
    .ir_bank_base = 0x2225A00,
#endif

#ifndef CONFIG_CAMCLK_SUPPORT
    .ir_reg_clkgen = {
        .bank_base  = 0x2207000,
        .reg_offset = 0x21,
        .bit_shift  = 10,
        .bit_mask   = 0x1F00
    },
#endif

#ifndef CONFIG_GPIO_SUPPORT
    .ir_reg_padmod = {
        .bank_base  = 0x2207800,
        .reg_offset = 0x24,
        .bit_shift  = 2,
        .bit_mask   = 0xC
    },
#endif

    },//end of ir_reg[2]

    {//IR3

#ifndef CONFIG_SYSDESC_SUPPORT
    .ir_bank_base = 0x2225C00,
#endif

#ifndef CONFIG_CAMCLK_SUPPORT
    .ir_reg_clkgen = {
        .bank_base  = 0x2207000,
        .reg_offset = 0x22,
        .bit_shift  = 2,
        .bit_mask   = 0x1F
    },
#endif

#ifndef CONFIG_GPIO_SUPPORT
    .ir_reg_padmod = {
        .bank_base  = 0x2207800,
        .reg_offset = 0x24,
        .bit_shift  = 4,
        .bit_mask   = 0x30
    },
#endif

    },//end of ir_reg[3]

    {//IR4

#ifndef CONFIG_SYSDESC_SUPPORT
    .ir_bank_base = 0x2225E00,
#endif

#ifndef CONFIG_CAMCLK_SUPPORT
    .ir_reg_clkgen = {
        .bank_base  = 0x2207000,
        .reg_offset = 0x22,
        .bit_shift  = 10,
        .bit_mask   = 0x1F00
    },
#endif

#ifndef CONFIG_GPIO_SUPPORT
    .ir_reg_padmod = {
        .bank_base  = 0x2207800,
        .reg_offset = 0x24,
        .bit_shift  = 6,
        .bit_mask   = 0xC0
    },
#endif

    },//end of ir_reg[4]

};
#endif
#endif

struct ir_config ir_cfg[IR_GROUP] =
{
    {//IR0

#ifdef CAM_OS_RTK

#ifdef SYSDESC_DEV_ir0
    .dev_id      = SYSDESC_DEV_ir0,
#else
    .dev_id      = SYSDESC_DEV_IR_NODEF,
#endif

#ifdef CONFIG_CAMCLK_SUPPORT
    .clk_mod     = CAMCLK_ir,
#else
    .clk_mod     = 1,
#endif

#ifdef CONFIG_GPIO_SUPPORT
    .pad_mod     = PINMUX_FOR_IR_IN_MODE_1,
#else
    .pad_mod      = 1,
#endif

    .enable      = 1,
    .irq         = INT_FIQ_IR,
    .rc_irq      = INT_FIQ_IR_RC,
#endif

    .decode_mode = IR_MODE_FULLDECODE,
    .header_code = {0x00, 0xFF},
    },//end of ir_cfg[0]

    {//IR1

#ifdef CAM_OS_RTK

#ifdef SYSDESC_DEV_ir1
    .dev_id      = SYSDESC_DEV_ir1,
#else
    .dev_id      = SYSDESC_DEV_IR_NODEF,
#endif

#ifdef CONFIG_CAMCLK_SUPPORT
    .clk_mod     = CAMCLK_ir_nonpm0,
#else
    .clk_mod     = 1,
#endif

#ifdef CONFIG_GPIO_SUPPORT
    .pad_mod     = PINMUX_FOR_IR0_MODE_1,
#else
    .pad_mod     = 2,
#endif

    .enable      = 1,
    .irq         = INT_FIQ_IR_NONPM0,
    .rc_irq      = 0xFFFF,
#endif

    .decode_mode = IR_MODE_FULLDECODE,
    .header_code = {0x00, 0xFF},
    },//end of ir_cfg[1]

    {//IR2

#ifdef CAM_OS_RTK

#ifdef SYSDESC_DEV_ir2
    .dev_id      = SYSDESC_DEV_ir2,
#else
    .dev_id      = SYSDESC_DEV_IR_NODEF,

#ifdef CONFIG_CAMCLK_SUPPORT
    .clk_mod     = CAMCLK_ir_nonpm1,
#else
    .clk_mod     = 1,
#endif

#ifdef CONFIG_GPIO_SUPPORT
    .pad_mod     = PINMUX_FOR_IR1_MODE_1,
#else
    .pad_mod     = 2,
#endif

    .enable      = 1,
    .irq         = INT_FIQ_IR_NONPM1,
    .rc_irq      = 0xFFFF,
#endif

    .decode_mode = IR_MODE_FULLDECODE,
    .header_code = {0x00, 0xFF},
    },//end of ir_cfg[2]

    {//IR3

#ifdef CAM_OS_RTK

#ifdef SYSDESC_DEV_ir3
    .dev_id      = SYSDESC_DEV_ir3,
#else
    .dev_id      = SYSDESC_DEV_IR_NODEF,
#endif

#ifdef CONFIG_CAMCLK_SUPPORT
    .clk_mod     = CAMCLK_ir_nonpm2,
#else
    .clk_mod     = 1,
#endif

#ifdef CONFIG_GPIO_SUPPORT
    .pad_mod     = PINMUX_FOR_IR2_MODE_1,
#else
    .pad_mod     = 2,
#endif

    .enable      = 1,
    .irq         = INT_FIQ_IR_NONPM2,
    .rc_irq      = 0xFFFF,
#endif

    .decode_mode = IR_MODE_FULLDECODE,
    .header_code = {0x00, 0xFF},
    },//end of ir_cfg[3]

    {//IR4

#ifdef CAM_OS_RTK

#ifdef SYSDESC_DEV_ir4
    .dev_id      = SYSDESC_DEV_ir4,
#else
    .dev_id      = SYSDESC_DEV_IR_NODEF,
#endif

#ifdef CONFIG_CAMCLK_SUPPORT
    .clk_mod     = CAMCLK_ir_nonpm3,
#endif
    .clk_mod     = 1,
#endif

#ifdef CONFIG_GPIO_SUPPORT
    .pad_mod     = PINMUX_FOR_IR3_MODE_1,
#else
    .pad_mod     = 2,
#endif

    .enable      = 1,
    .irq         = INT_FIQ_IR_NONPM3,
    .rc_irq      = 0xFFFF,
#endif

    .decode_mode = IR_MODE_FULLDECODE,
    .header_code = {0x00, 0xFF},
    },//end of ir_cfg[4]

};

#endif
