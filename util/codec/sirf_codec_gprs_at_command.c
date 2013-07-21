/**
 * @addtogroup platform_src_sirf_util_codec
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2005-2009 by SiRF Technology, Inc.  All rights reserved.
 *
 *    This Software is protected by United States copyright laws and
 *    international treaties.  You may not reverse engineer, decompile
 *    or disassemble this Software.
 *
 *    WARNING:
 *    This Software contains SiRF Technology Inc.’s confidential and
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this
 *    Software without SiRF Technology, Inc.’s  express written
 *    permission.   Use of any portion of the contents of this Software
 *    is subject to and restricted by your signed written agreement with
 *    SiRF Technology, Inc.
 *
 */

/**
 * @file   sirf_codec_gprs_at_command.c
 *
 * @brief  Routines to convert data to and from a byte stream
 *         defined by the AT Commands for the MultiModem GPRS
 *         to the Tracker API defined in the Programmers
 *         Reference Manual.
 *
 */




/* ----------------------------------------------------------------------------
 *   Included files
 * ------------------------------------------------------------------------- */

#include <stdio.h>
#include <string.h>

#include "sirf_types.h"
#include "sirf_msg.h"
#include "sirf_msg_gprs_at_command.h"
#include "sirf_codec.h"
#include "sirf_codec_gprs_at_command.h"
#include "string_sif.h"
/* ----------------------------------------------------------------------------
 *   Definitions
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 *    Types, Enums, and Structs
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 *    Global Variables
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 *    Local Variables
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 *    Function Prototypes
 * ------------------------------------------------------------------------- */

