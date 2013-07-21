/**
 * @addtogroup platform_src_sirf_util_codec
 * @{
 */

 /**************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. GPS Software                    *
 *                                                                         *
 *    Copyright (c) 2005-2010 by SiRF Technology, a CSR plc Company.       *
 *    All rights reserved.                                                 *
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
 * FILENAME:  sirf_codec_f.c
 *
 * DESCRIPTION: Routines to convert data to and from a byte stream defined
 *              by the SiRF System/F protocol from and to the Tracker API 
 *              defined in the Programmers Reference Manual
 *
 ***************************************************************************/

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include "sirf_types.h"
#include "sirf_msg.h"
#include "sirf_msg_f.h"
#include "sirf_codec.h"
#include "sirf_codec_f.h"

/***************************************************************************
 * @brief:   Convert an SLC-generated structure to a packed byte-stream
 * @param[in]      message_id - F Protocol message id, from the ICD
 * @param[in]      message_structure - data structure keyed to the message id
 * @param[out]     packet - buffer pointer for destination formatted output
 * @param[in out]  packet_length - size of same, computed after processing
 * @return:  SIRF_SUCCESS, SIRF_CODEC_ERROR_INVALID_PARAMETER,
 *           SIRF_CODEC_ERROR_INVALID_MSG_LENGTH, or
 *           SIRF_CODEC_ERROR_INVALID_MSG_ID.
***************************************************************************/

