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

#include <cam_os_wrapper.h>
#include <ms_platform.h>
#include <registers.h>
#include <drv_uart.h>
#include <hal_uart.h>
#include <irqs.h>
#if defined(CONFIG_PADMUX_SUPPORT) && defined(CONFIG_GPIO_SUPPORT)
#include <mdrv_gpio.h>
#endif
#if defined(CONFIG_CAM_CLK)
#include <drv_camclk_Api.h>
#include <camclk.h>
#endif
#if defined(CONFIG_SYSDESC_SUPPORT)
#include <drv_sysdesc.h>
#endif
#include <hal_uart_cfg.h>


#define SS_UART_DEBUG 0

#if SS_UART_DEBUG
#define uart_dbg(fmt, arg...) CamOsPrintf(KERN_INFO fmt, ##arg)
#else
#define uart_dbg(fmt, arg...)
#endif
#define uart_err(fmt, arg...) CamOsPrintf(KERN_ERR fmt, ##arg)

#define UART_RS485_SUPPORT  0

struct uart_port {
    CamOsSpinlock_t lock;          /* port lock */
    //unsigned long       iobase;         /* in/out[bwl] */
    //unsigned char    *membase;       /* read/write[bwl] */
    //unsigned char        irq;            /* irq number */
    unsigned int        uartclk;        /* base uart clock */
    unsigned int        fifosize;       /* tx fifo size */
};

struct sstar_uart
{
    struct uart_port port;
    struct uart_hal  hal;
#ifdef CONFIG_CAMCLK_SUPPORT
    CAMCLK_Get_Attribute stCfg;
    void *               pvclk;
    u16                  clkid;
#else
    struct clk_info *spi_src_clk;
    struct reg_t *reg_clkgen;
#endif

#if SS_UART_8250_BUG_THRE
    struct timer_list timer; /* "no irq" timer */
#endif

    char               rs485_task_name[32];
    CamOsThread        rs485_task;
    CamOsThreadAttrb_t rs485_tAttr;

    char               urdma_task_name[32];
    CamOsThread        urdma_task;
    CamOsThreadAttrb_t urdma_tAttr;

    char uart_irq_name[32];
    char urdma_irq_name[32];
    char tx_empty_irq_name[32];
    u32  uart_irq;
    u32  urdma_irq;
    u32  tx_empty_irq;

    u8  sctp_en;
    u8  urdma_en;
    u8  needdigmux;
    u16 indexgpio;
    u32 padmode;

    u32 rx_guard;
    u32 tx_silent;
    u32 tx_disable;
    u32 pad_disable;

    u8 uartName[16];
    u8 fifoInit;
    u8 isConsole;
    u8 disable;

    u8 *ptxbuf;
    u8 *prxbuf;
    u32 txbuf_size;
    u32 rxbuf_size;
    u32 txwrptr;
    u32 txrdptr;
    u32 rxwrptr;
    u32 rxrdptr;

    u8 tx_start;
    CamOsMutex_t tx_lock;
    CamOsTsem_t tx_signal;
    u32 is_rx_blocked;
    CamOsMutex_t rx_lock;
    CamOsTsem_t rx_signal;
};

static struct sstar_uart *ss_uart[NR_UART_PORTS] = {0};
static struct sstar_uart ss_uart_console;

static void ss_uart_stop_tx(struct sstar_uart *sstar_uart);
static void ss_uart_start_tx(struct sstar_uart *sstar_uart);
static int _DrvUartWriteTXBuf(struct sstar_uart *sstar_uart, u8 *buf,  u32 len);
static int _DrvUartReadRXBuf(struct sstar_uart *sstar_uart, u8 *buf,  u32 len);
static void _DrvUartRxInsertChar(struct sstar_uart *sstar_uart, u8 ch);


#if UART_RS485_SUPPORT
static int ss_uart_rs485_gpio(struct serial_rs485 *rs485, bool send)
{
    uart_dbg("delay:%d,%d\n", rs485->delay_rts_before_send, rs485->delay_rts_before_send);
    if ((rs485->flags & SER_RS485_ENABLED) && send)
    {
        if (rs485->delay_rts_before_send)
            udelay(rs485->delay_rts_before_send);
        if (rs485->flags & SER_RS485_RTS_ON_SEND)
        {
            gpio_set_value(rs485->padding[0], 1);
            uart_dbg("wend set higt\n");
        }
        else
        {
            gpio_set_value(rs485->padding[0], 0);
            uart_dbg("send set low\n");
        }
    }
    else if ((rs485->flags & SER_RS485_ENABLED) && !send)
    {
        if (rs485->delay_rts_after_send)
            udelay(rs485->delay_rts_after_send);
        if (rs485->flags & SER_RS485_RTS_AFTER_SEND)
        {
            gpio_set_value(rs485->padding[0], 1);
            uart_dbg("after set higt\n");
        }
        else
        {
            gpio_set_value(rs485->padding[0], 0);
            uart_dbg("after set low\n");
        }
    }
    return 0;
}

static int ss_uart_rs485_config(struct uart_port *port, struct serial_rs485 *rs485)
{
    struct sstar_uart *sstar_uart = (struct sstar_uart *)port->dev->driver_data;

    if (rs485->flags & SER_RS485_ENABLED)
    {
        uart_dbg("uart %d set 485 on,pin RTS:%d\n", port->line, rs485->padding[0]);
        if (gpio_request(rs485->padding[0], "gpio_irq_test") < 0)
        {
            uart_dbg("request gpio[%d] failed...\n", rs485->padding[0]);
            return -EFAULT;
        }
        if (rs485->flags & SER_RS485_RTS_ON_SEND)
        {
            if (gpio_direction_output(rs485->padding[0], 0) < 0)
            {
                uart_dbg("gpio_direction_output low[%d] failed...\n", rs485->padding[0]);
                return -EFAULT;
            }
        }
        else
        {
            if (gpio_direction_output(rs485->padding[0], 1) < 0)
            {
                uart_dbg("gpio_direction_output high[%d] failed...\n", rs485->padding[0]);
                return -EFAULT;
            }
        }
        uart_dbg("rs485->flags=0x%x\n", rs485->flags);

        if (sstar_uart->tx_empty_irq)
        {
            HAL_UART_irq_enable(&sstar_uart->hal, UART_IRQ_TX_EMPTY, 1);
        }
    }
    else
    {
        uart_dbg("uart %d set 485 off\n", port->line);
        gpio_free(rs485->padding[0]);
        if (sstar_uart->tx_empty_irq)
        {
            HAL_UART_irq_enable(&sstar_uart->hal, UART_IRQ_TX_EMPTY, 0);
        }
    }

    memcpy(&port->rs485, rs485, sizeof(*rs485));
    return 0;
}

static void rs485_Task(void *pUserdata)
{
    struct uart_port * p          = (struct uart_port *)pUserdata;
    struct sstar_uart *sstar_uart = (struct sstar_uart *)p->dev->driver_data;

    while (1)
    {
        CamOsThreadSchedule(1, CAM_OS_MAX_TIMEOUT);

        if (CAM_OS_OK == CamOsThreadShouldStop())
        {
            break;
        }

        while (!(UART_FIFO_TX_SHIFT_EMPTY & HAL_UART_get_fifo_status(&sstar_uart->hal)))
            ;

        ss_uart_rs485_gpio(&p->rs485, false);
    }
}

