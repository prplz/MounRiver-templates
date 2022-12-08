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
static int _PWMGroupTest(CLI_t *pCli, char *p)
{
    u32 PWMIndex, GroupIndex, Enable, Freq, Begin, End, Hold, Round;

    if(CliTokenCount(pCli) == 7)
    {
        PWMConfig_t *sPWMConfig = NULL;
        if (CliTokenPopNum(pCli, &PWMIndex, 0) != eCLI_PARSE_OK)
            goto _PWM_GROUP_HELP_EXIT;
        if (CliTokenPopNum(pCli, &GroupIndex, 0) != eCLI_PARSE_OK)
            goto _PWM_GROUP_HELP_EXIT;
        if (CliTokenPopNum(pCli, &Freq, 0) != eCLI_PARSE_OK)
            goto _PWM_GROUP_HELP_EXIT;
        if (CliTokenPopNum(pCli, &Begin, 0) != eCLI_PARSE_OK)
            goto _PWM_GROUP_HELP_EXIT;
        if (CliTokenPopNum(pCli, &End, 0) != eCLI_PARSE_OK)
            goto _PWM_GROUP_HELP_EXIT;
        if (CliTokenPopNum(pCli, &Hold, 0) != eCLI_PARSE_OK)
            goto _PWM_GROUP_HELP_EXIT;
        if (CliTokenPopNum(pCli, &Round, 0) != eCLI_PARSE_OK)
            goto _PWM_GROUP_HELP_EXIT;

        cliPrintf("PWMIndex:%d, GroupIndex:%d, Freq:%d, Begin:%d, End:%d, Hold:%d, Round:%d\n", PWMIndex,GroupIndex,Freq,Begin,End,Hold,Round);

        DrvPWMIrqRequest(sPWMConfig);
        if(Hold)
        {
            DrvGroupHold(GroupIndex, Hold);
        }
        DrvGroupModeIn(PWMIndex, 1);
        DrvGroupPeriodIn(PWMIndex, Freq);
        DrvGroupBeginIn(PWMIndex, 0, Begin);
        DrvGroupEndIn(PWMIndex, 0, End);
        if(Round)
        {
            DrvGroupRoundIn(GroupIndex, Round);
        }
    }
    else if(CliTokenCount(pCli) == 2)
    {
        if (CliTokenPopNum(pCli, &GroupIndex, 0) != eCLI_PARSE_OK)
            goto _PWM_GROUP_HELP_EXIT;
        if (CliTokenPopNum(pCli, &Enable, 0) != eCLI_PARSE_OK)
            goto _PWM_GROUP_HELP_EXIT;
        cliPrintf("GroupIndex:%d, Enable state:%d\n", GroupIndex, Enable);
        DrvGroupEnableIn(GroupIndex, Enable);
    }
    else if(CliTokenCount(pCli) == 1)
    {
        if (CliTokenPopNum(pCli, &GroupIndex, 0) != eCLI_PARSE_OK)
            goto _PWM_GROUP_HELP_EXIT;
        cliPrintf("Stop GroupIndex:%d\n", GroupIndex, Enable);
        DrvGroupStop(GroupIndex, 1);
    }
    else
    {
    _PWM_GROUP_HELP_EXIT:
        cliPrintf("Plz key in config:  pwm_group [PWMIndex] [GroupIndex] [Freq] [Begin] [End] [Hlod] [Round]\n");
        cliPrintf("Plz key in enable:  pwm_group [GroupIndex] [GroupEnable]\n");
        cliPrintf("Plz key in stop:  pwm_group [GroupIndex]\n");
        return eCLI_PARSE_INVALID_PARAMETER;
    }

    return eCLI_PARSE_OK;
}
SS_RTOS_CLI_CMD(pwm_group,
        "PWM GROUP Function",
        "",
        _PWMGroupTest);
#endif
