/* Copyright (c) 2016-2018, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _CNSS_MAIN_H
#define _CNSS_MAIN_H
#include <asm/arch_timer.h>
#include <linux/esoc_client.h>
#include <linux/etherdevice.h>
#include <linux/msm-bus.h>
#include <linux/pm_qos.h>
#include <net/cnss2.h>
#include <soc/qcom/memory_dump.h>
#include <soc/qcom/subsystem_restart.h>

#include "qmi.h"
#include "bus.h"

#define MAX_NO_OF_MAC_ADDR		4
#define QMI_WLFW_MAX_TIMESTAMP_LEN	32
#define QMI_WLFW_MAX_NUM_MEM_SEG	32
#define CNSS_RDDM_TIMEOUT_MS		20000
#define RECOVERY_TIMEOUT		60000
#define TIME_CLOCK_FREQ_HZ		19200000

#define CNSS_EVENT_SYNC   BIT(0)
#define CNSS_EVENT_UNINTERRUPTIBLE BIT(1)
#define CNSS_EVENT_SYNC_UNINTERRUPTIBLE (CNSS_EVENT_SYNC | \
				CNSS_EVENT_UNINTERRUPTIBLE)

enum cnss_dev_bus_type {
	CNSS_BUS_NONE = -1,
	CNSS_BUS_PCI,
	CNSS_BUS_AHB
};

struct cnss_vreg_cfg {
	const char *name;
	u32 min_uv;
	u32 max_uv;
	u32 load_ua;
	u32 delay_us;
	u32 need_unvote;
};

struct cnss_vreg_info {
	struct list_head list;
	struct regulator *reg;
	struct cnss_vreg_cfg cfg;
	u32 enabled;
};

enum cnss_vreg_type {
	CNSS_VREG_PRIM,
};

struct cnss_clk_cfg {
	const char *name;
	u32 freq;
	u32 required;
};

struct cnss_clk_info {
	struct list_head list;
	struct clk *clk;
	struct cnss_clk_cfg cfg;
	u32 enabled;
};

struct cnss_pinctrl_info {
	struct pinctrl *pinctrl;
	struct pinctrl_state *bootstrap_active;
	struct pinctrl_state *wlan_en_active;
	struct pinctrl_state *wlan_en_sleep;
};

struct cnss_subsys_info {
	struct subsys_device *subsys_device;
	struct subsys_desc subsys_desc;
	void *subsys_handle;
	bool subsystem_put_in_progress;
};

struct cnss_ramdump_info {
	struct ramdump_device *ramdump_dev;
	unsigned long ramdump_size;
	void *ramdump_va;
	phys_addr_t ramdump_pa;
	struct msm_dump_data dump_data;
};

struct cnss_dump_seg {
	unsigned long address;
	void *v_address;
	unsigned long size;
	u32 type;
};

struct cnss_dump_data {
	u32 version;
	u32 magic;
	char name[32];
	phys_addr_t paddr;
	int nentries;
	u32 seg_version;
};

struct cnss_ramdump_info_v2 {
	struct ramdump_device *ramdump_dev;
	unsigned long ramdump_size;
	void *dump_data_vaddr;
	u8 dump_data_valid;
	struct cnss_dump_data dump_data;
};

struct cnss_esoc_info {
	struct esoc_desc *esoc_desc;
	u8 notify_modem_status;
	void *modem_notify_handler;
	int modem_current_status;
};

struct cnss_bus_bw_info {
	struct msm_bus_scale_pdata *bus_scale_table;
	u32 bus_client;
	int current_bw_vote;
};

struct cnss_wlan_mac_addr {
	u8 mac_addr[MAX_NO_OF_MAC_ADDR][ETH_ALEN];
	u32 no_of_mac_addr_set;
};

struct cnss_wlan_mac_info {
	struct cnss_wlan_mac_addr wlan_mac_addr;
	bool is_wlan_mac_set;
};

struct cnss_fw_mem {
	size_t size;
	void *va;
	phys_addr_t pa;
	u8 valid;
	u32 type;
};

struct wlfw_rf_chip_info {
	u32 chip_id;
	u32 chip_family;
};

struct wlfw_rf_board_info {
	u32 board_id;
	u32 board_id_override;
};

struct wlfw_soc_info {
	u32 soc_id;
};

struct wlfw_fw_version_info {
	u32 fw_version;
	char fw_build_timestamp[QMI_WLFW_MAX_TIMESTAMP_LEN + 1];
};

enum cnss_mem_type {
	CNSS_MEM_TYPE_MSA,
	CNSS_MEM_TYPE_DDR,
	CNSS_MEM_BDF,
	CNSS_MEM_M3,
	CNSS_MEM_CAL_V01,
	CNSS_MEM_DPD_V01,
};

enum cnss_fw_dump_type {
	CNSS_FW_IMAGE,
	CNSS_FW_RDDM,
	CNSS_FW_REMOTE_HEAP,
};

enum cnss_driver_event_type {
	CNSS_DRIVER_EVENT_SERVER_ARRIVE,
	CNSS_DRIVER_EVENT_SERVER_EXIT,
	CNSS_DRIVER_EVENT_REQUEST_MEM,
	CNSS_DRIVER_EVENT_FW_MEM_READY,
	CNSS_DRIVER_EVENT_FW_READY,
	CNSS_DRIVER_EVENT_COLD_BOOT_CAL_START,
	CNSS_DRIVER_EVENT_COLD_BOOT_CAL_DONE,
	CNSS_DRIVER_EVENT_REGISTER_DRIVER,
	CNSS_DRIVER_EVENT_UNREGISTER_DRIVER,
	CNSS_DRIVER_EVENT_RECOVERY,
	CNSS_DRIVER_EVENT_FORCE_FW_ASSERT,
	CNSS_DRIVER_EVENT_POWER_UP,
	CNSS_DRIVER_EVENT_POWER_DOWN,
	CNSS_DRIVER_EVENT_IDLE_RESTART,
	CNSS_DRIVER_EVENT_IDLE_SHUTDOWN,
	CNSS_DRIVER_EVENT_QDSS_TRACE_REQ_MEM,
	CNSS_DRIVER_EVENT_QDSS_TRACE_SAVE,
	CNSS_DRIVER_EVENT_QDSS_TRACE_FREE,
	CNSS_DRIVER_EVENT_M3_DUMP_UPLOAD_REQ,
	CNSS_DRIVER_EVENT_MAX,
};

enum cnss_driver_state {
	CNSS_QMI_WLFW_CONNECTED,
	CNSS_FW_MEM_READY,
	CNSS_FW_READY,
	CNSS_COLD_BOOT_CAL,
	CNSS_DRIVER_LOADING,
	CNSS_DRIVER_UNLOADING,
	CNSS_DRIVER_IDLE_RESTART,
	CNSS_DRIVER_IDLE_SHUTDOWN,
	CNSS_DRIVER_PROBED,
	CNSS_DRIVER_RECOVERY,
	CNSS_FW_BOOT_RECOVERY,
	CNSS_DEV_ERR_NOTIFY,
	CNSS_DRIVER_DEBUG,
	CNSS_COEX_CONNECTED,
	CNSS_IMS_CONNECTED,
	CNSS_IN_SUSPEND_RESUME,
};

struct cnss_recovery_data {
	enum cnss_recovery_reason reason;
};

enum cnss_pins {
	CNSS_WLAN_EN,
	CNSS_PCIE_TXP,
	CNSS_PCIE_TXN,
	CNSS_PCIE_RXP,
	CNSS_PCIE_RXN,
	CNSS_PCIE_REFCLKP,
	CNSS_PCIE_REFCLKN,
	CNSS_PCIE_RST,
	CNSS_PCIE_WAKE,
};

struct cnss_pin_connect_result {
	u32 fw_pwr_pin_result;
	u32 fw_phy_io_pin_result;
	u32 fw_rf_pin_result;
	u32 host_pin_result;
};

enum cnss_debug_quirks {
	LINK_DOWN_SELF_RECOVERY,
	SKIP_DEVICE_BOOT,
	USE_CORE_ONLY_FW,
	SKIP_RECOVERY,
	QMI_BYPASS,
	ENABLE_WALTEST,
	ENABLE_PCI_LINK_DOWN_PANIC,
	FBC_BYPASS,
	ENABLE_DAEMON_SUPPORT,
	DISABLE_DRV,
};

enum cnss_bdf_type {
	CNSS_BDF_BIN,
	CNSS_BDF_ELF,
	CNSS_BDF_REGDB = 4,
	CNSS_BDF_WIN,
	CNSS_CALDATA_WIN,
	CNSS_BDF_DUMMY = 255,
};

enum cnss_cal_status {
	CNSS_CAL_DONE,
	CNSS_CAL_TIMEOUT,
};

struct cnss_cal_info {
	enum cnss_cal_status cal_status;
};

struct cnss_control_params {
	unsigned long quirks;
	unsigned int mhi_timeout;
	unsigned int qmi_timeout;
	unsigned int bdf_type;
	unsigned int time_sync_period;
};

struct cnss_cpr_info {
	resource_size_t tcs_cmd_base_addr;
	resource_size_t tcs_cmd_data_addr;
	void __iomem *tcs_cmd_base_addr_io;
	void __iomem *tcs_cmd_data_addr_io;
	u32 cpr_pmic_addr;
	u32 voltage;
};

enum cnss_ce_index {
	CNSS_CE_00,
	CNSS_CE_01,
	CNSS_CE_02,
	CNSS_CE_03,
	CNSS_CE_04,
	CNSS_CE_05,
	CNSS_CE_06,
	CNSS_CE_07,
	CNSS_CE_08,
	CNSS_CE_09,
	CNSS_CE_10,
	CNSS_CE_11,
	CNSS_CE_COMMON,
};

struct cnss_plat_data {
	void *wlan_priv;
	struct platform_device *plat_dev;
	struct platform_device_id *plat_dev_id;
	void *pci_dev;
	void *pci_dev_id;
	void *bus_priv;
	int qrtr_node_id;
	char device_name[16];
	struct cnss_vreg_info *vreg_info;
	enum cnss_dev_bus_type bus_type;
	struct list_head vreg_list;
	struct list_head clk_list;
	struct cnss_pinctrl_info pinctrl_info;
	struct cnss_subsys_info subsys_info;
	bool ramdump_enabled;
	struct cnss_ramdump_info ramdump_info;
	struct cnss_ramdump_info_v2 ramdump_info_v2;
	struct cnss_esoc_info esoc_info;
	struct cnss_bus_bw_info bus_bw_info;
	struct notifier_block modem_nb;
	struct cnss_platform_cap cap;
	struct pm_qos_request qos_request;
	struct cnss_device_version device_version;
	unsigned long device_id;
	struct cnss_wlan_driver *driver_ops;
	enum cnss_driver_status driver_status;
	u32 recovery_count;
	unsigned long driver_state;
	struct list_head event_list;
	spinlock_t event_lock; /* spinlock for driver work event handling */
	struct work_struct event_work;
	struct workqueue_struct *event_wq;
	struct qmi_handle qmi_wlfw;
	struct sockaddr_qrtr sq;
	struct wlfw_rf_chip_info chip_info;
	struct wlfw_rf_board_info board_info;
	struct wlfw_soc_info soc_info;
	struct wlfw_fw_version_info fw_version_info;
	u32 otp_version;
	u32 fw_mem_seg_len;
	struct cnss_fw_mem fw_mem[QMI_WLFW_MAX_NUM_MEM_SEG];
	struct cnss_fw_mem m3_mem;
	u32 qdss_mem_seg_len;
	struct cnss_fw_mem qdss_mem[QMI_WLFW_MAX_NUM_MEM_SEG];
	int tgt_mem_cfg_mode;
	u32 *qdss_reg;
	struct cnss_pin_connect_result pin_result;
	struct dentry *root_dentry;
	atomic_t pm_count;
	struct timer_list fw_boot_timer;
	struct completion power_up_complete;
	unsigned int wlfw_service_instance_id;
	unsigned int service_id;
	struct notifier_block modem_atomic_nb;
	struct completion cal_complete;
	struct mutex dev_lock; /* mutex for register access through debugfs */
	u32 device_freq_hz;
	u32 diag_reg_read_addr;
	u32 diag_reg_read_mem_type;
	u32 diag_reg_read_len;
	u8 *diag_reg_read_buf;
	u8 cal_done;
	u8 powered_on;
	char firmware_name[17];
	struct completion rddm_complete;
	struct completion recovery_complete;
	struct cnss_control_params ctrl_params;
	struct cnss_cpr_info cpr_info;
	u64 antenna;
	u64 grant;
	struct qmi_handle coex_qmi;
	struct qmi_handle ims_qmi;
	struct qmi_txn txn;
	u64 dynamic_feature;
	u64 target_assert_timestamp;
	u8 target_asserted;
	u32 flashcal_support;
	u32 eeprom_caldata_read_timeout;
};

