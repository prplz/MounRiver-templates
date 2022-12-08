
#include "vm_types.ht"
#include "sys_traces.ho"
#include "sys_sys_isw_cli.h"
#include "sys_sys_isw_uart.h"
#include "drv_uart.h"

static char _szUartMenuHelpTxt0[] = "UART";
static char _szUartMenuUsageTxt0[] = "";

char *debug_char = "/* SigmaStar uart test */ \r\n";


int UartTest(CLI_t * pCli, char * p)
{
    UART_HANDLE uartHandle;
    Uart_cfg_t UartCfg = {0};
    u8 read_buf[512];
    s32 read_len;

    CamOsPrintf("parameter :%s\n",p);

    UartCfg.rate = 115200;
    UartCfg.bit_length = 8;
    UartCfg.parity = UART_PARITY_NONE;
    UartCfg.stop = UART_STOP_1BIT;

    CamOsPrintf("uart1\n");

    uartHandle = DrvUartOpen("uart1");

    if (!uartHandle)
        CamOsPrintf("DrvUartOpen fail\n");

    DrvUartIOctrl(uartHandle, UART_CMD_CONFIG, (void *)&UartCfg);

    DrvUartWrite(uartHandle, (u8 *)debug_char, 28, -1);

    CamOsUsDelay(1000000);

    //DrvUartClose(uartHandle);

    CamOsPrintf("uart2\n");

    uartHandle = DrvUartOpen("uart2");

    if (!uartHandle)
        CamOsPrintf("DrvUartOpen fail\n");

    DrvUartIOctrl(uartHandle, UART_CMD_CONFIG, (void *)&UartCfg);

    DrvUartWrite(uartHandle, (u8 *)debug_char, 28, -1);

    CamOsUsDelay(1000000);

    //DrvUartClose(uartHandle);

    CamOsPrintf("uart3\n");

    uartHandle = DrvUartOpen("uart3");

    if (!uartHandle)
        CamOsPrintf("DrvUartOpen fail\n");

    DrvUartIOctrl(uartHandle, UART_CMD_CONFIG, (void *)&UartCfg);

    DrvUartWrite(uartHandle, (u8 *)debug_char, 28, -1);

    CamOsUsDelay(1000000);

    //DrvUartClose(uartHandle);

    CamOsPrintf("uart4\n");

    uartHandle = DrvUartOpen("uart4");

    if (!uartHandle)
        CamOsPrintf("DrvUartOpen fail\n");

    DrvUartIOctrl(uartHandle, UART_CMD_CONFIG, (void *)&UartCfg);

    DrvUartWrite(uartHandle, (u8 *)debug_char, 28, -1);

    CamOsUsDelay(1000000);

    //DrvUartClose(uartHandle);

    CamOsPrintf("fuart\n");

    uartHandle = DrvUartOpen("fuart");

    if (!uartHandle)
        CamOsPrintf("DrvUartOpen fail\n");

    DrvUartIOctrl(uartHandle, UART_CMD_CONFIG, (void *)&UartCfg);

    DrvUartWrite(uartHandle, (u8 *)debug_char, 28, -1);

    do
    {
        read_len = DrvUartRead(uartHandle, read_buf, sizeof(read_buf), -1);

        if (read_len >= 0)
        {
            read_buf[read_len] = '\0';

            CamOsPrintf("%s", read_buf);

            if (read_buf[0] == 'q')
                break;
        }
    } while(1);

    return eCLI_PARSE_OK;
}

SS_RTOS_CLI_CMD(uart, _szUartMenuHelpTxt0, _szUartMenuUsageTxt0, UartTest);

