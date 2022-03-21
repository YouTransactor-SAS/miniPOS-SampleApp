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
* @file: menu.h
*
* @date: March 2021
*
* @author: Khawla TAHRI <khawla.tahri@ext.youtransactor.com>
*/

/**
 * @addtogroup app_menu_definition
 * @{
 */

#ifndef __MENU__
#define __MENU__

#include <stdint.h>
#include "errors.h"

#define MENU_SELECT_PAY                 1
#define MENU_SELECT_SET_GET_INFO        2
#define MENU_SELECT_SET_PWR_OFF_TIMEOUT 3
#define MENU_SELECT_COMMUNICATION_MENU  4
#define MENU_SELECT_SEND_DATA           6
#define MENU_SELECT_DISPLAY             5
#define MENU_SELECT_DISPLAY_LIST_BOX    8
#define MENU_SELECT_DISPLAY_MENU        7

/**
 * @brief    start display menu and select function
 * @return    None
 */
error_t menu_start(uint8_t *selected_menu_item);

#endif //__MENU__

/* @} */
