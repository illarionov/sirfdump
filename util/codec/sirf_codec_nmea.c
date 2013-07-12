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
 * FILENAME:  sirf_codec_nmea.c
 *
 * DESCRIPTION: Routines to convert data to and from a byte stream defined
 *              by the NMEA protocol from and to the Tracker API
 *              defined in the Programmers Reference Manual
 *
 ***************************************************************************/

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <string.h>
#include <stdio.h>
#include <math.h>

#include "sirf_types.h"
#include "sirf_msg.h"
#include "sirf_msg_ssb.h"
#include "sirf_codec.h"
#include "sirf_codec_nmea.h"
#include "string_sif.h"

typedef struct
{
   float GDOP;  /**< Geometric  Dilution of Precision */
   float HDOP;  /**< Horizontal Dilution of Precision */
   float PDOP;  /**< Position   Dilution of Precision */
   float TDOP;  /**< Time       Dilution of Precision */
   float VDOP;  /**< Vertical   Dilution of Precision */
   unsigned char   maskExceed;
} MI_DOPS;

#define ABS(a)       (((a) < 0) ? (-(a)) : (a))
float const Power10[] ={ 1,1e1f,1e2f,1e3f,1e4f,1e5f,1e6f,1e7f,1e8f,1e9f,1e10f,1e11f,1e12f };


static tSIRF_RESULT SIRF_CODEC_NMEA_Encode_GGA(
   tSIRF_MSG_SSB_GEODETIC_NAVIGATION const*data,
   tSIRF_CHAR *buffer,
   tSIRF_UINT32 *buffer_size );
static tSIRF_RESULT SIRF_CODEC_NMEA_Encode_RMC(
   tSIRF_MSG_SSB_GEODETIC_NAVIGATION const*data,
   tSIRF_CHAR *buffer,
   tSIRF_UINT32 *buffer_size );
static tSIRF_RESULT SIRF_CODEC_NMEA_Encode_GLL(
   tSIRF_MSG_SSB_GEODETIC_NAVIGATION const*data,
   tSIRF_CHAR *buffer,
   tSIRF_UINT32 *buffer_size );
static tSIRF_RESULT SIRF_CODEC_NMEA_Encode_GSA(
   tSIRF_MSG_SSB_GEODETIC_NAVIGATION const*data,
   tSIRF_CHAR *buffer,
   tSIRF_UINT32 *buffer_size );
static tSIRF_RESULT SIRF_CODEC_NMEA_Encode_VTG(
   tSIRF_MSG_SSB_GEODETIC_NAVIGATION const*data,
   tSIRF_CHAR *buffer,
   tSIRF_UINT32 *buffer_size );
static tSIRF_RESULT SIRF_CODEC_NMEA_Encode_GSV(
   tSIRF_MSG_SSB_MEASURED_TRACKER    const*data,
   tSIRF_CHAR *buffer,
   tSIRF_UINT32 *buffer_size,
   tSIRF_UINT32 *options );



