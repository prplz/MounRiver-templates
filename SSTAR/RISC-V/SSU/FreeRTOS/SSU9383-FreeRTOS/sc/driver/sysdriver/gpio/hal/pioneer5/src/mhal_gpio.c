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

#include "mhal_gpio.h"
#include "gpio.h"
#include "padmux.h"
#include "mhal_pinmux.h"
#include "gpi-irqs.h"
#include "pmsleep-irqs.h"
#include "hal_int_ctrl_pub.h"
#include "drv_int_ctrl_pub_api.h"
#include "registers.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define GPIO_DBG 0
#if GPIO_DBG
#define GPIO_ERR(args...) printk(args)
#else
#define GPIO_ERR(args...)
#endif

#define _CONCAT( a, b )     a##b
#define CONCAT( a, b )      _CONCAT( a, b )

#define GPIO0_PAD        PAD_I2C5_SCL
#define GPIO0_NAME       "PAD_I2C5_SCL"
#define GPIO0_OEN        0x103E00, BIT2
#define GPIO0_OUT        0x103E00, BIT1
#define GPIO0_IN         0x103E00, BIT0
#define GPIO0_DRV        0x103E00, BIT7|BIT8
#define GPIO0_PE         0x103E00, BIT6
#define GPIO0_PS         0x103E00, BIT11

#define GPIO1_PAD        PAD_I2C5_SDA
#define GPIO1_NAME       "PAD_I2C5_SDA"
#define GPIO1_OEN        0x103E02, BIT2
#define GPIO1_OUT        0x103E02, BIT1
#define GPIO1_IN         0x103E02, BIT0
#define GPIO1_DRV        0x103E02, BIT7|BIT8
#define GPIO1_PE         0x103E02, BIT6
#define GPIO1_PS         0x103E02, BIT11

#define GPIO2_PAD        PAD_SD0_GPIO0
#define GPIO2_NAME       "PAD_SD0_GPIO0"
#define GPIO2_OEN        0x103E04, BIT2
#define GPIO2_OUT        0x103E04, BIT1
#define GPIO2_IN         0x103E04, BIT0
#define GPIO2_DRV        0x000000, BIT0
#define GPIO2_PE         0x103E04, BIT6
#define GPIO2_PS         0x103E04, BIT11

#define GPIO3_PAD        PAD_SD0_VCTRL
#define GPIO3_NAME       "PAD_SD0_VCTRL"
#define GPIO3_OEN        0x103E06, BIT2
#define GPIO3_OUT        0x103E06, BIT1
#define GPIO3_IN         0x103E06, BIT0
#define GPIO3_DRV        0x000000, BIT0
#define GPIO3_PE         0x103E06, BIT6
#define GPIO3_PS         0x103E06, BIT11

#define GPIO4_PAD        PAD_SD0_CDZ
#define GPIO4_NAME       "PAD_SD0_CDZ"
#define GPIO4_OEN        0x103E08, BIT2
#define GPIO4_OUT        0x103E08, BIT1
#define GPIO4_IN         0x103E08, BIT0
#define GPIO4_DRV        0x000000, BIT0
#define GPIO4_PE         0x103E08, BIT6
#define GPIO4_PS         0x103E08, BIT11

#define GPIO5_PAD        PAD_SD0_D1
#define GPIO5_NAME       "PAD_SD0_D1"
#define GPIO5_OEN        0x103E0A, BIT2
#define GPIO5_OUT        0x103E0A, BIT1
#define GPIO5_IN         0x103E0A, BIT0
#define GPIO5_DRV        0x000000, BIT0
#define GPIO5_PE         0x103E0A, BIT6
#define GPIO5_PS         0x103E0A, BIT11

#define GPIO6_PAD        PAD_SD0_D0
#define GPIO6_NAME       "PAD_SD0_D0"
#define GPIO6_OEN        0x103E0C, BIT2
#define GPIO6_OUT        0x103E0C, BIT1
#define GPIO6_IN         0x103E0C, BIT0
#define GPIO6_DRV        0x000000, BIT0
#define GPIO6_PE         0x103E0C, BIT6
#define GPIO6_PS         0x103E0C, BIT11

#define GPIO7_PAD        PAD_SD0_CLK
#define GPIO7_NAME       "PAD_SD0_CLK"
#define GPIO7_OEN        0x103E0E, BIT2
#define GPIO7_OUT        0x103E0E, BIT1
#define GPIO7_IN         0x103E0E, BIT0
#define GPIO7_DRV        0x000000, BIT0
#define GPIO7_PE         0x103E0E, BIT6
#define GPIO7_PS         0x103E0E, BIT11

#define GPIO8_PAD        PAD_SD0_CMD
#define GPIO8_NAME       "PAD_SD0_CMD"
#define GPIO8_OEN        0x103E10, BIT2
#define GPIO8_OUT        0x103E10, BIT1
#define GPIO8_IN         0x103E10, BIT0
#define GPIO8_DRV        0x000000, BIT0
#define GPIO8_PE         0x103E10, BIT6
#define GPIO8_PS         0x103E10, BIT11

#define GPIO9_PAD        PAD_SD0_D3
#define GPIO9_NAME       "PAD_SD0_D3"
#define GPIO9_OEN        0x103E12, BIT2
#define GPIO9_OUT        0x103E12, BIT1
#define GPIO9_IN         0x103E12, BIT0
#define GPIO9_DRV        0x000000, BIT0
#define GPIO9_PE         0x103E12, BIT6
#define GPIO9_PS         0x103E12, BIT11

#define GPIO10_PAD       PAD_SD0_D2
#define GPIO10_NAME      "PAD_SD0_D2"
#define GPIO10_OEN       0x103E14, BIT2
#define GPIO10_OUT       0x103E14, BIT1
#define GPIO10_IN        0x103E14, BIT0
#define GPIO10_DRV       0x000000, BIT0
#define GPIO10_PE        0x103E14, BIT6
#define GPIO10_PS        0x103E14, BIT11

#define GPIO11_PAD       PAD_KEY0
#define GPIO11_NAME      "PAD_KEY0"
#define GPIO11_OEN       0x103E16, BIT2
#define GPIO11_OUT       0x103E16, BIT1
#define GPIO11_IN        0x103E16, BIT0
#define GPIO11_DRV       0x103E16, BIT7
#define GPIO11_PE        0x103E16, BIT6
#define GPIO11_PS        0x103E16, BIT11

#define GPIO12_PAD       PAD_KEY1
#define GPIO12_NAME      "PAD_KEY1"
#define GPIO12_OEN       0x103E18, BIT2
#define GPIO12_OUT       0x103E18, BIT1
#define GPIO12_IN        0x103E18, BIT0
#define GPIO12_DRV       0x103E18, BIT7
#define GPIO12_PE        0x103E18, BIT6
#define GPIO12_PS        0x103E18, BIT11

#define GPIO13_PAD       PAD_KEY2
#define GPIO13_NAME      "PAD_KEY2"
#define GPIO13_OEN       0x103E1A, BIT2
#define GPIO13_OUT       0x103E1A, BIT1
#define GPIO13_IN        0x103E1A, BIT0
#define GPIO13_DRV       0x103E1A, BIT7
#define GPIO13_PE        0x103E1A, BIT6
#define GPIO13_PS        0x103E1A, BIT11

#define GPIO14_PAD       PAD_KEY3
#define GPIO14_NAME      "PAD_KEY3"
#define GPIO14_OEN       0x103E1C, BIT2
#define GPIO14_OUT       0x103E1C, BIT1
#define GPIO14_IN        0x103E1C, BIT0
#define GPIO14_DRV       0x103E1C, BIT7
#define GPIO14_PE        0x103E1C, BIT6
#define GPIO14_PS        0x103E1C, BIT11

#define GPIO15_PAD       PAD_KEY4
#define GPIO15_NAME      "PAD_KEY4"
#define GPIO15_OEN       0x103E1E, BIT2
#define GPIO15_OUT       0x103E1E, BIT1
#define GPIO15_IN        0x103E1E, BIT0
#define GPIO15_DRV       0x103E1E, BIT7
#define GPIO15_PE        0x103E1E, BIT6
#define GPIO15_PS        0x103E1E, BIT11

#define GPIO16_PAD       PAD_KEY5
#define GPIO16_NAME      "PAD_KEY5"
#define GPIO16_OEN       0x103E20, BIT2
#define GPIO16_OUT       0x103E20, BIT1
#define GPIO16_IN        0x103E20, BIT0
#define GPIO16_DRV       0x103E20, BIT7
#define GPIO16_PE        0x103E20, BIT6
#define GPIO16_PS        0x103E20, BIT11

#define GPIO17_PAD       PAD_KEY6
#define GPIO17_NAME      "PAD_KEY6"
#define GPIO17_OEN       0x103E22, BIT2
#define GPIO17_OUT       0x103E22, BIT1
#define GPIO17_IN        0x103E22, BIT0
#define GPIO17_DRV       0x103E22, BIT7
#define GPIO17_PE        0x103E22, BIT6
#define GPIO17_PS        0x103E22, BIT11

#define GPIO18_PAD       PAD_KEY7
#define GPIO18_NAME      "PAD_KEY7"
#define GPIO18_OEN       0x103E24, BIT2
#define GPIO18_OUT       0x103E24, BIT1
#define GPIO18_IN        0x103E24, BIT0
#define GPIO18_DRV       0x103E24, BIT7
#define GPIO18_PE        0x103E24, BIT6
#define GPIO18_PS        0x103E24, BIT11

#define GPIO19_PAD       PAD_KEY8
#define GPIO19_NAME      "PAD_KEY8"
#define GPIO19_OEN       0x103E26, BIT2
#define GPIO19_OUT       0x103E26, BIT1
#define GPIO19_IN        0x103E26, BIT0
#define GPIO19_DRV       0x103E26, BIT7
#define GPIO19_PE        0x103E26, BIT6
#define GPIO19_PS        0x103E26, BIT11

#define GPIO20_PAD       PAD_KEY9
#define GPIO20_NAME      "PAD_KEY9"
#define GPIO20_OEN       0x103E28, BIT2
#define GPIO20_OUT       0x103E28, BIT1
#define GPIO20_IN        0x103E28, BIT0
#define GPIO20_DRV       0x103E28, BIT7
#define GPIO20_PE        0x103E28, BIT6
#define GPIO20_PS        0x103E28, BIT11

#define GPIO21_PAD       PAD_KEY10
#define GPIO21_NAME      "PAD_KEY10"
#define GPIO21_OEN       0x103E2A, BIT2
#define GPIO21_OUT       0x103E2A, BIT1
#define GPIO21_IN        0x103E2A, BIT0
#define GPIO21_DRV       0x103E2A, BIT7
#define GPIO21_PE        0x103E2A, BIT6
#define GPIO21_PS        0x103E2A, BIT11

#define GPIO22_PAD       PAD_KEY11
#define GPIO22_NAME      "PAD_KEY11"
#define GPIO22_OEN       0x103E2C, BIT2
#define GPIO22_OUT       0x103E2C, BIT1
#define GPIO22_IN        0x103E2C, BIT0
#define GPIO22_DRV       0x103E2C, BIT7
#define GPIO22_PE        0x103E2C, BIT6
#define GPIO22_PS        0x103E2C, BIT11

#define GPIO23_PAD       PAD_KEY12
#define GPIO23_NAME      "PAD_KEY12"
#define GPIO23_OEN       0x103E2E, BIT2
#define GPIO23_OUT       0x103E2E, BIT1
#define GPIO23_IN        0x103E2E, BIT0
#define GPIO23_DRV       0x103E2E, BIT7
#define GPIO23_PE        0x103E2E, BIT6
#define GPIO23_PS        0x103E2E, BIT11

#define GPIO24_PAD       PAD_KEY13
#define GPIO24_NAME      "PAD_KEY13"
#define GPIO24_OEN       0x103E30, BIT2
#define GPIO24_OUT       0x103E30, BIT1
#define GPIO24_IN        0x103E30, BIT0
#define GPIO24_DRV       0x103E30, BIT7
#define GPIO24_PE        0x103E30, BIT6
#define GPIO24_PS        0x103E30, BIT11

#define GPIO25_PAD       PAD_PM_PWM0
#define GPIO25_NAME      "PAD_PM_PWM0"
#define GPIO25_OEN       0x103E32, BIT2
#define GPIO25_OUT       0x103E32, BIT1
#define GPIO25_IN        0x103E32, BIT0
#define GPIO25_DRV       0x103E32, BIT7
#define GPIO25_PE        0x103E32, BIT6
#define GPIO25_PS        0x103E32, BIT11

#define GPIO26_PAD       PAD_PM_PWM1
#define GPIO26_NAME      "PAD_PM_PWM1"
#define GPIO26_OEN       0x103E34, BIT2
#define GPIO26_OUT       0x103E34, BIT1
#define GPIO26_IN        0x103E34, BIT0
#define GPIO26_DRV       0x103E34, BIT7
#define GPIO26_PE        0x103E34, BIT6
#define GPIO26_PS        0x103E34, BIT11

#define GPIO27_PAD       PAD_PM_I2CM_SCL
#define GPIO27_NAME      "PAD_PM_I2CM_SCL"
#define GPIO27_OEN       0x103E36, BIT2
#define GPIO27_OUT       0x103E36, BIT1
#define GPIO27_IN        0x103E36, BIT0
#define GPIO27_DRV       0x103E36, BIT7
#define GPIO27_PE        0x103E36, BIT6
#define GPIO27_PS        0x103E36, BIT11

#define GPIO28_PAD       PAD_PM_I2CM_SDA
#define GPIO28_NAME      "PAD_PM_I2CM_SDA"
#define GPIO28_OEN       0x103E38, BIT2
#define GPIO28_OUT       0x103E38, BIT1
#define GPIO28_IN        0x103E38, BIT0
#define GPIO28_DRV       0x103E38, BIT7
#define GPIO28_PE        0x103E38, BIT6
#define GPIO28_PS        0x103E38, BIT11

#define GPIO29_PAD       PAD_PM_UART_RX0
#define GPIO29_NAME      "PAD_PM_UART_RX0"
#define GPIO29_OEN       0x103E3A, BIT2
#define GPIO29_OUT       0x103E3A, BIT1
#define GPIO29_IN        0x103E3A, BIT0
#define GPIO29_DRV       0x103E3A, BIT7
#define GPIO29_PE        0x103E3A, BIT6
#define GPIO29_PS        0x103E3A, BIT11

#define GPIO30_PAD       PAD_PM_UART_TX0
#define GPIO30_NAME      "PAD_PM_UART_TX0"
#define GPIO30_OEN       0x103E3C, BIT2
#define GPIO30_OUT       0x103E3C, BIT1
#define GPIO30_IN        0x103E3C, BIT0
#define GPIO30_DRV       0x103E3C, BIT7
#define GPIO30_PE        0x103E3C, BIT6
#define GPIO30_PS        0x103E3C, BIT11

#define GPIO31_PAD       PAD_PM_IR_RX
#define GPIO31_NAME      "PAD_PM_IR_RX"
#define GPIO31_OEN       0x103E3E, BIT2
#define GPIO31_OUT       0x103E3E, BIT1
#define GPIO31_IN        0x103E3E, BIT0
#define GPIO31_DRV       0x103E3E, BIT7
#define GPIO31_PE        0x103E3E, BIT6
#define GPIO31_PS        0x103E3E, BIT11