static void ss_tx_empty_interrupt(u32 irq, void *dev_id)
{
    struct uart_port * p           = dev_id;
    struct sstar_uart *sstar_uart  = (struct sstar_uart *)p->dev->driver_data;
    struct circ_buf *  xmit        = &p->state->xmit;
    u32                u32_irqType = 0;
    unsigned long      flags;

    spin_lock_irqsave(&p->lock, flags);

    u32_irqType = HAL_UART_get_irq_type(&sstar_uart->hal, UART_IRQ_TX_EMPTY);

    if (u32_irqType == UART_IRQ_TX_EMPTY_INT)
    {
        if (uart_circ_empty(xmit) || uart_tx_stopped(p))
        {
            ss_uart_rs485_gpio(&p->rs485, false);
        }
    }

    spin_unlock_irqrestore(&p->lock, flags);

    return IRQ_HANDLED;
}
#endif

static void ss_uart_check_rx_disable(struct sstar_uart *sstar_uart, u8 *buf, int count)
{
    int                i;

    if (!sstar_uart->isConsole)
        return;

    for (i = 0; i < count; i++)
    {
        if ('1' == buf[i])
        {
            sstar_uart->tx_disable++;
        }
        else
        {
            sstar_uart->tx_disable = 0;
        }

        if (5 == sstar_uart->tx_disable)
        {
            if (1 - sstar_uart->tx_silent)
            {
                uart_err("disable uart\n");
            }

            sstar_uart->tx_silent = 1 - sstar_uart->tx_silent;

            if (0 == sstar_uart->tx_silent)
            {
                uart_err("enable uart\n");
            }

            sstar_uart->tx_disable = 0;
        }

        if ('2' == buf[i])
        {
            sstar_uart->pad_disable++;
        }
        else
        {
            sstar_uart->pad_disable = 0;
        }

        if (5 == sstar_uart->pad_disable)
        {
            uart_err("uart pad disable\n");
            OUTREG16(0x1F203D4C, 0x000F);
        }
    }
}

static void ss_getchar(struct sstar_uart *sstar_uart)
{
    u32                u32_fifoStatus;
    u32                ch         = 0; /* Character read from UART port */


    u32_fifoStatus = HAL_UART_get_fifo_status(&sstar_uart->hal);

    /* check if Receiver Data Ready */
    if ((u32_fifoStatus & UART_FIFO_RX_READY) != UART_FIFO_RX_READY)
    {
        sstar_uart->rx_guard++;
        if (unlikely(sstar_uart->rx_guard > 2000))
        {
            HAL_UART_deinit(&sstar_uart->hal);
            HAL_UART_init(&sstar_uart->hal);
            uart_err("rx interrupts error!!!!!");
            sstar_uart->rx_guard = 0;
        }
        return;
    }
    sstar_uart->rx_guard = 0;

    /* while data ready, start to read data from UART FIFO */
    do
    {
        /* read data from UART IP */
        HAL_UART_read(&sstar_uart->hal, (u8 *)&ch, 1);

        _DrvUartRxInsertChar(sstar_uart, ch);

        ss_uart_check_rx_disable(sstar_uart, (u8 *)(&ch), 1);

        u32_fifoStatus = HAL_UART_get_fifo_status(&sstar_uart->hal);
    } while (u32_fifoStatus & UART_FIFO_RX_READY);

    if (sstar_uart->is_rx_blocked)
    {
        CamOsTsemUp(&sstar_uart->rx_signal);
    }
}

static void ss_putchar(struct sstar_uart *sstar_uart)
{
    int                count;
    u32                u32_fifoStatus;

    if (HAL_UART_circ_empty(sstar_uart->txwrptr, sstar_uart->txrdptr))
    {
        ss_uart_stop_tx(sstar_uart);
        return;
    }

    u32_fifoStatus = HAL_UART_get_fifo_status(&sstar_uart->hal);

    if (UART_FIFO_TX_EMPTY == (u32_fifoStatus & (UART_FIFO_TX_EMPTY))) // Tx FIFO Empty
    {
        count = sstar_uart->port.fifosize;
    }
    else if (UART_FIFO_TX_NOT_FULL == (u32_fifoStatus & (UART_FIFO_TX_NOT_FULL))) // not empty, but not full
    {
        count = 1;
    }
    else
    {
        count = 1;
    }

    do
    {
        HAL_UART_write(&sstar_uart->hal, &(sstar_uart->ptxbuf[sstar_uart->txrdptr]), 1);

        sstar_uart->txrdptr = (sstar_uart->txrdptr + 1) & (sstar_uart->txbuf_size - 1);

        if (sstar_uart->txrdptr == sstar_uart->txwrptr)
        {
            break;
        }
    } while (--count > 0);

    if (HAL_UART_circ_empty(sstar_uart->txwrptr, sstar_uart->txrdptr))
    {
        ss_uart_stop_tx(sstar_uart);
    }
}

static void ss_uart_interrupt(u32 irq, void *dev_id)
{
    /* Define Local Variables */
    struct sstar_uart *sstar_uart = (struct sstar_uart *)dev_id;
    u32                u32_irqType;

    CamOsSpinLockIrqSave(&sstar_uart->port.lock);

    u32_irqType = HAL_UART_get_irq_type(&sstar_uart->hal, UART_IRQ);

    if (u32_irqType == UART_IRQ_RX) /* Receive Data Available or Character timeout or Receiver line status */
    {
        ss_getchar(sstar_uart);
    }
    else if (u32_irqType == UART_IRQ_TX) /* Transmitter Holding Register Empty */
    {
        if (sstar_uart->tx_silent == 0)
            ss_putchar(sstar_uart);
    }

    CamOsSpinUnlockIrqRestore(&sstar_uart->port.lock);
}

static void ss_urdma_putchar(struct sstar_uart *sstar_uart)
{
    u16                circ_buf_out_size = HAL_UART_circ_cnt_to_end(sstar_uart->txwrptr, sstar_uart->txrdptr, sstar_uart->txbuf_size);
    int                count             = 0;

    if (circ_buf_out_size)
    {
        count = HAL_UART_write(&sstar_uart->hal, &sstar_uart->ptxbuf[sstar_uart->txrdptr], circ_buf_out_size);
        sstar_uart->txrdptr = (sstar_uart->txrdptr + count) & (sstar_uart->txbuf_size - 1);
    }

    return;
}