/***************************************************************************
 * @brief:   Convert a message structure to a byte stream
 *
 * For NMEA there are one to many relationships so the options flag is important.
 * @param[in]:     message id - GPRS_AT_COMMAND message id, from the GPRS_AT_COMMAND manual
 * @param[in]:     message_structure - data structure keyed to the message id
 * @param[in]:     message_length - used as string length if not a structure
 * @param[out]:    packet - buffer pointer for destination formatted output
 * @param[in out]: packet_length - size of same, computed after processing
 * @param[in out]: options - message to return, and other option on input
 *                           message # and total # of messages on output
 * @return:   SIRF_SUCCESS,
 *            SIRF_CODEC_GPRS_AT_COMMAND_LENGTH_ERROR,
 *            SIRF_CODEC_ERROR_INVALID_MSG_LENGTH,
 *            SIRF_CODEC_ERROR_INVALID_MSG_ID
 *            SIRF_CODEC_ERROR_INVALID_PARAMETER
***************************************************************************/
tSIRF_RESULT SIRF_CODEC_NMEA_Encode(
   tSIRF_UINT32               message_id,
   tSIRF_VOID   const * const message_structure,
   tSIRF_UINT32               message_length,
   tSIRF_UINT8        * const payload,
   tSIRF_UINT32       * const payload_length,
   tSIRF_UINT32       * const options )
{
   tSIRF_RESULT result;

   /* Check for NULL pointer and exit if detected. */
   if ( ((tSIRF_VOID *) NULL   == message_structure) ||
        ((tSIRF_UINT8 *) NULL  == payload) ||
        ((tSIRF_UINT32 *) NULL == payload_length) ||
        ((tSIRF_UINT32*)NULL   == options))
   {
      return SIRF_CODEC_ERROR_INVALID_PARAMETER;
   }

   switch ( message_id )
   {
   case SIRF_MSG_SSB_GEODETIC_NAVIGATION:
   {
      switch ( *options )
      {
      case SIRF_CODEC_NMEA_GGA:
         result = SIRF_CODEC_NMEA_Encode_GGA(
            (tSIRF_MSG_SSB_GEODETIC_NAVIGATION const*)message_structure,
            (tSIRF_CHAR*)payload,
            payload_length );
         if ( result == SIRF_SUCCESS )
         {
            /* Message 1 of 5*/
            *options = SIRF_CODEC_OPTIONS_MAKE_MSG_NUMBER(
               SIRF_CODEC_NMEA_GGA,
               SIRF_CODEC_NMEA_MAX_MSG_FROM_GEODETIC);
            return SIRF_SUCCESS;
         }
         /* else fall to the next case*/
      case SIRF_CODEC_NMEA_RMC:
         result = SIRF_CODEC_NMEA_Encode_RMC(
            (tSIRF_MSG_SSB_GEODETIC_NAVIGATION const*)message_structure,
            (tSIRF_CHAR*)payload,
            payload_length );
         if ( result == SIRF_SUCCESS )
         {
            /* Message 2 of 5*/
            *options = SIRF_CODEC_OPTIONS_MAKE_MSG_NUMBER(
               SIRF_CODEC_NMEA_RMC,
               SIRF_CODEC_NMEA_MAX_MSG_FROM_GEODETIC);
            return SIRF_SUCCESS;
         }
         /* else fall to the next case*/
      case SIRF_CODEC_NMEA_GLL:
         result = SIRF_CODEC_NMEA_Encode_GLL(
            (tSIRF_MSG_SSB_GEODETIC_NAVIGATION const*)message_structure,
            (tSIRF_CHAR*)payload,
            payload_length );
         if ( result == SIRF_SUCCESS )
         {
            /* Message 3 of 5*/
            *options = SIRF_CODEC_OPTIONS_MAKE_MSG_NUMBER(
               SIRF_CODEC_NMEA_GLL,
               SIRF_CODEC_NMEA_MAX_MSG_FROM_GEODETIC);
            return SIRF_SUCCESS;
         }
         /* else fall to the next case*/
      case SIRF_CODEC_NMEA_GSA:
         result = SIRF_CODEC_NMEA_Encode_GSA(
            (tSIRF_MSG_SSB_GEODETIC_NAVIGATION const*)message_structure,
            (tSIRF_CHAR*)payload,
            payload_length );
         if ( result == SIRF_SUCCESS )
         {
            /* Message 4 of 5*/
            *options = SIRF_CODEC_OPTIONS_MAKE_MSG_NUMBER(
               SIRF_CODEC_NMEA_GSA,
               SIRF_CODEC_NMEA_MAX_MSG_FROM_GEODETIC);
            return SIRF_SUCCESS;
         }
         /* else fall to the next case*/
      case SIRF_CODEC_NMEA_VTG:
         result = SIRF_CODEC_NMEA_Encode_VTG(
            (tSIRF_MSG_SSB_GEODETIC_NAVIGATION const*)message_structure,
            (tSIRF_CHAR*)payload,
            payload_length );
         if ( result == SIRF_SUCCESS )
         {
            /* Message 5 of 5*/
            *options = SIRF_CODEC_OPTIONS_MAKE_MSG_NUMBER(
               SIRF_CODEC_NMEA_VTG,
               SIRF_CODEC_NMEA_MAX_MSG_FROM_GEODETIC);
            return SIRF_SUCCESS;
         }
         /* else fall to the next case*/
      default:
         return SIRF_FAILURE;
      } /* switch( options )*/
   }

   case SIRF_MSG_SSB_MEASURED_TRACKER:
   {
      result = SIRF_CODEC_NMEA_Encode_GSV(
         (tSIRF_MSG_SSB_MEASURED_TRACKER const*)message_structure,
         (tSIRF_CHAR*)payload,
         payload_length,
         options );
      return result;
   }

   /*--------------------------------------------------------------------------
    * SIF Messages
    *------------------------------------------------------------------------*/
   case SIRF_MSG_SSB_EE_SEA_PROVIDE_EPH:
   {
      tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH const * msg = (tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH const*)message_structure;

      if ( message_length != sizeof(*msg) )
      {
         return SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
      }

      switch (*options)
      {
      case SIRF_CODEC_NMEA_PSRF108:
      {
         snprintf( (tSIRF_CHAR *)payload, sizeof(payload), "PSRF108,%d,%d,%d,%d,%d,%d,%d,%d",
                  (int)msg->extended_iono.alpha[0],
                  (int)msg->extended_iono.alpha[1],
                  (int)msg->extended_iono.alpha[2],
                  (int)msg->extended_iono.alpha[3],
                  (int)msg->extended_iono.beta[0],
                  (int)msg->extended_iono.beta[1],
                  (int)msg->extended_iono.beta[2],
                  (int)msg->extended_iono.beta[3]);
         *options = SIRF_CODEC_OPTIONS_MAKE_MSG_NUMBER(
            SIRF_CODEC_NMEA_PSRF108,
            SIRF_CODEC_NMEA_MAX_MSG_FROM_EE_SEA_PROVIDE_EPH);
         return SIRF_SUCCESS;
      }
      case SIRF_CODEC_NMEA_PSRF107_ONE_SAT:
      {
         snprintf( (tSIRF_CHAR *)payload, sizeof(payload), "PSRF107,%d,%d,%d,%lu,%lu,%d,%d,%d,%d,%d,%lu,"
                  "%d,%lu,%lu,%d,%d,%d,%d,%d,%d,%d,%d,%lu,%d,%lu,%d,%d,"
                  "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0",
                  (int)msg->week,(int)msg->extended_ephemeris[0].PRN,(int)msg->extended_ephemeris[0].ephemerisValidityFlag,(unsigned long)msg->extended_ephemeris[0].URA,
                  (unsigned long)msg->extended_ephemeris[0].URA,(int)msg->extended_ephemeris[0].age,(int)msg->extended_ephemeris[0].Crs, (int)msg->extended_ephemeris[0].deltaN,
                  (int)msg->extended_ephemeris[0].M0,  (int)msg->extended_ephemeris[0].Cuc, (unsigned long)msg->extended_ephemeris[0].eccen, (int)msg->extended_ephemeris[0].Cus,(unsigned long)msg->extended_ephemeris[0].sqrtA,
                  (unsigned long)msg->extended_ephemeris[0].toe, (int)msg->extended_ephemeris[0].Cic, (int)msg->extended_ephemeris[0].omega0, (int)msg->extended_ephemeris[0].Cis,(int)msg->extended_ephemeris[0].i0,
                  (int)msg->extended_ephemeris[0].Crc, (int)msg->extended_ephemeris[0].w, (int)msg->extended_ephemeris[0].omegaDot,(int)msg->extended_ephemeris[0].iDot, (unsigned long)msg->extended_ephemeris[0].toc,(int)msg->extended_ephemeris[0].Tgd,
                  (unsigned long)msg->extended_ephemeris[0].af2, (int)msg->extended_ephemeris[0].af1, (int)msg->extended_ephemeris[0].af0 );

         *options = SIRF_CODEC_OPTIONS_MAKE_MSG_NUMBER(
            SIRF_CODEC_NMEA_PSRF107_ONE_SAT,
            SIRF_CODEC_NMEA_MAX_MSG_FROM_EE_SEA_PROVIDE_EPH);
         return SIRF_SUCCESS;
      }
      case SIRF_CODEC_NMEA_PSRF107_TWO_SAT:
      {
         snprintf( (tSIRF_CHAR *)payload, sizeof(payload), "PSRF107,%d,%d,%d,%lu,%lu,%d,%d,%d,%d,%d,%lu,"
                  "%d,%lu,%lu,%d,%d,%d,%d,%d,%d,%d,%d,%lu,%d,%lu,%d,%d,"
                  "%d,%d,%lu,%lu,%d,%d,%d,%d,%d,%lu,"
                  "%d,%lu,%lu,%d,%d,%d,%d,%d,%d,%d,%d,%lu,%d,%lu,%d,%d",
                  (int)msg->week,(int)msg->extended_ephemeris[0].PRN,(int)msg->extended_ephemeris[0].ephemerisValidityFlag,(unsigned long)msg->extended_ephemeris[0].URA,
                  (unsigned long)msg->extended_ephemeris[0].IODE,(int)msg->extended_ephemeris[0].age,(int)msg->extended_ephemeris[0].Crs, (int)msg->extended_ephemeris[0].deltaN,
                  (int)msg->extended_ephemeris[0].M0,  (int)msg->extended_ephemeris[0].Cuc, (unsigned long)msg->extended_ephemeris[0].eccen, (int)msg->extended_ephemeris[0].Cus,(unsigned long)msg->extended_ephemeris[0].sqrtA,
                  (unsigned long)msg->extended_ephemeris[0].toe, (int)msg->extended_ephemeris[0].Cic, (int)msg->extended_ephemeris[0].omega0, (int)msg->extended_ephemeris[0].Cis,(int)msg->extended_ephemeris[0].i0,
                  (int)msg->extended_ephemeris[0].Crc, (int)msg->extended_ephemeris[0].w, (int)msg->extended_ephemeris[0].omegaDot,(int)msg->extended_ephemeris[0].iDot, (unsigned long)msg->extended_ephemeris[0].toc,(int)msg->extended_ephemeris[0].Tgd,
                  (unsigned long)msg->extended_ephemeris[0].af2, (int)msg->extended_ephemeris[0].af1, (int)msg->extended_ephemeris[0].af0,
                  (int)msg->extended_ephemeris[1].PRN,(int)msg->extended_ephemeris[1].ephemerisValidityFlag,(unsigned long)msg->extended_ephemeris[1].URA,
                  (unsigned long)msg->extended_ephemeris[1].URA,(int)msg->extended_ephemeris[1].age,(int)msg->extended_ephemeris[1].Crs, (int)msg->extended_ephemeris[1].deltaN,
                  (int)msg->extended_ephemeris[1].M0,  (int)msg->extended_ephemeris[1].Cuc, (unsigned long)msg->extended_ephemeris[1].eccen, (int)msg->extended_ephemeris[1].Cus,(unsigned long)msg->extended_ephemeris[1].sqrtA,
                  (unsigned long)msg->extended_ephemeris[1].toe, (int)msg->extended_ephemeris[1].Cic, (int)msg->extended_ephemeris[1].omega0, (int)msg->extended_ephemeris[1].Cis,(int)msg->extended_ephemeris[1].i0,
                  (int)msg->extended_ephemeris[1].Crc, (int)msg->extended_ephemeris[1].w, (int)msg->extended_ephemeris[1].omegaDot,(int)msg->extended_ephemeris[1].iDot, (unsigned long)msg->extended_ephemeris[1].toc,(int)msg->extended_ephemeris[1].Tgd,
                  (unsigned long)msg->extended_ephemeris[1].af2, (int)msg->extended_ephemeris[1].af1, (int)msg->extended_ephemeris[1].af0 );

         *options = SIRF_CODEC_OPTIONS_MAKE_MSG_NUMBER(
            SIRF_CODEC_NMEA_PSRF107_TWO_SAT,
            SIRF_CODEC_NMEA_MAX_MSG_FROM_EE_SEA_PROVIDE_EPH);
         return SIRF_SUCCESS;
      }
      default:
         return SIRF_CODEC_ERROR_INVALID_MSG_ID;
      }
   } /* case SIRF_MSG_SSB_EE_SEA_PROVIDE_EPH */

#ifdef CLM_STANDALONE
   case SIRF_MSG_SSB_EE_ACK:
   {
      tSEA_DEBUG_MSG * msg = (tSEA_DEBUG_MSG *)message_structure;
      snprintf( (tSIRF_CHAR *)payload, sizeof(payload), "PSRF110,0x%08lx",msg->debug_flags );
      return SIRF_SUCCESS;
   }
#endif

   case SIRF_MSG_SSB_SET_MSG_RATE:                 /* NEW NMEA MSG #####*/
   {
      if ( message_length != sizeof(tSIRF_MSG_SSB_SET_MSG_RATE) )
      {
         return SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
      }

      snprintf( (tSIRF_CHAR *)payload, sizeof(payload), "PSRF112,140,6,0" );
      /* PRINTF( "CLM : NMEA send out PSRF112,140,6,0\n" );*/
      /* OLD : sprintf( (tSIRF_CHAR *)payload, "PSRF112,%d,%d,%d",*/
      /* OLD :  (int)msg->msg_id, (int)msg->rate, (int)msg->mode);*/
      return SIRF_SUCCESS;
   }

   default:
      return SIRF_CODEC_ERROR_INVALID_MSG_ID;

   } /* switch( message_id )*/

} /* SIRF_CODEC_NMEA_Encode()*/

