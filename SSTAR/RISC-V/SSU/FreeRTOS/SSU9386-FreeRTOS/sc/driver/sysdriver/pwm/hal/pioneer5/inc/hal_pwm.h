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

#ifndef __PWM_H
#define __PWM_H
#include "drv_pwm.h"
#include "cam_os_wrapper.h"
#include "ms_platform.h"
#include "irqs.h"
#include "drv_int_ctrl_pub_api.h"
#include "hal_int_ctrl_pub.h"
#include "registers.h"
#define GET_BASE_ADDR_BY_BANK(x, y)         ((x) + ((y) << 1))
//#define MS_BASE_REG_RIU_PA                  (0x1F000000)

#define PWM_CLOCK      12000000

#define PWM_BANK                 GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x101900)
#define PWM_PM_BANK              GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x001A00)
#define PWM_INPUT_BANK           GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x100D00)
#define PIU_NONPM_BANK           GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x100700)

#define PWM_2_OFFSET       (0x0100 << 1)
#define PWM_ADC_OFFSET     (0x0200 << 1)
#define PWM_3_OFFSET       (0x0300 << 1)
#define PWM_ADC_WDT_OFFSET (0x0400 << 1)


#define BASE_REG_CLKGEN_PA        GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x103800)
#define BASE_REG_PAD_TOP          GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x103C00)
#define BASE_REG_PM_PAD_TOP       GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x001A00)
#define BASE_REG_CHIPTOP_PA       GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x101E00)

#define PWM_CLK_OFFSET             (0x38 << 2)

#define CLKG_PWM_REG                       0x38
#define CLKG_PWM_OFS                       8
#define CLKG_PWM_MASK                   BIT12|BIT11|BIT10|BIT9|BIT8

#define PWM_GROUP_NUM (5)
#define PWM_PER_GROUP (4)
#define PWM_NUM       (22)
#define PWM_IN_NUM       (8)
// Common PWM registers
#define PWM_SHIFT_ARG_MAX_NUM (4)
#define u16REG_PWM_SHIFT_L    (0x0 << 2)
#define u16REG_PWM_SHIFT_H    (0x1 << 2)
#define u16REG_PWM_DUTY_L     (0x2 << 2)
#define u16REG_PWM_DUTY_H     (0x3 << 2)
#define u16REG_PWM_PERIOD_L   (0x4 << 2) // reg_pwm0_period
#define u16REG_PWM_PERIOD_H   (0x5 << 2)
#define u16REG_PWM_DIV        (0x6 << 2)
#define u16REG_PWM_CTRL       (0x7 << 2)
#define VDBEN_SW_BIT          0
#define DBEN_BIT              1
#define DIFF_P_EN_BIT         2
#define SHIFT_GAT_BIT         3
#define POLARITY_BIT          4

#define u16REG_PWM_SHIFT2 (0x8 << 2)
#define u16REG_PWM_DUTY2  (0x9 << 2)
#define u16REG_PWM_SHIFT3 (0xA << 2)
#define u16REG_PWM_DUTY3  (0xB << 2)
#define u16REG_PWM_SHIFT4 (0xC << 2)
#define u16REG_PWM_DUTY4  (0xD << 2)

#define REG_GROUP_ROUND_0 (0x10 << 2)
#define REG_GROUP_ROUND_1 (0x2E << 2)
#define REG_GROUP_ROUND_2 (0x37 << 2)
#define REG_GROUP_ROUND_3 (0x0E << 2)
#define REG_GROUP_ROUND_4 (0x17 << 2)

#define REG_GROUP_HOLD      (0x71 << 2)
#define REG_GROUP_HOLD_SHFT (0x0)

#define REG_GROUP_STOP      (0x72 << 2)
#define REG_GROUP_STOP_SHFT (0x0)

#define REG_GROUP_ENABLE      (0x73 << 2)
#define REG_GROUP_ENABLE_SHFT (0x0)

#define REG_GROUP_JOIN_1      (0x74 << 2)
#define REG_GROUP_JOIN_1_SHFT (0x0)
#define REG_GROUP_JOIN_2      (0x73 << 2)
#define REG_GROUP_JOIN_2_SHFT (0x8)

#define REG_GROUP_INT           (0x75 << 2)
#define REG_GROUP_HOLD_INT_SHFT (0x0)
#define REG_GROUP_RUND_INT_SHFT (0x3)
#define PWM_INT_GRP0            (BIT0)
#define PWM_INT_GRP1            (BIT1)
#define PWM_INT_GRP2            (BIT2)
#define PWM_INT_GRP_MASK        (0xC7) // GRP0~GRP2

#define REG_PWM_DUTY_QE0      (0x76 << 2)
#define REG_PWM_DUTY_QE0_SHFT (0x0)

#define REG_GROUP_HOLD_MODE1    (0x77 << 2)
#define REG_GROUP_HALD_MD1_SHFT (0x0)

#define u16REG_SW_RESET_2       (0x7C << 2)
#define REG_GROUP_SW_RST_SHFT_2 (0x8)

#define REG_PWM_OUT      (0x7E << 2)
#define REG_PWM_OUT_SHFT (0x0)

#define u16REG_SW_RESET_1       (0x7F << 2)
#define REG_GROUP_SW_RST_SHFT_1 (0xB)

#define u16REG_8SYNC_MODE       (0x7F << 2)
#define REG_8SYNC_MODE_SHFT (0x0)

void MHal_PWMPadSet(u8 ePWNIndex, u32 ePadMode);
void MHal_PWMOutputPulse(u8 ePWNIndex,PWMConfig_t sPWMConfig);
void MHal_PWMEnable(u8 u8Id, u8 u8Val);
int MHal_PWMGroupJoin(u8 u8PWMId, u8 u8Val);
int MHal_PWMDiv(u8 u8Id, u8 u8Val);
void MHal_PWMSetDben(u8 u8Id, u8 u8Val);
void MHal_PWMSetPeriodEx(u8 u8Id, u32 u32Val);
int MHal_PWMSetPolarityEx(u8 u8Id, u8 u8Val);
int MHal_PWMSetEnd(u8 u8Id, u8 u8DutyId, u32 u32Val);
int MHal_PWMSetBegin(u8 u8Id, u8 u8ShftId, u32 u32Val);
int MHal_PWMGroupSetRound(u8 u8GroupId, U16 u16Val);
int MHal_PWMGroupIsEnable(u8 u8GroupId, u8 *pu8Val);
int MHal_PWMGroupStop(u8 u8GroupId, u8 u8Val);
int MHal_PWMGroupHold(u8 u8GroupId, u8 u8Val);
int MHal_PWMGroupEnable(u8 u8GroupId, u8 u8Val);
void MHal_PWMSetDben(u8 u8Id, u8 u8Val);
void MHal_PWMSetPolarity(u8 u8Id, u8 u8Val);
void MHal_PWM_SetClock(void);
void MHal_PWMSync8Mode(U8 u8Enable);
void MHal_PWMIrqRequest(PWMConfig_t *sPWMConfig);

#endif  //__PWM_H
