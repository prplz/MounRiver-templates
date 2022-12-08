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

#include <hal_padmux.h>
#include <gpio.h>
#include <padmux.h>
#include <mdrv_puse.h>

pad_info_t schematic[] =
{
#if defined(CONFIG_I2C_SUPPORT)
    {PAD_I2C5_SCL,          PINMUX_FOR_I2C5_MODE_1,             MDRV_PUSE_I2C5_SCL},
    {PAD_I2C5_SDA,          PINMUX_FOR_I2C5_MODE_1,             MDRV_PUSE_I2C5_SDA},
#endif
    //for console
    {PAD_PM_GPIO0,          PINMUX_FOR_PM_UART1_MODE_1,         MDRV_PUSE_PM_UART_RX},
    {PAD_PM_GPIO1,          PINMUX_FOR_PM_UART1_MODE_1,         MDRV_PUSE_PM_UART_TX},
};

int size_of_schematic = sizeof(schematic);

