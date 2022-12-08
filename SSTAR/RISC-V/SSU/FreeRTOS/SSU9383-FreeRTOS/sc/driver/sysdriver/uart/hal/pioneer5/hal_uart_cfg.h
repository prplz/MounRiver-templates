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
struct clk_info
{
    u32 rate;
    u16 value;
};

struct reg_t{
    u64 bank_base;
    u8  reg_offset;
    u8  bit_shift;
    u32 bit_mask;
};

struct uart_cfg_t
{
    char uart_name[16];
    u8 enable;
#if defined(CONFIG_SYSDESC_SUPPORT)
    u16  deviceid;
#else
    unsigned long uart_base;
    unsigned long urdma_base;
    u32  uart_irq;
    u32  urdma_irq;
    u32  tx_empty_irq;
    u8 tx_fifo_level;
    u8 rx_fifo_level;
    u8 sctp_enable;
    u8 dma;
    u8 rx_pin;
    u8 digmux;
    u8 needdigmux;

#if defined(CONFIG_CAMCLK_SUPPORT)
    u16   clkid;
#endif
#if defined(CONFIG_PADMUX_SUPPORT)
    u16   indexgpio;
    u32  padmode;
#endif
#endif

#if !defined(CONFIG_PADMUX_SUPPORT)
    u8           padmode;
    struct reg_t reg_padmod;
#endif

#if !defined(CONFIG_CAMCLK_SUPPORT)
    struct clk_info *spi_src_clk;
    struct reg_t reg_clkgen;
#endif
};


struct uart_cfg_t g_uart_cfg[] =
{
    // uart0
    [0] =
    {
        .uart_name = "uart0",
        .enable = 0,
#if defined(CONFIG_SYSDESC_SUPPORT)
#if defined(SYSDESC_DEV_uart0)
        .deviceid = SYSDESC_DEV_uart0,
#else
        .deviceid = 0xFFFF,
#endif

#else
        .uart_base = 0x2221000,
        .urdma_base = 0x2220E00,
        .uart_irq = INT_IRQ_FUART_0,
        .urdma_irq = INT_IRQ_URDMA_0,
        .tx_empty_irq = INT_IRQ_FUART0_EMPTY,
        .tx_fifo_level = 0,
        .rx_fifo_level = 0,
        .sctp_enable = 0,
        .dma = 0,
        .rx_pin = 0,
        .digmux = 0,
        .needdigmux = 0,

#if defined(CONFIG_CAMCLK_SUPPORT)
        .clkid = CAMCLK_fuart0,
#endif

#if defined(CONFIG_PADMUX_SUPPORT)
        .indexgpio = PAD_UNKNOWN,
        .padmode = 0,
#endif

#endif

#if !defined(CONFIG_PADMUX_SUPPORT)
        .padmode = 0,
        .reg_padmod =
        {
            .bank_base = 0,
            .reg_offset = 0,
            .bit_shift = 0,
            .bit_mask = 0,
        },
#endif

#if !defined(CONFIG_CAMCLK_SUPPORT)
        .spi_src_clk = NULL,
        .reg_clkgen =
        {
            .bank_base = 0,
            .reg_offset = 0,
            .bit_shift = 0,
            .bit_mask = 0,
        },
#endif
    },

    // uart1
    [1] =
    {
        .uart_name = "uart1",
        .enable = 1,
#if defined(CONFIG_SYSDESC_SUPPORT)
#if defined(SYSDESC_DEV_uart1)
        .deviceid = SYSDESC_DEV_uart1,
#else
        .deviceid = 0xFFFF,
#endif

#else
        .uart_base = 0x2221200,
        .urdma_base = 0x2221C00,
        .uart_irq = INT_IRQ_FUART_1,
        .urdma_irq = INT_IRQ_UARTDMA_1,
        .tx_empty_irq = INT_IRQ_FUART1_EMPTY,
        .tx_fifo_level = 0,
        .rx_fifo_level = 0,
        .sctp_enable = 0,
        .dma = 0,
        .rx_pin = 0,
        .digmux = 0,
        .needdigmux = 0,

#if defined(CONFIG_CAMCLK_SUPPORT)
        .clkid = CAMCLK_fuart1,
#endif

#if defined(CONFIG_PADMUX_SUPPORT)
        .indexgpio = PAD_UNKNOWN,
        .padmode = 0,
#endif

#endif

#if !defined(CONFIG_PADMUX_SUPPORT)
        .padmode = 0,
        .reg_padmod =
        {
            .bank_base = 0,
            .reg_offset = 0,
            .bit_shift = 0,
            .bit_mask = 0,
        },
#endif

#if !defined(CONFIG_CAMCLK_SUPPORT)
        .spi_src_clk = NULL,
        .reg_clkgen =
        {
            .bank_base = 0,
            .reg_offset = 0,
            .bit_shift = 0,
            .bit_mask = 0,
        },
#endif
    },