static void ss_urdma_getchar(struct sstar_uart *sstar_uart)
{
    u32 cnt = 0;

    HAL_UART_read(&sstar_uart->hal, NULL, 0);

    CamOsSpinLockIrqSave(&sstar_uart->port.lock);

    if (HAL_UART_circ_cnt_to_end(sstar_uart->rxwrptr, sstar_uart->rxrdptr, sstar_uart->rxbuf_size) > URDMA_RX_INTR_LEVEL)
    {
        sstar_uart->rxwrptr = sstar_uart->hal.rx_buf_wptr;
        sstar_uart->rxrdptr = sstar_uart->hal.rx_buf_wptr;
    }
    else
    {
        sstar_uart->rxwrptr = sstar_uart->hal.rx_buf_wptr;
    }

    while (sstar_uart)
    {
        cnt = HAL_UART_circ_cnt_to_end(sstar_uart->hal.rx_buf_wptr, sstar_uart->hal.rx_buf_rptr, sstar_uart->hal.rx_urdma_size);

        if (cnt <= 0)
            break;

        ss_uart_check_rx_disable(sstar_uart, &sstar_uart->hal.rx_buf[sstar_uart->hal.rx_buf_rptr], cnt);

        sstar_uart->hal.rx_buf_rptr = (sstar_uart->hal.rx_buf_rptr + cnt) & (sstar_uart->hal.rx_urdma_size - 1);
    }

    if (sstar_uart->is_rx_blocked)
    {
        CamOsTsemUp(&sstar_uart->rx_signal);
    }

    CamOsSpinUnlockIrqRestore(&sstar_uart->port.lock);

    return;
}

static void urdma_Task(void *pUserdata)
{
    struct sstar_uart *sstar_uart = (struct sstar_uart *)pUserdata;

    while (1)
    {
        CamOsThreadSchedule(1, CAM_OS_MAX_TIMEOUT);

        if (CAM_OS_OK == CamOsThreadShouldStop())
        {
            break;
        }

        CamOsSpinLockIrqSave(&sstar_uart->port.lock);

        if (HAL_UART_circ_empty(sstar_uart->txwrptr, sstar_uart->txrdptr))
        {
            ss_uart_stop_tx(sstar_uart);
        }
        else
        {
            HAL_UART_irq_enable(&sstar_uart->hal, UART_IRQ_URDMA_TX, 1);
        }

        if (!HAL_UART_circ_empty(sstar_uart->txwrptr, sstar_uart->txrdptr))
        {
            ss_urdma_putchar(sstar_uart);
        }

        CamOsSpinUnlockIrqRestore(&sstar_uart->port.lock);
    }
}

static void ss_urdma_interrupt(u32 irq, void *dev_id)
{
    /* Define Local Variables */
    u32                u32_irqType;
    struct sstar_uart *sstar_uart = (struct sstar_uart *)dev_id;

    u32_irqType = HAL_UART_get_irq_type(&sstar_uart->hal, UART_IRQ_URDMA);

    if (u32_irqType & UART_IRQ_URDMA_RX)
    {
        //HAL_UART_irq_enable(&sstar_uart->hal, UART_IRQ_URDMA_RX, 0);
        ss_urdma_getchar(sstar_uart);
        //HAL_UART_irq_enable(&sstar_uart->hal, UART_IRQ_URDMA_RX, 1);
    }
    else if (u32_irqType & UART_IRQ_URDMA_TX)
    {
        CamOsThreadWakeUp(sstar_uart->urdma_task);
    }
    else
        uart_err("URDMA dummy interrupt!\n");
}

#if SS_UART_8250_BUG_THRE
static u32 gu32_console_bug_thre_hits = 0;
module_param(gu32_console_bug_thre_hits, uint, S_IRUGO);

static void ss_serial8250_backup_timeout(struct timer_list *t)
{
    struct sstar_uart *sstar_uart = from_timer(sstar_uart, t, timer);
    unsigned long      flags;

    spin_lock_irqsave(&sstar_uart->port.lock, flags);

    /*
     * This should be a safe test for anyone who doesn't trust the
     * IIR bits on their UART, but it's specifically designed for
     * the "Diva" UART used on the management processor on many HP
     * ia64 and parisc boxes.
     */
    if ((UART_IRQ_NO_INT == HAL_UART_get_irq_type(&sstar_uart->hal, UART_IRQ)) && (sstar_uart->hal.tx_int_en))
    {
        HAL_UART_irq_enable(&sstar_uart->hal, UART_IRQ_TX, 0);
        HAL_UART_irq_enable(&sstar_uart->hal, UART_IRQ_TX, 1);
        gu32_console_bug_thre_hits++;
        ss_putchar((&sstar_uart->port));
    }

    spin_unlock_irqrestore(&sstar_uart->port.lock, flags);

    /* Standard timer interval plus 0.2s to keep the port running */
    mod_timer(&sstar_uart->timer, jiffies + uart_poll_timeout(&sstar_uart->port) + HZ / 5);
}
#endif

#ifdef CONFIG_CAMCLK_SUPPORT
static u16 ss_uart_set_clk(struct sstar_uart *sstar_uart, u32 request_baud)
{
    unsigned int num_parents;
    unsigned int tolerance, rate, real_baud;
    u16  divisor = 0;
    int i;

    {
        tolerance = 3;

        if (!sstar_uart->pvclk) {
            //OS is not ready,use default uart rate of clock gen
            // UART_ERR("%s: of_clk_get failed\n", p->dev->of_node->full_name);
            sstar_uart->stCfg.u32Rate=108000000;

            OUTREGMSK16(GET_REG_ADDR(BASE_REG_PMSLEEP_PA, 0x25), 0x8, 0xF);

            rate =  sstar_uart->stCfg.u32Rate;
            divisor = (rate + (8*request_baud)) / (16*request_baud);
            real_baud = rate / (16 * divisor);
            if( (abs(real_baud - request_baud)*100/request_baud) < tolerance)
            {
                // UART_ERR("[uart%d] uartclk=%d, request_baud=%d, real_baud=%d, divisor=0x%X\n", p->line, p->uartclk, request_baud, real_baud, divisor);
            }

            return divisor;
        }

        num_parents = sstar_uart->stCfg.u32NodeCount;
        if(num_parents==1)
        {
            rate=sstar_uart->stCfg.u32Rate;
            divisor = (rate + (8*request_baud)) / (16*request_baud);
            real_baud = rate / (16 * divisor);
            // UART_ERR("[uart%d]divisor=0x%02X, real_baud=%d,uart_clk=%d\n", p->line, divisor, real_baud,rate);
            if( (abs(real_baud - request_baud)*100/request_baud) < tolerance)
            {
                // UART_ERR("[uart%d] uartclk=%d, request_baud=%d, real_baud=%d, divisor=0x%X\n", p->line, p->uartclk, request_baud, real_baud, divisor);
            }
            return divisor;
        }
        else
        {
            for(i = 0; i < num_parents; i++)
            {
                if(sstar_uart->stCfg.u32Parent[i])
                {
                    CAMCLK_Set_Attribute stCfg;
                    rate = CamClkRateGet(sstar_uart->stCfg.u32Parent[i]);
                    divisor = (rate + (8*request_baud)) / (16*request_baud);
                    real_baud = rate / (16 * divisor);

                    uart_dbg("[%d]foreach parent divisor=0x%02X, real_baud=%d,uart_clk=%d\n", sstar_uart->uartName, divisor, real_baud,rate);
                    if( (abs(real_baud - request_baud)*100/request_baud) < tolerance)
                    {
                        CAMCLK_SETPARENT(stCfg,sstar_uart->stCfg.u32Parent[i]);
                        CamClkAttrSet(sstar_uart->pvclk,&stCfg);
                        uart_dbg("[%s] uartclk=%d, request_baud=%d, real_baud=%d, divisor=0x%X\n", sstar_uart->uartName, rate, request_baud, real_baud, divisor);
                        break;
                    }
                }
            }
            return divisor;
        }
    }
}

