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
* @file: firmware_update.c
*
* @date: June 2021
*
* @author: ktahri <khawla.tahri@ext.youtransactor.com>
*/

#include "firmware_update_ct.h"

#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

// Qualcomm include
#ifdef CONFIG_MODEM
#include "qapi_diag.h"
#include "qapi_fs.h"
#include "qapi_quectel.h"
#elif defined(CONFIG_PC)
#endif //CONFIG_XX
#include "debug.h"
#include "errors.h"
#include "firmware_update.h"
#include "firmware_update_ct.h"
#include "ucube_api.h"
#include "network_manager.h"
#include "flash.h"
#include "flash_hal.h"


#ifdef CONFIG_MODEM
#define    HTTP_REQUEST_URL        "https://mdm-test.youtransactor.com/quectel_application_v1.0.0.4.bin"

void requestResult_firm_up_cb (https_request_handler_t request_handler){

    PRINTF(" call from firmware update callback\r\n");
}
extern firmware_update_handle_t firmware_update_handle;

extern network_handler_t network_handler_ct;
socket_handler_t socket_handler_firmup;
error_t download_firmware(void)
{
    error_t err=ERRORNO;
    connect_interface_cellular(&network_handler_ct);
    memset(socket_handler_firmup.request_url, 0, sizeof(socket_handler_firmup.request_url));
    sprintf((char*)socket_handler_firmup.request_url, HTTP_REQUEST_URL);
    socket_handler_firmup.url_length = strlen((const char *)socket_handler_firmup.request_url);
    socket_handler_firmup.request_type = HTTPS_CLIENT_GET_E ;
    socket_handler_firmup.requestResult_cb = requestResult_firm_up_cb;
    UCube_api_send_http_request(&socket_handler_firmup);
    return err;
}

error_t write_file_ext_flash(uint8_t* file_buff, uint32_t size)
{
    error_t err = ERRORNO;
    unsigned long address = 0x000000;
    unsigned long len_to_send=0;
    uint8_t* file_buffer = file_buff;
    err = FLASH_INIT();

    if(err != 0 ){
        while(1);
    }
    err = FLASH_ERASE_CHIP();
    if(!err)
    {
        err = FLASH_INIT();
    }
    int sequencenbr = size/FLASH_PAGE_SIZE;
    PRINTF("size: %d , sequence nbr : %d\r\n", size, sequencenbr);
    for(int i=0; i<=sequencenbr ; i++)
    {
        PRINTF("**************************************************************write sequence nbr  : %d\r\n", i);
        if(i == sequencenbr)
        {
            len_to_send = size%FLASH_PAGE_SIZE;
        }
        else len_to_send = FLASH_PAGE_SIZE;
        FLASH_WR(address, (char *)file_buffer, len_to_send);
        file_buffer+=len_to_send;
        address+=len_to_send;
    }

    return err;
}

