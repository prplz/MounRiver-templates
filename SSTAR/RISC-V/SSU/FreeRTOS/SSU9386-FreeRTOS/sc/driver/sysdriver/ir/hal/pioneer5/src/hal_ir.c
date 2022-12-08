/*
 * hal_ir.c- Sigmastar
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

#include "hal_ir.h"
/*
 * function
 */
static void hal_set_timing(struct hal_ir_dev *ir_dev)
{
    switch (ir_dev->decode_mode)
    {
        case IR_MODE_FULLDECODE:
        case IR_MODE_RAWDATA:
        case IR_MODE_SWDECODE:
            // header code upper/lower bound
            IR_WRITE_REG(ir_dev->membase, REG_IR_HDC_UPB, IR_HDC_UPB);
            IR_WRITE_REG(ir_dev->membase, REG_IR_HDC_LOB, IR_HDC_LOB);

            // off code upper/lower bound
            IR_WRITE_REG(ir_dev->membase, REG_IR_OFC_UPB, IR_OFC_UPB);
            IR_WRITE_REG(ir_dev->membase, REG_IR_OFC_LOB, IR_OFC_LOB);

            // off code repeat upper/lower bound
            IR_WRITE_REG(ir_dev->membase, REG_IR_OFC_RP_UPB, IR_OFC_RP_UPB);
            IR_WRITE_REG(ir_dev->membase, REG_IR_OFC_RP_LOB, IR_OFC_RP_LOB);

            // logical 0/1 high upper/lower bound
            IR_WRITE_REG(ir_dev->membase, REG_IR_LG01H_UPB, IR_LG01H_UPB);
            IR_WRITE_REG(ir_dev->membase, REG_IR_LG01H_LOB, IR_LG01H_LOB);

            // logical 0 upper/lower bound
            IR_WRITE_REG(ir_dev->membase, REG_IR_LG0_UPB, IR_LG0_UPB);
            IR_WRITE_REG(ir_dev->membase, REG_IR_LG0_LOB, IR_LG0_LOB);

            // logical 1 upper/lower bound
            IR_WRITE_REG(ir_dev->membase, REG_IR_LG1_UPB, IR_LG1_UPB);
            IR_WRITE_REG(ir_dev->membase, REG_IR_LG1_LOB, IR_LG1_LOB);

            // timeout cycles
            IR_WRITE_REG(ir_dev->membase, REG_IR_TIMEOUT_CYC_L, IR_RP_TIMEOUT & 0xFFFF);
            // set up ccode bytes and code bytes/bits num
            IR_WRITE_REG(ir_dev->membase, REG_IR_TIMEOUT_CYC_H, IR_CCB_CB | 0x30UL | ((IR_RP_TIMEOUT >> 16) & 0x0F));

            IR_WRITE_REG(ir_dev->membase, REG_IR_CKDIV_NUM_KEY_DATA, IR_CKDIV_NUM);
            break;
        case RC5_MODE:
            IR_WRITE_REG(ir_dev->membase, REG_RC_DIV,
                         (IR_READ_REG(ir_dev->membase, REG_RC_DIV) & ~(RC_CLKDIV_MASE)) | (RC_CKDIV_NUM << 8));
            IR_WRITE_REG(ir_dev->membase, REG_RC_LONGPL_THR, RC5_LONGPL_THR);
            break;
        default:
            ir_err("UNKNOW MODE\n");
    }
}

void hal_set_sw_decode(struct hal_ir_dev *ir_dev, int bEnable)
{
    u16 regVal = 0;

    regVal = IR_READ_REG(ir_dev->membase, REG_IR_SEPR_BIT_FIFO_CTRL);
    if (bEnable)
    {
        regVal |= SW_MODE_FIFO_EN;
        regVal |= SW_MODE_RECOV_SHOT_CNT;
    }
    else
    {
        regVal &= ~SW_MODE_FIFO_EN;
        regVal &= ~SW_MODE_RECOV_SHOT_CNT;
    }
    IR_WRITE_REG(ir_dev->membase, REG_IR_SEPR_BIT_FIFO_CTRL, regVal);
}

