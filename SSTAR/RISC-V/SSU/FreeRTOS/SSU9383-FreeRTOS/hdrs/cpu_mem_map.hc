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

#ifndef __CPU_MEM_MAP_HC__
#define __CPU_MEM_MAP_HC__

#include "registers.h"

#define COMMON_START_ADDRESS        0x00000000
#ifdef CONFIG_LPAE_SUPPORT
#define COMMON_START_ADDRESS_VA     0x00200000
#else
#define COMMON_START_ADDRESS_VA     0x00100000
#endif
#define COMMON_ADDR_SPACE_SZ        0x04000000 // 64MB

#define SPI_START_ADDRESS           0x14000000
#define SPI_ADDR_SPACE_SZ           0x02000000 // 32MB

#define GIC_START_ADDRESS           0x16000000
#define GIC_ADDR_SPACE_SZ           0x00800000 // 8MB // 0x00100000

#define IPU_START_ADDRESS           0x18000000
#define IPU_ADDR_SPACE_SZ           0x00800000 // 8MB

#define IO_START_ADDRESS            0x02000000
#define IO_ADDR_SPACE_SZ            0x00400000 // 4MB (PM+NonPM RIU)

#define IMI_START_ADDRESS           0x04000000
#define IMI_ADDR_SPACE_SZ           0x00100000 // 1MB

/////////////////////////////////////////////////////////////////////////

#ifdef CONFIG_MIU0_BUS_BASE
#define MIU_BASE_PHYADDR            CONFIG_MIU0_BUS_BASE
#else
#define MIU_BASE_PHYADDR            0x20000000
#endif
#define MIU_MAX_PHYADDR             (MIU_BASE_PHYADDR + CONFIG_SYS_PHY_DRAM_SIZE)
#define MIU_ADDR_MASK               0x3FFFFFFF // This mask varies by CHIP.

#ifdef CONFIG_LPAE_SUPPORT
#ifdef CONFIG_MIU0_BUS_HIGH_BASE
#define MIU_BASE_HIGH_PHYADDR       CONFIG_MIU0_BUS_HIGH_BASE
#else
#define MIU_BASE_HIGH_PHYADDR       0x0100000000
#endif
#define MIU_MAX_HIGH_PHYADDR        (MIU_BASE_HIGH_PHYADDR + CONFIG_SYS_PHY_DRAM_SIZE)
#endif

#endif /* __CPU_MEM_MAP_HC__ */
