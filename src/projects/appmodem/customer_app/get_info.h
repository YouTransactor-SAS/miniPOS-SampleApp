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
* @file: get_info.h
*
* @date: Mars 2021
*
* @author: Khawla TAHRI <khawla.tahri@ext.youtransactor.com>
*/

/**
 * @addtogroup app_get_info
 * @{
 */

#ifndef __GET_INFO__
#define __GET_INFO__
#include "errors.h"
#include "types.h"

/**
 * @brief    send getinfo
 * @return    error
 */
error_t get_info(void);

/*
 * @brief   get info select response
 * @return  error
 */
error_t get_info_select();
/*
 * @brief   get svpp version
 * @return  error
 */
/*
 * @brief     get svpp version
 * @return    error
 */
error_t payment_svpp_get_info(uint8_t* tag_);
#endif //__GET_INFO__

/* @} */