    // uart2
    [2] =
    {
        .uart_name = "uart2",
        .enable = 1,
#if defined(CONFIG_SYSDESC_SUPPORT)
#if defined(SYSDESC_DEV_uart2)
        .deviceid = SYSDESC_DEV_uart2,
#else
        .deviceid = 0xFFFF,
#endif

#else
        .uart_base = 0x2221400,
        .urdma_base = 0x2221E00,
        .uart_irq = INT_IRQ_FUART_2,
        .urdma_irq = INT_IRQ_URDMA_2,
        .tx_empty_irq = INT_IRQ_FUART2_EMPTY,
        .tx_fifo_level = 0,
        .rx_fifo_level = 0,
        .sctp_enable = 0,
        .dma = 0,
        .rx_pin = 0,
        .digmux = 0,
        .needdigmux = 0,

#if defined(CONFIG_CAMCLK_SUPPORT)
        .clkid = CAMCLK_fuart2,
#endif

#if defined(CONFIG_PADMUX_SUPPORT)
        .indexgpio = PAD_UNKNOWN,
        .padmode = 0,
#endif

#endif

#if !defined(CONFIG_PADMUX_SUPPORT)
        .padmode = 0,
        .reg_padmod =
        {
            .bank_base = 0,
            .reg_offset = 0,
            .bit_shift = 0,
            .bit_mask = 0,
        },
#endif

#if !defined(CONFIG_CAMCLK_SUPPORT)
        .spi_src_clk = NULL,
        .reg_clkgen =
        {
            .bank_base = 0,
            .reg_offset = 0,
            .bit_shift = 0,
            .bit_mask = 0,
        },
#endif
    },

    // uart3
    [3] =
    {
        .uart_name = "uart3",
        .enable = 1,
#if defined(CONFIG_SYSDESC_SUPPORT)
#if defined(SYSDESC_DEV_uart3)
        .deviceid = SYSDESC_DEV_uart3,
#else
        .deviceid = 0xFFFF,
#endif

#else
        .uart_base = 0x2221600,
        .urdma_base = 0x2222400,
        .uart_irq = INT_IRQ_FUART_3,
        .urdma_irq = INT_IRQ_URDMA_3,
        .tx_empty_irq = INT_IRQ_FUART3_EMPTY,
        .tx_fifo_level = 0,
        .rx_fifo_level = 0,
        .sctp_enable = 0,
        .dma = 0,
        .rx_pin = 0,
        .digmux = 0,
        .needdigmux = 0,

#if defined(CONFIG_CAMCLK_SUPPORT)
        .clkid = CAMCLK_fuart3,
#endif

#if defined(CONFIG_PADMUX_SUPPORT)
        .indexgpio = PAD_UNKNOWN,
        .padmode = 0,
#endif

#endif

#if !defined(CONFIG_PADMUX_SUPPORT)
        .padmode = 0,
        .reg_padmod =
        {
            .bank_base = 0,
            .reg_offset = 0,
            .bit_shift = 0,
            .bit_mask = 0,
        },
#endif

#if !defined(CONFIG_CAMCLK_SUPPORT)
        .spi_src_clk = NULL,
        .reg_clkgen =
        {
            .bank_base = 0,
            .reg_offset = 0,
            .bit_shift = 0,
            .bit_mask = 0,
        },
#endif
    },

