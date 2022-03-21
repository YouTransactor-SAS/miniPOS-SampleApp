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
* @file: communication_menu.c
*
* @date: August 2021
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
#include "display_list_box.h"
#include "communication_menu.h"
#include "display.h"
#include "payment.h"
#include "customer_app.h"
#include "network.h"
#include "ucube_api.h"
#include "network_manager.h"
#include "driver_spi.h"
#include "display_message.h"
#include "driver_cellular.h"
#include "power_off_timeout.h"
#ifdef CONFIG_MODEM
extern customer_app_ctx_t customer_app_ctx;
extern uint8_t    g_u08MenuProgress;
extern network_handler_t  network_handler_ct;
extern socket_handler_t socket_handler_ct;

uint8_t  send_request_done=0;
uint8_t  wifi_connect_ap_complete=0;
uint8_t  wifi_disconnect_ap_complete=0;

const char * ret_str;
const char *get_device_name(uint8_t network_device)
{
    if (network_device == COMMUNICATION_DEVICE_4G){
        ret_str = "4G";
    }
    else if (network_device == COMMUNICATION_DEVICE_WIFI){
        ret_str = "WiFi";
    }else{
        ret_str = "unknown device";
    }
    return ret_str;
}

/*
 * @brief    display cellular interface result
 * @return    error
 */
error_t display_result_connection(uint8_t result, uint8_t success_value,
        uint8_t network_device)
{
    error_t err=ERRORNO;
    svpp_com_line_desciption_t line_tab[2];
    uint8_t msg1[50];
    uint8_t msg2[] = "";
    uint8_t size_msg1=0;
    const char *network_dev;
    network_dev = get_device_name(network_device);
    if(result == success_value){
        size_msg1 = sprintf((char*)msg1, "%s is connected", network_dev) +1;
    }
    else{
        size_msg1 = sprintf((char*)msg1, "%s is disconnected", network_dev) +1;
    }
    err = display_message(msg1, size_msg1,0xFF,0xFF,50);
    return err;
}

error_t display_operation_failure(uint8_t network_device)
{
    error_t err=ERRORNO;
    svpp_com_line_desciption_t line_tab[2];
    uint8_t msg1[50];
    uint8_t msg2[] = "";
    uint8_t size_msg1=0;
    const char *network_dev;
    network_dev = get_device_name(network_device);
    size_msg1 = sprintf((char*)msg1, "%s operation failure",
                network_dev) +1;
    err = display_message(msg1, size_msg1,0xFF,0xFF,50);
    return err;
}



/*
 * @brief    display list box of message screen
 * @return    error
 */
error_t select_communication_menu(uint8_t* selected_display)
{
    uint8_t list[6* SVPP_COM_APP_NAME_LEN + 10];
    uint8_t index= 0;
    uint8_t i=0;
    error_t err=ERRORNO;
    svpp_com_line_desciption_t line;

    memset((uint8_t*)&customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
            tx_msg, 0,
            sizeof(svpp_com_tx_hmi_drv_displaylistboxwithoutki_t));
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    timeout = 90;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    title_length =  1;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
        font = 1;

    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"Connection status");
    index += strlen((char*)"Connection status");
    list[index++] = '\n';
    list[index++] = 0x00;

    i++;
    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"Preferred interface");
    index += strlen((char*)"Preferred interface");
    list[index++] = '\n';
    list[index++] = 0x00;

    i++;
    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"Cellular");
    index += strlen((char*)"Cellular");
    list[index++] = '\n';
    list[index++] = 0x00;

    i++;
    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"WIFI");
    index += strlen((char*)"WIFI");
    list[index++] = '\n';
    list[index++] = 0x00;

    i++;
    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"Paris Temperature");
    index += strlen((char*)"Paris Temperature");
    list[index++] = '\n';
    list[index++] = 0x00;

    i++;
    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"MDM request");
    index += strlen((char*)"MDM request");
    list[index++] = '\n';
    list[index++] = 0x00;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.pList =
            list;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.list_length =
            index;

    err = svpp_com_send_rpc(&customer_app_ctx.rpc_ctx,
            SVPP_COM_HMI_DRV_DISPLAYLISTBOXWITHOUTKI_CMD_NB,
            SVPP_COM_RPC_NO_TIMEOUT);
    *selected_display = customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
            rx_msg.selected_item;
    PRINTF("selected item com %d\r\n",customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
            rx_msg.selected_item);
    return err;
}

