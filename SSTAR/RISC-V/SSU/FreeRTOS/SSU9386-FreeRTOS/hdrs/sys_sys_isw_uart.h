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

#ifndef __SYS_SYS_ISW_UART_H__
#define __SYS_SYS_ISW_UART_H__

#include "cam_os_wrapper.h"

#define KERN_SOH        "\001"          /* ASCII Start Of Header */
#define KERN_EMERG      KERN_SOH "0"    /* system is unusable */
#define KERN_ALERT      KERN_SOH "1"    /* action must be taken immediately */
#define KERN_CRIT       KERN_SOH "2"    /* critical conditions */
#define KERN_ERR        KERN_SOH "3"    /* error conditions */
#define KERN_WARNING    KERN_SOH "4"    /* warning conditions */
#define KERN_NOTICE     KERN_SOH "5"    /* normal but significant condition */
#define KERN_INFO       KERN_SOH "6"    /* informational */
#define KERN_DEBUG      KERN_SOH "7"    /* debug-level messages */

#define LOGLEVEL_EMERG      0   /* system is unusable */
#define LOGLEVEL_ALERT      1   /* action must be taken immediately */
#define LOGLEVEL_CRIT       2   /* critical conditions */
#define LOGLEVEL_ERR        3   /* error conditions */
#define LOGLEVEL_WARNING    4   /* warning conditions */
#define LOGLEVEL_NOTICE     5   /* normal but significant condition */
#define LOGLEVEL_INFO       6   /* informational */
#define LOGLEVEL_DEBUG      7   /* debug-level messages */

#define DIRECT_MODE     1
#define BUFFER_MODE     2

#define UART_RX_POLLING_MODE        1
#define UART_RX_INTERRUPT_MODE      2

/*--------------------------------------------------------------------------*/
/* Functions                                                                */
/*--------------------------------------------------------------------------*/

void send_msg(char *msg);

void drv_uart_dbg_console_log_level_privilege(bool val);
void drv_uart_dbg_set_console_log_level(u32 level);
u32 drv_uart_dbg_get_console_log_level(void);
void drv_uart_dbg_set_def_msg_log_level(u32 u32Level);
u32 drv_uart_dbg_get_def_msg_log_level(void);
bool drv_uart_dbg_is_tx_init(void);
void drv_uart_dbg_tx_init(void);
#if CONFIG_CONSOLE_UART_RX_MODE == UART_RX_INTERRUPT_MODE
void drv_uart_dbg_rx_interrupt_init(void);
#endif
void drv_uart_dbg_send_msg(char* msg);
int drv_uart_dbg_get_char_direct(void);
int drv_uart_dbg_get_char_nblk_direct(void);
int drv_uart_dbg_send_msg_direct(char *msg);
bool drv_uart_dbg_send_msg_level_valid(const char *msg);

s8 UartSendTrace(const char *strFormat, ...);
void ForceUartSendTrace(const char *strFormat, ...);

s32 vm_atoi(const char *input);
s32 vm_htoi(const char *input);
char *wait_for_command(void);

void console_early_init(void);
void console_init(void);

void drv_uart_dbg_flush_buf_data_to_uart(void);
bool drv_uart_dbg_is_buf_mode_enable(void);
void drv_uart_dbg_set_buf_mode_en(bool bEnable);
void console_Task(void);
void uart_Task(void);

#endif // __SYS_SYS_ISW_UART_H__
