/**
 * Copyright 2006-2025 YOUTRANSACTOR ALL RIGHTS RESERVED. YouTransactor,
 * 32, rue Brancion 75015 Paris France, RCS PARIS: B 491 208 500, YOUTRANSACTOR
 * CONFIDENTIAL AND PROPRIETARY INFORMATION , CONTROLLED DISTRIBUTION ONLY,
 * THEREFORE UNDER NDA ONLY. YOUTRANSACTOR Authorized Parties and who have
 * signed NDA do not have permission to distribute this documentation further.
 * Unauthorized recipients must destroy any electronic and hard-copies
 * and without reading and notify Gregory Mardinian, CTO, YOUTRANSACTOR
 * immediately at gregory.mardinian@youtransactor.com.
 *
 * @file: os_tasks_cfg_costumer.h
 *
 * @date: 23/07/2020
 *
 * @author: CHHIM Stephane (stephane.chhim@youtransactor.com)
 */

/**
 * @addtogroup os_sdk_tasks
 * @{
 */

#ifndef __OS_TASKS_CFG_CUSTOMER_H__
#define __OS_TASKS_CFG_CUSTOMER_H__

/* Define here all the tasks you want to create, examples below */

#define TASK_PAYMENT_STACK                1024*8*4
#define TASK_PAYMENT_PRIORITY            2
#define TASK_PAYMENT_THREAD_NAME        "Payment_Thread"

#define TASK_CUSTOMER_APP_STACK            1024*8*4
#define TASK_CUSTOMER_APP_PRIORITY        2
#define TASK_CUSTOMER_APP_THREAD_NAME    "Customer_App_Thread"


#define TASK_SPI_WIFI_STACK            4096*8*6
#define TASK_SPI_WIFI_PRIORITY        2
#define TASK_SPI_WIFI_THREAD_NAME    "Spi_Wifi_Thread"

#endif // __OS_TASKS_CFG_CUSTOMER_H__

/* @} */