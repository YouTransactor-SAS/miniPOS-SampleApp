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
 * @file: config_modem.h
 *
 * @date: 02/03/2020
 *
 * @author: CHHIM Stephane (stephane.chhim@youtransactor.com)
 */

/**
 * @addtogroup config_modem
 * @{
 */

#ifndef __CONFIG_MODEM_H__
#define __CONFIG_MODEM_H__

#include "types.h"
#include "errors.h"
#ifdef CONFIG_MODEM
#elif defined(CONFIG_PC)
#include "alltypes.h"
#endif //CONFIG_XX
#undef APP_MODEM_STAND_ALONE
#define APP_MODEM_STAND_ALONE

#undef SVPP_COM_USE_UART
//#define SVPP_COM_USE_UART

#undef EXAMPLE_HTTP

//#define MENU_UART
#define MENU_DISPLAY
#define SVPP_COM_USE_SPI
#ifdef APP_MODEM_STAND_ALONE
#define FORCE_TX_APPROVED // this is used to force the result of the transaction
#endif // APP_MODEM_STAND_ALONE

// kept for compilation only !!
// TODO: to be removed !!!

//Simulateur
#define ADRESSE_IP_SIMU         "127.0.0.1"
#define LISTEN_PORT             21350

//SSL port
#define PATH_CERT_TMS_PEM_FILE  "/tmp/cert.pem"
#define PATH_BINARY_FILE        "/tmp/binary.hex"
#define PATH_CERT_DONGLE_PATH   "/tmp"
#define ID_TYPE_FOR_REGISTER    0xC3
#define MAX_DEVICE_ID_LEN       30

//Dongle config
#define TERMINAL_ID              "1000091234"
#define WAIT_CARD_TIMEOUT       30

//EEPROM
#define PATH_FILE_TO_SAVE_DATA   "/tmp/data.txt"

//logs
#define PATH_TO_SAVE_LOG   "/tmp/log/"
#define PATH_TO_SAVE_ZIP   "/tmp/"
#define MAX_LOG_FILE_SIZE  10
#define MAX_LOG_FILE       5
#define TMS_PWD                  "gmxsas"


// External Flash config

// DEFINED BY YT
#define CONFIG_HAL_FLASH_SECTOR_BUFFER_SIZE 4096
#define CONFIG_HAL_FLASH_NB_FLASH_DEVICES 1

#define UNUSED(_x_) ((void)(_x_))
#define MIN(a,b)    ((a)<(b)?(a):(b))
#define HAL_PROPERTY_PARAM_CONTACT          "settings/TerminalSettings_database_ct2.bin"
#define HAL_PROPERTY_PARAM_MCL              "settings/TerminalSettings_database_mcl.bin"
#define HAL_PROPERTY_PARAM_VISA             "settings/TerminalSettings_database_pw.bin"
#define HAL_PROPERTY_PARAM_AMEX             "settings/TerminalSettings_database_axp.bin"
#define HAL_PROPERTY_PARAM_INTERAC          "settings/TerminalSettings_database_if.bin"
#define HAL_PROPERTY_PARAM_DISCOVER         "settings/TerminalSettings_database_dp.bin"
#define HAL_PROPERTY_PARAM_PURE             "settings/TerminalSettings_database_pure.bin"
#define HAL_PROPERTY_PARAM_LANGUAGE         "settings/languages.bin"
#define HAL_PROPERTY_PARAM_CA_KEY           "settings/CA_Keys.bin"
#define HAL_PROPERTY_PARAM_REVOCATION_LIST  "settings/revocation_list/revocation_list.bin"
#define HAL_PROPERTY_PARAM_EXCEPTION_FILE   "settings/exception_file/exception_file.bin"
#endif // __CONFIG_MODEM_H__

/* @} */
