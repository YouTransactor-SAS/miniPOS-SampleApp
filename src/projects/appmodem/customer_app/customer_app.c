
#include "config.h"
#include "types.h"
#include "errors.h"
#include "common.h"
#ifdef CONFIG_PC
#include "string.h"
#elif defined(CONFIG_MODEM)
#include "qapi_timer.h"
#include "qapi_quectel.h"
#endif // CONFIG_XXX
#include "customer_app.h"
#include "svpp_com_gen.h"
#include "payment.h"
#include "driver_uart.h"
#include "network_tms.h"
#include "yt_atmel_test_lib_itf.h"
#include "svpp_com.h"
#include "menu.h"
#include "ucube_api.h"
#include "payment_ct.h"
#include "network.h"
#include "network_manager.h"
#include "send_data.h"
#include "power_off_timeout.h"
#include "get_info.h"
#include "display.h"
#include "firmware_update.h"
#include "firmware_update_ct.h"
#include "is25lp128.h"
#include "flash.h"
#include "display_list_box.h"
#include "display.h"
#include "communication_menu.h"
#include "customer_app.h"
#include "display_message.h"

customer_app_ctx_t customer_app_ctx = {0};
UCubePaymentRequest_t UCubePaymentRequest_ct;
network_handler_t  network_handler_ct;
socket_handler_t socket_handler_ct;
int i32Amount = 10;
firmware_update_handle_t firmware_update_handle;

void test_Random(void)
{
    uint8_t random[16];
    Util_GetRandom(random, 16);
    PRINTF("RANDOM: ");
    PRINTF_HEX_BUFFER(random, 16);
    PRINTF("\r\n");
}

void customer_display_choices(void)
{
#ifdef MENU_UART
    /* display menu on uart display*/
    PRINTF("    Choose: \r\n");
    PRINTF("    a - launch a contact transaction    \r\n");
    PRINTF("    b - launch a cless transaction      \r\n");
    PRINTF("    e - get info                        \r\n");
    PRINTF("    c - terminal update                 \r\n");
    PRINTF("    d - test Random                     \r\n");
#elif defined(MENU_DISPLAY)
    /*display the menu on modem screen and get selected item*/
    //menu_start();

#endif //MENU_UART
}

/**
 * @brief     main of customer app
 * @return    NULL
 */
