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
* @file: display.h
*
* @date: July 2021
*
* @author: Khawla TAHRI <khawla.tahri@ext.youtransactor.com>
*/

/**
 * @addtogroup app_display_menu
 * @{
 */

#ifndef __ENTER_AMOUNT_H__
#define __ENTER_AMOUNT_H__
#include "errors.h"
#include "types.h"


#define  MENU_DISPLAY_MESSAGE_SCREEN        1
#define  MENU_DISPLAY_LOADER_ERROR          2
#define  MENU_DISPLAY_LOADER_FULL           3
#define  MENU_DISPLAY_LOADER_SCREEN         4

#define  DISPLAY_LOADER_FULL_VIEW_INDEX      14
#define  DISPLAY_LOADER_ERROR_VIEW_INDEX     15
#define  DISPLAY_LOADER_SCREEN_VIEW_INDEX    16
#define  DISPLAY_MESSAGE_SCREEN_VIEW_INDEX   17

/**
 * @brief    run enter amount menu
 * @return    error
 */
error_t run_display_menu(void);

#endif //__ENTER_AMOUNT_H__

/* @} */

