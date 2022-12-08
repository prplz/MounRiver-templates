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
#------------------------------------------------------------------------------
# Description of some variables owned by the library
#------------------------------------------------------------------------------
# Library module (lib) or Binary module (bin)
PROCESS = lib

#PP_OPT_COMMON += __VER_TIMER__

PATH_C +=\
    $(PATH_timer)/src

PATH_H +=\
    $(PATH_timer_hal)/pub\
    $(PATH_timer_hal)/inc

PATH_H += $(PATH_sysdriver)/timer/hal/common/pub/

#------------------------------------------------------------------------------
# List of source files of the library or executable to generate
#------------------------------------------------------------------------------

SRC_C_LIST = \
    drv_timer.c \
    drv_pm_timer.c \
    abi_headers_timer.c

#-------------------------------------------------------------------------------
# Include hal mak
#-------------------------------------------------------------------------------
include $(PATH_sysdriver)/timer/hal/common/timer_hal.mak
