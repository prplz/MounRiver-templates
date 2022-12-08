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

#define PWM_OUT_DBG 0
#if PWM_OUT_DBG
#define SSTAR_PWM_OUT_INFO(x, args...) CamOsPrintf(x, ##args)
#define SSTAR_PWM_OUT_DBG(x, args...)  CamOsPrintf(x, ##args)
#define SSTAR_PWM_OUT_ERR(x, args...)  CamOsPrintf(x, ##args)

#else
#define SSTAR_PWM_OUT_INFO(x, args...) CamOsPrintf(x, ##args)
#define SSTAR_PWM_OUT_DBG(x, args...)
#define SSTAR_PWM_OUT_ERR(x, args...) CamOsPrintf(x, ##args)
#endif

void MHal_PWMOUT_SetClock(void);
void MHal_PWMDdtP(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Id, u32 u32DdtLen);
void MHal_PWMDdtPIdle(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Id, u8 u8State);
void MHal_PWMDdtN(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Id, u32 u32DdtLen);
void MHal_PWMDdtNIdle(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Id, u8 u8State);
void MHal_PWMOutPConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Id, u8 u8Enable);
void MHal_PWMOutNConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Id, u8 u8Enable);
void MHal_PWMOutEnable(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Id, u8 u8Enable);
void MHal_PWMBkinSel(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Id, u8 u8Sel);
void MHal_PWMPIdleStatus(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Id, u8 u8Status);
void MHal_PWMNIdleStatus(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Id, u8 u8Status);
void MHal_ADCWDTConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Channel, u8 u8DatHigh, u8 u8DatLow);
void MHal_ADCWDTEnable(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Channel, u8 u8Enable);
void MHal_ADCEnable(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Enable);
void MHal_ADCRegSeqConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8SeqLen, u8 u8Id, u8 u8Channel);
void MHal_ADCInjSeqConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8SeqLen, u8 u8Id, u8 u8Channel);
void MHal_ADCRegConMode(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8ModeSelct);
void MHal_ADCInjConMode(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8ModeSelct);
void MHal_ADCRegStr(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Enable);
void MHal_ADCInjStr(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Enable);
void MHal_ADCRegTriConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8TriMode, u8 u8TriMethod);
void MHal_ADCInjTriConfig(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8TriMode, u8 u8TriMethod);
void MHal_ADCInjTriDelay(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8PTriDelay, u8 u8NTriDelay);
void MHal_ADCDMAConfig(struct sstar_pwm_out_chip *sstar_out_chip, u32 u8MiuSize);
void MHal_ADCDMAEnable(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8DatAlign, u8 u8Enable);
void MHal_ADCFreerun(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8AveCnt, u32 u32OneshotPrd);
void MHal_ADCSWTri(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8AdcMode);
u32 MHal_ADCData(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Channel, u16 *u16AdcData);
u32  MHal_ADCDMAData(struct sstar_pwm_out_chip *sstar_out_chip, u8 u8Selctor);
