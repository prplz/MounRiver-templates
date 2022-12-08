/*
 * hal_uart.h- Sigmastar
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

#ifndef _HAL_UART_H_
#define _HAL_UART_H_

#include <ms_platform.h>
#include <cam_os_wrapper.h>
#include <mdrv_gpio.h>

typedef signed char        s8;
typedef unsigned char      u8;
typedef signed short       s16;
typedef unsigned short     u16;
typedef signed int         s32;
typedef unsigned int       u32;
typedef signed long long   s64;
typedef unsigned long long u64;

#define URDMA_TX_BUF_LENGTH 0x1000 // must be 8 byte aligned, linux should better be PAGE_ALIGN
#define URDMA_RX_BUF_LENGTH 0x1000 // must be 8 byte aligned, linux should better be PAGE_ALIGN
#define URDMA_RX_INTR_LEVEL 0x500  // need to think
#define URDMA_RX_TIMEOUT    0x0F
#define URDMA_TX_TIMEOUT    0x0F

/* ===== os diff ======= */
#define UART_IMPL_USDELAY(x) CamOsUsDelay(x)

/* ===== chip diff ======= */
#define CONFIG_UART_BANK                                                          \
    {                                                                             \
        0, FUART_BANK, UART0_BANK, UART1_BANK, UART2_BANK, UART3_BANK, UART4_BANK \
    }
#define CONFIG_DIGMUX_REG                                                                                       \
    {                                                                                                           \
        REG_UART_SEL0, REG_UART_SEL1, REG_UART_SEL2, REG_UART_SEL3, REG_UART_SEL4, REG_UART_SEL5, REG_UART_SEL6 \
    }
#define CONFIG_DIGMUX_SHIFT                                                                                      \
    {                                                                                                            \
        REG_UART_SEL0_SHIFT, REG_UART_SEL1_SHIFT, REG_UART_SEL2_SHIFT, REG_UART_SEL3_SHIFT, REG_UART_SEL4_SHIFT, \
            REG_UART_SEL5_SHIFT, REG_UART_SEL6_SHIFT                                                             \
    }
#define CONFIG_DIGMUX_MASK                                                                                  \
    {                                                                                                       \
        REG_UART_SEL0_MASK, REG_UART_SEL1_MASK, REG_UART_SEL2_MASK, REG_UART_SEL3_MASK, REG_UART_SEL4_MASK, \
            REG_UART_SEL5_MASK, REG_UART_SEL6_MASK                                                          \
    }
#define CONFIG_DIGMUX_SEL   \
    {                       \
        0, 1, 2, 3, 4, 5, 6 \
    }

// uart prot number
#define NR_UART_PORTS 7
// uart bank
#define FUART_BANK   (0x1102)
#define UART0_BANK   (0x1108)
#define UART1_BANK   (0x1109)
#define UART2_BANK   (0x110A)
#define UART3_BANK   (0x110B)
#define UART4_BANK   (0x110C)
#define PM_UART_BANK (0x0035)
// uart digmux select
#define REG_UART_SEL0       ((0x2000000) + (0x101E << 9) + (0x53 << 2))
#define REG_UART_SEL1       ((0x2000000) + (0x101E << 9) + (0x53 << 2))
#define REG_UART_SEL2       ((0x2000000) + (0x101E << 9) + (0x53 << 2))
#define REG_UART_SEL3       ((0x2000000) + (0x101E << 9) + (0x53 << 2))
#define REG_UART_SEL4       ((0x2000000) + (0x101E << 9) + (0x54 << 2))
#define REG_UART_SEL5       ((0x2000000) + (0x101E << 9) + (0x54 << 2))
#define REG_UART_SEL6       ((0x2000000) + (0x101E << 9) + (0x59 << 2))
#define REG_UART_SEL0_SHIFT (0)
#define REG_UART_SEL1_SHIFT (4)
#define REG_UART_SEL2_SHIFT (8)
#define REG_UART_SEL3_SHIFT (12)
#define REG_UART_SEL4_SHIFT (0)
#define REG_UART_SEL5_SHIFT (4)
#define REG_UART_SEL6_SHIFT (0)
#define REG_UART_SEL0_MASK  (0xF << REG_UART_SEL0_SHIFT)
#define REG_UART_SEL1_MASK  (0xF << REG_UART_SEL1_SHIFT)
#define REG_UART_SEL2_MASK  (0xF << REG_UART_SEL2_SHIFT)
#define REG_UART_SEL3_MASK  (0xF << REG_UART_SEL3_SHIFT)
#define REG_UART_SEL4_MASK  (0xF << REG_UART_SEL4_SHIFT)
#define REG_UART_SEL5_MASK  (0xF << REG_UART_SEL5_SHIFT)
#define REG_UART_SEL6_MASK  (0xF << REG_UART_SEL6_SHIFT)
// force_rx_disable
#define REG_FORCE_RX_DISABLE ((0x2000000) + (0x101E << 9) + (0x57 << 2))
// tx interrupt exception
#define SS_UART_8250_BUG_THRE 0
// support uart tx empty interrupt
#define SS_UART_TX_EMPTY_INTERRUPT 1

