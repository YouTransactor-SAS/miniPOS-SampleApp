/**
 * Copyright 2006-2018 YOUTRANSACTOR ALL RIGHTS RESERVED. YouTransactor,
 * 32, rue Brancion 75015 Paris France, RCS PARIS: B 491 208 500, YOUTRANSACTOR
 * CONFIDENTIAL AND PROPRIETARY INFORMATION , CONTROLLED DISTRIBUTION ONLY,
 * THEREFORE UNDER NDA ONLY. YOUTRANSACTOR Authorized Parties and who have
 * signed NDA do not have permission to distribute this documentation further.
 * Unauthorized recipients must destroy any electronic and hard-copies
 * and without reading and notify Gregory Mardinian, CTO, YOUTRANSACTOR
 * immediately at gregory.mardinian@youtransactor.com.
 *
 * @file: os_itf.c
 *
 * @date: Jan 2020
 *
 * @author: amoussa <amani.moussa@ext.youtransactor.com>
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "types.h"
#include "errors.h"
#include "common.h"
#include "os_itf.h"
#ifdef CONFIG_PC
#include <sys/time.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/poll.h>
#include <openssl/conf.h>
#include <openssl/engine.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <pthread.h>
#include <curl/curl.h>
#include <sys/poll.h>
#include <netdb.h>
#include <sys/stat.h>
#include "config_pc.h"
#include "types.h"
#include "common.h"
#include "os_itf.h"

#define OS_ITF_HTTP_OK                200

#elif defined(CONFIG_MODEM)
#include "txm_module.h"
#include "quectel_utils.h"
#include "qapi_timer.h"
TX_BYTE_POOL *g_bytePoolTask;    // use to create the byte pool stack of the app
                                // each new task will get its own stack
                                // from this one

char free_memory_task[TASK_BYTE_POOL_SIZE];

#endif // CONFIG_XX

error_t os_itf_create_byte_pool(void)
{
    error_t err = EFAIL;
#ifdef CONFIG_PC
    err = ERRORNO;
#else

    err = txm_module_object_allocate(&g_bytePoolTask, sizeof(TX_BYTE_POOL));
    if(err == TX_SUCCESS)
    {
        err = tx_byte_pool_create(g_bytePoolTask, \
                                    "task application pool", \
                                    free_memory_task, \
                                    TASK_BYTE_POOL_SIZE);
        if (err != TX_SUCCESS)
        {
            PRINTF_OS("os_itf_create_byte_pool, tx_byte_pool_create %d", err);
        }
    }
    else
    {
        PRINTF_OS("os_itf_create_byte_pool, txm_module_object_allocate %d", err);
    }


#endif
    return err;
}

/**
 * @brief   create a task
 * @param   taskid
 *          taskroutine
 *          stacksize
 *          priority
 * @return  error code (0 if no error)
 */
error_t os_itf_task_create(os_itf_task_param_t *pTask)
{
    error_t err = ERRORNO;

#ifdef CONFIG_PC
    pthread_attr_t attr;

    err = pthread_attr_init(&attr);
    PRINTF_VERBOSE("ERROR: pthread_attr_init %d\r\n", err);
    if (err == ERRORNO)
    {
        if (pTask->stacksize > 0)
        {
            err = pthread_attr_setstacksize(&attr, pTask->stacksize);
            PRINTF_VERBOSE("ERROR: in task %s pthread_attr_setstacksize %d\r\n", pTask->threadName,err);

        }
    }
    if (err == ERRORNO)
    {
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        PRINTF_VERBOSE("ERROR: in task %s pthread_attr_setdetachstate %d\r\n", pTask->threadName, err);
        err = pthread_create(&pTask->taskid, &attr, pTask->function, NULL);
        PRINTF_VERBOSE("ERROR: in task %s  pthread_create %d\r\n", pTask->threadName, err);
    }

    if (err == ERRORNO)
    {
        if (pTask->priority)
        {
#ifdef PC_MODEM_SCHED_IS_RR
            err = pthread_setschedprio(pTask->taskid, pTask->priority);
#endif //PC_MODEM_SCHED_IS_RR
        }
    }

    pthread_attr_destroy(&attr);
    PRINTF_VERBOSE("ERROR: in task %s pthread_attr_destroy %d\r\n",pTask->threadName, err);

#elif defined(CONFIG_MODEM)
    // allocate memory for thread handle
    err = txm_module_object_allocate((VOID *)&pTask->threadHandle, \
                                                    sizeof(TX_THREAD));

    // allocate memory for thread entry function @ thread start address
    if (err == TX_SUCCESS)
    {
        err = tx_byte_allocate(g_bytePoolTask, \
                                (VOID **) &pTask->threadStartAddr, \
                                pTask->stacksize, \
                                TX_NO_WAIT);

        // create tx task
        if (err == TX_SUCCESS)
        {
            err = tx_thread_create(pTask->threadHandle, /* TX_THREAD *thread_ptr */
                                    pTask->threadName, /* CHAR *name_ptr */
                                    pTask->function, /* VOID (*function)(ULONG entry_input) */
                                    pTask->entryPoint, /* ULONG entry_input */
                                    pTask->threadStartAddr, /* VOID *stack_start */
                                    pTask->stacksize, /* ULONG stack_size */
                                    pTask->priority, /* UINT priority */
                                    pTask->priority, /* UINT preempt_threshold */
                                    TX_NO_TIME_SLICE, /* ULONG time_slice */
                                    TX_AUTO_START); /* UINT auto_start */

            if (err != TX_SUCCESS)
            {
                PRINTF_OS("ERROR: os_itf_task_create, tx_thread_create %d\r\n", err);
            }
        }
        else
        {
            PRINTF_OS("ERROR: os_itf_task_create, tx_byte_allocate %d\r\n", err);
        }
    }
    else
    {
        PRINTF_OS("ERROR: os_itf_task_create, txm_module_object_allocate %d\r\n", err);
    }
#endif

    return err;
}