tSIRF_RESULT SIRF_CODEC_F_EncodeSlc( tSIRF_UINT32 message_id, 
                                     tSIRF_VOID *message_structure, 
                                     tSIRF_UINT8 *packet, 
                                     tSIRF_UINT32 *packet_length )
{
   tSIRF_INT32 ii;
   tSIRF_UINT8 *ptr;
   tSIRF_RESULT tRet = SIRF_SUCCESS;
   tSIRF_UINT8 mid = SIRF_GET_MID(message_id);
   tSIRF_UINT8 lc  = SIRF_GET_LC(message_id);
   
   /* First make sure the function was called right */
   if ( ((tSIRF_VOID *) NULL == message_structure) ||
        ((tSIRF_UINT8 *) NULL ==  packet) ||
        ((tSIRF_UINT32 *) NULL == packet_length) ||
        (lc != SIRF_LC_F))
   {
      return SIRF_CODEC_ERROR_INVALID_PARAMETER;
   }
   
   /* At a minimum, make sure we have room for the basics */
   if ( *packet_length < (sizeof(mid) + sizeof(lc)) )
   {
      return SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
   }

   ptr = packet;
   SIRFBINARY_EXPORT8(lc, ptr);    /* logical channel */
   SIRFBINARY_EXPORT8(mid, ptr);   /* message ID */
      
   switch (message_id)
   {
      case SIRF_MSG_F_SESSION_OPENING_NOTIFICATION:
         {
            tSIRF_MSG_F_SESSION_OPENING_NOTIFICATION * msg 
               = (tSIRF_MSG_F_SESSION_OPENING_NOTIFICATION *) message_structure;
                                 
            /**
             * Attention programmers: the code compares the count of data bytes to *packet_length:
             * it generates the count by scanning the data store and adding up all of the 
             * UINT8s then all of the UINT16s, then all of the UINT32s.
             */

            /* Check to make sure that the length of the buffer we are writing into is big enough.
               Include the mid and lc lengths.  */
            if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->ses_open_not, ptr);
            }
         }
         break;

      case SIRF_MSG_F_SESSION_CLOSING_NOTIFICATION:
         {
            tSIRF_MSG_F_SESSION_CLOSING_NOTIFICATION * msg = 
               (tSIRF_MSG_F_SESSION_CLOSING_NOTIFICATION  *) message_structure;
               
            if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->ses_close_not, ptr);
            }
         }
         break;

      case SIRF_MSG_F_ERROR_NOTIFICATION:
         {
            tSIRF_MSG_F_ERROR_NOTIFICATION * msg = (tSIRF_MSG_F_ERROR_NOTIFICATION *) message_structure;

            if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->session_error_reason, ptr);
            }
         }
         break;

      case SIRF_MSG_F_SLC_STATUS:
         {
            tSIRF_MSG_F_SLC_STATUS * msg = (tSIRF_MSG_F_SLC_STATUS *) message_structure;
            
            if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->slc_status, ptr);
            }
         }
         break;

      case SIRF_MSG_F_HARDWARE_CONFIGURATION_REQ:
         break;

      case SIRF_MSG_F_TIME_TRANSFER_REQ:
         break;

      case SIRF_MSG_F_FREQ_TRANSFER_REQ:
         {
            tSIRF_MSG_F_FREQ_TRANSFER_REQ * msg = (tSIRF_MSG_F_FREQ_TRANSFER_REQ *) message_structure;
            
            if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->req_info, ptr);
            }
         }
         break;

      case SIRF_MSG_F_APRX_MS_POSITION_REQ:
         break;

      case SIRF_MSG_F_WIRELESS_POWER_REQ:
         break;

      case SIRF_MSG_F_REJECT_SLC:
         {
            tSIRF_MSG_F_REJECT_SLC * msg = (tSIRF_MSG_F_REJECT_SLC *) message_structure;
            
            if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->rej_msgid , ptr);
               SIRFBINARY_EXPORT8(msg->reason,     ptr);
            }
         }
         break;

      case SIRF_MSG_F_SOFTWARE_VERSION_RSP:
         {
            tSIRF_MSG_F_SOFTWARE_VERSION_RSP * msg = (tSIRF_MSG_F_SOFTWARE_VERSION_RSP *) message_structure;
            
            if ( *packet_length < ( (2 + msg->sirf_ver_bytes + msg->cust_ver_bytes) * sizeof(tSIRF_UINT8) +
                                     sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->sirf_ver_bytes, ptr);
               SIRFBINARY_EXPORT8(msg->cust_ver_bytes, ptr);
               for (ii = 0; ii < msg->sirf_ver_bytes; ii++)
               {
                  SIRFBINARY_EXPORT8(msg->sirf_ver_str[ii], ptr);
               }
               for (ii = 0; ii < msg->cust_ver_bytes; ii++)
               {
                  SIRFBINARY_EXPORT8(msg->cust_ver_str[ii], ptr);
               }
            }
         }
         break;

      case SIRF_MSG_F_ICD_VERSION_RSP:
         {
            tSIRF_MSG_F_ICD_VERSION_RSP * msg = (tSIRF_MSG_F_ICD_VERSION_RSP *) message_structure;
            
           if ( *packet_length < ( 6 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc) )  )

            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->f_icd_version_major, ptr);
               SIRFBINARY_EXPORT8(msg->f_icd_version_minor, ptr);
               SIRFBINARY_EXPORT8(msg->ai3_icd_version_major, ptr);
               SIRFBINARY_EXPORT8(msg->ai3_icd_version_minor, ptr);
               SIRFBINARY_EXPORT8(msg->spare1, ptr);
               SIRFBINARY_EXPORT8(msg->spare2, ptr);
            }
         }
         break;

      case SIRF_MSG_F_PUSH_AIDING_ACK_NACK:
         {
            tSIRF_MSG_F_PUSH_AIDING_ACK_NACK * msg = (tSIRF_MSG_F_PUSH_AIDING_ACK_NACK *) message_structure;
            
            if ( *packet_length < ( 2 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->ack_nack, ptr);
               SIRFBINARY_EXPORT8(msg->message_id, ptr);
            }
         }
         break;

      case SIRF_MSG_F_TIME_FREQ_APROX_POS_RSP:
         {
            tSIRF_MSG_F_TIME_FREQ_APROX_POS_RSP * msg = (tSIRF_MSG_F_TIME_FREQ_APROX_POS_RSP *) message_structure;
            
            if ( *packet_length < ( 5 * sizeof(tSIRF_UINT8) +
                                    3 * sizeof(tSIRF_UINT16) +
                                    5 * sizeof(tSIRF_UINT32) +
                                    sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8 (msg->status_resp_mask,   ptr);
               SIRFBINARY_EXPORT16(msg->gps_week,           ptr);
               SIRFBINARY_EXPORT32(msg->gps_tow,            ptr);
               SIRFBINARY_EXPORT8 (msg->time_acc_scale,     ptr);
               SIRFBINARY_EXPORT8 (msg->time_accuracy,      ptr);
               SIRFBINARY_EXPORT8 (msg->freq_acc_scale,     ptr);
               SIRFBINARY_EXPORT8 (msg->freq_accuracy,      ptr);
               SIRFBINARY_EXPORT16(msg->scaled_freq_offset, ptr);
               SIRFBINARY_EXPORT32(msg->freq_time_tag,      ptr);
               SIRFBINARY_EXPORT32(msg->slc_hor_unc,        ptr);
               SIRFBINARY_EXPORT16(msg->slc_ver_unc,        ptr);
               SIRFBINARY_EXPORT32(msg->spare03,            ptr);
               SIRFBINARY_EXPORT32(msg->spare47,            ptr);
            }
         }
         break;

      case SIRF_MSG_F_ACK_APM:
         {
            tSIRF_MSG_F_ACK_APM * msg = (tSIRF_MSG_F_ACK_APM *) message_structure;
            
            if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->apm_state, ptr);
               SIRFBINARY_EXPORT8(msg->spare,     ptr);
            }
         }
         break;

      case SIRF_MSG_F_SERIAL_PORT_SETTINGS_RSP:
         {
            tSIRF_MSG_F_SERIAL_PORT_SETTINGS_RSP * msg = (tSIRF_MSG_F_SERIAL_PORT_SETTINGS_RSP *) message_structure;
            
            if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + sizeof(tSIRF_UINT32) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8 (msg->port,       ptr);
               SIRFBINARY_EXPORT32(msg->baud_rate,  ptr);
               SIRFBINARY_EXPORT8 (msg->ack_number, ptr);
            }
         }
         break;

      case SIRF_MSG_F_CHANNEL_OPEN_RSP:
         {
            tSIRF_MSG_F_CHANNEL_OPEN_RSP * msg = (tSIRF_MSG_F_CHANNEL_OPEN_RSP *) message_structure;
            
            if ( *packet_length < (3 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->port,         ptr);
               SIRFBINARY_EXPORT8(msg->channel_type, ptr);
               SIRFBINARY_EXPORT8(msg->priority,     ptr);
            }
         }
         break;

      case SIRF_MSG_F_CHANNEL_CLOSE_RSP:
         {
            tSIRF_MSG_F_CHANNEL_CLOSE_RSP * msg = (tSIRF_MSG_F_CHANNEL_CLOSE_RSP *) message_structure;
            
            if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->port,         ptr);
               SIRFBINARY_EXPORT8(msg->channel_type, ptr);
            }
         }
         break;

      case SIRF_MSG_F_CHANNEL_PRIORITY_RSP:
         {
            tSIRF_MSG_F_CHANNEL_PRIORITY_RSP * msg = (tSIRF_MSG_F_CHANNEL_PRIORITY_RSP *) message_structure;
            
            if ( *packet_length < (3 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->port,         ptr);
               SIRFBINARY_EXPORT8(msg->channel_type, ptr);
               SIRFBINARY_EXPORT8(msg->priority,     ptr);
            }
         }
         break;

      case SIRF_MSG_F_PRIORITY_RSP:
         {
            tSIRF_MSG_F_PRIORITY_RSP * msg = (tSIRF_MSG_F_PRIORITY_RSP *) message_structure;
            
            if ( *packet_length < ( (2 + 2 * msg->number_of_channels) * sizeof(tSIRF_UINT8) + 
                                     sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               memset(msg,0,sizeof(tSIRF_MSG_F_PRIORITY_RSP));
   
               SIRFBINARY_EXPORT8(msg->port,               ptr);
               SIRFBINARY_EXPORT8(msg->number_of_channels, ptr);
               for (ii = 0; ii < msg->number_of_channels; ii++)
               {
                  tSIRF_MSG_F_PRIORITY_RSP_ONE_CHANNEL * pMsg = 
                     (tSIRF_MSG_F_PRIORITY_RSP_ONE_CHANNEL *) &msg->channel[ii];
                  SIRFBINARY_EXPORT8(pMsg->channel_type, ptr);
                  SIRFBINARY_EXPORT8(pMsg->priority,     ptr);
               }
            }
         }
         break;

      case SIRF_MSG_F_CHANNEL_LOAD_RSP:
         {
            tSIRF_MSG_F_CHANNEL_LOAD_RSP * msg = (tSIRF_MSG_F_CHANNEL_LOAD_RSP *) message_structure;
            
            if ( *packet_length < ( (3 + 2 * msg->number_of_channels) * sizeof(tSIRF_UINT8) + 
                                     sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->port,               ptr);
               SIRFBINARY_EXPORT8(msg->total_load,         ptr);
               SIRFBINARY_EXPORT8(msg->number_of_channels, ptr);
               for (ii = 0; ii < msg->number_of_channels; ii++)
               {
                  tSIRF_MSG_F_CHANNEL_LOAD_RSP_ONE_CHANNEL * pMsg = 
                     (tSIRF_MSG_F_CHANNEL_LOAD_RSP_ONE_CHANNEL *) &msg->load_entry[ii]; 
                  SIRFBINARY_EXPORT8(pMsg->channel_type, ptr);
                  SIRFBINARY_EXPORT8(pMsg->load,         ptr);
               }
            }
         }
         break;

      default:
         tRet = SIRF_CODEC_ERROR_INVALID_MSG_ID;
         break;
   }

   if (SIRF_SUCCESS == tRet)
   {
      /* return length */
      *packet_length = ptr - packet;

      if ( *packet_length > SIRF_MSG_F_MAX_MESSAGE_LEN )
      {
         tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
      }
   }
   return tRet;

} /* SIRF_CODEC_F_EncodeSlc */

