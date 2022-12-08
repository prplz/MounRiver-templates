/*
 * hal_uart.c- Sigmastar
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

#include <hal_uart.h>

/* Interrupt Enable Register (IER)*/
#define UART_IER_RDI   0x01 /* Received Data Available Interrupt */
#define UART_IER_THRI  0x02 /* Transmitter Holding Register Empty Interrupt */
#define UART_IER_RLSI  0x04 /* Receiver Line Status Interrupt */
#define UART_IER_MSI   0x08 /* Modem Status Interrupt */
#define UART_IER_PTHRI 0x80 /* Programmable THRE Interrupt */

/* Interrupt Identification Register (IIR) */
#define UART_IIR_MSI        0x00 /* 0000: Modem Status */
#define UART_IIR_NO_INT     0x01 /* 0001: No pending interrupts */
#define UART_IIR_THRI       0x02 /* 0010: Transmitter Holding Register Empty */
#define UART_IIR_RDI        0x04 /* 0100: Receiver Data Available */
#define UART_IIR_RLSI       0x06 /* 0110: Receiver Line Status */
#define UART_IIR_BUSY       0x07 /* 0111: Busy detect indication (try to write LCR while UART is busy) */
#define UART_IIR_RX_TIMEOUT 0x0C /* 1100: Character timeout */
#define UART_IIR_ID_MASK    0x0F /* Mask Bit[3:0] for IIR */

/* FIFO Control Register (FCR) */
#define UART_FCR_FIFO_ENABLE   0x01 /* Clear & Reset Rx FIFO buffer */
#define UART_FCR_CLEAR_RCVR    0x02 /* Clear & Reset Rx FIFO buffer */
#define UART_FCR_CLEAR_XMIT    0x04 /* Clear & Reset Tx FIFO buffer */
#define UART_FCR_TRIGGER_TX_L0 0x00 /* Trigger Write when emtpy */
#define UART_FCR_TRIGGER_TX_L1 0x10 /* Trigger Write when 2 characters */
#define UART_FCR_TRIGGER_TX_L2 0x20 /* Trigger Write when 1/4 full */
#define UART_FCR_TRIGGER_TX_L3 0x30 /* Trigger Write when 1/2 full */
#define UART_FCR_TRIGGER_RX_L0 0x00 /* Trigger Read when there is 1 char*/
#define UART_FCR_TRIGGER_RX_L1 0x40 /* Trigger Read when 1/4 full */
#define UART_FCR_TRIGGER_RX_L2 0x80 /* Trigger Read when 1/2 full */
#define UART_FCR_TRIGGER_RX_L3 0xC0 /* Trigger Read when 2 less then full  */

/* Line Control Register (LCR) */
#define UART_LCR_WL_MASK    0x03 /* Word length mask */
#define UART_LCR_WLEN5      0x00 /* Word length is 5 bits */
#define UART_LCR_WLEN6      0x01 /* Word length is 6 bits */
#define UART_LCR_WLEN7      0x02 /* Word length is 7 bits */
#define UART_LCR_WLEN8      0x03 /* Word length is 8 bits */
#define UART_LCR_STOP_MASK  0x04 /* Stop bit mask */
#define UART_LCR_STOP1      0x00 /* Stop length is 1 bit */
#define UART_LCR_STOP2      0x04 /* Stop length is 1.5 bits (5-bit char), 2 bits (otherwise) */
#define UART_LCR_PARITY_EN  0x08 /* Parity Enable */
#define UART_LCR_PARITY_SEL 0x10 /* Even Parity Select */
#define UART_LCR_SBC        0x40 /* Set break control */
#define UART_LCR_DLAB       0x80 /* Divisor Latch Access bit, 1=Divisor Latch, 0=Normal Register */

#define UART_MCR_DTR      0x01
#define UART_MCR_RTS      0x02
#define UART_MCR_OUT1     0x04
#define UART_MCR_OUT2     0x08
#define UART_MCR_LOOPBACK 0x10
#define UART_MCR_AFCE     0x20

/* Line Status Register */
#define UART_LSR_DR    0x01 /* Data Ready, at least one char in FIFO buffer*/
#define UART_LSR_OE    0x02 /* Overrun Error, FIFO buffer is full */
#define UART_LSR_PE    0x04 /* Parity Error */
#define UART_LSR_FE    0x08 /* Framing Error, no valid stop bit */
#define UART_LSR_BI    0x10 /* Break Interrupt */
#define UART_LSR_THRE  0x20 /* Tx FIFO buffer is empty*/
#define UART_LSR_TEMT  0x40 /* Both TX FIFO buffer & shift register are empty */
#define UART_LSR_ERROR 0x80 /* Rx FIFO buffer is error */

