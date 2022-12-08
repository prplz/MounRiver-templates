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

#include "drv_ir.h"
#include "sys_sys_isw_cli.h"

#define __VER_IR__
#if defined(__VER_IR__)
void ir_get_value(struct sstar_ir_dev *ir_dev)
{
    u32 data;
    data = drv_ir_key_dequeue(ir_dev);
    if(data)
        printf("IR%u KEY VALUE [0x%x]\n", ir_dev->group, data);
    return;
}

static int _IRTest(CLI_t *pCli, char *p)
{
    u32 group_id;
    u32 decode_mode;
    struct sstar_ir_dev *ir_dev = NULL;

    if(CliTokenCount(pCli) == 2)
    {
        if (CliTokenPopNum(pCli, &group_id, 0) != eCLI_PARSE_OK)
            goto _IR_HELP_EXIT;
        if (CliTokenPopNum(pCli, &decode_mode, 0) != eCLI_PARSE_OK)
            goto _IR_HELP_EXIT;

        cliPrintf("group_id:%d, decode_mode:%d\n", group_id, decode_mode);
        ir_dev = drv_ir_get_addr(group_id);
        if (!ir_dev)
        {
            cliPrintf("ir group inexistence\n");
            return eCLI_PARSE_ERROR;
        }
        ir_dev->decode_mode = decode_mode;
        ir_dev->calbak_input_key = ir_get_value;
        if (drv_ir_config(ir_dev))
        {
            cliPrintf("ir group inexistence\n");
            return eCLI_PARSE_ERROR;
        }
    }
    else
    {
    _IR_HELP_EXIT:
        cliPrintf("Plz key in : ir [group_id] [decode_mode] \n");
        return eCLI_PARSE_INVALID_PARAMETER;
    }

    return eCLI_PARSE_OK;
}
SS_RTOS_CLI_CMD(ir,
        "Infrared receiver setup ",
        "",
        _IRTest);
#endif
