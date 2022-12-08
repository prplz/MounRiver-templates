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


/*=============================================================*/
// Include files
/*=============================================================*/
#include "drv_int_ctrl_pub_api.h"
#include "mdrv_gpio_io.h"
#include "mdrv_gpio.h"
#include "sys_sys_isw_cli.h"
#include "sys_sys_isw_uart.h"

#if defined(__VER_GPIO__)
/*=============================================================*/
// pre-definition functions
/*=============================================================*/
static int _GPIOInput(CLI_t *pCli, char *p);
static int _GPIOOutput(CLI_t *pCli, char *p);
static int _GPIOISR(CLI_t *pCli, char *p);
static int _GPIOISRFree(CLI_t *pCli, char *p);
static int _GPIOToggle(CLI_t *pCli, char *p);
static int _GPIOState(CLI_t *pCli, char *p);
static int _GPIOList(CLI_t *pCli, char *p);
/*=============================================================*/
// Global Variable definition
/*=============================================================*/

int _GPIOTest(CLI_t * pCli, char * p)
{
    s32 argc;
    char *cmd;

    argc = CliTokenCount(pCli);
    if (argc < 1)
    {
        cliPrintf("args error please refs to help\n");
        return eCLI_PARSE_INPUT_ERROR;
    }
    cmd = CliTokenPop(pCli);
    if(strcmp(cmd, "output") == 0)
    {
        _GPIOOutput(pCli, p);
    }
    else if(strcmp(cmd, "input") == 0 || strcmp(cmd, "get") == 0)
    {
        _GPIOInput(pCli, p);
    }
    else if(strcmp(cmd, "toggle") == 0)
    {
        _GPIOToggle(pCli, p);
    }
    else if(strcmp(cmd, "state") == 0)
    {
        _GPIOState(pCli, p);
    }
    else if(strcmp(cmd, "list") == 0)
    {
        _GPIOList(pCli, p);
    }
    else if(strcmp(cmd, "isr") == 0)
    {
        _GPIOISR(pCli, p);
    }
    else if(strcmp(cmd, "isrfree") == 0)
    {
        _GPIOISRFree(pCli, p);
    }
    else
    {
        cliPrintf("unknown cmd%s\n", cmd);
    }
    return eCLI_PARSE_OK;
}

static void _GPIOInputISR(u32 eIntNum, void* p1)
{
    cliPrintf("!!!!GPIO ISR Callback : Number : %d!!!!\n",p1);
}

static int _GPIOInput(CLI_t *pCli, char *p)
{
    u32 nNum;

    if (CliTokenCount(pCli) == 1)
    {
        if (CliTokenPopNum(pCli, &nNum, 0) != eCLI_PARSE_OK)
            goto _HELP_EXIT;

        camdriver_gpio_request(NULL,nNum);
        camdriver_gpio_direction_input(NULL,nNum);
        cliPrintf("GPIO %d Input Level %d\n", nNum,camdriver_gpio_get(NULL,nNum));
    }
    else
    {
_HELP_EXIT:
        cliPrintf("Please enter [gpio]\n");
        return eCLI_PARSE_INVALID_PARAMETER;
    }

    return eCLI_PARSE_OK;
}

static int _GPIOOutput(CLI_t *pCli, char *p)
{
    u32 nNum, nOutLevel;

    if (CliTokenCount(pCli) == 2)
    {
        if (CliTokenPopNum(pCli, &nNum, 0) != eCLI_PARSE_OK)
            goto _HELP_EXIT;
        if (CliTokenPopNum(pCli, &nOutLevel, 0) != eCLI_PARSE_OK)
            goto _HELP_EXIT;

        cliPrintf("GPIO %d Output Level %d\n", nNum, nOutLevel);
        camdriver_gpio_request(NULL,nNum);
        camdriver_gpio_direction_output(NULL,nNum,nOutLevel);
    }
    else
    {
_HELP_EXIT:
        cliPrintf("Please enter [gpio] [1/0]\n");
        return eCLI_PARSE_INVALID_PARAMETER;
    }

    return eCLI_PARSE_OK;
}

static int _GPIOISR(CLI_t *pCli, char *p)
{
    u32 nNum, nTrigger;
    int irq = -1;
    int ret = 0;

    if (CliTokenCount(pCli) == 2)
    {
        if (CliTokenPopNum(pCli, &nNum, 0) != eCLI_PARSE_OK)
            goto _HELP_EXIT;
        if (CliTokenPopNum(pCli, &nTrigger, 0) != eCLI_PARSE_OK)
            goto _HELP_EXIT;

        cliPrintf("====GPIO ISR Number : %d , %s ====\n", nNum, nTrigger == 0 ? "Falling" : "Rasing");
        camdriver_gpio_request(NULL,nNum);
        camdriver_gpio_direction_input(NULL,nNum);
        MDrv_GPIO_Set_POLARITY(nNum,nTrigger);//Or Call DrvIntcSetPolarity after CamOsIrqRequest

        irq = camdriver_gpio_to_irq(NULL, nNum);

        if (irq < 0)
        {
            cliPrintf("gpio_to_irq fail..!\n");
            return eCLI_PARSE_OK;
        }
        else {
            ret = CamOsIrqRequest(irq, _GPIOInputISR, "GPIO ISR", (void *)nNum);
            if (ret != 0)
            {
                cliPrintf("request irq fail..!\n");
            }

            //DrvIntcSetPolarity(nTrigger,irq);
        }
    }
    else
    {
_HELP_EXIT:
        cliPrintf("Plz enter [GPIONum][Rasing:1/Falling:0]\n");
        return eCLI_PARSE_INVALID_PARAMETER;
    }

    return eCLI_PARSE_OK;
}