/***************************************************************************
 * @brief:   Convert a CP-generated structure to a packed byte-stream
 * @param[in]      message id - F Protocol message id, from the ICD
 * @param[in]      message_structure - data structure keyed to the message id
 * @param[in]      message_length - currently unused
 * @param[out]     packet - buffer pointer for destination formatted output
 * @param[in out]  packet_length - size of same, computed after processing
 * Returns:  SIRF_SUCCESS, SIRF_CODEC_ERROR_INVALID_PARAMETER,
 *           SIRF_CODEC_ERROR_INVALID_MSG_ID
***************************************************************************/

tSIRF_RESULT SIRF_CODEC_F_EncodeCp( tSIRF_UINT32 message_id, 
                                    tSIRF_VOID *message_structure, 
                                    tSIRF_UINT32 message_length,
                                    tSIRF_UINT8 *packet,
                                    tSIRF_UINT32 *packet_length )
{
   tSIRF_UINT8 *ptr;
   tSIRF_RESULT tRet = SIRF_SUCCESS;
   tSIRF_UINT8 mid = SIRF_GET_MID(message_id);
   tSIRF_UINT8 lc  = SIRF_GET_LC(message_id);
   
   /* Unused parameter */
   (void)message_length;
   
   /* First make sure the function was called right */
   if ( ((tSIRF_VOID *) NULL == message_structure) ||
        ((tSIRF_UINT8 *) NULL ==  packet) ||
        ((tSIRF_UINT32 *) NULL == packet_length) ||
        (lc != SIRF_LC_F))
   {
      return SIRF_CODEC_ERROR_INVALID_PARAMETER;
   }
   
   /* At a minimum, make sure we have room for the basics */
   if ( *packet_length < (sizeof(mid) + sizeof(lc)) )
   {
      return SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
   }

   ptr = packet;
   SIRFBINARY_EXPORT8(lc, ptr);    /* logical channel */
   SIRFBINARY_EXPORT8(mid, ptr);   /* message ID */

   switch (message_id)
   {
      case SIRF_MSG_F_SESSION_OPENING_REQ:
         {
            tSIRF_MSG_F_SESSION_OPENING_REQ * msg = (tSIRF_MSG_F_SESSION_OPENING_REQ *) message_structure;

            /* Check to make sure that the length of the buffer we are writing into is big enough */
            if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->ses_open_req, ptr);
            }
         }
         break;

      case SIRF_MSG_F_SESSION_CLOSING_REQ:
         {
            tSIRF_MSG_F_SESSION_CLOSING_REQ * msg = (tSIRF_MSG_F_SESSION_CLOSING_REQ  *) message_structure;
            
            if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->ses_close_req, ptr);
            }
         }
         break;

      case SIRF_MSG_F_HARDWARE_CONFIGURATION_RSP:
         {
            tSIRF_MSG_F_HARDWARE_CONFIGURATION_RSP * msg = 
               (tSIRF_MSG_F_HARDWARE_CONFIGURATION_RSP *) message_structure;
            
            if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + 1 * sizeof(tSIRF_UINT32) +
                                   sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8 (msg->hw_config_msg,     ptr);
               SIRFBINARY_EXPORT8 (msg->nominal_freq_high, ptr);
               SIRFBINARY_EXPORT32(msg->nominal_freq_low,  ptr);
            }
         }
         break;

      case SIRF_MSG_F_TIME_TRANSFER_RSP:
         {
            tSIRF_MSG_F_TIME_TRANSFER_RSP * msg = (tSIRF_MSG_F_TIME_TRANSFER_RSP *) message_structure;
            
            if ( *packet_length < (3 * sizeof(tSIRF_UINT8) + 
                                   1 * sizeof(tSIRF_UINT16) +
                                   3 /* 1 * sizeof(tSIRF_UINT24) */ + 
                                   1 * sizeof(tSIRF_UINT32) +
                                   sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8 (msg->tt_type ,      ptr);
               SIRFBINARY_EXPORT16(msg->week_num,      ptr);
               SIRFBINARY_EXPORT8 (msg->time_high,     ptr);
               SIRFBINARY_EXPORT32(msg->time_low,      ptr);
               SIRFBINARY_EXPORT24(msg->delta_utc,     ptr);
               SIRFBINARY_EXPORT8 (msg->time_accuracy, ptr);
            }
         }
         break;

      case SIRF_MSG_F_FREQ_TRANSFER_RSP:
         {
            tSIRF_MSG_F_FREQ_TRANSFER_RSP * msg = (tSIRF_MSG_F_FREQ_TRANSFER_RSP *) message_structure;
            
            if ( *packet_length < (3 * sizeof(tSIRF_UINT8) + 
                                   1 * sizeof(tSIRF_UINT16) +
                                   2 * sizeof(tSIRF_UINT32) +
                                   sizeof(mid) + sizeof(lc)) )       
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT16(msg->freq_offset,       ptr);
               SIRFBINARY_EXPORT8 (msg->accuracy,          ptr);
               SIRFBINARY_EXPORT32(msg->time_tag,          ptr);
               SIRFBINARY_EXPORT8 (msg->clock_ref,         ptr);
               SIRFBINARY_EXPORT8 (msg->nominal_freq_high, ptr);
               SIRFBINARY_EXPORT32(msg->nominal_freq_low,  ptr);
            }
         }
         break;

      case SIRF_MSG_F_APRX_MS_POSITION_RSP:
         {
            tSIRF_MSG_F_APRX_MS_POSITION_RSP * msg = (tSIRF_MSG_F_APRX_MS_POSITION_RSP *) message_structure;
            
            if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + 
                                   2 * sizeof(tSIRF_UINT16) +
                                   2 * sizeof(tSIRF_UINT32) +
                                   sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT32(msg->lat,            ptr);
               SIRFBINARY_EXPORT32(msg->lon,            ptr);
               SIRFBINARY_EXPORT16(msg->alt,            ptr);
               SIRFBINARY_EXPORT8 (msg->est_hor_err,    ptr);
               SIRFBINARY_EXPORT16(msg->est_ver_err,    ptr);
               SIRFBINARY_EXPORT8 (msg->use_alt_aiding, ptr);
            }
         }
         break;

      case SIRF_MSG_F_WIRELESS_POWER_RSP:
         {
            tSIRF_MSG_F_WIRELESS_POWER_RSP * msg = (tSIRF_MSG_F_WIRELESS_POWER_RSP *) message_structure;
            
            if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->wireless_power, ptr);
            }
         }
         break;

      case SIRF_MSG_F_REJECT_CP:
         {
            tSIRF_MSG_F_REJECT_CP * msg = (tSIRF_MSG_F_REJECT_CP *) message_structure;
            
            if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->rej_msgid, ptr);
               SIRFBINARY_EXPORT8(msg->reason,    ptr);
            }
         }
         break;

      case SIRF_MSG_F_RESET_GPS_COMMAND:
         {
            tSIRF_MSG_F_RESET_GPS_COMMAND * msg = (tSIRF_MSG_F_RESET_GPS_COMMAND *) message_structure;
            
            if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->reset_type, ptr);
            }
         }
         break;

      /* While not possible in normal operation, potential unit-test useage */ 
      case SIRF_MSG_F_SOFTWARE_VERSION_REQ:
         break; /* Zero length payload */

      /* While not possible in normal operation, potential unit-test useage */
      case SIRF_MSG_F_ICD_VERSION_REQ:
         break; /* Zero length payload */

      case SIRF_MSG_F_SET_APM:
         {
            tSIRF_MSG_F_SET_APM * msg = (tSIRF_MSG_F_SET_APM *) message_structure;
            
            if ( *packet_length < (4 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8 (msg->apm_enable,         ptr);
               SIRFBINARY_EXPORT8 (msg->power_duty_cycle,   ptr);
               SIRFBINARY_EXPORT8 (msg->time_duty_priority, ptr);
               SIRFBINARY_EXPORT8 (msg->spare1,             ptr);
            }
         }
         break;

      case SIRF_MSG_F_SET_BLANK:
         {
            tSIRF_MSG_F_SET_BLANK * msg = (tSIRF_MSG_F_SET_BLANK *) message_structure;
               
            if ( *packet_length < (4 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8 (msg->command,       ptr);
               SIRFBINARY_EXPORT8 (msg->air_interface, ptr);
               SIRFBINARY_EXPORT8 (msg->mode,          ptr);
               SIRFBINARY_EXPORT8 (msg->spare,         ptr);
            }
         }
         break;

      case SIRF_MSG_F_SERIAL_PORT_SETTINGS_REQ:
         {
            tSIRF_MSG_F_SERIAL_PORT_SETTINGS_REQ * msg = (tSIRF_MSG_F_SERIAL_PORT_SETTINGS_REQ *) message_structure;
            
            if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + 1 * sizeof(tSIRF_UINT32) +
                                   sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8 (msg->port,      ptr);
               SIRFBINARY_EXPORT32(msg->baud_rate, ptr);
               SIRFBINARY_EXPORT8 (msg->spare,     ptr);
            }
         }
         break;

      case SIRF_MSG_F_CHANNEL_OPEN_REQ:
         {
            tSIRF_MSG_F_CHANNEL_OPEN_REQ * msg = (tSIRF_MSG_F_CHANNEL_OPEN_REQ *) message_structure;
            
            if ( *packet_length < (3 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->port,         ptr);
               SIRFBINARY_EXPORT8(msg->channel_type, ptr);
               SIRFBINARY_EXPORT8(msg->priority,     ptr);
            }
         }
         break;

      case SIRF_MSG_F_CHANNEL_CLOSE_REQ:
         {
            tSIRF_MSG_F_CHANNEL_CLOSE_REQ * msg = (tSIRF_MSG_F_CHANNEL_CLOSE_REQ *) message_structure;
            
            if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->port,         ptr);
               SIRFBINARY_EXPORT8(msg->channel_type, ptr);
            }
         }
         break;

      case SIRF_MSG_F_CHANNEL_PRIORITY_REQ:
         {
            tSIRF_MSG_F_CHANNEL_PRIORITY_REQ * msg = (tSIRF_MSG_F_CHANNEL_PRIORITY_REQ *) message_structure;
            
            if ( *packet_length < (3 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->port,         ptr);
               SIRFBINARY_EXPORT8(msg->channel_type, ptr);
               SIRFBINARY_EXPORT8(msg->priority,     ptr);
            }
         }
         break;

      case SIRF_MSG_F_PRIORITY_QUERY:
         {
            tSIRF_MSG_F_PRIORITY_QUERY * msg = (tSIRF_MSG_F_PRIORITY_QUERY *) message_structure;
            
            if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->port,         ptr);
               SIRFBINARY_EXPORT8(msg->channel_type, ptr);
            }
         }
         break;

      case SIRF_MSG_F_CHANNEL_LOAD_QUERY:
         {
            tSIRF_MSG_F_PRIORITY_QUERY * msg = (tSIRF_MSG_F_PRIORITY_QUERY *) message_structure;
            
            if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->port,         ptr);
               SIRFBINARY_EXPORT8(msg->channel_type, ptr);
            }
         }
         break;

      case SIRF_MSG_F_PUSH_AIDING_AVAILABILITY_REQ:
         {
            tSIRF_MSG_F_PUSH_AIDING_AVAILABILITY_REQ * msg = 
                (tSIRF_MSG_F_PUSH_AIDING_AVAILABILITY_REQ *) message_structure;
            
            if ( *packet_length < (6 * sizeof(tSIRF_UINT8) + 
                                   2 * sizeof(tSIRF_UINT16) +
                                   sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8 (msg->aiding_availability_mask, ptr);
               SIRFBINARY_EXPORT8 (msg->forced_aiding_req_mask,   ptr);
               SIRFBINARY_EXPORT8 (msg->est_hor_err,              ptr);
               SIRFBINARY_EXPORT16(msg->est_ver_err,              ptr);
               SIRFBINARY_EXPORT8 (msg->rel_freq_acc,             ptr);
               SIRFBINARY_EXPORT8 (msg->time_accuracy_scale,      ptr);
               SIRFBINARY_EXPORT8 (msg->time_accuracy,            ptr);
               SIRFBINARY_EXPORT16(msg->spare,                    ptr);
            }
         }
         break;

      case SIRF_MSG_F_TIME_FREQ_APROX_POS_REQ:
         {
            tSIRF_MSG_F_TIME_FREQ_APROX_POS_REQ * msg = (tSIRF_MSG_F_TIME_FREQ_APROX_POS_REQ *) message_structure;
            
            if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->req_mask, ptr);
            }
         }
         break;

      default:
         tRet = SIRF_CODEC_ERROR_INVALID_MSG_ID;
         break;
   }

   if (SIRF_SUCCESS == tRet)
   {
      /* return length */
      *packet_length = ptr - packet;

      if ( *packet_length > SIRF_MSG_F_MAX_MESSAGE_LEN )
      {
         tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
      }
   }
   return tRet;

} /* SIRF_CODEC_F_EncodeCp */