/**
 * Decode an NMEA input message.
 *
 * @param payload           [in] Input payload
 * @param payload_length    [in] Length of the payload
 * @param message_id        [out] Message id of the decoded message
 * @param message_structure [out] Data of the decoded message
 * @param message_length    [out] Size of the decoded message
 * @param options           [in/out] Message number to decode on input
 *                                   Message M of N on output.
 *
 * @return
 */
tSIRF_RESULT SIRF_CODEC_NMEA_Decode(   tSIRF_UINT8  const * const payload,
   tSIRF_UINT32               payload_length,
   tSIRF_UINT32       * const message_id,
   tSIRF_VOID         * const message_structure,
   tSIRF_UINT32       * const message_length,
   tSIRF_UINT32       * const options )
{
   unsigned long nmea_message_id;
   int ret_val;

   /* Check for NULL pointer and exit if detected. */
   if ( ((tSIRF_UINT8 *) NULL  == payload) ||
        ( 0                    == payload_length) ||
        ((tSIRF_UINT32 *) NULL == message_id) ||
        ((tSIRF_VOID *) NULL   == message_structure) ||
        ((tSIRF_UINT32 *) NULL == message_length) ||
        ((tSIRF_UINT32*)NULL   == options))
   {
      return SIRF_CODEC_ERROR_INVALID_PARAMETER;
   }
   ret_val = sscanf( (const tSIRF_CHAR*)payload,
                     "PSRF%03lu",
                     &nmea_message_id );
   if ( 1 != ret_val )
   {
      return SIRF_FAILURE;
   }

   *message_length = 0;

   switch ( nmea_message_id )
   {
   case SIRF_CODEC_NMEA_PSRF100_SET_SERIAL_PORT:
   {
      unsigned long data[5];

      ret_val = sscanf( (const tSIRF_CHAR *)payload,
                        "PSRF100,%lu,%lu,%lu,%lu,%lu",
                        &data[0], &data[1], &data[2], &data[3], &data[4] );

      if ( 5 != ret_val )
      {
         return SIRF_FAILURE;
      }

      if (SIRF_CODEC_NMEA_SSB_SET_PORT_VALUES == *options)
      {
         tSIRF_MSG_SSB_SET_PORT_VALUES *msg = (tSIRF_MSG_SSB_SET_PORT_VALUES*)message_structure;

         *message_id     = SIRF_MSG_SSB_SET_PORT_VALUES;
         *message_length = sizeof(tSIRF_MSG_SSB_SET_PORT_VALUES);

         msg->baud_rate  = (tSIRF_UINT32)data[1];
         msg->data_bits  = (tSIRF_UINT8)data[2];
         msg->stop_bits  = (tSIRF_UINT8)data[3];
         msg->parity     = (tSIRF_UINT8)data[4];

         /* Message 1 of 2*/
         *options = SIRF_CODEC_OPTIONS_MAKE_MSG_NUMBER(
            SIRF_CODEC_NMEA_SSB_SET_PORT_VALUES,
            SIRF_CODEC_NMEA_MAX_MSG_FROM_PSRF100);
      }
      else if (SIRF_CODEC_NMEA_SSB_SET_PROTOCOL == *options)
      {
         tSIRF_MSG_SSB_SET_PORT_PROTOCOL *msg = (tSIRF_MSG_SSB_SET_PORT_PROTOCOL*)message_structure;

         *message_id     = SIRF_MSG_SSB_SET_PORT_PROTOCOL;
         *message_length = sizeof(tSIRF_MSG_SSB_SET_PORT_PROTOCOL);

         msg->protocol = (tSIRF_UINT8)data[0];

         /* Message 2 of 2*/
         *options = SIRF_CODEC_OPTIONS_MAKE_MSG_NUMBER(
            SIRF_CODEC_NMEA_SSB_SET_PROTOCOL,
            SIRF_CODEC_NMEA_MAX_MSG_FROM_PSRF100);
      }
      else
      {
         return SIRF_FAILURE;
      }
   }
   break;

   case SIRF_CODEC_NMEA_PSRF101_NAVIGATION_INITIALIZATION:
   {
      long data[8];
      tSIRF_MSG_SSB_INITIALIZE *msg = (tSIRF_MSG_SSB_INITIALIZE*)message_structure;

      ret_val = sscanf( (const tSIRF_CHAR *)payload,
                        "PSRF101,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld",
                        &data[0],
                        &data[1],
                        &data[2],
                        &data[3],
                        &data[4],
                        &data[5],
                        &data[6],
                        &data[7] );

      if ( 8 != ret_val )
      {
         return SIRF_FAILURE;
      }

      *message_id     = SIRF_MSG_SSB_INITIALIZE;
      *message_length = sizeof(tSIRF_MSG_SSB_INITIALIZE);

      msg->ecef_x        = (tSIRF_INT32)data[0];
      msg->ecef_y        = (tSIRF_INT32)data[1];
      msg->ecef_z        = (tSIRF_INT32)data[2];
      msg->clk_offset    = (tSIRF_INT32)data[3];
      msg->gps_tow       = (tSIRF_UINT32)data[4];
      msg->gps_week      = (tSIRF_UINT16)data[5];
      msg->chnl_cnt      = (tSIRF_UINT8)data[6];
      msg->restart_flags = (tSIRF_UINT8)data[7];

      /* Message 1 of 1*/
      *options = SIRF_CODEC_OPTIONS_MAKE_MSG_NUMBER(1,1);
   }
   break;

   case NMEA_EE_REQUEST_EPHEMERIS:
   {
      tSIRF_MSG_SSB_EE_GPS_TIME_INFO *msg = (tSIRF_MSG_SSB_EE_GPS_TIME_INFO*)message_structure;
      long                            time_valid_flag = 0;
      long                            gps_week       = 0;
      unsigned long                   gps_tow = 0;
      unsigned long                   ephemeris_request_mask;

      *message_id = SIRF_MSG_SSB_EE_GPS_TIME_INFO;
      *message_length         = sizeof(*msg);

      ret_val = sscanf((const tSIRF_CHAR *)payload, "PSRF151,%ld,%ld,%lu,0x%08lX",
                       &time_valid_flag,
                       &gps_week,
                       &gps_tow,
                       &ephemeris_request_mask);
      if ( 4 == ret_val )
      {
         msg->time_valid_flag = (tSIRF_UINT8)time_valid_flag;
         msg->gps_week        = (tSIRF_UINT16)gps_week;
         msg->gps_tow         = (tSIRF_UINT32)gps_tow;
         msg->ephemeris_request_mask = (tSIRF_UINT32)ephemeris_request_mask;
      }
      else
      {
         return SIRF_FAILURE;
      }
   }
   break;

   case NMEA_EE_INTEGRITY_WARNING:
   {
      unsigned long sat_pos_validity_mask, sat_clk_validity_mask, sat_health_mask;
      tSIRF_MSG_SSB_EE_INTEGRITY *msg = (tSIRF_MSG_SSB_EE_INTEGRITY*)message_structure;
      *message_length             = sizeof(*msg);
      *message_id = SIRF_MSG_SSB_EE_INTEGRITY;

      ret_val = sscanf((const tSIRF_CHAR *)payload, "PSRF152,0x%08lX,0x%08lX,0x%08lX",
                       &sat_pos_validity_mask,
                       &sat_clk_validity_mask,
                       &sat_health_mask);
      if (3 == ret_val)
      {
          msg->sat_pos_validity_mask = (tSIRF_UINT32)sat_pos_validity_mask;
          msg->sat_clk_validity_mask = (tSIRF_UINT32)sat_clk_validity_mask;
          msg->sat_health_mask = (tSIRF_UINT32)sat_health_mask;
      }
      else
      {
         return SIRF_FAILURE;
      }
   }
   break;

   case NMEA_EE_CLK_BIAS_ADJUSTMENT:
   {
      unsigned int svid[3];
      unsigned short toe[3];
      long int clk_bias_adj[3];
      unsigned short sequence_number  = 0;
      unsigned index            = 0;
      unsigned i                = 0;

      tSIRF_MSG_SSB_EE_CLK_BIAS_ADJ *msg = (tSIRF_MSG_SSB_EE_CLK_BIAS_ADJ*)message_structure;
      *message_length               = 3 * sizeof(*msg);
      *message_id = SIRF_MSG_SSB_EE_CLK_BIAS_ADJ;

      ret_val = sscanf( (const tSIRF_CHAR *)payload, "PSRF155,4,%hu,%u,%hu,%ld,%u,%hu,%ld,%u,%hu,%ld",
                        &sequence_number,
                        &svid[0],&toe[0],&clk_bias_adj[0],
                        &svid[1],&toe[1],&clk_bias_adj[1],
                        &svid[2],&toe[2],&clk_bias_adj[2] );
      if( 10 == ret_val )
      {
         index = ( sequence_number - 1 )* 3 ;
         for ( i = index; i < index+3; i++ )
         {
            msg[i].svid         = (tSIRF_UINT8)svid[i - index];
            msg[i].toe          = (tSIRF_UINT16)toe[i-index];/* *(2e-4); */
            msg[i].clk_bias_adjust = (tSIRF_UINT32)clk_bias_adj[i-index];/**(2e+31);*/
         }
      }
      else
      {
         return SIRF_FAILURE;
      }
   }
   break;

   case NMEA_EE_MSG_ACK:
   {
      tSIRF_MSG_SSB_EE_ACK   *msg    = (tSIRF_MSG_SSB_EE_ACK *)message_structure;
      int                     nmeaid;
      *message_length      = sizeof(*msg);
      *message_id          = SIRF_MSG_SSB_EE_ACK;

      if ( sscanf((const tSIRF_CHAR *)payload, "PSRF154,%d", &nmeaid) != 1 )
      {
         return SIRF_FAILURE;
      }

      switch ( nmeaid )
      {
      case NMEA_EE_PROVIDE_IONO_PARAMS:
      case NMEA_EE_PROVIDE_EPHEMERIS:
      {
         msg->acq_id = SIRF_GET_MID(SIRF_MSG_SSB_EE_SEA_PROVIDE_EPH);
         msg->acq_sub_id = SIRF_GET_SUB_ID(SIRF_MSG_SSB_EE_SEA_PROVIDE_EPH); /* SIRFBinary ACK Sub-ID */
      }
      break;

#ifndef CLM_STANDALONE
      case NMEA_EE_DEBUG_MSG:
      {
         msg->acq_id = SIRF_GET_MID(SIRF_MSG_SSB_EE_DEBUG);
         msg->acq_sub_id = SIRF_GET_SUB_ID(SIRF_MSG_SSB_EE_DEBUG); /* SIRFBinary ACK Sub-ID */
      }
      break;
#endif

      default:
         return SIRF_FAILURE;
      }/* End of nested switch */
   }
   break;

   case NMEA_EE_SUBFRAME_NAV_BITS_OUTPUT:
   {
      int chnl;
      int svid;
      tSIRF_MSG_SSB_50BPS_DATA  * msg    = (tSIRF_MSG_SSB_50BPS_DATA *)message_structure;
      *message_length                   = sizeof(*msg);
      *message_id = SIRF_MSG_SSB_50BPS_DATA;

      if ( sscanf((const tSIRF_CHAR *)payload,
                  "PSRF140,%d,%d,%08x%08x%08x%08x%08x%08x%08x%08x%08x%08x",
                  &chnl, &svid, (unsigned int*)&msg->word[0], (unsigned int*)&msg->word[1], (unsigned int*)&msg->word[2],
                  (unsigned int*)&msg->word[3], (unsigned int*)&msg->word[4], (unsigned int*)&msg->word[5], (unsigned int*)&msg->word[6], (unsigned int*)&msg->word[7],
                  (unsigned int*)&msg->word[8],(unsigned int*)&msg->word[9]) != 12 )
      {
         return SIRF_FAILURE;
      }
      else
      {
         msg->chnl = (tSIRF_UINT8)chnl;
         msg->svid = (tSIRF_UINT8)svid;
      }
   }
   break;

   default:
      *message_id = SIRF_MSG_NMEA_PASS_THRU;
      return SIRF_SUCCESS;

   } /* switch */

   return SIRF_SUCCESS;

} /* SIRF_CODEC_NMEA_Decode()*/


