/**
 * @addtogroup platform_src_sirf_util_codec
 * @{
 */

 /***************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. GPS Software                    *
 *                                                                         *
 *    Copyright (c) 2005-2009 by SiRF Technology, Inc. All rights reserved.*
 *                                                                         *
 *    This Software is protected by United States copyright laws and       *
 *    international treaties.  You may not reverse engineer, decompile     *
 *    or disassemble this Software.                                        *
 *                                                                         *
 *    WARNING:                                                             *
 *    This Software contains SiRF Technology Inc.’s confidential and       *
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,    *
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED      *
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this      *
 *    Software without SiRF Technology, Inc.’s  express written            *
 *    permission.   Use of any portion of the contents of this Software    *
 *    is subject to and restricted by your signed written agreement with   *
 *    SiRF Technology, Inc.                                                *
 *                                                                         *
 ***************************************************************************
 *
 * MODULE:  HOST
 *
 * FILENAME:  sirf_codec_stats.h
 *
 * DESCRIPTION: Routine prototypes and symbol definitions
 *
 ***************************************************************************/

#ifndef __SIRF_CODEC_STATS_H__
#define __SIRF_CODEC_STATS_H__

#include "sirf_types.h"

/***************************************************************************
   Prototype Definitions
***************************************************************************/

tSIRF_RESULT SIRF_CODEC_STATS_encode( tSIRF_UINT32 message_id, 
                                      tSIRF_VOID *message_structure, 
                                      tSIRF_UINT32 message_length,
                                      tSIRF_UINT8 *packet, 
                                      tSIRF_UINT32 *packet_length );

#endif /* __SIRF_CODEC_STATS_H__ */


/**
 * @}
 * End of file.
 */