    // uart4
    [4] =
    {
        .uart_name = "uart4",
        .enable = 1,
#if defined(CONFIG_SYSDESC_SUPPORT)
#if defined(SYSDESC_DEV_uart4)
        .deviceid = SYSDESC_DEV_uart4,
#else
        .deviceid = 0xFFFF,
#endif

#else
        .uart_base = 0x2221800,
        .urdma_base = 0x2222600,
        .uart_irq = INT_IRQ_FUART_4,
        .urdma_irq = INT_IRQ_URDMA_4,
        .tx_empty_irq = INT_IRQ_FUART4_EMPTY,
        .tx_fifo_level = 0,
        .rx_fifo_level = 0,
        .sctp_enable = 0,
        .dma = 0,
        .rx_pin = 0,
        .digmux = 0,
        .needdigmux = 0,

#if defined(CONFIG_CAMCLK_SUPPORT)
        .clkid = CAMCLK_fuart4,
#endif

#if defined(CONFIG_PADMUX_SUPPORT)
        .indexgpio = PAD_UNKNOWN,
        .padmode = 0,
#endif

#endif

#if !defined(CONFIG_PADMUX_SUPPORT)
        .padmode = 0,
        .reg_padmod =
        {
            .bank_base = 0,
            .reg_offset = 0,
            .bit_shift = 0,
            .bit_mask = 0,
        },
#endif

#if !defined(CONFIG_CAMCLK_SUPPORT)
        .spi_src_clk = NULL,
        .reg_clkgen =
        {
            .bank_base = 0,
            .reg_offset = 0,
            .bit_shift = 0,
            .bit_mask = 0,
        },
#endif
    },

    // fuart
    [5] =
    {
        .uart_name = "fuart",
        .enable = 1,
#if defined(CONFIG_SYSDESC_SUPPORT)
#if defined(SYSDESC_DEV_fuart)
        .deviceid = SYSDESC_DEV_fuart,
#else
        .deviceid = 0xFFFF,
#endif

#else
        .uart_base = 0x2220400,
        .urdma_base = 0x2220600,
        .uart_irq = INT_IRQ_FUART,
        .urdma_irq = INT_IRQ_URDMA,
        .tx_empty_irq = INT_IRQ_FUART_EMPTY,
        .tx_fifo_level = 0,
        .rx_fifo_level = 0,
        .sctp_enable = 0,
        .dma = 0,
        .rx_pin = 0,
        .digmux = 0,
        .needdigmux = 0,

#if defined(CONFIG_CAMCLK_SUPPORT)
        .clkid = CAMCLK_fuart,
#endif

#if defined(CONFIG_PADMUX_SUPPORT)
        .indexgpio = PAD_UNKNOWN,
        .padmode = 0,
#endif

#endif

#if !defined(CONFIG_PADMUX_SUPPORT)
        .padmode = 0,
        .reg_padmod =
        {
            .bank_base = 0,
            .reg_offset = 0,
            .bit_shift = 0,
            .bit_mask = 0,
        },
#endif

#if !defined(CONFIG_CAMCLK_SUPPORT)
        .spi_src_clk = NULL,
        .reg_clkgen =
        {
            .bank_base = 0,
            .reg_offset = 0,
            .bit_shift = 0,
            .bit_mask = 0,
        },
#endif
    },
    // pm_uart
    [6] =
    {
        .uart_name = "pm_uart",
        .enable = 1,
#if defined(CONFIG_SYSDESC_SUPPORT)
#if defined(SYSDESC_DEV_pm_uart)
        .deviceid = SYSDESC_DEV_pm_uart,
#else
        .deviceid = 0xFFFF,
#endif

#else
        .uart_base = 0x2006A00,
        .urdma_base = 0,
        .uart_irq = INT_IRQ_PM_UART,
        .urdma_irq = 0,
        .tx_empty_irq = 0,
        .tx_fifo_level = 0,
        .rx_fifo_level = 0,
        .sctp_enable = 0,
        .dma = 0,
        .rx_pin = 0,
        .digmux = 0,
        .needdigmux = 0,

#if defined(CONFIG_CAMCLK_SUPPORT)
        .clkid = CAMCLK_fuart,
#endif

#if defined(CONFIG_PADMUX_SUPPORT)
        .indexgpio = PAD_UNKNOWN,
        .padmode = 0,
#endif

#endif

#if !defined(CONFIG_PADMUX_SUPPORT)
        .padmode = 0,
        .reg_padmod =
        {
            .bank_base = 0,
            .reg_offset = 0,
            .bit_shift = 0,
            .bit_mask = 0,
        },
#endif

#if !defined(CONFIG_CAMCLK_SUPPORT)
        .spi_src_clk = NULL,
        .reg_clkgen =
        {
            .bank_base = 0,
            .reg_offset = 0,
            .bit_shift = 0,
            .bit_mask = 0,
        },
#endif
    }
};
