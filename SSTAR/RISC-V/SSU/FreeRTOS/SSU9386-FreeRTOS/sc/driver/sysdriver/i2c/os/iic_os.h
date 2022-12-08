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

#ifndef __IIC_OS__
#define __IIC_OS__

#define I2C_DELAY_N_US(_x) \
    do                     \
    {                      \
        CamOsUsDelay(_x);  \
    } while (0)

//#define DMSG_I2C_HAL_DEBUG
#define dmsg_i2c_halerr(fmt, ...)                                         \
    do                                                                    \
    {                                                                     \
        CamOsPrintf("[hal_i2c_err] <%s>, " fmt, __func__, ##__VA_ARGS__); \
    } while (0)
#ifdef DMSG_I2C_HAL_DEBUG
#define dmsg_i2c_halwarn(fmt, ...)                                        \
    do                                                                    \
    {                                                                     \
        CamOsPrintf("[hal_i2c_warn] <%s> " fmt, __func__, ##__VA_ARGS__); \
    } while (0)
#else
#define dmsg_i2c_halwarn(fmt, ...)
#endif

#define i2c_os_cache_flush(_ptr_, _size_)      CamOsMemFlush((void *)_ptr_, (u32)_size_)
#define i2c_os_cache_invalidate(_ptr_, _size_) CamOsMemInvalidate((void *)_ptr_, (u32)_size_)

#endif