/*------------------------------ P R I V A T E -------------------------------*/

/***************************************************************************
 * Description: Convert a floating point value to a string
 * Parameters:  input number, format: left.right, output string variable
 * Returns:     nothing
 ***************************************************************************/
static tSIRF_VOID Float2AsciiPad( float       FValue,
                                  tSIRF_INT16 left,
                                  tSIRF_INT16 right,
                                  tSIRF_CHAR *pOutString )
{
   if ((tSIRF_CHAR *) NULL != pOutString)
   {
      long IValue;
      long NValue;
      tSIRF_CHAR theString[20];
      tSIRF_CHAR *pEndString = &theString[19];
      tSIRF_CHAR *pDot = pEndString - right;
      tSIRF_CHAR *pLeftEnd = pDot - left;
      tSIRF_CHAR LsChar;

      IValue = (long) ( FValue * Power10[right + 1] );
      if ( IValue < 0 )
      {
         *pOutString++ = '-';
         IValue = -IValue;
         pLeftEnd++;
      }

      *pDot = '.';
      IValue = IValue / 10;  /* truncate instead of roundoff */

      for ( ; pEndString >= pLeftEnd; pEndString-- )
      {
         if( pEndString != pDot )
         {
            if( IValue )
            {
               NValue = IValue / 10;
               LsChar = (tSIRF_CHAR) (IValue - 10 * NValue);
               *pEndString = '0' + LsChar;
               IValue = NValue;
            }
            else
            {
               *pEndString = '0';
            }
         }
      }

      for ( pEndString++; pEndString < &theString[20]; )
      {
         *pOutString++ = *pEndString++;
      }
      *pOutString = 0;
   }
} /* Float2AsciiPad */


