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

#ifndef _DRV_MSPI_H_
#define _DRV_MSPI_H_

#include "mdrv_types.h"

enum SPI_RET
{
    SPI_SUCCESS,
    SPI_INVALID_BUS,
    SPI_INVALID_MODE,
    SPI_INVALID_SPEED,
    SPI_INVALID_PADMUX,
    SPI_INVALID_TRF_MODE,
    SPI_INVALID_WORD_BITS,
    SPI_INVALID_EXT_CS_NUM,
    SPI_INVALID_TRANSFER_LENGTH,
    SPI_DMA_READ_ERROR,
    SPI_DMA_WRITE_ERROR,
    SPI_BUFFER_READ_ERROR,
    SPI_BUFFER_WRITE_ERROR,
    SPI_PADMUX_SET_ERROR,
    SPI_SYSDESC_REG_ERROR,
    SPI_SYSDESC_INTERRUPT_ERROR,
};

struct spi_tfr
{
  const u8 * tx_buf;
  u8 *       rx_buf;
  u32        len;
  u8         cs_change;
  u16        delay_usecs;
  u8         bits_per_word;
};

struct spi_setup
{
    u8  bus_num;
    u32 max_speed_hz;
    u8  chip_select;
    u32 mode;
#define SPI_CPHA        0x01            /* clock phase */
#define SPI_CPOL        0x02            /* clock polarity */
#define SPI_MODE_0      (0|0)           /* (original MicroWire) */
#define SPI_MODE_1      (0|SPI_CPHA)
#define SPI_MODE_2      (SPI_CPOL|0)
#define SPI_MODE_3      (SPI_CPOL|SPI_CPHA)
#define SPI_CS_HIGH     0x04            /* chipselect active high? */
#define SPI_LSB_FIRST   0x08            /* per-word bits-on-wire */
#define SPI_3WIRE       0x10            /* SI/SO signals shared */
#define SPI_LOOP        0x20            /* loopback mode */
#define SPI_NO_CS       0x40            /* 1 dev/bus, no chipselect */
#define SPI_READY       0x80            /* slave pulls low to pause */
#define SPI_TX_DUAL     0x100           /* transmit with 2 wires */
#define SPI_TX_QUAD     0x200           /* transmit with 4 wires */
#define SPI_RX_DUAL     0x400           /* receive with 2 wires */
#define SPI_RX_QUAD     0x800           /* receive with 4 wires */
#define SPI_CS_WORD     0x1000          /* toggle cs after each word */
#define SPI_TX_OCTAL    0x2000          /* transmit with 8 wires */
#define SPI_RX_OCTAL    0x4000          /* receive with 8 wires */
#define SPI_3WIRE_HIZ   0x8000          /* high impedance turnaround */
};

extern s32 spi_ctrl_init(u8 group);
extern s32 spi_setup(struct spi_setup *setup);
extern s32 spi_transfer(struct spi_setup *setup, struct spi_tfr * tfr, u32 length);

#endif

