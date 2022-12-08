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

/***************************************************************************
 *  kernel_spi.h
 *--------------------------------------------------------------------------
 *  Scope: SPI related definitions
 *
 ****************************************************************************/

#ifndef __KERNEL_SPI_H__
#define __KERNEL_SPI_H__

#include "vm_types.ht"
#include "cpu_mem_map.hc"

/****************************************************************************/
/*        SPI registers                           */
/****************************************************************************/
/************************************************/
/* SPI registers definition                     */
/************************************************/

typedef struct OSpi_s
{
    u32 Config;
    u32 Control;
    u32 NWord;
    u32 RxTx;
    u32 Status;
    u32 Spare;
    u32 InterruptMask;
    u32 InterruptStatus;
    u32 InterruptClear;
    u32 RawInterruptStatus;
} OSpi_t;

/* configuration register */
#define SPI_DUPLEX_MODE_FULL        ((u32)1 << 31)  /* mode half/full duplex */
#define SPI_DUPLEX_MODE_HALF        ((u32)0 << 31)
#define SPI_APDDIS                  ((u32)1 << 30)  /* Auto Power Down Disable */
#define SPI_TFR_MODE_DMA            ((u32)1 << 29)  /* transfer mode */
#define SPI_TFR_MODE_CPU            ((u32)0 << 29)
#define SPI_BIG_ENDIAN              ((u32)1 << 28)  /* endian alignment */
#define SPI_LITTLE_ENDIAN           ((u32)0 << 28)
#define SPI_DBL_MODE                ((u32)1 << 27)  /* double byte mode */
#define SPI_MLOUT                   ((u32)1 << 26)  /* MSB or LSB first transmitted data */
#define SPI_MLIN                    ((u32)1 << 25)  /* MSB or LSB first received data */
#define SPI_SSL_DST_SSL0            ((u32)0 << 24)  /* Slave Select Destination*/
#define SPI_SSL_DST_SSL1            ((u32)1 << 24)  /* Slave Select Destination*/
#define SPI_SSL_MODE_SW             ((u32)0 << 23)  /* Slave Select Mode software/hardware */
#define SPI_SSL_MODE_HW             ((u32)1 << 23)  /* Slave Select Mode software/hardware */
#define SPI_SSL_POL_LOW             ((u32)0 << 22)  /* Slave Select Polarity low */
#define SPI_SSL_POL_HIGH            ((u32)1 << 22)  /* Slave Select Polarity high */
/* inter word mode determines the ssl & sclk level between 2 consecutive words */
/* 0 SSL is keep active / SCLK is shut down */
/* 1 SSL becomes inactive / SCLK is remained */
/* 2 SSL becomes inactive / SCLK is shut down */
#define SPI_ITR_MODE_POS            20
#define SPI_ITR_MODE_MASK           ((u32)3 << SPI_ITR_MODE_POS)

#define SPI_ITR_DEL_POS             16              /* inter word delay */
#define SPI_ITR_DEL_MASK            ((u32)0x0F << SPI_ITR_DEL_POS)
#define SPI_MODE_MASTER             ((u32)0 << 15)  /* spi mode */
#define SPI_MODE_SLAVE              ((u32)1 << 15)  /* spi mode */
#define SPI_BR_POS                  12              /* data byte rate */
#define SPI_BR_MASK                 ((u32)0x07 << SPI_BR_POS)

/*  Stop Clock Latency Indicates the number of SCLK advance or offset periods related to SSL starting*/
#define SPI_SPCL_POS                8
#define SPI_SPCL_MASK	              ((u32)0x0F << SPI_SPCL_POS) 

#define SPI_STCL_POS                4               /*  Start Clock Latency */
#define SPI_STCL_MASK               ((u32)0x0F << SPI_STCL_POS) 
#define SPI_CSPPOS                  ((u32)1 << 3)		/* clock stop  */
#define SPI_CSTPOS                  ((u32)1 << 2)		/* clock start */
#define SPI_CPHA                    ((u32)1 << 1)		/* clock phase */
#define SPI_CPOL                    ((u32)1 << 0)		/* clock polarity */

/* control register */
#define SPI_DTL_POS                 3                             /* data transfer length */
#define SPI_DTL_MASK                ((u32)0x0F << SPI_DTL_POS)    /* data transfer length */
#define SPI_CYCLE_STOP              ((u32)1 << 2 )                /* cycle stop */
#define SPI_READ                    ((u32)1 << 1)                 /* read transfer mode */
#define SPI_WRITE                   ((u32)1 << 0)                 /* write  transfer mode */
#define SPI_SSL_INACT_POL_EN        ((u32)1 << 8)
#define SPI_SSL0_INACT_POL_HIGH     ((u32)1 << 9)
#define SPI_SSL1_INACT_POL_HIGH     ((u32)1 << 10)