/******************************************************************************
 *
 * Description:
 *      The GGA-Global Positioning System Fix Data describes Time, position
 *      and fix related data for a GPS receiver.
 *
 ******************************************************************************/
static tSIRF_RESULT SIRF_CODEC_NMEA_Encode_GGA(
   tSIRF_MSG_SSB_GEODETIC_NAVIGATION const *data,
   tSIRF_CHAR *buffer,
   tSIRF_UINT32 *buffer_size )
{
   tSIRF_RESULT tRet = SIRF_FAILURE;

   if (( (tSIRF_MSG_SSB_GEODETIC_NAVIGATION *) NULL != data) &&
       (tSIRF_CHAR *) NULL != buffer)
   {
      tSIRF_DOUBLE latInDegree = 0;
      tSIRF_INT16 latDD = 0;
      tSIRF_DOUBLE latMMmm = 0;
      tSIRF_DOUBLE lonInDegree = 0;
      tSIRF_INT16 lonDDD = 0;
      tSIRF_DOUBLE lonMMmm = 0;
      unsigned NavMode = 0;
      tSIRF_CHAR latMMmmbuf[20] = "";
      tSIRF_CHAR lonMMmmbuf[20] = "";

      if ( (data->nav_mode & SIRF_MSG_SSB_MODE_MASK) == 0 )
      {
         NavMode = 0;
      }
      else if ( (data->nav_mode & SIRF_MSG_SSB_MODE_MASK) == 7 )
      {
         NavMode = 6;
      }
      else if ( data->nav_mode & SIRF_MSG_SSB_MODE_DGPS_USED )
      {
         NavMode = 2;
      }
      else
      {
         NavMode = 1;
      }

      latInDegree = data->lat * 1e-7;
      lonInDegree = data->lon * 1e-7;

      /* Convert Latitude to llll.ll format   -> degrees|minutes.decimal */
      latDD   = (tSIRF_INT16)latInDegree;
      latMMmm = fabs( (latInDegree - latDD) * 60.0 );

      /* Convert Longitude to yyyyy.yy format -> degrees|minutes.decimal */
      lonDDD  = (tSIRF_INT16)lonInDegree;
      lonMMmm = fabs( (lonInDegree - lonDDD) * 60.0 );

      /**
       * Note: function receives a msg #41 as input, which contains two height
       * formats ellipsoid height, and sea leval height.  The geoid separation
       * that is required for this message can be back-calculated as follows:
       *      geoid = (ellipse - msl)
       */

      Float2AsciiPad( (float) latMMmm, 2, 6, latMMmmbuf );
      Float2AsciiPad( (float) lonMMmm, 2, 6, lonMMmmbuf );

      if(NavMode)
      {
         *buffer_size = snprintf(
            buffer,
            *buffer_size,
            "GPGGA,%02d%02d%06.3f,%02d%s,%c,%03d%s,%c,%u,%02ld,%.1f,%.1f,M,%.1f,M,,",
            (int)data->utc_hour, (int)data->utc_min, data->utc_sec*1e-3,
            (int)ABS( latDD ),               /* tSIRF_INT32 deg   */
            latMMmmbuf,                      /* tSIRF_INT32 min   */
            latInDegree >= 0.0 ? 'N' : 'S',  /* direction */
            (int)ABS( lonDDD ),              /* tSIRF_INT32 deg   */
            lonMMmmbuf,                      /* tSIRF_INT32 min   */
            lonInDegree < 0.0 ? 'W' : 'E',   /* direction */
            NavMode,
            (unsigned long)data->sv_used_cnt,
            (double)data->hdop * SIRF_MSG_SSB_DOP_LSB,
            data->alt_msl*1e-2,
            (data->alt_ellips - data->alt_msl)*1e-2 );
      }
      else
      {
         *buffer_size = snprintf(
            buffer,
            *buffer_size,
            "GPGGA,%02d%02d%06.3f,,,,,,,%u,,,,,,,,",
            (int)data->utc_hour, (int)data->utc_min, data->utc_sec*1e-3,
            NavMode );
      }

      tRet = SIRF_SUCCESS;
   }

   return tRet;

} /* SIRF_CODEC_NMEA_Encode_GGA */


