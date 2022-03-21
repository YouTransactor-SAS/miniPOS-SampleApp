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
* @file: payment_ct.c
*
* @date: May 2021
*
* @author: ktahri <khawla.tahri@ext.youtransactor.com>
*/

#include "payment_ct.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "errors.h"
#include "quectel_utils.h"
#include "ucube_api.h"
#include "svpp_com.h"
#include "debug.h"
#include "payment.h"
#include "customer_app.h"
#include "cmn_display_result.h"
extern customer_app_ctx_t customer_app_ctx;

uint8_t payment_ct_authorization_serveur_response[] = {0x8A,0x02,0x30,0x30,0x91,0x08,0x01,0x00,0x01,0x00,0x01,0x00,0x02,0x01,0x71,0x01,0x00,0x72,0x01,0x00};

//UCubePaymentContext_t UCubePaymentContext;
uint8_t * authorizationTask (void){
    uint8_t list[5 * SVPP_COM_APP_NAME_LEN + 10];
    uint8_t list_index= 0;
    uint8_t i=0;
    uint8_t selected_item;
    svpp_com_line_desciption_t line;
    error_t err=ERRORNO;
    memset((char*)list,0x00,sizeof((const char*)list));
    memset((uint8_t*)&customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
            tx_msg, 0,
            sizeof(svpp_com_tx_hmi_drv_displaylistboxwithoutki_t));
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    timeout = 90;
    strcpy((char*)&list[list_index],
            (char*)"Approve");
    list_index += strlen((char*)"Approve");
    list[list_index++] = '\n';
    list[list_index++] = 0x00;
    strcpy((char*)&list[list_index],
            (char*)"Decline");
    list_index += strlen((char*)"Decline");
    list[list_index++] = '\n';
    list[list_index++] = 0x00;
    strcpy((char*)&list[list_index],
            (char*)"Unable to go online");
    list_index += strlen((char*)"unable to go online");
    list[list_index++] = '\n';
    list[list_index++] = 0x00;
    strcpy((char*)&list[list_index],
            (char*)"Issuer referral");
    list_index += strlen((char*)"Issuer referral");
    list[list_index++] = '\n';
    list[list_index++] = 0x00;
    strcpy((char*)&list[list_index],
            (char*)"Technical issue");
    list_index += strlen((char*)"technical issue");
    list[list_index++] = '\n';
    list[list_index++] = 0x00;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.pList =
            list;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.list_length =
    		list_index;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.font = 1;
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    title_length =  1;

    svpp_com_send_rpc(&customer_app_ctx.rpc_ctx,
            SVPP_COM_HMI_DRV_DISPLAYLISTBOXWITHOUTKI_CMD_NB,
            SVPP_COM_RPC_NO_TIMEOUT);

    selected_item = customer_app_ctx.rpc_ctx.param->
            hmi_drv_displaylistboxwithoutki.
            rx_msg.selected_item;


    switch(selected_item)
    {
    case 1:
    	payment_ct_authorization_serveur_response[2]  =(uint8_t)
        ((SVPP_COM_HOST_RESP_ACCEPTED & 0xFF00) >> 8);
    	payment_ct_authorization_serveur_response[3]  = (uint8_t)
                                      (SVPP_COM_HOST_RESP_ACCEPTED & 0x00FF);
        break;
    case 2:
    	payment_ct_authorization_serveur_response[2]  =(uint8_t)
        ((SVPP_COM_HOST_RESP_DECLINED & 0xFF00) >> 8);
    	payment_ct_authorization_serveur_response[3]  = (uint8_t)
                                      (SVPP_COM_HOST_RESP_DECLINED & 0x00FF);
        break;
    case 3:
    	payment_ct_authorization_serveur_response[2]  =(uint8_t)
        ((SVPP_COM_HOST_RESP_UNABLE_ONLINE & 0xFF00) >> 8);
    	payment_ct_authorization_serveur_response[3]  = (uint8_t)
                                          (SVPP_COM_HOST_RESP_UNABLE_ONLINE & 0x00FF);
        break;
    case 4:
    	payment_ct_authorization_serveur_response[2]  =(uint8_t)
        ((SVPP_COM_HOST_RESP_ISSUE_REF & 0xFF00) >> 8);
    	payment_ct_authorization_serveur_response[3]  = (uint8_t)
                                          (SVPP_COM_HOST_RESP_ISSUE_REF & 0x00FF);
        break;
    case 5:
    	payment_ct_authorization_serveur_response[2]  =(uint8_t)
        ((SVPP_COM_HOST_RESP_TECH_ISSUE & 0xFF00) >> 8);
    	payment_ct_authorization_serveur_response[3]  = (uint8_t)
                                          (SVPP_COM_HOST_RESP_TECH_ISSUE & 0x00FF);
        break;
    default:
        break;
    }

    return payment_ct_authorization_serveur_response;
}

