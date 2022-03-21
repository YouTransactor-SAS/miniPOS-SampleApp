/**
* 2006-2018 YOUTRANSACTOR ALL RIGHTS RESERVED. YouTransactor,
* 32, rue Brancion 75015 Paris France, RCS PARIS: B 491 208 500, YOUTRANSACTOR
* CONFIDENTIAL AND PROPRIETARY INFORMATION , CONTROLLED DISTRIBUTION ONLY,
* THEREFORE UNDER NDA ONLY. YOUTRANSACTOR Authorized Parties and who have
* signed NDA do not have permission to distribute this documentation further.
* Unauthorized recipients must destroy any electronic and hard-copies
* and without reading and notify Gregory Mardinian, CTO, YOUTRANSACTOR
* immediately at gregory.mardinian@youtransactor.com.
*
* @file: firmware_update.h
*
* @date: June 2021
*
* @author: ktahri <khawla.tahri@ext.youtransactor.com>
*/

/**
 * @addtogroup app_fw_update
 * @{
 */

#ifndef _FIRMWARE_UPDATE_CT__
#define _FIRMWARE_UPDATE_CT__

#include <stdint.h>
#include "errors.h"
#include "firmware_update.h"

#define FIRMWARE_UPDATE_PATH              "/datatx/oem_app_path.ini"
#define FIRMWARE_PATH_NAME_MAX_LEN        100
#define FIRMWARE_UPDATE_NAME              "quectel_application_v1.0.0.4.bin"
#define PATH_FILE_NAME_MAX_LEN            100


error_t firmware_update_init(firmware_update_handle_t * firm_upd_handle);
#endif //_FIRMWARE_UPDATE__

/* @} */
