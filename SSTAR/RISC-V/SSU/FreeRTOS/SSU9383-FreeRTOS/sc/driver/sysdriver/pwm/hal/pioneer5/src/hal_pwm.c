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

#include "hal_pwm.h"
#include "gpio.h"
#include "padmux.h"
#include "mhal_pinmux.h"
//------------------------------------------------------------------------------
//  Variables
//------------------------------------------------------------------------------
#define BASE_REG_NULL 0xFFFFFFFF
extern S32 HalPadSetVal(U32 u32PadID, U32 u32Mode);
extern s8 UartSendTrace(const char *strFormat, ...);
#define _PWM_PRINT(fmt,...)                    UartSendTrace(fmt, ##__VA_ARGS__)
#define CONFIG_PWM_NEW 1
typedef struct
{
    u32 u32Adr;
    u32 u32Val;
    u32 u32Msk;
} regSet_t;

typedef struct
{
    u32      u32PadId;
    u32    u32Mode;
} pwmPadTbl_t;

#define PADDEFALUT   (0x1F000000)
#define PMPADTOP     (0x3f << 9)
#define PAD_EXT_MODE (0x55 << 2)

static u8   _pwmEnSatus[PWM_NUM]                     = {0};
static u32  _pwmPeriod[PWM_NUM]                      = {0};
static u8   _pwmPolarity[PWM_NUM]                    = {0};
static u32  _pwmDuty[PWM_NUM][PWM_SHIFT_ARG_MAX_NUM] = {{0}}; // end  ( hardware support 0~4 set of duty )
static u8   _pwmDutyArgNum[PWM_NUM]                  = {0};
static u32  _pwmShft[PWM_NUM][PWM_SHIFT_ARG_MAX_NUM] = {{0}}; // begin ( hardware support 0~4 set of shift )
static u8   _pwmShftArgNum[PWM_NUM]                  = {0};
static u32  _pwmFreq[PWM_NUM]                        = {0};
static bool isSync                                   = 1; // isSync=0 --> need to sync register data from mem
#ifdef CONFIG_PWM_NEW
static u32 _pwmDiv[PWM_NUM]       = {0};
static u32 _pwmPeriod_ns[PWM_NUM] = {0};
static u32 _pwmDuty_ns[PWM_NUM]   = {0};
static U16 clk_pwm_div[7]         = {1, 2, 4, 8, 32, 64, 128};
#endif

void MHal_PWMPadSet(u8 u8Id, u32 PadMode)
{

    _PWM_PRINT("HalPWMPadSet (PadId, padMode) = (%d, %d)\n",  u8Id, PadMode);
    if(HalPadSetVal(u8Id, PadMode) == 0)
    {
        _PWM_PRINT("HalPadSetVal end\n");
    }
    else
    {
        _PWM_PRINT("HalPWMPadSet error!!!! (id: %x, pad: %x)\r\n", u8Id, PadMode);
    }
}

static void MHal_PWMGetGrpAddr(u32 *u32addr, u32 *u32PwmOffs, u8 u8Id)
{
    if (u8Id >= PWM_NUM)
        return;

    if (u8Id < 12)
    {
        *u32addr    = (U32)PWM_BANK;
        *u32PwmOffs = (u8Id < 4) ? (u8Id * 0x80) : ((4 * 0x80) + ((u8Id - 4) * 0x40));
    }
    else if (u8Id < 19)
    {
        *u32addr    = (U32)PWM_BANK;
        *u32PwmOffs = (u8Id < 13) ? ((12 * 0x80)) : ((12 * 0x80) + (0x20 * 4) + ((u8Id - 13) * 0x40));
    }
    else if (u8Id < 20)
    {
        *u32addr    = (U32)PWM_BANK;
        *u32PwmOffs = (0x50 * 4);
    }
    else if (u8Id < PWM_NUM)
    {
        *u32addr    = PWM_PM_BANK;
        *u32PwmOffs = (u8Id - 19) * 0x40;
    }
    else
        return;

}

void MHal_PWM_SetSyncFlag(bool bSync)
{
    isSync = bSync;
    // _PWM_PRINT(KERN_ERR "Sync:%d\r\n", isSync);
}

bool MHal_PWM_GetSyncFlag(void)
{
    return isSync;
}

void MHal_PWM_SetClock(void)
{
    if ((INREG16(BASE_REG_CLKGEN_PA + PWM_CLK_OFFSET) & BIT8))
    {
        OUTREGMSK16(BASE_REG_CLKGEN_PA + PWM_CLK_OFFSET, 0x00, BIT8 | BIT9 | BIT10 | BIT11 | BIT12);
        _PWM_PRINT("[PWM]CfgClk(12M)\r\n");
    }
}

