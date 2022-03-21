/**
 * 2020-2030 YOUTRANSACTOR ALL RIGHTS RESERVED. YouTransactor,
* 32, rue Brancion 75015 Paris France, RCS PARIS: B 491 208 500, YOUTRANSACTOR
* CONFIDENTIAL AND PROPRIETARY INFORMATION , CONTROLLED DISTRIBUTION ONLY,
* THEREFORE UNDER NDA ONLY. YOUTRANSACTOR Authorized Parties and who have
* signed NDA do not have permission to distribute this documentation further.
* Unauthorized recipients must destroy any electronic and hard-copies
* and without reading and notify Gregory Mardinian, CTO, YOUTRANSACTOR
* immediately at gregory.mardinian@youtransactor.com.
*
* @file: network_manager.h
*
* @date: May 2021
*
* @author: ktahri <khawla.tahri@ext.youtransactor.com>
*/

/**
 * @addtogroup app_network_mng
 * @{
 */

#ifndef _NETWORK_MANAGER__
#define _NETWORK_MANAGER__

#include "network.h"

#define NB_APN_LIST     4
#define APN_ORANGE      "orange"
#define APN_BOUYGUES    "mmsbouygtel.com"
#define APN_LYCA        "data.lycamobile.fr"
#define APN_CORIOLIS    "coriolis"

#define WIFI_SSID       "YouTransactor"
#define WIFI_PASSWORD   "wifiYT32"

error_t connect_interface_cellular(network_handler_t * network_handler);
error_t disconnect_interface_cellular(network_handler_t * network_handler);
error_t init_http_request(socket_handler_t * socket_handler);
void connect_network(void);
void test_san_wifi(void);
void test_get_rss_id(void);

uint8_t disconnect_wifi_access_pnt(void);
uint8_t connect_wifi_access_pnt(void);
error_t connect_interface_wifi(network_handler_t * network_handler);
error_t disconnect_interface_wifi(network_handler_t * network_handler);
error_t config_cellular_network(network_handler_t * network_handler);
error_t init_paris_temperature_request(socket_handler_t * socket_handler);
void init_mdm_request(void);
void send_wifi_ntw(socket_handler_t * socket_handler);
#endif //_NETWORK_MANAGER__

/* @} */