/***************************************************************************
 * @brief:   Convert a message structure to a byte stream
 * @param[in]:     message id - GPRS_AT_COMMAND message id, from the GPRS_AT_COMMAND manual
 * @param[in]:     message_structure - data structure keyed to the message id
 * @param[in]:     message_length - used as string length if not a structure
 * @param[out]:    packet - buffer pointer for destination formatted output
 * @param[in out]: packet_length - size of same, computed after processing
 * @return:   SIRF_SUCCESS, 
 *            SIRF_CODEC_GPRS_AT_COMMAND_LENGTH_ERROR,
 *            SIRF_CODEC_ERROR_INVALID_MSG_LENGTH,
 *            SIRF_CODEC_ERROR_INVALID_MSG_ID
***************************************************************************/
tSIRF_RESULT SIRF_CODEC_GPRS_AT_COMMAND_Encode(
   tSIRF_UINT32               message_id, 
   tSIRF_VOID   const * const message_structure,
   tSIRF_UINT32               message_length,
   tSIRF_UINT8        * const payload, 
   tSIRF_UINT32       * const payload_length,
   tSIRF_UINT32       * const options )
{
   tSIRF_RESULT result = SIRF_FAILURE;

   tSIRF_UINT32 available_buffer = *payload_length;

   *options = SIRF_CODEC_OPTIONS_MAKE_MSG_NUMBER(1,1); /* Message 1 of 1 */
   (void)message_length;

   switch (message_id)
   {
   case SIRF_MSG_GPRS_AT_COMMAND_PCGMI:
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PCGMI_FORMAT);
      result = SIRF_SUCCESS;
      break;
   case SIRF_MSG_GPRS_AT_COMMAND_PCGMM:
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PCGMM_FORMAT);
      result = SIRF_SUCCESS;
      break;
   case SIRF_MSG_GPRS_AT_COMMAND_PCMEE:
   {
      tSIRF_MSG_GPRS_AT_COMMAND_PCMEE const *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PCMEE const *) message_structure;
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PCMEE_FORMAT,
                                 (int)msg->mode);
      result = SIRF_SUCCESS;
      break;
   }
   case SIRF_MSG_GPRS_AT_COMMAND_PIFC:
   {
      tSIRF_MSG_GPRS_AT_COMMAND_PIFC const *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PIFC const*) message_structure;
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PIFC_FORMAT,
                                 (int)msg->dce_by_dte,
                                 (int)msg->dte_by_dce);
      result = SIRF_SUCCESS;
      break;
   }

   /*=========================================================================*/
   /* TCP/IP Commands */
   case SIRF_MSG_GPRS_AT_COMMAND_PWOPEN:
   {
      tSIRF_MSG_GPRS_AT_COMMAND_PWOPEN const *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PWOPEN const*) message_structure;
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PWOPEN_FORMAT,
                                 (int)msg->state);
      result = SIRF_SUCCESS;
      break;
   }
   /* PWIPCFG commands */
   case SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_STOP_TCPIP:
   {
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_STOP_TCPIP_FORMAT);
      result = SIRF_SUCCESS;
      break;
   }

   case SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_START_TCPIP:
   {
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_START_TCPIP_FORMAT);
      result = SIRF_SUCCESS;
      break;
   }
   case SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_CONFIGURE_TCPIP:
   {
      tSIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_CONFIGURE_TCPIP const *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_CONFIGURE_TCPIP const*) message_structure;
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_CONFIGURE_TCPIP_FORMAT,
                                 (int)msg->opt_num,
                                 (int)msg->value);
      result = SIRF_SUCCESS;
      break;
   }
   case SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_TCPIP_VERSION:
   {
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_TCPIP_VERSION_FORMAT);
      result = SIRF_SUCCESS;
      break;
   }
   case SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_STORE_TCPIP_CFG:
   {
      tSIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_STORE_TCPIP_CFG const *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_STORE_TCPIP_CFG const*) message_structure;
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_STORE_TCPIP_CFG_FORMAT,
                                 (int)msg->mode);
      result = SIRF_SUCCESS;
      break;
   }
   
   case SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_CLOSE:
   {
      tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_CLOSE const *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_CLOSE const*) message_structure;
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_CLOSE_FORMAT,
                                 (int)msg->bid);
      result = SIRF_SUCCESS;
      break;
   }
   case SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_OPEN:
   {
      tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_OPEN const *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_OPEN const*) message_structure;
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_OPEN_FORMAT,
                                 (int)msg->bid);
      result = SIRF_SUCCESS;
      break;
   }
   case SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_SET:
   {
      tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_SET const *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_SET const*) message_structure;
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_SET_FORMAT,
                                 (int)msg->bid,
                                 (int)msg->opt_num,
                                 msg->value);
      result = SIRF_SUCCESS;
      break;
   }
   case SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_GET:
   {
      tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_GET const *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_GET const*) message_structure;
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_GET_FORMAT,
                                 (int)msg->bid,
                                 (int)msg->opt_num);
      result = SIRF_SUCCESS;
      break;
   }
   case SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_START:
   {
      tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_START const *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_START const*) message_structure;
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_START_FORMAT,
                                 (int)msg->bid,
                                 (int)msg->mode,
                                 msg->login,
                                 msg->password,
                                 msg->caller_identity);
      result = SIRF_SUCCESS;
      break;
   }
   case SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_STOP:
   {
      tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_STOP const *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_STOP const*) message_structure;
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_STOP_FORMAT,
                                 (int)msg->bid);
      result = SIRF_SUCCESS;
      break;
   }
   case SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_CFG_MANAGEMENT:
   {
      tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_CFG_MANAGEMENT const *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_CFG_MANAGEMENT const*) message_structure;
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_CFG_MANAGEMENT_FORMAT,
                                 (int)msg->bid,
                                 (int)msg->mode);
      result = SIRF_SUCCESS;
      break;
   }
   case SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_UDP:
   {
      tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_UDP const *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_UDP const*) message_structure;
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_UDP_FORMAT,
                                 (int)msg->communication_index,
                                 (int)msg->local_port,
                                 msg->peer_ip,
                                 (int)msg->peer_port);
      result = SIRF_SUCCESS;
      break;
   }
   case SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_TCP_CLIENT:
   {
      tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_TCP_CLIENT const *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_TCP_CLIENT const*) message_structure;
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_TCP_CLIENT_FORMAT,
                                 (int)msg->communication_index,
                                 msg->peer_ip,
                                 (int)msg->peer_port);
      result = SIRF_SUCCESS;
      break;
   }
   case SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_TCP_SERVER:
   {
      tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_TCP_SERVER const *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_TCP_SERVER const*) message_structure;
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_TCP_SERVER_FORMAT,
                                 (int)msg->server_index,
                                 (int)msg->local_port,
                                 (int)msg->idx_from,
                                 (int)msg->idx_to);
      result = SIRF_SUCCESS;
      break;
   }
   case SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_FTP:
   {
      tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_FTP const *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_FTP const*) message_structure;
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_FTP_FORMAT,
                                 (int)msg->index,
                                 msg->server,
                                 (int)msg->peer_port,
                                 msg->username,
                                 msg->password,
                                 msg->account);
      result = SIRF_SUCCESS;
      break;
   }
   case SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_HTTP_CLIENT:
   {
      tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_HTTP_CLIENT const *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_HTTP_CLIENT const*) message_structure;
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_HTTP_CLIENT_FORMAT,
                                 (int)msg->index,
                                 msg->server,
                                 (int)msg->peer_port,
                                 msg->username,
                                 msg->password,
                                 msg->header_list);
      result = SIRF_SUCCESS;
      break;
   }
   case SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_SMTP_CLIENT:
   {
      tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_SMTP_CLIENT const *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_SMTP_CLIENT const*) message_structure;
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_SMTP_CLIENT_FORMAT,
                                 (int)msg->index,
                                 msg->server,
                                 (int)msg->peer_port,
                                 msg->username,
                                 msg->password);
      result = SIRF_SUCCESS;
      break;
   }
   case SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_POP3_CLIENT:
   {
      tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_POP3_CLIENT const *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_POP3_CLIENT const*) message_structure;
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_POP3_CLIENT_FORMAT,
                                 (int)msg->index,
                                 msg->server,
                                 (int)msg->peer_port,
                                 msg->username,
                                 msg->password);
      result = SIRF_SUCCESS;
      break;
   }
   case SIRF_MSG_GPRS_AT_COMMAND_PWIPDATA:
   {
      tSIRF_MSG_GPRS_AT_COMMAND_PWIPDATA const *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PWIPDATA const*) message_structure;
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PWIPDATA_FORMAT,
                                 (int)msg->protocol,
                                 (int)msg->idx,
                                 (int)msg->mode);
      result = SIRF_SUCCESS;
      break;
   }
   case SIRF_MSG_GPRS_AT_COMMAND_PWIPCLOSE:
   {
      tSIRF_MSG_GPRS_AT_COMMAND_PWIPCLOSE const *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PWIPCLOSE const*) message_structure;
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PWIPCLOSE_FORMAT,
                                 (int)msg->protocol,
                                 (int)msg->idx);
      result = SIRF_SUCCESS;
      break;
   }

   case SIRF_MSG_GPRS_AT_COMMAND_PCCED:
   {
      tSIRF_MSG_GPRS_AT_COMMAND_PCCED const *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PCCED const*) message_structure;
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PCCED_FORMAT,
                                 (int)msg->mode,
                                 (int)msg->requested_dump,
                                 msg->csq_step);
      result = SIRF_SUCCESS;
      break;
   }

   case SIRF_MSG_GPRS_AT_COMMAND_PCIMI:
   {
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_PCIMI_FORMAT);
      result = SIRF_SUCCESS;
      break;
   }

   case SIRF_MSG_GPRS_AT_COMMAND_E:
      *payload_length = snprintf((char*)payload,
                                 *payload_length,
                                 SIRF_MSG_GPRS_AT_COMMAND_E_FORMAT,
                                 (int)((tSIRF_MSG_GPRS_AT_COMMAND_E const*)message_structure)->echo);
      result = SIRF_SUCCESS;
      break;
   default:
      result = SIRF_CODEC_ERROR_INVALID_MSG_ID;
   }

   if (*payload_length >= available_buffer)
   {
      result = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
   }
   return result;
} /* SIRF_CODEC_GPRS_AT_COMMAND_Encode() */



