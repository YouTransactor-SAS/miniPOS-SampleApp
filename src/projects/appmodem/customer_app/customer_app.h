
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
* @file: customer_app.h
*
* @date: June 2020
*
* @author: schhim <stephane.chhim@youtransactor.com>
*/

/**
 * @addtogroup app_api
 * @{
 */

#ifndef __CUSTOMER_APP_H__
#define __CUSTOMER_APP_H__

#include "os_itf.h"
#include "svpp_com.h"

typedef struct
{
    os_itf_task_param_t task;
    os_itf_queue_t queue;
    os_itf_queue_msg_t msg;
    uint8_t buffer_in[SVPP_COM_MAX_RPC_PAYLOAD_SIZE];
    uint8_t buffer_out[SVPP_COM_MAX_RPC_PAYLOAD_SIZE];
    uint16_t size_in;
    uint16_t size_out;
    error_t rpc_err;
    com_rpc_ctx_t rpc_ctx;
    com_transaction_param_u param;
} customer_app_ctx_t;

error_t customer_app_init(void);
error_t customerAppSelectOption(void);

#endif // __CUSTOMER_APP_H__

/* @} */
