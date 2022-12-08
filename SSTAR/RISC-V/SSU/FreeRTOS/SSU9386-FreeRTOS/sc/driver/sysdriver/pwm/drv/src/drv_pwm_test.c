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

#include "drv_pwm.h"
#include "sys_sys_isw_cli.h"

/*=============================================================*/
// Global Functions
/*=============================================================*/

#if 1
static int _PWMTest(CLI_t *pCli, char *p)
{
    u32 PWMIndex, Freq, Shift, Duty, NotInvert, Enable;
    PWMConfig_t sPWMConfig;

    if (CliTokenCount(pCli) == 5)
    {
        if (CliTokenPopNum(pCli, &PWMIndex, 0) != eCLI_PARSE_OK)
            goto _PWM_HELP_EXIT;
        if (CliTokenPopNum(pCli, &Freq, 0) != eCLI_PARSE_OK)
            goto _PWM_HELP_EXIT;
        if (CliTokenPopNum(pCli, &Shift, 0) != eCLI_PARSE_OK)
            goto _PWM_HELP_EXIT;
        if (CliTokenPopNum(pCli, &Duty, 0) != eCLI_PARSE_OK)
            goto _PWM_HELP_EXIT;
        if (CliTokenPopNum(pCli, &NotInvert, 0) != eCLI_PARSE_OK)
            goto _PWM_HELP_EXIT;

        cliPrintf("PWMIndex:%d ,Pad:%d, PadMode:%d, Freq:%d, Shift:%d ,Duty:%d, NotInvert:%d\n", PWMIndex,Freq,Shift,Duty,NotInvert);

        sPWMConfig.bInvert = !NotInvert;
        sPWMConfig.nDuty = Duty;
        sPWMConfig.nFrequency = Freq;
        sPWMConfig.nPulseShift = Shift;
        DrvPWMOutputPulse(PWMIndex,sPWMConfig);
    }
    else if(CliTokenCount(pCli) == 2)
    {
        if (CliTokenPopNum(pCli, &PWMIndex, 0) != eCLI_PARSE_OK)
            goto _PWM_HELP_EXIT;
        if (CliTokenPopNum(pCli, &Enable, 0) != eCLI_PARSE_OK)
            goto _PWM_HELP_EXIT;
        cliPrintf("PWMIndex:%d ,Enable:%d\n", PWMIndex,Enable);
        DrvPWMEnable(PWMIndex, Enable);
    }
    else
    {
    _PWM_HELP_EXIT:
        cliPrintf("Plz key in : pwm [PWMIndex] [Freq(Hz)] [shift(%)] [duty(%)] [NOT invert]\n");
        cliPrintf("Plz key in : pwm [PWMIndex] [Enable]\n");
        return eCLI_PARSE_INVALID_PARAMETER;
    }

    return eCLI_PARSE_OK;
}
SS_RTOS_CLI_CMD(pwm,
        "PWM",
        "",
        _PWMTest);
#endif
