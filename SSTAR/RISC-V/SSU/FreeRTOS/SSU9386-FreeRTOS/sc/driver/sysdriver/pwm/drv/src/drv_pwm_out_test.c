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

#include "drv_pwm_out.h"
#include "sys_sys_isw_cli.h"
#include "drv_camclk_Api.h"
#include "camclk.h"

/*=============================================================*/
// Global Functions
/*=============================================================*/

#if 1
static int _PWMOUTTest(CLI_t *pCli, char *p)
{
    u32 PWMId, PDdtLen, NDdtLen, PEnable, NEnable;
    u32 ModeSel, Status, OutEnable;
    struct sstar_pwm_out_chip *sstar_pwm_out = NULL;
    sstar_pwm_out = PwmOutGetPtr();

    if (CliTokenCount(pCli) == 5)
    {
        if (CliTokenPopNum(pCli, &PWMId, 0) != eCLI_PARSE_OK)
            goto _PWMOUT_HELP_EXIT;
        if (CliTokenPopNum(pCli, &PDdtLen, 0) != eCLI_PARSE_OK)
            goto _PWMOUT_HELP_EXIT;
        if (CliTokenPopNum(pCli, &NDdtLen, 0) != eCLI_PARSE_OK)
            goto _PWMOUT_HELP_EXIT;
        if (CliTokenPopNum(pCli, &PEnable, 0) != eCLI_PARSE_OK)
            goto _PWMOUT_HELP_EXIT;
        if (CliTokenPopNum(pCli, &NEnable, 0) != eCLI_PARSE_OK)
            goto _PWMOUT_HELP_EXIT;

        cliPrintf("PWMIndex:%d ,PDdtLen:%d, NDdtLen:%d ,PEnable:%d, NEnable:%d\n", PWMId, PDdtLen, NDdtLen, PEnable, NEnable);

        DrvDdtPConfig(sstar_pwm_out, PWMId, PDdtLen);
        DrvDdtNConfig(sstar_pwm_out, PWMId, NDdtLen);
        DrvOutPConfig(sstar_pwm_out, PWMId, PEnable);
        DrvOutNConfig(sstar_pwm_out, PWMId, NEnable);
    }
    else if(CliTokenCount(pCli) == 3)
    {
        if (CliTokenPopNum(pCli, &PWMId, 0) != eCLI_PARSE_OK)
            goto _PWMOUT_HELP_EXIT;
        if (CliTokenPopNum(pCli, &ModeSel, 0) != eCLI_PARSE_OK)
            goto _PWMOUT_HELP_EXIT;
        if (CliTokenPopNum(pCli, &Status, 0) != eCLI_PARSE_OK)
            goto _PWMOUT_HELP_EXIT;

        cliPrintf("PWMIndex:%d ,ModeSel:%d, Status:%d\n", PWMId, ModeSel, Status);

        DrvBkinConfig(sstar_pwm_out, PWMId, ModeSel, Status);
        DrvBkinTri(sstar_pwm_out, PWMId);
    }
    else if(CliTokenCount(pCli) == 2)
    {
        if (CliTokenPopNum(pCli, &PWMId, 0) != eCLI_PARSE_OK)
            goto _PWMOUT_HELP_EXIT;
        if (CliTokenPopNum(pCli, &OutEnable, 0) != eCLI_PARSE_OK)
            goto _PWMOUT_HELP_EXIT;

        cliPrintf("PWMIndex:%d ,OutEnable:%d,\n", PWMId, OutEnable);

        DrvOutEnable(sstar_pwm_out, PWMId, OutEnable);
        CamOsMsDelay(1000);
    }
    else
    {
    _PWMOUT_HELP_EXIT:
        cliPrintf("Plz key in : pwm_out [PWMIndex] [PDdtLen] [NDdtLen] [PEnable(%)] [NEnable]\n");
        cliPrintf("Plz key in : pwm_out [PWMIndex] [ModeSel] [Idle Status]\n");
        cliPrintf("Plz key in : pwm_out [PWMIndex] [OutEnable]\n");
        return eCLI_PARSE_INVALID_PARAMETER;
    }

    return eCLI_PARSE_OK;
}
SS_RTOS_CLI_CMD(pwm_out,
        "PWMOUT",
        "",
        _PWMOUTTest);