#ifdef CONFIG_PWM_NEW
void MHal_PWMSetConfig(u8 u8Id, u32 duty, u32 period)
{
    U8  i;
    U16 u16Div     = 0;
    U32 common     = 0;
    U32 pwmclk     = 0;
    U32 periodmax  = 0;
    U32 u32Period  = 0x00000000;
    U32 u32Duty    = 0x00000000;
    U32 U32PwmAddr = 0;
    U32 u32PwmOffs = 0;

    if (u8Id >= PWM_NUM)
        return;

    MHal_PWMGetGrpAddr(&U32PwmAddr, &u32PwmOffs, u8Id);

    pwmclk = PWM_CLOCK;

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

    /*      select   div       */
    for (i = 0; i < (sizeof(clk_pwm_div) / sizeof(U16)); i++)
    {
        periodmax = (clk_pwm_div[i] * 262144 / pwmclk) * common;
        if (period < periodmax)
        {
            u16Div        = clk_pwm_div[i];
            _pwmDiv[u8Id] = clk_pwm_div[i];
            break;
        }
    }

    /*      select   period       */
    if (period < (0xFFFFFFFF / pwmclk))
    {
        u32Period = (pwmclk * period) / (u16Div * common);
        if (((pwmclk * period) % (u16Div * common)) > (u16Div * common / 2))
        {
            u32Period++;
        }
        _pwmPeriod_ns[u8Id] = (u32Period * u16Div * common) / pwmclk;
    }
    else
    {
        u32Period = (period / u16Div) * pwmclk / common;
        u32Period++;
        _pwmPeriod_ns[u8Id] = (u32Period * common / pwmclk) * u16Div;
    }

    /*      select   duty       */
    if (duty == 0)
    {
        if (_pwmEnSatus[u8Id])
        {
            if (u8Id < 12)
                SETREG16(U32PwmAddr + u16REG_SW_RESET_1, (BIT0 << (u8Id == 11 ? 14 : u8Id)));
            else if (u8Id < 20)
                SETREG16(U32PwmAddr + u16REG_SW_RESET_2, (BIT0 << (u8Id - 12)));
            else
                SETREG16(U32PwmAddr + u16REG_SW_RESET_1, (BIT0 << (u8Id - 20)));
        }
    }
    else
    {
        if (_pwmEnSatus[u8Id])
        {
            if (u8Id < 12)
                CLRREG16(U32PwmAddr + u16REG_SW_RESET_1, (BIT0 << (u8Id == 11 ? 14 : u8Id)));
            else if (u8Id < 20)
                CLRREG16(U32PwmAddr + u16REG_SW_RESET_2, (BIT0 << (u8Id - 12)));
            else
                CLRREG16(U32PwmAddr + u16REG_SW_RESET_1, (BIT0 << (u8Id - 20)));
        }
    }

    if (duty < (0xFFFFFFFF / pwmclk))
    {
        u32Duty = (pwmclk * duty) / (u16Div * common);
        if ((((pwmclk * duty) % (u16Div * common)) > (u16Div * common / 2)) || (u32Duty == 0))
        {
            u32Duty++;
        }
        _pwmDuty_ns[u8Id] = (u32Duty * u16Div * common) / pwmclk;
    }
    else
    {
        u32Duty = (duty / u16Div) * pwmclk / common;
        u32Duty++;
        _pwmPeriod_ns[u8Id] = (u32Duty * common / pwmclk) * u16Div;
    }

    /*      set  div period duty       */
    u16Div--;
    u32Period--;
    u32Duty--;
    _PWM_PRINT("clk=%d, u16Div=%d u32Duty=0x%x u32Period=0x%x,bank:0x%x,offset:0x%x\n", (U32)(PWM_CLOCK), u16Div,
                   u32Duty, u32Period,U32PwmAddr,u32PwmOffs);
    OUTREG16(U32PwmAddr + (u32PwmOffs) + u16REG_PWM_DIV, (u16Div & 0xFFFF));
    OUTREG16(U32PwmAddr + (u32PwmOffs) + u16REG_PWM_PERIOD_L, (u32Period & 0xFFFF));
    OUTREG16(U32PwmAddr + (u32PwmOffs) + u16REG_PWM_PERIOD_H, ((u32Period >> 16) & 0x3));
    OUTREG16(U32PwmAddr + (u32PwmOffs) + u16REG_PWM_DUTY_L, (u32Duty & 0xFFFF));
    OUTREG16(U32PwmAddr + (u32PwmOffs) + u16REG_PWM_DUTY_H, ((u32Duty >> 16) & 0x3));
}

void MHal_PWMGetConfig(u8 u8Id, u32 *Duty, u32 *Period)
{
    U16 u16Div     = 0;
    U32 u32Duty    = 0;
    U32 u32Period  = 0;
    U32 U32PwmAddr = 0;
    U32 pwmclk     = 0;
    U32 common     = 0;
    U32 u32PwmOffs = 0;
    MHal_PWMGetGrpAddr(&U32PwmAddr, &u32PwmOffs, u8Id);

    if (u8Id >= PWM_NUM)
        return;

    pwmclk = PWM_CLOCK;

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

    u16Div = INREG16(U32PwmAddr + (u32PwmOffs) + u16REG_PWM_DIV);
    u16Div++;

    if (Period != NULL)
    {
        if (_pwmPeriod_ns[u8Id] == 0)
        {
            u32Period = INREG16(U32PwmAddr + (u32PwmOffs) + u16REG_PWM_PERIOD_L)
                        | ((INREG16(U32PwmAddr + (u32PwmOffs) + u16REG_PWM_PERIOD_H) & 0x3) << 16);
            if (u32Period)
            {
                u32Period++;
            }
            _pwmPeriod_ns[u8Id] = (u32Period * u16Div * common) / pwmclk;
        }
        *Period = _pwmPeriod_ns[u8Id];
    }

    if (Duty != NULL)
    {
        if (_pwmDuty_ns[u8Id] == 0)
        {
            u32Duty = INREG16(U32PwmAddr + (u32PwmOffs) + u16REG_PWM_DUTY_L)
                      | ((INREG16(U32PwmAddr + (u32PwmOffs) + u16REG_PWM_DUTY_H) & 0x3) << 16);
            if (u32Duty)
            {
                u32Duty++;
            }
            _pwmDuty_ns[u8Id] = (u32Duty * u16Div * common) / pwmclk;
        }
        *Duty = _pwmDuty_ns[u8Id];
    }
}

