/*
 * hal_ir.h- Sigmastar
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

#ifndef _HAL_IR_H_
#define _HAL_IR_H_
#include "cam_os_wrapper.h"

#define IR_DBG 0
#if IR_DBG
#define ir_dbg(args...)  CamOsPrintf(args)
#define ir_err(args...)  CamOsPrintf(args)
#define ir_info(args...) CamOsPrintf(args)
#else
#define ir_dbg(args...)
#define ir_err(args...) CamOsPrintf(args)
#define ir_info(args...)
#endif

typedef signed char        s8;
typedef unsigned char      u8;
typedef signed short       s16;
typedef unsigned short     u16;
typedef signed int         s32;
typedef unsigned int       u32;
typedef signed long long   s64;
typedef unsigned long long u64;

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

#define IR_READ_REG(bank, offset)                  READ_WORD((bank + (offset << 2)))
#define IR_WRITE_REG(bank, offset, val)            WRITE_WORD((bank + (offset << 2)), val)
#define IR_WRITE_REG_MASK(bank, offset, val, mask) WRITE_WORD_MASK((bank + (offset << 2)), val, mask)

#define XTAL_CLOCK_FREQ 12000000 // 12 MHz

/*
 * IR decode mode options
 */
#define IR_MODE_FULLDECODE 1
#define IR_MODE_RAWDATA    2
#define RC5_MODE           3
#define IR_MODE_SWDECODE   4 // only support at linux
#define RC6_MODE           5 // not support now
#define IR_MODE_SPEC       6 // only support at linux and IR_TYPE_SEL == IR_TYPE_RCMM

/*
 * IR system parameter define for H/W setting (Please don't modify them)
 */
#define IR_CKDIV_NUM ((XTAL_CLOCK_FREQ / 1000000) - 1)
#define IR_CLK       (XTAL_CLOCK_FREQ / 1000000)
#define RC_CKDIV_NUM ((XTAL_CLOCK_FREQ / 1000000) - 4)

#define irGetMinCnt(time, tolerance) \
    ((u32)(((double)time * ((double)IR_CLK) / (IR_CKDIV_NUM + 1)) * ((double)1 - tolerance)))

#define irGetMaxCnt(time, tolerance) \
    ((u32)(((double)time * ((double)IR_CLK) / (IR_CKDIV_NUM + 1)) * ((double)1 + tolerance)))

#define irGetCnt(time) ((u32)((double)time * ((double)IR_CLK) / (IR_CKDIV_NUM + 1)))

#define rcGetCnt(time) ((u32)(double)time * ((double)IR_CLK) / (RC_CKDIV_NUM + 1))

#define IR_RP_TIMEOUT irGetCnt(IR_TIMEOUT_CYC)
#define IR_HDC_UPB    irGetMaxCnt(IR_HEADER_CODE_TIME, 0.2)
#define IR_HDC_LOB    irGetMinCnt(IR_HEADER_CODE_TIME, 0.2)
#define IR_OFC_UPB    irGetMaxCnt(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_LOB    irGetMinCnt(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_RP_UPB irGetMaxCnt(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_OFC_RP_LOB irGetMinCnt(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_LG01H_UPB  irGetMaxCnt(IR_LOGI_01H_TIME, 0.35)
#define IR_LG01H_LOB  irGetMinCnt(IR_LOGI_01H_TIME, 0.3)
#define IR_LG0_UPB    irGetMaxCnt(IR_LOGI_0_TIME, 0.2)
#define IR_LG0_LOB    irGetMinCnt(IR_LOGI_0_TIME, 0.2)
#define IR_LG1_UPB    irGetMaxCnt(IR_LOGI_1_TIME, 0.2)
#define IR_LG1_LOB    irGetMinCnt(IR_LOGI_1_TIME, 0.2)

#define RC5_SHOT_CNT   rcGetCnt(RC5_1T_TIME)
#define RC5_LONGPL_THR rcGetCnt((RC5_2T_TIME + RC5_1T_TIME))

#define IR_RAW_DATA_NUM 4

/*
 * IR Timing define
 */
#define IR_HEADER_CODE_TIME 9000   // us
#define IR_OFF_CODE_TIME    4500   // us
#define IR_OFF_CODE_RP_TIME 2500   // us
#define IR_LOGI_01H_TIME    560    // us
#define IR_LOGI_0_TIME      1120   // us
#define IR_LOGI_1_TIME      2240   // us
#define IR_TIMEOUT_CYC      140000 // us

#define IR_EVENT_TIMEOUT 220

#define RC5_1T_TIME 444 // us
#define RC5_2T_TIME 888 // us

/*
 * Register setting
 */
