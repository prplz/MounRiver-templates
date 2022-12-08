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

#-------------------------------------------------------------------------------
# Description of some variables owned by the library
#-------------------------------------------------------------------------------

PATH_H +=\
    $(PATH_i2c)/inc\
    $(PATH_i2c)/pub\
    $(PATH_i2c_os)\
    $(PATH_timer_hal)/pub\
    $(PATH_camclk)/pub\
    $(PATH_camclk_hal)/pub\
    $(PATH_kernel_hal)/pub\
    $(PATH_padmux)/pub

PATH_C +=\
    $(PATH_i2c_hal)/src

#-------------------------------------------------------------------------------
# List of source files of the library or executable to generate
#-------------------------------------------------------------------------------

SRC_C_LIST +=\
    hal_iic.c