static int _GPIOISRFree(CLI_t *pCli, char *p)
{
    u32 nNum;
    int irq = -1;

    if (CliTokenCount(pCli) == 1)
    {
        if (CliTokenPopNum(pCli, &nNum, 0) != eCLI_PARSE_OK)
            goto _HELP_EXIT;

        cliPrintf("====GPIO Free ISR Number : %d ====\n", nNum);

        irq = camdriver_gpio_to_irq(NULL, nNum);
        if(irq < 0) {
            cliPrintf("gpio_to_irq fail..!\n");
            return eCLI_PARSE_OK;
        }
        CamOsIrqFree(irq,NULL);
    }
    else
    {
_HELP_EXIT:
        cliPrintf("Plz enter [GPIONum]\n");
        return eCLI_PARSE_INVALID_PARAMETER;
    }

    return eCLI_PARSE_OK;
}

static int _GPIOToggle(CLI_t *pCli, char *p)
{
    u32 nNum;

    if (CliTokenCount(pCli) == 1)
    {
        if (CliTokenPopNum(pCli, &nNum, 0) != eCLI_PARSE_OK)
            goto _HELP_EXIT;

        camdriver_gpio_request(NULL,nNum);

        if(camdriver_gpio_get(NULL,nNum))
        {
            camdriver_gpio_direction_output(NULL,nNum,0);
            cliPrintf("GPIO %d Output Level %d\n", nNum, 0);
        }
        else
        {
            camdriver_gpio_direction_output(NULL,nNum,1);
            cliPrintf("GPIO %d Output Level %d\n", nNum, 1);
        }
    }
    else
    {
_HELP_EXIT:
        cliPrintf("Please enter [gpio]\n");
        return eCLI_PARSE_INVALID_PARAMETER;
    }

    return eCLI_PARSE_OK;
}

static int _GPIOState(CLI_t *pCli, char *p)
{
    u32 nNum;

    if (CliTokenCount(pCli) == 1)
    {
        if (CliTokenPopNum(pCli, &nNum, 0) != eCLI_PARSE_OK)
            goto _HELP_EXIT;

        camdriver_gpio_request(NULL,nNum);

        if(MDrv_GPIO_Pad_InOut(nNum))
        {
            cliPrintf("GPIO %d Input ", nNum);
        }
        else
        {
            cliPrintf("GPIO %d Output ", nNum);
        }
        cliPrintf("Level %d\n", camdriver_gpio_get(NULL,nNum));
    }
    else
    {
_HELP_EXIT:
        cliPrintf("Please enter [gpio]\n");
        return eCLI_PARSE_INVALID_PARAMETER;
    }

    return eCLI_PARSE_OK;
}

static int _GPIOList(CLI_t *pCli, char *p)
{
    u32 i;
    u32 nSize;

    if (CliTokenCount(pCli) == 0)
    {
        nSize = GPIO_NR;
    }
    else if(CliTokenCount(pCli) == 1)
    {
        if (CliTokenPopNum(pCli, &nSize, 0) != eCLI_PARSE_OK)
            goto _HELP_EXIT;
    }
    else
    {
_HELP_EXIT:
        cliPrintf("Please enter [num_of_pins]\n");
        return eCLI_PARSE_INVALID_PARAMETER;
    }

    for(i=0; i<nSize; i++)
    {
        cliPrintf("GPIO %3d ", i);

        if(MDrv_GPIO_Pad_InOut(i))
        {
            cliPrintf("In  ");
        }
        else
        {
            cliPrintf("Out ");
        }
        cliPrintf("pin=%d\n", camdriver_gpio_get(NULL,i));

        if((i>0) && (i%10==0)) cliPrintf("\n");
    }


    return eCLI_PARSE_OK;
}

static char _szGPIOMenuHelpTxt[] = "Config gpio port";
static char _szGPIOMenuUsageTxt[] =
    "(for 2nd parameter, you must type at least 3 characters)\n"
    "gpio output <gpio#> <1/0>  : ex: gpio output 69 1\n"
    "gpio input/get <gpio#>     : ex: gpio input 10  (gpio 10 set as input)\n"
    "gpio toggle <gpio#>        : ex: gpio tog 49 (toggle)\n"
    "gpio state <gpio#>         : ex: gpio sta 49 (get i/o status(direction) & pin status)\n"
    "gpio list [num_of_pins]    : ex: gpio list 10 (list GPIO1~GPIO10 status)\n";

SS_RTOS_CLI_CMD(gpio,          _szGPIOMenuHelpTxt,   _szGPIOMenuUsageTxt,  _GPIOTest);

#endif
