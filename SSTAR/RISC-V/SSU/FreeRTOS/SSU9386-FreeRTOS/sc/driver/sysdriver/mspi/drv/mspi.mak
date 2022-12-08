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

PP_OPT_COMMON += __VER_MSPI__

#-------------------------------------------------------------------------------
# Description of some variables owned by the library

PATH_H += \
    $(PATH_mspi)/inc \
    $(PATH_mspi_hal)/inc \
    $(PATH_gpio)/pub \
    $(PATH_bdma_hal)/inc \
    $(PATH_camclk)/pub \

PATH_C += \
    $(PATH_mspi)/src

#------------------------------------------------------------------------------
# List of source files of the library or executable to generate
#------------------------------------------------------------------------------

SRC_C_LIST = \
    drv_mspi.c \
    drv_mspi_test.c

#-----------------------------------------------------------------------------
# Include hal mak
#-----------------------------------------------------------------------------
include $(PATH_mspi_hal)/mspi_hal.mak
