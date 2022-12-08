/*
 * mspi_os.h- Sigmastar
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

#ifndef __MSPI_OS__
#define __MSPI_OS__

#include <cam_os_wrapper.h>

#define mspi_dbg 0
#if mspi_dbg == 1
#define mspi_dbgmsg(args...) CamOsPrintf("[MSPI] : " args)
#else
#define mspi_dbgmsg(args...) \
    do                       \
    {                        \
    } while (0)
#endif

#define MSPI_DELAY_US(__us) \
    do                      \
    {                       \
        CamOsUsDelay(__us); \
    } while (0)

#define mspi_errmsg(fmt, ...)  CamOsPrintf("[MSPI] error : " fmt, ##__VA_ARGS__)
#define mspi_warnmsg(fmt, ...) CamOsPrintf("[MSPI] warning : " fmt, ##__VA_ARGS__)

#define mspi_os_virt2phys(_virt_) (u64)CamOsMemVirtToPhys(_virt_)
#define mspi_os_phys2miu(_phys_)  (u32)CamOsMemPhysToMiu(_phys_)

#define mspi_os_cache_flush(_ptr_, _size_)      CamOsMemFlush((void *)_ptr_, (u32)_size_)
#define mspi_os_cache_invalidate(_ptr_, _size_) CamOsMemInvalidate((void *)_ptr_, (u32)_size_)

#endif