/**************************************************************************
*                                                                         *
* OutputRMC                                                               *
*                                                                         *
* Description:                                                            *
*      RMC-Recommended minimum specific GPS/Transit data                   *
*     and fix related data for a GPS receiver.                            *
*                                                                         *
****************************************************************************/
static tSIRF_RESULT SIRF_CODEC_NMEA_Encode_RMC(
   tSIRF_MSG_SSB_GEODETIC_NAVIGATION const *data,
   tSIRF_CHAR *buffer,
   tSIRF_UINT32 *buffer_size )
{
   tSIRF_DOUBLE latInDegree=0;
   tSIRF_INT16 latDD=0;
   tSIRF_DOUBLE latMMmm=0;

   tSIRF_DOUBLE lonInDegree=0;
   tSIRF_INT16 lonDDD=0;
   tSIRF_DOUBLE lonMMmm=0;

   tSIRF_CHAR   NavMode;
   tSIRF_CHAR   NavStatus;
   tSIRF_CHAR   latMMmmbuf[20]="";
   tSIRF_CHAR   lonMMmmbuf[20]="";

   tSIRF_DOUBLE speed_knots;

   if ( (data->nav_mode & SIRF_MSG_SSB_MODE_MASK) == 0 )
   {
      NavMode   = 'N';
      NavStatus = 'V';
   }
   else if ( (data->nav_mode & SIRF_MSG_SSB_MODE_MASK) == 7 )
   {
      NavMode   = 'E';
      NavStatus = 'A';
   }
   else if ( data->nav_mode & SIRF_MSG_SSB_MODE_DGPS_USED )
   {
      NavMode   = 'D';
      NavStatus = 'A';
   }
   else
   {
      NavMode   = 'A';
      NavStatus = 'A';
   }

   latInDegree = data->lat*1e-7;
   lonInDegree = data->lon*1e-7;


   /* Convert Latitude to llll.ll format   -> degrees|minutes.decimal */
   latDD   = (tSIRF_INT16)latInDegree;
   latMMmm = fabs( (latInDegree - latDD) * 60.0 );

   /* Convert Longitude to yyyyy.yy format -> degrees|minutes.decimal */
   lonDDD  = (tSIRF_INT16)lonInDegree;
   lonMMmm = fabs( (lonInDegree - lonDDD) * 60.0 );

   Float2AsciiPad( (float) latMMmm, 2, 6, latMMmmbuf );
   Float2AsciiPad( (float) lonMMmm, 2, 6, lonMMmmbuf );

   /* convert to knots */
   speed_knots = data->sog*1e-2 * 3600.0 / (0.3048 * 6076.0);

   Float2AsciiPad( (float) latMMmm, 2, 6, latMMmmbuf );
   Float2AsciiPad( (float) lonMMmm, 2, 6, lonMMmmbuf );

   if ( (data->nav_mode & SIRF_MSG_SSB_MODE_MASK) == 0 )
   {
      *buffer_size = snprintf(
         buffer,
         *buffer_size,
         "GPRMC,%02d%02d%06.3f,%c,,,,,,,,,%02d%02d%02d,,,%c",
         data->utc_hour, data->utc_min, data->utc_sec*1e-3,
         NavStatus,
         data->utc_day,
         data->utc_month,
         data->utc_year%100,
         NavMode );
   }
   else
   {
      *buffer_size = snprintf(
         buffer,
         *buffer_size,
         "GPRMC,%02d%02d%06.3f,%c,%02d%s,%c,%03d%s,%c,%.1f,%.1f,%02d%02d%02d,,,%c",
         data->utc_hour, data->utc_min, data->utc_sec*1e-3,
         NavStatus,
         ABS( latDD ),
         latMMmmbuf,
         latInDegree >= 0.0 ? 'N' : 'S',
         ABS( lonDDD ),
         lonMMmmbuf,
         lonInDegree < 0.0 ? 'W' : 'E',
         speed_knots,
         data->hdg*1e-2,
         data->utc_day,
         data->utc_month,
         data->utc_year%100,
         NavMode );
   }

   return SIRF_SUCCESS;

} /* SIRF_CODEC_NMEA_Encode_RMC() */