/**
 * @brief    display connection status
 * @return    error
 */
error_t display_connection_status(void)
{
    uint8_t index_view =17;
    error_t err = ERRORNO;
    svpp_com_line_desciption_t line_tab[2];
    network_status_t network_status;
    cellular_status_t cellular_status;
    wifi_status_t wifi_status;
    char enabled[] = "Enabled";
    char disabled[] = "Disabled";
    char wifi[] = "WiFi";
    char cellular[] = "Cellular";
    char connected[] = "connected";
    char disconnected[] = "disconnected";
    uint8_t msg1[120];
    uint8_t msg2[30];
    uint8_t msg3[30];
    uint8_t msg4[40];

    uint8_t size_msg1=0,size_msg2=0,size_msg3=0,size_msg4=0;

    network_status = UCube_api_get_connection_status();
    if(network_status.is_enabled){
        size_msg1 =sprintf((char*)msg1, "Network enabled\n\n") ;
    }
    else{
        size_msg1 =sprintf((char*)msg1, "Network disabled\n\n") ;
    }

    cellular_status = UCube_api_get_cellular_status();
    if(cellular_status.is_connected){
        size_msg2 =sprintf((char*)msg2, "4G ON\n\n") ;
    }
    else{
        size_msg2 =sprintf((char*)msg2, "4G OFF\n\n") ;
    }

    wifi_status = UCube_api_get_wifi_status();
    if(wifi_status.is_connected){
        size_msg3 =sprintf((char*)msg3, "WiFi ON\n\n") ;
    }
    else{
        size_msg3 =sprintf((char*)msg3, "WiFi OFF\n\n");
    }

    switch(network_status.preferred_interface)
    {
    case PREFERRED_INTERFACE_CELLULAR:
    {
        size_msg4 =sprintf((char*)msg4, "Preferred interface: 4G \n");
        break;
    }
    case PREFERRED_INTERFACE_WIFI:
    {
        size_msg4 =sprintf((char*)msg4, "Preferred interface:\n WiFi\n");
        break;
    }
    default:
    {
        size_msg4 =sprintf((char*)msg4, "No Preferred interface selected\n");
        break;
    }
    }

    strncat((char *)msg1, (char *)msg2, size_msg2);
    strncat((char *)msg1, (char *)msg3, size_msg3);
    strncat((char *)msg1, (char *)msg4, size_msg4);

    err = display_message(msg1, strlen((char*)msg1)+1,0xFF,0xFF,50);
    return err;
}

/*
 * @brief    display cellular interface result
 * @return    error
 */
error_t display_result(uint8_t result, uint8_t success_value)
{

    error_t err=ERRORNO;
    uint8_t index_view;
    svpp_com_line_desciption_t line_tab[2];
    uint8_t msg1[10];
    uint8_t msg2[] = "";
    uint8_t size_msg1=0;

    if(result == success_value){
        size_msg1 =sprintf((char*)msg1, "Done") +1;
        index_view =14;
    }
    else{
        size_msg1 =sprintf((char*)msg1, "Error") +1;
        index_view =15;
    }

    err = display_message(msg1, size_msg1,0xFF,0xFF,50);

    return err;
}

/*
 * @brief    start preferred communication option
 * @return    error
 */
void start_cellular_menu_item(uint8_t selected_option)
{

    error_t err=ERRORNO;
    switch (selected_option)
    {
        case MENU_CELLULAR_SELECTED_ACTION_CONNECT:
        {
            err=connect_interface_cellular(&network_handler_ct);
            if(network_handler_ct.cellular_status.is_connected){
                display_result_connection(err, 0, COMMUNICATION_DEVICE_4G);
            }else{
                display_operation_failure(1);
            }
            break;
        }
        case MENU_CELLULAR_SELECTED_ACTION_DISCONNECT:
        {
            err=disconnect_interface_cellular(&network_handler_ct);
            if(!network_handler_ct.cellular_status.is_connected){
                display_result_connection(err,1,COMMUNICATION_DEVICE_4G);
            }else{
                display_operation_failure(1);
            }
            break;
        }
        case MENU_CELLULAR_SELECTED_ACTION_CONFIG:
        {
            err=config_cellular_network(&network_handler_ct);
            display_result(err,0);
            break;
        }
        default:
            break;
    }
}