#else
void MHal_PWMSetDuty(u8 u8Id, u32 u32Val)
{
    U32 u32Period = 0x00000000;
    U32 u32Duty = 0x00000000;
    U32 U32PwmAddr = 0;
    U32 u32PwmOffs = 0;
    MHal_PWMGetGrpAddr(&U32PwmAddr, &u32PwmOffs, u8Id);

    if (PWM_NUM <= u8Id)
        return;

    MHal_PWM_SetClock();
    if (_pwmEnSatus[u8Id])
    {
        if (0 == u32Val)
            if (u8Id < 12)
            {
                OUTREGMSK16(U32PwmAddr + u16REG_SW_RESET_1, BIT0 << (u8Id == 11 ? 14 : u8Id),
                            BIT0 << (u8Id == 11 ? 14 : u8Id));
            }
            else if (u8Id < 20)
            {
                OUTREGMSK16(U32PwmAddr + u16REG_SW_RESET_1, BIT0 << (u8Id - 12), BIT0 << (u8Id - 12));
            }
            else
            {
                OUTREGMSK16(U32PwmAddr + u16REG_SW_RESET_1, BIT0 << (u8Id - 20), BIT0 << (u8Id - 20));
            }
    }
    if (u32Val == 0)
    {
        u32Duty = 0;
    }
    else
    {
        if (u32Val > 100)
        {
            u32Val = 100;
        }
        u32Period = _pwmPeriod[u8Id];
        u32Period++;
        u32Duty = ((u32Period * u32Val) / 100);
        if (((u32Period * u32Val) % 100) < 50)
        {
            u32Duty--;
        }
    }
    _PWM_PRINT("reg=x%08X(x%x) clk=%d, u32Duty=x%x\n", (U32)(U32PwmAddr + (u32PwmOffs) + u16REG_PWM_DUTY_L),
                   ((U32)(U32PwmAddr + (u32PwmOffs) + u16REG_PWM_DUTY_L) & 0xFFFFFF) >> 9,
                   (U32)(PWM_CLOCK), u32Duty);
    OUTREG16(U32PwmAddr + (u32PwmOffs) + u16REG_PWM_DUTY_L, (u32Duty & 0xFFFF));
    OUTREG16(U32PwmAddr + (u32PwmOffs) + u16REG_PWM_DUTY_H, ((u32Duty >> 16) & 0x3));

    if (_pwmEnSatus[u8Id])
    {
        U32 reset;
        if (u8Id < 12)
            reset = INREG16(U32PwmAddr + u16REG_SW_RESET_1) & (BIT0 << (u8Id == 11 ? 14 : u8Id));
        else if (u8Id < 20)
            reset = INREG16(U32PwmAddr + u16REG_SW_RESET_2) & (BIT0 << (u8Id - 12));
        else
            reset = INREG16(U32PwmAddr + u16REG_SW_RESET_1) & (BIT0 << (u8Id - 20));

        if (u32Val && reset)
        {
            if (u8Id < 12)
                CLRREG16(U32PwmAddr + u16REG_SW_RESET_1, (BIT0 << (u8Id == 11 ? 14 : u8Id)));
            else if (u8Id < 20)
                CLRREG16(U32PwmAddr + u16REG_SW_RESET_2, (BIT0 << (u8Id - 12)));
            else
                CLRREG16(U32PwmAddr + u16REG_SW_RESET_1, (BIT0 << (u8Id - 20)));
        }
    }
}

void MHal_PWMGetDuty(u8 u8Id, u32 *pu32Val)
{
    U32 u32Duty, U32PwmAddr = 0;
    U32 u32PwmOffs = 0;
    MHal_PWMGetGrpAddr(&U32PwmAddr, &u32PwmOffs, u8Id);

    *pu32Val = 0;
    if (PWM_NUM <= u8Id)
        return;
    u32Duty = INREG16(U32PwmAddr + u32PwmOffs + u16REG_PWM_DUTY_L)
              | ((INREG16(U32PwmAddr + u32PwmOffs + u16REG_PWM_DUTY_H) & 0x3) << 16);
    if (u32Duty)
    {
        U32 u32Period = _pwmPeriod[u8Id];
        if (u32Period)
        {
            *pu32Val = (u32Duty * 100) / u32Period;
        }
    }
}

void MHal_PWMSetPeriod(u8 u8Id, u32 u32Val)
{
    U32 u32Period = 0x00000000;
    U32 U32PwmAddr = 0;
    U32 u32FrqHz = u32Val;
    U32 u32PwmOffs = 0;

    MHal_PWMGetGrpAddr(&U32PwmAddr, &u32PwmOffs, u8Id);
    MHal_PWM_SetClock();
    u32Period = (U32)(PWM_CLOCK) / u32FrqHz;

    //[APN] range 2<=Period<=262144
    if (u32Period < 100)
    {
        u32Period = 100;
    }
    else if (u32Period > 262144)
    {
        u32Period = 262144;
    }
    //[APN] PWM _PERIOD= (REG_PERIOD+1)
    u32Period--;

    _PWM_PRINT("reg=0x%08X clk=%d, period=0x%x\n", (U32)(U32PwmAddr + u32PwmOffs + u16REG_PWM_PERIOD_L),
                   (U32)(PWM_CLOCK), u32Period);

    OUTREG16(U32PwmAddr + u32PwmOffs + u16REG_PWM_PERIOD_L, (u32Period & 0xFFFF));
    OUTREG16(U32PwmAddr + u32PwmOffs + u16REG_PWM_PERIOD_H, ((u32Period >> 16) & 0x3));

    _pwmPeriod[u8Id] = u32Period;
}

void MHal_PWMGetPeriod(u8 u8Id, u32 *pu32Val)
{
    U32 u32Period, u32PwmAddr = 0;
    U32 u32PwmOffs = 0;

    MHal_PWMGetGrpAddr(&u32PwmAddr, &u32PwmOffs, u8Id);

    u32Period = INREG16(u32PwmAddr + u32PwmOffs + u16REG_PWM_PERIOD_L)
                | ((INREG16(u32PwmAddr + u32PwmOffs + u16REG_PWM_PERIOD_H) & 0x3) << 16);
    if ((0 == _pwmPeriod[u8Id]) && (u32Period))
    {
        _pwmPeriod[u8Id] = u32Period;
    }

    *pu32Val = 0;
    if (u32Period)
    {
        *pu32Val = (U32)(PWM_CLOCK) / (u32Period + 1);
    }
}
#endif