#else

static u16 ss_uart_set_clk(struct sstar_uart *sstar_uart, u32 request_baud)
{
    //unsigned int tolerance, rate, real_baud;
    //u16  divisor = 0;

    //tolerance = 3;


    return 0;
}
#endif

static void ss_uart_stop_tx(struct sstar_uart *sstar_uart)
{
    if (!sstar_uart->tx_start)
        return;
    else
        sstar_uart->tx_start = 0;

#if UART_RS485_SUPPORT
    if ((p->rs485.flags & SER_RS485_ENABLED) && !(sstar_uart->tx_empty_irq))
    {
        CamOsThreadWakeUp(sstar_uart->rs485_task);
    }
#endif

    HAL_UART_irq_enable(&sstar_uart->hal, (sstar_uart->urdma_en) ? UART_IRQ_URDMA_TX : UART_IRQ_TX, 0);

    CamOsTsemUp(&sstar_uart->tx_signal);
}

static void ss_uart_start_tx(struct sstar_uart *sstar_uart)
{
    if (sstar_uart->tx_start)
        return;
    else
        sstar_uart->tx_start = 1;

#if UART_RS485_SUPPORT
    if (p->rs485.flags & SER_RS485_ENABLED)
    {
        ss_uart_rs485_gpio(&p->rs485, true);
    }
#endif

    if (sstar_uart->urdma_en)
    {
        CamOsThreadWakeUp(sstar_uart->urdma_task);
    }
    else
    {
        HAL_UART_irq_enable(&sstar_uart->hal, UART_IRQ_TX, 1);
    }
}


static s32 ss_uart_startup(struct sstar_uart *sstar_uart)
{
    /* Define Local Variables */
    HAL_UART_irq_enable(&sstar_uart->hal, (sstar_uart->urdma_en) ? UART_IRQ_URDMA_RX : UART_IRQ_RX, 0);
    HAL_UART_irq_enable(&sstar_uart->hal, (sstar_uart->urdma_en) ? UART_IRQ_URDMA_TX : UART_IRQ_TX, 0);

    if (sstar_uart->urdma_en)
    {
        memset((void *)(sstar_uart->urdma_task_name), 0, sizeof(sstar_uart->urdma_task_name));
        sprintf(sstar_uart->urdma_task_name, "ss_urdma_task_%s", sstar_uart->uartName);
        sstar_uart->urdma_tAttr.szName     = sstar_uart->urdma_task_name;
        sstar_uart->urdma_tAttr.nPriority  = 50;
        sstar_uart->urdma_tAttr.nStackSize = 0;

        if (CAM_OS_OK
            != CamOsThreadCreate(&sstar_uart->urdma_task, &sstar_uart->urdma_tAttr, (void *)urdma_Task, (void *)sstar_uart))
        {
            uart_err("urdma_task : %s CamOsThreadCreate()is failed.\n", sstar_uart->uartName);
            return -1;
        }

        memset((void *)(sstar_uart->urdma_irq_name), 0, sizeof(sstar_uart->urdma_irq_name));
        sprintf(sstar_uart->urdma_irq_name, "ss_urdma_%s", sstar_uart->uartName);

        if (CAM_OS_OK != CamOsIrqRequest(sstar_uart->urdma_irq, ss_urdma_interrupt, sstar_uart->urdma_irq_name, (void *)sstar_uart))
        {
            uart_err("urdma_irq : %s CamOsIrqRequest()is failed.\n", sstar_uart->uartName);
            return -1;
        }
    }
    else
    {
        memset((void *)(sstar_uart->uart_irq_name), 0, sizeof(sstar_uart->uart_irq_name));
        sprintf(sstar_uart->uart_irq_name, "ss_uart_%s", sstar_uart->uartName);

        if (CAM_OS_OK != CamOsIrqRequest(sstar_uart->uart_irq, ss_uart_interrupt, sstar_uart->uart_irq_name, (void *)sstar_uart))
        {
            uart_err("uart_irq : %s CamOsIrqRequest()is failed.\n", sstar_uart->uartName);
            return -1;
        }
    }

#if UART_RS485_SUPPORT
#if SS_UART_TX_EMPTY_INTERRUPT
    if (sstar_uart->tx_empty_irq)
    {
        memset((void *)(sstar_uart->tx_empty_irq_name), 0, sizeof(sstar_uart->tx_empty_irq_name));
        sprintf(sstar_uart->tx_empty_irq_name, "ss_tx_empty_int_%s", sstar_uart->uartName);

        if (CAM_OS_OK != CamOsIrqRequest(sstar_uart->tx_empty_irq, ss_tx_empty_interrupt, sstar_uart->tx_empty_irq_name, (void *)sstar_uart))
        {
            uart_err("tx_empty_irq : %s CamOsIrqRequest()is failed.\n", sstar_uart->uartName);
            return -1;
        }
    }
    else
#endif
    {
        memset((void *)(sstar_uart->rs485_task_name), 0, sizeof(sstar_uart->rs485_task_name));
        sprintf(sstar_uart->rs485_task_name, "ss_rs485_task_%s", sstar_uart->uartName);
        sstar_uart->rs485_tAttr.szName     = sstar_uart->rs485_task_name;
        sstar_uart->rs485_tAttr.nPriority  = 50;
        sstar_uart->rs485_tAttr.nStackSize = 0;

        if (CAM_OS_OK
            != CamOsThreadCreate(&sstar_uart->rs485_task, &sstar_uart->rs485_tAttr, (void *)rs485_Task, (void *)sstar_uart))
        {
            uart_err("rs485_task : %s CamOsThreadCreate()is failed.\n", sstar_uart->uartName);
            return -EAGAIN;
        }
    }
#endif

    sstar_uart->rx_guard = 0;

    HAL_UART_init(&sstar_uart->hal);

    HAL_UART_irq_enable(&sstar_uart->hal, (sstar_uart->urdma_en) ? UART_IRQ_URDMA_RX : UART_IRQ_RX, 1);

#if SS_UART_8250_BUG_THRE
    /*
     * [PATCH] from drivers/tty/serial/8250/8250_core.c
     * The above check will only give an accurate result the first time
     * the port is opened so this value needs to be preserved.
     */
    if (!sstar_uart->urdma_en)
    {
        uart_dbg("ms_startup(): enable UART_BUG_THRE\n");
        timer_setup(&sstar_uart->timer, ss_serial8250_backup_timeout, 0);
        mod_timer(&sstar_uart->timer, jiffies + uart_poll_timeout(&sstar_uart->port) + HZ / 5);
    }
#endif

    return 0;
}

