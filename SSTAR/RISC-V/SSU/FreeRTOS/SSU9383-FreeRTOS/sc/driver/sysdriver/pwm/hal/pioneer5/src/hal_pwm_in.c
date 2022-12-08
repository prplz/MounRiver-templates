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

/**
 * MHal_PWMIN_SetClock - enable PWM IN clock
 * @void
 *
 * Returns None.
 */
void MHal_PWMIN_SetClock(void)
{
    if ((READ_WORD(BASE_REG_CLKGEN_PA + PWM_CLK_OFFSET) & BIT4))
    {
        WRITE_WORD_MASK(BASE_REG_CLKGEN_PA + PWM_CLK_OFFSET, 0x00, BIT4 | BIT5 | BIT6 | BIT7);
        SSTAR_PWM_IN_INFO("[PWMIN]CfgClk(12M)\r\n");
    }
}

/**
 * MHal_PWMGetPWMINAddr - get the bank addr and offset addr of different PWM input channel
 * @sstar_chip: struct sstar_pwm_chip
 * @u32addr: bank address
 * @u32PwmOffs: offset address
 * @u8Id: PWM input channel index
 *
 * Returns None.
 */
static void MHal_PWMGetPWMINAddr(u32 *u32PwmOffs, u8 u8Id)
{
    if (u8Id >= PWM_IN_NUM)
        return;
    *u32PwmOffs = (u8Id / 2) * PWM_IN_CHAN_OFFSET;
}

/**
 * MHal_PWMInCapEn - PWM input capture enable
 * @sstar_chip: struct sstar_pwm_chip
 * @u8Id: 1: PWM input channel index
 * @u8Enable: 1:enable, 0: disable
 *
 * Returns None
 */
void MHal_PWMInCapEn(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Enable)
{
    u32 u32PwmOffs = 0;

    MHal_PWMGetPWMINAddr(&u32PwmOffs, u8Id);
    if (u8Enable)
    {
        WRITE_WORD_MASK(sstar_in_chip->base + u32PwmOffs + u16REG_CAPTURE_EN, CAPTURE_EN | SW_RST, CAPTURE_EN | SW_RST);
    }
    else
    {
        WRITE_WORD_MASK(sstar_in_chip->base + u32PwmOffs + u16REG_CAPTURE_EN, SW_RST, CAPTURE_EN | SW_RST);
    }
    if (u8Id % 2)
    {
        WRITE_WORD(sstar_in_chip->base + u32PwmOffs + u16REG_GLHRM_DIVIDE_CH1, PWM_IN_GLHRM_DIV);
        WRITE_WORD(sstar_in_chip->base + u32PwmOffs + u16REG_GLHRM_EN_CH1, (PWM_IN_GLHRM_NUM << 1) | GLHRM_EN_CH1_BIT);
    }
    else
    {
        WRITE_WORD(sstar_in_chip->base + u32PwmOffs + u16REG_GLHRM_DIVIDE_CH0, PWM_IN_GLHRM_DIV);
        WRITE_WORD(sstar_in_chip->base + u32PwmOffs + u16REG_GLHRM_EN_CH0, (PWM_IN_GLHRM_NUM << 1) | GLHRM_EN_CH0_BIT);
    }
}

/**
 * MHal_PWMInCapRstMode - PWM input capture reset mode enable, clear timer at the
 * beginning of each valid pulse
 * @sstar_chip: struct sstar_pwm_chip
 * @u8Id: 1: PWM input channel index
 * @u8Enable: 1:enable, 0: disable
 *
 * Returns None
 */
void MHal_PWMInRstMode(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Enable)
{
    u32 u32PwmOffs = 0;

    MHal_PWMGetPWMINAddr(&u32PwmOffs, u8Id);
    WRITE_WORD_MASK(sstar_in_chip->base + u32PwmOffs + u16REG_CAPTURE_EN, ((u8Id % 2) ? (RESET_MODE_SEL) : 0) | SW_RST,
                    RESET_MODE_SEL | SW_RST);
    if (u8Enable)
    {
        WRITE_WORD_MASK(sstar_in_chip->base + u32PwmOffs + u16REG_CAPTURE_EN, RESET_MODE_EN | SW_RST,
                        RESET_MODE_EN | SW_RST);
    }
    else
    {
        WRITE_WORD_MASK(sstar_in_chip->base + u32PwmOffs + u16REG_CAPTURE_EN, SW_RST, RESET_MODE_EN | SW_RST);
    }
}