void MHal_PWMSetPolarity(u8 u8Id, u8 u8Val)
{
    u32 U32PwmAddr = 0;
    u32 u32PwmOffs = 0;
    MHal_PWMGetGrpAddr(&U32PwmAddr, &u32PwmOffs, u8Id);
    OUTREGMSK16(U32PwmAddr + (u32PwmOffs) + u16REG_PWM_CTRL, (u8Val << POLARITY_BIT), (0x1 << POLARITY_BIT));
}

void MHal_PWMGetPolarity(u8 u8Id, u8 *pu8Val)
{
    u32 U32PwmAddr = 0;
    u32 u32PwmOffs = 0;

    MHal_PWMGetGrpAddr(&U32PwmAddr, &u32PwmOffs, u8Id);
    *pu8Val        = (INREG16(U32PwmAddr + (u32PwmOffs) + u16REG_PWM_CTRL) & (0x1 << POLARITY_BIT)) ? 1 : 0;
}

void MHal_PWMSetDben(u8 u8Id, u8 u8Val)
{
    u32 U32PwmAddr = 0;
    u32 u32PwmOffs = 0;

    MHal_PWMGetGrpAddr(&U32PwmAddr, &u32PwmOffs, u8Id);
    OUTREGMSK16(U32PwmAddr + (u32PwmOffs) + u16REG_PWM_CTRL, (u8Val << DBEN_BIT), (0x1 << DBEN_BIT));
    OUTREGMSK16(U32PwmAddr + (u32PwmOffs) + u16REG_PWM_CTRL, (u8Val << VDBEN_SW_BIT), (0x1 << VDBEN_SW_BIT));
}

void MHal_PWMEnable(u8 u8Id, u8 u8Val)
{
    U32 U32PwmAddr = 0;
    U32 u32PwmOffs = 0;
    U32 u32DutyL   = 0;
    U32 u32DutyH   = 0;

    if (PWM_NUM <= u8Id)
        return;
    MHal_PWMSetDben(u8Id, 1);

    MHal_PWMGetGrpAddr(&U32PwmAddr, &u32PwmOffs, u8Id);
    u32DutyL = INREG16(U32PwmAddr + (u32PwmOffs) + u16REG_PWM_DUTY_L);
    u32DutyH = INREG16(U32PwmAddr + (u32PwmOffs) + u16REG_PWM_DUTY_H);

    if (u8Val)
    {
        if (u32DutyL || u32DutyH)
        {
            if (u8Id < 12)
                CLRREG16(U32PwmAddr + u16REG_SW_RESET_1, (BIT0 << (u8Id == 11 ? 14 : u8Id)));
            else if (u8Id < 20)
                CLRREG16(U32PwmAddr + u16REG_SW_RESET_2, (BIT0 << (u8Id - 12)));
            else
                CLRREG16(U32PwmAddr + u16REG_SW_RESET_1, (BIT0 << (u8Id - 20)));
        }
        else
        {
            if (u8Id < 12)
                SETREG16(U32PwmAddr + u16REG_SW_RESET_1, (BIT0 << (u8Id == 11 ? 14 : u8Id)));
            else if (u8Id < 20)
                SETREG16(U32PwmAddr + u16REG_SW_RESET_2, (BIT0 << (u8Id - 12)));
            else
                SETREG16(U32PwmAddr + u16REG_SW_RESET_1, (BIT0 << (u8Id - 20)));
        }
    }
    else
    {
        if (u8Id < 12)
            SETREG16(U32PwmAddr + u16REG_SW_RESET_1, (BIT0 << (u8Id == 11 ? 14 : u8Id)));
        else if (u8Id < 20)
            SETREG16(U32PwmAddr + u16REG_SW_RESET_2, (BIT0 << (u8Id - 12)));
        else
            SETREG16(U32PwmAddr + u16REG_SW_RESET_1, (BIT0 << (u8Id - 20)));
    }
    _pwmEnSatus[u8Id] = u8Val;
}

void MHal_PWMEnableGet(u8 u8Id, u8 *pu8Val)
{
    *pu8Val = 0;
    if (PWM_NUM <= u8Id)
        return;
    *pu8Val = _pwmEnSatus[u8Id];
}

int MHal_PWMGroupCap(void)
{
    return (PWM_GROUP_NUM) ? 1 : 0;
}

int MHal_PWMGroupJoin(u8 u8PWMId, u8 u8Val)
{
    if (PWM_NUM <= u8PWMId)
        return 0;
    if (u8Val)
    {
        if (u8PWMId < 16)
            SETREG16(PWM_BANK + REG_GROUP_JOIN_1, (1 << (u8PWMId + REG_GROUP_JOIN_1_SHFT)));
        else
            SETREG16(PWM_BANK + REG_GROUP_JOIN_2, (1 << ((u8PWMId - 16) + REG_GROUP_JOIN_2_SHFT)));
    }
    else
    {
        //[interrupt function]
        // Each group bit0 must enable for interrupt function
        // please see sync mode description for detail
        // SW owner default need to enable h74 bit0, bit4, bit8
        if (!(u8PWMId % 4))
        {
            _PWM_PRINT("[%s][%d] Always enable BIT_%2d for sync mode!\r\n", __FUNCTION__, __LINE__, u8PWMId);
            return 0;
        }
        else
        {
            if (u8PWMId < 16)
                CLRREG16(PWM_BANK + REG_GROUP_JOIN_1, (1 << (u8PWMId + REG_GROUP_JOIN_1_SHFT)));
            else
                CLRREG16(PWM_BANK + REG_GROUP_JOIN_2, (1 << ((u8PWMId - 16) + REG_GROUP_JOIN_2_SHFT)));
        }
    }
    return 1;
}