/*
 * @brief    select cellular communication menu item
 * @return    error
 */
error_t select_cellular_menu(uint8_t *action)
{
    uint8_t list[2 * SVPP_COM_APP_NAME_LEN + 10];
    uint8_t index = 0;
    uint8_t i = 0;
    error_t err = ERRORNO;
    svpp_com_line_desciption_t line;
    uint8_t title[40];
    uint8_t size_title;
    cellular_status_t  cellular_status;
    uint8_t selected_item;
    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index], ENTRY_CONFIG_4G_APN);
    index += strlen(ENTRY_CONFIG_4G_APN);
    list[index++] = '\n';
    list[index++] = 0x00;
    i++;

    cellular_status = UCube_api_get_cellular_status();
    if(cellular_status.is_connected){
        list[index++] = (i + 1) + 0x30;
        list[index++] = '-';
        strcpy((char*)&list[index], ENTRY_DISCONNECT);
        index += strlen(ENTRY_DISCONNECT);
    }
    else{
        list[index++] = (i + 1) + 0x30;
        list[index++] = '-';
        strcpy((char*)&list[index], ENTRY_CONNECT);
        index += strlen(ENTRY_CONNECT);
    }
    list[index++] = '\n';
    list[index++] = 0x00;

    memset((uint8_t*)&customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
            tx_msg, 0,
            sizeof(svpp_com_tx_hmi_drv_displaylistboxwithoutki_t));
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    timeout = 90;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    title_length = 1;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    font = 1;

    list[index++] = '\n';
    list[index++] = 0x00;


    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.pList =
            list;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.list_length =
            index;
    err = svpp_com_send_rpc(&customer_app_ctx.rpc_ctx,
            SVPP_COM_HMI_DRV_DISPLAYLISTBOXWITHOUTKI_CMD_NB,
            SVPP_COM_RPC_DEFAULT_TIMEOUT);
    if(!err)
    {
        selected_item = customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
                rx_msg.selected_item;
        if(selected_item == 1)
        {
            *action = MENU_CELLULAR_SELECTED_ACTION_CONFIG;
        }
        else
        {
            if(cellular_status.is_connected){
                *action = MENU_CELLULAR_SELECTED_ACTION_DISCONNECT;
            }
            else{
                *action = MENU_CELLULAR_SELECTED_ACTION_CONNECT;
            }
        }
    }

    return err;
}

int run_cellular_menu(void)
{
    static uint8_t sim_card_already_checked = 0; // workaround
    // it seems that the QAPI for checking the SIM card returns unwanted
    // value when connection fails
    int err = ERRORNO;
    uint8_t selected_action = 0;
    char * ptr = NULL;
    char error_msg[60] = "SIM Card ";
    uint8_t idx = strnlen(error_msg, sizeof(error_msg));
    char status[30];

    if (!sim_card_already_checked)
    {
        err = driver_cellular_get_sim_status(status);

        // if no error on QAPI side
        if(!err)
        {
            // status is something like "+CPIN: XXXX"
            if (!strncmp(status,
                        CARD_SIM_STATUS_AT_CMD,
                        strlen(CARD_SIM_STATUS_AT_CMD)))
            {
                ptr = strchr(status, CARD_SIM_STATUS_WHITESPACE_IN_ASCII);
                if (ptr)
                {
                    ptr++; // skip whitespace
                    if (!strncmp(ptr, CARD_SIM_READY, strlen(CARD_SIM_READY)))
                    {
                        // launch menu
                        err = ERRORNO;
                        sim_card_already_checked = 1;
                    }
                    else
                    {
                        memcpy(&error_msg[idx],
                                ptr,
                                MIN(strlen(ptr), sizeof(error_msg) - idx));
                        err = -EFAIL;
                    }
                }
                else
                {
                    memcpy(error_msg,
                            CARD_SIM_NO_STATUS,
                            MIN(strlen(CARD_SIM_NO_STATUS), sizeof(error_msg)));
                    err = -EFAIL;
                }
            }
            else
            {
                memcpy(error_msg,
                        CARD_SIM_NO_STATUS,
                        MIN(strlen(CARD_SIM_NO_STATUS), sizeof(error_msg)));
                err = -EFAIL;
            }
        }
        else
        {
            memcpy(error_msg,
                    CARD_SIM_NO_STATUS,
                    MIN(strlen(CARD_SIM_NO_STATUS), sizeof(error_msg)));
        }
    }

    // if everything is OK, launch cellular menu
    if (!err)
    {
        do{
            err = select_cellular_menu(&selected_action);
            if(!err)
            {
                start_cellular_menu_item(selected_action);
            }
            qapi_Timer_Sleep(1000, QAPI_TIMER_UNIT_MSEC, true);
        }while(!err);
    }
    else
    {
        // +1 for the "\0"
        display_message((uint8_t*)error_msg,
                strnlen(error_msg, sizeof(error_msg)) + 1,0xFF,0xFF,50);
    }

    return err;
}

