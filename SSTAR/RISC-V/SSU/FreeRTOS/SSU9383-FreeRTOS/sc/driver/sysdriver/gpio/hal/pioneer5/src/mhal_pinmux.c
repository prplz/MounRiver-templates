/*
 * mhal_pinmux.c- Sigmastar
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
#include "ms_platform.h"
//#include "mdrv_types.h"
#include "mhal_gpio.h"
#include "padmux.h"
#include "gpio.h"
#include <string.h>
//==============================================================================
//
//                              MACRO DEFINE
//
//==============================================================================
extern s8 UartSendTrace(const char *strFormat, ...);
#define _PADMUX_PRINT(fmt,...)           UartSendTrace(fmt, ##__VA_ARGS__)

#ifdef CONFIG_CPU_ARCH_RISCV
#define BASE_RIU_PA    0X02000000
#else
#define BASE_RIU_PA    0X1F000000
#endif
#define PMSLEEP_BANK   0x000E00
#define PM_SAR_BANK    0x001400
#define ALBANY1_BANK   0x151500
#define ALBANY2_BANK   0x151600
#define CHIPTOP_BANK   0x101E00
#define PADTOP_BANK    0x103C00
#define PADGPIO_BANK   0x103E00
#define PM_PADTOP_BANK 0x003F00
#define UTMI0_BANK     0x142100
#define UTMI1_BANK     0x142900
#define PADGPIO2_BANK  0x110400

#define _GPIO_W_WORD(addr, val)                                    \
    {                                                              \
        (*(volatile u16*)(addr)) = (u16)(val); \
    }
#define _GPIO_W_WORD_MASK(addr, val, mask)                                                    \
    {                                                                                         \
        (*(volatile u16*)(addr)) =                                        \
            ((*(volatile u16*)(addr)) & ~(mask)) | ((u16)(val) & (mask)); \
    }
#define _GPIO_R_BYTE(addr)            (*(volatile u8*)(addr))
#define _GPIO_R_WORD_MASK(addr, mask) ((*(volatile u16*)(addr)) & (mask))

#define GET_BASE_ADDR_BY_BANK(x, y) ((x) + ((y) << 1))
#define _RIUA_8BIT(bank, offset)    GET_BASE_ADDR_BY_BANK(BASE_RIU_PA, bank) + (((offset) & ~1) << 1) + ((offset)&1)
#define _RIUA_16BIT(bank, offset)   GET_BASE_ADDR_BY_BANK(BASE_RIU_PA, bank) + ((offset) << 2)

/* please put GPIO_GEN marco to here start */

// PADMUX MASK MARCO START
#define REG_EJ_MODE                  0x60
#define REG_EJ_MODE_MASK             BIT7 | BIT8
#define REG_RISCV_EJ_MODE            0x71
#define REG_RISCV_EJ_MODE_MASK       BIT0 | BIT1
#define REG_TEST_IN_MODE             0x12
#define REG_TEST_IN_MODE_MASK        BIT0 | BIT1
#define REG_TEST_OUT_MODE            0x12
#define REG_TEST_OUT_MODE_MASK       BIT4 | BIT5
#define REG_ADC0_MODE                0x3b
#define REG_ADC0_MODE_MASK           BIT0
#define REG_ADC1_MODE                0x3b
#define REG_ADC1_MODE_MASK           BIT1
#define REG_ADC2_MODE                0x3b
#define REG_ADC2_MODE_MASK           BIT2
#define REG_ADC3_MODE                0x3b
#define REG_ADC3_MODE_MASK           BIT3
#define REG_ADC4_MODE                0x3b
#define REG_ADC4_MODE_MASK           BIT4
#define REG_ADC5_MODE                0x3b
#define REG_ADC5_MODE_MASK           BIT5
#define REG_ADC6_MODE                0x3b
#define REG_ADC6_MODE_MASK           BIT6
#define REG_ADC7_MODE                0x3b
#define REG_ADC7_MODE_MASK           BIT7
#define REG_ADC8_MODE                0x3b
#define REG_ADC8_MODE_MASK           BIT8
#define REG_ADC9_MODE                0x3b
#define REG_ADC9_MODE_MASK           BIT9
#define REG_ADC10_MODE               0x3b
#define REG_ADC10_MODE_MASK          BIT10
#define REG_ADC11_MODE               0x3b
#define REG_ADC11_MODE_MASK          BIT11
#define REG_ADC12_MODE               0x3b
#define REG_ADC12_MODE_MASK          BIT12
#define REG_ADC13_MODE               0x3b
#define REG_ADC13_MODE_MASK          BIT13
#define REG_ADC14_MODE               0x3b
#define REG_ADC14_MODE_MASK          BIT14
#define REG_ADC15_MODE               0x3b
#define REG_ADC15_MODE_MASK          BIT15
#define REG_ADC16_MODE               0x3c
#define REG_ADC16_MODE_MASK          BIT0
#define REG_ADC17_MODE               0x3c
#define REG_ADC17_MODE_MASK          BIT1
#define REG_ADC18_MODE               0x3c
#define REG_ADC18_MODE_MASK          BIT2
#define REG_ADC19_MODE               0x3c
#define REG_ADC19_MODE_MASK          BIT3
#define REG_ADC20_MODE               0x3c
#define REG_ADC20_MODE_MASK          BIT4
#define REG_ADC21_MODE               0x3c
#define REG_ADC21_MODE_MASK          BIT5
#define REG_ADC22_MODE               0x3c
#define REG_ADC22_MODE_MASK          BIT6
#define REG_ADC23_MODE               0x3c
#define REG_ADC23_MODE_MASK          BIT7
#define REG_ADC_INT_MODE             0x3a
#define REG_ADC_INT_MODE_MASK        BIT8 | BIT9
#define REG_I2C0_MODE                0x6f
#define REG_I2C0_MODE_MASK           BIT0 | BIT1 | BIT2
#define REG_I2C1_MODE                0x53
#define REG_I2C1_MODE_MASK           BIT0 | BIT1 | BIT2
#define REG_I2C2_MODE                0x6f
#define REG_I2C2_MODE_MASK           BIT8 | BIT9
#define REG_I2C3_MODE                0x73
#define REG_I2C3_MODE_MASK           BIT0 | BIT1
#define REG_I2C4_MODE                0x73
#define REG_I2C4_MODE_MASK           BIT4 | BIT5 | BIT6
#define REG_I2C5_MODE                0x73
#define REG_I2C5_MODE_MASK           BIT8 | BIT9 | BIT10
#define REG_SPI0_MODE                0x68
#define REG_SPI0_MODE_MASK           BIT0 | BIT1
#define REG_SPI0_CZ1_MODE            0x70
#define REG_SPI0_CZ1_MODE_MASK       BIT4 | BIT5
#define REG_SPI1_MODE                0x68
#define REG_SPI1_MODE_MASK           BIT3 | BIT4 | BIT5
#define REG_SPI1_CZ1_MODE            0x68
#define REG_SPI1_CZ1_MODE_MASK       BIT6 | BIT7
#define REG_FUART_MODE               0x6e
#define REG_FUART_MODE_MASK          BIT8 | BIT9 | BIT10
#define REG_FUART_2W_MODE            0x6e
#define REG_FUART_2W_MODE_MASK       BIT12 | BIT13 | BIT14
#define REG_UART0_MODE               0x6d
#define REG_UART0_MODE_MASK          BIT0 | BIT1
#define REG_UART1_MODE               0x6d
#define REG_UART1_MODE_MASK          BIT4 | BIT5
#define REG_UART2_MODE               0x6d
#define REG_UART2_MODE_MASK          BIT8 | BIT9 | BIT10
#define REG_UART3_MODE               0x6d
#define REG_UART3_MODE_MASK          BIT12 | BIT13
#define REG_UART4_MODE               0x49
#define REG_UART4_MODE_MASK          BIT0 | BIT1
#define REG_UART5_MODE               0x49
#define REG_UART5_MODE_MASK          BIT4 | BIT5 | BIT6
#define REG_SD0_BOOT_MODE            0x8
#define REG_SD0_BOOT_MODE_MASK       BIT0
#define REG_EMMC8B_BOOT_MODE         0x9
#define REG_EMMC8B_BOOT_MODE_MASK    BIT0
#define REG_EMMC4B_BOOT_MODE         0x9
#define REG_EMMC4B_BOOT_MODE_MASK    BIT8
#define REG_SD0_MODE                 0x67
#define REG_SD0_MODE_MASK            BIT8 | BIT9
#define REG_SD0_CDZ_MODE             0x67
#define REG_SD0_CDZ_MODE_MASK        BIT10 | BIT11
#define REG_SD0_RSTN_MODE            0x7a
#define REG_SD0_RSTN_MODE_MASK       BIT0 | BIT1
#define REG_SDIO_MODE                0x67
#define REG_SDIO_MODE_MASK           BIT12 | BIT13
#define REG_SDIO_CDZ_MODE            0x68
#define REG_SDIO_CDZ_MODE_MASK       BIT12 | BIT13
#define REG_SDIO_RSTN_MODE           0x68
#define REG_SDIO_RSTN_MODE_MASK      BIT14 | BIT15
#define REG_EMMC_8B_MODE             0x61
#define REG_EMMC_8B_MODE_MASK        BIT2
#define REG_EMMC_4B_MODE             0x61
#define REG_EMMC_4B_MODE_MASK        BIT0
#define REG_EMMC_RST_MODE            0x61
#define REG_EMMC_RST_MODE_MASK       BIT4
#define REG_EMMC_AS_SD_CDZ_MODE      0x61
#define REG_EMMC_AS_SD_CDZ_MODE_MASK BIT8
#define REG_LED0_MODE                0x63
#define REG_LED0_MODE_MASK           BIT0 | BIT1
#define REG_LED1_MODE                0x63
#define REG_LED1_MODE_MASK           BIT4 | BIT5
#define REG_PWM0_PN                  0x13
#define REG_PWM0_PN_MASK             BIT0
#define REG_PWM1_PN                  0x13
#define REG_PWM1_PN_MASK             BIT2
#define REG_PWM2_PN                  0x13
#define REG_PWM2_PN_MASK             BIT4
#define REG_PWM3_PN                  0x13
#define REG_PWM3_PN_MASK             BIT6
#define REG_PWM4_PN                  0x13
#define REG_PWM4_PN_MASK             BIT8
#define REG_PWM5_PN                  0x13
#define REG_PWM5_PN_MASK             BIT10
#define REG_PWM_OUT0_MODE            0x65
#define REG_PWM_OUT0_MODE_MASK       BIT0 | BIT1 | BIT2
#define REG_PWM_OUT1_MODE            0x65
#define REG_PWM_OUT1_MODE_MASK       BIT4 | BIT5 | BIT6
#define REG_PWM_OUT2_MODE            0x65
#define REG_PWM_OUT2_MODE_MASK       BIT8 | BIT9 | BIT10
#define REG_PWM_OUT3_MODE            0x65
#define REG_PWM_OUT3_MODE_MASK       BIT12 | BIT13 | BIT14
#define REG_PWM_OUT4_MODE            0x7b
#define REG_PWM_OUT4_MODE_MASK       BIT0 | BIT1 | BIT2
#define REG_PWM_OUT5_MODE            0x7b
#define REG_PWM_OUT5_MODE_MASK       BIT8 | BIT9
#define REG_PWM_OUT6_MODE            0x7b
#define REG_PWM_OUT6_MODE_MASK       BIT10 | BIT11
#define REG_PWM_OUT7_MODE            0x7b
#define REG_PWM_OUT7_MODE_MASK       BIT12 | BIT13
#define REG_PWM_OUT8_MODE            0x7c
#define REG_PWM_OUT8_MODE_MASK       BIT0 | BIT1
#define REG_PWM_OUT9_MODE            0x7c
#define REG_PWM_OUT9_MODE_MASK       BIT2 | BIT3
#define REG_PWM_OUT10_MODE           0x7c
#define REG_PWM_OUT10_MODE_MASK      BIT4 | BIT5
#define REG_PWM_OUT11_MODE           0x7c
#define REG_PWM_OUT11_MODE_MASK      BIT6 | BIT7
#define REG_PWM_OUT12_MODE           0x7d
#define REG_PWM_OUT12_MODE_MASK      BIT0 | BIT1 | BIT2
#define REG_PWM_OUT13_MODE           0x7c
#define REG_PWM_OUT13_MODE_MASK      BIT10 | BIT11
#define REG_PWM_OUT14_MODE           0x7c
#define REG_PWM_OUT14_MODE_MASK      BIT12 | BIT13
#define REG_PWM_OUT15_MODE           0x7c
#define REG_PWM_OUT15_MODE_MASK      BIT14 | BIT15
#define REG_PWM_OUT16_MODE           0x57
#define REG_PWM_OUT16_MODE_MASK      BIT8 | BIT9
#define REG_PWM_OUT17_MODE           0x7d
#define REG_PWM_OUT17_MODE_MASK      BIT4 | BIT5 | BIT6
#define REG_PWM_OUT18_MODE           0x57
#define REG_PWM_OUT18_MODE_MASK      BIT12 | BIT13
#define REG_PWM_OUT19_MODE           0x57
#define REG_PWM_OUT19_MODE_MASK      BIT14 | BIT15
#define REG_PWM_IN0_MODE             0x23
#define REG_PWM_IN0_MODE_MASK        BIT0 | BIT1
#define REG_PWM_IN1_MODE             0x23
#define REG_PWM_IN1_MODE_MASK        BIT2 | BIT3
#define REG_PWM_IN2_MODE             0x23
#define REG_PWM_IN2_MODE_MASK        BIT4 | BIT5
#define REG_PWM_IN3_MODE             0x23
#define REG_PWM_IN3_MODE_MASK        BIT6 | BIT7
#define REG_PWM_IN4_MODE             0x23
#define REG_PWM_IN4_MODE_MASK        BIT8 | BIT9
#define REG_PWM_IN5_MODE             0x23
#define REG_PWM_IN5_MODE_MASK        BIT10 | BIT11
#define REG_PWM_IN6_MODE             0x23
#define REG_PWM_IN6_MODE_MASK        BIT12 | BIT13
#define REG_PWM_IN7_MODE             0x23
#define REG_PWM_IN7_MODE_MASK        BIT14 | BIT15
#define REG_PWM_INT_MODE             0x3a
#define REG_PWM_INT_MODE_MASK        BIT0 | BIT1
#define REG_I2S0_MCK_MODE            0x62
#define REG_I2S0_MCK_MODE_MASK       BIT0 | BIT1
#define REG_I2S0_RX_MODE             0x62
#define REG_I2S0_RX_MODE_MASK        BIT8 | BIT9
#define REG_I2S0_TX_MODE             0x62
#define REG_I2S0_TX_MODE_MASK        BIT12 | BIT13
#define REG_I2S0_RXTX_MODE           0x4f
#define REG_I2S0_RXTX_MODE_MASK      BIT0 | BIT1
#define REG_I2S1_MCK_MODE            0x62
#define REG_I2S1_MCK_MODE_MASK       BIT4 | BIT5
#define REG_I2S1_RX_MODE             0x46
#define REG_I2S1_RX_MODE_MASK        BIT0 | BIT1
#define REG_I2S1_TX_MODE             0x46
#define REG_I2S1_TX_MODE_MASK        BIT4 | BIT5
#define REG_I2S1_RXTX_MODE           0x4f
#define REG_I2S1_RXTX_MODE_MASK      BIT4 | BIT5
#define REG_DMIC_2CH_MODE            0x60
#define REG_DMIC_2CH_MODE_MASK       BIT0 | BIT1 | BIT2
#define REG_DMIC_4CH_MODE            0x79
#define REG_DMIC_4CH_MODE_MASK       BIT0 | BIT1 | BIT2
#define REG_DMIC_8CH_MODE            0x79
#define REG_DMIC_8CH_MODE_MASK       BIT4 | BIT5 | BIT6
#define REG_SR0_MIPI_MODE            0x69
#define REG_SR0_MIPI_MODE_MASK       BIT0 | BIT1 | BIT2
#define REG_SR1_MIPI_MODE            0x69
#define REG_SR1_MIPI_MODE_MASK       BIT4 | BIT5 | BIT6
#define REG_SR00_PDN_MODE            0x51
#define REG_SR00_PDN_MODE_MASK       BIT0
#define REG_SR00_MCLK_MODE           0x6a
#define REG_SR00_MCLK_MODE_MASK      BIT0
#define REG_SR00_RST_MODE            0x54
#define REG_SR00_RST_MODE_MASK       BIT0
#define REG_SR01_PDN_MODE            0x51
#define REG_SR01_PDN_MODE_MASK       BIT2
#define REG_SR01_MCLK_MODE           0x6a
#define REG_SR01_MCLK_MODE_MASK      BIT3
#define REG_SR01_RST_MODE            0x54
#define REG_SR01_RST_MODE_MASK       BIT2
#define REG_SR10_PDN_MODE            0x51
#define REG_SR10_PDN_MODE_MASK       BIT8
#define REG_SR10_MCLK_MODE           0x6a
#define REG_SR10_MCLK_MODE_MASK      BIT8
#define REG_SR10_RST_MODE            0x54
#define REG_SR10_RST_MODE_MASK       BIT8
#define REG_SR11_PDN_MODE            0x51
#define REG_SR11_PDN_MODE_MASK       BIT10
#define REG_SR11_MCLK_MODE           0x6a
#define REG_SR11_MCLK_MODE_MASK      BIT10
#define REG_SR11_RST_MODE            0x54
#define REG_SR11_RST_MODE_MASK       BIT10
#define REG_SR0_SYNC_MODE            0x52
#define REG_SR0_SYNC_MODE_MASK       BIT8 | BIT9
#define REG_SR0_PCLK_MODE            0x52
#define REG_SR0_PCLK_MODE_MASK       BIT0 | BIT1
#define REG_SR0_MODE                 0x6b
#define REG_SR0_MODE_MASK            BIT0 | BIT1
#define REG_SR0_BT656_MODE           0x4a
#define REG_SR0_BT656_MODE_MASK      BIT0
#define REG_SR1_BT656_MODE           0x4a
#define REG_SR1_BT656_MODE_MASK      BIT2
#define REG_SR0_BT1120_MODE          0x48
#define REG_SR0_BT1120_MODE_MASK     BIT0 | BIT1
#define REG_SR_SLAVE_XLK_MODE        0x14
#define REG_SR_SLAVE_XLK_MODE_MASK   BIT0 | BIT1
#define REG_SR_SLAVE_MODE            0x12
#define REG_SR_SLAVE_MODE_MASK       BIT12 | BIT13
#define REG_LVDS_OUT_MODE            0x44
#define REG_LVDS_OUT_MODE_MASK       BIT8 | BIT9
#define REG_MIPITX0_OUT_MODE         0x44
#define REG_MIPITX0_OUT_MODE_MASK    BIT0 | BIT1 | BIT2
#define REG_MIPITX1_OUT_MODE         0x44
#define REG_MIPITX1_OUT_MODE_MASK    BIT4 | BIT5 | BIT6
#define REG_BT656_OUT_MODE           0x60
#define REG_BT656_OUT_MODE_MASK      BIT4 | BIT5
#define REG_BT601_OUT_MODE           0x72
#define REG_BT601_OUT_MODE_MASK      BIT4 | BIT5
#define REG_TTL24_MODE               0x6c
#define REG_TTL24_MODE_MASK          BIT8 | BIT9
#define REG_TTL18_MODE               0x6c
#define REG_TTL18_MODE_MASK          BIT4 | BIT5
#define REG_TTL16_MODE               0x6c
#define REG_TTL16_MODE_MASK          BIT0 | BIT1
#define REG_RGMII0_MODE              0x41
#define REG_RGMII0_MODE_MASK         BIT0
#define REG_GPHY0_REF_MODE           0x38
#define REG_GPHY0_REF_MODE_MASK      BIT0 | BIT1
#define REG_RMII_MODE                0x41
#define REG_RMII_MODE_MASK           BIT8
#define REG_RMII1_MODE               0x41
#define REG_RMII1_MODE_MASK          BIT1
#define REG_PSPI0_G_MODE             0x56
#define REG_PSPI0_G_MODE_MASK        BIT0
#define REG_PSPI0_TE_MODE            0x56
#define REG_PSPI0_TE_MODE_MASK       BIT4
#define REG_PSPI0_SENSOR_MODE        0x57
#define REG_PSPI0_SENSOR_MODE_MASK   BIT0 | BIT1
#define REG_PSPI0_CS_MODE            0x57
#define REG_PSPI0_CS_MODE_MASK       BIT2
#define REG_PSPI0_VSYNC_MODE         0x56
#define REG_PSPI0_VSYNC_MODE_MASK    BIT6
#define REG_PSPI0_CS2_MODE           0x56
#define REG_PSPI0_CS2_MODE_MASK      BIT2
#define REG_PSPI1_G_MODE             0x56
#define REG_PSPI1_G_MODE_MASK        BIT8
#define REG_PSPI1_PANEL_MODE         0x57
#define REG_PSPI1_PANEL_MODE_MASK    BIT4 | BIT5
#define REG_PSPI1_TE_MODE            0x56
#define REG_PSPI1_TE_MODE_MASK       BIT12 | BIT13
#define REG_PSPI1_CS2_MODE           0x56
#define REG_PSPI1_CS2_MODE_MASK      BIT10 | BIT11
#define REG_PSPI1_VSYNC_MODE         0x56
#define REG_PSPI1_VSYNC_MODE_MASK    BIT14
#define REG_IR0_MODE                 0x24
#define REG_IR0_MODE_MASK            BIT0 | BIT1
#define REG_IR1_MODE                 0x24
#define REG_IR1_MODE_MASK            BIT2 | BIT3
#define REG_IR2_MODE                 0x24
#define REG_IR2_MODE_MASK            BIT4 | BIT5
#define REG_IR3_MODE                 0x24
#define REG_IR3_MODE_MASK            BIT6 | BIT7
#define REG_KEY_FIX_MODE             0x21
#define REG_KEY_FIX_MODE_MASK        BIT8 | BIT9
#define REG_HDMI_I2C_MODE            0x22
#define REG_HDMI_I2C_MODE_MASK       BIT0
#define REG_HDMI_CEC_MODE            0x22
#define REG_HDMI_CEC_MODE_MASK       BIT4
#define REG_HDMI_HTP_MODE            0x22
#define REG_HDMI_HTP_MODE_MASK       BIT8
#define REG_HDMI_5VDET_MODE          0x22
#define REG_HDMI_5VDET_MODE_MASK     BIT12
#define REG_SPDIF_MODE               0x25
#define REG_SPDIF_MODE_MASK          BIT0 | BIT1
#define REG_OTP_TEST                 0x64
#define REG_OTP_TEST_MASK            BIT8
#define REG_IR_IN_MODE               0x62
#define REG_IR_IN_MODE_MASK          BIT0 | BIT1
#define REG_I2CM_MODE                0x50
#define REG_I2CM_MODE_MASK           BIT0 | BIT1
#define REG_PM_UART1_MODE            0x53
#define REG_PM_UART1_MODE_MASK       BIT0 | BIT1
#define REG_PM51_UART_MODE           0x65
#define REG_PM51_UART_MODE_MASK      BIT0 | BIT1
#define REG_PWM0_MODE                0x51
#define REG_PWM0_MODE_MASK           BIT0 | BIT1
#define REG_VID_MODE                 0x53
#define REG_VID_MODE_MASK            BIT2 | BIT3
#define REG_PWM1_MODE                0x51
#define REG_PWM1_MODE_MASK           BIT2 | BIT3
#define REG_PM_PAD_EXT_MODE0         0x55
#define REG_PM_PAD_EXT_MODE0_MASK    BIT0
#define REG_PM_PAD_EXT_MODE1         0x55
#define REG_PM_PAD_EXT_MODE1_MASK    BIT1
#define REG_PM_PAD_EXT_MODE2         0x55
#define REG_PM_PAD_EXT_MODE2_MASK    BIT2
#define REG_PM_PAD_EXT_MODE3         0x55
#define REG_PM_PAD_EXT_MODE3_MASK    BIT3
#define REG_PM_PAD_EXT_MODE4         0x55
#define REG_PM_PAD_EXT_MODE4_MASK    BIT4
#define REG_PM_PAD_EXT_MODE5         0x55
#define REG_PM_PAD_EXT_MODE5_MASK    BIT5
#define REG_PM_PAD_EXT_MODE6         0x55
#define REG_PM_PAD_EXT_MODE6_MASK    BIT6
#define REG_PM_PAD_EXT_MODE7         0x55
#define REG_PM_PAD_EXT_MODE7_MASK    BIT7
#define REG_PM_PAD_EXT_MODE8         0x55
#define REG_PM_PAD_EXT_MODE8_MASK    BIT8
#define REG_PM_PAD_EXT_MODE9         0x55
#define REG_PM_PAD_EXT_MODE9_MASK    BIT9
#define REG_PM_PAD_EXT_MODE10        0x55
#define REG_PM_PAD_EXT_MODE10_MASK   BIT10
#define REG_PM_PAD_EXT_MODE11        0x55
#define REG_PM_PAD_EXT_MODE11_MASK   BIT11
#define REG_PM_PAD_EXT_MODE12        0x55
#define REG_PM_PAD_EXT_MODE12_MASK   BIT12
#define REG_PM_PAD_EXT_MODE13        0x55
#define REG_PM_PAD_EXT_MODE13_MASK   BIT13
#define REG_PM_PAD_EXT_MODE14        0x55
#define REG_PM_PAD_EXT_MODE14_MASK   BIT14
#define REG_PM_PAD_EXT_MODE15        0x55
#define REG_PM_PAD_EXT_MODE15_MASK   BIT15
#define REG_PM_PAD_EXT_MODE16        0x56
#define REG_PM_PAD_EXT_MODE16_MASK   BIT0
#define REG_PM_PAD_EXT_MODE17        0x56
#define REG_PM_PAD_EXT_MODE17_MASK   BIT1
#define REG_PM_PAD_EXT_MODE18        0x56
#define REG_PM_PAD_EXT_MODE18_MASK   BIT2
#define REG_SPI_GPIO                 0x35
#define REG_SPI_GPIO_MASK            BIT0 | BIT1 | BIT2
#define REG_SPIWPN_GPIO              0x35
#define REG_SPIWPN_GPIO_MASK         BIT4
#define REG_SPICSZ1_GPIO             0x35
#define REG_SPICSZ1_GPIO_MASK        BIT2
#define REG_SPICSZ2_MODE             0x66
#define REG_SPICSZ2_MODE_MASK        BIT0 | BIT1
#define REG_SPIHOLDN_MODE            0x52
#define REG_SPIHOLDN_MODE_MASK       BIT0
#define REG_PM_UART_IS_GPIO          0x65
#define REG_PM_UART_IS_GPIO_MASK     BIT4
// PADMUX MASK MARCO END

#define REG_I2C5_SCL_GPIO_MODE           0x00
#define REG_I2C5_SCL_GPIO_MODE_MASK      BIT3
#define REG_I2C5_SDA_GPIO_MODE           0x01
#define REG_I2C5_SDA_GPIO_MODE_MASK      BIT3
#define REG_SD0_GPIO0_GPIO_MODE          0x02
#define REG_SD0_GPIO0_GPIO_MODE_MASK     BIT3
#define REG_SD0_VCTRL_GPIO_MODE          0x03
#define REG_SD0_VCTRL_GPIO_MODE_MASK     BIT3
#define REG_SD0_CDZ_GPIO_MODE            0x04
#define REG_SD0_CDZ_GPIO_MODE_MASK       BIT3
#define REG_SD0_D1_GPIO_MODE             0x05
#define REG_SD0_D1_GPIO_MODE_MASK        BIT3
#define REG_SD0_D0_GPIO_MODE             0x06
#define REG_SD0_D0_GPIO_MODE_MASK        BIT3
#define REG_SD0_CLK_GPIO_MODE            0x07
#define REG_SD0_CLK_GPIO_MODE_MASK       BIT3
#define REG_SD0_CMD_GPIO_MODE            0x08
#define REG_SD0_CMD_GPIO_MODE_MASK       BIT3
#define REG_SD0_D3_GPIO_MODE             0x09
#define REG_SD0_D3_GPIO_MODE_MASK        BIT3
#define REG_SD0_D2_GPIO_MODE             0x0A
#define REG_SD0_D2_GPIO_MODE_MASK        BIT3
#define REG_KEY0_GPIO_MODE               0x0B
#define REG_KEY0_GPIO_MODE_MASK          BIT3
#define REG_KEY1_GPIO_MODE               0x0C
#define REG_KEY1_GPIO_MODE_MASK          BIT3
#define REG_KEY2_GPIO_MODE               0x0D
#define REG_KEY2_GPIO_MODE_MASK          BIT3
#define REG_KEY3_GPIO_MODE               0x0E
#define REG_KEY3_GPIO_MODE_MASK          BIT3
#define REG_KEY4_GPIO_MODE               0x0F
#define REG_KEY4_GPIO_MODE_MASK          BIT3
#define REG_KEY5_GPIO_MODE               0x10
#define REG_KEY5_GPIO_MODE_MASK          BIT3
#define REG_KEY6_GPIO_MODE               0x11
#define REG_KEY6_GPIO_MODE_MASK          BIT3
#define REG_KEY7_GPIO_MODE               0x12
#define REG_KEY7_GPIO_MODE_MASK          BIT3
#define REG_KEY8_GPIO_MODE               0x13
#define REG_KEY8_GPIO_MODE_MASK          BIT3
#define REG_KEY9_GPIO_MODE               0x14
#define REG_KEY9_GPIO_MODE_MASK          BIT3
#define REG_KEY10_GPIO_MODE              0x15
#define REG_KEY10_GPIO_MODE_MASK         BIT3
#define REG_KEY11_GPIO_MODE              0x16
#define REG_KEY11_GPIO_MODE_MASK         BIT3
#define REG_KEY12_GPIO_MODE              0x17
#define REG_KEY12_GPIO_MODE_MASK         BIT3
#define REG_KEY13_GPIO_MODE              0x18
#define REG_KEY13_GPIO_MODE_MASK         BIT3
#define REG_PM_PWM0_GPIO_MODE            0x19
#define REG_PM_PWM0_GPIO_MODE_MASK       BIT3
#define REG_PM_PWM1_GPIO_MODE            0x1A
#define REG_PM_PWM1_GPIO_MODE_MASK       BIT3
#define REG_PM_I2CM_SCL_GPIO_MODE        0x1B
#define REG_PM_I2CM_SCL_GPIO_MODE_MASK   BIT3
#define REG_PM_I2CM_SDA_GPIO_MODE        0x1C
#define REG_PM_I2CM_SDA_GPIO_MODE_MASK   BIT3
#define REG_PM_UART_RX0_GPIO_MODE        0x1D
#define REG_PM_UART_RX0_GPIO_MODE_MASK   BIT3
#define REG_PM_UART_TX0_GPIO_MODE        0x1E
#define REG_PM_UART_TX0_GPIO_MODE_MASK   BIT3
#define REG_PM_IR_RX_GPIO_MODE           0x1F
#define REG_PM_IR_RX_GPIO_MODE_MASK      BIT3
#define REG_PM_GPIO0_GPIO_MODE           0x20
#define REG_PM_GPIO0_GPIO_MODE_MASK      BIT3
#define REG_PM_GPIO1_GPIO_MODE           0x21
#define REG_PM_GPIO1_GPIO_MODE_MASK      BIT3
#define REG_PM_GPIO2_GPIO_MODE           0x22
#define REG_PM_GPIO2_GPIO_MODE_MASK      BIT3
#define REG_PM_GPIO3_GPIO_MODE           0x23
#define REG_PM_GPIO3_GPIO_MODE_MASK      BIT3
#define REG_PM_GPIO4_GPIO_MODE           0x24
#define REG_PM_GPIO4_GPIO_MODE_MASK      BIT3
#define REG_PM_GPIO5_GPIO_MODE           0x25
#define REG_PM_GPIO5_GPIO_MODE_MASK      BIT3
#define REG_PM_SPI_WPZ_GPIO_MODE         0x26
#define REG_PM_SPI_WPZ_GPIO_MODE_MASK    BIT3
#define REG_PM_SPI_DO_GPIO_MODE          0x27
#define REG_PM_SPI_DO_GPIO_MODE_MASK     BIT3
#define REG_PM_SPI_CZ_GPIO_MODE          0x28
#define REG_PM_SPI_CZ_GPIO_MODE_MASK     BIT3
#define REG_PM_SPI_HLD_GPIO_MODE         0x29
#define REG_PM_SPI_HLD_GPIO_MODE_MASK    BIT3
#define REG_PM_SPI_CK_GPIO_MODE          0x2A
#define REG_PM_SPI_CK_GPIO_MODE_MASK     BIT3
#define REG_PM_SPI_DI_GPIO_MODE          0x2B
#define REG_PM_SPI_DI_GPIO_MODE_MASK     BIT3
#define REG_EMMC_RSTN_GPIO_MODE          0x2C
#define REG_EMMC_RSTN_GPIO_MODE_MASK     BIT3
#define REG_EMMC_CLK_GPIO_MODE           0x2D
#define REG_EMMC_CLK_GPIO_MODE_MASK      BIT3
#define REG_EMMC_CMD_GPIO_MODE           0x2E
#define REG_EMMC_CMD_GPIO_MODE_MASK      BIT3
#define REG_EMMC_DS_GPIO_MODE            0x2F
#define REG_EMMC_DS_GPIO_MODE_MASK       BIT3
#define REG_EMMC_D3_GPIO_MODE            0x30
#define REG_EMMC_D3_GPIO_MODE_MASK       BIT3
#define REG_EMMC_D4_GPIO_MODE            0x31
#define REG_EMMC_D4_GPIO_MODE_MASK       BIT3
#define REG_EMMC_D0_GPIO_MODE            0x32
#define REG_EMMC_D0_GPIO_MODE_MASK       BIT3
#define REG_EMMC_D5_GPIO_MODE            0x33
#define REG_EMMC_D5_GPIO_MODE_MASK       BIT3
#define REG_EMMC_D1_GPIO_MODE            0x34
#define REG_EMMC_D1_GPIO_MODE_MASK       BIT3
#define REG_EMMC_D6_GPIO_MODE            0x35
#define REG_EMMC_D6_GPIO_MODE_MASK       BIT3
#define REG_EMMC_D2_GPIO_MODE            0x36
#define REG_EMMC_D2_GPIO_MODE_MASK       BIT3
#define REG_EMMC_D7_GPIO_MODE            0x37
#define REG_EMMC_D7_GPIO_MODE_MASK       BIT3
#define REG_OUTP_RX1_CH_0_GPIO_MODE      0x38
#define REG_OUTP_RX1_CH_0_GPIO_MODE_MASK BIT3
#define REG_OUTN_RX1_CH_0_GPIO_MODE      0x39
#define REG_OUTN_RX1_CH_0_GPIO_MODE_MASK BIT3
#define REG_OUTP_RX1_CH_1_GPIO_MODE      0x3A
#define REG_OUTP_RX1_CH_1_GPIO_MODE_MASK BIT3
#define REG_OUTN_RX1_CH_1_GPIO_MODE      0x3B
#define REG_OUTN_RX1_CH_1_GPIO_MODE_MASK BIT3
#define REG_OUTP_RX1_CH_2_GPIO_MODE      0x3C
#define REG_OUTP_RX1_CH_2_GPIO_MODE_MASK BIT3
#define REG_OUTN_RX1_CH_2_GPIO_MODE      0x3D
#define REG_OUTN_RX1_CH_2_GPIO_MODE_MASK BIT3
#define REG_OUTP_RX1_CH_3_GPIO_MODE      0x3E
#define REG_OUTP_RX1_CH_3_GPIO_MODE_MASK BIT3
#define REG_OUTN_RX1_CH_3_GPIO_MODE      0x3F
#define REG_OUTN_RX1_CH_3_GPIO_MODE_MASK BIT3
#define REG_OUTP_RX0_CH_0_GPIO_MODE      0x40
#define REG_OUTP_RX0_CH_0_GPIO_MODE_MASK BIT3
#define REG_OUTN_RX0_CH_0_GPIO_MODE      0x41
#define REG_OUTN_RX0_CH_0_GPIO_MODE_MASK BIT3
#define REG_OUTP_RX0_CH_1_GPIO_MODE      0x42
#define REG_OUTP_RX0_CH_1_GPIO_MODE_MASK BIT3
#define REG_OUTN_RX0_CH_1_GPIO_MODE      0x43
#define REG_OUTN_RX0_CH_1_GPIO_MODE_MASK BIT3
#define REG_OUTP_RX0_CH_2_GPIO_MODE      0x44
#define REG_OUTP_RX0_CH_2_GPIO_MODE_MASK BIT3
#define REG_OUTN_RX0_CH_2_GPIO_MODE      0x45
#define REG_OUTN_RX0_CH_2_GPIO_MODE_MASK BIT3
#define REG_OUTP_RX0_CH_3_GPIO_MODE      0x46
#define REG_OUTP_RX0_CH_3_GPIO_MODE_MASK BIT3
#define REG_OUTN_RX0_CH_3_GPIO_MODE      0x47
#define REG_OUTN_RX0_CH_3_GPIO_MODE_MASK BIT3
#define REG_OUTP_RX0_CH_4_GPIO_MODE      0x48
#define REG_OUTP_RX0_CH_4_GPIO_MODE_MASK BIT3
#define REG_OUTN_RX0_CH_4_GPIO_MODE      0x49
#define REG_OUTN_RX0_CH_4_GPIO_MODE_MASK BIT3
#define REG_OUTP_RX0_CH_5_GPIO_MODE      0x4A
#define REG_OUTP_RX0_CH_5_GPIO_MODE_MASK BIT3
#define REG_OUTN_RX0_CH_5_GPIO_MODE      0x4B
#define REG_OUTN_RX0_CH_5_GPIO_MODE_MASK BIT3
#define REG_SPDIF_TX_GPIO_MODE           0x4C
#define REG_SPDIF_TX_GPIO_MODE_MASK      BIT3
#define REG_SR_IO3_GPIO_MODE             0x4D
#define REG_SR_IO3_GPIO_MODE_MASK        BIT3
#define REG_SR_IO2_GPIO_MODE             0x4E
#define REG_SR_IO2_GPIO_MODE_MASK        BIT3
#define REG_SR_IO1_GPIO_MODE             0x4F
#define REG_SR_IO1_GPIO_MODE_MASK        BIT3
#define REG_SR_IO0_GPIO_MODE             0x50
#define REG_SR_IO0_GPIO_MODE_MASK        BIT3
#define REG_SR_PDN1_GPIO_MODE            0x51
#define REG_SR_PDN1_GPIO_MODE_MASK       BIT3
#define REG_SR_MCLK1_GPIO_MODE           0x52
#define REG_SR_MCLK1_GPIO_MODE_MASK      BIT3
#define REG_SR_RST1_GPIO_MODE            0x53
#define REG_SR_RST1_GPIO_MODE_MASK       BIT3
#define REG_I2C1_SDA_GPIO_MODE           0x54
#define REG_I2C1_SDA_GPIO_MODE_MASK      BIT3
#define REG_I2C1_SCL_GPIO_MODE           0x55
#define REG_I2C1_SCL_GPIO_MODE_MASK      BIT3
#define REG_SR_PDN0_GPIO_MODE            0x56
#define REG_SR_PDN0_GPIO_MODE_MASK       BIT3
#define REG_SR_MCLK0_GPIO_MODE           0x57
#define REG_SR_MCLK0_GPIO_MODE_MASK      BIT3
#define REG_SR_RST0_GPIO_MODE            0x58
#define REG_SR_RST0_GPIO_MODE_MASK       BIT3
#define REG_I2C0_SDA_GPIO_MODE           0x59
#define REG_I2C0_SDA_GPIO_MODE_MASK      BIT3
#define REG_I2C0_SCL_GPIO_MODE           0x5A
#define REG_I2C0_SCL_GPIO_MODE_MASK      BIT3
#define REG_SR_PDN2_GPIO_MODE            0x5B
#define REG_SR_PDN2_GPIO_MODE_MASK       BIT3
#define REG_SR_MCLK2_GPIO_MODE           0x5C
#define REG_SR_MCLK2_GPIO_MODE_MASK      BIT3
#define REG_SR_RST2_GPIO_MODE            0x5D
#define REG_SR_RST2_GPIO_MODE_MASK       BIT3
#define REG_I2C2_SDA_GPIO_MODE           0x5E
#define REG_I2C2_SDA_GPIO_MODE_MASK      BIT3
#define REG_I2C2_SCL_GPIO_MODE           0x5F
#define REG_I2C2_SCL_GPIO_MODE_MASK      BIT3
#define REG_SR_PDN3_GPIO_MODE            0x60
#define REG_SR_PDN3_GPIO_MODE_MASK       BIT3
#define REG_SR_MCLK3_GPIO_MODE           0x61
#define REG_SR_MCLK3_GPIO_MODE_MASK      BIT3
#define REG_SR_RST3_GPIO_MODE            0x62
#define REG_SR_RST3_GPIO_MODE_MASK       BIT3
#define REG_I2C3_SDA_GPIO_MODE           0x63
#define REG_I2C3_SDA_GPIO_MODE_MASK      BIT3
#define REG_I2C3_SCL_GPIO_MODE           0x64
#define REG_I2C3_SCL_GPIO_MODE_MASK      BIT3
#define REG_PWM_OUT0_GPIO_MODE           0x65
#define REG_PWM_OUT0_GPIO_MODE_MASK      BIT3
#define REG_PWM_OUT1_GPIO_MODE           0x66
#define REG_PWM_OUT1_GPIO_MODE_MASK      BIT3
#define REG_PWM_OUT2_GPIO_MODE           0x67
#define REG_PWM_OUT2_GPIO_MODE_MASK      BIT3
#define REG_PWM_OUT3_GPIO_MODE           0x68
#define REG_PWM_OUT3_GPIO_MODE_MASK      BIT3
#define REG_PWM_OUT4_GPIO_MODE           0x69
#define REG_PWM_OUT4_GPIO_MODE_MASK      BIT3
#define REG_PWM_OUT5_GPIO_MODE           0x6A
#define REG_PWM_OUT5_GPIO_MODE_MASK      BIT3
#define REG_PWM_OUT6_GPIO_MODE           0x6B
#define REG_PWM_OUT6_GPIO_MODE_MASK      BIT3
#define REG_PWM_OUT7_GPIO_MODE           0x6C
#define REG_PWM_OUT7_GPIO_MODE_MASK      BIT3
#define REG_PWM_OUT8_GPIO_MODE           0x6D
#define REG_PWM_OUT8_GPIO_MODE_MASK      BIT3
#define REG_PWM_OUT9_GPIO_MODE           0x6E
#define REG_PWM_OUT9_GPIO_MODE_MASK      BIT3
#define REG_PWM_OUT10_GPIO_MODE          0x6F
#define REG_PWM_OUT10_GPIO_MODE_MASK     BIT3
#define REG_PWM_OUT11_GPIO_MODE          0x70
#define REG_PWM_OUT11_GPIO_MODE_MASK     BIT3
#define REG_OUTP_TX0_CH_0_GPIO_MODE      0x71
#define REG_OUTP_TX0_CH_0_GPIO_MODE_MASK BIT3
#define REG_OUTN_TX0_CH_0_GPIO_MODE      0x72
#define REG_OUTN_TX0_CH_0_GPIO_MODE_MASK BIT3
#define REG_OUTP_TX0_CH_1_GPIO_MODE      0x73
#define REG_OUTP_TX0_CH_1_GPIO_MODE_MASK BIT3
#define REG_OUTN_TX0_CH_1_GPIO_MODE      0x74
#define REG_OUTN_TX0_CH_1_GPIO_MODE_MASK BIT3
#define REG_OUTP_TX0_CH_2_GPIO_MODE      0x75
#define REG_OUTP_TX0_CH_2_GPIO_MODE_MASK BIT3
#define REG_OUTN_TX0_CH_2_GPIO_MODE      0x76
#define REG_OUTN_TX0_CH_2_GPIO_MODE_MASK BIT3
#define REG_OUTP_TX0_CH_3_GPIO_MODE      0x77
#define REG_OUTP_TX0_CH_3_GPIO_MODE_MASK BIT3
#define REG_OUTN_TX0_CH_3_GPIO_MODE      0x78
#define REG_OUTN_TX0_CH_3_GPIO_MODE_MASK BIT3
#define REG_OUTP_TX0_CH_4_GPIO_MODE      0x79
#define REG_OUTP_TX0_CH_4_GPIO_MODE_MASK BIT3
#define REG_OUTN_TX0_CH_4_GPIO_MODE      0x7A
#define REG_OUTN_TX0_CH_4_GPIO_MODE_MASK BIT3
#define REG_OUTP_TX1_CH_0_GPIO_MODE      0x7B
#define REG_OUTP_TX1_CH_0_GPIO_MODE_MASK BIT3
#define REG_OUTN_TX1_CH_0_GPIO_MODE      0x7C
#define REG_OUTN_TX1_CH_0_GPIO_MODE_MASK BIT3
#define REG_OUTP_TX1_CH_1_GPIO_MODE      0x7D
#define REG_OUTP_TX1_CH_1_GPIO_MODE_MASK BIT3
#define REG_OUTN_TX1_CH_1_GPIO_MODE      0x7E
#define REG_OUTN_TX1_CH_1_GPIO_MODE_MASK BIT3
#define REG_OUTP_TX1_CH_2_GPIO_MODE      0x7F
#define REG_OUTP_TX1_CH_2_GPIO_MODE_MASK BIT3
#define REG_OUTN_TX1_CH_2_GPIO_MODE      0x00
#define REG_OUTN_TX1_CH_2_GPIO_MODE_MASK BIT3
#define REG_OUTP_TX1_CH_3_GPIO_MODE      0x01
#define REG_OUTP_TX1_CH_3_GPIO_MODE_MASK BIT3
#define REG_OUTN_TX1_CH_3_GPIO_MODE      0x02
#define REG_OUTN_TX1_CH_3_GPIO_MODE_MASK BIT3
#define REG_OUTP_TX1_CH_4_GPIO_MODE      0x03
#define REG_OUTP_TX1_CH_4_GPIO_MODE_MASK BIT3
#define REG_OUTN_TX1_CH_4_GPIO_MODE      0x04
#define REG_OUTN_TX1_CH_4_GPIO_MODE_MASK BIT3
#define REG_SAR_ADC_0_GPIO_MODE          0x05
#define REG_SAR_ADC_0_GPIO_MODE_MASK     BIT3
#define REG_SAR_ADC_1_GPIO_MODE          0x06
#define REG_SAR_ADC_1_GPIO_MODE_MASK     BIT3
#define REG_SAR_ADC_2_GPIO_MODE          0x07
#define REG_SAR_ADC_2_GPIO_MODE_MASK     BIT3
#define REG_SAR_ADC_3_GPIO_MODE          0x08
#define REG_SAR_ADC_3_GPIO_MODE_MASK     BIT3
#define REG_SAR_ADC_4_GPIO_MODE          0x09
#define REG_SAR_ADC_4_GPIO_MODE_MASK     BIT3
#define REG_SAR_ADC_5_GPIO_MODE          0x0A
#define REG_SAR_ADC_5_GPIO_MODE_MASK     BIT3
#define REG_SAR_ADC_6_GPIO_MODE          0x0B
#define REG_SAR_ADC_6_GPIO_MODE_MASK     BIT3
#define REG_SAR_ADC_7_GPIO_MODE          0x0C
#define REG_SAR_ADC_7_GPIO_MODE_MASK     BIT3
#define REG_SAR_ADC_8_GPIO_MODE          0x0D
#define REG_SAR_ADC_8_GPIO_MODE_MASK     BIT3
#define REG_SAR_ADC_9_GPIO_MODE          0x0E
#define REG_SAR_ADC_9_GPIO_MODE_MASK     BIT3
#define REG_SAR_ADC_10_GPIO_MODE         0x0F
#define REG_SAR_ADC_10_GPIO_MODE_MASK    BIT3
#define REG_SAR_ADC_11_GPIO_MODE         0x10
#define REG_SAR_ADC_11_GPIO_MODE_MASK    BIT3
#define REG_SAR_ADC_12_GPIO_MODE         0x11
#define REG_SAR_ADC_12_GPIO_MODE_MASK    BIT3
#define REG_SAR_ADC_13_GPIO_MODE         0x12
#define REG_SAR_ADC_13_GPIO_MODE_MASK    BIT3
#define REG_SAR_ADC_14_GPIO_MODE         0x13
#define REG_SAR_ADC_14_GPIO_MODE_MASK    BIT3
#define REG_SAR_ADC_15_GPIO_MODE         0x14
#define REG_SAR_ADC_15_GPIO_MODE_MASK    BIT3
#define REG_SAR_ADC_16_GPIO_MODE         0x15
#define REG_SAR_ADC_16_GPIO_MODE_MASK    BIT3
#define REG_SAR_ADC_17_GPIO_MODE         0x16
#define REG_SAR_ADC_17_GPIO_MODE_MASK    BIT3
#define REG_SAR_ADC_18_GPIO_MODE         0x17
#define REG_SAR_ADC_18_GPIO_MODE_MASK    BIT3
#define REG_SAR_ADC_19_GPIO_MODE         0x18
#define REG_SAR_ADC_19_GPIO_MODE_MASK    BIT3
#define REG_SAR_ADC_20_GPIO_MODE         0x19
#define REG_SAR_ADC_20_GPIO_MODE_MASK    BIT3
#define REG_SAR_ADC_21_GPIO_MODE         0x1A
#define REG_SAR_ADC_21_GPIO_MODE_MASK    BIT3
#define REG_SAR_ADC_22_GPIO_MODE         0x1B
#define REG_SAR_ADC_22_GPIO_MODE_MASK    BIT3
#define REG_SAR_ADC_23_GPIO_MODE         0x1C
#define REG_SAR_ADC_23_GPIO_MODE_MASK    BIT3
#define REG_SR_IO4_GPIO_MODE             0x1D
#define REG_SR_IO4_GPIO_MODE_MASK        BIT3
#define REG_RGMII0_MCLK_GPIO_MODE        0x25
#define REG_RGMII0_MCLK_GPIO_MODE_MASK   BIT3
#define REG_RGMII0_RSTN_GPIO_MODE        0x26
#define REG_RGMII0_RSTN_GPIO_MODE_MASK   BIT3
#define REG_RGMII0_RXCLK_GPIO_MODE       0x27
#define REG_RGMII0_RXCLK_GPIO_MODE_MASK  BIT3
#define REG_RGMII0_RXCTL_GPIO_MODE       0x28
#define REG_RGMII0_RXCTL_GPIO_MODE_MASK  BIT3
#define REG_RGMII0_RXD0_GPIO_MODE        0x29
#define REG_RGMII0_RXD0_GPIO_MODE_MASK   BIT3
#define REG_RGMII0_RXD1_GPIO_MODE        0x2A
#define REG_RGMII0_RXD1_GPIO_MODE_MASK   BIT3
#define REG_RGMII0_RXD2_GPIO_MODE        0x2B
#define REG_RGMII0_RXD2_GPIO_MODE_MASK   BIT3
#define REG_RGMII0_RXD3_GPIO_MODE        0x2C
#define REG_RGMII0_RXD3_GPIO_MODE_MASK   BIT3
#define REG_RGMII0_TXCLK_GPIO_MODE       0x2D
#define REG_RGMII0_TXCLK_GPIO_MODE_MASK  BIT3
#define REG_RGMII0_TXCTL_GPIO_MODE       0x2E
#define REG_RGMII0_TXCTL_GPIO_MODE_MASK  BIT3
#define REG_RGMII0_TXD0_GPIO_MODE        0x2F
#define REG_RGMII0_TXD0_GPIO_MODE_MASK   BIT3
#define REG_RGMII0_TXD1_GPIO_MODE        0x30
#define REG_RGMII0_TXD1_GPIO_MODE_MASK   BIT3
#define REG_RGMII0_TXD2_GPIO_MODE        0x31
#define REG_RGMII0_TXD2_GPIO_MODE_MASK   BIT3
#define REG_RGMII0_TXD3_GPIO_MODE        0x32
#define REG_RGMII0_TXD3_GPIO_MODE_MASK   BIT3
#define REG_RGMII0_MDIO_GPIO_MODE        0x33
#define REG_RGMII0_MDIO_GPIO_MODE_MASK   BIT3
#define REG_RGMII0_MDC_GPIO_MODE         0x34
#define REG_RGMII0_MDC_GPIO_MODE_MASK    BIT3
#define REG_UART_RX2_GPIO_MODE           0x35
#define REG_UART_RX2_GPIO_MODE_MASK      BIT3
#define REG_UART_TX2_GPIO_MODE           0x36
#define REG_UART_TX2_GPIO_MODE_MASK      BIT3
#define REG_UART_RX3_GPIO_MODE           0x37
#define REG_UART_RX3_GPIO_MODE_MASK      BIT3
#define REG_UART_TX3_GPIO_MODE           0x38
#define REG_UART_TX3_GPIO_MODE_MASK      BIT3
#define REG_UART_RX4_GPIO_MODE           0x39
#define REG_UART_RX4_GPIO_MODE_MASK      BIT3
#define REG_UART_TX4_GPIO_MODE           0x3A
#define REG_UART_TX4_GPIO_MODE_MASK      BIT3
#define REG_UART_RX1_GPIO_MODE           0x3B
#define REG_UART_RX1_GPIO_MODE_MASK      BIT3
#define REG_UART_TX1_GPIO_MODE           0x3C
#define REG_UART_TX1_GPIO_MODE_MASK      BIT3
#define REG_FUART_RX_GPIO_MODE           0x3D
#define REG_FUART_RX_GPIO_MODE_MASK      BIT3
#define REG_FUART_TX_GPIO_MODE           0x3E
#define REG_FUART_TX_GPIO_MODE_MASK      BIT3
#define REG_FUART_RTS_GPIO_MODE          0x3F
#define REG_FUART_RTS_GPIO_MODE_MASK     BIT3
#define REG_FUART_CTS_GPIO_MODE          0x40
#define REG_FUART_CTS_GPIO_MODE_MASK     BIT3
/* for misc pad which must be added by hand start */

/* EMAC : ALBANY1_BANK */
#define REG_ATOP_RX_INOFF      0x69
#define REG_ATOP_RX_INOFF_MASK BIT14

/* EMAC : ALBANY2_BANK */
#define REG_ETH_GPIO_EN         0x71
#define REG_ETH_GPIO_EN_RN_MASK BIT0
#define REG_ETH_GPIO_EN_RP_MASK BIT1
#define REG_ETH_GPIO_EN_TN_MASK BIT2
#define REG_ETH_GPIO_EN_TP_MASK BIT3

/* UTMI0 : UTMI0_BANK */
#define REG_UTMI0_FL_XVR_PDN         0x00
#define REG_UTMI0_FL_XVR_PDN_MASK    BIT12
#define REG_UTMI0_REG_PDN            0x00
#define REG_UTMI0_REG_PDN_MASK       BIT15 // 1: power doen 0: enable
#define REG_UTMI0_CLK_EXTRA0_EN      0x04
#define REG_UTMI0_CLK_EXTRA0_EN_MASK BIT7 // 1: power down 0: enable
#define REG_UTMI0_GPIO_EN            0x1f
#define REG_UTMI0_GPIO_EN_MASK       BIT14

/* UTMI1 : UTMI1_BANK */
#define REG_UTMI1_FL_XVR_PDN         0x00
#define REG_UTMI1_FL_XVR_PDN_MASK    BIT12
#define REG_UTMI1_REG_PDN            0x00
#define REG_UTMI1_REG_PDN_MASK       BIT15 // 1: power doen 0: enable
#define REG_UTMI1_CLK_EXTRA0_EN      0x04
#define REG_UTMI1_CLK_EXTRA0_EN_MASK BIT7 // 1: power down 0: enable
#define REG_UTMI1_GPIO_EN            0x1f
#define REG_UTMI1_GPIO_EN_MASK       BIT14

/* for misc pad which must be added by hand end */

//-------------------- configuration -----------------
#define ENABLE_CHECK_ALL_PAD_CONFLICT 1

//==============================================================================
//
//                              STRUCTURE
//
//==============================================================================

typedef struct stPadMux
{
    U16 padID;
    U32 base;
    U16 offset;
    U16 mask;
    U16 val;
    U16 mode;
} ST_PadMuxInfo;

typedef struct stPadMode
{
    U8  u8PadName[32];
    U32 u32ModeRIU;
    U16 u16ModeMask;
    U16 u16ModeVal;
} ST_PadModeInfo;

typedef struct stPadCheck
{
    U16 base;
    U16 offset;
    U16 mask;
    U16 val;
    U16 regval;
} ST_PadCheckInfo;

typedef struct stPadCheckVal
{
    U8                infocount;
    struct stPadCheck infos[64];
} ST_PadCheckVal;

typedef struct stPadMuxEntry
{
    U32                  size;
    const ST_PadMuxInfo* padmux;
} ST_PadMuxEntry;

static S32 Pad_Mode[GPIO_NR] = {[0 ... GPIO_NR - 1] = PINMUX_FOR_UNKNOWN_MODE};
//==============================================================================
//
//                              VARIABLES
//
//==============================================================================

ST_PadCheckVal m_stPadCheckVal;

const ST_PadMuxInfo i2c5_scl_tbl[] = {
    {PAD_I2C5_SCL, PADGPIO_BANK, REG_I2C5_SCL_GPIO_MODE, REG_I2C5_SCL_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_I2C5_SCL, PADTOP_BANK, REG_I2C5_MODE, REG_I2C5_MODE_MASK, BIT8, PINMUX_FOR_I2C5_MODE_1},
    {PAD_I2C5_SCL, PADTOP_BANK, REG_UART0_MODE, REG_UART0_MODE_MASK, BIT1, PINMUX_FOR_UART0_MODE_2},
    {PAD_I2C5_SCL, PADTOP_BANK, REG_PWM_IN0_MODE, REG_PWM_IN0_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_PWM_IN0_MODE_3},
};
const ST_PadMuxInfo i2c5_sda_tbl[] = {
    {PAD_I2C5_SDA, PADGPIO_BANK, REG_I2C5_SDA_GPIO_MODE, REG_I2C5_SDA_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_I2C5_SDA, PADTOP_BANK, REG_I2C5_MODE, REG_I2C5_MODE_MASK, BIT8, PINMUX_FOR_I2C5_MODE_1},
    {PAD_I2C5_SDA, PADTOP_BANK, REG_UART0_MODE, REG_UART0_MODE_MASK, BIT1, PINMUX_FOR_UART0_MODE_2},
    {PAD_I2C5_SDA, PADTOP_BANK, REG_PWM_IN1_MODE, REG_PWM_IN1_MODE_MASK, BIT3 | BIT2, PINMUX_FOR_PWM_IN1_MODE_3},
};
const ST_PadMuxInfo sd0_gpio0_tbl[] = {
    {PAD_SD0_GPIO0, PADGPIO_BANK, REG_SD0_GPIO0_GPIO_MODE, REG_SD0_GPIO0_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SD0_GPIO0, PADTOP_BANK, REG_UART5_MODE, REG_UART5_MODE_MASK, BIT6 | BIT4, PINMUX_FOR_UART5_MODE_5},
    {PAD_SD0_GPIO0, PADTOP_BANK, REG_SD0_BOOT_MODE, REG_SD0_BOOT_MODE_MASK, BIT0, PINMUX_FOR_SD0_BOOT_MODE_1},
    {PAD_SD0_GPIO0, PADTOP_BANK, REG_PWM_OUT0_MODE, REG_PWM_OUT0_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_PWM_OUT0_MODE_3},
    {PAD_SD0_GPIO0, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo sd0_vctrl_tbl[] = {
    {PAD_SD0_VCTRL, PADGPIO_BANK, REG_SD0_VCTRL_GPIO_MODE, REG_SD0_VCTRL_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SD0_VCTRL, PADTOP_BANK, REG_UART5_MODE, REG_UART5_MODE_MASK, BIT6 | BIT4, PINMUX_FOR_UART5_MODE_5},
    {PAD_SD0_VCTRL, PADTOP_BANK, REG_SD0_BOOT_MODE, REG_SD0_BOOT_MODE_MASK, BIT0, PINMUX_FOR_SD0_BOOT_MODE_1},
    {PAD_SD0_VCTRL, PADTOP_BANK, REG_PWM_OUT1_MODE, REG_PWM_OUT1_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_PWM_OUT1_MODE_3},
    {PAD_SD0_VCTRL, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo sd0_cdz_tbl[] = {
    {PAD_SD0_CDZ, PADGPIO_BANK, REG_SD0_CDZ_GPIO_MODE, REG_SD0_CDZ_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SD0_CDZ, PADTOP_BANK, REG_SPI0_CZ1_MODE, REG_SPI0_CZ1_MODE_MASK, BIT4, PINMUX_FOR_SPI0_CZ1_MODE_1},
    {PAD_SD0_CDZ, PADTOP_BANK, REG_SD0_BOOT_MODE, REG_SD0_BOOT_MODE_MASK, BIT0, PINMUX_FOR_SD0_BOOT_MODE_1},
    {PAD_SD0_CDZ, PADTOP_BANK, REG_SD0_CDZ_MODE, REG_SD0_CDZ_MODE_MASK, BIT10, PINMUX_FOR_SD0_CDZ_MODE_1},
    {PAD_SD0_CDZ, PADTOP_BANK, REG_SD0_RSTN_MODE, REG_SD0_RSTN_MODE_MASK, BIT0, PINMUX_FOR_SD0_RSTN_MODE_1},
    {PAD_SD0_CDZ, PADTOP_BANK, REG_PWM_OUT2_MODE, REG_PWM_OUT2_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_PWM_OUT2_MODE_3},
    {PAD_SD0_CDZ, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo sd0_d1_tbl[] = {
    {PAD_SD0_D1, PADGPIO_BANK, REG_SD0_D1_GPIO_MODE, REG_SD0_D1_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SD0_D1, PADTOP_BANK, REG_SPI0_MODE, REG_SPI0_MODE_MASK, BIT0, PINMUX_FOR_SPI0_MODE_1},
    {PAD_SD0_D1, PADTOP_BANK, REG_SD0_BOOT_MODE, REG_SD0_BOOT_MODE_MASK, BIT0, PINMUX_FOR_SD0_BOOT_MODE_1},
    {PAD_SD0_D1, PADTOP_BANK, REG_SD0_MODE, REG_SD0_MODE_MASK, BIT8, PINMUX_FOR_SD0_MODE_1},
    {PAD_SD0_D1, PADTOP_BANK, REG_PWM_OUT3_MODE, REG_PWM_OUT3_MODE_MASK, BIT13 | BIT12, PINMUX_FOR_PWM_OUT3_MODE_3},
    {PAD_SD0_D1, PADTOP_BANK, REG_PSPI1_TE_MODE, REG_PSPI1_TE_MODE_MASK, BIT13, PINMUX_FOR_PSPI1_TE_MODE_2},
    {PAD_SD0_D1, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo sd0_d0_tbl[] = {
    {PAD_SD0_D0, PADGPIO_BANK, REG_SD0_D0_GPIO_MODE, REG_SD0_D0_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SD0_D0, PADTOP_BANK, REG_SPI0_MODE, REG_SPI0_MODE_MASK, BIT0, PINMUX_FOR_SPI0_MODE_1},
    {PAD_SD0_D0, PADTOP_BANK, REG_SD0_BOOT_MODE, REG_SD0_BOOT_MODE_MASK, BIT0, PINMUX_FOR_SD0_BOOT_MODE_1},
    {PAD_SD0_D0, PADTOP_BANK, REG_SD0_MODE, REG_SD0_MODE_MASK, BIT8, PINMUX_FOR_SD0_MODE_1},
    {PAD_SD0_D0, PADTOP_BANK, REG_PWM_OUT4_MODE, REG_PWM_OUT4_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_PWM_OUT4_MODE_3},
    {PAD_SD0_D0, PADTOP_BANK, REG_PSPI1_PANEL_MODE, REG_PSPI1_PANEL_MODE_MASK, BIT5, PINMUX_FOR_PSPI1_PANEL_MODE_2},
    {PAD_SD0_D0, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo sd0_clk_tbl[] = {
    {PAD_SD0_CLK, PADGPIO_BANK, REG_SD0_CLK_GPIO_MODE, REG_SD0_CLK_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SD0_CLK, PADTOP_BANK, REG_SPI0_MODE, REG_SPI0_MODE_MASK, BIT0, PINMUX_FOR_SPI0_MODE_1},
    {PAD_SD0_CLK, PADTOP_BANK, REG_SD0_BOOT_MODE, REG_SD0_BOOT_MODE_MASK, BIT0, PINMUX_FOR_SD0_BOOT_MODE_1},
    {PAD_SD0_CLK, PADTOP_BANK, REG_SD0_MODE, REG_SD0_MODE_MASK, BIT8, PINMUX_FOR_SD0_MODE_1},
    {PAD_SD0_CLK, PADTOP_BANK, REG_PWM_OUT5_MODE, REG_PWM_OUT5_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_PWM_OUT5_MODE_3},
    {PAD_SD0_CLK, PADTOP_BANK, REG_PSPI1_PANEL_MODE, REG_PSPI1_PANEL_MODE_MASK, BIT5, PINMUX_FOR_PSPI1_PANEL_MODE_2},
    {PAD_SD0_CLK, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo sd0_cmd_tbl[] = {
    {PAD_SD0_CMD, PADGPIO_BANK, REG_SD0_CMD_GPIO_MODE, REG_SD0_CMD_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SD0_CMD, PADTOP_BANK, REG_SPI0_MODE, REG_SPI0_MODE_MASK, BIT0, PINMUX_FOR_SPI0_MODE_1},
    {PAD_SD0_CMD, PADTOP_BANK, REG_SD0_BOOT_MODE, REG_SD0_BOOT_MODE_MASK, BIT0, PINMUX_FOR_SD0_BOOT_MODE_1},
    {PAD_SD0_CMD, PADTOP_BANK, REG_SD0_MODE, REG_SD0_MODE_MASK, BIT8, PINMUX_FOR_SD0_MODE_1},
    {PAD_SD0_CMD, PADTOP_BANK, REG_PWM_OUT6_MODE, REG_PWM_OUT6_MODE_MASK, BIT11 | BIT10, PINMUX_FOR_PWM_OUT6_MODE_3},
    {PAD_SD0_CMD, PADTOP_BANK, REG_PSPI1_PANEL_MODE, REG_PSPI1_PANEL_MODE_MASK, BIT5, PINMUX_FOR_PSPI1_PANEL_MODE_2},
};
const ST_PadMuxInfo sd0_d3_tbl[] = {
    {PAD_SD0_D3, PADGPIO_BANK, REG_SD0_D3_GPIO_MODE, REG_SD0_D3_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SD0_D3, PADTOP_BANK, REG_I2C1_MODE, REG_I2C1_MODE_MASK, BIT1, PINMUX_FOR_I2C1_MODE_2},
    {PAD_SD0_D3, PADTOP_BANK, REG_SD0_BOOT_MODE, REG_SD0_BOOT_MODE_MASK, BIT0, PINMUX_FOR_SD0_BOOT_MODE_1},
    {PAD_SD0_D3, PADTOP_BANK, REG_SD0_MODE, REG_SD0_MODE_MASK, BIT8, PINMUX_FOR_SD0_MODE_1},
    {PAD_SD0_D3, PADTOP_BANK, REG_PWM_OUT7_MODE, REG_PWM_OUT7_MODE_MASK, BIT13 | BIT12, PINMUX_FOR_PWM_OUT7_MODE_3},
    {PAD_SD0_D3, PADTOP_BANK, REG_PSPI1_PANEL_MODE, REG_PSPI1_PANEL_MODE_MASK, BIT5, PINMUX_FOR_PSPI1_PANEL_MODE_2},
};
const ST_PadMuxInfo sd0_d2_tbl[] = {
    {PAD_SD0_D2, PADGPIO_BANK, REG_SD0_D2_GPIO_MODE, REG_SD0_D2_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SD0_D2, PADTOP_BANK, REG_I2C1_MODE, REG_I2C1_MODE_MASK, BIT1, PINMUX_FOR_I2C1_MODE_2},
    {PAD_SD0_D2, PADTOP_BANK, REG_SD0_BOOT_MODE, REG_SD0_BOOT_MODE_MASK, BIT0, PINMUX_FOR_SD0_BOOT_MODE_1},
    {PAD_SD0_D2, PADTOP_BANK, REG_SD0_MODE, REG_SD0_MODE_MASK, BIT8, PINMUX_FOR_SD0_MODE_1},
    {PAD_SD0_D2, PADTOP_BANK, REG_PWM_OUT8_MODE, REG_PWM_OUT8_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_PWM_OUT8_MODE_3},
    {PAD_SD0_D2, PADTOP_BANK, REG_PSPI1_CS2_MODE, REG_PSPI1_CS2_MODE_MASK, BIT11, PINMUX_FOR_PSPI1_CS2_MODE_2},
};
const ST_PadMuxInfo key0_tbl[] = {
    {PAD_KEY0, PADGPIO_BANK, REG_KEY0_GPIO_MODE, REG_KEY0_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_KEY0, PADTOP_BANK, REG_PWM_IN0_MODE, REG_PWM_IN0_MODE_MASK, BIT0, PINMUX_FOR_PWM_IN0_MODE_1},
    {PAD_KEY0, PADTOP_BANK, REG_I2S0_MCK_MODE, REG_I2S0_MCK_MODE_MASK, BIT0, PINMUX_FOR_I2S0_MCK_MODE_1},
    {PAD_KEY0, PADTOP_BANK, REG_DMIC_2CH_MODE, REG_DMIC_2CH_MODE_MASK, BIT0, PINMUX_FOR_DMIC_2CH_MODE_1},
    {PAD_KEY0, PADTOP_BANK, REG_DMIC_4CH_MODE, REG_DMIC_4CH_MODE_MASK, BIT0, PINMUX_FOR_DMIC_4CH_MODE_1},
    {PAD_KEY0, PADTOP_BANK, REG_DMIC_8CH_MODE, REG_DMIC_8CH_MODE_MASK, BIT4, PINMUX_FOR_DMIC_8CH_MODE_1},
    {PAD_KEY0, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT8, PINMUX_FOR_KEY_FIX_MODE_1},
    {PAD_KEY0, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_KEY_FIX_MODE_3},
    {PAD_KEY0, PADTOP_BANK, REG_SPDIF_MODE, REG_SPDIF_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_SPDIF_MODE_3},
    {PAD_KEY0, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo key1_tbl[] = {
    {PAD_KEY1, PADGPIO_BANK, REG_KEY1_GPIO_MODE, REG_KEY1_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_KEY1, PADTOP_BANK, REG_PWM_IN1_MODE, REG_PWM_IN1_MODE_MASK, BIT2, PINMUX_FOR_PWM_IN1_MODE_1},
    {PAD_KEY1, PADTOP_BANK, REG_I2S0_RX_MODE, REG_I2S0_RX_MODE_MASK, BIT8, PINMUX_FOR_I2S0_RX_MODE_1},
    {PAD_KEY1, PADTOP_BANK, REG_I2S0_RXTX_MODE, REG_I2S0_RXTX_MODE_MASK, BIT0, PINMUX_FOR_I2S0_RXTX_MODE_1},
    {PAD_KEY1, PADTOP_BANK, REG_DMIC_2CH_MODE, REG_DMIC_2CH_MODE_MASK, BIT0, PINMUX_FOR_DMIC_2CH_MODE_1},
    {PAD_KEY1, PADTOP_BANK, REG_DMIC_4CH_MODE, REG_DMIC_4CH_MODE_MASK, BIT0, PINMUX_FOR_DMIC_4CH_MODE_1},
    {PAD_KEY1, PADTOP_BANK, REG_DMIC_8CH_MODE, REG_DMIC_8CH_MODE_MASK, BIT4, PINMUX_FOR_DMIC_8CH_MODE_1},
    {PAD_KEY1, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT8, PINMUX_FOR_KEY_FIX_MODE_1},
    {PAD_KEY1, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_KEY_FIX_MODE_3},
    {PAD_KEY1, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo key2_tbl[] = {
    {PAD_KEY2, PADGPIO_BANK, REG_KEY2_GPIO_MODE, REG_KEY2_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_KEY2, PADTOP_BANK, REG_PWM_IN2_MODE, REG_PWM_IN2_MODE_MASK, BIT4, PINMUX_FOR_PWM_IN2_MODE_1},
    {PAD_KEY2, PADTOP_BANK, REG_I2S0_RX_MODE, REG_I2S0_RX_MODE_MASK, BIT8, PINMUX_FOR_I2S0_RX_MODE_1},
    {PAD_KEY2, PADTOP_BANK, REG_I2S0_RXTX_MODE, REG_I2S0_RXTX_MODE_MASK, BIT0, PINMUX_FOR_I2S0_RXTX_MODE_1},
    {PAD_KEY2, PADTOP_BANK, REG_DMIC_4CH_MODE, REG_DMIC_4CH_MODE_MASK, BIT0, PINMUX_FOR_DMIC_4CH_MODE_1},
    {PAD_KEY2, PADTOP_BANK, REG_DMIC_8CH_MODE, REG_DMIC_8CH_MODE_MASK, BIT4, PINMUX_FOR_DMIC_8CH_MODE_1},
    {PAD_KEY2, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT8, PINMUX_FOR_KEY_FIX_MODE_1},
    {PAD_KEY2, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_KEY_FIX_MODE_3},
    {PAD_KEY2, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo key3_tbl[] = {
    {PAD_KEY3, PADGPIO_BANK, REG_KEY3_GPIO_MODE, REG_KEY3_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_KEY3, PADTOP_BANK, REG_PWM_IN3_MODE, REG_PWM_IN3_MODE_MASK, BIT6, PINMUX_FOR_PWM_IN3_MODE_1},
    {PAD_KEY3, PADTOP_BANK, REG_I2S0_RX_MODE, REG_I2S0_RX_MODE_MASK, BIT8, PINMUX_FOR_I2S0_RX_MODE_1},
    {PAD_KEY3, PADTOP_BANK, REG_I2S0_RXTX_MODE, REG_I2S0_RXTX_MODE_MASK, BIT0, PINMUX_FOR_I2S0_RXTX_MODE_1},
    {PAD_KEY3, PADTOP_BANK, REG_DMIC_8CH_MODE, REG_DMIC_8CH_MODE_MASK, BIT4, PINMUX_FOR_DMIC_8CH_MODE_1},
    {PAD_KEY3, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT8, PINMUX_FOR_KEY_FIX_MODE_1},
    {PAD_KEY3, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_KEY_FIX_MODE_3},
    {PAD_KEY3, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo key4_tbl[] = {
    {PAD_KEY4, PADGPIO_BANK, REG_KEY4_GPIO_MODE, REG_KEY4_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_KEY4, PADTOP_BANK, REG_PWM_OUT0_MODE, REG_PWM_OUT0_MODE_MASK, BIT1, PINMUX_FOR_PWM_OUT0_MODE_2},
    {PAD_KEY4, PADTOP_BANK, REG_PWM_IN4_MODE, REG_PWM_IN4_MODE_MASK, BIT8, PINMUX_FOR_PWM_IN4_MODE_1},
    {PAD_KEY4, PADTOP_BANK, REG_I2S0_TX_MODE, REG_I2S0_TX_MODE_MASK, BIT12, PINMUX_FOR_I2S0_TX_MODE_1},
    {PAD_KEY4, PADTOP_BANK, REG_I2S0_RXTX_MODE, REG_I2S0_RXTX_MODE_MASK, BIT0, PINMUX_FOR_I2S0_RXTX_MODE_1},
    {PAD_KEY4, PADTOP_BANK, REG_DMIC_8CH_MODE, REG_DMIC_8CH_MODE_MASK, BIT4, PINMUX_FOR_DMIC_8CH_MODE_1},
    {PAD_KEY4, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT8, PINMUX_FOR_KEY_FIX_MODE_1},
    {PAD_KEY4, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_KEY_FIX_MODE_3},
    {PAD_KEY4, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo key5_tbl[] = {
    {PAD_KEY5, PADGPIO_BANK, REG_KEY5_GPIO_MODE, REG_KEY5_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_KEY5, PADTOP_BANK, REG_I2C0_MODE, REG_I2C0_MODE_MASK, BIT1, PINMUX_FOR_I2C0_MODE_2},
    {PAD_KEY5, PADTOP_BANK, REG_I2C4_MODE, REG_I2C4_MODE_MASK, BIT4, PINMUX_FOR_I2C4_MODE_1},
    {PAD_KEY5, PADTOP_BANK, REG_PWM_OUT1_MODE, REG_PWM_OUT1_MODE_MASK, BIT5, PINMUX_FOR_PWM_OUT1_MODE_2},
    {PAD_KEY5, PADTOP_BANK, REG_PWM_IN5_MODE, REG_PWM_IN5_MODE_MASK, BIT10, PINMUX_FOR_PWM_IN5_MODE_1},
    {PAD_KEY5, PADTOP_BANK, REG_I2S0_TX_MODE, REG_I2S0_TX_MODE_MASK, BIT12, PINMUX_FOR_I2S0_TX_MODE_1},
    {PAD_KEY5, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT8, PINMUX_FOR_KEY_FIX_MODE_1},
    {PAD_KEY5, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_KEY_FIX_MODE_3},
    {PAD_KEY5, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo key6_tbl[] = {
    {PAD_KEY6, PADGPIO_BANK, REG_KEY6_GPIO_MODE, REG_KEY6_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_KEY6, PADTOP_BANK, REG_I2C0_MODE, REG_I2C0_MODE_MASK, BIT1, PINMUX_FOR_I2C0_MODE_2},
    {PAD_KEY6, PADTOP_BANK, REG_I2C4_MODE, REG_I2C4_MODE_MASK, BIT4, PINMUX_FOR_I2C4_MODE_1},
    {PAD_KEY6, PADTOP_BANK, REG_PWM_OUT2_MODE, REG_PWM_OUT2_MODE_MASK, BIT9, PINMUX_FOR_PWM_OUT2_MODE_2},
    {PAD_KEY6, PADTOP_BANK, REG_PWM_IN6_MODE, REG_PWM_IN6_MODE_MASK, BIT12, PINMUX_FOR_PWM_IN6_MODE_1},
    {PAD_KEY6, PADTOP_BANK, REG_I2S0_TX_MODE, REG_I2S0_TX_MODE_MASK, BIT12, PINMUX_FOR_I2S0_TX_MODE_1},
    {PAD_KEY6, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT8, PINMUX_FOR_KEY_FIX_MODE_1},
    {PAD_KEY6, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT9, PINMUX_FOR_KEY_FIX_MODE_2},
};
const ST_PadMuxInfo key7_tbl[] = {
    {PAD_KEY7, PADGPIO_BANK, REG_KEY7_GPIO_MODE, REG_KEY7_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_KEY7, PADTOP_BANK, REG_SDIO_CDZ_MODE, REG_SDIO_CDZ_MODE_MASK, BIT12, PINMUX_FOR_SDIO_CDZ_MODE_1},
    {PAD_KEY7, PADTOP_BANK, REG_SDIO_RSTN_MODE, REG_SDIO_RSTN_MODE_MASK, BIT14, PINMUX_FOR_SDIO_RSTN_MODE_1},
    {PAD_KEY7, PADTOP_BANK, REG_PWM_OUT3_MODE, REG_PWM_OUT3_MODE_MASK, BIT13, PINMUX_FOR_PWM_OUT3_MODE_2},
    {PAD_KEY7, PADTOP_BANK, REG_PWM_IN7_MODE, REG_PWM_IN7_MODE_MASK, BIT14, PINMUX_FOR_PWM_IN7_MODE_1},
    {PAD_KEY7, PADTOP_BANK, REG_I2S1_MCK_MODE, REG_I2S1_MCK_MODE_MASK, BIT4, PINMUX_FOR_I2S1_MCK_MODE_1},
    {PAD_KEY7, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT8, PINMUX_FOR_KEY_FIX_MODE_1},
    {PAD_KEY7, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT9, PINMUX_FOR_KEY_FIX_MODE_2},
};
const ST_PadMuxInfo key8_tbl[] = {
    {PAD_KEY8, PADGPIO_BANK, REG_KEY8_GPIO_MODE, REG_KEY8_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_KEY8, PADTOP_BANK, REG_SDIO_MODE, REG_SDIO_MODE_MASK, BIT12, PINMUX_FOR_SDIO_MODE_1},
    {PAD_KEY8, PADTOP_BANK, REG_PWM_OUT9_MODE, REG_PWM_OUT9_MODE_MASK, BIT3 | BIT2, PINMUX_FOR_PWM_OUT9_MODE_3},
    {PAD_KEY8, PADTOP_BANK, REG_I2S1_RX_MODE, REG_I2S1_RX_MODE_MASK, BIT0, PINMUX_FOR_I2S1_RX_MODE_1},
    {PAD_KEY8, PADTOP_BANK, REG_I2S1_RXTX_MODE, REG_I2S1_RXTX_MODE_MASK, BIT4, PINMUX_FOR_I2S1_RXTX_MODE_1},
    {PAD_KEY8, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT8, PINMUX_FOR_KEY_FIX_MODE_1},
    {PAD_KEY8, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT9, PINMUX_FOR_KEY_FIX_MODE_2},
};
const ST_PadMuxInfo key9_tbl[] = {
    {PAD_KEY9, PADGPIO_BANK, REG_KEY9_GPIO_MODE, REG_KEY9_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_KEY9, PADTOP_BANK, REG_SDIO_MODE, REG_SDIO_MODE_MASK, BIT12, PINMUX_FOR_SDIO_MODE_1},
    {PAD_KEY9, PADTOP_BANK, REG_PWM_OUT10_MODE, REG_PWM_OUT10_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_PWM_OUT10_MODE_3},
    {PAD_KEY9, PADTOP_BANK, REG_I2S1_RX_MODE, REG_I2S1_RX_MODE_MASK, BIT0, PINMUX_FOR_I2S1_RX_MODE_1},
    {PAD_KEY9, PADTOP_BANK, REG_I2S1_RXTX_MODE, REG_I2S1_RXTX_MODE_MASK, BIT4, PINMUX_FOR_I2S1_RXTX_MODE_1},
    {PAD_KEY9, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT8, PINMUX_FOR_KEY_FIX_MODE_1},
    {PAD_KEY9, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT9, PINMUX_FOR_KEY_FIX_MODE_2},
};
const ST_PadMuxInfo key10_tbl[] = {
    {PAD_KEY10, PADGPIO_BANK, REG_KEY10_GPIO_MODE, REG_KEY10_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_KEY10, PADTOP_BANK, REG_SDIO_MODE, REG_SDIO_MODE_MASK, BIT12, PINMUX_FOR_SDIO_MODE_1},
    {PAD_KEY10, PADTOP_BANK, REG_PWM_OUT11_MODE, REG_PWM_OUT11_MODE_MASK, BIT7 | BIT6, PINMUX_FOR_PWM_OUT11_MODE_3},
    {PAD_KEY10, PADTOP_BANK, REG_I2S1_RX_MODE, REG_I2S1_RX_MODE_MASK, BIT0, PINMUX_FOR_I2S1_RX_MODE_1},
    {PAD_KEY10, PADTOP_BANK, REG_I2S1_RXTX_MODE, REG_I2S1_RXTX_MODE_MASK, BIT4, PINMUX_FOR_I2S1_RXTX_MODE_1},
    {PAD_KEY10, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT8, PINMUX_FOR_KEY_FIX_MODE_1},
    {PAD_KEY10, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT9, PINMUX_FOR_KEY_FIX_MODE_2},
};
const ST_PadMuxInfo key11_tbl[] = {
    {PAD_KEY11, PADGPIO_BANK, REG_KEY11_GPIO_MODE, REG_KEY11_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_KEY11, PADTOP_BANK, REG_SDIO_MODE, REG_SDIO_MODE_MASK, BIT12, PINMUX_FOR_SDIO_MODE_1},
    {PAD_KEY11, PADTOP_BANK, REG_I2S1_TX_MODE, REG_I2S1_TX_MODE_MASK, BIT4, PINMUX_FOR_I2S1_TX_MODE_1},
    {PAD_KEY11, PADTOP_BANK, REG_I2S1_RXTX_MODE, REG_I2S1_RXTX_MODE_MASK, BIT4, PINMUX_FOR_I2S1_RXTX_MODE_1},
    {PAD_KEY11, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT8, PINMUX_FOR_KEY_FIX_MODE_1},
    {PAD_KEY11, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT9, PINMUX_FOR_KEY_FIX_MODE_2},
};
const ST_PadMuxInfo key12_tbl[] = {
    {PAD_KEY12, PADGPIO_BANK, REG_KEY12_GPIO_MODE, REG_KEY12_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_KEY12, PADTOP_BANK, REG_SDIO_MODE, REG_SDIO_MODE_MASK, BIT12, PINMUX_FOR_SDIO_MODE_1},
    {PAD_KEY12, PADTOP_BANK, REG_I2S1_TX_MODE, REG_I2S1_TX_MODE_MASK, BIT4, PINMUX_FOR_I2S1_TX_MODE_1},
    {PAD_KEY12, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT8, PINMUX_FOR_KEY_FIX_MODE_1},
    {PAD_KEY12, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT9, PINMUX_FOR_KEY_FIX_MODE_2},
};
const ST_PadMuxInfo key13_tbl[] = {
    {PAD_KEY13, PADGPIO_BANK, REG_KEY13_GPIO_MODE, REG_KEY13_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_KEY13, PADTOP_BANK, REG_SDIO_MODE, REG_SDIO_MODE_MASK, BIT12, PINMUX_FOR_SDIO_MODE_1},
    {PAD_KEY13, PADTOP_BANK, REG_I2S1_TX_MODE, REG_I2S1_TX_MODE_MASK, BIT4, PINMUX_FOR_I2S1_TX_MODE_1},
    {PAD_KEY13, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT8, PINMUX_FOR_KEY_FIX_MODE_1},
    {PAD_KEY13, PADTOP_BANK, REG_KEY_FIX_MODE, REG_KEY_FIX_MODE_MASK, BIT9, PINMUX_FOR_KEY_FIX_MODE_2},
};
const ST_PadMuxInfo pm_pwm0_tbl[] = {
    {PAD_PM_PWM0, PADGPIO_BANK, REG_PM_PWM0_GPIO_MODE, REG_PM_PWM0_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PM_PWM0, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT0, PINMUX_FOR_TEST_IN_MODE_1},
    {PAD_PM_PWM0, PM_PADTOP_BANK, REG_I2CM_MODE, REG_I2CM_MODE_MASK, BIT1, PINMUX_FOR_I2CM_MODE_2},
    {PAD_PM_PWM0, PM_PADTOP_BANK, REG_PWM0_MODE, REG_PWM0_MODE_MASK, BIT0, PINMUX_FOR_PWM0_MODE_1},
    {PAD_PM_PWM0, PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE0, REG_PM_PAD_EXT_MODE0_MASK, BIT0, PINMUX_FOR_PM_PAD_EXT_MODE0_1},
};
const ST_PadMuxInfo pm_pwm1_tbl[] = {
    {PAD_PM_PWM1, PADGPIO_BANK, REG_PM_PWM1_GPIO_MODE, REG_PM_PWM1_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PM_PWM1, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT0, PINMUX_FOR_TEST_IN_MODE_1},
    {PAD_PM_PWM1, PM_PADTOP_BANK, REG_I2CM_MODE, REG_I2CM_MODE_MASK, BIT1, PINMUX_FOR_I2CM_MODE_2},
    {PAD_PM_PWM1, PM_PADTOP_BANK, REG_PWM1_MODE, REG_PWM1_MODE_MASK, BIT2, PINMUX_FOR_PWM1_MODE_1},
    {PAD_PM_PWM1, PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE1, REG_PM_PAD_EXT_MODE1_MASK, BIT1, PINMUX_FOR_PM_PAD_EXT_MODE1_1},
};
const ST_PadMuxInfo pm_i2cm_scl_tbl[] = {
    {PAD_PM_I2CM_SCL, PADGPIO_BANK, REG_PM_I2CM_SCL_GPIO_MODE, REG_PM_I2CM_SCL_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_PM_I2CM_SCL, PADTOP_BANK, REG_I2C5_MODE, REG_I2C5_MODE_MASK, BIT10, PINMUX_FOR_I2C5_MODE_4},
    {PAD_PM_I2CM_SCL, PM_PADTOP_BANK, REG_I2CM_MODE, REG_I2CM_MODE_MASK, BIT0, PINMUX_FOR_I2CM_MODE_1},
    {PAD_PM_I2CM_SCL, PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE2, REG_PM_PAD_EXT_MODE2_MASK, BIT2,
     PINMUX_FOR_PM_PAD_EXT_MODE2_1},
};
const ST_PadMuxInfo pm_i2cm_sda_tbl[] = {
    {PAD_PM_I2CM_SDA, PADGPIO_BANK, REG_PM_I2CM_SDA_GPIO_MODE, REG_PM_I2CM_SDA_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_PM_I2CM_SDA, PADTOP_BANK, REG_I2C5_MODE, REG_I2C5_MODE_MASK, BIT10, PINMUX_FOR_I2C5_MODE_4},
    {PAD_PM_I2CM_SDA, PM_PADTOP_BANK, REG_I2CM_MODE, REG_I2CM_MODE_MASK, BIT0, PINMUX_FOR_I2CM_MODE_1},
    {PAD_PM_I2CM_SDA, PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE3, REG_PM_PAD_EXT_MODE3_MASK, BIT3,
     PINMUX_FOR_PM_PAD_EXT_MODE3_1},
};
const ST_PadMuxInfo pm_uart_rx0_tbl[] = {
    {PAD_PM_UART_RX0, PADGPIO_BANK, REG_PM_UART_RX0_GPIO_MODE, REG_PM_UART_RX0_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_PM_UART_RX0, PADTOP_BANK, REG_UART0_MODE, REG_UART0_MODE_MASK, BIT0, PINMUX_FOR_UART0_MODE_1},
    {PAD_PM_UART_RX0, PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE4, REG_PM_PAD_EXT_MODE4_MASK, BIT4,
     PINMUX_FOR_PM_PAD_EXT_MODE4_1},
    {PAD_PM_UART_RX0, PM_PADTOP_BANK, REG_PM_UART_IS_GPIO, REG_PM_UART_IS_GPIO_MASK, 0, PINMUX_FOR_PM_UART_IS_GPIO_0},
};
const ST_PadMuxInfo pm_uart_tx0_tbl[] = {
    {PAD_PM_UART_TX0, PADGPIO_BANK, REG_PM_UART_TX0_GPIO_MODE, REG_PM_UART_TX0_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_PM_UART_TX0, PADTOP_BANK, REG_UART0_MODE, REG_UART0_MODE_MASK, BIT0, PINMUX_FOR_UART0_MODE_1},
    {PAD_PM_UART_TX0, PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE5, REG_PM_PAD_EXT_MODE5_MASK, BIT5,
     PINMUX_FOR_PM_PAD_EXT_MODE5_1},
    {PAD_PM_UART_TX0, PM_PADTOP_BANK, REG_PM_UART_IS_GPIO, REG_PM_UART_IS_GPIO_MASK, 0, PINMUX_FOR_PM_UART_IS_GPIO_0},
};
const ST_PadMuxInfo pm_ir_rx_tbl[] = {
    {PAD_PM_IR_RX, PADGPIO_BANK, REG_PM_IR_RX_GPIO_MODE, REG_PM_IR_RX_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PM_IR_RX, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT0, PINMUX_FOR_TEST_IN_MODE_1},
    {PAD_PM_IR_RX, PM_PADTOP_BANK, REG_IR_IN_MODE, REG_IR_IN_MODE_MASK, BIT0, PINMUX_FOR_IR_IN_MODE_1},
    {PAD_PM_IR_RX, PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE6, REG_PM_PAD_EXT_MODE6_MASK, BIT6,
     PINMUX_FOR_PM_PAD_EXT_MODE6_1},
};
const ST_PadMuxInfo pm_gpio0_tbl[] = {
    {PAD_PM_GPIO0, PADGPIO_BANK, REG_PM_GPIO0_GPIO_MODE, REG_PM_GPIO0_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PM_GPIO0, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT0, PINMUX_FOR_TEST_IN_MODE_1},
    {PAD_PM_GPIO0, PM_PADTOP_BANK, REG_PM_UART1_MODE, REG_PM_UART1_MODE_MASK, BIT0, PINMUX_FOR_PM_UART1_MODE_1},
    {PAD_PM_GPIO0, PM_PADTOP_BANK, REG_PM51_UART_MODE, REG_PM51_UART_MODE_MASK, BIT0, PINMUX_FOR_PM51_UART_MODE_1},
    {PAD_PM_GPIO0, PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE7, REG_PM_PAD_EXT_MODE7_MASK, BIT7,
     PINMUX_FOR_PM_PAD_EXT_MODE7_1},
};
const ST_PadMuxInfo pm_gpio1_tbl[] = {
    {PAD_PM_GPIO1, PADGPIO_BANK, REG_PM_GPIO1_GPIO_MODE, REG_PM_GPIO1_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PM_GPIO1, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT0, PINMUX_FOR_TEST_IN_MODE_1},
    {PAD_PM_GPIO1, PM_PADTOP_BANK, REG_PM_UART1_MODE, REG_PM_UART1_MODE_MASK, BIT0, PINMUX_FOR_PM_UART1_MODE_1},
    {PAD_PM_GPIO1, PM_PADTOP_BANK, REG_PM51_UART_MODE, REG_PM51_UART_MODE_MASK, BIT0, PINMUX_FOR_PM51_UART_MODE_1},
    {PAD_PM_GPIO1, PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE8, REG_PM_PAD_EXT_MODE8_MASK, BIT8,
     PINMUX_FOR_PM_PAD_EXT_MODE8_1},
};
const ST_PadMuxInfo pm_gpio2_tbl[] = {
    {PAD_PM_GPIO2, PADGPIO_BANK, REG_PM_GPIO2_GPIO_MODE, REG_PM_GPIO2_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PM_GPIO2, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT0, PINMUX_FOR_TEST_IN_MODE_1},
    {PAD_PM_GPIO2, PM_PADTOP_BANK, REG_VID_MODE, REG_VID_MODE_MASK, BIT2, PINMUX_FOR_VID_MODE_1},
    {PAD_PM_GPIO2, PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE9, REG_PM_PAD_EXT_MODE9_MASK, BIT9,
     PINMUX_FOR_PM_PAD_EXT_MODE9_1},
};
const ST_PadMuxInfo pm_gpio3_tbl[] = {
    {PAD_PM_GPIO3, PADGPIO_BANK, REG_PM_GPIO3_GPIO_MODE, REG_PM_GPIO3_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PM_GPIO3, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT0, PINMUX_FOR_TEST_IN_MODE_1},
    {PAD_PM_GPIO3, PM_PADTOP_BANK, REG_VID_MODE, REG_VID_MODE_MASK, BIT2, PINMUX_FOR_VID_MODE_1},
    {PAD_PM_GPIO3, PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE10, REG_PM_PAD_EXT_MODE10_MASK, BIT10,
     PINMUX_FOR_PM_PAD_EXT_MODE10_1},
};
const ST_PadMuxInfo pm_gpio4_tbl[] = {
    {PAD_PM_GPIO4, PADGPIO_BANK, REG_PM_GPIO4_GPIO_MODE, REG_PM_GPIO4_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PM_GPIO4, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT0, PINMUX_FOR_TEST_IN_MODE_1},
    {PAD_PM_GPIO4, PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE11, REG_PM_PAD_EXT_MODE11_MASK, BIT11,
     PINMUX_FOR_PM_PAD_EXT_MODE11_1},
};
const ST_PadMuxInfo pm_gpio5_tbl[] = {
    {PAD_PM_GPIO5, PADGPIO_BANK, REG_PM_GPIO5_GPIO_MODE, REG_PM_GPIO5_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PM_GPIO5, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT0, PINMUX_FOR_TEST_IN_MODE_1},
    {PAD_PM_GPIO5, PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE12, REG_PM_PAD_EXT_MODE12_MASK, BIT12,
     PINMUX_FOR_PM_PAD_EXT_MODE12_1},
    {PAD_PM_GPIO5, PM_PADTOP_BANK, REG_SPICSZ2_MODE, REG_SPICSZ2_MODE_MASK, BIT0, PINMUX_FOR_SPICSZ2_MODE_1},
};
const ST_PadMuxInfo pm_spi_wpz_tbl[] = {
    {PAD_PM_SPI_WPZ, PADGPIO_BANK, REG_PM_SPI_WPZ_GPIO_MODE, REG_PM_SPI_WPZ_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PM_SPI_WPZ, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT0, PINMUX_FOR_TEST_IN_MODE_1},
    {PAD_PM_SPI_WPZ, PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE13, REG_PM_PAD_EXT_MODE13_MASK, BIT13,
     PINMUX_FOR_PM_PAD_EXT_MODE13_1},
    {PAD_PM_SPI_WPZ, PMSLEEP_BANK, REG_SPIWPN_GPIO, REG_SPIWPN_GPIO_MASK, 0, PINMUX_FOR_SPIWPN_GPIO_0},
};
const ST_PadMuxInfo pm_spi_do_tbl[] = {
    {PAD_PM_SPI_DO, PADGPIO_BANK, REG_PM_SPI_DO_GPIO_MODE, REG_PM_SPI_DO_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PM_SPI_DO, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT0, PINMUX_FOR_TEST_IN_MODE_1},
    {PAD_PM_SPI_DO, PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE14, REG_PM_PAD_EXT_MODE14_MASK, BIT14,
     PINMUX_FOR_PM_PAD_EXT_MODE14_1},
    {PAD_PM_SPI_DO, PMSLEEP_BANK, REG_SPI_GPIO, REG_SPI_GPIO_MASK, 0, PINMUX_FOR_SPI_GPIO_0},
};
const ST_PadMuxInfo pm_spi_cz_tbl[] = {
    {PAD_PM_SPI_CZ, PADGPIO_BANK, REG_PM_SPI_CZ_GPIO_MODE, REG_PM_SPI_CZ_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PM_SPI_CZ, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT0, PINMUX_FOR_TEST_IN_MODE_1},
    {PAD_PM_SPI_CZ, PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE15, REG_PM_PAD_EXT_MODE15_MASK, BIT15,
     PINMUX_FOR_PM_PAD_EXT_MODE15_1},
    {PAD_PM_SPI_CZ, PMSLEEP_BANK, REG_SPICSZ1_GPIO, REG_SPICSZ1_GPIO_MASK, 0, PINMUX_FOR_SPICSZ1_GPIO_0},
};
const ST_PadMuxInfo pm_spi_hld_tbl[] = {
    {PAD_PM_SPI_HLD, PADGPIO_BANK, REG_PM_SPI_HLD_GPIO_MODE, REG_PM_SPI_HLD_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PM_SPI_HLD, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT0, PINMUX_FOR_TEST_IN_MODE_1},
    {PAD_PM_SPI_HLD, PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE16, REG_PM_PAD_EXT_MODE16_MASK, BIT0,
     PINMUX_FOR_PM_PAD_EXT_MODE16_1},
    {PAD_PM_SPI_HLD, PM_PADTOP_BANK, REG_SPIHOLDN_MODE, REG_SPIHOLDN_MODE_MASK, 0, PINMUX_FOR_SPIHOLDN_MODE_0},
};
const ST_PadMuxInfo pm_spi_ck_tbl[] = {
    {PAD_PM_SPI_CK, PADGPIO_BANK, REG_PM_SPI_CK_GPIO_MODE, REG_PM_SPI_CK_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PM_SPI_CK, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT0, PINMUX_FOR_TEST_IN_MODE_1},
    {PAD_PM_SPI_CK, PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE17, REG_PM_PAD_EXT_MODE17_MASK, BIT1,
     PINMUX_FOR_PM_PAD_EXT_MODE17_1},
    {PAD_PM_SPI_CK, PMSLEEP_BANK, REG_SPI_GPIO, REG_SPI_GPIO_MASK, 0, PINMUX_FOR_SPI_GPIO_0},
};
const ST_PadMuxInfo pm_spi_di_tbl[] = {
    {PAD_PM_SPI_DI, PADGPIO_BANK, REG_PM_SPI_DI_GPIO_MODE, REG_PM_SPI_DI_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PM_SPI_DI, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT0, PINMUX_FOR_TEST_IN_MODE_1},
    {PAD_PM_SPI_DI, PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE18, REG_PM_PAD_EXT_MODE18_MASK, BIT2,
     PINMUX_FOR_PM_PAD_EXT_MODE18_1},
    {PAD_PM_SPI_DI, PMSLEEP_BANK, REG_SPI_GPIO, REG_SPI_GPIO_MASK, 0, PINMUX_FOR_SPI_GPIO_0},
};
const ST_PadMuxInfo emmc_rstn_tbl[] = {
    {PAD_EMMC_RSTN, PADGPIO_BANK, REG_EMMC_RSTN_GPIO_MODE, REG_EMMC_RSTN_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
};
const ST_PadMuxInfo emmc_clk_tbl[] = {
    {PAD_EMMC_CLK, PADGPIO_BANK, REG_EMMC_CLK_GPIO_MODE, REG_EMMC_CLK_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_EMMC_CLK, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT0, PINMUX_FOR_TEST_IN_MODE_1},
    {PAD_EMMC_CLK, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5, PINMUX_FOR_TEST_OUT_MODE_2},
    {PAD_EMMC_CLK, PADTOP_BANK, REG_EMMC8B_BOOT_MODE, REG_EMMC8B_BOOT_MODE_MASK, BIT0, PINMUX_FOR_EMMC8B_BOOT_MODE_1},
    {PAD_EMMC_CLK, PADTOP_BANK, REG_EMMC4B_BOOT_MODE, REG_EMMC4B_BOOT_MODE_MASK, BIT8, PINMUX_FOR_EMMC4B_BOOT_MODE_1},
    {PAD_EMMC_CLK, PADTOP_BANK, REG_SDIO_MODE, REG_SDIO_MODE_MASK, BIT13, PINMUX_FOR_SDIO_MODE_2},
    {PAD_EMMC_CLK, PADTOP_BANK, REG_EMMC_8B_MODE, REG_EMMC_8B_MODE_MASK, BIT2, PINMUX_FOR_EMMC_8B_MODE_1},
    {PAD_EMMC_CLK, PADTOP_BANK, REG_EMMC_4B_MODE, REG_EMMC_4B_MODE_MASK, BIT0, PINMUX_FOR_EMMC_4B_MODE_1},
};
const ST_PadMuxInfo emmc_cmd_tbl[] = {
    {PAD_EMMC_CMD, PADGPIO_BANK, REG_EMMC_CMD_GPIO_MODE, REG_EMMC_CMD_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_EMMC_CMD, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT0, PINMUX_FOR_TEST_IN_MODE_1},
    {PAD_EMMC_CMD, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5, PINMUX_FOR_TEST_OUT_MODE_2},
    {PAD_EMMC_CMD, PADTOP_BANK, REG_EMMC8B_BOOT_MODE, REG_EMMC8B_BOOT_MODE_MASK, BIT0, PINMUX_FOR_EMMC8B_BOOT_MODE_1},
    {PAD_EMMC_CMD, PADTOP_BANK, REG_EMMC4B_BOOT_MODE, REG_EMMC4B_BOOT_MODE_MASK, BIT8, PINMUX_FOR_EMMC4B_BOOT_MODE_1},
    {PAD_EMMC_CMD, PADTOP_BANK, REG_SDIO_MODE, REG_SDIO_MODE_MASK, BIT13, PINMUX_FOR_SDIO_MODE_2},
    {PAD_EMMC_CMD, PADTOP_BANK, REG_EMMC_8B_MODE, REG_EMMC_8B_MODE_MASK, BIT2, PINMUX_FOR_EMMC_8B_MODE_1},
    {PAD_EMMC_CMD, PADTOP_BANK, REG_EMMC_4B_MODE, REG_EMMC_4B_MODE_MASK, BIT0, PINMUX_FOR_EMMC_4B_MODE_1},
};
const ST_PadMuxInfo emmc_ds_tbl[] = {
    {PAD_EMMC_DS, PADGPIO_BANK, REG_EMMC_DS_GPIO_MODE, REG_EMMC_DS_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_EMMC_DS, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT0, PINMUX_FOR_TEST_IN_MODE_1},
    {PAD_EMMC_DS, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5, PINMUX_FOR_TEST_OUT_MODE_2},
    {PAD_EMMC_DS, PADTOP_BANK, REG_SPI1_CZ1_MODE, REG_SPI1_CZ1_MODE_MASK, BIT7, PINMUX_FOR_SPI1_CZ1_MODE_2},
    {PAD_EMMC_DS, PADTOP_BANK, REG_FUART_MODE, REG_FUART_MODE_MASK, BIT10, PINMUX_FOR_FUART_MODE_4},
    {PAD_EMMC_DS, PADTOP_BANK, REG_FUART_2W_MODE, REG_FUART_2W_MODE_MASK, BIT14, PINMUX_FOR_FUART_2W_MODE_4},
    {PAD_EMMC_DS, PADTOP_BANK, REG_EMMC8B_BOOT_MODE, REG_EMMC8B_BOOT_MODE_MASK, BIT0, PINMUX_FOR_EMMC8B_BOOT_MODE_1},
    {PAD_EMMC_DS, PADTOP_BANK, REG_EMMC_8B_MODE, REG_EMMC_8B_MODE_MASK, BIT2, PINMUX_FOR_EMMC_8B_MODE_1},
    {PAD_EMMC_DS, PADTOP_BANK, REG_PWM_OUT0_MODE, REG_PWM_OUT0_MODE_MASK, BIT2, PINMUX_FOR_PWM_OUT0_MODE_4},
    {PAD_EMMC_DS, PADTOP_BANK, REG_PWM_OUT8_MODE, REG_PWM_OUT8_MODE_MASK, BIT1, PINMUX_FOR_PWM_OUT8_MODE_2},
};
const ST_PadMuxInfo emmc_d3_tbl[] = {
    {PAD_EMMC_D3, PADGPIO_BANK, REG_EMMC_D3_GPIO_MODE, REG_EMMC_D3_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_EMMC_D3, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT0, PINMUX_FOR_TEST_IN_MODE_1},
    {PAD_EMMC_D3, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5, PINMUX_FOR_TEST_OUT_MODE_2},
    {PAD_EMMC_D3, PADTOP_BANK, REG_EMMC8B_BOOT_MODE, REG_EMMC8B_BOOT_MODE_MASK, BIT0, PINMUX_FOR_EMMC8B_BOOT_MODE_1},
    {PAD_EMMC_D3, PADTOP_BANK, REG_EMMC4B_BOOT_MODE, REG_EMMC4B_BOOT_MODE_MASK, BIT8, PINMUX_FOR_EMMC4B_BOOT_MODE_1},
    {PAD_EMMC_D3, PADTOP_BANK, REG_SDIO_MODE, REG_SDIO_MODE_MASK, BIT13, PINMUX_FOR_SDIO_MODE_2},
    {PAD_EMMC_D3, PADTOP_BANK, REG_EMMC_8B_MODE, REG_EMMC_8B_MODE_MASK, BIT2, PINMUX_FOR_EMMC_8B_MODE_1},
    {PAD_EMMC_D3, PADTOP_BANK, REG_EMMC_4B_MODE, REG_EMMC_4B_MODE_MASK, BIT0, PINMUX_FOR_EMMC_4B_MODE_1},
    {PAD_EMMC_D3, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
};
const ST_PadMuxInfo emmc_d4_tbl[] = {
    {PAD_EMMC_D4, PADGPIO_BANK, REG_EMMC_D4_GPIO_MODE, REG_EMMC_D4_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_EMMC_D4, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT0, PINMUX_FOR_TEST_IN_MODE_1},
    {PAD_EMMC_D4, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5, PINMUX_FOR_TEST_OUT_MODE_2},
    {PAD_EMMC_D4, PADTOP_BANK, REG_SPI1_MODE, REG_SPI1_MODE_MASK, BIT4 | BIT3, PINMUX_FOR_SPI1_MODE_3},
    {PAD_EMMC_D4, PADTOP_BANK, REG_FUART_MODE, REG_FUART_MODE_MASK, BIT10, PINMUX_FOR_FUART_MODE_4},
    {PAD_EMMC_D4, PADTOP_BANK, REG_FUART_2W_MODE, REG_FUART_2W_MODE_MASK, BIT14, PINMUX_FOR_FUART_2W_MODE_4},
    {PAD_EMMC_D4, PADTOP_BANK, REG_EMMC8B_BOOT_MODE, REG_EMMC8B_BOOT_MODE_MASK, BIT0, PINMUX_FOR_EMMC8B_BOOT_MODE_1},
    {PAD_EMMC_D4, PADTOP_BANK, REG_EMMC_8B_MODE, REG_EMMC_8B_MODE_MASK, BIT2, PINMUX_FOR_EMMC_8B_MODE_1},
    {PAD_EMMC_D4, PADTOP_BANK, REG_PWM_OUT1_MODE, REG_PWM_OUT1_MODE_MASK, BIT6, PINMUX_FOR_PWM_OUT1_MODE_4},
    {PAD_EMMC_D4, PADTOP_BANK, REG_PWM_OUT9_MODE, REG_PWM_OUT9_MODE_MASK, BIT3, PINMUX_FOR_PWM_OUT9_MODE_2},
    {PAD_EMMC_D4, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
};
const ST_PadMuxInfo emmc_d0_tbl[] = {
    {PAD_EMMC_D0, PADGPIO_BANK, REG_EMMC_D0_GPIO_MODE, REG_EMMC_D0_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_EMMC_D0, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT0, PINMUX_FOR_TEST_IN_MODE_1},
    {PAD_EMMC_D0, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5, PINMUX_FOR_TEST_OUT_MODE_2},
    {PAD_EMMC_D0, PADTOP_BANK, REG_EMMC8B_BOOT_MODE, REG_EMMC8B_BOOT_MODE_MASK, BIT0, PINMUX_FOR_EMMC8B_BOOT_MODE_1},
    {PAD_EMMC_D0, PADTOP_BANK, REG_EMMC4B_BOOT_MODE, REG_EMMC4B_BOOT_MODE_MASK, BIT8, PINMUX_FOR_EMMC4B_BOOT_MODE_1},
    {PAD_EMMC_D0, PADTOP_BANK, REG_SDIO_MODE, REG_SDIO_MODE_MASK, BIT13, PINMUX_FOR_SDIO_MODE_2},
    {PAD_EMMC_D0, PADTOP_BANK, REG_EMMC_8B_MODE, REG_EMMC_8B_MODE_MASK, BIT2, PINMUX_FOR_EMMC_8B_MODE_1},
    {PAD_EMMC_D0, PADTOP_BANK, REG_EMMC_4B_MODE, REG_EMMC_4B_MODE_MASK, BIT0, PINMUX_FOR_EMMC_4B_MODE_1},
    {PAD_EMMC_D0, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
};
const ST_PadMuxInfo emmc_d5_tbl[] = {
    {PAD_EMMC_D5, PADGPIO_BANK, REG_EMMC_D5_GPIO_MODE, REG_EMMC_D5_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_EMMC_D5, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5, PINMUX_FOR_TEST_OUT_MODE_2},
    {PAD_EMMC_D5, PADTOP_BANK, REG_SPI1_MODE, REG_SPI1_MODE_MASK, BIT4 | BIT3, PINMUX_FOR_SPI1_MODE_3},
    {PAD_EMMC_D5, PADTOP_BANK, REG_FUART_MODE, REG_FUART_MODE_MASK, BIT10, PINMUX_FOR_FUART_MODE_4},
    {PAD_EMMC_D5, PADTOP_BANK, REG_EMMC8B_BOOT_MODE, REG_EMMC8B_BOOT_MODE_MASK, BIT0, PINMUX_FOR_EMMC8B_BOOT_MODE_1},
    {PAD_EMMC_D5, PADTOP_BANK, REG_EMMC_8B_MODE, REG_EMMC_8B_MODE_MASK, BIT2, PINMUX_FOR_EMMC_8B_MODE_1},
    {PAD_EMMC_D5, PADTOP_BANK, REG_PWM_OUT2_MODE, REG_PWM_OUT2_MODE_MASK, BIT10, PINMUX_FOR_PWM_OUT2_MODE_4},
    {PAD_EMMC_D5, PADTOP_BANK, REG_PWM_OUT10_MODE, REG_PWM_OUT10_MODE_MASK, BIT5, PINMUX_FOR_PWM_OUT10_MODE_2},
    {PAD_EMMC_D5, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
};
const ST_PadMuxInfo emmc_d1_tbl[] = {
    {PAD_EMMC_D1, PADGPIO_BANK, REG_EMMC_D1_GPIO_MODE, REG_EMMC_D1_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_EMMC_D1, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT0, PINMUX_FOR_TEST_IN_MODE_1},
    {PAD_EMMC_D1, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5, PINMUX_FOR_TEST_OUT_MODE_2},
    {PAD_EMMC_D1, PADTOP_BANK, REG_EMMC8B_BOOT_MODE, REG_EMMC8B_BOOT_MODE_MASK, BIT0, PINMUX_FOR_EMMC8B_BOOT_MODE_1},
    {PAD_EMMC_D1, PADTOP_BANK, REG_EMMC4B_BOOT_MODE, REG_EMMC4B_BOOT_MODE_MASK, BIT8, PINMUX_FOR_EMMC4B_BOOT_MODE_1},
    {PAD_EMMC_D1, PADTOP_BANK, REG_SDIO_MODE, REG_SDIO_MODE_MASK, BIT13, PINMUX_FOR_SDIO_MODE_2},
    {PAD_EMMC_D1, PADTOP_BANK, REG_EMMC_8B_MODE, REG_EMMC_8B_MODE_MASK, BIT2, PINMUX_FOR_EMMC_8B_MODE_1},
    {PAD_EMMC_D1, PADTOP_BANK, REG_EMMC_4B_MODE, REG_EMMC_4B_MODE_MASK, BIT0, PINMUX_FOR_EMMC_4B_MODE_1},
    {PAD_EMMC_D1, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
};
const ST_PadMuxInfo emmc_d6_tbl[] = {
    {PAD_EMMC_D6, PADGPIO_BANK, REG_EMMC_D6_GPIO_MODE, REG_EMMC_D6_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_EMMC_D6, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT0, PINMUX_FOR_TEST_IN_MODE_1},
    {PAD_EMMC_D6, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5, PINMUX_FOR_TEST_OUT_MODE_2},
    {PAD_EMMC_D6, PADTOP_BANK, REG_SPI1_MODE, REG_SPI1_MODE_MASK, BIT4 | BIT3, PINMUX_FOR_SPI1_MODE_3},
    {PAD_EMMC_D6, PADTOP_BANK, REG_FUART_MODE, REG_FUART_MODE_MASK, BIT10, PINMUX_FOR_FUART_MODE_4},
    {PAD_EMMC_D6, PADTOP_BANK, REG_EMMC8B_BOOT_MODE, REG_EMMC8B_BOOT_MODE_MASK, BIT0, PINMUX_FOR_EMMC8B_BOOT_MODE_1},
    {PAD_EMMC_D6, PADTOP_BANK, REG_EMMC_8B_MODE, REG_EMMC_8B_MODE_MASK, BIT2, PINMUX_FOR_EMMC_8B_MODE_1},
    {PAD_EMMC_D6, PADTOP_BANK, REG_PWM_OUT3_MODE, REG_PWM_OUT3_MODE_MASK, BIT14, PINMUX_FOR_PWM_OUT3_MODE_4},
    {PAD_EMMC_D6, PADTOP_BANK, REG_PWM_OUT11_MODE, REG_PWM_OUT11_MODE_MASK, BIT7, PINMUX_FOR_PWM_OUT11_MODE_2},
    {PAD_EMMC_D6, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
};
const ST_PadMuxInfo emmc_d2_tbl[] = {
    {PAD_EMMC_D2, PADGPIO_BANK, REG_EMMC_D2_GPIO_MODE, REG_EMMC_D2_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_EMMC_D2, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5, PINMUX_FOR_TEST_OUT_MODE_2},
    {PAD_EMMC_D2, PADTOP_BANK, REG_EMMC8B_BOOT_MODE, REG_EMMC8B_BOOT_MODE_MASK, BIT0, PINMUX_FOR_EMMC8B_BOOT_MODE_1},
    {PAD_EMMC_D2, PADTOP_BANK, REG_EMMC4B_BOOT_MODE, REG_EMMC4B_BOOT_MODE_MASK, BIT8, PINMUX_FOR_EMMC4B_BOOT_MODE_1},
    {PAD_EMMC_D2, PADTOP_BANK, REG_SDIO_MODE, REG_SDIO_MODE_MASK, BIT13, PINMUX_FOR_SDIO_MODE_2},
    {PAD_EMMC_D2, PADTOP_BANK, REG_EMMC_8B_MODE, REG_EMMC_8B_MODE_MASK, BIT2, PINMUX_FOR_EMMC_8B_MODE_1},
    {PAD_EMMC_D2, PADTOP_BANK, REG_EMMC_4B_MODE, REG_EMMC_4B_MODE_MASK, BIT0, PINMUX_FOR_EMMC_4B_MODE_1},
    {PAD_EMMC_D2, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
};
const ST_PadMuxInfo emmc_d7_tbl[] = {
    {PAD_EMMC_D7, PADGPIO_BANK, REG_EMMC_D7_GPIO_MODE, REG_EMMC_D7_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_EMMC_D7, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5, PINMUX_FOR_TEST_OUT_MODE_2},
    {PAD_EMMC_D7, PADTOP_BANK, REG_SPI1_MODE, REG_SPI1_MODE_MASK, BIT4 | BIT3, PINMUX_FOR_SPI1_MODE_3},
    {PAD_EMMC_D7, PADTOP_BANK, REG_EMMC8B_BOOT_MODE, REG_EMMC8B_BOOT_MODE_MASK, BIT0, PINMUX_FOR_EMMC8B_BOOT_MODE_1},
    {PAD_EMMC_D7, PADTOP_BANK, REG_EMMC_8B_MODE, REG_EMMC_8B_MODE_MASK, BIT2, PINMUX_FOR_EMMC_8B_MODE_1},
    {PAD_EMMC_D7, PADTOP_BANK, REG_PWM_OUT4_MODE, REG_PWM_OUT4_MODE_MASK, BIT2, PINMUX_FOR_PWM_OUT4_MODE_4},
    {PAD_EMMC_D7, PADTOP_BANK, REG_PWM_OUT12_MODE, REG_PWM_OUT12_MODE_MASK, BIT1, PINMUX_FOR_PWM_OUT12_MODE_2},
    {PAD_EMMC_D7, PADTOP_BANK, REG_PWM_IN0_MODE, REG_PWM_IN0_MODE_MASK, BIT1, PINMUX_FOR_PWM_IN0_MODE_2},
    {PAD_EMMC_D7, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
};
const ST_PadMuxInfo outp_rx1_ch_0_tbl[] = {
    {PAD_OUTP_RX1_CH_0, PADGPIO_BANK, REG_OUTP_RX1_CH_0_GPIO_MODE, REG_OUTP_RX1_CH_0_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTP_RX1_CH_0, PADTOP_BANK, REG_SR1_MIPI_MODE, REG_SR1_MIPI_MODE_MASK, BIT4, PINMUX_FOR_SR1_MIPI_MODE_1},
    {PAD_OUTP_RX1_CH_0, PADTOP_BANK, REG_SR1_MIPI_MODE, REG_SR1_MIPI_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_SR1_MIPI_MODE_3},
    {PAD_OUTP_RX1_CH_0, PADTOP_BANK, REG_SR1_MIPI_MODE, REG_SR1_MIPI_MODE_MASK, BIT6, PINMUX_FOR_SR1_MIPI_MODE_4},
    {PAD_OUTP_RX1_CH_0, PADTOP_BANK, REG_SR0_SYNC_MODE, REG_SR0_SYNC_MODE_MASK, BIT8, PINMUX_FOR_SR0_SYNC_MODE_1},
    {PAD_OUTP_RX1_CH_0, PADTOP_BANK, REG_SR1_BT656_MODE, REG_SR1_BT656_MODE_MASK, BIT2, PINMUX_FOR_SR1_BT656_MODE_1},
    {PAD_OUTP_RX1_CH_0, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT1120_MODE_1},
    {PAD_OUTP_RX1_CH_0, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT1, PINMUX_FOR_SR0_BT1120_MODE_2},
};
const ST_PadMuxInfo outn_rx1_ch_0_tbl[] = {
    {PAD_OUTN_RX1_CH_0, PADGPIO_BANK, REG_OUTN_RX1_CH_0_GPIO_MODE, REG_OUTN_RX1_CH_0_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTN_RX1_CH_0, PADTOP_BANK, REG_SR1_MIPI_MODE, REG_SR1_MIPI_MODE_MASK, BIT4, PINMUX_FOR_SR1_MIPI_MODE_1},
    {PAD_OUTN_RX1_CH_0, PADTOP_BANK, REG_SR1_MIPI_MODE, REG_SR1_MIPI_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_SR1_MIPI_MODE_3},
    {PAD_OUTN_RX1_CH_0, PADTOP_BANK, REG_SR1_MIPI_MODE, REG_SR1_MIPI_MODE_MASK, BIT6, PINMUX_FOR_SR1_MIPI_MODE_4},
    {PAD_OUTN_RX1_CH_0, PADTOP_BANK, REG_SR0_SYNC_MODE, REG_SR0_SYNC_MODE_MASK, BIT8, PINMUX_FOR_SR0_SYNC_MODE_1},
    {PAD_OUTN_RX1_CH_0, PADTOP_BANK, REG_SR1_BT656_MODE, REG_SR1_BT656_MODE_MASK, BIT2, PINMUX_FOR_SR1_BT656_MODE_1},
    {PAD_OUTN_RX1_CH_0, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT1120_MODE_1},
    {PAD_OUTN_RX1_CH_0, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT1, PINMUX_FOR_SR0_BT1120_MODE_2},
};
const ST_PadMuxInfo outp_rx1_ch_1_tbl[] = {
    {PAD_OUTP_RX1_CH_1, PADGPIO_BANK, REG_OUTP_RX1_CH_1_GPIO_MODE, REG_OUTP_RX1_CH_1_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTP_RX1_CH_1, PADTOP_BANK, REG_SR1_MIPI_MODE, REG_SR1_MIPI_MODE_MASK, BIT4, PINMUX_FOR_SR1_MIPI_MODE_1},
    {PAD_OUTP_RX1_CH_1, PADTOP_BANK, REG_SR1_MIPI_MODE, REG_SR1_MIPI_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_SR1_MIPI_MODE_3},
    {PAD_OUTP_RX1_CH_1, PADTOP_BANK, REG_SR1_MIPI_MODE, REG_SR1_MIPI_MODE_MASK, BIT6, PINMUX_FOR_SR1_MIPI_MODE_4},
    {PAD_OUTP_RX1_CH_1, PADTOP_BANK, REG_SR0_PCLK_MODE, REG_SR0_PCLK_MODE_MASK, BIT0, PINMUX_FOR_SR0_PCLK_MODE_1},
    {PAD_OUTP_RX1_CH_1, PADTOP_BANK, REG_SR1_BT656_MODE, REG_SR1_BT656_MODE_MASK, BIT2, PINMUX_FOR_SR1_BT656_MODE_1},
    {PAD_OUTP_RX1_CH_1, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT1120_MODE_1},
    {PAD_OUTP_RX1_CH_1, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT1, PINMUX_FOR_SR0_BT1120_MODE_2},
};
const ST_PadMuxInfo outn_rx1_ch_1_tbl[] = {
    {PAD_OUTN_RX1_CH_1, PADGPIO_BANK, REG_OUTN_RX1_CH_1_GPIO_MODE, REG_OUTN_RX1_CH_1_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTN_RX1_CH_1, PADTOP_BANK, REG_SR1_MIPI_MODE, REG_SR1_MIPI_MODE_MASK, BIT4, PINMUX_FOR_SR1_MIPI_MODE_1},
    {PAD_OUTN_RX1_CH_1, PADTOP_BANK, REG_SR1_MIPI_MODE, REG_SR1_MIPI_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_SR1_MIPI_MODE_3},
    {PAD_OUTN_RX1_CH_1, PADTOP_BANK, REG_SR1_MIPI_MODE, REG_SR1_MIPI_MODE_MASK, BIT6, PINMUX_FOR_SR1_MIPI_MODE_4},
    {PAD_OUTN_RX1_CH_1, PADTOP_BANK, REG_SR1_BT656_MODE, REG_SR1_BT656_MODE_MASK, BIT2, PINMUX_FOR_SR1_BT656_MODE_1},
    {PAD_OUTN_RX1_CH_1, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT1120_MODE_1},
    {PAD_OUTN_RX1_CH_1, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT1, PINMUX_FOR_SR0_BT1120_MODE_2},
};
const ST_PadMuxInfo outp_rx1_ch_2_tbl[] = {
    {PAD_OUTP_RX1_CH_2, PADGPIO_BANK, REG_OUTP_RX1_CH_2_GPIO_MODE, REG_OUTP_RX1_CH_2_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTP_RX1_CH_2, PADTOP_BANK, REG_SR1_MIPI_MODE, REG_SR1_MIPI_MODE_MASK, BIT5, PINMUX_FOR_SR1_MIPI_MODE_2},
    {PAD_OUTP_RX1_CH_2, PADTOP_BANK, REG_SR1_MIPI_MODE, REG_SR1_MIPI_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_SR1_MIPI_MODE_3},
    {PAD_OUTP_RX1_CH_2, PADTOP_BANK, REG_SR1_MIPI_MODE, REG_SR1_MIPI_MODE_MASK, BIT6, PINMUX_FOR_SR1_MIPI_MODE_4},
    {PAD_OUTP_RX1_CH_2, PADTOP_BANK, REG_SR1_BT656_MODE, REG_SR1_BT656_MODE_MASK, BIT2, PINMUX_FOR_SR1_BT656_MODE_1},
    {PAD_OUTP_RX1_CH_2, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT1120_MODE_1},
    {PAD_OUTP_RX1_CH_2, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT1, PINMUX_FOR_SR0_BT1120_MODE_2},
};
const ST_PadMuxInfo outn_rx1_ch_2_tbl[] = {
    {PAD_OUTN_RX1_CH_2, PADGPIO_BANK, REG_OUTN_RX1_CH_2_GPIO_MODE, REG_OUTN_RX1_CH_2_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTN_RX1_CH_2, PADTOP_BANK, REG_SR1_MIPI_MODE, REG_SR1_MIPI_MODE_MASK, BIT5, PINMUX_FOR_SR1_MIPI_MODE_2},
    {PAD_OUTN_RX1_CH_2, PADTOP_BANK, REG_SR1_MIPI_MODE, REG_SR1_MIPI_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_SR1_MIPI_MODE_3},
    {PAD_OUTN_RX1_CH_2, PADTOP_BANK, REG_SR1_MIPI_MODE, REG_SR1_MIPI_MODE_MASK, BIT6, PINMUX_FOR_SR1_MIPI_MODE_4},
    {PAD_OUTN_RX1_CH_2, PADTOP_BANK, REG_SR1_BT656_MODE, REG_SR1_BT656_MODE_MASK, BIT2, PINMUX_FOR_SR1_BT656_MODE_1},
    {PAD_OUTN_RX1_CH_2, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT1120_MODE_1},
    {PAD_OUTN_RX1_CH_2, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT1, PINMUX_FOR_SR0_BT1120_MODE_2},
};
const ST_PadMuxInfo outp_rx1_ch_3_tbl[] = {
    {PAD_OUTP_RX1_CH_3, PADGPIO_BANK, REG_OUTP_RX1_CH_3_GPIO_MODE, REG_OUTP_RX1_CH_3_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTP_RX1_CH_3, PADTOP_BANK, REG_SR1_MIPI_MODE, REG_SR1_MIPI_MODE_MASK, BIT5, PINMUX_FOR_SR1_MIPI_MODE_2},
    {PAD_OUTP_RX1_CH_3, PADTOP_BANK, REG_SR1_MIPI_MODE, REG_SR1_MIPI_MODE_MASK, BIT6, PINMUX_FOR_SR1_MIPI_MODE_4},
    {PAD_OUTP_RX1_CH_3, PADTOP_BANK, REG_SR1_BT656_MODE, REG_SR1_BT656_MODE_MASK, BIT2, PINMUX_FOR_SR1_BT656_MODE_1},
    {PAD_OUTP_RX1_CH_3, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT1120_MODE_1},
    {PAD_OUTP_RX1_CH_3, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT1, PINMUX_FOR_SR0_BT1120_MODE_2},
};
const ST_PadMuxInfo outn_rx1_ch_3_tbl[] = {
    {PAD_OUTN_RX1_CH_3, PADGPIO_BANK, REG_OUTN_RX1_CH_3_GPIO_MODE, REG_OUTN_RX1_CH_3_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTN_RX1_CH_3, PADTOP_BANK, REG_SR1_MIPI_MODE, REG_SR1_MIPI_MODE_MASK, BIT5, PINMUX_FOR_SR1_MIPI_MODE_2},
    {PAD_OUTN_RX1_CH_3, PADTOP_BANK, REG_SR1_MIPI_MODE, REG_SR1_MIPI_MODE_MASK, BIT6, PINMUX_FOR_SR1_MIPI_MODE_4},
    {PAD_OUTN_RX1_CH_3, PADTOP_BANK, REG_SR1_BT656_MODE, REG_SR1_BT656_MODE_MASK, BIT2, PINMUX_FOR_SR1_BT656_MODE_1},
    {PAD_OUTN_RX1_CH_3, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT1120_MODE_1},
    {PAD_OUTN_RX1_CH_3, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT1, PINMUX_FOR_SR0_BT1120_MODE_2},
    {PAD_OUTN_RX1_CH_3, PADTOP_BANK, REG_PSPI0_TE_MODE, REG_PSPI0_TE_MODE_MASK, BIT4, PINMUX_FOR_PSPI0_TE_MODE_1},
};
const ST_PadMuxInfo outp_rx0_ch_0_tbl[] = {
    {PAD_OUTP_RX0_CH_0, PADGPIO_BANK, REG_OUTP_RX0_CH_0_GPIO_MODE, REG_OUTP_RX0_CH_0_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTP_RX0_CH_0, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_OUTP_RX0_CH_0, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT0, PINMUX_FOR_SR0_MIPI_MODE_1},
    {PAD_OUTP_RX0_CH_0, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT1, PINMUX_FOR_SR0_MIPI_MODE_2},
    {PAD_OUTP_RX0_CH_0, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT1,
     PINMUX_FOR_SR0_MIPI_MODE_6},
    {PAD_OUTP_RX0_CH_0, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT0, PINMUX_FOR_SR0_MODE_1},
    {PAD_OUTP_RX0_CH_0, PADTOP_BANK, REG_SR0_BT656_MODE, REG_SR0_BT656_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT656_MODE_1},
    {PAD_OUTP_RX0_CH_0, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT1120_MODE_1},
    {PAD_OUTP_RX0_CH_0, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT1, PINMUX_FOR_SR0_BT1120_MODE_2},
    {PAD_OUTP_RX0_CH_0, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTP_RX0_CH_0, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TTL18_MODE_3},
    {PAD_OUTP_RX0_CH_0, PADTOP_BANK, REG_PSPI0_G_MODE, REG_PSPI0_G_MODE_MASK, BIT0, PINMUX_FOR_PSPI0_G_MODE_1},
    {PAD_OUTP_RX0_CH_0, PADTOP_BANK, REG_PSPI0_CS_MODE, REG_PSPI0_CS_MODE_MASK, BIT2, PINMUX_FOR_PSPI0_CS_MODE_1},
};
const ST_PadMuxInfo outn_rx0_ch_0_tbl[] = {
    {PAD_OUTN_RX0_CH_0, PADGPIO_BANK, REG_OUTN_RX0_CH_0_GPIO_MODE, REG_OUTN_RX0_CH_0_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTN_RX0_CH_0, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_OUTN_RX0_CH_0, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT0, PINMUX_FOR_SR0_MIPI_MODE_1},
    {PAD_OUTN_RX0_CH_0, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT1, PINMUX_FOR_SR0_MIPI_MODE_2},
    {PAD_OUTN_RX0_CH_0, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT1,
     PINMUX_FOR_SR0_MIPI_MODE_6},
    {PAD_OUTN_RX0_CH_0, PADTOP_BANK, REG_SR0_SYNC_MODE, REG_SR0_SYNC_MODE_MASK, BIT9 | BIT8,
     PINMUX_FOR_SR0_SYNC_MODE_3},
    {PAD_OUTN_RX0_CH_0, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT0, PINMUX_FOR_SR0_MODE_1},
    {PAD_OUTN_RX0_CH_0, PADTOP_BANK, REG_SR0_BT656_MODE, REG_SR0_BT656_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT656_MODE_1},
    {PAD_OUTN_RX0_CH_0, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT1120_MODE_1},
    {PAD_OUTN_RX0_CH_0, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT1, PINMUX_FOR_SR0_BT1120_MODE_2},
    {PAD_OUTN_RX0_CH_0, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTN_RX0_CH_0, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TTL18_MODE_3},
    {PAD_OUTN_RX0_CH_0, PADTOP_BANK, REG_PSPI0_G_MODE, REG_PSPI0_G_MODE_MASK, BIT0, PINMUX_FOR_PSPI0_G_MODE_1},
    {PAD_OUTN_RX0_CH_0, PADTOP_BANK, REG_PSPI0_SENSOR_MODE, REG_PSPI0_SENSOR_MODE_MASK, BIT0,
     PINMUX_FOR_PSPI0_SENSOR_MODE_1},
    {PAD_OUTN_RX0_CH_0, PADTOP_BANK, REG_PSPI0_SENSOR_MODE, REG_PSPI0_SENSOR_MODE_MASK, BIT1,
     PINMUX_FOR_PSPI0_SENSOR_MODE_2},
    {PAD_OUTN_RX0_CH_0, PADTOP_BANK, REG_PSPI0_SENSOR_MODE, REG_PSPI0_SENSOR_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_PSPI0_SENSOR_MODE_3},
};
const ST_PadMuxInfo outp_rx0_ch_1_tbl[] = {
    {PAD_OUTP_RX0_CH_1, PADGPIO_BANK, REG_OUTP_RX0_CH_1_GPIO_MODE, REG_OUTP_RX0_CH_1_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTP_RX0_CH_1, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_OUTP_RX0_CH_1, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT0, PINMUX_FOR_SR0_MIPI_MODE_1},
    {PAD_OUTP_RX0_CH_1, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT1, PINMUX_FOR_SR0_MIPI_MODE_2},
    {PAD_OUTP_RX0_CH_1, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2, PINMUX_FOR_SR0_MIPI_MODE_4},
    {PAD_OUTP_RX0_CH_1, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT1,
     PINMUX_FOR_SR0_MIPI_MODE_6},
    {PAD_OUTP_RX0_CH_1, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT1 | BIT0,
     PINMUX_FOR_SR0_MIPI_MODE_7},
    {PAD_OUTP_RX0_CH_1, PADTOP_BANK, REG_SR0_SYNC_MODE, REG_SR0_SYNC_MODE_MASK, BIT9 | BIT8,
     PINMUX_FOR_SR0_SYNC_MODE_3},
    {PAD_OUTP_RX0_CH_1, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT0, PINMUX_FOR_SR0_MODE_1},
    {PAD_OUTP_RX0_CH_1, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT1, PINMUX_FOR_SR0_MODE_2},
    {PAD_OUTP_RX0_CH_1, PADTOP_BANK, REG_SR0_BT656_MODE, REG_SR0_BT656_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT656_MODE_1},
    {PAD_OUTP_RX0_CH_1, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT1120_MODE_1},
    {PAD_OUTP_RX0_CH_1, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT1, PINMUX_FOR_SR0_BT1120_MODE_2},
    {PAD_OUTP_RX0_CH_1, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTP_RX0_CH_1, PADTOP_BANK, REG_PSPI0_G_MODE, REG_PSPI0_G_MODE_MASK, BIT0, PINMUX_FOR_PSPI0_G_MODE_1},
    {PAD_OUTP_RX0_CH_1, PADTOP_BANK, REG_PSPI0_SENSOR_MODE, REG_PSPI0_SENSOR_MODE_MASK, BIT0,
     PINMUX_FOR_PSPI0_SENSOR_MODE_1},
    {PAD_OUTP_RX0_CH_1, PADTOP_BANK, REG_PSPI0_SENSOR_MODE, REG_PSPI0_SENSOR_MODE_MASK, BIT1,
     PINMUX_FOR_PSPI0_SENSOR_MODE_2},
    {PAD_OUTP_RX0_CH_1, PADTOP_BANK, REG_PSPI0_SENSOR_MODE, REG_PSPI0_SENSOR_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_PSPI0_SENSOR_MODE_3},
};
const ST_PadMuxInfo outn_rx0_ch_1_tbl[] = {
    {PAD_OUTN_RX0_CH_1, PADGPIO_BANK, REG_OUTN_RX0_CH_1_GPIO_MODE, REG_OUTN_RX0_CH_1_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTN_RX0_CH_1, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_OUTN_RX0_CH_1, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT0, PINMUX_FOR_SR0_MIPI_MODE_1},
    {PAD_OUTN_RX0_CH_1, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT1, PINMUX_FOR_SR0_MIPI_MODE_2},
    {PAD_OUTN_RX0_CH_1, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2, PINMUX_FOR_SR0_MIPI_MODE_4},
    {PAD_OUTN_RX0_CH_1, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT1,
     PINMUX_FOR_SR0_MIPI_MODE_6},
    {PAD_OUTN_RX0_CH_1, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT1 | BIT0,
     PINMUX_FOR_SR0_MIPI_MODE_7},
    {PAD_OUTN_RX0_CH_1, PADTOP_BANK, REG_SR0_PCLK_MODE, REG_SR0_PCLK_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_SR0_PCLK_MODE_3},
    {PAD_OUTN_RX0_CH_1, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT0, PINMUX_FOR_SR0_MODE_1},
    {PAD_OUTN_RX0_CH_1, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT1, PINMUX_FOR_SR0_MODE_2},
    {PAD_OUTN_RX0_CH_1, PADTOP_BANK, REG_SR0_BT656_MODE, REG_SR0_BT656_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT656_MODE_1},
    {PAD_OUTN_RX0_CH_1, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT1120_MODE_1},
    {PAD_OUTN_RX0_CH_1, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT1, PINMUX_FOR_SR0_BT1120_MODE_2},
    {PAD_OUTN_RX0_CH_1, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTN_RX0_CH_1, PADTOP_BANK, REG_PSPI0_G_MODE, REG_PSPI0_G_MODE_MASK, BIT0, PINMUX_FOR_PSPI0_G_MODE_1},
    {PAD_OUTN_RX0_CH_1, PADTOP_BANK, REG_PSPI0_SENSOR_MODE, REG_PSPI0_SENSOR_MODE_MASK, BIT0,
     PINMUX_FOR_PSPI0_SENSOR_MODE_1},
    {PAD_OUTN_RX0_CH_1, PADTOP_BANK, REG_PSPI0_SENSOR_MODE, REG_PSPI0_SENSOR_MODE_MASK, BIT1,
     PINMUX_FOR_PSPI0_SENSOR_MODE_2},
};
const ST_PadMuxInfo outp_rx0_ch_2_tbl[] = {
    {PAD_OUTP_RX0_CH_2, PADGPIO_BANK, REG_OUTP_RX0_CH_2_GPIO_MODE, REG_OUTP_RX0_CH_2_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTP_RX0_CH_2, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_OUTP_RX0_CH_2, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT0, PINMUX_FOR_SR0_MIPI_MODE_1},
    {PAD_OUTP_RX0_CH_2, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT1, PINMUX_FOR_SR0_MIPI_MODE_2},
    {PAD_OUTP_RX0_CH_2, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2, PINMUX_FOR_SR0_MIPI_MODE_4},
    {PAD_OUTP_RX0_CH_2, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT1,
     PINMUX_FOR_SR0_MIPI_MODE_6},
    {PAD_OUTP_RX0_CH_2, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT1 | BIT0,
     PINMUX_FOR_SR0_MIPI_MODE_7},
    {PAD_OUTP_RX0_CH_2, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT0, PINMUX_FOR_SR0_MODE_1},
    {PAD_OUTP_RX0_CH_2, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT1, PINMUX_FOR_SR0_MODE_2},
    {PAD_OUTP_RX0_CH_2, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_SR0_MODE_3},
    {PAD_OUTP_RX0_CH_2, PADTOP_BANK, REG_SR0_BT656_MODE, REG_SR0_BT656_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT656_MODE_1},
    {PAD_OUTP_RX0_CH_2, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT1120_MODE_1},
    {PAD_OUTP_RX0_CH_2, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT1, PINMUX_FOR_SR0_BT1120_MODE_2},
    {PAD_OUTP_RX0_CH_2, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTP_RX0_CH_2, PADTOP_BANK, REG_PSPI0_G_MODE, REG_PSPI0_G_MODE_MASK, BIT0, PINMUX_FOR_PSPI0_G_MODE_1},
    {PAD_OUTP_RX0_CH_2, PADTOP_BANK, REG_PSPI0_SENSOR_MODE, REG_PSPI0_SENSOR_MODE_MASK, BIT0,
     PINMUX_FOR_PSPI0_SENSOR_MODE_1},
};
const ST_PadMuxInfo outn_rx0_ch_2_tbl[] = {
    {PAD_OUTN_RX0_CH_2, PADGPIO_BANK, REG_OUTN_RX0_CH_2_GPIO_MODE, REG_OUTN_RX0_CH_2_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTN_RX0_CH_2, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_OUTN_RX0_CH_2, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT0, PINMUX_FOR_SR0_MIPI_MODE_1},
    {PAD_OUTN_RX0_CH_2, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT1, PINMUX_FOR_SR0_MIPI_MODE_2},
    {PAD_OUTN_RX0_CH_2, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2, PINMUX_FOR_SR0_MIPI_MODE_4},
    {PAD_OUTN_RX0_CH_2, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT1,
     PINMUX_FOR_SR0_MIPI_MODE_6},
    {PAD_OUTN_RX0_CH_2, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT1 | BIT0,
     PINMUX_FOR_SR0_MIPI_MODE_7},
    {PAD_OUTN_RX0_CH_2, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT0, PINMUX_FOR_SR0_MODE_1},
    {PAD_OUTN_RX0_CH_2, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT1, PINMUX_FOR_SR0_MODE_2},
    {PAD_OUTN_RX0_CH_2, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_SR0_MODE_3},
    {PAD_OUTN_RX0_CH_2, PADTOP_BANK, REG_SR0_BT656_MODE, REG_SR0_BT656_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT656_MODE_1},
    {PAD_OUTN_RX0_CH_2, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT1120_MODE_1},
    {PAD_OUTN_RX0_CH_2, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT1, PINMUX_FOR_SR0_BT1120_MODE_2},
    {PAD_OUTN_RX0_CH_2, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTN_RX0_CH_2, PADTOP_BANK, REG_PSPI0_G_MODE, REG_PSPI0_G_MODE_MASK, BIT0, PINMUX_FOR_PSPI0_G_MODE_1},
    {PAD_OUTN_RX0_CH_2, PADTOP_BANK, REG_PSPI0_SENSOR_MODE, REG_PSPI0_SENSOR_MODE_MASK, BIT0,
     PINMUX_FOR_PSPI0_SENSOR_MODE_1},
};
const ST_PadMuxInfo outp_rx0_ch_3_tbl[] = {
    {PAD_OUTP_RX0_CH_3, PADGPIO_BANK, REG_OUTP_RX0_CH_3_GPIO_MODE, REG_OUTP_RX0_CH_3_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTP_RX0_CH_3, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_OUTP_RX0_CH_3, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT0, PINMUX_FOR_SR0_MIPI_MODE_1},
    {PAD_OUTP_RX0_CH_3, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_SR0_MIPI_MODE_3},
    {PAD_OUTP_RX0_CH_3, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT1,
     PINMUX_FOR_SR0_MIPI_MODE_6},
    {PAD_OUTP_RX0_CH_3, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT0, PINMUX_FOR_SR0_MODE_1},
    {PAD_OUTP_RX0_CH_3, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT1, PINMUX_FOR_SR0_MODE_2},
    {PAD_OUTP_RX0_CH_3, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_SR0_MODE_3},
    {PAD_OUTP_RX0_CH_3, PADTOP_BANK, REG_SR0_BT656_MODE, REG_SR0_BT656_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT656_MODE_1},
    {PAD_OUTP_RX0_CH_3, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT1120_MODE_1},
    {PAD_OUTP_RX0_CH_3, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT1, PINMUX_FOR_SR0_BT1120_MODE_2},
    {PAD_OUTP_RX0_CH_3, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTP_RX0_CH_3, PADTOP_BANK, REG_PSPI0_G_MODE, REG_PSPI0_G_MODE_MASK, BIT0, PINMUX_FOR_PSPI0_G_MODE_1},
    {PAD_OUTP_RX0_CH_3, PADTOP_BANK, REG_PSPI0_VSYNC_MODE, REG_PSPI0_VSYNC_MODE_MASK, BIT6,
     PINMUX_FOR_PSPI0_VSYNC_MODE_1},
};
const ST_PadMuxInfo outn_rx0_ch_3_tbl[] = {
    {PAD_OUTN_RX0_CH_3, PADGPIO_BANK, REG_OUTN_RX0_CH_3_GPIO_MODE, REG_OUTN_RX0_CH_3_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTN_RX0_CH_3, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_OUTN_RX0_CH_3, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT0, PINMUX_FOR_SR0_MIPI_MODE_1},
    {PAD_OUTN_RX0_CH_3, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_SR0_MIPI_MODE_3},
    {PAD_OUTN_RX0_CH_3, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT1,
     PINMUX_FOR_SR0_MIPI_MODE_6},
    {PAD_OUTN_RX0_CH_3, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT0, PINMUX_FOR_SR0_MODE_1},
    {PAD_OUTN_RX0_CH_3, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT1, PINMUX_FOR_SR0_MODE_2},
    {PAD_OUTN_RX0_CH_3, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_SR0_MODE_3},
    {PAD_OUTN_RX0_CH_3, PADTOP_BANK, REG_SR0_BT656_MODE, REG_SR0_BT656_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT656_MODE_1},
    {PAD_OUTN_RX0_CH_3, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT1120_MODE_1},
    {PAD_OUTN_RX0_CH_3, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT1, PINMUX_FOR_SR0_BT1120_MODE_2},
    {PAD_OUTN_RX0_CH_3, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTN_RX0_CH_3, PADTOP_BANK, REG_PSPI0_G_MODE, REG_PSPI0_G_MODE_MASK, BIT0, PINMUX_FOR_PSPI0_G_MODE_1},
    {PAD_OUTN_RX0_CH_3, PADTOP_BANK, REG_PSPI0_CS2_MODE, REG_PSPI0_CS2_MODE_MASK, BIT2, PINMUX_FOR_PSPI0_CS2_MODE_1},
};
const ST_PadMuxInfo outp_rx0_ch_4_tbl[] = {
    {PAD_OUTP_RX0_CH_4, PADGPIO_BANK, REG_OUTP_RX0_CH_4_GPIO_MODE, REG_OUTP_RX0_CH_4_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTP_RX0_CH_4, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_OUTP_RX0_CH_4, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT0, PINMUX_FOR_SR0_MIPI_MODE_1},
    {PAD_OUTP_RX0_CH_4, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_SR0_MIPI_MODE_3},
    {PAD_OUTP_RX0_CH_4, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT0,
     PINMUX_FOR_SR0_MIPI_MODE_5},
    {PAD_OUTP_RX0_CH_4, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT1,
     PINMUX_FOR_SR0_MIPI_MODE_6},
    {PAD_OUTP_RX0_CH_4, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT1 | BIT0,
     PINMUX_FOR_SR0_MIPI_MODE_7},
    {PAD_OUTP_RX0_CH_4, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT0, PINMUX_FOR_SR0_MODE_1},
    {PAD_OUTP_RX0_CH_4, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT1, PINMUX_FOR_SR0_MODE_2},
    {PAD_OUTP_RX0_CH_4, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_SR0_MODE_3},
    {PAD_OUTP_RX0_CH_4, PADTOP_BANK, REG_SR0_BT656_MODE, REG_SR0_BT656_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT656_MODE_1},
    {PAD_OUTP_RX0_CH_4, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT0, PINMUX_FOR_SR0_BT1120_MODE_1},
    {PAD_OUTP_RX0_CH_4, PADTOP_BANK, REG_PSPI0_G_MODE, REG_PSPI0_G_MODE_MASK, BIT0, PINMUX_FOR_PSPI0_G_MODE_1},
};
const ST_PadMuxInfo outn_rx0_ch_4_tbl[] = {
    {PAD_OUTN_RX0_CH_4, PADGPIO_BANK, REG_OUTN_RX0_CH_4_GPIO_MODE, REG_OUTN_RX0_CH_4_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTN_RX0_CH_4, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_OUTN_RX0_CH_4, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT0, PINMUX_FOR_SR0_MIPI_MODE_1},
    {PAD_OUTN_RX0_CH_4, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_SR0_MIPI_MODE_3},
    {PAD_OUTN_RX0_CH_4, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT0,
     PINMUX_FOR_SR0_MIPI_MODE_5},
    {PAD_OUTN_RX0_CH_4, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT1,
     PINMUX_FOR_SR0_MIPI_MODE_6},
    {PAD_OUTN_RX0_CH_4, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT1 | BIT0,
     PINMUX_FOR_SR0_MIPI_MODE_7},
    {PAD_OUTN_RX0_CH_4, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT0, PINMUX_FOR_SR0_MODE_1},
    {PAD_OUTN_RX0_CH_4, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT1, PINMUX_FOR_SR0_MODE_2},
    {PAD_OUTN_RX0_CH_4, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_SR0_MODE_3},
    {PAD_OUTN_RX0_CH_4, PADTOP_BANK, REG_PSPI0_G_MODE, REG_PSPI0_G_MODE_MASK, BIT0, PINMUX_FOR_PSPI0_G_MODE_1},
};
const ST_PadMuxInfo outp_rx0_ch_5_tbl[] = {
    {PAD_OUTP_RX0_CH_5, PADGPIO_BANK, REG_OUTP_RX0_CH_5_GPIO_MODE, REG_OUTP_RX0_CH_5_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTP_RX0_CH_5, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_OUTP_RX0_CH_5, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_SR0_MIPI_MODE_3},
    {PAD_OUTP_RX0_CH_5, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT0,
     PINMUX_FOR_SR0_MIPI_MODE_5},
    {PAD_OUTP_RX0_CH_5, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT1,
     PINMUX_FOR_SR0_MIPI_MODE_6},
    {PAD_OUTP_RX0_CH_5, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT1 | BIT0,
     PINMUX_FOR_SR0_MIPI_MODE_7},
    {PAD_OUTP_RX0_CH_5, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT0, PINMUX_FOR_SR0_MODE_1},
    {PAD_OUTP_RX0_CH_5, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT1, PINMUX_FOR_SR0_MODE_2},
    {PAD_OUTP_RX0_CH_5, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_SR0_MODE_3},
    {PAD_OUTP_RX0_CH_5, PADTOP_BANK, REG_PSPI0_G_MODE, REG_PSPI0_G_MODE_MASK, BIT0, PINMUX_FOR_PSPI0_G_MODE_1},
};
const ST_PadMuxInfo outn_rx0_ch_5_tbl[] = {
    {PAD_OUTN_RX0_CH_5, PADGPIO_BANK, REG_OUTN_RX0_CH_5_GPIO_MODE, REG_OUTN_RX0_CH_5_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTN_RX0_CH_5, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_OUTN_RX0_CH_5, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_SR0_MIPI_MODE_3},
    {PAD_OUTN_RX0_CH_5, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT0,
     PINMUX_FOR_SR0_MIPI_MODE_5},
    {PAD_OUTN_RX0_CH_5, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT1,
     PINMUX_FOR_SR0_MIPI_MODE_6},
    {PAD_OUTN_RX0_CH_5, PADTOP_BANK, REG_SR0_MIPI_MODE, REG_SR0_MIPI_MODE_MASK, BIT2 | BIT1 | BIT0,
     PINMUX_FOR_SR0_MIPI_MODE_7},
    {PAD_OUTN_RX0_CH_5, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT0, PINMUX_FOR_SR0_MODE_1},
    {PAD_OUTN_RX0_CH_5, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT1, PINMUX_FOR_SR0_MODE_2},
    {PAD_OUTN_RX0_CH_5, PADTOP_BANK, REG_SR0_MODE, REG_SR0_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_SR0_MODE_3},
    {PAD_OUTN_RX0_CH_5, PADTOP_BANK, REG_SR1_BT656_MODE, REG_SR1_BT656_MODE_MASK, BIT2, PINMUX_FOR_SR1_BT656_MODE_1},
    {PAD_OUTN_RX0_CH_5, PADTOP_BANK, REG_SR0_BT1120_MODE, REG_SR0_BT1120_MODE_MASK, BIT1, PINMUX_FOR_SR0_BT1120_MODE_2},
    {PAD_OUTN_RX0_CH_5, PADTOP_BANK, REG_PSPI0_G_MODE, REG_PSPI0_G_MODE_MASK, BIT0, PINMUX_FOR_PSPI0_G_MODE_1},
};
const ST_PadMuxInfo spdif_tx_tbl[] = {
    {PAD_SPDIF_TX, PADGPIO_BANK, REG_SPDIF_TX_GPIO_MODE, REG_SPDIF_TX_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SPDIF_TX, PADTOP_BANK, REG_DMIC_2CH_MODE, REG_DMIC_2CH_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_DMIC_2CH_MODE_3},
    {PAD_SPDIF_TX, PADTOP_BANK, REG_DMIC_4CH_MODE, REG_DMIC_4CH_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_DMIC_4CH_MODE_3},
    {PAD_SPDIF_TX, PADTOP_BANK, REG_DMIC_8CH_MODE, REG_DMIC_8CH_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_DMIC_8CH_MODE_3},
    {PAD_SPDIF_TX, PADTOP_BANK, REG_SR_SLAVE_MODE, REG_SR_SLAVE_MODE_MASK, BIT13, PINMUX_FOR_SR_SLAVE_MODE_2},
    {PAD_SPDIF_TX, PADTOP_BANK, REG_SR_SLAVE_MODE, REG_SR_SLAVE_MODE_MASK, BIT13 | BIT12, PINMUX_FOR_SR_SLAVE_MODE_3},
    {PAD_SPDIF_TX, PADTOP_BANK, REG_SPDIF_MODE, REG_SPDIF_MODE_MASK, BIT0, PINMUX_FOR_SPDIF_MODE_1},
    {PAD_SPDIF_TX, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo sr_io3_tbl[] = {
    {PAD_SR_IO3, PADGPIO_BANK, REG_SR_IO3_GPIO_MODE, REG_SR_IO3_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SR_IO3, PADTOP_BANK, REG_DMIC_8CH_MODE, REG_DMIC_8CH_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_DMIC_8CH_MODE_3},
    {PAD_SR_IO3, PADTOP_BANK, REG_SR_SLAVE_XLK_MODE, REG_SR_SLAVE_XLK_MODE_MASK, BIT0, PINMUX_FOR_SR_SLAVE_XLK_MODE_1},
    {PAD_SR_IO3, PADTOP_BANK, REG_IR3_MODE, REG_IR3_MODE_MASK, BIT7, PINMUX_FOR_IR3_MODE_2},
    {PAD_SR_IO3, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo sr_io2_tbl[] = {
    {PAD_SR_IO2, PADGPIO_BANK, REG_SR_IO2_GPIO_MODE, REG_SR_IO2_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SR_IO2, PADTOP_BANK, REG_DMIC_2CH_MODE, REG_DMIC_2CH_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_DMIC_2CH_MODE_3},
    {PAD_SR_IO2, PADTOP_BANK, REG_DMIC_4CH_MODE, REG_DMIC_4CH_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_DMIC_4CH_MODE_3},
    {PAD_SR_IO2, PADTOP_BANK, REG_DMIC_8CH_MODE, REG_DMIC_8CH_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_DMIC_8CH_MODE_3},
    {PAD_SR_IO2, PADTOP_BANK, REG_SR_SLAVE_MODE, REG_SR_SLAVE_MODE_MASK, BIT12, PINMUX_FOR_SR_SLAVE_MODE_1},
    {PAD_SR_IO2, PADTOP_BANK, REG_IR2_MODE, REG_IR2_MODE_MASK, BIT5, PINMUX_FOR_IR2_MODE_2},
    {PAD_SR_IO2, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo sr_io1_tbl[] = {
    {PAD_SR_IO1, PADGPIO_BANK, REG_SR_IO1_GPIO_MODE, REG_SR_IO1_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SR_IO1, PADTOP_BANK, REG_DMIC_4CH_MODE, REG_DMIC_4CH_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_DMIC_4CH_MODE_3},
    {PAD_SR_IO1, PADTOP_BANK, REG_DMIC_8CH_MODE, REG_DMIC_8CH_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_DMIC_8CH_MODE_3},
    {PAD_SR_IO1, PADTOP_BANK, REG_SR_SLAVE_MODE, REG_SR_SLAVE_MODE_MASK, BIT12, PINMUX_FOR_SR_SLAVE_MODE_1},
    {PAD_SR_IO1, PADTOP_BANK, REG_IR1_MODE, REG_IR1_MODE_MASK, BIT3, PINMUX_FOR_IR1_MODE_2},
    {PAD_SR_IO1, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo sr_io0_tbl[] = {
    {PAD_SR_IO0, PADGPIO_BANK, REG_SR_IO0_GPIO_MODE, REG_SR_IO0_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SR_IO0, PADTOP_BANK, REG_DMIC_8CH_MODE, REG_DMIC_8CH_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_DMIC_8CH_MODE_3},
    {PAD_SR_IO0, PADTOP_BANK, REG_SR_SLAVE_MODE, REG_SR_SLAVE_MODE_MASK, BIT12, PINMUX_FOR_SR_SLAVE_MODE_1},
    {PAD_SR_IO0, PADTOP_BANK, REG_IR0_MODE, REG_IR0_MODE_MASK, BIT1, PINMUX_FOR_IR0_MODE_2},
    {PAD_SR_IO0, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo sr_pdn1_tbl[] = {
    {PAD_SR_PDN1, PADGPIO_BANK, REG_SR_PDN1_GPIO_MODE, REG_SR_PDN1_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SR_PDN1, PADTOP_BANK, REG_I2S1_MCK_MODE, REG_I2S1_MCK_MODE_MASK, BIT5, PINMUX_FOR_I2S1_MCK_MODE_2},
    {PAD_SR_PDN1, PADTOP_BANK, REG_DMIC_2CH_MODE, REG_DMIC_2CH_MODE_MASK, BIT1, PINMUX_FOR_DMIC_2CH_MODE_2},
    {PAD_SR_PDN1, PADTOP_BANK, REG_DMIC_4CH_MODE, REG_DMIC_4CH_MODE_MASK, BIT1, PINMUX_FOR_DMIC_4CH_MODE_2},
    {PAD_SR_PDN1, PADTOP_BANK, REG_DMIC_8CH_MODE, REG_DMIC_8CH_MODE_MASK, BIT5, PINMUX_FOR_DMIC_8CH_MODE_2},
    {PAD_SR_PDN1, PADTOP_BANK, REG_SR01_PDN_MODE, REG_SR01_PDN_MODE_MASK, BIT2, PINMUX_FOR_SR01_PDN_MODE_1},
    {PAD_SR_PDN1, PADTOP_BANK, REG_SR0_SYNC_MODE, REG_SR0_SYNC_MODE_MASK, BIT9, PINMUX_FOR_SR0_SYNC_MODE_2},
    {PAD_SR_PDN1, PADTOP_BANK, REG_SR_SLAVE_MODE, REG_SR_SLAVE_MODE_MASK, BIT13, PINMUX_FOR_SR_SLAVE_MODE_2},
    {PAD_SR_PDN1, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo sr_mclk1_tbl[] = {
    {PAD_SR_MCLK1, PADGPIO_BANK, REG_SR_MCLK1_GPIO_MODE, REG_SR_MCLK1_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SR_MCLK1, PADTOP_BANK, REG_I2S1_RX_MODE, REG_I2S1_RX_MODE_MASK, BIT1, PINMUX_FOR_I2S1_RX_MODE_2},
    {PAD_SR_MCLK1, PADTOP_BANK, REG_I2S1_TX_MODE, REG_I2S1_TX_MODE_MASK, BIT5, PINMUX_FOR_I2S1_TX_MODE_2},
    {PAD_SR_MCLK1, PADTOP_BANK, REG_I2S1_RXTX_MODE, REG_I2S1_RXTX_MODE_MASK, BIT5, PINMUX_FOR_I2S1_RXTX_MODE_2},
    {PAD_SR_MCLK1, PADTOP_BANK, REG_DMIC_2CH_MODE, REG_DMIC_2CH_MODE_MASK, BIT1, PINMUX_FOR_DMIC_2CH_MODE_2},
    {PAD_SR_MCLK1, PADTOP_BANK, REG_DMIC_4CH_MODE, REG_DMIC_4CH_MODE_MASK, BIT1, PINMUX_FOR_DMIC_4CH_MODE_2},
    {PAD_SR_MCLK1, PADTOP_BANK, REG_DMIC_8CH_MODE, REG_DMIC_8CH_MODE_MASK, BIT5, PINMUX_FOR_DMIC_8CH_MODE_2},
    {PAD_SR_MCLK1, PADTOP_BANK, REG_SR01_MCLK_MODE, REG_SR01_MCLK_MODE_MASK, BIT3, PINMUX_FOR_SR01_MCLK_MODE_1},
    {PAD_SR_MCLK1, PADTOP_BANK, REG_SR0_PCLK_MODE, REG_SR0_PCLK_MODE_MASK, BIT1, PINMUX_FOR_SR0_PCLK_MODE_2},
    {PAD_SR_MCLK1, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo sr_rst1_tbl[] = {
    {PAD_SR_RST1, PADGPIO_BANK, REG_SR_RST1_GPIO_MODE, REG_SR_RST1_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SR_RST1, PADTOP_BANK, REG_I2S1_RX_MODE, REG_I2S1_RX_MODE_MASK, BIT1, PINMUX_FOR_I2S1_RX_MODE_2},
    {PAD_SR_RST1, PADTOP_BANK, REG_I2S1_TX_MODE, REG_I2S1_TX_MODE_MASK, BIT5, PINMUX_FOR_I2S1_TX_MODE_2},
    {PAD_SR_RST1, PADTOP_BANK, REG_I2S1_RXTX_MODE, REG_I2S1_RXTX_MODE_MASK, BIT5, PINMUX_FOR_I2S1_RXTX_MODE_2},
    {PAD_SR_RST1, PADTOP_BANK, REG_DMIC_4CH_MODE, REG_DMIC_4CH_MODE_MASK, BIT1, PINMUX_FOR_DMIC_4CH_MODE_2},
    {PAD_SR_RST1, PADTOP_BANK, REG_DMIC_8CH_MODE, REG_DMIC_8CH_MODE_MASK, BIT5, PINMUX_FOR_DMIC_8CH_MODE_2},
    {PAD_SR_RST1, PADTOP_BANK, REG_SR01_RST_MODE, REG_SR01_RST_MODE_MASK, BIT2, PINMUX_FOR_SR01_RST_MODE_1},
    {PAD_SR_RST1, PADTOP_BANK, REG_SR0_SYNC_MODE, REG_SR0_SYNC_MODE_MASK, BIT9, PINMUX_FOR_SR0_SYNC_MODE_2},
    {PAD_SR_RST1, PADTOP_BANK, REG_SR_SLAVE_XLK_MODE, REG_SR_SLAVE_XLK_MODE_MASK, BIT1, PINMUX_FOR_SR_SLAVE_XLK_MODE_2},
};
const ST_PadMuxInfo i2c1_sda_tbl[] = {
    {PAD_I2C1_SDA, PADGPIO_BANK, REG_I2C1_SDA_GPIO_MODE, REG_I2C1_SDA_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_I2C1_SDA, PADTOP_BANK, REG_I2C0_MODE, REG_I2C0_MODE_MASK, BIT2, PINMUX_FOR_I2C0_MODE_4},
    {PAD_I2C1_SDA, PADTOP_BANK, REG_I2C1_MODE, REG_I2C1_MODE_MASK, BIT0, PINMUX_FOR_I2C1_MODE_1},
    {PAD_I2C1_SDA, PADTOP_BANK, REG_I2C1_MODE, REG_I2C1_MODE_MASK, BIT2, PINMUX_FOR_I2C1_MODE_4},
    {PAD_I2C1_SDA, PADTOP_BANK, REG_I2C1_MODE, REG_I2C1_MODE_MASK, BIT2 | BIT0, PINMUX_FOR_I2C1_MODE_5},
    {PAD_I2C1_SDA, PADTOP_BANK, REG_I2S1_RX_MODE, REG_I2S1_RX_MODE_MASK, BIT1, PINMUX_FOR_I2S1_RX_MODE_2},
    {PAD_I2C1_SDA, PADTOP_BANK, REG_I2S1_RXTX_MODE, REG_I2S1_RXTX_MODE_MASK, BIT5, PINMUX_FOR_I2S1_RXTX_MODE_2},
    {PAD_I2C1_SDA, PADTOP_BANK, REG_DMIC_8CH_MODE, REG_DMIC_8CH_MODE_MASK, BIT5, PINMUX_FOR_DMIC_8CH_MODE_2},
};
const ST_PadMuxInfo i2c1_scl_tbl[] = {
    {PAD_I2C1_SCL, PADGPIO_BANK, REG_I2C1_SCL_GPIO_MODE, REG_I2C1_SCL_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_I2C1_SCL, PADTOP_BANK, REG_I2C0_MODE, REG_I2C0_MODE_MASK, BIT2, PINMUX_FOR_I2C0_MODE_4},
    {PAD_I2C1_SCL, PADTOP_BANK, REG_I2C1_MODE, REG_I2C1_MODE_MASK, BIT0, PINMUX_FOR_I2C1_MODE_1},
    {PAD_I2C1_SCL, PADTOP_BANK, REG_I2C1_MODE, REG_I2C1_MODE_MASK, BIT2, PINMUX_FOR_I2C1_MODE_4},
    {PAD_I2C1_SCL, PADTOP_BANK, REG_I2C1_MODE, REG_I2C1_MODE_MASK, BIT2 | BIT0, PINMUX_FOR_I2C1_MODE_5},
    {PAD_I2C1_SCL, PADTOP_BANK, REG_I2S1_TX_MODE, REG_I2S1_TX_MODE_MASK, BIT5, PINMUX_FOR_I2S1_TX_MODE_2},
    {PAD_I2C1_SCL, PADTOP_BANK, REG_I2S1_RXTX_MODE, REG_I2S1_RXTX_MODE_MASK, BIT5, PINMUX_FOR_I2S1_RXTX_MODE_2},
    {PAD_I2C1_SCL, PADTOP_BANK, REG_DMIC_8CH_MODE, REG_DMIC_8CH_MODE_MASK, BIT5, PINMUX_FOR_DMIC_8CH_MODE_2},
};
const ST_PadMuxInfo sr_pdn0_tbl[] = {
    {PAD_SR_PDN0, PADGPIO_BANK, REG_SR_PDN0_GPIO_MODE, REG_SR_PDN0_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SR_PDN0, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_SR_PDN0, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_SR_PDN0, PADTOP_BANK, REG_SR00_PDN_MODE, REG_SR00_PDN_MODE_MASK, BIT0, PINMUX_FOR_SR00_PDN_MODE_1},
    {PAD_SR_PDN0, PADTOP_BANK, REG_SR_SLAVE_MODE, REG_SR_SLAVE_MODE_MASK, BIT13, PINMUX_FOR_SR_SLAVE_MODE_2},
    {PAD_SR_PDN0, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo sr_mclk0_tbl[] = {
    {PAD_SR_MCLK0, PADGPIO_BANK, REG_SR_MCLK0_GPIO_MODE, REG_SR_MCLK0_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SR_MCLK0, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_SR_MCLK0, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_SR_MCLK0, PADTOP_BANK, REG_SR00_MCLK_MODE, REG_SR00_MCLK_MODE_MASK, BIT0, PINMUX_FOR_SR00_MCLK_MODE_1},
    {PAD_SR_MCLK0, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo sr_rst0_tbl[] = {
    {PAD_SR_RST0, PADGPIO_BANK, REG_SR_RST0_GPIO_MODE, REG_SR_RST0_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SR_RST0, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_SR_RST0, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_SR_RST0, PADTOP_BANK, REG_SR00_RST_MODE, REG_SR00_RST_MODE_MASK, BIT0, PINMUX_FOR_SR00_RST_MODE_1},
    {PAD_SR_RST0, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo i2c0_sda_tbl[] = {
    {PAD_I2C0_SDA, PADGPIO_BANK, REG_I2C0_SDA_GPIO_MODE, REG_I2C0_SDA_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_I2C0_SDA, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_I2C0_SDA, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_I2C0_SDA, PADTOP_BANK, REG_I2C0_MODE, REG_I2C0_MODE_MASK, BIT0, PINMUX_FOR_I2C0_MODE_1},
    {PAD_I2C0_SDA, PADTOP_BANK, REG_I2C0_MODE, REG_I2C0_MODE_MASK, BIT2, PINMUX_FOR_I2C0_MODE_4},
    {PAD_I2C0_SDA, PADTOP_BANK, REG_I2C1_MODE, REG_I2C1_MODE_MASK, BIT2, PINMUX_FOR_I2C1_MODE_4},
    {PAD_I2C0_SDA, PADTOP_BANK, REG_I2C1_MODE, REG_I2C1_MODE_MASK, BIT2 | BIT0, PINMUX_FOR_I2C1_MODE_5},
    {PAD_I2C0_SDA, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo i2c0_scl_tbl[] = {
    {PAD_I2C0_SCL, PADGPIO_BANK, REG_I2C0_SCL_GPIO_MODE, REG_I2C0_SCL_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_I2C0_SCL, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_I2C0_SCL, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_I2C0_SCL, PADTOP_BANK, REG_I2C0_MODE, REG_I2C0_MODE_MASK, BIT0, PINMUX_FOR_I2C0_MODE_1},
    {PAD_I2C0_SCL, PADTOP_BANK, REG_I2C0_MODE, REG_I2C0_MODE_MASK, BIT2, PINMUX_FOR_I2C0_MODE_4},
    {PAD_I2C0_SCL, PADTOP_BANK, REG_I2C1_MODE, REG_I2C1_MODE_MASK, BIT2, PINMUX_FOR_I2C1_MODE_4},
    {PAD_I2C0_SCL, PADTOP_BANK, REG_I2C1_MODE, REG_I2C1_MODE_MASK, BIT2 | BIT0, PINMUX_FOR_I2C1_MODE_5},
    {PAD_I2C0_SCL, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo sr_pdn2_tbl[] = {
    {PAD_SR_PDN2, PADGPIO_BANK, REG_SR_PDN2_GPIO_MODE, REG_SR_PDN2_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SR_PDN2, PADTOP_BANK, REG_SR10_PDN_MODE, REG_SR10_PDN_MODE_MASK, BIT8, PINMUX_FOR_SR10_PDN_MODE_1},
    {PAD_SR_PDN2, PADTOP_BANK, REG_SR_SLAVE_MODE, REG_SR_SLAVE_MODE_MASK, BIT13 | BIT12, PINMUX_FOR_SR_SLAVE_MODE_3},
    {PAD_SR_PDN2, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo sr_mclk2_tbl[] = {
    {PAD_SR_MCLK2, PADGPIO_BANK, REG_SR_MCLK2_GPIO_MODE, REG_SR_MCLK2_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SR_MCLK2, PADTOP_BANK, REG_SR10_MCLK_MODE, REG_SR10_MCLK_MODE_MASK, BIT8, PINMUX_FOR_SR10_MCLK_MODE_1},
    {PAD_SR_MCLK2, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo sr_rst2_tbl[] = {
    {PAD_SR_RST2, PADGPIO_BANK, REG_SR_RST2_GPIO_MODE, REG_SR_RST2_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SR_RST2, PADTOP_BANK, REG_SR10_RST_MODE, REG_SR10_RST_MODE_MASK, BIT8, PINMUX_FOR_SR10_RST_MODE_1},
    {PAD_SR_RST2, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo i2c2_sda_tbl[] = {
    {PAD_I2C2_SDA, PADGPIO_BANK, REG_I2C2_SDA_GPIO_MODE, REG_I2C2_SDA_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_I2C2_SDA, PADTOP_BANK, REG_I2C0_MODE, REG_I2C0_MODE_MASK, BIT2, PINMUX_FOR_I2C0_MODE_4},
    {PAD_I2C2_SDA, PADTOP_BANK, REG_I2C1_MODE, REG_I2C1_MODE_MASK, BIT2 | BIT0, PINMUX_FOR_I2C1_MODE_5},
    {PAD_I2C2_SDA, PADTOP_BANK, REG_I2C2_MODE, REG_I2C2_MODE_MASK, BIT8, PINMUX_FOR_I2C2_MODE_1},
    {PAD_I2C2_SDA, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo i2c2_scl_tbl[] = {
    {PAD_I2C2_SCL, PADGPIO_BANK, REG_I2C2_SCL_GPIO_MODE, REG_I2C2_SCL_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_I2C2_SCL, PADTOP_BANK, REG_I2C0_MODE, REG_I2C0_MODE_MASK, BIT2, PINMUX_FOR_I2C0_MODE_4},
    {PAD_I2C2_SCL, PADTOP_BANK, REG_I2C1_MODE, REG_I2C1_MODE_MASK, BIT2 | BIT0, PINMUX_FOR_I2C1_MODE_5},
    {PAD_I2C2_SCL, PADTOP_BANK, REG_I2C2_MODE, REG_I2C2_MODE_MASK, BIT8, PINMUX_FOR_I2C2_MODE_1},
    {PAD_I2C2_SCL, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo sr_pdn3_tbl[] = {
    {PAD_SR_PDN3, PADGPIO_BANK, REG_SR_PDN3_GPIO_MODE, REG_SR_PDN3_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SR_PDN3, PADTOP_BANK, REG_SR11_PDN_MODE, REG_SR11_PDN_MODE_MASK, BIT10, PINMUX_FOR_SR11_PDN_MODE_1},
    {PAD_SR_PDN3, PADTOP_BANK, REG_SR_SLAVE_MODE, REG_SR_SLAVE_MODE_MASK, BIT13 | BIT12, PINMUX_FOR_SR_SLAVE_MODE_3},
    {PAD_SR_PDN3, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo sr_mclk3_tbl[] = {
    {PAD_SR_MCLK3, PADGPIO_BANK, REG_SR_MCLK3_GPIO_MODE, REG_SR_MCLK3_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SR_MCLK3, PADTOP_BANK, REG_SR11_MCLK_MODE, REG_SR11_MCLK_MODE_MASK, BIT10, PINMUX_FOR_SR11_MCLK_MODE_1},
    {PAD_SR_MCLK3, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo sr_rst3_tbl[] = {
    {PAD_SR_RST3, PADGPIO_BANK, REG_SR_RST3_GPIO_MODE, REG_SR_RST3_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SR_RST3, PADTOP_BANK, REG_SR11_RST_MODE, REG_SR11_RST_MODE_MASK, BIT10, PINMUX_FOR_SR11_RST_MODE_1},
    {PAD_SR_RST3, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo i2c3_sda_tbl[] = {
    {PAD_I2C3_SDA, PADGPIO_BANK, REG_I2C3_SDA_GPIO_MODE, REG_I2C3_SDA_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_I2C3_SDA, PADTOP_BANK, REG_I2C0_MODE, REG_I2C0_MODE_MASK, BIT2, PINMUX_FOR_I2C0_MODE_4},
    {PAD_I2C3_SDA, PADTOP_BANK, REG_I2C1_MODE, REG_I2C1_MODE_MASK, BIT2 | BIT0, PINMUX_FOR_I2C1_MODE_5},
    {PAD_I2C3_SDA, PADTOP_BANK, REG_I2C3_MODE, REG_I2C3_MODE_MASK, BIT0, PINMUX_FOR_I2C3_MODE_1},
    {PAD_I2C3_SDA, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo i2c3_scl_tbl[] = {
    {PAD_I2C3_SCL, PADGPIO_BANK, REG_I2C3_SCL_GPIO_MODE, REG_I2C3_SCL_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_I2C3_SCL, PADTOP_BANK, REG_I2C0_MODE, REG_I2C0_MODE_MASK, BIT2, PINMUX_FOR_I2C0_MODE_4},
    {PAD_I2C3_SCL, PADTOP_BANK, REG_I2C1_MODE, REG_I2C1_MODE_MASK, BIT2 | BIT0, PINMUX_FOR_I2C1_MODE_5},
    {PAD_I2C3_SCL, PADTOP_BANK, REG_I2C3_MODE, REG_I2C3_MODE_MASK, BIT0, PINMUX_FOR_I2C3_MODE_1},
    {PAD_I2C3_SCL, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo pwm_out0_tbl[] = {
    {PAD_PWM_OUT0, PADGPIO_BANK, REG_PWM_OUT0_GPIO_MODE, REG_PWM_OUT0_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PWM_OUT0, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_PWM_OUT0, PADTOP_BANK, REG_SDIO_CDZ_MODE, REG_SDIO_CDZ_MODE_MASK, BIT13 | BIT12, PINMUX_FOR_SDIO_CDZ_MODE_3},
    {PAD_PWM_OUT0, PADTOP_BANK, REG_SDIO_RSTN_MODE, REG_SDIO_RSTN_MODE_MASK, BIT15 | BIT14,
     PINMUX_FOR_SDIO_RSTN_MODE_3},
    {PAD_PWM_OUT0, PADTOP_BANK, REG_PWM0_PN, REG_PWM0_PN_MASK, BIT0, PINMUX_FOR_PWM0_PN_1},
    {PAD_PWM_OUT0, PADTOP_BANK, REG_PWM_OUT0_MODE, REG_PWM_OUT0_MODE_MASK, BIT0, PINMUX_FOR_PWM_OUT0_MODE_1},
    {PAD_PWM_OUT0, PADTOP_BANK, REG_I2S1_MCK_MODE, REG_I2S1_MCK_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_I2S1_MCK_MODE_3},
    {PAD_PWM_OUT0, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_PWM_OUT0, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT4, PINMUX_FOR_TTL18_MODE_1},
    {PAD_PWM_OUT0, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT0, PINMUX_FOR_TTL16_MODE_1},
};
const ST_PadMuxInfo pwm_out1_tbl[] = {
    {PAD_PWM_OUT1, PADGPIO_BANK, REG_PWM_OUT1_GPIO_MODE, REG_PWM_OUT1_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PWM_OUT1, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_PWM_OUT1, PADTOP_BANK, REG_UART5_MODE, REG_UART5_MODE_MASK, BIT4, PINMUX_FOR_UART5_MODE_1},
    {PAD_PWM_OUT1, PADTOP_BANK, REG_SDIO_MODE, REG_SDIO_MODE_MASK, BIT13 | BIT12, PINMUX_FOR_SDIO_MODE_3},
    {PAD_PWM_OUT1, PADTOP_BANK, REG_PWM0_PN, REG_PWM0_PN_MASK, BIT0, PINMUX_FOR_PWM0_PN_1},
    {PAD_PWM_OUT1, PADTOP_BANK, REG_PWM_OUT1_MODE, REG_PWM_OUT1_MODE_MASK, BIT4, PINMUX_FOR_PWM_OUT1_MODE_1},
    {PAD_PWM_OUT1, PADTOP_BANK, REG_I2S1_TX_MODE, REG_I2S1_TX_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_I2S1_TX_MODE_3},
    {PAD_PWM_OUT1, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_PWM_OUT1, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT4, PINMUX_FOR_TTL18_MODE_1},
    {PAD_PWM_OUT1, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT0, PINMUX_FOR_TTL16_MODE_1},
};
const ST_PadMuxInfo pwm_out2_tbl[] = {
    {PAD_PWM_OUT2, PADGPIO_BANK, REG_PWM_OUT2_GPIO_MODE, REG_PWM_OUT2_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PWM_OUT2, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_PWM_OUT2, PADTOP_BANK, REG_UART5_MODE, REG_UART5_MODE_MASK, BIT4, PINMUX_FOR_UART5_MODE_1},
    {PAD_PWM_OUT2, PADTOP_BANK, REG_SDIO_MODE, REG_SDIO_MODE_MASK, BIT13 | BIT12, PINMUX_FOR_SDIO_MODE_3},
    {PAD_PWM_OUT2, PADTOP_BANK, REG_PWM1_PN, REG_PWM1_PN_MASK, BIT2, PINMUX_FOR_PWM1_PN_1},
    {PAD_PWM_OUT2, PADTOP_BANK, REG_PWM_OUT2_MODE, REG_PWM_OUT2_MODE_MASK, BIT8, PINMUX_FOR_PWM_OUT2_MODE_1},
    {PAD_PWM_OUT2, PADTOP_BANK, REG_I2S1_TX_MODE, REG_I2S1_TX_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_I2S1_TX_MODE_3},
    {PAD_PWM_OUT2, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_PWM_OUT2, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT4, PINMUX_FOR_TTL18_MODE_1},
    {PAD_PWM_OUT2, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT0, PINMUX_FOR_TTL16_MODE_1},
};
const ST_PadMuxInfo pwm_out3_tbl[] = {
    {PAD_PWM_OUT3, PADGPIO_BANK, REG_PWM_OUT3_GPIO_MODE, REG_PWM_OUT3_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PWM_OUT3, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_PWM_OUT3, PADTOP_BANK, REG_SDIO_MODE, REG_SDIO_MODE_MASK, BIT13 | BIT12, PINMUX_FOR_SDIO_MODE_3},
    {PAD_PWM_OUT3, PADTOP_BANK, REG_PWM1_PN, REG_PWM1_PN_MASK, BIT2, PINMUX_FOR_PWM1_PN_1},
    {PAD_PWM_OUT3, PADTOP_BANK, REG_PWM_OUT3_MODE, REG_PWM_OUT3_MODE_MASK, BIT12, PINMUX_FOR_PWM_OUT3_MODE_1},
    {PAD_PWM_OUT3, PADTOP_BANK, REG_I2S1_RX_MODE, REG_I2S1_RX_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_I2S1_RX_MODE_3},
    {PAD_PWM_OUT3, PADTOP_BANK, REG_I2S1_RXTX_MODE, REG_I2S1_RXTX_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_I2S1_RXTX_MODE_3},
    {PAD_PWM_OUT3, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_PWM_OUT3, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT4, PINMUX_FOR_TTL18_MODE_1},
    {PAD_PWM_OUT3, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT0, PINMUX_FOR_TTL16_MODE_1},
};
const ST_PadMuxInfo pwm_out4_tbl[] = {
    {PAD_PWM_OUT4, PADGPIO_BANK, REG_PWM_OUT4_GPIO_MODE, REG_PWM_OUT4_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PWM_OUT4, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_PWM_OUT4, PADTOP_BANK, REG_SDIO_MODE, REG_SDIO_MODE_MASK, BIT13 | BIT12, PINMUX_FOR_SDIO_MODE_3},
    {PAD_PWM_OUT4, PADTOP_BANK, REG_LED0_MODE, REG_LED0_MODE_MASK, BIT0, PINMUX_FOR_LED0_MODE_1},
    {PAD_PWM_OUT4, PADTOP_BANK, REG_PWM2_PN, REG_PWM2_PN_MASK, BIT4, PINMUX_FOR_PWM2_PN_1},
    {PAD_PWM_OUT4, PADTOP_BANK, REG_PWM_OUT4_MODE, REG_PWM_OUT4_MODE_MASK, BIT0, PINMUX_FOR_PWM_OUT4_MODE_1},
    {PAD_PWM_OUT4, PADTOP_BANK, REG_I2S1_RX_MODE, REG_I2S1_RX_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_I2S1_RX_MODE_3},
    {PAD_PWM_OUT4, PADTOP_BANK, REG_I2S1_RXTX_MODE, REG_I2S1_RXTX_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_I2S1_RXTX_MODE_3},
};
const ST_PadMuxInfo pwm_out5_tbl[] = {
    {PAD_PWM_OUT5, PADGPIO_BANK, REG_PWM_OUT5_GPIO_MODE, REG_PWM_OUT5_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PWM_OUT5, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_PWM_OUT5, PADTOP_BANK, REG_SDIO_MODE, REG_SDIO_MODE_MASK, BIT13 | BIT12, PINMUX_FOR_SDIO_MODE_3},
    {PAD_PWM_OUT5, PADTOP_BANK, REG_LED1_MODE, REG_LED1_MODE_MASK, BIT4, PINMUX_FOR_LED1_MODE_1},
    {PAD_PWM_OUT5, PADTOP_BANK, REG_PWM2_PN, REG_PWM2_PN_MASK, BIT4, PINMUX_FOR_PWM2_PN_1},
    {PAD_PWM_OUT5, PADTOP_BANK, REG_PWM_OUT5_MODE, REG_PWM_OUT5_MODE_MASK, BIT8, PINMUX_FOR_PWM_OUT5_MODE_1},
    {PAD_PWM_OUT5, PADTOP_BANK, REG_I2S1_RX_MODE, REG_I2S1_RX_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_I2S1_RX_MODE_3},
    {PAD_PWM_OUT5, PADTOP_BANK, REG_I2S1_RXTX_MODE, REG_I2S1_RXTX_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_I2S1_RXTX_MODE_3},
};
const ST_PadMuxInfo pwm_out6_tbl[] = {
    {PAD_PWM_OUT6, PADGPIO_BANK, REG_PWM_OUT6_GPIO_MODE, REG_PWM_OUT6_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PWM_OUT6, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_PWM_OUT6, PADTOP_BANK, REG_SDIO_MODE, REG_SDIO_MODE_MASK, BIT13 | BIT12, PINMUX_FOR_SDIO_MODE_3},
    {PAD_PWM_OUT6, PADTOP_BANK, REG_PWM3_PN, REG_PWM3_PN_MASK, BIT6, PINMUX_FOR_PWM3_PN_1},
    {PAD_PWM_OUT6, PADTOP_BANK, REG_PWM_OUT6_MODE, REG_PWM_OUT6_MODE_MASK, BIT10, PINMUX_FOR_PWM_OUT6_MODE_1},
    {PAD_PWM_OUT6, PADTOP_BANK, REG_I2S1_TX_MODE, REG_I2S1_TX_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_I2S1_TX_MODE_3},
    {PAD_PWM_OUT6, PADTOP_BANK, REG_I2S1_RXTX_MODE, REG_I2S1_RXTX_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_I2S1_RXTX_MODE_3},
};
const ST_PadMuxInfo pwm_out7_tbl[] = {
    {PAD_PWM_OUT7, PADGPIO_BANK, REG_PWM_OUT7_GPIO_MODE, REG_PWM_OUT7_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PWM_OUT7, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_PWM_OUT7, PADTOP_BANK, REG_PWM3_PN, REG_PWM3_PN_MASK, BIT6, PINMUX_FOR_PWM3_PN_1},
    {PAD_PWM_OUT7, PADTOP_BANK, REG_PWM_OUT7_MODE, REG_PWM_OUT7_MODE_MASK, BIT12, PINMUX_FOR_PWM_OUT7_MODE_1},
    {PAD_PWM_OUT7, PADTOP_BANK, REG_HDMI_CEC_MODE, REG_HDMI_CEC_MODE_MASK, BIT4, PINMUX_FOR_HDMI_CEC_MODE_1},
};
const ST_PadMuxInfo pwm_out8_tbl[] = {
    {PAD_PWM_OUT8, PADGPIO_BANK, REG_PWM_OUT8_GPIO_MODE, REG_PWM_OUT8_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PWM_OUT8, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_PWM_OUT8, PADTOP_BANK, REG_I2C3_MODE, REG_I2C3_MODE_MASK, BIT1, PINMUX_FOR_I2C3_MODE_2},
    {PAD_PWM_OUT8, PADTOP_BANK, REG_PWM4_PN, REG_PWM4_PN_MASK, BIT8, PINMUX_FOR_PWM4_PN_1},
    {PAD_PWM_OUT8, PADTOP_BANK, REG_PWM_OUT8_MODE, REG_PWM_OUT8_MODE_MASK, BIT0, PINMUX_FOR_PWM_OUT8_MODE_1},
    {PAD_PWM_OUT8, PADTOP_BANK, REG_HDMI_HTP_MODE, REG_HDMI_HTP_MODE_MASK, BIT8, PINMUX_FOR_HDMI_HTP_MODE_1},
};
const ST_PadMuxInfo pwm_out9_tbl[] = {
    {PAD_PWM_OUT9, PADGPIO_BANK, REG_PWM_OUT9_GPIO_MODE, REG_PWM_OUT9_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PWM_OUT9, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_PWM_OUT9, PADTOP_BANK, REG_I2C3_MODE, REG_I2C3_MODE_MASK, BIT1, PINMUX_FOR_I2C3_MODE_2},
    {PAD_PWM_OUT9, PADTOP_BANK, REG_PWM4_PN, REG_PWM4_PN_MASK, BIT8, PINMUX_FOR_PWM4_PN_1},
    {PAD_PWM_OUT9, PADTOP_BANK, REG_PWM_OUT9_MODE, REG_PWM_OUT9_MODE_MASK, BIT2, PINMUX_FOR_PWM_OUT9_MODE_1},
    {PAD_PWM_OUT9, PADTOP_BANK, REG_HDMI_5VDET_MODE, REG_HDMI_5VDET_MODE_MASK, BIT12, PINMUX_FOR_HDMI_5VDET_MODE_1},
};
const ST_PadMuxInfo pwm_out10_tbl[] = {
    {PAD_PWM_OUT10, PADGPIO_BANK, REG_PWM_OUT10_GPIO_MODE, REG_PWM_OUT10_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PWM_OUT10, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_PWM_OUT10, PADTOP_BANK, REG_I2C4_MODE, REG_I2C4_MODE_MASK, BIT5, PINMUX_FOR_I2C4_MODE_2},
    {PAD_PWM_OUT10, PADTOP_BANK, REG_LED0_MODE, REG_LED0_MODE_MASK, BIT1, PINMUX_FOR_LED0_MODE_2},
    {PAD_PWM_OUT10, PADTOP_BANK, REG_PWM5_PN, REG_PWM5_PN_MASK, BIT10, PINMUX_FOR_PWM5_PN_1},
    {PAD_PWM_OUT10, PADTOP_BANK, REG_PWM_OUT10_MODE, REG_PWM_OUT10_MODE_MASK, BIT4, PINMUX_FOR_PWM_OUT10_MODE_1},
    {PAD_PWM_OUT10, PADTOP_BANK, REG_HDMI_I2C_MODE, REG_HDMI_I2C_MODE_MASK, BIT0, PINMUX_FOR_HDMI_I2C_MODE_1},
};
const ST_PadMuxInfo pwm_out11_tbl[] = {
    {PAD_PWM_OUT11, PADGPIO_BANK, REG_PWM_OUT11_GPIO_MODE, REG_PWM_OUT11_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_PWM_OUT11, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_PWM_OUT11, PADTOP_BANK, REG_I2C4_MODE, REG_I2C4_MODE_MASK, BIT5, PINMUX_FOR_I2C4_MODE_2},
    {PAD_PWM_OUT11, PADTOP_BANK, REG_LED1_MODE, REG_LED1_MODE_MASK, BIT5, PINMUX_FOR_LED1_MODE_2},
    {PAD_PWM_OUT11, PADTOP_BANK, REG_PWM5_PN, REG_PWM5_PN_MASK, BIT10, PINMUX_FOR_PWM5_PN_1},
    {PAD_PWM_OUT11, PADTOP_BANK, REG_PWM_OUT11_MODE, REG_PWM_OUT11_MODE_MASK, BIT6, PINMUX_FOR_PWM_OUT11_MODE_1},
    {PAD_PWM_OUT11, PADTOP_BANK, REG_HDMI_I2C_MODE, REG_HDMI_I2C_MODE_MASK, BIT0, PINMUX_FOR_HDMI_I2C_MODE_1},
};
const ST_PadMuxInfo outp_tx0_ch_0_tbl[] = {
    {PAD_OUTP_TX0_CH_0, PADGPIO_BANK, REG_OUTP_TX0_CH_0_GPIO_MODE, REG_OUTP_TX0_CH_0_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTP_TX0_CH_0, PADTOP_BANK, REG_EJ_MODE, REG_EJ_MODE_MASK, BIT8 | BIT7, PINMUX_FOR_EJ_MODE_3},
    {PAD_OUTP_TX0_CH_0, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_OUTP_TX0_CH_0, PADTOP_BANK, REG_UART5_MODE, REG_UART5_MODE_MASK, BIT6, PINMUX_FOR_UART5_MODE_4},
    {PAD_OUTP_TX0_CH_0, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT8, PINMUX_FOR_LVDS_OUT_MODE_1},
    {PAD_OUTP_TX0_CH_0, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT9, PINMUX_FOR_LVDS_OUT_MODE_2},
    {PAD_OUTP_TX0_CH_0, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT0,
     PINMUX_FOR_MIPITX0_OUT_MODE_1},
    {PAD_OUTP_TX0_CH_0, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT1,
     PINMUX_FOR_MIPITX0_OUT_MODE_2},
    {PAD_OUTP_TX0_CH_0, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_MIPITX0_OUT_MODE_3},
    {PAD_OUTP_TX0_CH_0, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT2,
     PINMUX_FOR_MIPITX0_OUT_MODE_4},
    {PAD_OUTP_TX0_CH_0, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT4, PINMUX_FOR_BT601_OUT_MODE_1},
    {PAD_OUTP_TX0_CH_0, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTP_TX0_CH_0, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
    {PAD_OUTP_TX0_CH_0, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TTL18_MODE_3},
    {PAD_OUTP_TX0_CH_0, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TTL16_MODE_3},
    {PAD_OUTP_TX0_CH_0, PADTOP_BANK, REG_PSPI1_TE_MODE, REG_PSPI1_TE_MODE_MASK, BIT12, PINMUX_FOR_PSPI1_TE_MODE_1},
    {PAD_OUTP_TX0_CH_0, PADTOP_BANK, REG_IR0_MODE, REG_IR0_MODE_MASK, BIT0, PINMUX_FOR_IR0_MODE_1},
};
const ST_PadMuxInfo outn_tx0_ch_0_tbl[] = {
    {PAD_OUTN_TX0_CH_0, PADGPIO_BANK, REG_OUTN_TX0_CH_0_GPIO_MODE, REG_OUTN_TX0_CH_0_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTN_TX0_CH_0, PADTOP_BANK, REG_EJ_MODE, REG_EJ_MODE_MASK, BIT8 | BIT7, PINMUX_FOR_EJ_MODE_3},
    {PAD_OUTN_TX0_CH_0, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_OUTN_TX0_CH_0, PADTOP_BANK, REG_UART5_MODE, REG_UART5_MODE_MASK, BIT6, PINMUX_FOR_UART5_MODE_4},
    {PAD_OUTN_TX0_CH_0, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT8, PINMUX_FOR_LVDS_OUT_MODE_1},
    {PAD_OUTN_TX0_CH_0, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT9, PINMUX_FOR_LVDS_OUT_MODE_2},
    {PAD_OUTN_TX0_CH_0, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT0,
     PINMUX_FOR_MIPITX0_OUT_MODE_1},
    {PAD_OUTN_TX0_CH_0, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT1,
     PINMUX_FOR_MIPITX0_OUT_MODE_2},
    {PAD_OUTN_TX0_CH_0, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_MIPITX0_OUT_MODE_3},
    {PAD_OUTN_TX0_CH_0, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT2,
     PINMUX_FOR_MIPITX0_OUT_MODE_4},
    {PAD_OUTN_TX0_CH_0, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT4, PINMUX_FOR_BT601_OUT_MODE_1},
    {PAD_OUTN_TX0_CH_0, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTN_TX0_CH_0, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
    {PAD_OUTN_TX0_CH_0, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TTL18_MODE_3},
    {PAD_OUTN_TX0_CH_0, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TTL16_MODE_3},
    {PAD_OUTN_TX0_CH_0, PADTOP_BANK, REG_PSPI1_G_MODE, REG_PSPI1_G_MODE_MASK, BIT8, PINMUX_FOR_PSPI1_G_MODE_1},
    {PAD_OUTN_TX0_CH_0, PADTOP_BANK, REG_PSPI1_PANEL_MODE, REG_PSPI1_PANEL_MODE_MASK, BIT4,
     PINMUX_FOR_PSPI1_PANEL_MODE_1},
    {PAD_OUTN_TX0_CH_0, PADTOP_BANK, REG_IR1_MODE, REG_IR1_MODE_MASK, BIT2, PINMUX_FOR_IR1_MODE_1},
};
const ST_PadMuxInfo outp_tx0_ch_1_tbl[] = {
    {PAD_OUTP_TX0_CH_1, PADGPIO_BANK, REG_OUTP_TX0_CH_1_GPIO_MODE, REG_OUTP_TX0_CH_1_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTP_TX0_CH_1, PADTOP_BANK, REG_EJ_MODE, REG_EJ_MODE_MASK, BIT8 | BIT7, PINMUX_FOR_EJ_MODE_3},
    {PAD_OUTP_TX0_CH_1, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_OUTP_TX0_CH_1, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_OUTP_TX0_CH_1, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT8, PINMUX_FOR_LVDS_OUT_MODE_1},
    {PAD_OUTP_TX0_CH_1, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT9, PINMUX_FOR_LVDS_OUT_MODE_2},
    {PAD_OUTP_TX0_CH_1, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT0,
     PINMUX_FOR_MIPITX0_OUT_MODE_1},
    {PAD_OUTP_TX0_CH_1, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT1,
     PINMUX_FOR_MIPITX0_OUT_MODE_2},
    {PAD_OUTP_TX0_CH_1, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_MIPITX0_OUT_MODE_3},
    {PAD_OUTP_TX0_CH_1, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT4, PINMUX_FOR_BT601_OUT_MODE_1},
    {PAD_OUTP_TX0_CH_1, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTP_TX0_CH_1, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
    {PAD_OUTP_TX0_CH_1, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TTL18_MODE_3},
    {PAD_OUTP_TX0_CH_1, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TTL16_MODE_3},
    {PAD_OUTP_TX0_CH_1, PADTOP_BANK, REG_PSPI1_G_MODE, REG_PSPI1_G_MODE_MASK, BIT8, PINMUX_FOR_PSPI1_G_MODE_1},
    {PAD_OUTP_TX0_CH_1, PADTOP_BANK, REG_PSPI1_PANEL_MODE, REG_PSPI1_PANEL_MODE_MASK, BIT4,
     PINMUX_FOR_PSPI1_PANEL_MODE_1},
    {PAD_OUTP_TX0_CH_1, PADTOP_BANK, REG_IR2_MODE, REG_IR2_MODE_MASK, BIT4, PINMUX_FOR_IR2_MODE_1},
};
const ST_PadMuxInfo outn_tx0_ch_1_tbl[] = {
    {PAD_OUTN_TX0_CH_1, PADGPIO_BANK, REG_OUTN_TX0_CH_1_GPIO_MODE, REG_OUTN_TX0_CH_1_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTN_TX0_CH_1, PADTOP_BANK, REG_EJ_MODE, REG_EJ_MODE_MASK, BIT8 | BIT7, PINMUX_FOR_EJ_MODE_3},
    {PAD_OUTN_TX0_CH_1, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_OUTN_TX0_CH_1, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_OUTN_TX0_CH_1, PADTOP_BANK, REG_I2S0_MCK_MODE, REG_I2S0_MCK_MODE_MASK, BIT1, PINMUX_FOR_I2S0_MCK_MODE_2},
    {PAD_OUTN_TX0_CH_1, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT8, PINMUX_FOR_LVDS_OUT_MODE_1},
    {PAD_OUTN_TX0_CH_1, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT9, PINMUX_FOR_LVDS_OUT_MODE_2},
    {PAD_OUTN_TX0_CH_1, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT0,
     PINMUX_FOR_MIPITX0_OUT_MODE_1},
    {PAD_OUTN_TX0_CH_1, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT1,
     PINMUX_FOR_MIPITX0_OUT_MODE_2},
    {PAD_OUTN_TX0_CH_1, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_MIPITX0_OUT_MODE_3},
    {PAD_OUTN_TX0_CH_1, PADTOP_BANK, REG_BT656_OUT_MODE, REG_BT656_OUT_MODE_MASK, BIT4, PINMUX_FOR_BT656_OUT_MODE_1},
    {PAD_OUTN_TX0_CH_1, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT4, PINMUX_FOR_BT601_OUT_MODE_1},
    {PAD_OUTN_TX0_CH_1, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTN_TX0_CH_1, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
    {PAD_OUTN_TX0_CH_1, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TTL18_MODE_3},
    {PAD_OUTN_TX0_CH_1, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TTL16_MODE_3},
    {PAD_OUTN_TX0_CH_1, PADTOP_BANK, REG_PSPI1_G_MODE, REG_PSPI1_G_MODE_MASK, BIT8, PINMUX_FOR_PSPI1_G_MODE_1},
    {PAD_OUTN_TX0_CH_1, PADTOP_BANK, REG_PSPI1_PANEL_MODE, REG_PSPI1_PANEL_MODE_MASK, BIT4,
     PINMUX_FOR_PSPI1_PANEL_MODE_1},
    {PAD_OUTN_TX0_CH_1, PADTOP_BANK, REG_IR3_MODE, REG_IR3_MODE_MASK, BIT6, PINMUX_FOR_IR3_MODE_1},
};
const ST_PadMuxInfo outp_tx0_ch_2_tbl[] = {
    {PAD_OUTP_TX0_CH_2, PADGPIO_BANK, REG_OUTP_TX0_CH_2_GPIO_MODE, REG_OUTP_TX0_CH_2_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTP_TX0_CH_2, PADTOP_BANK, REG_RISCV_EJ_MODE, REG_RISCV_EJ_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_RISCV_EJ_MODE_3},
    {PAD_OUTP_TX0_CH_2, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_OUTP_TX0_CH_2, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_OUTP_TX0_CH_2, PADTOP_BANK, REG_PWM_OUT12_MODE, REG_PWM_OUT12_MODE_MASK, BIT2, PINMUX_FOR_PWM_OUT12_MODE_4},
    {PAD_OUTP_TX0_CH_2, PADTOP_BANK, REG_I2S0_RX_MODE, REG_I2S0_RX_MODE_MASK, BIT9, PINMUX_FOR_I2S0_RX_MODE_2},
    {PAD_OUTP_TX0_CH_2, PADTOP_BANK, REG_I2S0_RXTX_MODE, REG_I2S0_RXTX_MODE_MASK, BIT1, PINMUX_FOR_I2S0_RXTX_MODE_2},
    {PAD_OUTP_TX0_CH_2, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT8, PINMUX_FOR_LVDS_OUT_MODE_1},
    {PAD_OUTP_TX0_CH_2, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT9, PINMUX_FOR_LVDS_OUT_MODE_2},
    {PAD_OUTP_TX0_CH_2, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT0,
     PINMUX_FOR_MIPITX0_OUT_MODE_1},
    {PAD_OUTP_TX0_CH_2, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT1,
     PINMUX_FOR_MIPITX0_OUT_MODE_2},
    {PAD_OUTP_TX0_CH_2, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_MIPITX0_OUT_MODE_3},
    {PAD_OUTP_TX0_CH_2, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT2,
     PINMUX_FOR_MIPITX0_OUT_MODE_4},
    {PAD_OUTP_TX0_CH_2, PADTOP_BANK, REG_BT656_OUT_MODE, REG_BT656_OUT_MODE_MASK, BIT4, PINMUX_FOR_BT656_OUT_MODE_1},
    {PAD_OUTP_TX0_CH_2, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT4, PINMUX_FOR_BT601_OUT_MODE_1},
    {PAD_OUTP_TX0_CH_2, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTP_TX0_CH_2, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
    {PAD_OUTP_TX0_CH_2, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TTL18_MODE_3},
    {PAD_OUTP_TX0_CH_2, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TTL16_MODE_3},
    {PAD_OUTP_TX0_CH_2, PADTOP_BANK, REG_PSPI1_G_MODE, REG_PSPI1_G_MODE_MASK, BIT8, PINMUX_FOR_PSPI1_G_MODE_1},
    {PAD_OUTP_TX0_CH_2, PADTOP_BANK, REG_PSPI1_PANEL_MODE, REG_PSPI1_PANEL_MODE_MASK, BIT4,
     PINMUX_FOR_PSPI1_PANEL_MODE_1},
};
const ST_PadMuxInfo outn_tx0_ch_2_tbl[] = {
    {PAD_OUTN_TX0_CH_2, PADGPIO_BANK, REG_OUTN_TX0_CH_2_GPIO_MODE, REG_OUTN_TX0_CH_2_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTN_TX0_CH_2, PADTOP_BANK, REG_RISCV_EJ_MODE, REG_RISCV_EJ_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_RISCV_EJ_MODE_3},
    {PAD_OUTN_TX0_CH_2, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_OUTN_TX0_CH_2, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_OUTN_TX0_CH_2, PADTOP_BANK, REG_PWM_OUT13_MODE, REG_PWM_OUT13_MODE_MASK, BIT11, PINMUX_FOR_PWM_OUT13_MODE_2},
    {PAD_OUTN_TX0_CH_2, PADTOP_BANK, REG_I2S0_RX_MODE, REG_I2S0_RX_MODE_MASK, BIT9, PINMUX_FOR_I2S0_RX_MODE_2},
    {PAD_OUTN_TX0_CH_2, PADTOP_BANK, REG_I2S0_RXTX_MODE, REG_I2S0_RXTX_MODE_MASK, BIT1, PINMUX_FOR_I2S0_RXTX_MODE_2},
    {PAD_OUTN_TX0_CH_2, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT8, PINMUX_FOR_LVDS_OUT_MODE_1},
    {PAD_OUTN_TX0_CH_2, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT9, PINMUX_FOR_LVDS_OUT_MODE_2},
    {PAD_OUTN_TX0_CH_2, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT0,
     PINMUX_FOR_MIPITX0_OUT_MODE_1},
    {PAD_OUTN_TX0_CH_2, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT1,
     PINMUX_FOR_MIPITX0_OUT_MODE_2},
    {PAD_OUTN_TX0_CH_2, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_MIPITX0_OUT_MODE_3},
    {PAD_OUTN_TX0_CH_2, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT2,
     PINMUX_FOR_MIPITX0_OUT_MODE_4},
    {PAD_OUTN_TX0_CH_2, PADTOP_BANK, REG_BT656_OUT_MODE, REG_BT656_OUT_MODE_MASK, BIT4, PINMUX_FOR_BT656_OUT_MODE_1},
    {PAD_OUTN_TX0_CH_2, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT4, PINMUX_FOR_BT601_OUT_MODE_1},
    {PAD_OUTN_TX0_CH_2, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTN_TX0_CH_2, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
    {PAD_OUTN_TX0_CH_2, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TTL18_MODE_3},
    {PAD_OUTN_TX0_CH_2, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TTL16_MODE_3},
    {PAD_OUTN_TX0_CH_2, PADTOP_BANK, REG_PSPI1_G_MODE, REG_PSPI1_G_MODE_MASK, BIT8, PINMUX_FOR_PSPI1_G_MODE_1},
    {PAD_OUTN_TX0_CH_2, PADTOP_BANK, REG_PSPI1_CS2_MODE, REG_PSPI1_CS2_MODE_MASK, BIT10, PINMUX_FOR_PSPI1_CS2_MODE_1},
};
const ST_PadMuxInfo outp_tx0_ch_3_tbl[] = {
    {PAD_OUTP_TX0_CH_3, PADGPIO_BANK, REG_OUTP_TX0_CH_3_GPIO_MODE, REG_OUTP_TX0_CH_3_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTP_TX0_CH_3, PADTOP_BANK, REG_RISCV_EJ_MODE, REG_RISCV_EJ_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_RISCV_EJ_MODE_3},
    {PAD_OUTP_TX0_CH_3, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_OUTP_TX0_CH_3, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_OUTP_TX0_CH_3, PADTOP_BANK, REG_PWM_OUT14_MODE, REG_PWM_OUT14_MODE_MASK, BIT13, PINMUX_FOR_PWM_OUT14_MODE_2},
    {PAD_OUTP_TX0_CH_3, PADTOP_BANK, REG_I2S0_RX_MODE, REG_I2S0_RX_MODE_MASK, BIT9, PINMUX_FOR_I2S0_RX_MODE_2},
    {PAD_OUTP_TX0_CH_3, PADTOP_BANK, REG_I2S0_RXTX_MODE, REG_I2S0_RXTX_MODE_MASK, BIT1, PINMUX_FOR_I2S0_RXTX_MODE_2},
    {PAD_OUTP_TX0_CH_3, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT8, PINMUX_FOR_LVDS_OUT_MODE_1},
    {PAD_OUTP_TX0_CH_3, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT9, PINMUX_FOR_LVDS_OUT_MODE_2},
    {PAD_OUTP_TX0_CH_3, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT0,
     PINMUX_FOR_MIPITX0_OUT_MODE_1},
    {PAD_OUTP_TX0_CH_3, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT1,
     PINMUX_FOR_MIPITX0_OUT_MODE_2},
    {PAD_OUTP_TX0_CH_3, PADTOP_BANK, REG_BT656_OUT_MODE, REG_BT656_OUT_MODE_MASK, BIT4, PINMUX_FOR_BT656_OUT_MODE_1},
    {PAD_OUTP_TX0_CH_3, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT4, PINMUX_FOR_BT601_OUT_MODE_1},
    {PAD_OUTP_TX0_CH_3, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTP_TX0_CH_3, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
    {PAD_OUTP_TX0_CH_3, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TTL18_MODE_3},
    {PAD_OUTP_TX0_CH_3, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TTL16_MODE_3},
    {PAD_OUTP_TX0_CH_3, PADTOP_BANK, REG_PSPI1_G_MODE, REG_PSPI1_G_MODE_MASK, BIT8, PINMUX_FOR_PSPI1_G_MODE_1},
};
const ST_PadMuxInfo outn_tx0_ch_3_tbl[] = {
    {PAD_OUTN_TX0_CH_3, PADGPIO_BANK, REG_OUTN_TX0_CH_3_GPIO_MODE, REG_OUTN_TX0_CH_3_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTN_TX0_CH_3, PADTOP_BANK, REG_RISCV_EJ_MODE, REG_RISCV_EJ_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_RISCV_EJ_MODE_3},
    {PAD_OUTN_TX0_CH_3, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_OUTN_TX0_CH_3, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_OUTN_TX0_CH_3, PADTOP_BANK, REG_PWM_OUT15_MODE, REG_PWM_OUT15_MODE_MASK, BIT15, PINMUX_FOR_PWM_OUT15_MODE_2},
    {PAD_OUTN_TX0_CH_3, PADTOP_BANK, REG_I2S0_TX_MODE, REG_I2S0_TX_MODE_MASK, BIT13, PINMUX_FOR_I2S0_TX_MODE_2},
    {PAD_OUTN_TX0_CH_3, PADTOP_BANK, REG_I2S0_RXTX_MODE, REG_I2S0_RXTX_MODE_MASK, BIT1, PINMUX_FOR_I2S0_RXTX_MODE_2},
    {PAD_OUTN_TX0_CH_3, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT8, PINMUX_FOR_LVDS_OUT_MODE_1},
    {PAD_OUTN_TX0_CH_3, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT9, PINMUX_FOR_LVDS_OUT_MODE_2},
    {PAD_OUTN_TX0_CH_3, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT0,
     PINMUX_FOR_MIPITX0_OUT_MODE_1},
    {PAD_OUTN_TX0_CH_3, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT1,
     PINMUX_FOR_MIPITX0_OUT_MODE_2},
    {PAD_OUTN_TX0_CH_3, PADTOP_BANK, REG_BT656_OUT_MODE, REG_BT656_OUT_MODE_MASK, BIT4, PINMUX_FOR_BT656_OUT_MODE_1},
    {PAD_OUTN_TX0_CH_3, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT4, PINMUX_FOR_BT601_OUT_MODE_1},
    {PAD_OUTN_TX0_CH_3, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTN_TX0_CH_3, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
    {PAD_OUTN_TX0_CH_3, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TTL18_MODE_3},
    {PAD_OUTN_TX0_CH_3, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TTL16_MODE_3},
    {PAD_OUTN_TX0_CH_3, PADTOP_BANK, REG_PSPI1_G_MODE, REG_PSPI1_G_MODE_MASK, BIT8, PINMUX_FOR_PSPI1_G_MODE_1},
};
const ST_PadMuxInfo outp_tx0_ch_4_tbl[] = {
    {PAD_OUTP_TX0_CH_4, PADGPIO_BANK, REG_OUTP_TX0_CH_4_GPIO_MODE, REG_OUTP_TX0_CH_4_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTP_TX0_CH_4, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_OUTP_TX0_CH_4, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_OUTP_TX0_CH_4, PADTOP_BANK, REG_ADC_INT_MODE, REG_ADC_INT_MODE_MASK, BIT9, PINMUX_FOR_ADC_INT_MODE_2},
    {PAD_OUTP_TX0_CH_4, PADTOP_BANK, REG_SPI0_CZ1_MODE, REG_SPI0_CZ1_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_SPI0_CZ1_MODE_3},
    {PAD_OUTP_TX0_CH_4, PADTOP_BANK, REG_PWM_OUT16_MODE, REG_PWM_OUT16_MODE_MASK, BIT9, PINMUX_FOR_PWM_OUT16_MODE_2},
    {PAD_OUTP_TX0_CH_4, PADTOP_BANK, REG_I2S0_TX_MODE, REG_I2S0_TX_MODE_MASK, BIT13, PINMUX_FOR_I2S0_TX_MODE_2},
    {PAD_OUTP_TX0_CH_4, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT8, PINMUX_FOR_LVDS_OUT_MODE_1},
    {PAD_OUTP_TX0_CH_4, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT9, PINMUX_FOR_LVDS_OUT_MODE_2},
    {PAD_OUTP_TX0_CH_4, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT0,
     PINMUX_FOR_MIPITX0_OUT_MODE_1},
    {PAD_OUTP_TX0_CH_4, PADTOP_BANK, REG_BT656_OUT_MODE, REG_BT656_OUT_MODE_MASK, BIT4, PINMUX_FOR_BT656_OUT_MODE_1},
    {PAD_OUTP_TX0_CH_4, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT4, PINMUX_FOR_BT601_OUT_MODE_1},
    {PAD_OUTP_TX0_CH_4, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTP_TX0_CH_4, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
    {PAD_OUTP_TX0_CH_4, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TTL18_MODE_3},
    {PAD_OUTP_TX0_CH_4, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TTL16_MODE_3},
    {PAD_OUTP_TX0_CH_4, PADTOP_BANK, REG_PSPI1_G_MODE, REG_PSPI1_G_MODE_MASK, BIT8, PINMUX_FOR_PSPI1_G_MODE_1},
};
const ST_PadMuxInfo outn_tx0_ch_4_tbl[] = {
    {PAD_OUTN_TX0_CH_4, PADGPIO_BANK, REG_OUTN_TX0_CH_4_GPIO_MODE, REG_OUTN_TX0_CH_4_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTN_TX0_CH_4, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_OUTN_TX0_CH_4, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_OUTN_TX0_CH_4, PADTOP_BANK, REG_SPI0_MODE, REG_SPI0_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_SPI0_MODE_3},
    {PAD_OUTN_TX0_CH_4, PADTOP_BANK, REG_PWM_INT_MODE, REG_PWM_INT_MODE_MASK, BIT1, PINMUX_FOR_PWM_INT_MODE_2},
    {PAD_OUTN_TX0_CH_4, PADTOP_BANK, REG_I2S0_TX_MODE, REG_I2S0_TX_MODE_MASK, BIT13, PINMUX_FOR_I2S0_TX_MODE_2},
    {PAD_OUTN_TX0_CH_4, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT8, PINMUX_FOR_LVDS_OUT_MODE_1},
    {PAD_OUTN_TX0_CH_4, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT9, PINMUX_FOR_LVDS_OUT_MODE_2},
    {PAD_OUTN_TX0_CH_4, PADTOP_BANK, REG_MIPITX0_OUT_MODE, REG_MIPITX0_OUT_MODE_MASK, BIT0,
     PINMUX_FOR_MIPITX0_OUT_MODE_1},
    {PAD_OUTN_TX0_CH_4, PADTOP_BANK, REG_BT656_OUT_MODE, REG_BT656_OUT_MODE_MASK, BIT4, PINMUX_FOR_BT656_OUT_MODE_1},
    {PAD_OUTN_TX0_CH_4, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT4, PINMUX_FOR_BT601_OUT_MODE_1},
    {PAD_OUTN_TX0_CH_4, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTN_TX0_CH_4, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
    {PAD_OUTN_TX0_CH_4, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TTL18_MODE_3},
    {PAD_OUTN_TX0_CH_4, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TTL16_MODE_3},
    {PAD_OUTN_TX0_CH_4, PADTOP_BANK, REG_PSPI1_G_MODE, REG_PSPI1_G_MODE_MASK, BIT8, PINMUX_FOR_PSPI1_G_MODE_1},
};
const ST_PadMuxInfo outp_tx1_ch_0_tbl[] = {
    {PAD_OUTP_TX1_CH_0, PADGPIO_BANK, REG_OUTP_TX1_CH_0_GPIO_MODE, REG_OUTP_TX1_CH_0_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTP_TX1_CH_0, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_OUTP_TX1_CH_0, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_OUTP_TX1_CH_0, PADTOP_BANK, REG_SPI0_MODE, REG_SPI0_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_SPI0_MODE_3},
    {PAD_OUTP_TX1_CH_0, PADTOP_BANK, REG_UART2_MODE, REG_UART2_MODE_MASK, BIT10, PINMUX_FOR_UART2_MODE_4},
    {PAD_OUTP_TX1_CH_0, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT8, PINMUX_FOR_LVDS_OUT_MODE_1},
    {PAD_OUTP_TX1_CH_0, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT9 | BIT8,
     PINMUX_FOR_LVDS_OUT_MODE_3},
    {PAD_OUTP_TX1_CH_0, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT4,
     PINMUX_FOR_MIPITX1_OUT_MODE_1},
    {PAD_OUTP_TX1_CH_0, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT5,
     PINMUX_FOR_MIPITX1_OUT_MODE_2},
    {PAD_OUTP_TX1_CH_0, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_MIPITX1_OUT_MODE_3},
    {PAD_OUTP_TX1_CH_0, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT6,
     PINMUX_FOR_MIPITX1_OUT_MODE_4},
    {PAD_OUTP_TX1_CH_0, PADTOP_BANK, REG_BT656_OUT_MODE, REG_BT656_OUT_MODE_MASK, BIT4, PINMUX_FOR_BT656_OUT_MODE_1},
    {PAD_OUTP_TX1_CH_0, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT4, PINMUX_FOR_BT601_OUT_MODE_1},
    {PAD_OUTP_TX1_CH_0, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTP_TX1_CH_0, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
    {PAD_OUTP_TX1_CH_0, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TTL18_MODE_3},
    {PAD_OUTP_TX1_CH_0, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TTL16_MODE_3},
    {PAD_OUTP_TX1_CH_0, PADTOP_BANK, REG_PSPI1_G_MODE, REG_PSPI1_G_MODE_MASK, BIT8, PINMUX_FOR_PSPI1_G_MODE_1},
};
const ST_PadMuxInfo outn_tx1_ch_0_tbl[] = {
    {PAD_OUTN_TX1_CH_0, PADGPIO_BANK, REG_OUTN_TX1_CH_0_GPIO_MODE, REG_OUTN_TX1_CH_0_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTN_TX1_CH_0, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_OUTN_TX1_CH_0, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_TEST_OUT_MODE_3},
    {PAD_OUTN_TX1_CH_0, PADTOP_BANK, REG_SPI0_MODE, REG_SPI0_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_SPI0_MODE_3},
    {PAD_OUTN_TX1_CH_0, PADTOP_BANK, REG_UART2_MODE, REG_UART2_MODE_MASK, BIT10, PINMUX_FOR_UART2_MODE_4},
    {PAD_OUTN_TX1_CH_0, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT8, PINMUX_FOR_LVDS_OUT_MODE_1},
    {PAD_OUTN_TX1_CH_0, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT9 | BIT8,
     PINMUX_FOR_LVDS_OUT_MODE_3},
    {PAD_OUTN_TX1_CH_0, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT4,
     PINMUX_FOR_MIPITX1_OUT_MODE_1},
    {PAD_OUTN_TX1_CH_0, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT5,
     PINMUX_FOR_MIPITX1_OUT_MODE_2},
    {PAD_OUTN_TX1_CH_0, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_MIPITX1_OUT_MODE_3},
    {PAD_OUTN_TX1_CH_0, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT6,
     PINMUX_FOR_MIPITX1_OUT_MODE_4},
    {PAD_OUTN_TX1_CH_0, PADTOP_BANK, REG_BT656_OUT_MODE, REG_BT656_OUT_MODE_MASK, BIT4, PINMUX_FOR_BT656_OUT_MODE_1},
    {PAD_OUTN_TX1_CH_0, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT4, PINMUX_FOR_BT601_OUT_MODE_1},
    {PAD_OUTN_TX1_CH_0, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTN_TX1_CH_0, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
    {PAD_OUTN_TX1_CH_0, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TTL18_MODE_3},
    {PAD_OUTN_TX1_CH_0, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TTL16_MODE_3},
    {PAD_OUTN_TX1_CH_0, PADTOP_BANK, REG_PSPI1_VSYNC_MODE, REG_PSPI1_VSYNC_MODE_MASK, BIT14,
     PINMUX_FOR_PSPI1_VSYNC_MODE_1},
};
const ST_PadMuxInfo outp_tx1_ch_1_tbl[] = {
    {PAD_OUTP_TX1_CH_1, PADGPIO_BANK, REG_OUTP_TX1_CH_1_GPIO_MODE, REG_OUTP_TX1_CH_1_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTP_TX1_CH_1, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_OUTP_TX1_CH_1, PADTOP_BANK, REG_SPI0_MODE, REG_SPI0_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_SPI0_MODE_3},
    {PAD_OUTP_TX1_CH_1, PADTOP_BANK, REG_UART3_MODE, REG_UART3_MODE_MASK, BIT13 | BIT12, PINMUX_FOR_UART3_MODE_3},
    {PAD_OUTP_TX1_CH_1, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT8, PINMUX_FOR_LVDS_OUT_MODE_1},
    {PAD_OUTP_TX1_CH_1, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT9 | BIT8,
     PINMUX_FOR_LVDS_OUT_MODE_3},
    {PAD_OUTP_TX1_CH_1, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT4,
     PINMUX_FOR_MIPITX1_OUT_MODE_1},
    {PAD_OUTP_TX1_CH_1, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT5,
     PINMUX_FOR_MIPITX1_OUT_MODE_2},
    {PAD_OUTP_TX1_CH_1, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_MIPITX1_OUT_MODE_3},
    {PAD_OUTP_TX1_CH_1, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTP_TX1_CH_1, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
    {PAD_OUTP_TX1_CH_1, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TTL18_MODE_3},
    {PAD_OUTP_TX1_CH_1, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TTL16_MODE_3},
};
const ST_PadMuxInfo outn_tx1_ch_1_tbl[] = {
    {PAD_OUTN_TX1_CH_1, PADGPIO_BANK, REG_OUTN_TX1_CH_1_GPIO_MODE, REG_OUTN_TX1_CH_1_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTN_TX1_CH_1, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_OUTN_TX1_CH_1, PADTOP_BANK, REG_I2C4_MODE, REG_I2C4_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_I2C4_MODE_3},
    {PAD_OUTN_TX1_CH_1, PADTOP_BANK, REG_UART3_MODE, REG_UART3_MODE_MASK, BIT13 | BIT12, PINMUX_FOR_UART3_MODE_3},
    {PAD_OUTN_TX1_CH_1, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT8, PINMUX_FOR_LVDS_OUT_MODE_1},
    {PAD_OUTN_TX1_CH_1, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT9 | BIT8,
     PINMUX_FOR_LVDS_OUT_MODE_3},
    {PAD_OUTN_TX1_CH_1, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT4,
     PINMUX_FOR_MIPITX1_OUT_MODE_1},
    {PAD_OUTN_TX1_CH_1, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT5,
     PINMUX_FOR_MIPITX1_OUT_MODE_2},
    {PAD_OUTN_TX1_CH_1, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_MIPITX1_OUT_MODE_3},
    {PAD_OUTN_TX1_CH_1, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTN_TX1_CH_1, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
    {PAD_OUTN_TX1_CH_1, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TTL18_MODE_3},
    {PAD_OUTN_TX1_CH_1, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TTL16_MODE_3},
};
const ST_PadMuxInfo outp_tx1_ch_2_tbl[] = {
    {PAD_OUTP_TX1_CH_2, PADGPIO_BANK, REG_OUTP_TX1_CH_2_GPIO_MODE, REG_OUTP_TX1_CH_2_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTP_TX1_CH_2, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_OUTP_TX1_CH_2, PADTOP_BANK, REG_I2C4_MODE, REG_I2C4_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_I2C4_MODE_3},
    {PAD_OUTP_TX1_CH_2, PADTOP_BANK, REG_FUART_MODE, REG_FUART_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_FUART_MODE_3},
    {PAD_OUTP_TX1_CH_2, PADTOP_BANK, REG_FUART_2W_MODE, REG_FUART_2W_MODE_MASK, BIT13 | BIT12,
     PINMUX_FOR_FUART_2W_MODE_3},
    {PAD_OUTP_TX1_CH_2, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT8, PINMUX_FOR_LVDS_OUT_MODE_1},
    {PAD_OUTP_TX1_CH_2, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT9 | BIT8,
     PINMUX_FOR_LVDS_OUT_MODE_3},
    {PAD_OUTP_TX1_CH_2, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT4,
     PINMUX_FOR_MIPITX1_OUT_MODE_1},
    {PAD_OUTP_TX1_CH_2, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT5,
     PINMUX_FOR_MIPITX1_OUT_MODE_2},
    {PAD_OUTP_TX1_CH_2, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_MIPITX1_OUT_MODE_3},
    {PAD_OUTP_TX1_CH_2, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT6,
     PINMUX_FOR_MIPITX1_OUT_MODE_4},
    {PAD_OUTP_TX1_CH_2, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTP_TX1_CH_2, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
    {PAD_OUTP_TX1_CH_2, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TTL18_MODE_3},
    {PAD_OUTP_TX1_CH_2, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TTL16_MODE_3},
};
const ST_PadMuxInfo outn_tx1_ch_2_tbl[] = {
    {PAD_OUTN_TX1_CH_2, PADGPIO2_BANK, REG_OUTN_TX1_CH_2_GPIO_MODE, REG_OUTN_TX1_CH_2_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTN_TX1_CH_2, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_OUTN_TX1_CH_2, PADTOP_BANK, REG_FUART_MODE, REG_FUART_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_FUART_MODE_3},
    {PAD_OUTN_TX1_CH_2, PADTOP_BANK, REG_FUART_2W_MODE, REG_FUART_2W_MODE_MASK, BIT13 | BIT12,
     PINMUX_FOR_FUART_2W_MODE_3},
    {PAD_OUTN_TX1_CH_2, PADTOP_BANK, REG_PWM_IN1_MODE, REG_PWM_IN1_MODE_MASK, BIT3, PINMUX_FOR_PWM_IN1_MODE_2},
    {PAD_OUTN_TX1_CH_2, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT8, PINMUX_FOR_LVDS_OUT_MODE_1},
    {PAD_OUTN_TX1_CH_2, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT9 | BIT8,
     PINMUX_FOR_LVDS_OUT_MODE_3},
    {PAD_OUTN_TX1_CH_2, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT4,
     PINMUX_FOR_MIPITX1_OUT_MODE_1},
    {PAD_OUTN_TX1_CH_2, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT5,
     PINMUX_FOR_MIPITX1_OUT_MODE_2},
    {PAD_OUTN_TX1_CH_2, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_MIPITX1_OUT_MODE_3},
    {PAD_OUTN_TX1_CH_2, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT6,
     PINMUX_FOR_MIPITX1_OUT_MODE_4},
    {PAD_OUTN_TX1_CH_2, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTN_TX1_CH_2, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
    {PAD_OUTN_TX1_CH_2, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TTL18_MODE_3},
    {PAD_OUTN_TX1_CH_2, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TTL16_MODE_3},
};
const ST_PadMuxInfo outp_tx1_ch_3_tbl[] = {
    {PAD_OUTP_TX1_CH_3, PADGPIO2_BANK, REG_OUTP_TX1_CH_3_GPIO_MODE, REG_OUTP_TX1_CH_3_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTP_TX1_CH_3, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_OUTP_TX1_CH_3, PADTOP_BANK, REG_FUART_MODE, REG_FUART_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_FUART_MODE_3},
    {PAD_OUTP_TX1_CH_3, PADTOP_BANK, REG_UART1_MODE, REG_UART1_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_UART1_MODE_3},
    {PAD_OUTP_TX1_CH_3, PADTOP_BANK, REG_PWM_IN2_MODE, REG_PWM_IN2_MODE_MASK, BIT5, PINMUX_FOR_PWM_IN2_MODE_2},
    {PAD_OUTP_TX1_CH_3, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT8, PINMUX_FOR_LVDS_OUT_MODE_1},
    {PAD_OUTP_TX1_CH_3, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT9 | BIT8,
     PINMUX_FOR_LVDS_OUT_MODE_3},
    {PAD_OUTP_TX1_CH_3, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT4,
     PINMUX_FOR_MIPITX1_OUT_MODE_1},
    {PAD_OUTP_TX1_CH_3, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT5,
     PINMUX_FOR_MIPITX1_OUT_MODE_2},
    {PAD_OUTP_TX1_CH_3, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTP_TX1_CH_3, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
    {PAD_OUTP_TX1_CH_3, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TTL18_MODE_3},
    {PAD_OUTP_TX1_CH_3, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TTL16_MODE_3},
};
const ST_PadMuxInfo outn_tx1_ch_3_tbl[] = {
    {PAD_OUTN_TX1_CH_3, PADGPIO2_BANK, REG_OUTN_TX1_CH_3_GPIO_MODE, REG_OUTN_TX1_CH_3_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTN_TX1_CH_3, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_OUTN_TX1_CH_3, PADTOP_BANK, REG_FUART_MODE, REG_FUART_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_FUART_MODE_3},
    {PAD_OUTN_TX1_CH_3, PADTOP_BANK, REG_UART1_MODE, REG_UART1_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_UART1_MODE_3},
    {PAD_OUTN_TX1_CH_3, PADTOP_BANK, REG_PWM_IN3_MODE, REG_PWM_IN3_MODE_MASK, BIT7, PINMUX_FOR_PWM_IN3_MODE_2},
    {PAD_OUTN_TX1_CH_3, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT8, PINMUX_FOR_LVDS_OUT_MODE_1},
    {PAD_OUTN_TX1_CH_3, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT9 | BIT8,
     PINMUX_FOR_LVDS_OUT_MODE_3},
    {PAD_OUTN_TX1_CH_3, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT4,
     PINMUX_FOR_MIPITX1_OUT_MODE_1},
    {PAD_OUTN_TX1_CH_3, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT5,
     PINMUX_FOR_MIPITX1_OUT_MODE_2},
    {PAD_OUTN_TX1_CH_3, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTN_TX1_CH_3, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
    {PAD_OUTN_TX1_CH_3, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TTL18_MODE_3},
    {PAD_OUTN_TX1_CH_3, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TTL16_MODE_3},
};
const ST_PadMuxInfo outp_tx1_ch_4_tbl[] = {
    {PAD_OUTP_TX1_CH_4, PADGPIO2_BANK, REG_OUTP_TX1_CH_4_GPIO_MODE, REG_OUTP_TX1_CH_4_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTP_TX1_CH_4, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_OUTP_TX1_CH_4, PADTOP_BANK, REG_I2C3_MODE, REG_I2C3_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_I2C3_MODE_3},
    {PAD_OUTP_TX1_CH_4, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT8, PINMUX_FOR_LVDS_OUT_MODE_1},
    {PAD_OUTP_TX1_CH_4, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT9 | BIT8,
     PINMUX_FOR_LVDS_OUT_MODE_3},
    {PAD_OUTP_TX1_CH_4, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT4,
     PINMUX_FOR_MIPITX1_OUT_MODE_1},
    {PAD_OUTP_TX1_CH_4, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTP_TX1_CH_4, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
    {PAD_OUTP_TX1_CH_4, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TTL18_MODE_3},
    {PAD_OUTP_TX1_CH_4, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TTL16_MODE_3},
};
const ST_PadMuxInfo outn_tx1_ch_4_tbl[] = {
    {PAD_OUTN_TX1_CH_4, PADGPIO2_BANK, REG_OUTN_TX1_CH_4_GPIO_MODE, REG_OUTN_TX1_CH_4_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_OUTN_TX1_CH_4, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_OUTN_TX1_CH_4, PADTOP_BANK, REG_I2C3_MODE, REG_I2C3_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_I2C3_MODE_3},
    {PAD_OUTN_TX1_CH_4, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT8, PINMUX_FOR_LVDS_OUT_MODE_1},
    {PAD_OUTN_TX1_CH_4, PADTOP_BANK, REG_LVDS_OUT_MODE, REG_LVDS_OUT_MODE_MASK, BIT9 | BIT8,
     PINMUX_FOR_LVDS_OUT_MODE_3},
    {PAD_OUTN_TX1_CH_4, PADTOP_BANK, REG_MIPITX1_OUT_MODE, REG_MIPITX1_OUT_MODE_MASK, BIT4,
     PINMUX_FOR_MIPITX1_OUT_MODE_1},
    {PAD_OUTN_TX1_CH_4, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9, PINMUX_FOR_TTL24_MODE_2},
    {PAD_OUTN_TX1_CH_4, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_TTL24_MODE_3},
    {PAD_OUTN_TX1_CH_4, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_TTL18_MODE_3},
    {PAD_OUTN_TX1_CH_4, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TTL16_MODE_3},
};
const ST_PadMuxInfo sar_adc_0_tbl[] = {
    {PAD_SAR_ADC_0, PADGPIO2_BANK, REG_SAR_ADC_0_GPIO_MODE, REG_SAR_ADC_0_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_0, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5, PINMUX_FOR_TEST_OUT_MODE_2},
    {PAD_SAR_ADC_0, PADTOP_BANK, REG_ADC0_MODE, REG_ADC0_MODE_MASK, BIT0, PINMUX_FOR_ADC0_MODE_1},
    {PAD_SAR_ADC_0, PADTOP_BANK, REG_UART4_MODE, REG_UART4_MODE_MASK, BIT1, PINMUX_FOR_UART4_MODE_2},
    {PAD_SAR_ADC_0, PADTOP_BANK, REG_SD0_CDZ_MODE, REG_SD0_CDZ_MODE_MASK, BIT11, PINMUX_FOR_SD0_CDZ_MODE_2},
    {PAD_SAR_ADC_0, PADTOP_BANK, REG_SD0_RSTN_MODE, REG_SD0_RSTN_MODE_MASK, BIT1, PINMUX_FOR_SD0_RSTN_MODE_2},
    {PAD_SAR_ADC_0, PADTOP_BANK, REG_PWM_OUT12_MODE, REG_PWM_OUT12_MODE_MASK, BIT0, PINMUX_FOR_PWM_OUT12_MODE_1},
    {PAD_SAR_ADC_0, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT5, PINMUX_FOR_BT601_OUT_MODE_2},
    {PAD_SAR_ADC_0, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_0, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT4, PINMUX_FOR_TTL18_MODE_1},
    {PAD_SAR_ADC_0, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5, PINMUX_FOR_TTL18_MODE_2},
    {PAD_SAR_ADC_0, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT0, PINMUX_FOR_TTL16_MODE_1},
    {PAD_SAR_ADC_0, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1, PINMUX_FOR_TTL16_MODE_2},
};
const ST_PadMuxInfo sar_adc_1_tbl[] = {
    {PAD_SAR_ADC_1, PADGPIO2_BANK, REG_SAR_ADC_1_GPIO_MODE, REG_SAR_ADC_1_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_1, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5, PINMUX_FOR_TEST_OUT_MODE_2},
    {PAD_SAR_ADC_1, PADTOP_BANK, REG_ADC1_MODE, REG_ADC1_MODE_MASK, BIT1, PINMUX_FOR_ADC1_MODE_1},
    {PAD_SAR_ADC_1, PADTOP_BANK, REG_UART4_MODE, REG_UART4_MODE_MASK, BIT1, PINMUX_FOR_UART4_MODE_2},
    {PAD_SAR_ADC_1, PADTOP_BANK, REG_SD0_MODE, REG_SD0_MODE_MASK, BIT9, PINMUX_FOR_SD0_MODE_2},
    {PAD_SAR_ADC_1, PADTOP_BANK, REG_PWM_OUT13_MODE, REG_PWM_OUT13_MODE_MASK, BIT10, PINMUX_FOR_PWM_OUT13_MODE_1},
    {PAD_SAR_ADC_1, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT5, PINMUX_FOR_BT601_OUT_MODE_2},
    {PAD_SAR_ADC_1, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_1, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT4, PINMUX_FOR_TTL18_MODE_1},
    {PAD_SAR_ADC_1, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5, PINMUX_FOR_TTL18_MODE_2},
    {PAD_SAR_ADC_1, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT0, PINMUX_FOR_TTL16_MODE_1},
    {PAD_SAR_ADC_1, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1, PINMUX_FOR_TTL16_MODE_2},
};
const ST_PadMuxInfo sar_adc_2_tbl[] = {
    {PAD_SAR_ADC_2, PADGPIO2_BANK, REG_SAR_ADC_2_GPIO_MODE, REG_SAR_ADC_2_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_2, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5, PINMUX_FOR_TEST_OUT_MODE_2},
    {PAD_SAR_ADC_2, PADTOP_BANK, REG_ADC2_MODE, REG_ADC2_MODE_MASK, BIT2, PINMUX_FOR_ADC2_MODE_1},
    {PAD_SAR_ADC_2, PADTOP_BANK, REG_I2C1_MODE, REG_I2C1_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_I2C1_MODE_3},
    {PAD_SAR_ADC_2, PADTOP_BANK, REG_UART1_MODE, REG_UART1_MODE_MASK, BIT5, PINMUX_FOR_UART1_MODE_2},
    {PAD_SAR_ADC_2, PADTOP_BANK, REG_SD0_MODE, REG_SD0_MODE_MASK, BIT9, PINMUX_FOR_SD0_MODE_2},
    {PAD_SAR_ADC_2, PADTOP_BANK, REG_PWM_OUT14_MODE, REG_PWM_OUT14_MODE_MASK, BIT12, PINMUX_FOR_PWM_OUT14_MODE_1},
    {PAD_SAR_ADC_2, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT5, PINMUX_FOR_BT601_OUT_MODE_2},
    {PAD_SAR_ADC_2, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_2, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT4, PINMUX_FOR_TTL18_MODE_1},
    {PAD_SAR_ADC_2, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5, PINMUX_FOR_TTL18_MODE_2},
    {PAD_SAR_ADC_2, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT0, PINMUX_FOR_TTL16_MODE_1},
    {PAD_SAR_ADC_2, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1, PINMUX_FOR_TTL16_MODE_2},
};
const ST_PadMuxInfo sar_adc_3_tbl[] = {
    {PAD_SAR_ADC_3, PADGPIO2_BANK, REG_SAR_ADC_3_GPIO_MODE, REG_SAR_ADC_3_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_3, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5, PINMUX_FOR_TEST_OUT_MODE_2},
    {PAD_SAR_ADC_3, PADTOP_BANK, REG_ADC3_MODE, REG_ADC3_MODE_MASK, BIT3, PINMUX_FOR_ADC3_MODE_1},
    {PAD_SAR_ADC_3, PADTOP_BANK, REG_I2C1_MODE, REG_I2C1_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_I2C1_MODE_3},
    {PAD_SAR_ADC_3, PADTOP_BANK, REG_UART1_MODE, REG_UART1_MODE_MASK, BIT5, PINMUX_FOR_UART1_MODE_2},
    {PAD_SAR_ADC_3, PADTOP_BANK, REG_SD0_MODE, REG_SD0_MODE_MASK, BIT9, PINMUX_FOR_SD0_MODE_2},
    {PAD_SAR_ADC_3, PADTOP_BANK, REG_PWM_OUT15_MODE, REG_PWM_OUT15_MODE_MASK, BIT14, PINMUX_FOR_PWM_OUT15_MODE_1},
    {PAD_SAR_ADC_3, PADTOP_BANK, REG_BT656_OUT_MODE, REG_BT656_OUT_MODE_MASK, BIT5, PINMUX_FOR_BT656_OUT_MODE_2},
    {PAD_SAR_ADC_3, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT5, PINMUX_FOR_BT601_OUT_MODE_2},
    {PAD_SAR_ADC_3, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_3, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT4, PINMUX_FOR_TTL18_MODE_1},
    {PAD_SAR_ADC_3, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5, PINMUX_FOR_TTL18_MODE_2},
    {PAD_SAR_ADC_3, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT0, PINMUX_FOR_TTL16_MODE_1},
    {PAD_SAR_ADC_3, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1, PINMUX_FOR_TTL16_MODE_2},
};
const ST_PadMuxInfo sar_adc_4_tbl[] = {
    {PAD_SAR_ADC_4, PADGPIO2_BANK, REG_SAR_ADC_4_GPIO_MODE, REG_SAR_ADC_4_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_4, PADTOP_BANK, REG_EJ_MODE, REG_EJ_MODE_MASK, BIT7, PINMUX_FOR_EJ_MODE_1},
    {PAD_SAR_ADC_4, PADTOP_BANK, REG_RISCV_EJ_MODE, REG_RISCV_EJ_MODE_MASK, BIT0, PINMUX_FOR_RISCV_EJ_MODE_1},
    {PAD_SAR_ADC_4, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5, PINMUX_FOR_TEST_OUT_MODE_2},
    {PAD_SAR_ADC_4, PADTOP_BANK, REG_ADC4_MODE, REG_ADC4_MODE_MASK, BIT4, PINMUX_FOR_ADC4_MODE_1},
    {PAD_SAR_ADC_4, PADTOP_BANK, REG_I2C5_MODE, REG_I2C5_MODE_MASK, BIT9, PINMUX_FOR_I2C5_MODE_2},
    {PAD_SAR_ADC_4, PADTOP_BANK, REG_SPI1_MODE, REG_SPI1_MODE_MASK, BIT3, PINMUX_FOR_SPI1_MODE_1},
    {PAD_SAR_ADC_4, PADTOP_BANK, REG_SD0_MODE, REG_SD0_MODE_MASK, BIT9, PINMUX_FOR_SD0_MODE_2},
    {PAD_SAR_ADC_4, PADTOP_BANK, REG_PWM_OUT16_MODE, REG_PWM_OUT16_MODE_MASK, BIT8, PINMUX_FOR_PWM_OUT16_MODE_1},
    {PAD_SAR_ADC_4, PADTOP_BANK, REG_BT656_OUT_MODE, REG_BT656_OUT_MODE_MASK, BIT5, PINMUX_FOR_BT656_OUT_MODE_2},
    {PAD_SAR_ADC_4, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT5, PINMUX_FOR_BT601_OUT_MODE_2},
    {PAD_SAR_ADC_4, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_4, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5, PINMUX_FOR_TTL18_MODE_2},
    {PAD_SAR_ADC_4, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1, PINMUX_FOR_TTL16_MODE_2},
};
const ST_PadMuxInfo sar_adc_5_tbl[] = {
    {PAD_SAR_ADC_5, PADGPIO2_BANK, REG_SAR_ADC_5_GPIO_MODE, REG_SAR_ADC_5_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_5, PADTOP_BANK, REG_EJ_MODE, REG_EJ_MODE_MASK, BIT7, PINMUX_FOR_EJ_MODE_1},
    {PAD_SAR_ADC_5, PADTOP_BANK, REG_RISCV_EJ_MODE, REG_RISCV_EJ_MODE_MASK, BIT0, PINMUX_FOR_RISCV_EJ_MODE_1},
    {PAD_SAR_ADC_5, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5, PINMUX_FOR_TEST_OUT_MODE_2},
    {PAD_SAR_ADC_5, PADTOP_BANK, REG_ADC5_MODE, REG_ADC5_MODE_MASK, BIT5, PINMUX_FOR_ADC5_MODE_1},
    {PAD_SAR_ADC_5, PADTOP_BANK, REG_I2C5_MODE, REG_I2C5_MODE_MASK, BIT9, PINMUX_FOR_I2C5_MODE_2},
    {PAD_SAR_ADC_5, PADTOP_BANK, REG_SPI1_MODE, REG_SPI1_MODE_MASK, BIT3, PINMUX_FOR_SPI1_MODE_1},
    {PAD_SAR_ADC_5, PADTOP_BANK, REG_SD0_MODE, REG_SD0_MODE_MASK, BIT9, PINMUX_FOR_SD0_MODE_2},
    {PAD_SAR_ADC_5, PADTOP_BANK, REG_PWM_OUT17_MODE, REG_PWM_OUT17_MODE_MASK, BIT4, PINMUX_FOR_PWM_OUT17_MODE_1},
    {PAD_SAR_ADC_5, PADTOP_BANK, REG_DMIC_2CH_MODE, REG_DMIC_2CH_MODE_MASK, BIT2, PINMUX_FOR_DMIC_2CH_MODE_4},
    {PAD_SAR_ADC_5, PADTOP_BANK, REG_DMIC_4CH_MODE, REG_DMIC_4CH_MODE_MASK, BIT2, PINMUX_FOR_DMIC_4CH_MODE_4},
    {PAD_SAR_ADC_5, PADTOP_BANK, REG_DMIC_8CH_MODE, REG_DMIC_8CH_MODE_MASK, BIT6, PINMUX_FOR_DMIC_8CH_MODE_4},
    {PAD_SAR_ADC_5, PADTOP_BANK, REG_BT656_OUT_MODE, REG_BT656_OUT_MODE_MASK, BIT5, PINMUX_FOR_BT656_OUT_MODE_2},
    {PAD_SAR_ADC_5, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT5, PINMUX_FOR_BT601_OUT_MODE_2},
    {PAD_SAR_ADC_5, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_5, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5, PINMUX_FOR_TTL18_MODE_2},
    {PAD_SAR_ADC_5, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1, PINMUX_FOR_TTL16_MODE_2},
};
const ST_PadMuxInfo sar_adc_6_tbl[] = {
    {PAD_SAR_ADC_6, PADGPIO2_BANK, REG_SAR_ADC_6_GPIO_MODE, REG_SAR_ADC_6_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_6, PADTOP_BANK, REG_EJ_MODE, REG_EJ_MODE_MASK, BIT7, PINMUX_FOR_EJ_MODE_1},
    {PAD_SAR_ADC_6, PADTOP_BANK, REG_RISCV_EJ_MODE, REG_RISCV_EJ_MODE_MASK, BIT0, PINMUX_FOR_RISCV_EJ_MODE_1},
    {PAD_SAR_ADC_6, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5, PINMUX_FOR_TEST_OUT_MODE_2},
    {PAD_SAR_ADC_6, PADTOP_BANK, REG_ADC6_MODE, REG_ADC6_MODE_MASK, BIT6, PINMUX_FOR_ADC6_MODE_1},
    {PAD_SAR_ADC_6, PADTOP_BANK, REG_I2C2_MODE, REG_I2C2_MODE_MASK, BIT9, PINMUX_FOR_I2C2_MODE_2},
    {PAD_SAR_ADC_6, PADTOP_BANK, REG_SPI1_MODE, REG_SPI1_MODE_MASK, BIT3, PINMUX_FOR_SPI1_MODE_1},
    {PAD_SAR_ADC_6, PADTOP_BANK, REG_SD0_MODE, REG_SD0_MODE_MASK, BIT9, PINMUX_FOR_SD0_MODE_2},
    {PAD_SAR_ADC_6, PADTOP_BANK, REG_PWM_OUT18_MODE, REG_PWM_OUT18_MODE_MASK, BIT12, PINMUX_FOR_PWM_OUT18_MODE_1},
    {PAD_SAR_ADC_6, PADTOP_BANK, REG_DMIC_2CH_MODE, REG_DMIC_2CH_MODE_MASK, BIT2, PINMUX_FOR_DMIC_2CH_MODE_4},
    {PAD_SAR_ADC_6, PADTOP_BANK, REG_DMIC_4CH_MODE, REG_DMIC_4CH_MODE_MASK, BIT2, PINMUX_FOR_DMIC_4CH_MODE_4},
    {PAD_SAR_ADC_6, PADTOP_BANK, REG_DMIC_8CH_MODE, REG_DMIC_8CH_MODE_MASK, BIT6, PINMUX_FOR_DMIC_8CH_MODE_4},
    {PAD_SAR_ADC_6, PADTOP_BANK, REG_BT656_OUT_MODE, REG_BT656_OUT_MODE_MASK, BIT5, PINMUX_FOR_BT656_OUT_MODE_2},
    {PAD_SAR_ADC_6, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT5, PINMUX_FOR_BT601_OUT_MODE_2},
    {PAD_SAR_ADC_6, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_6, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT4, PINMUX_FOR_TTL18_MODE_1},
    {PAD_SAR_ADC_6, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5, PINMUX_FOR_TTL18_MODE_2},
    {PAD_SAR_ADC_6, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1, PINMUX_FOR_TTL16_MODE_2},
};
const ST_PadMuxInfo sar_adc_7_tbl[] = {
    {PAD_SAR_ADC_7, PADGPIO2_BANK, REG_SAR_ADC_7_GPIO_MODE, REG_SAR_ADC_7_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_7, PADTOP_BANK, REG_EJ_MODE, REG_EJ_MODE_MASK, BIT7, PINMUX_FOR_EJ_MODE_1},
    {PAD_SAR_ADC_7, PADTOP_BANK, REG_RISCV_EJ_MODE, REG_RISCV_EJ_MODE_MASK, BIT0, PINMUX_FOR_RISCV_EJ_MODE_1},
    {PAD_SAR_ADC_7, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5, PINMUX_FOR_TEST_OUT_MODE_2},
    {PAD_SAR_ADC_7, PADTOP_BANK, REG_ADC7_MODE, REG_ADC7_MODE_MASK, BIT7, PINMUX_FOR_ADC7_MODE_1},
    {PAD_SAR_ADC_7, PADTOP_BANK, REG_I2C2_MODE, REG_I2C2_MODE_MASK, BIT9, PINMUX_FOR_I2C2_MODE_2},
    {PAD_SAR_ADC_7, PADTOP_BANK, REG_SPI1_MODE, REG_SPI1_MODE_MASK, BIT3, PINMUX_FOR_SPI1_MODE_1},
    {PAD_SAR_ADC_7, PADTOP_BANK, REG_PWM_OUT19_MODE, REG_PWM_OUT19_MODE_MASK, BIT14, PINMUX_FOR_PWM_OUT19_MODE_1},
    {PAD_SAR_ADC_7, PADTOP_BANK, REG_PWM_IN4_MODE, REG_PWM_IN4_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_PWM_IN4_MODE_3},
    {PAD_SAR_ADC_7, PADTOP_BANK, REG_DMIC_4CH_MODE, REG_DMIC_4CH_MODE_MASK, BIT2, PINMUX_FOR_DMIC_4CH_MODE_4},
    {PAD_SAR_ADC_7, PADTOP_BANK, REG_DMIC_8CH_MODE, REG_DMIC_8CH_MODE_MASK, BIT6, PINMUX_FOR_DMIC_8CH_MODE_4},
    {PAD_SAR_ADC_7, PADTOP_BANK, REG_BT656_OUT_MODE, REG_BT656_OUT_MODE_MASK, BIT5, PINMUX_FOR_BT656_OUT_MODE_2},
    {PAD_SAR_ADC_7, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT5, PINMUX_FOR_BT601_OUT_MODE_2},
    {PAD_SAR_ADC_7, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_7, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT4, PINMUX_FOR_TTL18_MODE_1},
    {PAD_SAR_ADC_7, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5, PINMUX_FOR_TTL18_MODE_2},
    {PAD_SAR_ADC_7, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT0, PINMUX_FOR_TTL16_MODE_1},
    {PAD_SAR_ADC_7, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1, PINMUX_FOR_TTL16_MODE_2},
};
const ST_PadMuxInfo sar_adc_8_tbl[] = {
    {PAD_SAR_ADC_8, PADGPIO2_BANK, REG_SAR_ADC_8_GPIO_MODE, REG_SAR_ADC_8_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_8, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5, PINMUX_FOR_TEST_OUT_MODE_2},
    {PAD_SAR_ADC_8, PADTOP_BANK, REG_ADC8_MODE, REG_ADC8_MODE_MASK, BIT8, PINMUX_FOR_ADC8_MODE_1},
    {PAD_SAR_ADC_8, PADTOP_BANK, REG_UART2_MODE, REG_UART2_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_UART2_MODE_3},
    {PAD_SAR_ADC_8, PADTOP_BANK, REG_PWM_IN5_MODE, REG_PWM_IN5_MODE_MASK, BIT11 | BIT10, PINMUX_FOR_PWM_IN5_MODE_3},
    {PAD_SAR_ADC_8, PADTOP_BANK, REG_DMIC_8CH_MODE, REG_DMIC_8CH_MODE_MASK, BIT6, PINMUX_FOR_DMIC_8CH_MODE_4},
    {PAD_SAR_ADC_8, PADTOP_BANK, REG_BT656_OUT_MODE, REG_BT656_OUT_MODE_MASK, BIT5, PINMUX_FOR_BT656_OUT_MODE_2},
    {PAD_SAR_ADC_8, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT5, PINMUX_FOR_BT601_OUT_MODE_2},
    {PAD_SAR_ADC_8, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_8, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT4, PINMUX_FOR_TTL18_MODE_1},
    {PAD_SAR_ADC_8, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5, PINMUX_FOR_TTL18_MODE_2},
    {PAD_SAR_ADC_8, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT0, PINMUX_FOR_TTL16_MODE_1},
    {PAD_SAR_ADC_8, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1, PINMUX_FOR_TTL16_MODE_2},
};
const ST_PadMuxInfo sar_adc_9_tbl[] = {
    {PAD_SAR_ADC_9, PADGPIO2_BANK, REG_SAR_ADC_9_GPIO_MODE, REG_SAR_ADC_9_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_9, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5, PINMUX_FOR_TEST_OUT_MODE_2},
    {PAD_SAR_ADC_9, PADTOP_BANK, REG_ADC9_MODE, REG_ADC9_MODE_MASK, BIT9, PINMUX_FOR_ADC9_MODE_1},
    {PAD_SAR_ADC_9, PADTOP_BANK, REG_UART2_MODE, REG_UART2_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_UART2_MODE_3},
    {PAD_SAR_ADC_9, PADTOP_BANK, REG_DMIC_8CH_MODE, REG_DMIC_8CH_MODE_MASK, BIT6, PINMUX_FOR_DMIC_8CH_MODE_4},
    {PAD_SAR_ADC_9, PADTOP_BANK, REG_BT656_OUT_MODE, REG_BT656_OUT_MODE_MASK, BIT5, PINMUX_FOR_BT656_OUT_MODE_2},
    {PAD_SAR_ADC_9, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT5, PINMUX_FOR_BT601_OUT_MODE_2},
    {PAD_SAR_ADC_9, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_9, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT4, PINMUX_FOR_TTL18_MODE_1},
    {PAD_SAR_ADC_9, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5, PINMUX_FOR_TTL18_MODE_2},
    {PAD_SAR_ADC_9, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT0, PINMUX_FOR_TTL16_MODE_1},
    {PAD_SAR_ADC_9, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1, PINMUX_FOR_TTL16_MODE_2},
    {PAD_SAR_ADC_9, PADTOP_BANK, REG_SPDIF_MODE, REG_SPDIF_MODE_MASK, BIT1, PINMUX_FOR_SPDIF_MODE_2},
};
const ST_PadMuxInfo sar_adc_10_tbl[] = {
    {PAD_SAR_ADC_10, PADGPIO2_BANK, REG_SAR_ADC_10_GPIO_MODE, REG_SAR_ADC_10_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_10, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5, PINMUX_FOR_TEST_OUT_MODE_2},
    {PAD_SAR_ADC_10, PADTOP_BANK, REG_ADC10_MODE, REG_ADC10_MODE_MASK, BIT10, PINMUX_FOR_ADC10_MODE_1},
    {PAD_SAR_ADC_10, PADTOP_BANK, REG_SPI1_MODE, REG_SPI1_MODE_MASK, BIT5, PINMUX_FOR_SPI1_MODE_4},
    {PAD_SAR_ADC_10, PADTOP_BANK, REG_BT656_OUT_MODE, REG_BT656_OUT_MODE_MASK, BIT5, PINMUX_FOR_BT656_OUT_MODE_2},
    {PAD_SAR_ADC_10, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT5, PINMUX_FOR_BT601_OUT_MODE_2},
    {PAD_SAR_ADC_10, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_10, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT4, PINMUX_FOR_TTL18_MODE_1},
    {PAD_SAR_ADC_10, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5, PINMUX_FOR_TTL18_MODE_2},
    {PAD_SAR_ADC_10, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT0, PINMUX_FOR_TTL16_MODE_1},
    {PAD_SAR_ADC_10, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1, PINMUX_FOR_TTL16_MODE_2},
    {PAD_SAR_ADC_10, PADTOP_BANK, REG_RMII1_MODE, REG_RMII1_MODE_MASK, BIT1, PINMUX_FOR_RMII1_MODE_1},
};
const ST_PadMuxInfo sar_adc_11_tbl[] = {
    {PAD_SAR_ADC_11, PADGPIO2_BANK, REG_SAR_ADC_11_GPIO_MODE, REG_SAR_ADC_11_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_11, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT5, PINMUX_FOR_TEST_OUT_MODE_2},
    {PAD_SAR_ADC_11, PADTOP_BANK, REG_ADC11_MODE, REG_ADC11_MODE_MASK, BIT11, PINMUX_FOR_ADC11_MODE_1},
    {PAD_SAR_ADC_11, PADTOP_BANK, REG_SPI1_MODE, REG_SPI1_MODE_MASK, BIT5, PINMUX_FOR_SPI1_MODE_4},
    {PAD_SAR_ADC_11, PADTOP_BANK, REG_BT656_OUT_MODE, REG_BT656_OUT_MODE_MASK, BIT5, PINMUX_FOR_BT656_OUT_MODE_2},
    {PAD_SAR_ADC_11, PADTOP_BANK, REG_BT601_OUT_MODE, REG_BT601_OUT_MODE_MASK, BIT5, PINMUX_FOR_BT601_OUT_MODE_2},
    {PAD_SAR_ADC_11, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_11, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT4, PINMUX_FOR_TTL18_MODE_1},
    {PAD_SAR_ADC_11, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5, PINMUX_FOR_TTL18_MODE_2},
    {PAD_SAR_ADC_11, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT0, PINMUX_FOR_TTL16_MODE_1},
    {PAD_SAR_ADC_11, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1, PINMUX_FOR_TTL16_MODE_2},
    {PAD_SAR_ADC_11, PADTOP_BANK, REG_RMII1_MODE, REG_RMII1_MODE_MASK, BIT1, PINMUX_FOR_RMII1_MODE_1},
};
const ST_PadMuxInfo sar_adc_12_tbl[] = {
    {PAD_SAR_ADC_12, PADGPIO2_BANK, REG_SAR_ADC_12_GPIO_MODE, REG_SAR_ADC_12_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_12, PADTOP_BANK, REG_ADC12_MODE, REG_ADC12_MODE_MASK, BIT12, PINMUX_FOR_ADC12_MODE_1},
    {PAD_SAR_ADC_12, PADTOP_BANK, REG_SPI1_MODE, REG_SPI1_MODE_MASK, BIT5, PINMUX_FOR_SPI1_MODE_4},
    {PAD_SAR_ADC_12, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_12, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5, PINMUX_FOR_TTL18_MODE_2},
    {PAD_SAR_ADC_12, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1, PINMUX_FOR_TTL16_MODE_2},
    {PAD_SAR_ADC_12, PADTOP_BANK, REG_RMII1_MODE, REG_RMII1_MODE_MASK, BIT1, PINMUX_FOR_RMII1_MODE_1},
};
const ST_PadMuxInfo sar_adc_13_tbl[] = {
    {PAD_SAR_ADC_13, PADGPIO2_BANK, REG_SAR_ADC_13_GPIO_MODE, REG_SAR_ADC_13_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_13, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_SAR_ADC_13, PADTOP_BANK, REG_ADC13_MODE, REG_ADC13_MODE_MASK, BIT13, PINMUX_FOR_ADC13_MODE_1},
    {PAD_SAR_ADC_13, PADTOP_BANK, REG_SPI1_MODE, REG_SPI1_MODE_MASK, BIT5, PINMUX_FOR_SPI1_MODE_4},
    {PAD_SAR_ADC_13, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_13, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5, PINMUX_FOR_TTL18_MODE_2},
    {PAD_SAR_ADC_13, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1, PINMUX_FOR_TTL16_MODE_2},
    {PAD_SAR_ADC_13, PADTOP_BANK, REG_RMII1_MODE, REG_RMII1_MODE_MASK, BIT1, PINMUX_FOR_RMII1_MODE_1},
};
const ST_PadMuxInfo sar_adc_14_tbl[] = {
    {PAD_SAR_ADC_14, PADGPIO2_BANK, REG_SAR_ADC_14_GPIO_MODE, REG_SAR_ADC_14_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_14, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_SAR_ADC_14, PADTOP_BANK, REG_ADC14_MODE, REG_ADC14_MODE_MASK, BIT14, PINMUX_FOR_ADC14_MODE_1},
    {PAD_SAR_ADC_14, PADTOP_BANK, REG_UART5_MODE, REG_UART5_MODE_MASK, BIT5, PINMUX_FOR_UART5_MODE_2},
    {PAD_SAR_ADC_14, PADTOP_BANK, REG_LED0_MODE, REG_LED0_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_LED0_MODE_3},
    {PAD_SAR_ADC_14, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_14, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT4, PINMUX_FOR_TTL18_MODE_1},
    {PAD_SAR_ADC_14, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5, PINMUX_FOR_TTL18_MODE_2},
    {PAD_SAR_ADC_14, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT0, PINMUX_FOR_TTL16_MODE_1},
    {PAD_SAR_ADC_14, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1, PINMUX_FOR_TTL16_MODE_2},
    {PAD_SAR_ADC_14, PADTOP_BANK, REG_RMII1_MODE, REG_RMII1_MODE_MASK, BIT1, PINMUX_FOR_RMII1_MODE_1},
};
const ST_PadMuxInfo sar_adc_15_tbl[] = {
    {PAD_SAR_ADC_15, PADGPIO2_BANK, REG_SAR_ADC_15_GPIO_MODE, REG_SAR_ADC_15_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_15, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_SAR_ADC_15, PADTOP_BANK, REG_ADC15_MODE, REG_ADC15_MODE_MASK, BIT15, PINMUX_FOR_ADC15_MODE_1},
    {PAD_SAR_ADC_15, PADTOP_BANK, REG_UART5_MODE, REG_UART5_MODE_MASK, BIT5, PINMUX_FOR_UART5_MODE_2},
    {PAD_SAR_ADC_15, PADTOP_BANK, REG_LED1_MODE, REG_LED1_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_LED1_MODE_3},
    {PAD_SAR_ADC_15, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_15, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT4, PINMUX_FOR_TTL18_MODE_1},
    {PAD_SAR_ADC_15, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5, PINMUX_FOR_TTL18_MODE_2},
    {PAD_SAR_ADC_15, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT0, PINMUX_FOR_TTL16_MODE_1},
    {PAD_SAR_ADC_15, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1, PINMUX_FOR_TTL16_MODE_2},
    {PAD_SAR_ADC_15, PADTOP_BANK, REG_RMII1_MODE, REG_RMII1_MODE_MASK, BIT1, PINMUX_FOR_RMII1_MODE_1},
};
const ST_PadMuxInfo sar_adc_16_tbl[] = {
    {PAD_SAR_ADC_16, PADGPIO2_BANK, REG_SAR_ADC_16_GPIO_MODE, REG_SAR_ADC_16_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_16, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_SAR_ADC_16, PADTOP_BANK, REG_ADC16_MODE, REG_ADC16_MODE_MASK, BIT0, PINMUX_FOR_ADC16_MODE_1},
    {PAD_SAR_ADC_16, PADTOP_BANK, REG_FUART_MODE, REG_FUART_MODE_MASK, BIT9, PINMUX_FOR_FUART_MODE_2},
    {PAD_SAR_ADC_16, PADTOP_BANK, REG_FUART_2W_MODE, REG_FUART_2W_MODE_MASK, BIT13, PINMUX_FOR_FUART_2W_MODE_2},
    {PAD_SAR_ADC_16, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_16, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT4, PINMUX_FOR_TTL18_MODE_1},
    {PAD_SAR_ADC_16, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5, PINMUX_FOR_TTL18_MODE_2},
    {PAD_SAR_ADC_16, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT0, PINMUX_FOR_TTL16_MODE_1},
    {PAD_SAR_ADC_16, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1, PINMUX_FOR_TTL16_MODE_2},
    {PAD_SAR_ADC_16, PADTOP_BANK, REG_RMII1_MODE, REG_RMII1_MODE_MASK, BIT1, PINMUX_FOR_RMII1_MODE_1},
};
const ST_PadMuxInfo sar_adc_17_tbl[] = {
    {PAD_SAR_ADC_17, PADGPIO2_BANK, REG_SAR_ADC_17_GPIO_MODE, REG_SAR_ADC_17_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_17, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_SAR_ADC_17, PADTOP_BANK, REG_ADC17_MODE, REG_ADC17_MODE_MASK, BIT1, PINMUX_FOR_ADC17_MODE_1},
    {PAD_SAR_ADC_17, PADTOP_BANK, REG_FUART_MODE, REG_FUART_MODE_MASK, BIT9, PINMUX_FOR_FUART_MODE_2},
    {PAD_SAR_ADC_17, PADTOP_BANK, REG_FUART_2W_MODE, REG_FUART_2W_MODE_MASK, BIT13, PINMUX_FOR_FUART_2W_MODE_2},
    {PAD_SAR_ADC_17, PADTOP_BANK, REG_I2S0_MCK_MODE, REG_I2S0_MCK_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_I2S0_MCK_MODE_3},
    {PAD_SAR_ADC_17, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_17, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT4, PINMUX_FOR_TTL18_MODE_1},
    {PAD_SAR_ADC_17, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5, PINMUX_FOR_TTL18_MODE_2},
    {PAD_SAR_ADC_17, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT0, PINMUX_FOR_TTL16_MODE_1},
    {PAD_SAR_ADC_17, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1, PINMUX_FOR_TTL16_MODE_2},
    {PAD_SAR_ADC_17, PADTOP_BANK, REG_RMII1_MODE, REG_RMII1_MODE_MASK, BIT1, PINMUX_FOR_RMII1_MODE_1},
};
const ST_PadMuxInfo sar_adc_18_tbl[] = {
    {PAD_SAR_ADC_18, PADGPIO2_BANK, REG_SAR_ADC_18_GPIO_MODE, REG_SAR_ADC_18_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_18, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_SAR_ADC_18, PADTOP_BANK, REG_ADC18_MODE, REG_ADC18_MODE_MASK, BIT2, PINMUX_FOR_ADC18_MODE_1},
    {PAD_SAR_ADC_18, PADTOP_BANK, REG_FUART_MODE, REG_FUART_MODE_MASK, BIT9, PINMUX_FOR_FUART_MODE_2},
    {PAD_SAR_ADC_18, PADTOP_BANK, REG_UART4_MODE, REG_UART4_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_UART4_MODE_3},
    {PAD_SAR_ADC_18, PADTOP_BANK, REG_PWM_IN6_MODE, REG_PWM_IN6_MODE_MASK, BIT13 | BIT12, PINMUX_FOR_PWM_IN6_MODE_3},
    {PAD_SAR_ADC_18, PADTOP_BANK, REG_I2S0_TX_MODE, REG_I2S0_TX_MODE_MASK, BIT13 | BIT12, PINMUX_FOR_I2S0_TX_MODE_3},
    {PAD_SAR_ADC_18, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_18, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT4, PINMUX_FOR_TTL18_MODE_1},
    {PAD_SAR_ADC_18, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5, PINMUX_FOR_TTL18_MODE_2},
    {PAD_SAR_ADC_18, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT0, PINMUX_FOR_TTL16_MODE_1},
    {PAD_SAR_ADC_18, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1, PINMUX_FOR_TTL16_MODE_2},
    {PAD_SAR_ADC_18, PADTOP_BANK, REG_RMII1_MODE, REG_RMII1_MODE_MASK, BIT1, PINMUX_FOR_RMII1_MODE_1},
};
const ST_PadMuxInfo sar_adc_19_tbl[] = {
    {PAD_SAR_ADC_19, PADGPIO2_BANK, REG_SAR_ADC_19_GPIO_MODE, REG_SAR_ADC_19_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_19, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_SAR_ADC_19, PADTOP_BANK, REG_ADC19_MODE, REG_ADC19_MODE_MASK, BIT3, PINMUX_FOR_ADC19_MODE_1},
    {PAD_SAR_ADC_19, PADTOP_BANK, REG_SPI0_CZ1_MODE, REG_SPI0_CZ1_MODE_MASK, BIT5, PINMUX_FOR_SPI0_CZ1_MODE_2},
    {PAD_SAR_ADC_19, PADTOP_BANK, REG_FUART_MODE, REG_FUART_MODE_MASK, BIT9, PINMUX_FOR_FUART_MODE_2},
    {PAD_SAR_ADC_19, PADTOP_BANK, REG_UART4_MODE, REG_UART4_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_UART4_MODE_3},
    {PAD_SAR_ADC_19, PADTOP_BANK, REG_PWM_IN7_MODE, REG_PWM_IN7_MODE_MASK, BIT15 | BIT14, PINMUX_FOR_PWM_IN7_MODE_3},
    {PAD_SAR_ADC_19, PADTOP_BANK, REG_I2S0_TX_MODE, REG_I2S0_TX_MODE_MASK, BIT13 | BIT12, PINMUX_FOR_I2S0_TX_MODE_3},
    {PAD_SAR_ADC_19, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_19, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT4, PINMUX_FOR_TTL18_MODE_1},
    {PAD_SAR_ADC_19, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5, PINMUX_FOR_TTL18_MODE_2},
    {PAD_SAR_ADC_19, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT0, PINMUX_FOR_TTL16_MODE_1},
    {PAD_SAR_ADC_19, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT1, PINMUX_FOR_TTL16_MODE_2},
};
const ST_PadMuxInfo sar_adc_20_tbl[] = {
    {PAD_SAR_ADC_20, PADGPIO2_BANK, REG_SAR_ADC_20_GPIO_MODE, REG_SAR_ADC_20_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_20, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_SAR_ADC_20, PADTOP_BANK, REG_ADC20_MODE, REG_ADC20_MODE_MASK, BIT4, PINMUX_FOR_ADC20_MODE_1},
    {PAD_SAR_ADC_20, PADTOP_BANK, REG_SPI0_MODE, REG_SPI0_MODE_MASK, BIT1, PINMUX_FOR_SPI0_MODE_2},
    {PAD_SAR_ADC_20, PADTOP_BANK, REG_I2S0_RX_MODE, REG_I2S0_RX_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_I2S0_RX_MODE_3},
    {PAD_SAR_ADC_20, PADTOP_BANK, REG_I2S0_RXTX_MODE, REG_I2S0_RXTX_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_I2S0_RXTX_MODE_3},
    {PAD_SAR_ADC_20, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_20, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5, PINMUX_FOR_TTL18_MODE_2},
    {PAD_SAR_ADC_20, PADTOP_BANK, REG_IR0_MODE, REG_IR0_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_IR0_MODE_3},
};
const ST_PadMuxInfo sar_adc_21_tbl[] = {
    {PAD_SAR_ADC_21, PADGPIO2_BANK, REG_SAR_ADC_21_GPIO_MODE, REG_SAR_ADC_21_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_21, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_SAR_ADC_21, PADTOP_BANK, REG_ADC21_MODE, REG_ADC21_MODE_MASK, BIT5, PINMUX_FOR_ADC21_MODE_1},
    {PAD_SAR_ADC_21, PADTOP_BANK, REG_SPI0_MODE, REG_SPI0_MODE_MASK, BIT1, PINMUX_FOR_SPI0_MODE_2},
    {PAD_SAR_ADC_21, PADTOP_BANK, REG_I2S0_RX_MODE, REG_I2S0_RX_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_I2S0_RX_MODE_3},
    {PAD_SAR_ADC_21, PADTOP_BANK, REG_I2S0_RXTX_MODE, REG_I2S0_RXTX_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_I2S0_RXTX_MODE_3},
    {PAD_SAR_ADC_21, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_21, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT5, PINMUX_FOR_TTL18_MODE_2},
    {PAD_SAR_ADC_21, PADTOP_BANK, REG_IR1_MODE, REG_IR1_MODE_MASK, BIT3 | BIT2, PINMUX_FOR_IR1_MODE_3},
};
const ST_PadMuxInfo sar_adc_22_tbl[] = {
    {PAD_SAR_ADC_22, PADGPIO2_BANK, REG_SAR_ADC_22_GPIO_MODE, REG_SAR_ADC_22_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_22, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_TEST_IN_MODE_3},
    {PAD_SAR_ADC_22, PADTOP_BANK, REG_ADC22_MODE, REG_ADC22_MODE_MASK, BIT6, PINMUX_FOR_ADC22_MODE_1},
    {PAD_SAR_ADC_22, PADTOP_BANK, REG_SPI0_MODE, REG_SPI0_MODE_MASK, BIT1, PINMUX_FOR_SPI0_MODE_2},
    {PAD_SAR_ADC_22, PADTOP_BANK, REG_I2S0_RX_MODE, REG_I2S0_RX_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_I2S0_RX_MODE_3},
    {PAD_SAR_ADC_22, PADTOP_BANK, REG_I2S0_RXTX_MODE, REG_I2S0_RXTX_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_I2S0_RXTX_MODE_3},
    {PAD_SAR_ADC_22, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_22, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT4, PINMUX_FOR_TTL18_MODE_1},
    {PAD_SAR_ADC_22, PADTOP_BANK, REG_IR2_MODE, REG_IR2_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_IR2_MODE_3},
};
const ST_PadMuxInfo sar_adc_23_tbl[] = {
    {PAD_SAR_ADC_23, PADGPIO2_BANK, REG_SAR_ADC_23_GPIO_MODE, REG_SAR_ADC_23_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_SAR_ADC_23, PADTOP_BANK, REG_ADC23_MODE, REG_ADC23_MODE_MASK, BIT7, PINMUX_FOR_ADC23_MODE_1},
    {PAD_SAR_ADC_23, PADTOP_BANK, REG_SPI0_MODE, REG_SPI0_MODE_MASK, BIT1, PINMUX_FOR_SPI0_MODE_2},
    {PAD_SAR_ADC_23, PADTOP_BANK, REG_I2S0_TX_MODE, REG_I2S0_TX_MODE_MASK, BIT13 | BIT12, PINMUX_FOR_I2S0_TX_MODE_3},
    {PAD_SAR_ADC_23, PADTOP_BANK, REG_I2S0_RXTX_MODE, REG_I2S0_RXTX_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_I2S0_RXTX_MODE_3},
    {PAD_SAR_ADC_23, PADTOP_BANK, REG_TTL24_MODE, REG_TTL24_MODE_MASK, BIT8, PINMUX_FOR_TTL24_MODE_1},
    {PAD_SAR_ADC_23, PADTOP_BANK, REG_TTL18_MODE, REG_TTL18_MODE_MASK, BIT4, PINMUX_FOR_TTL18_MODE_1},
    {PAD_SAR_ADC_23, PADTOP_BANK, REG_TTL16_MODE, REG_TTL16_MODE_MASK, BIT0, PINMUX_FOR_TTL16_MODE_1},
    {PAD_SAR_ADC_23, PADTOP_BANK, REG_IR3_MODE, REG_IR3_MODE_MASK, BIT7 | BIT6, PINMUX_FOR_IR3_MODE_3},
};
const ST_PadMuxInfo sr_io4_tbl[] = {
    {PAD_SR_IO4, PADGPIO2_BANK, REG_SR_IO4_GPIO_MODE, REG_SR_IO4_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_SR_IO4, PADTOP_BANK, REG_SR_SLAVE_XLK_MODE, REG_SR_SLAVE_XLK_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_SR_SLAVE_XLK_MODE_3},
};
const ST_PadMuxInfo rgmii0_mclk_tbl[] = {
    {PAD_RGMII0_MCLK, PADGPIO2_BANK, REG_RGMII0_MCLK_GPIO_MODE, REG_RGMII0_MCLK_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_RGMII0_MCLK, PADTOP_BANK, REG_UART3_MODE, REG_UART3_MODE_MASK, BIT13, PINMUX_FOR_UART3_MODE_2},
    {PAD_RGMII0_MCLK, PADTOP_BANK, REG_GPHY0_REF_MODE, REG_GPHY0_REF_MODE_MASK, BIT0, PINMUX_FOR_GPHY0_REF_MODE_1},
};
const ST_PadMuxInfo rgmii0_rstn_tbl[] = {
    {PAD_RGMII0_RSTN, PADGPIO2_BANK, REG_RGMII0_RSTN_GPIO_MODE, REG_RGMII0_RSTN_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_RGMII0_RSTN, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_RGMII0_RSTN, PADTOP_BANK, REG_UART3_MODE, REG_UART3_MODE_MASK, BIT13, PINMUX_FOR_UART3_MODE_2},
    {PAD_RGMII0_RSTN, PADTOP_BANK, REG_PWM_OUT12_MODE, REG_PWM_OUT12_MODE_MASK, BIT1 | BIT0,
     PINMUX_FOR_PWM_OUT12_MODE_3},
    {PAD_RGMII0_RSTN, PADTOP_BANK, REG_PWM_OUT17_MODE, REG_PWM_OUT17_MODE_MASK, BIT6, PINMUX_FOR_PWM_OUT17_MODE_4},
};
const ST_PadMuxInfo rgmii0_rxclk_tbl[] = {
    {PAD_RGMII0_RXCLK, PADGPIO2_BANK, REG_RGMII0_RXCLK_GPIO_MODE, REG_RGMII0_RXCLK_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_RGMII0_RXCLK, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_RGMII0_RXCLK, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_RGMII0_RXCLK, PADTOP_BANK, REG_PWM_OUT17_MODE, REG_PWM_OUT17_MODE_MASK, BIT5, PINMUX_FOR_PWM_OUT17_MODE_2},
    {PAD_RGMII0_RXCLK, PADTOP_BANK, REG_PWM_IN4_MODE, REG_PWM_IN4_MODE_MASK, BIT9, PINMUX_FOR_PWM_IN4_MODE_2},
    {PAD_RGMII0_RXCLK, PADTOP_BANK, REG_RGMII0_MODE, REG_RGMII0_MODE_MASK, BIT0, PINMUX_FOR_RGMII0_MODE_1},
    {PAD_RGMII0_RXCLK, PADTOP_BANK, REG_RMII_MODE, REG_RMII_MODE_MASK, BIT8, PINMUX_FOR_RMII_MODE_1},
};
const ST_PadMuxInfo rgmii0_rxctl_tbl[] = {
    {PAD_RGMII0_RXCTL, PADGPIO2_BANK, REG_RGMII0_RXCTL_GPIO_MODE, REG_RGMII0_RXCTL_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_RGMII0_RXCTL, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_RGMII0_RXCTL, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_RGMII0_RXCTL, PADTOP_BANK, REG_PWM_OUT18_MODE, REG_PWM_OUT18_MODE_MASK, BIT13, PINMUX_FOR_PWM_OUT18_MODE_2},
    {PAD_RGMII0_RXCTL, PADTOP_BANK, REG_PWM_IN5_MODE, REG_PWM_IN5_MODE_MASK, BIT11, PINMUX_FOR_PWM_IN5_MODE_2},
    {PAD_RGMII0_RXCTL, PADTOP_BANK, REG_RGMII0_MODE, REG_RGMII0_MODE_MASK, BIT0, PINMUX_FOR_RGMII0_MODE_1},
    {PAD_RGMII0_RXCTL, PADTOP_BANK, REG_RMII_MODE, REG_RMII_MODE_MASK, BIT8, PINMUX_FOR_RMII_MODE_1},
};
const ST_PadMuxInfo rgmii0_rxd0_tbl[] = {
    {PAD_RGMII0_RXD0, PADGPIO2_BANK, REG_RGMII0_RXD0_GPIO_MODE, REG_RGMII0_RXD0_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_RGMII0_RXD0, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_RGMII0_RXD0, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_RGMII0_RXD0, PADTOP_BANK, REG_I2C2_MODE, REG_I2C2_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_I2C2_MODE_3},
    {PAD_RGMII0_RXD0, PADTOP_BANK, REG_PWM_OUT19_MODE, REG_PWM_OUT19_MODE_MASK, BIT15, PINMUX_FOR_PWM_OUT19_MODE_2},
    {PAD_RGMII0_RXD0, PADTOP_BANK, REG_RGMII0_MODE, REG_RGMII0_MODE_MASK, BIT0, PINMUX_FOR_RGMII0_MODE_1},
    {PAD_RGMII0_RXD0, PADTOP_BANK, REG_RMII_MODE, REG_RMII_MODE_MASK, BIT8, PINMUX_FOR_RMII_MODE_1},
};
const ST_PadMuxInfo rgmii0_rxd1_tbl[] = {
    {PAD_RGMII0_RXD1, PADGPIO2_BANK, REG_RGMII0_RXD1_GPIO_MODE, REG_RGMII0_RXD1_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_RGMII0_RXD1, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_RGMII0_RXD1, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_RGMII0_RXD1, PADTOP_BANK, REG_I2C2_MODE, REG_I2C2_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_I2C2_MODE_3},
    {PAD_RGMII0_RXD1, PADTOP_BANK, REG_PWM_OUT13_MODE, REG_PWM_OUT13_MODE_MASK, BIT11 | BIT10,
     PINMUX_FOR_PWM_OUT13_MODE_3},
    {PAD_RGMII0_RXD1, PADTOP_BANK, REG_RGMII0_MODE, REG_RGMII0_MODE_MASK, BIT0, PINMUX_FOR_RGMII0_MODE_1},
    {PAD_RGMII0_RXD1, PADTOP_BANK, REG_RMII_MODE, REG_RMII_MODE_MASK, BIT8, PINMUX_FOR_RMII_MODE_1},
};
const ST_PadMuxInfo rgmii0_rxd2_tbl[] = {
    {PAD_RGMII0_RXD2, PADGPIO2_BANK, REG_RGMII0_RXD2_GPIO_MODE, REG_RGMII0_RXD2_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_RGMII0_RXD2, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_RGMII0_RXD2, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_RGMII0_RXD2, PADTOP_BANK, REG_SPI1_MODE, REG_SPI1_MODE_MASK, BIT4, PINMUX_FOR_SPI1_MODE_2},
    {PAD_RGMII0_RXD2, PADTOP_BANK, REG_UART2_MODE, REG_UART2_MODE_MASK, BIT9, PINMUX_FOR_UART2_MODE_2},
    {PAD_RGMII0_RXD2, PADTOP_BANK, REG_PWM_OUT14_MODE, REG_PWM_OUT14_MODE_MASK, BIT13 | BIT12,
     PINMUX_FOR_PWM_OUT14_MODE_3},
    {PAD_RGMII0_RXD2, PADTOP_BANK, REG_RGMII0_MODE, REG_RGMII0_MODE_MASK, BIT0, PINMUX_FOR_RGMII0_MODE_1},
};
const ST_PadMuxInfo rgmii0_rxd3_tbl[] = {
    {PAD_RGMII0_RXD3, PADGPIO2_BANK, REG_RGMII0_RXD3_GPIO_MODE, REG_RGMII0_RXD3_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_RGMII0_RXD3, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_RGMII0_RXD3, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_RGMII0_RXD3, PADTOP_BANK, REG_SPI1_MODE, REG_SPI1_MODE_MASK, BIT4, PINMUX_FOR_SPI1_MODE_2},
    {PAD_RGMII0_RXD3, PADTOP_BANK, REG_UART2_MODE, REG_UART2_MODE_MASK, BIT9, PINMUX_FOR_UART2_MODE_2},
    {PAD_RGMII0_RXD3, PADTOP_BANK, REG_PWM_OUT15_MODE, REG_PWM_OUT15_MODE_MASK, BIT15 | BIT14,
     PINMUX_FOR_PWM_OUT15_MODE_3},
    {PAD_RGMII0_RXD3, PADTOP_BANK, REG_RGMII0_MODE, REG_RGMII0_MODE_MASK, BIT0, PINMUX_FOR_RGMII0_MODE_1},
};
const ST_PadMuxInfo rgmii0_txclk_tbl[] = {
    {PAD_RGMII0_TXCLK, PADGPIO2_BANK, REG_RGMII0_TXCLK_GPIO_MODE, REG_RGMII0_TXCLK_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_RGMII0_TXCLK, CHIPTOP_BANK, REG_TEST_IN_MODE, REG_TEST_IN_MODE_MASK, BIT1, PINMUX_FOR_TEST_IN_MODE_2},
    {PAD_RGMII0_TXCLK, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_RGMII0_TXCLK, PADTOP_BANK, REG_SPI1_CZ1_MODE, REG_SPI1_CZ1_MODE_MASK, BIT6, PINMUX_FOR_SPI1_CZ1_MODE_1},
    {PAD_RGMII0_TXCLK, PADTOP_BANK, REG_UART5_MODE, REG_UART5_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_UART5_MODE_3},
    {PAD_RGMII0_TXCLK, PADTOP_BANK, REG_PWM_OUT16_MODE, REG_PWM_OUT16_MODE_MASK, BIT9 | BIT8,
     PINMUX_FOR_PWM_OUT16_MODE_3},
    {PAD_RGMII0_TXCLK, PADTOP_BANK, REG_RGMII0_MODE, REG_RGMII0_MODE_MASK, BIT0, PINMUX_FOR_RGMII0_MODE_1},
};
const ST_PadMuxInfo rgmii0_txctl_tbl[] = {
    {PAD_RGMII0_TXCTL, PADGPIO2_BANK, REG_RGMII0_TXCTL_GPIO_MODE, REG_RGMII0_TXCTL_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_RGMII0_TXCTL, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_RGMII0_TXCTL, PADTOP_BANK, REG_UART5_MODE, REG_UART5_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_UART5_MODE_3},
    {PAD_RGMII0_TXCTL, PADTOP_BANK, REG_PWM_OUT17_MODE, REG_PWM_OUT17_MODE_MASK, BIT5 | BIT4,
     PINMUX_FOR_PWM_OUT17_MODE_3},
    {PAD_RGMII0_TXCTL, PADTOP_BANK, REG_RGMII0_MODE, REG_RGMII0_MODE_MASK, BIT0, PINMUX_FOR_RGMII0_MODE_1},
    {PAD_RGMII0_TXCTL, PADTOP_BANK, REG_RMII_MODE, REG_RMII_MODE_MASK, BIT8, PINMUX_FOR_RMII_MODE_1},
};
const ST_PadMuxInfo rgmii0_txd0_tbl[] = {
    {PAD_RGMII0_TXD0, PADGPIO2_BANK, REG_RGMII0_TXD0_GPIO_MODE, REG_RGMII0_TXD0_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_RGMII0_TXD0, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_RGMII0_TXD0, PADTOP_BANK, REG_PWM_OUT18_MODE, REG_PWM_OUT18_MODE_MASK, BIT13 | BIT12,
     PINMUX_FOR_PWM_OUT18_MODE_3},
    {PAD_RGMII0_TXD0, PADTOP_BANK, REG_RGMII0_MODE, REG_RGMII0_MODE_MASK, BIT0, PINMUX_FOR_RGMII0_MODE_1},
    {PAD_RGMII0_TXD0, PADTOP_BANK, REG_RMII_MODE, REG_RMII_MODE_MASK, BIT8, PINMUX_FOR_RMII_MODE_1},
};
const ST_PadMuxInfo rgmii0_txd1_tbl[] = {
    {PAD_RGMII0_TXD1, PADGPIO2_BANK, REG_RGMII0_TXD1_GPIO_MODE, REG_RGMII0_TXD1_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_RGMII0_TXD1, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_RGMII0_TXD1, PADTOP_BANK, REG_PWM_OUT19_MODE, REG_PWM_OUT19_MODE_MASK, BIT15 | BIT14,
     PINMUX_FOR_PWM_OUT19_MODE_3},
    {PAD_RGMII0_TXD1, PADTOP_BANK, REG_RGMII0_MODE, REG_RGMII0_MODE_MASK, BIT0, PINMUX_FOR_RGMII0_MODE_1},
    {PAD_RGMII0_TXD1, PADTOP_BANK, REG_RMII_MODE, REG_RMII_MODE_MASK, BIT8, PINMUX_FOR_RMII_MODE_1},
};
const ST_PadMuxInfo rgmii0_txd2_tbl[] = {
    {PAD_RGMII0_TXD2, PADGPIO2_BANK, REG_RGMII0_TXD2_GPIO_MODE, REG_RGMII0_TXD2_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_RGMII0_TXD2, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_RGMII0_TXD2, PADTOP_BANK, REG_I2C5_MODE, REG_I2C5_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_I2C5_MODE_3},
    {PAD_RGMII0_TXD2, PADTOP_BANK, REG_SPI1_MODE, REG_SPI1_MODE_MASK, BIT4, PINMUX_FOR_SPI1_MODE_2},
    {PAD_RGMII0_TXD2, PADTOP_BANK, REG_RGMII0_MODE, REG_RGMII0_MODE_MASK, BIT0, PINMUX_FOR_RGMII0_MODE_1},
};
const ST_PadMuxInfo rgmii0_txd3_tbl[] = {
    {PAD_RGMII0_TXD3, PADGPIO2_BANK, REG_RGMII0_TXD3_GPIO_MODE, REG_RGMII0_TXD3_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_RGMII0_TXD3, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_RGMII0_TXD3, PADTOP_BANK, REG_I2C5_MODE, REG_I2C5_MODE_MASK, BIT9 | BIT8, PINMUX_FOR_I2C5_MODE_3},
    {PAD_RGMII0_TXD3, PADTOP_BANK, REG_SPI1_MODE, REG_SPI1_MODE_MASK, BIT4, PINMUX_FOR_SPI1_MODE_2},
    {PAD_RGMII0_TXD3, PADTOP_BANK, REG_RGMII0_MODE, REG_RGMII0_MODE_MASK, BIT0, PINMUX_FOR_RGMII0_MODE_1},
};
const ST_PadMuxInfo rgmii0_mdio_tbl[] = {
    {PAD_RGMII0_MDIO, PADGPIO2_BANK, REG_RGMII0_MDIO_GPIO_MODE, REG_RGMII0_MDIO_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_RGMII0_MDIO, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_RGMII0_MDIO, PADTOP_BANK, REG_I2C4_MODE, REG_I2C4_MODE_MASK, BIT6, PINMUX_FOR_I2C4_MODE_4},
    {PAD_RGMII0_MDIO, PADTOP_BANK, REG_PWM_IN6_MODE, REG_PWM_IN6_MODE_MASK, BIT13, PINMUX_FOR_PWM_IN6_MODE_2},
    {PAD_RGMII0_MDIO, PADTOP_BANK, REG_RGMII0_MODE, REG_RGMII0_MODE_MASK, BIT0, PINMUX_FOR_RGMII0_MODE_1},
    {PAD_RGMII0_MDIO, PADTOP_BANK, REG_RMII_MODE, REG_RMII_MODE_MASK, BIT8, PINMUX_FOR_RMII_MODE_1},
};
const ST_PadMuxInfo rgmii0_mdc_tbl[] = {
    {PAD_RGMII0_MDC, PADGPIO2_BANK, REG_RGMII0_MDC_GPIO_MODE, REG_RGMII0_MDC_GPIO_MODE_MASK, BIT3,
     PINMUX_FOR_GPIO_MODE},
    {PAD_RGMII0_MDC, CHIPTOP_BANK, REG_TEST_OUT_MODE, REG_TEST_OUT_MODE_MASK, BIT4, PINMUX_FOR_TEST_OUT_MODE_1},
    {PAD_RGMII0_MDC, PADTOP_BANK, REG_I2C4_MODE, REG_I2C4_MODE_MASK, BIT6, PINMUX_FOR_I2C4_MODE_4},
    {PAD_RGMII0_MDC, PADTOP_BANK, REG_PWM_IN7_MODE, REG_PWM_IN7_MODE_MASK, BIT15, PINMUX_FOR_PWM_IN7_MODE_2},
    {PAD_RGMII0_MDC, PADTOP_BANK, REG_RGMII0_MODE, REG_RGMII0_MODE_MASK, BIT0, PINMUX_FOR_RGMII0_MODE_1},
    {PAD_RGMII0_MDC, PADTOP_BANK, REG_RMII_MODE, REG_RMII_MODE_MASK, BIT8, PINMUX_FOR_RMII_MODE_1},
};
const ST_PadMuxInfo uart_rx2_tbl[] = {
    {PAD_UART_RX2, PADGPIO2_BANK, REG_UART_RX2_GPIO_MODE, REG_UART_RX2_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_UART_RX2, PADTOP_BANK, REG_UART2_MODE, REG_UART2_MODE_MASK, BIT8, PINMUX_FOR_UART2_MODE_1},
};
const ST_PadMuxInfo uart_tx2_tbl[] = {
    {PAD_UART_TX2, PADGPIO2_BANK, REG_UART_TX2_GPIO_MODE, REG_UART_TX2_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_UART_TX2, PADTOP_BANK, REG_UART2_MODE, REG_UART2_MODE_MASK, BIT8, PINMUX_FOR_UART2_MODE_1},
};
const ST_PadMuxInfo uart_rx3_tbl[] = {
    {PAD_UART_RX3, PADGPIO2_BANK, REG_UART_RX3_GPIO_MODE, REG_UART_RX3_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_UART_RX3, PADTOP_BANK, REG_UART3_MODE, REG_UART3_MODE_MASK, BIT12, PINMUX_FOR_UART3_MODE_1},
    {PAD_UART_RX3, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo uart_tx3_tbl[] = {
    {PAD_UART_TX3, PADGPIO2_BANK, REG_UART_TX3_GPIO_MODE, REG_UART_TX3_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_UART_TX3, PADTOP_BANK, REG_UART3_MODE, REG_UART3_MODE_MASK, BIT12, PINMUX_FOR_UART3_MODE_1},
    {PAD_UART_TX3, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo uart_rx4_tbl[] = {
    {PAD_UART_RX4, PADGPIO2_BANK, REG_UART_RX4_GPIO_MODE, REG_UART_RX4_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_UART_RX4, PADTOP_BANK, REG_UART4_MODE, REG_UART4_MODE_MASK, BIT0, PINMUX_FOR_UART4_MODE_1},
    {PAD_UART_RX4, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo uart_tx4_tbl[] = {
    {PAD_UART_TX4, PADGPIO2_BANK, REG_UART_TX4_GPIO_MODE, REG_UART_TX4_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_UART_TX4, PADTOP_BANK, REG_UART4_MODE, REG_UART4_MODE_MASK, BIT0, PINMUX_FOR_UART4_MODE_1},
    {PAD_UART_TX4, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo uart_rx1_tbl[] = {
    {PAD_UART_RX1, PADGPIO2_BANK, REG_UART_RX1_GPIO_MODE, REG_UART_RX1_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_UART_RX1, PADTOP_BANK, REG_UART1_MODE, REG_UART1_MODE_MASK, BIT4, PINMUX_FOR_UART1_MODE_1},
    {PAD_UART_RX1, PADTOP_BANK, REG_PWM_IN2_MODE, REG_PWM_IN2_MODE_MASK, BIT5 | BIT4, PINMUX_FOR_PWM_IN2_MODE_3},
    {PAD_UART_RX1, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo uart_tx1_tbl[] = {
    {PAD_UART_TX1, PADGPIO2_BANK, REG_UART_TX1_GPIO_MODE, REG_UART_TX1_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_UART_TX1, PADTOP_BANK, REG_UART1_MODE, REG_UART1_MODE_MASK, BIT4, PINMUX_FOR_UART1_MODE_1},
    {PAD_UART_TX1, PADTOP_BANK, REG_PWM_IN3_MODE, REG_PWM_IN3_MODE_MASK, BIT7 | BIT6, PINMUX_FOR_PWM_IN3_MODE_3},
    {PAD_UART_TX1, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo fuart_rx_tbl[] = {
    {PAD_FUART_RX, PADGPIO2_BANK, REG_FUART_RX_GPIO_MODE, REG_FUART_RX_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_FUART_RX, PADTOP_BANK, REG_EJ_MODE, REG_EJ_MODE_MASK, BIT8, PINMUX_FOR_EJ_MODE_2},
    {PAD_FUART_RX, PADTOP_BANK, REG_RISCV_EJ_MODE, REG_RISCV_EJ_MODE_MASK, BIT1, PINMUX_FOR_RISCV_EJ_MODE_2},
    {PAD_FUART_RX, PADTOP_BANK, REG_FUART_MODE, REG_FUART_MODE_MASK, BIT8, PINMUX_FOR_FUART_MODE_1},
    {PAD_FUART_RX, PADTOP_BANK, REG_FUART_2W_MODE, REG_FUART_2W_MODE_MASK, BIT12, PINMUX_FOR_FUART_2W_MODE_1},
    {PAD_FUART_RX, PADTOP_BANK, REG_PWM_OUT4_MODE, REG_PWM_OUT4_MODE_MASK, BIT1, PINMUX_FOR_PWM_OUT4_MODE_2},
    {PAD_FUART_RX, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo fuart_tx_tbl[] = {
    {PAD_FUART_TX, PADGPIO2_BANK, REG_FUART_TX_GPIO_MODE, REG_FUART_TX_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_FUART_TX, PADTOP_BANK, REG_EJ_MODE, REG_EJ_MODE_MASK, BIT8, PINMUX_FOR_EJ_MODE_2},
    {PAD_FUART_TX, PADTOP_BANK, REG_RISCV_EJ_MODE, REG_RISCV_EJ_MODE_MASK, BIT1, PINMUX_FOR_RISCV_EJ_MODE_2},
    {PAD_FUART_TX, PADTOP_BANK, REG_FUART_MODE, REG_FUART_MODE_MASK, BIT8, PINMUX_FOR_FUART_MODE_1},
    {PAD_FUART_TX, PADTOP_BANK, REG_FUART_2W_MODE, REG_FUART_2W_MODE_MASK, BIT12, PINMUX_FOR_FUART_2W_MODE_1},
    {PAD_FUART_TX, PADTOP_BANK, REG_PWM_OUT5_MODE, REG_PWM_OUT5_MODE_MASK, BIT9, PINMUX_FOR_PWM_OUT5_MODE_2},
    {PAD_FUART_TX, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo fuart_rts_tbl[] = {
    {PAD_FUART_RTS, PADGPIO2_BANK, REG_FUART_RTS_GPIO_MODE, REG_FUART_RTS_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_FUART_RTS, PADTOP_BANK, REG_EJ_MODE, REG_EJ_MODE_MASK, BIT8, PINMUX_FOR_EJ_MODE_2},
    {PAD_FUART_RTS, PADTOP_BANK, REG_RISCV_EJ_MODE, REG_RISCV_EJ_MODE_MASK, BIT1, PINMUX_FOR_RISCV_EJ_MODE_2},
    {PAD_FUART_RTS, PADTOP_BANK, REG_I2C0_MODE, REG_I2C0_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_I2C0_MODE_3},
    {PAD_FUART_RTS, PADTOP_BANK, REG_FUART_MODE, REG_FUART_MODE_MASK, BIT8, PINMUX_FOR_FUART_MODE_1},
    {PAD_FUART_RTS, PADTOP_BANK, REG_UART5_MODE, REG_UART5_MODE_MASK, BIT6 | BIT5, PINMUX_FOR_UART5_MODE_6},
    {PAD_FUART_RTS, PADTOP_BANK, REG_PWM_OUT6_MODE, REG_PWM_OUT6_MODE_MASK, BIT11, PINMUX_FOR_PWM_OUT6_MODE_2},
    {PAD_FUART_RTS, PADTOP_BANK, REG_PWM_INT_MODE, REG_PWM_INT_MODE_MASK, BIT0, PINMUX_FOR_PWM_INT_MODE_1},
    {PAD_FUART_RTS, PADTOP_BANK, REG_OTP_TEST, REG_OTP_TEST_MASK, BIT8, PINMUX_FOR_OTP_TEST_1},
};
const ST_PadMuxInfo fuart_cts_tbl[] = {
    {PAD_FUART_CTS, PADGPIO2_BANK, REG_FUART_CTS_GPIO_MODE, REG_FUART_CTS_GPIO_MODE_MASK, BIT3, PINMUX_FOR_GPIO_MODE},
    {PAD_FUART_CTS, PADTOP_BANK, REG_EJ_MODE, REG_EJ_MODE_MASK, BIT8, PINMUX_FOR_EJ_MODE_2},
    {PAD_FUART_CTS, PADTOP_BANK, REG_RISCV_EJ_MODE, REG_RISCV_EJ_MODE_MASK, BIT1, PINMUX_FOR_RISCV_EJ_MODE_2},
    {PAD_FUART_CTS, PADTOP_BANK, REG_ADC_INT_MODE, REG_ADC_INT_MODE_MASK, BIT8, PINMUX_FOR_ADC_INT_MODE_1},
    {PAD_FUART_CTS, PADTOP_BANK, REG_I2C0_MODE, REG_I2C0_MODE_MASK, BIT1 | BIT0, PINMUX_FOR_I2C0_MODE_3},
    {PAD_FUART_CTS, PADTOP_BANK, REG_FUART_MODE, REG_FUART_MODE_MASK, BIT8, PINMUX_FOR_FUART_MODE_1},
    {PAD_FUART_CTS, PADTOP_BANK, REG_UART5_MODE, REG_UART5_MODE_MASK, BIT6 | BIT5, PINMUX_FOR_UART5_MODE_6},
    {PAD_FUART_CTS, PADTOP_BANK, REG_PWM_OUT7_MODE, REG_PWM_OUT7_MODE_MASK, BIT13, PINMUX_FOR_PWM_OUT7_MODE_2},
};

const ST_PadMuxEntry m_stPadMuxEntry[] = {
    {(sizeof(i2c5_scl_tbl) / sizeof(ST_PadMuxInfo)), i2c5_scl_tbl},
    {(sizeof(i2c5_sda_tbl) / sizeof(ST_PadMuxInfo)), i2c5_sda_tbl},
    {(sizeof(sd0_gpio0_tbl) / sizeof(ST_PadMuxInfo)), sd0_gpio0_tbl},
    {(sizeof(sd0_vctrl_tbl) / sizeof(ST_PadMuxInfo)), sd0_vctrl_tbl},
    {(sizeof(sd0_cdz_tbl) / sizeof(ST_PadMuxInfo)), sd0_cdz_tbl},
    {(sizeof(sd0_d1_tbl) / sizeof(ST_PadMuxInfo)), sd0_d1_tbl},
    {(sizeof(sd0_d0_tbl) / sizeof(ST_PadMuxInfo)), sd0_d0_tbl},
    {(sizeof(sd0_clk_tbl) / sizeof(ST_PadMuxInfo)), sd0_clk_tbl},
    {(sizeof(sd0_cmd_tbl) / sizeof(ST_PadMuxInfo)), sd0_cmd_tbl},
    {(sizeof(sd0_d3_tbl) / sizeof(ST_PadMuxInfo)), sd0_d3_tbl},
    {(sizeof(sd0_d2_tbl) / sizeof(ST_PadMuxInfo)), sd0_d2_tbl},
    {(sizeof(key0_tbl) / sizeof(ST_PadMuxInfo)), key0_tbl},
    {(sizeof(key1_tbl) / sizeof(ST_PadMuxInfo)), key1_tbl},
    {(sizeof(key2_tbl) / sizeof(ST_PadMuxInfo)), key2_tbl},
    {(sizeof(key3_tbl) / sizeof(ST_PadMuxInfo)), key3_tbl},
    {(sizeof(key4_tbl) / sizeof(ST_PadMuxInfo)), key4_tbl},
    {(sizeof(key5_tbl) / sizeof(ST_PadMuxInfo)), key5_tbl},
    {(sizeof(key6_tbl) / sizeof(ST_PadMuxInfo)), key6_tbl},
    {(sizeof(key7_tbl) / sizeof(ST_PadMuxInfo)), key7_tbl},
    {(sizeof(key8_tbl) / sizeof(ST_PadMuxInfo)), key8_tbl},
    {(sizeof(key9_tbl) / sizeof(ST_PadMuxInfo)), key9_tbl},
    {(sizeof(key10_tbl) / sizeof(ST_PadMuxInfo)), key10_tbl},
    {(sizeof(key11_tbl) / sizeof(ST_PadMuxInfo)), key11_tbl},
    {(sizeof(key12_tbl) / sizeof(ST_PadMuxInfo)), key12_tbl},
    {(sizeof(key13_tbl) / sizeof(ST_PadMuxInfo)), key13_tbl},
    {(sizeof(pm_pwm0_tbl) / sizeof(ST_PadMuxInfo)), pm_pwm0_tbl},
    {(sizeof(pm_pwm1_tbl) / sizeof(ST_PadMuxInfo)), pm_pwm1_tbl},
    {(sizeof(pm_i2cm_scl_tbl) / sizeof(ST_PadMuxInfo)), pm_i2cm_scl_tbl},
    {(sizeof(pm_i2cm_sda_tbl) / sizeof(ST_PadMuxInfo)), pm_i2cm_sda_tbl},
    {(sizeof(pm_uart_rx0_tbl) / sizeof(ST_PadMuxInfo)), pm_uart_rx0_tbl},
    {(sizeof(pm_uart_tx0_tbl) / sizeof(ST_PadMuxInfo)), pm_uart_tx0_tbl},
    {(sizeof(pm_ir_rx_tbl) / sizeof(ST_PadMuxInfo)), pm_ir_rx_tbl},
    {(sizeof(pm_gpio0_tbl) / sizeof(ST_PadMuxInfo)), pm_gpio0_tbl},
    {(sizeof(pm_gpio1_tbl) / sizeof(ST_PadMuxInfo)), pm_gpio1_tbl},
    {(sizeof(pm_gpio2_tbl) / sizeof(ST_PadMuxInfo)), pm_gpio2_tbl},
    {(sizeof(pm_gpio3_tbl) / sizeof(ST_PadMuxInfo)), pm_gpio3_tbl},
    {(sizeof(pm_gpio4_tbl) / sizeof(ST_PadMuxInfo)), pm_gpio4_tbl},
    {(sizeof(pm_gpio5_tbl) / sizeof(ST_PadMuxInfo)), pm_gpio5_tbl},
    {(sizeof(pm_spi_wpz_tbl) / sizeof(ST_PadMuxInfo)), pm_spi_wpz_tbl},
    {(sizeof(pm_spi_do_tbl) / sizeof(ST_PadMuxInfo)), pm_spi_do_tbl},
    {(sizeof(pm_spi_cz_tbl) / sizeof(ST_PadMuxInfo)), pm_spi_cz_tbl},
    {(sizeof(pm_spi_hld_tbl) / sizeof(ST_PadMuxInfo)), pm_spi_hld_tbl},
    {(sizeof(pm_spi_ck_tbl) / sizeof(ST_PadMuxInfo)), pm_spi_ck_tbl},
    {(sizeof(pm_spi_di_tbl) / sizeof(ST_PadMuxInfo)), pm_spi_di_tbl},
    {(sizeof(emmc_rstn_tbl) / sizeof(ST_PadMuxInfo)), emmc_rstn_tbl},
    {(sizeof(emmc_clk_tbl) / sizeof(ST_PadMuxInfo)), emmc_clk_tbl},
    {(sizeof(emmc_cmd_tbl) / sizeof(ST_PadMuxInfo)), emmc_cmd_tbl},
    {(sizeof(emmc_ds_tbl) / sizeof(ST_PadMuxInfo)), emmc_ds_tbl},
    {(sizeof(emmc_d3_tbl) / sizeof(ST_PadMuxInfo)), emmc_d3_tbl},
    {(sizeof(emmc_d4_tbl) / sizeof(ST_PadMuxInfo)), emmc_d4_tbl},
    {(sizeof(emmc_d0_tbl) / sizeof(ST_PadMuxInfo)), emmc_d0_tbl},
    {(sizeof(emmc_d5_tbl) / sizeof(ST_PadMuxInfo)), emmc_d5_tbl},
    {(sizeof(emmc_d1_tbl) / sizeof(ST_PadMuxInfo)), emmc_d1_tbl},
    {(sizeof(emmc_d6_tbl) / sizeof(ST_PadMuxInfo)), emmc_d6_tbl},
    {(sizeof(emmc_d2_tbl) / sizeof(ST_PadMuxInfo)), emmc_d2_tbl},
    {(sizeof(emmc_d7_tbl) / sizeof(ST_PadMuxInfo)), emmc_d7_tbl},
    {(sizeof(outp_rx1_ch_0_tbl) / sizeof(ST_PadMuxInfo)), outp_rx1_ch_0_tbl},
    {(sizeof(outn_rx1_ch_0_tbl) / sizeof(ST_PadMuxInfo)), outn_rx1_ch_0_tbl},
    {(sizeof(outp_rx1_ch_1_tbl) / sizeof(ST_PadMuxInfo)), outp_rx1_ch_1_tbl},
    {(sizeof(outn_rx1_ch_1_tbl) / sizeof(ST_PadMuxInfo)), outn_rx1_ch_1_tbl},
    {(sizeof(outp_rx1_ch_2_tbl) / sizeof(ST_PadMuxInfo)), outp_rx1_ch_2_tbl},
    {(sizeof(outn_rx1_ch_2_tbl) / sizeof(ST_PadMuxInfo)), outn_rx1_ch_2_tbl},
    {(sizeof(outp_rx1_ch_3_tbl) / sizeof(ST_PadMuxInfo)), outp_rx1_ch_3_tbl},
    {(sizeof(outn_rx1_ch_3_tbl) / sizeof(ST_PadMuxInfo)), outn_rx1_ch_3_tbl},
    {(sizeof(outp_rx0_ch_0_tbl) / sizeof(ST_PadMuxInfo)), outp_rx0_ch_0_tbl},
    {(sizeof(outn_rx0_ch_0_tbl) / sizeof(ST_PadMuxInfo)), outn_rx0_ch_0_tbl},
    {(sizeof(outp_rx0_ch_1_tbl) / sizeof(ST_PadMuxInfo)), outp_rx0_ch_1_tbl},
    {(sizeof(outn_rx0_ch_1_tbl) / sizeof(ST_PadMuxInfo)), outn_rx0_ch_1_tbl},
    {(sizeof(outp_rx0_ch_2_tbl) / sizeof(ST_PadMuxInfo)), outp_rx0_ch_2_tbl},
    {(sizeof(outn_rx0_ch_2_tbl) / sizeof(ST_PadMuxInfo)), outn_rx0_ch_2_tbl},
    {(sizeof(outp_rx0_ch_3_tbl) / sizeof(ST_PadMuxInfo)), outp_rx0_ch_3_tbl},
    {(sizeof(outn_rx0_ch_3_tbl) / sizeof(ST_PadMuxInfo)), outn_rx0_ch_3_tbl},
    {(sizeof(outp_rx0_ch_4_tbl) / sizeof(ST_PadMuxInfo)), outp_rx0_ch_4_tbl},
    {(sizeof(outn_rx0_ch_4_tbl) / sizeof(ST_PadMuxInfo)), outn_rx0_ch_4_tbl},
    {(sizeof(outp_rx0_ch_5_tbl) / sizeof(ST_PadMuxInfo)), outp_rx0_ch_5_tbl},
    {(sizeof(outn_rx0_ch_5_tbl) / sizeof(ST_PadMuxInfo)), outn_rx0_ch_5_tbl},
    {(sizeof(spdif_tx_tbl) / sizeof(ST_PadMuxInfo)), spdif_tx_tbl},
    {(sizeof(sr_io3_tbl) / sizeof(ST_PadMuxInfo)), sr_io3_tbl},
    {(sizeof(sr_io2_tbl) / sizeof(ST_PadMuxInfo)), sr_io2_tbl},
    {(sizeof(sr_io1_tbl) / sizeof(ST_PadMuxInfo)), sr_io1_tbl},
    {(sizeof(sr_io0_tbl) / sizeof(ST_PadMuxInfo)), sr_io0_tbl},
    {(sizeof(sr_pdn1_tbl) / sizeof(ST_PadMuxInfo)), sr_pdn1_tbl},
    {(sizeof(sr_mclk1_tbl) / sizeof(ST_PadMuxInfo)), sr_mclk1_tbl},
    {(sizeof(sr_rst1_tbl) / sizeof(ST_PadMuxInfo)), sr_rst1_tbl},
    {(sizeof(i2c1_sda_tbl) / sizeof(ST_PadMuxInfo)), i2c1_sda_tbl},
    {(sizeof(i2c1_scl_tbl) / sizeof(ST_PadMuxInfo)), i2c1_scl_tbl},
    {(sizeof(sr_pdn0_tbl) / sizeof(ST_PadMuxInfo)), sr_pdn0_tbl},
    {(sizeof(sr_mclk0_tbl) / sizeof(ST_PadMuxInfo)), sr_mclk0_tbl},
    {(sizeof(sr_rst0_tbl) / sizeof(ST_PadMuxInfo)), sr_rst0_tbl},
    {(sizeof(i2c0_sda_tbl) / sizeof(ST_PadMuxInfo)), i2c0_sda_tbl},
    {(sizeof(i2c0_scl_tbl) / sizeof(ST_PadMuxInfo)), i2c0_scl_tbl},
    {(sizeof(sr_pdn2_tbl) / sizeof(ST_PadMuxInfo)), sr_pdn2_tbl},
    {(sizeof(sr_mclk2_tbl) / sizeof(ST_PadMuxInfo)), sr_mclk2_tbl},
    {(sizeof(sr_rst2_tbl) / sizeof(ST_PadMuxInfo)), sr_rst2_tbl},
    {(sizeof(i2c2_sda_tbl) / sizeof(ST_PadMuxInfo)), i2c2_sda_tbl},
    {(sizeof(i2c2_scl_tbl) / sizeof(ST_PadMuxInfo)), i2c2_scl_tbl},
    {(sizeof(sr_pdn3_tbl) / sizeof(ST_PadMuxInfo)), sr_pdn3_tbl},
    {(sizeof(sr_mclk3_tbl) / sizeof(ST_PadMuxInfo)), sr_mclk3_tbl},
    {(sizeof(sr_rst3_tbl) / sizeof(ST_PadMuxInfo)), sr_rst3_tbl},
    {(sizeof(i2c3_sda_tbl) / sizeof(ST_PadMuxInfo)), i2c3_sda_tbl},
    {(sizeof(i2c3_scl_tbl) / sizeof(ST_PadMuxInfo)), i2c3_scl_tbl},
    {(sizeof(pwm_out0_tbl) / sizeof(ST_PadMuxInfo)), pwm_out0_tbl},
    {(sizeof(pwm_out1_tbl) / sizeof(ST_PadMuxInfo)), pwm_out1_tbl},
    {(sizeof(pwm_out2_tbl) / sizeof(ST_PadMuxInfo)), pwm_out2_tbl},
    {(sizeof(pwm_out3_tbl) / sizeof(ST_PadMuxInfo)), pwm_out3_tbl},
    {(sizeof(pwm_out4_tbl) / sizeof(ST_PadMuxInfo)), pwm_out4_tbl},
    {(sizeof(pwm_out5_tbl) / sizeof(ST_PadMuxInfo)), pwm_out5_tbl},
    {(sizeof(pwm_out6_tbl) / sizeof(ST_PadMuxInfo)), pwm_out6_tbl},
    {(sizeof(pwm_out7_tbl) / sizeof(ST_PadMuxInfo)), pwm_out7_tbl},
    {(sizeof(pwm_out8_tbl) / sizeof(ST_PadMuxInfo)), pwm_out8_tbl},
    {(sizeof(pwm_out9_tbl) / sizeof(ST_PadMuxInfo)), pwm_out9_tbl},
    {(sizeof(pwm_out10_tbl) / sizeof(ST_PadMuxInfo)), pwm_out10_tbl},
    {(sizeof(pwm_out11_tbl) / sizeof(ST_PadMuxInfo)), pwm_out11_tbl},
    {(sizeof(outp_tx0_ch_0_tbl) / sizeof(ST_PadMuxInfo)), outp_tx0_ch_0_tbl},
    {(sizeof(outn_tx0_ch_0_tbl) / sizeof(ST_PadMuxInfo)), outn_tx0_ch_0_tbl},
    {(sizeof(outp_tx0_ch_1_tbl) / sizeof(ST_PadMuxInfo)), outp_tx0_ch_1_tbl},
    {(sizeof(outn_tx0_ch_1_tbl) / sizeof(ST_PadMuxInfo)), outn_tx0_ch_1_tbl},
    {(sizeof(outp_tx0_ch_2_tbl) / sizeof(ST_PadMuxInfo)), outp_tx0_ch_2_tbl},
    {(sizeof(outn_tx0_ch_2_tbl) / sizeof(ST_PadMuxInfo)), outn_tx0_ch_2_tbl},
    {(sizeof(outp_tx0_ch_3_tbl) / sizeof(ST_PadMuxInfo)), outp_tx0_ch_3_tbl},
    {(sizeof(outn_tx0_ch_3_tbl) / sizeof(ST_PadMuxInfo)), outn_tx0_ch_3_tbl},
    {(sizeof(outp_tx0_ch_4_tbl) / sizeof(ST_PadMuxInfo)), outp_tx0_ch_4_tbl},
    {(sizeof(outn_tx0_ch_4_tbl) / sizeof(ST_PadMuxInfo)), outn_tx0_ch_4_tbl},
    {(sizeof(outp_tx1_ch_0_tbl) / sizeof(ST_PadMuxInfo)), outp_tx1_ch_0_tbl},
    {(sizeof(outn_tx1_ch_0_tbl) / sizeof(ST_PadMuxInfo)), outn_tx1_ch_0_tbl},
    {(sizeof(outp_tx1_ch_1_tbl) / sizeof(ST_PadMuxInfo)), outp_tx1_ch_1_tbl},
    {(sizeof(outn_tx1_ch_1_tbl) / sizeof(ST_PadMuxInfo)), outn_tx1_ch_1_tbl},
    {(sizeof(outp_tx1_ch_2_tbl) / sizeof(ST_PadMuxInfo)), outp_tx1_ch_2_tbl},
    {(sizeof(outn_tx1_ch_2_tbl) / sizeof(ST_PadMuxInfo)), outn_tx1_ch_2_tbl},
    {(sizeof(outp_tx1_ch_3_tbl) / sizeof(ST_PadMuxInfo)), outp_tx1_ch_3_tbl},
    {(sizeof(outn_tx1_ch_3_tbl) / sizeof(ST_PadMuxInfo)), outn_tx1_ch_3_tbl},
    {(sizeof(outp_tx1_ch_4_tbl) / sizeof(ST_PadMuxInfo)), outp_tx1_ch_4_tbl},
    {(sizeof(outn_tx1_ch_4_tbl) / sizeof(ST_PadMuxInfo)), outn_tx1_ch_4_tbl},
    {(sizeof(sar_adc_0_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_0_tbl},
    {(sizeof(sar_adc_1_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_1_tbl},
    {(sizeof(sar_adc_2_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_2_tbl},
    {(sizeof(sar_adc_3_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_3_tbl},
    {(sizeof(sar_adc_4_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_4_tbl},
    {(sizeof(sar_adc_5_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_5_tbl},
    {(sizeof(sar_adc_6_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_6_tbl},
    {(sizeof(sar_adc_7_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_7_tbl},
    {(sizeof(sar_adc_8_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_8_tbl},
    {(sizeof(sar_adc_9_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_9_tbl},
    {(sizeof(sar_adc_10_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_10_tbl},
    {(sizeof(sar_adc_11_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_11_tbl},
    {(sizeof(sar_adc_12_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_12_tbl},
    {(sizeof(sar_adc_13_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_13_tbl},
    {(sizeof(sar_adc_14_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_14_tbl},
    {(sizeof(sar_adc_15_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_15_tbl},
    {(sizeof(sar_adc_16_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_16_tbl},
    {(sizeof(sar_adc_17_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_17_tbl},
    {(sizeof(sar_adc_18_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_18_tbl},
    {(sizeof(sar_adc_19_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_19_tbl},
    {(sizeof(sar_adc_20_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_20_tbl},
    {(sizeof(sar_adc_21_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_21_tbl},
    {(sizeof(sar_adc_22_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_22_tbl},
    {(sizeof(sar_adc_23_tbl) / sizeof(ST_PadMuxInfo)), sar_adc_23_tbl},
    {(sizeof(sr_io4_tbl) / sizeof(ST_PadMuxInfo)), sr_io4_tbl},
    {(sizeof(rgmii0_mclk_tbl) / sizeof(ST_PadMuxInfo)), rgmii0_mclk_tbl},
    {(sizeof(rgmii0_rstn_tbl) / sizeof(ST_PadMuxInfo)), rgmii0_rstn_tbl},
    {(sizeof(rgmii0_rxclk_tbl) / sizeof(ST_PadMuxInfo)), rgmii0_rxclk_tbl},
    {(sizeof(rgmii0_rxctl_tbl) / sizeof(ST_PadMuxInfo)), rgmii0_rxctl_tbl},
    {(sizeof(rgmii0_rxd0_tbl) / sizeof(ST_PadMuxInfo)), rgmii0_rxd0_tbl},
    {(sizeof(rgmii0_rxd1_tbl) / sizeof(ST_PadMuxInfo)), rgmii0_rxd1_tbl},
    {(sizeof(rgmii0_rxd2_tbl) / sizeof(ST_PadMuxInfo)), rgmii0_rxd2_tbl},
    {(sizeof(rgmii0_rxd3_tbl) / sizeof(ST_PadMuxInfo)), rgmii0_rxd3_tbl},
    {(sizeof(rgmii0_txclk_tbl) / sizeof(ST_PadMuxInfo)), rgmii0_txclk_tbl},
    {(sizeof(rgmii0_txctl_tbl) / sizeof(ST_PadMuxInfo)), rgmii0_txctl_tbl},
    {(sizeof(rgmii0_txd0_tbl) / sizeof(ST_PadMuxInfo)), rgmii0_txd0_tbl},
    {(sizeof(rgmii0_txd1_tbl) / sizeof(ST_PadMuxInfo)), rgmii0_txd1_tbl},
    {(sizeof(rgmii0_txd2_tbl) / sizeof(ST_PadMuxInfo)), rgmii0_txd2_tbl},
    {(sizeof(rgmii0_txd3_tbl) / sizeof(ST_PadMuxInfo)), rgmii0_txd3_tbl},
    {(sizeof(rgmii0_mdio_tbl) / sizeof(ST_PadMuxInfo)), rgmii0_mdio_tbl},
    {(sizeof(rgmii0_mdc_tbl) / sizeof(ST_PadMuxInfo)), rgmii0_mdc_tbl},
    {(sizeof(uart_rx2_tbl) / sizeof(ST_PadMuxInfo)), uart_rx2_tbl},
    {(sizeof(uart_tx2_tbl) / sizeof(ST_PadMuxInfo)), uart_tx2_tbl},
    {(sizeof(uart_rx3_tbl) / sizeof(ST_PadMuxInfo)), uart_rx3_tbl},
    {(sizeof(uart_tx3_tbl) / sizeof(ST_PadMuxInfo)), uart_tx3_tbl},
    {(sizeof(uart_rx4_tbl) / sizeof(ST_PadMuxInfo)), uart_rx4_tbl},
    {(sizeof(uart_tx4_tbl) / sizeof(ST_PadMuxInfo)), uart_tx4_tbl},
    {(sizeof(uart_rx1_tbl) / sizeof(ST_PadMuxInfo)), uart_rx1_tbl},
    {(sizeof(uart_tx1_tbl) / sizeof(ST_PadMuxInfo)), uart_tx1_tbl},
    {(sizeof(fuart_rx_tbl) / sizeof(ST_PadMuxInfo)), fuart_rx_tbl},
    {(sizeof(fuart_tx_tbl) / sizeof(ST_PadMuxInfo)), fuart_tx_tbl},
    {(sizeof(fuart_rts_tbl) / sizeof(ST_PadMuxInfo)), fuart_rts_tbl},
    {(sizeof(fuart_cts_tbl) / sizeof(ST_PadMuxInfo)), fuart_cts_tbl},
};

static const ST_PadModeInfo m_stPadModeInfoTbl[] = {
    {"GPIO_MODE", 0, 0, 0},
    {"EJ_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_EJ_MODE), REG_EJ_MODE_MASK, BIT7},
    {"EJ_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_EJ_MODE), REG_EJ_MODE_MASK, BIT8},
    {"EJ_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_EJ_MODE), REG_EJ_MODE_MASK, BIT8 | BIT7},
    {"RISCV_EJ_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_RISCV_EJ_MODE), REG_RISCV_EJ_MODE_MASK, BIT0},
    {"RISCV_EJ_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_RISCV_EJ_MODE), REG_RISCV_EJ_MODE_MASK, BIT1},
    {"RISCV_EJ_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_RISCV_EJ_MODE), REG_RISCV_EJ_MODE_MASK, BIT1 | BIT0},
    {"TEST_IN_MODE_1", _RIUA_16BIT(CHIPTOP_BANK, REG_TEST_IN_MODE), REG_TEST_IN_MODE_MASK, BIT0},
    {"TEST_IN_MODE_2", _RIUA_16BIT(CHIPTOP_BANK, REG_TEST_IN_MODE), REG_TEST_IN_MODE_MASK, BIT1},
    {"TEST_IN_MODE_3", _RIUA_16BIT(CHIPTOP_BANK, REG_TEST_IN_MODE), REG_TEST_IN_MODE_MASK, BIT1 | BIT0},
    {"TEST_OUT_MODE_1", _RIUA_16BIT(CHIPTOP_BANK, REG_TEST_OUT_MODE), REG_TEST_OUT_MODE_MASK, BIT4},
    {"TEST_OUT_MODE_2", _RIUA_16BIT(CHIPTOP_BANK, REG_TEST_OUT_MODE), REG_TEST_OUT_MODE_MASK, BIT5},
    {"TEST_OUT_MODE_3", _RIUA_16BIT(CHIPTOP_BANK, REG_TEST_OUT_MODE), REG_TEST_OUT_MODE_MASK, BIT5 | BIT4},
    {"ADC0_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC0_MODE), REG_ADC0_MODE_MASK, BIT0},
    {"ADC1_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC1_MODE), REG_ADC1_MODE_MASK, BIT1},
    {"ADC2_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC2_MODE), REG_ADC2_MODE_MASK, BIT2},
    {"ADC3_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC3_MODE), REG_ADC3_MODE_MASK, BIT3},
    {"ADC4_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC4_MODE), REG_ADC4_MODE_MASK, BIT4},
    {"ADC5_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC5_MODE), REG_ADC5_MODE_MASK, BIT5},
    {"ADC6_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC6_MODE), REG_ADC6_MODE_MASK, BIT6},
    {"ADC7_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC7_MODE), REG_ADC7_MODE_MASK, BIT7},
    {"ADC8_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC8_MODE), REG_ADC8_MODE_MASK, BIT8},
    {"ADC9_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC9_MODE), REG_ADC9_MODE_MASK, BIT9},
    {"ADC10_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC10_MODE), REG_ADC10_MODE_MASK, BIT10},
    {"ADC11_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC11_MODE), REG_ADC11_MODE_MASK, BIT11},
    {"ADC12_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC12_MODE), REG_ADC12_MODE_MASK, BIT12},
    {"ADC13_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC13_MODE), REG_ADC13_MODE_MASK, BIT13},
    {"ADC14_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC14_MODE), REG_ADC14_MODE_MASK, BIT14},
    {"ADC15_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC15_MODE), REG_ADC15_MODE_MASK, BIT15},
    {"ADC16_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC16_MODE), REG_ADC16_MODE_MASK, BIT0},
    {"ADC17_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC17_MODE), REG_ADC17_MODE_MASK, BIT1},
    {"ADC18_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC18_MODE), REG_ADC18_MODE_MASK, BIT2},
    {"ADC19_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC19_MODE), REG_ADC19_MODE_MASK, BIT3},
    {"ADC20_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC20_MODE), REG_ADC20_MODE_MASK, BIT4},
    {"ADC21_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC21_MODE), REG_ADC21_MODE_MASK, BIT5},
    {"ADC22_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC22_MODE), REG_ADC22_MODE_MASK, BIT6},
    {"ADC23_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC23_MODE), REG_ADC23_MODE_MASK, BIT7},
    {"ADC_INT_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_ADC_INT_MODE), REG_ADC_INT_MODE_MASK, BIT8},
    {"ADC_INT_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_ADC_INT_MODE), REG_ADC_INT_MODE_MASK, BIT9},
    {"I2C0_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_I2C0_MODE), REG_I2C0_MODE_MASK, BIT0},
    {"I2C0_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_I2C0_MODE), REG_I2C0_MODE_MASK, BIT1},
    {"I2C0_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_I2C0_MODE), REG_I2C0_MODE_MASK, BIT1 | BIT0},
    {"I2C0_MODE_4", _RIUA_16BIT(PADTOP_BANK, REG_I2C0_MODE), REG_I2C0_MODE_MASK, BIT2},
    {"I2C1_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_I2C1_MODE), REG_I2C1_MODE_MASK, BIT0},
    {"I2C1_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_I2C1_MODE), REG_I2C1_MODE_MASK, BIT1},
    {"I2C1_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_I2C1_MODE), REG_I2C1_MODE_MASK, BIT1 | BIT0},
    {"I2C1_MODE_4", _RIUA_16BIT(PADTOP_BANK, REG_I2C1_MODE), REG_I2C1_MODE_MASK, BIT2},
    {"I2C1_MODE_5", _RIUA_16BIT(PADTOP_BANK, REG_I2C1_MODE), REG_I2C1_MODE_MASK, BIT2 | BIT0},
    {"I2C2_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_I2C2_MODE), REG_I2C2_MODE_MASK, BIT8},
    {"I2C2_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_I2C2_MODE), REG_I2C2_MODE_MASK, BIT9},
    {"I2C2_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_I2C2_MODE), REG_I2C2_MODE_MASK, BIT9 | BIT8},
    {"I2C3_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_I2C3_MODE), REG_I2C3_MODE_MASK, BIT0},
    {"I2C3_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_I2C3_MODE), REG_I2C3_MODE_MASK, BIT1},
    {"I2C3_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_I2C3_MODE), REG_I2C3_MODE_MASK, BIT1 | BIT0},
    {"I2C4_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_I2C4_MODE), REG_I2C4_MODE_MASK, BIT4},
    {"I2C4_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_I2C4_MODE), REG_I2C4_MODE_MASK, BIT5},
    {"I2C4_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_I2C4_MODE), REG_I2C4_MODE_MASK, BIT5 | BIT4},
    {"I2C4_MODE_4", _RIUA_16BIT(PADTOP_BANK, REG_I2C4_MODE), REG_I2C4_MODE_MASK, BIT6},
    {"I2C5_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_I2C5_MODE), REG_I2C5_MODE_MASK, BIT8},
    {"I2C5_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_I2C5_MODE), REG_I2C5_MODE_MASK, BIT9},
    {"I2C5_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_I2C5_MODE), REG_I2C5_MODE_MASK, BIT9 | BIT8},
    {"I2C5_MODE_4", _RIUA_16BIT(PADTOP_BANK, REG_I2C5_MODE), REG_I2C5_MODE_MASK, BIT10},
    {"SPI0_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SPI0_MODE), REG_SPI0_MODE_MASK, BIT0},
    {"SPI0_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_SPI0_MODE), REG_SPI0_MODE_MASK, BIT1},
    {"SPI0_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_SPI0_MODE), REG_SPI0_MODE_MASK, BIT1 | BIT0},
    {"SPI0_CZ1_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SPI0_CZ1_MODE), REG_SPI0_CZ1_MODE_MASK, BIT4},
    {"SPI0_CZ1_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_SPI0_CZ1_MODE), REG_SPI0_CZ1_MODE_MASK, BIT5},
    {"SPI0_CZ1_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_SPI0_CZ1_MODE), REG_SPI0_CZ1_MODE_MASK, BIT5 | BIT4},
    {"SPI1_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SPI1_MODE), REG_SPI1_MODE_MASK, BIT3},
    {"SPI1_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_SPI1_MODE), REG_SPI1_MODE_MASK, BIT4},
    {"SPI1_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_SPI1_MODE), REG_SPI1_MODE_MASK, BIT4 | BIT3},
    {"SPI1_MODE_4", _RIUA_16BIT(PADTOP_BANK, REG_SPI1_MODE), REG_SPI1_MODE_MASK, BIT5},
    {"SPI1_CZ1_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SPI1_CZ1_MODE), REG_SPI1_CZ1_MODE_MASK, BIT6},
    {"SPI1_CZ1_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_SPI1_CZ1_MODE), REG_SPI1_CZ1_MODE_MASK, BIT7},
    {"FUART_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_FUART_MODE), REG_FUART_MODE_MASK, BIT8},
    {"FUART_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_FUART_MODE), REG_FUART_MODE_MASK, BIT9},
    {"FUART_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_FUART_MODE), REG_FUART_MODE_MASK, BIT9 | BIT8},
    {"FUART_MODE_4", _RIUA_16BIT(PADTOP_BANK, REG_FUART_MODE), REG_FUART_MODE_MASK, BIT10},
    {"FUART_2W_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_FUART_2W_MODE), REG_FUART_2W_MODE_MASK, BIT12},
    {"FUART_2W_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_FUART_2W_MODE), REG_FUART_2W_MODE_MASK, BIT13},
    {"FUART_2W_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_FUART_2W_MODE), REG_FUART_2W_MODE_MASK, BIT13 | BIT12},
    {"FUART_2W_MODE_4", _RIUA_16BIT(PADTOP_BANK, REG_FUART_2W_MODE), REG_FUART_2W_MODE_MASK, BIT14},
    {"UART0_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_UART0_MODE), REG_UART0_MODE_MASK, BIT0},
    {"UART0_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_UART0_MODE), REG_UART0_MODE_MASK, BIT1},
    {"UART1_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_UART1_MODE), REG_UART1_MODE_MASK, BIT4},
    {"UART1_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_UART1_MODE), REG_UART1_MODE_MASK, BIT5},
    {"UART1_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_UART1_MODE), REG_UART1_MODE_MASK, BIT5 | BIT4},
    {"UART2_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_UART2_MODE), REG_UART2_MODE_MASK, BIT8},
    {"UART2_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_UART2_MODE), REG_UART2_MODE_MASK, BIT9},
    {"UART2_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_UART2_MODE), REG_UART2_MODE_MASK, BIT9 | BIT8},
    {"UART2_MODE_4", _RIUA_16BIT(PADTOP_BANK, REG_UART2_MODE), REG_UART2_MODE_MASK, BIT10},
    {"UART3_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_UART3_MODE), REG_UART3_MODE_MASK, BIT12},
    {"UART3_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_UART3_MODE), REG_UART3_MODE_MASK, BIT13},
    {"UART3_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_UART3_MODE), REG_UART3_MODE_MASK, BIT13 | BIT12},
    {"UART4_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_UART4_MODE), REG_UART4_MODE_MASK, BIT0},
    {"UART4_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_UART4_MODE), REG_UART4_MODE_MASK, BIT1},
    {"UART4_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_UART4_MODE), REG_UART4_MODE_MASK, BIT1 | BIT0},
    {"UART5_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_UART5_MODE), REG_UART5_MODE_MASK, BIT4},
    {"UART5_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_UART5_MODE), REG_UART5_MODE_MASK, BIT5},
    {"UART5_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_UART5_MODE), REG_UART5_MODE_MASK, BIT5 | BIT4},
    {"UART5_MODE_4", _RIUA_16BIT(PADTOP_BANK, REG_UART5_MODE), REG_UART5_MODE_MASK, BIT6},
    {"UART5_MODE_5", _RIUA_16BIT(PADTOP_BANK, REG_UART5_MODE), REG_UART5_MODE_MASK, BIT6 | BIT4},
    {"UART5_MODE_6", _RIUA_16BIT(PADTOP_BANK, REG_UART5_MODE), REG_UART5_MODE_MASK, BIT6 | BIT5},
    {"SD0_BOOT_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SD0_BOOT_MODE), REG_SD0_BOOT_MODE_MASK, BIT0},
    {"EMMC8B_BOOT_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_EMMC8B_BOOT_MODE), REG_EMMC8B_BOOT_MODE_MASK, BIT0},
    {"EMMC4B_BOOT_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_EMMC4B_BOOT_MODE), REG_EMMC4B_BOOT_MODE_MASK, BIT8},
    {"SD0_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SD0_MODE), REG_SD0_MODE_MASK, BIT8},
    {"SD0_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_SD0_MODE), REG_SD0_MODE_MASK, BIT9},
    {"SD0_CDZ_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SD0_CDZ_MODE), REG_SD0_CDZ_MODE_MASK, BIT10},
    {"SD0_CDZ_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_SD0_CDZ_MODE), REG_SD0_CDZ_MODE_MASK, BIT11},
    {"SD0_RSTN_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SD0_RSTN_MODE), REG_SD0_RSTN_MODE_MASK, BIT0},
    {"SD0_RSTN_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_SD0_RSTN_MODE), REG_SD0_RSTN_MODE_MASK, BIT1},
    {"SDIO_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SDIO_MODE), REG_SDIO_MODE_MASK, BIT12},
    {"SDIO_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_SDIO_MODE), REG_SDIO_MODE_MASK, BIT13},
    {"SDIO_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_SDIO_MODE), REG_SDIO_MODE_MASK, BIT13 | BIT12},
    {"SDIO_CDZ_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SDIO_CDZ_MODE), REG_SDIO_CDZ_MODE_MASK, BIT12},
    {"SDIO_CDZ_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_SDIO_CDZ_MODE), REG_SDIO_CDZ_MODE_MASK, BIT13},
    {"SDIO_CDZ_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_SDIO_CDZ_MODE), REG_SDIO_CDZ_MODE_MASK, BIT13 | BIT12},
    {"SDIO_RSTN_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SDIO_RSTN_MODE), REG_SDIO_RSTN_MODE_MASK, BIT14},
    {"SDIO_RSTN_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_SDIO_RSTN_MODE), REG_SDIO_RSTN_MODE_MASK, BIT15},
    {"SDIO_RSTN_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_SDIO_RSTN_MODE), REG_SDIO_RSTN_MODE_MASK, BIT15 | BIT14},
    {"EMMC_8B_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_EMMC_8B_MODE), REG_EMMC_8B_MODE_MASK, BIT2},
    {"EMMC_4B_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_EMMC_4B_MODE), REG_EMMC_4B_MODE_MASK, BIT0},
    {"EMMC_RST_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_EMMC_RST_MODE), REG_EMMC_RST_MODE_MASK, BIT4},
    {"EMMC_AS_SD_CDZ_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_EMMC_AS_SD_CDZ_MODE), REG_EMMC_AS_SD_CDZ_MODE_MASK, BIT8},
    {"LED0_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_LED0_MODE), REG_LED0_MODE_MASK, BIT0},
    {"LED0_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_LED0_MODE), REG_LED0_MODE_MASK, BIT1},
    {"LED0_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_LED0_MODE), REG_LED0_MODE_MASK, BIT1 | BIT0},
    {"LED1_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_LED1_MODE), REG_LED1_MODE_MASK, BIT4},
    {"LED1_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_LED1_MODE), REG_LED1_MODE_MASK, BIT5},
    {"LED1_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_LED1_MODE), REG_LED1_MODE_MASK, BIT5 | BIT4},
    {"PWM0_PN_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM0_PN), REG_PWM0_PN_MASK, BIT0},
    {"PWM1_PN_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM1_PN), REG_PWM1_PN_MASK, BIT2},
    {"PWM2_PN_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM2_PN), REG_PWM2_PN_MASK, BIT4},
    {"PWM3_PN_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM3_PN), REG_PWM3_PN_MASK, BIT6},
    {"PWM4_PN_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM4_PN), REG_PWM4_PN_MASK, BIT8},
    {"PWM5_PN_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM5_PN), REG_PWM5_PN_MASK, BIT10},
    {"PWM_OUT0_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT0_MODE), REG_PWM_OUT0_MODE_MASK, BIT0},
    {"PWM_OUT0_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT0_MODE), REG_PWM_OUT0_MODE_MASK, BIT1},
    {"PWM_OUT0_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT0_MODE), REG_PWM_OUT0_MODE_MASK, BIT1 | BIT0},
    {"PWM_OUT0_MODE_4", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT0_MODE), REG_PWM_OUT0_MODE_MASK, BIT2},
    {"PWM_OUT1_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT1_MODE), REG_PWM_OUT1_MODE_MASK, BIT4},
    {"PWM_OUT1_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT1_MODE), REG_PWM_OUT1_MODE_MASK, BIT5},
    {"PWM_OUT1_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT1_MODE), REG_PWM_OUT1_MODE_MASK, BIT5 | BIT4},
    {"PWM_OUT1_MODE_4", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT1_MODE), REG_PWM_OUT1_MODE_MASK, BIT6},
    {"PWM_OUT2_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT2_MODE), REG_PWM_OUT2_MODE_MASK, BIT8},
    {"PWM_OUT2_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT2_MODE), REG_PWM_OUT2_MODE_MASK, BIT9},
    {"PWM_OUT2_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT2_MODE), REG_PWM_OUT2_MODE_MASK, BIT9 | BIT8},
    {"PWM_OUT2_MODE_4", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT2_MODE), REG_PWM_OUT2_MODE_MASK, BIT10},
    {"PWM_OUT3_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT3_MODE), REG_PWM_OUT3_MODE_MASK, BIT12},
    {"PWM_OUT3_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT3_MODE), REG_PWM_OUT3_MODE_MASK, BIT13},
    {"PWM_OUT3_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT3_MODE), REG_PWM_OUT3_MODE_MASK, BIT13 | BIT12},
    {"PWM_OUT3_MODE_4", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT3_MODE), REG_PWM_OUT3_MODE_MASK, BIT14},
    {"PWM_OUT4_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT4_MODE), REG_PWM_OUT4_MODE_MASK, BIT0},
    {"PWM_OUT4_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT4_MODE), REG_PWM_OUT4_MODE_MASK, BIT1},
    {"PWM_OUT4_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT4_MODE), REG_PWM_OUT4_MODE_MASK, BIT1 | BIT0},
    {"PWM_OUT4_MODE_4", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT4_MODE), REG_PWM_OUT4_MODE_MASK, BIT2},
    {"PWM_OUT5_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT5_MODE), REG_PWM_OUT5_MODE_MASK, BIT8},
    {"PWM_OUT5_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT5_MODE), REG_PWM_OUT5_MODE_MASK, BIT9},
    {"PWM_OUT5_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT5_MODE), REG_PWM_OUT5_MODE_MASK, BIT9 | BIT8},
    {"PWM_OUT6_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT6_MODE), REG_PWM_OUT6_MODE_MASK, BIT10},
    {"PWM_OUT6_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT6_MODE), REG_PWM_OUT6_MODE_MASK, BIT11},
    {"PWM_OUT6_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT6_MODE), REG_PWM_OUT6_MODE_MASK, BIT11 | BIT10},
    {"PWM_OUT7_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT7_MODE), REG_PWM_OUT7_MODE_MASK, BIT12},
    {"PWM_OUT7_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT7_MODE), REG_PWM_OUT7_MODE_MASK, BIT13},
    {"PWM_OUT7_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT7_MODE), REG_PWM_OUT7_MODE_MASK, BIT13 | BIT12},
    {"PWM_OUT8_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT8_MODE), REG_PWM_OUT8_MODE_MASK, BIT0},
    {"PWM_OUT8_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT8_MODE), REG_PWM_OUT8_MODE_MASK, BIT1},
    {"PWM_OUT8_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT8_MODE), REG_PWM_OUT8_MODE_MASK, BIT1 | BIT0},
    {"PWM_OUT9_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT9_MODE), REG_PWM_OUT9_MODE_MASK, BIT2},
    {"PWM_OUT9_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT9_MODE), REG_PWM_OUT9_MODE_MASK, BIT3},
    {"PWM_OUT9_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT9_MODE), REG_PWM_OUT9_MODE_MASK, BIT3 | BIT2},
    {"PWM_OUT10_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT10_MODE), REG_PWM_OUT10_MODE_MASK, BIT4},
    {"PWM_OUT10_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT10_MODE), REG_PWM_OUT10_MODE_MASK, BIT5},
    {"PWM_OUT10_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT10_MODE), REG_PWM_OUT10_MODE_MASK, BIT5 | BIT4},
    {"PWM_OUT11_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT11_MODE), REG_PWM_OUT11_MODE_MASK, BIT6},
    {"PWM_OUT11_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT11_MODE), REG_PWM_OUT11_MODE_MASK, BIT7},
    {"PWM_OUT11_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT11_MODE), REG_PWM_OUT11_MODE_MASK, BIT7 | BIT6},
    {"PWM_OUT12_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT12_MODE), REG_PWM_OUT12_MODE_MASK, BIT0},
    {"PWM_OUT12_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT12_MODE), REG_PWM_OUT12_MODE_MASK, BIT1},
    {"PWM_OUT12_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT12_MODE), REG_PWM_OUT12_MODE_MASK, BIT1 | BIT0},
    {"PWM_OUT12_MODE_4", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT12_MODE), REG_PWM_OUT12_MODE_MASK, BIT2},
    {"PWM_OUT13_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT13_MODE), REG_PWM_OUT13_MODE_MASK, BIT10},
    {"PWM_OUT13_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT13_MODE), REG_PWM_OUT13_MODE_MASK, BIT11},
    {"PWM_OUT13_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT13_MODE), REG_PWM_OUT13_MODE_MASK, BIT11 | BIT10},
    {"PWM_OUT14_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT14_MODE), REG_PWM_OUT14_MODE_MASK, BIT12},
    {"PWM_OUT14_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT14_MODE), REG_PWM_OUT14_MODE_MASK, BIT13},
    {"PWM_OUT14_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT14_MODE), REG_PWM_OUT14_MODE_MASK, BIT13 | BIT12},
    {"PWM_OUT15_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT15_MODE), REG_PWM_OUT15_MODE_MASK, BIT14},
    {"PWM_OUT15_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT15_MODE), REG_PWM_OUT15_MODE_MASK, BIT15},
    {"PWM_OUT15_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT15_MODE), REG_PWM_OUT15_MODE_MASK, BIT15 | BIT14},
    {"PWM_OUT16_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT16_MODE), REG_PWM_OUT16_MODE_MASK, BIT8},
    {"PWM_OUT16_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT16_MODE), REG_PWM_OUT16_MODE_MASK, BIT9},
    {"PWM_OUT16_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT16_MODE), REG_PWM_OUT16_MODE_MASK, BIT9 | BIT8},
    {"PWM_OUT17_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT17_MODE), REG_PWM_OUT17_MODE_MASK, BIT4},
    {"PWM_OUT17_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT17_MODE), REG_PWM_OUT17_MODE_MASK, BIT5},
    {"PWM_OUT17_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT17_MODE), REG_PWM_OUT17_MODE_MASK, BIT5 | BIT4},
    {"PWM_OUT17_MODE_4", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT17_MODE), REG_PWM_OUT17_MODE_MASK, BIT6},
    {"PWM_OUT18_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT18_MODE), REG_PWM_OUT18_MODE_MASK, BIT12},
    {"PWM_OUT18_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT18_MODE), REG_PWM_OUT18_MODE_MASK, BIT13},
    {"PWM_OUT18_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT18_MODE), REG_PWM_OUT18_MODE_MASK, BIT13 | BIT12},
    {"PWM_OUT19_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT19_MODE), REG_PWM_OUT19_MODE_MASK, BIT14},
    {"PWM_OUT19_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT19_MODE), REG_PWM_OUT19_MODE_MASK, BIT15},
    {"PWM_OUT19_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_OUT19_MODE), REG_PWM_OUT19_MODE_MASK, BIT15 | BIT14},
    {"PWM_IN0_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN0_MODE), REG_PWM_IN0_MODE_MASK, BIT0},
    {"PWM_IN0_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN0_MODE), REG_PWM_IN0_MODE_MASK, BIT1},
    {"PWM_IN0_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN0_MODE), REG_PWM_IN0_MODE_MASK, BIT1 | BIT0},
    {"PWM_IN1_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN1_MODE), REG_PWM_IN1_MODE_MASK, BIT2},
    {"PWM_IN1_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN1_MODE), REG_PWM_IN1_MODE_MASK, BIT3},
    {"PWM_IN1_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN1_MODE), REG_PWM_IN1_MODE_MASK, BIT3 | BIT2},
    {"PWM_IN2_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN2_MODE), REG_PWM_IN2_MODE_MASK, BIT4},
    {"PWM_IN2_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN2_MODE), REG_PWM_IN2_MODE_MASK, BIT5},
    {"PWM_IN2_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN2_MODE), REG_PWM_IN2_MODE_MASK, BIT5 | BIT4},
    {"PWM_IN3_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN3_MODE), REG_PWM_IN3_MODE_MASK, BIT6},
    {"PWM_IN3_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN3_MODE), REG_PWM_IN3_MODE_MASK, BIT7},
    {"PWM_IN3_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN3_MODE), REG_PWM_IN3_MODE_MASK, BIT7 | BIT6},
    {"PWM_IN4_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN4_MODE), REG_PWM_IN4_MODE_MASK, BIT8},
    {"PWM_IN4_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN4_MODE), REG_PWM_IN4_MODE_MASK, BIT9},
    {"PWM_IN4_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN4_MODE), REG_PWM_IN4_MODE_MASK, BIT9 | BIT8},
    {"PWM_IN5_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN5_MODE), REG_PWM_IN5_MODE_MASK, BIT10},
    {"PWM_IN5_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN5_MODE), REG_PWM_IN5_MODE_MASK, BIT11},
    {"PWM_IN5_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN5_MODE), REG_PWM_IN5_MODE_MASK, BIT11 | BIT10},
    {"PWM_IN6_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN6_MODE), REG_PWM_IN6_MODE_MASK, BIT12},
    {"PWM_IN6_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN6_MODE), REG_PWM_IN6_MODE_MASK, BIT13},
    {"PWM_IN6_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN6_MODE), REG_PWM_IN6_MODE_MASK, BIT13 | BIT12},
    {"PWM_IN7_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN7_MODE), REG_PWM_IN7_MODE_MASK, BIT14},
    {"PWM_IN7_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN7_MODE), REG_PWM_IN7_MODE_MASK, BIT15},
    {"PWM_IN7_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PWM_IN7_MODE), REG_PWM_IN7_MODE_MASK, BIT15 | BIT14},
    {"PWM_INT_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PWM_INT_MODE), REG_PWM_INT_MODE_MASK, BIT0},
    {"PWM_INT_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PWM_INT_MODE), REG_PWM_INT_MODE_MASK, BIT1},
    {"I2S0_MCK_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_I2S0_MCK_MODE), REG_I2S0_MCK_MODE_MASK, BIT0},
    {"I2S0_MCK_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_I2S0_MCK_MODE), REG_I2S0_MCK_MODE_MASK, BIT1},
    {"I2S0_MCK_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_I2S0_MCK_MODE), REG_I2S0_MCK_MODE_MASK, BIT1 | BIT0},
    {"I2S0_RX_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_I2S0_RX_MODE), REG_I2S0_RX_MODE_MASK, BIT8},
    {"I2S0_RX_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_I2S0_RX_MODE), REG_I2S0_RX_MODE_MASK, BIT9},
    {"I2S0_RX_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_I2S0_RX_MODE), REG_I2S0_RX_MODE_MASK, BIT9 | BIT8},
    {"I2S0_TX_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_I2S0_TX_MODE), REG_I2S0_TX_MODE_MASK, BIT12},
    {"I2S0_TX_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_I2S0_TX_MODE), REG_I2S0_TX_MODE_MASK, BIT13},
    {"I2S0_TX_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_I2S0_TX_MODE), REG_I2S0_TX_MODE_MASK, BIT13 | BIT12},
    {"I2S0_RXTX_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_I2S0_RXTX_MODE), REG_I2S0_RXTX_MODE_MASK, BIT0},
    {"I2S0_RXTX_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_I2S0_RXTX_MODE), REG_I2S0_RXTX_MODE_MASK, BIT1},
    {"I2S0_RXTX_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_I2S0_RXTX_MODE), REG_I2S0_RXTX_MODE_MASK, BIT1 | BIT0},
    {"I2S1_MCK_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_I2S1_MCK_MODE), REG_I2S1_MCK_MODE_MASK, BIT4},
    {"I2S1_MCK_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_I2S1_MCK_MODE), REG_I2S1_MCK_MODE_MASK, BIT5},
    {"I2S1_MCK_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_I2S1_MCK_MODE), REG_I2S1_MCK_MODE_MASK, BIT5 | BIT4},
    {"I2S1_RX_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_I2S1_RX_MODE), REG_I2S1_RX_MODE_MASK, BIT0},
    {"I2S1_RX_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_I2S1_RX_MODE), REG_I2S1_RX_MODE_MASK, BIT1},
    {"I2S1_RX_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_I2S1_RX_MODE), REG_I2S1_RX_MODE_MASK, BIT1 | BIT0},
    {"I2S1_TX_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_I2S1_TX_MODE), REG_I2S1_TX_MODE_MASK, BIT4},
    {"I2S1_TX_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_I2S1_TX_MODE), REG_I2S1_TX_MODE_MASK, BIT5},
    {"I2S1_TX_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_I2S1_TX_MODE), REG_I2S1_TX_MODE_MASK, BIT5 | BIT4},
    {"I2S1_RXTX_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_I2S1_RXTX_MODE), REG_I2S1_RXTX_MODE_MASK, BIT4},
    {"I2S1_RXTX_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_I2S1_RXTX_MODE), REG_I2S1_RXTX_MODE_MASK, BIT5},
    {"I2S1_RXTX_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_I2S1_RXTX_MODE), REG_I2S1_RXTX_MODE_MASK, BIT5 | BIT4},
    {"DMIC_2CH_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_DMIC_2CH_MODE), REG_DMIC_2CH_MODE_MASK, BIT0},
    {"DMIC_2CH_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_DMIC_2CH_MODE), REG_DMIC_2CH_MODE_MASK, BIT1},
    {"DMIC_2CH_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_DMIC_2CH_MODE), REG_DMIC_2CH_MODE_MASK, BIT1 | BIT0},
    {"DMIC_2CH_MODE_4", _RIUA_16BIT(PADTOP_BANK, REG_DMIC_2CH_MODE), REG_DMIC_2CH_MODE_MASK, BIT2},
    {"DMIC_4CH_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_DMIC_4CH_MODE), REG_DMIC_4CH_MODE_MASK, BIT0},
    {"DMIC_4CH_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_DMIC_4CH_MODE), REG_DMIC_4CH_MODE_MASK, BIT1},
    {"DMIC_4CH_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_DMIC_4CH_MODE), REG_DMIC_4CH_MODE_MASK, BIT1 | BIT0},
    {"DMIC_4CH_MODE_4", _RIUA_16BIT(PADTOP_BANK, REG_DMIC_4CH_MODE), REG_DMIC_4CH_MODE_MASK, BIT2},
    {"DMIC_8CH_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_DMIC_8CH_MODE), REG_DMIC_8CH_MODE_MASK, BIT4},
    {"DMIC_8CH_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_DMIC_8CH_MODE), REG_DMIC_8CH_MODE_MASK, BIT5},
    {"DMIC_8CH_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_DMIC_8CH_MODE), REG_DMIC_8CH_MODE_MASK, BIT5 | BIT4},
    {"DMIC_8CH_MODE_4", _RIUA_16BIT(PADTOP_BANK, REG_DMIC_8CH_MODE), REG_DMIC_8CH_MODE_MASK, BIT6},
    {"SR0_MIPI_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SR0_MIPI_MODE), REG_SR0_MIPI_MODE_MASK, BIT0},
    {"SR0_MIPI_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_SR0_MIPI_MODE), REG_SR0_MIPI_MODE_MASK, BIT1},
    {"SR0_MIPI_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_SR0_MIPI_MODE), REG_SR0_MIPI_MODE_MASK, BIT1 | BIT0},
    {"SR0_MIPI_MODE_4", _RIUA_16BIT(PADTOP_BANK, REG_SR0_MIPI_MODE), REG_SR0_MIPI_MODE_MASK, BIT2},
    {"SR0_MIPI_MODE_5", _RIUA_16BIT(PADTOP_BANK, REG_SR0_MIPI_MODE), REG_SR0_MIPI_MODE_MASK, BIT2 | BIT0},
    {"SR0_MIPI_MODE_6", _RIUA_16BIT(PADTOP_BANK, REG_SR0_MIPI_MODE), REG_SR0_MIPI_MODE_MASK, BIT2 | BIT1},
    {"SR0_MIPI_MODE_7", _RIUA_16BIT(PADTOP_BANK, REG_SR0_MIPI_MODE), REG_SR0_MIPI_MODE_MASK, BIT2 | BIT1 | BIT0},
    {"SR1_MIPI_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SR1_MIPI_MODE), REG_SR1_MIPI_MODE_MASK, BIT4},
    {"SR1_MIPI_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_SR1_MIPI_MODE), REG_SR1_MIPI_MODE_MASK, BIT5},
    {"SR1_MIPI_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_SR1_MIPI_MODE), REG_SR1_MIPI_MODE_MASK, BIT5 | BIT4},
    {"SR1_MIPI_MODE_4", _RIUA_16BIT(PADTOP_BANK, REG_SR1_MIPI_MODE), REG_SR1_MIPI_MODE_MASK, BIT6},
    {"SR00_PDN_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SR00_PDN_MODE), REG_SR00_PDN_MODE_MASK, BIT0},
    {"SR00_MCLK_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SR00_MCLK_MODE), REG_SR00_MCLK_MODE_MASK, BIT0},
    {"SR00_RST_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SR00_RST_MODE), REG_SR00_RST_MODE_MASK, BIT0},
    {"SR01_PDN_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SR01_PDN_MODE), REG_SR01_PDN_MODE_MASK, BIT2},
    {"SR01_MCLK_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SR01_MCLK_MODE), REG_SR01_MCLK_MODE_MASK, BIT3},
    {"SR01_RST_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SR01_RST_MODE), REG_SR01_RST_MODE_MASK, BIT2},
    {"SR10_PDN_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SR10_PDN_MODE), REG_SR10_PDN_MODE_MASK, BIT8},
    {"SR10_MCLK_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SR10_MCLK_MODE), REG_SR10_MCLK_MODE_MASK, BIT8},
    {"SR10_RST_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SR10_RST_MODE), REG_SR10_RST_MODE_MASK, BIT8},
    {"SR11_PDN_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SR11_PDN_MODE), REG_SR11_PDN_MODE_MASK, BIT10},
    {"SR11_MCLK_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SR11_MCLK_MODE), REG_SR11_MCLK_MODE_MASK, BIT10},
    {"SR11_RST_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SR11_RST_MODE), REG_SR11_RST_MODE_MASK, BIT10},
    {"SR0_SYNC_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SR0_SYNC_MODE), REG_SR0_SYNC_MODE_MASK, BIT8},
    {"SR0_SYNC_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_SR0_SYNC_MODE), REG_SR0_SYNC_MODE_MASK, BIT9},
    {"SR0_SYNC_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_SR0_SYNC_MODE), REG_SR0_SYNC_MODE_MASK, BIT9 | BIT8},
    {"SR0_PCLK_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SR0_PCLK_MODE), REG_SR0_PCLK_MODE_MASK, BIT0},
    {"SR0_PCLK_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_SR0_PCLK_MODE), REG_SR0_PCLK_MODE_MASK, BIT1},
    {"SR0_PCLK_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_SR0_PCLK_MODE), REG_SR0_PCLK_MODE_MASK, BIT1 | BIT0},
    {"SR0_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SR0_MODE), REG_SR0_MODE_MASK, BIT0},
    {"SR0_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_SR0_MODE), REG_SR0_MODE_MASK, BIT1},
    {"SR0_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_SR0_MODE), REG_SR0_MODE_MASK, BIT1 | BIT0},
    {"SR0_BT656_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SR0_BT656_MODE), REG_SR0_BT656_MODE_MASK, BIT0},
    {"SR1_BT656_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SR1_BT656_MODE), REG_SR1_BT656_MODE_MASK, BIT2},
    {"SR0_BT1120_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SR0_BT1120_MODE), REG_SR0_BT1120_MODE_MASK, BIT0},
    {"SR0_BT1120_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_SR0_BT1120_MODE), REG_SR0_BT1120_MODE_MASK, BIT1},
    {"SR_SLAVE_XLK_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SR_SLAVE_XLK_MODE), REG_SR_SLAVE_XLK_MODE_MASK, BIT0},
    {"SR_SLAVE_XLK_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_SR_SLAVE_XLK_MODE), REG_SR_SLAVE_XLK_MODE_MASK, BIT1},
    {"SR_SLAVE_XLK_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_SR_SLAVE_XLK_MODE), REG_SR_SLAVE_XLK_MODE_MASK, BIT1 | BIT0},
    {"SR_SLAVE_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SR_SLAVE_MODE), REG_SR_SLAVE_MODE_MASK, BIT12},
    {"SR_SLAVE_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_SR_SLAVE_MODE), REG_SR_SLAVE_MODE_MASK, BIT13},
    {"SR_SLAVE_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_SR_SLAVE_MODE), REG_SR_SLAVE_MODE_MASK, BIT13 | BIT12},
    {"LVDS_OUT_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_LVDS_OUT_MODE), REG_LVDS_OUT_MODE_MASK, BIT8},
    {"LVDS_OUT_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_LVDS_OUT_MODE), REG_LVDS_OUT_MODE_MASK, BIT9},
    {"LVDS_OUT_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_LVDS_OUT_MODE), REG_LVDS_OUT_MODE_MASK, BIT9 | BIT8},
    {"MIPITX0_OUT_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_MIPITX0_OUT_MODE), REG_MIPITX0_OUT_MODE_MASK, BIT0},
    {"MIPITX0_OUT_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_MIPITX0_OUT_MODE), REG_MIPITX0_OUT_MODE_MASK, BIT1},
    {"MIPITX0_OUT_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_MIPITX0_OUT_MODE), REG_MIPITX0_OUT_MODE_MASK, BIT1 | BIT0},
    {"MIPITX0_OUT_MODE_4", _RIUA_16BIT(PADTOP_BANK, REG_MIPITX0_OUT_MODE), REG_MIPITX0_OUT_MODE_MASK, BIT2},
    {"MIPITX1_OUT_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_MIPITX1_OUT_MODE), REG_MIPITX1_OUT_MODE_MASK, BIT4},
    {"MIPITX1_OUT_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_MIPITX1_OUT_MODE), REG_MIPITX1_OUT_MODE_MASK, BIT5},
    {"MIPITX1_OUT_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_MIPITX1_OUT_MODE), REG_MIPITX1_OUT_MODE_MASK, BIT5 | BIT4},
    {"MIPITX1_OUT_MODE_4", _RIUA_16BIT(PADTOP_BANK, REG_MIPITX1_OUT_MODE), REG_MIPITX1_OUT_MODE_MASK, BIT6},
    {"BT656_OUT_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_BT656_OUT_MODE), REG_BT656_OUT_MODE_MASK, BIT4},
    {"BT656_OUT_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_BT656_OUT_MODE), REG_BT656_OUT_MODE_MASK, BIT5},
    {"BT601_OUT_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_BT601_OUT_MODE), REG_BT601_OUT_MODE_MASK, BIT4},
    {"BT601_OUT_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_BT601_OUT_MODE), REG_BT601_OUT_MODE_MASK, BIT5},
    {"TTL24_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_TTL24_MODE), REG_TTL24_MODE_MASK, BIT8},
    {"TTL24_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_TTL24_MODE), REG_TTL24_MODE_MASK, BIT9},
    {"TTL24_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_TTL24_MODE), REG_TTL24_MODE_MASK, BIT9 | BIT8},
    {"TTL18_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_TTL18_MODE), REG_TTL18_MODE_MASK, BIT4},
    {"TTL18_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_TTL18_MODE), REG_TTL18_MODE_MASK, BIT5},
    {"TTL18_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_TTL18_MODE), REG_TTL18_MODE_MASK, BIT5 | BIT4},
    {"TTL16_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_TTL16_MODE), REG_TTL16_MODE_MASK, BIT0},
    {"TTL16_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_TTL16_MODE), REG_TTL16_MODE_MASK, BIT1},
    {"TTL16_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_TTL16_MODE), REG_TTL16_MODE_MASK, BIT1 | BIT0},
    {"RGMII0_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_RGMII0_MODE), REG_RGMII0_MODE_MASK, BIT0},
    {"GPHY0_REF_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_GPHY0_REF_MODE), REG_GPHY0_REF_MODE_MASK, BIT0},
    {"RMII_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_RMII_MODE), REG_RMII_MODE_MASK, BIT8},
    {"RMII1_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_RMII1_MODE), REG_RMII1_MODE_MASK, BIT1},
    {"PSPI0_G_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PSPI0_G_MODE), REG_PSPI0_G_MODE_MASK, BIT0},
    {"PSPI0_TE_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PSPI0_TE_MODE), REG_PSPI0_TE_MODE_MASK, BIT4},
    {"PSPI0_SENSOR_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PSPI0_SENSOR_MODE), REG_PSPI0_SENSOR_MODE_MASK, BIT0},
    {"PSPI0_SENSOR_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PSPI0_SENSOR_MODE), REG_PSPI0_SENSOR_MODE_MASK, BIT1},
    {"PSPI0_SENSOR_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_PSPI0_SENSOR_MODE), REG_PSPI0_SENSOR_MODE_MASK, BIT1 | BIT0},
    {"PSPI0_CS_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PSPI0_CS_MODE), REG_PSPI0_CS_MODE_MASK, BIT2},
    {"PSPI0_VSYNC_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PSPI0_VSYNC_MODE), REG_PSPI0_VSYNC_MODE_MASK, BIT6},
    {"PSPI0_CS2_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PSPI0_CS2_MODE), REG_PSPI0_CS2_MODE_MASK, BIT2},
    {"PSPI1_G_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PSPI1_G_MODE), REG_PSPI1_G_MODE_MASK, BIT8},
    {"PSPI1_PANEL_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PSPI1_PANEL_MODE), REG_PSPI1_PANEL_MODE_MASK, BIT4},
    {"PSPI1_PANEL_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PSPI1_PANEL_MODE), REG_PSPI1_PANEL_MODE_MASK, BIT5},
    {"PSPI1_TE_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PSPI1_TE_MODE), REG_PSPI1_TE_MODE_MASK, BIT12},
    {"PSPI1_TE_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PSPI1_TE_MODE), REG_PSPI1_TE_MODE_MASK, BIT13},
    {"PSPI1_CS2_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PSPI1_CS2_MODE), REG_PSPI1_CS2_MODE_MASK, BIT10},
    {"PSPI1_CS2_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_PSPI1_CS2_MODE), REG_PSPI1_CS2_MODE_MASK, BIT11},
    {"PSPI1_VSYNC_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_PSPI1_VSYNC_MODE), REG_PSPI1_VSYNC_MODE_MASK, BIT14},
    {"IR0_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_IR0_MODE), REG_IR0_MODE_MASK, BIT0},
    {"IR0_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_IR0_MODE), REG_IR0_MODE_MASK, BIT1},
    {"IR0_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_IR0_MODE), REG_IR0_MODE_MASK, BIT1 | BIT0},
    {"IR1_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_IR1_MODE), REG_IR1_MODE_MASK, BIT2},
    {"IR1_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_IR1_MODE), REG_IR1_MODE_MASK, BIT3},
    {"IR1_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_IR1_MODE), REG_IR1_MODE_MASK, BIT3 | BIT2},
    {"IR2_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_IR2_MODE), REG_IR2_MODE_MASK, BIT4},
    {"IR2_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_IR2_MODE), REG_IR2_MODE_MASK, BIT5},
    {"IR2_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_IR2_MODE), REG_IR2_MODE_MASK, BIT5 | BIT4},
    {"IR3_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_IR3_MODE), REG_IR3_MODE_MASK, BIT6},
    {"IR3_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_IR3_MODE), REG_IR3_MODE_MASK, BIT7},
    {"IR3_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_IR3_MODE), REG_IR3_MODE_MASK, BIT7 | BIT6},
    {"KEY_FIX_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_KEY_FIX_MODE), REG_KEY_FIX_MODE_MASK, BIT8},
    {"KEY_FIX_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_KEY_FIX_MODE), REG_KEY_FIX_MODE_MASK, BIT9},
    {"KEY_FIX_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_KEY_FIX_MODE), REG_KEY_FIX_MODE_MASK, BIT9 | BIT8},
    {"HDMI_I2C_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_HDMI_I2C_MODE), REG_HDMI_I2C_MODE_MASK, BIT0},
    {"HDMI_CEC_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_HDMI_CEC_MODE), REG_HDMI_CEC_MODE_MASK, BIT4},
    {"HDMI_HTP_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_HDMI_HTP_MODE), REG_HDMI_HTP_MODE_MASK, BIT8},
    {"HDMI_5VDET_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_HDMI_5VDET_MODE), REG_HDMI_5VDET_MODE_MASK, BIT12},
    {"SPDIF_MODE_1", _RIUA_16BIT(PADTOP_BANK, REG_SPDIF_MODE), REG_SPDIF_MODE_MASK, BIT0},
    {"SPDIF_MODE_2", _RIUA_16BIT(PADTOP_BANK, REG_SPDIF_MODE), REG_SPDIF_MODE_MASK, BIT1},
    {"SPDIF_MODE_3", _RIUA_16BIT(PADTOP_BANK, REG_SPDIF_MODE), REG_SPDIF_MODE_MASK, BIT1 | BIT0},
    {"OTP_TEST_1", _RIUA_16BIT(PADTOP_BANK, REG_OTP_TEST), REG_OTP_TEST_MASK, BIT8},
    {"IR_IN_MODE_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_IR_IN_MODE), REG_IR_IN_MODE_MASK, BIT0},
    {"I2CM_MODE_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_I2CM_MODE), REG_I2CM_MODE_MASK, BIT0},
    {"I2CM_MODE_2", _RIUA_16BIT(PM_PADTOP_BANK, REG_I2CM_MODE), REG_I2CM_MODE_MASK, BIT1},
    {"PM_UART1_MODE_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_PM_UART1_MODE), REG_PM_UART1_MODE_MASK, BIT0},
    {"PM51_UART_MODE_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_PM51_UART_MODE), REG_PM51_UART_MODE_MASK, BIT0},
    {"PWM0_MODE_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_PWM0_MODE), REG_PWM0_MODE_MASK, BIT0},
    {"VID_MODE_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_VID_MODE), REG_VID_MODE_MASK, BIT2},
    {"PWM1_MODE_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_PWM1_MODE), REG_PWM1_MODE_MASK, BIT2},
    {"PM_PAD_EXT_MODE0_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE0), REG_PM_PAD_EXT_MODE0_MASK, BIT0},
    {"PM_PAD_EXT_MODE1_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE1), REG_PM_PAD_EXT_MODE1_MASK, BIT1},
    {"PM_PAD_EXT_MODE2_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE2), REG_PM_PAD_EXT_MODE2_MASK, BIT2},
    {"PM_PAD_EXT_MODE3_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE3), REG_PM_PAD_EXT_MODE3_MASK, BIT3},
    {"PM_PAD_EXT_MODE4_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE4), REG_PM_PAD_EXT_MODE4_MASK, BIT4},
    {"PM_PAD_EXT_MODE5_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE5), REG_PM_PAD_EXT_MODE5_MASK, BIT5},
    {"PM_PAD_EXT_MODE6_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE6), REG_PM_PAD_EXT_MODE6_MASK, BIT6},
    {"PM_PAD_EXT_MODE7_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE7), REG_PM_PAD_EXT_MODE7_MASK, BIT7},
    {"PM_PAD_EXT_MODE8_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE8), REG_PM_PAD_EXT_MODE8_MASK, BIT8},
    {"PM_PAD_EXT_MODE9_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE9), REG_PM_PAD_EXT_MODE9_MASK, BIT9},
    {"PM_PAD_EXT_MODE10_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE10), REG_PM_PAD_EXT_MODE10_MASK, BIT10},
    {"PM_PAD_EXT_MODE11_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE11), REG_PM_PAD_EXT_MODE11_MASK, BIT11},
    {"PM_PAD_EXT_MODE12_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE12), REG_PM_PAD_EXT_MODE12_MASK, BIT12},
    {"PM_PAD_EXT_MODE13_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE13), REG_PM_PAD_EXT_MODE13_MASK, BIT13},
    {"PM_PAD_EXT_MODE14_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE14), REG_PM_PAD_EXT_MODE14_MASK, BIT14},
    {"PM_PAD_EXT_MODE15_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE15), REG_PM_PAD_EXT_MODE15_MASK, BIT15},
    {"PM_PAD_EXT_MODE16_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE16), REG_PM_PAD_EXT_MODE16_MASK, BIT0},
    {"PM_PAD_EXT_MODE17_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE17), REG_PM_PAD_EXT_MODE17_MASK, BIT1},
    {"PM_PAD_EXT_MODE18_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_PM_PAD_EXT_MODE18), REG_PM_PAD_EXT_MODE18_MASK, BIT2},
    {"SPI_GPIO_0", _RIUA_16BIT(PMSLEEP_BANK, REG_SPI_GPIO), REG_SPI_GPIO_MASK, 0},
    {"SPIWPN_GPIO_0", _RIUA_16BIT(PMSLEEP_BANK, REG_SPIWPN_GPIO), REG_SPIWPN_GPIO_MASK, 0},
    {"SPICSZ1_GPIO_0", _RIUA_16BIT(PMSLEEP_BANK, REG_SPICSZ1_GPIO), REG_SPICSZ1_GPIO_MASK, 0},
    {"SPICSZ2_MODE_1", _RIUA_16BIT(PM_PADTOP_BANK, REG_SPICSZ2_MODE), REG_SPICSZ2_MODE_MASK, BIT0},
    {"SPIHOLDN_MODE_0", _RIUA_16BIT(PM_PADTOP_BANK, REG_SPIHOLDN_MODE), REG_SPIHOLDN_MODE_MASK, 0},
    {"PM_UART_IS_GPIO_0", _RIUA_16BIT(PM_PADTOP_BANK, REG_PM_UART_IS_GPIO), REG_PM_UART_IS_GPIO_MASK, 0},
};

//==============================================================================
//
//                              FUNCTIONS
//
//==============================================================================

//------------------------------------------------------------------------------
//  Function    : _HalCheckPin
//  Description :
//------------------------------------------------------------------------------
static S32 _HalCheckPin(U32 padID)
{
    if (GPIO_NR <= padID)
    {
        return FALSE;
    }
    return TRUE;
}

#if 0
static void _HalSARGPIOWriteRegBit(u32 u32RegOffset, bool bEnable, U8 u8BitMsk)
{
    if (bEnable)
        _GPIO_R_BYTE(_RIUA_8BIT(PM_SAR_BANK, u32RegOffset)) |= u8BitMsk;
    else
        _GPIO_R_BYTE(_RIUA_8BIT(PM_SAR_BANK, u32RegOffset)) &= (~u8BitMsk);
}
#endif

void _HalPadDisablePadMux(U32 u32PadModeID)
{
    if (_GPIO_R_WORD_MASK(m_stPadModeInfoTbl[u32PadModeID].u32ModeRIU, m_stPadModeInfoTbl[u32PadModeID].u16ModeMask))
    {
        _GPIO_W_WORD_MASK(m_stPadModeInfoTbl[u32PadModeID].u32ModeRIU, 0, m_stPadModeInfoTbl[u32PadModeID].u16ModeMask);
    }
}

void _HalPadEnablePadMux(U32 u32PadModeID)
{
    _GPIO_W_WORD_MASK(m_stPadModeInfoTbl[u32PadModeID].u32ModeRIU, m_stPadModeInfoTbl[u32PadModeID].u16ModeVal,
                      m_stPadModeInfoTbl[u32PadModeID].u16ModeMask);
}

static S32 HalPadSetMode_General(U32 u32PadID, U32 u32Mode)
{
    U32 u32RegAddr   = 0;
    U16 u16RegVal    = 0;
    U8  u8ModeIsFind = 0;
    U16 i            = 0;

    for (i = 0; i < m_stPadMuxEntry[u32PadID].size; i++)
    {
        u32RegAddr = _RIUA_16BIT(m_stPadMuxEntry[u32PadID].padmux[i].base, m_stPadMuxEntry[u32PadID].padmux[i].offset);
        if (u32Mode == m_stPadMuxEntry[u32PadID].padmux[i].mode)
        {
            u16RegVal = _GPIO_R_WORD_MASK(u32RegAddr, 0xFFFF);
            u16RegVal &= ~(m_stPadMuxEntry[u32PadID].padmux[i].mask);
            u16RegVal |= m_stPadMuxEntry[u32PadID].padmux[i].val; // CHECK Multi-Pad Mode
            _GPIO_W_WORD_MASK(u32RegAddr, u16RegVal, 0xFFFF);
            u8ModeIsFind       = 1;
            Pad_Mode[u32PadID] = u32Mode;
#if (ENABLE_CHECK_ALL_PAD_CONFLICT == 0)
            break;
#endif
        }
        else
        {
            if ((u32Mode == PINMUX_FOR_GPIO_MODE) && (m_stPadMuxEntry[u32PadID].padmux[i].mode > PRIORITY_GREATER_GPIO))
                continue;
            u16RegVal = _GPIO_R_WORD_MASK(u32RegAddr, m_stPadMuxEntry[u32PadID].padmux[i].mask);
            if (u16RegVal == m_stPadMuxEntry[u32PadID].padmux[i].val)
            {
                _PADMUX_PRINT("[Padmux]reset PAD%d(reg 0x%x:%x; mask0x%x) t0 %s (org: %s)\n", u32PadID,
                       m_stPadMuxEntry[u32PadID].padmux[i].base, m_stPadMuxEntry[u32PadID].padmux[i].offset,
                       m_stPadMuxEntry[u32PadID].padmux[i].mask, m_stPadModeInfoTbl[u32Mode].u8PadName,
                       m_stPadModeInfoTbl[m_stPadMuxEntry[u32PadID].padmux[i].mode].u8PadName);
                if (m_stPadMuxEntry[u32PadID].padmux[i].val != 0)
                {
                    _GPIO_W_WORD_MASK(u32RegAddr, 0, m_stPadMuxEntry[u32PadID].padmux[i].mask);
                }
                else
                {
                    _GPIO_W_WORD_MASK(u32RegAddr, m_stPadMuxEntry[u32PadID].padmux[i].mask,
                                      m_stPadMuxEntry[u32PadID].padmux[i].mask);
                }
            }
        }
    }
    return (u8ModeIsFind) ? 0 : 1;
}

static S32 HalPadSetMode_GeneralPMPad(U32 u32PadID, U32 u32Mode)
{
    U32 u32RegAddr   = 0;
    U16 u16RegVal    = 0;
    U8  u8ModeIsFind = 0;
    U16 i, ExtItemID = 0;

    for (i = 0; i < m_stPadMuxEntry[u32PadID].size; i++)
    {
        u32RegAddr = _RIUA_16BIT(m_stPadMuxEntry[u32PadID].padmux[i].base, m_stPadMuxEntry[u32PadID].padmux[i].offset);
        if (u32Mode == m_stPadMuxEntry[u32PadID].padmux[i].mode)
        {
            u16RegVal = _GPIO_R_WORD_MASK(u32RegAddr, 0xFFFF);
            u16RegVal &= ~(m_stPadMuxEntry[u32PadID].padmux[i].mask);
            u16RegVal |= m_stPadMuxEntry[u32PadID].padmux[i].val; // CHECK Multi-Pad Mode
            _GPIO_W_WORD_MASK(u32RegAddr, u16RegVal, 0xFFFF);
            u8ModeIsFind       = 1;
            Pad_Mode[u32PadID] = u32Mode;
#if (ENABLE_CHECK_ALL_PAD_CONFLICT == 0)
            break;
#endif
        }
        else if ((m_stPadMuxEntry[u32PadID].padmux[i].mode >= PINMUX_FOR_PM_PAD_EXT_MODE0_1)
                 && (m_stPadMuxEntry[u32PadID].padmux[i].mode <= PINMUX_FOR_PM_PAD_EXT_MODE18_1))
        {
            ExtItemID = i;
        }
        else
        {
            if ((u32Mode == PINMUX_FOR_GPIO_MODE) && (m_stPadMuxEntry[u32PadID].padmux[i].mode > PRIORITY_GREATER_GPIO))
                continue;
            u16RegVal = _GPIO_R_WORD_MASK(u32RegAddr, m_stPadMuxEntry[u32PadID].padmux[i].mask);
            if (u16RegVal == m_stPadMuxEntry[u32PadID].padmux[i].val)
            {
                _PADMUX_PRINT("[Padmux]reset PAD%d(reg 0x%x:%x; mask0x%x) t0 %s (org: %s)\n", u32PadID,
                       m_stPadMuxEntry[u32PadID].padmux[i].base, m_stPadMuxEntry[u32PadID].padmux[i].offset,
                       m_stPadMuxEntry[u32PadID].padmux[i].mask, m_stPadModeInfoTbl[u32Mode].u8PadName,
                       m_stPadModeInfoTbl[m_stPadMuxEntry[u32PadID].padmux[i].mode].u8PadName);
                if (m_stPadMuxEntry[u32PadID].padmux[i].val != 0)
                {
                    _GPIO_W_WORD_MASK(u32RegAddr, 0, m_stPadMuxEntry[u32PadID].padmux[i].mask);
                }
                else
                {
                    _GPIO_W_WORD_MASK(u32RegAddr, m_stPadMuxEntry[u32PadID].padmux[i].mask,
                                      m_stPadMuxEntry[u32PadID].padmux[i].mask);
                }
            }
        }
    }

    if (u8ModeIsFind)
    {
        // set external data mode
        u32RegAddr = _RIUA_16BIT(m_stPadMuxEntry[u32PadID].padmux[ExtItemID].base,
                                 m_stPadMuxEntry[u32PadID].padmux[ExtItemID].offset);
        u16RegVal  = _GPIO_R_WORD_MASK(u32RegAddr, 0xFFFF);
        u16RegVal &= ~(m_stPadMuxEntry[u32PadID].padmux[ExtItemID].mask);
        u16RegVal |= m_stPadMuxEntry[u32PadID].padmux[ExtItemID].val; // CHECK Multi-Pad Mode

        _GPIO_W_WORD_MASK(u32RegAddr, u16RegVal, 0xFFFF);
    }

    return (u8ModeIsFind) ? 0 : 1;
}

//#define PAD_PM_IRIN_PATCH // m6 should patch because hardware design
//------------------------------------------------------------------------------
//  Function    : HalPadSetVal
//  Description :
//------------------------------------------------------------------------------
S32 HalPadSetVal(U32 u32PadID, U32 u32Mode)
{
    if (FALSE == _HalCheckPin(u32PadID))
    {
        return 1;
    }
    if ((u32PadID >= PAD_PM_PWM0 && u32PadID <= PAD_PM_SPI_DI))
    {
        return HalPadSetMode_GeneralPMPad(u32PadID, u32Mode);
    }
    else
    {
        return HalPadSetMode_General(u32PadID, u32Mode);
    }
}

//------------------------------------------------------------------------------
//  Function    : HalPadSetVal
//  Description :
//------------------------------------------------------------------------------
S32 HalPadGetVal(U32 u32PadID, U32* u32Mode)
{
    if (FALSE == _HalCheckPin(u32PadID))
    {
        return 1;
    }
    *u32Mode = Pad_Mode[u32PadID];
    return 0;
}

//------------------------------------------------------------------------------
//  Function    :set GPIO voltage value
//  Description :only for i7
//------------------------------------------------------------------------------
void HalGPIOSetVol(U32 u32Group, U32 u32Mode) {}
//------------------------------------------------------------------------------
//  Function    : HalPadSet
//  Description :
//------------------------------------------------------------------------------
S32 HalPadSetMode(U32 u32Mode)
{
    U32 u32PadID;
    U16 k = 0;
    U16 i = 0;

    for (k = 0; k < sizeof(m_stPadMuxEntry) / sizeof(struct stPadMuxEntry); k++)
    {
        for (i = 0; i < m_stPadMuxEntry[k].size; i++)
        {
            if (u32Mode == m_stPadMuxEntry[k].padmux[i].mode)
            {
                u32PadID = m_stPadMuxEntry[k].padmux[i].padID;
                if (u32PadID >= GPIO_NR)
                {
                    return 1;
                }
                if ((u32PadID >= PAD_PM_PWM0 && u32PadID <= PAD_PM_SPI_DI))
                {
                    if (HalPadSetMode_GeneralPMPad(u32PadID, u32Mode))
                    {
                        return 1;
                    }
                }
                else
                {
                    if (HalPadSetMode_General(u32PadID, u32Mode))
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

S32 _HalPadCheckDisablePadMux(U32 u32PadModeID)
{
    U16 u16RegVal = 0;

    u16RegVal =
        _GPIO_R_WORD_MASK(m_stPadModeInfoTbl[u32PadModeID].u32ModeRIU, m_stPadModeInfoTbl[u32PadModeID].u16ModeMask);

    if (u16RegVal != 0)
    {
        m_stPadCheckVal.infos[m_stPadCheckVal.infocount].base =
            (U16)((m_stPadModeInfoTbl[u32PadModeID].u32ModeRIU & 0xFFFF00) >> 9);
        m_stPadCheckVal.infos[m_stPadCheckVal.infocount].offset =
            (U16)((m_stPadModeInfoTbl[u32PadModeID].u32ModeRIU & 0x1FF) >> 2);
        m_stPadCheckVal.infos[m_stPadCheckVal.infocount].mask   = m_stPadModeInfoTbl[u32PadModeID].u16ModeMask;
        m_stPadCheckVal.infos[m_stPadCheckVal.infocount].val    = 0;
        m_stPadCheckVal.infos[m_stPadCheckVal.infocount].regval = u16RegVal;
        m_stPadCheckVal.infocount++;
        return 1;
    }
    return 0;
}

S32 _HalPadCheckEnablePadMux(U32 u32PadModeID)
{
    U16 u16RegVal = 0;

    u16RegVal =
        _GPIO_R_WORD_MASK(m_stPadModeInfoTbl[u32PadModeID].u32ModeRIU, m_stPadModeInfoTbl[u32PadModeID].u16ModeMask);

    if (u16RegVal != m_stPadModeInfoTbl[u32PadModeID].u16ModeVal)
    {
        m_stPadCheckVal.infos[m_stPadCheckVal.infocount].base =
            (U16)((m_stPadModeInfoTbl[u32PadModeID].u32ModeRIU & 0xFFFF00) >> 9);
        m_stPadCheckVal.infos[m_stPadCheckVal.infocount].offset =
            (U16)((m_stPadModeInfoTbl[u32PadModeID].u32ModeRIU & 0x1FF) >> 2);
        m_stPadCheckVal.infos[m_stPadCheckVal.infocount].mask   = m_stPadModeInfoTbl[u32PadModeID].u16ModeMask;
        m_stPadCheckVal.infos[m_stPadCheckVal.infocount].val    = m_stPadModeInfoTbl[u32PadModeID].u16ModeVal;
        m_stPadCheckVal.infos[m_stPadCheckVal.infocount].regval = u16RegVal;
        m_stPadCheckVal.infocount++;
        return 1;
    }
    return 0;
}

static S32 HalPadCheckMode_General(U32 u32PadID, U32 u32Mode)
{
    U32 u32RegAddr   = 0;
    U16 u16RegVal    = 0;
    U8  u8ModeIsFind = 0;
    U8  u8ModeIsErr  = 0;
    U16 i            = 0;

    for (i = 0; i < m_stPadMuxEntry[u32PadID].size; i++)
    {
        if (u32PadID == m_stPadMuxEntry[u32PadID].padmux[i].padID)
        {
            u32RegAddr =
                _RIUA_16BIT(m_stPadMuxEntry[u32PadID].padmux[i].base, m_stPadMuxEntry[u32PadID].padmux[i].offset);

            if (u32Mode == m_stPadMuxEntry[u32PadID].padmux[i].mode)
            {
                u16RegVal = _GPIO_R_WORD_MASK(u32RegAddr, 0xFFFF);
                u16RegVal &= (m_stPadMuxEntry[u32PadID].padmux[i].mask);
                if (u16RegVal != m_stPadMuxEntry[u32PadID].padmux[i].val) // CHECK Multi-Pad Mode
                {
                    u8ModeIsErr++;

                    m_stPadCheckVal.infos[m_stPadCheckVal.infocount].base =
                        (U16)(m_stPadMuxEntry[u32PadID].padmux[i].base >> 8);
                    m_stPadCheckVal.infos[m_stPadCheckVal.infocount].offset =
                        m_stPadMuxEntry[u32PadID].padmux[i].offset;
                    m_stPadCheckVal.infos[m_stPadCheckVal.infocount].mask   = m_stPadMuxEntry[u32PadID].padmux[i].mask;
                    m_stPadCheckVal.infos[m_stPadCheckVal.infocount].val    = m_stPadMuxEntry[u32PadID].padmux[i].val;
                    m_stPadCheckVal.infos[m_stPadCheckVal.infocount].regval = u16RegVal;
                    m_stPadCheckVal.infocount++;
                }

                u8ModeIsFind = 1;
#if (ENABLE_CHECK_ALL_PAD_CONFLICT == 0)
                break;
#endif
            }
            else
            {
                if (u32Mode == PINMUX_FOR_GPIO_MODE)
                    continue;
                u16RegVal = _GPIO_R_WORD_MASK(u32RegAddr, m_stPadMuxEntry[u32PadID].padmux[i].mask);
                if (u16RegVal == m_stPadMuxEntry[u32PadID].padmux[i].val)
                {
                    u8ModeIsErr++;

                    m_stPadCheckVal.infos[m_stPadCheckVal.infocount].base =
                        (U16)(m_stPadMuxEntry[u32PadID].padmux[i].base >> 8);
                    m_stPadCheckVal.infos[m_stPadCheckVal.infocount].offset =
                        m_stPadMuxEntry[u32PadID].padmux[i].offset;
                    m_stPadCheckVal.infos[m_stPadCheckVal.infocount].mask   = m_stPadMuxEntry[u32PadID].padmux[i].mask;
                    m_stPadCheckVal.infos[m_stPadCheckVal.infocount].val    = m_stPadMuxEntry[u32PadID].padmux[i].val;
                    m_stPadCheckVal.infos[m_stPadCheckVal.infocount].regval = u16RegVal;
                    m_stPadCheckVal.infocount++;
                }
            }
        }
    }

    return (u8ModeIsFind && !u8ModeIsErr) ? 0 : 1;
}

static S32 HalPadCheckMode_GeneralPMPad(U32 u32PadID, U32 u32Mode)
{
    U32 u32RegAddr   = 0;
    U16 u16RegVal    = 0;
    U8  u8ModeIsFind = 0;
    U8  u8ModeIsErr  = 0;
    U16 i, ExtItemID = 0;

    for (i = 0; i < m_stPadMuxEntry[u32PadID].size; i++)
    {
        if (u32PadID == m_stPadMuxEntry[u32PadID].padmux[i].padID)
        {
            u32RegAddr =
                _RIUA_16BIT(m_stPadMuxEntry[u32PadID].padmux[i].base, m_stPadMuxEntry[u32PadID].padmux[i].offset);

            if (u32Mode == m_stPadMuxEntry[u32PadID].padmux[i].mode)
            {
                u16RegVal = _GPIO_R_WORD_MASK(u32RegAddr, 0xFFFF);
                u16RegVal &= (m_stPadMuxEntry[u32PadID].padmux[i].mask);
                if (u16RegVal != m_stPadMuxEntry[u32PadID].padmux[i].val) // CHECK Multi-Pad Mode
                {
                    u8ModeIsErr++;

                    m_stPadCheckVal.infos[m_stPadCheckVal.infocount].base =
                        (U16)(m_stPadMuxEntry[u32PadID].padmux[i].base >> 8);
                    m_stPadCheckVal.infos[m_stPadCheckVal.infocount].offset =
                        m_stPadMuxEntry[u32PadID].padmux[i].offset;
                    m_stPadCheckVal.infos[m_stPadCheckVal.infocount].mask   = m_stPadMuxEntry[u32PadID].padmux[i].mask;
                    m_stPadCheckVal.infos[m_stPadCheckVal.infocount].val    = m_stPadMuxEntry[u32PadID].padmux[i].val;
                    m_stPadCheckVal.infos[m_stPadCheckVal.infocount].regval = u16RegVal;
                    m_stPadCheckVal.infocount++;
                }

                u8ModeIsFind = 1;
#if (ENABLE_CHECK_ALL_PAD_CONFLICT == 0)
                break;
#endif
            }
            else if ((m_stPadMuxEntry[u32PadID].padmux[i].mode >= PINMUX_FOR_PM_PAD_EXT_MODE0_1)
                     && (m_stPadMuxEntry[u32PadID].padmux[i].mode <= PINMUX_FOR_PM_PAD_EXT_MODE18_1))
            {
                ExtItemID = i;
            }
            else
            {
                if (u32Mode == PINMUX_FOR_GPIO_MODE)
                    continue;
                u16RegVal = _GPIO_R_WORD_MASK(u32RegAddr, m_stPadMuxEntry[u32PadID].padmux[i].mask);
                if (u16RegVal == m_stPadMuxEntry[u32PadID].padmux[i].val)
                {
                    u8ModeIsErr++;

                    m_stPadCheckVal.infos[m_stPadCheckVal.infocount].base =
                        (U16)(m_stPadMuxEntry[u32PadID].padmux[i].base >> 8);
                    m_stPadCheckVal.infos[m_stPadCheckVal.infocount].offset =
                        m_stPadMuxEntry[u32PadID].padmux[i].offset;
                    m_stPadCheckVal.infos[m_stPadCheckVal.infocount].mask   = m_stPadMuxEntry[u32PadID].padmux[i].mask;
                    m_stPadCheckVal.infos[m_stPadCheckVal.infocount].val    = m_stPadMuxEntry[u32PadID].padmux[i].val;
                    m_stPadCheckVal.infos[m_stPadCheckVal.infocount].regval = u16RegVal;
                    m_stPadCheckVal.infocount++;
                }
            }
        }
    }

    if (u8ModeIsFind)
    {
        // set external data mode
        u32RegAddr = _RIUA_16BIT(m_stPadMuxEntry[u32PadID].padmux[ExtItemID].base,
                                 m_stPadMuxEntry[u32PadID].padmux[ExtItemID].offset);
        u16RegVal  = _GPIO_R_WORD_MASK(u32RegAddr, 0xFFFF);
        u16RegVal &= (m_stPadMuxEntry[u32PadID].padmux[ExtItemID].mask);
        if (u16RegVal != m_stPadMuxEntry[u32PadID].padmux[ExtItemID].val)
        {
            u8ModeIsErr++;

            m_stPadCheckVal.infos[m_stPadCheckVal.infocount].base =
                (U16)(m_stPadMuxEntry[u32PadID].padmux[ExtItemID].base >> 8);
            m_stPadCheckVal.infos[m_stPadCheckVal.infocount].offset =
                m_stPadMuxEntry[u32PadID].padmux[ExtItemID].offset;
            m_stPadCheckVal.infos[m_stPadCheckVal.infocount].mask   = m_stPadMuxEntry[u32PadID].padmux[ExtItemID].mask;
            m_stPadCheckVal.infos[m_stPadCheckVal.infocount].val    = m_stPadMuxEntry[u32PadID].padmux[ExtItemID].val;
            m_stPadCheckVal.infos[m_stPadCheckVal.infocount].regval = u16RegVal;
            m_stPadCheckVal.infocount++;
        }
    }

    return (u8ModeIsFind && !u8ModeIsErr) ? 0 : 1;
}

S32 HalPadCheckVal(U32 u32PadID, U32 u32Mode)
{
    if (FALSE == _HalCheckPin(u32PadID))
    {
        return 1;
    }

    memset(&m_stPadCheckVal, 0, sizeof(m_stPadCheckVal));
    if ((u32PadID >= PAD_PM_PWM0 && u32PadID <= PAD_PM_SPI_DI))
    {
        return HalPadCheckMode_GeneralPMPad(u32PadID, u32Mode);
    }
    else
    {
        return HalPadCheckMode_General(u32PadID, u32Mode);
    }
}

U8 HalPadCheckInfoCount(void)
{
    return m_stPadCheckVal.infocount;
}

void* HalPadCheckInfoGet(U8 u8Index)
{
    return (void*)&m_stPadCheckVal.infos[u8Index];
}

U8 HalPadModeToVal(U8* pu8Mode, U8* u8Index)
{
    U16 index;
    for (index = 0; index < (sizeof(m_stPadModeInfoTbl) / sizeof(m_stPadModeInfoTbl[0])); index++)
    {
        if (!strcmp((const char *)m_stPadModeInfoTbl[index].u8PadName, (const char *)pu8Mode))
        {
            *u8Index = index;
            return 0;
        }
    }
    return 1;
}

static U32 u32PadID[GPIO_NR + 1] = {0};

U32* HalPadModeToPadIndex(U32 u32Mode)
{
    U16 k     = 0;
    U16 i     = 0;
    U16 Count = 0;

    for (k = 0; k < sizeof(m_stPadMuxEntry) / sizeof(struct stPadMuxEntry); k++)
    {
        for (i = 0; i < m_stPadMuxEntry[k].size; i++)
        {
            if (u32Mode == m_stPadMuxEntry[k].padmux[i].mode)
            {
                u32PadID[Count] = m_stPadMuxEntry[k].padmux[i].padID;
                Count += 1;
            }
        }
    }
    u32PadID[Count] = PAD_UNKNOWN;
    return u32PadID;
}