int MHal_PWMGroupIsEnable(u8 u8GroupId, u8 *pu8Val)
{
    *pu8Val = 0;
    if (PWM_GROUP_NUM <= u8GroupId)
        return 0;
    *pu8Val = (INREG16(PWM_BANK + REG_GROUP_ENABLE) >> (u8GroupId + REG_GROUP_ENABLE_SHFT)) & 0x1;
    return 1;
}

int MHal_PWMGroupGetRoundNum(u8 u8GroupId, U16 *pu16Val)
{
    U32 u32Reg;
    U32 u32Bank;

    if (PWM_GROUP_NUM <= u8GroupId)
        return 0;
    if (u8GroupId < 1)
        u32Reg = REG_GROUP_ROUND_0;
    else if ((u8GroupId < 2))
        u32Reg = REG_GROUP_ROUND_1;
    else if ((u8GroupId < 3))
        u32Reg = REG_GROUP_ROUND_2;
    else if ((u8GroupId < 4))
        u32Reg = REG_GROUP_ROUND_3;
    else
        u32Reg = REG_GROUP_ROUND_4;

    if (u8GroupId < 3)
        u32Bank = (U32)PWM_BANK;
    else
        u32Bank = (U32)PWM_BANK + PWM_3_OFFSET;

    *pu16Val = INREG16(u32Bank + u32Reg) & 0xFFFF;
    return 1;
}

int MHal_PWMGroupSetRound(u8 u8GroupId, U16 u16Val)
{
    U32 u32Reg;
    U32 u32Bank;

    if (PWM_GROUP_NUM <= u8GroupId)
        return 0;
    if (u8GroupId < 1)
        u32Reg = REG_GROUP_ROUND_0;
    else if ((u8GroupId < 1))
        u32Reg = REG_GROUP_ROUND_1;
    else if ((u8GroupId < 2))
        u32Reg = REG_GROUP_ROUND_2;
    else if ((u8GroupId < 3))
        u32Reg = REG_GROUP_ROUND_3;
    else
        u32Reg = REG_GROUP_ROUND_4;

    if (u8GroupId < 3)
        u32Bank = (U32)PWM_BANK;
    else
        u32Bank = (U32)PWM_BANK + PWM_3_OFFSET;

    OUTREG16(u32Bank + u32Reg, u16Val);
    return 1;
}

int MHal_PWMGroupStop(u8 u8GroupId, u8 u8Val)
{
    if (PWM_GROUP_NUM <= u8GroupId)
        return 0;

    if (u8Val)
        SETREG16(PWM_BANK + REG_GROUP_STOP, (1 << (u8GroupId + REG_GROUP_STOP_SHFT)));
    else
        CLRREG16(PWM_BANK + REG_GROUP_STOP, (1 << (u8GroupId + REG_GROUP_STOP_SHFT)));

    return 1;
}

int MHal_PWMGroupHold(u8 u8GroupId, u8 u8Val)
{
    if (PWM_GROUP_NUM <= u8GroupId)
        return 0;

    if (u8Val)
        SETREG16(PWM_BANK + REG_GROUP_HOLD, (1 << (u8GroupId + REG_GROUP_HOLD_SHFT)));
    else
        CLRREG16(PWM_BANK + REG_GROUP_HOLD, (1 << (u8GroupId + REG_GROUP_HOLD_SHFT)));

    return 1;
}

int MHal_PWMGroupGetHoldM1(void)
{
    return INREG16(PWM_BANK + REG_GROUP_HOLD_MODE1);
}

int MHal_PWMGroupHoldM1(u8 u8Val)
{
    if (u8Val)
    {
        SETREG16(PWM_BANK + REG_GROUP_HOLD_MODE1, 1);
        _PWM_PRINT("[%s L%d] hold mode1 en!(keep low)\n", __FUNCTION__, __LINE__);
    }
    else
    {
        CLRREG16(PWM_BANK + REG_GROUP_HOLD_MODE1, 0);
        _PWM_PRINT("[%s L%d] hold mode1 dis!\n", __FUNCTION__, __LINE__);
    }
    return 1;
}

int MHal_PWMDutyQE0(u8 u8Val)
{
    if (u8Val)
        SETREG16(PWM_BANK + REG_PWM_DUTY_QE0, (1 << REG_PWM_DUTY_QE0_SHFT));
    else
        CLRREG16(PWM_BANK + REG_PWM_DUTY_QE0, (1 << REG_PWM_DUTY_QE0_SHFT));

    return 1;
}

void MHal_PWMSync8Mode(U8 u8Enable)
{
    if (u8Enable)
        SETREG16(PWM_BANK + PWM_3_OFFSET + u16REG_8SYNC_MODE, (1 << REG_8SYNC_MODE_SHFT));
    else
        CLRREG16(PWM_BANK + PWM_3_OFFSET + u16REG_8SYNC_MODE, (1 << REG_8SYNC_MODE_SHFT));
}

void MHal_PWMInit(u8 u8Id)
{
    U32 reset, u32Period = 0, U32PwmAddr = 0, u32PwmOffs = 0;

    if (PWM_NUM <= u8Id)
        return;

    MHal_PWMGetGrpAddr(&U32PwmAddr, &u32PwmOffs, u8Id);
    if (u8Id < 12)
        reset = INREG16(U32PwmAddr + u16REG_SW_RESET_1) & (BIT0 << (u8Id == 11 ? 14 : u8Id));
    else if (u8Id < 20)
        reset = INREG16(U32PwmAddr + u16REG_SW_RESET_2) & (BIT0 << (u8Id - 12));
    else
        reset = INREG16(U32PwmAddr + u16REG_SW_RESET_1) & (BIT0 << (u8Id - 20));

#ifdef CONFIG_PWM_NEW
    MHal_PWMGetConfig(u8Id, NULL, &u32Period);
    MHal_PWMDutyQE0(0);
#else
    MHal_PWMGetPeriod(u8Id, &u32Period);
#endif

    if ((0 == reset) && (u32Period))
    {
        _pwmEnSatus[u8Id] = 1;
    }
    else
    {
        MHal_PWMEnable(u8Id, 0);
    }
}