/*
 * @brief    start preferred communication option
 * @return    error
 */
void start_wifi_menu_item(uint8_t selected_option)
{
    error_t err=ERRORNO;
    switch (selected_option)
    {
    case MENU_WIFI_SELECTED_ACTION_CONNECT:
        err = connect_interface_wifi(&network_handler_ct);
        if(network_handler_ct.wifi_status.is_connected){
            display_result_connection(err, 0, COMMUNICATION_DEVICE_WIFI);
        }else{
            display_operation_failure(COMMUNICATION_DEVICE_WIFI);
        }
        break;
    case MENU_WIFI_SELECTED_ACTION_DISCONNECT:
        err = disconnect_interface_wifi(&network_handler_ct);
        if(!network_handler_ct.wifi_status.is_connected){
            display_result_connection(err,1,COMMUNICATION_DEVICE_WIFI);
        }else{
            display_operation_failure(COMMUNICATION_DEVICE_WIFI);
        }
        break;
    case MENU_WIFI_ACCESS_POINT_CONFIG:
        int trf_to_ms_ap= 1000;
        PRINTF_YT("****************************************\r\n"
                  "****************************************\r\n"
                  "**********Enable Access Point***********\r\n");
        uint8_t msg_ap_ip[] =
                " Please note the logins and\n"
                " press ok to proceed\n"
                "\n"
                " Connect on WiFi:\n"
                "  - ID:       MCC_WIFI_AP\n"
                "  - Password: mcc_1920_AP\n"
                " Connect to this IP:\n"
                "  - http://192.168.4.1/login\n";
        uint8_t msg_ap_ip_disconnect[] = "Disconnecting SAP...";
        wifi_connect_ap_complete = 0;
        display_message(msg_ap_ip, sizeof(msg_ap_ip),0xFF, 0, 50);
        err =  connect_wifi_access_pnt();
        do{
            qapi_Timer_Sleep(10, QAPI_TIMER_UNIT_MSEC, true);
            trf_to_ms_ap-= 10;
        }while((!wifi_connect_ap_complete) && trf_to_ms_ap);
        wifi_connect_ap_complete=0;
        display_result_connection(err,WIFI_STATE_CONNECTED_AP_PROVIDING_SAP,
                COMMUNICATION_DEVICE_WIFI);
        qapi_Timer_Sleep(1, QAPI_TIMER_UNIT_USEC, true);
        display_message(msg_ap_ip_disconnect, sizeof(msg_ap_ip_disconnect),
                        3, 0xFF, 50);
        err = disconnect_wifi_access_pnt();
        display_result(err,0);
        break;
    default:
        break;
    }
}

/*
 * @brief    select cellular communication menu item
 * @return    error
 */