static void ss_uart_shutdown(struct sstar_uart *sstar_uart)
{
    HAL_UART_deinit(&sstar_uart->hal);

#if UART_RS485_SUPPORT
    if (!sstar_uart->tx_empty_irq)
    {
        CamOsThreadStop(sstar_uart->rs485_task);
    }
    else
    {
        CamOsIrqFree(sstar_uart->tx_empty_irq, (void *)sstar_uart);
    }
#endif

    if (sstar_uart->urdma_en)
    {
        CamOsThreadStop(sstar_uart->urdma_task);
        CamOsIrqFree(sstar_uart->urdma_irq, (void *)sstar_uart);
    }
    else
    {
        CamOsIrqFree(sstar_uart->uart_irq, (void *)sstar_uart);
    }

#if SS_UART_8250_BUG_THRE
    del_timer_sync(&sstar_uart->timer);
#endif
}

static void ss_uart_set_termios(struct sstar_uart *sstar_uart, Uart_cfg_t *UartCfg)
{
    /*------- Configure Chararacter Size --------*/
    switch (UartCfg->bit_length)
    {
        case 5: /* Word length is 5 bits */
            sstar_uart->hal.char_bits = 5;
            break;
        case 6: /* Word length is 6 bits */
            sstar_uart->hal.char_bits = 6;
            break;
        case 7: /* Word length is 7 bits */
            sstar_uart->hal.char_bits = 7;
            break;
        case 8: /* Word length is 8 bits */
            sstar_uart->hal.char_bits = 8;
            break;
        default:
            uart_err("%s unsupported bits:%d\n", __FUNCTION__, UartCfg->bit_length);
            break;
    }

    /*------------ Configure Stop bit -----------*/
    if (UART_STOP_2BIT == UartCfg->stop || UART_STOP_1_5BIT == UartCfg->stop)
    {
        /* Stop length is 1.5 bits (5-bit char), 2 bits (otherwise) */
        sstar_uart->hal.stop_bits = 2;
    }
    else
    {
        /* Stop length is 1 bit */
        sstar_uart->hal.stop_bits = 1;
    }

    /*----------- Configure Parity --------------*/
    if (UART_PARITY_NONE != UartCfg->parity)
    {
        /* Parity Enable */
        sstar_uart->hal.parity_en = 1;

        /* Set Odd/Even Parity */
        if (UART_PARITY_ODD != UartCfg->parity)
        {
            /* Odd Parity */
            sstar_uart->hal.even_parity_sel = 0;
        }
        else
        {
            /* Even Parity */
            sstar_uart->hal.even_parity_sel = 1;
        }
    }
    else
    {
        /* Parity Disable */
        sstar_uart->hal.parity_en = 0;
    }

    if (sstar_uart && sstar_uart->sctp_en == 1)
    {
        if (UartCfg->rtscts)
        {
            sstar_uart->hal.rtscts_en = 1;
        }
        else
        {
            sstar_uart->hal.rtscts_en = 0;
        }
    }

    // NOTE: we are going to set LCR, be carefully here
    sstar_uart->hal.divisor = ss_uart_set_clk(sstar_uart, UartCfg->rate);

    CamOsSpinLockIrqSave(&sstar_uart->port.lock);
    HAL_UART_config(&sstar_uart->hal);
    CamOsSpinUnlockIrqRestore(&sstar_uart->port.lock);
}