/******************************************************************************
 *
 *   OutputGLL
 *
 * Description:
 *     Geographic Position-Latidude/Longitude
 *
 ******************************************************************************/
static tSIRF_RESULT SIRF_CODEC_NMEA_Encode_GLL(
   tSIRF_MSG_SSB_GEODETIC_NAVIGATION const *data,
   tSIRF_CHAR *buffer,
   tSIRF_UINT32 *buffer_size )
{
   tSIRF_DOUBLE latInDegree=0;
   tSIRF_INT16 latDD=0;
   tSIRF_DOUBLE latMMmm=0;

   tSIRF_DOUBLE lonInDegree=0;
   tSIRF_INT16 lonDDD=0;
   tSIRF_DOUBLE lonMMmm=0;

   tSIRF_CHAR   NavMode;
   tSIRF_CHAR   NavStatus;

   tSIRF_CHAR   latMMmmbuf[20]="";
   tSIRF_CHAR   lonMMmmbuf[20]="";

   if ( (data->nav_mode & SIRF_MSG_SSB_MODE_MASK) == 0 )
   {
      NavMode   = 'N';
      NavStatus = 'V';
   }
   else if ( (data->nav_mode & SIRF_MSG_SSB_MODE_MASK) == 7 )
   {
      NavMode   = 'E';
      NavStatus = 'A';
   }
   else if ( data->nav_mode & SIRF_MSG_SSB_MODE_DGPS_USED )
   {
      NavMode   = 'D';
      NavStatus = 'A';
   }
   else
   {
      NavMode   = 'A';
      NavStatus = 'A';
   }

   latInDegree = data->lat*1e-7;
   lonInDegree = data->lon*1e-7;

   /* Convert Latitude to llll.ll format   -> degrees|minutes.decimal */
   latDD   = (tSIRF_INT16)latInDegree;
   latMMmm = fabs( (latInDegree - latDD) * 60.0 );

   /* Convert Longitude to yyyyy.yy format -> degrees|minutes.decimal */
   lonDDD  = (tSIRF_INT16)lonInDegree;
   lonMMmm = fabs( (lonInDegree - lonDDD) * 60.0 );

   Float2AsciiPad( (float) latMMmm, 2, 4, latMMmmbuf );
   Float2AsciiPad( (float) lonMMmm, 2, 4, lonMMmmbuf );

   *buffer_size = snprintf(
      buffer,
      *buffer_size,
      "GPGLL,%02d%s,%c,%03d%s,%c,%02d%02d%06.3f,%c,%c",
      ABS( latDD ),                    /* int deg*/
      latMMmmbuf,                      /* int min*/
      latInDegree >= 0.0 ? 'N' : 'S',
      ABS( lonDDD ),                   /* int deg*/
      lonMMmmbuf,                      /* int min*/
      lonInDegree  < 0.0 ? 'W' : 'E',
      data->utc_hour,
      data->utc_min,
      data->utc_sec*1e-3,
      NavStatus,
      NavMode );

   return SIRF_SUCCESS;

} /* SIRF_CODEC_NMEA_Encode_GLL */

/* extern signed int MI_GetNavDOPs (MI_DOPS *pData); */

/******************************************************************************
 *
 *   OutputGSA
 *
 * Description:
 *     GPS receiver operating mode, satellites used in the navigation
 *     solution reported by the $--GGA sentence, and DOP values.
 *
 ******************************************************************************/
static tSIRF_RESULT SIRF_CODEC_NMEA_Encode_GSA(
   tSIRF_MSG_SSB_GEODETIC_NAVIGATION const *data,
   tSIRF_CHAR *buffer,
   tSIRF_UINT32 *buffer_size )
{
   int i;
   unsigned NavMode;
   MI_DOPS dops;
   tSIRF_CHAR   str[128]="";

   switch ( data->nav_mode & SIRF_MSG_SSB_MODE_MASK )
   {
   case 4:
   case 6:
      NavMode = 3;  /* 3-d fix */
      break;
   case 0:
      NavMode = 1;  /* no nav */
      break;
   default:
      NavMode = 2;  /* 2-d fix */
      break;
   }

   /* put active chans svid */
   for ( i = 0; i < 32; i++ )
   {
      if (data->sv_used & (1<<i))
      {
         tSIRF_CHAR str1[128];
         snprintf(str1, sizeof(str1), "%02d,", i+1);
         strlcat(str, str1, sizeof(str));
      }
   } /* for */

   /* fill the other chan fields with ,,,, */
   for( i = 0; i < SIRF_CODEC_NMEA_MAX_CHANNELS - data->sv_used_cnt; i++ )
   {
      strlcat( str, ",", sizeof(str));
   }
/*
   if(MI_GetNavDOPs(&dops) == -1)
   { */
      memset(&dops, 0, sizeof(dops));
/*   } */

   if((data->nav_mode & SIRF_MSG_SSB_MODE_MASK) == 0)
   {
      /* format is ,pdop,hdop,vdop,   only have hdop :->  ,hdop,  */
      *buffer_size = snprintf( buffer, *buffer_size, "GPGSA,A,%1u,%s,,",
                               NavMode,
                               str );
   }
   else
   {
      /* format is ,pdop,hdop,vdop,   only have hdop :->  ,hdop,  */
      *buffer_size = snprintf( buffer, *buffer_size, "GPGSA,A,%1u,%s%.1f,%.1f,%.1f",
                               NavMode,
                               str,
                               (double)dops.PDOP,
                               (double)data->hdop * SIRF_MSG_SSB_DOP_LSB,
                               (double)dops.VDOP );
   }

   return SIRF_SUCCESS;

} /* SIRF_CODEC_NMEA_Encode_GSA */