int MHal_PWMGetOutput(u8 *pu8Output)
{
    *pu8Output = INREG16(PWM_BANK + REG_PWM_OUT);
    _PWM_PRINT("[%s L%d] output:x%x\n", __FUNCTION__, __LINE__, *pu8Output);
    return 1;
}

#ifdef CONFIG_PWM_NEW
int MHal_PWMSetEnd(u8 u8Id, u8 u8DutyId, u32 u32Val)
{
    U16 u16Div  = 0;
    U32 common  = 0;
    U32 pwmclk  = 0;
    U32 duty    = u32Val;
    U32 u32Duty = 0;
    if (u8Id >= PWM_NUM)
        return 0;
    pwmclk = (U32)(PWM_CLOCK);
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
    u16Div = (_pwmDiv[u8Id] + 1);
    if (duty < (0xFFFFFFFF / pwmclk))
    {
        u32Duty = (pwmclk * duty) / (u16Div * common);
        if ((((pwmclk * duty) % (u16Div * common)) > (u16Div * common / 2)) || (u32Duty == 0))
        {
            u32Duty++;
        }
        _pwmDuty_ns[u8Id] = (u32Duty * u16Div * common) / pwmclk;
    }
    else
    {
        u32Duty = (duty / u16Div) * pwmclk / common;
        u32Duty++;
        _pwmDuty_ns[u8Id] = (u32Duty * common / pwmclk) * u16Div;
    }
    u32Duty--;
    _pwmDutyArgNum[u8Id]     = u8DutyId + 1;
    _pwmDuty[u8Id][u8DutyId] = u32Duty;
    return 1;
}
#else
int MHal_PWMSetEnd(u8 u8Id, u8 u8DutyId, u32 u32Val)
{
    U32 u32Period;
    U32 u32Duty;

    if (PWM_NUM <= u8Id)
        return 0;

    u32Period = _pwmPeriod[u8Id];
    u32Duty = ((u32Period * u32Val) / 1000);
    _pwmDutyArgNum[u8Id] = u8DutyId + 1;
    _pwmDuty[u8Id][u8DutyId] = u32Duty;
    MHal_PWM_SetSyncFlag(0);

    if (u32Duty & 0xFFFC0000)
    {
        SSTAR_PWM_ERR("[%s][%d] too large duty 0x%08x (18 bits in max)\n", __FUNCTION__, __LINE__, u32Duty);
    }
    return 1;
}
#endif
int MHal_PWMSetEndToReg(u8 u8Id, u8 u8DutyArgNum)
{
    U8  arg_no;
    U32 U32PwmAddr = 0;
    U32 u32PwmOffs = 0;

    MHal_PWMGetGrpAddr(&U32PwmAddr, &u32PwmOffs, u8Id);
    for (arg_no = 0; arg_no < u8DutyArgNum; arg_no++)
    {
        if (0 == arg_no)
        {
            OUTREG16(U32PwmAddr + u32PwmOffs + u16REG_PWM_DUTY_L, (_pwmDuty[u8Id][arg_no] & 0xFFFF));
            OUTREG16(U32PwmAddr + u32PwmOffs + u16REG_PWM_DUTY_H, ((_pwmDuty[u8Id][arg_no] >> 16) & 0x0003));
        }
        else
        {
            OUTREG16(U32PwmAddr + u32PwmOffs + (arg_no << 3) + 28, (_pwmDuty[u8Id][arg_no] & 0xFFFF));
        }
    }
    return 1;
}
#ifdef CONFIG_PWM_NEW
int MHal_PWMSetBegin(u8 u8Id, u8 u8ShftId, u32 u32Val)
{
    U16 u16Div   = 0;
    U32 common   = 0;
    U32 pwmclk   = 0;
    U32 shift    = u32Val;
    U32 u32Shift = 0;
    if (u8Id >= PWM_NUM)
        return 0;
    pwmclk = (U32)(PWM_CLOCK);
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
    u16Div = (_pwmDiv[u8Id] + 1);
    if (shift < (0xFFFFFFFF / pwmclk))
    {
        u32Shift = (pwmclk * shift) / (u16Div * common);
        if ((((pwmclk * shift) % (u16Div * common)) > (u16Div * common / 2)) || (u32Shift == 0))
        {
            u32Shift++;
        }
    }
    else
    {
        u32Shift = (shift / u16Div) * pwmclk / common;
        u32Shift++;
    }
    u32Shift--;
    _pwmShftArgNum[u8Id]     = u8ShftId + 1;
    _pwmShft[u8Id][u8ShftId] = u32Shift;
    MHal_PWM_SetSyncFlag(0);
    return 1;
}
#else
int MHal_PWMSetBegin(u8 u8Id, u8 u8ShftId, u32 u32Val)
{
    U32 u32Period;
    U32 u32Shft;

    if (PWM_NUM <= u8Id)
        return 0;

    u32Period = _pwmPeriod[u8Id];
    u32Shft = ((u32Period * u32Val) / 1000);
    _pwmShftArgNum[u8Id] = u8ShftId + 1;
    _pwmShft[u8Id][u8ShftId] = u32Shft;
    MHal_PWM_SetSyncFlag(0);

    if (u32Shft & 0xFFFC0000)
    {
        SSTAR_PWM_ERR("[%s][%d] too large shift 0x%08x (18 bits in max)\n", __FUNCTION__, __LINE__, u32Shft);
    }

    return 1;
}
#endif
int MHal_PWMSetBeginToReg(u8 u8Id, u8 u8ShftArgNum)
{
    U8  arg_no;
    U32 U32PwmAddr = 0;
    U32 u32PwmOffs = 0;

    MHal_PWMGetGrpAddr(&U32PwmAddr, &u32PwmOffs, u8Id);
    for (arg_no = 0; arg_no < u8ShftArgNum; arg_no++)
    {
        if (0 == arg_no)
        {
            OUTREG16(U32PwmAddr + u32PwmOffs + u16REG_PWM_SHIFT_L, (_pwmShft[u8Id][arg_no] & 0xFFFF));
            OUTREG16(U32PwmAddr + u32PwmOffs + u16REG_PWM_SHIFT_H, ((_pwmShft[u8Id][arg_no] >> 16) & 0x0003));
        }
        else
        {
            OUTREG16(U32PwmAddr + u32PwmOffs + (arg_no << 3) + 24, (_pwmShft[u8Id][arg_no] & 0xFFFF));
        }
    }
    return 1;
}

