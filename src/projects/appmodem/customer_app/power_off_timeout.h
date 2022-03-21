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
* @file: set_poweroff_timeout.h
*
* @date: Mars 2021
*
* @author: Khawla TAHRI <khawla.tahri@ext.youtransactor.com>
*/


#ifndef __POWER_OFF_TIMEOUT__
#define __POWER_OFF_TIMEOUT__
#include "errors.h"
#include "types.h"

#define SVPP_COM_SET_INFO_TIMEOUT_32_S      32
#define SVPP_COM_SET_INFO_TIMEOUT_60_S      60
#define SVPP_COM_SET_INFO_TIMEOUT_120_S     120

/*
 * @brief   Set the power off timeout value
 *          Possible value : [32 - 255]
 *          Value "0" : infinite power off timeout (no automatic power off)
 * @return    error
 */
error_t power_off_timeout_set_value(uint8_t timeout);

/**
 * @brief       get power off timeout value from user
 * @return    error
 */
error_t power_off_timout_get_value_from_user(uint8_t* timeout);

/**
 * @brief    set power off timeout from the menu
 * @return    error
 */
error_t power_off_timeout_set_from_menu(void);

#endif //__POWER_OFF_TIMEOUT__