#define GPIO32_PAD       PAD_PM_GPIO0
#define GPIO32_NAME      "PAD_PM_GPIO0"
#define GPIO32_OEN       0x103E40, BIT2
#define GPIO32_OUT       0x103E40, BIT1
#define GPIO32_IN        0x103E40, BIT0
#define GPIO32_DRV       0x103E40, BIT7
#define GPIO32_PE        0x103E40, BIT6
#define GPIO32_PS        0x103E40, BIT11

#define GPIO33_PAD       PAD_PM_GPIO1
#define GPIO33_NAME      "PAD_PM_GPIO1"
#define GPIO33_OEN       0x103E42, BIT2
#define GPIO33_OUT       0x103E42, BIT1
#define GPIO33_IN        0x103E42, BIT0
#define GPIO33_DRV       0x103E42, BIT7
#define GPIO33_PE        0x103E42, BIT6
#define GPIO33_PS        0x103E42, BIT11

#define GPIO34_PAD       PAD_PM_GPIO2
#define GPIO34_NAME      "PAD_PM_GPIO2"
#define GPIO34_OEN       0x103E44, BIT2
#define GPIO34_OUT       0x103E44, BIT1
#define GPIO34_IN        0x103E44, BIT0
#define GPIO34_DRV       0x000000, BIT0
#define GPIO34_PE        0x000000, BIT0
#define GPIO34_PS        0x000000, BIT0

#define GPIO35_PAD       PAD_PM_GPIO3
#define GPIO35_NAME      "PAD_PM_GPIO3"
#define GPIO35_OEN       0x103E46, BIT2
#define GPIO35_OUT       0x103E46, BIT1
#define GPIO35_IN        0x103E46, BIT0
#define GPIO35_DRV       0x000000, BIT0
#define GPIO35_PE        0x000000, BIT0
#define GPIO35_PS        0x000000, BIT0

#define GPIO36_PAD       PAD_PM_GPIO4
#define GPIO36_NAME      "PAD_PM_GPIO4"
#define GPIO36_OEN       0x103E48, BIT2
#define GPIO36_OUT       0x103E48, BIT1
#define GPIO36_IN        0x103E48, BIT0
#define GPIO36_DRV       0x000000, BIT0
#define GPIO36_PE        0x000000, BIT0
#define GPIO36_PS        0x000000, BIT0

#define GPIO37_PAD       PAD_PM_GPIO5
#define GPIO37_NAME      "PAD_PM_GPIO5"
#define GPIO37_OEN       0x103E4A, BIT2
#define GPIO37_OUT       0x103E4A, BIT1
#define GPIO37_IN        0x103E4A, BIT0
#define GPIO37_DRV       0x000000, BIT0
#define GPIO37_PE        0x000000, BIT0
#define GPIO37_PS        0x000000, BIT0

#define GPIO38_PAD       PAD_PM_SPI_WPZ
#define GPIO38_NAME      "PAD_PM_SPI_WPZ"
#define GPIO38_OEN       0x103E4C, BIT2
#define GPIO38_OUT       0x103E4C, BIT1
#define GPIO38_IN        0x103E4C, BIT0
#define GPIO38_DRV       0x003F1A, BIT7|BIT8
#define GPIO38_PE        0x103E4C, BIT6
#define GPIO38_PS        0x103E4C, BIT11

#define GPIO39_PAD       PAD_PM_SPI_DO
#define GPIO39_NAME      "PAD_PM_SPI_DO"
#define GPIO39_OEN       0x103E4E, BIT2
#define GPIO39_OUT       0x103E4E, BIT1
#define GPIO39_IN        0x103E4E, BIT0
#define GPIO39_DRV       0x003F1C, BIT7|BIT8
#define GPIO39_PE        0x103E4E, BIT6
#define GPIO39_PS        0x103E4E, BIT11

#define GPIO40_PAD       PAD_PM_SPI_CZ
#define GPIO40_NAME      "PAD_PM_SPI_CZ"
#define GPIO40_OEN       0x103E50, BIT2
#define GPIO40_OUT       0x103E50, BIT1
#define GPIO40_IN        0x103E50, BIT0
#define GPIO40_DRV       0x003F1E, BIT7|BIT8
#define GPIO40_PE        0x103E50, BIT6
#define GPIO40_PS        0x103E50, BIT11

#define GPIO41_PAD       PAD_PM_SPI_HLD
#define GPIO41_NAME      "PAD_PM_SPI_HLD"
#define GPIO41_OEN       0x103E52, BIT2
#define GPIO41_OUT       0x103E52, BIT1
#define GPIO41_IN        0x103E52, BIT0
#define GPIO41_DRV       0x003F20, BIT7|BIT8
#define GPIO41_PE        0x103E52, BIT6
#define GPIO41_PS        0x103E52, BIT11

#define GPIO42_PAD       PAD_PM_SPI_CK
#define GPIO42_NAME      "PAD_PM_SPI_CK"
#define GPIO42_OEN       0x103E54, BIT2
#define GPIO42_OUT       0x103E54, BIT1
#define GPIO42_IN        0x103E54, BIT0
#define GPIO42_DRV       0x003F22, BIT7|BIT8
#define GPIO42_PE        0x103E54, BIT6
#define GPIO42_PS        0x103E54, BIT11

#define GPIO43_PAD       PAD_PM_SPI_DI
#define GPIO43_NAME      "PAD_PM_SPI_DI"
#define GPIO43_OEN       0x103E56, BIT2
#define GPIO43_OUT       0x103E56, BIT1
#define GPIO43_IN        0x103E56, BIT0
#define GPIO43_DRV       0x003F24, BIT7|BIT8
#define GPIO43_PE        0x103E56, BIT6
#define GPIO43_PS        0x103E56, BIT11

#define GPIO44_PAD       PAD_EMMC_RSTN
#define GPIO44_NAME      "PAD_EMMC_RSTN"
#define GPIO44_OEN       0x103E58, BIT2
#define GPIO44_OUT       0x103E58, BIT1
#define GPIO44_IN        0x103E58, BIT0
#define GPIO44_DRV       0x000000, BIT0
#define GPIO44_PE        0x103E58, BIT6
#define GPIO44_PS        0x103E58, BIT11

#define GPIO45_PAD       PAD_EMMC_CLK
#define GPIO45_NAME      "PAD_EMMC_CLK"
#define GPIO45_OEN       0x103E5A, BIT2
#define GPIO45_OUT       0x103E5A, BIT1
#define GPIO45_IN        0x103E5A, BIT0
#define GPIO45_DRV       0x000000, BIT0
#define GPIO45_PE        0x103E5A, BIT6
#define GPIO45_PS        0x103E5A, BIT11

#define GPIO46_PAD       PAD_EMMC_CMD
#define GPIO46_NAME      "PAD_EMMC_CMD"
#define GPIO46_OEN       0x103E5C, BIT2
#define GPIO46_OUT       0x103E5C, BIT1
#define GPIO46_IN        0x103E5C, BIT0
#define GPIO46_DRV       0x000000, BIT0
#define GPIO46_PE        0x103E5C, BIT6
#define GPIO46_PS        0x103E5C, BIT11

#define GPIO47_PAD       PAD_EMMC_DS
#define GPIO47_NAME      "PAD_EMMC_DS"
#define GPIO47_OEN       0x103E5E, BIT2
#define GPIO47_OUT       0x103E5E, BIT1
#define GPIO47_IN        0x103E5E, BIT0
#define GPIO47_DRV       0x000000, BIT0
#define GPIO47_PE        0x103E5E, BIT6
#define GPIO47_PS        0x103E5E, BIT11

#define GPIO48_PAD       PAD_EMMC_D3
#define GPIO48_NAME      "PAD_EMMC_D3"
#define GPIO48_OEN       0x103E60, BIT2
#define GPIO48_OUT       0x103E60, BIT1
#define GPIO48_IN        0x103E60, BIT0
#define GPIO48_DRV       0x000000, BIT0
#define GPIO48_PE        0x103E60, BIT6
#define GPIO48_PS        0x103E60, BIT11

#define GPIO49_PAD       PAD_EMMC_D4
#define GPIO49_NAME      "PAD_EMMC_D4"
#define GPIO49_OEN       0x103E62, BIT2
#define GPIO49_OUT       0x103E62, BIT1
#define GPIO49_IN        0x103E62, BIT0
#define GPIO49_DRV       0x000000, BIT0
#define GPIO49_PE        0x103E62, BIT6
#define GPIO49_PS        0x103E62, BIT11

#define GPIO50_PAD       PAD_EMMC_D0
#define GPIO50_NAME      "PAD_EMMC_D0"
#define GPIO50_OEN       0x103E64, BIT2
#define GPIO50_OUT       0x103E64, BIT1
#define GPIO50_IN        0x103E64, BIT0
#define GPIO50_DRV       0x000000, BIT0
#define GPIO50_PE        0x103E64, BIT6
#define GPIO50_PS        0x103E64, BIT11

#define GPIO51_PAD       PAD_EMMC_D5
#define GPIO51_NAME      "PAD_EMMC_D5"
#define GPIO51_OEN       0x103E66, BIT2
#define GPIO51_OUT       0x103E66, BIT1
#define GPIO51_IN        0x103E66, BIT0
#define GPIO51_DRV       0x000000, BIT0
#define GPIO51_PE        0x103E66, BIT6
#define GPIO51_PS        0x103E66, BIT11

#define GPIO52_PAD       PAD_EMMC_D1
#define GPIO52_NAME      "PAD_EMMC_D1"
#define GPIO52_OEN       0x103E68, BIT2
#define GPIO52_OUT       0x103E68, BIT1
#define GPIO52_IN        0x103E68, BIT0
#define GPIO52_DRV       0x000000, BIT0
#define GPIO52_PE        0x103E68, BIT6
#define GPIO52_PS        0x103E68, BIT11

#define GPIO53_PAD       PAD_EMMC_D6
#define GPIO53_NAME      "PAD_EMMC_D6"
#define GPIO53_OEN       0x103E6A, BIT2
#define GPIO53_OUT       0x103E6A, BIT1
#define GPIO53_IN        0x103E6A, BIT0
#define GPIO53_DRV       0x000000, BIT0
#define GPIO53_PE        0x103E6A, BIT6
#define GPIO53_PS        0x103E6A, BIT11

#define GPIO54_PAD       PAD_EMMC_D2
#define GPIO54_NAME      "PAD_EMMC_D2"
#define GPIO54_OEN       0x103E6C, BIT2
#define GPIO54_OUT       0x103E6C, BIT1
#define GPIO54_IN        0x103E6C, BIT0
#define GPIO54_DRV       0x000000, BIT0
#define GPIO54_PE        0x103E6C, BIT6
#define GPIO54_PS        0x103E6C, BIT11

#define GPIO55_PAD       PAD_EMMC_D7
#define GPIO55_NAME      "PAD_EMMC_D7"
#define GPIO55_OEN       0x103E6E, BIT2
#define GPIO55_OUT       0x103E6E, BIT1
#define GPIO55_IN        0x103E6E, BIT0
#define GPIO55_DRV       0x000000, BIT0
#define GPIO55_PE        0x103E6E, BIT6
#define GPIO55_PS        0x103E6E, BIT11

#define GPIO56_PAD       PAD_OUTP_RX1_CH_0
#define GPIO56_NAME      "PAD_OUTP_RX1_CH_0"
#define GPIO56_OEN       0x103E70, BIT2
#define GPIO56_OUT       0x103E70, BIT1
#define GPIO56_IN        0x103E70, BIT0
#define GPIO56_DRV       0x103E70, BIT7|BIT8
#define GPIO56_PE        0x103E70, BIT6
#define GPIO56_PS        0x103E70, BIT11

#define GPIO57_PAD       PAD_OUTN_RX1_CH_0
#define GPIO57_NAME      "PAD_OUTN_RX1_CH_0"
#define GPIO57_OEN       0x103E72, BIT2
#define GPIO57_OUT       0x103E72, BIT1
#define GPIO57_IN        0x103E72, BIT0
#define GPIO57_DRV       0x103E72, BIT7|BIT8
#define GPIO57_PE        0x103E72, BIT6
#define GPIO57_PS        0x103E72, BIT11

#define GPIO58_PAD       PAD_OUTP_RX1_CH_1
#define GPIO58_NAME      "PAD_OUTP_RX1_CH_1"
#define GPIO58_OEN       0x103E74, BIT2
#define GPIO58_OUT       0x103E74, BIT1
#define GPIO58_IN        0x103E74, BIT0
#define GPIO58_DRV       0x103E74, BIT7|BIT8
#define GPIO58_PE        0x103E74, BIT6
#define GPIO58_PS        0x103E74, BIT11

#define GPIO59_PAD       PAD_OUTN_RX1_CH_1
#define GPIO59_NAME      "PAD_OUTN_RX1_CH_1"
#define GPIO59_OEN       0x103E76, BIT2
#define GPIO59_OUT       0x103E76, BIT1
#define GPIO59_IN        0x103E76, BIT0
#define GPIO59_DRV       0x103E76, BIT7|BIT8
#define GPIO59_PE        0x103E76, BIT6
#define GPIO59_PS        0x103E76, BIT11

#define GPIO60_PAD       PAD_OUTP_RX1_CH_2
#define GPIO60_NAME      "PAD_OUTP_RX1_CH_2"
#define GPIO60_OEN       0x103E78, BIT2
#define GPIO60_OUT       0x103E78, BIT1
#define GPIO60_IN        0x103E78, BIT0
#define GPIO60_DRV       0x103E78, BIT7|BIT8
#define GPIO60_PE        0x103E78, BIT6
#define GPIO60_PS        0x103E78, BIT11

#define GPIO61_PAD       PAD_OUTN_RX1_CH_2
#define GPIO61_NAME      "PAD_OUTN_RX1_CH_2"
#define GPIO61_OEN       0x103E7A, BIT2
#define GPIO61_OUT       0x103E7A, BIT1
#define GPIO61_IN        0x103E7A, BIT0
#define GPIO61_DRV       0x103E7A, BIT7|BIT8
#define GPIO61_PE        0x103E7A, BIT6
#define GPIO61_PS        0x103E7A, BIT11

#define GPIO62_PAD       PAD_OUTP_RX1_CH_3
#define GPIO62_NAME      "PAD_OUTP_RX1_CH_3"
#define GPIO62_OEN       0x103E7C, BIT2
#define GPIO62_OUT       0x103E7C, BIT1
#define GPIO62_IN        0x103E7C, BIT0
#define GPIO62_DRV       0x103E7C, BIT7|BIT8
#define GPIO62_PE        0x103E7C, BIT6
#define GPIO62_PS        0x103E7C, BIT11

#define GPIO63_PAD       PAD_OUTN_RX1_CH_3
#define GPIO63_NAME      "PAD_OUTN_RX1_CH_3"
#define GPIO63_OEN       0x103E7E, BIT2
#define GPIO63_OUT       0x103E7E, BIT1
#define GPIO63_IN        0x103E7E, BIT0
#define GPIO63_DRV       0x103E7E, BIT7|BIT8
#define GPIO63_PE        0x103E7E, BIT6
#define GPIO63_PS        0x103E7E, BIT11

#define GPIO64_PAD       PAD_OUTP_RX0_CH_0
#define GPIO64_NAME      "PAD_OUTP_RX0_CH_0"
#define GPIO64_OEN       0x103E80, BIT2
#define GPIO64_OUT       0x103E80, BIT1
#define GPIO64_IN        0x103E80, BIT0
#define GPIO64_DRV       0x103E80, BIT7|BIT8
#define GPIO64_PE        0x103E80, BIT6
#define GPIO64_PS        0x103E80, BIT11

