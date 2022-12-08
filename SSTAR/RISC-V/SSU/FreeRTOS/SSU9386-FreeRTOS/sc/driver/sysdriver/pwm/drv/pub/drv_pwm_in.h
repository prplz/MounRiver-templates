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
#ifndef __DRV_PWM_IN_H__
#define __DRV_PWM_IN_H__

#include "vm_types.ht"
#include "cam_os_wrapper.h"
#ifdef CONFIG_CAM_CLK
#include "drv_camclk_DataType.h"
#endif

struct sstar_pwm_in_chip
{
#ifdef CONFIG_ARM64
    u64 base;
#else
    u32         base;
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

void DrvPWMINEnable(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8PwmId, u8 u8Enable);
void DrvPWMINRstMode(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8PwmId, u8 u8Enable);
void DrvPWMINPulCal(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Enable);
void DrvPWMINDetMode(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Enable);
void DrvPWMINTimeDiv(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8DivNum);
void DrvPWMINEdgeSel(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Edge);
void DrvPWMINPulDiv(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Div);
void DrvPWMINPulCalEdge(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Edge);
void DrvPWMINGet(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u32 *u32Period, u32 *u32DutyI, u32 *u32DutyD);
void DrvPWMINGetPul(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id);
struct sstar_pwm_in_chip* PwmInGetPtr(void);

#endif /* __DRV_PWM_OUT_H__ */
