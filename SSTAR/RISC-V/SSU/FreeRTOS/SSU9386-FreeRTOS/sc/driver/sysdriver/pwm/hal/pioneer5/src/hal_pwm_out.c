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
#include "hal_pwm_reg.h"

#ifdef CONFIG_ARM64
#define READ_WORD(_reg)        (*(volatile u16 *)(u64)(_reg))
#define WRITE_WORD(_reg, _val) ((*(volatile u16 *)(u64)(_reg)) = (u16)(_val))
#define WRITE_WORD_MASK(_reg, _val, _mask) \
    ((*(volatile u16 *)(u64)(_reg)) = ((*(volatile u16 *)(u64)(_reg)) & ~(_mask)) | ((u16)(_val) & (_mask)))
#define SET_WORD(_reg, _mask) ((*(volatile u16 *)(u64)(_reg)) = (u16)((*(volatile u16 *)(u64)(_reg)) | (_mask)))
#define CLR_WORD(_reg, _mask) ((*(volatile u16 *)(u64)(_reg)) = (u16)((*(volatile u16 *)(u64)(_reg)) & ~(_mask)))

#else
#define READ_WORD(_reg)        (*(volatile u16 *)(u32)(_reg))
#define WRITE_WORD(_reg, _val) ((*(volatile u16 *)(u32)(_reg)) = (u16)(_val))
#define WRITE_WORD_MASK(_reg, _val, _mask) \
    ((*(volatile u16 *)(u32)(_reg)) = ((*(volatile u16 *)(u32)(_reg)) & ~(_mask)) | ((u16)(_val) & (_mask)))
#define SET_WORD(_reg, _mask) ((*(volatile u16 *)(u32)(_reg)) = (u16)((*(volatile u16 *)(u32)(_reg)) | (_mask)))
#define CLR_WORD(_reg, _mask) ((*(volatile u16 *)(u32)(_reg)) = (u16)((*(volatile u16 *)(u32)(_reg)) & ~(_mask)))
#endif

static u8  _pwmDdtNSatus[PWM_NUM] = {0};
static u8  _pwmDdtPSatus[PWM_NUM] = {0};
static void *_miu_addr              = NULL;

/**
 * MHal_PWMOUT_SetClock - enable PWM OUT clock
 * @void
 *
 * Returns None
 */
void MHal_PWMOUT_SetClock(void)
{
    if ((READ_WORD(BASE_REG_CLKGEN_PA + PWM_CLK_OFFSET) & BIT0))
    {
        WRITE_WORD_MASK( BASE_REG_CLKGEN_PA + PWM_CLK_OFFSET, 0x00, BIT0 | BIT1 | BIT2 | BIT3);
        SSTAR_PWM_OUT_INFO("[PWMOUT]CfgClk(12M)\r\n");
    }
}

/**
 * MHal_PWMDdtP - enable PWM ddt_p and set ddt length
 * @sstar_chip: struct sstar_pwm_chip
 * @u8Id: PWM channel
 * @u32DdtLen: pwm ddt length
 *
 * Returns None
 */
void MHal_PWMDdtP(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Id, u32 u32DdtLen)
{
    u32 common    = 0;
    u32 pwmclk    = 0;
    u32 u32length = 0;

    pwmclk = sstar_out_chip->clk_fre;
    switch (pwmclk)
    {
        case 12000000:
            pwmclk = 3;
            common = 250;
            break;
        default:
            pwmclk = 3;
            common = 250;
    }

    u32length = (pwmclk * u32DdtLen) / (common);
    SSTAR_PWM_OUT_DBG("P dead time length is 0x%x\n", u32length);
    if (u32length)
    {
        SET_WORD(sstar_out_chip->base + (u8Id * 12) + u16REG_PWM_OUT, PWM_DDTP_EN_BIT);
        WRITE_WORD(sstar_out_chip->base + (u8Id * 12) + u16REG_PWM_DDTP_LEN, u32length);
    }
    else
    {
        CLR_WORD(sstar_out_chip->base + (u8Id * 12) + u16REG_PWM_OUT, PWM_DDTP_EN_BIT);
    }
    _pwmDdtPSatus[u8Id] = 1;
}