#define GPIO65_PAD       PAD_OUTN_RX0_CH_0
#define GPIO65_NAME      "PAD_OUTN_RX0_CH_0"
#define GPIO65_OEN       0x103E82, BIT2
#define GPIO65_OUT       0x103E82, BIT1
#define GPIO65_IN        0x103E82, BIT0
#define GPIO65_DRV       0x103E82, BIT7|BIT8
#define GPIO65_PE        0x103E82, BIT6
#define GPIO65_PS        0x103E82, BIT11

#define GPIO66_PAD       PAD_OUTP_RX0_CH_1
#define GPIO66_NAME      "PAD_OUTP_RX0_CH_1"
#define GPIO66_OEN       0x103E84, BIT2
#define GPIO66_OUT       0x103E84, BIT1
#define GPIO66_IN        0x103E84, BIT0
#define GPIO66_DRV       0x103E84, BIT7|BIT8
#define GPIO66_PE        0x103E84, BIT6
#define GPIO66_PS        0x103E84, BIT11

#define GPIO67_PAD       PAD_OUTN_RX0_CH_1
#define GPIO67_NAME      "PAD_OUTN_RX0_CH_1"
#define GPIO67_OEN       0x103E86, BIT2
#define GPIO67_OUT       0x103E86, BIT1
#define GPIO67_IN        0x103E86, BIT0
#define GPIO67_DRV       0x103E86, BIT7|BIT8
#define GPIO67_PE        0x103E86, BIT6
#define GPIO67_PS        0x103E86, BIT11

#define GPIO68_PAD       PAD_OUTP_RX0_CH_2
#define GPIO68_NAME      "PAD_OUTP_RX0_CH_2"
#define GPIO68_OEN       0x103E88, BIT2
#define GPIO68_OUT       0x103E88, BIT1
#define GPIO68_IN        0x103E88, BIT0
#define GPIO68_DRV       0x103E88, BIT7|BIT8
#define GPIO68_PE        0x103E88, BIT6
#define GPIO68_PS        0x103E88, BIT11

#define GPIO69_PAD       PAD_OUTN_RX0_CH_2
#define GPIO69_NAME      "PAD_OUTN_RX0_CH_2"
#define GPIO69_OEN       0x103E8A, BIT2
#define GPIO69_OUT       0x103E8A, BIT1
#define GPIO69_IN        0x103E8A, BIT0
#define GPIO69_DRV       0x103E8A, BIT7|BIT8
#define GPIO69_PE        0x103E8A, BIT6
#define GPIO69_PS        0x103E8A, BIT11

#define GPIO70_PAD       PAD_OUTP_RX0_CH_3
#define GPIO70_NAME      "PAD_OUTP_RX0_CH_3"
#define GPIO70_OEN       0x103E8C, BIT2
#define GPIO70_OUT       0x103E8C, BIT1
#define GPIO70_IN        0x103E8C, BIT0
#define GPIO70_DRV       0x103E8C, BIT7|BIT8
#define GPIO70_PE        0x103E8C, BIT6
#define GPIO70_PS        0x103E8C, BIT11

#define GPIO71_PAD       PAD_OUTN_RX0_CH_3
#define GPIO71_NAME      "PAD_OUTN_RX0_CH_3"
#define GPIO71_OEN       0x103E8E, BIT2
#define GPIO71_OUT       0x103E8E, BIT1
#define GPIO71_IN        0x103E8E, BIT0
#define GPIO71_DRV       0x103E8E, BIT7|BIT8
#define GPIO71_PE        0x103E8E, BIT6
#define GPIO71_PS        0x103E8E, BIT11

#define GPIO72_PAD       PAD_OUTP_RX0_CH_4
#define GPIO72_NAME      "PAD_OUTP_RX0_CH_4"
#define GPIO72_OEN       0x103E90, BIT2
#define GPIO72_OUT       0x103E90, BIT1
#define GPIO72_IN        0x103E90, BIT0
#define GPIO72_DRV       0x103E90, BIT7|BIT8
#define GPIO72_PE        0x103E90, BIT6
#define GPIO72_PS        0x103E90, BIT11

#define GPIO73_PAD       PAD_OUTN_RX0_CH_4
#define GPIO73_NAME      "PAD_OUTN_RX0_CH_4"
#define GPIO73_OEN       0x103E92, BIT2
#define GPIO73_OUT       0x103E92, BIT1
#define GPIO73_IN        0x103E92, BIT0
#define GPIO73_DRV       0x103E92, BIT7|BIT8
#define GPIO73_PE        0x103E92, BIT6
#define GPIO73_PS        0x103E92, BIT11

#define GPIO74_PAD       PAD_OUTP_RX0_CH_5
#define GPIO74_NAME      "PAD_OUTP_RX0_CH_5"
#define GPIO74_OEN       0x103E94, BIT2
#define GPIO74_OUT       0x103E94, BIT1
#define GPIO74_IN        0x103E94, BIT0
#define GPIO74_DRV       0x103E94, BIT7|BIT8
#define GPIO74_PE        0x103E94, BIT6
#define GPIO74_PS        0x103E94, BIT11

#define GPIO75_PAD       PAD_OUTN_RX0_CH_5
#define GPIO75_NAME      "PAD_OUTN_RX0_CH_5"
#define GPIO75_OEN       0x103E96, BIT2
#define GPIO75_OUT       0x103E96, BIT1
#define GPIO75_IN        0x103E96, BIT0
#define GPIO75_DRV       0x103E96, BIT7|BIT8
#define GPIO75_PE        0x103E96, BIT6
#define GPIO75_PS        0x103E96, BIT11

#define GPIO76_PAD       PAD_SPDIF_TX
#define GPIO76_NAME      "PAD_SPDIF_TX"
#define GPIO76_OEN       0x103E98, BIT2
#define GPIO76_OUT       0x103E98, BIT1
#define GPIO76_IN        0x103E98, BIT0
#define GPIO76_DRV       0x103E98, BIT7
#define GPIO76_PE        0x103E98, BIT6
#define GPIO76_PS        0x103E98, BIT11

#define GPIO77_PAD       PAD_SR_IO3
#define GPIO77_NAME      "PAD_SR_IO3"
#define GPIO77_OEN       0x103E9A, BIT2
#define GPIO77_OUT       0x103E9A, BIT1
#define GPIO77_IN        0x103E9A, BIT0
#define GPIO77_DRV       0x103E9A, BIT7
#define GPIO77_PE        0x103E9A, BIT6
#define GPIO77_PS        0x103E9A, BIT11

#define GPIO78_PAD       PAD_SR_IO2
#define GPIO78_NAME      "PAD_SR_IO2"
#define GPIO78_OEN       0x103E9C, BIT2
#define GPIO78_OUT       0x103E9C, BIT1
#define GPIO78_IN        0x103E9C, BIT0
#define GPIO78_DRV       0x103E9C, BIT7
#define GPIO78_PE        0x103E9C, BIT6
#define GPIO78_PS        0x103E9C, BIT11

#define GPIO79_PAD       PAD_SR_IO1
#define GPIO79_NAME      "PAD_SR_IO1"
#define GPIO79_OEN       0x103E9E, BIT2
#define GPIO79_OUT       0x103E9E, BIT1
#define GPIO79_IN        0x103E9E, BIT0
#define GPIO79_DRV       0x103E9E, BIT7
#define GPIO79_PE        0x103E9E, BIT6
#define GPIO79_PS        0x103E9E, BIT11

#define GPIO80_PAD       PAD_SR_IO0
#define GPIO80_NAME      "PAD_SR_IO0"
#define GPIO80_OEN       0x103EA0, BIT2
#define GPIO80_OUT       0x103EA0, BIT1
#define GPIO80_IN        0x103EA0, BIT0
#define GPIO80_DRV       0x103EA0, BIT7
#define GPIO80_PE        0x103EA0, BIT6
#define GPIO80_PS        0x103EA0, BIT11

#define GPIO81_PAD       PAD_SR_PDN1
#define GPIO81_NAME      "PAD_SR_PDN1"
#define GPIO81_OEN       0x103EA2, BIT2
#define GPIO81_OUT       0x103EA2, BIT1
#define GPIO81_IN        0x103EA2, BIT0
#define GPIO81_DRV       0x103EA2, BIT7
#define GPIO81_PE        0x103EA2, BIT6
#define GPIO81_PS        0x103EA2, BIT11

#define GPIO82_PAD       PAD_SR_MCLK1
#define GPIO82_NAME      "PAD_SR_MCLK1"
#define GPIO82_OEN       0x103EA4, BIT2
#define GPIO82_OUT       0x103EA4, BIT1
#define GPIO82_IN        0x103EA4, BIT0
#define GPIO82_DRV       0x103EA4, BIT7
#define GPIO82_PE        0x103EA4, BIT6
#define GPIO82_PS        0x103EA4, BIT11

#define GPIO83_PAD       PAD_SR_RST1
#define GPIO83_NAME      "PAD_SR_RST1"
#define GPIO83_OEN       0x103EA6, BIT2
#define GPIO83_OUT       0x103EA6, BIT1
#define GPIO83_IN        0x103EA6, BIT0
#define GPIO83_DRV       0x103EA6, BIT7
#define GPIO83_PE        0x103EA6, BIT6
#define GPIO83_PS        0x103EA6, BIT11

#define GPIO84_PAD       PAD_I2C1_SDA
#define GPIO84_NAME      "PAD_I2C1_SDA"
#define GPIO84_OEN       0x103EA8, BIT2
#define GPIO84_OUT       0x103EA8, BIT1
#define GPIO84_IN        0x103EA8, BIT0
#define GPIO84_DRV       0x103EA8, BIT7
#define GPIO84_PE        0x103EA8, BIT6
#define GPIO84_PS        0x103EA8, BIT11

#define GPIO85_PAD       PAD_I2C1_SCL
#define GPIO85_NAME      "PAD_I2C1_SCL"
#define GPIO85_OEN       0x103EAA, BIT2
#define GPIO85_OUT       0x103EAA, BIT1
#define GPIO85_IN        0x103EAA, BIT0
#define GPIO85_DRV       0x103EAA, BIT7
#define GPIO85_PE        0x103EAA, BIT6
#define GPIO85_PS        0x103EAA, BIT11

#define GPIO86_PAD       PAD_SR_PDN0
#define GPIO86_NAME      "PAD_SR_PDN0"
#define GPIO86_OEN       0x103EAC, BIT2
#define GPIO86_OUT       0x103EAC, BIT1
#define GPIO86_IN        0x103EAC, BIT0
#define GPIO86_DRV       0x103EAC, BIT7
#define GPIO86_PE        0x103EAC, BIT6
#define GPIO86_PS        0x103EAC, BIT11

#define GPIO87_PAD       PAD_SR_MCLK0
#define GPIO87_NAME      "PAD_SR_MCLK0"
#define GPIO87_OEN       0x103EAE, BIT2
#define GPIO87_OUT       0x103EAE, BIT1
#define GPIO87_IN        0x103EAE, BIT0
#define GPIO87_DRV       0x103EAE, BIT7
#define GPIO87_PE        0x103EAE, BIT6
#define GPIO87_PS        0x103EAE, BIT11

#define GPIO88_PAD       PAD_SR_RST0
#define GPIO88_NAME      "PAD_SR_RST0"
#define GPIO88_OEN       0x103EB0, BIT2
#define GPIO88_OUT       0x103EB0, BIT1
#define GPIO88_IN        0x103EB0, BIT0
#define GPIO88_DRV       0x103EB0, BIT7
#define GPIO88_PE        0x103EB0, BIT6
#define GPIO88_PS        0x103EB0, BIT11

#define GPIO89_PAD       PAD_I2C0_SDA
#define GPIO89_NAME      "PAD_I2C0_SDA"
#define GPIO89_OEN       0x103EB2, BIT2
#define GPIO89_OUT       0x103EB2, BIT1
#define GPIO89_IN        0x103EB2, BIT0
#define GPIO89_DRV       0x103EB2, BIT7
#define GPIO89_PE        0x103EB2, BIT6
#define GPIO89_PS        0x103EB2, BIT11

#define GPIO90_PAD       PAD_I2C0_SCL
#define GPIO90_NAME      "PAD_I2C0_SCL"
#define GPIO90_OEN       0x103EB4, BIT2
#define GPIO90_OUT       0x103EB4, BIT1
#define GPIO90_IN        0x103EB4, BIT0
#define GPIO90_DRV       0x103EB4, BIT7
#define GPIO90_PE        0x103EB4, BIT6
#define GPIO90_PS        0x103EB4, BIT11

#define GPIO91_PAD       PAD_SR_PDN2
#define GPIO91_NAME      "PAD_SR_PDN2"
#define GPIO91_OEN       0x103EB6, BIT2
#define GPIO91_OUT       0x103EB6, BIT1
#define GPIO91_IN        0x103EB6, BIT0
#define GPIO91_DRV       0x103EB6, BIT7
#define GPIO91_PE        0x103EB6, BIT6
#define GPIO91_PS        0x103EB6, BIT11

#define GPIO92_PAD       PAD_SR_MCLK2
#define GPIO92_NAME      "PAD_SR_MCLK2"
#define GPIO92_OEN       0x103EB8, BIT2
#define GPIO92_OUT       0x103EB8, BIT1
#define GPIO92_IN        0x103EB8, BIT0
#define GPIO92_DRV       0x103EB8, BIT7
#define GPIO92_PE        0x103EB8, BIT6
#define GPIO92_PS        0x103EB8, BIT11

#define GPIO93_PAD       PAD_SR_RST2
#define GPIO93_NAME      "PAD_SR_RST2"
#define GPIO93_OEN       0x103EBA, BIT2
#define GPIO93_OUT       0x103EBA, BIT1
#define GPIO93_IN        0x103EBA, BIT0
#define GPIO93_DRV       0x103EBA, BIT7
#define GPIO93_PE        0x103EBA, BIT6
#define GPIO93_PS        0x103EBA, BIT11

#define GPIO94_PAD       PAD_I2C2_SDA
#define GPIO94_NAME      "PAD_I2C2_SDA"
#define GPIO94_OEN       0x103EBC, BIT2
#define GPIO94_OUT       0x103EBC, BIT1
#define GPIO94_IN        0x103EBC, BIT0
#define GPIO94_DRV       0x103EBC, BIT7
#define GPIO94_PE        0x103EBC, BIT6
#define GPIO94_PS        0x103EBC, BIT11

#define GPIO95_PAD       PAD_I2C2_SCL
#define GPIO95_NAME      "PAD_I2C2_SCL"
#define GPIO95_OEN       0x103EBE, BIT2
#define GPIO95_OUT       0x103EBE, BIT1
#define GPIO95_IN        0x103EBE, BIT0
#define GPIO95_DRV       0x103EBE, BIT7
#define GPIO95_PE        0x103EBE, BIT6
#define GPIO95_PS        0x103EBE, BIT11

#define GPIO96_PAD       PAD_SR_PDN3
#define GPIO96_NAME      "PAD_SR_PDN3"
#define GPIO96_OEN       0x103EC0, BIT2
#define GPIO96_OUT       0x103EC0, BIT1
#define GPIO96_IN        0x103EC0, BIT0
#define GPIO96_DRV       0x103EC0, BIT7
#define GPIO96_PE        0x103EC0, BIT6
#define GPIO96_PS        0x103EC0, BIT11

