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
 * @file: os_queues_common.h
 *
 * @date: 23/07/2020
 *
 * @author: CHHIM Stephane (stephane.chhim@youtransactor.com)
 */

/**
 * @addtogroup os_sdk_queues
 * @{
 */

#ifndef __OS_QUEUES_COMMON_H__
#define __OS_QUEUES_COMMON_H__

#include "os_queues_cfg_yt.h"
#include "os_queues_cfg_customer.h"
#include "common.h"
#define OS_ITF_MAX_QUEUE_MSG_DATA            128
#define OS_ITF_MAX_PATH_LEN                    255
#define OS_ITF_MAX_QUEUE_MSG_SIZE            sizeof(os_itf_queue_msg_t)

#pragma pack(1)
typedef union
{
    uint8_t *pData;
    uint8_t data[OS_ITF_MAX_QUEUE_MSG_DATA];
} msg_payload;

typedef struct
{
    message_list_e type;
    msg_payload payload;
    int size;
} os_itf_queue_msg_t;
#pragma pack(0)

#ifdef CONFIG_PC
#define OS_ITF_QUEUE_API_ARG                os_itf_queue_msg_t
#define OS_ITF_QUEUE_API_ARG_PTR_CAST        (OS_ITF_QUEUE_API_ARG *)&
#elif defined(CONFIG_MODEM)
#define OS_ITF_QUEUE_API_ARG                uint32_t
#define OS_ITF_QUEUE_API_ARG_PTR_CAST        (OS_ITF_QUEUE_API_ARG *)
#endif


#endif // __OS_QUEUES_COMMON_H__

/* @} */
