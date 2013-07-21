/**
 * @addtogroup platform_src_sirf_util_proto
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
 **************************************************************************/

#ifndef __SIRF_PROTO_MAS_H__
#define __SIRF_PROTO_MAS_H__

#include "sirf_types.h"

/** ASCII ETX character. End of Text */
#define SIRF_PROTO_MAS_ETX (0x03) 
/** ASCII DLE characeter Data Link Escape */
#define SIRF_PROTO_MAS_DLE (0x10) 

#ifdef __cplusplus
extern "C" {
#endif


tSIRF_RESULT SIRF_PROTO_MAS_Encode( 
   tSIRF_UINT32               message_id, 
   tSIRF_VOID   const * const message_structure,
   tSIRF_UINT32               message_length,
   tSIRF_UINT8        * const packet, 
   tSIRF_UINT32       * const packet_length,
   tSIRF_UINT32       * const options );
   
tSIRF_RESULT SIRF_PROTO_MAS_Decode(
   tSIRF_UINT8  const * const packet, 
   tSIRF_UINT32               packet_length,
   tSIRF_UINT32       * const message_id, 
   tSIRF_VOID         * const message_structure,
   tSIRF_UINT32       * const message_length,
   tSIRF_UINT32       * const options );

tSIRF_RESULT SIRF_PROTO_MAS_Parser( 
   tSIRF_UINT8  const * const buffer, 
   tSIRF_UINT32               bytes_in_buffer, 
   tSIRF_UINT32       * const data_length );


#ifdef __cplusplus
}
#endif


#endif /* __SIRF_PROTO_MAS_H__ */

/**
 * @}
 * End of file.
 */
