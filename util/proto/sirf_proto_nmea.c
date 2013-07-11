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
#include <stdio.h>

#include "sirf_types.h"

#include "sirf_msg.h"
#include "sirf_codec.h"
#include "sirf_codec_nmea.h"
#include "sirf_proto.h"
#include "sirf_proto_nmea.h"
#include "string_sif.h"

#define SIRF_PROTO_NMEA_MAX_MSG_LENGTH  (1024)


#define SIRF_PROTO_NMEA_LENGTH_HEADER (1) /* $ */
#define SIRF_PROTO_NMEA_LENGTH_FOOTER (6) /* *CK\r\n\0 */
#define SIRF_PROTO_NMEA_LENGTH_WRAPPER (SIRF_PROTO_NMEA_LENGTH_HEADER + SIRF_PROTO_NMEA_LENGTH_FOOTER) 


/*=============================================================================
 *
 * Inputs:  pBuf  pointer to message string, null terminated
 *          msgid ID of message, to determine if ck sum is T/F
 *                use to index UI_ msg table
 *
 * Outputs: None
 *
 * Desc:    if checksum is enabled for that particular message, calculate
 *          the checksum and strcat() the checksum to the message
 *
 *          It is the caller function responsibility to ensure enough buffer
 *          space for the '*CK'  astrisk checksum delimiter and 2 character
 *          checksum
 *
 *---------------------------------------------------------------------------*/
static tSIRF_RESULT SIRF_PROTO_NMEA_AddCheckSum( tSIRF_UINT8 *pBuf, 
                                                 tSIRF_UINT32 bufsize)
{
   tSIRF_UINT8 ckSum = 0;
   tSIRF_UINT32 msgsize = 0;
   int result;

   while( (*pBuf != '\0') && (msgsize < bufsize) )
   {
      ckSum ^= *pBuf++;
      msgsize++;
   }
   /* +"*CK\x0d\x0a" */
   result = snprintf( (char*)pBuf, 
                      bufsize - msgsize, 
                      "*%02lX\x0D\x0A", 
                      (tSIRF_UINT32)ckSum );  
   if (result < 0)
   {
      return SIRF_PROTO_FRAME_CHECKSUM_ERROR;
   }
   else 
   {
      return SIRF_SUCCESS;
   }

} /* AddCheckSum() */


/** 
 * The NMEA encode function produces a NMEA string from the input message
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
tSIRF_RESULT SIRF_PROTO_NMEA_Encode(
   tSIRF_UINT32               message_id, 
   tSIRF_VOID   const * const message_structure,
   tSIRF_UINT32               message_length,
   tSIRF_UINT8        * const packet, 
   tSIRF_UINT32       * const packet_length,
   tSIRF_UINT32       * const options )
{
   tSIRF_UINT8 * payload;
   tSIRF_UINT32 payload_length;
   tSIRF_UINT32 result;

   /* If no characers are escaped we need message_length worth of data */
   if (SIRF_PROTO_NMEA_LENGTH_WRAPPER > *packet_length) 
   {
      return SIRF_PROTO_BUFFER_ERROR;
   }

   packet[0] = '$';
   payload = &packet[SIRF_PROTO_NMEA_LENGTH_HEADER];
   payload_length = *packet_length - SIRF_PROTO_NMEA_LENGTH_WRAPPER;

   result = SIRF_CODEC_NMEA_Encode(message_id, 
                                   message_structure,
                                   message_length,
                                   payload, 
                                   &payload_length,
                                   options);

   if (SIRF_SUCCESS == result)
   {
      result = SIRF_PROTO_NMEA_AddCheckSum(payload,payload_length+SIRF_PROTO_NMEA_LENGTH_FOOTER);

      *packet_length = payload_length + SIRF_PROTO_NMEA_LENGTH_WRAPPER;
   }

   return result;
} /* SIRF_PROTO_NMEA_Encode()*/