/**
 * @brief   change the priority of the task
 * @param   os_itf_task_param_t *pTask
 *          newPriority
 * @return  error code (0 if no error)
 */
error_t os_itf_task_change_priority(os_itf_task_param_t *pTask,
        int32_t newPriority)
{
    error_t err = EFAIL;
#ifdef CONFIG_PC
    err = pthread_setschedprio(pTask->taskid, newPriority);

    if (err != ERRORNO)
    {
        err = -ENULPTR;
    }
#elif defined(CONFIG_MODEM)
    err = tx_thread_priority_change(pTask->threadHandle, newPriority,
            &pTask->priority);
#endif
    return err;
}

/**
 * @brief   sleep system
 * @param   delay (in TickSystem for MODEM ARCH)
 */
void os_itf_task_sleep(int32_t delay)
{
#ifdef CONFIG_PC
    usleep(delay * 1000);
#elif defined(CONFIG_MODEM)
    tx_thread_sleep(delay);
#endif
}

/**
 * @brief   kill the requested task
 * @param   task param
 * @return  error code (0 if no error)
 */
error_t os_itf_task_kill(os_itf_task_param_t *pTask)
{
    error_t err = EFAIL;
#ifdef CONFIG_PC
        err = pthread_cancel(pTask->taskid);
#elif defined(CONFIG_MODEM)
        err = tx_thread_delete(pTask->threadHandle);
#endif
    return err;
}

/**
 * @brief  initialize a queue message
 * @param queue_name
 * @param mq_msgsize
 * @param mq_maxmsg
 * @return ret
 */

error_t os_itf_queue_create(os_itf_queue_t *pQueue, \
                                    const char *queue_name, \
                                    long int mq_msgsize, \
                                    long int mq_maxmsg)
{
    error_t err = ERRORNO;
#ifdef CONFIG_PC
    struct mq_attr attr;
    /* initialize the queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = mq_maxmsg;
    attr.mq_msgsize = mq_msgsize;
    attr.mq_curmsgs = 0;


    mq_unlink(queue_name);
    pQueue->fd = mq_open(queue_name,
            O_CREAT | O_RDWR,
            0666, /*S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP,*/
            &attr);

//    printf("error mq_open is %s\n", strerror(errno));

    if (pQueue->fd == -ENULPTR)
    {
        err = -ENULPTR;
    }
    else
    {
        err = ERRORNO;
    }
    printf("error mq_open for queue %s is %d\n",queue_name, err);


#elif defined(CONFIG_MODEM)
    // allocate memory for queue handle
    err = txm_module_object_allocate(&pQueue->handle, sizeof(TX_QUEUE));

    // allocate memory for queue @ thread start address
    if (err == TX_SUCCESS)
    {
        err = tx_byte_allocate(g_bytePoolTask, \
                                (VOID **) &pQueue->queueStartAddr, \
                                mq_msgsize * mq_maxmsg, \
                                TX_NO_WAIT);

        if (err == TX_SUCCESS)
        {

            //uint32_t message_size = mq_msgsize / sizeof(uint32_t);
            err = tx_queue_create(pQueue->handle, /* TX_QUEUE *queue_ptr */
                                    queue_name, /* CHAR *name_ptr */
                                    TX_1_ULONG, /* UINT message_size */ /* TX_16_ULONG */
                                    pQueue->queueStartAddr, /* VOID *queue_start */
                                    mq_msgsize * mq_maxmsg); /* ULONG queue_size */

            if (err != TX_SUCCESS)
            {
                PRINTF_OS("Error: os_itf_queue_create, tx_queue_create %d", err);
            }
        }
        else
        {
            PRINTF_OS("Error: os_itf_queue_create, tx_byte_allocate %d", err);
        }

    }
    else
    {
        PRINTF_OS("Error: os_itf_queue_create, txm_module_object_allocate %d", err);
    }
