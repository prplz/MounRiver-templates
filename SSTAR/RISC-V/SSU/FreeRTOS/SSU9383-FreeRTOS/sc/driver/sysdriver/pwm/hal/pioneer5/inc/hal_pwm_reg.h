/*
 * hal_pwm_reg.h- Sigmastar
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

#define PWM_IN_NUM         (8)
#define PWM_INPUT_BANK     (0x1F201A00)
#define PWM_CLK_OFFSET     (0x38 << 2)
#define PWM_IN_CHAN_OFFSET (0x20 << 2)
#define PWM_IN_GLHRM_DIV   (0x0000)
#define PWM_IN_GLHRM_NUM   (0x2)
#define BASE_REG_CLKGEN_PA (0x2207000)
//------------------------------------------------------------------------------
// PWM INPUT
//------------------------------------------------------------------------------
#define u16REG_CAPTURE_EN (0x00 << 2)
#define CAPTURE_EN        BIT0
#define RESET_MODE_EN     BIT1
#define RESET_MODE_SEL    BIT2
#define PULSE_CNT_EN      BIT4
#define PULSE_CNT_SEL     BIT5
#define DETECT_MODE       BIT8
#define COUNT_LOAD_EN     BIT12
#define SW_RST            BIT15

#define u16REG_TIMER_DIV       (0x01 << 2)
#define u16REG_EDGE_SEL_CH0    (0x04 << 2)
#define POSEDGE_CH_BIT         BIT0
#define NEGEDGE_CH_BIT         BIT1
#define PULSE_DIV_CH0_OFFSET   2
#define TRI_MODE_EN_CH0_OFFSET 8

#define u16REG_GLHRM_EN_CH0     (0x05 << 2)
#define GLHRM_EN_CH0_BIT        BIT0
#define GLHRM_NUM_CH0_OFFSET    1
#define u16REG_GLHRM_DIVIDE_CH0 (0x06 << 2)

#define u16REG_START_CNT_CH0_L (0x07 << 2)
#define u16REG_START_CNT_CH0_H (0x08 << 2)
#define u16REG_PSHOT_CNT_CH0_L (0x09 << 2)
#define u16REG_PSHOT_CNT_CH0_H (0x0A << 2)
#define u16REG_NSHOT_CNT_CH0_L (0x0B << 2)
#define u16REG_NSHOT_CNT_CH0_H (0x0C << 2)

#define u16REG_EDGE_SEL_CH1 (0x0F << 2)
//#define POSEDGE_CH1_BIT BIT0
//#define NEGEDGE_CH1_BIT BIT1
#define PULSE_DIV_CH1_OFFSET 4
#define u16REG_TRI_MODE_CH1  (0x10 << 2)

#define u16REG_GLHRM_EN_CH1     (0x11 << 2)
#define GLHRM_EN_CH1_BIT        BIT0
#define GLHRM_NUM_CH1_OFFSET    1
#define u16REG_GLHRM_DIVIDE_CH1 (0x12 << 2)

#define u16REG_START_CNT_CH1_L (0x13 << 2)
#define u16REG_START_CNT_CH1_H (0x14 << 2)
#define u16REG_PSHOT_CNT_CH1_L (0x15 << 2)
#define u16REG_PSHOT_CNT_CH1_H (0x16 << 2)
#define u16REG_NSHOT_CNT_CH1_L (0x17 << 2)
#define u16REG_NSHOT_CNT_CH1_H (0x18 << 2)

#define u16REG_PULSE_CNT_SEL (0x19 << 2)
#define PULSE_POSEDGE_CH0    BIT0
#define PULSE_NEGEDGE_CH0    BIT1
#define PULSE_POSEDGE_CH1    BIT2
#define PULSE_NEGEDGE_CH1    BIT3

#define u16REG_PULSE_CNT (0x1A << 2)
#define CNT_POS_CH0_BIT  BIT0
#define CNT_NEG_CH0_BIT  BIT1
#define CNT_POS_CH1_BIT  BIT2
#define CNT_NEG_CH1_BIT  BIT3

#define PWM_2_OFFSET       (0x0100 << 1)
#define PWM_ADC_OFFSET     (0x0200 << 1)
#define PWM_3_OFFSET       (0x0300 << 1)
#define PWM_ADC_WDT_OFFSET (0x0400 << 1)
#define PWM_NUM            (22)
#define PWM_CLOCK          12000000
#define PWM_CLK_OFFSET     (0x38 << 2)

//------------------------------------------------------------------------------
// PWM_ADC
//------------------------------------------------------------------------------

#define u16REG_PWM_OUT      (0x0 << 2)
#define PWM_OUTP_EN_BIT     BIT0
#define PWM_OUTN_EN_BIT     BIT1
#define PWM_DDTP_EN_BIT     BIT2
#define PWM_DDTN_EN_BIT     BIT3
#define PWM_IDLE_P_BIT      BIT4
#define PWM_IDLE_N_BIT      BIT5
#define PWM_ACTIVE_P_BIT    BIT6
#define PWM_ACTIVE_N_BIT    BIT7
#define PWM_BRK_POL_BIT     BIT10
#define PWM_AOE_BIT         BIT11
#define PWM_MOE_BIT         BIT12
#define PWM_MOE_TRI_BIT     BIT13
#define u16REG_PWM_DDTP_LEN (0x1 << 2)
#define u16REG_PWM_DDTN_LEN (0x2 << 2)

#define u16REG_PWM_ADC_EN (0x30 << 2)
#define ADC_EN_BG_BIT     BIT2

#define u16REG_GPIO_EN_0 (0x34 << 2)
#define u16REG_GPIO_EN_1 (0x35 << 2)

#define u16REG_PWMP_TRI_DELAY_L (0x37 << 2)
#define u16REG_PWMP_TRI_DELAY_H (0x38 << 2)
#define u16REG_PWMN_TRI_DELAY_L (0x39 << 2)
#define u16REG_PWMN_TRI_DELAY_H (0x3A << 2)

#define u16REG_PWM_SAR_EN (0x50 << 2)
#define SAR_EN_BIT        BIT0
#define REGU_CON_MODE_BIT BIT1
#define INJ_CON_MODE_BIT  BIT2
#define REGU_STR_BIT      BIT4
#define INJ_STR_BIT       BIT5
#define REGU_END_BIT      BIT6
#define INJ_END_BIT       BIT7
#define DATA_ALIGN        BIT13

#define u16REG_ADC_SEQ_LEN  (0x51 << 2)
#define INJ_SEQ_LEN_OFFSET  0
#define REGU_SEQ_LEN_OFFSET 8

#define u16REG_INJ_ADC_SEQ_1 (0x52 << 2)
#define INJ_SEQ_LIST_OFFSET  5

#define u16REG_REGU_ADC_SEQ_1 (0x56 << 2)
#define REGU_SEQ_LIST_OFFSET  5

#define u16REG_ADC_TRI   (0x5E << 2)
#define REGU_TRI_OFFSET  0
#define INJ_TRI_OFFSET   4
#define INJ_SW_TRI_BIT   BIT8
#define REGU_SW_TRI_BIT  BIT9
#define REGU_TRI_MET_BIT BIT10
#define INJ_TRI_MET_BIT  BIT11

#define u16REG_ADC_AVE_CNT (0x5F << 2)
#define AVE_CNT_OFFSET     1

#define u16REG_LOAD_EN (0x60 << 2)
#define LOAD_EN_BIT    BIT0

#define u16REG_ADC_DATA (0x65 << 2)

#define u16REG_ONESHOT_PRD (0x4E << 2)

#define u16REG_ADC_FLAG   (0x7F << 2)
#define REGU_STATE_OFFSET 0
#define INJ_STATE_OFFSET  1
#define CHAN_STATE_OFFSET 2

#define u16REG_8SYNC_MODE   (0x7F << 2)
#define REG_8SYNC_MODE_SHFT (0x0)
//------------------------------------------------------------------------------
// ADC DMA
//------------------------------------------------------------------------------
#define u16REG_ADC_DMA_EN       (0x40 << 2)
#define DMA_EN_BIT              BIT0
#define ADC_SW_RST_BIT          BIT1
#define u16REG_MIU_ADDR_L       (0x41 << 2)
#define u16REG_MIU_ADDR_M       (0x42 << 2)
#define u16REG_MIU_ADDR_H       (0x43 << 2)
#define u16REG_MIU_SIZE_L       (0x44 << 2)
#define u16REG_MIU_SIZE_H       (0x45 << 2)
#define u16REG_DMA_POL          (0x46 << 2)
#define u16REG_DMA_DONE_INT_EN  (0x47 << 2)
#define ADC_DMA_INT_EN_OFFSET   0
#define ADC_DMA_INT_FLAG_OFFSET 8
//------------------------------------------------------------------------------
// ADC WDT
//------------------------------------------------------------------------------
#define u16REG_ADC_WDT_EN (0x30 << 2)

#if !defined(BIT0) && !defined(BIT1)
#define BIT0  0x0001
#define BIT1  0x0002
#define BIT2  0x0004
#define BIT3  0x0008
#define BIT4  0x0010
#define BIT5  0x0020
#define BIT6  0x0040
#define BIT7  0x0080
#define BIT8  0x0100
#define BIT9  0x0200
#define BIT10 0x0400
#define BIT11 0x0800
#define BIT12 0x1000
#define BIT13 0x2000
#define BIT14 0x4000
#define BIT15 0x8000
#endif

