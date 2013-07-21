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
 * FILENAME:  sirf_codec_ai3.h
 *
 * DESCRIPTION: Routine prototypes and symbol definitions
 *
 ***************************************************************************/

#ifndef __SIRF_CODEC_AI3_H__
#define __SIRF_CODEC_AI3_H__

#include "sirf_types.h"

/***************************************************************************
 * Prototype Definitions
 ***************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Decodes a message from a serial byte stream into its corresponding message
 * structure.  
 *
 * @see sirf_msg_ai3.h for message structures supported
 */ 
tSIRF_RESULT SIRF_CODEC_AI3_Decode( tSIRF_UINT8 *payload, 
                                    tSIRF_UINT32 payload_length,
                                    tSIRF_UINT32 *message_id, 
                                    tSIRF_VOID *message_structure, 
                                    tSIRF_UINT32 *message_length );

/*
 * Encodes a message structure into a serial byte stream for transmission across
 * a UART or other serial interface
 *
 * @see sirf_msg_ai3.h for message structures supported
 */ 
tSIRF_RESULT SIRF_CODEC_AI3_Encode( tSIRF_UINT32 message_id,
                                    tSIRF_VOID *message_structure,
                                    tSIRF_UINT32 message_length,
                                    tSIRF_UINT8 *packet, 
                                    tSIRF_UINT32 *packet_length );

/* Leave C naming convention */
#ifdef __cplusplus
}
#endif /*__cplusplus*/
#endif /* __SIRF_CODEC_AI3_H__ */


/**
 * @}
 */
