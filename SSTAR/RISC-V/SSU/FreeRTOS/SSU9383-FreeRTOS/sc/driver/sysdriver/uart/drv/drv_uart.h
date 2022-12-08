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

#ifndef _UART_DRV_H_
#define _UART_DRV_H_


#define UART_CMD_CONFIG 0x0001
#define UART_CMD_SET_CONSOLE 0x0002
#define UART_CMD_DISABLE 0x0004


typedef enum
{
    UART_STOP_1BIT=0,
    UART_STOP_1_5BIT,
    UART_STOP_2BIT
} Uart_StopBit_t;

/**
 * @brief Type of UART Indication
 *
 *  Used in @ref UartCallbackInd_t callback function to specify the type of UART Indication.
 */
typedef enum
{
    UART_PARITY_NONE=0,
    UART_PARITY_ODD,
    UART_PARITY_EVEN
} Uart_Parity_t;

/**
 * @brief UART HW configuration type
 *
 *  Used in @ref DrvUartFifoInit function to specify the parameters of the UART to use.
 */
typedef struct
{
    u8             bit_length;       ///< length in number of bits
    u8             parity;           ///< parity
    u8             stop;             ///< stop bit
    u8             rtscts;
    u32            rate;             ///< bit rate
} Uart_cfg_t;

typedef void* UART_HANDLE;

UART_HANDLE DrvUartFifoInit(char *uartName);
void DrvUartFifoDeInit(UART_HANDLE uartHandle);
void DrvUartFifoWrite(UART_HANDLE uartHandle, u8 *buf, u32 len);
s32 DrvUartFifoRead(UART_HANDLE uartHandle, u8 *buf, u32 len);


UART_HANDLE DrvUartOpen(char *uartName);
void DrvUartClose(UART_HANDLE uartHandle);
s32 DrvUartIOctrl(UART_HANDLE uartHandle, u32 cmd, void *arg);
s32 DrvUartWrite(UART_HANDLE uartHandle, u8 *buf, u32 len, s32 timeOutMs);
s32 DrvUartRead(UART_HANDLE uartHandle, u8 *buf,  u32 len, s32 timeOutMs);


#endif /* _UART_DRV_H_ */