error_t select_wifi_menu(uint8_t *action)
{
    uint8_t list[3 * SVPP_COM_APP_NAME_LEN + 10];
    uint8_t index= 0;
    uint8_t i=0;
    error_t err=ERRORNO;
    svpp_com_line_desciption_t line;
    uint8_t title[40];
    uint8_t size_title;
    wifi_status_t  wifi_status;
    uint8_t selected_item;
    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index], ENTRY_CONFIG_WIFI_SAP);
    index += strlen(ENTRY_CONFIG_WIFI_SAP);
    list[index++] = '\n';
    list[index++] = 0x00;
    i++;

    wifi_status = UCube_api_get_wifi_status();
    if(wifi_status.is_connected){
        list[index++] = (i + 1) + 0x30;
        list[index++] = '-';
        strcpy((char*)&list[index], ENTRY_DISCONNECT);
        index += strlen(ENTRY_DISCONNECT);
    }
    else{
        list[index++] = (i + 1) + 0x30;
        list[index++] = '-';
        strcpy((char*)&list[index], ENTRY_CONNECT);
        index += strlen(ENTRY_CONNECT);
    }
    list[index++] = '\n';
    list[index++] = 0x00;

    memset((uint8_t*)&customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
            tx_msg, 0,
            sizeof(svpp_com_tx_hmi_drv_displaylistboxwithoutki_t));
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    timeout = 90;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    title_length = 1;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
        font = 1;

    list[index++] = '\n';
    list[index++] = 0x00;


    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.pList =
            list;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.list_length =
            index;
    err = svpp_com_send_rpc(&customer_app_ctx.rpc_ctx,
            SVPP_COM_HMI_DRV_DISPLAYLISTBOXWITHOUTKI_CMD_NB,
            SVPP_COM_RPC_NO_TIMEOUT);
    selected_item = customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
            rx_msg.selected_item;
    if(selected_item == 1)
    {
        *action = MENU_WIFI_ACCESS_POINT_CONFIG;
    }
    else
    {
        if(wifi_status.is_connected){
            *action = MENU_WIFI_SELECTED_ACTION_DISCONNECT ;
        }
        else{
            *action = MENU_WIFI_SELECTED_ACTION_CONNECT;
        }
    }

    return err;
}

error_t run_wifi_menu(void)
{
    error_t err = ERRORNO;
    uint8_t selected_action = 0;

    do
    {
        err = select_wifi_menu(&selected_action);
        if(!err)
        {
            qapi_Timer_Sleep(50, QAPI_TIMER_UNIT_MSEC, true);
            start_wifi_menu_item(selected_action);
        }
        qapi_Timer_Sleep(50, QAPI_TIMER_UNIT_MSEC, true);
    } while(!err);

    return  err;
}
/*
 * @brief    display preferred interface result
 * @return    error
 */
error_t display_preferred_interface_sub_menu(uint8_t result)
{

    error_t err=ERRORNO;
    uint8_t index_view;
    svpp_com_line_desciption_t line_tab[2];
    uint8_t msg1[10];
    uint8_t msg2[] = "";
    uint8_t size_msg1=0;

    if(result == ERRORNO){
        size_msg1 =sprintf((char*)msg1, "Done") +1;
        index_view =14;
    }
    else{
        size_msg1 =sprintf((char*)msg1, "Error") +1;
        index_view =15;
    }

     err = display_message(msg1, size_msg1,0xFF,0xFF,50);
     return err;
}

/*
 * @brief    start preferred communication option
 * @return    error
 */
error_t start_preferred_communication_menu_item(uint8_t selected_display)
{

    error_t err=ERRORNO;

    switch (selected_display)
    {
    case MENU_PREFERRED_COMMUNICATION_WIFI:
        err=UCube_api_set_preferred_network_interface(PREFERRED_INTERFACE_WIFI);
        //display_cellular_sub_menu(err);
        display_preferred_interface_sub_menu(err);
        break;
    case MENU_PREFERRED_COMMUNICATION_CELLULAR:
        err =UCube_api_set_preferred_network_interface(PREFERRED_INTERFACE_CELLULAR);
        //display_cellular_sub_menu(err);
        display_preferred_interface_sub_menu(err);
        break;
    default:
        break;
    }
    return err;
}

/*
 * @brief    start preferred communication menu
 * @return    error
 */
