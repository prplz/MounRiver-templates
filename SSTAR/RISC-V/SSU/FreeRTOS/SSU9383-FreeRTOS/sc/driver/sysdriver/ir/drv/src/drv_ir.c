/*
 * drv_ir.c- Sigmastar
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

#include "drv_ir.h"
#include "hal_ir.h"
#include "hal_ir_cfg.h"
#include "errno.h"
#ifdef CONFIG_GPIO_SUPPORT
#include "mdrv_gpio.h"
#endif
#ifdef CONFIG_CAMCLK_SUPPORT
#include "drv_camclk_Api.h"
#endif
#ifdef CONFIG_SYSDESC_SUPPORT
#include "drv_sysdesc.h"
#endif
#include "initcall.h"

static struct sstar_ir_dev* ir_addr[IR_GROUP] = {NULL};

 u32 drv_ir_key_dequeue(struct sstar_ir_dev *ir_dev)
{
    u32 data = 0x00;

    if (!ir_dev->queue_lock)
        return data;

    if (ir_dev->queue.front == ir_dev->queue.rear)
    {
        ir_dbg("queue is empty \n");
    }
    else
    {
        data = ir_dev->queue.item[ir_dev->queue.front];
        ir_dev->queue.front = (ir_dev->queue.front + 1) % MAXQUEUE;
    }

    ir_dev->queue_lock = 0;

    return data;
}

void drv_ir_key_enqueue(struct sstar_ir_dev *ir_dev)
{
    if (ir_dev->queue_lock)
        return;

    if (ir_dev->queue.front == ((ir_dev->queue.rear + 1) % MAXQUEUE))
    {
        ir_dbg("queue is full \n");
    }
    else
    {
        ir_dev->queue.item[ir_dev->queue.rear] = ir_dev->key_value;
        ir_dev->queue.rear = (ir_dev->queue.rear + 1) % MAXQUEUE;
    }

    ir_dev->queue_lock = 1;
}

void drv_ir_irq_handler(u32 nIrq, void *pDevId)
{
    struct sstar_ir_dev *ir_dev = (struct sstar_ir_dev *)pDevId;

    ir_dbg("IR%u IRQ IN\n", ir_dev->group);
    ir_dev->key_value = hal_get_key(ir_dev->hal_ir_dev);
    if (ir_dev->key_value)
    {
        drv_ir_key_enqueue(ir_dev);
    }
    if (ir_dev->calbak_input_key != NULL)
        ir_dev->calbak_input_key(ir_dev);
}

int drv_ir_config(struct sstar_ir_dev *ir_dev)
{
    if (ir_dev->group < IR_GROUP)
    {
        ir_dev->hal_ir_dev->decode_mode = ir_dev->decode_mode;
        hal_ir_config(ir_dev->hal_ir_dev);
    }
    else
    {
        ir_dbg("drv_ir_config() fail\n");
        return -1;
    }

    return 0;
}

struct sstar_ir_dev* drv_ir_get_addr(int group)
{
    if (group < IR_GROUP)
        return ir_addr[group];
    else
        return NULL;
}

static void drv_ir_set_addr(int group, struct sstar_ir_dev *ir_dev)
{
    ir_addr[group] = ir_dev;
}

static void drv_ir_release(struct sstar_ir_dev *ir_dev)
{
    CamOsMemRelease(ir_dev->hal_ir_dev->ir_keyinfo);
    CamOsMemRelease(ir_dev->hal_ir_dev);
    CamOsMemRelease(ir_dev);
}

u64 drv_ir_get_systime(void)
{
    return CamOsGetTimeInMs();
}

static int drv_ir_padmod_set(struct sstar_ir_dev *ir_dev)
{

#ifdef CONFIG_PADMUX_SUPPORT
    ir_info("please confirm set padmux in xx-padmux.c already!\n");
#else
#ifdef CONFIG_GPIO_SUPPORT
    MDrv_GPIO_PadGroupMode_Set(ir_cfg[ir_dev->group].pad_mod);
#else
    ir_info("ir padmod setting by operation register\n");
    IR_WRITE_REG_MASK(ir_reg[ir_dev->group].ir_reg_padmod.bank_base, \
                      ir_reg[ir_dev->group].ir_reg_padmod.reg_offset,\
                      ir_cfg[ir_dev->group].pad_mod <<               \
                      ir_reg[ir_dev->group].ir_reg_padmod.bit_shift, \
                      ir_reg[ir_dev->group].ir_reg_padmod.bit_mask);
#endif
#endif

    return 0;
}

static int drv_ir_irq_request(struct sstar_ir_dev *ir_dev)
{
    u8          retval  = 0;
    int         irq_ret = 1;
    int         rc_irq_ret = 1;
    static char ir_irqname[4];
    static char rc_irqname[4];

#ifdef CONFIG_SYSDESC_SUPPORT
#ifdef SYSDESC_PRO_interrupts_u32_u32
    u32         irqs[2];
    retval = MDrv_SysDesc_Read_MultiTypes(ir_dev->dev_id, SYSDESC_PRO_interrupts_u32_u32, \
                                          &irqs, sizeof(irqs), sizeof(irqs));
    if (retval != E_SYS_DESC_PASS)
    {
        ir_dbg("Fail to get interrupts_u32_u32 property\n");
        goto out;
    }
    else
    {
        ir_info("ir irq num setting by sysdesc property\n");
        ir_dev->irq    = irqs[0];
        ir_dev->rc_irq = irqs[1];
        ir_dbg("irq = %d, rc_irq = %d\n", ir_dev->irq, ir_dev->rc_irq);
    }
#else
    retval = 1;
    goto out;
#endif
#else
    ir_info("ir irq acquire by initialization definition in hal_ir_cfg.h\n");
    ir_dev->irq    = ir_cfg[ir_dev->group].irq;
    ir_dev->rc_irq = ir_cfg[ir_dev->group].rc_irq;
#endif

    if(CamOsSnprintf(ir_irqname, sizeof(ir_irqname), "IR%d", ir_dev->group) >= 0)
        irq_ret = CamOsIrqRequest(ir_dev->irq, (CamOsIrqHandler)drv_ir_irq_handler, ir_irqname, ir_dev);
    if((CamOsSnprintf(rc_irqname, sizeof(rc_irqname), "RC%d", ir_dev->group) >= 0) && (ir_dev->group == 0))
        rc_irq_ret = CamOsIrqRequest(ir_dev->rc_irq, (CamOsIrqHandler)drv_ir_irq_handler, rc_irqname, ir_dev);
    if (irq_ret != 0 && rc_irq_ret!=0)
    {
        ir_dbg("%s:%d request_irq() failed\n", __func__, __LINE__);
        retval = irq_ret;
        goto out;
    }

    CamOsIrqDisable(ir_dev->irq);
    if (ir_dev->group == 0)
        CamOsIrqDisable(ir_dev->rc_irq);
    CamOsIrqEnable(ir_dev->irq);
    if (ir_dev->group == 0)
        CamOsIrqEnable(ir_dev->rc_irq);

out:
    return retval;
}

#ifdef CONFIG_CAMCLK_SUPPORT
static u8 drv_ir_set_clk(struct sstar_ir_dev *ir_dev)
{
    u8    retval;
    void *pvIrClk;
    void *ClkName;
    CAMCLK_Set_Attribute stSetCfg;
    CAMCLK_Get_Attribute stGetCfg;

#ifdef CONFIG_SYSDESC_SUPPORT
#ifdef SYSDESC_PRO_camclk_u32
    retval = MDrv_SysDesc_Read_U32(ir_dev->dev_id, SYSDESC_PRO_camclk_u32, &ir_dev->camclk_id);
    if (retval != E_SYS_DESC_PASS)
    {
        ir_dbg("Fail to get camclk_u32 property\n");
        goto out;
    }
#else
    retval = 1;
    goto out;
#endif
#else
    ir_dev->camclk_id = ir_cfg[ir_dev->group].clk_mod;
#endif

    ClkName = "IR";
    retval = CamClkRegister(ClkName, ir_dev->camclk_id, &pvIrClk);
    if (retval == CAMCLK_RET_OK)
    {
        CamClkAttrGet(pvIrClk, &stGetCfg);
        CAMCLK_SETPARENT(stSetCfg, stGetCfg.u32Parent[1]);
        CamClkAttrSet(pvIrClk, &stSetCfg);
        CamClkSetOnOff(pvIrClk, 1);
    }
    else
    {
        ir_dbg("CamClkRegister() fail\n");
        goto out;;
    }

out:
    return retval;
}
#endif

#ifdef CONFIG_SYSDESC_SUPPORT
static u8 drv_ir_get_property(struct sstar_ir_dev *ir_dev)
{
    u8 status;
    u8 retval;
    struct resource phy_addr;

#ifdef SYSDESC_PRO_status_u8
        retval = MDrv_SysDesc_Read_U8(ir_dev->dev_id, SYSDESC_PRO_status_u8, &status);
        if (retval != E_SYS_DESC_PASS)
        {
            ir_dbg("Fail to get status_u8 property\n");
            return retval;
        }
        else if (!status)
        {
            ir_dbg("Fail to get status enable\n");
            return E_SYS_DESC_CONTENT_ERROR;
        }
#else
        retval = 1;
        goto out;
#endif

#ifdef SYSDESC_PRO_reg_u32_u16
        retval = MDrv_SysDesc_Read_MultiTypes(ir_dev->dev_id, SYSDESC_PRO_reg_u32_u16, &phy_addr, \
                                              sizeof(struct resource), sizeof(struct resource));
        if (retval != E_SYS_DESC_PASS)
        {
            ir_dbg("Fail to get reg_u32_16 property\n");
            goto out;
        }
        else if (!phy_addr.start)
        {
            ir_dbg("Fail to get ir physical base address\n");
            retval = E_SYS_DESC_CONTENT_ERROR;
            goto out;
        }
        else
        {
            ir_dev->hal_ir_dev->membase = (u64)phy_addr.start;
            ir_dbg("membase = [0x%llx]\n", ir_dev->hal_ir_dev->membase);
        }
#else
        retval = 2;
        goto out;
#endif

#ifdef SYSDESC_PRO_mode_u32
    retval = MDrv_SysDesc_Read_U32(ir_dev->dev_id, SYSDESC_PRO_mode_u32, &ir_dev->hal_ir_dev->decode_mode);
    if (retval != E_SYS_DESC_PASS)
    {
        ir_dbg("Fail to get mode_u32 property\n");
        goto out;
    }
#else
    retval = 3;
    goto out;
#endif

#ifdef SYSDESC_PRO_header_code_u32_u32
    retval = MDrv_SysDesc_Read_MultiTypes(ir_dev->dev_id, SYSDESC_PRO_header_code_u32_u32, \
                                          &ir_dev->hal_ir_dev->header_code,                \
                                          sizeof(ir_dev->hal_ir_dev->header_code),         \
                                          sizeof(ir_dev->hal_ir_dev->header_code));
    if (retval != E_SYS_DESC_PASS)
    {
        ir_dbg("Fail to get header_code_u32_u32 property\n");
        return retval;
    }
#else
    retval = 4;
    goto out;
#endif

out:
    return retval;
}
#endif

static int drv_ir_init(int group)
{
    u8 retval;
    struct sstar_ir_dev *ir_dev = NULL;

    ir_dev                         = CamOsMemAlloc(sizeof(struct sstar_ir_dev));
    ir_dev->hal_ir_dev             = CamOsMemAlloc(sizeof(struct hal_ir_dev));
    ir_dev->hal_ir_dev->ir_keyinfo = CamOsMemAlloc(sizeof(struct key_info));
    if (!ir_dev || !ir_dev->hal_ir_dev || !ir_dev->hal_ir_dev->ir_keyinfo)
    {
        ir_dbg("%s:%d CamOsMemAlloc() failed\n", __func__, __LINE__);
        return -ENOMEM;
    }

    ir_dev->queue_lock       = 0;
    ir_dev->key_value        = 0;
    ir_dev->group            = group;
    ir_dev->calbak_input_key = NULL;
    ir_dev->dev_id = ir_cfg[ir_dev->group].dev_id;

#ifdef CONFIG_SYSDESC_SUPPORT
    retval = drv_ir_get_property(ir_dev);
    if (!retval)
    {
        ir_info("ir use sysdesc property\n");
    }
    else
    {
        ir_dbg("drv_ir_get_property() fail\n", ir_dev->group);
        drv_ir_release(ir_dev);
        return retval;
    }
#else
        ir_info("ir property use initialization definition in hal_ir_cfg.h\n");
        ir_dev->hal_ir_dev->membase        = ir_reg[ir_dev->group].ir_bank_base;
        ir_dbg("membase = [0x%llx]\n", ir_dev->hal_ir_dev->membase);
        ir_dev->hal_ir_dev->decode_mode    = ir_cfg[ir_dev->group].decode_mode;
        ir_dbg("mode = [%d]\n", ir_dev->hal_ir_dev->decode_mode);
        ir_dev->hal_ir_dev->header_code[0] = ir_cfg[ir_dev->group].header_code[0];
        ir_dev->hal_ir_dev->header_code[1] = ir_cfg[ir_dev->group].header_code[1];
        ir_dbg("header_code[0] = 0x%x, header_code[1] = 0x%x\n", ir_dev->hal_ir_dev->header_code[0],\
                                                                 ir_dev->hal_ir_dev->header_code[1]);
#endif

#ifdef CONFIG_CAMCLK_SUPPORT
    retval = drv_ir_set_clk(ir_dev);
    if (!retval)
    {
        ir_info("ir clk setting by camclk\n");
    }
    else
    {
        ir_dbg("drv_ir_set_clk() fail\n", ir_dev->group);
        drv_ir_release(ir_dev);
        return retval;
    }
#else
        ir_info("ir clk setting by operation register\n");
        IR_WRITE_REG_MASK(ir_reg[ir_dev->group].ir_reg_clkgen.bank_base, \
                          ir_reg[ir_dev->group].ir_reg_clkgen.reg_offset,\
                          ir_cfg[ir_dev->group].clk_mod <<               \
                          ir_reg[ir_dev->group].ir_reg_clkgen.bit_shift, \
                          ir_reg[ir_dev->group].ir_reg_clkgen.bit_mask);
#endif

    retval = drv_ir_irq_request(ir_dev);
    if (retval)
    {
        ir_dbg("drv_ir_irq_request() fail\n");
        drv_ir_release(ir_dev);
        return retval;
    }

    drv_ir_padmod_set(ir_dev);

    hal_ir_init(ir_dev->hal_ir_dev);
    ir_dev->hal_ir_dev->calbak_get_sys_time = drv_ir_get_systime;

    drv_ir_set_addr(group, ir_dev);
    ir_dbg("ir group %d init done\n", ir_dev->group);

    return 0;
}

void drv_ir_probe(void)
{
    int i;

#ifdef CONFIG_SYSDESC_SUPPORT
    for (i = 0; i < IR_GROUP; i++)
    {
        if (ir_cfg[i].dev_id != SYSDESC_DEV_IR_NODEF)
            drv_ir_init(i);
        else
            ir_err("ir group %d init fail\n", i);
    }
#else
    for (i = 0; i < IR_GROUP; i++)
    {
        if (ir_cfg[i].enable == 1)
            drv_ir_init(i);
        else
            ir_err("ir group %d init fail\n", i);
    }
#endif
}

rtos_late_initcall(drv_ir_probe);
