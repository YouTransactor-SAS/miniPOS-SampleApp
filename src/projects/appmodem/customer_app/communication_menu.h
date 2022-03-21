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
* @file: communication_menu.h
*
* @date: August 2021
*
* @author: Khawla TAHRI <khawla.tahri@ext.youtransactor.com>
*/

/**
 * @addtogroup app_communication_menu
 * @{
 */

#ifndef __COMMUNICATION_MENU__
#define __COMMUNICATION_MENU__
#include "errors.h"
#include "types.h"

#define COMMUNICATION_DEVICE_4G   1
#define COMMUNICATION_DEVICE_WIFI 2

#define  MENU_COMMUNICATION_CONNECTION_STATUS                   1
#define  MENU_COMMUNICATION_PREFFERED_COMMUNICATION_TYPE        2
#define  MENU_COMMUNICATION_CELLULAR                            3
#define  MENU_COMMUNICATION_WIFI                                4
#define  MENU_COMMUNICATION_PARIS_TEMPERATURE_REQUEST           5
#define  MENU_COMMUNICATION_MDM_REQUEST                         6

#define  MENU_PREFERRED_COMMUNICATION_WIFI                      1
#define  MENU_PREFERRED_COMMUNICATION_CELLULAR                  2

#define  MENU_CELLULAR_SELECTED_ACTION_CONFIG                   2
#define  MENU_CELLULAR_SELECTED_ACTION_DISCONNECT               0
#define  MENU_CELLULAR_SELECTED_ACTION_CONNECT                  1

#define  MENU_WIFI_SELECTED_ACTION_CONNECT                      1
#define  MENU_WIFI_SELECTED_ACTION_DISCONNECT                   0
#define  MENU_WIFI_ACCESS_POINT_CONFIG                          2

#define ENTRY_CONFIG_4G_APN         "config 4G APN"
#define ENTRY_CONFIG_WIFI_SAP       "config Wi-Fi SAP"
#define ENTRY_CONNECT               "connect"
#define ENTRY_DISCONNECT            "disconnect"

error_t run_communication_menu(void);
/**
 * @brief    run list box menu
 * @return    error
 */
error_t run_list_box_menu(void);
#endif //__COMMUNICATION_MENU__

/* @} */