#define UART_USR_BUSY            0x01
#define UART_USR_TXFIFO_NOT_FULL 0x02
#define UART_USR_TXFIFO_EMPTY    0x04

#define REG_DLL_THR_RBR(hal) ((hal->uart_base) + ((0x00) << 2))
#define REG_DLH_IER(hal)     ((hal->uart_base) + ((0x02) << 2))
#define REG_IIR_FCR(hal)     ((hal->uart_base) + ((0x04) << 2))
#define REG_LCR(hal)         ((hal->uart_base) + ((0x06) << 2))
#define REG_MCR(hal)         ((hal->uart_base) + ((0x08) << 2))
#define REG_LSR(hal)         ((hal->uart_base) + ((0x0A) << 2))
#define REG_MSR(hal)         ((hal->uart_base) + ((0x0C) << 2))
#define REG_USR(hal)         ((hal->uart_base) + ((0x0E) << 2))
#define REG_TFL(hal)         ((hal->uart_base) + ((0x10) << 2))
#define REG_RFL(hal)         ((hal->uart_base) + ((0x12) << 2))
#define REG_RST(hal)         ((hal->uart_base) + ((0x14) << 2))

/* urdma ctrl */
#define URDMA_SW_RST     (1 << 0)
#define URDMA_DMA_MODE   (1 << 1)
#define URDMA_TX_DMA_EN  (1 << 2)
#define URDMA_RX_DMA_EN  (1 << 3)
#define URDMA_TX_ENDINA  (1 << 4)
#define URDMA_RX_ENDINA  (1 << 5)
#define URDMA_TX_SW_RST  (1 << 6)
#define URDMA_RX_SW_RST  (1 << 7)
#define URDMA_DMA2MIU_NS (1 << 8)
#define URDMA_RX_OP_MODE (1 << 11)
#define URDMA_TX_BUSY    (1 << 12)
#define URDMA_RX_BUSY    (1 << 13)

/* urdma int ctrl */
#define URDMA_RX_INTR_CLR      (1 << 0)
#define URDMA_RX_INTR1_EN      (1 << 1)
#define URDMA_RX_INTR2_EN      (1 << 2)
#define URDMA_RX_INTR1         (1 << 4)
#define URDMA_RX_INTR2         (1 << 5)
#define URDMA_RX_MCU_INTR      (1 << 7)
#define URDMA_TX_INTR_CLR      (1 << 8)
#define URDMA_TX_INTR_EN       (1 << 9)
#define URDMA_TX_EMPTY_INT_EN  (1 << 10)
#define URDMA_TX_EMPTY_INT_CLR (1 << 11)
#define URDMA_TX_EMPTY_INT     (1 << 14)
#define URDMA_TX_MCU_INTR      (1 << 15)

#define REG_URDMA_CTRL(hal)           ((hal->urdma_base) + ((0x00) << 2))
#define REG_URDMA_INTR_THRESHOLD(hal) ((hal->urdma_base) + ((0x01) << 2))
#define REG_URDMA_TXBUF_H(hal)        ((hal->urdma_base) + ((0x02) << 2))
#define REG_URDMA_TXBUF_L(hal)        ((hal->urdma_base) + ((0x03) << 2))
#define REG_URDMA_TXBUF_SIZE(hal)     ((hal->urdma_base) + ((0x04) << 2))
#define REG_URDMA_TXBUF_RPTR(hal)     ((hal->urdma_base) + ((0x05) << 2))
#define REG_URDMA_TXBUF_WPTR(hal)     ((hal->urdma_base) + ((0x06) << 2))
#define REG_URDMA_TX_TIMEOUT(hal)     ((hal->urdma_base) + ((0x07) << 2))
#define REG_URDMA_RXBUF_H(hal)        ((hal->urdma_base) + ((0x08) << 2))
#define REG_URDMA_RXBUF_L(hal)        ((hal->urdma_base) + ((0x09) << 2))
#define REG_URDMA_RXBUF_SIZE(hal)     ((hal->urdma_base) + ((0x0A) << 2))
#define REG_URDMA_RXBUF_WPTR(hal)     ((hal->urdma_base) + ((0x0B) << 2))
#define REG_URDMA_RX_TIMEOUT(hal)     ((hal->urdma_base) + ((0x0C) << 2))
#define REG_URDMA_INT_CTRL(hal)       ((hal->urdma_base) + ((0x0D) << 2))
#define REG_URDMA_DEBUG(hal)          ((hal->urdma_base) + ((0x0F) << 2))
#define REG_URDMA_2MIU_SEL(hal)       ((hal->urdma_base) + ((0x10) << 2))
#define REG_URDMA_TXBUF_MSB(hal)      ((hal->urdma_base) + ((0x14) << 2))
#define REG_URDMA_RXBUF_MSB(hal)      ((hal->urdma_base) + ((0x15) << 2))
#define REG_URDMA_MIU_ADDR0_SEL(hal)  ((hal->urdma_base) + ((0x16) << 2))

