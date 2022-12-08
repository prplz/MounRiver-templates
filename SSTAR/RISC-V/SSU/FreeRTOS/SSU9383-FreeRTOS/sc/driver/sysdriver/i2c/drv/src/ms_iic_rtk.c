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

#include <cam_os_wrapper.h>
#include "vm_types.ht"
#include "hal_iic.h"
#include "hal_iic_reg.h"
#include "drv_iic.h"
#include "registers.h"
#ifdef CONFIG_SYSDESC_SUPPORT
#include <drv_sysdesc.h>
#endif

void rtk_i2c_init(u8 u8Port)
{
    sstar_i2c_init(u8Port);
}

s32 i2c_transfer(u8 u8Port, tI2cMsg *pmsg, s32 length)
{
    return sstar_i2c_master_xfer(u8Port, pmsg, length);
}

s32 rtk_i2c_set_speed(u8 u8Port, u32 speed)
{
    return sstar_i2c_set_speed(u8Port, speed);
}

#if defined(CONFIG_POWER_MANAG_SUPPORT)
#include "drv_power_manag.h"
#include "initcall.h"

int DrvI2cSystemSuspend(void)
{
    int eRet = 0;

    sstar_i2c_suspend();
    return eRet;
}

int DrvI2cSystemResume(void)
{
    int eRet = 0;

    sstar_i2c_resume();
    return eRet;
}
static power_manag_dev_handle_t m_tI2CPowerManagHandle = {0};

static void I2C_Probe(void)
{
    m_tI2CPowerManagHandle.pcName = "I2C";
    m_tI2CPowerManagHandle.pfSuspendFunc = DrvI2cSystemSuspend;
    m_tI2CPowerManagHandle.pfResumeFunc = DrvI2cSystemResume;
    m_tI2CPowerManagHandle.u32Prio = 0;
    drv_power_manag_register_dev_handle(&m_tI2CPowerManagHandle);
}
rtos_device_initcall(I2C_Probe)
#endif