/**
 * MHal_PWMDdtPIdle - set PWM ddt_p idle state
 * @sstar_chip: struct sstar_pwm_chip
 * @u8Id: PWM channel
 * @u8State: pwm idle state
 *
 * Returns None
 */
void MHal_PWMDdtPIdle(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Id, u8 u8State)
{
    if (u8State)
    {
        SET_WORD(sstar_out_chip->base + (u8Id * 12) + u16REG_PWM_OUT, PWM_IDLE_P_BIT);
    }
    else
    {
        CLR_WORD(sstar_out_chip->base + (u8Id * 12) + u16REG_PWM_OUT, PWM_IDLE_P_BIT);
    }
}

/**
 * MHal_PWMDdtN - enable PWM ddt_n and set ddt length
 * @sstar_chip: struct sstar_pwm_chip
 * @u8Id: PWM channel
 * @u32DdtLen: pwm ddt length
 *
 * Returns None
 */
void MHal_PWMDdtN(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Id, u32 u32DdtLen)
{
    u32 common    = 0;
    u32 pwmclk    = 0;
    u32 u32length = 0;

    pwmclk = sstar_out_chip->clk_fre;
    switch (pwmclk)
    {
        case 12000000:
            pwmclk = 3;
            common = 250;
            break;
        default:
            pwmclk = 3;
            common = 250;
    }

    u32length = (pwmclk * u32DdtLen) / (common);
    SSTAR_PWM_OUT_DBG("N dead time length is 0x%x\n", u32length);

    if (u32length)
    {
        SET_WORD(sstar_out_chip->base + (u8Id * 12) + u16REG_PWM_OUT, PWM_DDTN_EN_BIT);
        WRITE_WORD(sstar_out_chip->base + (u8Id * 12) + u16REG_PWM_DDTN_LEN, u32length);
    }
    else
    {
        CLR_WORD(sstar_out_chip->base + (u8Id * 12) + u16REG_PWM_OUT, PWM_DDTN_EN_BIT);
    }
    _pwmDdtNSatus[u8Id] = 1;
}

/**
 * MHal_PWMDdtNIdle - set PWM ddt_n idle state
 * @sstar_chip: struct sstar_pwm_chip
 * @u8Id: PWM channel
 * @u8State: pwm idle state
 *
 * Returns None
 */
void MHal_PWMDdtNIdle(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Id, u8 u8State)
{
    if (u8State)
    {
        SET_WORD(sstar_out_chip->base + (u8Id * 12) + u16REG_PWM_OUT, PWM_IDLE_N_BIT);
    }
    else
    {
        CLR_WORD(sstar_out_chip->base + (u8Id * 12) + u16REG_PWM_OUT, PWM_IDLE_N_BIT);
    }
}

/**
 * MHal_PWMOutPConfig - PWM out_p enable or disable
 * @sstar_chip: struct sstar_pwm_chip
 * @u8Id: PWM channel
 * @u8Enable: 1: enable, 0: disable
 *
 * Returns None
 */
void MHal_PWMOutPConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Id, u8 u8Enable)
{
    if (u8Enable)
    {
        SET_WORD(sstar_out_chip->base + (u8Id * 12) + u16REG_PWM_OUT, PWM_OUTP_EN_BIT);
    }
    else
    {
        CLR_WORD(sstar_out_chip->base + (u8Id * 12) + u16REG_PWM_OUT, PWM_OUTP_EN_BIT);
    }
}

/**
 * MHal_PWMOutNConfig - PWM out_n enable or disable
 * @sstar_chip: struct sstar_pwm_chip
 * @u8Id: PWM channel
 * @u8Enable: 1: enable, 0: disable
 *
 * Returns None
 */