#define GPIO97_PAD       PAD_SR_MCLK3
#define GPIO97_NAME      "PAD_SR_MCLK3"
#define GPIO97_OEN       0x103EC2, BIT2
#define GPIO97_OUT       0x103EC2, BIT1
#define GPIO97_IN        0x103EC2, BIT0
#define GPIO97_DRV       0x103EC2, BIT7
#define GPIO97_PE        0x103EC2, BIT6
#define GPIO97_PS        0x103EC2, BIT11

#define GPIO98_PAD       PAD_SR_RST3
#define GPIO98_NAME      "PAD_SR_RST3"
#define GPIO98_OEN       0x103EC4, BIT2
#define GPIO98_OUT       0x103EC4, BIT1
#define GPIO98_IN        0x103EC4, BIT0
#define GPIO98_DRV       0x103EC4, BIT7
#define GPIO98_PE        0x103EC4, BIT6
#define GPIO98_PS        0x103EC4, BIT11

#define GPIO99_PAD       PAD_I2C3_SDA
#define GPIO99_NAME      "PAD_I2C3_SDA"
#define GPIO99_OEN       0x103EC6, BIT2
#define GPIO99_OUT       0x103EC6, BIT1
#define GPIO99_IN        0x103EC6, BIT0
#define GPIO99_DRV       0x103EC6, BIT7
#define GPIO99_PE        0x103EC6, BIT6
#define GPIO99_PS        0x103EC6, BIT11

#define GPIO100_PAD      PAD_I2C3_SCL
#define GPIO100_NAME     "PAD_I2C3_SCL"
#define GPIO100_OEN      0x103EC8, BIT2
#define GPIO100_OUT      0x103EC8, BIT1
#define GPIO100_IN       0x103EC8, BIT0
#define GPIO100_DRV      0x103EC8, BIT7
#define GPIO100_PE       0x103EC8, BIT6
#define GPIO100_PS       0x103EC8, BIT11

#define GPIO101_PAD      PAD_PWM_OUT0
#define GPIO101_NAME     "PAD_PWM_OUT0"
#define GPIO101_OEN      0x103ECA, BIT2
#define GPIO101_OUT      0x103ECA, BIT1
#define GPIO101_IN       0x103ECA, BIT0
#define GPIO101_DRV      0x103ECA, BIT7
#define GPIO101_PE       0x103ECA, BIT6
#define GPIO101_PS       0x103ECA, BIT11

#define GPIO102_PAD      PAD_PWM_OUT1
#define GPIO102_NAME     "PAD_PWM_OUT1"
#define GPIO102_OEN      0x103ECC, BIT2
#define GPIO102_OUT      0x103ECC, BIT1
#define GPIO102_IN       0x103ECC, BIT0
#define GPIO102_DRV      0x103ECC, BIT7
#define GPIO102_PE       0x103ECC, BIT6
#define GPIO102_PS       0x103ECC, BIT11

#define GPIO103_PAD      PAD_PWM_OUT2
#define GPIO103_NAME     "PAD_PWM_OUT2"
#define GPIO103_OEN      0x103ECE, BIT2
#define GPIO103_OUT      0x103ECE, BIT1
#define GPIO103_IN       0x103ECE, BIT0
#define GPIO103_DRV      0x103ECE, BIT7
#define GPIO103_PE       0x103ECE, BIT6
#define GPIO103_PS       0x103ECE, BIT11

#define GPIO104_PAD      PAD_PWM_OUT3
#define GPIO104_NAME     "PAD_PWM_OUT3"
#define GPIO104_OEN      0x103ED0, BIT2
#define GPIO104_OUT      0x103ED0, BIT1
#define GPIO104_IN       0x103ED0, BIT0
#define GPIO104_DRV      0x103ED0, BIT7
#define GPIO104_PE       0x103ED0, BIT6
#define GPIO104_PS       0x103ED0, BIT11

#define GPIO105_PAD      PAD_PWM_OUT4
#define GPIO105_NAME     "PAD_PWM_OUT4"
#define GPIO105_OEN      0x103ED2, BIT2
#define GPIO105_OUT      0x103ED2, BIT1
#define GPIO105_IN       0x103ED2, BIT0
#define GPIO105_DRV      0x103ED2, BIT7
#define GPIO105_PE       0x103ED2, BIT6
#define GPIO105_PS       0x103ED2, BIT11

#define GPIO106_PAD      PAD_PWM_OUT5
#define GPIO106_NAME     "PAD_PWM_OUT5"
#define GPIO106_OEN      0x103ED4, BIT2
#define GPIO106_OUT      0x103ED4, BIT1
#define GPIO106_IN       0x103ED4, BIT0
#define GPIO106_DRV      0x103ED4, BIT7
#define GPIO106_PE       0x103ED4, BIT6
#define GPIO106_PS       0x103ED4, BIT11

#define GPIO107_PAD      PAD_PWM_OUT6
#define GPIO107_NAME     "PAD_PWM_OUT6"
#define GPIO107_OEN      0x103ED6, BIT2
#define GPIO107_OUT      0x103ED6, BIT1
#define GPIO107_IN       0x103ED6, BIT0
#define GPIO107_DRV      0x103ED6, BIT7
#define GPIO107_PE       0x103ED6, BIT6
#define GPIO107_PS       0x103ED6, BIT11

#define GPIO108_PAD      PAD_PWM_OUT7
#define GPIO108_NAME     "PAD_PWM_OUT7"
#define GPIO108_OEN      0x103ED8, BIT2
#define GPIO108_OUT      0x103ED8, BIT1
#define GPIO108_IN       0x103ED8, BIT0
#define GPIO108_DRV      0x103ED8, BIT7
#define GPIO108_PE       0x103ED8, BIT6
#define GPIO108_PS       0x103ED8, BIT11

#define GPIO109_PAD      PAD_PWM_OUT8
#define GPIO109_NAME     "PAD_PWM_OUT8"
#define GPIO109_OEN      0x103EDA, BIT2
#define GPIO109_OUT      0x103EDA, BIT1
#define GPIO109_IN       0x103EDA, BIT0
#define GPIO109_DRV      0x103EDA, BIT7
#define GPIO109_PE       0x103EDA, BIT6
#define GPIO109_PS       0x103EDA, BIT11

#define GPIO110_PAD      PAD_PWM_OUT9
#define GPIO110_NAME     "PAD_PWM_OUT9"
#define GPIO110_OEN      0x103EDC, BIT2
#define GPIO110_OUT      0x103EDC, BIT1
#define GPIO110_IN       0x103EDC, BIT0
#define GPIO110_DRV      0x103EDC, BIT7
#define GPIO110_PE       0x103EDC, BIT6
#define GPIO110_PS       0x103EDC, BIT11

#define GPIO111_PAD      PAD_PWM_OUT10
#define GPIO111_NAME     "PAD_PWM_OUT10"
#define GPIO111_OEN      0x103EDE, BIT2
#define GPIO111_OUT      0x103EDE, BIT1
#define GPIO111_IN       0x103EDE, BIT0
#define GPIO111_DRV      0x103EDE, BIT7
#define GPIO111_PE       0x103EDE, BIT6
#define GPIO111_PS       0x103EDE, BIT11

#define GPIO112_PAD      PAD_PWM_OUT11
#define GPIO112_NAME     "PAD_PWM_OUT11"
#define GPIO112_OEN      0x103EE0, BIT2
#define GPIO112_OUT      0x103EE0, BIT1
#define GPIO112_IN       0x103EE0, BIT0
#define GPIO112_DRV      0x103EE0, BIT7
#define GPIO112_PE       0x103EE0, BIT6
#define GPIO112_PS       0x103EE0, BIT11

#define GPIO113_PAD      PAD_OUTP_TX0_CH_0
#define GPIO113_NAME     "PAD_OUTP_TX0_CH_0"
#define GPIO113_OEN      0x103EE2, BIT2
#define GPIO113_OUT      0x103EE2, BIT1
#define GPIO113_IN       0x103EE2, BIT0
#define GPIO113_DRV      0x103EE2, BIT7
#define GPIO113_PE       0x103EE2, BIT6
#define GPIO113_PS       0x103EE2, BIT11

#define GPIO114_PAD      PAD_OUTN_TX0_CH_0
#define GPIO114_NAME     "PAD_OUTN_TX0_CH_0"
#define GPIO114_OEN      0x103EE4, BIT2
#define GPIO114_OUT      0x103EE4, BIT1
#define GPIO114_IN       0x103EE4, BIT0
#define GPIO114_DRV      0x103EE4, BIT7
#define GPIO114_PE       0x103EE4, BIT6
#define GPIO114_PS       0x103EE4, BIT11

#define GPIO115_PAD      PAD_OUTP_TX0_CH_1
#define GPIO115_NAME     "PAD_OUTP_TX0_CH_1"
#define GPIO115_OEN      0x103EE6, BIT2
#define GPIO115_OUT      0x103EE6, BIT1
#define GPIO115_IN       0x103EE6, BIT0
#define GPIO115_DRV      0x103EE6, BIT7
#define GPIO115_PE       0x103EE6, BIT6
#define GPIO115_PS       0x103EE6, BIT11

#define GPIO116_PAD      PAD_OUTN_TX0_CH_1
#define GPIO116_NAME     "PAD_OUTN_TX0_CH_1"
#define GPIO116_OEN      0x103EE8, BIT2
#define GPIO116_OUT      0x103EE8, BIT1
#define GPIO116_IN       0x103EE8, BIT0
#define GPIO116_DRV      0x103EE8, BIT7
#define GPIO116_PE       0x103EE8, BIT6
#define GPIO116_PS       0x103EE8, BIT11

#define GPIO117_PAD      PAD_OUTP_TX0_CH_2
#define GPIO117_NAME     "PAD_OUTP_TX0_CH_2"
#define GPIO117_OEN      0x103EEA, BIT2
#define GPIO117_OUT      0x103EEA, BIT1
#define GPIO117_IN       0x103EEA, BIT0
#define GPIO117_DRV      0x103EEA, BIT7
#define GPIO117_PE       0x103EEA, BIT6
#define GPIO117_PS       0x103EEA, BIT11

#define GPIO118_PAD      PAD_OUTN_TX0_CH_2
#define GPIO118_NAME     "PAD_OUTN_TX0_CH_2"
#define GPIO118_OEN      0x103EEC, BIT2
#define GPIO118_OUT      0x103EEC, BIT1
#define GPIO118_IN       0x103EEC, BIT0
#define GPIO118_DRV      0x103EEC, BIT7
#define GPIO118_PE       0x103EEC, BIT6
#define GPIO118_PS       0x103EEC, BIT11

#define GPIO119_PAD      PAD_OUTP_TX0_CH_3
#define GPIO119_NAME     "PAD_OUTP_TX0_CH_3"
#define GPIO119_OEN      0x103EEE, BIT2
#define GPIO119_OUT      0x103EEE, BIT1
#define GPIO119_IN       0x103EEE, BIT0
#define GPIO119_DRV      0x103EEE, BIT7
#define GPIO119_PE       0x103EEE, BIT6
#define GPIO119_PS       0x103EEE, BIT11

#define GPIO120_PAD      PAD_OUTN_TX0_CH_3
#define GPIO120_NAME     "PAD_OUTN_TX0_CH_3"
#define GPIO120_OEN      0x103EF0, BIT2
#define GPIO120_OUT      0x103EF0, BIT1
#define GPIO120_IN       0x103EF0, BIT0
#define GPIO120_DRV      0x103EF0, BIT7
#define GPIO120_PE       0x103EF0, BIT6
#define GPIO120_PS       0x103EF0, BIT11

#define GPIO121_PAD      PAD_OUTP_TX0_CH_4
#define GPIO121_NAME     "PAD_OUTP_TX0_CH_4"
#define GPIO121_OEN      0x103EF2, BIT2
#define GPIO121_OUT      0x103EF2, BIT1
#define GPIO121_IN       0x103EF2, BIT0
#define GPIO121_DRV      0x103EF2, BIT7
#define GPIO121_PE       0x103EF2, BIT6
#define GPIO121_PS       0x103EF2, BIT11

#define GPIO122_PAD      PAD_OUTN_TX0_CH_4
#define GPIO122_NAME     "PAD_OUTN_TX0_CH_4"
#define GPIO122_OEN      0x103EF4, BIT2
#define GPIO122_OUT      0x103EF4, BIT1
#define GPIO122_IN       0x103EF4, BIT0
#define GPIO122_DRV      0x103EF4, BIT7
#define GPIO122_PE       0x103EF4, BIT6
#define GPIO122_PS       0x103EF4, BIT11

#define GPIO123_PAD      PAD_OUTP_TX1_CH_0
#define GPIO123_NAME     "PAD_OUTP_TX1_CH_0"
#define GPIO123_OEN      0x103EF6, BIT2
#define GPIO123_OUT      0x103EF6, BIT1
#define GPIO123_IN       0x103EF6, BIT0
#define GPIO123_DRV      0x103EF6, BIT7
#define GPIO123_PE       0x103EF6, BIT6
#define GPIO123_PS       0x103EF6, BIT11

#define GPIO124_PAD      PAD_OUTN_TX1_CH_0
#define GPIO124_NAME     "PAD_OUTN_TX1_CH_0"
#define GPIO124_OEN      0x103EF8, BIT2
#define GPIO124_OUT      0x103EF8, BIT1
#define GPIO124_IN       0x103EF8, BIT0
#define GPIO124_DRV      0x103EF8, BIT7
#define GPIO124_PE       0x103EF8, BIT6
#define GPIO124_PS       0x103EF8, BIT11

#define GPIO125_PAD      PAD_OUTP_TX1_CH_1
#define GPIO125_NAME     "PAD_OUTP_TX1_CH_1"
#define GPIO125_OEN      0x103EFA, BIT2
#define GPIO125_OUT      0x103EFA, BIT1
#define GPIO125_IN       0x103EFA, BIT0
#define GPIO125_DRV      0x103EFA, BIT7
#define GPIO125_PE       0x103EFA, BIT6
#define GPIO125_PS       0x103EFA, BIT11

#define GPIO126_PAD      PAD_OUTN_TX1_CH_1
#define GPIO126_NAME     "PAD_OUTN_TX1_CH_1"
#define GPIO126_OEN      0x103EFC, BIT2
#define GPIO126_OUT      0x103EFC, BIT1
#define GPIO126_IN       0x103EFC, BIT0
#define GPIO126_DRV      0x103EFC, BIT7
#define GPIO126_PE       0x103EFC, BIT6
#define GPIO126_PS       0x103EFC, BIT11

#define GPIO127_PAD      PAD_OUTP_TX1_CH_2
#define GPIO127_NAME     "PAD_OUTP_TX1_CH_2"
#define GPIO127_OEN      0x103EFE, BIT2
#define GPIO127_OUT      0x103EFE, BIT1
#define GPIO127_IN       0x103EFE, BIT0
#define GPIO127_DRV      0x103EFE, BIT7
#define GPIO127_PE       0x103EFE, BIT6
#define GPIO127_PS       0x103EFE, BIT11

#define GPIO128_PAD      PAD_OUTN_TX1_CH_2
#define GPIO128_NAME     "PAD_OUTN_TX1_CH_2"
#define GPIO128_OEN      0x110400, BIT2
#define GPIO128_OUT      0x110400, BIT1
#define GPIO128_IN       0x110400, BIT0
#define GPIO128_DRV      0x110400, BIT7
#define GPIO128_PE       0x110400, BIT6
#define GPIO128_PS       0x110400, BIT11

