/*==========================================================================
 |							QUECTEL - Build a smart world.
 |
 |							Copyright(c) 2017 QUECTEL Incorporated.
 |
 |--------------------------------------------------------------------------
 | File Description
 | ----------------
 |			quectel raw data 
 |
 |--------------------------------------------------------------------------
 |
 |	Designed by 		: 	will.shao
 |--------------------------------------------------------------------------
 | Revision History
 | ----------------
 |	2018/05/04				will.shao 		 Initialization
        2019/06/27                              jacen.huang             add include head file ql_types.h
 |	------------------------------------------------------------------------
 \=========================================================================*/


#ifndef _QUEC_RAW_FLASH_DATA_H
#define _QUEC_RAW_FLASH_DATA_H

#include <quectel/quec_raw_data_item.h>
#include <quectel/ql_types.h>  //2019.06.27 add by jacen.huang add head file

boolean quec_raw_flash_data_open(void);
boolean quec_raw_flash_data_close(void);

boolean quec_raw_flash_data_write_item(quec_raw_data_id data_id, void* data);
boolean quec_raw_flash_data_read_item(quec_raw_data_id data_id, void* data);
boolean quec_raw_flash_data_sync(void);

#endif //_QUEC_RAW_FLASH_DATA_H

