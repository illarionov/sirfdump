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

#include "sirf_proto.h"
#include "sirf_proto_gprs_at_command.h"
#include "sirf_codec_gprs_at_command.h"
#if 0 /* @todo when util_if.h or equivilent functionality is added to the
       * SiRFRunTimeLib the else clause can be removed. */
#include "util_if.h"
#else
#ifndef UTIL_Assert
   #define UTIL_Assert(_condition)
#endif

#ifndef UTIL_AssertAlways
   #define UTIL_AssertAlways()
#endif
#endif

/** 
 * Fully encode a message to be sent to the GPRS modem as an AT command.
 * 
 * Encode the payload and add the wrapper
 * 
 * @param message_id        Subject of the message
 * @param message_structure Data of the mssage
 * @param message_length    Size of the message data
 * @param packet            Packet to be sent
 * @param packet_length     Size of packet
 * @param options           Codec options
 * 
 * @return SIRF_SUCCESS or non-zero on error
 */
tSIRF_RESULT SIRF_PROTO_GPRS_AT_COMMAND_Encode( 
   tSIRF_UINT32               message_id, 
   tSIRF_VOID   const * const message_structure,
   tSIRF_UINT32               message_length,
   tSIRF_UINT8        * const packet, 
   tSIRF_UINT32       * const packet_length,
   tSIRF_UINT32       * const options )
{
   tSIRF_RESULT result;
   tSIRF_UINT32 payload_length;

   tSIRF_UINT8  *ptr = packet;

   /* packet max length less header and trailer*/
   payload_length = *packet_length - SIRF_PROTO_GPRS_AT_COMMAND_WRAPPER_LENGTH;
   result = SIRF_CODEC_GPRS_AT_COMMAND_Encode( message_id, 
                                               message_structure, 
                                               message_length,
                                               &ptr[SIRF_PROTO_GPRS_AT_COMMAND_HEADER_LENGTH], 
                                               &payload_length,
                                               options );
   if ( SIRF_SUCCESS != result )
   {
      return result;
   }

   UTIL_Assert((payload_length + SIRF_PROTO_GPRS_AT_COMMAND_WRAPPER_LENGTH) < *packet_length);
   /* ATtention */
   *ptr++ = 'A';
   *ptr++ = 'T';
   ptr   += payload_length;
   /* Add final <CR> */
   *ptr   = '\r';
   
   *packet_length = payload_length + SIRF_PROTO_GPRS_AT_COMMAND_WRAPPER_LENGTH;

   return SIRF_SUCCESS;

} /* SIRF_PROTO_GPRS_AT_COMMAND_Encode()*/

/** 
 * Decocde an AT command message.
 * 
 * Offset from the wrapper and then pass to the Codec to finish
 *
 * @param packet            Incoming packet
 * @param packet_length     Size of packet
 * @param message_id        Decoded message ID
 * @param message_structure Decoded Message
 * @param message_length    Size of decoded message
 * @param options           Codec Options
 * 
 * @return SIRF_SUCCESS or non-zero on error
 */
tSIRF_RESULT SIRF_PROTO_GPRS_AT_COMMAND_Decode( 
   tSIRF_UINT8  const * const packet, 
   tSIRF_UINT32               packet_length,
   tSIRF_UINT32       * const message_id, 
   tSIRF_VOID         * const message_structure,
   tSIRF_UINT32       * const message_length,
   tSIRF_UINT32       * const options )
{
   tSIRF_RESULT  result;
   tSIRF_UINT32  payload_length = (packet_length - 
                                   SIRF_PROTO_GPRS_AT_RESPONSE_WRAPPER_LENGTH);
   
   /* Yes we are casting away the const so that we can mofify the value */
   tSIRF_UINT8  *ptr = (tSIRF_UINT8*)packet;

   /* validate the inputs */
   if ( SIRF_PROTO_GPRS_AT_RESPONSE_MIN_MSG_LENGTH > packet_length || 
        SIRF_PROTO_GPRS_AT_RESPONSE_MAX_MSG_LENGTH < packet_length)
   {
      return SIRF_PROTO_FRAME_ERROR;
   }

   if ( *ptr++ != '\r' )
   {
      return SIRF_PROTO_FRAME_ERROR;
   }

   if ( *ptr++ != '\n' )
   {
      return SIRF_PROTO_FRAME_ERROR;
   }

   if ('\r' != ptr[payload_length] || '\n' != ptr[payload_length + 1])
   {
      return SIRF_PROTO_FRAME_ERROR;
   }

   /* The decode function needs null terminated strings because of its use of
    * sscanf.  Null terminate it. */
   ptr[payload_length] = '\0';

   result = SIRF_CODEC_GPRS_AT_COMMAND_Decode( ptr,
                                               payload_length+1,
                                               message_id, 
                                               message_structure, 
                                               message_length,
                                               options );

   /* Here we maintain that the payload truely is constand */
   ptr[payload_length] = '\r';
   return result;

} /* SIRF_PROTO_GPRS_AT_COMMAND_Decode()*/