int MHal_PWMSetPolarityEx(u8 u8Id, u8 u8Val)
{
    if (PWM_NUM <= u8Id)
        return 0;
    _pwmPolarity[u8Id] = u8Val;
    MHal_PWM_SetSyncFlag(0);
    return 1;
}

int MHal_PWMSetPolarityExToReg(u8 u8Id, u8 u8Val)
{
    u32 U32PwmAddr = 0;
    u32 u32PwmOffs = 0;

    MHal_PWMGetGrpAddr(&U32PwmAddr, &u32PwmOffs, u8Id);

    if (u8Val)
        SETREG16(U32PwmAddr + u32PwmOffs + u16REG_PWM_CTRL, (0x1 << POLARITY_BIT));
    else
        CLRREG16(U32PwmAddr + u32PwmOffs + u16REG_PWM_CTRL, (0x1 << POLARITY_BIT));
    return 1;
}
#ifdef CONFIG_PWM_NEW
void MHal_PWMSetPeriodEx(u8 u8Id, u32 u32Val)
{
    U8  i;
    U16 u16Div     = 0;
    U32 common     = 0;
    U32 pwmclk     = 0;
    U32 periodmax  = 0;
    U32 u32Period  = 0x00000000;
    U32 period     = u32Val;
    U32 U32PwmAddr = 0;
    U32 u32PwmOffs = 0;

    if (u8Id >= PWM_NUM)
        return;
    MHal_PWMGetGrpAddr(&U32PwmAddr, &u32PwmOffs, u8Id);
    pwmclk = (U32)(PWM_CLOCK);
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

    /*      select   div       */
    for (i = 0; i < (sizeof(clk_pwm_div) / sizeof(U16)); i++)
    {
        periodmax = (clk_pwm_div[i] * 262144 / pwmclk) * common;
        if (period < periodmax)
        {
            u16Div = clk_pwm_div[i];
            break;
        }
    }

    /*      select   period       */
    if (period < (0xFFFFFFFF / pwmclk))
    {
        u32Period = (pwmclk * period) / (u16Div * common);
        if (((pwmclk * period) % (u16Div * common)) > (u16Div * common / 2))
        {
            u32Period++;
        }
        _pwmPeriod_ns[u8Id] = (u32Period * u16Div * common) / pwmclk;
    }
    else
    {
        u32Period = (period / u16Div) * pwmclk / common;
        u32Period++;
        _pwmPeriod_ns[u8Id] = (u32Period * common / pwmclk) * u16Div;
    }
    u16Div--;
    u32Period--;
    _pwmDiv[u8Id]    = u16Div;
    _pwmPeriod[u8Id] = u32Period;
    _pwmFreq[u8Id]   = 1000000000 / u32Val;
    MHal_PWM_SetSyncFlag(0);
    _PWM_PRINT("reg=0x%08X clk=%d, period=0x%x\n", (U32)(U32PwmAddr + u32PwmOffs + u16REG_PWM_PERIOD_L),
           PWM_CLOCK, u32Period);
}
#else
void MHal_PWMSetPeriodEx(u8 u8Id, u32 u32Val)
{
    U32 u32Period;
    U32 u32Div;
    U32 U32PwmAddr = 0;
    U32 u32PwmOffs = 0;

    if (u8Id >= PWM_NUM)
        return;

    MHal_PWMGetGrpAddr(&U32PwmAddr, &u32PwmOffs, u8Id);
    u32Div = INREG16(U32PwmAddr + u32PwmOffs + u16REG_PWM_DIV); // workaround

    u32Period = (U32)(PWM_CLOCK) / u32Val;
    u32Period /= (u32Div + 1); // workaround

    //[APN] range 2<=Period<=262144
    if (u32Period < 2)
        u32Period = 2;
    if (u32Period > 262144)
        u32Period = 262144;

    //[APN] PWM _PERIOD= (REG_PERIOD+1)
    u32Period--;
    _pwmPeriod[u8Id] = u32Period;
    _pwmFreq[u8Id] = u32Val;
    MHal_PWM_SetSyncFlag(0);

    _PWM_PRINT("reg=0x%08X clk=%d, period=0x%x\n", (U32)(U32PwmAddr + u32PwmOffs + u16REG_PWM_PERIOD_L),
                   (U32)(PWM_CLOCK), u32Period);
}

#endif
void MHal_PWMSetPeriodExToReg(u8 u8Id, u32 u32Period)
{
    u32 U32PwmAddr = 0;
    u32 u32PwmOffs = 0;

    MHal_PWMGetGrpAddr(&U32PwmAddr, &u32PwmOffs, u8Id);

    OUTREG16(U32PwmAddr + u32PwmOffs + u16REG_PWM_PERIOD_L, (u32Period & 0xFFFF));
    OUTREG16(U32PwmAddr + u32PwmOffs + u16REG_PWM_PERIOD_H, ((u32Period >> 16) & 0x3));
}

int MHal_PWMDiv(u8 u8Id, u8 u8Val)
{
    u32 U32PwmAddr = 0;
    u32 u32PwmOffs = 0;

    MHal_PWMGetGrpAddr(&U32PwmAddr, &u32PwmOffs, u8Id);

    if (PWM_NUM <= u8Id)
        return 0;
    OUTREG16(U32PwmAddr + u32PwmOffs + u16REG_PWM_DIV, u8Val);
    _PWM_PRINT("mhal  MHal_PWMDiv done !\n");

    return 1;
}