enum UART_IRQ_TYPE
{
    UART_IRQ_UNKNOWN            = 0x00000000,
    UART_IRQ_RX                 = 0x00000001,
    UART_IRQ_TX                 = 0x00000002,
    UART_IRQ_MODEM              = 0x00000004,
    UART_IRQ_BUSY               = 0x00000008,
    UART_IRQ_NO_INT             = 0x00000010,
    UART_IRQ                    = 0x000000FF,
    UART_IRQ_URDMA_RX           = 0x00000100,
    UART_IRQ_URDMA_TX           = 0x00000200,
    UART_IRQ_URDMA_RX_TIMEOUT   = 0x00000400,
    UART_IRQ_URDMA_RX_THRESHOLD = 0x00000800,
    UART_IRQ_URDMA              = 0x0000FF00,
    UART_IRQ_TX_EMPTY_INT       = 0x00010000,
    UART_IRQ_TX_EMPTY           = 0x00FF0000,
};

enum UART_FIFO_STATUS
{
    UART_FIFO_RX_READY       = 0x00000001,
    UART_FIFO_RX_OE          = 0x00000002,
    UART_FIFO_RX_PE          = 0x00000004,
    UART_FIFO_RX_FE          = 0x00000008,
    UART_FIFO_BI             = 0x00000010,
    UART_FIFO_RX_ERROR       = 0x00000020,
    UART_FIFO_TX_SHIFT_EMPTY = 0x00000040,
    UART_FIFO_TX_NOT_FULL    = 0x00000080,
    UART_FIFO_TX_EMPTY       = 0x00000100,
    UART_FIFO_RX_NOT_EMPTY   = 0x00000200,
    UART_FIFO_RX_FULL        = 0x00000400,
};

enum UART_ERRNO_e
{
    ERR_UART_SUCCESS = 0x00,
    ERR_UART_TIMEOUT,
    ERR_UART_FAILURE
};

struct uart_hal
{
    /* software flag */
    u8 urdma_en;
    u8 tx_int_en;

    /* gpio disable rx */
    u8 rx_pin;

    /* hardware configuration */
    u8  digmux;
    u8  char_bits;
    u8  stop_bits;
    u8  parity_en;
    u8  even_parity_sel;
    u8  rtscts_en;
    u8  tx_fifo_level;
    u8  rx_fifo_level;
    u16 divisor;

    /* urdma */
    u16 tx_first;
    u16 rx_sw_rptr;
    u16 rx_buf_wptr;
    u16 rx_buf_rptr;
    u64 rx_urdma_base;
    u64 tx_urdma_base;
    u32 rx_urdma_size;
    u32 tx_urdma_size;
    u8 *rx_buf;
    u8 *tx_buf;

    /* register bank */
    unsigned long uart_base;
    unsigned long urdma_base;
};

u32 HAL_UART_circ_empty(u16 wptr, u16 rptr);
u32 HAL_UART_circ_cnt_to_end(u16 wptr, u16 rptr, u32 size);
u32 HAL_UART_circ_space_end(u16 wptr, u16 rptr, u32 size);

void HAL_UART_set_digmux(struct uart_hal *hal);
void HAL_UART_init(struct uart_hal *hal);
void HAL_UART_deinit(struct uart_hal *hal);
void HAL_UART_config(struct uart_hal *hal);
void HAL_UART_irq_enable(struct uart_hal *hal, enum UART_IRQ_TYPE type, u8 u8_enable);
u32  HAL_UART_get_irq_type(struct uart_hal *hal, enum UART_IRQ_TYPE type);
u32  HAL_UART_get_fifo_status(struct uart_hal *hal);
u32  HAL_UART_write(struct uart_hal *hal, u8 *buf, u32 size);
u32  HAL_UART_read(struct uart_hal *hal, u8 *buf, u32 size);

#endif /* _HAL_UART_H_ */