/***************************************************************************
 * @brief:   Convert a CP-Generated packed bit-stream to to a structure 
 * @param[in]   payload - pointer to the byte stream per the ICD
 * @param[in]   payload_length - size in bytes of same
 * @param[out]  message_id - extracted F message ID byte, from the ICD
 * @param[out]  message_structure - data structure keyed to the message id
 * @param[out]  message_length - returned as the size of the structure
 * @return:  SIRF_SUCCESS, SIRF_CODEC_ERROR_INVALID_MSG_ID,
 *           SIRF_CODEC_ERROR_INVALID_MSG_LENGTH
***************************************************************************/

tSIRF_RESULT SIRF_CODEC_F_DecodeCp( tSIRF_UINT8 *payload, 
                                    tSIRF_UINT32 payload_length,
                                    tSIRF_UINT32 *message_id, 
                                    tSIRF_VOID *message_structure, 
                                    tSIRF_UINT32 *message_length )
{
   tSIRF_RESULT tRet = SIRF_SUCCESS;
   
   if ( ((tSIRF_UINT8 *) NULL !=  payload) && 
        ((tSIRF_UINT32 *) NULL != message_id) &&  
        ((tSIRF_VOID *) NULL != message_structure) && 
        ((tSIRF_UINT32 *) NULL != message_length) )
   {
      tSIRF_UINT8 *ptr = payload;
      tSIRF_UINT8 lc;
      tSIRF_UINT8 mid;
      
      /* At a minimum, make sure we have room for the basics */
      if ( payload_length < (sizeof(mid) + sizeof(lc)) )
      {
         return SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
      }
      
      lc = SIRFBINARY_IMPORT_UINT8(ptr);
      mid = SIRFBINARY_IMPORT_UINT8(ptr);

      if (lc != SIRF_LC_F){
         return SIRF_CODEC_ERROR_INVALID_MSG_ID;
      }
      
      *message_id = SIRF_MAKE_MSG_ID(lc,mid,0);
      switch (*message_id)
      {
         case SIRF_MSG_F_SESSION_OPENING_REQ:
            {
               tSIRF_MSG_F_SESSION_OPENING_REQ * msg = (tSIRF_MSG_F_SESSION_OPENING_REQ *) message_structure;
               
               /* Check to make sure that the buffer we are reading from is big enough */
               if ( payload_length < (1 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->ses_open_req = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_SESSION_CLOSING_REQ:
            {
               tSIRF_MSG_F_SESSION_CLOSING_REQ * msg = (tSIRF_MSG_F_SESSION_CLOSING_REQ *) message_structure;
               
               if ( payload_length < (1 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->ses_close_req = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_HARDWARE_CONFIGURATION_RSP:
            {
               tSIRF_MSG_F_HARDWARE_CONFIGURATION_RSP * msg = 
                  (tSIRF_MSG_F_HARDWARE_CONFIGURATION_RSP *) message_structure;
                  
               if ( payload_length < (2 * sizeof(tSIRF_UINT8) + 
                                      1 * sizeof(tSIRF_UINT32) +
                                      sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->hw_config_msg = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->nominal_freq_high = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->nominal_freq_low = SIRFBINARY_IMPORT_UINT32(ptr);
               }
            }
            break;

         case SIRF_MSG_F_TIME_TRANSFER_RSP:
            {
               tSIRF_MSG_F_TIME_TRANSFER_RSP * msg = (tSIRF_MSG_F_TIME_TRANSFER_RSP *) message_structure;
               
               if ( payload_length < (3 * sizeof(tSIRF_UINT8) + 
                                      1 * sizeof(tSIRF_UINT16) +
                                      3 /* 1 * sizeof(tSIRF_INT24) */ + 
                                      1 * sizeof(tSIRF_UINT32) +
                                      sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->tt_type = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->week_num = SIRFBINARY_IMPORT_UINT16(ptr);
                  msg->time_high = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->time_low = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->delta_utc = SIRFBINARY_IMPORT_SINT24(ptr);
                  msg->time_accuracy = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_FREQ_TRANSFER_RSP:
            {
               tSIRF_MSG_F_FREQ_TRANSFER_RSP * msg = (tSIRF_MSG_F_FREQ_TRANSFER_RSP *) message_structure;
               /* Minimum size is 8 bytes + LC + MID, maximum is 13 + LC + MID */
               if ( payload_length < (2 * sizeof(tSIRF_UINT8) + 
                                      1 * sizeof(tSIRF_UINT16) +
                                      1 * sizeof(tSIRF_UINT32) +
                                      sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->freq_offset = SIRFBINARY_IMPORT_UINT16(ptr);
                  msg->accuracy = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->time_tag = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->clock_ref = SIRFBINARY_IMPORT_UINT8(ptr);
                  /* Variable length message, see if there are more bytes indicated and available */
                  if( (msg->clock_ref & SIRF_MSG_F_FTRSP_RCI_NOMINAL_FREQ_INCLUDED_HERE) )
                  { 
                     if( payload_length < (3 * sizeof(tSIRF_UINT8)  + 
                                            1 * sizeof(tSIRF_UINT16) +
                                            2 * sizeof(tSIRF_UINT32) +
                                            sizeof(mid) + sizeof(lc)) )
                     {
                        tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
                     }
                     else
                     {
                        msg->nominal_freq_high = SIRFBINARY_IMPORT_UINT8(ptr);
                        msg->nominal_freq_low = SIRFBINARY_IMPORT_UINT32(ptr);
                     }
                  }
                  else
                  {
                     msg->nominal_freq_high = 0;
                     msg->nominal_freq_low  = 0;
                  }
               }
            }
            break;

         case SIRF_MSG_F_APRX_MS_POSITION_RSP:
            {
               tSIRF_MSG_F_APRX_MS_POSITION_RSP * msg = (tSIRF_MSG_F_APRX_MS_POSITION_RSP *) message_structure;
               
               if ( payload_length < (2 * sizeof(tSIRF_UINT8) + 
                                      2 * sizeof(tSIRF_UINT16) +
                                      2 * sizeof(tSIRF_UINT32) +
                                      sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->lat = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->lon = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->alt = SIRFBINARY_IMPORT_UINT16(ptr);
                  msg->est_hor_err = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->est_ver_err = SIRFBINARY_IMPORT_UINT16(ptr);
                  msg->use_alt_aiding = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_WIRELESS_POWER_RSP:
            {
               tSIRF_MSG_F_WIRELESS_POWER_RSP * msg = (tSIRF_MSG_F_WIRELESS_POWER_RSP *) message_structure;
               
               if ( payload_length < (1 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->wireless_power = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_REJECT_CP:
            {
               tSIRF_MSG_F_REJECT_CP * msg = (tSIRF_MSG_F_REJECT_CP *) message_structure;
               
               if ( payload_length < (2 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->rej_msgid = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->reason    = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_RESET_GPS_COMMAND:
            {
               tSIRF_MSG_F_RESET_GPS_COMMAND * msg = (tSIRF_MSG_F_RESET_GPS_COMMAND *) message_structure;
               
               if ( payload_length < (1 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->reset_type   = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_SOFTWARE_VERSION_REQ:
            {
              tSIRF_MSG_F_SOFTWARE_VERSION_REQ * msg = (tSIRF_MSG_F_SOFTWARE_VERSION_REQ *) message_structure;
               
               if ( payload_length < (sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {  /* Note payload is size zero */
                  *message_length = sizeof(*msg);
                  msg->reserved = 0;
               }
            }
            break;

         case SIRF_MSG_F_ICD_VERSION_REQ:
            {
              tSIRF_MSG_F_ICD_VERSION_REQ * msg = (tSIRF_MSG_F_ICD_VERSION_REQ *) message_structure;
               
               if ( payload_length < (sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  /* Note payload is size zero */
                  *message_length = sizeof(*msg);
                  msg->reserved = 0;
               }
            }
            break;


         case SIRF_MSG_F_SET_APM:
            {
               tSIRF_MSG_F_SET_APM * msg = (tSIRF_MSG_F_SET_APM *) message_structure;
               
               if ( payload_length < (4 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->apm_enable         = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->power_duty_cycle   = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->time_duty_priority = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->spare1             = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_SET_BLANK:
            {
               tSIRF_MSG_F_SET_BLANK * msg = (tSIRF_MSG_F_SET_BLANK *) message_structure;
               
               if ( payload_length < (4 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->command         = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->air_interface   = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->mode            = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->spare           = SIRFBINARY_IMPORT_UINT32(ptr);
               }
            }
            break;

         case SIRF_MSG_F_SERIAL_PORT_SETTINGS_REQ:
            {
               tSIRF_MSG_F_SERIAL_PORT_SETTINGS_REQ * msg = 
                  (tSIRF_MSG_F_SERIAL_PORT_SETTINGS_REQ *) message_structure;
               
               if ( payload_length < (2 * sizeof(tSIRF_UINT8) + 
                                      1 * sizeof(tSIRF_UINT32) +
                                      sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->port      = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->baud_rate = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->spare     = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_CHANNEL_OPEN_REQ:
            {
               tSIRF_MSG_F_CHANNEL_OPEN_REQ * msg = (tSIRF_MSG_F_CHANNEL_OPEN_REQ *) message_structure;
               
               if ( payload_length < (3 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->port         = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->channel_type = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->priority     = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_CHANNEL_CLOSE_REQ:
            {
               tSIRF_MSG_F_CHANNEL_CLOSE_REQ * msg = (tSIRF_MSG_F_CHANNEL_CLOSE_REQ *) message_structure;
               
               if ( payload_length < (2 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->port         = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->channel_type = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_CHANNEL_PRIORITY_REQ:
            {
               tSIRF_MSG_F_CHANNEL_PRIORITY_REQ * msg = (tSIRF_MSG_F_CHANNEL_PRIORITY_REQ *) message_structure;
               
               if ( payload_length < (3 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->port         = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->channel_type = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->priority     = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_PRIORITY_QUERY:
            {
               tSIRF_MSG_F_PRIORITY_QUERY * msg = (tSIRF_MSG_F_PRIORITY_QUERY *) message_structure;
              
               if ( payload_length < (2 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->port         = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->channel_type = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_CHANNEL_LOAD_QUERY:
            {
               tSIRF_MSG_F_CHANNEL_LOAD_QUERY * msg = (tSIRF_MSG_F_CHANNEL_LOAD_QUERY *) message_structure;
               
               if ( payload_length < (2 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->port         = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->mode         = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_PUSH_AIDING_AVAILABILITY_REQ:
            {
               tSIRF_MSG_F_PUSH_AIDING_AVAILABILITY_REQ * msg = 
                  (tSIRF_MSG_F_PUSH_AIDING_AVAILABILITY_REQ *) message_structure;
                  
               if ( payload_length < (6 * sizeof(tSIRF_UINT8) + 
                                      2 * sizeof(tSIRF_UINT16) +
                                      sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->aiding_availability_mask = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->forced_aiding_req_mask = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->est_hor_err = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->est_ver_err = SIRFBINARY_IMPORT_UINT16(ptr);
                  msg->rel_freq_acc = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->time_accuracy_scale = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->time_accuracy = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->spare = SIRFBINARY_IMPORT_UINT16(ptr);
               }
            }
            break;

         case SIRF_MSG_F_TIME_FREQ_APROX_POS_REQ:
            {
               tSIRF_MSG_F_TIME_FREQ_APROX_POS_REQ * msg = 
                  (tSIRF_MSG_F_TIME_FREQ_APROX_POS_REQ *) message_structure;
                  
               if ( payload_length < (1 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->req_mask = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         default:
            *message_id = 0;
            *message_length = 0;
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_ID;
      }
   }
   else
   {
      if ( (tSIRF_UINT32 *) NULL != message_id )
      {
         *message_id = 0;
      }
      if ( (tSIRF_UINT32 *) NULL != message_length )
      {
         *message_length = 0;
      }
      tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
   }
   return tRet;
}

/***************************************************************************
 * @brief:   Convert an SLC-Generated packed bit-stream to to a structure 
 * @param[in]   payload - pointer to the byte stream per the ICD
 * @param[in]   payload_length - size in bytes of same
 * @param[out]  message_id - extracted F message ID byte, from the ICD
 * @param[out]  message_structure - data structure keyed to the message id
 * @param[out]  message_length - returned as the size of the structure
 * Returns:  SIRF_SUCCESS, SIRF_CODEC_ERROR_INVALID_MSG_LENGTH,
 *           SIRF_CODEC_ERROR_INVALID_PARAMETER, or
 *           SIRF_CODEC_ERROR_INVALID_MSG_ID.
***************************************************************************/

tSIRF_RESULT SIRF_CODEC_F_DecodeSlc( tSIRF_UINT8 *payload, 
                                     tSIRF_UINT32 payload_length,
                                     tSIRF_UINT32 *message_id, 
                                     tSIRF_VOID *message_structure, 
                                     tSIRF_UINT32 *message_length )
{
   tSIRF_RESULT tRet = SIRF_SUCCESS;
   
   if ( ((tSIRF_UINT8 *) NULL !=  payload) && 
        ((tSIRF_UINT32 *) NULL != message_id) &&  
        ((tSIRF_VOID *) NULL != message_structure) && 
        ((tSIRF_UINT32 *) NULL != message_length) )
   {
      tSIRF_UINT8 *ptr = payload;
      tSIRF_UINT32 ii;
      tSIRF_UINT8 lc;
      tSIRF_UINT8 mid;
      
      /* At a minimum, make sure we have room for the basics */
      if ( payload_length < (sizeof(mid) + sizeof(lc)) )
      {
         return SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
      }
      
      lc = SIRFBINARY_IMPORT_UINT8(ptr);
      mid = SIRFBINARY_IMPORT_UINT8(ptr);
      
      *message_id = SIRF_MAKE_MSG_ID(lc,mid,0);
      
      switch (*message_id)
      {
         case SIRF_MSG_F_SESSION_OPENING_NOTIFICATION:
            {
               tSIRF_MSG_F_SESSION_OPENING_NOTIFICATION * msg = 
                  (tSIRF_MSG_F_SESSION_OPENING_NOTIFICATION *) message_structure;
                  
               if ( payload_length < (1 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->ses_open_not = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_SESSION_CLOSING_NOTIFICATION:
            {
               tSIRF_MSG_F_SESSION_CLOSING_NOTIFICATION * msg = 
                  (tSIRF_MSG_F_SESSION_CLOSING_NOTIFICATION *) message_structure;
                  
               if ( payload_length < (1 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->ses_close_not = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_ERROR_NOTIFICATION:
            {
               tSIRF_MSG_F_ERROR_NOTIFICATION * msg = (tSIRF_MSG_F_ERROR_NOTIFICATION *) message_structure;
               
               if ( payload_length < (1 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->session_error_reason = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_SLC_STATUS:
            {
               tSIRF_MSG_F_SLC_STATUS * msg = (tSIRF_MSG_F_SLC_STATUS *) message_structure;
               
               if ( payload_length < (1 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->slc_status = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_HARDWARE_CONFIGURATION_REQ:
            *message_length = 0;
            break;

         case SIRF_MSG_F_TIME_TRANSFER_REQ:
            *message_length = 0;
            break;

         case SIRF_MSG_F_FREQ_TRANSFER_REQ:
            {
               tSIRF_MSG_F_FREQ_TRANSFER_REQ * msg = (tSIRF_MSG_F_FREQ_TRANSFER_REQ *) message_structure;
              
               if ( payload_length < (1 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->req_info = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_APRX_MS_POSITION_REQ:
            *message_length = 0;
            break;

         case SIRF_MSG_F_WIRELESS_POWER_REQ:
            *message_length = 0;
            break;

         case SIRF_MSG_F_REJECT_SLC:
            {
               tSIRF_MSG_F_REJECT_SLC * msg = (tSIRF_MSG_F_REJECT_SLC *) message_structure;
               
               if ( payload_length < (2 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->rej_msgid = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->reason = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_SOFTWARE_VERSION_RSP:
            {
               tSIRF_MSG_F_SOFTWARE_VERSION_RSP * msg = (tSIRF_MSG_F_SOFTWARE_VERSION_RSP *) message_structure;
               
               if ( payload_length < ( 2 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  msg->sirf_ver_bytes = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->cust_ver_bytes = SIRFBINARY_IMPORT_UINT8(ptr);

                  if ( payload_length < ( (2 * sizeof(tSIRF_UINT8)) +
                                          (msg->sirf_ver_bytes * sizeof(tSIRF_UINT8)) +
                                          (msg->cust_ver_bytes * sizeof(tSIRF_UINT8)) +
                                           sizeof(mid) + sizeof(lc)) )
                  {
                     tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
                  }
                  else
                  {
                     *message_length = sizeof(*msg);
                     for (ii = 0; ii < msg->sirf_ver_bytes; ii++)
                     {
                        msg->sirf_ver_str[ii] = SIRFBINARY_IMPORT_UINT8(ptr);
                     }
                     for (ii = 0; ii < msg->cust_ver_bytes; ii++)
                     {
                        msg->cust_ver_str[ii] = SIRFBINARY_IMPORT_UINT8(ptr);
                     }
                  }
               }
            }
            break;

         case SIRF_MSG_F_ICD_VERSION_RSP:
            {
               tSIRF_MSG_F_ICD_VERSION_RSP * msg = (tSIRF_MSG_F_ICD_VERSION_RSP *) message_structure;
               
               if ( payload_length < ( 6 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->f_icd_version_major   = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->f_icd_version_minor   = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->ai3_icd_version_major = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->ai3_icd_version_major = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->spare1                = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->spare2                = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_PUSH_AIDING_ACK_NACK:
            {
               tSIRF_MSG_F_PUSH_AIDING_ACK_NACK * msg = (tSIRF_MSG_F_PUSH_AIDING_ACK_NACK *) message_structure;
               
               if ( payload_length < (2 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->ack_nack = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->message_id = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_TIME_FREQ_APROX_POS_RSP:
            {
               tSIRF_MSG_F_TIME_FREQ_APROX_POS_RSP * msg = (tSIRF_MSG_F_TIME_FREQ_APROX_POS_RSP *) message_structure;
               
               if ( payload_length < (5 * sizeof(tSIRF_UINT8) + 
                                      3 * sizeof(tSIRF_UINT16) +
                                      5 * sizeof(tSIRF_UINT32) +
                                      sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->status_resp_mask = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->gps_week = SIRFBINARY_IMPORT_UINT16(ptr);
                  msg->gps_tow = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->time_acc_scale = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->time_accuracy = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->freq_acc_scale = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->freq_accuracy = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->scaled_freq_offset = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->freq_time_tag = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->slc_hor_unc = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->slc_ver_unc = SIRFBINARY_IMPORT_UINT16(ptr);
                  msg->spare03 = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->spare47 = SIRFBINARY_IMPORT_UINT32(ptr);
               }
            }
            break;

         case SIRF_MSG_F_ACK_APM:
            {
               tSIRF_MSG_F_ACK_APM * msg = (tSIRF_MSG_F_ACK_APM *) message_structure;
               
               if ( payload_length < (2 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->apm_state = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->spare = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_SERIAL_PORT_SETTINGS_RSP:
            {
               tSIRF_MSG_F_SERIAL_PORT_SETTINGS_RSP * msg = 
                  (tSIRF_MSG_F_SERIAL_PORT_SETTINGS_RSP *) message_structure;
               
               if ( payload_length < (3 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->port = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->baud_rate = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->ack_number = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_CHANNEL_OPEN_RSP:
            {
               tSIRF_MSG_F_CHANNEL_OPEN_RSP * msg = (tSIRF_MSG_F_CHANNEL_OPEN_RSP *) message_structure;
               
               if ( payload_length < (3 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->port = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->channel_type = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->priority = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_CHANNEL_CLOSE_RSP:
            {
               tSIRF_MSG_F_CHANNEL_CLOSE_RSP * msg = (tSIRF_MSG_F_CHANNEL_CLOSE_RSP *) message_structure;
               
               if ( payload_length < (2 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->port = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->channel_type = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_CHANNEL_PRIORITY_RSP:
            {
               tSIRF_MSG_F_CHANNEL_PRIORITY_RSP * msg = (tSIRF_MSG_F_CHANNEL_PRIORITY_RSP *) message_structure;
               
               if ( payload_length < (3 * sizeof(tSIRF_UINT8) + sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->port = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->channel_type = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->priority = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

         case SIRF_MSG_F_PRIORITY_RSP:
            {
               tSIRF_MSG_F_PRIORITY_RSP * msg = (tSIRF_MSG_F_PRIORITY_RSP *) message_structure;
               
               if ( payload_length < ( (2 + 2 * msg->number_of_channels) * sizeof(tSIRF_UINT8) + 
                                        sizeof(mid) + sizeof(lc)) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->port = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->number_of_channels = SIRFBINARY_IMPORT_UINT8(ptr);
                  for (ii = 0; ii < msg->number_of_channels; ii++)
                  {
                     tSIRF_MSG_F_PRIORITY_RSP_ONE_CHANNEL * pMsg = 
                        (tSIRF_MSG_F_PRIORITY_RSP_ONE_CHANNEL *) &msg->channel[ii];
                     pMsg->channel_type = SIRFBINARY_IMPORT_UINT8(ptr);
                     pMsg->priority = SIRFBINARY_IMPORT_UINT8(ptr);
                     SIRFBINARY_EXPORT8(pMsg->priority,     ptr); /* What is this? */
                  }
               }
            }
            break;

/***************************************************************************
   SIRF_MSG_F_CHANNEL_LOAD_QUERY                 ( 0xA8 )   (CP)
   SIRF_MSG_F_CHANNEL_LOAD_RSP                   ( 0xA8 )   (SLC)
***************************************************************************/
         case SIRF_MSG_F_CHANNEL_LOAD_RSP:
            {
               tSIRF_MSG_F_CHANNEL_LOAD_RSP * msg = (tSIRF_MSG_F_CHANNEL_LOAD_RSP *) message_structure;
               
               if ( payload_length < ( (3 + 2 * msg->number_of_channels) * sizeof(tSIRF_UINT8) + 
                                        sizeof(mid) + sizeof(lc)) )               
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->port = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->total_load = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->number_of_channels = SIRFBINARY_IMPORT_UINT8(ptr);
                  for (ii = 0; ii < msg->number_of_channels; ii++)
                  {
                     tSIRF_MSG_F_CHANNEL_LOAD_RSP_ONE_CHANNEL * pMsg = 
                        (tSIRF_MSG_F_CHANNEL_LOAD_RSP_ONE_CHANNEL *) &msg->load_entry[ii];
                     pMsg->channel_type = SIRFBINARY_IMPORT_UINT8(ptr);
                     pMsg->load = SIRFBINARY_IMPORT_UINT8(ptr);
                  }
               }
            }
            break;

         default:
            *message_id = 0;
            *message_length = 0;
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_ID;
      }
   }
   else
   {
      if ( (tSIRF_UINT32 *) NULL != message_id )
      {
         *message_id = 0;
      }
      if ( (tSIRF_UINT32 *) NULL != message_length )
      {
         *message_length = 0;
      }
      tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
   }
   return tRet;
}

/**
 * @}
 */