#ifdef CONFIG_ARCH_QCOM
static inline u64 cnss_get_host_timestamp(struct cnss_plat_data *plat_priv)
{
	u64 ticks = arch_counter_get_cntvct();

	do_div(ticks, TIME_CLOCK_FREQ_HZ / 100000);

	return ticks * 10;
}
#else
static inline u64 cnss_get_host_timestamp(struct cnss_plat_data *plat_priv)
{
	struct timespec ts;

	ktime_get_ts(&ts);

	return ((u64)ts.tv_sec * 1000000) + (ts.tv_nsec / 1000);
}
#endif

struct cnss_plat_data *cnss_get_plat_priv(struct platform_device *plat_dev);
int cnss_driver_event_post(struct cnss_plat_data *plat_priv,
			   enum cnss_driver_event_type type,
			   u32 flags, void *data);
int cnss_get_vreg(struct cnss_plat_data *plat_priv);
int cnss_get_vreg_type(struct cnss_plat_data *plat_priv,
		       enum cnss_vreg_type type);
void cnss_put_vreg_type(struct cnss_plat_data *plat_priv,
			enum cnss_vreg_type type);
int cnss_vreg_on_type(struct cnss_plat_data *plat_priv,
		      enum cnss_vreg_type type);
int cnss_vreg_off_type(struct cnss_plat_data *plat_priv,
		       enum cnss_vreg_type type);
