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
#ifndef ___REGS_H
#define ___REGS_H

#include "reg_id.h"

#ifdef CONFIG_MIU0_BUS_BASE
#define ARM_MIU0_BUS_BASE       CONFIG_MIU0_BUS_BASE
#else
#define ARM_MIU0_BUS_BASE       0x20000000
#endif
#define ARM_MIU0_BASE_ADDR      0x00000000
#define ARM_MIU1_BUS_BASE       0xFFFFFFFF
#define ARM_MIU1_BASE_ADDR      0xFFFFFFFF
#define ARM_MIU2_BUS_BASE       0xFFFFFFFF
#define ARM_MIU2_BASE_ADDR      0xFFFFFFFF

#ifdef CONFIG_CPU_ARCH_RISCV
#define BASE_REG_RIU_PA         0x02000000
#else
#define BASE_REG_RIU_PA         0x1F000000
#endif
#define BASE_REG_PADTOP_PA      GET_BASE_ADDR_BY_BANK(BASE_REG_RIU_PA, 0x103C00)
#define BASE_REG_PMSLEEP_PA     GET_BASE_ADDR_BY_BANK(BASE_REG_RIU_PA, 0x0E00)
#define BASE_REG_PMGPIO_PA      GET_BASE_ADDR_BY_BANK(BASE_REG_RIU_PA, 0x0F00)
#define BASE_REG_PMSAR_PA       GET_BASE_ADDR_BY_BANK(BASE_REG_RIU_PA, 0x1400)
#define BASE_REG_PMTOP_PA       GET_BASE_ADDR_BY_BANK(BASE_REG_RIU_PA, 0x1E00)
#define BASE_REG_EFUSE_PA       GET_BASE_ADDR_BY_BANK(BASE_REG_RIU_PA, 0x2000)
#define BASE_REG_MAILBOX_PA     GET_BASE_ADDR_BY_BANK(BASE_REG_RIU_PA, 0x100400)
#define BASE_REG_INTRCTL_PA     GET_BASE_ADDR_BY_BANK(BASE_REG_RIU_PA, 0x100900)
#define BASE_REG_MOVDMA_PA      GET_BASE_ADDR_BY_BANK(BASE_REG_RIU_PA, 0x100B00)
#define BASE_REG_ATOP_PA        GET_BASE_ADDR_BY_BANK(BASE_REG_RIU_PA, 0x101000)
#define BASE_REG_MIU_PA         GET_BASE_ADDR_BY_BANK(BASE_REG_RIU_PA, 0x101200)
#define BASE_REG_OTP_PA         GET_BASE_ADDR_BY_BANK(BASE_REG_RIU_PA, 0x101800)
#define BASE_REG_MIUPLL_PA      GET_BASE_ADDR_BY_BANK(BASE_REG_RIU_PA, 0x103100)
#define BASE_REG_GPI_INT_PA     GET_BASE_ADDR_BY_BANK(BASE_REG_RIU_PA, 0x103D00)
#define BASE_REG_GPI2_INT_PA    GET_BASE_ADDR_BY_BANK(BASE_REG_RIU_PA, 0x110500)

///////////////////////////////////////////////////////////////////////////////

#define REG(Reg_Addr)                       (*(volatile U16*)(Reg_Addr))
#define GET_REG_ADDR(x, y)                  ((x) + ((y) << 2))
#define GET_BASE_ADDR_BY_BANK(x, y)         ((x) + ((y) << 1))

#define MS_GIC_BASE_PA                      (0x16000000)
#define MS_GIC_CPU_INTERFACE_OFFSET         (0x1000) // UnUsed

#ifdef CONFIG_CPU_ARCH_RISCV
#define MS_BASE_REG_RIU_PA                  (0x02000000)
#define MS_BASE_REG_IMI_PA                  (0x04000000)
#else
#define MS_BASE_REG_RIU_PA                  (0x1F000000)
#define MS_BASE_REG_IMI_PA                  (0xA0000000)
#endif

#define MS_BASE_REG_PM_UART_PA              GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x003500)
#define MS_BASE_REG_FUART_PA                GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x110200)
#define MS_BASE_REG_URDMA_PA                GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x110300)
#define MS_BASE_REG_UART0_PA                GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x110800)
#define MS_BASE_REG_UART1_PA                GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x110900)
#define MS_BASE_REG_UART2_PA                GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x110A00)
#define MS_BASE_REG_UART3_PA                GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x110B00)
#define MS_BASE_REG_UART4_PA                GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x110C00)

