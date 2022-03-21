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
* @file: display_message.h
*
* @date: Mars 2021
*
* @author: Khawla TAHRI <khawla.tahri@ext.youtransactor.com>
*/

/**
 * @addtogroup app_display_msg
 * @{
 */

#ifndef __DISPLAY_MESSAGE__
#define __DISPLAY_MESSAGE__
#include "errors.h"
#include "types.h"

/**
 * @brief    display message
 * @return    error
 */
error_t display_message(uint8_t *msg, uint8_t size, uint8_t timeout, uint8_t x,
         uint8_t y);
void display_message_menu_item(void);
#endif //__DISPLAY_MESSAGE__

/* @} */