uint8_t riskManagementTask(void){

    uint8_t list[2 * SVPP_COM_APP_NAME_LEN + 10];
    uint8_t index= 0;
    uint8_t i=0;
    svpp_com_line_desciption_t line;
    uint8_t selected_item=1;
/*
    //    err = clear_screen_msg();
    memset((uint8_t*)&customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
            tx_msg, 0,
            sizeof(svpp_com_tx_hmi_drv_displaylistboxwithoutki_t));
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    timeout = 90;

    strcpy((char*)&list[index],
            (char*)"approve");
    index += strlen((char*)"approve");
    list[index++] = '\n';
    list[index++] = 0x00;

    strcpy((char*)&list[index],
            (char*)"decline");
    index += strlen((char*)"decline");
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
            (char*)"Main Title");
    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
    title_length = strlen((char*)"Main Title") +1;

//    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
//    upper_title_length =  1;
//    customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.tx_msg.
//    sub_title_length =  1;

    svpp_com_send_rpc(&customer_app_ctx.rpc_ctx,
            SVPP_COM_HMI_DRV_DISPLAYLISTBOXWITHOUTKI_CMD_NB,
            SVPP_COM_RPC_NO_TIMEOUT);
    selected_item = customer_app_ctx.rpc_ctx.param->hmi_drv_displaylistboxwithoutki.
            rx_msg.selected_item;
*/
    return selected_item;


}
void applicationSelectionTask(EMVApplicationDescriptor *
        appList, uint8_t *size)
{

    EMVApplicationDescriptor appList_out[APP_AID_LEN];
    uint8_t size_out=0;

    if (appList != NULL) {
        for (int i=0; i<(*size); i++) {
            if (size_out==0) {
                appList_out[i] = appList[i];
                size_out++;
            } else if (appList_out[i].priority >= appList[0].priority) {
                if (appList_out[i].priority == appList[0].priority) {
                    appList_out[i] = appList[i];
                    size_out++;
                }
            }
        }
    }

    memcpy(appList,appList_out,size_out);
    *size =size_out;

}

void onProgress(UCubePaymentContext_t* context,PaymentState* state){

    PRINTF("********Payment State: %s \r\n",
            getStateName(*state));
}

void onFinish(UCubePaymentContext_t* context)
{
    PRINTF("********Payment Status: %s \r\n",
            getStatusName(context->paymentStatus));
}

