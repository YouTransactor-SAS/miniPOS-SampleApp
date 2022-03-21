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
* @file: get_info.c
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
#include "get_info.h"
#include "payment.h"
#include "customer_app.h"

extern uint8_t g_u08MenuProgress;
extern customer_app_ctx_t customer_app_ctx;
extern payment_ctx_t payment_ctx;

/**
 * @brief    send getinfo
 * @return    error
 */
error_t get_info(void)
{
    error_t err = ERRORNO;
#ifdef APP_MODEM_STAND_ALONE
    g_u08MenuProgress = 1;
#endif // APP_MODEM_STAND_ALONE
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

    uint8_t tag_selected=0;
    uint8_t tag=0;
    if((err == ERRORNO) || (err == -EBADSTATE))
    {
        err = ERRORNO;
        customer_app_ctx.rpc_err = err;
        PRINTF("*** Progress :GET INFO  ***\r\n");
        do {
			err = get_info_select(&tag_selected);
			switch (tag_selected)
			{
			case 1:
				tag =SVPP_COM_TAG_INFO_SVPP_VERSION;
				break;
			case 2:
				tag =SVPP_COM_TAG_INFO_EMVL1_VERSION;
				break;
			case 3:
				tag = SVPP_COM_TAG_INFO_EMVL2_VERSION;
				break;
			case 4:
				tag =SVPP_COM_TAG_INFO_POW_TIMEOUT;
				break;
			default:
				PRINTF("error select get info\r\n");
				break;
			}
			if(!err)
			{
				err = payment_svpp_get_info(&tag);
			}
        }while(!err);
    }
    PRINTF("GET Info Complete \r\n");
#ifdef APP_MODEM_STAND_ALONE
    g_u08MenuProgress = 0;
#endif // APP_MODEM_STAND_ALONE
    return err;
}

/*
 * @brief    send the result of get info
 * @return    error
 */
error_t cmn_display_result_get_info(uint8_t *msg)
{
    error_t err = ERRORNO;
    svpp_com_line_desciption_t line;
    memset((uint8_t*)&customer_app_ctx.rpc_ctx.param->hmi_drv_displaymessagewithoutki.
            tx_msg, 0,
            sizeof(svpp_com_tx_hmi_drv_displaymessagewithoutki_t));
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaymessagewithoutki.tx_msg.
        timeout = 0xFF; // in sec
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaymessagewithoutki.tx_msg.
        number_of_lines_to_display = 1;
    line.font = 0;
    line.len = strlen((char*)msg) + 1; // +1 is for '\0'
    line.pText = msg;
    line.x_coord = 4;
    line.y_coord = 4;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaymessagewithoutki.tx_msg.pLine =
        &line;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaymessagewithoutki.tx_msg.index_view =
            17;

    customer_app_ctx.rpc_ctx.param->hmi_drv_displaymessagewithoutki.tx_msg.
    abort_key = SVPP_COM_ABORT_WITH_ABORT_KEY;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaymessagewithoutki.tx_msg.
    clear_display_config = ((1<<SVPP_COM_YES_NO_PROMPT) |SVPP_COM_CLEAR_DISPLAY);
    err = svpp_com_send_rpc(&customer_app_ctx.rpc_ctx,
                            SVPP_COM_HMI_DRV_DISPLAYMESSAGEWITHOUTKI_CMD_NB,
                            SVPP_COM_RPC_DEFAULT_TIMEOUT);

    return err;
}
/*
 * @brief     get svpp version
 * @return    error
 */
