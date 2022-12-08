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


#ifndef _DRV_IIC_H_
#define _DRV_IIC_H_
#include "cam_os_wrapper.h"


#ifdef __KERNEL__
#include <linux/i2c.h>
typedef struct i2c_msg tI2cMsg;

#else
typedef struct i2c_msg {
    u16 addr;   /*slave address*/
    u16 flags;
#define CAM_I2C_RD                   0x0001
#define CAM_I2C_STOP_BEFORE_RESTART  0x0002
#define CAM_I2C_TEN                  0x0010
#define CAM_I2C_DMA_SAFE             0x0200
#define CAM_I2C_RECV_LEN             0x0400
#define CAM_I2C_NO_RD_ACK            0x0800
#define CAM_I2C_IGNORE_NAK           0x1000
#define CAM_I2C_REV_DIR_ADDR         0x2000
#define CAM_I2C_NOSTART              0x4000
#define CAM_I2C_STOP                 0x8000
    u16 len;        /*msg length*/
    u8 *buf;        /*pointer to msg data*/
}tI2cMsg;
#endif

extern s32 sstar_i2c_deinit(u8 Group);
extern s32 sstar_i2c_init(u8 Group);
extern s32 sstar_i2c_master_xfer(u8 Group, struct i2c_msg *para_msg, s32 para_num);
extern s32 sstar_i2c_set_speed(u8 Group, u32 speed);

#ifdef CONFIG_POWER_MANAG_SUPPORT
extern s32 sstar_i2c_suspend(void);
extern s32 sstar_i2c_resume(void);
#endif


#endif