void MHal_PWMOutNConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Id, u8 u8Enable)
{
    if (u8Enable)
    {
        SET_WORD(sstar_out_chip->base + (u8Id * 12) + u16REG_PWM_OUT, PWM_OUTN_EN_BIT);
    }
    else
    {
        CLR_WORD(sstar_out_chip->base + (u8Id * 12) + u16REG_PWM_OUT, PWM_OUTN_EN_BIT);
    }
}

/**
 * MHal_PWMOutEnable - PWM out enable or disable
 * @sstar_chip: struct sstar_pwm_chip
 * @u8Id: PWM channel
 * @u8Enable: 1: enable, 0: disable
 *
 * Returns None
 */
void MHal_PWMOutEnable(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Id, u8 u8Enable)
{
    if (u8Enable)
    {
        SET_WORD(sstar_out_chip->base + (u8Id * 12) + u16REG_PWM_OUT, PWM_MOE_BIT | PWM_MOE_TRI_BIT);
    }
    else
    {
        WRITE_WORD_MASK(sstar_out_chip->base + (u8Id * 12) + u16REG_PWM_OUT, PWM_MOE_TRI_BIT, PWM_MOE_BIT | PWM_MOE_TRI_BIT);
    }
}

/**
 * MHal_PWMBkinSel - PWM break input mode select
 * @sstar_chip: struct sstar_pwm_chip
 * @u8Id: PWM channel
 * @u8Sel: 1: hw & sw trigger, 0: only sw trigger
 *
 * Returns None
 */
void MHal_PWMBkinSel(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Id, u8 u8Sel)
{
    if (u8Sel)
        SET_WORD(sstar_out_chip->base + (u8Id * 12) + u16REG_PWM_OUT, PWM_AOE_BIT);
    else
        CLR_WORD(sstar_out_chip->base + (u8Id * 12) + u16REG_PWM_OUT, PWM_AOE_BIT);
}

/**
 * MHal_PWMPIdleStatus - PWM out_p idle status setting
 * @sstar_chip: struct sstar_pwm_chip
 * @u8Id: PWM channel
 * @u8Status: 1: high, 0: low
 *
 * Returns None
 */
void MHal_PWMPIdleStatus(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Id, u8 u8Status)
{
    if (u8Status)
        SET_WORD(sstar_out_chip->base + (u8Id * 12) + u16REG_PWM_OUT, PWM_IDLE_P_BIT);
    else
        CLR_WORD(sstar_out_chip->base + (u8Id * 12) + u16REG_PWM_OUT, PWM_IDLE_P_BIT);
}

/**
 * MHal_PWMNIdleStatus - PWM out_n idle status setting
 * @sstar_chip: struct sstar_pwm_chip
 * @u8Id: PWM channel
 * @u8Status: 1: high, 0: low
 *
 * Returns None
 */
void MHal_PWMNIdleStatus(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Id, u8 u8Status)
{
    if (u8Status)
        SET_WORD(sstar_out_chip->base + (u8Id * 12) + u16REG_PWM_OUT, PWM_IDLE_N_BIT);
    else
        CLR_WORD(sstar_out_chip->base + (u8Id * 12) + u16REG_PWM_OUT, PWM_IDLE_N_BIT);
}

/**
 * MHal_ADCWDTConfig - PWM adc watchdog config
 * @sstar_chip: struct sstar_pwm_chip
 * @u8Channel: ADC channel
 * @u8DatHigh: the max adc data
 * @u8DatLow: the min adc data
 *
 * Returns None
 */
void MHal_ADCWDTConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Channel, u8 u8DatHigh, u8 u8DatLow)
{
    WRITE_WORD(sstar_out_chip->base + PWM_ADC_WDT_OFFSET + ((u8Channel * 2 + 1) << 2), u8DatHigh);
    WRITE_WORD(sstar_out_chip->base + PWM_ADC_WDT_OFFSET + ((u8Channel * 2) << 2), u8DatLow);
}

