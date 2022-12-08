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

#ifndef __SYS_SWLA_H__
#define __SYS_SWLA_H__

#include "vm_types.ht"

#define SWLA_LOG_NAME_LENGTH (8)

typedef enum
{
    SS_SWLA_LOG_START = 0,
    SS_SWLA_LOG_STOP = 1,
    SS_SWLA_LOG_LABEL = 2,
    SS_SWLA_LOG_SWITCH_IN = 3
} SS_SWLA_LOG_TYPE_e;

typedef enum
{
    SS_SWLA_DUMP_UART = 0,
    SS_SWLA_DUMP_FS = 1,
} SS_SWLA_DUMP_METHOD_e;

typedef struct __attribute__((packed)) {
    u8   log_type:2;
    u8   cpu_id:4;
    u64  timestamp_ns:58;
    u8   log_name[SWLA_LOG_NAME_LENGTH];
} SS_SWLA_LOG_t;

typedef struct {
    void* handle;
    void (*init)(void *);
    void (*write)(void *, const unsigned char *, u32);
    void (*finish)(void *);
} SwlaOutputOp_t;

#if CONFIG_SYS_SWLA_SUPPORT
void sys_swla_start(u32 enable_overwrite);
void sys_swla_stop(void);
void sys_swla_log_add_event(const char *event_name, const SS_SWLA_LOG_TYPE_e log_type);
void sys_swla_log_add_irq(const u32 irq_num, const SS_SWLA_LOG_TYPE_e log_type);
void sys_swla_dump(SS_SWLA_DUMP_METHOD_e dump_method, const char *file_name);
#else
#define sys_swla_start(...)
#define sys_swla_stop(...)
#define sys_swla_log_add_event(...)
#define sys_swla_log_add_irq(...)
#define sys_swla_dump(...)
#endif
#endif //__SYS_SWLA_H__