TASK_FUNCTION_RETURN_TYPE customer_app_routine(TASK_FUNCTION_ARGS)
{
#ifdef MENU_UART
    customer_display_choices();
#endif //MENU_UART

    // TODO: rework on itf
    OS_ITF_QUEUE_API_ARG msg_from_queue;
//    memset(&customer_app_ctx.msg,0, sizeof(customer_app_ctx.msg));
    int status = -1;
    error_t err = ERRORNO;
    for(;;)
    {
        /*receive in queue menu selected item from main*/
        status = os_itf_queue_rcv(&customer_app_ctx.queue, &msg_from_queue);
        if(!status)
        {
            memcpy(&customer_app_ctx.msg,(OS_ITF_QUEUE_API_ARG *)msg_from_queue/* &msg_from_queue*/, sizeof(os_itf_queue_msg_t));
            switch(customer_app_ctx.msg.type)
            {
            case MESSAGE_RECV:
            {
                char input = (char)customer_app_ctx.msg.payload.pData[0];

#ifdef CONFIG_PC
                /* start the payment according to the selected uart menu item*/
                switch (input)
                {
                case MENU_SELECT_PAY:
                    /* start payment payment*/
                    PRINTF("menu pay \r\n");
                    preparePaymentRequest(&UCubePaymentRequest_ct);
                    //connect_network();
                    break;
                case MENU_SELECT_SET_GET_INFO:
                    /* start get info*/
                	err = get_info();
                    //init_http_request(&socket_handler_ct);
                    break;
                case MENU_SELECT_SET_PWR_OFF_TIMEOUT:
                    /* start set powerOff timeout*/
                    power_off_timeout_set_from_menu();
                    break;
                case MENU_SELECT_COMMUNICATION_MENU:
                    /* start set powerOff timeout*/
//                    run_communication_menu();
                    break;
                case MENU_SELECT_SEND_DATA:
                    /* start set powerOff timeout*/
                    //init_rpc();
                    break;
//                case MENU_SELECT_DISPLAY:
//                    /* start display menu */;
//                    run_display_menu();
                case MENU_SELECT_DISPLAY_LIST_BOX:
                    /* start display list box menu */
                    run_list_box_menu();
                    break;
                case MENU_SELECT_DISPLAY_MENU:
                    /* start display list box menu */
                     run_display_menu();
                    break;
#elif defined(CONFIG_MODEM)
#ifdef MENU_UART
                /* start the payment according to the selected uart menu item*/
                switch(input)
                {
                case 'a':
                    payment_start(MESSAGE_PREPARE_PAYEMENT_CONTEXT);
                    break;
                case 'b':
                    payment_start(MESSAGE_START_NFC_PAYEMENT);
                    break;
                case 'c':
                    network_tms_start(MESSAGE_START_TERMINAL_UPDATE);
                    break;
#elif defined (MENU_DISPLAY)
                /* start the payment according to the selected modem screen menu item*/
                switch (input)
                {
                case MENU_SELECT_PAY:
                    /* start payment payment*/
                    preparePaymentRequest(&UCubePaymentRequest_ct);
                    break;
                case MENU_SELECT_SET_GET_INFO:
                    /* start get info*/
                    err = get_info();
                    break;
                case MENU_SELECT_SET_PWR_OFF_TIMEOUT:
                    /* start set powerOff timeout*/
                    power_off_timeout_set_from_menu();
                    break;
                case MENU_SELECT_COMMUNICATION_MENU:
                    /* start set powerOff timeout*/
                    run_communication_menu();
                    break;
                case MENU_SELECT_SEND_DATA:
                    /* start set powerOff timeout*/
                    uint8_t msg[] = "This is a message example";
                    display_message(msg, sizeof(msg),0xFF, 0xFF,50);
                    break;
                case MENU_SELECT_DISPLAY:
                    /* start display menu */
                    display_message_menu_item();
                    break;
                case MENU_SELECT_DISPLAY_LIST_BOX:
                    /* start display list box menu */
                    run_list_box_menu();
                    break;
                case MENU_SELECT_DISPLAY_MENU:
                    /* start display list box menu */
                     run_display_menu();
                    break;
#endif //MENU_XX
#endif //CONFIG_XX
                default:
                {
#ifdef APP_MODEM_STAND_ALONE
                    PRINTF_YT("Wrong key input %d\r\n", input);
//                    selected_menu_item =0;
//                    /* redisplay menu to select new item */
//                    customer_display_choices();
#endif // APP_MODEM_STAND_ALONE
                    break;
                }
                memset(&customer_app_ctx.msg,0, sizeof(customer_app_ctx.msg));
                }
                break;
            }

            default:
                break;
            }
#ifdef MENU_UART

            // queue the next rx buffer
            driver_uart_queue_rx_data(e_Uart_Debug);
#endif //MENU_UART
        }

    }
    TASK_FUNCTION_RETURN_INSTRUCTION
}

/**
 * @brief     intialize customer app
 * create customer task and queue
 * @return    error
 */
error_t customer_app_init(void)
{
    error_t err = EFAIL;

    /* create queue for customer task*/
    err = os_itf_queue_create(&customer_app_ctx.queue,
                                QUEUE_CUSTOMER_APP_NAME,
                                OS_ITF_MAX_QUEUE_MSG_SIZE,
                                CUSTOMER_APP_MAX_MESSAGE_IN_QUEUE);

    if (err == ERRORNO)
    {
        customer_app_ctx.task.stacksize = TASK_CUSTOMER_APP_STACK;
        customer_app_ctx.task.priority = TASK_CUSTOMER_APP_PRIORITY;
        customer_app_ctx.task.function = customer_app_routine;
        memcpy(customer_app_ctx.task.threadName, \
                TASK_CUSTOMER_APP_THREAD_NAME, \
                strlen(TASK_CUSTOMER_APP_THREAD_NAME));
        /* create  customer task*/
        err = os_itf_task_create(&customer_app_ctx.task);

        if (err != ERRORNO)
        {
            PRINTF_VERBOSE("ERROR: customer_app_init, os_itf_task_create %d", err);
        }
    }
    else
    {
        PRINTF_VERBOSE("ERROR: customer_app_init, os_itf_queue_create %d", err);
    }

    return err;
}

/**
 * @brief     stop customer app
 * @return    None
 */
void customer_app_stop(void)
{
    os_itf_task_kill(&customer_app_ctx.task);
    os_itf_queue_close(&customer_app_ctx.queue);
}