error_t select_preferred_communication_menu(uint8_t *selected_interface )
{
    uint8_t list[2 * SVPP_COM_APP_NAME_LEN + 10];
    uint8_t index= 0;
    uint8_t i=0;
    error_t err=ERRORNO;
    svpp_com_line_desciption_t line;
    uint8_t title[100];
    uint8_t size_title;
    uint8_t preferred_interface;

    preferred_interface = UCube_api_get_preferred_network_interface();

    memset((uint8_t*)&customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
            tx_msg, 0,
            sizeof(svpp_com_tx_hmi_drv_displaylistboxwithoutki_t));
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    timeout = 90;

    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"WiFi");
    index += strlen((char*)"WiFi");
    list[index++] = '\n';
    list[index++] = 0x00;

    i++;
    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"Cellular");
    index += strlen((char*)"Cellular");
    list[index++] = '\n';
    list[index++] = 0x00;

    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.pList =
            list;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.list_length =
            index;

    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.font = 1;

    strcpy((char*)&customer_app_ctx.rpc_ctx.param->
            hmi_drv_displaylistboxwithoutki.tx_msg.title,
            (char *)title);
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    title_length = 1;//size_title;

    err = svpp_com_send_rpc(&customer_app_ctx.rpc_ctx,
            SVPP_COM_HMI_DRV_DISPLAYLISTBOXWITHOUTKI_CMD_NB,
            SVPP_COM_RPC_NO_TIMEOUT);

    *selected_interface = customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
            rx_msg.selected_item;
    PRINTF("selected item preferred com %d\r\n",customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
            rx_msg.selected_item);
    return err;
}

error_t run_preferred_communication_menu(void)
{
    error_t err=ERRORNO;
    uint8_t communication_menu_option =0;
    while(!err) //((!err)|| (err==-2))
    {
        qapi_Timer_Sleep(20, QAPI_TIMER_UNIT_MSEC, true);
        err = select_preferred_communication_menu(&communication_menu_option);
        if(!err)
        {
            qapi_Timer_Sleep(20, QAPI_TIMER_UNIT_MSEC, true);
            start_preferred_communication_menu_item(communication_menu_option);
        }
    }
    return err;
}

/*
 * @brief    test send request
 * @return    error
 */
error_t display_test_request_wait(uint8_t timeout)
{

    error_t err=ERRORNO;
    uint8_t index_view=16;
    svpp_com_line_desciption_t line_tab[2];
    uint8_t msg1[12];
    uint8_t msg2[] = "";
    uint8_t size_msg1=0;

    size_msg1 =sprintf((char*)msg1, "On Progress") +1;
    index_view =16;

    memset((uint8_t*)&customer_app_ctx.rpc_ctx.param->hmi_drv_displaymessagewithoutki.
            tx_msg, 0,
            sizeof(svpp_com_tx_hmi_drv_displaymessagewithoutki_t));
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaymessagewithoutki.tx_msg.
    timeout = timeout;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaymessagewithoutki.tx_msg.
    number_of_lines_to_display = 2;

    line_tab[0].len = size_msg1;//sizeof(msg1_loader_full);
    line_tab[0].pText = msg1;
    line_tab[1].len = sizeof(msg2); //sizeof(msg2_loader_full);
    line_tab[1].pText = msg2;
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
 * @brief    start communication menu
 * @return    error
 */
int start_communication_menu_item(uint8_t selected_option)
{

    int err=ERRORNO;

    switch (selected_option)
    {
    case MENU_COMMUNICATION_CONNECTION_STATUS:
        err=display_connection_status();
        break;
    case MENU_COMMUNICATION_PREFFERED_COMMUNICATION_TYPE:
        err=run_preferred_communication_menu();
        break;
    case MENU_COMMUNICATION_CELLULAR:
        err=run_cellular_menu();
        break;
    case MENU_COMMUNICATION_WIFI:
        err=run_wifi_menu();
        break;
    case MENU_COMMUNICATION_PARIS_TEMPERATURE_REQUEST:
        init_paris_temperature_request(&socket_handler_ct);
        break;
    case MENU_COMMUNICATION_MDM_REQUEST:
        send_wifi_ntw(&socket_handler_ct);
        break;
    default:
        break;
    }
    return err;
}

error_t run_communication_menu(void)
{
    error_t err=ERRORNO;
    uint8_t selected_menu_option =0;
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

    do{
        err = select_communication_menu(&selected_menu_option);
        if(!err)
        {
            start_communication_menu_item(selected_menu_option);
            qapi_Timer_Sleep(50, QAPI_TIMER_UNIT_MSEC, true);
        }
        qapi_Timer_Sleep(50, QAPI_TIMER_UNIT_MSEC, true);
    }while(!err); //((!err)||(err==-2)||(err==-114))
#ifdef APP_MODEM_STAND_ALONE
    g_u08MenuProgress =0;
#endif // APP_MODEM_STAND_ALONE
    return err;
}
#elif defined(CONFIG_PC)
#endif //CONFIG_XX