void MHal_PWM_MemToReg(u8 u8GrpId)
{
    u8 pwmId, idx;
    for (idx = 0; idx < PWM_PER_GROUP; idx++)
    {
        pwmId = (u8GrpId * PWM_PER_GROUP) + idx; // idx+(groupid*4)
        if (pwmId < PWM_NUM)
        {
#ifdef CONFIG_PWM_NEW
            MHal_PWMDiv(pwmId, _pwmDiv[pwmId]);
#endif
            MHal_PWMSetPolarityExToReg(pwmId, _pwmPolarity[pwmId]);
            MHal_PWMSetPeriodExToReg(pwmId, _pwmPeriod[pwmId]);
            MHal_PWMSetBeginToReg(pwmId, _pwmShftArgNum[pwmId]);
            MHal_PWMSetEndToReg(pwmId, _pwmDutyArgNum[pwmId]);
        }
    }
    // _PWM_PRINT(KERN_ERR "\r\n[P2R]Sync:%d\r\n", isSync);
}

int MHal_PWMGroupEnable(u8 u8GroupId, u8 u8Val)
{
    U32 u32JoinMask;
    U32 u32GroupMask;
    U32 u32GroupReg;
    if (PWM_GROUP_NUM <= u8GroupId)
        return 0;

    if (!MHal_PWM_GetSyncFlag())
    {
        MHal_PWM_MemToReg(u8GroupId);
        _PWM_PRINT("MEM to REG done !\n");
    }
    if (u8GroupId < PWM_GROUP_NUM - 2)
    {
        u32GroupMask = ((u8GroupId == 2) ? 0x47 : 0xF) << (u8GroupId << 2);
        u32GroupReg  = u8GroupId + REG_GROUP_SW_RST_SHFT_1;
    }
    else
    {
        u32GroupMask = (0xF) << ((u8GroupId - 3) << 2);
        u32GroupReg  = u8GroupId + REG_GROUP_SW_RST_SHFT_2;
    }
    u32JoinMask = u32GroupMask | u32GroupReg;
    if (u8Val)
    {
        SETREG16(PWM_BANK + REG_GROUP_ENABLE, (1 << (u8GroupId + REG_GROUP_ENABLE_SHFT)));
        if (u8GroupId < PWM_GROUP_NUM - 2)
            CLRREG16(PWM_BANK + u16REG_SW_RESET_1, u32JoinMask);
        else
            CLRREG16(PWM_BANK + u16REG_SW_RESET_2, u32JoinMask);
        MHal_PWM_SetSyncFlag(1); // dont need to sync until new data in
    }
    else
    {
        CLRREG16(PWM_BANK + REG_GROUP_ENABLE, (1 << (u8GroupId + REG_GROUP_ENABLE_SHFT)));
        if (u8GroupId < PWM_GROUP_NUM - 2)
            SETREG16(PWM_BANK + u16REG_SW_RESET_1, u32JoinMask);
        else
            SETREG16(PWM_BANK + u16REG_SW_RESET_2, u32JoinMask);
    }

    return 1;
}

int MHal_PWMGroupShowRoundNum(char *buf_start, char *buf_end)
{
    return 0;
}

void MHal_PWMOutputPulse(u8 PWNIndex,PWMConfig_t PWMConfig)
{
    MHal_PWM_SetClock();
    MHal_PWMSetDben(PWNIndex, 1);
    MHal_PWMSetPolarity(PWNIndex, PWMConfig.bInvert);
    MHal_PWMSetConfig(PWNIndex, PWMConfig.nDuty, PWMConfig.nFrequency);
}

void PWM_IRQ(int irq, void *dev)
{
    volatile u16           u16_Events = 0x0000;
    volatile u16           gid        = 0x0000;

    u16_Events = INREG16(PWM_BANK + REG_GROUP_INT);
    if ((u16_Events & PWM_INT_GRP_MASK))
    {
        for (gid = 0; gid < PWM_GROUP_NUM; gid++)
        {
            // one time enable one bit
            if ((u16_Events >> REG_GROUP_HOLD_INT_SHFT) & (1 << gid))
            {
                if (!MHal_PWMGroupGetHoldM1())
                { // isr fill parameter only for HoldM0
                    MHal_PWM_MemToReg(gid);
                }
                MHal_PWMGroupHold(gid, 0); // For clear hold event;
                MHal_PWM_SetSyncFlag(1);
                _PWM_PRINT("hold_int GrpId:%d(event:x%x)\n", gid, u16_Events);
            }
        }
    }
    if ((u16_Events >> REG_GROUP_RUND_INT_SHFT))
    {
        for (gid = 0; gid < PWM_GROUP_NUM; gid++)
        {
            if ((u16_Events >> REG_GROUP_RUND_INT_SHFT) & (1 << gid))
            {
                if (MHal_PWM_GetSyncFlag())
                {                                         // no new data
                    MHal_PWMGroupEnable(gid, 0);   // For clear round event;
                    MHal_PWMGroupSetRound(gid, 0); // set round=0
                }
                else
                { // have new data
                    MHal_PWMGroupEnable(gid, 0);
                    MHal_PWMGroupEnable(gid, 1);
                }
                _PWM_PRINT("round_int GrpId:%d\n", gid);
            }
        }
    }
}

void MHal_PWMIrqRequest(PWMConfig_t *sPWMConfig)
{
    sPWMConfig = NULL;
    CamOsIrqRequest(INT_IRQ_PWM, (CamOsIrqHandler)PWM_IRQ, "pwm_irq", (void *)sPWMConfig);
    CamOsIrqDisable(INT_IRQ_PWM);
    CamOsIrqEnable(INT_IRQ_PWM);
}