/**
 * MHal_ADCWDTEnable - enable watchdog funtion
 * @sstar_chip: struct sstar_pwm_chip
 * @u8Channel: ADC channel
 * @u8Enable: 1: enable, 0: disable
 *
 * Returns None
 */
void MHal_ADCWDTEnable(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Channel, u8 u8Enable)
{
    if (u8Enable)
    {
        if (u8Channel < 16)
            SET_WORD(sstar_out_chip->base + PWM_ADC_WDT_OFFSET + u16REG_ADC_WDT_EN, BIT0 << u8Channel);
        else
            SET_WORD(sstar_out_chip->base + PWM_ADC_WDT_OFFSET + u16REG_ADC_WDT_EN, BIT0 << (u8Channel - 16));
    }
    else
    {
        if (u8Channel < 16)
            CLR_WORD(sstar_out_chip->base + PWM_ADC_WDT_OFFSET + u16REG_ADC_WDT_EN, BIT0 << u8Channel);
        else
            CLR_WORD(sstar_out_chip->base + PWM_ADC_WDT_OFFSET + u16REG_ADC_WDT_EN, BIT0 << (u8Channel - 16));
    }
}

/**
 * MHal_ADCEnable - PWM adc enable or disable
 * @sstar_chip: struct sstar_pwm_chip
 * @u8Enable: 1: enable, 0: disable
 *
 * Returns None
 */
void MHal_ADCEnable(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Enable)
{
    if (u8Enable)
    {
        SET_WORD(sstar_out_chip->base + u16REG_PWM_ADC_EN, ADC_EN_BG_BIT);
        SET_WORD(sstar_out_chip->base + u16REG_PWM_SAR_EN, SAR_EN_BIT);
    }
    else
    {
        CLR_WORD(sstar_out_chip->base + u16REG_PWM_ADC_EN, ADC_EN_BG_BIT);
        CLR_WORD(sstar_out_chip->base + u16REG_PWM_SAR_EN, SAR_EN_BIT);
    }
}

/**
 * MHal_ADCRegSeqConfig - PWM adc regular channel config
 * @sstar_chip: struct sstar_pwm_chip
 * @u8SeqLen: regular sequence length
 * @u8Id: sequence list index
 * @u8Channel: adc channel
 *
 * Returns None
 */
void MHal_ADCRegSeqConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8SeqLen, u8 u8Id, u8 u8Channel)
{
    u32 u32SeqLen;

    if (u8Channel < 16)
    {
        SET_WORD(sstar_out_chip->base + u16REG_GPIO_EN_0, BIT0 << u8Channel);
    }
    else
    {
        SET_WORD(sstar_out_chip->base + u16REG_GPIO_EN_1, BIT0 << (u8Channel - 16));
    }
    u32SeqLen = READ_WORD(sstar_out_chip->base + u16REG_ADC_SEQ_LEN);
    WRITE_WORD(sstar_out_chip->base + u16REG_ADC_SEQ_LEN, u32SeqLen | ((u8SeqLen - 1) << REGU_SEQ_LEN_OFFSET));
    WRITE_WORD_MASK(sstar_out_chip->base + u16REG_REGU_ADC_SEQ_1 + ((u8Id / 3) << 2),
                    (u8Channel << ((u8Id % 3) * REGU_SEQ_LIST_OFFSET)), (0x1F << ((u8Id % 3) * REGU_SEQ_LIST_OFFSET)));
}

/**
 * MHal_ADCInjSeqConfig - PWM adc inject channel config
 * @sstar_chip: struct sstar_pwm_chip
 * @u8SeqLen: inject sequence length
 * @u8Id: sequence list index
 * @u8Channel: adc channel
 *
 * Returns None
 */