#define UART_READ(_reg_)         (*(volatile unsigned char *)(IO_ADDRESS(_reg_)))
#define UART_WRITE(_reg_, _val_) ((*(volatile unsigned char *)(IO_ADDRESS(_reg_))) = (unsigned char)(_val_))
#define UART_WRITE_MASK(_reg_, _val_, mask)             \
    ((*(volatile unsigned char *)(IO_ADDRESS(_reg_))) = \
         ((*(volatile unsigned char *)(IO_ADDRESS(_reg_))) & ~(mask)) | ((unsigned char)(_val_) & (mask)))
#define URDMA_READ(_reg_)         (*(volatile unsigned short *)(IO_ADDRESS(_reg_)))
#define URDMA_WRITE(_reg_, _val_) ((*(volatile unsigned short *)(IO_ADDRESS(_reg_))) = (unsigned short)(_val_))
#define URDMA_WRITE_MASK(_reg_, _val_, mask)             \
    ((*(volatile unsigned short *)(IO_ADDRESS(_reg_))) = \
         ((*(volatile unsigned short *)(IO_ADDRESS(_reg_))) & ~(mask)) | ((unsigned short)(_val_) & (mask)))
#define REG16_WRITE_MASK(_reg_, _val_, mask)             \
    ((*(volatile unsigned short *)(IO_ADDRESS(_reg_))) = \
         ((*(volatile unsigned short *)(IO_ADDRESS(_reg_))) & ~(mask)) | ((unsigned short)(_val_) & (mask)))

/* chip diff */
static u16 g_uart_bank[]                                     = CONFIG_UART_BANK;
static u32 g_digmux_reg[sizeof(g_uart_bank) / sizeof(u16)]   = CONFIG_DIGMUX_REG;
static u16 g_digmux_shift[sizeof(g_uart_bank) / sizeof(u16)] = CONFIG_DIGMUX_SHIFT;
static u16 g_digmux_mask[sizeof(g_uart_bank) / sizeof(u16)]  = CONFIG_DIGMUX_MASK;
static u16 g_digmux_sel[sizeof(g_uart_bank) / sizeof(u16)]   = CONFIG_DIGMUX_SEL;

u32 HAL_UART_circ_empty(u16 wptr, u16 rptr)
{
    return (wptr == rptr) ? 1 : 0;
}

u32 HAL_UART_circ_cnt_to_end(u16 wptr, u16 rptr, u32 size)
{
    u32 end = size - rptr;
    u32 n   = (wptr + end) & (size - 1);

    return ((n < end) ? n : end);
}

u32 HAL_UART_circ_space_end(u16 wptr, u16 rptr, u32 size)
{
    u32 end = size - 1 - wptr;
    u32 n   = (end + rptr) & (size - 1);

    return (n <= end ? n : end + 1);
}

static void HAL_UART_force_rx_disable(struct uart_hal *hal, u8 u8_status)
{
    if (hal->rx_pin)
    {
        if (u8_status)
        {
            MDrv_GPIO_Pad_Oen(hal->rx_pin);
        }
        else
        {
            MDrv_GPIO_Pad_Odn(hal->rx_pin);
        }
    }
    else if (hal->digmux != 0xFF)
    {
        REG16_WRITE_MASK(REG_FORCE_RX_DISABLE, (~u8_status << hal->digmux), (1 << hal->digmux));
    }
}

void HAL_UART_set_digmux(struct uart_hal *hal)
{
    u8 i;

    for (i = 0; i < (sizeof(g_uart_bank) / sizeof(u16)); i++)
    {
        if (g_uart_bank[i] == ((hal->uart_base & 0xFFFF00) >> 9) && (hal->digmux < (sizeof(g_uart_bank) / sizeof(u16))))
        {
            break;
        }
        else if (g_uart_bank[i] == ((hal->uart_base & 0xFFFF00) >> 9) && (hal->digmux == 0xFF))
        {
            hal->digmux = i;
            break;
        }
    }

    if (i != (sizeof(g_uart_bank) / sizeof(u16)))
    {
        REG16_WRITE_MASK(g_digmux_reg[hal->digmux], g_digmux_sel[i] << g_digmux_shift[hal->digmux],
                         g_digmux_mask[hal->digmux]);
    }
}

