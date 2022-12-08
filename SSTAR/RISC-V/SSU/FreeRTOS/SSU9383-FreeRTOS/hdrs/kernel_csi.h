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

#ifndef __KERNEL_CSI_H__
#define __KERNEL_CSI_H__

#include "cpu_mem_map.hc"

#define PAD16BITS  u32 :16

typedef struct {
	u32   null:1;
	u32   blinking:1;
	u32   embedded:1;
	u32   :5;
	u32   :6;
	u32   YUV4228B:1;
	u32   :1;
	PAD16BITS;
	u32   :8; //16:23
	u32   :2;
	u32   RAW8:1;
	u32   RAW10:1;
	u32   RAW12:1;
	u32   :3;
	PAD16BITS;
	PAD16BITS;
	PAD16BITS;
}__attribute__((packed, aligned(1))) csi_dt;

#ifndef __ABICC__

typedef struct CSImac_s {
	//0x0000
	u32 :1;
	u32 reg_ecc_off   :1;
	u32 reg_lane_en   :4;
	u32 reg_rm_err_sot:1;
	u32 reg_fun_md    :2;
	u32 reg_vc_en     :4;
	u32 reg_rm_err_fs :1;
	u32 reg_uy_vy_sel :1;
	u32 :1;
	PAD16BITS;
#define CSI_LAN0_EN 0x01
#define CSI_LAN1_EN 0x02
#define CSI_LAN2_EN 0x04
#define CSI_LAN3_EN 0x08
#define CSI_VC0_EN 0x01
#define CSI_VC1_EN 0x02
#define CSI_VC3_EN 0x04
#define CSI_VC4_EN 0x08

	//0x0004
	u32 reg_vc0_hs_mode:3;
	u32 reg_vc1_hs_mode:3;
	u32 reg_vc2_hs_mode:3;
	u32 reg_vc3_hs_mode:3;
	u32 reg_debug_sel  :4;
	PAD16BITS;

	//0x0008
	u32 reg_vc0_vs_mode:3;
	u32 reg_vc1_vs_mode:3;
	u32 reg_vc2_vs_mode:3;
	u32 reg_vc3_vs_mode:3;
	u32 reg_debug_en   :1;
	u32 :3;
	PAD16BITS;
#define CSI_V_SYNC_MODE_FS 0x00
#define CSI_V_SYNC_MODE_FE 0x01

	//0x000C
	u32 reg_mac_err_int_mask:15;
	u32 :1;
	PAD16BITS;

	//0x0010
	u32 reg_mac_err_int_force:15;
	u32 :1;
	PAD16BITS;

	//0x0014
	u32 reg_mac_err_int_clr:15;
	u32 :1;
	PAD16BITS;

	//0x0018
	u32 reg_mac_rpt_int_mask:7;
	u32 :9;
	PAD16BITS;
#define CSI_LINE_INT    0x01
#define CSI_FRAME_INT  0x02
#define CSI_VC3_FE_INT  0x08
#define CSI_VC2_FE_INT  0x10
#define CSI_VC1_FE_INT  0x20
#define CSI_VC0_INT    0x40  //FS or FE depend on reg_mac_rpt_int_src[1] selection

	u32 reg_mac_rpt_int_force:7;
	u32 :9;
	PAD16BITS;

	u32 reg_mac_rpt_int_clr:7;
	u32 :9;
	PAD16BITS;

	u32 reg_phy_int_mask:10;
	u32 :6;
	PAD16BITS;

	u32 reg_phy_int_force:10;
	u32 :6;
	PAD16BITS;

	u32 reg_phy_int_clr:10;
	u32 :6;
	PAD16BITS;

	u32 reg_mac_err_int_src:15;
	u32 :1;
	PAD16BITS;

	u32 reg_mac_err_int_raw_src:15;
	u32 :1;
	PAD16BITS;

	u32 reg_mac_rpt_int_src:7;
	u32 :9;
	PAD16BITS;

	u32 reg_mac_rpt_int_raw_src:7;
	u32 :9;
	PAD16BITS;

	u32 reg_phy_int_src:10;
	u32 :6;
	PAD16BITS;

	u32 reg_phy_int_raw_src:10;
	u32 :6;
	PAD16BITS;

	u32 reg_frm_num:16;
	PAD16BITS;

	u32 reg_line_num:16;
	PAD16BITS;

	u32 reg_g8spd_wc:16;
	PAD16BITS;

	u32 reg_g8spd_dt:8;
	u32 :8;
	PAD16BITS;

	u32 reg_mac_idle:1;
	u32 :15;
	PAD16BITS;

	u32 reg_1frame_trig:1;
	u32 :15;
	PAD16BITS;

	u32 reg_ckg_csi_mac:6;
	u32 :2;
	u32 reg_ckg_csi_mac_lptx:5;
	u32 :3;
	PAD16BITS;

	u32 reg_ckg_ns:5;
	u32 :11;
	PAD16BITS;

	u32 reg_csi_mac_reserved:16;
	PAD16BITS;

	u32 reg_csi_mac_reserved1:16;
	PAD16BITS;

	u32 reg_csi_mac_reserved2:16;
	PAD16BITS;

	csi_dt reg_dt_en;

	u32 reg_sw_rst      :1;
	u32 reg_mac_en      :1;
	u32 reg_dont_care_dt:1;
	u32 :13;
	PAD16BITS;

	u32 reg_raw_l_sot_sel:2;
	u32 reg_sot_sel      :1;
	u32 reg_eot_sel      :1;
	u32 reg_rpt_fnum_cond:1;
	u32 reg_rpt_lnum_cond:1;
	u32 :10;
	PAD16BITS;
#define CSI_RPT_FS 0x0    //report frame start
#define CSI_RTP_FE 0x01  //report frame end

	u32 reg_clk_lane_fsm_sts_int_mask:16;
	PAD16BITS;

	u32 reg_clk_lane_fsm_sts_int_force:16;
	PAD16BITS;

	u32 reg_clk_lane_fsm_sts_int_clr:16;
	PAD16BITS;

	u32 reg_clk_lane_fsm_sts_int_src:16;
	PAD16BITS;

	u32 reg_clk_lane_fsm_sts_int_raw_src:16;
	PAD16BITS;

	u32 reg_data_lane0_fsm_sts_int_mask:16;
	PAD16BITS;

	u32 reg_data_lane0_fsm_sts_int_force:16;
	PAD16BITS;

	u32 reg_data_lane0_fsm_sts_int_clr:16;
	PAD16BITS;

	u32 reg_data_lane0_fsm_sts_int_src:16;
	PAD16BITS;

	u32 reg_data_lane0_fsm_sts_int_raw_src:16;
	PAD16BITS;

	u32 reg_data_lane1_fsm_sts_int_mask:16;
	PAD16BITS;

	u32 reg_data_lane1_fsm_sts_int_force:16;
	PAD16BITS;

	u32 reg_data_lane1_fsm_sts_int_clr:16;
	PAD16BITS;

	u32 reg_data_lane1_fsm_sts_int_src:16;
	PAD16BITS;

	u32 reg_data_lane1_fsm_sts_int_raw_src:16;
	PAD16BITS;
} __attribute__((packed, aligned(1))) CSImac_t;

extern volatile CSImac_t * const g_ptCSI;
#endif // __ABICC__

#endif