void MHal_ADCInjSeqConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8SeqLen, u8 u8Id, u8 u8Channel)
{
    u32 u32SeqLen;

    if (u8Channel < 16)
    {
        SET_WORD(sstar_out_chip->base + u16REG_GPIO_EN_0, BIT0 << u8Channel);
    }
    else
    {
        SET_WORD(sstar_out_chip->base + u16REG_GPIO_EN_1, BIT0 << (u8Channel - 16));
    }

    u32SeqLen = READ_WORD(sstar_out_chip->base + u16REG_ADC_SEQ_LEN);
    WRITE_WORD(sstar_out_chip->base + u16REG_ADC_SEQ_LEN, u32SeqLen | ((u8SeqLen - 1) << INJ_SEQ_LEN_OFFSET));
    WRITE_WORD_MASK(sstar_out_chip->base + u16REG_INJ_ADC_SEQ_1 + ((u8Id / 3) << 2),
                    (u8Channel << ((u8Id % 3) * INJ_SEQ_LIST_OFFSET)), (0x1F << ((u8Id % 3) * INJ_SEQ_LIST_OFFSET)));
}

/**
 * MHal_ADCRegConMode - set regular ADC conversion mode
 * @sstar_chip: struct sstar_pwm_chip
 * @u8ModeSelct: channel conversion mode, 0:single, 1:continuous
 *
 * Returns None
 */
void MHal_ADCRegConMode(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8ModeSelct)
{
    if (u8ModeSelct)
    {
        SET_WORD(sstar_out_chip->base + u16REG_PWM_SAR_EN, REGU_CON_MODE_BIT); // continuous mode
    }
    else
    {
        CLR_WORD(sstar_out_chip->base + u16REG_PWM_SAR_EN, REGU_CON_MODE_BIT); // single mode
    }
}

/**
 * MHal_ADCInjConMode - set inject ADC conversion mode
 * @sstar_chip: struct sstar_pwm_chip
 * @u8ModeSelct: channel conversion mode, 0:single, 1:continuous
 *
 * Returns None
 */
void MHal_ADCInjConMode(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8ModeSelct)
{
    if (u8ModeSelct)
    {
        SET_WORD(sstar_out_chip->base + u16REG_PWM_SAR_EN, INJ_CON_MODE_BIT); // continuous mode
    }
    else
    {
        CLR_WORD(sstar_out_chip->base + u16REG_PWM_SAR_EN, INJ_CON_MODE_BIT); // single mode
    }
}

/**
 * MHal_ADCRegStr - PWM trigger start or end
 * @irq: struct sstar_pwm_chip
 * @data: sstar_pwm_chip
 *
 * Returns None
 */
void MHal_ADCRegStr(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Enable)
{
    if (u8Enable)
    {
        SET_WORD(sstar_out_chip->base + u16REG_PWM_SAR_EN, REGU_STR_BIT);
    }
    else
    {
        SET_WORD(sstar_out_chip->base + u16REG_PWM_SAR_EN, REGU_END_BIT);
    }
}

/**
 * MHal_ADCInjStr - PWM interrupt service function
 * @irq: struct sstar_pwm_chip
 * @data: sstar_pwm_chip
 *
 * Returns None
 */
void MHal_ADCInjStr(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Enable)
{
    if (u8Enable)
    {
        SET_WORD(sstar_out_chip->base + u16REG_PWM_SAR_EN, INJ_STR_BIT);
    }
    else
    {
        SET_WORD(sstar_out_chip->base + u16REG_PWM_SAR_EN, INJ_END_BIT);
    }
}

/**
 * MHal_ADCRegTriConfig - PWM trigger config
 * @sstar_chip: struct sstar_pwm_chip
 * @u8TriMode: trigger mode, 0~b pwm tri, c sw tri, d external tri, f freerun
 * @u8TriMethod: 0: conversion, 1: sequence
 *
 * Returns None
 */
