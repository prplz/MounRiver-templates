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
#	Description of some variables owned by the library
#-------------------------------------------------------------------------------
# Library module (lib) or Binary module (bin)
PROCESS = lib

PATH_C +=\
    $(PATH_pwm)/src

PATH_H +=\
    $(PATH_pwm)/pub\
    $(PATH_pwm_hal)/inc

#-------------------------------------------------------------------------------
#	List of source files of the library or executable to generate
#-------------------------------------------------------------------------------
SRC_C_LIST =\
    drv_pwm.c\
    drv_pwm_out.c\
    drv_pwm_in.c\
    drv_pwm_test.c\
	drv_pwm_group_test.c\
	drv_pwm_out_test.c\
	drv_pwm_in_test.c
#-------------------------------------------------------------------------------
#	Include hal mak
#-------------------------------------------------------------------------------
include $(PATH_sysdriver)/pwm/hal/$(BB_CHIP_ID)/pwm_hal.mak