/***************************************************************************
 * @brief:   Convert a byte stream to a formatted GPRS_AT_COMMAND structure
 *
 * @param payload           - contains the byte stream, in protocol format
 * @param payload_length    - the size of same, including overhead
 * @param message_id        - GPRS_AT_COMMAND message id, derived from the input
 * @param message_structure - data structure keyed to the message id
 * @param message_length    - pointer to the length of the message
 * @param options           - Options parameter defined in sirf_msg.h
 * @return:  SIRF_SUCCESS, or error code
 ***************************************************************************/
tSIRF_RESULT SIRF_CODEC_GPRS_AT_COMMAND_Decode(
   tSIRF_UINT8  const * const payload, 
   tSIRF_UINT32               payload_length,
   tSIRF_UINT32       * const message_id, 
   tSIRF_VOID         * const message_structure,
   tSIRF_UINT32       * const message_length,
   tSIRF_UINT32       * const options )
{
   /* default */
   *options = SIRF_CODEC_OPTIONS_MAKE_MSG_NUMBER(1,1);
   
   /* OK message */
   if (0 == strncmp((const char*)payload,
                    SIRF_MSG_GPRS_AT_COMMAND_OK_FORMAT,
                    sizeof(SIRF_MSG_GPRS_AT_COMMAND_OK_FORMAT)-1))
   {
      *message_length = sizeof(tSIRF_MSG_GPRS_AT_COMMAND_OK);
      *message_id = SIRF_MSG_GPRS_AT_COMMAND_OK;
      return SIRF_SUCCESS;
   }

   /* ERROR message */
   if (0 == strncmp((const char*)payload,
                    SIRF_MSG_GPRS_AT_COMMAND_ERROR_FORMAT,
                    sizeof(SIRF_MSG_GPRS_AT_COMMAND_ERROR_FORMAT)-1))
   {
      *message_length = sizeof(tSIRF_MSG_GPRS_AT_COMMAND_ERROR);
      *message_id = SIRF_MSG_GPRS_AT_COMMAND_ERROR;
      return SIRF_SUCCESS;
   }

   /* CONNECT message */
   if (0 == strncmp((const char*)payload,
                    SIRF_MSG_GPRS_AT_COMMAND_CONNECT_FORMAT,
                    sizeof(SIRF_MSG_GPRS_AT_COMMAND_CONNECT_FORMAT)-1))
   {
      *message_length = sizeof(tSIRF_MSG_GPRS_AT_COMMAND_CONNECT);
      *message_id = SIRF_MSG_GPRS_AT_COMMAND_CONNECT;
      return SIRF_SUCCESS;
   }
   
   /* +CME ERROR: message */
   {
      int error;
      if (0 != sscanf((const char*)payload,
                      SIRF_MSG_GPRS_AT_COMMAND_PCME_ERROR_FORMAT,
                      &error))
      {
         *message_length = sizeof(tSIRF_MSG_GPRS_AT_COMMAND_PCME_ERROR);
         *message_id = SIRF_MSG_GPRS_AT_COMMAND_PCME_ERROR;
         if (NULL != message_structure)
         {
            tSIRF_MSG_GPRS_AT_COMMAND_PCME_ERROR *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PCME_ERROR*)message_structure;
            msg->error = (tSIRF_UINT32)error;
         }
         return SIRF_SUCCESS;
      }
   }

   /* +WIPREADY: message */
   {
      int mode;
      int communication_index;
      if (0 != sscanf((const char*)payload,
                      SIRF_MSG_GPRS_AT_COMMAND_PWIPREADY_FORMAT,
                      &mode,
                      &communication_index))
      {
         *message_length = sizeof(tSIRF_MSG_GPRS_AT_COMMAND_PWIPREADY);
         *message_id = SIRF_MSG_GPRS_AT_COMMAND_PWIPREADY;
         if (NULL != message_structure)
         {
            tSIRF_MSG_GPRS_AT_COMMAND_PWIPREADY *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PWIPREADY*)message_structure;
            msg->mode = (tSIRF_UINT32)mode;
            msg->communication_index = (tSIRF_UINT32)communication_index;
         }
         return SIRF_SUCCESS;
      }
   }

   /* +WIPPEERCLOSE: message */
   {
      int mode;
      int communication_index;
      if (0 != sscanf((const char*)payload,
                      SIRF_MSG_GPRS_AT_COMMAND_PWIPPEERCLOSE_FORMAT,
                      &mode,
                      &communication_index))
      {
         *message_length = sizeof(tSIRF_MSG_GPRS_AT_COMMAND_PWIPPEERCLOSE);
         *message_id = SIRF_MSG_GPRS_AT_COMMAND_PWIPPEERCLOSE;
         if (NULL != message_structure)
         {
            tSIRF_MSG_GPRS_AT_COMMAND_PWIPPEERCLOSE *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PWIPPEERCLOSE*)message_structure;
            msg->mode = (tSIRF_UINT32)mode;
            msg->communication_index = (tSIRF_UINT32)communication_index;
         }
         return SIRF_SUCCESS;
      }
   }

   /* +CCED: message */
   {
      tSIRF_MSG_GPRS_AT_COMMAND_PCCED_RESPONSE_STRING string;
      if (0 != sscanf((const char*)payload,
                      SIRF_MSG_GPRS_AT_COMMAND_PCCED_RESPONSE_FORMAT,
                      string.string))
      {
         *message_length = sizeof(tSIRF_MSG_GPRS_AT_COMMAND_PCCED_RESPONSE_STRING);
         *message_id = SIRF_MSG_GPRS_AT_COMMAND_PCCED_RESPONSE;
         if (NULL != message_structure)
         {
            tSIRF_MSG_GPRS_AT_COMMAND_PCCED_RESPONSE_STRING *msg = (tSIRF_MSG_GPRS_AT_COMMAND_PCCED_RESPONSE_STRING*)message_structure;
            memcpy(msg->string,string.string,sizeof(msg->string));
         }
         return SIRF_SUCCESS;
      }
   }

   /* Anything else interpret as a COMMAND_STRING */
   {

      *message_length = payload_length+1;
      *message_id = SIRF_MSG_GPRS_AT_COMMAND_STRING;
      if (NULL != message_structure)
      {
         tSIRF_MSG_GPRS_AT_COMMAND_STRING *msg = (tSIRF_MSG_GPRS_AT_COMMAND_STRING*)message_structure;
         memcpy(msg->string,payload,payload_length);
         msg->string[payload_length] = '\0';
      }

      return SIRF_SUCCESS;
   }
} /* SIRF_CODEC_GPRS_AT_COMMAND_Decode() */

/**
 * @}
 */
