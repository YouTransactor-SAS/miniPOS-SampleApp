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
* @file: display_list_box.c
*
* @date: Juin 2021
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
#include "power_off_timeout.h"
#include "display_list_box.h"
#include "display.h"
#include "payment.h"
#include "customer_app.h"


extern customer_app_ctx_t customer_app_ctx;
extern uint8_t    g_u08MenuProgress;

/*
 * @brief    display list box of message screen
 * @return    error
 */
error_t select_menu_list_box(uint8_t* selected_display)
{
    uint8_t list[3 * SVPP_COM_APP_NAME_LEN + 10];
    uint8_t index= 0;
    uint8_t i=0;
    error_t err = ERRORNO;
    svpp_com_line_desciption_t line;
    PRINTF("inside list box *******************\r\n");

    memset((uint8_t*)&customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
            tx_msg, 0,
            sizeof(svpp_com_tx_hmi_drv_displaylistboxwithoutki_t));
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    timeout = 90;

    strcpy((char*)&list[index],
            (char*)"Layout S");
    index += strlen((char*)"Layout S");
    list[index++] = '\n';
    list[index++] = 0x00;

    strcpy((char*)&list[index],
            (char*)"Layout M");
    index += strlen((char*)"Layout M");
    list[index++] = '\n';
    list[index++] = 0x00;

    strcpy((char*)&list[index],
            (char*)"Layout L");
    index += strlen((char*)"Layout L");
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
            (char*)"Display List Box");
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    title_length = strlen((char*)"Display List Box") +1;

//    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
//    upper_title_length =  1;
//    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
//    sub_title_length =  1;


    err = svpp_com_send_rpc(&customer_app_ctx.rpc_ctx,
            SVPP_COM_HMI_DRV_DISPLAYLISTBOXWITHOUTKI_CMD_NB,
            SVPP_COM_RPC_NO_TIMEOUT);

    *selected_display = customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
            rx_msg.selected_item;
    PRINTF("selected item list box %d\r\n",customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
            rx_msg.selected_item);
    return err;
}


/**
 * @brief    display list box
 * @return    error
 */
error_t display_list_box(uint8_t layout)
{
    uint8_t list[6 * SVPP_COM_APP_NAME_LEN + 10];
    uint8_t index= 0;
    uint8_t i=0;
    error_t err=ERRORNO;
    svpp_com_line_desciption_t line;
    memset((uint8_t*)&customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
            tx_msg, 0,
            sizeof(svpp_com_tx_hmi_drv_displaylistboxwithoutki_t));
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    timeout = 90;

    strcpy((char*)&list[index],
            (char*)"Item 1");
    index += strlen((char*)"Item 1");
    list[index++] = '\n';
    list[index++] = 0x00;

    strcpy((char*)&list[index],
            (char*)"Item 2");
    index += strlen((char*)"Item 2");
    list[index++] = '\n';
    list[index++] = 0x00;

    if(layout >= MENU_LIST_BOX_LAYOUT_M)
    {
    strcpy((char*)&list[index],
            (char*)"Item 3");
    index += strlen((char*)"Item 3");
    list[index++] = '\n';
    list[index++] = 0x00;
    }
    if(layout >= MENU_LIST_BOX_LAYOUT_L)
    {
    strcpy((char*)&list[index],
            (char*)"Item 4");
    index += strlen((char*)"Item 4");
    list[index++] = '\n';
    list[index++] = 0x00;
    strcpy((char*)&list[index],
            (char*)"Item 5");
    index += strlen((char*)"Item 5");
    list[index++] = '\n';
    list[index++] = 0x00;
    }
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.pList =
            list;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.list_length =
            index;

    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.font = 1;
//    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.view_index = 11;
//    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.choice_format = 0;

    strcpy((char*)&customer_app_ctx.rpc_ctx.param->
            hmi_drv_displaylistboxwithoutki.tx_msg.title,
            (char*)"Display Menu");
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    title_length = strlen((char*)"Display Menu") +1;

//    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
//    upper_title_length =  1;
//    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
//    sub_title_length =  1;

    err = svpp_com_send_rpc(&customer_app_ctx.rpc_ctx,
            SVPP_COM_HMI_DRV_DISPLAYLISTBOXWITHOUTKI_CMD_NB,
            SVPP_COM_RPC_NO_TIMEOUT);

    return err;
}

/*
 * @brief    display list box of message screen
 * @return    error
 */
error_t start_menu_list_box_item(uint8_t selected_display)
{

    error_t err=ERRORNO;

    switch (selected_display)
    {
    case MENU_LIST_BOX_LAYOUT_S:
        PRINTF("MENU_LIST_BOX_LAYOUT_S\r\n");
        err=display_list_box(MENU_LIST_BOX_LAYOUT_S);
        break;
    case MENU_LIST_BOX_LAYOUT_M:
        PRINTF("MENU_LIST_BOX_LAYOUT_M\r\n");
        err=display_list_box(MENU_LIST_BOX_LAYOUT_M);
        break;
    case MENU_LIST_BOX_LAYOUT_L:
        PRINTF("MENU_LIST_BOX_LAYOUT_L\r\n");
        err=display_list_box(MENU_LIST_BOX_LAYOUT_L);
        break;
    default:
        break;
    }
    return err;
}

error_t run_list_box_menu(void)
{
    error_t err=ERRORNO;
    uint8_t selected_layout =0;
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
        err = select_menu_list_box(&selected_layout);
        if(!err)
        {
            start_menu_list_box_item(selected_layout);
        }
    }
#ifdef APP_MODEM_STAND_ALONE
    g_u08MenuProgress =0;
#endif // APP_MODEM_STAND_ALONE
    return err;
}
