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


#ifndef __KERNEL_RTC_H__
#define __KERNEL_RTC_H__

/************************************************/
/* RTC Register Bank                            */
/************************************************/

typedef struct KeRtcTimer_s
{
    u16 reg_c_soft_rstz      :1;// 0x00
    u16 reg_c_cnt_en         :1;
    u16 reg_c_wrap_en        :1;
    u16 reg_c_load_en        :1;
    u16 reg_c_read_en        :1;
    u16 reg_c_int_mask       :1;
    u16 reg_c_int_force      :1;
    u16 reg_c_int_clear      :1;
    u16 reserved00           :8;
    u16                      :16;
    u16 reg_c_freq_cw_low    :16;//0x4
    u16                      :16;
    u16 reg_c_freq_cw_high   :16;//0x8
    u16                      :16;
    u16 reg_c_load_val_low   :16;//0xc
    u16                      :16;
    u16 reg_c_load_val_high  :16;//0x10
    u16                      :16;
    u16 reg_c_match_val_low  :16;//0x14
    u16                      :16;
    u16 reg_c_match_val_high :16;//0x18
    u16                      :16;
    u16 reg_rtc_raw_int      :1;//0x1c
    u16 reg_rtc_int          :1;
    u16 reserved01           :14;
    u16                      :16;
    u16 reg_rtc_cnt_low      :16;//0x20
    u16                      :16;
    u16 reg_rtc_cnt_high     :16;//0x24
    u16                      :16;
}KeRtcTimer_t;

extern volatile KeRtcTimer_t* const g_ptRtcReg;

typedef struct AsuraRtcReg_s
{
    u16 ctrl;               // 0x00
    u16 reserved00;
    u16 load_val_l;         // 0x01
    u16 reserved01;
    u16 load_val_h;         // 0x02
    u16 reserved02;
    u16 sec_cnt_l;          // 0x03
    u16 reserved03;
    u16 sec_cnt_h;          // 0x04
    u16 reserved04;
    u16 sub_cnt;            // 0x05
    u16 reserved05;
    u16 mskcmp_subcnt;      // 0x06
    u16 reserved06;
    u16 freq_cw;            // 0x07
    u16 reserved07;
    u16 match_value_l;      // 0x08
    u16 reserved08;
    u16 match_value_h;      // 0x09
    u16 reserved09;
    u16 misc_ctrl_sts;      // 0x0A
    u16 reserved0A;
    u16 raw_sec_cnt_l;      // 0x0B
    u16 reserved0B;
    u16 raw_sec_cnt_h;     	// 0x0C
    u16 reserved0C;
    u16 raw_sub_cnt;        // 0x0D
    u16 reserved0D;
    u16 rtc_snapshot;     	// 0x0E
    u16 reserved0E;
    u16 rtc_fsm_state;      // 0x0F
    u16 reserved0F;
    u16 initial_flag_l;     // 0x10
    u16 reserved10;
    u16 initial_flag_h;     // 0x11
    u16 reserved11;
    u16 offset_flag_l;      // 0x12
    u16 reserved12;
    u16 offset_flag_h;      // 0x13
    u16 reserved13;
    u16 alarm_flag_l;       // 0x14
    u16 reserved14;
    u16 alarm_flag_h;       // 0x15
    u16 reserved15;
    u16 event1_flag_l;      // 0x16
    u16 reserved16;
    u16 event1_flag_h;      // 0x17
    u16 reserved17;
    u16 event2_flag_l;      // 0x18
    u16 reserved18;
    u16 event2_flag_h;      // 0x19
    u16 reserved19;
    u16 event3_flag_l;      // 0x1A
    u16 reserved1A;
    u16 event3_flag_h;      // 0x1B
    u16 reserved1B;
    u16 current_flag_l;     // 0x1C
    u16 reserved1C;
    u16 current_flag_h;     // 0x1D
    u16 reserved1D;
    u16 flag_update;        // 0x1E
    u16 reserved1E;
    u16 int_en;             // 0x1F
    u16 reserved1F;
    u16 reg_life_0;         // 0x20
    u16 reserved20;
    u16 reg_life_1;         // 0x21
    u16 reserved21;
    u16 reg_life_2;         // 0x22
    u16 reserved22;
    u16 reg_life_3;         // 0x23
    u16 reserved23;
    u16 pad_passwd;         // 0x24
    u16 reserved24;
    u16 spare0;             // 0x25
    u16 reserved25;
    u16 spare1;             // 0x26
    u16 reserved26;
    u16 ws_expire;          // 0x27
    u16 reserved27;
    u16 turn_on_ctrl;       // 0x28
    u16 reserved28;
    u16 analog_test;        // 0x29
    u16 reserved29;
    u32 reserved2A;         // 0x2A
    u16 rtc_irq_mask;       // 0x2B
    u16 reserved2B;
    u16 rtc_irq_force;      // 0x2C
    u16 reserved2C;
    u16 rtc_irq_clr;        // 0x2D
    u16 reserved2D;
    u16 rtc_irq_status;     // 0x2E
    u16 reserved2E;
    u16 rtc_passwd;         // 0x2F
    u16 reserved2F;

    /* [0x30] */
    u32 reg_no_por_passwd             :16; /// no power-on reset password
                                           /// when reg_on_por_passwd = 16'hA5A5
                                           /// rtc_irq, rtc_cnt, rtc_cal will remove power-on reset
    u32 :16; /// [0x30]
    /* [0x31] */
    u32 reg_vbatlvl1_rtc              :1 ; /// VBAT LEVEL1 FLAG
    u32 :31; /// [0x31]
    /* [0x32] */
    u32 :1 ; /// [0x32]
    u32 reg_xtal32k_sel_rtc           :1 ; /// XTAL 32K SEL
                                           /// 0: use FRO 32K
                                           /// 1: CHIP use external 32K XTAL
    u32 reg_xtal32k_sel               :1 ; /// ext 32k selection foto clk_gen ; mux result by reg_xtal32k_sel_ow and reg_xtal32k_ow
    u32 reg_xtal32k_sel_ow            :1 ; /// ext 32k selection over write enable
                                           /// 0: no over write
                                           /// 1: over write by reg_xtal32k_ow
    u32 reg_xtal32k_ow                :1 ; /// ext 32k selection  over write vaule
                                           /// 0: use 32k from 26Mhz
                                           /// 1: use XTAL 32K    (default 32K need come from RTC domain)
    u32 reg_ext_1hz_sel               :1 ; /// rtc use ext 1hz sync
                                           ///
    u32 reg_ext_1hz_index             :1 ; /// pmu to rtc  ext 1hz  index flag
                                           ///
    u32 reg_ext_1hz_ready             :1 ; /// pmu to rtc  ext 1hz  ready flag
                                           ///
    u32 :24; /// [0x32]
    /* [0x33] */
    u32 reg_32k_freq_trim             :3 ; /// fro 32k freq trim bit
    u32 :29; /// [0x33]
    u32 :32; /// [0x34]
    u32 :32; /// [0x35]
    u32 :32; /// [0x36]
    u32 :32; /// [0x37]
    u32 :32; /// [0x38]
    u32 :32; /// [0x39]
    u32 :32; /// [0x3A]
    u32 :32; /// [0x3B]
    u32 :32; /// [0x3C]
    u32 :32; /// [0x3D]
    u32 :32; /// [0x3E]
    u32 :32; /// [0x3F]
    /* [0x40] */
    u32 reg_1hz_sub_cnt               :16; /// FRO 32k sub cnt  when external 1hz trigger;
    u32 :16; /// [0x40]
    /* [0x41] */
    u32 reg_1hz_cmp_cnt               :16; /// 32k sub cnt  compare value
                                           /// mux result of reg_1hz_sub_cnt  (when xtal32k_sel =0)
                                           /// and reg_freq_cw[15:0]  (when xtal32k_sel =1)
    u32 :16; /// [0x41]
    u32 :32; /// [0x42]
    u32 :32; /// [0x43]
    u32 :32; /// [0x44]
    u32 :32; /// [0x45]
    u32 :32; /// [0x46]
    u32 :32; /// [0x47]
    u32 :32; /// [0x48]
    u32 :32; /// [0x49]
    u32 :32; /// [0x4A]
    u32 :32; /// [0x4B]
    u32 :32; /// [0x4C]
    u32 :32; /// [0x4D]
    u32 :32; /// [0x4E]
    u32 :32; /// [0x4F]
    u32 reserved5X[16];
    u32 reserved6X[16];
    u32 reserved7X[15];
    u16 riu_w_passwd;       // 0x7F
    u16 reserved7F;
} AsuraRtcReg_t;