#define GPIO129_PAD      PAD_OUTP_TX1_CH_3
#define GPIO129_NAME     "PAD_OUTP_TX1_CH_3"
#define GPIO129_OEN      0x110402, BIT2
#define GPIO129_OUT      0x110402, BIT1
#define GPIO129_IN       0x110402, BIT0
#define GPIO129_DRV      0x110402, BIT7
#define GPIO129_PE       0x110402, BIT6
#define GPIO129_PS       0x110402, BIT11

#define GPIO130_PAD      PAD_OUTN_TX1_CH_3
#define GPIO130_NAME     "PAD_OUTN_TX1_CH_3"
#define GPIO130_OEN      0x110404, BIT2
#define GPIO130_OUT      0x110404, BIT1
#define GPIO130_IN       0x110404, BIT0
#define GPIO130_DRV      0x110404, BIT7
#define GPIO130_PE       0x110404, BIT6
#define GPIO130_PS       0x110404, BIT11

#define GPIO131_PAD      PAD_OUTP_TX1_CH_4
#define GPIO131_NAME     "PAD_OUTP_TX1_CH_4"
#define GPIO131_OEN      0x110406, BIT2
#define GPIO131_OUT      0x110406, BIT1
#define GPIO131_IN       0x110406, BIT0
#define GPIO131_DRV      0x110406, BIT7
#define GPIO131_PE       0x110406, BIT6
#define GPIO131_PS       0x110406, BIT11

#define GPIO132_PAD      PAD_OUTN_TX1_CH_4
#define GPIO132_NAME     "PAD_OUTN_TX1_CH_4"
#define GPIO132_OEN      0x110408, BIT2
#define GPIO132_OUT      0x110408, BIT1
#define GPIO132_IN       0x110408, BIT0
#define GPIO132_DRV      0x110408, BIT7
#define GPIO132_PE       0x110408, BIT6
#define GPIO132_PS       0x110408, BIT11

#define GPIO133_PAD      PAD_SAR_ADC_0
#define GPIO133_NAME     "PAD_SAR_ADC_0"
#define GPIO133_OEN      0x11040A, BIT2
#define GPIO133_OUT      0x11040A, BIT1
#define GPIO133_IN       0x11040A, BIT0
#define GPIO133_DRV      0x11040A, BIT7|BIT8
#define GPIO133_PE       0x11040A, BIT6
#define GPIO133_PS       0x11040A, BIT11

#define GPIO134_PAD      PAD_SAR_ADC_1
#define GPIO134_NAME     "PAD_SAR_ADC_1"
#define GPIO134_OEN      0x11040C, BIT2
#define GPIO134_OUT      0x11040C, BIT1
#define GPIO134_IN       0x11040C, BIT0
#define GPIO134_DRV      0x11040C, BIT7|BIT8
#define GPIO134_PE       0x11040C, BIT6
#define GPIO134_PS       0x11040C, BIT11

#define GPIO135_PAD      PAD_SAR_ADC_2
#define GPIO135_NAME     "PAD_SAR_ADC_2"
#define GPIO135_OEN      0x11040E, BIT2
#define GPIO135_OUT      0x11040E, BIT1
#define GPIO135_IN       0x11040E, BIT0
#define GPIO135_DRV      0x11040E, BIT7|BIT8
#define GPIO135_PE       0x11040E, BIT6
#define GPIO135_PS       0x11040E, BIT11

#define GPIO136_PAD      PAD_SAR_ADC_3
#define GPIO136_NAME     "PAD_SAR_ADC_3"
#define GPIO136_OEN      0x110410, BIT2
#define GPIO136_OUT      0x110410, BIT1
#define GPIO136_IN       0x110410, BIT0
#define GPIO136_DRV      0x110410, BIT7|BIT8
#define GPIO136_PE       0x110410, BIT6
#define GPIO136_PS       0x110410, BIT11

#define GPIO137_PAD      PAD_SAR_ADC_4
#define GPIO137_NAME     "PAD_SAR_ADC_4"
#define GPIO137_OEN      0x110412, BIT2
#define GPIO137_OUT      0x110412, BIT1
#define GPIO137_IN       0x110412, BIT0
#define GPIO137_DRV      0x110412, BIT7|BIT8
#define GPIO137_PE       0x110412, BIT6
#define GPIO137_PS       0x110412, BIT11

#define GPIO138_PAD      PAD_SAR_ADC_5
#define GPIO138_NAME     "PAD_SAR_ADC_5"
#define GPIO138_OEN      0x110414, BIT2
#define GPIO138_OUT      0x110414, BIT1
#define GPIO138_IN       0x110414, BIT0
#define GPIO138_DRV      0x110414, BIT7|BIT8
#define GPIO138_PE       0x110414, BIT6
#define GPIO138_PS       0x110414, BIT11

#define GPIO139_PAD      PAD_SAR_ADC_6
#define GPIO139_NAME     "PAD_SAR_ADC_6"
#define GPIO139_OEN      0x110416, BIT2
#define GPIO139_OUT      0x110416, BIT1
#define GPIO139_IN       0x110416, BIT0
#define GPIO139_DRV      0x110416, BIT7|BIT8
#define GPIO139_PE       0x110416, BIT6
#define GPIO139_PS       0x110416, BIT11

#define GPIO140_PAD      PAD_SAR_ADC_7
#define GPIO140_NAME     "PAD_SAR_ADC_7"
#define GPIO140_OEN      0x110418, BIT2
#define GPIO140_OUT      0x110418, BIT1
#define GPIO140_IN       0x110418, BIT0
#define GPIO140_DRV      0x110418, BIT7|BIT8
#define GPIO140_PE       0x110418, BIT6
#define GPIO140_PS       0x110418, BIT11

#define GPIO141_PAD      PAD_SAR_ADC_8
#define GPIO141_NAME     "PAD_SAR_ADC_8"
#define GPIO141_OEN      0x11041A, BIT2
#define GPIO141_OUT      0x11041A, BIT1
#define GPIO141_IN       0x11041A, BIT0
#define GPIO141_DRV      0x11041A, BIT7|BIT8
#define GPIO141_PE       0x11041A, BIT6
#define GPIO141_PS       0x11041A, BIT11

#define GPIO142_PAD      PAD_SAR_ADC_9
#define GPIO142_NAME     "PAD_SAR_ADC_9"
#define GPIO142_OEN      0x11041C, BIT2
#define GPIO142_OUT      0x11041C, BIT1
#define GPIO142_IN       0x11041C, BIT0
#define GPIO142_DRV      0x11041C, BIT7|BIT8
#define GPIO142_PE       0x11041C, BIT6
#define GPIO142_PS       0x11041C, BIT11

#define GPIO143_PAD      PAD_SAR_ADC_10
#define GPIO143_NAME     "PAD_SAR_ADC_10"
#define GPIO143_OEN      0x11041E, BIT2
#define GPIO143_OUT      0x11041E, BIT1
#define GPIO143_IN       0x11041E, BIT0
#define GPIO143_DRV      0x11041E, BIT7|BIT8
#define GPIO143_PE       0x11041E, BIT6
#define GPIO143_PS       0x11041E, BIT11

#define GPIO144_PAD      PAD_SAR_ADC_11
#define GPIO144_NAME     "PAD_SAR_ADC_11"
#define GPIO144_OEN      0x110420, BIT2
#define GPIO144_OUT      0x110420, BIT1
#define GPIO144_IN       0x110420, BIT0
#define GPIO144_DRV      0x110420, BIT7|BIT8
#define GPIO144_PE       0x110420, BIT6
#define GPIO144_PS       0x110420, BIT11

#define GPIO145_PAD      PAD_SAR_ADC_12
#define GPIO145_NAME     "PAD_SAR_ADC_12"
#define GPIO145_OEN      0x110422, BIT2
#define GPIO145_OUT      0x110422, BIT1
#define GPIO145_IN       0x110422, BIT0
#define GPIO145_DRV      0x110422, BIT7|BIT8
#define GPIO145_PE       0x110422, BIT6
#define GPIO145_PS       0x110422, BIT11

#define GPIO146_PAD      PAD_SAR_ADC_13
#define GPIO146_NAME     "PAD_SAR_ADC_13"
#define GPIO146_OEN      0x110424, BIT2
#define GPIO146_OUT      0x110424, BIT1
#define GPIO146_IN       0x110424, BIT0
#define GPIO146_DRV      0x110424, BIT7|BIT8
#define GPIO146_PE       0x110424, BIT6
#define GPIO146_PS       0x110424, BIT11

#define GPIO147_PAD      PAD_SAR_ADC_14
#define GPIO147_NAME     "PAD_SAR_ADC_14"
#define GPIO147_OEN      0x110426, BIT2
#define GPIO147_OUT      0x110426, BIT1
#define GPIO147_IN       0x110426, BIT0
#define GPIO147_DRV      0x110426, BIT7|BIT8
#define GPIO147_PE       0x110426, BIT6
#define GPIO147_PS       0x110426, BIT11

#define GPIO148_PAD      PAD_SAR_ADC_15
#define GPIO148_NAME     "PAD_SAR_ADC_15"
#define GPIO148_OEN      0x110428, BIT2
#define GPIO148_OUT      0x110428, BIT1
#define GPIO148_IN       0x110428, BIT0
#define GPIO148_DRV      0x110428, BIT7|BIT8
#define GPIO148_PE       0x110428, BIT6
#define GPIO148_PS       0x110428, BIT11

#define GPIO149_PAD      PAD_SAR_ADC_16
#define GPIO149_NAME     "PAD_SAR_ADC_16"
#define GPIO149_OEN      0x11042A, BIT2
#define GPIO149_OUT      0x11042A, BIT1
#define GPIO149_IN       0x11042A, BIT0
#define GPIO149_DRV      0x11042A, BIT7|BIT8
#define GPIO149_PE       0x11042A, BIT6
#define GPIO149_PS       0x11042A, BIT11

#define GPIO150_PAD      PAD_SAR_ADC_17
#define GPIO150_NAME     "PAD_SAR_ADC_17"
#define GPIO150_OEN      0x11042C, BIT2
#define GPIO150_OUT      0x11042C, BIT1
#define GPIO150_IN       0x11042C, BIT0
#define GPIO150_DRV      0x11042C, BIT7|BIT8
#define GPIO150_PE       0x11042C, BIT6
#define GPIO150_PS       0x11042C, BIT11

#define GPIO151_PAD      PAD_SAR_ADC_18
#define GPIO151_NAME     "PAD_SAR_ADC_18"
#define GPIO151_OEN      0x11042E, BIT2
#define GPIO151_OUT      0x11042E, BIT1
#define GPIO151_IN       0x11042E, BIT0
#define GPIO151_DRV      0x11042E, BIT7|BIT8
#define GPIO151_PE       0x11042E, BIT6
#define GPIO151_PS       0x11042E, BIT11

#define GPIO152_PAD      PAD_SAR_ADC_19
#define GPIO152_NAME     "PAD_SAR_ADC_19"
#define GPIO152_OEN      0x110430, BIT2
#define GPIO152_OUT      0x110430, BIT1
#define GPIO152_IN       0x110430, BIT0
#define GPIO152_DRV      0x110430, BIT7|BIT8
#define GPIO152_PE       0x110430, BIT6
#define GPIO152_PS       0x110430, BIT11

#define GPIO153_PAD      PAD_SAR_ADC_20
#define GPIO153_NAME     "PAD_SAR_ADC_20"
#define GPIO153_OEN      0x110432, BIT2
#define GPIO153_OUT      0x110432, BIT1
#define GPIO153_IN       0x110432, BIT0
#define GPIO153_DRV      0x110432, BIT7|BIT8
#define GPIO153_PE       0x110432, BIT6
#define GPIO153_PS       0x110432, BIT11

#define GPIO154_PAD      PAD_SAR_ADC_21
#define GPIO154_NAME     "PAD_SAR_ADC_21"
#define GPIO154_OEN      0x110434, BIT2
#define GPIO154_OUT      0x110434, BIT1
#define GPIO154_IN       0x110434, BIT0
#define GPIO154_DRV      0x110434, BIT7|BIT8
#define GPIO154_PE       0x110434, BIT6
#define GPIO154_PS       0x110434, BIT11

#define GPIO155_PAD      PAD_SAR_ADC_22
#define GPIO155_NAME     "PAD_SAR_ADC_22"
#define GPIO155_OEN      0x110436, BIT2
#define GPIO155_OUT      0x110436, BIT1
#define GPIO155_IN       0x110436, BIT0
#define GPIO155_DRV      0x110436, BIT7|BIT8
#define GPIO155_PE       0x110436, BIT6
#define GPIO155_PS       0x110436, BIT11

#define GPIO156_PAD      PAD_SAR_ADC_23
#define GPIO156_NAME     "PAD_SAR_ADC_23"
#define GPIO156_OEN      0x110438, BIT2
#define GPIO156_OUT      0x110438, BIT1
#define GPIO156_IN       0x110438, BIT0
#define GPIO156_DRV      0x110438, BIT7|BIT8
#define GPIO156_PE       0x110438, BIT6
#define GPIO156_PS       0x110438, BIT11

#define GPIO157_PAD      PAD_SR_IO4
#define GPIO157_NAME     "PAD_SR_IO4"
#define GPIO157_OEN      0x11043A, BIT2
#define GPIO157_OUT      0x11043A, BIT1
#define GPIO157_IN       0x11043A, BIT0
#define GPIO157_DRV      0x11043A, BIT7
#define GPIO157_PE       0x11043A, BIT6
#define GPIO157_PS       0x11043A, BIT11

#define GPIO158_PAD      PAD_RGMII0_MCLK
#define GPIO158_NAME     "PAD_RGMII0_MCLK"
#define GPIO158_OEN      0x11044A, BIT2
#define GPIO158_OUT      0x11044A, BIT1
#define GPIO158_IN       0x11044A, BIT0
#define GPIO158_DRV      0x11044A, BIT7|BIT8
#define GPIO158_PE       0x11044A, BIT6
#define GPIO158_PS       0x11044A, BIT11

#define GPIO159_PAD      PAD_RGMII0_RSTN
#define GPIO159_NAME     "PAD_RGMII0_RSTN"
#define GPIO159_OEN      0x11044C, BIT2
#define GPIO159_OUT      0x11044C, BIT1
#define GPIO159_IN       0x11044C, BIT0
#define GPIO159_DRV      0x11044C, BIT7|BIT8
#define GPIO159_PE       0x11044C, BIT6
#define GPIO159_PS       0x11044C, BIT11

#define GPIO160_PAD      PAD_RGMII0_RXCLK
#define GPIO160_NAME     "PAD_RGMII0_RXCLK"
#define GPIO160_OEN      0x11044E, BIT2
#define GPIO160_OUT      0x11044E, BIT1
#define GPIO160_IN       0x11044E, BIT0
#define GPIO160_DRV      0x11044E, BIT7|BIT8
#define GPIO160_PE       0x11044E, BIT6
#define GPIO160_PS       0x11044E, BIT11

