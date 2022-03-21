/**
* Copyright 2020-2030 YOUTRANSACTOR ALL RIGHTS RESERVED. YouTransactor,
* 32, rue Brancion 75015 Paris France, RCS PARIS: B 491 208 500, YOUTRANSACTOR
* CONFIDENTIAL AND PROPRIETARY INFORMATION , CONTROLLED DISTRIBUTION ONLY,
* THEREFORE UNDER NDA ONLY. YOUTRANSACTOR Authorized Parties and who have
* signed NDA do not have permission to distribute this documentation  further.
* Unauthorized recipients must destroy any electronic and hard-copies
* and without reading and notify Gregory Mardinian, CTO, YOUTRANSACTOR
* immediately at gregory.mardinian@youtransactor.com.
 *
 * @file: os_itf.h
 *
 * @date: jan 2020
 *
 * @author: amoussa <amani.moussa@ext.youtransactor.com>
 */

#ifndef __OS_ITF__
#define __OS_ITF__

#include "os_tasks_common.h"
#include "os_queues_common.h"
#include "errors.h"

/**
 * @addtogroup os_interface
 * @{
 */

#define TASK_NAME_MAX_LEN        100

#ifdef CONFIG_PC
#include <pthread.h>
#include <mqueue.h>
#include <ossl_typ.h>


#define TASK_FUNCTION_RETURN_TYPE            void *
#define TASK_FUNCTION_ARGS                    void *args
#define TASK_FUNCTION_RETURN_INSTRUCTION    return NULL;

typedef size_t(*curl_callback)(void *contents,
                                size_t size,
                                size_t nmemb,
                                void *userp);

#pragma pack(1)

typedef struct
{
    mqd_t fd;
} os_itf_queue_t;

typedef struct
{
    pthread_t taskid;
    TASK_FUNCTION_RETURN_TYPE(*function)(TASK_FUNCTION_ARGS);
    uint32_t stacksize;
    int32_t  priority;
    char threadName[TASK_NAME_MAX_LEN]; // not used but keep for compatibility
} os_itf_task_param_t;

//SSL
typedef struct
{
    uint16_t port;
    uint8_t double_authen_enable;
    uint8_t ca_cert_file[OS_ITF_MAX_PATH_LEN];
    uint8_t ca_path[OS_ITF_MAX_PATH_LEN];
    uint8_t client_key_file[OS_ITF_MAX_PATH_LEN];
    uint8_t hostname[OS_ITF_MAX_PATH_LEN];
    SSL*    ssl;
    SSL_CTX* ctx;
    uint32_t sockfd;
}ssl_ctx_t;

typedef struct
{
    uint8_t* url;
    uint8_t* content_type;
    uint8_t* device_id;
    uint8_t* id_type;
    curl_callback fcallback;
    uint8_t ssl_auth;
    uint8_t* data;
    uint16_t size;
    uint8_t* cert_path;
}post_curl_ctx;

typedef pthread_mutex_t os_itf_mutex_t;
#pragma pack(0)

#elif defined(CONFIG_MODEM)
#include "tx_api.h"

#define TASK_FUNCTION_RETURN_TYPE            void
#define TASK_FUNCTION_ARGS                    ULONG entryPoint
#define TASK_FUNCTION_RETURN_INSTRUCTION    return;

#pragma pack(1)

typedef TX_MUTEX os_itf_mutex_t;

typedef struct
{
    TX_QUEUE* handle;
    void* queueStartAddr;
} os_itf_queue_t;

// mutex

typedef struct
{
    TX_THREAD* threadHandle;
    char threadName[TASK_NAME_MAX_LEN];
    void (*function)(TASK_FUNCTION_ARGS);
    TASK_FUNCTION_ARGS;
    void *threadStartAddr;
    ULONG stacksize;
    UINT priority;
} os_itf_task_param_t;

// TODO: to be replaced
//SSL
typedef struct
{
  uint16_t port;
  uint8_t enable;
  uint8_t double_authen_enable;
  uint8_t* ca_cert;
  uint8_t* ca_path;
  uint8_t* client_cert;
  uint8_t* client_key;
  uint8_t* hostname;
} ssl_ctx_t;


typedef size_t(*curl_callback)(void *contents,
                                size_t size,
                                size_t nmemb,
                                void *userp);

