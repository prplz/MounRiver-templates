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
#include "hal_pwm_out.h"
#include "mdrv_gpio.h"
#include "initcall.h"
#include "drv_int_ctrl_pub_api.h"
#include "hal_int_ctrl_pub.h"
#if defined(CONFIG_SYSDESC_SUPPORT)
#include "drv_sysdesc.h"
#endif
#ifdef CONFIG_CAM_CLK
#include "drv_camclk_Api.h"
#endif
/*=============================================================*/
// Global Functions
/*=============================================================*/
static struct sstar_pwm_out_chip* ptr_sstar_pwm_out;

void DrvDdtPConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 PwmId, u32 DdtLen)
{
    MHal_PWMDdtP(sstar_out_chip, PwmId, DdtLen);
}

void DrvDdtNConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 PwmId, u32 DdtLen)
{
    MHal_PWMDdtN(sstar_out_chip, PwmId, DdtLen);
}

void DrvOutPConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 PwmId, u8 Enable)
{
    MHal_PWMOutPConfig(sstar_out_chip, PwmId, Enable);
}

void DrvOutNConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 PwmId, u8 Enable)
{
    MHal_PWMOutNConfig(sstar_out_chip, PwmId, Enable);
}

void DrvOutEnable(struct sstar_pwm_out_chip *sstar_out_chip, u8 PwmId, u8 Enable)
{
    MHal_PWMOutEnable(sstar_out_chip, PwmId, Enable);
}

void DrvBkinConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 PwmId, u8 ModeSel, u8 Status)
{
    MHal_PWMBkinSel(sstar_out_chip, PwmId, ModeSel);
    MHal_PWMPIdleStatus(sstar_out_chip, PwmId, Status);
    MHal_PWMNIdleStatus(sstar_out_chip, PwmId, Status);
}

void DrvBkinTri(struct sstar_pwm_out_chip *sstar_out_chip, u8 PwmId)
{
    MHal_PWMOutEnable(sstar_out_chip, PwmId, 0);
}

void adc_enable(struct sstar_pwm_out_chip *sstar_out_chip, u8 enable)
{
    MHal_ADCEnable(sstar_out_chip, enable);
}

void adc_regu_seq_config(struct sstar_pwm_out_chip *sstar_out_chip, u8 SeqLen, u8 u8Id, u8 u8Channel)
{
    MHal_ADCRegSeqConfig(sstar_out_chip, SeqLen, u8Id, u8Channel);
}

void adc_regu_tri_sel(struct sstar_pwm_out_chip *sstar_out_chip, u8 trimode, u8 trimethod)
{
    MHal_ADCRegTriConfig(sstar_out_chip, trimode, trimethod);
}

void adc_regu_con_sel(struct sstar_pwm_out_chip *sstar_out_chip, u8 conmode_sel)
{
    MHal_ADCRegConMode(sstar_out_chip, conmode_sel);
}

void adc_regu_tri_star(struct sstar_pwm_out_chip *sstar_out_chip, u8 enable)
{
    MHal_ADCRegStr(sstar_out_chip, enable);
}

void adc_inj_seq_config(struct sstar_pwm_out_chip *sstar_out_chip, u8 SeqLen, u8 u8Id, u8 u8Channel)
{
    MHal_ADCInjSeqConfig(sstar_out_chip, SeqLen, u8Id, u8Channel);
}

void adc_inj_tri_sel(struct sstar_pwm_out_chip *sstar_out_chip, u8 trimode, u8 trimethod)
{
    MHal_ADCInjTriConfig(sstar_out_chip, trimode, trimethod);
}

void adc_inj_con_sel(struct sstar_pwm_out_chip *sstar_out_chip, u8 conmode_sel)
{
    MHal_ADCInjConMode(sstar_out_chip, conmode_sel);
}

void adc_inj_tri_star(struct sstar_pwm_out_chip *sstar_out_chip, u8 enable)
{
    MHal_ADCInjStr(sstar_out_chip, enable);
}

void adc_dma_config(struct sstar_pwm_out_chip *sstar_out_chip, u32 miusize)
{
    MHal_ADCDMAConfig(sstar_out_chip, miusize);
}

void adc_dma_enable(struct sstar_pwm_out_chip *sstar_out_chip, u8 align, u8 enable)
{
    MHal_ADCDMAEnable(sstar_out_chip, align, enable);
}

void adc_wdt_config(struct sstar_pwm_out_chip *sstar_out_chip, u8 channelid, u8 datahigh, u8 datalow)
{
    MHal_ADCWDTConfig(sstar_out_chip, channelid, datahigh, datalow);
}

void adc_wdt_enable(struct sstar_pwm_out_chip *sstar_out_chip, u8 channelid, u8 enable)
{
    MHal_ADCWDTEnable(sstar_out_chip, channelid, enable);
}

void adc_freerun(struct sstar_pwm_out_chip *sstar_out_chip, u8 avecnt, u32 oneshotprd)
{
    MHal_ADCFreerun(sstar_out_chip, avecnt, oneshotprd);
}

void adc_data(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Channel, u16 *u8AdcData)
{
    MHal_ADCData(sstar_out_chip, u8Channel, u8AdcData);
}

void adc_dma_data(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Selctor)
{
    MHal_ADCDMAData(sstar_out_chip, u8Selctor);
}

