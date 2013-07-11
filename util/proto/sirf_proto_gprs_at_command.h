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

#ifndef __SIRF_PROTO_GPRS_AT_COMMAND_H__
#define __SIRF_PROTO_GPRS_AT_COMMAND_H__


/* SiRF Standard Binary protocol API prototypes*/

/** Maximum buffer needed for an AT command */
#define SIRF_PROTO_GPRS_AT_COMMAND_MAX_MSG_LENGTH  (1024)
/** Maximum buffer needed for an AT response */
#define SIRF_PROTO_GPRS_AT_RESPONSE_MAX_MSG_LENGTH  (1024)
/* The minimum message length for an AT command is <CR><LF>OK<CR><LF> */
#define SIRF_PROTO_GPRS_AT_RESPONSE_MIN_MSG_LENGTH (6)
/* The length of AT<CR>: 3 bytes */
#define SIRF_PROTO_GPRS_AT_COMMAND_WRAPPER_LENGTH  (3)
/* The length of <CR><LF><CR><LF>: 4 bytes */
#define SIRF_PROTO_GPRS_AT_RESPONSE_WRAPPER_LENGTH (4)
/* The length of <CR><LF>: 2 bytes */
#define SIRF_PROTO_GPRS_AT_COMMAND_HEADER_LENGTH   (2)
/* The length of <CR><LF>: 2 bytes */
#define SIRF_PROTO_GPRS_AT_COMMAND_FOOTER_LENGTH   (2)

#ifdef __cplusplus
extern "C" {
#endif


tSIRF_RESULT SIRF_PROTO_GPRS_AT_COMMAND_Encode( 
   tSIRF_UINT32               message_id, 
   tSIRF_VOID   const * const message_structure,
   tSIRF_UINT32               message_length,
   tSIRF_UINT8        * const packet, 
   tSIRF_UINT32       * const packet_length,
   tSIRF_UINT32       * const options );

tSIRF_RESULT SIRF_PROTO_GPRS_AT_COMMAND_Decode( 
   tSIRF_UINT8  const * const packet, 
   tSIRF_UINT32               packet_length,
   tSIRF_UINT32       * const message_id, 
   tSIRF_VOID         * const message_structure,
   tSIRF_UINT32       * const message_length,
   tSIRF_UINT32       * const options );

tSIRF_RESULT SIRF_PROTO_GPRS_AT_COMMAND_Parser( 
   tSIRF_UINT8  const * const buffer, 
   tSIRF_UINT32               bytes_in_buffer, 
   tSIRF_UINT32       * const data_length );


#ifdef __cplusplus
}
#endif


#endif /* __SIRF_PROTO_GPRS_AT_COMMAND_H__ */

/**
 * @}
 * End of file.
 */