error_t payment_svpp_get_info(uint8_t* tag_)
{
    error_t err = ERRORNO;
    uint8_t tag =  *tag_;

    customer_app_ctx.rpc_ctx.param->ident_getinfo.tx_msg.pData = &tag;
    customer_app_ctx.rpc_ctx.param->ident_getinfo.tx_msg.len_data= sizeof(tag);
    err = svpp_com_send_rpc(&customer_app_ctx.rpc_ctx,
            SVPP_COM_IDENT_GETINFO_CMD_NB,
            SVPP_COM_RPC_DEFAULT_TIMEOUT);
    PRINTF("SVPP VERSION complete  err :%d  , rpc err :%d\r\n",
            err,customer_app_ctx.rpc_err);

    if((err == ERRORNO) && (customer_app_ctx.rpc_err == ERRORNO))
    {
        err = -EBADDATA;
        if(memcmp(customer_app_ctx.rpc_ctx.param->ident_getinfo.rx_msg.pData,
                (uint8_t*)&tag,
                sizeof(tag)) == 0)
        {
            switch (tag)
            {
            case SVPP_COM_TAG_INFO_SVPP_VERSION:
                if(customer_app_ctx.rpc_ctx.param->ident_getinfo.rx_msg.pData[1] ==
                        sizeof(payment_ctx.pay_ctx.svpp_version))
                {
                    memcpy(payment_ctx.pay_ctx.svpp_version,
                            &customer_app_ctx.rpc_ctx.param->ident_getinfo.rx_msg.pData[2],
                            sizeof(payment_ctx.pay_ctx.svpp_version));
                    PRINTF("SVPP VERSION : \r\n");
                    PRINTF_HEX_BUFFER(payment_ctx.pay_ctx.svpp_version,4);
                    uint8_t msg[40];
                    uint8_t size = sprintf((char*)msg, "SVPP VERSION : \n");
                    for(int i=0;i<4;i++)
                    {
                        size += sprintf((char*)&msg[size], "%d",payment_ctx.pay_ctx.svpp_version[i]);
                        if(i<3)
                        {size += sprintf((char*)&msg[size], "%s",".");}

                    }
                    err = cmn_display_result_get_info(msg);
                }
                break;
            case SVPP_COM_TAG_INFO_EMVL1_VERSION:
                if(customer_app_ctx.rpc_ctx.param->ident_getinfo.rx_msg.pData[1] ==
                        sizeof(payment_ctx.pay_ctx.emvl1_version))
                {
                    memcpy(payment_ctx.pay_ctx.emvl1_version,
                            &customer_app_ctx.rpc_ctx.param->ident_getinfo.rx_msg.pData[2],
                            sizeof(payment_ctx.pay_ctx.emvl1_version));
                    PRINTF("EMVL1 VERSION : \r\n");
                    PRINTF_HEX_BUFFER(payment_ctx.pay_ctx.emvl1_version,4);
                    uint8_t msg[20];
                    uint8_t size = sprintf((char*)msg, "EMVL1 VERSION : \n");
                    for(int i=0;i<4;i++)
                    {
                        size += sprintf((char*)&msg[size], "%d",payment_ctx.pay_ctx.emvl1_version[i]);
                        if(i<3)
                        {size += sprintf((char*)&msg[size], "%s",".");}
                    }
                    err = cmn_display_result_get_info(msg);
                }
                break;
            case SVPP_COM_TAG_INFO_EMVL2_VERSION:
                if(customer_app_ctx.rpc_ctx.param->ident_getinfo.rx_msg.pData[1] ==
                        sizeof(payment_ctx.pay_ctx.emvl2_version))
                {
                    memcpy(payment_ctx.pay_ctx.emvl2_version,
                            &customer_app_ctx.rpc_ctx.param->ident_getinfo.rx_msg.pData[2],
                            sizeof(payment_ctx.pay_ctx.emvl2_version));
                    PRINTF("EMVL2 VERSION : \r\n");
                    PRINTF_HEX_BUFFER(payment_ctx.pay_ctx.emvl2_version,4);
                    uint8_t msg[20];
                    uint8_t size = sprintf((char*)msg, "EMVL2 VERSION : \n");
                    for(int i=0;i<4;i++)
                    {
                        size += sprintf((char*)&msg[size], "%d",payment_ctx.pay_ctx.emvl2_version[i]);
                        if(i<3)
                        {size += sprintf((char*)&msg[size], "%s",".");}

                    }
                    err = cmn_display_result_get_info(msg);
                }
                break;
            case SVPP_COM_TAG_INFO_POW_TIMEOUT:
                if(customer_app_ctx.rpc_ctx.param->ident_getinfo.rx_msg.pData[1] ==
                        sizeof(payment_ctx.pay_ctx.poweroff_timeout))
                {
                    memcpy(&payment_ctx.pay_ctx.poweroff_timeout,
                            &customer_app_ctx.rpc_ctx.param->ident_getinfo.rx_msg.pData[2],
                            sizeof(payment_ctx.pay_ctx.poweroff_timeout));
                    uint8_t msg[40];
                    uint8_t size = sprintf((char*)msg, "Power Off Timeout : \n");
                    size += sprintf((char*)&msg[size], "%d",payment_ctx.pay_ctx.poweroff_timeout);
                    PRINTF_YT("Power Off Timeout : %d\r\n",payment_ctx.pay_ctx.poweroff_timeout);
                    err = cmn_display_result_get_info(msg);
                }
                break;
            default:
                PRINTF("error select get info\r\n");
                break;
            }

        }
    }
    return err;
}

/**
 * @brief     get info select responde
 * @return    error
 */
error_t get_info_select(uint8_t* tag)
{
    uint8_t list[4 * SVPP_COM_APP_NAME_LEN + 10];
    uint8_t index= 0;
    uint8_t i=0;;
    error_t err=ERRORNO;
    uint8_t selected_tag=0;
    svpp_com_line_desciption_t line;
    //    err = clear_screen_msg();
    memset((uint8_t*)&customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
            tx_msg, 0,
            sizeof(svpp_com_tx_hmi_drv_displaylistboxwithoutki_t));
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    timeout = 90;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.font =1;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    title_length =  1;

    list[index++] = (i + 1) + 0x30;
        list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"SVPP version");
    index += strlen((char*)"SVPP version");
    list[index++] = 0x00;

    i++;
    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"EMVL1 version");
    index += strlen((char*)"EMVL1 version");
    list[index++] = 0x00;

    i++;
    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"EMVL2 version");
    index += strlen((char*)"EMVL2 version");
    list[index++] = 0x00;

    i++;
    list[index++] = (i + 1) + 0x30;
    list[index++] = '-';
    strcpy((char*)&list[index],
            (char*)"PwrOff timeout");
    index += strlen((char*)"PwrOff timeout");
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
        *tag = customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
                rx_msg.selected_item;
    }


    return err;
}