/**
 * MHal_PWMInPulCalMode - PWM input pulse number calculate enable
 * @sstar_chip: struct sstar_pwm_chip
 * @u8Id: 1: PWM input channel index
 * @u8Enable: 1:enable, 0: disable
 *
 * Returns None
 */
void MHal_PWMInPulCalMode(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Enable)
{
    u32 u32PwmOffs = 0;

    MHal_PWMGetPWMINAddr(&u32PwmOffs, u8Id);
    WRITE_WORD_MASK(sstar_in_chip->base + u32PwmOffs + u16REG_CAPTURE_EN, ((u8Id % 2) ? (PULSE_CNT_SEL) : 0) | SW_RST,
                    PULSE_CNT_SEL | SW_RST);
    if (u8Enable)
    {
        WRITE_WORD_MASK(sstar_in_chip->base + u32PwmOffs + u16REG_CAPTURE_EN, PULSE_CNT_EN | SW_RST,
                        PULSE_CNT_EN | SW_RST);
    }
    else
    {
        WRITE_WORD_MASK(sstar_in_chip->base + u32PwmOffs + u16REG_CAPTURE_EN, SW_RST, PULSE_CNT_EN | SW_RST);
    }
}

/**
 * MHal_PWMInCapRstMode - PWM input capture detect mode enable, select input pulse
 * from other channal
 * @sstar_chip: struct sstar_pwm_chip
 * @u8Id: 1: PWM input channel index
 * @u8Enable: 1:enable, 0: disable
 *
 * Returns None
 */
void MHal_PWMInDetMode(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Enable)
{
    u32 u32PwmOffs = 0;

    MHal_PWMGetPWMINAddr(&u32PwmOffs, u8Id);
    if (u8Enable)
    {
        WRITE_WORD_MASK(sstar_in_chip->base + u32PwmOffs + u16REG_CAPTURE_EN, DETECT_MODE | SW_RST,
                        DETECT_MODE | SW_RST);
    }
    else
    {
        WRITE_WORD_MASK(sstar_in_chip->base + u32PwmOffs + u16REG_CAPTURE_EN, SW_RST, DETECT_MODE | SW_RST);
    }
}

/**
 * MHal_PWMInTimerDiv - PWM input capture timer divide counter number
 * timer counter = clk/divreg, clk is 12M
 * @sstar_chip: struct sstar_pwm_chip
 * @u8Id: 1: PWM input channel index
 * @u8DivNum: divide counter number
 *
 * Returns None
 */
void MHal_PWMInTimerDiv(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8DivNum)
{
    u32 u32PwmOffs = 0;

    MHal_PWMGetPWMINAddr(&u32PwmOffs, u8Id);
    WRITE_WORD(sstar_in_chip->base + u32PwmOffs + u16REG_TIMER_DIV, u8DivNum);
    SET_WORD(sstar_in_chip->base + u32PwmOffs + u16REG_EDGE_SEL_CH0, BIT15);
}

/**
 * MHal_PWMInEdgeSel - PWM input capture select valid edge of input pulse
 * @sstar_chip: struct sstar_pwm_chip
 * @u8Id: 1: PWM input channel index
 * @u8Edge: 0: neg edge, 1: pos edge, 2: both
 *
 * Returns None
 */
void MHal_PWMInEdgeSel(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Edge)
{
    u32 u32PwmOffs = 0;

    MHal_PWMGetPWMINAddr(&u32PwmOffs, u8Id);
    u32PwmOffs = u32PwmOffs + ((u8Id % 2) ? u16REG_EDGE_SEL_CH1 : u16REG_EDGE_SEL_CH0);
    SET_WORD(sstar_in_chip->base + u32PwmOffs,
             (u8Edge < 2) ? (u8Edge ? POSEDGE_CH_BIT : NEGEDGE_CH_BIT) : (NEGEDGE_CH_BIT | POSEDGE_CH_BIT));
}

