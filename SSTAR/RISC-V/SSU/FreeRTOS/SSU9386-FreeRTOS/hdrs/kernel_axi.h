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

#ifndef __KERNEL_AXI_H__
#define __KERNEL_AXI_H__

/************************************************/
/* AXI interface registers definition           */
/************************************************/

typedef struct KeInfinityAxi_s
{
  /* [0x00] */
  u16 reg_uncache_no_pack_miu0_en               :1;
  u16 reg_uncache_no_pack_imi_en                :1;
  u16 reg_w_pack_always_no_hit_miu0             :1;
  u16 reg_w_pack_always_no_hit_imi	            :1;
  u16 reg_w_always_flush_miu0                   :1;
  u16 reg_w_always_flush_imi                    :1;
  u16 reg_miu_access_mode                       :1;
  u16                                           :1;
  u16 reg_l3_dynamic_gat_en                     :1;
  u16 reg_l3_clk_latency                        :2;
  u16                                           :5;
  u16                                           :16;
  /* [0x04] */
  u16 reg_mcu_req_max_miu0                      :7;
  u16 reg_clk_miu2x_sel                         :1;
  u16 reg_mcu_req_max_imi                       :7;
  u16 reg_wriu_32b_en                           :1;
  u16                                           :16;
  /* [0x08] */
  u16 reg_mcu_req_prior_miu0                    :6;
  u16 reg_clk_miu2x_en_switch                   :1;
  u16 reg_clk_miu1x_en_switch                   :1;
  u16 reg_mcu_req_prior_imi                     :6;
  u16 reg_clk_miu2x_switch_bypass_waiting       :1;
  u16                                           :1;
  u16                                           :16;
  /* [0x0C] */
  u16 reg_w_pack_timeout                        :16;
  u16                                           :16;
  /* [0x10] */
  u16 reg_dummy_44                              :16;
  u16                                           :16;
  /* [0x14] */
  u16 reg_flush_op_on_fire                      :1;
  u16 reg_dummy_45                              :15;
  u16                                           :16;
  /* [0x18] */
  u16 reg_timeout_cnt                           :16;
  u16                                           :16;
  /* [0x1C] */
  u16 reg_status_clear                          :1;
  u16                                           :3;
  u16 reg_ro_resp_flag_ms0                      :2;
  u16 reg_ro_resp_flag_ms1                      :2;
  u16 reg_req_wait_cyc                          :4;
  u16 reg_acp_req_mask                          :1;
  u16                                           :3;
  u16                                           :16;
  /* [0x20] */
  u16 reg_debug_port_out_l                      :16;
  u16                                           :16;
  /* [0x24] */
  u16 reg_debug_port_out_h                      :8;
  u16                                           :5;
  u16 reg_debug_port_sel                        :3;
  u16                                           :16;
  /* [0x28] */
  u16 reg_miu_8x_2x_mode                        :2;
  u16                                           :14;
  u16                                           :16;
  /* [0x2C] */
  u16 reg_testout_sel                           :2;
  u16 reg_ca9miu_strb_inv                       :1;
  u16                                           :13;
  u16                                           :16;
  /* [0x30] */
  u16 reg_ecb_testout_l                         :16;
  u16                                           :16;
  /* [0x34] */
  u16 reg_ecb_testout_h                         :8;
  u16                                           :8;
  u16                                           :16;
  /* [0x38] */
  u16 reg_acp_pack_timeout                      :4;
  u16                                           :4;
  u16 reg_acp_idle                              :1;
  u16                                           :7;
  u16                                           :16;
  /* [0x3C] */
  u16 reg_testbus_en                            :1;
  u16 reg_ckg_alldft                            :1;
  u16                                           :6;
  u16 reg_ro_miucmd_come_before_set_miu2x       :1;
  u16 reg_ro_clk_miu2x_state                    :2;
  u16                                           :5;
  u16                                           :16;
  /* [0x40] */
  u16 reg_dummy50_ro                            :16;
  u16                                           :16;
} KeInfinityAxi_t;

/**
 * @brief Register 10h
 */
#define L3_FLUSH_STATUS  (1<<12) ///< Check Status of Flush Pipe Finish

extern volatile KeInfinityAxi_t* const g_ptKeInfinityAxi;

#endif /*__KERNEL_AXI_H__*/
