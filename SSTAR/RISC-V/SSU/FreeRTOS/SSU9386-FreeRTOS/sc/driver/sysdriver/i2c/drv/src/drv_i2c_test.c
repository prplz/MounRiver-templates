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

#if defined(__VER_I2C__)
#include "vm_types.ht"
#include "drv_bus_i2c.h"
#include "sys_sys_isw_cli.h"
#include "sys_sys_isw_uart.h"
#include "cam_drv_i2c.h"
#include "drv_iic.h"

static int i2c_ut_test(CLI_t * pCli, char * p)
{
    u8 argc;
    u32 port;
    u32 slave;
    char *cmd;
    tI2cHandle handle;
    tI2cMsg msg;
    u32 addr;
    u32 value;
    u8 data[4];
    u8 r_flag = 0;

    cmd = CliTokenPop(pCli);
    if (strcmp(cmd, "r") == 0)
    {
        argc = CliTokenCount(pCli);
        if (argc != 4)
            return eCLI_PARSE_INPUT_ERROR;
        r_flag = 1;

    }
    else if (strcmp(cmd, "w") == 0)
    {
        argc = CliTokenCount(pCli);
        if (argc != 5)
            return eCLI_PARSE_INPUT_ERROR;
    }
    else
    {
        return eCLI_PARSE_INPUT_ERROR;
    }

    if (CliTokenPopNum(pCli, &port, 0) != eCLI_PARSE_OK)
    {
        return eCLI_PARSE_INPUT_ERROR;
    }

    if (CliTokenPopNum(pCli, &slave, 0) != eCLI_PARSE_OK)
    {
        return eCLI_PARSE_INPUT_ERROR;
    }

    cmd = CliTokenPop(pCli);
    if (strcmp(cmd, "A8D8") == 0)
    {
        handle.nPortNum = (s16)port;
        msg.addr = (u16)slave;
        if (r_flag)
        {
            if (CliTokenPopNum(pCli, &addr, 0) != eCLI_PARSE_OK)
            {
                return eCLI_PARSE_INPUT_ERROR;
            }

            msg.flags = 0;
            data[0] = addr;
            msg.buf = data;
            msg.len = 1;
            CamI2cTransfer(&handle, &msg, 1);

            msg.flags = CAM_I2C_RD;
            msg.buf = data;
            msg.len = 1;
            CamI2cTransfer(&handle, &msg, 1);
            cliPrintf(" %02x : %02x\r\n", (u8)addr, data[0]);
        }
        else
        {
            if (CliTokenPopNum(pCli, &addr, 0) != eCLI_PARSE_OK)
            {
                return eCLI_PARSE_INPUT_ERROR;
            }

            if (CliTokenPopNum(pCli, &value, 0) != eCLI_PARSE_OK)
            {
                return eCLI_PARSE_INPUT_ERROR;
            }

            data[0] = (u8)(addr & 0xff);
            data[1] = (u8)(value & 0xff);

            msg.flags = 0;
            msg.buf = data;
            msg.len = 2;

            CamI2cTransfer(&handle, &msg, 1);
        }
    }
    else if (strcmp(cmd, "A8D16") == 0)
    {
        handle.nPortNum = (s16)port;
        msg.addr = (u16)slave;
        if (r_flag)
        {
            if (CliTokenPopNum(pCli, &addr, 0) != eCLI_PARSE_OK)
            {
                return eCLI_PARSE_INPUT_ERROR;
            }

            msg.flags = 0;
            data[0] = addr;
            msg.buf = data;
            msg.len = 1;
            CamI2cTransfer(&handle, &msg, 1);

            msg.flags = CAM_I2C_RD;
            msg.buf = data;
            msg.len = 2;
            CamI2cTransfer(&handle, &msg, 1);
            value = (data[0] << 8) + data[1];
            cliPrintf(" %02x : %04x\r\n", (u8)addr, (u16)value);
        }
        else
        {
            if (CliTokenPopNum(pCli, &addr, 0) != eCLI_PARSE_OK)
            {
                return eCLI_PARSE_INPUT_ERROR;
            }

            if (CliTokenPopNum(pCli, &value, 0) != eCLI_PARSE_OK)
            {
                return eCLI_PARSE_INPUT_ERROR;
            }

            data[0] = (u8)(addr & 0xff);
            data[1] = (u8)((value >> 8) & 0xff);
            data[2] = (u8)((value ) & 0xff);

            msg.flags = 0;
            msg.buf = data;
            msg.len = 3;

            CamI2cTransfer(&handle, &msg, 1);
        }

    }
    else if (strcmp(cmd, "A16D8") == 0)
    {
        handle.nPortNum = (s16)port;
        msg.addr = (u16)slave;
        if (r_flag)
        {
            if (CliTokenPopNum(pCli, &addr, 0) != eCLI_PARSE_OK)
            {
                return eCLI_PARSE_INPUT_ERROR;
            }

            msg.flags = 0;
            data[0] = (u8)((addr >> 8) & 0xff);
            data[1] = (u8)(addr & 0xff);
            msg.buf = data;
            msg.len = 2;
            CamI2cTransfer(&handle, &msg, 1);

            msg.flags = CAM_I2C_RD;
            msg.buf = data;
            msg.len = 1;
            CamI2cTransfer(&handle, &msg, 1);
            cliPrintf(" %04x : %02x\r\n", (u16)addr, (u8)data[0]);
        }
        else
        {
            if (CliTokenPopNum(pCli, &addr, 0) != eCLI_PARSE_OK)
            {
                return eCLI_PARSE_INPUT_ERROR;
            }

            if (CliTokenPopNum(pCli, &value, 0) != eCLI_PARSE_OK)
            {
                return eCLI_PARSE_INPUT_ERROR;
            }

            data[0] = (u8)((addr >> 8) & 0xff);
            data[1] = (u8)(addr & 0xff);
            data[2] = (u8)(value & 0xff);

            msg.flags = 0;
            msg.buf = data;
            msg.len = 3;

            CamI2cTransfer(&handle, &msg, 1);
        }

    }
    else if (strcmp(cmd, "A16D16") == 0)
    {
        handle.nPortNum = (s16)port;
        msg.addr = (u16)slave;
        if (r_flag)
        {
            if (CliTokenPopNum(pCli, &addr, 0) != eCLI_PARSE_OK)
            {
                return eCLI_PARSE_INPUT_ERROR;
            }

            msg.flags = 0;
            data[0] = (u8)((addr >> 8) & 0xff);
            data[1] = (u8)(addr & 0xff);
            msg.buf = data;
            msg.len = 2;
            CamI2cTransfer(&handle, &msg, 1);

            msg.flags = CAM_I2C_RD;
            msg.buf = data;
            msg.len = 2;
            CamI2cTransfer(&handle, &msg, 1);
            value = (data[0] << 8) + data[1];
            cliPrintf(" %04x : %04x\r\n", (u16)addr, (u16)value);
        }
        else
        {
            if (CliTokenPopNum(pCli, &addr, 0) != eCLI_PARSE_OK)
            {
                return eCLI_PARSE_INPUT_ERROR;
            }

            if (CliTokenPopNum(pCli, &value, 0) != eCLI_PARSE_OK)
            {
                return eCLI_PARSE_INPUT_ERROR;
            }

            data[0] = (u8)((addr >> 8) & 0xff);
            data[1] = (u8)((addr) & 0xff);
            data[2] = (u8)((value >> 8) & 0xff);
            data[3] = (u8)((value ) & 0xff);

            msg.flags = 0;
            msg.buf = data;
            msg.len = 4;

            CamI2cTransfer(&handle, &msg, 1);
        }

    }

    return eCLI_PARSE_OK;
}

SS_RTOS_CLI_CMD(i2c, " i2c ut test",
                         " i2c <r/w> <port> <slave> <format> <data...>\r\n"
                         " <format> : A8D8 A8D16 A16D8 A16D16\r\n"
                         " e.g.\r\n"
                         "    i2c w 0 0x50 A16D8 0x0000 0x55\r\n"
                         "    i2c r 0 0x50 A16D8 0x0000", i2c_ut_test);
#endif