static int _PWMADCTest(CLI_t *pCli, char *p)
{
    u32 ChanSel       = 0;
    u32 i            = 0;
    u32 adcId[23]    = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22};
    u32 enable       = 0;
    u32 dmaenable       = 0;
    u32 trimode      = 0;
    u32 trimethod    = 0;
    u32 conmode_sel  = 0;
    u32 SeqLen       = 0;
    u32 Channelid    = 0;
    u16 AdcData = 0;
    u32 Selector = 0;

    struct sstar_pwm_out_chip *sstar_pwm_out = NULL;

    if (CliTokenCount(pCli) == 6)
    {
        if (CliTokenPopNum(pCli, &ChanSel, 0) != eCLI_PARSE_OK)
            goto _PWMOUT_HELP_EXIT;
        if (CliTokenPopNum(pCli, &trimode, 0) != eCLI_PARSE_OK)
            goto _PWMOUT_HELP_EXIT;
        if (CliTokenPopNum(pCli, &trimethod, 0) != eCLI_PARSE_OK)
            goto _PWMOUT_HELP_EXIT;
        if (CliTokenPopNum(pCli, &conmode_sel, 0) != eCLI_PARSE_OK)
            goto _PWMOUT_HELP_EXIT;
        if (CliTokenPopNum(pCli, &dmaenable, 0) != eCLI_PARSE_OK)
            goto _PWMOUT_HELP_EXIT;
        if (CliTokenPopNum(pCli, &enable, 0) != eCLI_PARSE_OK)
            goto _PWMOUT_HELP_EXIT;

        cliPrintf("ChanSel:%d ,trimode:%d, trimethod:%d ,conmode_sel:%d, dmaenable:%d, enable:%d\n", ChanSel, trimode, trimethod, conmode_sel, dmaenable, enable);
        sstar_pwm_out = PwmOutGetPtr();
        if (ChanSel)
        {
            SeqLen = 23;
            for (i = 0; i < SeqLen; i++)
            {
                adc_regu_seq_config(sstar_pwm_out, SeqLen, i, adcId[i]);
            }
            adc_regu_tri_sel(sstar_pwm_out, trimode, trimethod);
            adc_regu_con_sel(sstar_pwm_out, conmode_sel);
            adc_enable(sstar_pwm_out, enable);
            adc_dma_config(sstar_pwm_out, 0x30);
            adc_dma_enable(sstar_pwm_out, 1, dmaenable);
            adc_regu_tri_star(sstar_pwm_out, enable);
            CamOsMsDelay(2000);
        }
        else
        {
            SeqLen = 12;
            for (i = 0; i < SeqLen; i++)
            {
                adc_inj_seq_config(sstar_pwm_out, SeqLen, i, adcId[i]);
            }
            adc_inj_tri_sel(sstar_pwm_out, trimode, trimethod);
            adc_inj_con_sel(sstar_pwm_out, conmode_sel);
            adc_enable(sstar_pwm_out, enable);
            adc_dma_config(sstar_pwm_out, 0x30);
            adc_dma_enable(sstar_pwm_out, 1, dmaenable);
            adc_inj_tri_star(sstar_pwm_out, enable);
            CamOsMsDelay(2000);
        }
    }
    else if (CliTokenCount(pCli) == 2)
    {
        if (CliTokenPopNum(pCli, &Selector, 0) != eCLI_PARSE_OK)
            goto _PWMOUT_HELP_EXIT;
        if (CliTokenPopNum(pCli, &Channelid, 0) != eCLI_PARSE_OK)
            goto _PWMOUT_HELP_EXIT;

        cliPrintf("ChanSel:%d\n", Channelid);
        sstar_pwm_out = PwmOutGetPtr();
        if (Selector)
        {
            adc_dma_data(sstar_pwm_out, Selector);
        }
        else
        {
            adc_data(sstar_pwm_out, Channelid, &AdcData);
            cliPrintf("Channel[%d] data is :%d\n", Channelid, AdcData);    }
    }

    else
    {
    _PWMOUT_HELP_EXIT:
        cliPrintf("Plz key in : pwm_adc [ChanSel] [trimode] [trimethod] [conmode_sel] [dmaenable] [enable]\n");
        cliPrintf("Plz key in : pwm_adc [Selector] [Channelid]\n");
        return eCLI_PARSE_INVALID_PARAMETER;
    }

    return eCLI_PARSE_OK;
}
SS_RTOS_CLI_CMD(pwm_adc,
        "PWMADC",
        "",
        _PWMADCTest);

#endif