int cnss_get_clk(struct cnss_plat_data *plat_priv);
void cnss_put_clk(struct cnss_plat_data *plat_priv);
int cnss_vreg_unvote_type(struct cnss_plat_data *plat_priv,
			  enum cnss_vreg_type type);
int cnss_get_pinctrl(struct cnss_plat_data *plat_priv);
int cnss_power_on_device(struct cnss_plat_data *plat_priv, int device_id);
int cnss_power_off_device(struct cnss_plat_data *plat_priv, int device_id);
bool cnss_is_device_powered_on(struct cnss_plat_data *plat_priv);
int cnss_register_subsys(struct cnss_plat_data *plat_priv);
void cnss_unregister_subsys(struct cnss_plat_data *plat_priv);
int cnss_register_ramdump(struct cnss_plat_data *plat_priv);
void cnss_unregister_ramdump(struct cnss_plat_data *plat_priv);
void cnss_set_pin_connect_status(struct cnss_plat_data *plat_priv);
u32 cnss_get_wake_msi(struct cnss_plat_data *plat_priv);
struct cnss_plat_data *cnss_get_plat_priv_by_device_id(int id);
struct cnss_plat_data *cnss_get_plat_priv_by_qrtr_node_id(int node_id);
struct cnss_plat_data *cnss_get_plat_priv(struct platform_device *plat_dev);
int cnss_qca9000_shutdown_part2(struct cnss_plat_data *plat_priv);
int cnss_get_cpr_info(struct cnss_plat_data *plat_priv);
int cnss_update_cpr_info(struct cnss_plat_data *plat_priv);

#endif /* _CNSS_MAIN_H */
