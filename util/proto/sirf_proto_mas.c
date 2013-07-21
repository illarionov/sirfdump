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

#include <string.h>

#include "sirf_types.h"

#include "sirf_msg.h"
#include "sirf_codec.h"
#include "sirf_proto.h"
#include "sirf_proto_mas.h"

/** 
 * The MAS encode function simply escapes any ETX or DLE characeters
 * 
 * @param message_id        message to encode 
 * @param message_structure data to encode
 * @param message_length    sizeof data to encode
 * @param packet            encoded packet
 * @param packet_length     encoded length
 * @param options           codec options
 * 
 * @return SIRF_SUCCESS or non-zero error code
 */
tSIRF_RESULT SIRF_PROTO_MAS_Encode(
   tSIRF_UINT32               message_id, 
   tSIRF_VOID   const * const message_structure,
   tSIRF_UINT32               message_length,
   tSIRF_UINT8        * const packet, 
   tSIRF_UINT32       * const packet_length,
   tSIRF_UINT32       * const options )
{
   tSIRF_UINT32 ii,jj = 0;
   tSIRF_UINT8 const * const data = message_structure;

   (void)message_id;
   /* If no characers are escaped we need message_length worth of data */
   if (*packet_length < message_length) 
   {
      return SIRF_PROTO_BUFFER_ERROR;
   }

   /* Go through and escape any ETX or DLE characters */
   for (ii = 0; ii < message_length; ii++)
   {
      if (SIRF_PROTO_MAS_ETX == data[ii] || SIRF_PROTO_MAS_DLE == data[ii])
      {
         /* Check to make sure we have room */
         if (jj+2 > *packet_length)
         {
            return SIRF_PROTO_BUFFER_ERROR;
         }
         /* Escape this character */
         packet[jj++] = SIRF_PROTO_MAS_DLE;
         packet[jj++] = data[ii];
      }
      else
      {
         /* Check to make sure we have room */
         if (jj+1 > *packet_length)
         {
            return SIRF_PROTO_BUFFER_ERROR;
         }
         packet[jj++] = data[ii];
      }
   }

   *packet_length = jj;
   *options = SIRF_CODEC_OPTIONS_MAKE_MSG_NUMBER(1,1);   
   return SIRF_SUCCESS;
} /* SIRF_PROTO_MAS_Encode()*/

/** 
 * Decode a MAS message from the GPRS modem, ETX and DLE characters are
 * escaped.
 * 
 * @param packet            Data to decode
 * @param packet_length     Size of data to decode
 * @param message_id        Subject of message
 * @param message_structure Decoded message
 * @param message_length    Size of decoded message
 * @param options           Codec options
 * 
 * @return SIRF_SUCCESS or non-zero error code
 */
tSIRF_RESULT SIRF_PROTO_MAS_Decode(
   tSIRF_UINT8  const * const packet, 
   tSIRF_UINT32               packet_length,
   tSIRF_UINT32       * const message_id, 
   tSIRF_VOID         * const message_structure,
   tSIRF_UINT32       * const message_length,
   tSIRF_UINT32       * const options )
{

   tSIRF_UINT32 ii,jj = 0;
   /* If no characers are escaped we need message_length worth of data */

   tSIRF_UINT8 * const msg = message_structure;
   tSIRF_UINT32 escaped = SIRF_FALSE;

   /* Look for escaped characters */
   for (ii = 0; ii < packet_length; ii++)
   {
      if (!escaped && SIRF_PROTO_MAS_DLE == packet[ii])
      {
         escaped = SIRF_TRUE;
         continue;
      }
      else
      {
         escaped = SIRF_FALSE;
         if(NULL != msg)
         {
            msg[jj] = packet[ii];
         }
         jj++;
      }
   }

   *message_id = SIRF_MAKE_MSG_ID(SIRF_LC_MAS,0,0);
   *message_length = jj;
   /* default */
   *options = SIRF_CODEC_OPTIONS_MAKE_MSG_NUMBER(1,1);

   return SIRF_SUCCESS;

} /* SIRF_PROTO_MAS_Decode()*/

/** 
 * Frames the MAS messages.  For now this simply looks for a non-escaped ETX
 * character wich signifies the end of text, time to switch protocols
 * 
 * @param buffer          Data buffer to scan
 * @param bytes_in_buffer Number of bytes in the buffer
 * @param data_length     Number of bytes consumed, always set even if a full
 *                        frame is not found.
 * 
 * @return SIRF_SUCCESS or non-zero error code
 */
tSIRF_RESULT SIRF_PROTO_MAS_Parser( 
   tSIRF_UINT8  const * const buffer, 
   tSIRF_UINT32               bytes_in_buffer, 
   tSIRF_UINT32       * const data_length )
{
   tSIRF_UINT32 ii;
   tSIRF_RESULT result = SIRF_SUCCESS;

   /* if there is no ETX character then return all bytes */
   *data_length = bytes_in_buffer;
   /* Currently the MAS parser returns everything as valid as the parser and 
    * codec work is done further up the chain.  Simply look for an unescaped 
    * ETX */
   for ( ii = 0; ii < bytes_in_buffer; ii++)
   {
      if (SIRF_PROTO_MAS_ETX == buffer[ii])
      {
         /* ETX Found check to see if it was escaped.  If so ignore it */
         if (ii == 0 || SIRF_PROTO_MAS_DLE != buffer[ii-1])
         {
            *data_length = ii+1;
            break;
         }
      }
   }

   return result;
} /* SIRF_PROTO_MAS_Parser()*/

/**
 * @}
 * End of file.
 */