#define GPIO161_PAD      PAD_RGMII0_RXCTL
#define GPIO161_NAME     "PAD_RGMII0_RXCTL"
#define GPIO161_OEN      0x110450, BIT2
#define GPIO161_OUT      0x110450, BIT1
#define GPIO161_IN       0x110450, BIT0
#define GPIO161_DRV      0x110450, BIT7|BIT8
#define GPIO161_PE       0x110450, BIT6
#define GPIO161_PS       0x110450, BIT11

#define GPIO162_PAD      PAD_RGMII0_RXD0
#define GPIO162_NAME     "PAD_RGMII0_RXD0"
#define GPIO162_OEN      0x110452, BIT2
#define GPIO162_OUT      0x110452, BIT1
#define GPIO162_IN       0x110452, BIT0
#define GPIO162_DRV      0x110452, BIT7|BIT8
#define GPIO162_PE       0x110452, BIT6
#define GPIO162_PS       0x110452, BIT11

#define GPIO163_PAD      PAD_RGMII0_RXD1
#define GPIO163_NAME     "PAD_RGMII0_RXD1"
#define GPIO163_OEN      0x110454, BIT2
#define GPIO163_OUT      0x110454, BIT1
#define GPIO163_IN       0x110454, BIT0
#define GPIO163_DRV      0x110454, BIT7|BIT8
#define GPIO163_PE       0x110454, BIT6
#define GPIO163_PS       0x110454, BIT11

#define GPIO164_PAD      PAD_RGMII0_RXD2
#define GPIO164_NAME     "PAD_RGMII0_RXD2"
#define GPIO164_OEN      0x110456, BIT2
#define GPIO164_OUT      0x110456, BIT1
#define GPIO164_IN       0x110456, BIT0
#define GPIO164_DRV      0x110456, BIT7|BIT8
#define GPIO164_PE       0x110456, BIT6
#define GPIO164_PS       0x110456, BIT11

#define GPIO165_PAD      PAD_RGMII0_RXD3
#define GPIO165_NAME     "PAD_RGMII0_RXD3"
#define GPIO165_OEN      0x110458, BIT2
#define GPIO165_OUT      0x110458, BIT1
#define GPIO165_IN       0x110458, BIT0
#define GPIO165_DRV      0x110458, BIT7|BIT8
#define GPIO165_PE       0x110458, BIT6
#define GPIO165_PS       0x110458, BIT11

#define GPIO166_PAD      PAD_RGMII0_TXCLK
#define GPIO166_NAME     "PAD_RGMII0_TXCLK"
#define GPIO166_OEN      0x11045A, BIT2
#define GPIO166_OUT      0x11045A, BIT1
#define GPIO166_IN       0x11045A, BIT0
#define GPIO166_DRV      0x11045A, BIT7|BIT8
#define GPIO166_PE       0x11045A, BIT6
#define GPIO166_PS       0x11045A, BIT11

#define GPIO167_PAD      PAD_RGMII0_TXCTL
#define GPIO167_NAME     "PAD_RGMII0_TXCTL"
#define GPIO167_OEN      0x11045C, BIT2
#define GPIO167_OUT      0x11045C, BIT1
#define GPIO167_IN       0x11045C, BIT0
#define GPIO167_DRV      0x11045C, BIT7|BIT8
#define GPIO167_PE       0x11045C, BIT6
#define GPIO167_PS       0x11045C, BIT11

#define GPIO168_PAD      PAD_RGMII0_TXD0
#define GPIO168_NAME     "PAD_RGMII0_TXD0"
#define GPIO168_OEN      0x11045E, BIT2
#define GPIO168_OUT      0x11045E, BIT1
#define GPIO168_IN       0x11045E, BIT0
#define GPIO168_DRV      0x11045E, BIT7|BIT8
#define GPIO168_PE       0x11045E, BIT6
#define GPIO168_PS       0x11045E, BIT11

#define GPIO169_PAD      PAD_RGMII0_TXD1
#define GPIO169_NAME     "PAD_RGMII0_TXD1"
#define GPIO169_OEN      0x110460, BIT2
#define GPIO169_OUT      0x110460, BIT1
#define GPIO169_IN       0x110460, BIT0
#define GPIO169_DRV      0x110460, BIT7|BIT8
#define GPIO169_PE       0x110460, BIT6
#define GPIO169_PS       0x110460, BIT11

#define GPIO170_PAD      PAD_RGMII0_TXD2
#define GPIO170_NAME     "PAD_RGMII0_TXD2"
#define GPIO170_OEN      0x110462, BIT2
#define GPIO170_OUT      0x110462, BIT1
#define GPIO170_IN       0x110462, BIT0
#define GPIO170_DRV      0x110462, BIT7|BIT8
#define GPIO170_PE       0x110462, BIT6
#define GPIO170_PS       0x110462, BIT11

#define GPIO171_PAD      PAD_RGMII0_TXD3
#define GPIO171_NAME     "PAD_RGMII0_TXD3"
#define GPIO171_OEN      0x110464, BIT2
#define GPIO171_OUT      0x110464, BIT1
#define GPIO171_IN       0x110464, BIT0
#define GPIO171_DRV      0x110464, BIT7|BIT8
#define GPIO171_PE       0x110464, BIT6
#define GPIO171_PS       0x110464, BIT11

#define GPIO172_PAD      PAD_RGMII0_MDIO
#define GPIO172_NAME     "PAD_RGMII0_MDIO"
#define GPIO172_OEN      0x110466, BIT2
#define GPIO172_OUT      0x110466, BIT1
#define GPIO172_IN       0x110466, BIT0
#define GPIO172_DRV      0x110466, BIT7|BIT8
#define GPIO172_PE       0x110466, BIT6
#define GPIO172_PS       0x110466, BIT11

#define GPIO173_PAD      PAD_RGMII0_MDC
#define GPIO173_NAME     "PAD_RGMII0_MDC"
#define GPIO173_OEN      0x110468, BIT2
#define GPIO173_OUT      0x110468, BIT1
#define GPIO173_IN       0x110468, BIT0
#define GPIO173_DRV      0x110468, BIT7|BIT8
#define GPIO173_PE       0x110468, BIT6
#define GPIO173_PS       0x110468, BIT11

#define GPIO174_PAD      PAD_UART_RX2
#define GPIO174_NAME     "PAD_UART_RX2"
#define GPIO174_OEN      0x11046A, BIT2
#define GPIO174_OUT      0x11046A, BIT1
#define GPIO174_IN       0x11046A, BIT0
#define GPIO174_DRV      0x11046A, BIT7
#define GPIO174_PE       0x11046A, BIT6
#define GPIO174_PS       0x11046A, BIT11

#define GPIO175_PAD      PAD_UART_TX2
#define GPIO175_NAME     "PAD_UART_TX2"
#define GPIO175_OEN      0x11046C, BIT2
#define GPIO175_OUT      0x11046C, BIT1
#define GPIO175_IN       0x11046C, BIT0
#define GPIO175_DRV      0x11046C, BIT7
#define GPIO175_PE       0x11046C, BIT6
#define GPIO175_PS       0x11046C, BIT11

#define GPIO176_PAD      PAD_UART_RX3
#define GPIO176_NAME     "PAD_UART_RX3"
#define GPIO176_OEN      0x11046E, BIT2
#define GPIO176_OUT      0x11046E, BIT1
#define GPIO176_IN       0x11046E, BIT0
#define GPIO176_DRV      0x11046E, BIT7
#define GPIO176_PE       0x11046E, BIT6
#define GPIO176_PS       0x11046E, BIT11

#define GPIO177_PAD      PAD_UART_TX3
#define GPIO177_NAME     "PAD_UART_TX3"
#define GPIO177_OEN      0x110470, BIT2
#define GPIO177_OUT      0x110470, BIT1
#define GPIO177_IN       0x110470, BIT0
#define GPIO177_DRV      0x110470, BIT7
#define GPIO177_PE       0x110470, BIT6
#define GPIO177_PS       0x110470, BIT11

#define GPIO178_PAD      PAD_UART_RX4
#define GPIO178_NAME     "PAD_UART_RX4"
#define GPIO178_OEN      0x110472, BIT2
#define GPIO178_OUT      0x110472, BIT1
#define GPIO178_IN       0x110472, BIT0
#define GPIO178_DRV      0x110472, BIT7
#define GPIO178_PE       0x110472, BIT6
#define GPIO178_PS       0x110472, BIT11

#define GPIO179_PAD      PAD_UART_TX4
#define GPIO179_NAME     "PAD_UART_TX4"
#define GPIO179_OEN      0x110474, BIT2
#define GPIO179_OUT      0x110474, BIT1
#define GPIO179_IN       0x110474, BIT0
#define GPIO179_DRV      0x110474, BIT7
#define GPIO179_PE       0x110474, BIT6
#define GPIO179_PS       0x110474, BIT11

#define GPIO180_PAD      PAD_UART_RX1
#define GPIO180_NAME     "PAD_UART_RX1"
#define GPIO180_OEN      0x110476, BIT2
#define GPIO180_OUT      0x110476, BIT1
#define GPIO180_IN       0x110476, BIT0
#define GPIO180_DRV      0x110476, BIT7
#define GPIO180_PE       0x110476, BIT6
#define GPIO180_PS       0x110476, BIT11

#define GPIO181_PAD      PAD_UART_TX1
#define GPIO181_NAME     "PAD_UART_TX1"
#define GPIO181_OEN      0x110478, BIT2
#define GPIO181_OUT      0x110478, BIT1
#define GPIO181_IN       0x110478, BIT0
#define GPIO181_DRV      0x110478, BIT7
#define GPIO181_PE       0x110478, BIT6
#define GPIO181_PS       0x110478, BIT11

#define GPIO182_PAD      PAD_FUART_RX
#define GPIO182_NAME     "PAD_FUART_RX"
#define GPIO182_OEN      0x11047A, BIT2
#define GPIO182_OUT      0x11047A, BIT1
#define GPIO182_IN       0x11047A, BIT0
#define GPIO182_DRV      0x11047A, BIT7
#define GPIO182_PE       0x11047A, BIT6
#define GPIO182_PS       0x11047A, BIT11

#define GPIO183_PAD      PAD_FUART_TX
#define GPIO183_NAME     "PAD_FUART_TX"
#define GPIO183_OEN      0x11047C, BIT2
#define GPIO183_OUT      0x11047C, BIT1
#define GPIO183_IN       0x11047C, BIT0
#define GPIO183_DRV      0x11047C, BIT7
#define GPIO183_PE       0x11047C, BIT6
#define GPIO183_PS       0x11047C, BIT11

#define GPIO184_PAD      PAD_FUART_RTS
#define GPIO184_NAME     "PAD_FUART_RTS"
#define GPIO184_OEN      0x11047E, BIT2
#define GPIO184_OUT      0x11047E, BIT1
#define GPIO184_IN       0x11047E, BIT0
#define GPIO184_DRV      0x11047E, BIT7
#define GPIO184_PE       0x11047E, BIT6
#define GPIO184_PS       0x11047E, BIT11

#define GPIO185_PAD      PAD_FUART_CTS
#define GPIO185_NAME     "PAD_FUART_CTS"
#define GPIO185_OEN      0x110480, BIT2
#define GPIO185_OUT      0x110480, BIT1
#define GPIO185_IN       0x110480, BIT0
#define GPIO185_DRV      0x110480, BIT7
#define GPIO185_PE       0x110480, BIT6
#define GPIO185_PS       0x110480, BIT11

U32 gChipBaseAddr    = GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x101E00);
U32 gPmSleepBaseAddr = GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x000E00);
U32 gSarBaseAddr     = GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x001400);
U32 gRIUBaseAddr     = MS_BASE_REG_RIU_PA;

extern struct vint_handler_t ss_pm_vintc;
extern struct vint_handler_t ss_gpi_vintc;

#define MHal_CHIPTOP_REG(addr)  (*(volatile U16*)(gChipBaseAddr  + (addr << 1)))
#define MHal_PM_SLEEP_REG(addr) (*(volatile U16*)(gPmSleepBaseAddr + (addr << 1)))
#define MHal_SAR_GPIO_REG(addr) (*(volatile U16*)(gSarBaseAddr + (addr << 1)))
#define MHal_RIU_REG(addr)      (*(volatile U16*)(gRIUBaseAddr +  (addr << 1)))

#define REG_ALL_PAD_IN     0x24

#define MHAL_GPIO_SUPPORT_INTR  0

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

static int _pmsleep_to_irq_table[] = {
    INT_PMSLEEP_PM_PWM0,     INT_PMSLEEP_PM_PWM1,     INT_PMSLEEP_PM_I2CM_SCL, INT_PMSLEEP_PM_I2CM_SDA,
    INT_PMSLEEP_PM_UART_RX0, INT_PMSLEEP_PM_UART_TX0, INT_PMSLEEP_PM_IR_RX,    INT_PMSLEEP_PM_GPIO0,
    INT_PMSLEEP_PM_GPIO1,    INT_PMSLEEP_PM_GPIO2,    INT_PMSLEEP_PM_GPIO3,    INT_PMSLEEP_PM_GPIO4,
    INT_PMSLEEP_PM_GPIO5,    INT_PMSLEEP_PM_SPI_WPZ,  INT_PMSLEEP_PM_SPI_DO,   INT_PMSLEEP_PM_SPI_CZ,
    INT_PMSLEEP_PM_SPI_HLD,  INT_PMSLEEP_PM_SPI_CK,   INT_PMSLEEP_PM_SPI_DI,
};

