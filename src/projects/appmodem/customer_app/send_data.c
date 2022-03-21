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
* @file: send_data.c
*
* @date: June 2021
*
* @author: ktahri <khawla.tahri@ext.youtransactor.com>
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "errors.h"

#include "ucube_api.h"
#include "debug.h"
#include "payment.h"

extern payment_ctx_t payment_ctx;

void init_rpc(void)
{
 /*   error_t err;
    uint8_t svpp_version[4];
    svpp_com_transaction_param_u param_ct;
    uint8_t tag =  0xD1;
    param_ct.ident_getinfo.tx_msg.pData = &tag;
    param_ct.ident_getinfo.tx_msg.len_data= sizeof(tag);
    err= UCube_api_send_Data(param_ct,(uint32_t)SVPP_COM_IDENT_GETINFO_CMD_NB);
    param_ct = *payment_ctx.rpc_ctx.param;
    if(err == ERRORNO )
    {
        if((memcmp(param_ct.ident_getinfo.rx_msg.pData,&tag,sizeof(tag)))==0)
        {
            if(param_ct.ident_getinfo.rx_msg.pData[1] ==
                    sizeof(svpp_version))
            {
                memcpy(svpp_version,
                        &param_ct.ident_getinfo.rx_msg.pData[2],
                        sizeof(svpp_version));
                PRINTF("********** SVPP VERSION :");
                PRINTF_HEX_BUFFER(svpp_version,4);
                PRINTF("\r\n :");
            }
        }
    }*/
}