typedef struct
{
    uint8_t* url;
    uint8_t* content_type;
    uint8_t* device_id;
    uint8_t* id_type;
    curl_callback fcallback;
    uint8_t ssl_auth;
    uint8_t* data;
    uint16_t size;
    uint8_t* cert_path;
}post_curl_ctx;
#pragma pack (0)

#else
#error NO_CONFIG_HAS_BEEN_SELECTED
#endif // CONFIG_PC

/**
 * @brief   create a byte pool for the application
 * @details only for MODEM ARCH
 * @return  error code (0 if no error)
 */
error_t os_itf_create_byte_pool(void);

/**
 * @brief   create a pthread task
 * @param   taskid
 *          taskroutine
 *          stacksize
 *          priority
 * @details set pTask with API "os_itf_task_set_param" first !!
 * @return  error code (0 if no error)
 */
error_t os_itf_task_create(os_itf_task_param_t *pTask);

/**
 * @brief   change the priority of the task
 * @param   taskid
 *          priority
 * @return  error code (0 if no error)
 */
error_t os_itf_task_change_priority(os_itf_task_param_t *pTask, \
                                    int32_t newPriority);

/**
 * os_itf_task_sleep_ms
 *
* @brief   sleep system
 * @param   delay
 * @return  none
 */
void os_itf_task_sleep_ms(int32_t delay_ms);
void os_itf_task_sleep_ms_no_print(int32_t delay_ms);

/**
 * @brief   kill the requested task
 * @param   task param
 * @return  error code (0 if no error)
 */
error_t os_itf_task_kill(os_itf_task_param_t *pTask);

/**
 * os_itf_queue_create
 *
* @brief  create queue
 * @param pQueue, queue_name, mq_msgsize, mq_maxmsg
 * @return ret
 */
error_t os_itf_queue_create(os_itf_queue_t *pQueue, \
                                    const char *queue_name, \
                                    long int mq_msgsize, \
                                    long int mq_maxmsg);

/**
 * @brief  send msg to queue
 * @param queue, buffer, size
 * @return ret
 */
error_t os_itf_queue_send(os_itf_queue_t *pQueue, OS_ITF_QUEUE_API_ARG msg);


//error_t os_itf_queue_send1(os_itf_queue_t *pQueue, uint32_t addr);
//int os_itf_queue_rcv1(os_itf_queue_t *pQueue, uint32_t *pAddr);


/**
 * @brief  receive a msg from the queue
 * @param queue, msg
 * @return ret
 */
int os_itf_queue_rcv(os_itf_queue_t *pQueue, OS_ITF_QUEUE_API_ARG* msg);
int os_itf_queue_rcv_timeout(os_itf_queue_t *pQueue,
        OS_ITF_QUEUE_API_ARG* pMsg, int timeout_in_tick_os);

/**
 * @brief  close the queue
 * @param queue
 * @return ret
 */
error_t os_itf_queue_close(os_itf_queue_t *pQueue);

/**
 * @brief  connect to socket
 * @return ret
 */
error_t os_itf_socket_connect(ssl_ctx_t *ssl, int* sockfd);

/**
 * @brief  send data to socket
 * @return ret
 */
error_t os_itf_socket_send_data(ssl_ctx_t ssl_ctx,
                                uint8_t* data,
                                uint16_t len);

/**
 * @brief  receive data from socket
 * @return nb of received bytes
 */
error_t os_itf_socket_recv_data(ssl_ctx_t ssl_ctx,
                                 uint8_t* data,
                                 uint16_t size);

/**
 * @brief  rclose socket
 */
void os_itf_socket_close(ssl_ctx_t ssl_ctx, int sockfd);

/**
 * @brief    send POST to TMS server
 * @return error
 */
error_t os_itf_send_curl_post(post_curl_ctx post_ctx);

/**
 * @brief    init mutex
 * @return error
 */
error_t os_itf_init_mutex(os_itf_mutex_t *mutex, char* name, int priority);

/**
 * @brief    lock mutex
 * @return error
 */
error_t os_itf_lock_mutex(os_itf_mutex_t *mutex, unsigned long ulWaitOption);

/**
 * @brief    unlock mutex
 * @return error
 */
error_t os_itf_unlock_mutex(os_itf_mutex_t *mutex);

/**
 * @brief    mutex destruction
 * @return error
 */
error_t os_itf_destroy_mutex(os_itf_mutex_t *mutex);

/**
 * @brief    delay
 * @return error
 */
error_t os_itf_delay(int  delay_ms);

/* @} */
#endif // __OS_ITF__