static u8 HAL_UART_clear_fifos(struct uart_hal *hal)
{
    unsigned int timeout = 0;

    while (((UART_READ(REG_USR(hal)) & UART_USR_BUSY)) && timeout < 2000)
    {
        UART_IMPL_USDELAY(2);
        UART_WRITE(REG_IIR_FCR(hal), UART_FCR_FIFO_ENABLE | UART_FCR_CLEAR_RCVR | UART_FCR_CLEAR_XMIT
                                         | ((hal->tx_fifo_level & 0x3) << 4) | ((hal->rx_fifo_level & 0x3) << 6));
        timeout++;
    }

    if (timeout == 2000)
        return ERR_UART_TIMEOUT;

    return ERR_UART_SUCCESS;
}

static u8 HAL_UART_reset(struct uart_hal *hal)
{
    u32 i;

    if (hal->urdma_en)
    {
        URDMA_WRITE_MASK(REG_URDMA_CTRL(hal), URDMA_DMA_MODE, URDMA_DMA_MODE);
        URDMA_WRITE_MASK(REG_URDMA_CTRL(hal), 0, URDMA_TX_DMA_EN);
        URDMA_WRITE_MASK(REG_URDMA_CTRL(hal), 0, URDMA_RX_DMA_EN);
        URDMA_WRITE_MASK(REG_URDMA_CTRL(hal), URDMA_TX_INTR_CLR, URDMA_TX_INTR_CLR);
        URDMA_WRITE_MASK(REG_URDMA_CTRL(hal), URDMA_RX_INTR_CLR, URDMA_RX_INTR_CLR);

        URDMA_WRITE_MASK(REG_URDMA_CTRL(hal), URDMA_SW_RST, URDMA_SW_RST);

        for (i = 0; ((URDMA_RX_BUSY | URDMA_TX_BUSY) & URDMA_READ(REG_URDMA_CTRL(hal))); i++)
        {
            if (0xFFFF == i)
            {
                return ERR_UART_FAILURE;
            }
        }

        URDMA_WRITE_MASK(REG_URDMA_CTRL(hal), 0, URDMA_SW_RST);
        URDMA_WRITE_MASK(REG_URDMA_CTRL(hal), 0, URDMA_DMA_MODE);
        URDMA_WRITE_MASK(REG_URDMA_CTRL(hal), URDMA_DMA_MODE, URDMA_DMA_MODE);

        URDMA_WRITE_MASK(REG_URDMA_CTRL(hal), 0, URDMA_TX_DMA_EN);
        URDMA_WRITE_MASK(REG_URDMA_CTRL(hal), URDMA_TX_SW_RST, URDMA_TX_SW_RST);

        for (i = 0; (URDMA_TX_BUSY & URDMA_READ(REG_URDMA_CTRL(hal))); i++)
        {
            if (0xFFFF == i)
            {
                return ERR_UART_FAILURE;
            }
        }

        URDMA_WRITE_MASK(REG_URDMA_CTRL(hal), 0, URDMA_TX_SW_RST);

        hal->tx_first = 1;

        URDMA_WRITE_MASK(REG_URDMA_CTRL(hal), 0, URDMA_RX_DMA_EN);
        URDMA_WRITE_MASK(REG_URDMA_CTRL(hal), URDMA_RX_SW_RST, URDMA_RX_SW_RST);

        for (i = 0; (URDMA_RX_BUSY & URDMA_READ(REG_URDMA_CTRL(hal))); i++)
        {
            if (0xFFFF == i)
            {
                return ERR_UART_FAILURE;
            }
        }

        URDMA_WRITE_MASK(REG_URDMA_CTRL(hal), 0, URDMA_RX_SW_RST);

        hal->rx_sw_rptr = 0;

        URDMA_WRITE_MASK(REG_URDMA_CTRL(hal), URDMA_TX_DMA_EN, URDMA_TX_DMA_EN);
        UART_WRITE_MASK(REG_URDMA_CTRL(hal), URDMA_RX_DMA_EN, URDMA_RX_DMA_EN);
    }

    return ERR_UART_SUCCESS;
}

