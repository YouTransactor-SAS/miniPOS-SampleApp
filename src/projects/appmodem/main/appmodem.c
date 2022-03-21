/**
 * 2006-2025 YOUTRANSACTOR ALL RIGHTS RESERVED. YouTransactor,
 * 32, rue Brancion 75015 Paris France, RCS PARIS: B 491 208 500, YOUTRANSACTOR
 * CONFIDENTIAL AND PROPRIETARY INFORMATION , CONTROLLED DISTRIBUTION ONLY,
 * THEREFORE UNDER NDA ONLY. YOUTRANSACTOR Authorized Parties and who have
 * signed NDA do not have permission to distribute this documentation further.
 * Unauthorized recipients must destroy any electronic and hard-copies
 * and without reading and notify Gregory Mardinian, CTO, YOUTRANSACTOR
 * immediately at gregory.mardinian@youtransactor.com.
 *
 * @file: appmodem.c
 *
 * @date: Mar 2020
 *
 * @author: amoussa <amani.moussa@ext.youtransactor.com>
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "config.h"
#include "common.h"
#include "log.h"
#include "os_itf.h"
#include "eep.h"
#include "flash.h"
#include "init_bsp.h"
#include "log_mng.h"
#include "svpp_com.h"
#include "network_manager.h"
#include "payment.h"
#include "network_tms.h"
#include "ucube_api.h"
#include "driver_cellular.h"
#include "send_data.h"
#include "communication_menu.h"
#include "menu.h"
#include "customer_app.h"
#include "power_off_timeout.h"
#ifdef EXAMPLE_PING
#include "example_ping.h"
#endif // EXAMPLE_PING

#ifdef EXAMPLE_HTTP
#include "example_http.h"
#endif // EXAMPLE_HTTP
uint8_t g_u08MenuProgress;
extern customer_app_ctx_t customer_app_ctx;
#include "get_info.h"
#ifdef CONFIG_PC
//void app_get_cb_cfg(payment_pay_cfg_t* cfg)
//{
//    cfg->amount = 10;
//    cfg->cash_back_amount = -1;
//    cfg->currency_code[0] = (uint8_t)((SVPP_COM_CUREENCCY_EURO & 0xFF00) >> 8);
//    cfg->currency_code[1] = (uint8_t)(SVPP_COM_CUREENCCY_EURO & 0x00FF);
//    cfg->currency_exp = 1;
//    cfg->target_slot = SVPP_COM_SMART_CARD_SLOT;
//    cfg->tx_type = SVPP_COM_TRANS_CASH;
//    cfg->lang[0] = 'e';
//    cfg->lang[1] = 'n';
//    cfg->tip = 0;
//}

uint8_t menu_pc_start(void)
{
    char entry;
    uint8_t selected_item=0;
    puts("    Choose:                   \n");
    puts("    a - start a payment       \n");
    puts("    b - get info              \n");
    puts("    c - set poweroff timeout  \n");
    puts("    d - communication menu    \n");
    puts("    Q - send data              \n");
    entry = getchar();
    while(getchar() != '\n')    /* read \0 */
    {}

    printf("\n");
    switch(entry)
    {
    case 'a':
        PRINTF("Selected Menu: MENU_SELECT_PAY ");
        selected_item = MENU_SELECT_PAY;
        break;

    case 'b':
        PRINTF("Selected Menu: MENU_SELECT_SET_GET_INFO ");
        selected_item = MENU_SELECT_SET_GET_INFO;
        break;

    case 'c':
        PRINTF("Selected Menu: MENU_SELECT_SET_PWR_OFF_TIMEOUT ");
        selected_item = MENU_SELECT_SET_PWR_OFF_TIMEOUT;
        break;

    case 'd':
        PRINTF("Selected Menu: MENU_SELECT_COMMUNICATION_MENU ");
        selected_item = MENU_SELECT_COMMUNICATION_MENU;
        break;

    case 'Q':
        PRINTF("Selected Menu: MENU_SELECT_COMMUNICATION_MENU ");
        selected_item = MENU_SELECT_SEND_DATA;
        break;
    default:
        break;
    }
    return selected_item;
}
int main(int xargc, char * xargv[])
{
    uint8_t start = 0xFF;
    uint8_t menu_item=0;
    uint8_t startApp=0;
//    payment_pay_cfg_t cfg;
    puts(" Starting: \r\n");
    puts(" Init bsp: \r\n");
    init_bsp();
    puts("Start payment init: \r\n ");
    start = payment_init();
    printf(" Start payment end %d: \r\n",start);
    if(start == 0)
    {
        puts(" Start payment svpp com init: \r\n");
        start = svpp_com_init();
    }
    if(start == 0)
    {
        puts(" Start network_tms_init: \r\n");
        start = network_tms_init();
    }
    if(start == 0)
    {
        puts(" Start  customer_app_init: \r\n");
        start =  customer_app_init();
    }
    if (start == 0)
    {
        puts(" Start log_mng_init: \r\n");
        start = log_mng_init();
    }

    while(1)
    {
        if (g_u08MenuProgress == 0)
        {
            /*display the menu on modem screen and get selected item*/
            menu_item = menu_pc_start();
            PRINTF("menu start end ==> selected item %d \r\n",menu_item);
            startApp =menu_item;
            menu_item=0;

            /* send selected item to customer queue*/
            os_itf_queue_msg_t msg_from_main_to_customer_task;
            msg_from_main_to_customer_task.type = MESSAGE_RECV;
            msg_from_main_to_customer_task.size = 1;
            msg_from_main_to_customer_task.payload.pData = &startApp;
            OS_ITF_QUEUE_API_ARG msg_to_queue = msg_from_main_to_customer_task;
            PRINTF("send msg to queue\r\n");
            os_itf_queue_send(&customer_app_ctx.queue, msg_to_queue);

        }
    }
    return EXIT_SUCCESS;
}
#elif defined(CONFIG_MODEM)

