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
#ifndef __DRV_PWM_OUT_H__
#define __DRV_PWM_OUT_H__

#include "vm_types.ht"
#include "cam_os_wrapper.h"
#ifdef CONFIG_CAM_CLK
#include "drv_camclk_DataType.h"
#endif

#define __va(x) ((void *)(x))

struct sstar_pwm_out_chip
{
#ifdef CONFIG_ARM64
    u64 base;
    u64 piubase;
#else
    u32         base;
    u32         piubase;
#endif
#ifdef CONFIG_CAM_CLK
    void*                pvPwmClk;
    CAMCLK_Set_Attribute stSetCfg;
    CAMCLK_Get_Attribute stGetCfg;
#else
    struct clk *clk;
#endif
    u32 clk_fre;
    int irq;
};


typedef struct
{
    u32     regbase;
    u16     regbanksize;
} __attribute__ ((__packed__)) reg_desc_t;

void DrvDdtPConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 PwmId, u32 DdtLen);
void DrvDdtNConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 PwmId, u32 DdtLen);
void DrvOutPConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 PwmId, u8 Enable);
void DrvOutNConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 PwmId, u8 Enable);
void DrvOutEnable(struct sstar_pwm_out_chip *sstar_out_chip, u8 PwmId, u8 Enable);
void DrvBkinConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 PwmId, u8 ModeSel, u8 Status);
void DrvBkinTri(struct sstar_pwm_out_chip *sstar_out_chip, u8 PwmId);
void adc_enable(struct sstar_pwm_out_chip *sstar_out_chip, u8 enable);
void adc_regu_seq_config(struct sstar_pwm_out_chip *sstar_out_chip, u8 SeqLen, u8 u8Id, u8 u8Channel);
void adc_regu_tri_sel(struct sstar_pwm_out_chip *sstar_out_chip, u8 trimode, u8 trimethod);
void adc_regu_con_sel(struct sstar_pwm_out_chip *sstar_out_chip, u8 conmode_sel);
void adc_regu_tri_star(struct sstar_pwm_out_chip *sstar_out_chip, u8 enable);
void adc_inj_seq_config(struct sstar_pwm_out_chip *sstar_out_chip, u8 SeqLen, u8 u8Id, u8 u8Channel);
void adc_inj_tri_sel(struct sstar_pwm_out_chip *sstar_out_chip, u8 trimode, u8 trimethod);
void adc_inj_con_sel(struct sstar_pwm_out_chip *sstar_out_chip, u8 conmode_sel);
void adc_inj_tri_star(struct sstar_pwm_out_chip *sstar_out_chip, u8 enable);
void adc_dma_config(struct sstar_pwm_out_chip *sstar_out_chip, u32 miusize);
void adc_dma_enable(struct sstar_pwm_out_chip *sstar_out_chip, u8 align, u8 enable);
void adc_wdt_config(struct sstar_pwm_out_chip *sstar_out_chip, u8 channelid, u8 datahigh, u8 datalow);
void adc_wdt_enable(struct sstar_pwm_out_chip *sstar_out_chip, u8 channelid, u8 enable);
void adc_freerun(struct sstar_pwm_out_chip *sstar_out_chip, u8 avecnt, u32 oneshotprd);
void adc_data(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Channel, u16 *u8AdcData);
void adc_dma_data(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Selctor);
struct sstar_pwm_out_chip* PwmOutGetPtr(void);

#endif /* __DRV_PWM_OUT_H__ */
