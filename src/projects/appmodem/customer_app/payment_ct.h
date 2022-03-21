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
* @file: payment_ct.h
*
* @date: May 2021
*
* @author: ktahri <khawla.tahri@ext.youtransactor.com>
*/

/**
 * @addtogroup app_emv_transaction
 * @{
 */

#ifndef _PAYMENT_CT__
#define _PAYMENT_CT__

#include <stdint.h>

#include "payment.h"
#include "ucube_api.h"
#include "ucube_api.h"

//SRED TAGS
#define  TAG_SECURE_5A_APPLICATION_PRIMARY_ACCOUNT_NUMBER           0x5A
#define  TAG_SECURE_57_TRACK_2_EQUIVALENT_DATA                      0x57
#define  TAG_SECURE_56_TRACK_1_DATA                                 0x56
#define  TAG_SECURE_5F20_CARDHOLDER_NAME                            0x5F20
#define  TAG_SECURE_5F24_APPLICATION_EXPIRATION_DATE                0x5F24
#define  TAG_SECURE_5F30_PAYMENT_CODE                               0x5F30
#define  TAG_SECURE_9F0B_CARDHOLDER_NAME_EXTENDED                   0x9F0B
#define  TAG_SECURE_9F6B_TRACK_2_DATA                               0x9F6B

//PLAIN TAGS
#define  TAG_4F_APPLICATION_IDENTIFIER                               0x4F
#define  TAG_50_APPLICATION_LABEL                                    0x50
#define  TAG_5F2A_TRANSACTION_CURRENCY_CODE                          0x5F2A
#define  TAG_5F34_APPLICATION_PRIMARY_ACCOUNT_NUMBER_SEQUENCE_NUMBER 0x5F34
#define  TAG_81_AMOUNT_AUTHORISED                                    0x81
#define  TAG_8E_CARDHOLDER_VERIFICATION_METHOD_LIST                  0x8E
#define  TAG_95_TERMINAL_VERIFICATION_RESULTS                        0X95
#define  TAG_9B_TRANSACTION_STATUS_INFORMATION                       0x9B
#define  TAG_99_TRANSACTION_PERSONAL_IDENTIFICATION_NUMBER_DATA      0x99
#define  TAG_9A_TRANSACTION_DATE                                     0x9A
#define  TAG_9C_TRANSACTION_TYPE                                     0x9C
#define  TAG_9F02_AMOUNT_AUTHORISED                                  0x9F02
#define  TAG_9F06_APPLICATION_IDENTIFIER__TERMINAL                   0x9F06
#define  TAG_9F10_ISSUER_APPLICATION_DATA                            0x9F10
#define  TAG_9F1A_TERMINAL_COUNTRY_CODE                              0x9F1A
#define  TAG_9F26_APPLICATION_CRYPTOGRAM                             0x9F26
#define  TAG_9F27_CRYPTOGRAM_INFORMATION_DATA                        0x9F27
#define  TAG_9F33_TERMINAL_CAPABILITIES                              0x9F33
#define  TAG_9F34_CARDHOLDER_VERIFICATION_METHOD_RESULTS             0x9F34
#define  TAG_9F36_APPLICATION_TRANSACTION_COUNTER                    0x9F36
#define  TAG_DF02_PEK_VERSION_NUMBER                                 0xDF02
#define  TAG_84_APPLICATION_ID                                       0x84
#define  TAG_9F12_APPLICATION_PREFERRED_NAME                         0x9F12
#define  TAG_9F39_POINT_OF_PAYMENT_ENTRY_MODE                        0x9F39
#define  TAG_8A_AUTHORIZATION_RESPONSE_CODE                          0x8A
#define  TAG_91_ISSUER_AUTHENTICATION_DATA                           0x91
#define  TAG_71_ISSUER_SCRIPT_TEMPLATE1                              0x71
#define  TAG_72_ISSUER_SCRIPT_TEMPLATE2                              0x72
#define  TAG_9F6E_NFC_FORM_FORMAT                                    0x9F6E
#define  TAG_5F34_PAN_SEQUENCE_NUMBER                                0x5F34
#define  TAG_DF37_SELECTED_CARDHOLDER_LANGUAGE                       0xDF37
#define  TAG_9F08_APPLICATION_VERSION_NUMBER                         0x9F08
#define  TAG_5F25_APPLICATION_EFFECTIVE_DATE                         0x5F25
#define  TAG_82_APPLICATION_INTERCHANGE_PROFILE                      0x82
#define  TAG_9F07_APPLICATION_USAGE_CONTROL                          0x9F07
#define  TAG_9F37_UNPREDICTABLE_NUMBER                               0x9F37

error_t preparePaymentRequest(UCubePaymentRequest_t * UCubePaymentRequest);
#endif //_PAYMENT_CT__

/* @} */