#endif
    return err;
}

/**
 * @brief send msg to queue
 * @param queue
 * @param buffer
 * @param size
 * @param max_size
 * @return ret
 */
error_t os_itf_queue_send(os_itf_queue_t *pQueue, OS_ITF_QUEUE_API_ARG msg)
{
    error_t err = EFAIL;
#ifdef CONFIG_PC
    PRINTF("inside msg queue \r\n");
    err = mq_send(pQueue->fd, (char *)&msg, OS_ITF_MAX_QUEUE_MSG_SIZE, 0);
    PRINTF("error in sending queue %d\r\n",err);
#elif defined(CONFIG_MODEM)
    err = tx_queue_send(pQueue->handle, &msg, TX_WAIT_FOREVER);
#endif
    return err;
}

/**
 * @brief  add a message to the queue
 * @param queue
 * @param buffer
 * @param size
 * @return ret
 */
int os_itf_queue_rcv(os_itf_queue_t *pQueue, OS_ITF_QUEUE_API_ARG* pMsg)
{
    int status = -1; // nothing by default
#ifdef CONFIG_PC
    status = mq_receive(pQueue->fd, (char *)pMsg, sizeof(os_itf_queue_msg_t), NULL);
    if (status != -1)
    {
        pMsg->size = status; // for this API, status = nb of bytes received
        status = 0;
    }
#elif defined(CONFIG_MODEM)
    status = tx_queue_receive(pQueue->handle, pMsg, TX_WAIT_FOREVER);
#endif
    return status;
}


int os_itf_queue_rcv_timeout(os_itf_queue_t *pQueue, OS_ITF_QUEUE_API_ARG* pMsg, int timeout_in_tick_os)
{
    int status = -1; // nothing by default
#ifdef CONFIG_PC
    status = mq_receive(pQueue->fd, (char *)pMsg, sizeof(os_itf_queue_msg_t), NULL);
    if (status != -1)
    {
        pMsg->size = status; // for this API, status = nb of bytes received
    }
#elif defined(CONFIG_MODEM)
    status = tx_queue_receive(pQueue->handle, pMsg, timeout_in_tick_os);
#endif
    return status;
}

/**
 * @brief  close the queue
 * @param queue
 * @return ret
 */
error_t os_itf_queue_close(os_itf_queue_t *pQueue)
{
    error_t err = EFAIL;
#ifdef CONFIG_PC
    err = mq_close(pQueue->fd);
#elif defined(CONFIG_MODEM)
    err = tx_queue_delete(pQueue->handle);
#endif
    return err;
}

/**
 * @brief  connect to socket
 * @return err
 */