/*=============================================================*/
// Variable definition
/*=============================================================*/
extern volatile AsuraRtcReg_t* const g_ptAsuraRtcReg;

//ctrl (0x01)
#define reg_load_en(d)                  (d << 1)
#define reg_cnt_en(d)                   (d << 4)
#define reg_vdd_rtc_status              (1 << 6)

//misc_ctrl_sts (0xA)
#define reg_onoff_status                (1 << 6)

// (0x0E)
#define reg_snapshot_en                 (1 << 0)

//flag_update (0x1E)
#define reg_initial_flag_update(d)      (d << 0)
#define reg_offset_flag_update(d)       (d << 1)
#define reg_alarm_flag_update(d)        (d << 2)
#define reg_event1_flag_update(d)       (d << 3)
#define reg_event2_flag_update(d)       (d << 4)
#define reg_event3_flag_update(d)       (d << 5)

//int_en (0x1F)
#define reg_alarm_int_en(d)             (d << 0)
#define reg_event1_int_en(d)            (d << 1)
#define reg_event2_int_en(d)            (d << 2)
#define reg_event3_int_en(d)            (d << 3)

//turn_on_ctrl(0x28)
#define reg_onoff_sel                   (1 << 4)

//rtc_passwd (0x2F)
#define ASURA_RTC_PASSCODE 0x0467 ///< set this password to 0x8172 to allow alarm power on.

//riu_w_passwd(0x7F)
#define RIU_ENABLE  0xCAFE
#define RIU_DISABLE 0xDDDD

//int define
#define RTC_IRQ_MASK_ALL                    0x7f
#define reg_rtc_32K_failed(d)           (d << 0)
#define reg_sub_cnt_match_int(d)        (d << 1)
#define reg_rtc_match_int(d)            (d << 2)
#define reg_alarm_int(d)                (d << 3)
#define reg_event1_int(d)               (d << 4)
#define reg_event2_int(d)               (d << 5)
#define reg_event3_int(d)               (d << 6)
#define reg_reserved_int(d)             (d << 7)
#endif //__KERNEL_RTC_H__

