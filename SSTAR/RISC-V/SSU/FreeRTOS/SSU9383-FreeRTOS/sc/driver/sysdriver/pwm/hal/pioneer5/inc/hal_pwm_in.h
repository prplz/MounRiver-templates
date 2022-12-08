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

#include "cam_os_wrapper.h"
#include "ms_platform.h"

#define PWM_IN_DBG 0
#if PWM_IN_DBG
#define SSTAR_PWM_IN_INFO(x, args...) CamOsPrintf(x, ##args)
#define SSTAR_PWM_IN_DBG(x, args...)  CamOsPrintf(x, ##args)
#define SSTAR_PWM_IN_ERR(x, args...)  CamOsPrintf(x, ##args)

#else
#define SSTAR_PWM_IN_INFO(x, args...) CamOsPrintf(x, ##args)
#define SSTAR_PWM_IN_DBG(x, args...)
#define SSTAR_PWM_IN_ERR(x, args...) CamOsPrintf(x, ##args)
#endif

void MHal_PWMIN_SetClock(void);
void MHal_PWMInCapEn(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Enable);
void MHal_PWMInRstMode(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Enable);
void MHal_PWMInPulCalMode(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Enable);
void MHal_PWMInDetMode(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Enable);
void MHal_PWMInTimerDiv(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8DivNum);
void MHal_PWMInEdgeSel(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Edge);
void MHal_PWMInPulDiv(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Div);
void MHal_PWMInPulCalEdge(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u8 u8Edge);
void MHal_PWMInGet(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id, u32 *u32Period, u32 *u32DutyI, u32 *u32DutyD);
void MHal_PWMInPulNumGet(struct sstar_pwm_in_chip *sstar_in_chip, u8 u8Id);