static s32  ss_uart_probe(struct sstar_uart *sstar_uart)
{
#if defined(CONFIG_SYSDESC_SUPPORT)
    u32  regdesc[2] = {0};
    u32  irqdesc[3] = {0};
    u16 u16Size = 0;
    u16 u16Remainder = 0;
#endif
    struct uart_cfg_t uart_cfg;
    u8 i;

    memset(&uart_cfg, 0, sizeof(struct uart_cfg_t));

    for (i=0; i<(sizeof(g_uart_cfg)/sizeof(struct uart_cfg_t)); i++)
    {
        if(g_uart_cfg[i].enable && strcasecmp((char *)sstar_uart->uartName, g_uart_cfg[i].uart_name) == 0)
        {
            memcpy(&uart_cfg, &g_uart_cfg[i], sizeof(struct uart_cfg_t));
            break;
        }
    }

    if (i == (sizeof(g_uart_cfg)/sizeof(struct uart_cfg_t)))
    {
        uart_dbg("[%s] no support %s!\n", __func__, sstar_uart->uartName);
        return -1;
    }

#if defined(CONFIG_SYSDESC_SUPPORT)
    if (0xFFFF == uart_cfg.deviceid)
    {
        uart_dbg("[%s] Fail to get deviceid!\n", __func__);
        return -1;
    }

#if defined(SYSDESC_PRO_status_u8)
    if (E_SYS_DESC_PASS != MDrv_SysDesc_Read_U8_Array(uart_cfg.deviceid, SYSDESC_PRO_status_u8, &i, 1) || !i)
    {
        uart_dbg("[%s][%s] sysdesc no enable!\n", __func__, sstar_uart->uartName);
        return -1;
    }
#else
    uart_dbg("[%s][%s] sysdesc no enable!\n", __func__, sstar_uart->uartName);
    return -1;
#endif

#if defined(CONFIG_CAMCLK_SUPPORT) && defined(SYSDESC_PRO_camclk_u16)
    if (E_SYS_DESC_PASS != MDrv_SysDesc_Read_U16_Array(uart_cfg.deviceid, SYSDESC_PRO_camclk_u16, &sstar_uart->clkid, 1))
    {
        uart_dbg("[%s] Fail to get clk!\n", __func__);
        return -1;
    }
#endif

#if defined(CONFIG_PADMUX_SUPPORT) && defined(SYSDESC_PRO_gpio_u16) && defined(SYSDESC_PRO_padmode_u32)
    if (E_SYS_DESC_PASS != MDrv_SysDesc_Read_U16_Array(uart_cfg.deviceid, SYSDESC_PRO_gpio_u16, &sstar_uart->indexgpio, 1) ||
        E_SYS_DESC_PASS != MDrv_SysDesc_Read_U32_Array(uart_cfg.deviceid, SYSDESC_PRO_padmode_u32, &sstar_uart->padmode, 1))
    {
        uart_dbg("[%s] Fail to get padmux!\n", __func__);
        return -1;
    }
#endif

#if defined(SYSDESC_PRO_reg_u32)
    if (E_SYS_DESC_PASS != MDrv_SysDesc_GetElementCount(uart_cfg.deviceid, SYSDESC_PRO_reg_u32, &u16Size,&u16Remainder))
    {
        uart_dbg("[%s] Fail to get reg!\n", __func__);
        return -1;
    }

    MDrv_SysDesc_Read_MultiTypes(uart_cfg.deviceid, SYSDESC_PRO_reg_u32, (void *)regdesc, sizeof(u32), sizeof(u32) * u16Size);

    sstar_uart->hal.uart_base = regdesc[0];
    sstar_uart->hal.urdma_base = regdesc[1];

    uart_dbg("[%s] uart_base 0x%x urdma_base 0x%x\n", sstar_uart->uartName, sstar_uart->hal.uart_base, sstar_uart->hal.urdma_base);
#endif

#if defined(SYSDESC_PRO_interrupts_u32)
    if (E_SYS_DESC_PASS != MDrv_SysDesc_GetElementCount(uart_cfg.deviceid, SYSDESC_PRO_interrupts_u32, &u16Size,&u16Remainder))
    {
        uart_dbg("[%s] Fail to get irq!\n", __func__);
        return -1;
    }

    MDrv_SysDesc_Read_MultiTypes(uart_cfg.deviceid, SYSDESC_PRO_interrupts_u32, (void *)irqdesc, sizeof(u32), sizeof(u32) * u16Size);

    sstar_uart->uart_irq = irqdesc[0];
    sstar_uart->urdma_irq = irqdesc[1];
    sstar_uart->tx_empty_irq = irqdesc[2];

    uart_dbg("[%s] uart_irq %d urdma_irq %d tx_empty_irq %d\n", sstar_uart->uartName, sstar_uart->uart_irq, sstar_uart->urdma_irq, sstar_uart->tx_empty_irq);
#endif

#if defined(SYSDESC_PRO_sctp_enable_u8)
    MDrv_SysDesc_Read_U8_Array(uart_cfg.deviceid, SYSDESC_PRO_sctp_enable_u8, &sstar_uart->sctp_en, 1);
#endif
#if defined(SYSDESC_PRO_dma_u8)
    MDrv_SysDesc_Read_U8_Array(uart_cfg.deviceid, SYSDESC_PRO_dma_u8, &sstar_uart->urdma_en, 1);
#endif
#if defined(SYSDESC_PRO_digmux_u8)
    sstar_uart->needdigmux = !MDrv_SysDesc_Read_U8_Array(uart_cfg.deviceid, SYSDESC_PRO_digmux_u8, &sstar_uart->hal.digmux, 1);
#endif
#if defined(SYSDESC_PRO_tx_fifo_level_u8)
    MDrv_SysDesc_Read_U8_Array(uart_cfg.deviceid, SYSDESC_PRO_tx_fifo_level_u8, &sstar_uart->hal.tx_fifo_level, 1);
#endif
#if defined(SYSDESC_PRO_rx_fifo_level_u8)
    MDrv_SysDesc_Read_U8_Array(uart_cfg.deviceid, SYSDESC_PRO_rx_fifo_level_u8, &sstar_uart->hal.rx_fifo_level, 1);
#endif
#if defined(SYSDESC_PRO_rx_pin_u8)
    MDrv_SysDesc_Read_U8_Array(uart_cfg.deviceid, SYSDESC_PRO_rx_pin_u8, &sstar_uart->hal.rx_pin, 1);
#endif

#else

#if defined(CONFIG_CAMCLK_SUPPORT)
    sstar_uart->clkid = uart_cfg.clkid;
#endif

#if defined(CONFIG_PADMUX_SUPPORT)
    sstar_uart->indexgpio = uart_cfg.indexgpio;
    sstar_uart->padmode = uart_cfg.padmode;
#endif

    sstar_uart->hal.uart_base = uart_cfg.uart_base;
    sstar_uart->hal.urdma_base = uart_cfg.urdma_base;

    sstar_uart->uart_irq = uart_cfg.uart_irq;
    sstar_uart->urdma_irq = uart_cfg.urdma_irq;
    sstar_uart->tx_empty_irq = uart_cfg.tx_empty_irq;

    sstar_uart->sctp_en = uart_cfg.sctp_enable;
    sstar_uart->urdma_en = uart_cfg.dma;
    sstar_uart->needdigmux = uart_cfg.needdigmux;

    sstar_uart->hal.digmux = uart_cfg.digmux;
    sstar_uart->hal.tx_fifo_level = uart_cfg.tx_fifo_level;
    sstar_uart->hal.rx_fifo_level = uart_cfg.rx_fifo_level;
    sstar_uart->hal.rx_pin = uart_cfg.rx_pin;
#endif // defined(CONFIG_SYSDESC_SUPPORT)

#if defined(CONFIG_PADMUX_SUPPORT)
    if (sstar_uart->indexgpio != PAD_UNKNOWN)
        MDrv_GPIO_PadVal_Set((u8)sstar_uart->indexgpio, sstar_uart->padmode);
#else
    // set padmux
#endif

#if defined(CONFIG_CAMCLK_SUPPORT)
    CamClkRegister(sstar_uart->uartName, sstar_uart->clkid, &sstar_uart->pvclk);

    if(sstar_uart->pvclk)
    {
        CamClkSetOnOff(sstar_uart->pvclk,1);
        CamClkAttrGet(sstar_uart->pvclk,&sstar_uart->stCfg);
    }
#else
    sstar_uart->spi_src_clk = uart_cfg.spi_src_clk;
    sstar_uart->reg_clkgen = &uart_cfg.reg_clkgen;
#endif

    if (sstar_uart->needdigmux)
    {
        if (0xFF != sstar_uart->hal.digmux)
        {
            HAL_UART_set_digmux(&sstar_uart->hal);
        }
    }
    else
    {
        sstar_uart->hal.digmux = 0xFF;
        HAL_UART_set_digmux(&sstar_uart->hal);
    }

    sstar_uart->port.fifosize = 32;
    CamOsSpinInit(&sstar_uart->port.lock);

    if (sstar_uart->fifoInit)
    {
        sstar_uart->urdma_en = 0;
        return 0;
    }

    if (sstar_uart->urdma_en)
    {
        sstar_uart->hal.rx_buf = sstar_uart->prxbuf;
        sstar_uart->hal.tx_buf = (u8*)CamOsMemAlloc(URDMA_TX_BUF_LENGTH);

        if (!sstar_uart->hal.rx_buf || !sstar_uart->hal.tx_buf)
        {
            sstar_uart->urdma_en = 0;
            uart_err("Allocate urdma rx_buffer/tx_buffer failed, use UART mode\n");
            goto dma_err;
        }

        sstar_uart->hal.rx_urdma_base = (u64)(unsigned long)CamOsMemPhysToMiu(CamOsMemVirtToPhys((void*)(sstar_uart->hal.rx_buf)));
        sstar_uart->hal.tx_urdma_base = (u64)(unsigned long)CamOsMemPhysToMiu(CamOsMemVirtToPhys((void*)(sstar_uart->hal.tx_buf)));
        sstar_uart->hal.rx_urdma_size = URDMA_TX_BUF_LENGTH;
        sstar_uart->hal.tx_urdma_size = sstar_uart->txbuf_size;

        uart_dbg("[%s] URDMA mode enable, reg_base=0x%08X, irq=%d\n", __func__, (unsigned int)(sstar_uart->hal.urdma_base), sstar_uart->urdma_irq);
    dma_err:
        sstar_uart->hal.urdma_en = sstar_uart->urdma_en;
    }

    return 0;
}

static s32 ss_uart_remove(struct sstar_uart *sstar_uart)
{
    if (sstar_uart->urdma_en)
    {
        CamOsMemRelease((void *)sstar_uart->hal.tx_buf);
    }

#if defined(CONFIG_CAMCLK_SUPPORT)
    if (sstar_uart->pvclk)
    {
        CamClkSetOnOff(sstar_uart->pvclk, 0);
        CamClkUnregister(sstar_uart->pvclk);
        sstar_uart->pvclk = NULL;
    }
#endif

    return 0;
}

