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

// Include files
/*=============================================================*/

#ifdef __VER_MSPI__

#include "vm_types.ht"
#include "sys_sys_isw_cli.h"
#include "sys_sys_isw_uart.h"
#include "drv_mspi.h"

static u8 rdata[512];
static u8 wdata[512];

static int spi_ut_test(CLI_t * pCli, char * p)
{
    u32  i;
    s32  ret;
    u8   argc;
    u32  port;
    u32  cs_select;
    u32  speed;
    char *cmd;
    struct spi_setup setup = {0};
    struct spi_tfr transfers[2] = {0};

    argc = CliTokenCount(pCli);

    if (argc != 4)
        return eCLI_PARSE_INPUT_ERROR;

    cmd = CliTokenPop(pCli);

    if (CliTokenPopNum(pCli, &port, 0) != eCLI_PARSE_OK)
    {
        return eCLI_PARSE_INPUT_ERROR;
    }

    if (CliTokenPopNum(pCli, &cs_select, 0) != eCLI_PARSE_OK)
    {
        return eCLI_PARSE_INPUT_ERROR;
    }

    if (CliTokenPopNum(pCli, &speed, 0) != eCLI_PARSE_OK)
    {
        return eCLI_PARSE_INPUT_ERROR;
    }

    setup.bus_num = (u8)port;
    setup.chip_select = (u8)cs_select;
    setup.max_speed_hz = speed;
    //setup.mode = SPI_CPHA | SPI_CPOL;

    ret = spi_setup(&setup);
    if(ret)
    {
        cliPrintf("spi_setup fail : %d\n", ret);
        return eCLI_PARSE_UNKNOWN;
    }

    if (strcmp(cmd, "r") == 0)
    {
        wdata[0] = 0x03;
        wdata[1] = 0x00;
        wdata[2] = 0x00;
        wdata[3] = 0x00;
        transfers[0].tx_buf = wdata;
        transfers[0].rx_buf = NULL;
        transfers[0].len = 4;
        transfers[0].bits_per_word = 8;
        transfers[1].tx_buf = NULL;
        transfers[1].rx_buf = rdata;
        transfers[1].len = 256;
        transfers[1].bits_per_word = 8;
        ret = spi_transfer(&setup, transfers, 2);
        if (ret)
        {
            cliPrintf("spi_transfer fail : %d\n", ret);
            return eCLI_PARSE_UNKNOWN;
        }
    }
    else if (strcmp(cmd, "w") == 0)
    {
        wdata[0] = 0x06;
        transfers[0].tx_buf = wdata;
        transfers[0].rx_buf = NULL;
        transfers[0].len = 1;
        transfers[0].bits_per_word = 8;
        ret = spi_transfer(&setup, transfers, 1);
        if (ret)
        {
            cliPrintf("spi_transfer fail : %d\n", ret);
            return eCLI_PARSE_UNKNOWN;
        }

        wdata[0] = 0x02;
        wdata[1] = 0x00;
        wdata[2] = 0x00;
        wdata[3] = 0x00;
        for (i = 0; i < 256; i++)
        {
            wdata[i+4] = i;
        }
        transfers[0].tx_buf = wdata;
        transfers[0].rx_buf = NULL;
        transfers[0].len = 260;
        transfers[0].bits_per_word = 8;
        ret = spi_transfer(&setup, transfers, 1);
        if (ret)
        {
            cliPrintf("spi_transfer fail : %d\n", ret);
            return eCLI_PARSE_UNKNOWN;
        }
    }
    else if (strcmp(cmd, "e") == 0)
    {
        wdata[0] = 0x06;
        transfers[0].tx_buf = wdata;
        transfers[0].rx_buf = NULL;
        transfers[0].len = 1;
        transfers[0].bits_per_word = 8;
        ret = spi_transfer(&setup, transfers, 1);
        if (ret)
        {
            cliPrintf("spi_transfer fail : %d\n", ret);
            return eCLI_PARSE_UNKNOWN;
        }

        wdata[0] = 0x20;
        wdata[1] = 0x00;
        wdata[2] = 0x00;
        wdata[3] = 0x00;
        transfers[0].tx_buf = wdata;
        transfers[0].rx_buf = NULL;
        transfers[0].len = 4;
        transfers[0].bits_per_word = 8;
        ret = spi_transfer(&setup, transfers, 1);
        if (ret)
        {
            cliPrintf("spi_transfer fail : %d\n", ret);
            return eCLI_PARSE_UNKNOWN;
        }
    }
    else
    {
        return eCLI_PARSE_INPUT_ERROR;
    }

    return eCLI_PARSE_OK;
}

SS_RTOS_CLI_CMD(spi, " spi ut test (device WB25Q128)",
                         " spi <r/e/w> <port> <cs> <speed>\n"
                         " <r/e/w> r - read\n"
                         "         e - erase\n"
                         "         w - write", spi_ut_test);

#endif
