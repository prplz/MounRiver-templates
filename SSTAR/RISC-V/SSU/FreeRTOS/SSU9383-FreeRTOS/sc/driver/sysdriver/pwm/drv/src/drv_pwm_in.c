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
#include "hal_pwm_in.h"
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
static struct sstar_pwm_in_chip* ptr_sstar_pwm_in;

void DrvPWMINEnable(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8PwmId, u8 u8Enable)
{
    MHal_PWMInCapEn(sstar_in_chip, u8PwmId, u8Enable);
}

void DrvPWMINRstMode(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8PwmId, u8 u8Enable)
{
    MHal_PWMInRstMode(sstar_in_chip, u8PwmId, u8Enable);
}

void DrvPWMINPulCal(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Enable)
{
    MHal_PWMInPulCalMode(sstar_in_chip, u8Id, u8Enable);
}

void DrvPWMINDetMode(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Enable)
{
    MHal_PWMInDetMode(sstar_in_chip, u8Id, u8Enable);
}

void DrvPWMINTimeDiv(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8DivNum)
{
    MHal_PWMInTimerDiv(sstar_in_chip, u8Id, u8DivNum);
}

void DrvPWMINEdgeSel(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Edge)
{
    MHal_PWMInEdgeSel(sstar_in_chip, u8Id, u8Edge);
}

void DrvPWMINPulDiv(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Div)
{
    MHal_PWMInPulDiv(sstar_in_chip, u8Id, u8Div);
}

void DrvPWMINPulCalEdge(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Edge)
{
    MHal_PWMInPulCalEdge(sstar_in_chip, u8Id, u8Edge);
}

void DrvPWMINGet(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u32 *u32Period, u32 *u32DutyI, u32 *u32DutyD)
{
    MHal_PWMInGet(sstar_in_chip, u8Id, u32Period, u32DutyI, u32DutyD);
}
void DrvPWMINGetPul(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id)
{
    MHal_PWMInPulNumGet(sstar_in_chip, u8Id);
}

struct sstar_pwm_in_chip* PwmInGetPtr(void)
{
    return ptr_sstar_pwm_in;
}

static s32 pwm_in_probe(void)
{
    reg_desc_t sRegdesc[2];
    int IrqNum[2] = {0, 0};
    u32 clk_frequency = 0;
    u8 i;
#ifdef CONFIG_CAM_CLK
    u32 ret_clk;
    u16 PwminClkId;
#endif
    struct sstar_pwm_in_chip *sstar_pwm_in = NULL;
#if defined(CONFIG_SYSDESC_SUPPORT)
    u32 ret_desc;
    u16 u16Size = 0;
    u16 u16Remainder = 0;
#endif
    sstar_pwm_in = CamOsMemAlloc(sizeof(struct sstar_pwm_in_chip));
    if (!sstar_pwm_in)
    {
        SSTAR_PWM_IN_ERR("%s:%d CamOsMemAlloc() failed\n", __func__, __LINE__);
    }
    memset(sRegdesc, 0, sizeof(reg_desc_t) * 2);

#if defined(SYSDESC_PRO_status_u8)
        if (E_SYS_DESC_PASS != MDrv_SysDesc_Read_U8_Array(SYSDESC_DEV_pwm_in, SYSDESC_PRO_status_u8, &i, 1) || !i)
        {
            SSTAR_PWM_IN_DBG("[%s][pwm_in] sysdesc no enable!\n", __func__);
            return -1;
        }
#else
        SSTAR_PWM_IN_DBG("[%s][pwm_in] sysdesc no enable!\n", __func__);
        return -1;
#endif

#if defined(CONFIG_SYSDESC_SUPPORT)
    MDrv_SysDesc_GetElementCount(SYSDESC_DEV_pwm_in, SYSDESC_PRO_reg_u32_u16, &u16Size, &u16Remainder);
    ret_desc = MDrv_SysDesc_Read_MultiTypes(SYSDESC_DEV_pwm_in, SYSDESC_PRO_reg_u32_u16, (void *)sRegdesc, sizeof(reg_desc_t), sizeof(reg_desc_t)*u16Size);
    if (ret_desc != E_SYS_DESC_PASS)
    {
        SSTAR_PWM_IN_ERR("Fail to get register address\n");
    }
    MDrv_SysDesc_GetElementCount(SYSDESC_DEV_pwm_in, SYSDESC_PRO_interrupts_u8, &u16Size,&u16Remainder);
    ret_desc = MDrv_SysDesc_Read_MultiTypes(SYSDESC_DEV_pwm_in, SYSDESC_PRO_interrupts_u8, (void *)IrqNum, 1, u16Size);
    if (ret_desc != E_SYS_DESC_PASS)
    {
        SSTAR_PWM_IN_ERR("Fail to get interrupts number\n");
    }
    ret_desc = MDrv_SysDesc_Read_U32(SYSDESC_DEV_pwm_in, SYSDESC_PRO_clkfrequency_u32, &clk_frequency);
    if (ret_desc != E_SYS_DESC_PASS)
    {
        SSTAR_PWM_IN_ERR("Fail to get clock frequency\n");
    }
#else
    sRegdesc[0].regbase = 0x2201A00;
    sRegdesc[0].regbanksize = 0x200;
    IrqNum[0] = INT_IRQ_PWM_CAPTURE;
    clk_frequency = 12000000;
#endif

    sstar_pwm_in->base = sRegdesc[0].regbase;
    sstar_pwm_in->irq = IrqNum[0];
    sstar_pwm_in->clk_fre = clk_frequency;
    ptr_sstar_pwm_in = sstar_pwm_in;

#ifdef CONFIG_CAM_CLK
    ret_clk = MDrv_SysDesc_Read_U16(SYSDESC_DEV_pwm_in, SYSDESC_PRO_camclk_u16, &PwminClkId);
    if (ret_clk != E_SYS_DESC_PASS)
    {
        SSTAR_PWM_IN_ERR("Fail to get pwm in CAMCLK ID\n");
    }
    ret_clk = CamClkRegister((u8* )"PWM_IN", PwminClkId, &sstar_pwm_in->pvPwmClk);
    if (ret_clk == CAMCLK_RET_OK)
    {
        CamClkAttrGet(sstar_pwm_in->pvPwmClk, &sstar_pwm_in->stGetCfg);
        CAMCLK_SETRATE_ROUND(sstar_pwm_in->stSetCfg, sstar_pwm_in->clk_fre);
        CamClkAttrSet(sstar_pwm_in->pvPwmClk, &sstar_pwm_in->stSetCfg);
        CamClkSetOnOff(sstar_pwm_in->pvPwmClk, 1);
    }
    else
    {
        SSTAR_PWM_IN_ERR("CamClkRegister fail\n");
    }
#else
    MHal_PWMIN_SetClock();
#endif
    return 0;
}

void pwm_in_init(void)
{
    pwm_in_probe();
}

rtos_application_initcall(pwm_in_init, 3);
