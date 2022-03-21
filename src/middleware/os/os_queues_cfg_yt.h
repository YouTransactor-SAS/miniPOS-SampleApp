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
 * @file: os_queues_cfg_yt.h
 *
 * @date: 11/03/2020
 *
 * @author: CHHIM Stephane (stephane.chhim@youtransactor.com)
 */

/**
 * @addtogroup os_sdk_queues
 * @{
 */

#ifndef __OS_QUEUES_CFG_YT_H__
#define __OS_QUEUES_CFG_YT_H__

#define QUEUE_NETWORK_TMS_NAME                "/NETWORK_TMS_QUEUE"
#define NETWORK_TMS_MAX_MESSAGE_IN_QUEUE    20

#define QUEUE_NETWORK_NAME                    "/NETWORK_QUEUE"
#define NETWORK_MAX_MESSAGE_IN_QUEUE        20

#define QUEUE_SVPP_COM_NAME                    "/SVPP_COM_QUEUE"
#define QUEUE_SVPP_COM_NAME_RX                "/SVPP_COM_QUEUE_RX"

#define SVPP_COM_MAX_MESSAGE_IN_QUEUE        20

#define QUEUE_WIFI_COM_NAME                    "/WIFI_COM_QUEUE"
#define QUEUE_WIFI_COM_NAME_RX                "/WIFI_COM_QUEUE_RX"

#define WIFI_COM_MAX_MESSAGE_IN_QUEUE        20

/* @} */

#endif // __OS_QUEUES_CFG_YT_H__