/**
 * MHal_PWMInPulDiv - set the divide of input pulse
 * @sstar_chip: struct sstar_pwm_chip
 * @u8Id: 1: PWM input channel index
 * @u8Div: 0: 1 div, 1: 2 div, 2: 4 div, 3: 8 div, 4: 16 div
 *
 * Returns None
 */
void MHal_PWMInPulDiv(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Div)
{
    u32 u32PwmOffs = 0;
    u32 u32Regval  = 0;

    MHal_PWMGetPWMINAddr(&u32PwmOffs, u8Id);
    if (u8Id % 2)
    {
        u32Regval = READ_WORD(sstar_in_chip->base + u32PwmOffs + u16REG_EDGE_SEL_CH1);
        WRITE_WORD((sstar_in_chip->base + u32PwmOffs + u16REG_EDGE_SEL_CH1),
                   u32Regval | (u8Div << PULSE_DIV_CH1_OFFSET));
    }
    else
    {
        u32Regval = READ_WORD(sstar_in_chip->base + u32PwmOffs + u16REG_EDGE_SEL_CH0);
        WRITE_WORD((sstar_in_chip->base + u32PwmOffs + u16REG_EDGE_SEL_CH0),
                   u32Regval | (u8Div << PULSE_DIV_CH0_OFFSET));
    }
}

/**
 * MHal_PWMInPulCalEdge - set the valid edge of input pulse of channal when calculate
 * pulse number
 * @sstar_chip: struct sstar_pwm_chip
 * @u8Id: 1: PWM input channel index
 * @u8Edge: 0: neg edge, 1: pos edge, 2: both
 *
 * Returns None
 */
void MHal_PWMInPulCalEdge(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Edge)
{
    u32 u32PwmOffs = 0;

    MHal_PWMGetPWMINAddr(&u32PwmOffs, u8Id);
    if (u8Id % 2)
    {
        SET_WORD(sstar_in_chip->base + u32PwmOffs + u16REG_PULSE_CNT_SEL,
                 (u8Edge < 2) ? (u8Edge ? CNT_POS_CH1_BIT : CNT_NEG_CH1_BIT) : (CNT_NEG_CH1_BIT | CNT_POS_CH1_BIT));
    }
    else
    {
        SET_WORD(sstar_in_chip->base + u32PwmOffs + u16REG_PULSE_CNT_SEL,
                 (u8Edge < 2) ? (u8Edge ? CNT_POS_CH0_BIT : CNT_NEG_CH0_BIT) : (CNT_NEG_CH0_BIT | CNT_POS_CH0_BIT));
    }
}

/**
 * MHal_PWMInGet - get the period of pwm input
 *
 * @sstar_chip: struct sstar_pwm_chip
 * @u8Id: 1: PWM input channel index
 *
 * Returns None
 */
