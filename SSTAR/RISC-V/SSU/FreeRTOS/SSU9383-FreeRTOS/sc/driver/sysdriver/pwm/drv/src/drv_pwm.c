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
#include "hal_pwm.h"

/*=============================================================*/
// Global Functions
/*=============================================================*/
#define _PWM_PRINT(fmt,...)                    UartSendTrace(fmt, ##__VA_ARGS__)
static int IRQ_Flag = 0;
void DrvPWMOutputPulse(u8 ePWNIndex,PWMConfig_t sPWMConfig)
{
    MHal_PWMOutputPulse(ePWNIndex, sPWMConfig);
}

void DrvPWMEnable(u8 ePWNIndex, u8 u8Enable)
{
    MHal_PWMEnable(ePWNIndex, u8Enable);
}

void DrvGroupModeIn(u8 ePWNIndex, u8 u8Enable)
{
    u8Enable = (u8Enable) ? 1 : 0;
    MHal_PWM_SetClock();
    MHal_PWMGroupJoin(ePWNIndex, u8Enable);
    MHal_PWMDiv(ePWNIndex, 1);
    MHal_PWMSetDben( ePWNIndex, 1);
    MHal_PWMEnable(ePWNIndex, 0);
}

void DrvGroupPeriodIn(u8 ePWNIndex, u32 u32Val)
{
    MHal_PWMSetPeriodEx(ePWNIndex, u32Val);
}

void DrvGroupPolarityIn(u8 ePWNIndex, u8 u8Val)
{
    MHal_PWMSetPolarityEx(ePWNIndex, u8Val);
}

void DrvGroupEndIn(u8 ePWNIndex, u8 u8DutyId, u32 u32Val)
{
    MHal_PWMSetEnd(ePWNIndex, u8DutyId, u32Val);
}

void DrvGroupBeginIn(u8 ePWNIndex, u8 u8ShftId, u32 u32Val)
{
    MHal_PWMSetBegin(ePWNIndex, u8ShftId, u32Val);
}

void DrvGroupRoundIn(u8 u8GroupId, U16 u16Val)
{
    MHal_PWMGroupSetRound(u8GroupId, u16Val);
}

void DrvGroupHold(u8 u8GroupId, U16 u16Val)
{
    MHal_PWMGroupHold(u8GroupId, u16Val);
}

void DrvGroupStop(u8 u8GroupId, U16 u16Val)
{
    MHal_PWMGroupStop(u8GroupId, u16Val);
}

void DrvGroupEnableIn(u8 u8GroupId, u8 u8Enable)
{
    u8 enable_status = 0;
    if (!MHal_PWMGroupIsEnable(u8GroupId, &enable_status))
    {
        _PWM_PRINT("unable to get enable status of group %d\n", u8GroupId);
        return;
    }
    u8Enable = (u8Enable) ? 1 : 0;
    if (u8Enable == enable_status)
    {
        _PWM_PRINT("cannot enable/disable group %d again. enable status = %d\n", u8GroupId, enable_status);
        return;
    }
    if (u8Enable)
    {
        MHal_PWMGroupStop(u8GroupId, 0);
        MHal_PWMGroupHold(u8GroupId, 0);
        MHal_PWMGroupEnable(u8GroupId, 1);
    }
    else
    {
        MHal_PWMGroupEnable(u8GroupId, 0);
        MHal_PWMGroupStop(u8GroupId, 0);
        MHal_PWMGroupHold(u8GroupId, 0);
    }
}

void DrvPWMIrqRequest(PWMConfig_t *sPWMConfig)
{
    if(IRQ_Flag == 0)
    {
        MHal_PWMIrqRequest(sPWMConfig);
        IRQ_Flag = 1;
    }
}