#define MS_BASE_REG_TIMER0_PA               GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x003020)
#define MS_BASE_REG_CHIPTOP_PA              GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x101E00)
#define MS_BASE_REG_INTRCTL_PA              GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x100900)
#define MS_BASE_REG_INTRCTL2_PA             GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x101500)
#define MS_BASE_REG_INTRCTL3_PA             GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x101600)
#define MS_BASE_REG_INTRCTL4_PA             GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x101700)
#define MS_BASE_REG_FSP_PA                  GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x0016C0)
#define MS_BASE_REG_QSPI_PA                 GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x001780)
#define MS_BASE_REG_CLKGEN_PA               GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x103800)
#define MS_BASE_REG_IIC0_PA                 GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x111800)
#define MS_BASE_REG_IIC1_PA                 GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x111900)
#define MS_BASE_REG_CSI_PA                  GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x120400)
#define MS_BASE_REG_VIF_PA                  GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x131200)
#define MS_BASE_REG_CKG_PA                  GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x103800)
#define MS_BASE_REG_SC_GP_CTRL_PA           GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x113300)
#define MS_BASE_REG_PM_SLEEP_CKG_PA         GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x000E00)
#define MS_BASE_REG_MIU_PA                  GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x101200)
#define MS_BASE_REG_PMTOP_PA                GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x001E00)

#define REG_ADDR_BASE_WDT                   GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x003000)
#define REG_ADDR_BASE_AESDMA                GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x112200)
#define MS_BASE_REG_L3_BRIDGE_PA            GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x102200)
#define REG_ADDR_BASE_DID_KEY               GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x003800)
#define REG_ADDR_BASE_BDMA                  GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x100200)
#define REG_ADDR_BASE_RTC                   GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x001200)
#define REG_ADDR_BASE_RTCPWC                GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x003400)
#define REG_ADDR_BASE_MBOX                  GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x100400)

#define REG_ADDR_BASE_BDMA0                 GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x100200)
#define REG_ADDR_BASE_BDMA1                 GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x100B00)
#define REG_ADDR_BASE_BDMA2                 GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x100C00)
//#define REG_ADDR_BASE_BDMA3                 GET_BASE_ADDR_BY_BANK(MS_BASE_REG_RIU_PA, 0x1002C0)

#define BASE_REG_TIMER                      MS_BASE_REG_TIMER0_PA
#define TIMER0_BASE_REG                     GET_REG_ADDR(BASE_REG_TIMER, 0x00)
#define TIMER1_BASE_REG                     GET_REG_ADDR(BASE_REG_TIMER, 0x10)
#define TIMER2_BASE_REG                     GET_REG_ADDR(BASE_REG_TIMER, 0x20)

#define BASE_REG_INTRCTL                    MS_BASE_REG_INTRCTL_PA
#define INTRCTL_HST0_BASE_REG               GET_REG_ADDR(BASE_REG_INTRCTL, 0x00)
#define INTRCTL_HST1_BASE_REG               GET_REG_ADDR(BASE_REG_INTRCTL, 0x20)
#define INTRCTL_HST2_BASE_REG               GET_REG_ADDR(BASE_REG_INTRCTL, 0x40)
#define INTRCTL_HST3_BASE_REG               GET_REG_ADDR(BASE_REG_INTRCTL, 0x60)

#define BASE_REG_INTRCTL2                   MS_BASE_REG_INTRCTL2_PA
#define INTRCTL2_HST0_BASE_REG              GET_REG_ADDR(BASE_REG_INTRCTL2, 0x00)
#define INTRCTL2_HST1_BASE_REG              GET_REG_ADDR(BASE_REG_INTRCTL2, 0x20)
#define INTRCTL2_HST2_BASE_REG              GET_REG_ADDR(BASE_REG_INTRCTL2, 0x40)
#define INTRCTL2_HST3_BASE_REG              GET_REG_ADDR(BASE_REG_INTRCTL2, 0x60)

#define BASE_REG_INTRCTL3                   MS_BASE_REG_INTRCTL3_PA
#define INTRCTL3_HST6_BASE_REG              GET_REG_ADDR(BASE_REG_INTRCTL3, 0x40)

#define BASE_REG_INTRCTL4                   MS_BASE_REG_INTRCTL4_PA
#define INTRCTL4_HST6_BASE_REG              GET_REG_ADDR(BASE_REG_INTRCTL4, 0x40)

#define WDT_BASE_REG                        REG_ADDR_BASE_WDT

#define MBOX_RTOS_TO_LINUX_BASE_REG         GET_REG_ADDR(REG_ADDR_BASE_MBOX, 0x50)
#define MBOX_LINUX_TO_RTOS_BASE_REG         GET_REG_ADDR(REG_ADDR_BASE_MBOX, 0x60)

#endif // ___REGS_H