void MHal_ADCRegTriConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8TriMode, u8 u8TriMethod)
{
    if (u8TriMethod)
    {
        SET_WORD(sstar_out_chip->base + u16REG_ADC_TRI, REGU_TRI_MET_BIT);
    }
    else
    {
        CLR_WORD(sstar_out_chip->base + u16REG_ADC_TRI, REGU_TRI_MET_BIT);
    }
    WRITE_WORD_MASK(sstar_out_chip->base + u16REG_ADC_TRI, u8TriMode, BIT0 | BIT1 | BIT2 | BIT3);
}

/**
 * MHal_ADCInjTriConfig - PWM trigger config
 * @sstar_chip: struct sstar_pwm_chip
 * @u8TriMode: trigger mode, 0~b pwm tri, c sw tri, d external tri
 * @u8TriMethod: 0: conversion, 1: sequence
 *
 * Returns None
 */
void MHal_ADCInjTriConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8TriMode, u8 u8TriMethod)
{
    if (u8TriMethod)
    {
        SET_WORD(sstar_out_chip->base + u16REG_ADC_TRI, INJ_TRI_MET_BIT);
    }
    else
    {
        CLR_WORD(sstar_out_chip->base + u16REG_ADC_TRI, INJ_TRI_MET_BIT);
    }
    WRITE_WORD_MASK(sstar_out_chip->base + u16REG_ADC_TRI, (u8TriMode << 4), BIT4 | BIT5 | BIT6 | BIT7);
}

/**
 * MHal_ADCInjTriDelay - PWM adc inject trigger delay count setting
 * @sstar_chip: struct sstar_pwm_chip
 * @u8PTriDelay: pwm_p triger delay conut
 * @u8NTriDelay: pwm_n triger delay conut
 *
 * Returns None
 */
void MHal_ADCInjTriDelay(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8PTriDelay, u8 u8NTriDelay)
{
    WRITE_WORD(sstar_out_chip->base + u16REG_PWMP_TRI_DELAY_L, u8PTriDelay & 0xFFFF);
    WRITE_WORD(sstar_out_chip->base + u16REG_PWMP_TRI_DELAY_H, (u8PTriDelay >> 16) & 0x3);
    WRITE_WORD(sstar_out_chip->base + u16REG_PWMN_TRI_DELAY_L, u8NTriDelay);
    WRITE_WORD(sstar_out_chip->base + u16REG_PWMN_TRI_DELAY_H, (u8NTriDelay >> 16) & 0x3);
}

/**
 * MHal_ADCDMAConfig - PWM adc dma mode config
 * @sstar_chip: struct sstar_pwm_chip
 * @u8MiuAddr: miu address
 * @u8MiuSize: miu size
 *
 * Returns None
 */
void MHal_ADCDMAConfig(struct sstar_pwm_out_chip *sstar_out_chip, u32 u8MiuSize)
{
    WRITE_WORD(sstar_out_chip->piubase + u16REG_MIU_SIZE_L, (u8MiuSize)&0xFFFF);
    WRITE_WORD(sstar_out_chip->piubase + u16REG_MIU_SIZE_H, (u8MiuSize >> 16) & 0x000F);
    _miu_addr = CamOsMemAlloc(u8MiuSize);

    WRITE_WORD(sstar_out_chip->piubase + u16REG_MIU_ADDR_L, ((u32)CamOsMemPhysToMiu(CamOsMemVirtToPhys(_miu_addr))) & 0xFFFF);
    WRITE_WORD(sstar_out_chip->piubase + u16REG_MIU_ADDR_M, ((u32)CamOsMemPhysToMiu(CamOsMemVirtToPhys(_miu_addr)) >> 16) & 0xFFFF);
}

/**
 * MHal_ADCDMAEnable - PWM adc dma mode config
 * @sstar_chip: struct sstar_pwm_chip
 * @u8DatAlign: 1: left align, 0: right align
 * @u8Enable: 1:enable, 0: disable
 *
 * Returns None
 */
