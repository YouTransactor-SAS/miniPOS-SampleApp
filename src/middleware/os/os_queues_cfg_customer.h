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
 * @file: os_queues_cfg_costumer.h
 *
 * @date: 23/07/2020
 *
 * @author: CHHIM Stephane (stephane.chhim@youtransactor.com)
 */

/**
 * @addtogroup os_sdk_queues
 * @{
 */

#ifndef __OS_QUEUES_CFG_CUSTOMER_H__
#define __OS_QUEUES_CFG_CUSTOMER_H__

/* Define here all the queues you want to create, examples below: */

#define QUEUE_PAYMENT_NAME                    "/PAYMENT_QUEUE"
#define PAYMENT_MAX_MESSAGE_IN_QUEUE        10

#define QUEUE_CUSTOMER_APP_NAME                "/CUSTOMER_APP_QUEUE"
#define CUSTOMER_APP_MAX_MESSAGE_IN_QUEUE    10


#define QUEUE_SPI_WIFI_NAME                    "/SPI_TASK_QUEUE"
#define SPI_WIFI_MAX_MESSAGE_IN_QUEUE         10

#endif // __OS_QUEUES_CFG_CUSTOMER_H__