void HAL_UART_init(struct uart_hal *hal)
{
    if (hal->urdma_en)
    {
        URDMA_WRITE_MASK(REG_URDMA_CTRL(hal), 0, URDMA_DMA_MODE);
    }

    HAL_UART_force_rx_disable(hal, 0);
    HAL_UART_clear_fifos(hal);

    if (hal->urdma_en)
    {
        URDMA_WRITE(REG_URDMA_TXBUF_H(hal), (hal->tx_urdma_base >> 16) & 0xFFFF);
        URDMA_WRITE(REG_URDMA_TXBUF_L(hal), hal->tx_urdma_base & 0xFFFF);
        URDMA_WRITE(REG_URDMA_TXBUF_MSB(hal), (hal->tx_urdma_base >> 32) & 0xF);
        URDMA_WRITE(REG_URDMA_TXBUF_SIZE(hal), hal->tx_urdma_size >> 3);
        URDMA_WRITE(REG_URDMA_TX_TIMEOUT(hal), URDMA_TX_TIMEOUT);
        URDMA_WRITE(REG_URDMA_TXBUF_WPTR(hal), 0x0);

        URDMA_WRITE(REG_URDMA_RXBUF_H(hal), (hal->rx_urdma_base >> 16) & 0xFFFF);
        URDMA_WRITE(REG_URDMA_RXBUF_L(hal), hal->rx_urdma_base & 0xFFFF);
        URDMA_WRITE(REG_URDMA_RXBUF_MSB(hal), (hal->rx_urdma_base >> 32) & 0xF);
        URDMA_WRITE(REG_URDMA_RXBUF_SIZE(hal), hal->rx_urdma_size >> 3);
        URDMA_WRITE(REG_URDMA_RX_TIMEOUT(hal), URDMA_RX_TIMEOUT);
        URDMA_WRITE(REG_URDMA_INTR_THRESHOLD(hal), URDMA_RX_INTR_LEVEL);
    }

    HAL_UART_reset(hal);
    HAL_UART_force_rx_disable(hal, 1);
}

void HAL_UART_deinit(struct uart_hal *hal)
{
    u32 i;

    HAL_UART_force_rx_disable(hal, 0);

    if (hal->urdma_en)
    {
        URDMA_WRITE_MASK(REG_URDMA_CTRL(hal), 0, URDMA_RX_DMA_EN);

        for (i = 0; (URDMA_RX_BUSY & URDMA_READ(REG_URDMA_CTRL(hal))); i++)
        {
            if (0xFFFF == i)
            {
                return;
            }
        }

        URDMA_WRITE_MASK(REG_URDMA_CTRL(hal), 0, URDMA_TX_DMA_EN);

        for (i = 0; (URDMA_TX_BUSY & URDMA_READ(REG_URDMA_CTRL(hal))); i++)
        {
            if (0xFFFF == i)
            {
                return;
            }
        }

        URDMA_WRITE_MASK(REG_URDMA_CTRL(hal), 0, URDMA_DMA_MODE);
    }
}

