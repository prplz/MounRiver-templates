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

#include "drv_pwm_in.h"
#include "sys_sys_isw_cli.h"

/*=============================================================*/
// Global Functions
/*=============================================================*/

#if 1
static int _PWMINTest(CLI_t *pCli, char *p)
{
    u32 PWMId, CapEnable, RstMode, DetMode, EdgeSel, TimerDiv, PulDiv;
    u32 period = 0;
    u32 dutyi  = 0;
    u32 dutyd  = 0;
    struct sstar_pwm_in_chip *sstar_pwm_in = NULL;
    sstar_pwm_in = PwmInGetPtr();

    if (CliTokenCount(pCli) == 7)
    {
        if (CliTokenPopNum(pCli, &PWMId, 0) != eCLI_PARSE_OK)
            goto _PWMIN_HELP_EXIT;
        if (CliTokenPopNum(pCli, &CapEnable, 0) != eCLI_PARSE_OK)
            goto _PWMIN_HELP_EXIT;
        if (CliTokenPopNum(pCli, &RstMode, 0) != eCLI_PARSE_OK)
            goto _PWMIN_HELP_EXIT;
        if (CliTokenPopNum(pCli, &DetMode, 0) != eCLI_PARSE_OK)
            goto _PWMIN_HELP_EXIT;
        if (CliTokenPopNum(pCli, &TimerDiv, 0) != eCLI_PARSE_OK)
            goto _PWMIN_HELP_EXIT;
        if (CliTokenPopNum(pCli, &PulDiv, 0) != eCLI_PARSE_OK)
            goto _PWMIN_HELP_EXIT;
        if (CliTokenPopNum(pCli, &EdgeSel, 0) != eCLI_PARSE_OK)
            goto _PWMIN_HELP_EXIT;

        cliPrintf("PWMId:%d ,Capture Enable:%d, Reswt Mode:%d ,DetMode:%d, TimerDiv:%d, PulDiv:%d, EdgeSel:%d\n", PWMId,CapEnable,RstMode,DetMode,TimerDiv,PulDiv,EdgeSel);

        CamOsMsDelay(1000);
        DrvPWMINEnable(sstar_pwm_in, PWMId, 0);
        CamOsMsDelay(1000);
        DrvPWMINRstMode(sstar_pwm_in, PWMId, RstMode);
        CamOsMsDelay(1000);
        DrvPWMINDetMode(sstar_pwm_in, PWMId,DetMode);
        CamOsMsDelay(1000);
        DrvPWMINEdgeSel(sstar_pwm_in, PWMId, EdgeSel);
        CamOsMsDelay(1000);
        DrvPWMINTimeDiv(sstar_pwm_in, PWMId, TimerDiv);
        CamOsMsDelay(1000);
        DrvPWMINPulDiv(sstar_pwm_in, PWMId, PulDiv);
        CamOsMsDelay(1000);
        DrvPWMINEnable(sstar_pwm_in, PWMId, CapEnable);
        CamOsMsDelay(1000);
    }
    else if(CliTokenCount(pCli) == 1)
    {
        if (CliTokenPopNum(pCli, &PWMId, 0) != eCLI_PARSE_OK)
            goto _PWMIN_HELP_EXIT;

        cliPrintf("PWMId:%d\n", PWMId);
        DrvPWMINGet(sstar_pwm_in, PWMId, &period, &dutyi, &dutyd);
        cliPrintf("period :   %u\n", period);
        cliPrintf("duty   :   %d.%d%%\n", dutyi, dutyd);
    }
    else
    {
    _PWMIN_HELP_EXIT:
        cliPrintf("Plz key in : pwm_in [PWMId] [Capture Enable] [Reswt Mode] [DetMode] [TimerDiv] [PulDiv] [EdgeSel]\n");
        cliPrintf("Plz key in : pwm_in [PWMId]\n");
        return eCLI_PARSE_INVALID_PARAMETER;
    }

    return eCLI_PARSE_OK;
}
SS_RTOS_CLI_CMD(pwm_in,
        "PWMIN",
        "",
        _PWMINTest);
#endif
