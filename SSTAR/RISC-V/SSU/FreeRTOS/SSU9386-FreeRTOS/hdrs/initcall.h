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

#ifndef __RTOS_INIT_H__
#define __RTOS_INIT_H__

///////////////////////////////////////////////////////////////////////

typedef void (*initcall_t)(void);

#define __used __attribute__((__used__))

// vm_types.ht redefine the __section__.
#ifdef __section__
#undef __section__
#endif

///////////////////////////////////////////////////////////////////////

#define __define_premain_initcall(fn, id) \
    static initcall_t __initcall_##fn##id __used __attribute__((__section__(".premain_initcall" #id ".init"))) = fn;

#define premain_1st_initcall(fn)        __define_premain_initcall(fn, 0)
#define premain_2nd_initcall(fn)        __define_premain_initcall(fn, 1)

///////////////////////////////////////////////////////////////////////

#define __define_normal_initcall(fn, id)         \
    static initcall_t __initcall_##fn##id __used __attribute__((__section__(".initcall" #id ".init"))) = fn;

#define rtos_pure_initcall(fn)          __define_normal_initcall(fn, 0)
#define rtos_core_initcall(fn)          __define_normal_initcall(fn, 1)
#define rtos_postcore_initcall(fn)      __define_normal_initcall(fn, 2)
#define rtos_arch_initcall(fn)          __define_normal_initcall(fn, 3)
#define rtos_subsys_initcall(fn)        __define_normal_initcall(fn, 4)
#define rtos_fs_initcall(fn)            __define_normal_initcall(fn, 5)
#define rtos_device_initcall(fn)        __define_normal_initcall(fn, 6)
#define rtos_late_initcall(fn)          __define_normal_initcall(fn, 7)

///////////////////////////////////////////////////////////////////////

#define __define_application_initcall(fn, id) \
    static initcall_t __initcall_##fn##id __used __attribute__((__section__(".application_initcall" #id ".init"))) = fn;

#define rtos_application_initcall(fn, id)  __define_application_initcall(fn, id)

///////////////////////////////////////////////////////////////////////

void do_rtos_premain_initcalls(u32 u32Select);
void do_rtos_normal_initcalls(void);
void do_rtos_application_initcalls(void);

#endif /* __RTOS_INIT_H__ */