void HAL_UART_config(struct uart_hal *hal)
{
    u8 uartflag = 0;

    /*------- Configure Chararacter Size --------*/
    switch (hal->char_bits)
    {
        case 5: /* Word length is 5 bits */
            uartflag |= UART_LCR_WLEN5;
            break;
        case 6: /* Word length is 6 bits */
            uartflag |= UART_LCR_WLEN6;
            break;
        case 7: /* Word length is 7 bits */
            uartflag |= UART_LCR_WLEN7;
            break;
        case 8: /* Word length is 8 bits */
            uartflag |= UART_LCR_WLEN8;
            break;
        default:
            break;
    }

    /*------------ Configure Stop bit -----------*/
    if (hal->stop_bits == 2)
    {
        /* Stop length is 1.5 bits (5-bit char), 2 bits (otherwise) */
        uartflag |= UART_LCR_STOP2;
    }
    else
    {
        /* Stop length is 1 bit */
        uartflag |= UART_LCR_STOP1;
    }

    /*----------- Configure Parity --------------*/
    if (hal->parity_en)
    {
        /* Parity Enable */
        uartflag |= UART_LCR_PARITY_EN;

        /* Set Odd/Even Parity */
        if (!hal->even_parity_sel)
        {
            /* Odd Parity */
            uartflag &= (~UART_LCR_PARITY_SEL);
        }
        else
        {
            /* Even Parity */
            uartflag |= UART_LCR_PARITY_SEL;
        }
    }
    else
    {
        /* Parity Disable */
        uartflag &= (~UART_LCR_PARITY_EN);
    }

    if (hal->urdma_en)
    {
        URDMA_WRITE_MASK(REG_URDMA_CTRL(hal), 0, URDMA_DMA_MODE);
    }

    HAL_UART_force_rx_disable(hal, 0);
    // UART_WRITE_MASK(REG_MCR(hal), UART_MCR_LOOPBACK, UART_MCR_LOOPBACK);
    HAL_UART_clear_fifos(hal);

    UART_WRITE(REG_LCR(hal), uartflag);

    if (hal->divisor)
    {
        UART_WRITE_MASK(REG_LCR(hal), UART_LCR_DLAB, UART_LCR_DLAB);
        UART_WRITE(REG_DLH_IER(hal),
                   (u8)((hal->divisor >> 8) & 0xff)); // CAUTION: this causes IER being overwritten also
        UART_WRITE(REG_DLL_THR_RBR(hal), (u8)(hal->divisor & 0xff));
        UART_WRITE_MASK(REG_LCR(hal), 0, UART_LCR_DLAB);
    }

    UART_WRITE(REG_IIR_FCR(hal),
               UART_FCR_FIFO_ENABLE | ((hal->tx_fifo_level & 0x3) << 4) | ((hal->rx_fifo_level & 0x3) << 6));

    if (hal->rtscts_en)
    {
        // rts cts enable
        UART_WRITE_MASK(REG_MCR(hal), UART_MCR_AFCE | UART_MCR_RTS, UART_MCR_AFCE | UART_MCR_RTS);
    }
    else
    {
        // rts cts disable
        UART_WRITE_MASK(REG_MCR(hal), 0, UART_MCR_AFCE | UART_MCR_RTS);
    }

    // UART_WRITE_MASK(REG_MCR(hal), 0, UART_MCR_LOOPBACK);
    HAL_UART_reset(hal);
    HAL_UART_force_rx_disable(hal, 1);
}

void HAL_UART_irq_enable(struct uart_hal *hal, enum UART_IRQ_TYPE type, u8 u8_enable)
{
    if (hal->urdma_en && (type == UART_IRQ_URDMA_TX))
    {
        URDMA_WRITE_MASK(REG_URDMA_INT_CTRL(hal), u8_enable ? URDMA_TX_INTR_EN : 0, URDMA_TX_INTR_EN);
    }
    else if (hal->urdma_en && (type == UART_IRQ_URDMA_RX))
    {
        URDMA_WRITE_MASK(REG_URDMA_INT_CTRL(hal), u8_enable ? URDMA_RX_INTR1_EN : 0, URDMA_RX_INTR1_EN);
        URDMA_WRITE_MASK(REG_URDMA_INT_CTRL(hal), u8_enable ? URDMA_RX_INTR2_EN : 0, URDMA_RX_INTR2_EN);
    }
    else if (!hal->urdma_en && (type == UART_IRQ_TX))
    {
        if (u8_enable)
        {
            UART_WRITE_MASK(REG_DLH_IER(hal), UART_IER_THRI | UART_IER_PTHRI, UART_IER_THRI | UART_IER_PTHRI);
        }
        else
        {
            UART_WRITE_MASK(REG_DLH_IER(hal), 0, UART_IER_THRI | UART_IER_PTHRI);
        }

        hal->tx_int_en = u8_enable;
    }
    else if (!hal->urdma_en && (type == UART_IRQ_RX))
    {
        if (u8_enable)
        {
            UART_WRITE_MASK(REG_DLH_IER(hal), UART_IER_RDI | UART_IER_RLSI, UART_IER_RDI | UART_IER_RLSI);
        }
        else
        {
            UART_WRITE_MASK(REG_DLH_IER(hal), 0, UART_IER_RDI | UART_IER_RLSI);
        }
    }
#if SS_UART_TX_EMPTY_INTERRUPT
    else if (hal->urdma_base && (type == UART_IRQ_TX_EMPTY))
    {
        URDMA_WRITE_MASK(REG_URDMA_INT_CTRL(hal), u8_enable ? URDMA_TX_EMPTY_INT_EN : 0, URDMA_TX_EMPTY_INT_EN);
    }
#endif
}

