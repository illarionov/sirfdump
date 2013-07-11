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
 * FILENAME:  sirf_codec_stats.c
 *
 * DESCRIPTION: Routines to convert data to and from a byte stream defined
 *              by the Sirf Standard Binary API from and to the Tracker API 
 *              defined in the Programmers Reference Manual
 *
 ***************************************************************************/

/***************************************************************************
 *   Include Files
 ***************************************************************************/

#include <stdio.h>
#include <string.h>

#include "sirf_types.h"
#include "sirf_msg.h"
#include "sirf_codec.h"
#include "sirf_codec_stats.h"

/* Max message length. Set it to the SSB length for now */
#define SIRF_MSG_STATS_MAX_MESSAGE_LEN  SIRF_MSG_SSB_MAX_MESSAGE_LEN

/***************************************************************************
 *   Global Variables
 ***************************************************************************/

/***************************************************************************
 * @brief:   Convert a message structure to a byte stream
 * @param[in]:     message_id - STATS message id, from the STATS manual
 * @param[in]:     message_structure - data structure keyed to the message id
 * @param[in]:     message_length - used as string length if not a structure
 * @param[out]:    packet - buffer pointer for destination formatted output
 * @param[in out]: packet_length - size of same, computed after processing
 * @return:   SIRF_SUCCESS, SIRF_CODEC_ERROR_INVALID_PARAMETER, or
 *            SIRF_CODEC_ERROR_INVALID_MSG_LENGTH
***************************************************************************/
tSIRF_RESULT SIRF_CODEC_STATS_encode( tSIRF_UINT32 message_id, 
                                      tSIRF_VOID *message_structure, 
                                      tSIRF_UINT32 message_length, 
                                      tSIRF_UINT8 *packet, 
                                      tSIRF_UINT32 *packet_length )
{
   tSIRF_RESULT tRet = SIRF_SUCCESS;

   /* Unused Parameters. */
   (tSIRF_VOID)message_id;
   
   if ( ((tSIRF_VOID *) NULL != message_structure) &&
        ((tSIRF_UINT8 *) NULL !=  packet) &&
        ((tSIRF_UINT32 *) NULL != packet_length))
   {
      /* Verify that we are not overrunning the packet.
         Account for the logical channel byte as well as message_length. */
      if ( *packet_length < (message_length * sizeof(tSIRF_UINT8) + 1 * sizeof(tSIRF_UINT8)) )
      {
         tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
      }
      else
      {
         tSIRF_UINT8 *ptr = packet;

         /* insert the logical channel */
         SIRFBINARY_EXPORT8((tSIRF_UINT8)SIRF_LC_STATS,ptr);
   
         /* Message id is embedded in the message structure for now */
         memcpy(ptr,message_structure,message_length);
         ptr += message_length;
   
         /* return length */
         *packet_length = ptr - packet;
         if (! (*packet_length > 8 && *packet_length < SIRF_MSG_STATS_MAX_MESSAGE_LEN))
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
      }
   } /* if check on input parameters */
   else 
   {
      tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
   }
   return tRet;
}

/**
 * @}
 */

