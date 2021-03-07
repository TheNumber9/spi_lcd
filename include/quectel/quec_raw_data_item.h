
/*==========================================================================
 |							QUECTEL - Build a smart world.
 |
 |							Copyright(c) 2017 QUECTEL Incorporated.
 |
 |--------------------------------------------------------------------------
 | File Description
 | ----------------
 |			quectel raw data item
 |
 |--------------------------------------------------------------------------
 |
 |	Designed by 		: 	will.shao
 |--------------------------------------------------------------------------
 | Revision History
 | ----------------
 |	2018/05/04				will.shao 		 Initialization
 |	------------------------------------------------------------------------
 \=========================================================================*/

#ifndef _QUEC_RAW_DATA_ITEM_H_
#define _QUEC_RAW_DATA_ITEM_H_
/*===========================================================================
                         EDIT HISTORY FOR MODULE
This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.
when       who      what, where, why
--------   ---      ---------------------------------------------------------
02/01/2019	duke		MBIM  Switch   SBL  usb DM port  Just for EM12G  Add raw_data pid value
===========================================================================*/
#include "quectel-features-config.h"  //quectel add

//#include <quectel/ql_types.h>


#define RAW_FLASH_DATA_BLOCK_NUMS 			(5)
#define QUECTEL_RAW_DATA_PARTITION_NAME ("rawdata")

// RAW DATA ID
typedef enum
{
	RAW_DATA_USB_CFG,
	RAW_DATA_DOWNLOAD_INFO_ID,
	/* Refer to [Req-Depot].[RQ0000236][Submitter:duke.xin,Date:2018-12-28]*/
	RAW_DATA_UPDATE_PID_ID,
	RAW_DATA_USB_SPEED_ID, //2019.06.27 add by jacen.huang add usb speed id for at+qusbcfg command
	RAW_DATA_MAX_ID
} quec_raw_data_id;

#define QUEC_RAW_USB_MANUF_DATA_LEN     32
#define QUEC_RAW_USB_PRODUCT_DATA_LEN   32
typedef enum
{
	USB_FUNC_DIAG,
	USB_FUNC_NMEA,
	USB_FUNC_AT,
	USB_FUNC_MODEM,
	USB_FUNC_RMNET,
	USB_FUNC_ADB,
	USB_FUNC_MAX
} ql_usb_func_enum;

typedef enum
{
	USB_NET_RMNET,
	USB_NET_ECM,
	USB_NET_MBIM,
	USB_NET_RNDIS,
	USB_NET_MAX,
} ql_usb_net_enum;

typedef struct
{
    int magic;
	int vid;
	int pid;
	char usb_manuf_info[QUEC_RAW_USB_MANUF_DATA_LEN];
	char usb_product_info[QUEC_RAW_USB_PRODUCT_DATA_LEN];
} ql_usb_cfg_type;

typedef enum
{
	BOOT_MODE_DOWNLOAD,
	BOOT_MODE_BACKUP_SBL,
	BOOT_MODE_NORMAL,
} boot_mode_type;

typedef struct 
{
	unsigned int boot_mode;
	unsigned int port;
	unsigned int baudrate;
	unsigned int fota;
} quec_download_info_type;

#define PARTITION_NAME_LEN_MAX              (40)
#define PARTITION_RECOVERY_MAX              (10)
#define PARTITION_RECOVERY_WHERE_MAX        (12)

typedef struct
{
	char partition[PARTITION_NAME_LEN_MAX];
	int where;
	int recovery_retry;
} quec_recovery_info_type;

typedef struct
{
	struct 
	{
		unsigned short restore_times[PARTITION_RECOVERY_WHERE_MAX];
	} partition[PARTITION_RECOVERY_MAX];
	
} quec_recovery_history_type;

#define CRASH_MODE_DOWNLOAD     (0)
#define CRASH_MODE_REBOOT       (1)

typedef struct 
{
	unsigned int modem;
	unsigned int ap;
} ql_crash_mode_type;

/* Refer to [Req-Depot].[RQ0000236][Submitter:duke.xin,Date:2018-12-28]*/
typedef struct 
{
  unsigned int pid;
  unsigned int reserved;
} ql_update_pid_type;


//2019.06.27 add by jacen.huang  add usb speed type for at+qusbcfg command
typedef struct
{
   unsigned char  value;
}ql_usb_speed_type;

#endif //_QUEC_RAW_DATA_ITEM_H_
