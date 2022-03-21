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
 * @file: config.h
 *
 * @date: 02/03/2020
 *
 * @author: CHHIM Stephane (stephane.chhim@youtransactor.com)
 */

/**
 * @addtogroup config
 * @{
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__
#undef CONFIG_PC
#undef CONFIG_MODEM
#define CONFIG_MODEM
#ifdef CONFIG_PC
#include "config_pc.h"
#elif defined(CONFIG_MODEM)
#include "config_modem.h"
#else // CONFIG_XXX
#error "no config was selected !!!"
#endif // CONFIG_XXX

#include "debug.h"

#endif // __CONFIG_H__