u32 HAL_UART_get_irq_type(struct uart_hal *hal, enum UART_IRQ_TYPE type)
{
    u8  iir_fcr     = 0; /* Interrupt Identification Register (IIR) */
    u8  count       = 0;
    u8  retry       = 100;
    u32 u32_irqType = 0;

    if (hal->urdma_en && (type == UART_IRQ_URDMA))
    {
        if (URDMA_RX_MCU_INTR & URDMA_READ(REG_URDMA_INT_CTRL(hal)))
        {
            if (URDMA_RX_INTR1 & URDMA_READ(REG_URDMA_INT_CTRL(hal)))
            {
                u32_irqType |= UART_IRQ_URDMA_RX_TIMEOUT;
            }

            if (URDMA_RX_INTR2 & URDMA_READ(REG_URDMA_INT_CTRL(hal)))
            {
                u32_irqType |= UART_IRQ_URDMA_RX_THRESHOLD;
            }

            URDMA_WRITE_MASK(REG_URDMA_INT_CTRL(hal), URDMA_RX_INTR_CLR, URDMA_RX_INTR_CLR);
            u32_irqType |= UART_IRQ_URDMA_RX;
        }
        else if (URDMA_TX_MCU_INTR & URDMA_READ(REG_URDMA_INT_CTRL(hal)))
        {
            URDMA_WRITE_MASK(REG_URDMA_INT_CTRL(hal), URDMA_TX_INTR_CLR, URDMA_TX_INTR_CLR);
            u32_irqType |= UART_IRQ_URDMA_TX;
        }
    }
    else if (!hal->urdma_en && (type == UART_IRQ))
    {
        /* Read Interrupt Identification Register */
        iir_fcr = UART_READ(REG_IIR_FCR(hal)) & UART_IIR_ID_MASK;

        u32_irqType = UART_IRQ_UNKNOWN;

        if ((iir_fcr == UART_IIR_RDI || iir_fcr == UART_IIR_RX_TIMEOUT
             || iir_fcr == UART_IIR_RLSI)) /* Receive Data Available or Character timeout or Receiver line status */
        {
            u32_irqType = UART_IRQ_RX;
        }
        else if (iir_fcr == UART_IIR_THRI) /* Transmitter Holding Register Empty */
        {
            u32_irqType = UART_IRQ_TX;
        }
        else if (iir_fcr == UART_IIR_MSI) /* Modem Status */
        {
            // UART_ERR("UART Interrupt: Modem status\n");
            // Read MSR to clear
            UART_READ(REG_MSR(hal));
            u32_irqType = UART_IRQ_MODEM;
        }
        else if (iir_fcr == UART_IIR_BUSY) /* Busy detect indication */
        {
            // Read USR to clear
            UART_READ(REG_USR(hal));

            while (((UART_READ(REG_IIR_FCR(hal)) & UART_IIR_ID_MASK) == UART_IIR_BUSY) && (count < retry))
            {
                // Read USR to clear
                UART_READ(REG_USR(hal));
                count++;
            }
            if (count == retry)
                u32_irqType = UART_IRQ_BUSY;
        }
        else if (iir_fcr == UART_IIR_NO_INT) /* No pending interrupts */
        {
            u32_irqType = UART_IRQ_NO_INT;
        }
    }
#if SS_UART_TX_EMPTY_INTERRUPT
    else if (hal->urdma_base && (type & UART_IRQ_TX_EMPTY))
    {
        if (URDMA_TX_EMPTY_INT & URDMA_READ(REG_URDMA_INT_CTRL(hal)))
        {
            URDMA_WRITE_MASK(REG_URDMA_INT_CTRL(hal), URDMA_TX_EMPTY_INT_CLR, URDMA_TX_EMPTY_INT_CLR);
            u32_irqType = UART_IRQ_TX_EMPTY_INT;
        }
    }
#endif

    return u32_irqType;
}

u32 HAL_UART_get_fifo_status(struct uart_hal *hal)
{
    u8  lsr            = 0;
    u32 u32_fifoStatus = 0;

    if (!hal->urdma_en)
    {
        lsr = UART_READ(REG_LSR(hal));

        if (lsr & UART_LSR_DR)
        {
            u32_fifoStatus |= UART_FIFO_RX_READY;
        }

        if (lsr & UART_LSR_OE)
        {
            u32_fifoStatus |= UART_FIFO_RX_OE;
        }

        if (lsr & UART_LSR_PE)
        {
            u32_fifoStatus |= UART_FIFO_RX_PE;
        }

        if (lsr & UART_LSR_FE)
        {
            u32_fifoStatus |= UART_FIFO_RX_FE;
        }

        if (lsr & UART_LSR_BI)
        {
            u32_fifoStatus |= UART_FIFO_BI;
        }

        if (lsr & UART_LSR_TEMT)
        {
            u32_fifoStatus |= UART_FIFO_TX_SHIFT_EMPTY;
        }

        if (lsr & UART_LSR_THRE)
        {
            u32_fifoStatus |= UART_FIFO_TX_EMPTY;
        }

        if (lsr & UART_LSR_ERROR)
        {
            u32_fifoStatus |= UART_FIFO_RX_ERROR;
        }
    }
    else
    {
        if (URDMA_READ(REG_URDMA_TXBUF_RPTR(hal)) == URDMA_READ(REG_URDMA_TXBUF_WPTR(hal)))
        {
            u32_fifoStatus |= UART_FIFO_TX_EMPTY;
        }
    }

    return u32_fifoStatus;
}

