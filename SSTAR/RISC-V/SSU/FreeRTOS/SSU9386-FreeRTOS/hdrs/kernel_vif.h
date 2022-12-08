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

#ifndef __KERNEL_VIF_H__
#define __KERNEL_VIF_H__

#include "cpu_mem_map.hc"

typedef struct VIF_s{
	// h0000, bit: 0
	/* Software Reset for Sensor0
	# Low Active
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch0_sensor_sw_rstz (0)
	#define mask_of_reg_vif_ch0_sensor_sw_rstz (0x1)
	unsigned int reg_vif_ch0_sensor_sw_rstz:1;

	// h0000, bit: 1
	/* VIF IF status reset*/
	#define offset_of_reg_vif_ch0_if_state_rst (0)
	#define mask_of_reg_vif_ch0_if_state_rst (0x2)
	unsigned int reg_vif_ch0_if_state_rst:1;

	// h0000, bit: 2
	/* Reset Sensor0*/
	#define offset_of_reg_vif_ch0_sensor_rst (0)
	#define mask_of_reg_vif_ch0_sensor_rst (0x4)
	unsigned int reg_vif_ch0_sensor_rst:1;

	// h0000, bit: 3
	/* Power down sensor0*/
	#define offset_of_reg_vif_ch0_sensor_pwrdn (0)
	#define mask_of_reg_vif_ch0_sensor_pwrdn (0x8)
	unsigned int reg_vif_ch0_sensor_pwrdn:1;

	// h0000, bit: 4
	/* HDR function enable*/
	#define offset_of_reg_hdr0_en (0)
	#define mask_of_reg_hdr0_en (0x10)
	unsigned int reg_hdr0_en:1;

	// h0000, bit: 7
	/* HDR source select
	# 3'd0 MIPI0 source
	# 3'd1 MIPI1 source
	# 3'd2 MIPI2 source
	# 3'd3 MIPI3 source
	# 3'd4 HISPI0 source
	# 3'd5 HISPI1 source
	# 3'd6 HISPI2 source
	# 3'd7 HISPI3 source*/
	#define offset_of_reg_hdr0_sel (0)
	#define mask_of_reg_hdr0_sel (0xe0)
	unsigned int reg_hdr0_sel:3;

	// h0000, bit: 12
	/* VIF source select
	# 5'd0 MIPI0
	# 5'd1 MIPI1
	# 5'd2 MIPI2
	# 5'd3 MIPI3
	# 5'd4 HISPI0
	# 5'd5 HISPI1
	# 5'd6 HISPI2
	# 5'd7 HISPI3
	# 5'd8 MIPI0 VC0
	# 5'd9 MIPI0 VC1
	# 5'd10 MIPI0 VC2
	# 5'd11 MIPI0 VC3
	# 5'd12 MIPI1 VC0
	# 5'd13 MIPI1 VC1
	# 5'd14 MIPI1 VC2
	# 5'd15 MIPI1 VC3
	# 5'd16 Parallel Sensor 0
	# 5'd17 Parallel Sensor 1
	# 5'd18 BT656 0
	# 5'd19 BT656 1
	# 5'd20 BT656 2
	# 5'd21 BT601*/
	#define offset_of_reg_vif_ch0_src_sel (0)
	#define mask_of_reg_vif_ch0_src_sel (0x1f00)
	unsigned int reg_vif_ch0_src_sel:5;

	// h0000, bit: 14
	/* */
	unsigned int :3;

	// h0000
	unsigned int /* padding 16 bit */:16;

	// h0001, bit: 0
	/* Mask sensor/csi ready to ISP_IF*/
	#define offset_of_reg_vif_ch0_sensor_mask (2)
	#define mask_of_reg_vif_ch0_sensor_mask (0x1)
	unsigned int reg_vif_ch0_sensor_mask:1;

	// h0001, bit: 1
	/* Ready only mode*/
	#define offset_of_reg_vif_ch0_if_de_mode (2)
	#define mask_of_reg_vif_ch0_if_de_mode (0x2)
	unsigned int reg_vif_ch0_if_de_mode:1;

	// h0001, bit: 14
	/* */
	unsigned int :14;

	// h0001
	unsigned int /* padding 16 bit */:16;

	// h0002, bit: 0
	/* Enable Software Flash Strobe
	# 1'b0: Disable
	# 1'b1: Enable
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch0_en_sw_strobe (4)
	#define mask_of_reg_vif_ch0_en_sw_strobe (0x1)
	unsigned int reg_vif_ch0_en_sw_strobe:1;

	// h0002, bit: 1
	/* Software Strobe Setting
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch0_sw_strobe (4)
	#define mask_of_reg_vif_ch0_sw_strobe (0x2)
	unsigned int reg_vif_ch0_sw_strobe:1;

	// h0002, bit: 3
	/* */
	unsigned int :2;

	// h0002, bit: 4
	/* Strobe Polarity
	# 1'b0: high active
	# 1'b1: low active*/
	#define offset_of_reg_vif_ch0_strobe_polarity (4)
	#define mask_of_reg_vif_ch0_strobe_polarity (0x10)
	unsigned int reg_vif_ch0_strobe_polarity:1;

	// h0002, bit: 14
	/* */
	unsigned int :11;

	// h0002
	unsigned int /* padding 16 bit */:16;

	// h0003, bit: 14
	/* Start Strobe after reference strobe start (unit:256xPCLK)
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch0_strobe_start (6)
	#define mask_of_reg_vif_ch0_strobe_start (0x7fff)
	unsigned int reg_vif_ch0_strobe_start:15;

	// h0003, bit: 15
	/* Reference Start for strobe signal
	# 1'b0 : vertical active start
	# 1'b1 : vertical blanking start
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch0_strobe_ref (6)
	#define mask_of_reg_vif_ch0_strobe_ref (0x8000)
	unsigned int reg_vif_ch0_strobe_ref:1;

	// h0003
	unsigned int /* padding 16 bit */:16;

	// h0004, bit: 14
	/* End Strobe after reference strobe start (unit:256xPCLK)
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch0_strobe_end (8)
	#define mask_of_reg_vif_ch0_strobe_end (0x7fff)
	unsigned int reg_vif_ch0_strobe_end:15;

	// h0004, bit: 15
	/* Enable Output Strobe Signal By Hardware Control
	# 1'b0: Disable
	# 1'b1: Enable
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch0_en_hw_strobe (8)
	#define mask_of_reg_vif_ch0_en_hw_strobe (0x8000)
	unsigned int reg_vif_ch0_en_hw_strobe:1;

	// h0004
	unsigned int /* padding 16 bit */:16;

	// h0005, bit: 3
	/* Hardware Flash Strobe Counter to count sensor frame number
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch0_hw_strobe_cnt (10)
	#define mask_of_reg_vif_ch0_hw_strobe_cnt (0xf)
	unsigned int reg_vif_ch0_hw_strobe_cnt:4;

	// h0005, bit: 13
	/* */
	unsigned int :10;

	// h0005, bit: 14
	/* Reference END for Hardware Long Strobe
	# 1'b0: vertical active start
	# 1'b1: vertical blanking start
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch0_hw_long_strobe_end_ref (10)
	#define mask_of_reg_vif_ch0_hw_long_strobe_end_ref (0x4000)
	unsigned int reg_vif_ch0_hw_long_strobe_end_ref:1;

	// h0005, bit: 15
	/* Hardware Flash Strobe Mode
	# 1'b0: short strobe
	# 1'b1: long strobe (enable several frames)
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch0_hw_strobe_mode (10)
	#define mask_of_reg_vif_ch0_hw_strobe_mode (0x8000)
	unsigned int reg_vif_ch0_hw_strobe_mode:1;

	// h0005
	unsigned int /* padding 16 bit */:16;

	// h0006, bit: 1
	/* */
	unsigned int :2;

	// h0006, bit: 3
	/* Left shift sensor input data
	# 2'b00 : no shift
	# 2'b01 : shift 2bit
	# 2'b10 : shift 4bit
	# 2'b11 : shift 6bit*/
	#define offset_of_reg_vif_ch0_sensor_format_left_sht (12)
	#define mask_of_reg_vif_ch0_sensor_format_left_sht (0xc)
	unsigned int reg_vif_ch0_sensor_format_left_sht:2;

	// h0006, bit: 6
	/* */
	unsigned int :3;

	// h0006, bit: 7
	/* swap sensor input data*/
	#define offset_of_reg_vif_ch0_sensor_bit_swap (12)
	#define mask_of_reg_vif_ch0_sensor_bit_swap (0x80)
	unsigned int reg_vif_ch0_sensor_bit_swap:1;

	// h0006, bit: 8
	/* Sensor Hsync Polarity
	# 1'b0: high active
	# 1'b1: low active
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch0_sensor_hsync_polarity (12)
	#define mask_of_reg_vif_ch0_sensor_hsync_polarity (0x100)
	unsigned int reg_vif_ch0_sensor_hsync_polarity:1;

	// h0006, bit: 9
	/* Sensor Vsync Polarity
	# 1'b0: high active
	# 1'b1: low active
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch0_sensor_vsync_polarity (12)
	#define mask_of_reg_vif_ch0_sensor_vsync_polarity (0x200)
	unsigned int reg_vif_ch0_sensor_vsync_polarity:1;

	// h0006, bit: 11
	/* Sensor Format
	# 2'b00: 8 bit
	# 2'b01: 10bit
	# 2'b10: 16bit
	# 2'b11: 12bit*/
	#define offset_of_reg_vif_ch0_sensor_format (12)
	#define mask_of_reg_vif_ch0_sensor_format (0xc00)
	unsigned int reg_vif_ch0_sensor_format:2;

	// h0006, bit: 12
	/* Sensor Input Format
	# 1'b0: YUV 422 format
	# 1'b1: RGB pattern
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch0_sensor_rgb_in (12)
	#define mask_of_reg_vif_ch0_sensor_rgb_in (0x1000)
	unsigned int reg_vif_ch0_sensor_rgb_in:1;

	// h0006, bit: 13
	/* */
	unsigned int :1;

	// h0006, bit: 14
	/* Sensor 8 bit to 12 bit mode
	# 2'b00: {[7:0], [7:4]}
	# 2'b01: {[7:0], 4'b0}
	# 2'b10: {[11:4], [11:8]}
	# 2'b11: {[11:4], 4'b0}
	Sensor 10 bit to 12 bit mode
	# 2'b00: {[9:0], [9:8]}
	# 2'b01: {[9:0], 2'b0}
	# 2'b10: {[11:2], [11:10]}
	# 2'b11: {[11:2], 2'b0}*/
	#define offset_of_reg_vif_ch0_sensor_format_ext_mode (12)
	#define mask_of_reg_vif_ch0_sensor_format_ext_mode (0xc000)
	unsigned int reg_vif_ch0_sensor_format_ext_mode:2;

	// h0006
	unsigned int /* padding 16 bit */:16;

	// h0007, bit: 5
	/* */
	unsigned int :6;

	// h0007, bit: 6
	/* Sensor Input Format
	# 1'b0: separate Y/C mode
	# 1'b1: YC 16bit mode*/
	#define offset_of_reg_vif_ch0_sensor_yc16bit (14)
	#define mask_of_reg_vif_ch0_sensor_yc16bit (0x40)
	unsigned int reg_vif_ch0_sensor_yc16bit:1;

	// h0007, bit: 7
	/* */
	unsigned int :1;

	// h0007, bit: 9
	/* Sensor vsync pulse delay
	# 2'd0: vsync falling edge
	# 2'd1: vsync rising edge delay 2 line
	# 2'd2: vsync rising edge delay 1 line
	# 2'd3: vsync rising edge*/
	#define offset_of_reg_vif_ch0_sensor_vs_dly (14)
	#define mask_of_reg_vif_ch0_sensor_vs_dly (0x300)
	unsigned int reg_vif_ch0_sensor_vs_dly:2;

	// h0007, bit: 11
	/* */
	unsigned int :2;

	// h0007, bit: 12
	/* Sensor hsync pulse delay
	# 1'b0: hsync rising edge
	# 1'b1: hsync falling edge*/
	#define offset_of_reg_vif_ch0_sensor_hs_dly (14)
	#define mask_of_reg_vif_ch0_sensor_hs_dly (0x1000)
	unsigned int reg_vif_ch0_sensor_hs_dly:1;

	// h0007, bit: 14
	/* */
	unsigned int :3;

	// h0007
	unsigned int /* padding 16 bit */:16;

	// h0008, bit: 13
	/* Parallel sensor input image width
	Only avalible in parallel sensor & BT601 mode*/
	#define offset_of_reg_vif_ch0_par_width (16)
	#define mask_of_reg_vif_ch0_par_width (0x3fff)
	unsigned int reg_vif_ch0_par_width:14;

	// h0008, bit: 14
	/* */
	unsigned int :2;

	// h0008
	unsigned int /* padding 16 bit */:16;

	// h0009, bit: 0
	/* BT601 Byte-interleaved mode*/
	#define offset_of_reg_bt601_byteint_en (18)
	#define mask_of_reg_bt601_byteint_en (0x1)
	unsigned int reg_bt601_byteint_en:1;

	// h0009, bit: 1
	/* BT601 Byte-interleaved channel select*/
	#define offset_of_reg_vif_ch0_bt601_ch_sel (18)
	#define mask_of_reg_vif_ch0_bt601_ch_sel (0x2)
	unsigned int reg_vif_ch0_bt601_ch_sel:1;

	// h0009, bit: 2
	/* BT656 YC16bit mode
	# 1'b0: YC
	# 1'b1: CY*/
	#define offset_of_reg_vif_ch0_bt656_yc16bit_mode (18)
	#define mask_of_reg_vif_ch0_bt656_yc16bit_mode (0x4)
	unsigned int reg_vif_ch0_bt656_yc16bit_mode:1;

	// h0009, bit: 14
	/* */
	unsigned int :13;

	// h0009
	unsigned int /* padding 16 bit */:16;

	// h000a, bit: 3
	/* VIF debug bus select*/
	#define offset_of_reg_vif_debug_sel (20)
	#define mask_of_reg_vif_debug_sel (0xf)
	unsigned int reg_vif_debug_sel:4;

	// h000a, bit: 7
	/* */
	unsigned int :4;

	// h000a, bit: 8
	/* Capture Enable (only for FPGA Debug Mode)
	# 1'b0: disable
	# 1'b1: enable*/
	#define offset_of_reg_vif_ch0_capture_en (20)
	#define mask_of_reg_vif_ch0_capture_en (0x100)
	unsigned int reg_vif_ch0_capture_en:1;

	// h000a, bit: 14
	/* */
	unsigned int :7;

	// h000a
	unsigned int /* padding 16 bit */:16;

	// h000b, bit: 6
	/* Interrupt Mask*/
	#define offset_of_reg_c_irq_mask0 (22)
	#define mask_of_reg_c_irq_mask0 (0x7f)
	unsigned int reg_c_irq_mask0:7;

	// h000b, bit: 14
	/* */
	unsigned int :9;

	// h000b
	unsigned int /* padding 16 bit */:16;

	// h000c, bit: 6
	/* Force Interrupt Enable*/
	#define offset_of_reg_c_irq_force0 (24)
	#define mask_of_reg_c_irq_force0 (0x7f)
	unsigned int reg_c_irq_force0:7;

	// h000c, bit: 14
	/* */
	unsigned int :9;

	// h000c
	unsigned int /* padding 16 bit */:16;

	// h000d, bit: 6
	/* Interrupt Clear*/
	#define offset_of_reg_c_irq_clr0 (26)
	#define mask_of_reg_c_irq_clr0 (0x7f)
	unsigned int reg_c_irq_clr0:7;

	// h000d, bit: 14
	/* */
	unsigned int :9;

	// h000d
	unsigned int /* padding 16 bit */:16;

	// h000e, bit: 6
	/* Status of Interrupt on CPU side*/
	#define offset_of_reg_irq_final_status0 (28)
	#define mask_of_reg_irq_final_status0 (0x7f)
	unsigned int reg_irq_final_status0:7;

	// h000e, bit: 14
	/* */
	unsigned int :9;

	// h000e
	unsigned int /* padding 16 bit */:16;

	// h000f, bit: 6
	/* Status of Interrupt on IP side
	#[0]: Sensor Source VREF rising edge
	#[1]: Sensor Source VREF falling edge
	#[2]: Hardware Flash Strobe done
	#[3]: PAD2VIF_VSYNC rising edge
	#[4]: PAD2VIF_VSYNC falling edge
	#[5]: VIF to ISP line count hit0
	#[6]: VIF to ISP line count hit1*/
	#define offset_of_reg_irq_raw_status0 (30)
	#define mask_of_reg_irq_raw_status0 (0x7f)
	unsigned int reg_irq_raw_status0:7;

	// h000f, bit: 14
	/* */
	unsigned int :9;

	// h000f
	unsigned int /* padding 16 bit */:16;

	// h0010, bit: 0
	/* timing generator enable*/
	#define offset_of_reg_vif_ch0_pat_tgen_en (32)
	#define mask_of_reg_vif_ch0_pat_tgen_en (0x1)
	unsigned int reg_vif_ch0_pat_tgen_en:1;

	// h0010, bit: 1
	/* data generator enable*/
	#define offset_of_reg_vif_ch0_pat_dgen_en (32)
	#define mask_of_reg_vif_ch0_pat_dgen_en (0x2)
	unsigned int reg_vif_ch0_pat_dgen_en:1;

	// h0010, bit: 2
	/* hsync generator*/
	#define offset_of_reg_vif_ch0_pat_hsgen_en (32)
	#define mask_of_reg_vif_ch0_pat_hsgen_en (0x4)
	unsigned int reg_vif_ch0_pat_hsgen_en:1;

	// h0010, bit: 3
	/* data generator  reset*/
	#define offset_of_reg_vif_ch0_pat_dgen_rst (32)
	#define mask_of_reg_vif_ch0_pat_dgen_rst (0x8)
	unsigned int reg_vif_ch0_pat_dgen_rst:1;

	// h0010, bit: 5
	/* bayer format*/
	#define offset_of_reg_vif_ch0_pat_sensor_array (32)
	#define mask_of_reg_vif_ch0_pat_sensor_array (0x30)
	unsigned int reg_vif_ch0_pat_sensor_array:2;

	// h0010, bit: 7
	/* */
	unsigned int :2;

	// h0010, bit: 10
	/* data enable valid rate control*/
	#define offset_of_reg_vif_ch0_pat_de_rate (32)
	#define mask_of_reg_vif_ch0_pat_de_rate (0x700)
	unsigned int reg_vif_ch0_pat_de_rate:3;

	// h0010, bit: 14
	/* */
	unsigned int :5;

	// h0010
	unsigned int /* padding 16 bit */:16;

	// h0011, bit: 12
	/* image width minus one*/
	#define offset_of_reg_vif_ch0_pat_img_width_m1 (34)
	#define mask_of_reg_vif_ch0_pat_img_width_m1 (0x1fff)
	unsigned int reg_vif_ch0_pat_img_width_m1:13;

	// h0011, bit: 14
	/* */
	unsigned int :3;

	// h0011
	unsigned int /* padding 16 bit */:16;

	// h0012, bit: 12
	/* image height minus one*/
	#define offset_of_reg_vif_ch0_pat_img_height_m1 (36)
	#define mask_of_reg_vif_ch0_pat_img_height_m1 (0x1fff)
	unsigned int reg_vif_ch0_pat_img_height_m1:13;

	// h0012, bit: 14
	/* */
	unsigned int :3;

	// h0012
	unsigned int /* padding 16 bit */:16;

	// h0013, bit: 7
	/* vsync pulse position, line number before v-active*/
	#define offset_of_reg_vif_ch0_pat_vs_line (38)
	#define mask_of_reg_vif_ch0_pat_vs_line (0xff)
	unsigned int reg_vif_ch0_pat_vs_line:8;

	// h0013, bit: 14
	/* hsync pulse position, pixel number before v-active*/
	#define offset_of_reg_vif_ch0_pat_hs_pxl (38)
	#define mask_of_reg_vif_ch0_pat_hs_pxl (0xff00)
	unsigned int reg_vif_ch0_pat_hs_pxl:8;

	// h0013
	unsigned int /* padding 16 bit */:16;

	// h0014, bit: 7
	/* H-blanking time (x2)*/
	#define offset_of_reg_vif_ch0_pat_hblank (40)
	#define mask_of_reg_vif_ch0_pat_hblank (0xff)
	unsigned int reg_vif_ch0_pat_hblank:8;

	// h0014, bit: 14
	/* V-blanking time*/
	#define offset_of_reg_vif_ch0_pat_vblank (40)
	#define mask_of_reg_vif_ch0_pat_vblank (0xff00)
	unsigned int reg_vif_ch0_pat_vblank:8;

	// h0014
	unsigned int /* padding 16 bit */:16;

	// h0015, bit: 12
	/* pattern block width minus one*/
	#define offset_of_reg_vif_ch0_pat_blk_width_m1 (42)
	#define mask_of_reg_vif_ch0_pat_blk_width_m1 (0x1fff)
	unsigned int reg_vif_ch0_pat_blk_width_m1:13;

	// h0015, bit: 14
	/* */
	unsigned int :3;

	// h0015
	unsigned int /* padding 16 bit */:16;

	// h0016, bit: 12
	/* pattern block height minus one*/
	#define offset_of_reg_vif_ch0_pat_blk_height_m1 (44)
	#define mask_of_reg_vif_ch0_pat_blk_height_m1 (0x1fff)
	unsigned int reg_vif_ch0_pat_blk_height_m1:13;

	// h0016, bit: 14
	/* */
	unsigned int :3;

	// h0016
	unsigned int /* padding 16 bit */:16;

	// h0017, bit: 2
	/* initial color bar index*/
	#define offset_of_reg_vif_ch0_pat_color_init_idx (46)
	#define mask_of_reg_vif_ch0_pat_color_init_idx (0x7)
	unsigned int reg_vif_ch0_pat_color_init_idx:3;

	// h0017, bit: 3
	/* */
	unsigned int :1;

	// h0017, bit: 5
	/* color bar value percentage*/
	#define offset_of_reg_vif_ch0_pat_color_percent (46)
	#define mask_of_reg_vif_ch0_pat_color_percent (0x30)
	unsigned int reg_vif_ch0_pat_color_percent:2;

	// h0017, bit: 14
	/* */
	unsigned int :10;

	// h0017
	unsigned int /* padding 16 bit */:16;

	// h0018, bit: 7
	/* frame pattern change rate*/
	#define offset_of_reg_vif_ch0_pat_frm_chg_rate (48)
	#define mask_of_reg_vif_ch0_pat_frm_chg_rate (0xff)
	unsigned int reg_vif_ch0_pat_frm_chg_rate:8;

	// h0018, bit: 11
	/* pattern shift pixel number when frame change*/
	#define offset_of_reg_vif_ch0_pat_frm_chg_x (48)
	#define mask_of_reg_vif_ch0_pat_frm_chg_x (0xf00)
	unsigned int reg_vif_ch0_pat_frm_chg_x:4;

	// h0018, bit: 14
	/* pattern shift line number when frame change*/
	#define offset_of_reg_vif_ch0_pat_frm_chg_y (48)
	#define mask_of_reg_vif_ch0_pat_frm_chg_y (0xf000)
	unsigned int reg_vif_ch0_pat_frm_chg_y:4;

	// h0018
	unsigned int /* padding 16 bit */:16;

	// h0019, bit: 14
	/* */
	unsigned int :16;

	// h0019
	unsigned int /* padding 16 bit */:16;

	// h001a, bit: 12
	/* VIF line cnt number0 for interrupt*/
	#define offset_of_reg_vif_ch0_vif2isp_line_cnt0 (52)
	#define mask_of_reg_vif_ch0_vif2isp_line_cnt0 (0x1fff)
	unsigned int reg_vif_ch0_vif2isp_line_cnt0:13;

	// h001a, bit: 14
	/* */
	unsigned int :3;

	// h001a
	unsigned int /* padding 16 bit */:16;

	// h001b, bit: 12
	/* VIF line cnt number1 for interrupt*/
	#define offset_of_reg_vif_ch0_vif2isp_line_cnt1 (54)
	#define mask_of_reg_vif_ch0_vif2isp_line_cnt1 (0x1fff)
	unsigned int reg_vif_ch0_vif2isp_line_cnt1:13;

	// h001b, bit: 14
	/* */
	unsigned int :3;

	// h001b
	unsigned int /* padding 16 bit */:16;

	// h001c, bit: 12
	/* input HSYNC count*/
	#define offset_of_reg_vif_ch0_input_hs_cnt (56)
	#define mask_of_reg_vif_ch0_input_hs_cnt (0x1fff)
	unsigned int reg_vif_ch0_input_hs_cnt:13;

	// h001c, bit: 14
	/* */
	unsigned int :3;

	// h001c
	unsigned int /* padding 16 bit */:16;

	// h001d, bit: 12
	/* input DE count*/
	#define offset_of_reg_vif_ch0_input_de_cnt (58)
	#define mask_of_reg_vif_ch0_input_de_cnt (0x1fff)
	unsigned int reg_vif_ch0_input_de_cnt:13;

	// h001d, bit: 14
	/* */
	unsigned int :3;

	// h001d
	unsigned int /* padding 16 bit */:16;

	// h001e, bit: 12
	/* Line interleaved start line*/
	#define offset_of_reg_vif_ch0_li_st (60)
	#define mask_of_reg_vif_ch0_li_st (0x1fff)
	unsigned int reg_vif_ch0_li_st:13;

	// h001e, bit: 13
	/* Line interleaved skip first 4 data*/
	#define offset_of_reg_vif_ch0_li_skip_4data (60)
	#define mask_of_reg_vif_ch0_li_skip_4data (0x2000)
	unsigned int reg_vif_ch0_li_skip_4data:1;

	// h001e, bit: 14
	/* Line interleaved channel*/
	#define offset_of_reg_vif_ch0_li_ch (60)
	#define mask_of_reg_vif_ch0_li_ch (0x4000)
	unsigned int reg_vif_ch0_li_ch:1;

	// h001e, bit: 15
	/* Line interleaved mode enable*/
	#define offset_of_reg_vif_ch0_li_mode_en (60)
	#define mask_of_reg_vif_ch0_li_mode_en (0x8000)
	unsigned int reg_vif_ch0_li_mode_en:1;

	// h001e
	unsigned int /* padding 16 bit */:16;

	// h001f, bit: 12
	/* Line interleaved end line*/
	#define offset_of_reg_vif_ch0_li_end (62)
	#define mask_of_reg_vif_ch0_li_end (0x1fff)
	unsigned int reg_vif_ch0_li_end:13;

	// h001f, bit: 14
	/* */
	unsigned int :3;

	// h001f
	unsigned int /* padding 16 bit */:16;

	// h0020, bit: 0
	/* Software Reset for Sensor1
	# Low Active
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch1_sensor_sw_rstz (64)
	#define mask_of_reg_vif_ch1_sensor_sw_rstz (0x1)
	unsigned int reg_vif_ch1_sensor_sw_rstz:1;

	// h0020, bit: 1
	/* VIF IF status reset*/
	#define offset_of_reg_vif_ch1_if_state_rst (64)
	#define mask_of_reg_vif_ch1_if_state_rst (0x2)
	unsigned int reg_vif_ch1_if_state_rst:1;

	// h0020, bit: 2
	/* Reset Sensor1*/
	#define offset_of_reg_vif_ch1_sensor_rst (64)
	#define mask_of_reg_vif_ch1_sensor_rst (0x4)
	unsigned int reg_vif_ch1_sensor_rst:1;

	// h0020, bit: 3
	/* Power down sensor1*/
	#define offset_of_reg_vif_ch1_sensor_pwrdn (64)
	#define mask_of_reg_vif_ch1_sensor_pwrdn (0x8)
	unsigned int reg_vif_ch1_sensor_pwrdn:1;

	// h0020, bit: 4
	/* HDR function enable*/
	#define offset_of_reg_hdr1_en (64)
	#define mask_of_reg_hdr1_en (0x10)
	unsigned int reg_hdr1_en:1;

	// h0020, bit: 7
	/* HDR source select
	# 3'd0 MIPI0 source
	# 3'd1 MIPI1 source
	# 3'd2 MIPI2 source
	# 3'd3 MIPI3 source
	# 3'd4 HISPI0 source
	# 3'd5 HISPI1 source
	# 3'd6 HISPI2 source
	# 3'd7 HISPI3 source*/
	#define offset_of_reg_hdr1_sel (64)
	#define mask_of_reg_hdr1_sel (0xe0)
	unsigned int reg_hdr1_sel:3;

	// h0020, bit: 12
	/* VIF source select
	# 5'd0 MIPI0
	# 5'd1 MIPI1
	# 5'd2 MIPI2
	# 5'd3 MIPI3
	# 5'd4 HISPI0
	# 5'd5 HISPI1
	# 5'd6 HISPI2
	# 5'd7 HISPI3
	# 5'd8 MIPI0 VC0
	# 5'd9 MIPI0 VC1
	# 5'd10 MIPI0 VC2
	# 5'd11 MIPI0 VC3
	# 5'd12 MIPI1 VC0
	# 5'd13 MIPI1 VC1
	# 5'd14 MIPI1 VC2
	# 5'd15 MIPI1 VC3
	# 5'd16 Parallel Sensor 0
	# 5'd17 Parallel Sensor 1
	# 5'd18 BT656 0
	# 5'd19 BT656 1
	# 5'd20 BT656 2
	# 5'd21 BT601*/
	#define offset_of_reg_vif_ch1_src_sel (64)
	#define mask_of_reg_vif_ch1_src_sel (0x1f00)
	unsigned int reg_vif_ch1_src_sel:5;

	// h0020, bit: 14
	/* */
	unsigned int :3;

	// h0020
	unsigned int /* padding 16 bit */:16;

	// h0021, bit: 0
	/* Mask sensor/csi ready to ISP_IF*/
	#define offset_of_reg_vif_ch1_sensor_mask (66)
	#define mask_of_reg_vif_ch1_sensor_mask (0x1)
	unsigned int reg_vif_ch1_sensor_mask:1;

	// h0021, bit: 1
	/* Ready only mode*/
	#define offset_of_reg_vif_ch1_if_de_mode (66)
	#define mask_of_reg_vif_ch1_if_de_mode (0x2)
	unsigned int reg_vif_ch1_if_de_mode:1;

	// h0021, bit: 14
	/* */
	unsigned int :14;

	// h0021
	unsigned int /* padding 16 bit */:16;

	// h0022, bit: 0
	/* Enable Software Flash Strobe
	# 1'b0: Disable
	# 1'b1: Enable
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch1_en_sw_strobe (68)
	#define mask_of_reg_vif_ch1_en_sw_strobe (0x1)
	unsigned int reg_vif_ch1_en_sw_strobe:1;

	// h0022, bit: 1
	/* Software Strobe Setting
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch1_sw_strobe (68)
	#define mask_of_reg_vif_ch1_sw_strobe (0x2)
	unsigned int reg_vif_ch1_sw_strobe:1;

	// h0022, bit: 3
	/* */
	unsigned int :2;

	// h0022, bit: 4
	/* Strobe Polarity
	# 1'b0: high active
	# 1'b1: low active*/
	#define offset_of_reg_vif_ch1_strobe_polarity (68)
	#define mask_of_reg_vif_ch1_strobe_polarity (0x10)
	unsigned int reg_vif_ch1_strobe_polarity:1;

	// h0022, bit: 14
	/* */
	unsigned int :11;

	// h0022
	unsigned int /* padding 16 bit */:16;

	// h0023, bit: 14
	/* Start Strobe after reference strobe start (unit:256xPCLK)
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch1_strobe_start (70)
	#define mask_of_reg_vif_ch1_strobe_start (0x7fff)
	unsigned int reg_vif_ch1_strobe_start:15;

	// h0023, bit: 15
	/* Reference Start for strobe signal
	# 1'b0 : vertical active start
	# 1'b1 : vertical blanking start
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch1_strobe_ref (70)
	#define mask_of_reg_vif_ch1_strobe_ref (0x8000)
	unsigned int reg_vif_ch1_strobe_ref:1;

	// h0023
	unsigned int /* padding 16 bit */:16;

	// h0024, bit: 14
	/* End Strobe after reference strobe start (unit:256xPCLK)
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch1_strobe_end (72)
	#define mask_of_reg_vif_ch1_strobe_end (0x7fff)
	unsigned int reg_vif_ch1_strobe_end:15;

	// h0024, bit: 15
	/* Enable Output Strobe Signal By Hardware Control
	# 1'b0: Disable
	# 1'b1: Enable
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch1_en_hw_strobe (72)
	#define mask_of_reg_vif_ch1_en_hw_strobe (0x8000)
	unsigned int reg_vif_ch1_en_hw_strobe:1;

	// h0024
	unsigned int /* padding 16 bit */:16;

	// h0025, bit: 3
	/* Hardware Flash Strobe Counter to count sensor frame number
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch1_hw_strobe_cnt (74)
	#define mask_of_reg_vif_ch1_hw_strobe_cnt (0xf)
	unsigned int reg_vif_ch1_hw_strobe_cnt:4;

	// h0025, bit: 13
	/* */
	unsigned int :10;

	// h0025, bit: 14
	/* Reference END for Hardware Long Strobe
	# 1'b0: vertical active start
	# 1'b1: vertical blanking start
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch1_hw_long_strobe_end_ref (74)
	#define mask_of_reg_vif_ch1_hw_long_strobe_end_ref (0x4000)
	unsigned int reg_vif_ch1_hw_long_strobe_end_ref:1;

	// h0025, bit: 15
	/* Hardware Flash Strobe Mode
	# 1'b0: short strobe
	# 1'b1: long strobe (enable several frames)
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch1_hw_strobe_mode (74)
	#define mask_of_reg_vif_ch1_hw_strobe_mode (0x8000)
	unsigned int reg_vif_ch1_hw_strobe_mode:1;

	// h0025
	unsigned int /* padding 16 bit */:16;

	// h0026, bit: 1
	/* */
	unsigned int :2;

	// h0026, bit: 3
	/* Left shift sensor input data
	# 2'b00 : no shift
	# 2'b01 : shift 2bit
	# 2'b10 : shift 4bit
	# 2'b11 : shift 6bit*/
	#define offset_of_reg_vif_ch1_sensor_format_left_sht (76)
	#define mask_of_reg_vif_ch1_sensor_format_left_sht (0xc)
	unsigned int reg_vif_ch1_sensor_format_left_sht:2;

	// h0026, bit: 6
	/* */
	unsigned int :3;

	// h0026, bit: 7
	/* swap sensor input data*/
	#define offset_of_reg_vif_ch1_sensor_bit_swap (76)
	#define mask_of_reg_vif_ch1_sensor_bit_swap (0x80)
	unsigned int reg_vif_ch1_sensor_bit_swap:1;

	// h0026, bit: 8
	/* Sensor Hsync Polarity
	# 1'b0: high active
	# 1'b1: low active
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch1_sensor_hsync_polarity (76)
	#define mask_of_reg_vif_ch1_sensor_hsync_polarity (0x100)
	unsigned int reg_vif_ch1_sensor_hsync_polarity:1;

	// h0026, bit: 9
	/* Sensor Vsync Polarity
	# 1'b0: high active
	# 1'b1: low active
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch1_sensor_vsync_polarity (76)
	#define mask_of_reg_vif_ch1_sensor_vsync_polarity (0x200)
	unsigned int reg_vif_ch1_sensor_vsync_polarity:1;

	// h0026, bit: 11
	/* Sensor Format
	# 2'b00: 8 bit
	# 2'b01: 10bit
	# 2'b10: 16bit
	# 2'b11: 12bit*/
	#define offset_of_reg_vif_ch1_sensor_format (76)
	#define mask_of_reg_vif_ch1_sensor_format (0xc00)
	unsigned int reg_vif_ch1_sensor_format:2;

	// h0026, bit: 12
	/* Sensor Input Format
	# 1'b0: YUV 422 format
	# 1'b1: RGB pattern
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch1_sensor_rgb_in (76)
	#define mask_of_reg_vif_ch1_sensor_rgb_in (0x1000)
	unsigned int reg_vif_ch1_sensor_rgb_in:1;

	// h0026, bit: 13
	/* */
	unsigned int :1;

	// h0026, bit: 14
	/* Sensor 8 bit to 12 bit mode
	# 2'b00: {[7:0], [7:4]}
	# 2'b01: {[7:0], 4'b0}
	# 2'b10: {[11:4], [11:8]}
	# 2'b11: {[11:4], 4'b0}
	Sensor 10 bit to 12 bit mode
	# 2'b00: {[9:0], [9:8]}
	# 2'b01: {[9:0], 2'b0}
	# 2'b10: {[11:2], [11:10]}
	# 2'b11: {[11:2], 2'b0}*/
	#define offset_of_reg_vif_ch1_sensor_format_ext_mode (76)
	#define mask_of_reg_vif_ch1_sensor_format_ext_mode (0xc000)
	unsigned int reg_vif_ch1_sensor_format_ext_mode:2;

	// h0026
	unsigned int /* padding 16 bit */:16;

	// h0027, bit: 5
	/* */
	unsigned int :6;

	// h0027, bit: 6
	/* Sensor Input Format
	# 1'b0: separate Y/C mode
	# 1'b1: YC 16bit mode*/
	#define offset_of_reg_vif_ch1_sensor_yc16bit (78)
	#define mask_of_reg_vif_ch1_sensor_yc16bit (0x40)
	unsigned int reg_vif_ch1_sensor_yc16bit:1;

	// h0027, bit: 7
	/* */
	unsigned int :1;

	// h0027, bit: 9
	/* Sensor vsync pulse delay
	# 2'd0: vsync falling edge
	# 2'd1: vsync rising edge delay 2 line
	# 2'd2: vsync rising edge delay 1 line
	# 2'd3: vsync rising edge*/
	#define offset_of_reg_vif_ch1_sensor_vs_dly (78)
	#define mask_of_reg_vif_ch1_sensor_vs_dly (0x300)
	unsigned int reg_vif_ch1_sensor_vs_dly:2;

	// h0027, bit: 11
	/* */
	unsigned int :2;

	// h0027, bit: 12
	/* Sensor hsync pulse delay
	# 1'b0: hsync rising edge
	# 1'b1: hsync falling edge*/
	#define offset_of_reg_vif_ch1_sensor_hs_dly (78)
	#define mask_of_reg_vif_ch1_sensor_hs_dly (0x1000)
	unsigned int reg_vif_ch1_sensor_hs_dly:1;

	// h0027, bit: 14
	/* */
	unsigned int :3;

	// h0027
	unsigned int /* padding 16 bit */:16;

	// h0028, bit: 13
	/* Parallel sensor input image width
	Only avalible in parallel sensor & BT601 mode*/
	#define offset_of_reg_vif_ch1_par_width (80)
	#define mask_of_reg_vif_ch1_par_width (0x3fff)
	unsigned int reg_vif_ch1_par_width:14;

	// h0028, bit: 14
	/* */
	unsigned int :2;

	// h0028
	unsigned int /* padding 16 bit */:16;

	// h0029, bit: 0
	/* */
	unsigned int :1;

	// h0029, bit: 1
	/* BT601 Byte-interleaved channel select*/
	#define offset_of_reg_vif_ch1_bt601_ch_sel (82)
	#define mask_of_reg_vif_ch1_bt601_ch_sel (0x2)
	unsigned int reg_vif_ch1_bt601_ch_sel:1;

	// h0029, bit: 2
	/* BT656 YC16bit mode
	# 1'b0: YC
	# 1'b1: CY*/
	#define offset_of_reg_vif_ch1_bt656_yc16bit_mode (82)
	#define mask_of_reg_vif_ch1_bt656_yc16bit_mode (0x4)
	unsigned int reg_vif_ch1_bt656_yc16bit_mode:1;

	// h0029, bit: 14
	/* */
	unsigned int :13;

	// h0029
	unsigned int /* padding 16 bit */:16;

	// h002a, bit: 7
	/* */
	unsigned int :8;

	// h002a, bit: 8
	/* Capture Enable (only for FPGA Debug Mode)
	# 1'b0: disable
	# 1'b1: enable*/
	#define offset_of_reg_vif_ch1_capture_en (84)
	#define mask_of_reg_vif_ch1_capture_en (0x100)
	unsigned int reg_vif_ch1_capture_en:1;

	// h002a, bit: 14
	/* */
	unsigned int :7;

	// h002a
	unsigned int /* padding 16 bit */:16;

	// h002b, bit: 6
	/* Interrupt Mask*/
	#define offset_of_reg_c_irq_mask1 (86)
	#define mask_of_reg_c_irq_mask1 (0x7f)
	unsigned int reg_c_irq_mask1:7;

	// h002b, bit: 14
	/* */
	unsigned int :9;

	// h002b
	unsigned int /* padding 16 bit */:16;

	// h002c, bit: 6
	/* Force Interrupt Enable*/
	#define offset_of_reg_c_irq_force1 (88)
	#define mask_of_reg_c_irq_force1 (0x7f)
	unsigned int reg_c_irq_force1:7;

	// h002c, bit: 14
	/* */
	unsigned int :9;

	// h002c
	unsigned int /* padding 16 bit */:16;

	// h002d, bit: 6
	/* Interrupt Clear*/
	#define offset_of_reg_c_irq_clr1 (90)
	#define mask_of_reg_c_irq_clr1 (0x7f)
	unsigned int reg_c_irq_clr1:7;

	// h002d, bit: 14
	/* */
	unsigned int :9;

	// h002d
	unsigned int /* padding 16 bit */:16;

	// h002e, bit: 6
	/* Status of Interrupt on CPU side*/
	#define offset_of_reg_irq_final_status1 (92)
	#define mask_of_reg_irq_final_status1 (0x7f)
	unsigned int reg_irq_final_status1:7;

	// h002e, bit: 14
	/* */
	unsigned int :9;

	// h002e
	unsigned int /* padding 16 bit */:16;

	// h002f, bit: 6
	/* Status of Interrupt on IP side
	#[0]: Sensor Source VREF rising edge
	#[1]: Sensor Source VREF falling edge
	#[2]: Hardware Flash Strobe done
	#[3]: PAD2VIF_VSYNC rising edge
	#[4]: PAD2VIF_VSYNC falling edge
	#[5]: VIF to ISP line count hit0
	#[6]: VIF to ISP line count hit1*/
	#define offset_of_reg_irq_raw_status1 (94)
	#define mask_of_reg_irq_raw_status1 (0x7f)
	unsigned int reg_irq_raw_status1:7;

	// h002f, bit: 14
	/* */
	unsigned int :9;

	// h002f
	unsigned int /* padding 16 bit */:16;

	// h0030, bit: 0
	/* timing generator enable*/
	#define offset_of_reg_vif_ch1_pat_tgen_en (96)
	#define mask_of_reg_vif_ch1_pat_tgen_en (0x1)
	unsigned int reg_vif_ch1_pat_tgen_en:1;

	// h0030, bit: 1
	/* data generator enable*/
	#define offset_of_reg_vif_ch1_pat_dgen_en (96)
	#define mask_of_reg_vif_ch1_pat_dgen_en (0x2)
	unsigned int reg_vif_ch1_pat_dgen_en:1;

	// h0030, bit: 2
	/* hsync generator*/
	#define offset_of_reg_vif_ch1_pat_hsgen_en (96)
	#define mask_of_reg_vif_ch1_pat_hsgen_en (0x4)
	unsigned int reg_vif_ch1_pat_hsgen_en:1;

	// h0030, bit: 3
	/* data generator  reset*/
	#define offset_of_reg_vif_ch1_pat_dgen_rst (96)
	#define mask_of_reg_vif_ch1_pat_dgen_rst (0x8)
	unsigned int reg_vif_ch1_pat_dgen_rst:1;

	// h0030, bit: 5
	/* bayer format*/
	#define offset_of_reg_vif_ch1_pat_sensor_array (96)
	#define mask_of_reg_vif_ch1_pat_sensor_array (0x30)
	unsigned int reg_vif_ch1_pat_sensor_array:2;

	// h0030, bit: 7
	/* */
	unsigned int :2;

	// h0030, bit: 10
	/* data enable valid rate control*/
	#define offset_of_reg_vif_ch1_pat_de_rate (96)
	#define mask_of_reg_vif_ch1_pat_de_rate (0x700)
	unsigned int reg_vif_ch1_pat_de_rate:3;

	// h0030, bit: 14
	/* */
	unsigned int :5;

	// h0030
	unsigned int /* padding 16 bit */:16;

	// h0031, bit: 12
	/* image width minus one*/
	#define offset_of_reg_vif_ch1_pat_img_width_m1 (98)
	#define mask_of_reg_vif_ch1_pat_img_width_m1 (0x1fff)
	unsigned int reg_vif_ch1_pat_img_width_m1:13;

	// h0031, bit: 14
	/* */
	unsigned int :3;

	// h0031
	unsigned int /* padding 16 bit */:16;

	// h0032, bit: 12
	/* image height minus one*/
	#define offset_of_reg_vif_ch1_pat_img_height_m1 (100)
	#define mask_of_reg_vif_ch1_pat_img_height_m1 (0x1fff)
	unsigned int reg_vif_ch1_pat_img_height_m1:13;

	// h0032, bit: 14
	/* */
	unsigned int :3;

	// h0032
	unsigned int /* padding 16 bit */:16;

	// h0033, bit: 7
	/* vsync pulse position, line number before v-active*/
	#define offset_of_reg_vif_ch1_pat_vs_line (102)
	#define mask_of_reg_vif_ch1_pat_vs_line (0xff)
	unsigned int reg_vif_ch1_pat_vs_line:8;

	// h0033, bit: 14
	/* hsync pulse position, pixel number before v-active*/
	#define offset_of_reg_vif_ch1_pat_hs_pxl (102)
	#define mask_of_reg_vif_ch1_pat_hs_pxl (0xff00)
	unsigned int reg_vif_ch1_pat_hs_pxl:8;

	// h0033
	unsigned int /* padding 16 bit */:16;

	// h0034, bit: 7
	/* H-blanking time (x2)*/
	#define offset_of_reg_vif_ch1_pat_hblank (104)
	#define mask_of_reg_vif_ch1_pat_hblank (0xff)
	unsigned int reg_vif_ch1_pat_hblank:8;

	// h0034, bit: 14
	/* V-blanking time*/
	#define offset_of_reg_vif_ch1_pat_vblank (104)
	#define mask_of_reg_vif_ch1_pat_vblank (0xff00)
	unsigned int reg_vif_ch1_pat_vblank:8;

	// h0034
	unsigned int /* padding 16 bit */:16;

	// h0035, bit: 12
	/* pattern block width minus one*/
	#define offset_of_reg_vif_ch1_pat_blk_width_m1 (106)
	#define mask_of_reg_vif_ch1_pat_blk_width_m1 (0x1fff)
	unsigned int reg_vif_ch1_pat_blk_width_m1:13;

	// h0035, bit: 14
	/* */
	unsigned int :3;

	// h0035
	unsigned int /* padding 16 bit */:16;

	// h0036, bit: 12
	/* pattern block height minus one*/
	#define offset_of_reg_vif_ch1_pat_blk_height_m1 (108)
	#define mask_of_reg_vif_ch1_pat_blk_height_m1 (0x1fff)
	unsigned int reg_vif_ch1_pat_blk_height_m1:13;

	// h0036, bit: 14
	/* */
	unsigned int :3;

	// h0036
	unsigned int /* padding 16 bit */:16;

	// h0037, bit: 2
	/* initial color bar index*/
	#define offset_of_reg_vif_ch1_pat_color_init_idx (110)
	#define mask_of_reg_vif_ch1_pat_color_init_idx (0x7)
	unsigned int reg_vif_ch1_pat_color_init_idx:3;

	// h0037, bit: 3
	/* */
	unsigned int :1;

	// h0037, bit: 5
	/* color bar value percentage*/
	#define offset_of_reg_vif_ch1_pat_color_percent (110)
	#define mask_of_reg_vif_ch1_pat_color_percent (0x30)
	unsigned int reg_vif_ch1_pat_color_percent:2;

	// h0037, bit: 14
	/* */
	unsigned int :10;

	// h0037
	unsigned int /* padding 16 bit */:16;

	// h0038, bit: 7
	/* frame pattern change rate*/
	#define offset_of_reg_vif_ch1_pat_frm_chg_rate (112)
	#define mask_of_reg_vif_ch1_pat_frm_chg_rate (0xff)
	unsigned int reg_vif_ch1_pat_frm_chg_rate:8;

	// h0038, bit: 11
	/* pattern shift pixel number when frame change*/
	#define offset_of_reg_vif_ch1_pat_frm_chg_x (112)
	#define mask_of_reg_vif_ch1_pat_frm_chg_x (0xf00)
	unsigned int reg_vif_ch1_pat_frm_chg_x:4;

	// h0038, bit: 14
	/* pattern shift line number when frame change*/
	#define offset_of_reg_vif_ch1_pat_frm_chg_y (112)
	#define mask_of_reg_vif_ch1_pat_frm_chg_y (0xf000)
	unsigned int reg_vif_ch1_pat_frm_chg_y:4;

	// h0038
	unsigned int /* padding 16 bit */:16;

	// h0039, bit: 14
	/* */
	unsigned int :16;

	// h0039
	unsigned int /* padding 16 bit */:16;

	// h003a, bit: 12
	/* VIF line cnt number0 for interrupt*/
	#define offset_of_reg_vif_ch1_vif2isp_line_cnt0 (116)
	#define mask_of_reg_vif_ch1_vif2isp_line_cnt0 (0x1fff)
	unsigned int reg_vif_ch1_vif2isp_line_cnt0:13;

	// h003a, bit: 14
	/* */
	unsigned int :3;

	// h003a
	unsigned int /* padding 16 bit */:16;

	// h003b, bit: 12
	/* VIF line cnt number1 for interrupt*/
	#define offset_of_reg_vif_ch1_vif2isp_line_cnt1 (118)
	#define mask_of_reg_vif_ch1_vif2isp_line_cnt1 (0x1fff)
	unsigned int reg_vif_ch1_vif2isp_line_cnt1:13;

	// h003b, bit: 14
	/* */
	unsigned int :3;

	// h003b
	unsigned int /* padding 16 bit */:16;

	// h003c, bit: 12
	/* input HSYNC count*/
	#define offset_of_reg_vif_ch1_input_hs_cnt (120)
	#define mask_of_reg_vif_ch1_input_hs_cnt (0x1fff)
	unsigned int reg_vif_ch1_input_hs_cnt:13;

	// h003c, bit: 14
	/* */
	unsigned int :3;

	// h003c
	unsigned int /* padding 16 bit */:16;

	// h003d, bit: 12
	/* input DE count*/
	#define offset_of_reg_vif_ch1_input_de_cnt (122)
	#define mask_of_reg_vif_ch1_input_de_cnt (0x1fff)
	unsigned int reg_vif_ch1_input_de_cnt:13;

	// h003d, bit: 14
	/* */
	unsigned int :3;

	// h003d
	unsigned int /* padding 16 bit */:16;

	// h003e, bit: 12
	/* Line interleaved start line*/
	#define offset_of_reg_vif_ch1_li_st (124)
	#define mask_of_reg_vif_ch1_li_st (0x1fff)
	unsigned int reg_vif_ch1_li_st:13;

	// h003e, bit: 13
	/* Line interleaved skip first 4 data*/
	#define offset_of_reg_vif_ch1_li_skip_4data (124)
	#define mask_of_reg_vif_ch1_li_skip_4data (0x2000)
	unsigned int reg_vif_ch1_li_skip_4data:1;

	// h003e, bit: 14
	/* Line interleaved channel*/
	#define offset_of_reg_vif_ch1_li_ch (124)
	#define mask_of_reg_vif_ch1_li_ch (0x4000)
	unsigned int reg_vif_ch1_li_ch:1;

	// h003e, bit: 15
	/* Line interleaved mode enable*/
	#define offset_of_reg_vif_ch1_li_mode_en (124)
	#define mask_of_reg_vif_ch1_li_mode_en (0x8000)
	unsigned int reg_vif_ch1_li_mode_en:1;

	// h003e
	unsigned int /* padding 16 bit */:16;

	// h003f, bit: 12
	/* Line interleaved end line*/
	#define offset_of_reg_vif_ch1_li_end (126)
	#define mask_of_reg_vif_ch1_li_end (0x1fff)
	unsigned int reg_vif_ch1_li_end:13;

	// h003f, bit: 14
	/* */
	unsigned int :3;

	// h003f
	unsigned int /* padding 16 bit */:16;

	// h0040, bit: 0
	/* Software Reset for Sensor2
	# Low Active
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch2_sensor_sw_rstz (128)
	#define mask_of_reg_vif_ch2_sensor_sw_rstz (0x1)
	unsigned int reg_vif_ch2_sensor_sw_rstz:1;

	// h0040, bit: 1
	/* VIF IF status reset*/
	#define offset_of_reg_vif_ch2_if_state_rst (128)
	#define mask_of_reg_vif_ch2_if_state_rst (0x2)
	unsigned int reg_vif_ch2_if_state_rst:1;

	// h0040, bit: 2
	/* Reset Sensor2*/
	#define offset_of_reg_vif_ch2_sensor_rst (128)
	#define mask_of_reg_vif_ch2_sensor_rst (0x4)
	unsigned int reg_vif_ch2_sensor_rst:1;

	// h0040, bit: 3
	/* Power down sensor2*/
	#define offset_of_reg_vif_ch2_sensor_pwrdn (128)
	#define mask_of_reg_vif_ch2_sensor_pwrdn (0x8)
	unsigned int reg_vif_ch2_sensor_pwrdn:1;

	// h0040, bit: 7
	/* */
	unsigned int :4;

	// h0040, bit: 12
	/* VIF source select
	# 5'd0 MIPI0
	# 5'd1 MIPI1
	# 5'd2 MIPI2
	# 5'd3 MIPI3
	# 5'd4 HISPI0
	# 5'd5 HISPI1
	# 5'd6 HISPI2
	# 5'd7 HISPI3
	# 5'd8 MIPI0 VC0
	# 5'd9 MIPI0 VC1
	# 5'd10 MIPI0 VC2
	# 5'd11 MIPI0 VC3
	# 5'd12 MIPI1 VC0
	# 5'd13 MIPI1 VC1
	# 5'd14 MIPI1 VC2
	# 5'd15 MIPI1 VC3
	# 5'd16 Parallel Sensor 0
	# 5'd17 Parallel Sensor 1
	# 5'd18 BT656 0
	# 5'd19 BT656 1
	# 5'd20 BT656 2
	# 5'd21 BT601*/
	#define offset_of_reg_vif_ch2_src_sel (128)
	#define mask_of_reg_vif_ch2_src_sel (0x1f00)
	unsigned int reg_vif_ch2_src_sel:5;

	// h0040, bit: 14
	/* */
	unsigned int :3;

	// h0040
	unsigned int /* padding 16 bit */:16;

	// h0041, bit: 0
	/* Mask sensor/csi ready to ISP_IF*/
	#define offset_of_reg_vif_ch2_sensor_mask (130)
	#define mask_of_reg_vif_ch2_sensor_mask (0x1)
	unsigned int reg_vif_ch2_sensor_mask:1;

	// h0041, bit: 1
	/* Ready only mode*/
	#define offset_of_reg_vif_ch2_if_de_mode (130)
	#define mask_of_reg_vif_ch2_if_de_mode (0x2)
	unsigned int reg_vif_ch2_if_de_mode:1;

	// h0041, bit: 14
	/* */
	unsigned int :14;

	// h0041
	unsigned int /* padding 16 bit */:16;

	// h0042, bit: 0
	/* Enable Software Flash Strobe
	# 1'b0: Disable
	# 1'b1: Enable
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch2_en_sw_strobe (132)
	#define mask_of_reg_vif_ch2_en_sw_strobe (0x1)
	unsigned int reg_vif_ch2_en_sw_strobe:1;

	// h0042, bit: 1
	/* Software Strobe Setting
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch2_sw_strobe (132)
	#define mask_of_reg_vif_ch2_sw_strobe (0x2)
	unsigned int reg_vif_ch2_sw_strobe:1;

	// h0042, bit: 3
	/* */
	unsigned int :2;

	// h0042, bit: 4
	/* Strobe Polarity
	# 1'b0: high active
	# 1'b1: low active*/
	#define offset_of_reg_vif_ch2_strobe_polarity (132)
	#define mask_of_reg_vif_ch2_strobe_polarity (0x10)
	unsigned int reg_vif_ch2_strobe_polarity:1;

	// h0042, bit: 14
	/* */
	unsigned int :11;

	// h0042
	unsigned int /* padding 16 bit */:16;

	// h0043, bit: 14
	/* Start Strobe after reference strobe start (unit:256xPCLK)
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch2_strobe_start (134)
	#define mask_of_reg_vif_ch2_strobe_start (0x7fff)
	unsigned int reg_vif_ch2_strobe_start:15;

	// h0043, bit: 15
	/* Reference Start for strobe signal
	# 1'b0 : vertical active start
	# 1'b1 : vertical blanking start
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch2_strobe_ref (134)
	#define mask_of_reg_vif_ch2_strobe_ref (0x8000)
	unsigned int reg_vif_ch2_strobe_ref:1;

	// h0043
	unsigned int /* padding 16 bit */:16;

	// h0044, bit: 14
	/* End Strobe after reference strobe start (unit:256xPCLK)
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch2_strobe_end (136)
	#define mask_of_reg_vif_ch2_strobe_end (0x7fff)
	unsigned int reg_vif_ch2_strobe_end:15;

	// h0044, bit: 15
	/* Enable Output Strobe Signal By Hardware Control
	# 1'b0: Disable
	# 1'b1: Enable
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch2_en_hw_strobe (136)
	#define mask_of_reg_vif_ch2_en_hw_strobe (0x8000)
	unsigned int reg_vif_ch2_en_hw_strobe:1;

	// h0044
	unsigned int /* padding 16 bit */:16;

	// h0045, bit: 3
	/* Hardware Flash Strobe Counter to count sensor frame number
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch2_hw_strobe_cnt (138)
	#define mask_of_reg_vif_ch2_hw_strobe_cnt (0xf)
	unsigned int reg_vif_ch2_hw_strobe_cnt:4;

	// h0045, bit: 13
	/* */
	unsigned int :10;

	// h0045, bit: 14
	/* Reference END for Hardware Long Strobe
	# 1'b0: vertical active start
	# 1'b1: vertical blanking start
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch2_hw_long_strobe_end_ref (138)
	#define mask_of_reg_vif_ch2_hw_long_strobe_end_ref (0x4000)
	unsigned int reg_vif_ch2_hw_long_strobe_end_ref:1;

	// h0045, bit: 15
	/* Hardware Flash Strobe Mode
	# 1'b0: short strobe
	# 1'b1: long strobe (enable several frames)
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch2_hw_strobe_mode (138)
	#define mask_of_reg_vif_ch2_hw_strobe_mode (0x8000)
	unsigned int reg_vif_ch2_hw_strobe_mode:1;

	// h0045
	unsigned int /* padding 16 bit */:16;

	// h0046, bit: 1
	/* */
	unsigned int :2;

	// h0046, bit: 3
	/* Left shift sensor input data
	# 2'b00 : no shift
	# 2'b01 : shift 2bit
	# 2'b10 : shift 4bit
	# 2'b11 : shift 6bit*/
	#define offset_of_reg_vif_ch2_sensor_format_left_sht (140)
	#define mask_of_reg_vif_ch2_sensor_format_left_sht (0xc)
	unsigned int reg_vif_ch2_sensor_format_left_sht:2;

	// h0046, bit: 6
	/* */
	unsigned int :3;

	// h0046, bit: 7
	/* swap sensor input data*/
	#define offset_of_reg_vif_ch2_sensor_bit_swap (140)
	#define mask_of_reg_vif_ch2_sensor_bit_swap (0x80)
	unsigned int reg_vif_ch2_sensor_bit_swap:1;

	// h0046, bit: 8
	/* Sensor Hsync Polarity
	# 1'b0: high active
	# 1'b1: low active
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch2_sensor_hsync_polarity (140)
	#define mask_of_reg_vif_ch2_sensor_hsync_polarity (0x100)
	unsigned int reg_vif_ch2_sensor_hsync_polarity:1;

	// h0046, bit: 9
	/* Sensor Vsync Polarity
	# 1'b0: high active
	# 1'b1: low active
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch2_sensor_vsync_polarity (140)
	#define mask_of_reg_vif_ch2_sensor_vsync_polarity (0x200)
	unsigned int reg_vif_ch2_sensor_vsync_polarity:1;

	// h0046, bit: 11
	/* Sensor Format
	# 2'b00: 8 bit
	# 2'b01: 10bit
	# 2'b10: 16bit
	# 2'b11: 12bit*/
	#define offset_of_reg_vif_ch2_sensor_format (140)
	#define mask_of_reg_vif_ch2_sensor_format (0xc00)
	unsigned int reg_vif_ch2_sensor_format:2;

	// h0046, bit: 12
	/* Sensor Input Format
	# 1'b0: YUV 422 format
	# 1'b1: RGB pattern
	 ( single buffer register )*/
	#define offset_of_reg_vif_ch2_sensor_rgb_in (140)
	#define mask_of_reg_vif_ch2_sensor_rgb_in (0x1000)
	unsigned int reg_vif_ch2_sensor_rgb_in:1;

	// h0046, bit: 13
	/* */
	unsigned int :1;

	// h0046, bit: 14
	/* Sensor 8 bit to 12 bit mode
	# 2'b00: {[7:0], [7:4]}
	# 2'b01: {[7:0], 4'b0}
	# 2'b10: {[11:4], [11:8]}
	# 2'b11: {[11:4], 4'b0}
	Sensor 10 bit to 12 bit mode
	# 2'b00: {[9:0], [9:8]}
	# 2'b01: {[9:0], 2'b0}
	# 2'b10: {[11:2], [11:10]}
	# 2'b11: {[11:2], 2'b0}*/
	#define offset_of_reg_vif_ch2_sensor_format_ext_mode (140)
	#define mask_of_reg_vif_ch2_sensor_format_ext_mode (0xc000)
	unsigned int reg_vif_ch2_sensor_format_ext_mode:2;

	// h0046
	unsigned int /* padding 16 bit */:16;

	// h0047, bit: 5
	/* */
	unsigned int :6;

	// h0047, bit: 6
	/* Sensor Input Format
	# 1'b0: separate Y/C mode
	# 1'b1: YC 16bit mode*/
	#define offset_of_reg_vif_ch2_sensor_yc16bit (142)
	#define mask_of_reg_vif_ch2_sensor_yc16bit (0x40)
	unsigned int reg_vif_ch2_sensor_yc16bit:1;

	// h0047, bit: 7
	/* */
	unsigned int :1;

	// h0047, bit: 9
	/* Sensor vsync pulse delay
	# 2'd0: vsync falling edge
	# 2'd1: vsync rising edge delay 2 line
	# 2'd2: vsync rising edge delay 1 line
	# 2'd3: vsync rising edge*/
	#define offset_of_reg_vif_ch2_sensor_vs_dly (142)
	#define mask_of_reg_vif_ch2_sensor_vs_dly (0x300)
	unsigned int reg_vif_ch2_sensor_vs_dly:2;

	// h0047, bit: 11
	/* */
	unsigned int :2;

	// h0047, bit: 12
	/* Sensor hsync pulse delay
	# 1'b0: hsync rising edge
	# 1'b1: hsync falling edge*/
	#define offset_of_reg_vif_ch2_sensor_hs_dly (142)
	#define mask_of_reg_vif_ch2_sensor_hs_dly (0x1000)
	unsigned int reg_vif_ch2_sensor_hs_dly:1;

	// h0047, bit: 14
	/* */
	unsigned int :3;

	// h0047
	unsigned int /* padding 16 bit */:16;

	// h0048, bit: 13
	/* Parallel sensor input image width
	Only avalible in parallel sensor & BT601 mode*/
	#define offset_of_reg_vif_ch2_par_width (144)
	#define mask_of_reg_vif_ch2_par_width (0x3fff)
	unsigned int reg_vif_ch2_par_width:14;

	// h0048, bit: 14
	/* */
	unsigned int :2;

	// h0048
	unsigned int /* padding 16 bit */:16;

	// h0049, bit: 0
	/* */
	unsigned int :1;

	// h0049, bit: 1
	/* BT601 Byte-interleaved channel select*/
	#define offset_of_reg_vif_ch2_bt601_ch_sel (146)
	#define mask_of_reg_vif_ch2_bt601_ch_sel (0x2)
	unsigned int reg_vif_ch2_bt601_ch_sel:1;

	// h0049, bit: 2
	/* BT656 YC16bit mode
	# 1'b0: YC
	# 1'b1: CY*/
	#define offset_of_reg_vif_ch2_bt656_yc16bit_mode (146)
	#define mask_of_reg_vif_ch2_bt656_yc16bit_mode (0x4)
	unsigned int reg_vif_ch2_bt656_yc16bit_mode:1;

	// h0049, bit: 14
	/* */
	unsigned int :13;

	// h0049
	unsigned int /* padding 16 bit */:16;

	// h004a, bit: 7
	/* */
	unsigned int :8;

	// h004a, bit: 8
	/* Capture Enable (only for FPGA Debug Mode)
	# 1'b0: disable
	# 1'b1: enable*/
	#define offset_of_reg_vif_ch2_capture_en (148)
	#define mask_of_reg_vif_ch2_capture_en (0x100)
	unsigned int reg_vif_ch2_capture_en:1;

	// h004a, bit: 14
	/* */
	unsigned int :7;

	// h004a
	unsigned int /* padding 16 bit */:16;

	// h004b, bit: 6
	/* Interrupt Mask*/
	#define offset_of_reg_c_irq_mask2 (150)
	#define mask_of_reg_c_irq_mask2 (0x7f)
	unsigned int reg_c_irq_mask2:7;

	// h004b, bit: 14
	/* */
	unsigned int :9;

	// h004b
	unsigned int /* padding 16 bit */:16;

	// h004c, bit: 6
	/* Force Interrupt Enable*/
	#define offset_of_reg_c_irq_force2 (152)
	#define mask_of_reg_c_irq_force2 (0x7f)
	unsigned int reg_c_irq_force2:7;

	// h004c, bit: 14
	/* */
	unsigned int :9;

	// h004c
	unsigned int /* padding 16 bit */:16;

	// h004d, bit: 6
	/* Interrupt Clear*/
	#define offset_of_reg_c_irq_clr2 (154)
	#define mask_of_reg_c_irq_clr2 (0x7f)
	unsigned int reg_c_irq_clr2:7;

	// h004d, bit: 14
	/* */
	unsigned int :9;

	// h004d
	unsigned int /* padding 16 bit */:16;

	// h004e, bit: 6
	/* Status of Interrupt on CPU side*/
	#define offset_of_reg_irq_final_status2 (156)
	#define mask_of_reg_irq_final_status2 (0x7f)
	unsigned int reg_irq_final_status2:7;

	// h004e, bit: 14
	/* */
	unsigned int :9;

	// h004e
	unsigned int /* padding 16 bit */:16;

	// h004f, bit: 6
	/* Status of Interrupt on IP side
	#[0]: Sensor Source VREF rising edge
	#[1]: Sensor Source VREF falling edge
	#[2]: Hardware Flash Strobe done
	#[3]: PAD2VIF_VSYNC rising edge
	#[4]: PAD2VIF_VSYNC falling edge
	#[5]: VIF to ISP line count hit0
	#[6]: VIF to ISP line count hit1*/
	#define offset_of_reg_irq_raw_status2 (158)
	#define mask_of_reg_irq_raw_status2 (0x7f)
	unsigned int reg_irq_raw_status2:7;

	// h004f, bit: 14
	/* */
	unsigned int :9;

	// h004f
	unsigned int /* padding 16 bit */:16;

	// h0050, bit: 0
	/* timing generator enable*/
	#define offset_of_reg_vif_ch2_pat_tgen_en (160)
	#define mask_of_reg_vif_ch2_pat_tgen_en (0x1)
	unsigned int reg_vif_ch2_pat_tgen_en:1;

	// h0050, bit: 1
	/* data generator enable*/
	#define offset_of_reg_vif_ch2_pat_dgen_en (160)
	#define mask_of_reg_vif_ch2_pat_dgen_en (0x2)
	unsigned int reg_vif_ch2_pat_dgen_en:1;

	// h0050, bit: 2
	/* hsync generator*/
	#define offset_of_reg_vif_ch2_pat_hsgen_en (160)
	#define mask_of_reg_vif_ch2_pat_hsgen_en (0x4)
	unsigned int reg_vif_ch2_pat_hsgen_en:1;

	// h0050, bit: 3
	/* data generator  reset*/
	#define offset_of_reg_vif_ch2_pat_dgen_rst (160)
	#define mask_of_reg_vif_ch2_pat_dgen_rst (0x8)
	unsigned int reg_vif_ch2_pat_dgen_rst:1;

	// h0050, bit: 5
	/* bayer format*/
	#define offset_of_reg_vif_ch2_pat_sensor_array (160)
	#define mask_of_reg_vif_ch2_pat_sensor_array (0x30)
	unsigned int reg_vif_ch2_pat_sensor_array:2;

	// h0050, bit: 7
	/* */
	unsigned int :2;

	// h0050, bit: 10
	/* data enable valid rate control*/
	#define offset_of_reg_vif_ch2_pat_de_rate (160)
	#define mask_of_reg_vif_ch2_pat_de_rate (0x700)
	unsigned int reg_vif_ch2_pat_de_rate:3;

	// h0050, bit: 14
	/* */
	unsigned int :5;

	// h0050
	unsigned int /* padding 16 bit */:16;

	// h0051, bit: 12
	/* image width minus one*/
	#define offset_of_reg_vif_ch2_pat_img_width_m1 (162)
	#define mask_of_reg_vif_ch2_pat_img_width_m1 (0x1fff)
	unsigned int reg_vif_ch2_pat_img_width_m1:13;

	// h0051, bit: 14
	/* */
	unsigned int :3;

	// h0051
	unsigned int /* padding 16 bit */:16;

	// h0052, bit: 12
	/* image height minus one*/
	#define offset_of_reg_vif_ch2_pat_img_height_m1 (164)
	#define mask_of_reg_vif_ch2_pat_img_height_m1 (0x1fff)
	unsigned int reg_vif_ch2_pat_img_height_m1:13;

	// h0052, bit: 14
	/* */
	unsigned int :3;

	// h0052
	unsigned int /* padding 16 bit */:16;

	// h0053, bit: 7
	/* vsync pulse position, line number before v-active*/
	#define offset_of_reg_vif_ch2_pat_vs_line (166)
	#define mask_of_reg_vif_ch2_pat_vs_line (0xff)
	unsigned int reg_vif_ch2_pat_vs_line:8;

	// h0053, bit: 14
	/* hsync pulse position, pixel number before v-active*/
	#define offset_of_reg_vif_ch2_pat_hs_pxl (166)
	#define mask_of_reg_vif_ch2_pat_hs_pxl (0xff00)
	unsigned int reg_vif_ch2_pat_hs_pxl:8;

	// h0053
	unsigned int /* padding 16 bit */:16;

	// h0054, bit: 7
	/* H-blanking time (x2)*/
	#define offset_of_reg_vif_ch2_pat_hblank (168)
	#define mask_of_reg_vif_ch2_pat_hblank (0xff)
	unsigned int reg_vif_ch2_pat_hblank:8;

	// h0054, bit: 14
	/* V-blanking time*/
	#define offset_of_reg_vif_ch2_pat_vblank (168)
	#define mask_of_reg_vif_ch2_pat_vblank (0xff00)
	unsigned int reg_vif_ch2_pat_vblank:8;

	// h0054
	unsigned int /* padding 16 bit */:16;

	// h0055, bit: 12
	/* pattern block width minus one*/
	#define offset_of_reg_vif_ch2_pat_blk_width_m1 (170)
	#define mask_of_reg_vif_ch2_pat_blk_width_m1 (0x1fff)
	unsigned int reg_vif_ch2_pat_blk_width_m1:13;

	// h0055, bit: 14
	/* */
	unsigned int :3;

	// h0055
	unsigned int /* padding 16 bit */:16;

	// h0056, bit: 12
	/* pattern block height minus one*/
	#define offset_of_reg_vif_ch2_pat_blk_height_m1 (172)
	#define mask_of_reg_vif_ch2_pat_blk_height_m1 (0x1fff)
	unsigned int reg_vif_ch2_pat_blk_height_m1:13;

	// h0056, bit: 14
	/* */
	unsigned int :3;

	// h0056
	unsigned int /* padding 16 bit */:16;

	// h0057, bit: 2
	/* initial color bar index*/
	#define offset_of_reg_vif_ch2_pat_color_init_idx (174)
	#define mask_of_reg_vif_ch2_pat_color_init_idx (0x7)
	unsigned int reg_vif_ch2_pat_color_init_idx:3;

	// h0057, bit: 3
	/* */
	unsigned int :1;

	// h0057, bit: 5
	/* color bar value percentage*/
	#define offset_of_reg_vif_ch2_pat_color_percent (174)
	#define mask_of_reg_vif_ch2_pat_color_percent (0x30)
	unsigned int reg_vif_ch2_pat_color_percent:2;

	// h0057, bit: 14
	/* */
	unsigned int :10;

	// h0057
	unsigned int /* padding 16 bit */:16;

	// h0058, bit: 7
	/* frame pattern change rate*/
	#define offset_of_reg_vif_ch2_pat_frm_chg_rate (176)
	#define mask_of_reg_vif_ch2_pat_frm_chg_rate (0xff)
	unsigned int reg_vif_ch2_pat_frm_chg_rate:8;

	// h0058, bit: 11
	/* pattern shift pixel number when frame change*/
	#define offset_of_reg_vif_ch2_pat_frm_chg_x (176)
	#define mask_of_reg_vif_ch2_pat_frm_chg_x (0xf00)
	unsigned int reg_vif_ch2_pat_frm_chg_x:4;

	// h0058, bit: 14
	/* pattern shift line number when frame change*/
	#define offset_of_reg_vif_ch2_pat_frm_chg_y (176)
	#define mask_of_reg_vif_ch2_pat_frm_chg_y (0xf000)
	unsigned int reg_vif_ch2_pat_frm_chg_y:4;

	// h0058
	unsigned int /* padding 16 bit */:16;

	// h0059, bit: 14
	/* */
	unsigned int :16;

	// h0059
	unsigned int /* padding 16 bit */:16;

	// h005a, bit: 12
	/* VIF line cnt number0 for interrupt*/
	#define offset_of_reg_vif_ch2_vif2isp_line_cnt0 (180)
	#define mask_of_reg_vif_ch2_vif2isp_line_cnt0 (0x1fff)
	unsigned int reg_vif_ch2_vif2isp_line_cnt0:13;

	// h005a, bit: 14
	/* */
	unsigned int :3;

	// h005a
	unsigned int /* padding 16 bit */:16;

	// h005b, bit: 12
	/* VIF line cnt number1 for interrupt*/
	#define offset_of_reg_vif_ch2_vif2isp_line_cnt1 (182)
	#define mask_of_reg_vif_ch2_vif2isp_line_cnt1 (0x1fff)
	unsigned int reg_vif_ch2_vif2isp_line_cnt1:13;

	// h005b, bit: 14
	/* */
	unsigned int :3;

	// h005b
	unsigned int /* padding 16 bit */:16;

	// h005c, bit: 12
	/* input HSYNC count*/
	#define offset_of_reg_vif_ch2_input_hs_cnt (184)
	#define mask_of_reg_vif_ch2_input_hs_cnt (0x1fff)
	unsigned int reg_vif_ch2_input_hs_cnt:13;

	// h005c, bit: 14
	/* */
	unsigned int :3;

	// h005c
	unsigned int /* padding 16 bit */:16;

	// h005d, bit: 12
	/* input DE count*/
	#define offset_of_reg_vif_ch2_input_de_cnt (186)
	#define mask_of_reg_vif_ch2_input_de_cnt (0x1fff)
	unsigned int reg_vif_ch2_input_de_cnt:13;

	// h005d, bit: 14
	/* */
	unsigned int :3;

	// h005d
	unsigned int /* padding 16 bit */:16;

	// h005e, bit: 12
	/* Line interleaved start line*/
	#define offset_of_reg_vif_ch2_li_st (188)
	#define mask_of_reg_vif_ch2_li_st (0x1fff)
	unsigned int reg_vif_ch2_li_st:13;

	// h005e, bit: 13
	/* Line interleaved skip first 4 data*/
	#define offset_of_reg_vif_ch2_li_skip_4data (188)
	#define mask_of_reg_vif_ch2_li_skip_4data (0x2000)
	unsigned int reg_vif_ch2_li_skip_4data:1;

	// h005e, bit: 14
	/* Line interleaved channel*/
	#define offset_of_reg_vif_ch2_li_ch (188)
	#define mask_of_reg_vif_ch2_li_ch (0x4000)
	unsigned int reg_vif_ch2_li_ch:1;

	// h005e, bit: 15
	/* Line interleaved mode enable*/
	#define offset_of_reg_vif_ch2_li_mode_en (188)
	#define mask_of_reg_vif_ch2_li_mode_en (0x8000)
	unsigned int reg_vif_ch2_li_mode_en:1;

	// h005e
	unsigned int /* padding 16 bit */:16;

	// h005f, bit: 12
	/* Line interleaved end line*/
	#define offset_of_reg_vif_ch2_li_end (190)
	#define mask_of_reg_vif_ch2_li_end (0x1fff)
	unsigned int reg_vif_ch2_li_end:13;

	// h005f, bit: 14
	/* */
	unsigned int :3;

	// h005f
	unsigned int /* padding 16 bit */:16;

	// h0060, bit: 0
	/* BT656 input source select*/
	#define offset_of_reg_vif_ch0_bt656_input_sel (192)
	#define mask_of_reg_vif_ch0_bt656_input_sel (0x1)
	unsigned int reg_vif_ch0_bt656_input_sel:1;

	// h0060, bit: 1
	/* BT656 time mux channel detect enable*/
	#define offset_of_reg_vif_ch0_bt656_ch_det_en (192)
	#define mask_of_reg_vif_ch0_bt656_ch_det_en (0x2)
	unsigned int reg_vif_ch0_bt656_ch_det_en:1;

	// h0060, bit: 2
	/* BT656 time mux channel select
	#[0]: EAV detect
	#[1]: SAV detect*/
	#define offset_of_reg_vif_ch0_bt656_ch_det_sel (192)
	#define mask_of_reg_vif_ch0_bt656_ch_det_sel (0x4)
	unsigned int reg_vif_ch0_bt656_ch_det_sel:1;

	// h0060, bit: 3
	/* BT656 bit swap function*/
	#define offset_of_reg_vif_ch0_bt656_bit_swap (192)
	#define mask_of_reg_vif_ch0_bt656_bit_swap (0x8)
	unsigned int reg_vif_ch0_bt656_bit_swap:1;

	// h0060, bit: 4
	/* BT656 8bit mode extend LSB bit*/
	#define offset_of_reg_vif_ch0_bt656_8bit_ext (192)
	#define mask_of_reg_vif_ch0_bt656_8bit_ext (0x10)
	unsigned int reg_vif_ch0_bt656_8bit_ext:1;

	// h0060, bit: 7
	/* */
	unsigned int :3;

	// h0060, bit: 8
	/* BT656 time mux channel*/
	#define offset_of_reg_vif_ch0_bt656_ch (192)
	#define mask_of_reg_vif_ch0_bt656_ch (0x100)
	unsigned int reg_vif_ch0_bt656_ch:1;

	// h0060, bit: 9
	/* BT656 time mux channel detect done*/
	#define offset_of_reg_vif_ch0_bt656_ch_det_done (192)
	#define mask_of_reg_vif_ch0_bt656_ch_det_done (0x200)
	unsigned int reg_vif_ch0_bt656_ch_det_done:1;

	// h0060, bit: 14
	/* */
	unsigned int :6;

	// h0060
	unsigned int /* padding 16 bit */:16;

	// h0061, bit: 1
	/* 0:  vsync is sync to hsync dely 1 line 1:  vsync is sync to hsync dely 2 line 2: vsync is sync to hsync dely 0 line 3:vsync is decoded by bt656 decoder*/
	#define offset_of_reg_vif_ch0_bt656_vs_sel (194)
	#define mask_of_reg_vif_ch0_bt656_vs_sel (0x3)
	unsigned int reg_vif_ch0_bt656_vs_sel:2;

	// h0061, bit: 2
	/* bt656 decoder hsync inv*/
	#define offset_of_reg_vif_ch0_bt656_hs_inv (194)
	#define mask_of_reg_vif_ch0_bt656_hs_inv (0x4)
	unsigned int reg_vif_ch0_bt656_hs_inv:1;

	// h0061, bit: 3
	/* bt656 decoder vsync inv*/
	#define offset_of_reg_vif_ch0_bt656_vs_inv (194)
	#define mask_of_reg_vif_ch0_bt656_vs_inv (0x8)
	unsigned int reg_vif_ch0_bt656_vs_inv:1;

	// h0061, bit: 6
	/* */
	unsigned int :3;

	// h0061, bit: 7
	/* bt656 crop enable*/
	#define offset_of_reg_vif_ch0_bt656_clamp_en (194)
	#define mask_of_reg_vif_ch0_bt656_clamp_en (0x80)
	unsigned int reg_vif_ch0_bt656_clamp_en:1;

	// h0061, bit: 13
	/* */
	unsigned int :6;

	// h0061, bit: 14
	/* Bt656 mode detection status 0*/
	#define offset_of_reg_vif_ch0_bt656_det_status0 (194)
	#define mask_of_reg_vif_ch0_bt656_det_status0 (0x4000)
	unsigned int reg_vif_ch0_bt656_det_status0:1;

	// h0061, bit: 15
	/* Bt656 mode detection status 1*/
	#define offset_of_reg_vif_ch0_bt656_det_status1 (194)
	#define mask_of_reg_vif_ch0_bt656_det_status1 (0x8000)
	unsigned int reg_vif_ch0_bt656_det_status1:1;

	// h0061
	unsigned int /* padding 16 bit */:16;

	// h0062, bit: 11
	/* bt656 vertical crop size V*/
	#define offset_of_reg_vif_ch0_bt656_crop_v (196)
	#define mask_of_reg_vif_ch0_bt656_crop_v (0xfff)
	unsigned int reg_vif_ch0_bt656_crop_v:12;

	// h0062, bit: 14
	/* */
	unsigned int :4;

	// h0062
	unsigned int /* padding 16 bit */:16;

	// h0063, bit: 11
	/* bt656 vertical crop size H*/
	#define offset_of_reg_vif_ch0_bt656_crop_h (198)
	#define mask_of_reg_vif_ch0_bt656_crop_h (0xfff)
	unsigned int reg_vif_ch0_bt656_crop_h:12;

	// h0063, bit: 14
	/* */
	unsigned int :4;

	// h0063
	unsigned int /* padding 16 bit */:16;

	// h0064, bit: 0
	/* BT656 input source select*/
	#define offset_of_reg_vif_ch1_bt656_input_sel (200)
	#define mask_of_reg_vif_ch1_bt656_input_sel (0x1)
	unsigned int reg_vif_ch1_bt656_input_sel:1;

	// h0064, bit: 1
	/* BT656 time mux channel detect enable*/
	#define offset_of_reg_vif_ch1_bt656_ch_det_en (200)
	#define mask_of_reg_vif_ch1_bt656_ch_det_en (0x2)
	unsigned int reg_vif_ch1_bt656_ch_det_en:1;

	// h0064, bit: 2
	/* BT656 time mux channel select
	#[0]: EAV detect
	#[1]: SAV detect*/
	#define offset_of_reg_vif_ch1_bt656_ch_det_sel (200)
	#define mask_of_reg_vif_ch1_bt656_ch_det_sel (0x4)
	unsigned int reg_vif_ch1_bt656_ch_det_sel:1;

	// h0064, bit: 3
	/* BT656 bit swap function*/
	#define offset_of_reg_vif_ch1_bt656_bit_swap (200)
	#define mask_of_reg_vif_ch1_bt656_bit_swap (0x8)
	unsigned int reg_vif_ch1_bt656_bit_swap:1;

	// h0064, bit: 4
	/* BT656 8bit mode extend LSB bit*/
	#define offset_of_reg_vif_ch1_bt656_8bit_ext (200)
	#define mask_of_reg_vif_ch1_bt656_8bit_ext (0x10)
	unsigned int reg_vif_ch1_bt656_8bit_ext:1;

	// h0064, bit: 7
	/* */
	unsigned int :3;

	// h0064, bit: 8
	/* BT656 phase 0 channel*/
	#define offset_of_reg_vif_ch1_bt656_ch (200)
	#define mask_of_reg_vif_ch1_bt656_ch (0x100)
	unsigned int reg_vif_ch1_bt656_ch:1;

	// h0064, bit: 9
	/* BT656 time mux channel detect done*/
	#define offset_of_reg_vif_ch1_bt656_ch_det_done (200)
	#define mask_of_reg_vif_ch1_bt656_ch_det_done (0x200)
	unsigned int reg_vif_ch1_bt656_ch_det_done:1;

	// h0064, bit: 14
	/* */
	unsigned int :6;

	// h0064
	unsigned int /* padding 16 bit */:16;

	// h0065, bit: 1
	/* 0:  vsync is sync to hsync dely 1 line 1:  vsync is sync to hsync dely 2 line 2: vsync is sync to hsync dely 0 line 3:vsync is decoded by bt656 decoder*/
	#define offset_of_reg_vif_ch1_bt656_vs_sel (202)
	#define mask_of_reg_vif_ch1_bt656_vs_sel (0x3)
	unsigned int reg_vif_ch1_bt656_vs_sel:2;

	// h0065, bit: 2
	/* bt656 decoder hsync inv*/
	#define offset_of_reg_vif_ch1_bt656_hs_inv (202)
	#define mask_of_reg_vif_ch1_bt656_hs_inv (0x4)
	unsigned int reg_vif_ch1_bt656_hs_inv:1;

	// h0065, bit: 3
	/* bt656 decoder vsync inv*/
	#define offset_of_reg_vif_ch1_bt656_vs_inv (202)
	#define mask_of_reg_vif_ch1_bt656_vs_inv (0x8)
	unsigned int reg_vif_ch1_bt656_vs_inv:1;

	// h0065, bit: 6
	/* */
	unsigned int :3;

	// h0065, bit: 7
	/* bt656 crop enable*/
	#define offset_of_reg_vif_ch1_bt656_clamp_en (202)
	#define mask_of_reg_vif_ch1_bt656_clamp_en (0x80)
	unsigned int reg_vif_ch1_bt656_clamp_en:1;

	// h0065, bit: 13
	/* */
	unsigned int :6;

	// h0065, bit: 14
	/* Bt656 mode detection status 0*/
	#define offset_of_reg_vif_ch1_bt656_det_status0 (202)
	#define mask_of_reg_vif_ch1_bt656_det_status0 (0x4000)
	unsigned int reg_vif_ch1_bt656_det_status0:1;

	// h0065, bit: 15
	/* Bt656 mode detection status 1*/
	#define offset_of_reg_vif_ch1_bt656_det_status1 (202)
	#define mask_of_reg_vif_ch1_bt656_det_status1 (0x8000)
	unsigned int reg_vif_ch1_bt656_det_status1:1;

	// h0065
	unsigned int /* padding 16 bit */:16;

	// h0066, bit: 11
	/* bt656 vertical crop size V*/
	#define offset_of_reg_vif_ch1_bt656_crop_v (204)
	#define mask_of_reg_vif_ch1_bt656_crop_v (0xfff)
	unsigned int reg_vif_ch1_bt656_crop_v:12;

	// h0066, bit: 14
	/* */
	unsigned int :4;

	// h0066
	unsigned int /* padding 16 bit */:16;

	// h0067, bit: 11
	/* bt656 vertical crop size H*/
	#define offset_of_reg_vif_ch1_bt656_crop_h (206)
	#define mask_of_reg_vif_ch1_bt656_crop_h (0xfff)
	unsigned int reg_vif_ch1_bt656_crop_h:12;

	// h0067, bit: 14
	/* */
	unsigned int :4;

	// h0067
	unsigned int /* padding 16 bit */:16;

	// h0068, bit: 0
	/* BT656 input source select*/
	#define offset_of_reg_vif_ch2_bt656_input_sel (208)
	#define mask_of_reg_vif_ch2_bt656_input_sel (0x1)
	unsigned int reg_vif_ch2_bt656_input_sel:1;

	// h0068, bit: 1
	/* BT656 time mux channel detect enable*/
	#define offset_of_reg_vif_ch2_bt656_ch_det_en (208)
	#define mask_of_reg_vif_ch2_bt656_ch_det_en (0x2)
	unsigned int reg_vif_ch2_bt656_ch_det_en:1;

	// h0068, bit: 2
	/* BT656 time mux channel select
	#[0]: EAV detect
	#[1]: SAV detect*/
	#define offset_of_reg_vif_ch2_bt656_ch_det_sel (208)
	#define mask_of_reg_vif_ch2_bt656_ch_det_sel (0x4)
	unsigned int reg_vif_ch2_bt656_ch_det_sel:1;

	// h0068, bit: 3
	/* BT656 bit swap function*/
	#define offset_of_reg_vif_ch2_bt656_bit_swap (208)
	#define mask_of_reg_vif_ch2_bt656_bit_swap (0x8)
	unsigned int reg_vif_ch2_bt656_bit_swap:1;

	// h0068, bit: 4
	/* BT656 8bit mode extend LSB bit*/
	#define offset_of_reg_vif_ch2_bt656_8bit_ext (208)
	#define mask_of_reg_vif_ch2_bt656_8bit_ext (0x10)
	unsigned int reg_vif_ch2_bt656_8bit_ext:1;

	// h0068, bit: 7
	/* */
	unsigned int :3;

	// h0068, bit: 8
	/* BT656 phase 0 channel*/
	#define offset_of_reg_vif_ch2_bt656_ch (208)
	#define mask_of_reg_vif_ch2_bt656_ch (0x100)
	unsigned int reg_vif_ch2_bt656_ch:1;

	// h0068, bit: 9
	/* BT656 time mux channel detect done*/
	#define offset_of_reg_vif_ch2_bt656_ch_det_done (208)
	#define mask_of_reg_vif_ch2_bt656_ch_det_done (0x200)
	unsigned int reg_vif_ch2_bt656_ch_det_done:1;

	// h0068, bit: 14
	/* */
	unsigned int :6;

	// h0068
	unsigned int /* padding 16 bit */:16;

	// h0069, bit: 1
	/* 0:  vsync is sync to hsync dely 1 line 1:  vsync is sync to hsync dely 2 line 2: vsync is sync to hsync dely 0 line 3:vsync is decoded by bt656 decoder*/
	#define offset_of_reg_vif_ch2_bt656_vs_sel (210)
	#define mask_of_reg_vif_ch2_bt656_vs_sel (0x3)
	unsigned int reg_vif_ch2_bt656_vs_sel:2;

	// h0069, bit: 2
	/* bt656 decoder hsync inv*/
	#define offset_of_reg_vif_ch2_bt656_hs_inv (210)
	#define mask_of_reg_vif_ch2_bt656_hs_inv (0x4)
	unsigned int reg_vif_ch2_bt656_hs_inv:1;

	// h0069, bit: 3
	/* bt656 decoder vsync inv*/
	#define offset_of_reg_vif_ch2_bt656_vs_inv (210)
	#define mask_of_reg_vif_ch2_bt656_vs_inv (0x8)
	unsigned int reg_vif_ch2_bt656_vs_inv:1;

	// h0069, bit: 6
	/* */
	unsigned int :3;

	// h0069, bit: 7
	/* bt656 crop enable*/
	#define offset_of_reg_vif_ch2_bt656_clamp_en (210)
	#define mask_of_reg_vif_ch2_bt656_clamp_en (0x80)
	unsigned int reg_vif_ch2_bt656_clamp_en:1;

	// h0069, bit: 13
	/* */
	unsigned int :6;

	// h0069, bit: 14
	/* Bt656 mode detection status 0*/
	#define offset_of_reg_vif_ch2_bt656_det_status0 (210)
	#define mask_of_reg_vif_ch2_bt656_det_status0 (0x4000)
	unsigned int reg_vif_ch2_bt656_det_status0:1;

	// h0069, bit: 15
	/* Bt656 mode detection status 1*/
	#define offset_of_reg_vif_ch2_bt656_det_status1 (210)
	#define mask_of_reg_vif_ch2_bt656_det_status1 (0x8000)
	unsigned int reg_vif_ch2_bt656_det_status1:1;

	// h0069
	unsigned int /* padding 16 bit */:16;

	// h006a, bit: 11
	/* bt656 vertical crop size V*/
	#define offset_of_reg_vif_ch2_bt656_crop_v (212)
	#define mask_of_reg_vif_ch2_bt656_crop_v (0xfff)
	unsigned int reg_vif_ch2_bt656_crop_v:12;

	// h006a, bit: 14
	/* */
	unsigned int :4;

	// h006a
	unsigned int /* padding 16 bit */:16;

	// h006b, bit: 11
	/* bt656 vertical crop size H*/
	#define offset_of_reg_vif_ch2_bt656_crop_h (214)
	#define mask_of_reg_vif_ch2_bt656_crop_h (0xfff)
	unsigned int reg_vif_ch2_bt656_crop_h:12;

	// h006b, bit: 14
	/* */
	unsigned int :4;

	// h006b
	unsigned int /* padding 16 bit */:16;

	// h006c, bit: 14
	/* */
	unsigned int :16;

	// h006c
	unsigned int /* padding 16 bit */:16;

	// h006d, bit: 14
	/* */
	unsigned int :16;

	// h006d
	unsigned int /* padding 16 bit */:16;

	// h006e, bit: 14
	/* */
	unsigned int :16;

	// h006e
	unsigned int /* padding 16 bit */:16;

	// h006f, bit: 14
	/* */
	unsigned int :16;

	// h006f
	unsigned int /* padding 16 bit */:16;

	// h0070, bit: 14
	/* */
	unsigned int :16;

	// h0070
	unsigned int /* padding 16 bit */:16;

	// h0071, bit: 14
	/* */
	unsigned int :16;

	// h0071
	unsigned int /* padding 16 bit */:16;

	// h0072, bit: 14
	/* */
	unsigned int :16;

	// h0072
	unsigned int /* padding 16 bit */:16;

	// h0073, bit: 14
	/* */
	unsigned int :16;

	// h0073
	unsigned int /* padding 16 bit */:16;

	// h0074, bit: 14
	/* */
	unsigned int :16;

	// h0074
	unsigned int /* padding 16 bit */:16;

	// h0075, bit: 14
	/* */
	unsigned int :16;

	// h0075
	unsigned int /* padding 16 bit */:16;

	// h0076, bit: 14
	/* */
	unsigned int :16;

	// h0076
	unsigned int /* padding 16 bit */:16;

	// h0077, bit: 14
	/* */
	unsigned int :16;

	// h0077
	unsigned int /* padding 16 bit */:16;

	// h0078, bit: 14
	/* */
	unsigned int :16;

	// h0078
	unsigned int /* padding 16 bit */:16;

	// h0079, bit: 14
	/* */
	unsigned int :16;

	// h0079
	unsigned int /* padding 16 bit */:16;

	// h007a, bit: 14
	/* */
	unsigned int :16;

	// h007a
	unsigned int /* padding 16 bit */:16;

	// h007b, bit: 14
	/* */
	unsigned int :16;

	// h007b
	unsigned int /* padding 16 bit */:16;

	// h007c, bit: 14
	/* */
	unsigned int :16;

	// h007c
	unsigned int /* padding 16 bit */:16;

	// h007d, bit: 14
	/* */
	unsigned int :16;

	// h007d
	unsigned int /* padding 16 bit */:16;

	// h007e, bit: 14
	/* */
	unsigned int :16;

	// h007e
	unsigned int /* padding 16 bit */:16;

	// h007f, bit: 14
	/* */
	unsigned int :16;

	// h007f
	unsigned int /* padding 16 bit */:16;

}  __attribute__((packed, aligned(1))) VIF_t;

extern volatile VIF_t * const g_ptVIF;

#endif
