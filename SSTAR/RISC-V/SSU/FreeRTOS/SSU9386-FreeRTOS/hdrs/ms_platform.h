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

#ifndef __MS_PLATFORM_H__
#define __MS_PLATFORM_H__

#define MS_IO_OFFSET 0x00000000

/* macro to get at MMIO space when running virtually */
#define IO_ADDRESS(x)           ( (u32)(x) + MS_IO_OFFSET )

// Register macros
#define GET_REG_ADDR(x, y)           ((x) + ((y) << 2))
#define GET_BASE_ADDR_BY_BANK(x, y)  ((x) + ((y) << 1))

/* read register by byte */
#define ms_readb(a)     (*(volatile unsigned char *)IO_ADDRESS(a))

/* read register by word */
#define ms_readw(a)     (*(volatile unsigned short *)IO_ADDRESS(a))

/* read register by long */
#define ms_readl(a)     (*(volatile unsigned int *)IO_ADDRESS(a))

/* write register by byte */
#define ms_writeb(v,a)  (*(volatile unsigned char *)IO_ADDRESS(a) = (v))

/* write register by word */
#define ms_writew(v,a)  (*(volatile unsigned short *)IO_ADDRESS(a) = (v))

/* write register by long */
#define ms_writel(v,a)  (*(volatile unsigned int *)IO_ADDRESS(a) = (v))

//------------------------------------------------------------------------------
//
//  Macros:  INREGx/OUTREGx/SETREGx/CLRREGx
//
//  This macros encapsulates basic I/O operations.
//  Memory address space operation is used on all platforms.
//
#define INREG8(x)               ms_readb(x)
#define OUTREG8(x, y)           ms_writeb((u8)(y), x)
#define SETREG8(x, y)           OUTREG8(x, INREG8(x)|(y))
#define CLRREG8(x, y)           OUTREG8(x, INREG8(x)&~(y))
#define INREGMSK8(x, y)         (INREG8(x) & (y))
#define OUTREGMSK8(x, y, z)     OUTREG8(x, ((INREG8(x)&~(z))|((y)&(z))))

#define INREG16(x)              ms_readw(x)
#define OUTREG16(x, y)          ms_writew((u16)(y), x)
#define SETREG16(x, y)          OUTREG16(x, INREG16(x)|(y))
#define CLRREG16(x, y)          OUTREG16(x, INREG16(x)&~(y))
#define INREGMSK16(x, y)        (INREG16(x) & (y))
#define OUTREGMSK16(x, y, z)    OUTREG16(x, ((INREG16(x)&~(z))|((y)&(z))))

/* Make sure riu bit operation enabled, or the effect of SETREG16_BIT_OP is the same as SETREG16 */
#define SETREG16_BIT_OP(x, y)       OUTREG32(x, INREG16(x)|(y)|(u32)(y)<<16)
/* Make sure riu bit operation enabled, or the effect of CLRREG16_BIT_OP is the same as CLRREG16 */
#define CLRREG16_BIT_OP(x, y)       OUTREG32(x, (INREG16(x)&~(y))|(u32)(y)<<16)
/* Make sure riu bit operation enabled, or the effect of OUTREGMSK16_BIT_OP is the same as OUTREGMSK16 */
#define OUTREGMSK16_BIT_OP(x, y, z) OUTREG32(x, ((INREG16(x)&~(z))|((y)&(z)))|(u32)(z)<<16)

#define INREG32(x)              ms_readl(x)
#define OUTREG32(x, y)          ms_writel((u32)(y), x)
#define SETREG32(x, y)          OUTREG32(x, INREG32(x)|(y))
#define CLRREG32(x, y)          OUTREG32(x, INREG32(x)&~(y))
#define INREGMSK32(x, y)        (INREG32(x) & (y))
#define OUTREGMSK32(x, y, z)    OUTREG32(x, ((INREG32(x)&~(z))|((y)&(z))))

#define PM_READ32(x)            ((((unsigned int)INREG16(x+4)<<16)&0xFFFF0000) | (INREG16(x) & 0x0000FFFF))
#define PM_WRITE32(x,y)         do{OUTREG16(x,(y&0x0000FFFF));OUTREG16((x+4),((y>>16)&0x0000FFFF));}while(0)

#ifdef CONFIG_CPU_ARCH_RISCV
#define BYTE_ENABLE_BUG_WORKAROUND (OUTREG32((GET_BASE_ADDR_BY_BANK(BASE_REG_RIU_PA, 0x101E00) + REG_ID_73), 0xFFFFFFFF))
#else
#define BYTE_ENABLE_BUG_WORKAROUND
#endif

#endif //__MS_PLATFORM_H__
