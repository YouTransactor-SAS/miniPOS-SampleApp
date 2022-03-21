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
* @file: display.c
*
* @date: July 2021
*
* @author: Khawla TAHRI <khawla.tahri@ext.youtransactor.com>
*/

#include "display.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "config.h"
#include "util_lib.h"
#include "common.h"
#include "svpp_com.h"
#include "yt_atmel_test_lib_itf.h"
#include "customer_app.h"
#include "payment.h"
#include "display_list_box.h"
extern customer_app_ctx_t customer_app_ctx;
extern uint8_t    g_u08MenuProgress;

/*
 * @brief    display list box of message screen
 * @return    error
 */
error_t select_menu_display(uint8_t* selected_display)
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

    strcpy((char*)&list[index],
            (char*)"Message Screen");
    index += strlen((char*)"Message Screen");
    list[index++] = '\n';
    list[index++] = 0x00;

    strcpy((char*)&list[index],
            (char*)"Loader Error");
    index += strlen((char*)"Loader Error");
    list[index++] = '\n';
    list[index++] = 0x00;

    strcpy((char*)&list[index],
            (char*)"Loader Full");
    index += strlen((char*)"Loader Full");
    list[index++] = '\n';
    list[index++] = 0x00;

    strcpy((char*)&list[index],
            (char*)"Loader Screen");
    index += strlen((char*)"Loader Screen");
    list[index++] = '\n';
    list[index++] = 0x00;

    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.pList =
            list;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.list_length =
            index;

    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.font = 1;
//    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.view_index = 11;
//    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.choice_format = 0;

    strcpy((char*)&customer_app_ctx.rpc_ctx.param->
            hmi_drv_displaylistboxwithoutki.tx_msg.title,
            (char*)"Display");
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    title_length = strlen((char*)"Display") +1;

//    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
//    upper_title_length =  1;
//    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
//    sub_title_length =  1;

    err = svpp_com_send_rpc(&customer_app_ctx.rpc_ctx,
            SVPP_COM_HMI_DRV_DISPLAYLISTBOXWITHOUTKI_CMD_NB,
            SVPP_COM_RPC_NO_TIMEOUT);

    *selected_display = customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
            rx_msg.selected_item;
    PRINTF("selected item display %d\r\n",customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
            rx_msg.selected_item);
    return err;
}


/**
 * @brief    display message
 * @return    error
 */
error_t display_message_with_ki(uint8_t index_view)
{
    error_t err = ERRORNO;
    svpp_com_line_desciption_t line_tab[2];

    uint8_t msg1_loader_full[]   = "1.00 €\n Present Card";
    uint8_t msg2_loader_full[]   = "Cancel";
    uint8_t msg1_loader_error[]  = "1.00 €\n Present Card";
    uint8_t msg2_loader_error[]  = "Cancel";
    uint8_t msg1_loader_screen[] = "1.00 €\n Present Card";
    uint8_t msg2_loader_screen[] = "Cancel";
    uint8_t msg1_message_screen[] = "1.00 €\n Present Card";
    uint8_t msg2_message_screen[] = "Cancel";
    memset((uint8_t*)&customer_app_ctx.rpc_ctx.param->hmi_drv_displaymessagewithoutki.
            tx_msg, 0,
            sizeof(svpp_com_tx_hmi_drv_displaymessagewithoutki_t));
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaymessagewithoutki.tx_msg.
    timeout = 0xFF;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaymessagewithoutki.tx_msg.
    number_of_lines_to_display = 2;

    switch(index_view)
    {
    case 14 :
        line_tab[0].len = sizeof(msg1_loader_full);
        line_tab[0].pText = msg1_loader_full;
        line_tab[1].len = sizeof(msg2_loader_full);
        line_tab[1].pText = msg2_loader_full;
        break;
    case 15 :
        line_tab[0].len = sizeof(msg1_loader_error);
        line_tab[0].pText = msg1_loader_error;
        line_tab[1].len = sizeof(msg2_loader_error);
        line_tab[1].pText = msg2_loader_error;
        break;
    case 16 :
        line_tab[0].len = sizeof(msg1_loader_screen);
        line_tab[0].pText = msg1_loader_screen;
        line_tab[1].len = sizeof(msg2_loader_screen);
        line_tab[1].pText = msg2_loader_screen;
        break;
    case 17 :
        line_tab[0].len = sizeof(msg1_message_screen);
        line_tab[0].pText = msg1_message_screen;
        line_tab[1].len = sizeof(msg2_message_screen);
        line_tab[1].pText = msg2_message_screen;
        break;
    default:
        break;
    }
    line_tab[0].font = 0;
    line_tab[0].x_coord = 0;
    line_tab[0].y_coord = 0;
    line_tab[1].font = 0;
    line_tab[1].x_coord = 0;
    line_tab[1].y_coord = 0;

    customer_app_ctx.rpc_ctx.param->hmi_drv_displaymessagewithoutki.tx_msg.pLine =
            line_tab;

    customer_app_ctx.rpc_ctx.param->hmi_drv_displaymessagewithoutki.tx_msg.
    abort_key = SVPP_COM_ABORT_WITH_OK_KEY;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaymessagewithoutki.tx_msg.
    clear_display_config = ((1<<SVPP_COM_YES_NO_PROMPT) |SVPP_COM_CLEAR_DISPLAY);

    customer_app_ctx.rpc_ctx.param->hmi_drv_displaymessagewithoutki.tx_msg.index_view = index_view;

    err = svpp_com_send_rpc(&customer_app_ctx.rpc_ctx,
            SVPP_COM_HMI_DRV_DISPLAYMESSAGEWITHOUTKI_CMD_NB,
            SVPP_COM_RPC_DEFAULT_TIMEOUT);
    return err;
}

/*
 * @brief    display list box of message screen
 * @return    error
 */
error_t start_menu_display_item(uint8_t selected_display)
{

    error_t err=ERRORNO;

    switch (selected_display)
    {
    case 1:
        PRINTF("MENU_DISPLAY_MESSAGE_SCREEN\r\n");
        err=display_message_with_ki(17);
        break;
    case 2:
        PRINTF("MENU_DISPLAY_LOADER_ERROR\r\n");
        err=display_message_with_ki(15);
        break;
    case 3:
        PRINTF("MENU_DISPLAY_LOADER_FULL\r\n");
        err=display_message_with_ki(14);
        break;
    case 4:
        PRINTF("MENU_DISPLAY_LOADER_SCREEN\r\n");
        err=display_message_with_ki(16);
        break;
    default:
        break;
    }
    return err;
}

error_t run_display_menu(void)
{
    error_t err=ERRORNO;
    uint8_t selected_display =0;
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
#ifdef APP_MODEM_STAND_ALONE
    g_u08MenuProgress = 1;
#endif // APP_MODEM_STAND_ALONE
    while(!err)
    {
        err = select_menu_display(&selected_display);
        if(!err)
        {
            start_menu_display_item(selected_display);
        }
    }
#ifdef APP_MODEM_STAND_ALONE
    g_u08MenuProgress =0;
#endif // APP_MODEM_STAND_ALONE
    return err;
}

