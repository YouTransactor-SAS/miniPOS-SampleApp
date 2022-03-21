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
* @file: display_list_box.h
*
* @date: Juin 2021
*
* @author: Khawla TAHRI <khawla.tahri@ext.youtransactor.com>
*/

/**
 * @addtogroup app_list_box_menu
 * @{
 */

#ifndef __MENU_LIST_BOX__
#define __MENU_LIST_BOX__
#include "errors.h"
#include "types.h"


#define  MENU_LIST_BOX_LAYOUT_S               1
#define  MENU_LIST_BOX_LAYOUT_M               2
#define  MENU_LIST_BOX_LAYOUT_L               3

#define  LIST_BOX_VIEW_INDEX                  11


/**
 * @brief    run list box menu
 * @return    error
 */
error_t run_list_box_menu(void);
#endif //__MENU_LIST_BOX__

/* @} */