/**************************************************************************
*                                                                         *
* OutputVTG                                                               *
*                                                                         *
* Description:                                                            *
*     The VTG - Course Over Ground and Ground Speed                       *
*               The actual course and speed relative to the ground.       *
*                                                                         *
****************************************************************************/
static tSIRF_RESULT SIRF_CODEC_NMEA_Encode_VTG(
   tSIRF_MSG_SSB_GEODETIC_NAVIGATION const *data,
   tSIRF_CHAR *buffer,
   tSIRF_UINT32 *buffer_size )
{
   tSIRF_DOUBLE speed_knots=0;
   tSIRF_DOUBLE speed_kmh=0;

   tSIRF_CHAR   NavMode;

   if ( (data->nav_mode & SIRF_MSG_SSB_MODE_MASK) == 0 )
      NavMode = 'N';
   else if ( (data->nav_mode & SIRF_MSG_SSB_MODE_MASK) == 7 )
      NavMode = 'E';
   else if ( data->nav_mode & SIRF_MSG_SSB_MODE_DGPS_USED )
      NavMode = 'D';
   else
      NavMode = 'A';
   /* convert to knots */
   speed_knots = data->sog*1e-2 * 3600.0 / (0.3048 * 6076.0);
   /* convert to kmh */
   speed_kmh = data->sog*1e-2 * 3600.0 / 1000.0;

   *buffer_size = snprintf(
      buffer,
      *buffer_size,
      "GPVTG,%.1f,T,,M,%.1f,N,%.1f,K,%c",
      data->hdg*1e-2,
      speed_knots,
      speed_kmh,
      NavMode);

   return SIRF_SUCCESS;

}  /* SIRF_CODEC_NMEA_Encode_VTG */


/*
***************************************************************************
*                                                                         *
* OutputGSV                                                               *
*                                                                         *
* Description:                                                            *
*                                                                         *
*     The GSV - GPS Satellites in view                                    *
*     Number of satellites in view, PRN numbers, elevation, azimuth,      *
*     and SNR value.  Four satellites max. per transmission, additional   *
*     satellite data sent in second or third message. Total # of messages *
*     being transmitted and the # of the message being transmitted is     *
*     indicated in the first two fields.                                  *
*                                                                         *
****************************************************************************/
static tSIRF_RESULT SIRF_CODEC_NMEA_Encode_GSV(
   tSIRF_MSG_SSB_MEASURED_TRACKER const *data,
   tSIRF_CHAR *buffer,
   tSIRF_UINT32 *buffer_size,
   tSIRF_UINT32 *options )
{
   unsigned nMsgCnt, svidcnt, msgno, chnlno, smplno, avgcnocnt;
   tSIRF_FLOAT  avgcno;
   tSIRF_CHAR   str[64];

   /* Find how many messages do we need to generate, since each message */
   /* can only contain up to 4 satellites information */
   svidcnt = 0;
   if(data->chnl_cnt > SIRF_NUM_CHANNELS)
   {
      return SIRF_FAILURE;
   }
   for ( chnlno = 0; chnlno < data->chnl_cnt; chnlno++ )
   {
      if(( data->chnl[chnlno].svid != 0 ) &&(data->chnl[chnlno].cno[0] !=0))
      {
         svidcnt++;
      }
   }

   msgno = *options;
   nMsgCnt = (svidcnt + 3) / 4;

   if ( 0 == nMsgCnt )
   {
         nMsgCnt = 1;
         *options = (SIRF_CODEC_NMEA_GSV << 16) | (msgno << 8) | nMsgCnt;
         *buffer_size = snprintf( buffer, *buffer_size, "GPGSV,%u,%u,,,,,,,,,,,,,",
                                  nMsgCnt,    /* total # of msgs */
                                  nMsgCnt     /* Msg #           */
                                  );
         return SIRF_SUCCESS;
   }
   else if (*options > nMsgCnt)
   {
      return SIRF_FAILURE;
   }

   *options = (SIRF_CODEC_NMEA_GSV << 16) | (msgno << 8) | nMsgCnt;

   snprintf( buffer, *buffer_size, "GPGSV,%u,%u,%02u",
             nMsgCnt,    /* total # of msgs */
             msgno,      /* Msg #           */
             svidcnt );

   svidcnt = 0;
   for ( chnlno = 0; chnlno < data->chnl_cnt; chnlno++ )
   {
       if (( data->chnl[chnlno].svid != 0 ) &&(data->chnl[chnlno].cno[0] !=0))
      {
         svidcnt++;

         if ( (svidcnt > (msgno * 4 - 4)) && (svidcnt <= (msgno * 4)) )
         {
            avgcno    = 0.0f;
            avgcnocnt = 0;
            for ( smplno=0; smplno<10; smplno++ )
            {
               if ( 0 < data->chnl[chnlno].cno[smplno] )
               {
                  avgcno += (tSIRF_FLOAT)data->chnl[chnlno].cno[smplno];
                  avgcnocnt++;
               }
            }

            if ( 0 < avgcnocnt )
            {
               avgcno /= (tSIRF_FLOAT)avgcnocnt;
            }

            snprintf(
               str,
               sizeof(str),
               ",%02d,%02hd,%03hd,%02hd",
               (data->chnl[chnlno].svid >= 120 ? data->chnl[chnlno].svid - 87 : data->chnl[chnlno].svid ),
               (short) (data->chnl[chnlno].elevation * 0.5f),
               (short) (data->chnl[chnlno].azimuth * 1.5f),
               (short) avgcno );
            strlcat( buffer, str, *buffer_size);
         }
      }
   } /* for()*/

   *buffer_size = (tSIRF_UINT32)strnlen( buffer, *buffer_size );

   return SIRF_SUCCESS;

} /* SIRF_CODEC_NMEA_Encode_GSV() */

/**
 * @}
 */

