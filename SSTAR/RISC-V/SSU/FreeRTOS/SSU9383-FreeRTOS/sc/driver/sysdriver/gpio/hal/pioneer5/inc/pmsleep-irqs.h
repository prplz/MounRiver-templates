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

/*  Not used in dtsi,
if need to get the interrupt number for request_irq(), use gpio_to_irq() to obtain irq number.
Or manual calculate the number is
GIC_SGI_NR+GIC_PPI_NR+GIC_SPI_ARM_INTERNAL_NR+GIC_SPI_MS_IRQ_NR+GIC_SPI_MS_FIQ_NR+X=160+X   */
/* MS_PM_SLEEP_FIQ 0-31 */
#define PMSLEEP_FIQ_START       0
#define INT_PMSLEEP_PM_PWM0     (PMSLEEP_FIQ_START + 0)
#define INT_PMSLEEP_PM_PWM1     (PMSLEEP_FIQ_START + 1)
#define INT_PMSLEEP_PM_I2CM_SCL (PMSLEEP_FIQ_START + 2)
#define INT_PMSLEEP_PM_I2CM_SDA (PMSLEEP_FIQ_START + 3)
#define INT_PMSLEEP_PM_UART_RX0 (PMSLEEP_FIQ_START + 4)
#define INT_PMSLEEP_PM_UART_TX0 (PMSLEEP_FIQ_START + 5)
#define INT_PMSLEEP_PM_IR_RX    (PMSLEEP_FIQ_START + 6)
#define INT_PMSLEEP_PM_GPIO0    (PMSLEEP_FIQ_START + 7)
#define INT_PMSLEEP_PM_GPIO1    (PMSLEEP_FIQ_START + 8)
#define INT_PMSLEEP_PM_GPIO2    (PMSLEEP_FIQ_START + 9)
#define INT_PMSLEEP_PM_GPIO3    (PMSLEEP_FIQ_START + 10)
#define INT_PMSLEEP_PM_GPIO4    (PMSLEEP_FIQ_START + 11)
#define INT_PMSLEEP_PM_GPIO5    (PMSLEEP_FIQ_START + 12)
#define INT_PMSLEEP_PM_SPI_WPZ  (PMSLEEP_FIQ_START + 13)
#define INT_PMSLEEP_PM_SPI_DO   (PMSLEEP_FIQ_START + 14)
#define INT_PMSLEEP_PM_SPI_CZ   (PMSLEEP_FIQ_START + 15)
#define INT_PMSLEEP_PM_SPI_HLD  (PMSLEEP_FIQ_START + 16)
#define INT_PMSLEEP_PM_SPI_CK   (PMSLEEP_FIQ_START + 17)
#define INT_PMSLEEP_PM_SPI_DI   (PMSLEEP_FIQ_START + 18)
#define INT_PMSLEEP_DUMMY_19    (PMSLEEP_FIQ_START + 19)
#define INT_PMSLEEP_DUMMY_20    (PMSLEEP_FIQ_START + 20)
#define INT_PMSLEEP_DUMMY_21    (PMSLEEP_FIQ_START + 21)
#define INT_PMSLEEP_DUMMY_22    (PMSLEEP_FIQ_START + 22)
#define INT_PMSLEEP_DUMMY_23    (PMSLEEP_FIQ_START + 23)
#define INT_PMSLEEP_DUMMY_24    (PMSLEEP_FIQ_START + 24)
#define INT_PMSLEEP_DUMMY_25    (PMSLEEP_FIQ_START + 25)
#define INT_PMSLEEP_DUMMY_26    (PMSLEEP_FIQ_START + 26)
#define INT_PMSLEEP_DUMMY_27    (PMSLEEP_FIQ_START + 27)
#define INT_PMSLEEP_DUMMY_28    (PMSLEEP_FIQ_START + 28)
#define INT_PMSLEEP_DUMMY_29    (PMSLEEP_FIQ_START + 29)
#define INT_PMSLEEP_DUMMY_30    (PMSLEEP_FIQ_START + 30)
#define INT_PMSLEEP_DUMMY_31    (PMSLEEP_FIQ_START + 31)
#define INT_PMSLEEP_DUMMY_32    (PMSLEEP_FIQ_START + 32)
#define INT_PMSLEEP_DUMMY_33    (PMSLEEP_FIQ_START + 33)
#define INT_PMSLEEP_DUMMY_34    (PMSLEEP_FIQ_START + 34)
#define INT_PMSLEEP_DUMMY_35    (PMSLEEP_FIQ_START + 35)
#define INT_PMSLEEP_DUMMY_36    (PMSLEEP_FIQ_START + 36)
#define INT_PMSLEEP_DUMMY_37    (PMSLEEP_FIQ_START + 37)
#define INT_PMSLEEP_DUMMY_38    (PMSLEEP_FIQ_START + 38)
#define INT_PMSLEEP_DUMMY_39    (PMSLEEP_FIQ_START + 39)
#define INT_PMSLEEP_DUMMY_40    (PMSLEEP_FIQ_START + 40)
#define INT_PMSLEEP_DUMMY_41    (PMSLEEP_FIQ_START + 41)
#define INT_PMSLEEP_DUMMY_42    (PMSLEEP_FIQ_START + 42)
#define INT_PMSLEEP_DUMMY_43    (PMSLEEP_FIQ_START + 43)
#define INT_PMSLEEP_DUMMY_44    (PMSLEEP_FIQ_START + 44)
#define INT_PMSLEEP_DUMMY_45    (PMSLEEP_FIQ_START + 45)
#define INT_PMSLEEP_DUMMY_46    (PMSLEEP_FIQ_START + 46)
#define INT_PMSLEEP_DUMMY_47    (PMSLEEP_FIQ_START + 47)
#define INT_PMSLEEP_DUMMY_48    (PMSLEEP_FIQ_START + 48)
#define INT_PMSLEEP_DUMMY_49    (PMSLEEP_FIQ_START + 49)
#define INT_PMSLEEP_DUMMY_50    (PMSLEEP_FIQ_START + 50)
#define INT_PMSLEEP_DUMMY_51    (PMSLEEP_FIQ_START + 51)
#define INT_PMSLEEP_DUMMY_52    (PMSLEEP_FIQ_START + 52)
#define INT_PMSLEEP_DUMMY_53    (PMSLEEP_FIQ_START + 53)
#define INT_PMSLEEP_DUMMY_54    (PMSLEEP_FIQ_START + 54)
#define INT_PMSLEEP_DUMMY_55    (PMSLEEP_FIQ_START + 55)
#define INT_PMSLEEP_DUMMY_56    (PMSLEEP_FIQ_START + 56)
#define INT_PMSLEEP_DUMMY_57    (PMSLEEP_FIQ_START + 57)
#define INT_PMSLEEP_DUMMY_58    (PMSLEEP_FIQ_START + 58)
#define INT_PMSLEEP_DUMMY_59    (PMSLEEP_FIQ_START + 59)
#define INT_PMSLEEP_DUMMY_60    (PMSLEEP_FIQ_START + 60)
#define INT_PMSLEEP_DUMMY_61    (PMSLEEP_FIQ_START + 61)
#define INT_PMSLEEP_DUMMY_62    (PMSLEEP_FIQ_START + 62)
#define INT_PMSLEEP_DUMMY_63    (PMSLEEP_FIQ_START + 63)
#define INT_PMSLEEP_DUMMY_64    (PMSLEEP_FIQ_START + 64)
#define INT_PMSLEEP_DUMMY_65    (PMSLEEP_FIQ_START + 65)
#define INT_PMSLEEP_DUMMY_66    (PMSLEEP_FIQ_START + 66)
#define INT_PMSLEEP_DUMMY_67    (PMSLEEP_FIQ_START + 67)
#define INT_PMSLEEP_DUMMY_68    (PMSLEEP_FIQ_START + 68)
#define INT_PMSLEEP_DUMMY_69    (PMSLEEP_FIQ_START + 69)
#define INT_PMSLEEP_DUMMY_70    (PMSLEEP_FIQ_START + 70)
#define INT_PMSLEEP_DUMMY_71    (PMSLEEP_FIQ_START + 71)
#define INT_PMSLEEP_DUMMY_72    (PMSLEEP_FIQ_START + 72)
#define INT_PMSLEEP_DUMMY_73    (PMSLEEP_FIQ_START + 73)
#define INT_PMSLEEP_DUMMY_74    (PMSLEEP_FIQ_START + 74)
#define INT_PMSLEEP_DUMMY_75    (PMSLEEP_FIQ_START + 75)
#define PMSLEEP_FIQ_END         (PMSLEEP_FIQ_START + 76)
#define PMSLEEP_FIQ_NR          (PMSLEEP_FIQ_END - PMSLEEP_FIQ_START)

#define INT_PMSLEEP_INVALID 0xFF

#define PMSLEEP_IRQ_START        PMSLEEP_FIQ_END
#define INT_PMSLEEP_IRQ_DUMMY_00 (PMSLEEP_IRQ_START + 0)
#define INT_PMSLEEP_IRQ_SAR      (PMSLEEP_IRQ_START + 1)
#define INT_PMSLEEP_IRQ_WOL      (PMSLEEP_IRQ_START + 2)
#define INT_PMSLEEP_IRQ_DUMMY_03 (PMSLEEP_IRQ_START + 3)
#define INT_PMSLEEP_IRQ_RTC      (PMSLEEP_IRQ_START + 4)
#define INT_PMSLEEP_IRQ_DUMMY_05 (PMSLEEP_IRQ_START + 5)
#define INT_PMSLEEP_IRQ_DUMMY_06 (PMSLEEP_IRQ_START + 6)
#define INT_PMSLEEP_IRQ_DUMMY_07 (PMSLEEP_IRQ_START + 7)
#define PMSLEEP_IRQ_END          (PMSLEEP_IRQ_START + 8)
#define PMSLEEP_IRQ_NR           (PMSLEEP_IRQ_END - PMSLEEP_IRQ_START)
