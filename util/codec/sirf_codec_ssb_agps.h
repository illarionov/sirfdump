/**
 * @addtogroup platform_src_sirf_util_codec
 * @{
 */

/***************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. GPS Software                    *
 *                                                                         *
 *    Copyright (c) 2005 - 2009 by SiRF Technology, Inc.  All rights       *
 *    reserved.                                                            *
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
 * MODULE:  (CODEC Module)
 *
 * FILENAME: sirf_codec_ssb_agps.h
 *
 * DESCRIPTION: Routine prototypes and symbol definitions
 *
 ***************************************************************************
 *
 *  These are keywords for Perforce.  Do not modify.
 *
 *  $File: //customs/customer/Marvell-U1/sirf/Software/sirf/util/codec/sirf_codec_ssb_agps.h $
 *
 *  $DateTime: 2011/10/17 14:33:40 $
 *
 *  $Revision: #2 $
 *
 ***************************************************************************/


#ifndef __SIRF_CODEC_SSB_AGPS_H__
#define __SIRF_CODEC_SSB_AGPS_H__

#include "sirf_types.h"

/* =============================================================================
 * Prototype Definitions
 * -------------------------------------------------------------------------- */


#ifdef __cplusplus
extern "C" {
#endif


tSIRF_RESULT SIRF_CODEC_SSB_AGPS_Encode( tSIRF_UINT32 message_id,
                                    tSIRF_VOID *message_structure,
                                    tSIRF_UINT32 message_length,
                                    tSIRF_UINT8 *packet,
                                    tSIRF_UINT32 *packet_length );

tSIRF_RESULT SIRF_CODEC_SSB_AGPS_Decode( tSIRF_UINT8* payload,
                                    tSIRF_UINT32 payload_length,
                                    tSIRF_UINT32 *message_id,
                                    tSIRF_VOID *message_structure,
                                    tSIRF_UINT32 *message_length );

/* Leave C naming convention */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /* __SIRF_CODEC_SSB_AGPS_H__ */

/**
 * @}
 * End of file.
 */