/** 
 * Decode a NMEA message.
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
tSIRF_RESULT SIRF_PROTO_NMEA_Decode(
   tSIRF_UINT8  const * const packet, 
   tSIRF_UINT32               packet_length,
   tSIRF_UINT32       * const message_id, 
   tSIRF_VOID         * const message_structure,
   tSIRF_UINT32       * const message_length,
   tSIRF_UINT32       * const options )
{

   tSIRF_RESULT  result;
   tSIRF_UINT32  payload_length = 0;
   tSIRF_UINT8  const *ptr  = packet;
   tSIRF_UINT32  read_checksum, calculated_checksum = 0;
   tSIRF_CHAR    checksum_buffer[SIRF_PROTO_NMEA_LENGTH_FOOTER];
   tSIRF_UINT32  i;
   int           ret_val;

   if ( packet_length < SIRF_PROTO_NMEA_LENGTH_WRAPPER )
   {
      return SIRF_PROTO_FRAME_ERROR;
   }

   if ( *ptr++ != '$' )
   {
      return SIRF_PROTO_FRAME_ERROR;
   }

   /* Find the Checksum, while computing the checksum */
   while ( (*ptr != '*') && (payload_length<(packet_length-1)) )
   {
      calculated_checksum ^= *ptr++;
      payload_length++;
   }

   if ( (payload_length == packet_length) || 
        (payload_length > SIRF_PROTO_NMEA_MAX_MSG_LENGTH) )
   {
      return SIRF_PROTO_FRAME_ERROR;
   }

   /* Fill it all but the Null character */
   for ( i=0; i<SIRF_PROTO_NMEA_LENGTH_FOOTER-1; i++ )
   {
      checksum_buffer[i] = *ptr++;
   }
   /* Add a null so we can use sscanf */
   checksum_buffer[i] = '\0';

   ret_val = sscanf( checksum_buffer, "*%02lX\x0D\x0A", &read_checksum );

   if ( ret_val != 1 )
   {
      return SIRF_PROTO_FRAME_ERROR;
   }

   if ( calculated_checksum != read_checksum )
   {
      return SIRF_PROTO_FRAME_CHECKSUM_ERROR;
   }

   result = SIRF_CODEC_NMEA_Decode( packet+1, payload_length,
                                    message_id, message_structure, message_length,
                                    options );
   return result;


} /* SIRF_PROTO_NMEA_Decode()*/

/** 
 * Finds the NMEA Frames 
 * 
 * @param buffer          Data buffer to scan
 * @param bytes_in_buffer Number of bytes in the buffer
 * @param data_length     Number of bytes consumed, always set even if a full
 *                        frame is not found.
 * 
 * @return SIRF_SUCCESS or non-zero error code
 */
tSIRF_RESULT SIRF_PROTO_NMEA_Parser( 
   tSIRF_UINT8  const * const buffer, 
   tSIRF_UINT32               bytes_in_buffer, 
   tSIRF_UINT32       * const data_length )
{
   tSIRF_UINT32   i;

   if ( bytes_in_buffer < SIRF_PROTO_NMEA_LENGTH_WRAPPER )
      return SIRF_PROTO_NOT_ENOUGH_DATA_YET;

   /* Search for header */
   for ( i=0; i<bytes_in_buffer; i++ )
   {
      if ( buffer[i] == '$' )
         break;
   }

   if ( i )
   {
      /*  Report error data */
      *data_length = i;
      return SIRF_PROTO_FRAME_ERROR;
   }

   /*  Search for trailer */
   for ( i=1; i<bytes_in_buffer; i++ )
   {
      if ( buffer[i] == 0x0A )
      {
         /*  Have message! */
         *data_length = i + 1;
         return SIRF_SUCCESS;
      }
      else if ( buffer[i] == '$' )
      {
         /*  Report error data */
         *data_length = i;
         return SIRF_PROTO_FRAME_ERROR;
      }
   }

   return SIRF_PROTO_NOT_ENOUGH_DATA_YET;
} /* SIRF_PROTO_NMEA_Parser()*/

/**
 * @}
 * End of file.
 */