boolean write_update_firmware_file(int32 handle, char* src, uint32 size)
{
    uint32 len;
    boolean ret_val = FALSE;
     ret_val = qapi_FS_Write(handle, src, size, &len);
     if(size == len && ret_val == QAPI_OK)
     {
        return TRUE;
     }
     else
     {
        return FALSE;
    }
}
error_t read_file_ext_flash(uint8_t* file_buff, uint32_t size)
{
    error_t err = ERRORNO;
    uint8_t* file_buffer = file_buff;
    int fd = -1;
    int wr_bytes = 0;
    qapi_FS_Offset_t seek_status = 0;
    struct qapi_FS_Stat_Type_s firmware_stat;
    int firmware_update_handle_read;
    struct qapi_FS_Stat_Type_s sbuf_read;
    uint32 real_read_firmware_update;
    unsigned long len_to_read=0;
    unsigned long address = 0x000000;

    /*open the firmware  file in read/write mode
     */
    err = qapi_FS_Open("/datatx/quectel_application_v1.0.0.4.bin",
            QAPI_FS_O_RDWR_E | QAPI_FS_O_CREAT_E ,
            &fd);

    /* write the firmware in external flash*/

    PRINTF("read file from external flash and write it in FS\r\n");
    int sequencenbr = size/FLASH_PAGE_SIZE;
    for(int i=0; i<=sequencenbr ; i++)
    {
        PRINTF("sequence nbr : %d*\r\n", i);

        if(i == sequencenbr)
        {
            len_to_read = size%FLASH_PAGE_SIZE;
        }
        else len_to_read = FLASH_PAGE_SIZE;

        FLASH_RD(address, (char *)file_buffer, len_to_read);
        err = write_update_firmware_file(fd,
                (char *)file_buffer,
                len_to_read);
        file_buffer+=len_to_read;
        address+=len_to_read;
        PRINTF("update firmware file err = %d \r\n",err);
    }
    err = qapi_FS_Stat("/datatx/quectel_application_v1.0.0.4.bin", &firmware_stat);
     PRINTF("******************* new firmware size %d\r\n",firmware_stat.st_size);
    return err;
}
error_t external_flash_write_firmware(firmware_update_handle_t * firm_upd_handle)
{
    error_t err=ERRORNO;
    int fd = -1;
    int wr_bytes = 0;
    qapi_FS_Offset_t seek_status = 0;
    struct qapi_FS_Stat_Type_s firmware_stat;
    int firmware_update_handle_read;
    struct qapi_FS_Stat_Type_s sbuf_read;
    uint32 real_read_firmware_update;

    /*open the firmware  file in read/write mode
     */
    err = qapi_FS_Open("/datatx/quectel_application_v1.0.0.5.bin",
            QAPI_FS_O_RDWR_E ,
            &fd);
    PRINTF("open firmware file .2 err =  %d \r\n", err);
    err = qapi_FS_Stat("/datatx/quectel_application_v1.0.0.5.bin", &firmware_stat);
    PRINTF("read firmware file stat err = %d size = %d \r\n",err,firmware_stat.st_size);

    /* if firmware exist and file opened, read the firmware
     * then save it in the firmware buffer
     */
    if((err == QAPI_OK) && (-1 != fd))
    {
        PRINTF("before erase firmware buffer  \r\n");
        PRINTF("erase firmware buffer:  \r\n");
        memset(firmware_update_handle.firmware_buff,0,FIRMWARE_BUFF_MAX_LEN);
        err = qapi_FS_Read(fd, firmware_update_handle.firmware_buff,
                firmware_stat.st_size, &real_read_firmware_update    );

        if((err != 0) || (real_read_firmware_update!=firmware_stat.st_size))
        {
            PRINTF("read file external flash  err = %d\r\n",err);
            return -1;
        }

        /* write the firmware in external flash*/
        err=write_file_ext_flash(firmware_update_handle.firmware_buff,firmware_stat.st_size);
        PRINTF("write file external flash  err = %d\r\n",err);
        PRINTF("erase firmware buffer:  \r\n");
    //    memset(firmware_update_handle.firmware_buff,0,FIRMWARE_BUFF_MAX_LEN);
        err=read_file_ext_flash(firm_upd_handle->firmware_buff,firmware_stat.st_size);
        PRINTF("read file external flash  err = %d\r\n",err);
    }
    else
    {
        PRINTF("error !!!\r\n");
    }

    return err;
}

void write_read_file(void)
{
    error_t err=ERRORNO;
    int fd5 = -1;
    int fd4 = -1;
    uint8_t buf5[200];
    uint8_t buf4[200];
    int read4;
    int read5;
    err = qapi_FS_Open("/datatx/quectel_application_v1.0.0.4.bin",
            QAPI_FS_O_RDWR_E ,
            &fd4);
    err = qapi_FS_Read(fd4, buf4,
            200, &read4);

    err = qapi_FS_Open("/datatx/quectel_application_v1.0.0.5.bin",
            QAPI_FS_O_RDWR_E ,
            &fd5);
    err = qapi_FS_Read(fd5,buf5,
            200, &read5);

    PRINTF ("buff4:");
    PRINTF_HEX_BUFFER(buf4,200);
    PRINTF ("\r\n");
    PRINTF ("buff5:");
    PRINTF_HEX_BUFFER(buf5,200);
    PRINTF ("\r\n");

}
error_t firmware_update_init(firmware_update_handle_t * firm_upd_handle){

    error_t err = QAPI_OK;

    external_flash_write_firmware(firm_upd_handle);
    write_read_file();
    //download_firmware();
    //external_flash_write_firmware(firm_upd_handle);
    /* init the firmware path file name */
    strlcpy(firm_upd_handle->path_name,
            FIRMWARE_UPDATE_PATH,
            strlen(FIRMWARE_UPDATE_PATH)+1);
    PRINTF(" firmware path  : %s\r\n", firm_upd_handle->path_name);
    /* initialize the firmware name variable with the new firmware name*/
    memset(firm_upd_handle->firmware_name, 0,
            sizeof(firm_upd_handle->firmware_name));
    strlcpy(firm_upd_handle->firmware_name, FIRMWARE_UPDATE_NAME,
            sizeof(firm_upd_handle->firmware_name));
    PRINTF(" firmware name : %s\r\n",firm_upd_handle->firmware_name);
    err = UCube_api_firmware_update(firm_upd_handle);
    return err;
}
#elif defined(CONFIG_PC)
#endif //CONFIG_XX
