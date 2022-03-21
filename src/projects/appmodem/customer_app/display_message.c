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
* @file: display_message.c
*
* @date: Mars 2021
*
* @author: Khawla TAHRI <khawla.tahri@ext.youtransactor.com>
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "config.h"
#include "util_lib.h"
#include "common.h"
#include "svpp_com.h"
#include "yt_atmel_test_lib_itf.h"
#include "display_message.h"
#include "customer_app.h"
#include "payment.h"

extern uint8_t g_u08MenuProgress;
extern customer_app_ctx_t customer_app_ctx;

/**
 * @brief    display message
 * @return    error
 */

void display_message_menu_item(void)
{
#ifdef APP_MODEM_STAND_ALONE
    g_u08MenuProgress = 1;
#endif // APP_MODEM_STAND_ALONE
    char  msg1[] = "This is a message example";
    display_message((uint8_t*)&msg1, sizeof(msg1),0xFF, 0xFF,50);

#ifdef APP_MODEM_STAND_ALONE
    g_u08MenuProgress = 0;
#endif // APP_MODEM_STAND_ALONE
}
error_t display_message(uint8_t *msg, uint8_t size, uint8_t timeout,
         uint8_t x,
         uint8_t y)
{
    error_t err = ERRORNO;
    svpp_com_line_desciption_t line;

    customer_app_ctx.rpc_ctx.buffer_in = customer_app_ctx.buffer_in;
    customer_app_ctx.rpc_ctx.buffer_out = customer_app_ctx.buffer_out;
    customer_app_ctx.rpc_ctx.size_in = &customer_app_ctx.size_in;
    customer_app_ctx.rpc_ctx.size_out = &customer_app_ctx.size_out;
    customer_app_ctx.rpc_ctx.queue = customer_app_ctx.queue;
    customer_app_ctx.rpc_ctx.param = &customer_app_ctx.param;
    customer_app_ctx.rpc_ctx.pErr = &customer_app_ctx.rpc_err;
    memset((uint8_t*)&customer_app_ctx.rpc_ctx.param->hmi_drv_displaymessagewithoutki.
            tx_msg, 0,
            sizeof(svpp_com_tx_hmi_drv_displaymessagewithoutki_t));
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaymessagewithoutki.tx_msg.
    timeout = timeout ; // 0xFF;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaymessagewithoutki.tx_msg.
    number_of_lines_to_display = 1;
    line.font = 1;
    line.len = size; //sizeof(msg);
    line.pText = msg;
    line.x_coord = x; //0xFF;
    line.y_coord = y; //50;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaymessagewithoutki.tx_msg.pLine =
            &line;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaymessagewithoutki.tx_msg.
    abort_key = SVPP_COM_ABORT_WITH_OK_KEY;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaymessagewithoutki.tx_msg.
    clear_display_config = ((1<<SVPP_COM_YES_NO_PROMPT) |SVPP_COM_CLEAR_DISPLAY);
    err = svpp_com_send_rpc(&customer_app_ctx.rpc_ctx,
            SVPP_COM_HMI_DRV_DISPLAYMESSAGEWITHOUTKI_CMD_NB,
            SVPP_COM_RPC_DEFAULT_TIMEOUT);

    return err;
}