static void hal_clear_fifo(struct hal_ir_dev *ir_dev)
{
    IR_WRITE_REG_MASK(ir_dev->membase, REG_IR_SEPR_BIT_FIFO_CTRL, IR_FIFO_CLEARL, IR_FIFO_CLEARL);
}

static u8 hal_read_fifo(struct hal_ir_dev *ir_dev)
{
    u8 keyVal = 0;

    keyVal = IR_READ_REG(ir_dev->membase, REG_IR_CKDIV_NUM_KEY_DATA) >> IR_GET_KEY_DATA;
    IR_WRITE_REG_MASK(ir_dev->membase, REG_IR_FIFO_RD_PULSE, IR_FIFO_RD_PULSE, IR_FIFO_RD_PULSE);
    return keyVal;
}

static void hal_get_key_fullmode(struct hal_ir_dev *ir_dev, struct key_info *keyInfo)
{
    u8  currKey = 0;
    u16 regVal  = 0;

    regVal = IR_READ_REG(ir_dev->membase, REG_IR_SHOT_CNT_H_FIFO_STATUS);
    if (regVal & IR_FIFO_EMPTY)
    {
        ir_dev->rept_full = 0;
        keyInfo->u8Valid  = 0;
        return;
    }

    while (!(regVal & IR_FIFO_EMPTY))
    {
        currKey = hal_read_fifo(ir_dev);
        regVal  = IR_READ_REG(ir_dev->membase, REG_IR_SHOT_CNT_H_FIFO_STATUS);
    }

    ir_dev->current_time = ir_dev->calbak_get_sys_time();
    if (ir_dev->current_time - ir_dev->prev_time >= IR_TIMEOUT_CYC / 1000)
    {
        currKey              = hal_read_fifo(ir_dev);
        ir_dev->rept_full    = 0;
        ir_dev->prev_fullkey = currKey;
        keyInfo->u8Key       = currKey;
        keyInfo->u8Valid     = 1;
    }
    else
    {
        if (ir_dev->rept_full == 0)
        {
            ir_dev->rept_full = 1;
            keyInfo->u8Valid  = 0;
        }
        else
        {
            currKey          = hal_read_fifo(ir_dev);
            regVal           = IR_READ_REG(ir_dev->membase, REG_IR_SHOT_CNT_H_FIFO_STATUS);
            keyInfo->u8Flag  = (regVal & IR_RPT_FLAG) ? 1 : 0;
            keyInfo->u8Key   = currKey;
            keyInfo->u8Valid = ((keyInfo->u8Flag) && (currKey == ir_dev->prev_fullkey));
        }
    }
    ir_dev->prev_time = ir_dev->current_time;

    ir_info("IR FLAG  : 0x%x\n", keyInfo->u8Flag);
    ir_info("IR CMD   : 0x%x\n", keyInfo->u8Key);
    ir_info("IR VALID : 0x%x\n", keyInfo->u8Valid);
}

