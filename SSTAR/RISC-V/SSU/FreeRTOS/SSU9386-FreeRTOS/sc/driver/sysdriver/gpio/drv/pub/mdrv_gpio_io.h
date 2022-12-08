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

struct gpio_chip {};

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void camdriver_gpio_set(struct gpio_chip *chip, unsigned offset, int value);
int camdriver_gpio_get(struct gpio_chip *chip, unsigned offset);
int camdriver_gpio_direction_input(struct gpio_chip *chip, unsigned offset);
int camdriver_gpio_request(struct gpio_chip *chip, unsigned offset);
void camdriver_gpio_free(struct gpio_chip *chip, unsigned offset);
int camdriver_gpio_direction_output(struct gpio_chip *chip, unsigned offset,
                    int value);
int camdriver_gpio_to_irq(struct gpio_chip *chip, unsigned offset);