void MHal_PWMInGet(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u32 *u32Period, u32 *u32DutyI, u32 *u32DutyD)
{
    u32 u32PwmOffs  = 0;
    u32 u32StartCnt = 0;
    u32 u32PshotCnt = 0;
    u32 u32NshotCnt = 0;
    u32 u32TimerDiv = 0;
    u32 u32PulseDiv = 0;
    u32 u32TimerClk = 0;
    u32 u32Common   = 0;
    u32 u32PeriodNs = 0;
    u32 u32DutyNs   = 0;
    u8  u8index     = 0;
    u8  u8Div       = 0;

    MHal_PWMGetPWMINAddr(&u32PwmOffs, u8Id);

    u32PulseDiv = 0;
    WRITE_WORD_MASK(sstar_in_chip->base + u32PwmOffs + u16REG_CAPTURE_EN, COUNT_LOAD_EN | SW_RST,
                    COUNT_LOAD_EN | SW_RST);
    if (u8Id % 2)
    {
        u32StartCnt = 0;
        u32PshotCnt = (READ_WORD(sstar_in_chip->base + u32PwmOffs + u16REG_PSHOT_CNT_CH1_L)
                       | (READ_WORD(sstar_in_chip->base + u32PwmOffs + u16REG_PSHOT_CNT_CH1_H) << 16))
                      + 1;
        u32NshotCnt = (READ_WORD(sstar_in_chip->base + u32PwmOffs + u16REG_NSHOT_CNT_CH1_L)
                       | (READ_WORD(sstar_in_chip->base + u32PwmOffs + u16REG_NSHOT_CNT_CH1_H) << 16))
                      + 1;
    }
    else
    {
        u32StartCnt = 0;
        u32PshotCnt = (READ_WORD(sstar_in_chip->base + u32PwmOffs + u16REG_PSHOT_CNT_CH0_L)
                       | (READ_WORD(sstar_in_chip->base + u32PwmOffs + u16REG_PSHOT_CNT_CH0_H) << 16))
                      + 1;
        u32NshotCnt = (READ_WORD(sstar_in_chip->base + u32PwmOffs + u16REG_NSHOT_CNT_CH0_L)
                       | (READ_WORD(sstar_in_chip->base + u32PwmOffs + u16REG_NSHOT_CNT_CH0_H) << 16))
                      + 1;
    }
    SSTAR_PWM_IN_DBG("u32StartCnt is %u ns, u32PshotCnt is %d, u32NshotCnt is %d\n", u32StartCnt, u32PshotCnt,
                     u32NshotCnt);
    u32TimerDiv = READ_WORD(sstar_in_chip->base + u32PwmOffs + u16REG_TIMER_DIV) + 1;
    u8Div       = 1;
    if (u8Id % 2)
    {
        u32PulseDiv = (READ_WORD(sstar_in_chip->base + u32PwmOffs + u16REG_EDGE_SEL_CH1) >> PULSE_DIV_CH1_OFFSET);
    }
    else
    {
        u32PulseDiv = (READ_WORD(sstar_in_chip->base + u32PwmOffs + u16REG_EDGE_SEL_CH0) >> PULSE_DIV_CH0_OFFSET);
    }
    for (u8index = 0; u8index < u32PulseDiv; u8index++)
    {
        u8Div *= 2;
    }
    u32PulseDiv = u8Div;

    switch (sstar_in_chip->clk_fre)
    {
        case 12000000:
            u32TimerClk = 3;
            u32Common   = 250;
            break;
        default:
            u32TimerClk = 3;
            u32Common   = 250;
    }

    u32PeriodNs = (u32PshotCnt > u32NshotCnt) ? (u32PshotCnt - u32StartCnt) : (u32NshotCnt - u32StartCnt);
    u32PeriodNs = u32PeriodNs / u32PulseDiv;
    u32DutyNs   = (u32PshotCnt > u32NshotCnt) ? (u32PeriodNs - u32PshotCnt + u32NshotCnt) : (u32NshotCnt - u32PshotCnt);

    SSTAR_PWM_IN_DBG("u32PeriodNs is %u ns, u32TimerDiv is %d, u32PulseDiv is %d\n", u32PeriodNs, u32TimerDiv,
                     u32PulseDiv);
    SSTAR_PWM_IN_DBG("The u32PeriodNs of input PWM is %d ns, the u32DutyNs of is %d\n", u32PeriodNs, u32DutyNs);

    *u32DutyD   = (10000 * u32DutyNs / u32PeriodNs % 100);
    u32DutyNs   = 100 * u32DutyNs / u32PeriodNs;
    u32PeriodNs = u32Common * u32TimerDiv * u32PeriodNs / u32TimerClk;

    *u32DutyI  = u32DutyNs;
    *u32Period = u32PeriodNs;
}


/**
 * MHal_PWMInPulNumGet - get the pulse number of pwm input
 *
 * @sstar_chip: struct sstar_pwm_chip
 * @u8Id: 1: PWM input channel index
 *
 * Returns None
 */
void MHal_PWMInPulNumGet(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id)
{
    u32 u32PulNum  = 0;
    u32 u32PwmOffs = 0;

    MHal_PWMGetPWMINAddr(&u32PwmOffs, u8Id);

    u32PulNum = READ_WORD(sstar_in_chip->base + u32PwmOffs + u16REG_PULSE_CNT);
    SSTAR_PWM_IN_INFO("The Pulse Number of input PWM is %d\n", u32PulNum);
}