static int _gpi_to_irq_table[] = {
    INT_GPI_FIQ_PAD_I2C5_SCL,      INT_GPI_FIQ_PAD_I2C5_SDA,      INT_GPI_FIQ_PAD_SD0_GPIO0,
    INT_GPI_FIQ_PAD_SD0_VCTRL,     INT_GPI_FIQ_PAD_SD0_CDZ,       INT_GPI_FIQ_PAD_SD0_D1,
    INT_GPI_FIQ_PAD_SD0_D0,        INT_GPI_FIQ_PAD_SD0_CLK,       INT_GPI_FIQ_PAD_SD0_CMD,
    INT_GPI_FIQ_PAD_SD0_D3,        INT_GPI_FIQ_PAD_SD0_D2,        INT_GPI_FIQ_PAD_KEY0,
    INT_GPI_FIQ_PAD_KEY1,          INT_GPI_FIQ_PAD_KEY2,          INT_GPI_FIQ_PAD_KEY3,
    INT_GPI_FIQ_PAD_KEY4,          INT_GPI_FIQ_PAD_KEY5,          INT_GPI_FIQ_PAD_KEY6,
    INT_GPI_FIQ_PAD_KEY7,          INT_GPI_FIQ_PAD_KEY8,          INT_GPI_FIQ_PAD_KEY9,
    INT_GPI_FIQ_PAD_KEY10,         INT_GPI_FIQ_PAD_KEY11,         INT_GPI_FIQ_PAD_KEY12,
    INT_GPI_FIQ_PAD_KEY13,         INT_GPI_FIQ_PAD_PM_PWM0,       INT_GPI_FIQ_PAD_PM_PWM1,
    INT_GPI_FIQ_PAD_PM_I2CM_SCL,   INT_GPI_FIQ_PAD_PM_I2CM_SDA,   INT_GPI_FIQ_PAD_PM_UART_RX0,
    INT_GPI_FIQ_PAD_PM_UART_TX0,   INT_GPI_FIQ_PAD_PM_IR_RX,      INT_GPI_FIQ_PAD_PM_GPIO0,
    INT_GPI_FIQ_PAD_PM_GPIO1,      INT_GPI_FIQ_PAD_PM_GPIO2,      INT_GPI_FIQ_PAD_PM_GPIO3,
    INT_GPI_FIQ_PAD_PM_GPIO4,      INT_GPI_FIQ_PAD_PM_GPIO5,      INT_GPI_FIQ_PAD_PM_SPI_WPZ,
    INT_GPI_FIQ_PAD_PM_SPI_DO,     INT_GPI_FIQ_PAD_PM_SPI_CZ,     INT_GPI_FIQ_PAD_PM_SPI_HLD,
    INT_GPI_FIQ_PAD_PM_SPI_CK,     INT_GPI_FIQ_PAD_PM_SPI_DI,     INT_GPI_FIQ_PAD_EMMC_RSTN,
    INT_GPI_FIQ_PAD_EMMC_CLK,      INT_GPI_FIQ_PAD_EMMC_CMD,      INT_GPI_FIQ_PAD_EMMC_DS,
    INT_GPI_FIQ_PAD_EMMC_D3,       INT_GPI_FIQ_PAD_EMMC_D4,       INT_GPI_FIQ_PAD_EMMC_D0,
    INT_GPI_FIQ_PAD_EMMC_D5,       INT_GPI_FIQ_PAD_EMMC_D1,       INT_GPI_FIQ_PAD_EMMC_D6,
    INT_GPI_FIQ_PAD_EMMC_D2,       INT_GPI_FIQ_PAD_EMMC_D7,       INT_GPI_FIQ_PAD_OUTP_RX1_CH_0,
    INT_GPI_FIQ_PAD_OUTN_RX1_CH_0, INT_GPI_FIQ_PAD_OUTP_RX1_CH_1, INT_GPI_FIQ_PAD_OUTN_RX1_CH_1,
    INT_GPI_FIQ_PAD_OUTP_RX1_CH_2, INT_GPI_FIQ_PAD_OUTN_RX1_CH_2, INT_GPI_FIQ_PAD_OUTP_RX1_CH_3,
    INT_GPI_FIQ_PAD_OUTN_RX1_CH_3, INT_GPI_FIQ_PAD_OUTP_RX0_CH_0, INT_GPI_FIQ_PAD_OUTN_RX0_CH_0,
    INT_GPI_FIQ_PAD_OUTP_RX0_CH_1, INT_GPI_FIQ_PAD_OUTN_RX0_CH_1, INT_GPI_FIQ_PAD_OUTP_RX0_CH_2,
    INT_GPI_FIQ_PAD_OUTN_RX0_CH_2, INT_GPI_FIQ_PAD_OUTP_RX0_CH_3, INT_GPI_FIQ_PAD_OUTN_RX0_CH_3,
    INT_GPI_FIQ_PAD_OUTP_RX0_CH_4, INT_GPI_FIQ_PAD_OUTN_RX0_CH_4, INT_GPI_FIQ_PAD_OUTP_RX0_CH_5,
    INT_GPI_FIQ_PAD_OUTN_RX0_CH_5, INT_GPI_FIQ_PAD_SPDIF_TX,      INT_GPI_FIQ_PAD_SR_IO3,
    INT_GPI_FIQ_PAD_SR_IO2,        INT_GPI_FIQ_PAD_SR_IO1,        INT_GPI_FIQ_PAD_SR_IO0,
    INT_GPI_FIQ_PAD_SR_PDN1,       INT_GPI_FIQ_PAD_SR_MCLK1,      INT_GPI_FIQ_PAD_SR_RST1,
    INT_GPI_FIQ_PAD_I2C1_SDA,      INT_GPI_FIQ_PAD_I2C1_SCL,      INT_GPI_FIQ_PAD_SR_PDN0,
    INT_GPI_FIQ_PAD_SR_MCLK0,      INT_GPI_FIQ_PAD_SR_RST0,       INT_GPI_FIQ_PAD_I2C0_SDA,
    INT_GPI_FIQ_PAD_I2C0_SCL,      INT_GPI_FIQ_PAD_SR_PDN2,       INT_GPI_FIQ_PAD_SR_MCLK2,
    INT_GPI_FIQ_PAD_SR_RST2,       INT_GPI_FIQ_PAD_I2C2_SDA,      INT_GPI_FIQ_PAD_I2C2_SCL,
    INT_GPI_FIQ_PAD_SR_PDN3,       INT_GPI_FIQ_PAD_SR_MCLK3,      INT_GPI_FIQ_PAD_SR_RST3,
    INT_GPI_FIQ_PAD_I2C3_SDA,      INT_GPI_FIQ_PAD_I2C3_SCL,      INT_GPI_FIQ_PAD_PWM_OUT0,
    INT_GPI_FIQ_PAD_PWM_OUT1,      INT_GPI_FIQ_PAD_PWM_OUT2,      INT_GPI_FIQ_PAD_PWM_OUT3,
    INT_GPI_FIQ_PAD_PWM_OUT4,      INT_GPI_FIQ_PAD_PWM_OUT5,      INT_GPI_FIQ_PAD_PWM_OUT6,
    INT_GPI_FIQ_PAD_PWM_OUT7,      INT_GPI_FIQ_PAD_PWM_OUT8,      INT_GPI_FIQ_PAD_PWM_OUT9,
    INT_GPI_FIQ_PAD_PWM_OUT10,     INT_GPI_FIQ_PAD_PWM_OUT11,     INT_GPI_FIQ_PAD_OUTP_TX0_CH_0,
    INT_GPI_FIQ_PAD_OUTN_TX0_CH_0, INT_GPI_FIQ_PAD_OUTP_TX0_CH_1, INT_GPI_FIQ_PAD_OUTN_TX0_CH_1,
    INT_GPI_FIQ_PAD_OUTP_TX0_CH_2, INT_GPI_FIQ_PAD_OUTN_TX0_CH_2, INT_GPI_FIQ_PAD_OUTP_TX0_CH_3,
    INT_GPI_FIQ_PAD_OUTN_TX0_CH_3, INT_GPI_FIQ_PAD_OUTP_TX0_CH_4, INT_GPI_FIQ_PAD_OUTN_TX0_CH_4,
    INT_GPI_FIQ_PAD_OUTP_TX1_CH_0, INT_GPI_FIQ_PAD_OUTN_TX1_CH_0, INT_GPI_FIQ_PAD_OUTP_TX1_CH_1,
    INT_GPI_FIQ_PAD_OUTN_TX1_CH_1, INT_GPI_FIQ_PAD_OUTP_TX1_CH_2, INT_GPI_FIQ_PAD_OUTN_TX1_CH_2,
    INT_GPI_FIQ_PAD_OUTP_TX1_CH_3, INT_GPI_FIQ_PAD_OUTN_TX1_CH_3, INT_GPI_FIQ_PAD_OUTP_TX1_CH_4,
    INT_GPI_FIQ_PAD_OUTN_TX1_CH_4, INT_GPI_FIQ_PAD_SAR_ADC_0,     INT_GPI_FIQ_PAD_SAR_ADC_1,
    INT_GPI_FIQ_PAD_SAR_ADC_2,     INT_GPI_FIQ_PAD_SAR_ADC_3,     INT_GPI_FIQ_PAD_SAR_ADC_4,
    INT_GPI_FIQ_PAD_SAR_ADC_5,     INT_GPI_FIQ_PAD_SAR_ADC_6,     INT_GPI_FIQ_PAD_SAR_ADC_7,
    INT_GPI_FIQ_PAD_SAR_ADC_8,     INT_GPI_FIQ_PAD_SAR_ADC_9,     INT_GPI_FIQ_PAD_SAR_ADC_10,
    INT_GPI_FIQ_PAD_SAR_ADC_11,    INT_GPI_FIQ_PAD_SAR_ADC_12,    INT_GPI_FIQ_PAD_SAR_ADC_13,
    INT_GPI_FIQ_PAD_SAR_ADC_14,    INT_GPI_FIQ_PAD_SAR_ADC_15,    INT_GPI_FIQ_PAD_SAR_ADC_16,
    INT_GPI_FIQ_PAD_SAR_ADC_17,    INT_GPI_FIQ_PAD_SAR_ADC_18,    INT_GPI_FIQ_PAD_SAR_ADC_19,
    INT_GPI_FIQ_PAD_SAR_ADC_20,    INT_GPI_FIQ_PAD_SAR_ADC_21,    INT_GPI_FIQ_PAD_SAR_ADC_22,
    INT_GPI_FIQ_PAD_SAR_ADC_23,    INT_GPI_FIQ_PAD_SR_IO4,        INT_GPI_FIQ_PAD_RGMII0_MCLK,
    INT_GPI_FIQ_PAD_RGMII0_RSTN,   INT_GPI_FIQ_PAD_RGMII0_RXCLK,  INT_GPI_FIQ_PAD_RGMII0_RXCTL,
    INT_GPI_FIQ_PAD_RGMII0_RXD0,   INT_GPI_FIQ_PAD_RGMII0_RXD1,   INT_GPI_FIQ_PAD_RGMII0_RXD2,
    INT_GPI_FIQ_PAD_RGMII0_RXD3,   INT_GPI_FIQ_PAD_RGMII0_TXCLK,  INT_GPI_FIQ_PAD_RGMII0_TXCTL,
    INT_GPI_FIQ_PAD_RGMII0_TXD0,   INT_GPI_FIQ_PAD_RGMII0_TXD1,   INT_GPI_FIQ_PAD_RGMII0_TXD2,
    INT_GPI_FIQ_PAD_RGMII0_TXD3,   INT_GPI_FIQ_PAD_RGMII0_MDIO,   INT_GPI_FIQ_PAD_RGMII0_MDC,
    INT_GPI_FIQ_PAD_UART_RX2,      INT_GPI_FIQ_PAD_UART_TX2,      INT_GPI_FIQ_PAD_UART_RX3,
    INT_GPI_FIQ_PAD_UART_TX3,      INT_GPI_FIQ_PAD_UART_RX4,      INT_GPI_FIQ_PAD_UART_TX4,
    INT_GPI_FIQ_PAD_UART_RX1,      INT_GPI_FIQ_PAD_UART_TX1,      INT_GPI_FIQ_PAD_FUART_RX,
    INT_GPI_FIQ_PAD_FUART_TX,      INT_GPI_FIQ_PAD_FUART_RTS,     INT_GPI_FIQ_PAD_FUART_CTS,
};

static const struct gpio_setting
{
    U8  p_name[32];
    U32 r_oen;
    U16 m_oen;
    U32 r_out;
    U16 m_out;
    U32 r_in;
    U16 m_in;
    U32 r_drv;
    U16 m_drv;
    U32 r_pe;
    U16 m_pe;
    U32 r_ps;
    U16 m_ps;
} gpio_table[] =
{
#define __GPIO__(_x_)   { CONCAT(CONCAT(GPIO, _x_), _NAME),  \
                          CONCAT(CONCAT(GPIO, _x_), _OEN),   \
                          CONCAT(CONCAT(GPIO, _x_), _OUT),   \
                          CONCAT(CONCAT(GPIO, _x_), _IN),    \
                          CONCAT(CONCAT(GPIO, _x_), _DRV),   \
                          CONCAT(CONCAT(GPIO, _x_), _PE),    \
                          CONCAT(CONCAT(GPIO, _x_), _PS) }
#define __GPIO(_x_)     __GPIO__(_x_)

//
// !! WARNING !! DO NOT MODIFIY !!!!
//
// These defines order must match following
// 1. the PAD name in GPIO excel
// 2. the perl script to generate the package header file
//
    __GPIO(0),   __GPIO(1),   __GPIO(2),   __GPIO(3),   __GPIO(4),   __GPIO(5),   __GPIO(6),   __GPIO(7),
    __GPIO(8),   __GPIO(9),   __GPIO(10),  __GPIO(11),  __GPIO(12),  __GPIO(13),  __GPIO(14),  __GPIO(15),
    __GPIO(16),  __GPIO(17),  __GPIO(18),  __GPIO(19),  __GPIO(20),  __GPIO(21),  __GPIO(22),  __GPIO(23),
    __GPIO(24),  __GPIO(25),  __GPIO(26),  __GPIO(27),  __GPIO(28),  __GPIO(29),  __GPIO(30),  __GPIO(31),
    __GPIO(32),  __GPIO(33),  __GPIO(34),  __GPIO(35),  __GPIO(36),  __GPIO(37),  __GPIO(38),  __GPIO(39),
    __GPIO(40),  __GPIO(41),  __GPIO(42),  __GPIO(43),  __GPIO(44),  __GPIO(45),  __GPIO(46),  __GPIO(47),
    __GPIO(48),  __GPIO(49),  __GPIO(50),  __GPIO(51),  __GPIO(52),  __GPIO(53),  __GPIO(54),  __GPIO(55),
    __GPIO(56),  __GPIO(57),  __GPIO(58),  __GPIO(59),  __GPIO(60),  __GPIO(61),  __GPIO(62),  __GPIO(63),
    __GPIO(64),  __GPIO(65),  __GPIO(66),  __GPIO(67),  __GPIO(68),  __GPIO(69),  __GPIO(70),  __GPIO(71),
    __GPIO(72),  __GPIO(73),  __GPIO(74),  __GPIO(75),  __GPIO(76),  __GPIO(77),  __GPIO(78),  __GPIO(79),
    __GPIO(80),  __GPIO(81),  __GPIO(82),  __GPIO(83),  __GPIO(84),  __GPIO(85),  __GPIO(86),  __GPIO(87),
    __GPIO(88),  __GPIO(89),  __GPIO(90),  __GPIO(91),  __GPIO(92),  __GPIO(93),  __GPIO(94),  __GPIO(95),
    __GPIO(96),  __GPIO(97),  __GPIO(98),  __GPIO(99),  __GPIO(100), __GPIO(101), __GPIO(102), __GPIO(103),
    __GPIO(104), __GPIO(105), __GPIO(106), __GPIO(107), __GPIO(108), __GPIO(109), __GPIO(110), __GPIO(111),
    __GPIO(112), __GPIO(113), __GPIO(114), __GPIO(115), __GPIO(116), __GPIO(117), __GPIO(118), __GPIO(119),
    __GPIO(120), __GPIO(121), __GPIO(122), __GPIO(123), __GPIO(124), __GPIO(125), __GPIO(126), __GPIO(127),
    __GPIO(128), __GPIO(129), __GPIO(130), __GPIO(131), __GPIO(132), __GPIO(133), __GPIO(134), __GPIO(135),
    __GPIO(136), __GPIO(137), __GPIO(138), __GPIO(139), __GPIO(140), __GPIO(141), __GPIO(142), __GPIO(143),
    __GPIO(144), __GPIO(145), __GPIO(146), __GPIO(147), __GPIO(148), __GPIO(149), __GPIO(150), __GPIO(151),
    __GPIO(152), __GPIO(153), __GPIO(154), __GPIO(155), __GPIO(156), __GPIO(157), __GPIO(158), __GPIO(159),
    __GPIO(160), __GPIO(161), __GPIO(162), __GPIO(163), __GPIO(164), __GPIO(165), __GPIO(166), __GPIO(167),
    __GPIO(168), __GPIO(169), __GPIO(170), __GPIO(171), __GPIO(172), __GPIO(173), __GPIO(174), __GPIO(175),
    __GPIO(176), __GPIO(177), __GPIO(178), __GPIO(179), __GPIO(180), __GPIO(181), __GPIO(182), __GPIO(183),
    __GPIO(184), __GPIO(185),

};

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