struct sstar_pwm_out_chip* PwmOutGetPtr(void)
{
    return ptr_sstar_pwm_out;
}

static s32 pwm_out_probe(void)
{
    reg_desc_t sRegdesc[2];
    int IrqNum[2] = {0, 0};
    u32 clk_frequency = 0;
    u8 i;
#ifdef CONFIG_CAM_CLK
    u32 ret_clk;
    u16 PwmoutClkId;
#endif
    struct sstar_pwm_out_chip *sstar_pwm_out = NULL;
#if defined(CONFIG_SYSDESC_SUPPORT)
    u32 ret_desc;
    u16 u16Size = 0;
    u16 u16Remainder = 0;
#endif
    sstar_pwm_out = CamOsMemAlloc(sizeof(struct sstar_pwm_out_chip));
    if (!sstar_pwm_out)
    {
        SSTAR_PWM_OUT_ERR("%s:%d CamOsMemAlloc() failed\n", __func__, __LINE__);
    }
    memset(sRegdesc, 0, sizeof(reg_desc_t) * 2);

#if defined(SYSDESC_PRO_status_u8)
    if (E_SYS_DESC_PASS != MDrv_SysDesc_Read_U8_Array(SYSDESC_DEV_pwm_out, SYSDESC_PRO_status_u8, &i, 1) || !i)
    {
        SSTAR_PWM_OUT_DBG("[%s][pwm_out] sysdesc no enable!\n", __func__);
        return -1;
    }
#else
    SSTAR_PWM_OUT_DBG("[%s][pwm_out] sysdesc no enable!\n", __func__);
    return -1;
#endif

#if defined(CONFIG_SYSDESC_SUPPORT)
    MDrv_SysDesc_GetElementCount(SYSDESC_DEV_pwm_out, SYSDESC_PRO_reg_u32_u16, &u16Size, &u16Remainder);
    ret_desc = MDrv_SysDesc_Read_MultiTypes(SYSDESC_DEV_pwm_out, SYSDESC_PRO_reg_u32_u16, (void *)sRegdesc, sizeof(reg_desc_t), sizeof(reg_desc_t)*u16Size);
    if (ret_desc != E_SYS_DESC_PASS)
    {
        SSTAR_PWM_OUT_ERR("Fail to get register address\n");
    }
    MDrv_SysDesc_GetElementCount(SYSDESC_DEV_pwm_out, SYSDESC_PRO_interrupts_u8, &u16Size,&u16Remainder);
    ret_desc = MDrv_SysDesc_Read_MultiTypes(SYSDESC_DEV_pwm_out, SYSDESC_PRO_interrupts_u8, (void *)IrqNum, 1, u16Size);
    if (ret_desc != E_SYS_DESC_PASS)
    {
        SSTAR_PWM_OUT_ERR("Fail to get interrupts number\n");
    }
    MDrv_SysDesc_GetElementCount(SYSDESC_DEV_pwm_out, SYSDESC_PRO_clkfrequency_u32, &u16Size,&u16Remainder);
    ret_desc = MDrv_SysDesc_Read_U32(SYSDESC_DEV_pwm_out, SYSDESC_PRO_clkfrequency_u32, &clk_frequency);
    if (ret_desc != E_SYS_DESC_PASS)
    {
        SSTAR_PWM_OUT_ERR("Fail to get clock frequency\n");
    }
#else
    sRegdesc[0].regbase = 0x2203600;
    sRegdesc[0].regbanksize = 0x200;
    sRegdesc[1].regbase = 0x2200E00;
    sRegdesc[1].regbanksize = 0x200;
    IrqNum[0] = INT_FIQ_ADC_FIQ;
    clk_frequency = 12000000;
#endif

    sstar_pwm_out->base = sRegdesc[0].regbase;
    sstar_pwm_out->piubase = sRegdesc[1].regbase;
    sstar_pwm_out->irq = IrqNum[0];
    sstar_pwm_out->clk_fre = clk_frequency;
    ptr_sstar_pwm_out = sstar_pwm_out;

#ifdef CONFIG_CAM_CLK
    ret_clk = MDrv_SysDesc_Read_U16(SYSDESC_DEV_pwm_out, SYSDESC_PRO_camclk_u16, &PwmoutClkId);
    if (ret_clk != E_SYS_DESC_PASS)
    {
        SSTAR_PWM_OUT_ERR("Fail to get CAMCLK ID11123\n");
    }
    ret_clk = CamClkRegister((u8* )"PWM_OUT", PwmoutClkId, &sstar_pwm_out->pvPwmClk);
    if (ret_clk == CAMCLK_RET_OK)
    {
        CamClkAttrGet(sstar_pwm_out->pvPwmClk, &sstar_pwm_out->stGetCfg);
        CAMCLK_SETRATE_ROUND(sstar_pwm_out->stSetCfg, sstar_pwm_out->clk_fre);
        CamClkAttrSet(sstar_pwm_out->pvPwmClk, &sstar_pwm_out->stSetCfg);
        CamClkSetOnOff(sstar_pwm_out->pvPwmClk, 1);
    }
    else
    {
        SSTAR_PWM_OUT_ERR("CamClkRegister fail\n");
    }
#else
    MHal_PWMOUT_SetClock();
#endif
    return 0;
}

void pwm_out_init(void)
{
    pwm_out_probe();
}

rtos_application_initcall(pwm_out_init, 3);
