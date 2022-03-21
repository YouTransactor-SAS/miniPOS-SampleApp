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
 * @file: debug_level.c
 *
 * @date: 23/07/2020
 *
 * @author: schhim
 */

#include "config.h"
#include "debug.h"

/**
 * @brief    debugLevel must be considered as a bitstring
 * ex: g_u32DbgLvl = (1 << eDbgLvlPrintHexBuffer) + (1 << eDbgLvlPrint);
 */
uint32_t g_u32DbgLvl = eDbgLvlPrintAll;


