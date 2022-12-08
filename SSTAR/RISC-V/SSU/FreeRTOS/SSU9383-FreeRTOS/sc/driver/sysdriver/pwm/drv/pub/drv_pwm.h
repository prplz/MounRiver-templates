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
#ifndef __DRV_PWM_H__
#define __DRV_PWM_H__

#include "vm_types.ht"
#include "cam_os_wrapper.h"

#define DRV_PWM_PM_PAD_BIT		4

typedef enum
{
	DRV_PWM_0,
	DRV_PWM_1,
	DRV_PWM_2,
	DRV_PWM_3,
	DRV_PWM_4,
	DRV_PWM_5,
	DRV_PWM_6,
	DRV_PWM_7,
	DRV_PWM_8,
	DRV_PWM_9,
	DRV_PWM_10,
	DRV_PWM_MAX
} DrvPWMSe;

typedef enum
{
	DRV_PWM_PAD_MODE0 = 0x00,								//disable non-PM Pad
	DRV_PWM_PAD_MODE1,
	DRV_PWM_PAD_MODE2,
	DRV_PWM_PAD_MODE3,
	DRV_PWM_PAD_MODE4,
	DRV_PWM_PAD_MODE5,
	DRV_PWM_PAD_MODE6,
	DRV_PWM_PAD_MODE7,
	DRV_PWM_PAD_MODE8,
	DRV_PWM_PAD_MODE9,
	DRV_PWM_PAD_MODE_NOSUPPORT,
	DRV_PWM_PM_PAD_MODE0 = (1 << DRV_PWM_PM_PAD_BIT) | 0x00,	//disable PM Pad
	DRV_PWM_PM_PAD_MODE1,
	DRV_PWM_PM_PAD_MODE2,
	DRV_PWM_PM_PAD_MODE3,
	DRV_PWM_PM_PAD_MODE4,
	DRV_PWM_PM_PAD_MODE5,
	DRV_PWM_PM_PAD_MODE6,
	DRV_PWM_PM_PAD_MODE7,
	DRV_PWM_PM_PAD_MODE8,
	DRV_PWM_PM_PAD_MODE9,
	DRV_PWM_PM_PAD_MODE_NOSUPPORT
} DrvPadModeSe;

typedef struct{
    u32                 nFrequency;        	//PWM period in Hz
    u32					nDuty;				//Duty in percentage(0~100)
    bool				bInvert;			//Invert signal or not
    u32					nPulseShift;		//shift of duty cycle in percentage(0~100)
} PWMConfig_t;

void DrvPWMOutputPulse(u8 ePWNIndex,PWMConfig_t sPWMConfig);
void DrvPWMEnable(u8 ePWNIndex, u8 u8Enable);
void DrvGroupModeIn(u8 ePWNIndex,u8 u8Enable);
void DrvGroupModeIn(u8 ePWNIndex, u8 u8Enable);
void DrvGroupPeriodIn(u8 ePWNIndex, u32 u32Val);
void DrvGroupPolarityIn(u8 ePWNIndex, u8 u8Val);
void DrvGroupEndIn(u8 ePWNIndex, u8 u8DutyId, u32 u32Val);
void DrvGroupBeginIn(u8 ePWNIndex, u8 u8ShftId, u32 u32Val);
void DrvGroupRoundIn(u8 u8GroupId, u16 u16Val);
void DrvGroupHold(u8 u8GroupId, u16 u16Val);
void DrvGroupStop(u8 u8GroupId, u16 u16Val);
void DrvGroupEnableIn(u8 u8GroupId, u8 u8Enable);
void DrvPWMIrqRequest(PWMConfig_t *sPWMConfig);


#endif /* __DRV_PWM_H__ */