static void hal_get_key_rawmode(struct hal_ir_dev *ir_dev, struct key_info *keyInfo)
{
    u8  i;
    u16 regVal = 0;

    regVal = IR_READ_REG(ir_dev->membase, REG_IR_SHOT_CNT_H_FIFO_STATUS);
    if (ir_dev->rept_raw == 1 && (regVal & IR_RPT_FLAG) == IR_RPT_FLAG)
    {
        ir_dev->rept_raw = 0;
        keyInfo->u8Valid = 1;
        keyInfo->u8Key = ir_dev->prev_rawkey;
        hal_clear_fifo(ir_dev);
        return;
    }
    ir_dev->rept_raw = 1;

    for (i = 0; i < IR_RAW_DATA_NUM; i++)
    {
        regVal = IR_READ_REG(ir_dev->membase, REG_IR_SHOT_CNT_H_FIFO_STATUS);
        if (regVal & IR_FIFO_EMPTY)
        {
            ir_dev->rept_raw = 0;
            keyInfo->u8Valid = 0;
            return;
        }

        ir_dev->raw_key[ir_dev->key_count++] = hal_read_fifo(ir_dev);
        if (ir_dev->key_count == IR_RAW_DATA_NUM)
        {
            ir_dev->key_count = 0;
            if ((ir_dev->raw_key[0] == ir_dev->header_code[0]) && (ir_dev->raw_key[1] == ir_dev->header_code[1])
                && (ir_dev->raw_key[2] == (u8)(~ir_dev->raw_key[3])))
            {
                ir_dev->rept_raw    = 0;
                ir_dev->prev_rawkey = ir_dev->raw_key[2];
                keyInfo->u8Key      = ir_dev->raw_key[2];
                keyInfo->u8Valid    = 1;
            }
        }
    }

    hal_clear_fifo(ir_dev);
}

static void hal_get_key_rc5mode(struct hal_ir_dev *ir_dev, struct key_info *keyInfo)
{
    static u8  rc5_addr;
    static u8  rc5_cmd;
    static u16 rc5_key;

    rc5_key          = IR_READ_REG(ir_dev->membase, REG_RC_KEY);
    rc5_addr         = rc5_key & RC_KEY_ADDR_MSK;
    rc5_cmd          = ((rc5_key & RC_KEY_CMD_MSK) >> 8);
    keyInfo->u8Flag  = rc5_addr;
    keyInfo->u8Key   = rc5_cmd;
    keyInfo->u8Valid = 1;
    IR_WRITE_REG(ir_dev->membase, REG_RC_FIFO_RD, RC5_FIFO_RD_PULSE);
    ir_info("IR FLAG  : 0x%x\n", keyInfo->u8Flag);
    ir_info("IR CMD   : 0x%x\n", keyInfo->u8Key);
    ir_info("IR VALID : 0x%x\n", keyInfo->u8Valid);
}

static void hal_get_key_swmode(struct hal_ir_dev *ir_dev)
{
    u32 i;
    u32 status;

    status = (IR_READ_REG(ir_dev->membase, REG_IR_SHOT_CNT_H_FIFO_STATUS) & SW_MODE_FIFO_STATUS) == SW_MODE_FIFO_STATUS;
    for (i = 0; !status; i++)
    {
        ir_dev->sw_shot_count[i] =
            (IR_READ_REG(ir_dev->membase, REG_IR_SHOT_CNT_L)
             | ((IR_READ_REG(ir_dev->membase, REG_IR_SHOT_CNT_H_FIFO_STATUS) & SW_MODE_SHOT_CNT_H) << 16));

        ir_dev->sw_shot_type[i] =
            ((IR_READ_REG(ir_dev->membase, REG_IR_SHOT_CNT_H_FIFO_STATUS) & SW_MODE_SHOT_TYPE) == SW_MODE_SHOT_TYPE);

        IR_WRITE_REG_MASK(ir_dev->membase, REG_IR_FIFO_RD_PULSE, IR_FIFO_RD_PULSE, IR_FIFO_RD_PULSE);

        status =
            (IR_READ_REG(ir_dev->membase, REG_IR_SHOT_CNT_H_FIFO_STATUS) & SW_MODE_FIFO_STATUS) == SW_MODE_FIFO_STATUS;

        ir_dbg("sw_shot_count[%u] : %u\n", i, ir_dev->sw_shot_count[i]);
        ir_dbg("sw_shot_type [%u] : %u\n", i, ir_dev->sw_shot_type[i]);
    }
    ir_dev->sw_shot_total = i;
}