#define REG_RC_CFG                    0x00
#define RC5_DECODE                    0x0100
#define RC5_EXT_DECODE                0x0500
#define RC6_DECODE                    0x0300
#define REG_RC_LONGPL_THR             0x01
#define REG_RC_DIV                    0x03
#define RC_CLKDIV_MASE                0x1F00
#define REG_RC_KEY                    0x06
#define REG_RC_FIFO_STATUS            0x08
#define RC_KEY_ADDR_MSK               0x001F
#define RC_KEY_CMD_MSK                0x7F00
#define REG_RC_FIFO_RD                0x09
#define REG_RC_WAKE_UP                0x0A
#define REG_IR_CTRL                   0x40
#define REG_IR_HDC_UPB                0x41
#define REG_IR_HDC_LOB                0x42
#define REG_IR_OFC_UPB                0x43
#define REG_IR_OFC_LOB                0x44
#define REG_IR_OFC_RP_UPB             0x45
#define REG_IR_OFC_RP_LOB             0x46
#define REG_IR_LG01H_UPB              0x47
#define REG_IR_LG01H_LOB              0x48
#define REG_IR_LG0_UPB                0x49
#define REG_IR_LG0_LOB                0x4A
#define REG_IR_LG1_UPB                0x4B
#define REG_IR_LG1_LOB                0x4C
#define REG_IR_SEPR_UPB               0x4D
#define REG_IR_SEPR_LOB               0x4E
#define REG_IR_TIMEOUT_CYC_L          0x4F
#define REG_IR_TIMEOUT_CYC_H          0x50
#define IR_CCB_CB                     0x9F00 // ir_ccode_byte:1+ir_code_bit_num:32
#define REG_IR_SEPR_BIT_FIFO_CTRL     0x51
#define REG_IR_CCODE                  0x52
#define REG_IR_GLHRM_NUM              0x53
#define REG_IR_CKDIV_NUM_KEY_DATA     0x54
#define REG_IR_SHOT_CNT_L             0x55
#define REG_IR_SHOT_CNT_H_FIFO_STATUS 0x56
#define IR_RPT_FLAG                   0x0100
#define IR_FIFO_EMPTY                 0x0200
#define REG_IR_FIFO_RD_PULSE          0x58

#define GENERAL_CTRL               0x01BF
#define GENERAL_GLHRM_NUM          0x0804
#define GENERAL_SEPR_BIT_FIFO_CTRL 0x0F00
#define IR_FIFO_CLEARL             (0x01 << 15)
#define IR_GET_KEY_DATA            0X0008
#define IR_FIFO_RD_PULSE           0x0001

#define FULL_MODE_GLHRM_NUM     (0x03 << 12)
#define FULL_MODE_FIFO_RD_PULSE 0x0020

#define RAW_MODE_CTRL          0x01B3
#define RAW_MODE_GLHRM_NUM     (0x02 << 12)
#define RAW_MODE_FIFO_RD_PULSE 0x0020

#define RC5_MODE_WAKE_UP    (0X1 << 9)
#define RC5_MODE_FIFO_EMPTY 0x0001
#define RC5_FIFO_RD_PULSE   0x0001

#define SW_MODE_GLHRM_NUM            (0x01 << 12)
#define SW_MODE_FIFO_CTRL_EDGE_PSHOT (0x01 << 12)
#define SW_MODE_FIFO_CTRL_EDGE_NSHOT 0x2F00
#define SW_MODE_FIFO_CTRL_EDGE_ALL   (0x03 << 12)
#define SW_MODE_CKDIV_NUM            0x00CF
#define SW_MODE_FIFO_EN              (0x01 << 14)
#define SW_MODE_RECOV_SHOT_CNT       (0x01 << 6)
#define SW_MODE_FIFO_STATUS          0X0200
#define SW_MODE_SHOT_CNT_H           0X0007
#define SW_MODE_SHOT_TYPE            0x0010

#ifdef CAM_OS_RTK
#define REG_PMCLK_BANK    0X2001C00
#define REG_NONPMCLK_BANK 0x2207000
#else
#define REG_PMCLK_BANK    0X1F001C00
#define REG_NONPMCLK_BANK 0x1F207000
#endif
#define REG_PMCLK_OFFSET     0x21
#define REG_PMCLK_BIT        0x3E0
#define REG_PMCLK_FREQ_24M   0x0
#define REG_PMCLK_FREQ_12M   0x80
#define REG_PMCLK_FREQ_6M    0x100
#define REG_PMCLK_FREQ_3M    0x180
#define REG_PMCLK_FREQ_1500K 0x200
#define REG_PMCLK_FREQ_32K   0x280

#define REG_NONPMCLK1_OFFSET    0x21
#define REG_NONPMCLK2_OFFSET    0x22
#define REG_NONPMCLK_BIT        0x1F1F
#define REG_NONPMCLK_FREQ_24M   0x0
#define REG_NONPMCLK_FREQ_12M   0x0404
#define REG_NONPMCLK_FREQ_6M    0x0808
#define REG_NONPMCLK_FREQ_3M    0x0C0C
#define REG_NONPMCLK_FREQ_1500K 0x1010

/*
 * Struct declaration
 */

struct key_info
{
    u8 u8Key;
    u8 u8System;
    u8 u8Flag;
    u8 u8Valid;
};

struct hal_ir_dev
{
    u8  prev_fullkey;
    s32 rept_full;
    u64 prev_time;
    u64 current_time;

    u8  prev_rawkey;
    u8  key_count;
    u8  raw_key[IR_RAW_DATA_NUM];
    s32 rept_raw;

    u8  sw_shot_type[64];
    u32 sw_shot_count[64];
    u32 sw_shot_total;

    u32 decode_mode;
    u32 header_code[2];
    u64 membase;
    u64 (*calbak_get_sys_time)(void);
    struct key_info *ir_keyinfo;
};

/*
 * Function declaration
 */
extern int  hal_ir_init(struct hal_ir_dev *ir_dev);
extern void hal_ir_config(struct hal_ir_dev *ir_dev);
extern u16  hal_get_status(struct hal_ir_dev *ir_dev);
extern u32  hal_get_key(struct hal_ir_dev *ir_dev);
extern void hal_set_sw_decode(struct hal_ir_dev *ir_dev, int bEnable);

#endif
