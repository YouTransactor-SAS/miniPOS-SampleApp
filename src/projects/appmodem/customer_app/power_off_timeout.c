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
* @file: set_poweroff_timeout.c
*
* @date: Mars 2021
*
* @author: Khawla TAHRI <khawla.tahri@ext.youtransactor.com>
*/

#include "config.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "util_lib.h"
#include "common.h"
#include "svpp_com.h"
#include "yt_atmel_test_lib_itf.h"
#include "payment.h"
#include "customer_app.h"
#include "power_off_timeout.h"

extern customer_app_ctx_t customer_app_ctx;
extern uint8_t g_u08MenuProgress;

/*
 * @brief   Set the power off timeout value
 *          Possible value in seconds: [32 - 255]
 *          Value "0" : infinite power off timeout (no automatic power off)
 * @return    error
 */
error_t power_off_timeout_set_value(uint8_t timeout)
{
    error_t err = ERRORNO;

    if ((timeout == 0) || (timeout >= 32))
    {
        uint16_t len = 0;

        // set rpc context
        customer_app_ctx.rpc_ctx.buffer_in = customer_app_ctx.buffer_in;
        customer_app_ctx.rpc_ctx.buffer_out = customer_app_ctx.buffer_out;
        customer_app_ctx.rpc_ctx.size_in = &customer_app_ctx.size_in;
        customer_app_ctx.rpc_ctx.size_out = &customer_app_ctx.size_out;
        customer_app_ctx.rpc_ctx.queue = customer_app_ctx.queue;
        customer_app_ctx.rpc_ctx.param = &customer_app_ctx.param;
        customer_app_ctx.rpc_ctx.pErr = &customer_app_ctx.rpc_err;
        // set value
        customer_app_ctx.rpc_ctx.buffer_in[len++] =
                                                SVPP_COM_TAG_INFO_POW_TIMEOUT;
        customer_app_ctx.rpc_ctx.buffer_in[len++] = sizeof(timeout);
        customer_app_ctx.rpc_ctx.buffer_in[len++] = timeout;
        customer_app_ctx.rpc_ctx.param->
                                    ident_setinfofield.tx_msg.pData =
                                                    customer_app_ctx.buffer_in;
        customer_app_ctx.rpc_ctx.param->
                                    ident_setinfofield.tx_msg.len_data = len;

        err = svpp_com_send_rpc(&customer_app_ctx.rpc_ctx,
                SVPP_COM_IDENT_SETINFOFIELD_CMD_NB,
                SVPP_COM_RPC_DEFAULT_TIMEOUT);
    }
    else
    {
        err = - EBADPARAM;
    }

    return err;
}

/** set power off timeout from the menu
 * @return    error
 */
error_t power_off_timeout_set_from_menu(void)
{
    error_t err = ERRORNO;
    uint8_t timeout_option = 0;
    uint8_t timeout_converted = 0;
#ifdef APP_MODEM_STAND_ALONE
    g_u08MenuProgress = 1;
#endif // APP_MODEM_STAND_ALONE
    customer_app_ctx.rpc_ctx.buffer_in = customer_app_ctx.buffer_in;
    customer_app_ctx.rpc_ctx.buffer_out = customer_app_ctx.buffer_out;
    customer_app_ctx.rpc_ctx.size_in = &customer_app_ctx.size_in;
    customer_app_ctx.rpc_ctx.size_out = &customer_app_ctx.size_out;
    customer_app_ctx.rpc_ctx.queue = customer_app_ctx.queue;
    customer_app_ctx.rpc_ctx.param = &customer_app_ctx.param;
    customer_app_ctx.rpc_ctx.pErr = &customer_app_ctx.rpc_err;
    err = ERRORNO;
    customer_app_ctx.rpc_err = err;
    err = power_off_timout_get_value_from_user(&timeout_option);
    switch (timeout_option)
    {
    case 1:
        timeout_converted = SVPP_COM_SET_INFO_TIMEOUT_32_S;
        break;
    case 2:
        timeout_converted = SVPP_COM_SET_INFO_TIMEOUT_60_S;
        break;
    case 3:
        timeout_converted = SVPP_COM_SET_INFO_TIMEOUT_120_S;
        break;
    default:
        break;
    }

    err = power_off_timeout_set_value(timeout_converted);
#ifdef APP_MODEM_STAND_ALONE
    g_u08MenuProgress = 0;
#endif // APP_MODEM_STAND_ALONE
    return err;
}

/**
 * @brief       get power off timeout value from user
 * @return    error
 */
error_t power_off_timout_get_value_from_user(uint8_t* timeout)
{
    uint8_t list[3 * SVPP_COM_APP_NAME_LEN + 10];
    uint8_t index= 0;
    uint8_t i=0;;
    error_t err=ERRORNO;
    uint8_t selected_tag=0;
    svpp_com_line_desciption_t line;
    memset((uint8_t*)&customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
            tx_msg, 0,
            sizeof(svpp_com_tx_hmi_drv_displaylistboxwithoutki_t));
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    timeout = 90;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.font = 1;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    title_length =  1;

    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"32s");
    index += strlen((char*)"32s");
    list[index++] = '\n';
    list[index++] = 0x00;

    i++;
    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"60s");
    index += strlen((char*)"60s");
    list[index++] = '\n';
    list[index++] = 0x00;
    i++;
    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"120s");
    index += strlen((char*)"120s");
    list[index++] = '\n';
    list[index++] = 0x00;

    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.pList =
            list;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.list_length =
            index;

    err = svpp_com_send_rpc(&customer_app_ctx.rpc_ctx,
            SVPP_COM_HMI_DRV_DISPLAYLISTBOXWITHOUTKI_CMD_NB,
            SVPP_COM_RPC_NO_TIMEOUT);
    if(err == ERRORNO)
    {
        *timeout = customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
                rx_msg.selected_item;
    }

    return err;
}
