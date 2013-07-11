/**
 * @addtogroup platform_src_sirf_util_codec
 * @{
 */

 /**************************************************************************
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
 * FILENAME:  sirf_codec_f.h
 *
 * DESCRIPTION: Routine prototypes and symbol definitions
 *
 ***************************************************************************/

#ifndef __SIRF_CODEC_F_H__
#define __SIRF_CODEC_F_H__

#include "sirf_types.h"

/***************************************************************************
   Prototype Definitions
***************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* There are two sets of functions here, one for encoding/decoding messages to/from 
 * the CP and another for for encoding/decoding messages to/from the SLC 
 */

/* Convert a CP-generated structure to a packed byte-stream
 * This function is not used by the SLC, but is made available for unit test
 * purposes and for use inside of a CP.
 */
tSIRF_RESULT SIRF_CODEC_F_EncodeCp( tSIRF_UINT32 message_id, 
                                    tSIRF_VOID *message_structure, 
                                    tSIRF_UINT32 message_length,
                                    tSIRF_UINT8 *packet,             /* Returned */
                                    tSIRF_UINT32 *packet_length );   /* Returned */

/* Convert an SLC-Generated packed bit-stream to to a structure 
 * This function is not used by the SLC, but is made available for unit test
 * purposes and for use inside of a CP.
 */
tSIRF_RESULT SIRF_CODEC_F_DecodeSlc( tSIRF_UINT8 *payload, 
                                     tSIRF_UINT32 payload_length, 
                                     tSIRF_UINT32 *message_id,       /* Returned */
                                     tSIRF_VOID *message_structure,  /* Returned */
                                     tSIRF_UINT32 *message_length ); /* Returned */

/* Convert an SLC-generated structure to a packed byte-stream
 * This is an often used function by the SLC for encoding messages for the CP 
 */
tSIRF_RESULT SIRF_CODEC_F_EncodeSlc( tSIRF_UINT32 message_id, 
                                     tSIRF_VOID *message_structure, 
                                     tSIRF_UINT8 *packet,           /* Returned */
                                     tSIRF_UINT32 *packet_length ); /* Returned */

/* Convert a CP-Generated packed bit-stream to to a structure
 * This is an often used function by the SLC to decode the incoming data to usable structures.
 */
tSIRF_RESULT SIRF_CODEC_F_DecodeCp( tSIRF_UINT8 *payload, 
                                    tSIRF_UINT32 payload_length, 
                                    tSIRF_UINT32 *message_id,       /* Returned */
                                    tSIRF_VOID *message_structure,  /* Returned */ 
                                    tSIRF_UINT32 *message_length ); /* Returned */

/* Leave C naming convention */
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /* __SIRF_CODEC_F_H__ */


/**
 * @}
 */