int hal_ir_init(struct hal_ir_dev *ir_dev)
{
    u16 regVal = 0;

    ir_dev->prev_fullkey = 0;
    ir_dev->rept_full    = 0;
    ir_dev->prev_time    = 0;
    ir_dev->current_time = 0;

    ir_dev->rept_raw    = 0;
    ir_dev->prev_rawkey = 0;
    ir_dev->key_count   = 0;

    hal_set_timing(ir_dev);

    IR_WRITE_REG(ir_dev->membase, REG_IR_CCODE, ((ir_dev->header_code[1] << 8) | ir_dev->header_code[0]));
    IR_WRITE_REG(ir_dev->membase, REG_IR_CTRL, GENERAL_CTRL);
    IR_WRITE_REG(ir_dev->membase, REG_IR_GLHRM_NUM, GENERAL_GLHRM_NUM);
    IR_WRITE_REG(ir_dev->membase, REG_IR_SEPR_BIT_FIFO_CTRL, GENERAL_SEPR_BIT_FIFO_CTRL);

    if (ir_dev->decode_mode == IR_MODE_RAWDATA)
    {
        IR_WRITE_REG(ir_dev->membase, REG_IR_CTRL, RAW_MODE_CTRL);
        IR_WRITE_REG_MASK(ir_dev->membase, REG_IR_GLHRM_NUM, RAW_MODE_GLHRM_NUM, RAW_MODE_GLHRM_NUM);
        IR_WRITE_REG_MASK(ir_dev->membase, REG_IR_FIFO_RD_PULSE, RAW_MODE_FIFO_RD_PULSE, RAW_MODE_FIFO_RD_PULSE);
    }
    else if (ir_dev->decode_mode == IR_MODE_FULLDECODE)
    {
        IR_WRITE_REG_MASK(ir_dev->membase, REG_IR_GLHRM_NUM, FULL_MODE_GLHRM_NUM, FULL_MODE_GLHRM_NUM);
        IR_WRITE_REG_MASK(ir_dev->membase, REG_IR_FIFO_RD_PULSE, FULL_MODE_FIFO_RD_PULSE, FULL_MODE_FIFO_RD_PULSE);
    }
    else if (ir_dev->decode_mode == RC5_MODE)
    {
        IR_WRITE_REG(ir_dev->membase, REG_RC_CFG, RC5_EXT_DECODE);
        IR_WRITE_REG_MASK(ir_dev->membase, REG_RC_WAKE_UP, RC5_MODE_WAKE_UP, RC5_MODE_WAKE_UP);
    }
    else if (ir_dev->decode_mode == IR_MODE_SWDECODE)
    {
        IR_WRITE_REG_MASK(ir_dev->membase, REG_IR_GLHRM_NUM, SW_MODE_GLHRM_NUM, SW_MODE_GLHRM_NUM);
        IR_WRITE_REG_MASK(ir_dev->membase, REG_IR_SEPR_BIT_FIFO_CTRL, SW_MODE_FIFO_CTRL_EDGE_ALL,
                          SW_MODE_FIFO_CTRL_EDGE_ALL);
        hal_set_sw_decode(ir_dev, 1);
        IR_WRITE_REG(ir_dev->membase, REG_IR_CKDIV_NUM_KEY_DATA, SW_MODE_CKDIV_NUM);
    }
    else
    {
        IR_WRITE_REG_MASK(ir_dev->membase, REG_IR_GLHRM_NUM, SW_MODE_GLHRM_NUM, SW_MODE_GLHRM_NUM);
        if (ir_dev->decode_mode == IR_MODE_SPEC)
        {
            regVal = IR_READ_REG(ir_dev->membase, REG_IR_SEPR_BIT_FIFO_CTRL) | SW_MODE_FIFO_CTRL_EDGE_PSHOT;
            IR_WRITE_REG(ir_dev->membase, REG_IR_SEPR_BIT_FIFO_CTRL, regVal);
        }
        else
        {
#ifdef IR_INT_NP_EDGE_TRIG
            regVal = IR_READ_REG(ir_dev->membase, REG_IR_SEPR_BIT_FIFO_CTRL) | SW_MODE_FIFO_CTRL_EDGE_ALL;
            IR_WRITE_REG(ir_dev->membase, REG_IR_SEPR_BIT_FIFO_CTRL, regVal);
#else
            IR_WRITE_REG(ir_dev->membase, REG_IR_SEPR_BIT_FIFO_CTRL,
                         SW_MODE_FIFO_CTRL_EDGE_NSHOT); //[10:8]: FIFO depth, [11]:Enable FIFO full
#endif
        }
    }

    if ((ir_dev->decode_mode == IR_MODE_RAWDATA) || (ir_dev->decode_mode == IR_MODE_FULLDECODE)
        || (ir_dev->decode_mode == IR_MODE_SWDECODE))
    {
        hal_clear_fifo(ir_dev);
    }

    return 0;
}

