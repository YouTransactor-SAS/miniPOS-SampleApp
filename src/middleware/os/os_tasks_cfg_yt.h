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
 * @file: os_tasks_cfg_yt.h
 *
 * @date: 11/03/2020
 *
 * @author: CHHIM Stephane (stephane.chhim@youtransactor.com)
 */

/**
 * @addtogroup os_sdk_tasks
 * @{
 */

#ifndef __OS_TASKS_CFG_YT_H__
#define __OS_TASKS_CFG_YT_H__

#define TASK_SVPP_COM_STACK                1024*8*4
#define TASK_SVPP_COM_PRIORITY            1 //3
#define TASK_SVPP_COM_THREAD_NAME        "Svpp_Com_Thread"

#define TASK_NETWORK_TMS_STACK            1024*8*4
#define TASK_NETWORK_TMS_PRIORITY        2
#define TASK_NETWORK_TMS_THREAD_NAME    "Network_TMS_Thread"

#define TASK_NETWORK_STACK                1024*320
#define TASK_NETWORK_PRIORITY            3
#define TASK_NETWORK_THREAD_NAME        "Network_Thread"

#define TASK_LOG_MNG_STACK                1024*8*4
#define TASK_LOG_MNG_PRIORITY            2
#define TASK_LOG_MNG_THREAD_NAME        "Log_Mng_Thread"

#endif // __OS_TASKS_CFG_YT_H__

/* @} */