static int _DrvUartWriteTXBuf(struct sstar_uart *sstar_uart, u8 *buf,  u32 len)
{
    u32 space      = 0;
    u32 count      = 0;
    u16 sw_tx_wptr = 0;
    u16 sw_tx_rptr = 0;

    CamOsSpinLockIrqSave(&sstar_uart->port.lock);

    sw_tx_wptr = sstar_uart->txwrptr;
    sw_tx_rptr = sstar_uart->txrdptr;

    do
    {
        space = HAL_UART_circ_space_end(sw_tx_wptr, sw_tx_rptr, sstar_uart->txbuf_size);

        if (len < space)
            space = len;

        if (space <= 0)
            break;

        memcpy(sstar_uart->ptxbuf + sw_tx_wptr, buf, space);
        sstar_uart->txwrptr = sw_tx_wptr = (sw_tx_wptr + space) & (sstar_uart->txbuf_size - 1);
        buf += space;
        len -= space;
        count += space;
    } while (len);

    CamOsSpinUnlockIrqRestore(&sstar_uart->port.lock);

    return count;
}

static int _DrvUartReadRXBuf(struct sstar_uart *sstar_uart, u8 *buf,  u32 len)
{
    u32 cnt        = 0;
    u32 count      = 0;
    u16 sw_rx_wptr = 0;
    u16 sw_rx_rptr = 0;

    CamOsSpinLockIrqSave(&sstar_uart->port.lock);

    sw_rx_wptr = sstar_uart->rxwrptr;
    sw_rx_rptr = sstar_uart->rxrdptr;

    while (buf)
    {
        cnt = HAL_UART_circ_cnt_to_end(sw_rx_wptr, sw_rx_rptr, sstar_uart->rxbuf_size);

        if (len < cnt)
            cnt = len;

        if (cnt <= 0)
            break;

        memcpy(buf, sstar_uart->prxbuf + sw_rx_rptr, cnt);
        sstar_uart->rxrdptr = sw_rx_rptr = (sw_rx_rptr + cnt) & (sstar_uart->rxbuf_size - 1);
        buf += cnt;
        len -= cnt;
        count += cnt;
    }

    CamOsSpinUnlockIrqRestore(&sstar_uart->port.lock);

    return count;
}

static void _DrvUartRxInsertChar(struct sstar_uart *sstar_uart, u8 ch)
{
    u32 free_size;
    u32 wr, rd;

    wr = sstar_uart->rxwrptr;
    rd = sstar_uart->rxrdptr;

    free_size = (rd > wr) ? (rd -wr) : (sstar_uart->rxbuf_size - wr + rd);
    if(free_size == 1) //RX fifo will full dump the latest data received
    {
        rd++;
        if(rd > (sstar_uart->rxbuf_size -1))
            rd = 0;
    }
    *(sstar_uart->prxbuf + wr) = ch;
    wr++;
    if(wr > sstar_uart->rxbuf_size - 1)
    {
        wr = 0;
    }

    sstar_uart->rxwrptr = wr;
    sstar_uart->rxrdptr = rd;
}

static int _DrvUartInitChannel(struct sstar_uart *sstar_uart)
{
    int ret = 0;

    sstar_uart->rxrdptr = 0;
    sstar_uart->rxwrptr = 0;
    sstar_uart->txrdptr = 0;
    sstar_uart->txwrptr = 0;

    CamOsTsemInit(&sstar_uart->tx_signal,0);
    CamOsTsemInit(&sstar_uart->rx_signal,0);
    CamOsMutexInit(&sstar_uart->tx_lock);
    CamOsMutexInit(&sstar_uart->rx_lock);

    sstar_uart->txbuf_size = URDMA_TX_BUF_LENGTH;

    sstar_uart->ptxbuf = (u8*)CamOsMemAlloc(URDMA_TX_BUF_LENGTH);
    if(sstar_uart->ptxbuf == NULL)
    {
        ret = -1;
    }

    sstar_uart->rxbuf_size = URDMA_RX_BUF_LENGTH;

    sstar_uart->prxbuf = (u8*)CamOsMemAlloc(URDMA_RX_BUF_LENGTH);
    if(sstar_uart->prxbuf == NULL)
    {
        ret = -1;
    }


    return ret;
}

static void _DrvUartDeinitChannel(struct sstar_uart *sstar_uart)
{
    sstar_uart->rxrdptr = 0;
    sstar_uart->rxwrptr = 0;
    sstar_uart->txrdptr = 0;
    sstar_uart->txwrptr = 0;

    CamOsTsemDeinit(&sstar_uart->tx_signal);
    CamOsTsemDeinit(&sstar_uart->rx_signal);
    CamOsMutexDestroy(&sstar_uart->tx_lock);
    CamOsMutexDestroy(&sstar_uart->rx_lock);

    if(sstar_uart->txbuf_size)
    {
        CamOsMemRelease(sstar_uart->ptxbuf);
        sstar_uart->txbuf_size = 0;
    }

    if(sstar_uart->rxbuf_size)
    {
        CamOsMemRelease(sstar_uart->prxbuf);
        sstar_uart->rxbuf_size = 0;
    }
}

UART_HANDLE DrvUartFifoInit(char *uartName)
{
    u8 i;
    struct sstar_uart *sstar_uart = NULL;

    for (i=0; i<NR_UART_PORTS; i++)
    {
        if (ss_uart[i] && !memcmp((void *)ss_uart[i]->uartName, uartName, strlen(uartName)))
        {
            return ss_uart[i];
        }
    }

    sstar_uart = (struct sstar_uart *)CamOsMemAlloc(sizeof(struct sstar_uart));

    if (!sstar_uart)
    {
        sstar_uart = &ss_uart_console;
    }

    for (i=0; i<NR_UART_PORTS; i++)
    {
        if (ss_uart[i] == sstar_uart)
        {
            return NULL;
        }
    }

    memset(sstar_uart, 0, sizeof(struct sstar_uart));

    memset(sstar_uart->uartName, 0, sizeof(sstar_uart->uartName));
    memcpy(sstar_uart->uartName, uartName, strlen(uartName));
    sstar_uart->fifoInit = 1;

    if (0 != ss_uart_probe(sstar_uart))
    {
        if (sstar_uart != &ss_uart_console)
            CamOsMemRelease(sstar_uart);

        return NULL;
    }

    for (i=0; i<NR_UART_PORTS; i++)
    {
        if (!ss_uart[i])
        {
            ss_uart[i] = sstar_uart;
        }
    }

    return sstar_uart;
}

void DrvUartFifoDeInit(UART_HANDLE uartHandle)
{
    u8 i;
    struct sstar_uart *sstar_uart = (struct sstar_uart *)uartHandle;

    if (!sstar_uart)
        return;

    ss_uart_remove(sstar_uart);

    for (i=0; i<NR_UART_PORTS; i++)
    {
        if (ss_uart[i] == uartHandle)
        {
            if (ss_uart[i] != &ss_uart_console)
                CamOsMemRelease(uartHandle);

            ss_uart[i] = NULL;
            break;
        }
    }
}

