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

#include "sys_sys_isw_cli.h"
#include "initcall.h"
#include "ms_platform.h"
#include "registers.h"
#include "core_portme.h"

#define MBOX_BENCH_STATUS_REG		GET_REG_ADDR(REG_ADDR_BASE_MBOX, 0x06)
#define MBOX_BENCH_VALUE_REG		GET_REG_ADDR(REG_ADDR_BASE_MBOX, 0x07)
#define STATUS_DONE					0x0BEEB

extern int coremark_main(CLI_t *pCli, char *p);
extern ee_u32 get_benchmark_value(void);
extern void set_iteration(ee_s32 iteration);

void RtosAppMainEntry(void)
{
	u16 *p_mbox_status = (u16 *)(MBOX_BENCH_STATUS_REG);
	u16 *p_mbox_value = (u16 *)(MBOX_BENCH_VALUE_REG);
	
	*p_mbox_status = 0x0;
	set_iteration(0); //auto calibration to run about LEAST_SECS seconds
	coremark_main(NULL, NULL);
	*p_mbox_status = STATUS_DONE;
	*p_mbox_value = get_benchmark_value();
}

rtos_application_initcall(RtosAppMainEntry, 0);