#include "tx_thread.h"
#include "customer_app.h"
#include "menu.h"
#include "network.h"
uint8_t selected_menu_item=0;

#ifdef APP_MODEM_STAND_ALONE
#include "qapi_timer.h"
extern customer_app_ctx_t customer_app_ctx;
#endif
//TX_MUTEX *tx_mutex_handle = NULL;

//! @Brief Entry point of the Modem Application
//! @Details This is the equivalent of "main"
int quectel_task_entry(void)
{
    error_t err = TX_FEATURE_NOT_ENABLED;

    init_bsp();

//     Creation of bytes pool
    err = os_itf_create_byte_pool();
    if (err != TX_SUCCESS)
    {
        PRINTF_VERBOSE("ERROR: Could not create byte pool !, error %d\r\n", err);
    }
    else
    {
        PRINTF_VERBOSE("Creating byte pool success !\r\n");
    }

    // Creation of Service Task
    err = EFAIL;
    err = payment_init();
    if(err != ESUCCESS)
    {
        PRINTF_VERBOSE("ERROR: Could not create Service Task, error %d\r\n", err);
    }
    else
    {
        PRINTF_VERBOSE("Service Task created with success !\r\n");
    }

    // Creation of Svpp Com Task
    err = EFAIL;
    err = svpp_com_init();
    if(err != ESUCCESS)
    {
        PRINTF_VERBOSE("ERROR: Could not create Svpp Com Task, error %d\r\n", err);
    }
    else
    {
        PRINTF_VERBOSE("Svpp Com Task created with success !\r\n");
    }

    // Creation of Network Task
//    err = EFAIL;
//    err = network_tms_init();
//    if(err != ESUCCESS)
//    {
//        PRINTF_VERBOSE("ERROR: Could not create Network Task, error %d\r\n", err);
//    }
//    else
//    {
//        PRINTF_VERBOSE("Network Task created with success !\r\n");
//    }

    err = EFAIL;
    err =  network_init();
    if(err != ESUCCESS)
    {
        PRINTF_VERBOSE("ERROR: Could not create http Task, error %d\r\n", err);
    }
    else
    {
        PRINTF_VERBOSE("Network Task created with success !\r\n");
    }

    // Creation of Customer Task
    err = EFAIL;
    err = customer_app_init();
    if(err != ESUCCESS)
    {
        PRINTF_VERBOSE("ERROR: Could not create Customer App Task, error %d\r\n", err);
    }
    else
    {
        PRINTF_VERBOSE("Customer App Task created with success !\r\n");
    }
#ifdef EXAMPLE_PING

        // Creation of ping Task
            err = EFAIL;
            err = ping_init();
            if(err != ESUCCESS)
            {
                PRINTF("ERROR: Could not create ping Task, error %d\r\n");
            }
            else
            {
                PRINTF("ping Task created with success !\r\n");
            }

            ping_process();

#endif // EXAMPLE_PING

#ifdef EXAMPLE_HTTP

        // Creation of http Task
            err = EFAIL;
            err = http_init();
            if(err != ESUCCESS)
            {
                PRINTF("ERROR: Could not create ping Task, error %d\r\n");
            }
            else
            {
                PRINTF("ping Task created with success !\r\n");
            }

#endif // EXAMPLE_HTTP

#ifdef APP_MODEM_STAND_ALONE
    // wait before launching the contact transaction
    // this is a ugly synchro...
    // there is an IO that could be used (GPIO2 alias GPIO21)
    uint8_t u08Cnt = 2;
    uint8_t first_run=0;
    uint8_t startApp=0;
    while (1)
    {
        if(!first_run)
        {
            qapi_Timer_Sleep(2, QAPI_TIMER_UNIT_SEC, true);
            first_run=1;
            svpp_comm_power_on_wifi_chip();
            power_off_timeout_set_value(240); // 240 s
        }
        else
        {
            qapi_Timer_Sleep(1, QAPI_TIMER_UNIT_MSEC, true);
        }

        if (g_u08MenuProgress == 0)
        {
            selected_menu_item=0;
            if (u08Cnt == 5)
            {
#ifdef MENU_UART
                /*start payment payment on uart display menu*/
                startApp = 'a';
#elif defined(MENU_DISPLAY)
              /*display the menu on modem screen and get selected item*/
                menu_start(&selected_menu_item);
                //              selected_menu_item =2;
                //              get_info();
              startApp =selected_menu_item;
              selected_menu_item=0;
#endif //MENU_UART

                /* send selected item to customer queue*/
                os_itf_queue_msg_t msg_from_main_to_customer_task;
                OS_ITF_QUEUE_API_ARG msg_to_queue =
                        (OS_ITF_QUEUE_API_ARG)&msg_from_main_to_customer_task;
                msg_from_main_to_customer_task.type = MESSAGE_RECV;
                msg_from_main_to_customer_task.size = 1;
                msg_from_main_to_customer_task.payload.pData = &startApp;
                os_itf_queue_send(&customer_app_ctx.queue, msg_to_queue);
                u08Cnt = 0;
            }
            else
            {
                u08Cnt++;
            }
        }
    }
#endif // APP_MODEM_STAND_ALONE

    return 0;
}
#endif
