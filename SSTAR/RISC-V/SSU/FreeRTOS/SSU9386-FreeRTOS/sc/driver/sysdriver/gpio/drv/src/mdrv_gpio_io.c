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

#include "mdrv_gpio_io.h"
#include "mhal_gpio.h"
#include "mdrv_gpio.h"
#include "ms_platform.h"
#include "gpio.h"


//#include "mdrv_probe.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define GPIO_DBG_ENABLE              0

#if GPIO_DBG_ENABLE
#define GPIO_DBG(_f)                 (_f)
#else
#define GPIO_DBG(_f)
#endif

#if 0
#define LINE_DBG()                  printf("GPIO %d\n", __LINE__)
#else
#define LINE_DBG()
#endif

#define GPIO_PRINT(fmt, args...)        //printk("\n[GPIO][%05d] " fmt, __LINE__, ## args)

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

int camdriver_gpio_request(struct gpio_chip *chip, unsigned offset)
{
    MDrv_GPIO_Pad_Set(offset);
    GPIO_PRINT("[camdriver-gpio]camdriver_gpio_request offset=%d\n",offset);
    return 0;
}

void camdriver_gpio_free(struct gpio_chip *chip, unsigned offset)
{
    GPIO_PRINT("[camdriver-gpio]camdriver_gpio_free\n");
}

void camdriver_gpio_set(struct gpio_chip *chip, unsigned offset, int value)
{
    if(value==0)
        MDrv_GPIO_Pull_Low(offset);
    else
        MDrv_GPIO_Pull_High(offset);
    GPIO_PRINT("[camdriver-gpio]camdriver_gpio_set\n");
}

int camdriver_gpio_get(struct gpio_chip *chip, unsigned offset)
{
    u8 PadLevel;
    GPIO_PRINT("[camdriver-gpio]camdriver_gpio_get\n");
    MDrv_GPIO_Pad_Read(offset, &PadLevel);
    return PadLevel;
}

int camdriver_gpio_direction_input(struct gpio_chip *chip, unsigned offset)
{
    GPIO_PRINT("[camdriver-gpio]camdriver_gpio_direction_input\n");
    MDrv_GPIO_Pad_Odn(offset);
    return 0;
}

int camdriver_gpio_direction_output(struct gpio_chip *chip, unsigned offset,
                    int value)
{
    MDrv_GPIO_Pad_Oen(offset);
    if(value==0)
        MDrv_GPIO_Pull_Low(offset);
    else
        MDrv_GPIO_Pull_High(offset);
    GPIO_PRINT("[camdriver-gpio]camdriver_gpio_direction_output\n");
    return 0;
}

int camdriver_gpio_to_irq(struct gpio_chip *chip, unsigned offset)
{
    int virq;

    virq = MDrv_GPIO_To_Irq(offset);
    if (virq < 0)
        return -6; // -ENXIO;

    GPIO_PRINT("%s virq:%d \n", __FUNCTION__, virq);
    return virq;
}

int camdriver_gpio_get_direction(struct gpio_chip *chip, unsigned offset)
{
    u8 status;
    MDrv_GPIO_Pad_InOut(offset, &status);
    return status;
}