void MHal_ADCDMAEnable(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8DatAlign, u8 u8Enable)
{
    if (u8DatAlign)
    {
        CLR_WORD(sstar_out_chip->base + u16REG_PWM_SAR_EN, DATA_ALIGN);
    }
    else
    {
        SET_WORD(sstar_out_chip->base + u16REG_PWM_SAR_EN, DATA_ALIGN);
    }

    if (u8Enable)
    {
        SET_WORD(sstar_out_chip->piubase + u16REG_ADC_DMA_EN, ADC_SW_RST_BIT | DMA_EN_BIT);
    }
    else
    {
        CLR_WORD(sstar_out_chip->piubase + u16REG_ADC_DMA_EN, ADC_SW_RST_BIT | DMA_EN_BIT);
    }
}

/**
 * MHal_ADCFreerun - PWM adc freerun mode
 * @sstar_chip: struct sstar_pwm_chip
 * @u8AveCnt: 0: 1 time, 1: 4 times, 2: 8 times, 3: 16 times
 * @u32OneshotPrd: oneshot length
 *
 * Returns None
 */
void MHal_ADCFreerun(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8AveCnt, u32 u32OneshotPrd)
{
    u32 u32Average;
    u32Average = READ_WORD(sstar_out_chip->base + u16REG_ADC_AVE_CNT);
    WRITE_WORD(sstar_out_chip->base + u16REG_ADC_AVE_CNT, u32Average | (u8AveCnt << AVE_CNT_OFFSET));
    WRITE_WORD(sstar_out_chip->base + u16REG_ADC_TRI, 0xE);
    SET_WORD(sstar_out_chip->base + u16REG_ADC_TRI, REGU_TRI_MET_BIT);
    WRITE_WORD(sstar_out_chip->base + u16REG_ONESHOT_PRD, u32OneshotPrd);
}

/**
 * MHal_ADCSWTri - PWM conversion sw trigger
 * @sstar_chip: struct sstar_pwm_chip
 * @u8AdcMode: 1: regular trigger, 0: inject trigger
 *
 * Returns None
 */
void MHal_ADCSWTri(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8AdcMode)
{
    if (u8AdcMode)
    {
        SET_WORD(sstar_out_chip->base + u16REG_ADC_TRI, REGU_SW_TRI_BIT);
    }
    else
    {
        SET_WORD(sstar_out_chip->base + u16REG_ADC_TRI, INJ_SW_TRI_BIT);
    }
}

/**
 * MHal_ADCData - Get ADC channel data
 * @sstar_out_chip: struct sstar_pwm_chip
 * @u8Channel: ADC channel
 *
 * Returns None
 */
u32 MHal_ADCData(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Channel, u16 *u16AdcData)
{
    SET_WORD(sstar_out_chip->base + u16REG_LOAD_EN, LOAD_EN_BIT);
    *u16AdcData = READ_WORD(sstar_out_chip->base + u16REG_ADC_DATA + (u8Channel << 2))
        SSTAR_PWM_OUT_DBG("The ADC[%d] data is %d\n", u8Channel, u16AdcData);
    return 1;
}


/**
 * MHal_ADCDMAData - Get ADC channel data in dma mode
 * @sstar_out_chip: struct sstar_pwm_chip
 * @u8Selctor: 1: enable, 0: disbale
 *
 * Returns None
 */
u32 MHal_ADCDMAData(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Selctor)
{
    int index;
    u32 miu_size;
    u32 ddr_data = 0;

    if (u8Selctor)
    {
        miu_size = READ_WORD(sstar_out_chip->piubase + u16REG_MIU_SIZE_L)
                   | (READ_WORD(sstar_out_chip->piubase + u16REG_MIU_SIZE_H) << 16);
        CamOsMemInvalidate(_miu_addr, miu_size);
        for (index = 0; index < (miu_size / 2); index++)
        {
            ddr_data = (*(volatile u16 *)((_miu_addr + (index * 2))));
            SSTAR_PWM_OUT_INFO("The ADC data is %x\n", ddr_data);
        }
        CamOsMemRelease(_miu_addr);
    }
    return 1;
}