u32 HAL_UART_write(struct uart_hal *hal, u8 *buf, u32 size)
{
    u32 space      = 0;
    u32 count      = 0;
    u16 sw_tx_wptr = 0;
    u16 sw_tx_rptr = 0;

    if (hal->urdma_en)
    {
        // sw_tx_wptr = hal->urdma_base->tx_buf_wptr;
        // sw_tx_rptr = hal->urdma_base->tx_buf_rptr;
        sw_tx_wptr = URDMA_READ(REG_URDMA_TXBUF_WPTR(hal));
        sw_tx_rptr = URDMA_READ(REG_URDMA_TXBUF_RPTR(hal));

        if (sw_tx_wptr == 0 && sw_tx_rptr == 0 && hal->tx_first)
        {
            hal->tx_first = 0;
        }
        else
        {
            sw_tx_wptr = (sw_tx_wptr + 1) & (hal->tx_urdma_size - 1);
        }

        do
        {
            space = HAL_UART_circ_space_end(sw_tx_wptr, sw_tx_rptr, hal->tx_urdma_size);

            if (size < space)
                space = size;

            if (space <= 0)
                break;

            memcpy(hal->tx_buf + sw_tx_wptr, buf, space);
            sw_tx_wptr = (sw_tx_wptr + space) & (hal->tx_urdma_size - 1);
            buf += space;
            size -= space;
            count += space;
        } while (size);

        if (count)
        {
            CamOsMemFlush(hal->tx_buf, hal->tx_urdma_size);
            CamOsMiuPipeFlush();
            URDMA_WRITE(REG_URDMA_TXBUF_WPTR(hal), (sw_tx_wptr - 1));
        }
    }
    else
    {
        do
        {
            if ((UART_READ(REG_USR(hal)) & UART_USR_TXFIFO_NOT_FULL) != UART_USR_TXFIFO_NOT_FULL)
            {
                break;
            }

            UART_WRITE(REG_DLL_THR_RBR(hal), *buf);
            count++;
            buf++;
        } while (--size);
    }

    return count;
}

u32 HAL_UART_read(struct uart_hal *hal, u8 *buf, u32 size)
{
    u32 cnt        = 0;
    u32 count      = 0;
    u16 sw_rx_wptr = 0;
    u16 sw_rx_rptr = 0;

    if (hal->urdma_en)
    {
        // sw_rx_wptr  = hal->urdma_base->rx_buf_wptr;
        hal->rx_buf_wptr = sw_rx_wptr = (URDMA_READ(REG_URDMA_RXBUF_WPTR(hal)) + 1) & (hal->tx_urdma_size - 1);
        hal->rx_buf_rptr = sw_rx_rptr = hal->rx_sw_rptr;
        hal->rx_sw_rptr               = sw_rx_wptr;

        CamOsMemInvalidate((void *)hal->rx_buf, hal->tx_urdma_size);

        while (buf)
        {
            cnt = HAL_UART_circ_cnt_to_end(sw_rx_wptr, sw_rx_rptr, hal->tx_urdma_size);

            if (size < cnt)
                cnt = size;

            if (cnt <= 0)
                break;

            memcpy(buf, hal->rx_buf + sw_rx_rptr, cnt);
            hal->rx_sw_rptr = sw_rx_rptr = (sw_rx_rptr + cnt) & (hal->tx_urdma_size - 1);
            buf += cnt;
            size -= cnt;
            count += cnt;
        }
    }
    else
    {
        do
        {
            /* check if Receiver Data Ready */
            if ((UART_READ(REG_LSR(hal)) & UART_LSR_DR) != UART_LSR_DR)
            {
                break;
            }

            *buf = UART_READ(REG_DLL_THR_RBR(hal));
            count++;
            buf++;
        } while (--size);
    }

    return count;
}
