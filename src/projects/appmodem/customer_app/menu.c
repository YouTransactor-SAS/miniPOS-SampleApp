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
* @file: menu.c
*
* @date: March 2021
*
* @author: Khawla TAHRI <khawla.tahri@ext.youtransactor.com>
*/

#include "menu.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "config.h"
#include "util_lib.h"
#include "common.h"
#include "os_itf.h"
#include "svpp_com.h"
#include "svpp_com.h"
#include "menu.h"

#include "yt_atmel_test_lib_itf.h"

#include "cless_complete_nfc.h"
#include "cless_get_tags.h"
#include "cless_simp_online_pin.h"
#include "cless_start_nfc.h"
#include "cmn_auth_mng.h"
#include "cmn_get_rtc_from_term.h"
#include "contact_get_tags.h"
#include "contact_risk_mng.h"
#include "contact_select_app.h"
#include "contact_tx_final.h"
#include "contact_tx_init.h"
#include "contact_tx_process.h"
#include "payment.h"
#include "get_info.h"
#include "customer_app.h"
#ifdef SVPP_COM_USE_SPI
#include "driver_spi.h"
#include "qapi_timer.h"
#include "customer_app.h"
#endif

customer_app_ctx_t customer_app_ctx;

/**
 * @brief     select menu item
 * @return    error
 */
error_t menu_select(uint8_t* pSelected_menu_item)
{
    uint8_t list[4 * SVPP_COM_APP_NAME_LEN + 10];
    uint8_t index= 0;
    uint8_t i=0;
    error_t err=ERRORNO;
    svpp_com_line_desciption_t line;

    //    err = clear_screen_msg();
    memset((uint8_t*)&customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
            tx_msg, 0,
            sizeof(svpp_com_tx_hmi_drv_displaylistboxwithoutki_t));
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    timeout = 90;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    title_length =  1;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
        font = 1;
#if 1
    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"Pay");
    index += strlen((char*)"Pay");
    list[index++] = 0x00;

    i++;
    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"Get Info");
    index += strlen((char*)"Get Info");
    list[index++] = 0x00;

    i++;
    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"Set PowrOff timeout");
    index += strlen((char*)"Set PowrOff timeout");
    list[index++] = 0x00;

    i++;
    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"Communication Menu");
    index += strlen((char*)"Communication Menu");
    list[index++] = 0x00;

    i++;
    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"Display Text");
    index += strlen((char*)"Display Text");
    list[index++] = 0x00;

#else

    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"A1");
    index += strlen((char*)"A1");
    list[index++] = 0x00;

    i++;
    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"B1");
    index += strlen((char*)"B1");
    list[index++] = 0x00;

    i++;
    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"C1");
    index += strlen((char*)"C1");
    list[index++] = 0x00;

    i++;
    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"D2");
    index += strlen((char*)"D2");
    list[index++] = 0x00;
#endif

    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.pList =
            list;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.list_length =
            index;

    err = svpp_com_send_rpc(&customer_app_ctx.rpc_ctx,
            SVPP_COM_HMI_DRV_DISPLAYLISTBOXWITHOUTKI_CMD_NB,
            SVPP_COM_RPC_NO_TIMEOUT);
    if(err == ERRORNO)
    {
        *pSelected_menu_item = customer_app_ctx.
                rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
                rx_msg.selected_item;
        PRINTF("selected item menu %d\r\n",customer_app_ctx.
                rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
                rx_msg.selected_item);
        switch (*pSelected_menu_item)
        {
        case MENU_SELECT_PAY:
            PRINTF("Selected Menu: MENU_SELECT_PAY ");
            break;
        case MENU_SELECT_SET_GET_INFO:
            PRINTF("Selected Menu: MENU_SELECT_SET_GET_INFO ");
            break;
        case MENU_SELECT_SET_PWR_OFF_TIMEOUT:
            PRINTF("Selected Menu: MENU_SELECT_SET_PWR_OFF_TIMEOUT ");
            break;
        case MENU_SELECT_COMMUNICATION_MENU:
            PRINTF("Selected Menu: MENU_SELECT_COMMUNICATION_MENU ");
            break;
        case MENU_SELECT_SEND_DATA:
            PRINTF("Selected Menu: MENU_SELECT_SEND_DATA ");
            break;
//        case MENU_SELECT_DISPLAY:
//            PRINTF("Selected Menu: MENU_SELECT_DISPLAY ");
//            break;
        case MENU_SELECT_DISPLAY_LIST_BOX:
            PRINTF("Selected Menu: MENU_SELECT_DISPLAY_LIST_BOX ");
            break;
        case MENU_SELECT_DISPLAY_MENU:
            PRINTF("Selected Menu: MENU_SELECT_DISPLAY ");
            break;
        default:
            break;
        }
    }
    return err;

}

/**
 * @brief     start menu
 * @return    error
 */
error_t menu_start(uint8_t* pSelected_menu_item)
{
    error_t err = ERRORNO;
    memset(customer_app_ctx.buffer_in,0, sizeof(customer_app_ctx.buffer_in));
    memset(customer_app_ctx.buffer_out,0, sizeof(customer_app_ctx.buffer_out));
    memset(&customer_app_ctx.rpc_ctx,0, sizeof(customer_app_ctx.rpc_ctx));
    customer_app_ctx.rpc_ctx.buffer_in = customer_app_ctx.buffer_in;
    customer_app_ctx.rpc_ctx.buffer_out = customer_app_ctx.buffer_out;
    customer_app_ctx.rpc_ctx.size_in = &customer_app_ctx.size_in;
    customer_app_ctx.rpc_ctx.size_out = &customer_app_ctx.size_out;
    customer_app_ctx.rpc_ctx.queue = customer_app_ctx.queue;
    customer_app_ctx.rpc_ctx.param = &customer_app_ctx.param;
    customer_app_ctx.rpc_ctx.pErr = &customer_app_ctx.rpc_err;
    menu_select(pSelected_menu_item);
    return err;
}