/** 
 * Find and frame an AT command message.
 *
 * Look for <CR><LF>data<CR><LF>
 * 
 * @param buffer          Buffer to scan for a message.
 * @param bytes_in_buffer Size of the buffer.
 * @param data_length     Data consumed by this scan.  Always set
 * 
 * @return SIRF_SUCCESS if a full frame is found.  Non-zero otherwise.  If
 * a <CR><LF> is not found at the start of the buffer SIRF_PROTO_FRAME_ERROR 
 * is returned.  If <CR><LF> is not found later SIRF_PROTO_NOT_ENOUGH_DATA_YET
 * is returned.  If the maximum response length is reached a frame error is
 * returned.
 */
tSIRF_RESULT SIRF_PROTO_GPRS_AT_COMMAND_Parser( 
   tSIRF_UINT8  const * const buffer, 
   tSIRF_UINT32               bytes_in_buffer, 
   tSIRF_UINT32       * const data_length )
{
   tSIRF_UINT32   i;
   tSIRF_UINT32   length;

   if (SIRF_PROTO_GPRS_AT_RESPONSE_MIN_MSG_LENGTH > bytes_in_buffer )
   {
      *data_length = 0;
      return SIRF_PROTO_NOT_ENOUGH_DATA_YET;
   }

   i = 0;
   /* Make sure the start of frame is <CR><LF> */
   if('\r' != buffer[i] || '\n' != buffer[i+1])
   {
      /* Figure out how much data can be thrown away by looking for the next
       *  start of frame */
      for ( i=1; i<bytes_in_buffer-1; i++ )
      {
         if('\r' == buffer[i] && '\n' == buffer[i+1])
         {
            break;
         }
      }
      
      /* At this point i is at most bytes_in_buffer - 1.  If the last byte 
       * found was not <CR> incriment past it */
      if (buffer[i] != '\r')
      {
         i++;
      }
      *data_length = i;
      return SIRF_PROTO_FRAME_ERROR;
   }

   /* At this point <CR><LF> are at buffer[0] and buffer[1] respectively.
    * Search for the end of frame. */
   for (i = 3; i < bytes_in_buffer; i++)
   {
      if('\r' == buffer[i-1] && '\n' == buffer[i])
      {
         break;
      }
   }
   
   /* Set the length */
   length = i+1;

   /* Check if the length is a valid length */
   if (( SIRF_PROTO_GPRS_AT_RESPONSE_MAX_MSG_LENGTH + 
         SIRF_PROTO_GPRS_AT_RESPONSE_WRAPPER_LENGTH < length) ||
       (SIRF_PROTO_GPRS_AT_RESPONSE_MIN_MSG_LENGTH > length))
   {
      /* Here we use the fact that the Start Frame <CR><LF> is the same as
       * the end frame <CR><LF> to throw away as much data that does not
       * have a Start Frame in it as possible. */

      /* There are currently three possibilities: */
      /* 1) i = bytes_in_buffer and <CR><LF> isn't anywhere in the buffer */
      /* but <CR> is the last byte */
      if ((i >= bytes_in_buffer) &&
          ('\r' != buffer[i-1]))
      {
         *data_length = i;
      }
      /* 2) i = bytes_in_buffer and <CR> == buffer[i-1]
       * 3) i < bytes_in_buffer and 
       *    buffer[i-1] == '\r' and buffer[i] == '\n' 
       * Both of these cases need to consume i-1 bytes */
      else
      {
         *data_length = i-1;
      }
      
      *data_length = SIRF_PROTO_GPRS_AT_COMMAND_HEADER_LENGTH;
      return SIRF_PROTO_FRAME_ERROR;
   }

   /* Check if we ran out of characters */
   if (i >= bytes_in_buffer)
   {
      *data_length = 0;
      return SIRF_PROTO_NOT_ENOUGH_DATA_YET;
   }

   /* Have frame!*/
   *data_length = length;

   return SIRF_SUCCESS;

} /* SIRF_PROTO_GPRS_AT_COMMAND_Parser()*/

/**
 * @}
 * End of file.
 */