void MHal_GPIO_Init(void)
{
    MHal_CHIPTOP_REG(REG_ALL_PAD_IN) &= ~(BIT0 | BIT1);
}

void MHal_GPIO_Pad_Set(U8 u8IndexGPIO)
{
    HalPadSetVal(u8IndexGPIO, PINMUX_FOR_GPIO_MODE);
}

U8 MHal_GPIO_PadGroupMode_Set(U32 u32PadMode)
{
    return HalPadSetMode(u32PadMode);
}

U8 MHal_GPIO_PadVal_Set(U8 u8IndexGPIO, U32 u32PadMode)
{
    return HalPadSetVal((U32)u8IndexGPIO, u32PadMode);
}

U8 MHal_GPIO_PadVal_Get(U8 u8IndexGPIO, U32 *u32PadMode)
{
    return HalPadGetVal((U32)u8IndexGPIO, u32PadMode);
}

void MHal_GPIO_VolVal_Set(U8 u8Group, U32 u32Mode)
{
    HalGPIOSetVol((U32)u8Group, u32Mode);
}

U8 MHal_GPIO_PadVal_Check(U8 u8IndexGPIO, U32 u32PadMode)
{
    return HalPadCheckVal((U32)u8IndexGPIO, u32PadMode);
}

U8 MHal_GPIO_Pad_Oen(U8 u8IndexGPIO)
{
    U32 u32PadMode;
    if (u8IndexGPIO >= GPIO_NR)
    {
        return 1;
    }
    HalPadGetVal(u8IndexGPIO, &u32PadMode);
    if (u32PadMode == PINMUX_FOR_GPIO_MODE)
    {
        MHal_RIU_REG(gpio_table[u8IndexGPIO].r_oen) &= (~gpio_table[u8IndexGPIO].m_oen);
        return 0;
    }
    else
    {
        GPIO_ERR("GPIO[%d] not in GPIO MODE\n", u8IndexGPIO);
        return 1;
    }
}

U8 MHal_GPIO_Pad_Odn(U8 u8IndexGPIO)
{
    U32 u32PadMode;
    if (u8IndexGPIO >= GPIO_NR)
    {
        return 1;
    }
    HalPadGetVal(u8IndexGPIO, &u32PadMode);
    if (u32PadMode == PINMUX_FOR_GPIO_MODE)
    {
        MHal_RIU_REG(gpio_table[u8IndexGPIO].r_oen) |= gpio_table[u8IndexGPIO].m_oen;
        return 0;
    }
    else
    {
        GPIO_ERR("GPIO[%d] not in GPIO MODE\n", u8IndexGPIO);
        return 1;
    }
}

U8 MHal_GPIO_Pad_Level(U8 u8IndexGPIO, U8 *u8PadLevel)
{
    U32 u32PadMode;
    if (u8IndexGPIO >= GPIO_NR)
    {
        return 1;
    }
    HalPadGetVal(u8IndexGPIO, &u32PadMode);
    if (u32PadMode == PINMUX_FOR_GPIO_MODE)
    {
        *u8PadLevel = ((MHal_RIU_REG(gpio_table[u8IndexGPIO].r_in) & gpio_table[u8IndexGPIO].m_in) ? 1 : 0);
        return 0;
    }
    else
    {
        GPIO_ERR("GPIO[%d] not in GPIO MODE\n", u8IndexGPIO);
        return 1;
    }
}

U8 MHal_GPIO_Pad_InOut(U8 u8IndexGPIO, U8 *u8PadInOut)
{
    U32 u32PadMode;
    if (u8IndexGPIO >= GPIO_NR)
    {
        return 1;
    }
    HalPadGetVal(u8IndexGPIO, &u32PadMode);
    if (u32PadMode == PINMUX_FOR_GPIO_MODE)
    {
        *u8PadInOut = ((MHal_RIU_REG(gpio_table[u8IndexGPIO].r_oen) & gpio_table[u8IndexGPIO].m_oen) ? 1 : 0);
        return 0;
    }
    else
    {
        GPIO_ERR("GPIO[%d] not in GPIO MODE\n", u8IndexGPIO);
        return 1;
    }
}

U8 MHal_GPIO_Pull_High(U8 u8IndexGPIO)
{
    U32 u32PadMode;
    if (u8IndexGPIO >= GPIO_NR)
    {
        return 1;
    }
    HalPadGetVal(u8IndexGPIO, &u32PadMode);
    if (u32PadMode == PINMUX_FOR_GPIO_MODE)
    {
        MHal_RIU_REG(gpio_table[u8IndexGPIO].r_out) |= gpio_table[u8IndexGPIO].m_out;
        return 0;
    }
    else
    {
        GPIO_ERR("GPIO[%d] not in GPIO MODE\n", u8IndexGPIO);
        return 1;
    }
}

U8 MHal_GPIO_Pull_Low(U8 u8IndexGPIO)
{
    U32 u32PadMode;
    if (u8IndexGPIO >= GPIO_NR)
    {
        return 1;
    }
    HalPadGetVal(u8IndexGPIO, &u32PadMode);
    if (u32PadMode == PINMUX_FOR_GPIO_MODE)
    {
        MHal_RIU_REG(gpio_table[u8IndexGPIO].r_out) &= (~gpio_table[u8IndexGPIO].m_out);
        return 0;
    }
    else
    {
        GPIO_ERR("GPIO[%d] not in GPIO MODE\n", u8IndexGPIO);
        return 1;
    }
}

U8 MHal_GPIO_Pull_Up(U8 u8IndexGPIO)
{
    if (u8IndexGPIO >= GPIO_NR)
    {
        return 1;
    }
    else if (gpio_table[u8IndexGPIO].r_pe)
    {
        MHal_RIU_REG(gpio_table[u8IndexGPIO].r_pe) |= gpio_table[u8IndexGPIO].m_pe;
        if (gpio_table[u8IndexGPIO].r_ps)
        {
            MHal_RIU_REG(gpio_table[u8IndexGPIO].r_ps) |= gpio_table[u8IndexGPIO].m_ps;
        }
    }
    else
    {
        return 1; // no support pull up
    }
    return 0;
}

U8 MHal_GPIO_Pull_Down(U8 u8IndexGPIO)
{
    if (u8IndexGPIO >= GPIO_NR)
    {
        return 1;
    }
    else if (gpio_table[u8IndexGPIO].r_pe)
    {
        MHal_RIU_REG(gpio_table[u8IndexGPIO].r_pe) |= gpio_table[u8IndexGPIO].m_pe;
        if (gpio_table[u8IndexGPIO].r_ps)
        {
            MHal_RIU_REG(gpio_table[u8IndexGPIO].r_ps) &= ~gpio_table[u8IndexGPIO].m_ps;
        }
    }
    else
    {
        return 1; // no support pull down
    }
    return 0;
}

U8 MHal_GPIO_Pull_Off(U8 u8IndexGPIO)
{
    if (u8IndexGPIO >= GPIO_NR)
    {
        return 1;
    }
    else if (gpio_table[u8IndexGPIO].r_pe)
    {
        MHal_RIU_REG(gpio_table[u8IndexGPIO].r_pe) &= ~gpio_table[u8IndexGPIO].m_pe;
    }
    else
    {
        return 1; // no support pull enable
    }
    return 0;
}

U8 MHal_GPIO_Pull_status(U8 u8IndexGPIO, U8 *u8PullStatus)
{
    if (u8IndexGPIO >= GPIO_NR)
    {
        return 1;
    }
    else if (gpio_table[u8IndexGPIO].r_pe)
    {
        if (MHal_RIU_REG(gpio_table[u8IndexGPIO].r_pe) & gpio_table[u8IndexGPIO].m_pe)
        {
            if (MHal_RIU_REG(gpio_table[u8IndexGPIO].r_ps) & gpio_table[u8IndexGPIO].m_ps)
                *u8PullStatus = MHAL_PULL_UP;
            else
                *u8PullStatus = MHAL_PULL_DOWN;
        }
        else
            *u8PullStatus = MHAL_PULL_OFF;
    }
    else
    {
        return 1;
    }
    return 0;
}

U8 MHal_GPIO_Set_High(U8 u8IndexGPIO)
{
    U32 u32PadMode;
    if (u8IndexGPIO >= GPIO_NR)
    {
        return 1;
    }
    HalPadGetVal(u8IndexGPIO, &u32PadMode);
    if (u32PadMode == PINMUX_FOR_GPIO_MODE)
    {
        MHal_RIU_REG(gpio_table[u8IndexGPIO].r_oen) &= (~gpio_table[u8IndexGPIO].m_oen);
        MHal_RIU_REG(gpio_table[u8IndexGPIO].r_out) |= gpio_table[u8IndexGPIO].m_out;
        return 0;
    }
    else
    {
        GPIO_ERR("GPIO[%d] not in GPIO MODE\n", u8IndexGPIO);
        return 1;
    }
}

U8 MHal_GPIO_Set_Low(U8 u8IndexGPIO)
{
    U32 u32PadMode;
    if (u8IndexGPIO >= GPIO_NR)
    {
        return 1;
    }
    HalPadGetVal(u8IndexGPIO, &u32PadMode);
    if (u32PadMode == PINMUX_FOR_GPIO_MODE)
    {
        MHal_RIU_REG(gpio_table[u8IndexGPIO].r_oen) &= (~gpio_table[u8IndexGPIO].m_oen);
        MHal_RIU_REG(gpio_table[u8IndexGPIO].r_out) &= (~gpio_table[u8IndexGPIO].m_out);
        return 0;
    }
    else
    {
        GPIO_ERR("GPIO[%d] not in GPIO MODE\n", u8IndexGPIO);
        return 1;
    }
}

U8 MHal_GPIO_Drv_Set(U8 u8IndexGPIO, U8 u8Level)
{
    U8  u8Lsb   = 0;
    U16 u16Mask = 0;
    if (u8IndexGPIO >= GPIO_NR)
    {
        return 1;
    }
    else if (gpio_table[u8IndexGPIO].r_drv)
    {
        u16Mask = gpio_table[u8IndexGPIO].m_drv;

        if (!u16Mask)
        {
            return 1;
        }

        // Calculate LSB by dichotomy
        if ((u16Mask & 0xFF) == 0)
        {
            u16Mask >>= 8;
            u8Lsb += 8;
        }
        if ((u16Mask & 0xF) == 0)
        {
            u16Mask >>= 4;
            u8Lsb += 4;
        }
        if ((u16Mask & 0x3) == 0)
        {
            u16Mask >>= 2;
            u8Lsb += 2;
        }
        if ((u16Mask & 0x1) == 0)
        {
            u8Lsb += 1;
        }

        if (u8Level > (gpio_table[u8IndexGPIO].m_drv >> u8Lsb))
        {
            return 1;
        }
        MHal_RIU_REG(gpio_table[u8IndexGPIO].r_drv) &= ~gpio_table[u8IndexGPIO].m_drv;
        MHal_RIU_REG(gpio_table[u8IndexGPIO].r_drv) |= ((u8Level << u8Lsb) & gpio_table[u8IndexGPIO].m_drv);
    }
    else
    {
        return 1; // no support set driving
    }
    return 0;
}

U8 MHal_GPIO_Drv_Get(U8 u8IndexGPIO, U8 *u8Level)
{
    U8  u8Lsb   = 0;
    U16 u16Mask = 0;

    if (u8IndexGPIO >= GPIO_NR)
    {
        return 1;
    }
    else if (gpio_table[u8IndexGPIO].r_drv)
    {
        u16Mask = gpio_table[u8IndexGPIO].m_drv;

        // Calculate LSB by dichotomy
        if ((u16Mask & 0xFF) == 0)
        {
            u16Mask >>= 8;
            u8Lsb += 8;
        }
        if ((u16Mask & 0xF) == 0)
        {
            u16Mask >>= 4;
            u8Lsb += 4;
        }
        if ((u16Mask & 0x3) == 0)
        {
            u16Mask >>= 2;
            u8Lsb += 2;
        }
        if ((u16Mask & 0x1) == 0)
        {
            u8Lsb += 1;
        }
        *u8Level = ((MHal_RIU_REG(gpio_table[u8IndexGPIO].r_drv) & gpio_table[u8IndexGPIO].m_drv) >> u8Lsb);
        return 0;
    }
    else
    {
        return 1;
    }
}

static int PMSLEEP_GPIO_To_Irq(U8 u8IndexGPIO)
{
    if ((u8IndexGPIO < PAD_PM_PWM0) || (u8IndexGPIO > PAD_PM_SPI_DI))
    {
        return -1;
    }
    else
    {
        if(_pmsleep_to_irq_table[u8IndexGPIO - PAD_PM_PWM0] != INT_PMSLEEP_INVALID)
            return _pmsleep_to_irq_table[u8IndexGPIO - PAD_PM_PWM0];
        else
            return -1;
    }
}

int GPI_GPIO_To_Irq(U8 u8IndexGPIO)
{
    if ((u8IndexGPIO < PAD_I2C5_SCL) || (u8IndexGPIO > PAD_FUART_CTS))
        return -1;
    else
        return _gpi_to_irq_table[u8IndexGPIO- PAD_I2C5_SCL];
}

//MHal_GPIO_To_Irq return any virq
int MHal_GPIO_To_Irq(U8 u8IndexGPIO)
{
	IntInitParam_u uInitParam = {{0}};
    int hwirq = 0, virq = -1;

    if ((virq = PMSLEEP_GPIO_To_Irq(u8IndexGPIO)) >= 0)
    {
        uInitParam.intc.eMap = INTC_MAP_IRQ;
        uInitParam.intc.ePriority = INTC_PRIORITY_7;
        uInitParam.intc.pfnIsr = (PfnIntcISR)ss_pm_vintc.pvIntIsrCB;
    	DrvRegisterVirtualInterrupt(&uInitParam,&ss_pm_vintc);

    	hwirq = ss_pm_vintc.nHWIRQNum;
    }
    else if ((virq = GPI_GPIO_To_Irq(u8IndexGPIO)) >= 0)
    {
        uInitParam.intc.eMap = INTC_MAP_IRQ;
        uInitParam.intc.ePriority = INTC_PRIORITY_7;
        uInitParam.intc.pfnIsr = (PfnIntcISR)ss_gpi_vintc.pvIntIsrCB;
    	DrvRegisterVirtualInterrupt(&uInitParam,&ss_gpi_vintc);

    	hwirq = ss_gpi_vintc.nHWIRQNum;
    }

    return (virq << 16 | hwirq);
}

U8 MHal_GPIO_Get_CheckCount(void)
{
    return HalPadCheckInfoCount();
}

void *MHal_GPIO_Get_CheckInfo(U8 u8Index)
{
    return HalPadCheckInfoGet(u8Index);
}

U8 MHal_GPIO_NameToNum(U8 *pu8Name, U8 *GpioIndex)
{
    u8 index;
    for (index = 0; index < (sizeof(gpio_table) / sizeof(gpio_table[0])); index++)
    {
        if (!strcmp((const char *)gpio_table[index].p_name, (const char *)pu8Name))
        {
            *GpioIndex = index;
            return 0;
        }
    }
    return 1;
}

U8 MHal_GPIO_PadModeToVal(U8 *pu8Mode, U8 *u8Index)
{
    return HalPadModeToVal(pu8Mode, u8Index);
}

U32 *MHal_GPIO_PadModeToPadIndex(U32 u32Mode)
{
    return HalPadModeToPadIndex(u32Mode);
}