void DrvUartFifoWrite(UART_HANDLE uartHandle, u8 *buf, u32 len)
{
    struct uart_hal *hal = NULL;
    struct sstar_uart *sstar_uart = (struct sstar_uart *)uartHandle;
    //struct uart_hal uart0_hal;

    if (!sstar_uart)
    {
        return;
    }
    else
    {
        hal = &sstar_uart->hal;
    }

    if (sstar_uart->disable)
    {
        return;
    }

    CamOsSpinLockIrqSave(&sstar_uart->port.lock);

    if (sstar_uart->tx_silent)
    {
        CamOsSpinUnlockIrqRestore(&sstar_uart->port.lock);
        return;
    }

    *(buf + len) = '\0';

    while (*buf)
    {
        if (*buf == '\n' && *(buf+1) != '\r')
        {
            while(1 != HAL_UART_write(hal, buf, 1));
            buf++;
            while(1 != HAL_UART_write(hal, (u8 *)"\r", 1));
            continue;
        }

        while(1 != HAL_UART_write(hal, buf, 1));
        buf++;
    }

    CamOsSpinUnlockIrqRestore(&sstar_uart->port.lock);
}

s32 DrvUartFifoRead(UART_HANDLE uartHandle, u8 *buf, u32 len)
{
    struct uart_hal *hal = NULL;
    struct sstar_uart *sstar_uart = (struct sstar_uart *)uartHandle;
    s32 count = 0;

    if (!sstar_uart)
    {
        return 0;
    }
    else
    {
        hal = &sstar_uart->hal;
    }

    if (sstar_uart->disable)
    {
        return 0;
    }

    CamOsSpinLockIrqSave(&sstar_uart->port.lock);

    count = HAL_UART_read(hal, buf, len);

    CamOsSpinUnlockIrqRestore(&sstar_uart->port.lock);

    return count;
}

UART_HANDLE DrvUartOpen(char *uartName)
{
    u8 i;
    struct sstar_uart *sstar_uart = NULL;

    for (i=0; i<NR_UART_PORTS; i++)
    {
        if (ss_uart[i] && !memcmp((void *)ss_uart[i]->uartName, uartName, strlen(uartName)))
        {
            if (ss_uart[i]->fifoInit)
            {
                ss_uart_remove(ss_uart[i]);
                sstar_uart = ss_uart[i];
                CamOsSpinLockIrqSave(&sstar_uart->port.lock);
                sstar_uart->disable = 1;
                sstar_uart->fifoInit = 0;
                CamOsSpinUnlockIrqRestore(&sstar_uart->port.lock);
                ss_uart[i] = NULL;
                goto complete;
            }
            else
            {
                return ss_uart[i];
            }
        }
    }

    sstar_uart = (struct sstar_uart *)CamOsMemAlloc(sizeof(struct sstar_uart));

    if (!sstar_uart)
    {
        return NULL;
    }

    memset(sstar_uart, 0, sizeof(struct sstar_uart));

complete:
    if (_DrvUartInitChannel(sstar_uart))
    {
        CamOsMemRelease(sstar_uart);
        return NULL;
    }

    memset(sstar_uart->uartName, 0, sizeof(sstar_uart->uartName));
    memcpy(sstar_uart->uartName, uartName, strlen(uartName));

    if (0 != ss_uart_probe(sstar_uart))
    {
        if (sstar_uart != &ss_uart_console)
            CamOsMemRelease(sstar_uart);

        return NULL;
    }

    for (i=0; i<NR_UART_PORTS; i++)
    {
        if (!ss_uart[i])
        {
            ss_uart[i] = sstar_uart;
        }
    }

    ss_uart_startup(sstar_uart);

    sstar_uart->disable = 0;

    return sstar_uart;
}

void DrvUartClose(UART_HANDLE uartHandle)
{
    u8 i;
    struct sstar_uart *sstar_uart = (struct sstar_uart *)uartHandle;

    if (!sstar_uart)
        return;

    ss_uart_shutdown(sstar_uart);

    ss_uart_remove(sstar_uart);

    _DrvUartDeinitChannel(sstar_uart);

    for (i=0; i<NR_UART_PORTS; i++)
    {
        if (ss_uart[i] == uartHandle)
        {
            if ((ss_uart[i] != &ss_uart_console))
                CamOsMemRelease(uartHandle);

            ss_uart[i] = NULL;
            break;
        }
    }
}

s32 DrvUartIOctrl(UART_HANDLE uartHandle, u32 cmd, void *arg)
{
    struct sstar_uart *sstar_uart = (struct sstar_uart *)uartHandle;

    if (!sstar_uart)
        return -1;

    if (sstar_uart->disable)
    {
        return 0;
    }

    switch (cmd)
    {
        case UART_CMD_CONFIG:
            ss_uart_set_termios(sstar_uart, (Uart_cfg_t *)arg);
            break;
        case UART_CMD_SET_CONSOLE:
            sstar_uart->isConsole = 1;
            break;
    }

    return 0;
}

s32 DrvUartWrite(UART_HANDLE uartHandle, u8 *buf, u32 len, s32 timeOutMs)
{
    struct sstar_uart *sstar_uart = (struct sstar_uart *)uartHandle;
    CamOsTimespec_t startTime, currTime;
    u32 waittimeout;
    u32 count = 0;

    if (sstar_uart->disable)
    {
        return 0;
    }

    CamOsMutexLock(&sstar_uart->tx_lock);

    if (timeOutMs > 0)
        CamOsGetMonotonicTime(&startTime);

    while (len > 0)
    {
        count = _DrvUartWriteTXBuf(sstar_uart, buf, len);
        len -= count;

        ss_uart_start_tx(sstar_uart);

        if (len > 0 && timeOutMs != 0)
        {
            if (timeOutMs > 0)
            {
                CamOsGetMonotonicTime(&currTime);
                waittimeout = timeOutMs - (s32)CamOsTimeDiff(&startTime, &currTime, CAM_OS_TIME_DIFF_MS);
                if (waittimeout > 0)
                {
                    CamOsTsemTimedDown(&sstar_uart->tx_signal, waittimeout);
                }
                else
                    break;
            }
            else
            {
                CamOsTsemDown(&sstar_uart->tx_signal);
            }
        }
    }
    CamOsMutexUnlock(&sstar_uart->tx_lock);

    return count;
}


s32 DrvUartRead(UART_HANDLE uartHandle, u8 *buf,  u32 len, s32 timeOutMs)
{
    s32 count = 0;
    struct sstar_uart *sstar_uart = (struct sstar_uart *)uartHandle;

    if (sstar_uart->disable)
    {
        return 0;
    }

    CamOsMutexLock(&sstar_uart->rx_lock);

    if(HAL_UART_circ_empty(sstar_uart->rxwrptr, sstar_uart->rxrdptr))
    {
        if (timeOutMs != 0)
        {
            if (timeOutMs > 0)
            {
                sstar_uart->is_rx_blocked = 1;
                CamOsTsemTimedDown(&sstar_uart->rx_signal, timeOutMs);
                sstar_uart->is_rx_blocked = 0;
            }
            else
            {
                sstar_uart->is_rx_blocked = 1;
                CamOsTsemDown(&sstar_uart->rx_signal);
                sstar_uart->is_rx_blocked = 0;
            }
        }
    }

    count = _DrvUartReadRXBuf(sstar_uart, buf, len);

    CamOsMutexUnlock(&sstar_uart->rx_lock);

    return count;
}
