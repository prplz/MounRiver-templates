/*
 * drv_ir.h- Sigmastar
 *
 * Copyright (c) [2019~2020] SigmaStar Technology.
 *
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.
 *
 */

#ifndef _DRV_IR_H_
#define _DRV_IR_H_
#include "cam_os_wrapper.h"

/*
 * Struct declaration
 */

struct resource
{
    u32 start;
    u16 size;
} __attribute__ ((__packed__));

#define MAXQUEUE 100
struct key_queue
{
    u32 item[MAXQUEUE];
    int front;
    int rear;
};

struct sstar_ir_dev
{
    u8                 queue_lock;
    u16                dev_id;
    u32                key_value;
    u32                irq;
    u32                rc_irq;
    u32                camclk_id;
    u32                group;
    u32                decode_mode;
    struct key_queue   queue;
    struct hal_ir_dev *hal_ir_dev;
    void (*calbak_input_key)(struct sstar_ir_dev *);
};

extern u32 drv_ir_key_dequeue(struct sstar_ir_dev *ir_dev);
extern struct sstar_ir_dev* drv_ir_get_addr(int group);
extern int drv_ir_config(struct sstar_ir_dev *ir_dev);

#endif