void hal_ir_config(struct hal_ir_dev *ir_dev)
{
    hal_ir_init(ir_dev);
    switch (ir_dev->decode_mode)
    {
        case IR_MODE_FULLDECODE:
            ir_info("successed to change decode mode into FULL\n");
            break;
        case IR_MODE_RAWDATA:
            ir_info("successed to change decode mode into RAW\n");
            break;
        case IR_MODE_SWDECODE:
            ir_info("successed to change decode mode into SW\n");
            break;
        case RC5_MODE:
            ir_info("successed to change decode mode into RC5\n");
            break;
    }
}

u16 hal_get_status(struct hal_ir_dev *ir_dev)
{
    u16 ret = 1;

    switch (ir_dev->decode_mode)
    {
        case IR_MODE_FULLDECODE:
        case IR_MODE_RAWDATA:
        case IR_MODE_SWDECODE:
            ret = IR_READ_REG(ir_dev->membase, REG_IR_SHOT_CNT_H_FIFO_STATUS) & IR_FIFO_EMPTY;
            break;
        case RC5_MODE:
            ret = IR_READ_REG(ir_dev->membase, REG_RC_FIFO_STATUS) & RC5_MODE_FIFO_EMPTY;
            break;
        default:
            ir_err("UNKNOW MODE\n");
    }

    return ret;
}

u32 hal_get_key(struct hal_ir_dev *ir_dev)
{
    ir_dev->ir_keyinfo->u8Valid  = 0;
    ir_dev->ir_keyinfo->u8System = 0;
    ir_dev->ir_keyinfo->u8Key    = 0;
    ir_dev->ir_keyinfo->u8Flag   = 0;

    if (ir_dev->decode_mode == IR_MODE_FULLDECODE)
    {
        ir_dbg("USE FULL MODE\n");
        hal_get_key_fullmode(ir_dev, ir_dev->ir_keyinfo);
    }
    else if (ir_dev->decode_mode == IR_MODE_RAWDATA)
    {
        ir_dbg("USE RAW MODE\n");
        hal_get_key_rawmode(ir_dev, ir_dev->ir_keyinfo);
    }
    else if (ir_dev->decode_mode == RC5_MODE)
    {
        ir_dbg("USE RC5 MODE\n");
        hal_get_key_rc5mode(ir_dev, ir_dev->ir_keyinfo);
    }
    else if (ir_dev->decode_mode == IR_MODE_SWDECODE)
    {
        ir_dbg("USE SW MODE\n");
        hal_get_key_swmode(ir_dev);
    }

    ir_dbg("IR KEY INFO System  is 0x%x\n", ir_dev->ir_keyinfo->u8System);
    ir_dbg("IR KEY INFO Flag    is 0x%x\n", ir_dev->ir_keyinfo->u8Flag);
    ir_dbg("IR KEY INFO Valid   is 0x%x\n", ir_dev->ir_keyinfo->u8Valid);
    ir_dbg("IR KEY INFO Key     is 0x%x\n", ir_dev->ir_keyinfo->u8Key);
    return (ir_dev->ir_keyinfo->u8Valid) ? (ir_dev->ir_keyinfo->u8System << 8 | ir_dev->ir_keyinfo->u8Key)
                                         : (ir_dev->ir_keyinfo->u8Valid);
}