/* NWORD register */
#define SPI_RX_NWORD_POS            16                            /* number of word to receive */
#define SPI_RX_NWORD_MASK           (0x7FFF << SPI_RX_NWORD_POS)  /* number of word to receive */
#define SPI_TX_NWORD_POS            0                             /* number of word to transmit */
#define SPI_TX_NWORD_MASK           (0x7FFF << SPI_TX_NWORD_POS)  /* number of word to transmit */

/* SPI Status*/
#define SPI_SPTE       (1 << 1)     /* Transmit register empty */
#define SPI_SPRF       (1 << 0)     /* Receiver register full */

/* raw interrupt status */
#define SPI_RAW_WAKEUP (1 << 11)    /* wake up required */
#define SPI_RAW_END    (1 << 10)    /* spi cycle end */
#define SPI_RAW_SSLSP  (1 << 9)     /* ssl stop detect */
#define SPI_RAW_SSLST  (1 << 8)     /* ssl start detect */
#define SPI_RAW_TX_OVR (1 << 7)     /* Tx Overrun */
#define SPI_RAW_RX_OVR (1 << 6)     /* Rx Overrun */
#define SPI_RAW_TX_OVF (1 << 5)     /* Tx Overflow */
#define SPI_RAW_RX_OVF (1 << 4)     /* Tx Overflow */
#define SPI_RAW_TX_NWC (1 << 3)     /* Tx N word completed */
#define SPI_RAW_RX_NWC (1 << 2)     /* Rx N word completed */
#define SPI_RAW_SPTE   (1 << 1)     /* Transmitter Register Empty */
#define SPI_RAW_SPRF   (1 << 0)     /* Receiver Register Full */

/* interrupt mask */
#define SPI_IMR_WAKEUP (1 << 11)    /* wake up required */
#define SPI_IMR_END    (1 << 10)    /* spi cycle end */
#define SPI_IMR_SSLSP  (1 << 9)     /* ssl stop detect */
#define SPI_IMR_SSLST  (1 << 8)     /* ssl start detect */
#define SPI_IMR_TX_OVR (1 << 7)     /* Tx Overrun */
#define SPI_IMR_RX_OVR (1 << 6)     /* Rx Overrun */
#define SPI_IMR_TX_OVF (1 << 5)     /* Tx Overflow */
#define SPI_IMR_RX_OVF (1 << 4)     /* Tx Overflow */
#define SPI_IMR_TX_NWC (1 << 3)     /* Tx N word completed */
#define SPI_IMR_RX_NWC (1 << 2)     /* Rx N word completed */
#define SPI_IMR_SPTE   (1 << 1)     /* Transmitter Register Empty */
#define SPI_IMR_SPRF   (1 << 0)     /* Receiver Register Full */

/* interrupt status */
#define SPI_ISR_WAKEUP (1 << 11)    /* wake up required */
#define SPI_ISR_END    (1 << 10)    /* spi cycle end */
#define SPI_ISR_SSLSP  (1 << 9)     /* ssl stop detect */
#define SPI_ISR_SSLST  (1 << 8)     /* ssl start detect */
#define SPI_ISR_TX_OVR (1 << 7)     /* Tx Overrun */
#define SPI_ISR_RX_OVR (1 << 6)     /* Rx Overrun */
#define SPI_ISR_TX_OVF (1 << 5)     /* Tx Overflow */
#define SPI_ISR_RX_OVF (1 << 4)     /* Tx Overflow */
#define SPI_ISR_TX_NWC (1 << 3)     /* Tx N word completed */
#define SPI_ISR_RX_NWC (1 << 2)     /* Rx N word completed */
#define SPI_ISR_SPTE   (1 << 1)     /* Transmitter Register Empty */
#define SPI_ISR_SPRF   (1 << 0)     /* Receiver Register Full */

/* interrupt clear */
#define SPI_ICR_WAKEUP (1 << 11)    /* wake up required */
#define SPI_ICR_END    (1 << 10)    /* spi cycle end */
#define SPI_ICR_SSLSP  (1 << 9)     /* ssl stop detect */
#define SPI_ICR_SSLST  (1 << 8)     /* ssl start detect */
#define SPI_ICR_TX_OVR (1 << 7)     /* Tx Overrun */
#define SPI_ICR_RX_OVR (1 << 6)     /* Rx Overrun */
#define SPI_ICR_TX_OVF (1 << 5)     /* Tx Overflow */
#define SPI_ICR_RX_OVF (1 << 4)     /* Tx Overflow */
#define SPI_ICR_TX_NWC (1 << 3)     /* Tx N word completed */
#define SPI_ICR_RX_NWC (1 << 2)     /* Rx N word completed */
#define SPI_ICR_SPTE   (1 << 1)     /* Transmitter Register Empty */
#define SPI_ICR_SPRF   (1 << 0)     /* Receiver Register Full */

extern volatile OSpi_t* const g_ptOSpi1;

#endif // __KERNEL_SPI_H__

