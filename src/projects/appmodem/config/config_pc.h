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
 * @file: config_pc.h
 *
 * @date:
 *
 * @author:
 */

/**
 * @addtogroup config_pc
 * @{
 */

#ifndef __CONFIG_PC_H__
#define __CONFIG_PC_H__

#ifdef CONFIG_PC
#include <string.h>
#include <comm/cm_comm.h>
#include <errors.h>

//Simulateur
#define ADRESSE_IP_SIMU         "127.0.0.1"
#define LISTEN_PORT             21350

//SSL port
#define PATH_CERT_TMS_PEM_FILE  "/tmp/cert.pem"
#define PATH_BINARY_FILE        "/tmp/binary.hex"
#define PATH_CERT_DONGLE_PATH   "/tmp"
#define ID_TYPE_FOR_REGISTER    0xC2
#define MAX_DEVICE_ID_LEN       30

//Dongle config
#define WAIT_CARD_TIMEOUT       30

//EEPROM
#define PATH_FILE_TO_SAVE_DATA   "/tmp/data.txt"

//logs
#define PATH_TO_SAVE_LOG   "/tmp/log/"
#define PATH_TO_SAVE_ZIP   "/tmp/"
#define MAX_LOG_FILE_SIZE  10
#define MAX_LOG_FILE       5
#define TMS_PWD                  "gmxsas"

#define true 1
#define false 0
#define QAPI_TIMER_UNIT_MSEC 0
// to be removed when os_itf is used for delays and yield
void qapi_Timer_Sleep(uint32_t nb, uint32_t type, uint32_t unused);

#endif // CONFIG_PC
#endif // __CONFIG_PC_H__

/* @} */
