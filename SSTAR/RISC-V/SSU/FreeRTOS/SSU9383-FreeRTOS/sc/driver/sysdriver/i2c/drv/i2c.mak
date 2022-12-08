# SigmaStar trade secret
# Copyright (c) [2019~2020] SigmaStar Technology.
# All rights reserved.
#
# Unless otherwise stipulated in writing, any and all information contained
# herein regardless in any format shall remain the sole proprietary of
# SigmaStar and be kept in strict confidence
# (SigmaStar Confidential Information) by the recipient.
# Any unauthorized act including without limitation unauthorized disclosure,
# copying, use, reproduction, sale, distribution, modification, disassembling,
# reverse engineering and compiling of the contents of SigmaStar Confidential
# Information is unlawful and strictly prohibited. SigmaStar hereby reserves the
# rights to any and all damages, losses, costs and expenses resulting therefrom.
#

PP_OPT_COMMON += __VER_I2C__

#-------------------------------------------------------------------------------
# Description of some variables owned by the library
#-------------------------------------------------------------------------------

PATH_C +=\
    $(PATH_i2c)/src
PATH_H +=\
    $(PATH_i2c)/pub\
    $(PATH_i2c_hal)/inc\
    $(PATH_kernel_hal)/pub\
    $(PATH_int_hal)/pub\
    $(PATH_padmux)/pub\
    $(PATH_system)/initcall/pub\
    $(PATH_system)/memmang/pub\
    $(PATH_sysdesc_hal)/pub\
    $(PATH_camclk)/pub\
    $(PATH_sysdesc)/pub
#-------------------------------------------------------------------------------
# List of source files of the library or executable to generate
#-------------------------------------------------------------------------------

SRC_C_LIST =\
    drv_iic.c \
    ms_iic_rtk.c \
    cam_drv_i2c.c \
    drv_i2c_test.c

#-------------------------------------------------------------------------------
# Include hal mak
#-------------------------------------------------------------------------------
include $(PATH_i2c_hal)/i2c_hal.mak