error_t preparePaymentRequest(UCubePaymentRequest_t * UCubePaymentRequest)
{
    error_t err = ERRORNO;
    Language preferredLanguageList[1];
    Currency CURRENCY_EUR;

    CardReaderType_t card_reader_list;
    CardReaderTypeEnum readerList[2]= {ICC, NFC};
    card_reader_list.p_list = readerList;
    card_reader_list.nb = sizeof(readerList);
    Date date;
    uint16_t authorizationPlainTags[]={
              TAG_4F_APPLICATION_IDENTIFIER,
              TAG_50_APPLICATION_LABEL
    };
    uint16_t authorizationSecuredTags[]={
            TAG_SECURE_5A_APPLICATION_PRIMARY_ACCOUNT_NUMBER,
            TAG_SECURE_57_TRACK_2_EQUIVALENT_DATA,
            TAG_SECURE_56_TRACK_1_DATA  ,
            TAG_SECURE_5F20_CARDHOLDER_NAME  ,
            TAG_SECURE_5F24_APPLICATION_EXPIRATION_DATE ,
            TAG_SECURE_5F30_PAYMENT_CODE ,
            TAG_SECURE_9F0B_CARDHOLDER_NAME_EXTENDED  ,
            TAG_SECURE_9F6B_TRACK_2_DATA
    };
    uint16_t finalizationPlainTags[]={
              TAG_9F06_APPLICATION_IDENTIFIER__TERMINAL,
              TAG_9F10_ISSUER_APPLICATION_DATA,
              TAG_9F1A_TERMINAL_COUNTRY_CODE
    };
    uint16_t finalizationSecuredTags[]={
            TAG_SECURE_5F24_APPLICATION_EXPIRATION_DATE,
            TAG_SECURE_5F30_PAYMENT_CODE
    };

    UCubePaymentRequest->authorizationPlainTags_len =
            sizeof(authorizationPlainTags)/sizeof(uint16_t);
    UCubePaymentRequest->authorizationSecuredTags_len =
            sizeof(authorizationSecuredTags)/sizeof(uint16_t);
    UCubePaymentRequest->finalizationPlainTags_len =
            sizeof(finalizationPlainTags)/sizeof(uint16_t);
    UCubePaymentRequest->finalizationSecuredTags_len =
            sizeof(finalizationSecuredTags)/sizeof(uint16_t);

    CURRENCY_EUR.currency_code[0] = (uint8_t)((SVPP_COM_CUREENCCY_EURO & 0xFF00) >> 8);
    CURRENCY_EUR.currency_code[1] = (uint8_t)(SVPP_COM_CUREENCCY_EURO & 0x00FF);
    CURRENCY_EUR.currency_exp = 2;

    preferredLanguageList[0].language[0] = 'e';
    preferredLanguageList[0].language[1] = 'n';

    UCubePaymentRequest->amount =1;
    UCubePaymentRequest->currency = CURRENCY_EUR;
    UCubePaymentRequest->transactionType = PURCHASE;
    UCubePaymentRequest->readerList = &card_reader_list;
    UCubePaymentRequest->authorizationTask = authorizationTask;
    UCubePaymentRequest->riskManagementTask = riskManagementTask;
    UCubePaymentRequest->applicationSelectionTask = applicationSelectionTask;
    UCubePaymentRequest->preferredLanguageList = preferredLanguageList;

    UCubePaymentRequest->onProgress = onProgress ;
    UCubePaymentRequest->onFinish = onFinish;

     //Add optional variables
    UCubePaymentRequest->forceOnlinePin = FALSE;
  /*  err = get_date(&date);
    if(!err)
    {
       UCubePaymentRequest->transactionDate= date;
    }*/
    UCubePaymentRequest->forceAuthorisation = FALSE;
    UCubePaymentRequest->cardWaitTimeout = 10;
    UCubePaymentRequest->systemFailureInfo2 = FALSE,
    UCubePaymentRequest->forceDebug = FALSE;
    UCubePaymentRequest->skipCardRemoval = TRUE;
    UCubePaymentRequest->authorizationPlainTags = authorizationPlainTags;
    UCubePaymentRequest->authorizationSecuredTags =authorizationSecuredTags;
    UCubePaymentRequest->finalizationPlainTags = finalizationPlainTags;
    UCubePaymentRequest->finalizationSecuredTags =finalizationSecuredTags;
    UCubePaymentRequest->onlinePinBlockFormat = PIN_BLOCK_ISO9564_FORMAT_0;

    UCube_api_pay(UCubePaymentRequest);
    return err;

}