error_t os_itf_socket_connect(ssl_ctx_t *ssl_ctx, int* sockfd)
{
    error_t err = -ENULPTR;
#ifdef CONFIG_PC
    struct hostent* host;
    int result;
    struct sockaddr_in sockaddr;
    const SSL_METHOD *meth;

    if((host = gethostbyname((char*)ssl_ctx->hostname)) != NULL)
    {
        printf("error gethostbyname is %s\n", strerror(errno));
        ssl_ctx->sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(ssl_ctx->sockfd > 0)
        {
            memset(&sockaddr, 0, sizeof(sockaddr));
            sockaddr.sin_family = AF_INET;
            sockaddr.sin_addr.s_addr = *(long *)(host->h_addr_list[0]);
            sockaddr.sin_port = htons(ssl_ctx->port);
            SSL_library_init();
            SSLeay_add_ssl_algorithms();
            SSL_load_error_strings();
            meth = SSLv23_client_method();
            ssl_ctx->ctx = SSL_CTX_new(meth);
            if(ssl_ctx->ctx)
            {
                err = ERRORNO;
                if(ssl_ctx->double_authen_enable)
                {
                    if(!SSL_CTX_use_certificate_file(ssl_ctx->ctx,
                                            (const char*)ssl_ctx->ca_cert_file,
                                            SSL_FILETYPE_PEM))
                    {
                        err = -EHCINOTCO;
                    }
                    if(err == ERRORNO)
                    {
                        if(!SSL_CTX_use_PrivateKey_file(
                                ssl_ctx->ctx,
                                (const char*)ssl_ctx->client_key_file,
                                SSL_FILETYPE_PEM))
                        {
                            err = -EHCINOTCO;
                        }
                    }
                }
                if((err == ERRORNO) &&
                    (!SSL_CTX_load_verify_locations(
                     ssl_ctx->ctx,
                     (const char*)ssl_ctx->ca_cert_file,
                     NULL)))
                {
                    err = -EHCINOTCO;
                    if(err == ERRORNO)
                    {
                        if(!SSL_CTX_check_private_key(ssl_ctx->ctx))
                        {
                            err = -EHCINOTCO;
                        }
                    }
                }
                if(err == ERRORNO)
                {
                    SSL_CTX_set_verify(ssl_ctx->ctx, SSL_VERIFY_PEER, NULL);
                }
                if(err == ERRORNO)
                {
                    ssl_ctx->ssl = SSL_new(ssl_ctx->ctx);
                    if(ssl_ctx->ssl)
                    {
                        err = ERRORNO;
                        err=connect(ssl_ctx->sockfd,
                                    (struct sockaddr*)&sockaddr,
                                    sizeof(sockaddr));
                        if(err == ERRORNO)
                        {
                            err = -EHCINOTCO;
                            SSL_get_fd(ssl_ctx->ssl  );
                            SSL_set_fd(ssl_ctx->ssl, ssl_ctx->sockfd);
                            if(SSL_connect(ssl_ctx->ssl) == 1)
                            {
                                err = ERRORNO;
                                if(ssl_ctx->double_authen_enable)
                                {
                                    SSL_get0_param(ssl_ctx->ssl);
                                    result =
                                        (int)SSL_get_verify_result(ssl_ctx->ssl);
                                    if(result != X509_V_OK)
                                    {
                                        err = EHCINOTCO;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if(err)
    {
        os_itf_socket_close(*ssl_ctx, ssl_ctx->sockfd);
    }
#elif defined(CONFIG_MODEM)
    // TODO
#endif
    return err;
}

/**
 * @brief  send data to socket
 * @return ret
 */
error_t os_itf_socket_send_data(ssl_ctx_t ssl_ctx, uint8_t* data, uint16_t len)
{
    error_t err = -ENULPTR;
#ifdef CONFIG_PC
    uint16_t size = 0;
    if (data) {
        size = SSL_write(ssl_ctx.ssl, data, len);
    }
    if (size == len) {
        err = ERRORNO;
    }
#elif defined(CONFIG_MODEM)
    // TODO
#endif
    return err;
}

/**
 * @brief  receive data from socket
 * @return nb of received bytes
 */
error_t os_itf_socket_recv_data(ssl_ctx_t ssl_ctx,
                                    uint8_t* data,
                                    uint16_t size)
{
    error_t err = -ENULPTR;
#ifdef CONFIG_PC
    uint16_t len = 0;
    if (data)
    {
        len = SSL_read(ssl_ctx.ssl, data , size);
        if(len == size)
        {
            err = ERRORNO;
        }
    }

#elif defined(CONFIG_MODEM)
    // TODO
#endif
    return err;
}

/**
 * @brief    close socket
 * @return error
 */
void os_itf_socket_close(ssl_ctx_t ssl_ctx, int sockfd)

{
#ifdef CONFIG_PC
    if(ssl_ctx.ctx)
    {
        SSL_CTX_free(ssl_ctx.ctx);
    }
    if(ssl_ctx.ssl)
    {
        SSL_shutdown(ssl_ctx.ssl);
        SSL_free(ssl_ctx.ssl);
    }
    if(sockfd)
    {
        close(sockfd);
    }
    ERR_free_strings();
#elif defined(CONFIG_MODEM)
    // TODO
#endif
}

/**
 * @brief    send POST to TMS server
 * @return error
 */
error_t os_itf_send_curl_post(post_curl_ctx post_ctx)
{
    error_t err = ERRORNO;
#ifdef CONFIG_PC
    CURL *curl;
    CURLcode res;

    struct curl_slist *headers = NULL;
    struct stat file_info;
    FILE *fd;
    uint16_t size, size_file;

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);
    if(post_ctx.content_type)
    {
        headers = curl_slist_append(headers,
                                    (char*)post_ctx.content_type);
    }
    if(post_ctx.device_id)
    {
        headers = curl_slist_append(headers,
                                    (char*)post_ctx.device_id);
    }
    if(post_ctx.id_type)
    {
        headers = curl_slist_append(headers,
                                    (char*)post_ctx.id_type);
    }
    /* get a curl handle */
    curl = curl_easy_init();
    if(curl)
    {
        curl_easy_setopt(curl,
                        CURLOPT_URL,
                        post_ctx.url);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        if(post_ctx.ssl_auth)
        {
            curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
            curl_easy_setopt(curl,CURLOPT_SSLCERT, post_ctx.cert_path);
            curl_easy_setopt(curl, CURLOPT_SSLKEY, post_ctx.cert_path);
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        /* send all data to this function    */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, post_ctx.fcallback);
        if(post_ctx.size)
        {
            curl_easy_setopt(curl,
                            CURLOPT_POSTFIELDS,
                            post_ctx.data);
            /* Now specify the POST data */
            curl_easy_setopt(curl,
                            CURLOPT_POSTFIELDSIZE,
                            post_ctx.size);
            /* Perform the request, res will get the return code */
            res = curl_easy_perform(curl);
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &res);
            /* Check for errors */
            if(res != OS_ITF_HTTP_OK)
            {
                err = res;
            }
        }
        else
        {
            fd = fopen((char*)post_ctx.data, "r");
            if(fstat(fileno(fd), &file_info) == 0)
            {
                size_file = file_info.st_size;
                while(feof(fd) != EOF)
                {
                    err = -ENULPTR;
                    size = fread(post_ctx.data,
                                1,
                                (size_file > 2048) ?
                                2048 : size_file,
                                fd);
                    if(size_file > 2048)
                    {
                        size_file -= 2048;
                    }
                    if(size > 0)
                    {
                        curl_easy_setopt(curl,
                                        CURLOPT_POSTFIELDS,
                                        post_ctx.data);
                        curl_easy_setopt(curl,
                                        CURLOPT_POSTFIELDSIZE,
                                        size);
                    }
                    res = curl_easy_perform(curl);
                    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &res);
                    if(res != OS_ITF_HTTP_OK)
                    {
                        err = res;
                    }
                    if(err != ERRORNO)
                    {
                        break;
                    }
                }
            }
        }
        curl_global_cleanup();
        curl_slist_free_all(headers);
    }
#elif defined(CONFIG_MODEM)
    // TODO
#endif
    return err;
}

/**
 * @brief    init mutex
 * @return error
 */
error_t os_itf_init_mutex(os_itf_mutex_t *mutex, char* name, int priority)
{
#ifdef CONFIG_PC
    return pthread_mutex_init(mutex, NULL);
#elif defined(CONFIG_MODEM)
    return tx_mutex_create(mutex, name, priority);
#endif // CONFIG_XXX
}

/**
 * @brief    lock mutex
 * @return error
 */
error_t os_itf_lock_mutex(os_itf_mutex_t *mutex, unsigned long ulWaitOption)
{
#ifdef CONFIG_PC
    return pthread_mutex_lock(mutex);
#elif defined(CONFIG_MODEM)
    return tx_mutex_get(mutex, ulWaitOption);
#endif // CONFIG_XXX
}

/**
 * @brief    unlock mutex
 * @return error
 */
error_t os_itf_unlock_mutex(os_itf_mutex_t *mutex)
{
#ifdef CONFIG_PC
    return pthread_mutex_unlock(mutex);
#elif defined(CONFIG_MODEM)
    return tx_mutex_put(mutex); // this is the release
#endif // CONFIG_XXX
}

/**
 * @brief    mutex destruction
 * @return error
 */
error_t os_itf_destroy_mutex(os_itf_mutex_t *mutex)
{
#ifdef CONFIG_PC
    return pthread_mutex_destroy(mutex);
#elif defined(CONFIG_MODEM)
    return tx_mutex_delete(mutex);
#endif // CONFIG_XXX
}

/**
 * @brief    mutex destruction
 * @return error
 */
error_t os_itf_delay(int  delay )
{
    error_t ret = ERRORNO;
#ifdef CONFIG_PC
#elif defined(CONFIG_MODEM)
    ret = qapi_Timer_Sleep(delay, QAPI_TIMER_UNIT_MSEC, true);
#endif // CONFIG_XXX
    return ret;
}

#ifdef CONFIG_PC // TODO: remove when all delay are handled through os itf
void qapi_Timer_Sleep(uint32_t nb, uint32_t type, uint32_t unused)
{

}
#endif // CONFIG_PC
