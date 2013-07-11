/**
 * @addtogroup platform_src_sirf_util_codec
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2005 - 2011 by SiRF Technology, a CSR plc Company.
 *    All rights reserved.
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
 * @file   sirf_codec_ssb.c
 *
 * @brief  Routines to convert data to and from a byte stream
 *         defined by the Sirf Standard Binary API from and
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
#include "sirf_codec.h"
#include "sirf_codec_ssb.h"
#ifdef SIRF_AGPS
#include "sirf_codec_ssb_agps.h"
#endif
#ifdef PVT_BUILD
#include "sirf_cck_params.h"
#endif /* PVT_BUILD */

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
 * @param[in]:     message id - SSB message id, from the SSB manual
 * @param[in]:     message_structure - data structure keyed to the message id
 * @param[in]:     message_length - used as string length if not a structure
 * @param[out]:    packet - buffer pointer for destination formatted output
 * @param[in out]: packet_length - size of same, computed after processing
 * @return:   SIRF_SUCCESS, SIRF_CODEC_SSB_LENGTH_ERROR,
 *            SIRF_CODEC_ERROR_INVALID_MSG_LENGTH,
 *            SIRF_CODEC_SSB_NULL_POINTER or SIRF_CODEC_SSB_INVALID_MSG_ID
***************************************************************************/
tSIRF_RESULT SIRF_CODEC_SSB_Encode( tSIRF_UINT32 message_id,
                                    tSIRF_VOID *message_structure,
                                    tSIRF_UINT32 message_length,
                                    tSIRF_UINT8 *packet,
                                    tSIRF_UINT32 *packet_length,
                                    tSIRF_UINT32 *options )
{
   tSIRF_RESULT tRet = SIRF_SUCCESS;

   /* Check for NULL pointer and exit if detected. */
   if ( ((tSIRF_VOID *) NULL == message_structure) ||
        ((tSIRF_UINT8 *) NULL ==  packet) ||
        ((tSIRF_UINT32 *) NULL == packet_length))
   {
      return (SIRF_CODEC_SSB_NULL_POINTER);
   }

   /* Otherwise, process the message */
   {
      tSIRF_UINT32 i, j;
      tSIRF_INT32 k;
      tSIRF_UINT8 *ptr = packet;
      tSIRF_UINT8 header_len = 0;

      if (SIRF_GET_SUB_ID(message_id))
      {
         header_len++; /* start with sid byte count */
      }
#ifndef SIRF_LOC
      header_len += 1 * sizeof(tSIRF_UINT8); /* mid byte only */
#else
      header_len += 2 * sizeof(tSIRF_UINT8); /* mid + channel bytes */

      /* @todo It is possible for SIRF_LOC to be outputting onto Port B
       * and thus not wanting to input the logical channel  THIS IS BROKEN */
      /* insert the logical channel */
      SIRFBINARY_EXPORT8((tSIRF_UINT8)SIRF_LC_SSB,ptr);
#endif

      /* At a minimum, make sure we have room for the header bytes */
      if ( *packet_length < header_len )
      {
         return SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
      }

      /* insert the message id */
      SIRFBINARY_EXPORT8((tSIRF_UINT8)message_id, ptr);
      /* this is the second byte of the message id for those that offer one */
      if (SIRF_GET_SUB_ID(message_id))
      {
         SIRFBINARY_EXPORT8((tSIRF_UINT8)(SIRF_GET_SUB_ID(message_id)), ptr);
      }

      switch (message_id)
      {
      case SIRF_MSG_SSB_MEASURED_NAVIGATION: /* 0x02 */
      {
         tSIRF_MSG_SSB_MEASURED_NAVIGATION * msg = (tSIRF_MSG_SSB_MEASURED_NAVIGATION*) message_structure;

         /**
          * Attention programmers: the code compares the count of data bytes to *packet_length:
          * it generates the count by scanning the data store and adding up all of the
          * UINT8s then all of the UINT16s, then all of the UINT32s.
          */

         /* Check to make sure that the length of the buffer we are writing into is big enough.
            Include the header bytes.  */
         if ( *packet_length < ( (4 + SIRF_NUM_CHANNELS) * sizeof(tSIRF_UINT8) +
                                  4 * sizeof(tSIRF_UINT16) +
                                  4 * sizeof(tSIRF_UINT32) +
                                  header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->ecef_x,        ptr);
            SIRFBINARY_EXPORT32(msg->ecef_y,        ptr);
            SIRFBINARY_EXPORT32(msg->ecef_z,        ptr);
            SIRFBINARY_EXPORT16(msg->ecef_vel_x,    ptr);
            SIRFBINARY_EXPORT16(msg->ecef_vel_y,    ptr);
            SIRFBINARY_EXPORT16(msg->ecef_vel_z,    ptr);
            SIRFBINARY_EXPORT8( msg->nav_mode,      ptr);
            SIRFBINARY_EXPORT8( msg->hdop,          ptr);
            SIRFBINARY_EXPORT8( msg->nav_mode2,     ptr);
            SIRFBINARY_EXPORT16(msg->gps_week,      ptr);
            SIRFBINARY_EXPORT32(msg->gps_tow,       ptr);
            SIRFBINARY_EXPORT8( msg->sv_used_cnt,   ptr);
            for (i = 0; i < SIRF_NUM_CHANNELS; i++)
            {
               SIRFBINARY_EXPORT8(msg->sv_used[i],  ptr);
            }
         }
      }
      break;
      case SIRF_MSG_SSB_MEASURED_TRACKER: /* 0x04 */
      {
         tSIRF_MSG_SSB_MEASURED_TRACKER * msg = (tSIRF_MSG_SSB_MEASURED_TRACKER*) message_structure;

         if ( *packet_length < ( (1 + 3 * SIRF_NUM_CHANNELS) * sizeof(tSIRF_UINT8) +
                                 (SIRF_NUM_CHANNELS * SIRF_NUM_POINTS) * sizeof(tSIRF_UINT8) +
                                 (1 + SIRF_NUM_CHANNELS) * sizeof(tSIRF_UINT16) +
                                  1 * sizeof(tSIRF_UINT32) +
                                  header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->gps_week,      ptr);
            SIRFBINARY_EXPORT32(msg->gps_tow,       ptr);
            SIRFBINARY_EXPORT8( msg->chnl_cnt,      ptr);
            for (i = 0; i < SIRF_NUM_CHANNELS; i++)
            {
               SIRFBINARY_EXPORT8( msg->chnl[i].svid,      ptr);
               SIRFBINARY_EXPORT8( msg->chnl[i].azimuth,   ptr);
               SIRFBINARY_EXPORT8( msg->chnl[i].elevation, ptr);
               SIRFBINARY_EXPORT16(msg->chnl[i].state,     ptr);
               for (j = 0; j < SIRF_NUM_POINTS; j++)
               {
                  SIRFBINARY_EXPORT8(msg->chnl[i].cno[j],  ptr);
               }
            }
         }
      }
      break;
      case SIRF_MSG_SSB_RAW_TRACKER: /* 0x05 */
      {
         tSIRF_MSG_SSB_RAW_TRACKER * msg = (tSIRF_MSG_SSB_RAW_TRACKER*) message_structure;

         if ( *packet_length < ( (2 + SIRF_NUM_POINTS) * sizeof(tSIRF_UINT8) +
                                  7 * sizeof(tSIRF_UINT16) +
                                  6 * sizeof(tSIRF_UINT32) +
                                  header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->channel,              ptr);
            SIRFBINARY_EXPORT16(msg->svid,                 ptr);
            SIRFBINARY_EXPORT16(msg->state,                ptr);
            SIRFBINARY_EXPORT32(msg->bit_number,           ptr);
            SIRFBINARY_EXPORT16(msg->msec_number,          ptr);
            SIRFBINARY_EXPORT16(msg->chip_number,          ptr);
            SIRFBINARY_EXPORT32(msg->code_phase,           ptr);
            SIRFBINARY_EXPORT32(msg->carrier_doppler,      ptr);
            SIRFBINARY_EXPORT32(msg->measure_timetag,      ptr);
            SIRFBINARY_EXPORT32(msg->delta_carrier_phase,  ptr);
            SIRFBINARY_EXPORT16(msg->search_cnt,           ptr);
            for (i = 0; i < SIRF_NUM_POINTS; i++)
            {
               SIRFBINARY_EXPORT8 (msg->cno[i],  ptr);
            }
            SIRFBINARY_EXPORT8 (msg->power_bad_count,      ptr);
            SIRFBINARY_EXPORT8 (msg->phase_bad_count,      ptr);
            SIRFBINARY_EXPORT16(msg->delta_car_interval,   ptr);
            SIRFBINARY_EXPORT16(msg->correl_interval,      ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SW_VERSION:   /* 0x06 */
      {
         tSIRF_MSG_SSB_SW_VERSION * msg = (tSIRF_MSG_SSB_SW_VERSION *) message_structure;
            
         if ( *packet_length < ( (2 + msg->sirf_ver_bytes + msg->cust_ver_bytes) * sizeof(tSIRF_UINT8) + header_len))
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else if( (msg->sirf_ver_bytes > MAX_VERSION_LENGTH) ||
                     (msg->cust_ver_bytes > MAX_VERSION_LENGTH) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->sirf_ver_bytes, ptr);
            SIRFBINARY_EXPORT8(msg->cust_ver_bytes, ptr);
            for (i = 0; i < msg->sirf_ver_bytes; i++)
            {
               SIRFBINARY_EXPORT8(msg->sirf_ver_str[i], ptr);
            }
            for (i = 0; i < msg->cust_ver_bytes; i++)
            {
               SIRFBINARY_EXPORT8(msg->cust_ver_str[i], ptr);
            }
         }
      }
      break;
      case SIRF_MSG_SSB_CLOCK_STATUS: /* 0x07 */
      {
         tSIRF_MSG_SSB_CLOCK_STATUS * msg = (tSIRF_MSG_SSB_CLOCK_STATUS*) message_structure;

         if ( *packet_length < ( 1 * sizeof(tSIRF_UINT8) +
                                 1 * sizeof(tSIRF_UINT16) +
                                 4 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->gps_week,       ptr);
            SIRFBINARY_EXPORT32(msg->gps_tow,        ptr);
            SIRFBINARY_EXPORT8( msg->sv_used_cnt,    ptr);
            SIRFBINARY_EXPORT32(msg->clk_offset,     ptr);
            SIRFBINARY_EXPORT32(msg->clk_bias,       ptr);
            SIRFBINARY_EXPORT32(msg->est_gps_time,   ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_50BPS_DATA: /* 0x08 */
      case SIRF_MSG_SSB_EE_50BPS_DATA_VERIFIED: /* 0x38, 0x05 */
      {
         tSIRF_MSG_SSB_50BPS_DATA * msg = (tSIRF_MSG_SSB_50BPS_DATA*) message_structure;

         if ( *packet_length < ( 2 * sizeof(tSIRF_UINT8) +
                                 10 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->chnl,             ptr);
            SIRFBINARY_EXPORT8 (msg->svid,             ptr);
            for (i = 0; i < 10; i++)
            {
               SIRFBINARY_EXPORT32(msg->word[i],       ptr);
            }
         }
      }
      break;
      case SIRF_MSG_SSB_THROUGHPUT: /* 0x09 */
      {
         tSIRF_MSG_SSB_THROUGHPUT * msg = (tSIRF_MSG_SSB_THROUGHPUT*) message_structure;

         if ( *packet_length < ( 4 * sizeof(tSIRF_UINT16) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->seg_stat_max,  ptr);
            SIRFBINARY_EXPORT16(msg->seg_stat_lat,  ptr);
            SIRFBINARY_EXPORT16(msg->avg_trk_time,  ptr);
            SIRFBINARY_EXPORT16(msg->last_ms,       ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_ERROR: /* 0x0A */
      {
         tSIRF_MSG_SSB_ERROR * msg = (tSIRF_MSG_SSB_ERROR*) message_structure;

         if ( *packet_length < ( 2 * sizeof(tSIRF_UINT16) +
                                 SIRF_MSG_SSB_MAX_ERROR_PARAMS * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->err_id,         ptr);
            SIRFBINARY_EXPORT16(msg->param_cnt,      ptr);
            if (msg->param_cnt > SIRF_MSG_SSB_MAX_ERROR_PARAMS)
            {
               msg->param_cnt = SIRF_MSG_SSB_MAX_ERROR_PARAMS;
            }
            if (SIRF_MSG_SSB_ERRID_MI_WDOREXCEPTIONCONDITION == msg->err_id)
            {
               char *p = (char *)&msg->param[0];
               for (i = 0; i < (SIRF_MSG_SSB_MAX_ERROR_PARAMS*4); i++)
               {
                  SIRFBINARY_EXPORT8(*p++,    ptr);
               }
            }
            else
            {
               for (i = 0; i < SIRF_MSG_SSB_MAX_ERROR_PARAMS; i++)
               {
                  SIRFBINARY_EXPORT32(msg->param[i],    ptr);
               }
            }
         }
      }
      break;
      case SIRF_MSG_SSB_ACK: /* 0x0B */
      {
         tSIRF_MSG_SSB_ACK * msg = (tSIRF_MSG_SSB_ACK*) message_structure;

         if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->msg_id, ptr);
            SIRFBINARY_EXPORT8(msg->sub_id, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_NAK: /* 0x0C */
      {
         tSIRF_MSG_SSB_NAK * msg = (tSIRF_MSG_SSB_NAK *) message_structure;

         if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->msg_id, ptr);
            SIRFBINARY_EXPORT8(msg->sub_id, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_VISIBILITY_LIST: /* 0x0D */
      {
         tSIRF_MSG_SSB_VISIBILITY_LIST * msg = (tSIRF_MSG_SSB_VISIBILITY_LIST*) message_structure;

         if ( *packet_length < ( (1 + msg->svid_cnt) * sizeof(tSIRF_UINT8) +
                                 (2 * msg->svid_cnt) * sizeof(tSIRF_UINT16) +
                                  header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else if (msg->svid_cnt > SIRF_MAX_SVID_CNT)
         {
            tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->svid_cnt,                 ptr);
            for (i = 0; i < msg->svid_cnt; i++)
            {
               SIRFBINARY_EXPORT8 (msg->visible[i].svid,        ptr);
               SIRFBINARY_EXPORT16(msg->visible[i].azimuth,     ptr);
               SIRFBINARY_EXPORT16(msg->visible[i].elevation,   ptr);
            }
         }
      }
      break;
      case SIRF_MSG_SSB_ALMANAC: /* 0x0E */
      {
         tSIRF_MSG_SSB_ALMANAC * msg = (tSIRF_MSG_SSB_ALMANAC*) message_structure;

         if ( *packet_length < ( 1 * sizeof(tSIRF_UINT8) +
                                (2 + SIRF_MSG_SSB_ALMANAC_ENTRIES) * sizeof(tSIRF_UINT16) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->svid,            ptr);
            SIRFBINARY_EXPORT16(msg->week_and_status, ptr);
            for (i = 0; i < SIRF_MSG_SSB_ALMANAC_ENTRIES; i++)
            {
               SIRFBINARY_EXPORT16(msg->data[i], ptr);
            }
            SIRFBINARY_EXPORT16(msg->almanac_checksum,ptr);

         }
      }
      break;
      case SIRF_MSG_SSB_EPHEMERIS: /* 0x0F */
      {
         tSIRF_MSG_SSB_EPHEMERIS * msg = (tSIRF_MSG_SSB_EPHEMERIS*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT8) +
                                3 * 15 * sizeof(tSIRF_UINT16) +
                                header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->svid,                ptr);
            for (i = 0; i < 3; i++)
            {
               for (j = 0; j < 15; j++)
               {
                  SIRFBINARY_EXPORT16(msg->subframe[i][j], ptr);
               }
            }
         }
      }
      break;
      case SIRF_MSG_SSB_OK_TO_SEND: /* 0x12 */
      {
         tSIRF_MSG_SSB_OK_TO_SEND * msg = (tSIRF_MSG_SSB_OK_TO_SEND*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->input_enabled, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_RECEIVER_PARAMS: /* 0x13 */
      {
         tSIRF_MSG_SSB_RECEIVER_PARAMS * msg = (tSIRF_MSG_SSB_RECEIVER_PARAMS*) message_structure;

         if ( *packet_length < ( 20 * sizeof(tSIRF_UINT8) +
                                 4 * sizeof(tSIRF_UINT16) +
                                 9 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->reserved_1a,            ptr);
            SIRFBINARY_EXPORT16(msg->reserved_1b,            ptr);
            SIRFBINARY_EXPORT8 (msg->pos_mode_enable,        ptr);
            SIRFBINARY_EXPORT8 (msg->alt_mode,               ptr);
            SIRFBINARY_EXPORT8 (msg->alt_src,                ptr);
            SIRFBINARY_EXPORT16(msg->alt_input,              ptr);
            SIRFBINARY_EXPORT8 (msg->degraded_mode,          ptr);
            SIRFBINARY_EXPORT8 (msg->degraded_timeout,       ptr);
            SIRFBINARY_EXPORT8 (msg->dr_timeout,             ptr);
            SIRFBINARY_EXPORT8 (msg->trk_smooth,             ptr);
            SIRFBINARY_EXPORT8 (msg->static_nav_mode,        ptr);
            SIRFBINARY_EXPORT8 (msg->enable_3sv_lsq,         ptr);
            SIRFBINARY_EXPORT32(msg->reserved_2,             ptr);
            SIRFBINARY_EXPORT8 (msg->dop_mask_mode,          ptr);
            SIRFBINARY_EXPORT16(msg->nav_elev_mask,          ptr);
            SIRFBINARY_EXPORT8 (msg->nav_pwr_mask,           ptr);
            SIRFBINARY_EXPORT32(msg->reserved_3,             ptr);
            SIRFBINARY_EXPORT8 (msg->dgps_src,               ptr);
            SIRFBINARY_EXPORT8 (msg->dgps_mode,              ptr);
            SIRFBINARY_EXPORT8 (msg->dgps_timeout,           ptr);
            SIRFBINARY_EXPORT32(msg->reserved_4,             ptr);
            SIRFBINARY_EXPORT8 (msg->lp_push_to_fix,         ptr);
            SIRFBINARY_EXPORT32(msg->lp_on_time,             ptr);
            SIRFBINARY_EXPORT32(msg->lp_interval,            ptr);
            SIRFBINARY_EXPORT8 (msg->lp_user_tasks_enabled,  ptr);
            SIRFBINARY_EXPORT32(msg->lp_user_task_interval,  ptr);
            SIRFBINARY_EXPORT8 (msg->lp_pwr_cycling_enabled, ptr);
            SIRFBINARY_EXPORT32(msg->lp_max_acq_time,        ptr);
            SIRFBINARY_EXPORT32(msg->lp_max_off_time,        ptr);
            SIRFBINARY_EXPORT8 (msg->apm_enabled_power_duty_cycle, ptr);
            SIRFBINARY_EXPORT16(msg->number_of_fixes,        ptr);
            SIRFBINARY_EXPORT16(msg->time_between_fixes,      ptr);
            SIRFBINARY_EXPORT8 (msg->horz_vert_error_max,      ptr);
            SIRFBINARY_EXPORT8 (msg->response_time_max,        ptr);
            SIRFBINARY_EXPORT8 (msg->time_accu_time_duty_cycle_priority, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_TEST_MODE_DATA: /* 0x14 */
      {
         tSIRF_MSG_SSB_TEST_MODE_DATA * msg = (tSIRF_MSG_SSB_TEST_MODE_DATA*) message_structure;

         if ( *packet_length < ( 13 * sizeof(tSIRF_UINT16) +
                                 6 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->svid,               ptr);
            SIRFBINARY_EXPORT16(msg->period,             ptr);
            SIRFBINARY_EXPORT16(msg->bit_synch_time,     ptr);
            SIRFBINARY_EXPORT16(msg->bit_count,          ptr);
            SIRFBINARY_EXPORT16(msg->poor_status_count,  ptr);
            SIRFBINARY_EXPORT16(msg->good_status_count,  ptr);
            SIRFBINARY_EXPORT16(msg->parity_error_count, ptr);
            SIRFBINARY_EXPORT16(msg->lost_vco_count,     ptr);
            SIRFBINARY_EXPORT16(msg->frame_synch_time,   ptr);
            SIRFBINARY_EXPORT16(msg->cno_mean,           ptr);
            SIRFBINARY_EXPORT16(msg->cno_sigma,          ptr);
            SIRFBINARY_EXPORT16(msg->clock_drift,        ptr);
            SIRFBINARY_EXPORT32(msg->clock_offset_mean,  ptr);
            SIRFBINARY_EXPORT16(msg->bad_1khz_bit_count, ptr);
            SIRFBINARY_EXPORT32(msg->abs_i20ms,          ptr);
            SIRFBINARY_EXPORT32(msg->abs_q1ms,           ptr);
            SIRFBINARY_EXPORT32(msg->reserved[0],        ptr);
            SIRFBINARY_EXPORT32(msg->reserved[1],        ptr);
            SIRFBINARY_EXPORT32(msg->reserved[2],        ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DGPS_STATUS: /* 0x1B */
      {
         tSIRF_MSG_SSB_DGPS_STATUS * msg = (tSIRF_MSG_SSB_DGPS_STATUS*) message_structure;

         if ( *packet_length < ( (3 + 2 * SIRF_NUM_CHANNELS) * sizeof(tSIRF_UINT8) +
                                  SIRF_NUM_CHANNELS * sizeof(tSIRF_UINT16) +
                                  header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->src,                    ptr);
            for (i = 0; i < SIRF_NUM_CHANNELS; i++)
            {
               SIRFBINARY_EXPORT8 (msg->cor_age[i],          ptr);
            }
            SIRFBINARY_EXPORT8 (msg->reserved[0],            ptr);
            SIRFBINARY_EXPORT8 (msg->reserved[1],            ptr);
            for (i = 0; i < SIRF_NUM_CHANNELS; i++)
            {
               SIRFBINARY_EXPORT8 (msg->corrections[i].prn,  ptr);
               SIRFBINARY_EXPORT16(msg->corrections[i].cor,  ptr);
            }
         }
      }
      break;
      case SIRF_MSG_SSB_NL_MEAS_DATA: /* 0x1C */
      {
         tSIRF_MSG_SSB_NL_MEAS_DATA * msg = (tSIRF_MSG_SSB_NL_MEAS_DATA*) message_structure;

         if ( *packet_length < ( (5 + 10) * sizeof(tSIRF_UINT8) +
                                  4 * sizeof(tSIRF_UINT16) +
                                  2 * sizeof(tSIRF_UINT32) +
                                  3 * sizeof(tSIRF_DOUBLE) +
                                  header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->Chnl,                 ptr);
            SIRFBINARY_EXPORT32(msg->Timetag,              ptr);
            SIRFBINARY_EXPORT8 (msg->svid,                 ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->gps_sw_time,          ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->pseudorange,          ptr);
            SIRFBINARY_EXPORT32(msg->carrier_freq,         ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->carrier_phase,        ptr);
            SIRFBINARY_EXPORT16(msg->time_in_track,        ptr);
            SIRFBINARY_EXPORT8 (msg->sync_flags,           ptr);
            for (i = 0; i <10; i++)
            {
               SIRFBINARY_EXPORT8(msg->cton[i],            ptr);
            }
            SIRFBINARY_EXPORT16(msg->delta_range_interval, ptr);
            SIRFBINARY_EXPORT16(msg->mean_delta_range_time,ptr);
            SIRFBINARY_EXPORT16(msg->extrapolation_time,   ptr);
            SIRFBINARY_EXPORT8 (msg->phase_error_count,    ptr);
            SIRFBINARY_EXPORT8 (msg->low_power_count,      ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_NL_DGPS_DATA: /* 0x1D */
      {
         tSIRF_MSG_SSB_NL_DGPS_DATA * msg = (tSIRF_MSG_SSB_NL_DGPS_DATA*) message_structure;

         if ( *packet_length < ( 1 * sizeof(tSIRF_UINT8) +
                                 2 * sizeof(tSIRF_UINT16) +
                                 5 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->svid,           ptr);
            SIRFBINARY_EXPORT16(msg->iod,            ptr);
            SIRFBINARY_EXPORT8 (msg->source,         ptr);
            SIRFBINARY_EXPORT32(msg->pr_correction,  ptr);
            SIRFBINARY_EXPORT32(msg->prr_correction, ptr);
            SIRFBINARY_EXPORT32(msg->correction_age, ptr);
            SIRFBINARY_EXPORT32(msg->prc_variance,   ptr);
            SIRFBINARY_EXPORT32(msg->prrc_variance,  ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_NL_SV_STATE_DATA: /* 0x1E */
      {
         tSIRF_MSG_SSB_NL_SV_STATE_DATA * msg = (tSIRF_MSG_SSB_NL_SV_STATE_DATA*) message_structure;

         if ( *packet_length < ( 2 * sizeof(tSIRF_UINT8) +
                                 4 * sizeof(tSIRF_UINT32) +
                                (2 + 3 + 3) * sizeof(tSIRF_DOUBLE) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->svid,        ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->time,        ptr);
            for (i = 0; i < 3; i++)
            {
               SIRFBINARY_EXPORT_DOUBLE(msg->pos[i],   ptr);
            }
            for (i = 0; i < 3; i++)
            {
               SIRFBINARY_EXPORT_DOUBLE(msg->vel[i],   ptr);
            }
            SIRFBINARY_EXPORT_DOUBLE(msg->clk,         ptr);
            SIRFBINARY_EXPORT32(msg->clf,         ptr);
            SIRFBINARY_EXPORT8 (msg->eph,         ptr);
            SIRFBINARY_EXPORT32(msg->posvar,      ptr);
            SIRFBINARY_EXPORT32(msg->clkvar,      ptr);
            SIRFBINARY_EXPORT32(msg->iono,        ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_NL_INIT_DATA: /* 0x1F */
      {
         tSIRF_MSG_SSB_NL_INIT_DATA * msg = (tSIRF_MSG_SSB_NL_INIT_DATA*) message_structure;

         if ( *packet_length < ( 13 * sizeof(tSIRF_UINT8) +
                                 13 * sizeof(tSIRF_UINT16) +
                                 1 * sizeof(tSIRF_UINT32) +
                                 5 * sizeof(tSIRF_DOUBLE) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->enable_con_alt_mode,        ptr);
            SIRFBINARY_EXPORT8 (msg->alt_mode,                   ptr);
            SIRFBINARY_EXPORT8 (msg->alt_source,                 ptr);
            SIRFBINARY_EXPORT32(msg->altitude,                   ptr);
            SIRFBINARY_EXPORT8 (msg->degraded_mode,              ptr);
            SIRFBINARY_EXPORT16(msg->degraded_timeout,           ptr);
            SIRFBINARY_EXPORT16(msg->dr_timeout,                 ptr);
            SIRFBINARY_EXPORT16(msg->coast_timeout,              ptr);
            SIRFBINARY_EXPORT8 (msg->tracksmooth_mode,           ptr);
            SIRFBINARY_EXPORT8 (msg->dop_selection,              ptr);
            SIRFBINARY_EXPORT16(msg->hdop_thresh,                ptr);
            SIRFBINARY_EXPORT16(msg->gdop_thresh,                ptr);
            SIRFBINARY_EXPORT16(msg->pdop_thresh,                ptr);
            SIRFBINARY_EXPORT8 (msg->dgps_selection,             ptr);
            SIRFBINARY_EXPORT16(msg->dgps_timeout,               ptr);
            SIRFBINARY_EXPORT16(msg->elev_nav_mask,              ptr);
            SIRFBINARY_EXPORT16(msg->pow_nav_mask,               ptr);
            SIRFBINARY_EXPORT8 (msg->editing_residual_mode,      ptr);
            SIRFBINARY_EXPORT16(msg->editing_residual_threshold, ptr);
            SIRFBINARY_EXPORT8 (msg->ssd_mode,                   ptr);
            SIRFBINARY_EXPORT16(msg->ssd_threshold,              ptr);
            SIRFBINARY_EXPORT8 (msg->static_nav_mode,            ptr);
            SIRFBINARY_EXPORT16(msg->static_nav_threshold,       ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->ecef_x,                ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->ecef_y,                ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->ecef_z,                ptr);
            SIRFBINARY_EXPORT8 (msg->position_init_source,       ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->gps_time,              ptr);
            SIRFBINARY_EXPORT16(msg->gps_week,                   ptr);
            SIRFBINARY_EXPORT8 (msg->time_init_source,           ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->clk_offset,            ptr);
            SIRFBINARY_EXPORT8 (msg->clk_offset_init_source,     ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_NL_AUX_INIT_DATA:  /* 0x40 0x01 */
      {
         tSIRF_MSG_SSB_NL_AUX_INIT_DATA *msg = (tSIRF_MSG_SSB_NL_AUX_INIT_DATA *) message_structure;

         if ( *packet_length < ( 2 * sizeof(tSIRF_UINT8)  +
                                 4 * sizeof(tSIRF_UINT16) +
                                 6 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32 (msg->time_init_unc, ptr);
            SIRFBINARY_EXPORT16 (msg->saved_pos_week, ptr);
            SIRFBINARY_EXPORT32 (msg->saved_pos_tow, ptr);
            SIRFBINARY_EXPORT16 (msg->saved_pos_ehe, ptr);
            SIRFBINARY_EXPORT16 (msg->saved_pos_eve, ptr);
            SIRFBINARY_EXPORT8  (msg->sw_version, ptr);
            SIRFBINARY_EXPORT8  (msg->icd_version, ptr);
            SIRFBINARY_EXPORT16 (msg->chip_version, ptr);
            SIRFBINARY_EXPORT32 (msg->acq_clk_speed, ptr);
            SIRFBINARY_EXPORT32 (msg->default_clock_offset, ptr);
            SIRFBINARY_EXPORT32 (msg->tracker_status, ptr);
            SIRFBINARY_EXPORT32 (msg->reserved, ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_NL_AUX_MEAS_DATA:  /* 0x40 0x02 */
      {
         tSIRF_MSG_SSB_NL_AUX_MEAS_DATA *msg = (tSIRF_MSG_SSB_NL_AUX_MEAS_DATA *) message_structure;

         if ( *packet_length < (  5 * sizeof(tSIRF_UINT8)  +
                                  8 * sizeof(tSIRF_INT16)  +
                                  2 * sizeof(tSIRF_UINT16) +
                                  7 * sizeof(tSIRF_INT32)  +
                                  3 * sizeof(tSIRF_UINT32) +
                                  header_len) )

         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8  (msg->sv_prn,              ptr);
            SIRFBINARY_EXPORT8  (msg->status,              ptr);
            SIRFBINARY_EXPORT8  (msg->extended_status,     ptr);
            SIRFBINARY_EXPORT8  (msg->bit_sync_qual,       ptr);
            SIRFBINARY_EXPORT32 (msg->time_tag,            ptr);
            SIRFBINARY_EXPORT32 (msg->code_phase,          ptr);
            SIRFBINARY_EXPORT32 (msg->carrier_phase,       ptr);
            SIRFBINARY_EXPORT32 (msg->carrier_freq,        ptr);
            SIRFBINARY_EXPORT16 (msg->carrier_accel,       ptr);
            SIRFBINARY_EXPORT16 (msg->ms_num,              ptr);
            SIRFBINARY_EXPORT32 (msg->bit_num,             ptr);
            SIRFBINARY_EXPORT32 (msg->code_correction,     ptr);
            SIRFBINARY_EXPORT32 (msg->smooth_code,         ptr);
            SIRFBINARY_EXPORT32 (msg->code_offset,         ptr);
            SIRFBINARY_EXPORT16 (msg->pseudorange_noise,   ptr);
            SIRFBINARY_EXPORT16 (msg->delta_range_qual,    ptr);
            SIRFBINARY_EXPORT16 (msg->phase_lock_qual,     ptr);
            SIRFBINARY_EXPORT16 (msg->ms_uncertainty,      ptr);
            SIRFBINARY_EXPORT16 (msg->sum_abs_I,           ptr);
            SIRFBINARY_EXPORT16 (msg->sum_abs_Q,           ptr);
            SIRFBINARY_EXPORT32 (msg->sv_bit_num,          ptr);
            SIRFBINARY_EXPORT16 (msg->mp_los_det_value,    ptr);
            SIRFBINARY_EXPORT16 (msg->mp_only_det_value,   ptr);
            SIRFBINARY_EXPORT8  (msg->recovery_status,     ptr);
            SIRFBINARY_EXPORT32 (msg->sw_time_uncertainty, ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_NL_AUX_AID_DATA:  /* 0x40 0x03 */
      {
         tSIRF_MSG_SSB_NL_AUX_AID_DATA *msg = (tSIRF_MSG_SSB_NL_AUX_AID_DATA *) message_structure;

         if ( *packet_length < ( 3 * sizeof(tSIRF_INT32)  +
                                 2 * sizeof(tSIRF_UINT32) +
                                 1 * sizeof(tSIRF_UINT16) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->ecef_x,        ptr);
            SIRFBINARY_EXPORT32(msg->ecef_y,        ptr);
            SIRFBINARY_EXPORT32(msg->ecef_z,        ptr);
            SIRFBINARY_EXPORT32(msg->horiz_pos_unc, ptr);
            SIRFBINARY_EXPORT16(msg->alt_unc,       ptr);
            SIRFBINARY_EXPORT32(msg->sw_tow,        ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_MPM_STATUS_SVD: /* 0x4D 0x01 */
      {
         tSIRF_MSG_SSB_MPM_STATUS_SVD * msg = (tSIRF_MSG_SSB_MPM_STATUS_SVD*) message_structure;

         if ( *packet_length < ( sizeof(tSIRF_UINT32) +
                                 3 * sizeof(tSIRF_UINT16) +
                                 2 * sizeof(tSIRF_UINT8) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->timeinFPMode,    ptr);
            SIRFBINARY_EXPORT16(msg->rtcUncAtWakeUp,ptr);
            SIRFBINARY_EXPORT16(msg->totalRtcCorr,  ptr);
            SIRFBINARY_EXPORT32(msg->gpsTow,        ptr);
            SIRFBINARY_EXPORT8(msg->unusedTokens,   ptr);
            SIRFBINARY_EXPORT8(msg->trTemp,         ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_MPM_STATUS_EPH: /* 0x4D 0x02 */
      {
         tSIRF_MSG_SSB_MPM_STATUS_EPH * msg = (tSIRF_MSG_SSB_MPM_STATUS_EPH*) message_structure;

         if ( *packet_length < ( 3 *sizeof(tSIRF_UINT32) +
                                 3 * sizeof(tSIRF_UINT16) +
                                 2 * sizeof(tSIRF_UINT8) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->timeinFPMode,    ptr);
            SIRFBINARY_EXPORT32(msg->svBitMaskBefore, ptr);
            SIRFBINARY_EXPORT32(msg->svBitMaskAfter,  ptr);
            SIRFBINARY_EXPORT8(msg->navSuccess,       ptr);
            SIRFBINARY_EXPORT16(msg->rtcUncAtWakeUp,  ptr);
            SIRFBINARY_EXPORT16(msg->totalRtcCorr,    ptr);
            SIRFBINARY_EXPORT32(msg->gpsTow,          ptr);
            SIRFBINARY_EXPORT8(msg->unusedTokens,     ptr);
            SIRFBINARY_EXPORT8(msg->trTemp,           ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_MPM_STATUS_ALM: /* 0x4D 0x03 */
      {
         tSIRF_MSG_SSB_MPM_STATUS_ALM * msg = (tSIRF_MSG_SSB_MPM_STATUS_ALM*) message_structure;

         if ( *packet_length < ( sizeof(tSIRF_UINT32) +
                                 3 * sizeof(tSIRF_UINT16) +
                                 4 * sizeof(tSIRF_UINT8) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->timeinFPMode,      ptr);
            SIRFBINARY_EXPORT8(msg->idAlmCollected,     ptr);
            SIRFBINARY_EXPORT8(msg->collectionSuccess,  ptr);
            SIRFBINARY_EXPORT8(msg->navSuccess,         ptr);
            SIRFBINARY_EXPORT16(msg->rtcUncAtWakeUp,    ptr);
            SIRFBINARY_EXPORT16(msg->totalRtcCorr,      ptr);
            SIRFBINARY_EXPORT32(msg->gpsTow,            ptr);
            SIRFBINARY_EXPORT8(msg->unusedTokens,       ptr);
            SIRFBINARY_EXPORT8(msg->trTemp,             ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_MPM_STATUS_UPD: /* 0x4D 0x04 */
      {
         tSIRF_MSG_SSB_MPM_STATUS_UPD * msg = (tSIRF_MSG_SSB_MPM_STATUS_UPD*) message_structure;

         if ( *packet_length < ( sizeof(tSIRF_UINT32) +
                                 4 * sizeof(tSIRF_INT32) +
                                 2 * sizeof(tSIRF_UINT16) +
                                 sizeof(tSIRF_INT16) +
                                 6 * sizeof(tSIRF_UINT8) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->rtcUncAtWakeUp,    ptr);
            SIRFBINARY_EXPORT16(msg->totalRtcCorr,      ptr);
            SIRFBINARY_EXPORT32(msg->gpsTow,            ptr);
            SIRFBINARY_EXPORT8(msg->unusedTokens,       ptr);
            SIRFBINARY_EXPORT8(msg->trTemp,             ptr);
            SIRFBINARY_EXPORT8(msg->uNavStatus,         ptr);
            SIRFBINARY_EXPORT8(msg->numBESVs,           ptr);
            SIRFBINARY_EXPORT8(msg->numEESVs,           ptr);
            SIRFBINARY_EXPORT8(msg->numAlmSVs,          ptr);
            SIRFBINARY_EXPORT16(msg->uNavTimeCorr,      ptr);
            SIRFBINARY_EXPORT32(msg->meanCodeRes,       ptr);
            SIRFBINARY_EXPORT32(msg->stdPR,             ptr);
            SIRFBINARY_EXPORT32(msg->meanDoppRes,       ptr);
            SIRFBINARY_EXPORT32(msg->stdDR,             ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_MPM_STATUS_REC: /* 0x4D 0x05 */
      {
         tSIRF_MSG_SSB_MPM_STATUS_REC * msg = (tSIRF_MSG_SSB_MPM_STATUS_REC*) message_structure;

         if ( *packet_length < ( 2 *sizeof(tSIRF_INT32) +
                                 3 * sizeof(tSIRF_UINT16) +
                                 4 * sizeof(tSIRF_UINT8) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->timeinFPMode,   ptr);
            SIRFBINARY_EXPORT8(msg->bitSyncDone,     ptr);
            SIRFBINARY_EXPORT8(msg->frameSyncDone,   ptr);
            SIRFBINARY_EXPORT32(msg->totalTimeCorr,  ptr);
            SIRFBINARY_EXPORT16(msg->rtcUncAtWakeUp, ptr);
            SIRFBINARY_EXPORT16(msg->totalRtcCorr,   ptr);
            SIRFBINARY_EXPORT32(msg->gpsTow,         ptr);
            SIRFBINARY_EXPORT8(msg->unusedTokens,    ptr);
            SIRFBINARY_EXPORT8(msg->trTemp,          ptr);
         }
      }
      break;

      case  SIRF_MSG_SSB_PWR_MODE_FPM_RSP:   /* 0x5A, 0x0 */
      /* SIRF_MSG_SSB_PWR_MODE_APM_RSP 0x5A, 0x1 is encoded in SIRF_CODEC_SSB_AGPS_Encode() */
      case  SIRF_MSG_SSB_PWR_MODE_TP_RSP:    /* 0x5A, 0x3 */
      case  SIRF_MSG_SSB_PWR_MODE_PTF_RSP:   /* 0x5A, 0x4 */
      case  SIRF_MSG_SSB_PWR_MODE_ERR_RSP:   /* 0x5A, 0x5 */
      {
         const tSIRF_MSG_SSB_PWR_MODE_RSP * msg = (const tSIRF_MSG_SSB_PWR_MODE_RSP*) message_structure;

         if ( (sizeof(*msg) != message_length) || 
            (*packet_length < ( 8 * sizeof(tSIRF_UINT8) + header_len)))
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }
         SIRFBINARY_EXPORT8(msg->reserved,ptr);
      }
      break;
      
      case SIRF_MSG_SSB_CW_DATA: /* 0x5C 0x1 */
      {
         tSIRF_MSG_SSB_CW_DATA * msg = (tSIRF_MSG_SSB_CW_DATA*) message_structure;

         if ( *packet_length < ( (SIRF_CW_PEAKS * sizeof(tSIRF_INT32)) +
                                 (SIRF_CW_PEAKS * sizeof(tSIRF_UINT16)) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            for (i=0; i < SIRF_CW_PEAKS; i++)
            {
               SIRFBINARY_EXPORT32 (msg->freq[i], ptr);
            }

            for (i=0; i < SIRF_CW_PEAKS; i++)
            {
               SIRFBINARY_EXPORT16 (msg->cno[i], ptr);
            }
         }
      }
      break;

      case SIRF_MSG_SSB_CW_FILTER: /* 0x5C 0x2 */
      {
         tSIRF_MSG_SSB_CW_FILTER * msg = (tSIRF_MSG_SSB_CW_FILTER*) message_structure;

         if ( *packet_length < ( (2 * sizeof(tSIRF_UINT8)) +
                                 (SIRF_CW_PEAKS * sizeof(tSIRF_UINT8)) +
                                 (SIRF_CW_PEAKS * sizeof(tSIRF_INT8)) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->sampling, ptr);
            SIRFBINARY_EXPORT8 (msg->adc, ptr);
            for (i=0; i < SIRF_CW_PEAKS; i++)
            {
               SIRFBINARY_EXPORT8 (msg->freqbin[i], ptr);
               SIRFBINARY_EXPORT8 (msg->nbin[i], ptr);
            }
         }
      }
      break;

      case SIRF_MSG_SSB_GEODETIC_NAVIGATION: /* 0x29 */
      {
         tSIRF_MSG_SSB_GEODETIC_NAVIGATION * msg = (tSIRF_MSG_SSB_GEODETIC_NAVIGATION*) message_structure;

         if ( *packet_length < ( 8 * sizeof(tSIRF_UINT8) +
                                 13 * sizeof(tSIRF_UINT16) +
                                 14 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->nav_valid,                ptr);
            SIRFBINARY_EXPORT16(msg->nav_mode,                 ptr);
            SIRFBINARY_EXPORT16(msg->gps_week,                 ptr);
            SIRFBINARY_EXPORT32(msg->gps_tow,                  ptr);
            SIRFBINARY_EXPORT16(msg->utc_year,                 ptr);
            SIRFBINARY_EXPORT8 (msg->utc_month,                ptr);
            SIRFBINARY_EXPORT8 (msg->utc_day,                  ptr);
            SIRFBINARY_EXPORT8 (msg->utc_hour,                 ptr);
            SIRFBINARY_EXPORT8 (msg->utc_min,                  ptr);
            SIRFBINARY_EXPORT16(msg->utc_sec,                  ptr);
            SIRFBINARY_EXPORT32(msg->sv_used,                  ptr);
            SIRFBINARY_EXPORT32(msg->lat,                      ptr);
            SIRFBINARY_EXPORT32(msg->lon,                      ptr);
            SIRFBINARY_EXPORT32(msg->alt_ellips,               ptr);
            SIRFBINARY_EXPORT32(msg->alt_msl,                  ptr);
            SIRFBINARY_EXPORT8 (msg->datum,                    ptr);
            SIRFBINARY_EXPORT16(msg->sog,                      ptr);
            SIRFBINARY_EXPORT16(msg->hdg,                      ptr);
            SIRFBINARY_EXPORT16(msg->mag_var,                  ptr);
            SIRFBINARY_EXPORT16(msg->climb_rate,               ptr);
            SIRFBINARY_EXPORT16(msg->heading_rate,             ptr);
            SIRFBINARY_EXPORT32(msg->ehpe,                     ptr);
            SIRFBINARY_EXPORT32(msg->evpe,                     ptr);
            SIRFBINARY_EXPORT32(msg->ete,                      ptr);
            SIRFBINARY_EXPORT16(msg->ehve,                     ptr);
            SIRFBINARY_EXPORT32(msg->clk_bias,                 ptr);
            SIRFBINARY_EXPORT32(msg->clk_bias_error,           ptr);
            SIRFBINARY_EXPORT32(msg->clk_offset,               ptr);
            SIRFBINARY_EXPORT32(msg->clk_offset_error,         ptr);
            SIRFBINARY_EXPORT32(msg->distance_travelled,       ptr);
            SIRFBINARY_EXPORT16(msg->distance_travelled_error, ptr);
            SIRFBINARY_EXPORT16(msg->heading_error,            ptr);
            SIRFBINARY_EXPORT8 (msg->sv_used_cnt,              ptr);
            SIRFBINARY_EXPORT8 (msg->hdop,                     ptr);
            SIRFBINARY_EXPORT8 (msg->additional_mode_info,     ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_ADC_ODOMETER_DATA: /* 0x2D */
      {
         /* programmers note: the "_1HZ" version is just ten of the non- "_1HZ" version,
          * so does it make sense to copy what's there, progressively, or does it
          * make sense to reject the message if it is not precisely the right size? */
         if (sizeof(tSIRF_MSG_SSB_ADC_ODOMETER_DATA) == message_length)
         {
            tSIRF_MSG_SSB_ADC_ODOMETER_DATA * msg = (tSIRF_MSG_SSB_ADC_ODOMETER_DATA*) message_structure;

            if ( *packet_length < ( 1 * sizeof(tSIRF_UINT8) +
                                    3 * sizeof(tSIRF_UINT16) +
                                    1 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT32(msg->current_time,   ptr);
               SIRFBINARY_EXPORT16(msg->adc2_avg,       ptr);
               SIRFBINARY_EXPORT16(msg->adc3_avg,       ptr);
               SIRFBINARY_EXPORT16(msg->odo_count,      ptr);
               SIRFBINARY_EXPORT8 (msg->gpio_stat,      ptr);
            }
         }
         else if (sizeof(tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ) == message_length)
         {
            tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ * msg = (tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ*) message_structure;

            if ( *packet_length < ( 1 * 10 * sizeof(tSIRF_UINT8) +
                                    3 * 10 * sizeof(tSIRF_UINT16) +
                                    1 * 10 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               for (i = 0; i < 10; i++)
               {
                  SIRFBINARY_EXPORT32(msg->dataset[i].current_time,   ptr);
                  SIRFBINARY_EXPORT16(msg->dataset[i].adc2_avg,       ptr);
                  SIRFBINARY_EXPORT16(msg->dataset[i].adc3_avg,       ptr);
                  SIRFBINARY_EXPORT16(msg->dataset[i].odo_count,      ptr);
                  SIRFBINARY_EXPORT8 (msg->dataset[i].gpio_stat,      ptr);
               }
            }
         }
         else
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
      }
      break;
      case SIRF_MSG_SSB_TEST_MODE_DATA_3: /* 0x2E */
      {
         tSIRF_MSG_SSB_TEST_MODE_DATA_3 *msg = (tSIRF_MSG_SSB_TEST_MODE_DATA_3 *)message_structure;

         if ( *packet_length < ( 2 * sizeof(tSIRF_UINT8) +
                                 16 * sizeof(tSIRF_UINT16) +
                                 4 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->svid,              ptr);
            SIRFBINARY_EXPORT16(msg->period,            ptr);
            SIRFBINARY_EXPORT16(msg->bit_synch_time,    ptr);
            SIRFBINARY_EXPORT16(msg->bit_count,         ptr);
            SIRFBINARY_EXPORT16(msg->poor_status_count, ptr);
            SIRFBINARY_EXPORT16(msg->good_status_count, ptr);
            SIRFBINARY_EXPORT16(msg->parity_error_count,ptr);
            SIRFBINARY_EXPORT16(msg->lost_vco_count,    ptr);
            SIRFBINARY_EXPORT16(msg->frame_synch_time,  ptr);
            SIRFBINARY_EXPORT16(msg->cno_mean,          ptr);
            SIRFBINARY_EXPORT16(msg->cno_sigma,         ptr);
            SIRFBINARY_EXPORT16(msg->clock_drift,       ptr);
            SIRFBINARY_EXPORT32(msg->clock_offset,      ptr);
            SIRFBINARY_EXPORT16(msg->bad_1khz_bit_count,ptr);
            SIRFBINARY_EXPORT32(msg->abs_i20ms,         ptr);
            SIRFBINARY_EXPORT32(msg->abs_q20ms,         ptr);
            SIRFBINARY_EXPORT32(msg->phase_lock,        ptr);
            SIRFBINARY_EXPORT16(msg->rtc_frequency,     ptr);
            SIRFBINARY_EXPORT16(msg->e_to_acq_ratio,    ptr);
            SIRFBINARY_EXPORT8(msg->t_sync_agc_gain,    ptr);
            SIRFBINARY_EXPORT8(msg->tm_5_ready,         ptr);
            SIRFBINARY_EXPORT16(msg->ClkDriftUnc,       ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_NAV_STATUS: /* 0x01 0x30 */
      {
         tSIRF_MSG_SSB_DR_NAV_STATUS * msg = (tSIRF_MSG_SSB_DR_NAV_STATUS*) message_structure;

         if ( *packet_length < ( 14 * sizeof(tSIRF_UINT8) +
                                  2 * sizeof(tSIRF_UINT16) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->nav,ptr);
            SIRFBINARY_EXPORT16(msg->data,ptr);
            SIRFBINARY_EXPORT8(msg->cal_gb_cal,ptr);
            SIRFBINARY_EXPORT8(msg->gsf_cal_ssf_cal,ptr);
            SIRFBINARY_EXPORT8(msg->nav_across_reset_pos,ptr);
            SIRFBINARY_EXPORT8(msg->hd,ptr);
            SIRFBINARY_EXPORT8(msg->gyr_sub_op_spd_sub_op,ptr);
            SIRFBINARY_EXPORT8(msg->nav_st_int_ran_z_gb_cal_upd,ptr);
            SIRFBINARY_EXPORT8(msg->gbsf_cal_upd_spd_cal_upd_upd_nav_st,ptr);
            SIRFBINARY_EXPORT8(msg->gps_upd_pos,ptr);
            SIRFBINARY_EXPORT8(msg->gps_upd_hd,ptr);
            SIRFBINARY_EXPORT8(msg->gps_pos_gps_vel,ptr);

            /* Where did these fields come from? */
            SIRFBINARY_EXPORT8(msg->dws_hd_rt_s_f_cal_valid,ptr);
            SIRFBINARY_EXPORT8(msg->dws_hd_rt_s_f_cal_upd,ptr);
            SIRFBINARY_EXPORT16(msg->dws_spd_s_f_cal_valid,ptr);
            SIRFBINARY_EXPORT8(msg->dws_spd_s_f_cal_upd,ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_NAV_STATE: /* 0x02 0x30 */
      {
         tSIRF_MSG_SSB_DR_NAV_STATE * msg = (tSIRF_MSG_SSB_DR_NAV_STATE*) message_structure;

         if ( *packet_length < ( 3 * sizeof(tSIRF_UINT8) +
                                 33 * sizeof(tSIRF_UINT16) +
                                 1 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->spd,ptr);
            SIRFBINARY_EXPORT16(msg->spd_e,ptr);
            SIRFBINARY_EXPORT16(msg->ssf,ptr);
            SIRFBINARY_EXPORT16(msg->ssf_e,ptr);
            SIRFBINARY_EXPORT16(msg->hd_rte,ptr);
            SIRFBINARY_EXPORT16(msg->hd_rte_e,ptr);
            SIRFBINARY_EXPORT16(msg->gb,ptr);
            SIRFBINARY_EXPORT16(msg->gbE,ptr);
            SIRFBINARY_EXPORT16(msg->gsf,ptr);
            SIRFBINARY_EXPORT16(msg->gsf_e,ptr);
            SIRFBINARY_EXPORT32(msg->tpe,ptr);
            SIRFBINARY_EXPORT16(msg->the,ptr);
            SIRFBINARY_EXPORT8(msg->nav_ctrl,ptr);
            SIRFBINARY_EXPORT8(msg->reverse,ptr);
            SIRFBINARY_EXPORT16(msg->hd,ptr);
            SIRFBINARY_EXPORT8(msg->sensor_pkg,ptr);
            SIRFBINARY_EXPORT16(msg->odo_spd,ptr);
            SIRFBINARY_EXPORT16(msg->odo_spd_s_f,ptr);
            SIRFBINARY_EXPORT16(msg->odo_spd_s_f_err,ptr);
            SIRFBINARY_EXPORT16(msg->lf_wheel_spd_sf,ptr);
            SIRFBINARY_EXPORT16(msg->lf_wheel_spd_sf_err,ptr);
            SIRFBINARY_EXPORT16(msg->rf_wheel_spd_sf,ptr);
            SIRFBINARY_EXPORT16(msg->rf_wheel_spd_sf_err,ptr);
            SIRFBINARY_EXPORT16(msg->lr_wheel_spd_sf,ptr);
            SIRFBINARY_EXPORT16(msg->lr_wheel_spd_sf_err,ptr);
            SIRFBINARY_EXPORT16(msg->rr_wheel_spd_sf,ptr);
            SIRFBINARY_EXPORT16(msg->rr_wheel_spd_sf_err,ptr);
            SIRFBINARY_EXPORT16(msg->rear_axle_spd_delta,ptr);
            SIRFBINARY_EXPORT16(msg->rear_axle_avg_spd,ptr);
            SIRFBINARY_EXPORT16(msg->rear_axle_spd_err,ptr);
            SIRFBINARY_EXPORT16(msg->rear_axle_hd_rt,ptr);
            SIRFBINARY_EXPORT16(msg->rear_axle_hd_rt_err,ptr);
            SIRFBINARY_EXPORT16(msg->front_axle_spd_delta,ptr);
            SIRFBINARY_EXPORT16(msg->front_axle_avg_spd,ptr);
            SIRFBINARY_EXPORT16(msg->front_axle_spd_err,ptr);
            SIRFBINARY_EXPORT16(msg->front_axle_hd_rt,ptr);
            SIRFBINARY_EXPORT16(msg->front_axle_hd_rt_err,ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_NAV_SUBSYS: /* 0x03 0x30 */
      {
         tSIRF_MSG_SSB_DR_NAV_SUBSYS * msg = (tSIRF_MSG_SSB_DR_NAV_SUBSYS*) message_structure;

         if ( *packet_length < ( 2 * sizeof(tSIRF_UINT8) +
                                 12 * sizeof(tSIRF_UINT16) +
                                 2 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->gps_hd_rte,ptr);
            SIRFBINARY_EXPORT16(msg->gps_hd_rte_e,ptr);
            SIRFBINARY_EXPORT16(msg->gps_hd,ptr);
            SIRFBINARY_EXPORT16(msg->gps_hd_e,ptr);
            SIRFBINARY_EXPORT16(msg->gps_spd,ptr);
            SIRFBINARY_EXPORT16(msg->gps_spd_e,ptr);
            SIRFBINARY_EXPORT32(msg->gps_pos_e,ptr);
            SIRFBINARY_EXPORT16(msg->dr_hd_rte,ptr);
            SIRFBINARY_EXPORT16(msg->dr_hd_rte_e,ptr);
            SIRFBINARY_EXPORT16(msg->dr_hd,ptr);
            SIRFBINARY_EXPORT16(msg->dr_hd_e,ptr);
            SIRFBINARY_EXPORT16(msg->dr_spd,ptr);
            SIRFBINARY_EXPORT16(msg->dr_spd_e,ptr);
            SIRFBINARY_EXPORT32(msg->dr_pos_e,ptr);
            SIRFBINARY_EXPORT8 (msg->reserved[0],ptr);
            SIRFBINARY_EXPORT8 (msg->reserved[1],ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_VALID: /* 0x05 0x30 */
      {
         tSIRF_MSG_SSB_DR_VALID * msg = (tSIRF_MSG_SSB_DR_VALID*) message_structure;

         if ( *packet_length < (2 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->valid,                         ptr);
            SIRFBINARY_EXPORT32(msg->reserved,                      ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_GYR_FACT_CAL: /* 0x06 30 */
      {
         tSIRF_MSG_SSB_DR_GYR_FACT_CAL * msg = (tSIRF_MSG_SSB_DR_GYR_FACT_CAL*) message_structure;

         if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->cal,                           ptr);
            SIRFBINARY_EXPORT8 (msg->reserved,                      ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_SENS_PARAM: /* 0x07 30 */
      {
         tSIRF_MSG_SSB_DR_SENS_PARAM * msg = (tSIRF_MSG_SSB_DR_SENS_PARAM*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT8) +
                                2 * sizeof(tSIRF_UINT16) +
                                header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->base_ssf,ptr);
            SIRFBINARY_EXPORT16(msg->base_gb,ptr);
            SIRFBINARY_EXPORT16(msg->base_gsf,ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_DATA_BLK: /* 0x08 0x30 */
      {
         tSIRF_MSG_SSB_DR_DATA_BLK * msg = (tSIRF_MSG_SSB_DR_DATA_BLK*) message_structure;

         if ( *packet_length < (  2 * sizeof(tSIRF_UINT8) +
                                 (1 + 2 * SIRF_MSG_SSB_DR_DATASET_LENGTH) * sizeof(tSIRF_UINT16) +
                                 (1 + 1 * SIRF_MSG_SSB_DR_DATASET_LENGTH) * sizeof(tSIRF_UINT32) +
                                  header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->meas_type,ptr);
            SIRFBINARY_EXPORT8(msg->valid_cnt,ptr);
            SIRFBINARY_EXPORT16(msg->bkup_flgs,ptr);
            for (i = 0; i < SIRF_MSG_SSB_DR_DATASET_LENGTH; i++)
            {
               SIRFBINARY_EXPORT32(msg->blk[i].tag,                 ptr);
               SIRFBINARY_EXPORT16(msg->blk[i].spd,                 ptr);
               SIRFBINARY_EXPORT16(msg->blk[i].hd_rte,              ptr);
            }
         }
      }
      break;
      case SIRF_MSG_SSB_MMF_STATUS: /*  0x5030 */
      {
         tSIRF_MSG_SSB_MMF_STATUS *msg = (tSIRF_MSG_SSB_MMF_STATUS*) message_structure;

         if ( *packet_length < ( 2 * sizeof(tSIRF_UINT16) +
                                 9 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->status,   ptr);
            SIRFBINARY_EXPORT16(msg->sys_hdg,  ptr);
            SIRFBINARY_EXPORT32(msg->sys_lat,  ptr);
            SIRFBINARY_EXPORT32(msg->sys_lon,  ptr);
            SIRFBINARY_EXPORT32(msg->sys_alt,  ptr);
            SIRFBINARY_EXPORT32(msg->sys_tow,  ptr);
            SIRFBINARY_EXPORT16(msg->mmf_hdg,  ptr);
            SIRFBINARY_EXPORT32(msg->mmf_lat,  ptr);
            SIRFBINARY_EXPORT32(msg->mmf_lon,  ptr);
            SIRFBINARY_EXPORT32(msg->mmf_alt,  ptr);
            SIRFBINARY_EXPORT32(msg->mmf_tow,  ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SBAS_PARAM: /* 0x32 */
      {
         tSIRF_MSG_SSB_SBAS_PARAM * msg = (tSIRF_MSG_SSB_SBAS_PARAM*) message_structure;

         if ( *packet_length < (12 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->prn,      ptr);
            SIRFBINARY_EXPORT8 (msg->mode,     ptr);
            SIRFBINARY_EXPORT8 (msg->timeout,  ptr);
            SIRFBINARY_EXPORT8 (msg->flg_bits, ptr);
            SIRFBINARY_EXPORT8 (msg->spare[0], ptr);
            SIRFBINARY_EXPORT8 (msg->spare[1], ptr);
            SIRFBINARY_EXPORT8 (msg->spare[2], ptr);
            SIRFBINARY_EXPORT8 (msg->spare[3], ptr);
            SIRFBINARY_EXPORT8 (msg->spare[4], ptr);
            SIRFBINARY_EXPORT8 (msg->spare[5], ptr);
            SIRFBINARY_EXPORT8 (msg->spare[6], ptr);
            SIRFBINARY_EXPORT8 (msg->spare[7], ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SIRFNAV_COMPLETE: /* 0x01 0x33 */
      {
         tSIRF_MSG_SSB_SIRFNAV_COMPLETE *msg = (tSIRF_MSG_SSB_SIRFNAV_COMPLETE*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->reserved, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SIRFNAV_TIME_TAGS: /* 0x04 0x33 */
      {
         tSIRF_MSG_SSB_SIRFNAV_TIME_TAGS *msg = (tSIRF_MSG_SSB_SIRFNAV_TIME_TAGS*) message_structure;

         if ( *packet_length < ( 6 * sizeof(tSIRF_UINT8) +
                                 2 * sizeof(tSIRF_UINT16) +
                                 3 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->tracker_time_tag, ptr);
            SIRFBINARY_EXPORT16(msg->gps_week, ptr);
            SIRFBINARY_EXPORT32(msg->gps_tow, ptr);
            SIRFBINARY_EXPORT8 (msg->tracker_rtc_day, ptr);
            SIRFBINARY_EXPORT8 (msg->tracker_rtc_hour, ptr);
            SIRFBINARY_EXPORT8 (msg->tracker_rtc_min, ptr);
            SIRFBINARY_EXPORT8 (msg->tracker_rtc_sec, ptr);
            SIRFBINARY_EXPORT16(msg->tracker_rtc_clkctr, ptr);
            SIRFBINARY_EXPORT8 (msg->tracker_rtc_ok, ptr);
            SIRFBINARY_EXPORT8 (msg->tracker_rtc_rollover_complete, ptr);
            SIRFBINARY_EXPORT32(msg->tracker_rtc_rollover_seconds, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_TRACKER_LOADER_STATE: /* 0x0633 (51, 6) */
      {
         tSIRF_MSG_SSB_TRACKER_LOADER_STATE *msg = (tSIRF_MSG_SSB_TRACKER_LOADER_STATE*)message_structure;

         if ( *packet_length < ( 4 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32( msg->loader_state,       ptr );
            SIRFBINARY_EXPORT32( msg->percentage_loaded,  ptr );
            SIRFBINARY_EXPORT32( msg->loader_error,       ptr );
            SIRFBINARY_EXPORT32( msg->time_tag,           ptr );
         }
      }
      break;
      case SIRF_MSG_SSB_SIRFNAV_START: /* 0x0733 (51, 7) */
      {
         tSIRF_MSG_SSB_SIRFNAV_START* msg = (tSIRF_MSG_SSB_SIRFNAV_START*) message_structure;

         if ( *packet_length < (4 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->start_mode    ,ptr);
            SIRFBINARY_EXPORT32(msg->clock_offset  ,ptr);
            SIRFBINARY_EXPORT32(msg->port_num      ,ptr);
            SIRFBINARY_EXPORT32(msg->baud_rate     ,ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SIRFNAV_STOP: /* 0x0833 (51, 8) */
      {
         tSIRF_MSG_SSB_SIRFNAV_STOP* msg = (tSIRF_MSG_SSB_SIRFNAV_STOP*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->stop_mode  ,ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_STARTUP_INFO: /* 0x01 0x36 */
      {
         tSIRF_MSG_SSB_STARTUP_INFO * msg = (tSIRF_MSG_SSB_STARTUP_INFO*) message_structure;

         if ( *packet_length < ( 7 * sizeof(tSIRF_UINT8) +
                                 6 * sizeof(tSIRF_UINT16) +
                                 9 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->mode,                   ptr);
            SIRFBINARY_EXPORT8 (msg->status,                 ptr);
            SIRFBINARY_EXPORT8 (msg->dr_status,              ptr);
            SIRFBINARY_EXPORT32(msg->gps_off_time,           ptr);
            SIRFBINARY_EXPORT16(msg->gps_week,               ptr);
            SIRFBINARY_EXPORT32(msg->gps_tow,                ptr);
            SIRFBINARY_EXPORT16(msg->utc_year,               ptr);
            SIRFBINARY_EXPORT8 (msg->utc_month,              ptr);
            SIRFBINARY_EXPORT8 (msg->utc_day,                ptr);
            SIRFBINARY_EXPORT8 (msg->utc_hour,               ptr);
            SIRFBINARY_EXPORT8 (msg->utc_min,                ptr);
            SIRFBINARY_EXPORT16(msg->utc_sec,                ptr);
            SIRFBINARY_EXPORT32(msg->lat,                    ptr);
            SIRFBINARY_EXPORT32(msg->lon,                    ptr);
            SIRFBINARY_EXPORT32(msg->alt_ellips,             ptr);
            SIRFBINARY_EXPORT16(msg->heading,                ptr);
            SIRFBINARY_EXPORT32(msg->valid_ephemeris,        ptr);
            SIRFBINARY_EXPORT32(msg->collected_almanac,      ptr);
            SIRFBINARY_EXPORT16(msg->collected_almanac_week, ptr);
            SIRFBINARY_EXPORT16(msg->factory_almanac_week,   ptr);
            SIRFBINARY_EXPORT32(msg->clk_offset,             ptr);
            SIRFBINARY_EXPORT32(msg->reserved,               ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_EE_GPS_TIME_INFO:  /* 0x01 0x38 */
      {
         tSIRF_MSG_SSB_EE_GPS_TIME_INFO *msg = (tSIRF_MSG_SSB_EE_GPS_TIME_INFO *) message_structure;

         if ( *packet_length < ( 1 * sizeof(tSIRF_UINT8) +
                                 1 * sizeof(tSIRF_UINT16) +
                                 2 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->time_valid_flag,         ptr);
            SIRFBINARY_EXPORT16(msg->gps_week,               ptr);
            SIRFBINARY_EXPORT32(msg->gps_tow,                ptr);
            SIRFBINARY_EXPORT32(msg->ephemeris_request_mask, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_EE_INTEGRITY:  /* 0x02 0x38 */
      {
         tSIRF_MSG_SSB_EE_INTEGRITY *msg = (tSIRF_MSG_SSB_EE_INTEGRITY *) message_structure;

         if ( *packet_length < (3 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->sat_pos_validity_mask, ptr);
            SIRFBINARY_EXPORT32(msg->sat_clk_validity_mask, ptr);
            SIRFBINARY_EXPORT32(msg->sat_health_mask,       ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_EE_STATE:      /* 0x03 0x38 */
      {
         tSIRF_MSG_SSB_EE_STATE *msg =
            (tSIRF_MSG_SSB_EE_STATE *) message_structure;

         if ( *packet_length < (6 * SIRF_MAX_SVID_CNT * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            for( i = 0; i < SIRF_MAX_SVID_CNT; i++)
            {
               SIRFBINARY_EXPORT8(msg[i].svid,            ptr);
               SIRFBINARY_EXPORT8(msg[i].eph_source_flag, ptr);
               SIRFBINARY_EXPORT8(msg[i].week_no,         ptr);
               SIRFBINARY_EXPORT8(msg[i].toe,             ptr);
               SIRFBINARY_EXPORT8(msg[i].eph_integrity,   ptr);
               SIRFBINARY_EXPORT8(msg[i].eph_age,         ptr);
            }
         }
      }
      break;
      case SIRF_MSG_SSB_EE_CLK_BIAS_ADJ:  /* 0x04 0x38 */
      {
         tSIRF_MSG_SSB_EE_CLK_BIAS_ADJ *msg =
            (tSIRF_MSG_SSB_EE_CLK_BIAS_ADJ *) message_structure;

         if ( *packet_length < ( 3 * SIRF_NUM_CHANNELS * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            for(i = 0; i < SIRF_NUM_CHANNELS; i++)
            {
               SIRFBINARY_EXPORT8(msg[i].svid,            ptr);
               SIRFBINARY_EXPORT8(msg[i].toe,             ptr);
               SIRFBINARY_EXPORT8(msg[i].clk_bias_adjust, ptr);
            }
         }
      }
      break;

      case SIRF_MSG_SSB_EE_EPHEMERIS_AGE:     /* 0x11 0x38 */
      {
         tSIRF_MSG_EE_AGE *msg = (tSIRF_MSG_EE_AGE*) message_structure;

         if ( *packet_length < ( 3 * sizeof(tSIRF_UINT8) +
                                 2 * sizeof(tSIRF_UINT16) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->gps_week, ptr);   /* GPS week number in weeks */
            SIRFBINARY_EXPORT16(msg->gps_tow,  ptr);   /* GPS time of week in seconds */
            SIRFBINARY_EXPORT8 (msg->indicator, ptr);  /* 0 means age is valid */
            SIRFBINARY_EXPORT8 (msg->age, ptr);
            SIRFBINARY_EXPORT8 (msg->age_in_days, ptr);
         }
      }
      break;
#ifdef EMB_SIF
      /* added for Embedded CLM */
      case SIRF_MSG_SSB_SIF_ACK_NACK:     /* 0x20 0x38 */
      {
         tSIRF_MSG_SSB_SIF_ACK_NACK *msg = (tSIRF_MSG_SSB_SIF_ACK_NACK*) message_structure;

         if ( *packet_length < ( 4 * sizeof(tSIRF_UINT8) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->ackMsgId, ptr);
            SIRFBINARY_EXPORT8(msg->ackSid,  ptr);
            SIRFBINARY_EXPORT8(msg->ackNack, ptr);
            SIRFBINARY_EXPORT8(msg->reason, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SIF_EE_AGE:     /* 0x21 0x38 */
      {
         tSIRF_MSG_SSB_SIF_GET_EE_AGE *msg = (tSIRF_MSG_SSB_SIF_GET_EE_AGE*) message_structure;

         if ( *packet_length < ( 1 * sizeof(tSIRF_UINT8) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->numSAT, ptr);
            if ( *packet_length < ( 1 * sizeof(tSIRF_UINT8) +
                                  ((3 * sizeof(tSIRF_UINT8) + 6 * sizeof(tSIRF_UINT16)) * msg->numSAT) +
                                  header_len) )
            {
                tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else if (msg->numSAT > SIRF_MAX_SVID_CNT)
            {
               tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
            }
            else
            {
                for(i=0;i<msg->numSAT;i++)
                {
                    SIRFBINARY_EXPORT8(msg->eeAgeStruct[i].prnNum, ptr);
                    SIRFBINARY_EXPORT8(msg->eeAgeStruct[i].ephPosFlag, ptr);
                    SIRFBINARY_EXPORT16(msg->eeAgeStruct[i].eePosAge, ptr);
                    SIRFBINARY_EXPORT16(msg->eeAgeStruct[i].cgeePosGPSWeek, ptr);
                    SIRFBINARY_EXPORT16(msg->eeAgeStruct[i].cgeePosTOE, ptr);
                    SIRFBINARY_EXPORT8(msg->eeAgeStruct[i].ephClkFlag, ptr);
                    SIRFBINARY_EXPORT16(msg->eeAgeStruct[i].eeClkAge, ptr);
                    SIRFBINARY_EXPORT16(msg->eeAgeStruct[i].cgeeClkGPSWeek, ptr);
                    SIRFBINARY_EXPORT16(msg->eeAgeStruct[i].cgeeClkTOE, ptr);
                }
            }
         }
      }
      break;
      case SIRF_MSG_SSB_SIF_SGEE_AGE:     /* 0x22 0x38 */
      {
         tSIRF_MSG_SSB_SIF_SGEE_AGE *msg = (tSIRF_MSG_SSB_SIF_SGEE_AGE*) message_structure;

         if ( *packet_length < ( 2 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->sgeeAge, ptr);
            SIRFBINARY_EXPORT32(msg->predictionInterval,  ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_SIF_PKT_INVOKE_DLD:     /* 0x23 0x38 */
      {
         tSIRF_MSG_SSB_SIF_PKT_INVOKE_DLD *msg = (tSIRF_MSG_SSB_SIF_PKT_INVOKE_DLD*) message_structure;

         if ( *packet_length < ( sizeof(tSIRF_UINT8) + sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->start, ptr);
            SIRFBINARY_EXPORT32(msg->waitTime, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SIF_PKT_ERASE:     /* 0x24 0x38 */
      {
         tSIRF_MSG_SSB_SIF_PKT_ERASE *msg = (tSIRF_MSG_SSB_SIF_PKT_ERASE*) message_structure;

         if ( *packet_length < ( sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->NVMID, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SIF_STR_PKT_DATA:     /* 0x25 0x38 */
      {
         tSIRF_MSG_SSB_SIF_STR_PKT_DATA *msg = (tSIRF_MSG_SSB_SIF_STR_PKT_DATA*) message_structure;
         /* Check we have minimum size of packet assuming we still dont know actual data size*/
         if ( *packet_length < ( sizeof(tSIRF_UINT8) + sizeof(tSIRF_UINT16) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->NVMID, ptr);
            SIRFBINARY_EXPORT16(msg->size,  ptr);
            /* Check we have received specified data*/
            if ( *packet_length < ( sizeof(tSIRF_UINT8) + msg->size * sizeof(tSIRF_UINT8)
                        + 2 * sizeof(tSIRF_UINT16) + sizeof(tSIRF_UINT32) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else if (msg->size > SIF_MAX_PKT_DATA)
            {
               tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
            }
            else
            {
                SIRFBINARY_EXPORT32(msg->offset, ptr);
                SIRFBINARY_EXPORT16(msg->seqNum, ptr);
                for (i=0; i<msg->size; i++)            /** msg->size of data to be copied */
                {
                   SIRFBINARY_EXPORT8 (msg->data[i], ptr);
                }
            }
         }
      }
      break;
      case SIRF_MSG_SSB_SIF_RCV_PKT_DATA:     /* 0x26 0x38 */
      {
         tSIRF_MSG_SSB_SIF_RCV_PKT_DATA *msg = (tSIRF_MSG_SSB_SIF_RCV_PKT_DATA*) message_structure;

         if ( *packet_length < ( 2 * sizeof(tSIRF_UINT8) + msg->numBlocks * sizeof(tSIRF_UINT16) + msg->numBlocks * sizeof(tSIRF_UINT32) + sizeof(tSIRF_UINT16) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else if (msg->numBlocks > MAX_RCV_BLOCKS)
         {
               tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->NVMID, ptr);
            SIRFBINARY_EXPORT16(msg->seqNum, ptr);
            SIRFBINARY_EXPORT8(msg->numBlocks, ptr);
            for (i=0; i<msg->numBlocks; i++)            /** msg->size of data to be copied */
            {
                SIRFBINARY_EXPORT16(msg->size[i],  ptr);
                SIRFBINARY_EXPORT32(msg->offset[i], ptr);
            }
         }
      }
      break;
      case SIRF_MSG_SSB_SIF_NVM_HEADER_DATA:     /* 0x27 0x38 */
      {
         tSIRF_MSG_SSB_SIF_NVM_HEADER_DATA *msg = (tSIRF_MSG_SSB_SIF_NVM_HEADER_DATA*) message_structure;

         if ( *packet_length < ( 2* sizeof(tSIRF_UINT16) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            if ( *packet_length < ( 2* sizeof(tSIRF_UINT16) + msg->pktSize * sizeof(tSIRF_UINT8) +
                                 header_len) )
            {
                tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else if (msg->pktSize > BBRAM_SGEE_HEADER_SIZE_DEFINE)
            {
               tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
            }
            else
            {
                SIRFBINARY_EXPORT16(msg->offset, ptr);
                SIRFBINARY_EXPORT16(msg->pktSize, ptr);
                for (i=0; i<msg->pktSize; i++)            /** msg->size of data to be copied */
                {
                    SIRFBINARY_EXPORT8(msg->pktData[i],  ptr);
                }
            }
         }
      }
      break;
      case SIRF_MSG_SSB_SIF_GET_HOST_HEADER: /* 0x28 0x38 */
      {
         /* similar to E8 1D */
         tSIRF_MSG_SSB_SIF_GET_NVM_HEADER *msg = (tSIRF_MSG_SSB_SIF_GET_NVM_HEADER*) message_structure;

         if ( *packet_length < ( 1 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->reserved, ptr);
         }
      }
      break;
#endif /* EMB_SIF */
      case SIRF_MSG_SSB_EE_ACK:       /* 0xFF 0x38 */
      {
         tSIRF_MSG_SSB_EE_ACK *msg = (tSIRF_MSG_SSB_EE_ACK*) message_structure;

         if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->acq_id, ptr);
            SIRFBINARY_EXPORT8(msg->acq_sub_id, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_TEST_MODE_DATA_7:     /* 0x07 0x3F */
      {
         tSIRF_MSG_SSB_TEST_MODE_DATA_7 *msg = (tSIRF_MSG_SSB_TEST_MODE_DATA_7*) message_structure;

         if ( *packet_length < ( 1 * sizeof(tSIRF_UINT8) +
                                 4 * sizeof(tSIRF_UINT16) +
                                 4 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->test_status, ptr);       /* TM7 status */
            SIRFBINARY_EXPORT32(msg->spur1_frequency,  ptr);  /* spur freq */
            SIRFBINARY_EXPORT16(msg->spur1_sig_to_noise, ptr);/* spur power */
            SIRFBINARY_EXPORT32(msg->spur2_frequency,  ptr);  /* spur freq */
            SIRFBINARY_EXPORT16(msg->spur2_sig_to_noise, ptr);/* spur power */
            SIRFBINARY_EXPORT32(msg->spur3_frequency,  ptr);  /* spur freq */
            SIRFBINARY_EXPORT16(msg->spur3_sig_to_noise, ptr);/* spur power */
            SIRFBINARY_EXPORT32(msg->spur4_frequency, ptr);   /* spur freq */
            SIRFBINARY_EXPORT16(msg->spur4_sig_to_noise, ptr);/* spur power */
         }
      }
      break;
      case SIRF_MSG_SSB_TEST_MODE_DATA_8:     /* 0x08 0x3F */
      {
         tSIRF_MSG_SSB_TEST_MODE_DATA_8 *msg = (tSIRF_MSG_SSB_TEST_MODE_DATA_8*) message_structure;

         if ( *packet_length < ( 1 * sizeof(tSIRF_INT32) +
                                 1 * sizeof(tSIRF_UINT16) +
                                 1 * sizeof(tSIRF_UINT16) +
                                 1 * sizeof(tSIRF_UINT16) +
                                 1 * sizeof(tSIRF_UINT16) +
                                 1 * sizeof(tSIRF_UINT8) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->spurDoppler,  ptr);  /* spur freq */
            SIRFBINARY_EXPORT16(msg->spurCN0, ptr);       /* spur cn0 */
            SIRFBINARY_EXPORT16(msg->jitter, ptr);        /* Osc Phase Jitter */
            SIRFBINARY_EXPORT16(msg->rtcFrequency,  ptr); /* RTC Frequency */
            SIRFBINARY_EXPORT16(msg->etoAcqRatio, ptr);   /* EClk to AcqClk Ratio */
            SIRFBINARY_EXPORT8(msg->tSyncAGCGain, ptr);   /* tSync and AGCGain */
         }
      }
      break;
#ifdef ENABLE_TM9
      case SIRF_MSG_SSB_TEST_MODE_DATA_9:     /* 0x09 0x3F */
      {
         tSIRF_MSG_SSB_TEST_MODE_DATA_9 *msg = (tSIRF_MSG_SSB_TEST_MODE_DATA_9*) message_structure;

         if ( *packet_length < ( 1 * sizeof(tSIRF_UINT8) +
                                 1 * sizeof(tSIRF_UINT8) +
                                 SIRF_MSG_SSB_TEST_MODE_DATA_9_PKT_LEN * sizeof(tSIRF_UINT8) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->iqbuf_seq_num, ptr);  /* buf sequence number */
            SIRFBINARY_EXPORT8(msg->packet_seq_num, ptr); /* packet sequence number */
            for(i=0;i<SIRF_MSG_SSB_TEST_MODE_DATA_9_PKT_LEN;i++)
            {
               SIRFBINARY_EXPORT8(msg->iqsample[i], ptr);    /* IQ samples */
            }
         }
      }
      break;
#endif
      case SIRF_MSG_SSB_GPIO_READ: /* 0xC041 (65,192) */
      {
         tSIRF_MSG_SSB_GPIO_READ *msg = (tSIRF_MSG_SSB_GPIO_READ*)message_structure;

         if ( *packet_length < ( 1 * sizeof(tSIRF_UINT16) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16( msg->gpio_state,       ptr );
         }
      }
      break;

      /* Not Used 0x00, 0x5D */

      case SIRF_MSG_SSB_XO_DEFAULTS_OUT:   /* 0x01, 0x5D */
      {
         tSIRF_MSG_SSB_XO_DEFAULTS_OUT *msg = (tSIRF_MSG_SSB_XO_DEFAULTS_OUT *) message_structure;

         if ( *packet_length < (tSIRF_UINT32)(SIRF_MSG_SSB_XO_DEFAULTS_OUT_LEN + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
#ifdef PVT_BUILD
            /* For 4E, the words need to be swapped. */
            SIRFBINARY_EXPORT8 (msg->source, ptr);          /** Bit Mask with the source of entries */
            SIRFBINARY_EXPORT8 (msg->agingRateUnc, ptr);    /** aging rate uncertainty */
            SIRFBINARY_EXPORT8 (msg->initialOffsetUnc, ptr);/** initial offset uncertainty */
            SIRFBINARY_EXPORT8 (msg->spare1, ptr);
            SIRFBINARY_EXPORT32(msg->clockDrift, ptr);      /** clock drift */
            SIRFBINARY_EXPORT16(msg->tempUnc, ptr);         /** temperature uncertainty */
            SIRFBINARY_EXPORT16(msg->mfgWeek, ptr);         /** manufacturing wn for aging */
            SIRFBINARY_EXPORT32(msg->spare2, ptr);
#else
            /* 4T build goes through MEI.
             * NOTE: This message has already been stored based on the endian
             * so it does need to be swapped again. EXPORT8 = COPYOUT8. */
            SIRFBINARY_EXPORT8 (msg->source, ptr);          /** Bit Mask with the source of entries */
            SIRFBINARY_EXPORT8 (msg->agingRateUnc, ptr);    /** aging rate uncertainty */
            SIRFBINARY_EXPORT8 (msg->initialOffsetUnc, ptr);/** initial offset uncertainty */
            SIRFBINARY_EXPORT8 (msg->spare1, ptr);
            SIRF_COPYOUT32(msg->clockDrift, ptr);           /** clock drift */
            SIRF_COPYOUT16(msg->tempUnc, ptr);              /** temperature uncertainty */
            SIRF_COPYOUT16(msg->mfgWeek, ptr);              /** manufacturing wn for aging */
            SIRF_COPYOUT32(msg->spare2, ptr);
#endif /* PVT_BUILD */
         }
      }
      break;

      case SIRF_MSG_SSB_TCXO_TABLE_OUT:   /* 0x02, 0x5D */
      {
         tSIRF_MSG_SSB_TCXO_TABLE_OUT *msg = (tSIRF_MSG_SSB_TCXO_TABLE_OUT *) message_structure;

         if ( *packet_length < (tSIRF_UINT32)(SIRF_MSG_SSB_TCXO_TABLE_OUT_LEN + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
#ifdef PVT_BUILD
            /* For 4E, the words need to be swapped. */
            SIRFBINARY_EXPORT32(msg->ctr, ptr);             /** Counter incremented with each output */
            SIRFBINARY_EXPORT16(msg->offset, ptr);          /** frequency offset bias from CD default */
            SIRFBINARY_EXPORT16(msg->globalMin, ptr);       /** minimum xo error observed */
            SIRFBINARY_EXPORT16(msg->globalMax, ptr);       /** maximum xo error observed */
            SIRFBINARY_EXPORT16(msg->firstWeek, ptr);       /** full gps week of first table update. */
            SIRFBINARY_EXPORT16(msg->lastWeek, ptr);        /** full gps week of last table update */
            SIRFBINARY_EXPORT16(msg->lsb, ptr);             /** array LSB of Min[] and Max[] */
            SIRFBINARY_EXPORT8 (msg->agingBin, ptr);        /** Bin of last aging update. */
            SIRFBINARY_EXPORT8 (msg->agingUpcount, ptr);    /** Aging detection accumulator */
            SIRFBINARY_EXPORT8 (msg->binCnt, ptr);          /** count of min bins filled */
            SIRFBINARY_EXPORT8 (msg->spare, ptr);           /** not used */
#else
            /* 4T build goes through MEI. */
            /* NOTE: This message has already been stored based on the endian
             * so it does need to be swapped again. EXPORT8 = COPYOUT8. */
            SIRF_COPYOUT32(msg->ctr, ptr);                  /** Counter incremented with each output */
            SIRF_COPYOUT16(msg->offset, ptr);               /** frequency offset bias from CD default */
            SIRF_COPYOUT16(msg->globalMin, ptr);            /** minimum xo error observed */
            SIRF_COPYOUT16(msg->globalMax, ptr);            /** maximum xo error observed */
            SIRF_COPYOUT16(msg->firstWeek, ptr);            /** full gps week of first table update. */
            SIRF_COPYOUT16(msg->lastWeek, ptr);             /** full gps week of last table update */
            SIRF_COPYOUT16(msg->lsb, ptr);                  /** array LSB of Min[] and Max[] */
            SIRFBINARY_EXPORT8 (msg->agingBin, ptr);        /** Bin of last aging update. */
            SIRFBINARY_EXPORT8 (msg->agingUpcount, ptr);    /** Aging detection accumulator */
            SIRFBINARY_EXPORT8 (msg->binCnt, ptr);          /** count of min bins filled */
            SIRFBINARY_EXPORT8 (msg->spare, ptr);           /** not used */
#endif /* PVT_BUILD */

            for (i=0; i<SSB_XOT_TABLE_SIZE; i++)            /** Min XO error at each temperature */
            {
               SIRFBINARY_EXPORT8 (msg->min[i], ptr);
            }
            for (i=0; i<SSB_XOT_TABLE_SIZE; i++)               /** Max XO error at each temperature */
            {
               SIRFBINARY_EXPORT8 (msg->max[i], ptr);
            }
         }
      }
      break;

      /* Not Used 0x03, 0x5D */

      case SIRF_MSG_SSB_XO_TEMP_REC_OUT:   /* 0x04, 0x5D */
      {
         tSIRF_MSG_SSB_XO_TEMP_REC_OUT *msg = (tSIRF_MSG_SSB_XO_TEMP_REC_OUT *) message_structure;

         if ( *packet_length < (tSIRF_UINT32)(SIRF_MSG_SSB_XO_TEMP_REC_OUT_LEN + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
#ifdef PVT_BUILD
            /* For 4E, the words need to be swapped. */
            SIRFBINARY_EXPORT32(msg->currentTimeCount, ptr);     /** time since power on, ms */
            SIRFBINARY_EXPORT16(msg->trTime, ptr);               /** Latest TRec time. */
            SIRFBINARY_EXPORT8 (msg->trTemp, ptr);          /** Latest TRec temperature. */
            SIRFBINARY_EXPORT8 (msg->nCnt,   ptr);          /** Rec N Count. */
            SIRFBINARY_EXPORT8 (msg->totCnt, ptr);          /** Total Rec Count. */
            SIRFBINARY_EXPORT8 (msg->status, ptr);          /** Echo Test Control mode */
            SIRFBINARY_EXPORT16(msg->ctr, ptr);                  /** counter for output messages.
                                                             *  uninitialized and wraparound counter. */
#else
            /* 4T build goes through MEI. */
            /* NOTE: This message has already been stored based on the endian
             * so it does need to be swapped again. EXPORT8 = COPYOUT8. */
            SIRF_COPYOUT32(msg->currentTimeCount, ptr);     /** time since power on, ms */
            SIRF_COPYOUT16(msg->trTime, ptr);               /** Latest TRec time. */
            SIRFBINARY_EXPORT8 (msg->trTemp, ptr);          /** Latest TRec temperature. */
            SIRFBINARY_EXPORT8 (msg->nCnt,   ptr);          /** Rec N Count. */
            SIRFBINARY_EXPORT8 (msg->totCnt, ptr);          /** Total Rec Count. */
            SIRFBINARY_EXPORT8 (msg->status, ptr);          /** Echo Test Control mode */
            SIRF_COPYOUT16(msg->ctr, ptr);                  /** counter for output messages.
                                                             *  uninitialized and wraparound counter. */
#endif /* PVT_BUILD */
         }
      }
      break;

      case SIRF_MSG_SSB_XO_EARC_OUT:   /* 0x05, 0x5D */
      {
         tSIRF_MSG_SSB_XO_EARC_OUT *msg = (tSIRF_MSG_SSB_XO_EARC_OUT *) message_structure;

         if ( *packet_length < (tSIRF_UINT32)(SIRF_MSG_SSB_XO_EARC_OUT_LEN + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
#ifdef PVT_BUILD
            /* For 4E, the words need to be swapped. */
            SIRFBINARY_EXPORT32(msg->currentTimeCount,  ptr);    /** time since power on, ms */
            SIRFBINARY_EXPORT32(msg->acqclkLsw,         ptr);    /** acqclk, lsw */
            SIRFBINARY_EXPORT32(msg->rtcWclkSec,        ptr);    /** rtc wclk secs */
            SIRFBINARY_EXPORT16(msg->rtcWclkCtr,        ptr);    /** rtc wclk ctr */
            SIRFBINARY_EXPORT16(msg->EARC_r0,           ptr);    /** earc r0 */
            SIRFBINARY_EXPORT16(msg->EARC_r1,           ptr);    /** earc r1 */
            SIRFBINARY_EXPORT32(msg->spare,             ptr);
#else
            /* 4T build goes through MEI. */
            /* NOTE: This message has already been stored based on the endian
             * so it does need to be swapped again. EXPORT8 = COPYOUT8. */
            SIRF_COPYOUT32(msg->currentTimeCount,  ptr);    /** time since power on, ms */
            SIRF_COPYOUT32(msg->acqclkLsw,         ptr);    /** acqclk, lsw */
            SIRF_COPYOUT32(msg->rtcWclkSec,        ptr);    /** rtc wclk secs */
            SIRF_COPYOUT16(msg->rtcWclkCtr,        ptr);    /** rtc wclk ctr */
            SIRF_COPYOUT16(msg->EARC_r0,           ptr);    /** earc r0 */
            SIRF_COPYOUT16(msg->EARC_r1,           ptr);    /** earc r1 */
            SIRF_COPYOUT32(msg->spare,             ptr);
#endif /* PVT_BUILD */
         }
      }
      break;

      case SIRF_MSG_SSB_XO_RTC_ALARM_OUT:   /* 0x06, 0x5D */
      {
         tSIRF_MSG_SSB_XO_RTC_ALARM_OUT *msg = (tSIRF_MSG_SSB_XO_RTC_ALARM_OUT *) message_structure;

         if ( *packet_length < (tSIRF_UINT32)(SIRF_MSG_SSB_XO_RTC_ALARM_OUT_LEN + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
#ifdef PVT_BUILD
            /* For 4E, the words need to be swapped. */
            SIRFBINARY_EXPORT32(msg->currentTimeCount,  ptr);    /** time since power on, ms */
            SIRFBINARY_EXPORT32(msg->acqclkLsw,         ptr);    /** acqclk, lsw */
            SIRFBINARY_EXPORT32(msg->rtcWclkSec,        ptr);    /** rtc wclk secs */
            SIRFBINARY_EXPORT16(msg->rtcWclkCtr,        ptr);    /** rtc wclk ctr */
            SIRFBINARY_EXPORT16(msg->spare,             ptr);
#else
            /* 4T build goes through MEI. */
            /* NOTE: This message has already been stored based on the endian
             * so it does need to be swapped again. EXPORT8 = COPYOUT8. */
            SIRF_COPYOUT32(msg->currentTimeCount,  ptr);    /** time since power on, ms */
            SIRF_COPYOUT32(msg->acqclkLsw,         ptr);    /** acqclk, lsw */
            SIRF_COPYOUT32(msg->rtcWclkSec,        ptr);    /** rtc wclk secs */
            SIRF_COPYOUT16(msg->rtcWclkCtr,        ptr);    /** rtc wclk ctr */
            SIRF_COPYOUT16(msg->spare,             ptr);
#endif /* PVT_BUILD */
         }
      }
      break;

      case SIRF_MSG_SSB_XO_RTC_CAL_OUT:   /* 0x07, 0x5D */
      {
         tSIRF_MSG_SSB_XO_RTC_CAL_OUT *msg = (tSIRF_MSG_SSB_XO_RTC_CAL_OUT *) message_structure;

         if ( *packet_length < (tSIRF_UINT32)(SIRF_MSG_SSB_XO_RTC_CAL_OUT_LEN + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
#ifdef PVT_BUILD
            /* For 4E, the words need to be swapped. */
            SIRFBINARY_EXPORT32(msg->currentTimeCount,  ptr);    /** time since power on, ms */
            SIRFBINARY_EXPORT32(msg->acqclkLsw,         ptr);    /** acqclk, lsw */
            SIRFBINARY_EXPORT32(msg->gpsTimeSec,        ptr);    /** gps time, secs */
            SIRFBINARY_EXPORT32(msg->gpsTimeFrac,       ptr);    /** gps time, fractional nsecs */
            SIRFBINARY_EXPORT32(msg->rtcWclkSec,        ptr);    /** rtc time, seconds */
            SIRFBINARY_EXPORT16(msg->rtcWclkCtr,        ptr);    /** rtc time, counter */
            SIRFBINARY_EXPORT16(msg->rtcFreqUnc,        ptr);    /** rtc freq unc, ppb */
            SIRFBINARY_EXPORT32(msg->rtcDriftInt,       ptr);    /** rtc drift ratio, integer */
            SIRFBINARY_EXPORT32(msg->rtcDriftFrac,      ptr);    /** rtc drift ratio, frac */
            SIRFBINARY_EXPORT32(msg->rtcTimeUnc,        ptr);    /** rtc time unc, usec */
            SIRFBINARY_EXPORT32(msg->rtcFreqOffset,     ptr);    /** rtc freq offset, ppb*/
            SIRFBINARY_EXPORT32(msg->xoFreqOffset,      ptr);    /** rtc freq offset, ppb */
            SIRFBINARY_EXPORT16(msg->gpsWeek,           ptr);    /** gps week */
            SIRFBINARY_EXPORT16(msg->spare,             ptr);
#else
            /* 4T build goes through MEI. */
            /* NOTE: This message has already been stored based on the endian
             * so it does need to be swapped again. EXPORT8 = COPYOUT8. */
            SIRF_COPYOUT32(msg->currentTimeCount,  ptr);    /** time since power on, ms */
            SIRF_COPYOUT32(msg->acqclkLsw,         ptr);    /** acqclk, lsw */
            SIRF_COPYOUT32(msg->gpsTimeSec,        ptr);    /** gps time, secs */
            SIRF_COPYOUT32(msg->gpsTimeFrac,       ptr);    /** gps time, fractional nsecs */
            SIRF_COPYOUT32(msg->rtcWclkSec,        ptr);    /** rtc time, seconds */
            SIRF_COPYOUT16(msg->rtcWclkCtr,        ptr);    /** rtc time, counter */
            SIRF_COPYOUT16(msg->rtcFreqUnc,        ptr);    /** rtc freq unc, ppb */
            SIRF_COPYOUT32(msg->rtcDriftInt,       ptr);    /** rtc drift ratio, integer */
            SIRF_COPYOUT32(msg->rtcDriftFrac,      ptr);    /** rtc drift ratio, frac */
            SIRF_COPYOUT32(msg->rtcTimeUnc,        ptr);    /** rtc time unc, usec */
            SIRF_COPYOUT32(msg->rtcFreqOffset,     ptr);    /** rtc freq offset, ppb*/
            SIRF_COPYOUT32(msg->xoFreqOffset,      ptr);    /** rtc freq offset, ppb */
            SIRF_COPYOUT16(msg->gpsWeek,           ptr);    /** gps week */
            SIRF_COPYOUT16(msg->spare,             ptr);
#endif /* PVT_BUILD */
         }
      }
      break;

      /* Not Used. 0x08, 0x5D */

      case SIRF_MSG_SSB_XO_MPM_ACQ_OUT:   /* 0x09, 0x5D */
      {
         tSIRF_MSG_SSB_XO_MPM_ACQ_OUT *msg = (tSIRF_MSG_SSB_XO_MPM_ACQ_OUT *) message_structure;

         /* The message records are serially positioned in the buffer */
         tSIRF_MSG_SSB_XO_MPM_ACQ_OUT_REC *mPRec = (tSIRF_MSG_SSB_XO_MPM_ACQ_OUT_REC *)&msg->pRec;

         if ( *packet_length < (tSIRF_UINT32)(SIRF_MSG_SSB_XO_MPM_ACQ_OUT_LEN
                             + (msg->numRecs) * SIRF_MSG_SSB_XO_MPM_ACQ_OUT_REC_LEN
                             + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
#ifdef PVT_BUILD
            /* For 4E, the words need to be swapped. */
            SIRFBINARY_EXPORT8 (msg->numRecs,         ptr);    /** numRecs */
            SIRFBINARY_EXPORT8 (msg->spare1,          ptr);    /** spare */
            SIRFBINARY_EXPORT16(msg->spare2,          ptr);    /** spare */
            SIRFBINARY_EXPORT32(msg->currentTimeCount,ptr);    /** time since power on, ms */
            SIRFBINARY_EXPORT32(msg->acqclkLsw,       ptr);    /** acqclk, lsw */

            /** following fields all based on numRecs */
            for (i=0; i<msg->numRecs; i++, msg->pRec++)
            {
               SIRFBINARY_EXPORT32(mPRec->codePhase,  ptr);    /** code Phase */
               SIRFBINARY_EXPORT32(mPRec->doppler,    ptr);    /** doppler */
               SIRFBINARY_EXPORT32(mPRec->codeOffset, ptr);    /** code Offset */
               SIRFBINARY_EXPORT32(mPRec->peakMag,    ptr);    /** peak Magnitude */
               SIRFBINARY_EXPORT16(mPRec->status,     ptr);    /** status */
               SIRFBINARY_EXPORT8 (mPRec->prn,        ptr);    /** svid */
               SIRFBINARY_EXPORT8 (mPRec->spare,      ptr);
            }
#else
            /* 4T build goes through MEI. */
            /* NOTE: This message has already been stored based on the endian
             * so it does need to be swapped again. EXPORT8 = COPYOUT8. */
            SIRFBINARY_EXPORT8 (msg->numRecs,      ptr);    /** numRecs */
            SIRFBINARY_EXPORT8 (msg->spare1,       ptr);    /** spare */
            SIRF_COPYOUT16(msg->spare2,            ptr);    /** spare */
            SIRF_COPYOUT32(msg->currentTimeCount,  ptr);    /** time since power on, ms */
            SIRF_COPYOUT32(msg->acqclkLsw,         ptr);    /** acqclk, lsw */

            /** following fields all based on numRecs */
            for (i=0; i<msg->numRecs; i++, msg->pRec++)
            {
               SIRF_COPYOUT32(mPRec->codePhase,ptr);    /** code Phase */
               SIRF_COPYOUT32(mPRec->doppler,  ptr);    /** doppler */
               SIRF_COPYOUT32(mPRec->codeOffset,ptr);   /** code Offset */
               SIRF_COPYOUT32(mPRec->peakMag,  ptr);    /** peak Magnitude */
               SIRF_COPYOUT16(mPRec->status,   ptr);    /** status */
               SIRFBINARY_EXPORT8 (mPRec->prn, ptr);    /** svid */
               SIRFBINARY_EXPORT8 (mPRec->spare,ptr);
            }
#endif /* PVT_BUILD */
         }
      }
      break;

      case SIRF_MSG_SSB_XO_MPM_PREPOS_OUT:   /* 0x0A, 0x5D */
      {
         tSIRF_MSG_SSB_XO_MPM_PREPOS_OUT *msg = (tSIRF_MSG_SSB_XO_MPM_PREPOS_OUT *) message_structure;

         /* The message records are serially positioned in the buffer */
         tSIRF_MSG_SSB_XO_MPM_PREPOS_OUT_REC *mPRec = (tSIRF_MSG_SSB_XO_MPM_PREPOS_OUT_REC *)&msg->pRec;

         if ( *packet_length < (tSIRF_UINT32)(SIRF_MSG_SSB_XO_MPM_PREPOS_OUT_LEN
                             + (msg->numRecs) * SIRF_MSG_SSB_XO_MPM_PREPOS_OUT_REC_LEN
                             + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
#ifdef PVT_BUILD
            /* For 4E, the words need to be swapped. */
            SIRFBINARY_EXPORT8 (msg->numRecs,         ptr);    /** numRecs */
            SIRFBINARY_EXPORT8 (msg->spare1,          ptr);
            SIRFBINARY_EXPORT16(msg->spare2,          ptr);
            SIRFBINARY_EXPORT32(msg->currentTimeCount,ptr);    /** time since power on, ms */
            SIRFBINARY_EXPORT32(msg->acqclkLsw,       ptr);    /** acqclk, lsw */

            /** following fields all based on numRecs */
            for (i=0; i<msg->numRecs; i++, mPRec++)
            {
               SIRFBINARY_EXPORT32(mPRec->pr,         ptr);    /** psuedoRange, 0.1 m*/
               SIRFBINARY_EXPORT16(mPRec->prr,        ptr);    /** psuedoRangeRate, m/s */
               SIRFBINARY_EXPORT8 (mPRec->prn,        ptr);    /** svid */
               SIRFBINARY_EXPORT8 (mPRec->spare,      ptr);
            }
#else
            /* 4T build goes through MEI. */
            /* NOTE: This message has already been stored based on the endian
             * so it does need to be swapped again. EXPORT8 = COPYOUT8. */
            SIRFBINARY_EXPORT8 (msg->numRecs,      ptr);    /** numRecs */
            SIRFBINARY_EXPORT8 (msg->spare1,       ptr);
            SIRF_COPYOUT16(msg->spare2,            ptr);
            SIRF_COPYOUT32(msg->currentTimeCount,  ptr);    /** time since power on, ms */
            SIRF_COPYOUT32(msg->acqclkLsw,         ptr);    /** acqclk, lsw */

            /** following fields all based on numRecs */
            for (i=0; i<msg->numRecs; i++, mPRec++)
            {
               SIRF_COPYOUT32(mPRec->pr,       ptr);    /** psuedoRange, 0.1 m*/
               SIRF_COPYOUT16(mPRec->prr,      ptr);    /** psuedoRangeRate, m/s */
               SIRFBINARY_EXPORT8 (mPRec->prn, ptr);    /** svid */
               SIRFBINARY_EXPORT8 (mPRec->spare,ptr);
            }
#endif /* PVT_BUILD */
         }
      }
      break;

      case SIRF_MSG_SSB_XO_UNL_MEAS_OUT:   /* 0x0B, 0x5D */
      {
         tSIRF_MSG_SSB_XO_UNL_MEAS_OUT *msg = (tSIRF_MSG_SSB_XO_UNL_MEAS_OUT *) message_structure;

         /* The message records are serially positioned in the buffer */
         tSIRF_MSG_SSB_XO_UNL_MEAS_OUT_REC *mPRec = (tSIRF_MSG_SSB_XO_UNL_MEAS_OUT_REC *)&msg->pRec;


         if ( *packet_length < (tSIRF_UINT32)(SIRF_MSG_SSB_XO_UNL_MEAS_OUT_LEN
                             + (msg->numRecs) * SIRF_MSG_SSB_XO_UNL_MEAS_OUT_REC_LEN
                             + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
#ifdef PVT_BUILD
            /* For 4E, the words need to be swapped. */
            SIRFBINARY_EXPORT8 (msg->numRecs,         ptr);    /**< numRecs */
            SIRFBINARY_EXPORT8 (msg->mode,            ptr);    /**< mode */
            SIRFBINARY_EXPORT16(msg->spare,           ptr);
            SIRFBINARY_EXPORT32(msg->currentTimeCount,ptr);    /**< time since power on, ms */
            SIRFBINARY_EXPORT32(msg->acqclkLsw,       ptr);    /**< acqclk, lsw */
            SIRFBINARY_EXPORT32(msg->timeCorr,        ptr);    /**< time correction */
            SIRFBINARY_EXPORT32(msg->timeCorrUnc,     ptr);    /**< time correction unc */
            SIRFBINARY_EXPORT16(msg->freqCorr,        ptr);    /**< frequency correction */
            SIRFBINARY_EXPORT16(msg->freqCorrUnc,     ptr);    /**< frequency correction unc */

            /** following fields all based on numRecs */
            for (i=0; i<msg->numRecs; i++, mPRec++)
            {
               SIRFBINARY_EXPORT32(mPRec->pr,         ptr);    /**< psuedoRange, 0.1 m */
               SIRFBINARY_EXPORT16(mPRec->prr,        ptr);    /**< psuedoRangeRate, m/s */
               SIRFBINARY_EXPORT16(mPRec->cno,        ptr);    /**< CNo 0.1 dbHz */
               SIRFBINARY_EXPORT8(mPRec->prn,         ptr);    /**< svid */
               SIRFBINARY_EXPORT8(mPRec->spare1,      ptr);
               SIRFBINARY_EXPORT16(mPRec->spare2,     ptr);
            }
#else
            /* 4T build goes through MEI. */
            /* NOTE: This message has already been stored based on the endian
             * so it does need to be swapped again. EXPORT8 = COPYOUT8. */
            SIRFBINARY_EXPORT8 (msg->numRecs,      ptr);    /**< numRecs */
            SIRFBINARY_EXPORT8 (msg->mode,         ptr);    /**< mode */
            SIRF_COPYOUT16(msg->spare,             ptr);
            SIRF_COPYOUT32(msg->currentTimeCount,  ptr);    /**< time since power on, ms */
            SIRF_COPYOUT32(msg->acqclkLsw,         ptr);    /**< acqclk, lsw */
            SIRF_COPYOUT32(msg->timeCorr,          ptr);    /**< time correction */
            SIRF_COPYOUT32(msg->timeCorrUnc,       ptr);    /**< time correction unc */
            SIRF_COPYOUT16(msg->freqCorr,          ptr);    /**< frequency correction */
            SIRF_COPYOUT16(msg->freqCorrUnc,       ptr);    /**< frequency correction unc */

            /** following fields all based on numRecs */
            for (i=0; i<msg->numRecs; i++, mPRec++)
            {
               SIRF_COPYOUT32(mPRec->pr,           ptr);    /**< psuedoRange, 0.1 m */
               SIRF_COPYOUT16(mPRec->prr,          ptr);    /**< psuedoRangeRate, m/s */
               SIRF_COPYOUT16(mPRec->cno,          ptr);    /**< CNo 0.1 dbHz */
               SIRFBINARY_EXPORT8(mPRec->prn,      ptr);    /**< svid */
               SIRFBINARY_EXPORT8(mPRec->spare1,   ptr);
               SIRF_COPYOUT16(mPRec->spare2,       ptr);
            }
#endif /* PVT_BUILD */
         }
      }
      break;

      case SIRF_MSG_SSB_XO_UNCERTAINTY_OUT:   /* 0x0C, 0x5D */
      {
         tSIRF_MSG_SSB_XO_UNCERTAINTY_OUT *msg = (tSIRF_MSG_SSB_XO_UNCERTAINTY_OUT *) message_structure;

         if ( *packet_length < (tSIRF_UINT32)(SIRF_MSG_SSB_XO_UNCERTAINTY_OUT_LEN + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
#ifdef PVT_BUILD
            /* For 4E, the words need to be swapped. */
            SIRFBINARY_EXPORT32(msg->currentTimeCount,ptr);    /** time since power on, ms */
            SIRFBINARY_EXPORT32(msg->acqclkLsw,       ptr);    /** acqclk, lsw */
            SIRFBINARY_EXPORT32(msg->freq,            ptr);    /** freq, ppb */
            SIRFBINARY_EXPORT16(msg->freqUncNom,      ptr);    /** freq unc nominal, ppb */
            SIRFBINARY_EXPORT16(msg->freqUncFull,     ptr);    /** freq unc full, ppb */
            SIRFBINARY_EXPORT16(msg->tempUncNom,      ptr);    /** temperature unc nominal, ppb */
            SIRFBINARY_EXPORT16(msg->tempUncFull,     ptr);    /** temperature unc full, ppb */
            SIRFBINARY_EXPORT16(msg->ageUncNom,       ptr);    /** age unc nominal, ppb */
            SIRFBINARY_EXPORT16(msg->ageUncFull,      ptr);    /** age unc full, ppb */
            SIRFBINARY_EXPORT16(msg->measUncNom,      ptr);    /** meas unc nominal, ppb */
            SIRFBINARY_EXPORT16(msg->measUncFull,     ptr);    /** meas unc full, ppb */
            SIRFBINARY_EXPORT16(msg->gpsWeek,         ptr);    /** gps week */
            SIRFBINARY_EXPORT8 (msg->trTemp,          ptr);    /** Latest TRec temperature. */
            SIRFBINARY_EXPORT8 (msg->spare,           ptr);
#else
            /* 4T build goes through MEI. */
            /* NOTE: This message has already been stored based on the endian
             * so it does need to be swapped again. EXPORT8 = COPYOUT8. */
            SIRF_COPYOUT32(msg->currentTimeCount,  ptr);    /** time since power on, ms */
            SIRF_COPYOUT32(msg->acqclkLsw,         ptr);    /** acqclk, lsw */
            SIRF_COPYOUT32(msg->freq,              ptr);    /** freq, ppb */
            SIRF_COPYOUT16(msg->freqUncNom,        ptr);    /** freq unc nominal, ppb */
            SIRF_COPYOUT16(msg->freqUncFull,       ptr);    /** freq unc full, ppb */
            SIRF_COPYOUT16(msg->tempUncNom,        ptr);    /** temperature unc nominal, ppb */
            SIRF_COPYOUT16(msg->tempUncFull,       ptr);    /** temperature unc full, ppb */
            SIRF_COPYOUT16(msg->ageUncNom,         ptr);    /** age unc nominal, ppb */
            SIRF_COPYOUT16(msg->ageUncFull,        ptr);    /** age unc full, ppb */
            SIRF_COPYOUT16(msg->measUncNom,        ptr);    /** meas unc nominal, ppb */
            SIRF_COPYOUT16(msg->measUncFull,       ptr);    /** meas unc full, ppb */
            SIRF_COPYOUT16(msg->gpsWeek,           ptr);    /** gps week */
            SIRFBINARY_EXPORT8(msg->trTemp,        ptr);    /** Latest TRec temperature. */
            SIRFBINARY_EXPORT8(msg->spare,         ptr);
#endif /* PVT_BUILD */
         }
      }
      break;

      case SIRF_MSG_SSB_XO_SYS_TIME_OUT:   /* 0x0D, 0x5D */
      {
         tSIRF_MSG_SSB_XO_SYS_TIME_OUT *msg = (tSIRF_MSG_SSB_XO_SYS_TIME_OUT *) message_structure;

         if ( *packet_length < (tSIRF_UINT32)(SIRF_MSG_SSB_XO_SYS_TIME_OUT_LEN + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
#ifdef PVT_BUILD
            /* For 4E, the words need to be swapped. */
            SIRFBINARY_EXPORT32(msg->currentTimeCount,  ptr);    /** time since power on, ms */
            SIRFBINARY_EXPORT32(msg->acqclkMsw,         ptr);    /** acqclk, msw */
            SIRFBINARY_EXPORT32(msg->acqclkLsw,         ptr);    /** acqclk, lsw */
            SIRFBINARY_EXPORT32(msg->gpsTimeSec,        ptr);    /** gps time, secs */
            SIRFBINARY_EXPORT32(msg->gpsTimeFrac,       ptr);    /** gps time, fractional nsecs */
            SIRFBINARY_EXPORT32(msg->rtcWclkSec,        ptr);    /** rtc wclk secs */
            SIRFBINARY_EXPORT16(msg->rtcWclkCtr,        ptr);    /** rtc wclk ctr */
            SIRFBINARY_EXPORT16(msg->clockBias,         ptr);    /** clock bias */
            SIRFBINARY_EXPORT16(msg->clockDrift,        ptr);    /** clock drift */
            SIRFBINARY_EXPORT16(msg->spare,             ptr);
#else
            /* 4T build goes through MEI. */
            /* NOTE: This message has already been stored based on the endian
             * so it does need to be swapped again. EXPORT8 = COPYOUT8. */
            SIRF_COPYOUT32(msg->currentTimeCount,  ptr);    /** time since power on, ms */
            SIRF_COPYOUT32(msg->acqclkMsw,         ptr);    /** acqclk, msw */
            SIRF_COPYOUT32(msg->acqclkLsw,         ptr);    /** acqclk, lsw */
            SIRF_COPYOUT32(msg->gpsTimeSec,        ptr);    /** gps time, secs */
            SIRF_COPYOUT32(msg->gpsTimeFrac,       ptr);    /** gps time, fractional nsecs */
            SIRF_COPYOUT32(msg->rtcWclkSec,        ptr);    /** rtc wclk secs */
            SIRF_COPYOUT16(msg->rtcWclkCtr,        ptr);    /** rtc wclk ctr */
            SIRF_COPYOUT16(msg->clockBias,         ptr);    /** clock bias */
            SIRF_COPYOUT16(msg->clockDrift,        ptr);    /** clock drift */
            SIRF_COPYOUT16(msg->spare,             ptr);
#endif /* PVT_BUILD */
         }
      }
      break;

      case SIRF_MSG_SSB_XO_RTC_TABLE_OUT:   /* 0x0E, 0x5D */
      {
         tSIRF_MSG_SSB_XO_RTC_TABLE_OUT *msg = (tSIRF_MSG_SSB_XO_RTC_TABLE_OUT *) message_structure;

         if ( *packet_length < (tSIRF_UINT32)(SIRF_MSG_SSB_XO_RTC_TABLE_OUT_LEN + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
#ifdef PVT_BUILD
            /* For 4E, the words need to be swapped. */
            for (i=0; i<SSB_XO_RTC_TABLE_SIZE; i++)
            {
               SIRFBINARY_EXPORT16(msg->val[i], ptr);
            }
            SIRFBINARY_EXPORT16(msg->fMax,   ptr);    /** Max value = (1 - f) * 1e-6 */
            SIRFBINARY_EXPORT16(msg->spare,  ptr);
#else
            /* 4T build goes through MEI. */
            /* NOTE: This message has already been stored based on the endian
             * so it does need to be swapped again. EXPORT8 = COPYOUT8. */
            for (i=0; i<SSB_XO_RTC_TABLE_SIZE; i++)
            {
               SIRF_COPYOUT16(msg->val[i], ptr);
            }
            SIRF_COPYOUT16(msg->fMax,   ptr);    /** Max value = (1 - f) * 1e-6 */
            SIRF_COPYOUT16(msg->spare,  ptr);
#endif /* PVT_BUILD */
         }
      }
      break;

#ifdef XO_ENABLED

      case SIRF_MSG_SSB_XO_POLY_OUT:   /* 0x0F, 0x5D */
      {
         tSIRF_MSG_SSB_XO_POLY_OUT *msg = (tSIRF_MSG_SSB_XO_POLY_OUT *) message_structure;

         if ( *packet_length < (tSIRF_UINT32)(SIRF_MSG_SSB_XO_POLY_OUT_LEN + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
#ifdef PVT_BUILD
            /* For 4E, the words need to be swapped.
             * If 4T is added, they pass through MEI and do not get swapped */
            SIRFBINARY_EXPORT32(msg->C[0],      ptr);          /** polynomial co-efficients */
            SIRFBINARY_EXPORT32(msg->C[1],      ptr);
            SIRFBINARY_EXPORT32(msg->C[2],      ptr);
            SIRFBINARY_EXPORT32(msg->C[3],      ptr);
            SIRFBINARY_EXPORT16(msg->tempUnc,   ptr);          /** temperature uncertainty */
            SIRFBINARY_EXPORT8(msg->polySource, ptr);          /** Source of the initial Polynomial values */
            SIRFBINARY_EXPORT8(msg->spare1,     ptr);
            SIRFBINARY_EXPORT32(msg->spare2,    ptr);
#else
            /* 4T build goes through MEI. */
            /* NOTE: This message has already been stored based on the endian
             * so it does need to be swapped again. EXPORT8 = COPYOUT8. */

            /* TBD */
#endif /* PVT_BUILD */
         }
      }
      break;

      case SIRF_MSG_SSB_XO_TABLE_OUT:   /* 0x10, 0x5D */
      {
         tSIRF_MSG_SSB_XO_TABLE_OUT *msg = (tSIRF_MSG_SSB_XO_TABLE_OUT *) message_structure;

         if ( *packet_length < (tSIRF_UINT32)(SIRF_MSG_SSB_XO_TABLE_OUT_LEN + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
#ifdef PVT_BUILD
            /* For 4E, the words need to be swapped.
             * If 4T is added, they pass through MEI and do not get swapped */

            /** T for XO update */
            SIRFBINARY_EXPORT8(msg->calT, ptr);

            /** Active susbsystem, T = XO, F= TCXO. */
            SIRFBINARY_EXPORT8(msg->xoActive, ptr);

            /** Source of XoActive */
            SIRFBINARY_EXPORT8(msg->xoActiveSource, ptr);

            /** Update Mode
             * 0 = no updates
             * 1 = update C0 term when 1 bin filled
             * 2 = update C0, C1 term when 2-4 bins filled
             * 3 = update all C terms when 5 or more bins filled */
            SIRFBINARY_EXPORT8(msg->updMode, ptr);

            /** Average frequency for the bin, in ppb  */
            for (i=0; i<SSB_XO_MODEL_SIZE; i++)
            {
               SIRFBINARY_EXPORT16(msg->fMean[i], ptr);
            }

            /** Average Temperature for the bin, 1/256 T  */
            for (i=0; i<SSB_XO_MODEL_SIZE; i++)
            {
               SIRFBINARY_EXPORT16(msg->tMean[i], ptr);
            }
#else
            /* 4T build goes through MEI. */
            /* NOTE: This message has already been stored based on the endian
             * so it does need to be swapped again. EXPORT8 = COPYOUT8. */

            /* TBD */
#endif /* PVT_BUILD */
         }
      }
      break;

      case SIRF_MSG_SSB_XO_DENSITY_OUT:   /* 0x11, 0x5D */
      {
         tSIRF_MSG_SSB_XO_DENSITY_OUT *msg = (tSIRF_MSG_SSB_XO_DENSITY_OUT *) message_structure;

         if ( *packet_length < (tSIRF_UINT32)(SIRF_MSG_SSB_XO_DENSITY_OUT_LEN + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
#ifdef PVT_BUILD
            /* For 4E, the words need to be swapped.
             * If 4T is added, they pass through MEI and do not get swapped */

            for (i=0; i<SSB_XO_MODEL_SIZE; i++)
            {
               SIRFBINARY_EXPORT8(msg->bin[i], ptr);
            }
#else
            /* 4T build goes through MEI. */
            /* NOTE: This message has already been stored based on the endian
             * so it does need to be swapped again. EXPORT8 = COPYOUT8. */

            /* TBD */
#endif /* PVT_BUILD */
         }
      }
      break;

#endif /* XO_ENABLED */

      case SIRF_MSG_SSB_DOP_VALUES: /* 0x42 */
      {
         tSIRF_MSG_SSB_DOP_VALUES *msg = (tSIRF_MSG_SSB_DOP_VALUES*) message_structure;

         if ( *packet_length < ( 5 * sizeof(tSIRF_UINT16) +
                                 1 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->gps_tow, ptr);
            SIRFBINARY_EXPORT16(msg->gdop,    ptr);
            SIRFBINARY_EXPORT16(msg->pdop,    ptr);
            SIRFBINARY_EXPORT16(msg->hdop,    ptr);
            SIRFBINARY_EXPORT16(msg->vdop,    ptr);
            SIRFBINARY_EXPORT16(msg->tdop,    ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_INITIALIZE: /* 0x80 */
      {
         tSIRF_MSG_SSB_INITIALIZE * msg = (tSIRF_MSG_SSB_INITIALIZE*) message_structure;

         if ( *packet_length < ( 2 * sizeof(tSIRF_UINT8) +
                                 1 * sizeof(tSIRF_UINT16) +
                                 5 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->ecef_x,        ptr);
            SIRFBINARY_EXPORT32(msg->ecef_y,        ptr);
            SIRFBINARY_EXPORT32(msg->ecef_z,        ptr);
            SIRFBINARY_EXPORT32(msg->clk_offset,    ptr);
            SIRFBINARY_EXPORT32(msg->gps_tow,       ptr);
            SIRFBINARY_EXPORT16(msg->gps_week,      ptr);
            SIRFBINARY_EXPORT8 (msg->chnl_cnt,      ptr);
            SIRFBINARY_EXPORT8 (msg->restart_flags, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SET_NMEA_MODE: /* 0x81 */
      {
         tSIRF_MSG_SSB_SET_NMEA_MODE * msg = (tSIRF_MSG_SSB_SET_NMEA_MODE*) message_structure;

         if ( *packet_length < ( 2 * 10 * sizeof(tSIRF_UINT8) +
                                 2 * sizeof(tSIRF_UINT16) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->mode,                ptr);
            for (i = 0; i < SIRF_MSG_SSB_MAX_NUM_NMEA_MSG; i++)
            {
               SIRFBINARY_EXPORT8(msg->nmea_cfg[i].rate,  ptr);
               SIRFBINARY_EXPORT8(msg->nmea_cfg[i].cksum, ptr);
            }
            SIRFBINARY_EXPORT16(msg->baud_rate,           ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SET_ALMANAC: /* 0x82 */
      {
         tSIRF_MSG_SSB_SET_ALMANAC * msg = (tSIRF_MSG_SSB_SET_ALMANAC*) message_structure;

         if ( *packet_length < ( SIRF_MAX_SVID_CNT * SIRF_MSG_SSB_ALMANAC_DATA_ENTRY * sizeof(tSIRF_UINT16) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            for (i = 0; i < SIRF_MAX_SVID_CNT; i++)
            {
               for (j = 0; j < SIRF_MSG_SSB_ALMANAC_DATA_ENTRY; j++)
               {
                  SIRFBINARY_EXPORT16(msg->almanac[i].data[j], ptr);
               }
            }
         }
      }
      break;
      case SIRF_MSG_SSB_POLL_SW_VERSION: /* 0x84 */
      {
         tSIRF_MSG_SSB_POLL_SW_VERSION * msg = (tSIRF_MSG_SSB_POLL_SW_VERSION*) message_structure;

         if ( *packet_length < header_len )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            if ( *packet_length >= (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               SIRFBINARY_EXPORT8 (msg->reserved, ptr);
            }
         }
      }
      break;
      case SIRF_MSG_SSB_SET_DGPS_SOURCE: /* 0x85 */
      {
         tSIRF_MSG_SSB_SET_DGPS_SOURCE * msg = (tSIRF_MSG_SSB_SET_DGPS_SOURCE*) message_structure;

         if ( *packet_length < ( 2 * sizeof(tSIRF_UINT8) +
                                 1 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->src,      ptr);
            SIRFBINARY_EXPORT32(msg->freq,     ptr);
            SIRFBINARY_EXPORT8 (msg->bit_rate, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SET_NAV_MODE: /* 0x88 */
      {
         tSIRF_MSG_SSB_SET_NAV_MODE* msg = (tSIRF_MSG_SSB_SET_NAV_MODE*) message_structure;

         if ( *packet_length < ( 11 * sizeof(tSIRF_UINT8) +
                                 1 * sizeof(tSIRF_UINT16) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->enable_3d,          ptr);
            SIRFBINARY_EXPORT8 (msg->enable_con_alt,     ptr);
            SIRFBINARY_EXPORT8 (msg->degraded_mode,      ptr);
            SIRFBINARY_EXPORT8 (msg->pos_mode_enable,    ptr);
            SIRFBINARY_EXPORT8 (msg->dr_enable,          ptr);
            SIRFBINARY_EXPORT16(msg->alt_input,          ptr);
            SIRFBINARY_EXPORT8 (msg->alt_mode,           ptr);
            SIRFBINARY_EXPORT8 (msg->alt_src,            ptr);
            SIRFBINARY_EXPORT8 (msg->coast_timeout,      ptr);
            SIRFBINARY_EXPORT8 (msg->degraded_timeout,   ptr);
            SIRFBINARY_EXPORT8 (msg->dr_timeout,         ptr);
            SIRFBINARY_EXPORT8 (msg->trk_smooth,         ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SET_DOP_MODE: /* 0x89 */
      {
         tSIRF_MSG_SSB_SET_DOP_MODE * msg = (tSIRF_MSG_SSB_SET_DOP_MODE*) message_structure;

         if ( *packet_length < (4 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->mode,    ptr);
            SIRFBINARY_EXPORT8(msg->gdop_th, ptr);
            SIRFBINARY_EXPORT8(msg->pdop_th, ptr);
            SIRFBINARY_EXPORT8(msg->hdop_th, ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_SET_DGPS_MODE: /* 0x8A */
      {
         tSIRF_MSG_SSB_SET_DGPS_MODE *msg = (tSIRF_MSG_SSB_SET_DGPS_MODE*)message_structure;
         if (*packet_length < (2*sizeof(tSIRF_UINT8) + header_len))
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->mode,    ptr);
            SIRFBINARY_EXPORT8(msg->timeout, ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_SET_ELEV_MASK: /* 0x8B */
      {
         tSIRF_MSG_SSB_SET_ELEV_MASK * msg = (tSIRF_MSG_SSB_SET_ELEV_MASK*) message_structure;

         if ( *packet_length < (2 * sizeof(tSIRF_UINT16) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->trk, ptr);
            SIRFBINARY_EXPORT16(msg->nav, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SET_POWER_MASK: /* 0x8C */
      {
         tSIRF_MSG_SSB_SET_POWER_MASK * msg = (tSIRF_MSG_SSB_SET_POWER_MASK*) message_structure;

         if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->trk, ptr);
            SIRFBINARY_EXPORT8(msg->nav, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SET_STAT_NAV: /* 0x8F */
      {
         tSIRF_MSG_SSB_SET_STAT_NAV* msg = (tSIRF_MSG_SSB_SET_STAT_NAV*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->staticnav, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_POLL_CLOCK_STATUS: /* 0x90 */
      {
         tSIRF_MSG_SSB_POLL_CLOCK_STATUS * msg = (tSIRF_MSG_SSB_POLL_CLOCK_STATUS*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->reserved, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_POLL_ALMANAC: /* 0x92 */
      {
         tSIRF_MSG_SSB_POLL_ALMANAC * msg = (tSIRF_MSG_SSB_POLL_ALMANAC*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->reserved, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_POLL_EPHEMERIS: /* 0x93 */
      {
         tSIRF_MSG_SSB_POLL_EPHEMERIS * msg = (tSIRF_MSG_SSB_POLL_EPHEMERIS*) message_structure;

         if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->svid,     ptr);
            SIRFBINARY_EXPORT8 (msg->reserved, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SET_EPHEMERIS: /* 0x95 */
      {
         tSIRF_MSG_SSB_SET_EPHEMERIS * msg = (tSIRF_MSG_SSB_SET_EPHEMERIS*) message_structure;

         if ( *packet_length < (3 * 15 * sizeof(tSIRF_UINT16) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            for (i = 0; i < 3; i++)
            {
               for (j = 0; j < 15; j++)
               {
                  SIRFBINARY_EXPORT16(msg->subframe[i][j], ptr);
               }
            }
         }
      }
      break;
      case SIRF_MSG_SSB_SET_OP_MODE: /* 0x96 */
      {
         tSIRF_MSG_SSB_SET_OP_MODE * msg = (tSIRF_MSG_SSB_SET_OP_MODE*) message_structure;

         if ( *packet_length < ( 1 * sizeof(tSIRF_UINT8) +
                                 3 * sizeof(tSIRF_UINT16) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->mode,   ptr);
            SIRFBINARY_EXPORT16(msg->svid,   ptr);
            SIRFBINARY_EXPORT16(msg->period, ptr);
            SIRFBINARY_EXPORT8(msg->tm5Cmd, ptr );
         }
      }
      break;
      case SIRF_MSG_SSB_POLL_RECEIVER_PARAMS: /* 0x98 */
      {
         tSIRF_MSG_SSB_NAV_POLL_RECEIVER_PARAMS * msg = (tSIRF_MSG_SSB_NAV_POLL_RECEIVER_PARAMS*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->reserved, ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_DEMO_TEST_CPU_STRESS: /* 0x02A1 */
      {
         tSIRF_MSG_SSB_DEMO_TEST_CPU_STRESS *msg = (tSIRF_MSG_SSB_DEMO_TEST_CPU_STRESS*)message_structure;

         if ( *packet_length < ( 1 * sizeof(tSIRF_UINT16) +
                                 5 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16 ( msg->TaskID,           ptr );
            SIRFBINARY_EXPORT32 ( msg->PeriodOn,         ptr );
            SIRFBINARY_EXPORT32 ( msg->PeriodOff,        ptr );
            SIRFBINARY_EXPORT32 ( msg->LoadDuration,     ptr );
            SIRFBINARY_EXPORT32 ( msg->FullLoadDuration, ptr );
            SIRFBINARY_EXPORT32 ( msg->OffsetTime,       ptr );
         }
      }
      break;

      case SIRF_MSG_SSB_DEMO_START_GPS_ENGINE: /* 0x05 0xA1 */
      {
         tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE *msg = (tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE*) message_structure;

         if ( *packet_length < (4 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32 (msg->start_mode  , ptr);
            SIRFBINARY_EXPORT32 (msg->clock_offset, ptr);
            SIRFBINARY_EXPORT32 (msg->port_num    , ptr);
            SIRFBINARY_EXPORT32 (msg->baud_rate   , ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE: /* 0x06 0xA1 */
      {
         tSIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE *msg = (tSIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32 (msg->stop_mode  , ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SIRFNAV_STORE_NOW: /* 0x07A1 */
      {
         /* Nothing to do here, message has no data */
      }
      break;
      case SIRF_MSG_SSB_DEMO_START_NAV_ENGINE: /* 0x08A1 */
      {
         tSIRF_UINT8 temp[MAX_PORT_NUM_STRING_LENGTH];
         tSIRF_UINT32 i;

         tSIRF_MSG_SSB_DEMO_START_NAV_ENGINE *msg = (tSIRF_MSG_SSB_DEMO_START_NAV_ENGINE*) message_structure;

         /* Add correct size */
         if ( *packet_length < ( (unsigned) SIRF_CONFIG_SIZE + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32 (msg->config.start_mode                    , ptr);
            SIRFBINARY_EXPORT8  (msg->config.uart_max_preamble             , ptr);
            SIRFBINARY_EXPORT8  (msg->config.uart_idle_byte_wakeup_delay   , ptr);
            SIRFBINARY_EXPORT32 (msg->config.ref_clk_offset                , ptr);
            SIRFBINARY_EXPORT8  (msg->config.uart_hw_fc                    , ptr);
            SIRFBINARY_EXPORT8  (msg->config.lna_type                      , ptr);
            SIRFBINARY_EXPORT8  (msg->config.debug_settings                , ptr);
            SIRFBINARY_EXPORT16 (msg->config.ref_clk_warmup_delay          , ptr);
            SIRFBINARY_EXPORT32 (msg->config.ref_clk_frequency             , ptr);
            SIRFBINARY_EXPORT32 (msg->config.ref_clk_uncertainty           , ptr);
            SIRFBINARY_EXPORT32 (msg->config.uart_baud_rate                , ptr);
            SIRFBINARY_EXPORT8  (msg->config.io_pin_configuration_mode     , ptr);
            for (i=0; i<SIRFNAV_UI_CTRL_NUM_GPIO_PINS_CONFIG; i++)
            {
               SIRFBINARY_EXPORT16 (msg->config.io_pin_configuration[i]    , ptr);
            }
            SIRFBINARY_EXPORT16 (msg->config.i2c_host_address              , ptr);
            SIRFBINARY_EXPORT16 (msg->config.i2c_tracker_address           , ptr);
            SIRFBINARY_EXPORT8  (msg->config.i2c_mode                      , ptr);
            SIRFBINARY_EXPORT8  (msg->config.i2c_rate                      , ptr);
            SIRFBINARY_EXPORT8  (msg->config.spi_rate                      , ptr);
            SIRFBINARY_EXPORT8  (msg->config.on_off_control                , ptr);
            SIRFBINARY_EXPORT8  (msg->config.flash_mode                    , ptr);
            SIRFBINARY_EXPORT8  (msg->config.storage_mode                  , ptr);
            memcpy(&temp, msg->config.tracker_port, sizeof(msg->config.tracker_port));
            for (i=0; i<MAX_PORT_NUM_STRING_LENGTH; i++)
            {
               SIRFBINARY_EXPORT8 (temp[i]                                 , ptr);
            }
            SIRFBINARY_EXPORT8  (msg->config.tracker_port_select           , ptr);
            SIRFBINARY_EXPORT32 (msg->config.weak_signal_enabled           , ptr);
            SIRFBINARY_EXPORT8  (msg->config.backup_LDO_mode_enabled       , ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SET_MSG_RATE: /* 0xA6 */
      {
         tSIRF_MSG_SSB_SET_MSG_RATE * msg = (tSIRF_MSG_SSB_SET_MSG_RATE*) message_structure;

         if ( *packet_length < (7 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->mode,         ptr);
            SIRFBINARY_EXPORT8 (msg->msg_id,       ptr);
            SIRFBINARY_EXPORT8 (msg->rate,         ptr);
            SIRFBINARY_EXPORT8 (msg->sub_id,       ptr);
            SIRFBINARY_EXPORT8 (msg->reserved[0],  ptr);
            SIRFBINARY_EXPORT8 (msg->reserved[1],  ptr);
            SIRFBINARY_EXPORT8 (msg->reserved[2],  ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SET_SBAS_PRN: /* 0xAA */
      {
         tSIRF_MSG_SSB_SET_SBAS_PRN * msg = (tSIRF_MSG_SSB_SET_SBAS_PRN*) message_structure;

         if ( *packet_length < ( 5 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->prnOrRegion, ptr);
            SIRFBINARY_EXPORT8 (msg->mode,        ptr);
            SIRFBINARY_EXPORT8 (msg->flagBits,    ptr);
            SIRFBINARY_EXPORT8 (msg->region,      ptr);
            SIRFBINARY_EXPORT8 (msg->regionPRN,   ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_SET_NAV_INIT: /* 0x01 0xAC */
      {
         tSIRF_MSG_SSB_DR_SET_NAV_INIT * msg = (tSIRF_MSG_SSB_DR_SET_NAV_INIT*) message_structure;

         if ( *packet_length < ( 2 * sizeof(tSIRF_UINT8) +
                                 2 * sizeof(tSIRF_UINT16) +
                                 5 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->lat,            ptr);
            SIRFBINARY_EXPORT32(msg->lon,            ptr);
            SIRFBINARY_EXPORT32(msg->alt_ellips,     ptr);
            SIRFBINARY_EXPORT16(msg->heading,        ptr);
            SIRFBINARY_EXPORT32(msg->clk_offset,     ptr);
            SIRFBINARY_EXPORT32(msg->gps_tow,        ptr);
            SIRFBINARY_EXPORT16(msg->gps_week,       ptr);
            SIRFBINARY_EXPORT8 (msg->chnl_cnt,       ptr);
            SIRFBINARY_EXPORT8 (msg->restart_flags,  ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_SET_NAV_MODE: /* 0x02 0xAC */
      {
         tSIRF_MSG_SSB_DR_SET_NAV_MODE * msg = (tSIRF_MSG_SSB_DR_SET_NAV_MODE*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->mode,ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_SET_GYR_FACT_CAL: /* 0x03 0xAC */
      {
         tSIRF_MSG_SSB_DR_SET_GYR_FACT_CAL * msg = (tSIRF_MSG_SSB_DR_SET_GYR_FACT_CAL*) message_structure;

         if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->cal,        ptr);
            SIRFBINARY_EXPORT8 (msg->reserved,   ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_SET_SENS_PARAM: /* 0x04 0xAC */
      {
         tSIRF_MSG_SSB_DR_SET_SENS_PARAM * msg = (tSIRF_MSG_SSB_DR_SET_SENS_PARAM*) message_structure;

         if ( *packet_length < ( 1 * sizeof(tSIRF_UINT8) +
                                 2 * sizeof(tSIRF_UINT16) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->base_ssf,ptr);
            SIRFBINARY_EXPORT16(msg->base_gb,ptr);
            SIRFBINARY_EXPORT16(msg->base_gsf,ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_DR_POLL_VALID:        /* 0x05 0xAC */
      case SIRF_MSG_SSB_DR_POLL_GYR_FACT_CAL: /* 0x06 0xAC */
      case SIRF_MSG_SSB_DR_POLL_SENS_PARAM:   /* 0x07 0xAC */
         break;

      case SIRF_MSG_SSB_DR_CAR_BUS_DATA: /* 0x09 0xAC */
      {
         tSIRF_MSG_SSB_DR_CAR_BUS_DATA *msg = (tSIRF_MSG_SSB_DR_CAR_BUS_DATA*) message_structure;

         if ( *packet_length < ( (2 + 2 * msg->num_of_data_sets) * sizeof(tSIRF_UINT8) +
                                 (1 + 5 * msg->num_of_data_sets) * sizeof(tSIRF_UINT16) +
                                 msg->num_of_data_sets * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else if (msg->num_of_data_sets > MAX_DR_CAR_BUS_DATA_SETS)
         {
            tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->sensor_data_type, ptr);
            SIRFBINARY_EXPORT8 (msg->num_of_data_sets, ptr);
            SIRFBINARY_EXPORT16(msg->reverse_bitmap,   ptr);

            for (i = 0; i < (msg->num_of_data_sets); i++)
            {
               SIRFBINARY_EXPORT8 (msg->data[i].valid_sensor_indication,  ptr);
               SIRFBINARY_EXPORT32(msg->data[i].data_set_time_tag,        ptr);
               SIRFBINARY_EXPORT16(msg->data[i].odometer_speed,           ptr);
               SIRFBINARY_EXPORT16(msg->data[i].data1,                    ptr);
               SIRFBINARY_EXPORT16(msg->data[i].data2,                    ptr);
               SIRFBINARY_EXPORT16(msg->data[i].data3,                    ptr);
               SIRFBINARY_EXPORT16(msg->data[i].data4,                    ptr);
               SIRFBINARY_EXPORT8 (msg->data[i].reserved,                 ptr);
            }
         }
      }
      break;
      case SIRF_MSG_SSB_DR_CAR_BUS_ENABLED: /* 0x0A 0xAC */
      {
         tSIRF_MSG_SSB_DR_CAR_BUS_ENABLED *msg = (tSIRF_MSG_SSB_DR_CAR_BUS_ENABLED*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32 (msg->mode, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_CAR_BUS_DISABLED: /* 0x0B 0xAC */
      {
         tSIRF_MSG_SSB_DR_CAR_BUS_DISABLED *msg = (tSIRF_MSG_SSB_DR_CAR_BUS_DISABLED*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32 (msg->mode, ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_DR_SENSOR_DATA: /* 0x19 0xAC */
      {
         tSIRF_MSG_SSB_DR_SENSOR_DATA *msg = (tSIRF_MSG_SSB_DR_SENSOR_DATA*) message_structure;

         if ( *packet_length < ( 2 * sizeof(tSIRF_UINT8) +
                                 (1 + (1+SIRF_MSG_SSB_DR_SENSOR_DATA_SET_MAX) * msg->num_of_data_sets) * sizeof(tSIRF_UINT16) +
                                 msg->num_of_data_sets * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else if (msg->num_of_data_sets > SIRF_MSG_SSB_DR_SENSOR_DATA_MAX)
         {
            tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->sensor_data_type, ptr);
            SIRFBINARY_EXPORT8 (msg->num_of_data_sets, ptr);
            SIRFBINARY_EXPORT16(msg->reserved,         ptr);

            for ( i = 0; i < (msg->num_of_data_sets); i++ )
            {
               SIRFBINARY_EXPORT16(msg->data_set[i].valid_data_indication,  ptr);
               SIRFBINARY_EXPORT32(msg->data_set[i].data_set_time_tag,      ptr);

               for ( j=0; j<SIRF_MSG_SSB_DR_SENSOR_DATA_SET_MAX; j++ )
               {
                  SIRFBINARY_EXPORT16(msg->data_set[i].data[j],             ptr);
               }
            }
         }
      }
      break;

      case SIRF_MSG_SSB_MMF_DATA: /* 0x50 0xAC */
      {
         tSIRF_MSG_SSB_MMF_DATA *msg = (tSIRF_MSG_SSB_MMF_DATA*) message_structure;

         if ( *packet_length < (  2 * sizeof(tSIRF_UINT8) +
                                 (3 * 3) * sizeof(tSIRF_UINT16) +
                                 (1 + 5 * 3) * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->gps_tow_reference, ptr);
            SIRFBINARY_EXPORT8 (msg->num_of_data_sets,  ptr);
            SIRFBINARY_EXPORT8 (msg->control,           ptr);
            for (i = 0; i < 3; i++)
            {
               SIRFBINARY_EXPORT32(msg->mmf_data[i].lat,              ptr);
               SIRFBINARY_EXPORT32(msg->mmf_data[i].lon,              ptr);
               SIRFBINARY_EXPORT32(msg->mmf_data[i].horiz_pos_uncert, ptr);
               SIRFBINARY_EXPORT32(msg->mmf_data[i].alt_ellips,       ptr);
               SIRFBINARY_EXPORT32(msg->mmf_data[i].vert_pos_uncert,  ptr);
               SIRFBINARY_EXPORT16(msg->mmf_data[i].heading,          ptr);
               SIRFBINARY_EXPORT16(msg->mmf_data[i].heading_uncert,   ptr);
               SIRFBINARY_EXPORT16(msg->mmf_data[i].reserved,         ptr);
            }
         }
      }
      break;
      case SIRF_MSG_SSB_MMF_SET_MODE: /* 0x51 0xAC */
      {
         tSIRF_MSG_SSB_MMF_SET_MODE *msg = (tSIRF_MSG_SSB_MMF_SET_MODE*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->mode,                   ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_GPIO_WRITE: /* 0xB2 0x30 (178,48) */
      {
         tSIRF_MSG_SSB_GPIO_WRITE *msg = (tSIRF_MSG_SSB_GPIO_WRITE*)message_structure;

         if ( *packet_length < (2 * sizeof(tSIRF_UINT16) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->gpio_to_write, ptr);
            SIRFBINARY_EXPORT16(msg->gpio_state,    ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_GPIO_MODE_SET: /* 0xB2 0x31 (178,49) */
      {
         tSIRF_MSG_SSB_GPIO_MODE_SET *msg = (tSIRF_MSG_SSB_GPIO_MODE_SET*)message_structure;

         if ( *packet_length < (3 * sizeof(tSIRF_UINT16) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->gpio_to_set, ptr);
            SIRFBINARY_EXPORT16(msg->gpio_mode,   ptr);
            SIRFBINARY_EXPORT16(msg->gpio_state,  ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_TRK_HW_TEST_CONFIG: /* 0xCF */
      {
         tSIRF_MSG_SSB_TRK_HW_TEST_CONFIG *msg = (tSIRF_MSG_SSB_TRK_HW_TEST_CONFIG*) message_structure;

         if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->RFTestPoint,             ptr);
            SIRFBINARY_EXPORT8(msg->INTCpuPause,             ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_SET_IF_TESTPOINT: /* 0xD2 */
      {
         tSIRF_MSG_SSB_SET_IF_TESTPOINT *msg = (tSIRF_MSG_SSB_SET_IF_TESTPOINT*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->test_point_control,      ptr);
         }
      }
      break;
      case SIRF_MSG_SSB_TRKR_PEEKPOKE_RSP:   /* 0x04 0xB2 */
      {
         tSIRF_MSG_SSB_TRKR_PEEKPOKE_CMD *msg = (tSIRF_MSG_SSB_TRKR_PEEKPOKE_CMD*) message_structure;

         /* 4-byte or n-byte poke response sent to SLD */
         SIRFBINARY_EXPORT8(msg->type, ptr);

         /* send the rest of the data only if peek or multi-peek response */
         if (msg->type == 0)        /* 4-byte peek response */
         {
            /* Simpler version of mei_reverseBytes() */
            SIRFBINARY_EXPORT32(msg->address,   ptr);
            for (k=3 ; k>=0 ; k--)
            {
               SIRFBINARY_EXPORT8(msg->data[k], ptr);
            }
         }
         else if (msg->type == 2)   /* n-byte peek response */
         {
            SIRFBINARY_EXPORT32(msg->address,   ptr);
            SIRFBINARY_EXPORT16(msg->numbytes,  ptr);
            if(msg->numbytes > MAX_DATA_SIZE)
            {
               tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
            }
            else
            {
               for (k = 0; k < msg->numbytes; k++)
               {
                  SIRFBINARY_EXPORT8(msg->data[k], ptr);
               }
            }
         }
      }
      break;

      case SIRF_MSG_SSB_TRKR_FLASHSTORE_RSP:   /* 0x05 0xB2 */
      {
         tSIRF_MSG_SSB_TRKR_FLASHSTORE_CMD *msg = (tSIRF_MSG_SSB_TRKR_FLASHSTORE_CMD*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_RESULT) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            /* ACK/NAK sent to SLD */
            SIRFBINARY_EXPORT32(msg->ack_result, ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_TRKR_FLASHERASE_RSP:   /* 0x06 0xB2 */
      {
         tSIRF_MSG_SSB_TRKR_FLASHERASE_CMD *msg = (tSIRF_MSG_SSB_TRKR_FLASHERASE_CMD*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_RESULT) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            /* ACK/NAK sent to SLD */
            SIRFBINARY_EXPORT32(msg->ack_result, ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_TRKR_CONFIG_RSP:       /* 0x07 0xB2 */
      {
         tSIRF_MSG_SSB_TRKR_CONFIG_CMD *msg = (tSIRF_MSG_SSB_TRKR_CONFIG_CMD*) message_structure;

         if ( *packet_length < (sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            /* ACK/NAK sent to the user */
            SIRFBINARY_EXPORT8(msg->ack_result, ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_TRKR_CUSTOMIO_RSP:     /* 0x08 0xB2 */
      {
         if ( *packet_length < (header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         /* There's no payload sent to the user for this message.  Only the
            message ID is sufficient for the user to know the custom I/O
            message was received correctly by the tracker.                    */
      }
      break;

#ifdef PVT_BUILD
      case SIRF_MSG_SSB_TRKR_CONFIG_POLL_RSP:       /* 0x0A 0xB2 */
      {
         tSIRF_MSG_SSB_TRKR_CONFIG* msg = (tSIRF_MSG_SSB_TRKR_CONFIG*) message_structure;

         if ( *packet_length < (SIRF_MSG_SSB_TRKR_CONFIG_LENGTH + (tSIRF_UINT32)header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->ref_clk_frequency,            ptr);
            SIRFBINARY_EXPORT16(msg->ref_clk_warmup_delay,         ptr);
            SIRFBINARY_EXPORT32(msg->ref_clk_uncertainty,          ptr);
            SIRFBINARY_EXPORT32(msg->ref_clk_offset,               ptr);
            SIRFBINARY_EXPORT8(msg->ext_lna_enable,                ptr);
            SIRFBINARY_EXPORT8(msg->io_pin_config_enable,          ptr);
            for(i=0; i<SIRFNAV_UI_CTRL_NUM_GPIO_PINS_CONFIG; i++)
            {
               SIRFBINARY_EXPORT16(msg->io_pin_config[i],          ptr);
            }
            SIRFBINARY_EXPORT8(msg->uart_max_preamble,             ptr);
            SIRFBINARY_EXPORT8(msg->uart_idle_byte_wakeup_delay,   ptr);
            SIRFBINARY_EXPORT32(msg->uart_baud_rate,               ptr);
            SIRFBINARY_EXPORT8(msg->uart_hw_fc,                    ptr);
            SIRFBINARY_EXPORT16(msg->i2c_master_addr,              ptr);
            SIRFBINARY_EXPORT16(msg->i2c_slave_addr,               ptr);
            SIRFBINARY_EXPORT8(msg->i2c_rate,                      ptr);
            SIRFBINARY_EXPORT8(msg->i2c_mode,                      ptr);
            SIRFBINARY_EXPORT16(msg->i2c_max_msg_length,           ptr);
            SIRFBINARY_EXPORT8(msg->pwr_ctrl_on_off,               ptr);
            SIRFBINARY_EXPORT8(msg->backup_LDO_mode_enabled,       ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_CCK_POLL_RSP:       /* 0x0C 0xB2 */
      {
         tSIRF_MSG_SSB_CCK_POLL_RSP* msg = (tSIRF_MSG_SSB_CCK_POLL_RSP*) message_structure;

         if ( *packet_length < (sizeof(tSIRF_MSG_SSB_CCK_POLL_RSP) + (tSIRF_UINT32)header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->baudRate,                 ptr);
            SIRFBINARY_EXPORT32(msg->i2cHostAddr,              ptr);
            SIRFBINARY_EXPORT32(msg->i2c4EAddr,                ptr);
            SIRFBINARY_EXPORT32(msg->waitStates,               ptr);
            SIRFBINARY_EXPORT32(msg->tcxoWarmupDelay,          ptr);
            SIRFBINARY_EXPORT32(msg->cgeeDisableNumSecs,       ptr);
            SIRFBINARY_EXPORT32(msg->maxEPELimit,              ptr);
            for(i=0; i<USR_GPIO_PIN_MAX; i++)
            {
               SIRFBINARY_EXPORT16(msg->ioPinConfig[i],        ptr);
            }
            SIRFBINARY_EXPORT16(msg->maxAlt,                   ptr);
            for(i=0; i<USR_MAX_NUM_SENSORS; i++)
            {
               SIRFBINARY_EXPORT8(msg->drMemsI2CAddTable[i],   ptr);
            }
            SIRFBINARY_EXPORT8(msg->drFlashI2CAdd,             ptr);
            SIRFBINARY_EXPORT8(msg->ioProtocol,                ptr);
            for(i=0; i<MAX_RATE_CNFG_MSGS; i++)
            {
               SIRFBINARY_EXPORT8(msg->ioRate[i],              ptr);
            }
            SIRFBINARY_EXPORT8(msg->uartFlowCtrl,              ptr);
            SIRFBINARY_EXPORT8(msg->tcxoFreq,                  ptr);
            SIRFBINARY_EXPORT8(msg->tcxoUnc,                   ptr);
            SIRFBINARY_EXPORT8(msg->trkSmoothing,              ptr);
            SIRFBINARY_EXPORT8(msg->staticNav,                 ptr);
            SIRFBINARY_EXPORT8(msg->drTimeout,                 ptr);
            SIRFBINARY_EXPORT8(msg->revEESupport,              ptr);
            SIRFBINARY_EXPORT8(msg->startUpPowMode,            ptr);
            SIRFBINARY_EXPORT8(msg->powCycleTime,              ptr);
            SIRFBINARY_EXPORT8(msg->spiFirstBit,               ptr);
            SIRFBINARY_EXPORT8(msg->spiMode,                   ptr);
            SIRFBINARY_EXPORT8(msg->lnaSetting,                ptr);
            SIRFBINARY_EXPORT8(msg->eeStorage,                 ptr);
            SIRFBINARY_EXPORT8(msg->patchStorage,              ptr);
            SIRFBINARY_EXPORT8(msg->i2cClkRate,                ptr);
            SIRFBINARY_EXPORT8(msg->i2cMode,                   ptr);
            SIRFBINARY_EXPORT8(msg->i2cAddType,                ptr);
            SIRFBINARY_EXPORT8(msg->ioPinConfigEn,             ptr);
            SIRFBINARY_EXPORT8(msg->hostPortSelect,            ptr);
            SIRFBINARY_EXPORT8(msg->backupLDOCntl,             ptr);
            SIRFBINARY_EXPORT8(msg->drI2CRate,                 ptr);
            SIRFBINARY_EXPORT8(msg->cgeeDisable,               ptr);
            SIRFBINARY_EXPORT8(msg->fiveHzNav,                 ptr);
            SIRFBINARY_EXPORT8(msg->sgeeSupport,               ptr);
            SIRFBINARY_EXPORT8(msg->i2cEepromPart,             ptr);
            SIRFBINARY_EXPORT32(msg->reserved_1,               ptr);
            SIRFBINARY_EXPORT32(msg->reserved_2,               ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_SWTB_PMPROMPT_OUT:     /* 0x90 0xB2 */
      {
         tSIRF_MSG_SSB_PM_PROMPT_OUT *msg = (tSIRF_MSG_SSB_PM_PROMPT_OUT *)message_structure;
           /* packet length is compared with the size of the structure, which is determined by
            * adding all the fields*sizeof(field) of the structure,
            * alternatively it could be compared with sizeof(structure) */
         if ( *packet_length < (4 * sizeof(tSIRF_UINT16) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16( msg->ChipId, ptr );
            SIRFBINARY_EXPORT16( msg->SiliconVersion, ptr );
            SIRFBINARY_EXPORT16( msg->ROMVersionCode, ptr );
            SIRFBINARY_EXPORT16( msg->PatchVersionCode, ptr );
         }
      }
      break;

      case SIRF_MSG_SSB_SWTB_PMACK_OUT:         /* 0x91 0xB2 */
      {
         tSIRF_MSG_SSB_PM_ACK_OUT *msg = (tSIRF_MSG_SSB_PM_ACK_OUT *)message_structure;
          /* packet length is compared with the size of the structure, which is determined by
           * adding all the fields*sizeof(field) of the structure,
           * alternatively it could be compared with sizeof(structure) */
         if ( *packet_length < ( 1 * sizeof(tSIRF_UINT16) +
                                 2 * sizeof(tSIRF_UINT8) + header_len ) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16( msg->MessageSeqNo, ptr );
            SIRFBINARY_EXPORT8 ( msg->MessageIdAck, ptr );
            SIRFBINARY_EXPORT8 ( msg->AckStatus, ptr );
         }
      }
      break;
#endif /* PVT_BUILD */

      case SIRF_MSG_SSB_SIRF_INTERNAL:   /* 0xE1 */
      {
         if ( *packet_length < ( (message_length * sizeof(tSIRF_UINT8)) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            memcpy(ptr, message_structure, message_length);
            ptr += message_length;
         }
      }
      break;

      case SIRF_MSG_SSB_EE_SEA_PROVIDE_EPH:   /* 0x01 0xE8 */
      {
         tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH *msg = (tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH*) message_structure;
           /* packet length is compared with the size of the structure, which is determined by
            * adding all the fields*sizeof(field) of the structure,
            *  alrenatively it could be compared with sizeof(structure) */
         if ( *packet_length < ( (8 + 7 * SV_PER_PACKET) * sizeof(tSIRF_UINT8) +
                                 (1 + 11 * SV_PER_PACKET) * sizeof(tSIRF_UINT16) +
                                 (8 * SV_PER_PACKET) * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16( msg->week,      ptr );
            SIRFBINARY_EXPORT32( msg->svid_mask, ptr );
            for( i = 0; i < SV_PER_PACKET; i++ )
            {
               SIRFBINARY_EXPORT8 ( msg->extended_ephemeris[i].PRN,      ptr );
               SIRFBINARY_EXPORT8 ( msg->extended_ephemeris[i].ephemerisValidityFlag, ptr );
               SIRFBINARY_EXPORT8 ( msg->extended_ephemeris[i].URA,      ptr );
               SIRFBINARY_EXPORT8 ( msg->extended_ephemeris[i].IODE,     ptr );
               SIRFBINARY_EXPORT16( msg->extended_ephemeris[i].Crs,      ptr );
               SIRFBINARY_EXPORT16( msg->extended_ephemeris[i].deltaN,   ptr );
               SIRFBINARY_EXPORT32( msg->extended_ephemeris[i].M0,       ptr );
               SIRFBINARY_EXPORT16( msg->extended_ephemeris[i].Cuc,      ptr );
               SIRFBINARY_EXPORT32( msg->extended_ephemeris[i].eccen,    ptr );
               SIRFBINARY_EXPORT16( msg->extended_ephemeris[i].Cus,      ptr );
               SIRFBINARY_EXPORT32( msg->extended_ephemeris[i].sqrtA,    ptr );
               SIRFBINARY_EXPORT16( msg->extended_ephemeris[i].toe,      ptr );
               SIRFBINARY_EXPORT16( msg->extended_ephemeris[i].Cic,      ptr );
               SIRFBINARY_EXPORT32( msg->extended_ephemeris[i].omega0,   ptr );
               SIRFBINARY_EXPORT16( msg->extended_ephemeris[i].Cis,      ptr );
               SIRFBINARY_EXPORT32( msg->extended_ephemeris[i].i0,       ptr );
               SIRFBINARY_EXPORT16( msg->extended_ephemeris[i].Crc,      ptr );
               SIRFBINARY_EXPORT32( msg->extended_ephemeris[i].w,        ptr );
               SIRFBINARY_EXPORT32( msg->extended_ephemeris[i].omegaDot, ptr );
               SIRFBINARY_EXPORT16( msg->extended_ephemeris[i].iDot,     ptr );
               SIRFBINARY_EXPORT16( msg->extended_ephemeris[i].toc,      ptr );
               SIRFBINARY_EXPORT8 ( msg->extended_ephemeris[i].Tgd,      ptr );
               SIRFBINARY_EXPORT8 ( msg->extended_ephemeris[i].af2,      ptr );
               SIRFBINARY_EXPORT16( msg->extended_ephemeris[i].af1,      ptr );
               SIRFBINARY_EXPORT32( msg->extended_ephemeris[i].af0,      ptr );
               SIRFBINARY_EXPORT8 ( msg->extended_ephemeris[i].age,      ptr );
            }
            SIRFBINARY_EXPORT8 ( msg->extended_iono.alpha[0], ptr );
            SIRFBINARY_EXPORT8 ( msg->extended_iono.alpha[1], ptr );
            SIRFBINARY_EXPORT8 ( msg->extended_iono.alpha[2], ptr );
            SIRFBINARY_EXPORT8 ( msg->extended_iono.alpha[3], ptr );
            SIRFBINARY_EXPORT8 ( msg->extended_iono.beta[0],  ptr );
            SIRFBINARY_EXPORT8 ( msg->extended_iono.beta[1],  ptr );
            SIRFBINARY_EXPORT8 ( msg->extended_iono.beta[2],  ptr );
            SIRFBINARY_EXPORT8 ( msg->extended_iono.beta[3],  ptr );
         }
      }
      break;

      case SIRF_MSG_SSB_EE_POLL_STATE:    /* 0x02 0xE8 */
      break;

      case SIRF_MSG_SSB_EE_FILE_DOWNLOAD:
      {
         tSIRF_MSG_SSB_EE_FILE_DOWNLOAD *msg = (tSIRF_MSG_SSB_EE_FILE_DOWNLOAD*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32 (msg->reserved,             ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_SIF_SET_CONFIG:     /* 0xFC 0xE8*/
      {
         tSIRF_MSG_SSB_SIF_SET_CONFIG *msg = (tSIRF_MSG_SSB_SIF_SET_CONFIG*) message_structure;

         if ( *packet_length < ( 5 * sizeof(tSIRF_UINT8) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8 (msg->operation_mode,        ptr);
            SIRFBINARY_EXPORT8 (msg->file_format,           ptr);
            SIRFBINARY_EXPORT8 (msg->ext_gps_time_src,      ptr);
            SIRFBINARY_EXPORT8 (msg->cgee_input_method,     ptr);
            SIRFBINARY_EXPORT8 (msg->sgee_input_method,     ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_EE_FILE_PART:     /* 0x12 0xE8*/
      {
         tSIRF_MSG_SSB_EE_FILE_PART *msg = (tSIRF_MSG_SSB_EE_FILE_PART*) message_structure;

         if ( (*packet_length < ( 4 * sizeof(tSIRF_UINT8) +header_len) )
                    || (msg->buffSize > SSB_DLD_MAX_PKT_LEN))
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
             SIRFBINARY_EXPORT32 (msg->buffSize,ptr);
             if ( *packet_length < ( (4 + msg->buffSize) * sizeof(tSIRF_UINT8) +
                                     header_len) )
             {
                tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
             }
             else
             {
                for(i = 0; i < msg->buffSize; i++)
                {
                    SIRFBINARY_EXPORT8 (msg->buff[i], ptr);
                }
             }
         }
      }
      break;

      case SIRF_MSG_SSB_EE_DISABLE_EE_SECS: /* 0xFE 0xE8 */
      {
         tSIRF_MSG_SSB_EE_DISABLE_EE_SECS *msg = (tSIRF_MSG_SSB_EE_DISABLE_EE_SECS*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32( msg->num_secs, ptr );
         }
      }
      break;

#ifdef EMB_SIF
      case SIRF_MSG_SSB_SIF_AIDING_STATUS:
      {
         tSIRF_MSG_SSB_SIF_AIDING_STATUS *msg = (tSIRF_MSG_SSB_SIF_AIDING_STATUS*) message_structure;

         if ( *packet_length < (2 * sizeof(tSIRF_UINT8)+ 2 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8( msg->sgeeDisable, ptr );
            SIRFBINARY_EXPORT8( msg->cgeeDisable, ptr );
            SIRFBINARY_EXPORT32( msg->CGEEDisableSeconds, ptr );
            SIRFBINARY_EXPORT32( msg->CurrentReceiverSeconds, ptr );
         }
      }
      break;
#endif /*EMB_SIF*/

      case SIRF_MSG_SSB_EE_DEBUG:         /* 0xFF 0xE8 */
      {
         tSIRF_MSG_SSB_EE_PROPRIETARY_DEBUG *msg = (tSIRF_MSG_SSB_EE_PROPRIETARY_DEBUG*) message_structure;

         if ( *packet_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32( msg->debug_flags, ptr );
         }
      }
      break;

      case SIRF_MSG_SSB_TEXT:         /* 0xFF */
      {
         if ( *packet_length < ( (message_length * sizeof(tSIRF_UINT8)) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            memcpy(ptr, message_structure, message_length);
            ptr += message_length;
         }
      }
      break;
      case SIRF_MSG_SSB_TRKR_DBG:         /* 0x44 */
      {
         if ( *packet_length < ( (message_length * sizeof(tSIRF_UINT8)) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            memcpy(ptr, message_structure, message_length);
            ptr += message_length;
         }
      }
      break;
      case SIRF_MSG_SSB_SENSOR_CONFIG:    /* 0xEA 0x1 */
      {
         tSIRF_MSG_SSB_SENSOR_CONFIG *msg = (tSIRF_MSG_SSB_SENSOR_CONFIG*) message_structure;

         tSIRF_UINT32 required_length = (sizeof(tSIRF_UINT8) *
                      ( 3 + msg->numSensors * (14 + 2* (SIRF_MSG_SSB_MAX_SENSOR_INIT_REGS  +
                                                        SIRF_MSG_SSB_MAX_SENSOR_READ_REGS  +
                                                        SIRF_MSG_SSB_MAX_SENSOR_CTRL_REGS  )
                                               )
                       ) + sizeof(tSIRF_UINT16)* ( 3* msg->numSensors) +  header_len);

         if ( *packet_length < required_length )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else if (msg->numSensors > SIRF_MSG_SSB_MAX_NUM_SENSORS)
         {
            tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->numSensors,ptr);
            SIRFBINARY_EXPORT8(msg->i2cSpeed,ptr);
            for(i = 0; (i < msg->numSensors) && (i < SIRF_MSG_SSB_MAX_NUM_SENSORS); i++)
            {
               SIRFBINARY_EXPORT16(msg->Sensors[i].i2cAddress,ptr);
               SIRFBINARY_EXPORT8(msg->Sensors[i].sensorType,ptr);
               SIRFBINARY_EXPORT8(msg->Sensors[i].initTime,ptr);
               SIRFBINARY_EXPORT8(msg->Sensors[i].nBytesResol,ptr);
               SIRFBINARY_EXPORT8(msg->Sensors[i].sampRate,ptr);
               SIRFBINARY_EXPORT8(msg->Sensors[i].sendRate,ptr);
               SIRFBINARY_EXPORT8(msg->Sensors[i].decmMethod,ptr);
               SIRFBINARY_EXPORT8(msg->Sensors[i].acqTime,ptr);
               SIRFBINARY_EXPORT8(msg->Sensors[i].numReadReg,ptr);
               SIRFBINARY_EXPORT8(msg->Sensors[i].measState,ptr);
               for(j=0;j<msg->Sensors[i].numReadReg;j++)
               {
                  SIRFBINARY_EXPORT8(msg->Sensors[i].sensorReadReg[j].readOprMethod,ptr);
                  SIRFBINARY_EXPORT8(msg->Sensors[i].sensorReadReg[j].dataReadReg,ptr);
               }
               SIRFBINARY_EXPORT8(msg->Sensors[i].pwrCtrlReg,ptr);
               SIRFBINARY_EXPORT8(msg->Sensors[i].pwrOffSetting,ptr);
               SIRFBINARY_EXPORT8(msg->Sensors[i].pwrOnSetting,ptr);
               SIRFBINARY_EXPORT8(msg->Sensors[i].numInitReadReg,ptr);
               for(j=0;j<msg->Sensors[i].numInitReadReg;j++)
               {
                  SIRFBINARY_EXPORT8(msg->Sensors[i].sensorInitReg[j].address,ptr);
                  SIRFBINARY_EXPORT8(msg->Sensors[i].sensorInitReg[j].nBytes,ptr);
               }
               SIRFBINARY_EXPORT8(msg->Sensors[i].numCtrlReg,ptr);
               SIRFBINARY_EXPORT8(msg->Sensors[i].ctrlRegWriteDelay,ptr);
               for(j=0;j<msg->Sensors[i].numCtrlReg;j++)
               {
                  SIRFBINARY_EXPORT8(msg->Sensors[i].sensorCtrlReg[j].address,ptr);
                  SIRFBINARY_EXPORT8(msg->Sensors[i].sensorCtrlReg[j].value,ptr);
               }
            } /* for loop = numSensors */

            SIRFBINARY_EXPORT8(msg->processingRate,ptr);

            for(i=0;i<msg->numSensors;i++)
            {
               SIRFBINARY_EXPORT16(msg->sensorScaleZeroPointVal[i].zeroPointVal,ptr);
               SIRFBINARY_EXPORT16(msg->sensorScaleZeroPointVal[i].scaleFactor,ptr);
            }
         }
      }
      break;

      case SIRF_MSG_SSB_SENSOR_SWITCH:    /* 0xEA 0x2 */
      {
         tSIRF_MSG_SSB_SENSOR_SWITCH *msg = (tSIRF_MSG_SSB_SENSOR_SWITCH*) message_structure;

         if ( *packet_length  < (sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->sensorSetState,ptr);
         }
      }
      break;

      case  SIRF_MSG_SSB_SENSOR_READINGS:             /*0x48, 0x1*/
      {
         tSIRF_MSG_SSB_SENSOR_READINGS *msg = (tSIRF_MSG_SSB_SENSOR_READINGS*) message_structure;

         tSIRF_UINT32 required_length = ((3 + (SIRF_MSG_SSB_MAX_SENSOR_DATA_LEN * msg->numDataSet)) *
                                         sizeof(tSIRF_UINT8) + sizeof(tSIRF_UINT16) +
                                         msg->numDataSet * sizeof(tSIRF_UINT32) + header_len);

         if ( *packet_length < required_length  )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else if (msg->numDataSet > SIRF_MSG_SSB_MAX_SENSOR_DATA_SETS)
         {
            tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->sensorID,ptr);
            SIRFBINARY_EXPORT8(msg->dataLength,ptr);
            SIRFBINARY_EXPORT8(msg->numDataSet,ptr);
            SIRFBINARY_EXPORT8(msg->dataMode,ptr);
            for(i=0;i<msg->numDataSet;i++)
            {
               SIRFBINARY_EXPORT32(msg->dataSet[i].timeTag,ptr);
               memcpy(ptr,msg->dataSet[i].data,msg->dataLength);
               ptr += msg->dataLength;
            }
         }
      }
      break;

      case SIRF_MSG_SSB_SENSOR_FACTORY_STORED_PARAMS: /* 0x48, 0x2 */
      {
         tSIRF_MSG_SSB_SENSOR_FACTORY_STORED_PARAMS *msg
                                = (tSIRF_MSG_SSB_SENSOR_FACTORY_STORED_PARAMS*) message_structure;

         tSIRF_UINT32 required_length = (sizeof (tSIRF_UINT8) * (1 + ( ( 1 + SIRF_MSG_SSB_MAX_SENSOR_INIT_REG_DATA_LEN) * msg->numInitReadReg)) +
                                        sizeof(tSIRF_UINT16) +
                                        header_len);

        if ( *packet_length < required_length )
        {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
        }
        else if (msg->numInitReadReg > SIRF_MSG_SSB_MAX_SENSOR_INIT_REGS)
        {
           tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
        }
         else
         {
            SIRFBINARY_EXPORT16(msg->sensorID,ptr);
            SIRFBINARY_EXPORT8(msg->numInitReadReg,ptr);

            for(i=0;i<msg->numInitReadReg;i++)
            {
                SIRFBINARY_EXPORT8(msg->initData[i].nBytes,ptr);
                memcpy(ptr, msg->initData[i].data,msg->initData[i].nBytes);
                ptr +=  msg->initData[i].nBytes;
            } /* for */
         }        /* else */
       } /* case */
      break;

      case SIRF_MSG_SSB_RCVR_STATE:    /* 0x48 0x03 */
      {
         tSIRF_MSG_SBB_RCVR_STATE *msg = (tSIRF_MSG_SBB_RCVR_STATE*) message_structure;

         if ( *packet_length  < (sizeof(tSIRF_UINT32) + sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->timeStamp, ptr);
            SIRFBINARY_EXPORT8(msg->rcvrPhysicalState, ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_POINT_N_TELL_OUTPUT:    /* 0x48 0x04 */
      {
         tSIRF_MSG_SSB_POINT_N_TELL_OUTPUT *msg = (tSIRF_MSG_SSB_POINT_N_TELL_OUTPUT*) message_structure;

         if ( *packet_length  < ((3*sizeof(tSIRF_UINT32)) + (6*sizeof(tSIRF_UINT16)) + sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT32(msg->timeStamp,ptr);
            SIRFBINARY_EXPORT32(msg->latitude,ptr);
            SIRFBINARY_EXPORT32(msg->longitude,ptr);
            SIRFBINARY_EXPORT16(msg->heading,ptr);
            SIRFBINARY_EXPORT16(msg->pitch,ptr);
            SIRFBINARY_EXPORT16(msg->roll,ptr);
            SIRFBINARY_EXPORT16(msg->headingUnc,ptr);
            SIRFBINARY_EXPORT16(msg->pitchUnc,ptr);
            SIRFBINARY_EXPORT16(msg->rollUnc,ptr);
            SIRFBINARY_EXPORT8(msg->status, ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_SENSOR_CALIBRATION_OUTPUT:    /* 0x48 0x05 */
      {
         tSIRF_MSG_SSB_SENSOR_CALIBRATION_OUTPUT *msg = (tSIRF_MSG_SSB_SENSOR_CALIBRATION_OUTPUT*) message_structure;
         tSIRF_UINT32 i=0;

         if ( *packet_length  < ((40*sizeof(tSIRF_UINT32)) + (6*sizeof(tSIRF_DOUBLE)) + sizeof(tSIRF_UINT8) + sizeof(tSIRF_UINT16) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT16(msg->msgDescriptor,ptr);
            SIRFBINARY_EXPORT8(msg->sensorType,ptr);
            SIRFBINARY_EXPORT32(msg->timeStamp,ptr);
            for(i =0;i<6;i++)
            {
               SIRF_SWAPOUT_DOUBLE(msg->CAL_FLD8[i],ptr);
            }

            for(i =0;i<24;i++)
            {
               SIRFBINARY_EXPORT32(msg->CAL_FLD4[i],ptr);
            }
         }
      }
      break;

      case SIRF_MSG_SSB_MAG_MODEL_PARAMS:    /* 0x48 0x06 */
      {
         tSIRF_MSG_SSB_MAG_MODEL_PARAMS *msg = (tSIRF_MSG_SSB_MAG_MODEL_PARAMS*) message_structure;

         if ( *packet_length  < ((3*sizeof(tSIRF_INT32)) + (7*sizeof(tSIRF_FLOAT)) + sizeof(tSIRF_UINT16) + (3*sizeof(tSIRF_UINT8)) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            SIRFBINARY_EXPORT8(msg->magModelParamSrc,ptr);
            SIRFBINARY_EXPORT32(msg->latitude,ptr);
            SIRFBINARY_EXPORT32(msg->longitude,ptr);
            SIRFBINARY_EXPORT32(msg->altitude,ptr);
            SIRFBINARY_EXPORT16(msg->year,ptr);
            SIRFBINARY_EXPORT8(msg->month,ptr);
            SIRFBINARY_EXPORT8(msg->day,ptr);
            SIRFBINARY_EXPORT32(msg->declination,ptr);
            SIRFBINARY_EXPORT32(msg->totalField,ptr);
            SIRFBINARY_EXPORT32(msg->horizonField,ptr);
            SIRFBINARY_EXPORT32(msg->inclination,ptr);
            SIRFBINARY_EXPORT32(msg->northComp,ptr);
            SIRFBINARY_EXPORT32(msg->eastComp,ptr);
            SIRFBINARY_EXPORT32(msg->downComp,ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_SIRF_STATS:    /* 0xE1 0x06 */
         {
            tSIRF_MSG_SSB_SIRF_STATS *msg = (tSIRF_MSG_SSB_SIRF_STATS *) message_structure;

            if ( *packet_length < ( 4 * sizeof(tSIRF_UINT32) +
                                    6 * sizeof(tSIRF_UINT16) +
                                    9 * sizeof(tSIRF_UINT8) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT16(msg->ttff_since_reset,       ptr);
               SIRFBINARY_EXPORT16(msg->ttff_since_aiding,      ptr);
               SIRFBINARY_EXPORT16(msg->ttff_first_nav,         ptr);
               SIRFBINARY_EXPORT32(msg->pos_aiding_error_north, ptr);
               SIRFBINARY_EXPORT32(msg->pos_aiding_error_east,  ptr);
               SIRFBINARY_EXPORT32(msg->pos_aiding_error_down,  ptr);
               SIRFBINARY_EXPORT32(msg->time_aiding_error,      ptr);
               SIRFBINARY_EXPORT16(msg->freq_aiding_error,      ptr);
               SIRFBINARY_EXPORT8(msg->hor_pos_uncertainty,     ptr);
               SIRFBINARY_EXPORT16(msg->ver_pos_uncertainty,    ptr);
               SIRFBINARY_EXPORT8(msg->time_uncertainty,        ptr);
               SIRFBINARY_EXPORT8(msg->freq_uncertainty,        ptr);
               SIRFBINARY_EXPORT8(msg->num_aided_ephemeris,     ptr);
               SIRFBINARY_EXPORT8(msg->num_aided_acq_assist,    ptr);
               SIRFBINARY_EXPORT8(msg->nav_mode,                ptr);
               SIRFBINARY_EXPORT8(msg->pos_mode,                ptr);
               SIRFBINARY_EXPORT16(msg->nav_status,             ptr);
               SIRFBINARY_EXPORT8(msg->start_mode,              ptr);
               SIRFBINARY_EXPORT8(msg->aiding_status,           ptr);
            }
         }
         break;

      case SIRF_MSG_SSB_SIRF_INTERNAL_OUT:    /* 0xE1 0x00 */
         { 
            /* Because the subID for this message is 0, it is not being handled/exported at the top this 
             * function.  It is being exported here instead. */
            if ( *packet_length < ( (message_length * sizeof(tSIRF_UINT8)) + header_len + (1 * sizeof(tSIRF_UINT8))) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               /* Export the SubID */
               SIRFBINARY_EXPORT8(0x00, ptr);
               /* Export the remaining message content */
               memcpy(ptr, message_structure, message_length);
               ptr += message_length;
            }
         }
         break;

#ifdef SIRF_AGPS
      default:
         {
            tRet = SIRF_CODEC_SSB_AGPS_Encode(message_id,message_structure,message_length,
                                              packet,packet_length);
            if(SIRF_SUCCESS == tRet)
               {
                  ptr = packet + (*packet_length);
               }
         }
#else
      default:
         /* Pass-thru messages 0x61 to 0x7F */
         if (message_id >= SIRF_MSG_SSB_PASSTHRU_OUTPUT_BEGIN && message_id <= SIRF_MSG_SSB_PASSTHRU_OUTPUT_END)
         {
            if ( *packet_length < ( (message_length * sizeof(tSIRF_UINT8)) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               memcpy(ptr, message_structure, message_length);
               ptr += message_length;
            }
         }
         else
         {
            tRet = SIRF_CODEC_SSB_INVALID_MSG_ID;
         }
#endif
         break;

      } /* switch */

      if (SIRF_SUCCESS == tRet)
      {
         *packet_length = (tSIRF_UINT32)(ptr - packet);
         if ( *packet_length > SIRF_MSG_SSB_MAX_MESSAGE_LEN )
         {
            tRet = SIRF_CODEC_SSB_LENGTH_ERROR;
         }
      }
   } /* end of process message */


   if ( tRet != SIRF_SUCCESS )
   {
      *packet_length = 0;
   }

   *options = SIRF_CODEC_OPTIONS_MAKE_MSG_NUMBER(1,1); /* Message 1 of 1 */

   return tRet;

} /* SIRF_CODEC_SSB_Encode() */



/***************************************************************************
 * @brief:   Convert a byte stream to a formatted SSB structure
 * @param[in]:  payload - contains the byte stream, in protocol format
 * @param[in]   payload_length - the size of same, including overhead
 * @param[out]  message_id - SSB message id, derived from the input
 * @param[out]  message_structure - data structure keyed to the message id
 * @param[out]  message_length - pointer to the length of the message
 * @return:  SIRF_SUCCESS, SIRF_CODEC_SSB_NULL_POINTER, or
 *           SIRF_CODEC_SSB_LENGTH_ERROR
***************************************************************************/
tSIRF_RESULT SIRF_CODEC_SSB_Decode( tSIRF_UINT8 *payload,
                                    tSIRF_UINT32 payload_length,
                                    tSIRF_UINT32 *message_id,
                                    tSIRF_VOID   *message_structure,
                                    tSIRF_UINT32 *message_length,
                                    tSIRF_UINT32 *options )
{
   tSIRF_RESULT tRet = SIRF_SUCCESS;

   if ( ((tSIRF_UINT8 *) NULL !=  payload) &&
        ((tSIRF_UINT32 *) NULL != message_id) &&
        ((tSIRF_VOID *) NULL != message_structure) &&
        ((tSIRF_UINT32 *) NULL != message_length) )
   {
      tSIRF_UINT8 *ptr = payload;
      tSIRF_UINT32 i, j;
      tSIRF_INT32 k;
      tSIRF_UINT8 mid, sid = 0;
      tSIRF_UINT8 header_len = 1 * sizeof(tSIRF_UINT8); /* At least the mid byte */

      /* At a minimum, make sure we have room for the first header byte */
      if ( payload_length < header_len )
      {
         return SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
      }

      mid = SIRFBINARY_IMPORT_UINT8(ptr);
      *message_id     = SIRF_MAKE_MSG_ID(SIRF_LC_SSB,mid,sid);
      *message_length = 0;

      /*   add the sub-id to the message id */
      if (
         SIRF_MSG_SSB_SIRFNAV_COMMAND      == *message_id ||
         SIRF_MSG_SSB_ADVANCED_NAV_INIT    == *message_id ||
         SIRF_MSG_SSB_SW_CONTROL           == *message_id ||
         SIRF_MSG_SSB_EE_INPUT             == *message_id ||
         SIRF_MSG_SSB_DR_OUTPUT            == *message_id ||
         SIRF_MSG_SSB_SIRFNAV_NOTIFICATION == *message_id ||
         SIRF_MSG_SSB_EVENT                == *message_id ||
         SIRF_MSG_SSB_IC_CONTROL           == *message_id ||
         SIRF_MSG_SSB_CW_INPUT             == *message_id ||
         SIRF_MSG_SSB_XO_LEARNING_IN       == *message_id ||
         SIRF_MSG_SSB_EE                   == *message_id ||
#ifdef SENS_SSB_DATA_INPUT_MODE
         SIRF_MSG_SSB_RCVR_STATE           == *message_id ||
         SIRF_MSG_SSB_POINT_N_TELL_OUTPUT  == *message_id ||
         SIRF_MSG_SSB_SENSOR_READINGS      == *message_id ||
#endif
         SIRF_MSG_SSB_SENSOR_DATA          == *message_id ||
         SIRF_MSG_SSB_SENSOR_CONTROL_INPUT == *message_id ||
         SIRF_MSG_SSB_EE_INPUT             == *message_id ||
         SIRF_MSG_SSB_IC_DATA              == *message_id ||
         SIRF_MSG_SSB_QUEUE_CMD_PARAM      == *message_id ||
         SIRF_MSG_SSB_TEST_MODE_DATA_ID    == *message_id ||
         SIRF_MSG_SSB_NL_AUX_DATA          == *message_id ||
         SIRF_MSG_SSB_SIRF_INTERNAL_OUT    == *message_id ||
         SIRF_MSG_SSB_MPM_STATUS_OUT       == *message_id ||
         SIRF_MSG_SSB_PWR_MODE_REQ         == *message_id
         )
      {
         header_len += 1 * sizeof(tSIRF_UINT8);
         if ( payload_length < header_len )
         {
            return SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         sid = SIRFBINARY_IMPORT_UINT8(ptr);
      }

      *message_id = SIRF_MAKE_MSG_ID(SIRF_LC_SSB,mid,sid);
      /* be aware that this is now a 16-bit value */

      switch (*message_id)
      {
         case SIRF_MSG_SSB_MEASURED_NAVIGATION: /* 0x02 */
         {   /* coercive cast the output to the format we want */
            tSIRF_MSG_SSB_MEASURED_NAVIGATION * msg = (tSIRF_MSG_SSB_MEASURED_NAVIGATION*) message_structure;

            if ( payload_length < ( (4 + SIRF_NUM_CHANNELS) * sizeof(tSIRF_UINT8) +
                                     4 * sizeof(tSIRF_UINT16) +
                                     4 * sizeof(tSIRF_UINT32) +
                                     header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->ecef_x         = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->ecef_y         = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->ecef_z         = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->ecef_vel_x     = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->ecef_vel_y     = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->ecef_vel_z     = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->nav_mode       = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->hdop           = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->nav_mode2      = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->gps_week       = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_tow        = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->sv_used_cnt    = SIRFBINARY_IMPORT_UINT8 (ptr);
               for (i = 0; i < SIRF_NUM_CHANNELS; i++)
               {
                  msg->sv_used[i]  = SIRFBINARY_IMPORT_UINT8 (ptr);
               }
            }
         }
         break;
         case SIRF_MSG_SSB_MEASURED_TRACKER: /* 0x04 */
         {
            tSIRF_MSG_SSB_MEASURED_TRACKER * msg = (tSIRF_MSG_SSB_MEASURED_TRACKER*) message_structure;

            if ( payload_length < ( (1 + 3 * SIRF_NUM_CHANNELS + SIRF_NUM_CHANNELS * SIRF_NUM_POINTS) *
                                                                                     sizeof(tSIRF_UINT8) +
                                    (1 + SIRF_NUM_CHANNELS) * sizeof(tSIRF_UINT16) +
                                     1 * sizeof(tSIRF_UINT32) +
                                     header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->gps_week             = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_tow              = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->chnl_cnt             = SIRFBINARY_IMPORT_UINT8( ptr);
               for (i = 0; i < SIRF_NUM_CHANNELS; i++)
               {
                  msg->chnl[i].svid      = SIRFBINARY_IMPORT_UINT8( ptr);
                  msg->chnl[i].azimuth   = SIRFBINARY_IMPORT_UINT8( ptr);
                  msg->chnl[i].elevation = SIRFBINARY_IMPORT_UINT8( ptr);
                  msg->chnl[i].state     = SIRFBINARY_IMPORT_UINT16(ptr);
                  for(j = 0; j < SIRF_NUM_POINTS; j++)
                  {
                     msg->chnl[i].cno[j] = SIRFBINARY_IMPORT_UINT8( ptr);
                  }
               }
            }
         }
         break;
         case SIRF_MSG_SSB_RAW_TRACKER: /* 0x05 */
         {
            tSIRF_MSG_SSB_RAW_TRACKER * msg = (tSIRF_MSG_SSB_RAW_TRACKER*) message_structure;

            if ( payload_length < ( (2 + SIRF_NUM_POINTS) * sizeof(tSIRF_UINT8) +
                                     7 * sizeof(tSIRF_INT16) +
                                     6 * sizeof(tSIRF_INT32) +
                                     header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->channel              = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->svid                 = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->state                = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->bit_number           = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->msec_number          = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->chip_number          = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->code_phase           = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->carrier_doppler      = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->measure_timetag      = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->delta_carrier_phase  = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->search_cnt           = SIRFBINARY_IMPORT_SINT16(ptr);
               for (i = 0; i < SIRF_NUM_POINTS; i++)
               {
                  msg->cno[i]            = SIRFBINARY_IMPORT_UINT8 (ptr);
               }
               msg->power_bad_count      = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->phase_bad_count      = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->delta_car_interval   = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->correl_interval      = SIRFBINARY_IMPORT_SINT16(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_CLOCK_STATUS: /* 0x07 */
         {
            tSIRF_MSG_SSB_CLOCK_STATUS * msg = (tSIRF_MSG_SSB_CLOCK_STATUS*) message_structure;

            if ( payload_length < ( 1 * sizeof(tSIRF_UINT8) +
                                    1 * sizeof(tSIRF_UINT16) +
                                    4 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->gps_week      = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_tow       = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->sv_used_cnt   = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->clk_offset    = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->clk_bias      = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->est_gps_time  = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_50BPS_DATA: /* 0x08 */
         case SIRF_MSG_SSB_EE_50BPS_DATA_VERIFIED: /* 0x38, 0x05 */
         {
            tSIRF_MSG_SSB_50BPS_DATA * msg = (tSIRF_MSG_SSB_50BPS_DATA*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT8) +
                                    10 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->chnl       = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->svid       = SIRFBINARY_IMPORT_UINT8 (ptr);
               for (i = 0; i <10; i++)
               {
                  msg->word[i] = SIRFBINARY_IMPORT_UINT32(ptr);
               }
            }
         }
         break;
         case SIRF_MSG_SSB_THROUGHPUT: /* 0x09 */
         {
            tSIRF_MSG_SSB_THROUGHPUT * msg = (tSIRF_MSG_SSB_THROUGHPUT*) message_structure;

            if ( payload_length < (  4 * sizeof(tSIRF_UINT16) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->seg_stat_max = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->seg_stat_lat = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->avg_trk_time = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->last_ms      = SIRFBINARY_IMPORT_UINT16(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_ERROR: /* 0x0A */
         {
            tSIRF_MSG_SSB_ERROR * msg = (tSIRF_MSG_SSB_ERROR*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT16) +
                                    SIRF_MSG_SSB_MAX_ERROR_PARAMS * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->err_id     = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->param_cnt  = SIRFBINARY_IMPORT_UINT16(ptr);
               if (msg->param_cnt > SIRF_MSG_SSB_MAX_ERROR_PARAMS)
               {
                  msg->param_cnt = SIRF_MSG_SSB_MAX_ERROR_PARAMS;
               }
               for (i = 0; i < SIRF_MSG_SSB_MAX_ERROR_PARAMS; i++)
               {
                  msg->param[i] = SIRFBINARY_IMPORT_UINT32(ptr);
               }
            }
         }
         break;
         case SIRF_MSG_SSB_ACK: /* 0x0B */
         {
            tSIRF_MSG_SSB_ACK * msg = (tSIRF_MSG_SSB_ACK*) message_structure;

            if ( payload_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->msg_id = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->sub_id = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_NAK: /* 0x0C */
         {
            tSIRF_MSG_SSB_NAK * msg = (tSIRF_MSG_SSB_NAK*) message_structure;

            if ( payload_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->msg_id = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->sub_id = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_VISIBILITY_LIST: /* 0x0D */
         {
            tSIRF_MSG_SSB_VISIBILITY_LIST * msg = (tSIRF_MSG_SSB_VISIBILITY_LIST*) message_structure;

            if ( payload_length < ( (1 + msg->svid_cnt) * sizeof(tSIRF_UINT8) +
                                    (2 * msg->svid_cnt) * sizeof(tSIRF_UINT16) +
                                     header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->svid_cnt                = SIRFBINARY_IMPORT_UINT8 (ptr);
               if (msg->svid_cnt > SIRF_MAX_SVID_CNT)
               {
                  msg->svid_cnt = SIRF_MAX_SVID_CNT;
               }
               for (i = 0; i < msg->svid_cnt; i++)
               {
                  msg->visible[i].svid      = SIRFBINARY_IMPORT_UINT8 (ptr);
                  msg->visible[i].azimuth   = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->visible[i].elevation = SIRFBINARY_IMPORT_SINT16(ptr);
               }
            }
         }
         break;
         case SIRF_MSG_SSB_ALMANAC: /* 0x0E */
         {
            tSIRF_MSG_SSB_ALMANAC * msg = (tSIRF_MSG_SSB_ALMANAC*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) +
                                  (2 + SIRF_MSG_SSB_ALMANAC_ENTRIES) * sizeof(tSIRF_UINT16) +
                                   header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->svid            = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->week_and_status = SIRFBINARY_IMPORT_UINT16 (ptr);
               for (i = 0; i < SIRF_MSG_SSB_ALMANAC_ENTRIES; i++)
               {
                  msg->data[i] = SIRFBINARY_IMPORT_UINT16(ptr);
               }
               msg->almanac_checksum = SIRFBINARY_IMPORT_UINT16 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_EPHEMERIS: /* 0x0F */
         {
            tSIRF_MSG_SSB_EPHEMERIS * msg = (tSIRF_MSG_SSB_EPHEMERIS*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) +
                                   3 * 15 * sizeof(tSIRF_UINT16) +
                                   header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->svid                 = SIRFBINARY_IMPORT_UINT8 (ptr);
               for (i = 0; i < 3; i++)
               {
                  for (j = 0; j < 15; j++)
                  {
                     msg->subframe[i][j] = SIRFBINARY_IMPORT_UINT16(ptr);
                  }
               }
            }
         }
         break;
         case SIRF_MSG_SSB_OK_TO_SEND: /* 0x12 */
         {
            tSIRF_MSG_SSB_OK_TO_SEND * msg = (tSIRF_MSG_SSB_OK_TO_SEND*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->input_enabled = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_RECEIVER_PARAMS: /* 0x13 */
         {
            tSIRF_MSG_SSB_RECEIVER_PARAMS * msg = (tSIRF_MSG_SSB_RECEIVER_PARAMS*) message_structure;

            if ( payload_length < (19 * sizeof(tSIRF_UINT8) +
                                   4 * sizeof(tSIRF_UINT16) +
                                   9 * sizeof(tSIRF_UINT32) +
                                   header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->reserved_1a            = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->reserved_1b            = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->pos_mode_enable        = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->alt_mode               = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->alt_src                = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->alt_input              = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->degraded_mode          = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->degraded_timeout       = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->dr_timeout             = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->trk_smooth             = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->static_nav_mode        = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->enable_3sv_lsq         = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->reserved_2             = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->dop_mask_mode          = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->nav_elev_mask          = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->nav_pwr_mask           = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->reserved_3             = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->dgps_src               = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->dgps_mode              = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->dgps_timeout           = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->reserved_4             = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->lp_push_to_fix         = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->lp_on_time             = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->lp_interval            = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->lp_user_tasks_enabled  = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->lp_user_task_interval  = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->lp_pwr_cycling_enabled = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->lp_max_acq_time        = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->lp_max_off_time        = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->apm_enabled_power_duty_cycle      =SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->number_of_fixes                   =SIRFBINARY_IMPORT_UINT16(ptr);
               msg->time_between_fixes                =SIRFBINARY_IMPORT_UINT16(ptr);
               msg->horz_vert_error_max               =SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->response_time_max                 =SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->time_accu_time_duty_cycle_priority=SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_TEST_MODE_DATA: /* 0x14 */
         {
            tSIRF_MSG_SSB_TEST_MODE_DATA * msg = (tSIRF_MSG_SSB_TEST_MODE_DATA*) message_structure;

            if ( payload_length < (13 * sizeof(tSIRF_UINT16) +
                                   6 * sizeof(tSIRF_UINT32) +
                                   header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->svid =                SIRFBINARY_IMPORT_UINT16(ptr);
               msg->period =              SIRFBINARY_IMPORT_UINT16(ptr);
               msg->bit_synch_time =      SIRFBINARY_IMPORT_UINT16(ptr);
               msg->bit_count =           SIRFBINARY_IMPORT_UINT16(ptr);
               msg->poor_status_count =   SIRFBINARY_IMPORT_UINT16(ptr);
               msg->good_status_count =   SIRFBINARY_IMPORT_UINT16(ptr);
               msg->parity_error_count =  SIRFBINARY_IMPORT_UINT16(ptr);
               msg->lost_vco_count =      SIRFBINARY_IMPORT_UINT16(ptr);
               msg->frame_synch_time =    SIRFBINARY_IMPORT_UINT16(ptr);
               msg->cno_mean =            SIRFBINARY_IMPORT_SINT16(ptr);
               msg->cno_sigma =           SIRFBINARY_IMPORT_SINT16(ptr);
               msg->clock_drift =         SIRFBINARY_IMPORT_SINT16(ptr);
               msg->clock_offset_mean =   SIRFBINARY_IMPORT_SINT32(ptr);
               msg->bad_1khz_bit_count =  SIRFBINARY_IMPORT_SINT16(ptr);
               msg->abs_i20ms =           SIRFBINARY_IMPORT_SINT32(ptr);
               msg->abs_q1ms =            SIRFBINARY_IMPORT_SINT32(ptr);
               msg->reserved[0] =         SIRFBINARY_IMPORT_SINT32(ptr);
               msg->reserved[1] =         SIRFBINARY_IMPORT_SINT32(ptr);
               msg->reserved[2] =         SIRFBINARY_IMPORT_SINT32(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_DGPS_STATUS: /* 0x1B */
         {
            tSIRF_MSG_SSB_DGPS_STATUS * msg = (tSIRF_MSG_SSB_DGPS_STATUS*) message_structure;

            if ( payload_length < ( (3 + 2 * SIRF_NUM_CHANNELS) * sizeof(tSIRF_UINT8) +
                                     SIRF_NUM_CHANNELS * sizeof(tSIRF_UINT16) +
                                     header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->src = SIRFBINARY_IMPORT_UINT8 (ptr);
               for (i = 0; i < SIRF_NUM_CHANNELS; i++)
               {
                  msg->cor_age[i] = SIRFBINARY_IMPORT_UINT8 (ptr);
               }
               msg->reserved[0] = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->reserved[1] = SIRFBINARY_IMPORT_UINT8 (ptr);
               for (i = 0; i < SIRF_NUM_CHANNELS; i++)
               {
                  msg->corrections[i].prn = SIRFBINARY_IMPORT_UINT8 (ptr);
                  msg->corrections[i].cor = SIRFBINARY_IMPORT_UINT16(ptr);
               }
            }
         }
         break;
         case SIRF_MSG_SSB_NL_MEAS_DATA: /* 0x1C */
         {
            tSIRF_MSG_SSB_NL_MEAS_DATA * msg = (tSIRF_MSG_SSB_NL_MEAS_DATA*) message_structure;

            if ( payload_length < ( (5 + SIRF_NUM_POINTS) * sizeof(tSIRF_UINT8) +
                                     4 * sizeof(tSIRF_UINT16) +
                                     1 * sizeof(tSIRF_UINT32) +
                                     1 * sizeof(tSIRF_FLOAT) +
                                     3 * sizeof(tSIRF_DOUBLE) +
                                     header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->Chnl                  = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->Timetag               = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->svid                  = SIRFBINARY_IMPORT_UINT8 (ptr);
               SIRFBINARY_IMPORT_DOUBLE(msg->gps_sw_time,            ptr);
               SIRFBINARY_IMPORT_DOUBLE(msg->pseudorange,            ptr);
               SIRFBINARY_IMPORT_FLOAT (msg->carrier_freq,           ptr);
               SIRFBINARY_IMPORT_DOUBLE(msg->carrier_phase,          ptr);
               msg->time_in_track         = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->sync_flags            = SIRFBINARY_IMPORT_UINT8 (ptr);
               for (i = 0; i < SIRF_NUM_POINTS; i++)
               {
                  msg->cton[i]            = SIRFBINARY_IMPORT_UINT8 (ptr);
               }
               msg->delta_range_interval  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->mean_delta_range_time = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->extrapolation_time    = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->phase_error_count     = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->low_power_count       = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_NL_DGPS_DATA: /* 0x1D */
         {
            tSIRF_MSG_SSB_NL_DGPS_DATA * msg = (tSIRF_MSG_SSB_NL_DGPS_DATA*) message_structure;

            if ( payload_length < ( 1 * sizeof(tSIRF_UINT8) +
                                    2 * sizeof(tSIRF_UINT16) +
                                    5 * sizeof(tSIRF_FLOAT) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->svid           = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->iod            = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->source         = SIRFBINARY_IMPORT_UINT8 (ptr);
               SIRFBINARY_IMPORT_FLOAT (msg->pr_correction,   ptr);
               SIRFBINARY_IMPORT_FLOAT (msg->prr_correction,  ptr);
               SIRFBINARY_IMPORT_FLOAT (msg->correction_age,  ptr);
               SIRFBINARY_IMPORT_FLOAT (msg->prc_variance,    ptr);
               SIRFBINARY_IMPORT_FLOAT (msg->prrc_variance,   ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_NL_SV_STATE_DATA: /* 0x1E */
         {
            tSIRF_MSG_SSB_NL_SV_STATE_DATA * msg = (tSIRF_MSG_SSB_NL_SV_STATE_DATA*) message_structure;

            if ( payload_length < (  2 * sizeof(tSIRF_UINT8) +
                                     4 * sizeof(tSIRF_FLOAT) +
                                    (2 + 3 + 3) * sizeof(tSIRF_DOUBLE) +
                                     header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->svid       = SIRFBINARY_IMPORT_UINT8(ptr);
               SIRFBINARY_IMPORT_DOUBLE (msg->time,       ptr);
               for (i = 0; i < 3; i++)
               {
                  SIRFBINARY_IMPORT_DOUBLE(msg->pos[i],  ptr);
               }
               for (i = 0; i < 3; i++)
               {
                  SIRFBINARY_IMPORT_DOUBLE(msg->vel[i],  ptr);
               }
               SIRFBINARY_IMPORT_DOUBLE(msg->clk,        ptr);
               SIRFBINARY_IMPORT_FLOAT (msg->clf,        ptr);
               msg->eph        = SIRFBINARY_IMPORT_UINT8(ptr);
               SIRFBINARY_IMPORT_FLOAT (msg->posvar,     ptr);
               SIRFBINARY_IMPORT_FLOAT (msg->clkvar,     ptr);
               SIRFBINARY_IMPORT_FLOAT (msg->iono,       ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_NL_INIT_DATA: /* 0x1F */
         {
            tSIRF_MSG_SSB_NL_INIT_DATA * msg = (tSIRF_MSG_SSB_NL_INIT_DATA*) message_structure;

            if ( payload_length < ( 13 * sizeof(tSIRF_UINT8) +
                                    13 * sizeof(tSIRF_INT16) +
                                    1 * sizeof(tSIRF_FLOAT) +
                                    5 * sizeof(tSIRF_DOUBLE) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->enable_con_alt_mode        = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->alt_mode                   = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->alt_source                 = SIRFBINARY_IMPORT_UINT8 (ptr);
               SIRFBINARY_IMPORT_FLOAT (msg->altitude,                    ptr);
               msg->degraded_mode              = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->degraded_timeout           = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->dr_timeout                 = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->coast_timeout              = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->tracksmooth_mode           = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->dop_selection              = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->hdop_thresh                = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->gdop_thresh                = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->pdop_thresh                = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->dgps_selection             = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->dgps_timeout               = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->elev_nav_mask              = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->pow_nav_mask               = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->editing_residual_mode      = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->editing_residual_threshold = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->ssd_mode                   = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->ssd_threshold              = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->static_nav_mode            = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->static_nav_threshold       = SIRFBINARY_IMPORT_SINT16(ptr);
               SIRFBINARY_IMPORT_DOUBLE(msg->ecef_x,                      ptr);
               SIRFBINARY_IMPORT_DOUBLE(msg->ecef_y,                      ptr);
               SIRFBINARY_IMPORT_DOUBLE(msg->ecef_z,                      ptr);
               msg->position_init_source       = SIRFBINARY_IMPORT_UINT8 (ptr);
               SIRFBINARY_IMPORT_DOUBLE(msg->gps_time,                    ptr);
               msg->gps_week                   = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->time_init_source           = SIRFBINARY_IMPORT_UINT8 (ptr);
               SIRFBINARY_IMPORT_DOUBLE(msg->clk_offset,                  ptr);
               msg->clk_offset_init_source     = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_NL_AUX_INIT_DATA: /* 0x40 0x01 */
         {
            tSIRF_MSG_SSB_NL_AUX_INIT_DATA * msg = (tSIRF_MSG_SSB_NL_AUX_INIT_DATA*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT8)  +
                                    4 * sizeof(tSIRF_UINT16) +
                                    6 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->time_init_unc        = SIRFBINARY_IMPORT_UINT32 (ptr);
               msg->saved_pos_week       = SIRFBINARY_IMPORT_UINT16 (ptr);
               msg->saved_pos_tow        = SIRFBINARY_IMPORT_UINT32 (ptr);
               msg->saved_pos_ehe        = SIRFBINARY_IMPORT_UINT16 (ptr);
               msg->saved_pos_eve        = SIRFBINARY_IMPORT_UINT16 (ptr);
               msg->sw_version           = SIRFBINARY_IMPORT_UINT8  (ptr);
               msg->icd_version          = SIRFBINARY_IMPORT_UINT8  (ptr);
               msg->chip_version         = SIRFBINARY_IMPORT_UINT16 (ptr);
               msg->acq_clk_speed        = SIRFBINARY_IMPORT_UINT32 (ptr);
               msg->default_clock_offset = SIRFBINARY_IMPORT_UINT32 (ptr);
               msg->tracker_status       = SIRFBINARY_IMPORT_UINT32 (ptr);
               msg->reserved             = SIRFBINARY_IMPORT_UINT32 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_NL_AUX_MEAS_DATA: /* 0x40 0x02 */
         {
            tSIRF_MSG_SSB_NL_AUX_MEAS_DATA * msg = (tSIRF_MSG_SSB_NL_AUX_MEAS_DATA*) message_structure;

            if ( payload_length < (  5 * sizeof(tSIRF_UINT8)  +
                                     8 * sizeof(tSIRF_INT16)  +
                                     2 * sizeof(tSIRF_UINT16) +
                                     7 * sizeof(tSIRF_INT32)  +
                                     3 * sizeof(tSIRF_UINT32) +
                                     header_len) )

            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->sv_prn              = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->status              = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->extended_status     = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->bit_sync_qual       = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->time_tag            = SIRFBINARY_IMPORT_UINT32 (ptr);
               msg->code_phase          = SIRFBINARY_IMPORT_UINT32 (ptr);
               msg->carrier_phase       = SIRFBINARY_IMPORT_SINT32 (ptr);
               msg->carrier_freq        = SIRFBINARY_IMPORT_SINT32 (ptr);
               msg->carrier_accel       = SIRFBINARY_IMPORT_SINT16 (ptr);
               msg->ms_num              = SIRFBINARY_IMPORT_SINT16 (ptr);
               msg->bit_num             = SIRFBINARY_IMPORT_SINT32 (ptr);
               msg->code_correction     = SIRFBINARY_IMPORT_SINT32 (ptr);
               msg->smooth_code         = SIRFBINARY_IMPORT_SINT32 (ptr);
               msg->code_offset         = SIRFBINARY_IMPORT_SINT32 (ptr);
               msg->pseudorange_noise   = SIRFBINARY_IMPORT_SINT16 (ptr);
               msg->delta_range_qual    = SIRFBINARY_IMPORT_SINT16 (ptr);
               msg->phase_lock_qual     = SIRFBINARY_IMPORT_SINT16 (ptr);
               msg->ms_uncertainty      = SIRFBINARY_IMPORT_SINT16 (ptr);
               msg->sum_abs_I           = SIRFBINARY_IMPORT_UINT16 (ptr);
               msg->sum_abs_Q           = SIRFBINARY_IMPORT_UINT16 (ptr);
               msg->sv_bit_num          = SIRFBINARY_IMPORT_SINT32 (ptr);
               msg->mp_los_det_value    = SIRFBINARY_IMPORT_SINT16 (ptr);
               msg->mp_only_det_value   = SIRFBINARY_IMPORT_SINT16 (ptr);
               msg->recovery_status     = SIRFBINARY_IMPORT_UINT8  (ptr);
               msg->sw_time_uncertainty = SIRFBINARY_IMPORT_UINT32 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_NL_AUX_AID_DATA: /* 0x40 0x03 */
         {
            tSIRF_MSG_SSB_NL_AUX_AID_DATA * msg = (tSIRF_MSG_SSB_NL_AUX_AID_DATA*) message_structure;

            if ( payload_length < ( 3 * sizeof(tSIRF_INT32)  +
                                    2 * sizeof(tSIRF_UINT32) +
                                    1 * sizeof(tSIRF_UINT16) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->ecef_x        = SIRFBINARY_IMPORT_SINT32 (ptr);
               msg->ecef_y        = SIRFBINARY_IMPORT_SINT32 (ptr);
               msg->ecef_z        = SIRFBINARY_IMPORT_SINT32 (ptr);
               msg->horiz_pos_unc = SIRFBINARY_IMPORT_UINT32 (ptr);
               msg->alt_unc       = SIRFBINARY_IMPORT_UINT16 (ptr);
               msg->sw_tow        = SIRFBINARY_IMPORT_UINT32 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_GEODETIC_NAVIGATION: /* 0x29 */
         {
            tSIRF_MSG_SSB_GEODETIC_NAVIGATION* msg = (tSIRF_MSG_SSB_GEODETIC_NAVIGATION*) message_structure;

            if ( payload_length < ( 8 * sizeof(tSIRF_UINT8) +
                                    13 * sizeof(tSIRF_UINT16) +
                                    14 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->nav_valid                 = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->nav_mode                  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_week                  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_tow                   = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->utc_year                  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->utc_month                 = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->utc_day                   = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->utc_hour                  = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->utc_min                   = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->utc_sec                   = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->sv_used                   = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->lat                       = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->lon                       = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->alt_ellips                = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->alt_msl                   = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->datum                     = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->sog                       = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->hdg                       = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->mag_var                   = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->climb_rate                = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->heading_rate              = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->ehpe                      = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->evpe                      = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->ete                       = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->ehve                      = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->clk_bias                  = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->clk_bias_error            = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->clk_offset                = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->clk_offset_error          = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->distance_travelled        = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->distance_travelled_error  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->heading_error             = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->sv_used_cnt               = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->hdop                      = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->additional_mode_info      = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_ADC_ODOMETER_DATA: /* 0x2D */
         {
            tSIRF_UINT8 len = header_len + 1 * sizeof(tSIRF_UINT8) + 3 * sizeof(tSIRF_UINT16) + 1 * sizeof(tSIRF_UINT32);
            tSIRF_UINT8 len_1hz = header_len +
                                  10 * (1 * sizeof(tSIRF_UINT8) + 3 * sizeof(tSIRF_UINT16) + 1 * sizeof(tSIRF_UINT32));

            if (len == payload_length)
            {
               tSIRF_MSG_SSB_ADC_ODOMETER_DATA * msg = (tSIRF_MSG_SSB_ADC_ODOMETER_DATA*) message_structure;

               *message_length = sizeof(*msg);
               msg->current_time = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->adc2_avg     = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->adc3_avg     = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->odo_count    = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gpio_stat    = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
            else if (len_1hz == payload_length)
            {
               tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ * msg = (tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ*) message_structure;

               *message_length = sizeof(*msg);
               for (i = 0; i < 10; i++)
               {
                  msg->dataset[i].current_time = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->dataset[i].adc2_avg     = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->dataset[i].adc3_avg     = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->dataset[i].odo_count    = SIRFBINARY_IMPORT_UINT16(ptr);
                  msg->dataset[i].gpio_stat    = SIRFBINARY_IMPORT_UINT8 (ptr);
               }
            }
            else
            {
               tRet = SIRF_CODEC_SSB_LENGTH_ERROR;
            }
         }
         break;
         case SIRF_MSG_SSB_TEST_MODE_DATA_3: /* 0x2E */
         {
            tSIRF_MSG_SSB_TEST_MODE_DATA_3 *msg = (tSIRF_MSG_SSB_TEST_MODE_DATA_3 *)message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT8) +
                                    16 * sizeof(tSIRF_UINT16) +
                                    4 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->svid                  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->period                = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->bit_synch_time        = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->bit_count             = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->poor_status_count     = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->good_status_count     = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->parity_error_count    = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->lost_vco_count        = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->frame_synch_time      = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->cno_mean              = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->cno_sigma             = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->clock_drift           = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->clock_offset          = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->bad_1khz_bit_count    = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->abs_i20ms             = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->abs_q20ms             = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->phase_lock            = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->rtc_frequency         = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->e_to_acq_ratio        = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->t_sync_agc_gain       = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->tm_5_ready            = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->ClkDriftUnc           = SIRFBINARY_IMPORT_UINT16(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_DR_NAV_STATUS: /* 0x01 0x30 */
         {
            tSIRF_MSG_SSB_DR_NAV_STATUS * msg = (tSIRF_MSG_SSB_DR_NAV_STATUS*) message_structure;

            if ( payload_length < ( 14 * sizeof(tSIRF_UINT8) +
                                    2 * sizeof(tSIRF_UINT16) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->nav = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->data = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->cal_gb_cal = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->gsf_cal_ssf_cal = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->nav_across_reset_pos = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->hd = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->gyr_sub_op_spd_sub_op = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->nav_st_int_ran_z_gb_cal_upd = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->gbsf_cal_upd_spd_cal_upd_upd_nav_st = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->gps_upd_pos = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->gps_upd_hd = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->gps_pos_gps_vel = SIRFBINARY_IMPORT_UINT8(ptr);

               /* Where did these fields come from? */
               msg->dws_hd_rt_s_f_cal_valid = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->dws_hd_rt_s_f_cal_upd = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->dws_spd_s_f_cal_valid = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->dws_spd_s_f_cal_upd = SIRFBINARY_IMPORT_UINT8(ptr);
            }

         }
         break;
         case SIRF_MSG_SSB_DR_NAV_STATE: /* 0x02 0x30 */
         {
            tSIRF_MSG_SSB_DR_NAV_STATE * msg = (tSIRF_MSG_SSB_DR_NAV_STATE*) message_structure;

            if ( payload_length < ( 3 * sizeof(tSIRF_UINT8) +
                                    33 * sizeof(tSIRF_UINT16) +
                                    1 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->spd = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->spd_e = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->ssf = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->ssf_e = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->hd_rte = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->hd_rte_e = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gb = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gbE = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gsf = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gsf_e = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->tpe = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->the = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->nav_ctrl = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->reverse = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->hd = SIRFBINARY_IMPORT_UINT16(ptr);

               /* where did these fields come from */
               msg->sensor_pkg = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->odo_spd = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->odo_spd_s_f = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->odo_spd_s_f_err = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->lf_wheel_spd_sf = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->lf_wheel_spd_sf_err = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->rf_wheel_spd_sf = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->rf_wheel_spd_sf_err = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->lr_wheel_spd_sf = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->lr_wheel_spd_sf_err = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->rr_wheel_spd_sf = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->rr_wheel_spd_sf_err = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->rear_axle_spd_delta = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->rear_axle_avg_spd = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->rear_axle_spd_err = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->rear_axle_hd_rt = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->rear_axle_hd_rt_err = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->front_axle_spd_delta = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->front_axle_avg_spd = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->front_axle_spd_err = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->front_axle_hd_rt = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->front_axle_hd_rt_err = SIRFBINARY_IMPORT_UINT16(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_DR_NAV_SUBSYS: /* 0x03 0x30 */
         {
            tSIRF_MSG_SSB_DR_NAV_SUBSYS * msg = (tSIRF_MSG_SSB_DR_NAV_SUBSYS*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT8) +
                                    12 * sizeof(tSIRF_UINT16) +
                                    2 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->gps_hd_rte = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_hd_rte_e = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_hd = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_hd_e = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_spd = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_spd_e = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_pos_e = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->dr_hd_rte = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->dr_hd_rte_e = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->dr_hd = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->dr_hd_e = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->dr_spd = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->dr_spd_e = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->dr_pos_e = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->reserved[0]   = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->reserved[1]   = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_DR_VALID: /* 0x05 0x30 */
         {
            tSIRF_MSG_SSB_DR_VALID * msg = (tSIRF_MSG_SSB_DR_VALID*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->valid    = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->reserved = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_DR_GYR_FACT_CAL: /* 0x06 0x30 */
         {
            tSIRF_MSG_SSB_DR_GYR_FACT_CAL * msg = (tSIRF_MSG_SSB_DR_GYR_FACT_CAL*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT8) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->cal       = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->reserved  = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_DR_SENS_PARAM: /* 0x07 0x30 */
         {
            tSIRF_MSG_SSB_DR_SENS_PARAM * msg = (tSIRF_MSG_SSB_DR_SENS_PARAM*) message_structure;

            if ( payload_length < ( 1 * sizeof(tSIRF_UINT8) +
                                    2 * sizeof(tSIRF_UINT16) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->base_ssf = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->base_gb = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->base_gsf = SIRFBINARY_IMPORT_UINT16(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_DR_DATA_BLK: /* 0x08 0x30 */
         {
            tSIRF_MSG_SSB_DR_DATA_BLK * msg = (tSIRF_MSG_SSB_DR_DATA_BLK*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT8) +
                                    (1 + 2 * 10) * sizeof(tSIRF_UINT16) +
                                    10 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->meas_type = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->valid_cnt = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->bkup_flgs = SIRFBINARY_IMPORT_UINT16(ptr);
               for (i = 0; i < 10; i++)
               {
                  msg->blk[i].tag    = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->blk[i].spd    = SIRFBINARY_IMPORT_UINT16(ptr);
                  msg->blk[i].hd_rte = SIRFBINARY_IMPORT_UINT16(ptr);
               }
            }
         }
         break;
         case SIRF_MSG_SSB_MMF_STATUS: /* 0x50 0x30 */
         {
            tSIRF_MSG_SSB_MMF_STATUS *msg = (tSIRF_MSG_SSB_MMF_STATUS*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT16) +
                                    9 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->status   = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->sys_hdg  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->sys_lat  = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->sys_lon  = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->sys_alt  = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->sys_tow  = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->mmf_hdg  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->mmf_lat  = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->mmf_lon  = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->mmf_alt  = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->mmf_tow  = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_SBAS_PARAM: /* 0x32 */
         {
            tSIRF_MSG_SSB_SBAS_PARAM * msg = (tSIRF_MSG_SSB_SBAS_PARAM*) message_structure;

            if ( payload_length < (12 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->prn       = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->mode      = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->timeout   = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->flg_bits  = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->spare[0]  = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->spare[1]  = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->spare[2]  = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->spare[3]  = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->spare[4]  = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->spare[5]  = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->spare[6]  = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->spare[7]  = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_SIRFNAV_COMPLETE: /* 0x01 0x33 */
         {
            tSIRF_MSG_SSB_SIRFNAV_COMPLETE *msg = (tSIRF_MSG_SSB_SIRFNAV_COMPLETE*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->reserved   = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_SIRFNAV_TIME_TAGS: /* 0x04 0x33 */
         {
            tSIRF_MSG_SSB_SIRFNAV_TIME_TAGS *msg = (tSIRF_MSG_SSB_SIRFNAV_TIME_TAGS*) message_structure;

            if ( payload_length < ( 6 * sizeof(tSIRF_UINT8) +
                                    2 * sizeof(tSIRF_UINT16) +
                                    3 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->tracker_time_tag              = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->gps_week                      = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_tow                       = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->tracker_rtc_day               = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->tracker_rtc_hour              = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->tracker_rtc_min               = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->tracker_rtc_sec               = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->tracker_rtc_clkctr            = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->tracker_rtc_ok                = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->tracker_rtc_rollover_complete = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->tracker_rtc_rollover_seconds  = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;
      case SIRF_MSG_SSB_TRACKER_LOADER_STATE: /* 0x0633 (51, 6) */
      {
         tSIRF_MSG_SSB_TRACKER_LOADER_STATE *msg = (tSIRF_MSG_SSB_TRACKER_LOADER_STATE*)message_structure;

         if ( payload_length < ( 4 * sizeof(tSIRF_UINT32) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length        = sizeof(*msg);
            msg->loader_state      = SIRFBINARY_IMPORT_UINT32( ptr );
            msg->percentage_loaded = SIRFBINARY_IMPORT_UINT32( ptr );
            msg->loader_error      = SIRFBINARY_IMPORT_UINT32( ptr );
            msg->time_tag          = SIRFBINARY_IMPORT_UINT32( ptr );
         }
      }
      break;
         case SIRF_MSG_SSB_SIRFNAV_START: /* 0x0733 (51, 7) */
         {
            tSIRF_MSG_SSB_SIRFNAV_START* msg = (tSIRF_MSG_SSB_SIRFNAV_START*) message_structure;

            if ( payload_length < (4 * sizeof(tSIRF_UINT32) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->start_mode   = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->clock_offset = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->port_num     = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->baud_rate    = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_SIRFNAV_STOP: /* 0x0833 (51, 8) */
         {
            tSIRF_MSG_SSB_SIRFNAV_STOP* msg = (tSIRF_MSG_SSB_SIRFNAV_STOP*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length   = sizeof(*msg);
               msg->stop_mode = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_STARTUP_INFO: /* 0x0136 */
         {
            tSIRF_MSG_SSB_STARTUP_INFO * msg = (tSIRF_MSG_SSB_STARTUP_INFO*) message_structure;

            if ( payload_length < ( 7 * sizeof(tSIRF_UINT8) +
                                    6 * sizeof(tSIRF_UINT16) +
                                    9 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->mode                     = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->status                   = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->dr_status                = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->gps_off_time             = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->gps_week                 = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->gps_tow                  = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->utc_year                 = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->utc_month                = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->utc_day                  = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->utc_hour                 = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->utc_min                  = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->utc_sec                  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->lat                      = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->lon                      = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->alt_ellips               = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->heading                  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->valid_ephemeris          = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->collected_almanac        = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->collected_almanac_week   = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->factory_almanac_week     = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->clk_offset               = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->reserved                 = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_EE_GPS_TIME_INFO: /* 0x0138 */
         {
            tSIRF_MSG_SSB_EE_GPS_TIME_INFO *msg = (tSIRF_MSG_SSB_EE_GPS_TIME_INFO *) message_structure;

            if ( payload_length < ( 1 * sizeof(tSIRF_UINT8) +
                                    1 * sizeof(tSIRF_UINT16) +
                                    2 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               msg->time_valid_flag        = SIRFBINARY_IMPORT_UINT8(  ptr );
               msg->gps_week               = SIRFBINARY_IMPORT_UINT16( ptr );
               msg->gps_tow                = SIRFBINARY_IMPORT_UINT32( ptr );
               msg->ephemeris_request_mask = SIRFBINARY_IMPORT_UINT32( ptr );
            }
         }
         break;

         case SIRF_MSG_SSB_EE_INTEGRITY:     /* 0x0238 */
         case SIRF_MSG_SSB_EE_STATE:         /* 0x0338 */
         case SIRF_MSG_SSB_EE_CLK_BIAS_ADJ:  /* 0x0438 */
         break;

         case SIRF_MSG_SSB_EE_ACK:           /* 0xFF38 */
         {
            tSIRF_MSG_SSB_EE_ACK *msg = (tSIRF_MSG_SSB_EE_ACK*) message_structure;

            if ( payload_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               msg->acq_id     = SIRFBINARY_IMPORT_UINT8( ptr );
               msg->acq_sub_id = SIRFBINARY_IMPORT_UINT8( ptr );
            }
         }
         break;

         case SIRF_MSG_SSB_GPIO_READ: /* 0xC041 (65,192) */
         {
            tSIRF_MSG_SSB_GPIO_READ *msg = (tSIRF_MSG_SSB_GPIO_READ*)message_structure;

            if ( payload_length < ( 1 * sizeof(tSIRF_UINT16) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length        = sizeof(*msg);
               msg->gpio_state        = SIRFBINARY_IMPORT_UINT16( ptr );
            }
         }
         break;

         case SIRF_MSG_SSB_DOP_VALUES: /* 0x42 */
         {
            tSIRF_MSG_SSB_DOP_VALUES *msg = (tSIRF_MSG_SSB_DOP_VALUES*) message_structure;

            if ( payload_length < ( 5 * sizeof(tSIRF_UINT16) +
                                    1 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->gps_tow = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->gdop    = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->pdop    = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->hdop    = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->vdop    = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->tdop    = SIRFBINARY_IMPORT_UINT16(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_INITIALIZE: /* 0x80 */
         {
            tSIRF_MSG_SSB_INITIALIZE * msg = (tSIRF_MSG_SSB_INITIALIZE*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT8) +
                                    1 * sizeof(tSIRF_UINT16) +
                                    5 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->ecef_x        = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->ecef_y        = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->ecef_z        = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->clk_offset    = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->gps_tow       = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->gps_week      = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->chnl_cnt      = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->restart_flags = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_SET_NMEA_MODE: /* 0x81 */
         {
            tSIRF_MSG_SSB_SET_NMEA_MODE * msg = (tSIRF_MSG_SSB_SET_NMEA_MODE*) message_structure;

            if ( payload_length < ( (1 + 2 * 10) * sizeof(tSIRF_UINT8) +
                                     1 * sizeof(tSIRF_UINT16) +
                                     header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               msg->mode                 = SIRFBINARY_IMPORT_UINT8  (ptr);
               for (i = 0; i <SIRF_MSG_SSB_MAX_NUM_NMEA_MSG; i++)
               {
                  msg->nmea_cfg[i].rate  = SIRFBINARY_IMPORT_UINT8  (ptr);
                  msg->nmea_cfg[i].cksum = SIRFBINARY_IMPORT_UINT8  (ptr);
               }
               msg->baud_rate            = SIRFBINARY_IMPORT_UINT16 (ptr);
               *message_length = sizeof(*msg);
            }
         }
         break;
         case SIRF_MSG_SSB_SET_ALMANAC: /* 0x82 */
         {
            tSIRF_MSG_SSB_SET_ALMANAC * msg = (tSIRF_MSG_SSB_SET_ALMANAC*) message_structure;

            if ( payload_length < ( SIRF_MAX_SVID_CNT * SIRF_MSG_SSB_ALMANAC_DATA_ENTRY * sizeof(tSIRF_INT16) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               for (i = 0; i < SIRF_MAX_SVID_CNT; i++)
               {
                  for (j = 0; j < SIRF_MSG_SSB_ALMANAC_DATA_ENTRY; j++)
                  {
                     msg->almanac[i].data[j] = SIRFBINARY_IMPORT_SINT16(ptr);
                  }
               }
            }
         }
         break;
         case SIRF_MSG_SSB_POLL_SW_VERSION: /* 0x84 */
         {
            tSIRF_MSG_SSB_POLL_SW_VERSION * msg = (tSIRF_MSG_SSB_POLL_SW_VERSION*) message_structure;
            memset(msg,0,sizeof(*msg));
            if ( payload_length <  header_len )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               if(payload_length >= (1 * sizeof(tSIRF_UINT8) + header_len))
               {
                  msg->reserved = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
         }
         break;
         case SIRF_MSG_SSB_SET_DGPS_SOURCE: /* 0x85 */
         {
            tSIRF_MSG_SSB_SET_DGPS_SOURCE * msg = (tSIRF_MSG_SSB_SET_DGPS_SOURCE*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT8) +
                                    1 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->src      = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->freq     = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->bit_rate = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_SET_NAV_MODE: /* 0x88 */
         {
            tSIRF_MSG_SSB_SET_NAV_MODE* msg = (tSIRF_MSG_SSB_SET_NAV_MODE*) message_structure;

            if ( payload_length < ( 11 * sizeof(tSIRF_UINT8) +
                                    1 * sizeof(tSIRF_INT16) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->enable_3d         = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->enable_con_alt    = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->degraded_mode     = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->pos_mode_enable   = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->dr_enable         = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->alt_input         = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->alt_mode          = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->alt_src           = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->coast_timeout     = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->degraded_timeout  = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->dr_timeout        = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->trk_smooth        = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_SET_DOP_MODE: /* 0x89 */
         {
            tSIRF_MSG_SSB_SET_DOP_MODE* msg = (tSIRF_MSG_SSB_SET_DOP_MODE*) message_structure;

            if ( payload_length < (4 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->mode      = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->gdop_th   = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->pdop_th   = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->hdop_th   = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_SET_DGPS_MODE: /* 0x8A */
         {
            tSIRF_MSG_SSB_SET_DGPS_MODE *msg = (tSIRF_MSG_SSB_SET_DGPS_MODE*)message_structure;
            if (payload_length < (2*sizeof(tSIRF_UINT8) + header_len))
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->mode    = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->timeout = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_SET_ELEV_MASK: /* 0x8B */
         {
            tSIRF_MSG_SSB_SET_ELEV_MASK* msg = (tSIRF_MSG_SSB_SET_ELEV_MASK*) message_structure;

            if ( payload_length < (2 * sizeof(tSIRF_INT16) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->trk     = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->nav     = SIRFBINARY_IMPORT_SINT16(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_SET_POWER_MASK: /* 0x8C */
         {
            tSIRF_MSG_SSB_SET_POWER_MASK* msg = (tSIRF_MSG_SSB_SET_POWER_MASK*) message_structure;

            if ( payload_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->trk     = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->nav     = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_SET_STAT_NAV: /* 0x8F */
         {
            tSIRF_MSG_SSB_SET_STAT_NAV* msg = (tSIRF_MSG_SSB_SET_STAT_NAV*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->staticnav = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_POLL_CLOCK_STATUS: /* 0x90 */
         {
            tSIRF_MSG_SSB_POLL_CLOCK_STATUS * msg = (tSIRF_MSG_SSB_POLL_CLOCK_STATUS*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->reserved = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_POLL_ALMANAC: /* 0x92 */
         {
            tSIRF_MSG_SSB_POLL_ALMANAC * msg = (tSIRF_MSG_SSB_POLL_ALMANAC*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->reserved = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_POLL_EPHEMERIS: /* 0x93 */
         {
            tSIRF_MSG_SSB_POLL_EPHEMERIS * msg = (tSIRF_MSG_SSB_POLL_EPHEMERIS*) message_structure;

            if ( payload_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->svid     = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->reserved = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_FLASH_UPDATE: /* 0x94 */
         {
            if ( payload_length < header_len )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
                /* There is no payload in this message */
                *message_length = 0;
            }
         }
         break;
         case SIRF_MSG_SSB_SET_EPHEMERIS: /* 0x95 */
         {
            tSIRF_MSG_SSB_SET_EPHEMERIS * msg = (tSIRF_MSG_SSB_SET_EPHEMERIS*) message_structure;

            if ( payload_length < (3 * 15 * sizeof(tSIRF_UINT16) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               for (i = 0; i < 3; i++)
               {
                  for (j = 0; j < 15; j++)
                  {
                     msg->subframe[i][j] = SIRFBINARY_IMPORT_UINT16(ptr);
                  }
               }
            }
         }
         break;
         case SIRF_MSG_SSB_SET_OP_MODE: /* 0x96 */
         {
            tSIRF_MSG_SSB_SET_OP_MODE * msg = (tSIRF_MSG_SSB_SET_OP_MODE*) message_structure;

            if ( payload_length < ( 1 * sizeof(tSIRF_UINT8) +
                                    3 * sizeof(tSIRF_UINT16) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->mode   = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->svid   = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->period = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->tm5Cmd = SIRFBINARY_IMPORT_UINT8( ptr );
            }
         }
         break;
         case SIRF_MSG_SSB_SET_LOW_POWER: /* 0x97 */
         {
            tSIRF_MSG_SSB_SET_LOW_POWER* msg = (tSIRF_MSG_SSB_SET_LOW_POWER*)  message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT16) +
                                    1 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length  = sizeof(*msg);
               msg->push_to_fix = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->duty_cycle  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->on_time     = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_POLL_RECEIVER_PARAMS: /* 0x98 */
         {
            tSIRF_MSG_SSB_NAV_POLL_RECEIVER_PARAMS * msg = (tSIRF_MSG_SSB_NAV_POLL_RECEIVER_PARAMS*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->reserved = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_DEMO_SET_RESTART_MODE: /* 0x01A1 */
         {
            tSIRF_MSG_SSB_DEMO_SET_RESTART_MODE* msg = (tSIRF_MSG_SSB_DEMO_SET_RESTART_MODE*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->control = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_DEMO_TEST_CPU_STRESS: /* 0x02A1 */
         {
            tSIRF_MSG_SSB_DEMO_TEST_CPU_STRESS* msg = (tSIRF_MSG_SSB_DEMO_TEST_CPU_STRESS*) message_structure;

            if ( payload_length < ( 1 * sizeof(tSIRF_UINT16) +
                                    5 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->TaskID           = SIRFBINARY_IMPORT_UINT16( ptr );
               msg->PeriodOn         = SIRFBINARY_IMPORT_UINT32( ptr );
               msg->PeriodOff        = SIRFBINARY_IMPORT_UINT32( ptr );
               msg->LoadDuration     = SIRFBINARY_IMPORT_UINT32( ptr );
               msg->FullLoadDuration = SIRFBINARY_IMPORT_UINT32( ptr );
               msg->OffsetTime       = SIRFBINARY_IMPORT_UINT32( ptr );
            }
         }
         break;

         case SIRF_MSG_SSB_DEMO_STOP_TEST_APP: /* 0x03A1 */
         {
            /* Message has no data */
            memset( message_structure, 0, sizeof(tSIRF_MSG_SSB_DEMO_STOP_TEST_APP) );
            *message_length = sizeof(tSIRF_MSG_SSB_DEMO_STOP_TEST_APP);
         }
         break;

         case SIRF_MSG_SSB_DEMO_START_GPS_ENGINE: /* 0x05A1 */
         {
            tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE* msg = (tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE*) message_structure;

            if ( payload_length < (4 * sizeof(tSIRF_UINT32) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->start_mode   = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->clock_offset = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->port_num     = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->baud_rate    = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE: /* 0x06A1 */
         {
            tSIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE* msg = (tSIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length   = sizeof(*msg);
               msg->stop_mode = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_SIRFNAV_STORE_NOW: /* 0x07A1 */
         {
            /* Message has no data */
            memset( message_structure, 0, sizeof(tSIRF_MSG_SSB_SIRFNAV_STORE_NOW) );
            *message_length = sizeof(tSIRF_MSG_SSB_SIRFNAV_STORE_NOW);
         }
         break;

         case SIRF_MSG_SSB_DEMO_START_NAV_ENGINE: /* 0x08A1 */
         {
            tSIRF_UINT8 temp[MAX_PORT_NUM_STRING_LENGTH];
            tSIRF_UINT32 i;

            tSIRF_MSG_SSB_DEMO_START_NAV_ENGINE *msg = (tSIRF_MSG_SSB_DEMO_START_NAV_ENGINE*) message_structure;

            /* Add correct size */
            if ( payload_length < ((unsigned) SIRF_CONFIG_SIZE + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->config.start_mode                  = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->config.uart_max_preamble           = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->config.uart_idle_byte_wakeup_delay = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->config.ref_clk_offset              = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->config.uart_hw_fc                  = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->config.lna_type                    = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->config.debug_settings              = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->config.ref_clk_warmup_delay        = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->config.ref_clk_frequency           = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->config.ref_clk_uncertainty         = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->config.uart_baud_rate              = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->config.code_load_baud_rate         = msg->config.uart_baud_rate;
               msg->config.io_pin_configuration_mode   = SIRFBINARY_IMPORT_UINT8(ptr);
               for (i=0; i<SIRFNAV_UI_CTRL_NUM_GPIO_PINS_CONFIG; i++)
               {
                  msg->config.io_pin_configuration[i]  = SIRFBINARY_IMPORT_UINT16(ptr);
               }
               msg->config.i2c_host_address            = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->config.i2c_tracker_address         = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->config.i2c_mode                    = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->config.i2c_rate                    = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->config.spi_rate                    = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->config.on_off_control              = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->config.flash_mode                  = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->config.storage_mode                = SIRFBINARY_IMPORT_UINT8(ptr);
               for (i=0; i<MAX_PORT_NUM_STRING_LENGTH; i++)
               {
                  temp[i]                              = SIRFBINARY_IMPORT_UINT8(ptr);
               }
               memcpy(msg->config.tracker_port, &temp, sizeof(msg->config.tracker_port));
               msg->config.tracker_port_select         = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->config.weak_signal_enabled         = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->config.backup_LDO_mode_enabled     = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_SET_MSG_RATE: /* 0xA6 */
         {
            tSIRF_MSG_SSB_SET_MSG_RATE * msg = (tSIRF_MSG_SSB_SET_MSG_RATE*) message_structure;

            if ( payload_length < (7 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->mode        = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->msg_id      = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->rate        = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->sub_id      = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->reserved[0] = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->reserved[1] = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->reserved[2] = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_SET_LOW_POWER_PARAMS:  /* 0xA7 */
         {
            tSIRF_MSG_SSB_SET_LOW_POWER_PARAMS* msg = (tSIRF_MSG_SSB_SET_LOW_POWER_PARAMS*)  message_structure;

            if ( payload_length < ( 1 * sizeof(tSIRF_UINT16) +
                                    3 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length  = sizeof(*msg);
               msg->MaxOffTime             = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->MaxAcqTime             = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->push_to_fix_period     = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->adaptive_tp            = SIRFBINARY_IMPORT_UINT16(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_SET_SBAS_PRN: /* 0xAA */
         {
            tSIRF_MSG_SSB_SET_SBAS_PRN * msg = (tSIRF_MSG_SSB_SET_SBAS_PRN*) message_structure;

            if ( payload_length < ( 5 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->prnOrRegion = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->mode        = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->flagBits    = SIRFBINARY_IMPORT_SINT8 (ptr);
               msg->region      = SIRFBINARY_IMPORT_SINT8 (ptr);
               msg->regionPRN   = SIRFBINARY_IMPORT_SINT8 (ptr);
            }
         }
         break;

        case SIRF_MSG_SSB_ADVANCED_NAV_INIT: /**  0x00AC  */
        {
            tSIRF_MSG_SSB_ADVANCED_NAV_INIT * msg = (tSIRF_MSG_SSB_ADVANCED_NAV_INIT*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT16) +
                                    8 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               *(tSIRF_UINT32*)&msg->control = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->timeSkew   = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->timeUncert = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->pos[0]     = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->pos[1]     = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->pos[2]     = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->horzUncert = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->vertUncert = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->clkSkew    = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->clkUncert  = SIRFBINARY_IMPORT_UINT32(ptr);
               ptr += 6; /** message has 6 reserved bytes at end */
            }
         }
         break;

         case SIRF_MSG_SSB_DR_SET_NAV_INIT:  /* 0x01 0xAC */
         {
            tSIRF_MSG_SSB_DR_SET_NAV_INIT * msg = (tSIRF_MSG_SSB_DR_SET_NAV_INIT*) message_structure;

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT8) +
                                    2 * sizeof(tSIRF_UINT16) +
                                    5 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->lat            = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->lon            = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->alt_ellips     = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->heading        = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->clk_offset     = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->gps_tow        = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->gps_week       = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->chnl_cnt       = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->restart_flags  = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_DR_SET_NAV_MODE: /* 0x02 0xAC */
         {
            tSIRF_MSG_SSB_DR_SET_NAV_MODE * msg = (tSIRF_MSG_SSB_DR_SET_NAV_MODE*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->mode        = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_DR_SET_GYR_FACT_CAL: /* 0x03 0xAC */
         {
            tSIRF_MSG_SSB_DR_SET_GYR_FACT_CAL * msg = (tSIRF_MSG_SSB_DR_SET_GYR_FACT_CAL*) message_structure;

            if ( payload_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->cal         = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->reserved    = SIRFBINARY_IMPORT_UINT8 (ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_DR_SET_SENS_PARAM: /* 0x04 0xAC */
         {
            tSIRF_MSG_SSB_DR_SET_SENS_PARAM * msg = (tSIRF_MSG_SSB_DR_SET_SENS_PARAM*) message_structure;

            if ( payload_length < ( 1 * sizeof(tSIRF_UINT8) +
                                    2 * sizeof(tSIRF_UINT16) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->base_ssf = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->base_gb  = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->base_gsf = SIRFBINARY_IMPORT_UINT16(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_DR_POLL_SENS_PARAM:   /* 0x07 0xAC */
         {
            /* Message has no data */
            memset( message_structure, 0, sizeof(tSIRF_MSG_SSB_DR_POLL_SENS_PARAM) );
            *message_length = sizeof(tSIRF_MSG_SSB_DR_POLL_SENS_PARAM);
         }
         break;

         case SIRF_MSG_SSB_DR_POLL_GYR_FACT_CAL: /* 0x06 0xAC */
         {
            /* Message has no data */
            memset( message_structure, 0, sizeof(tSIRF_MSG_SSB_DR_POLL_GYR_FACT_CAL) );
            *message_length = sizeof(tSIRF_MSG_SSB_DR_POLL_GYR_FACT_CAL);
         }
         break;

         case SIRF_MSG_SSB_DR_POLL_VALID:        /* 0x05 0xAC */
         {
            /* Message has no data */
            memset( message_structure, 0, sizeof(tSIRF_MSG_SSB_DR_POLL_VALID) );
            *message_length = sizeof(tSIRF_MSG_SSB_DR_POLL_VALID);
         }
         break;

         case SIRF_MSG_SSB_DR_CAR_BUS_DATA: /* 0x09 0xAC */
         {
            tSIRF_MSG_SSB_DR_CAR_BUS_DATA *msg = (tSIRF_MSG_SSB_DR_CAR_BUS_DATA*) message_structure;

         *message_length = sizeof(*msg);
         if (payload_length < 2 * sizeof(tSIRF_UINT8) + sizeof(tSIRF_UINT16))
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

         msg->sensor_data_type  = SIRFBINARY_IMPORT_UINT8 (ptr);
         msg->num_of_data_sets  = SIRFBINARY_IMPORT_UINT8 (ptr);
         msg->reverse_bitmap    = SIRFBINARY_IMPORT_UINT16(ptr);

            if ( payload_length < ( (2 + 2 * msg->num_of_data_sets) * sizeof(tSIRF_UINT8) +
                                    (1 + 5 * msg->num_of_data_sets) * sizeof(tSIRF_UINT16) +
                                    msg->num_of_data_sets * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else if (msg->num_of_data_sets > MAX_DR_CAR_BUS_DATA_SETS)
            {
               tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
            }
            else
            {
               for(i = 0; i <(msg->num_of_data_sets); i++)
               {
                  msg->data[i].valid_sensor_indication   = SIRFBINARY_IMPORT_UINT8 (ptr);
                  msg->data[i].data_set_time_tag         = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->data[i].odometer_speed            = SIRFBINARY_IMPORT_UINT16(ptr);
                  msg->data[i].data1                     = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->data[i].data2                     = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->data[i].data3                     = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->data[i].data4                     = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->data[i].reserved                  = SIRFBINARY_IMPORT_UINT8 (ptr);
               }
            }
         }
         break;

         case SIRF_MSG_SSB_DR_CAR_BUS_ENABLED: /* 0x0A 0xAC */
         {
            tSIRF_MSG_SSB_DR_CAR_BUS_ENABLED *msg = (tSIRF_MSG_SSB_DR_CAR_BUS_ENABLED*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length              = sizeof(*msg);
               msg->mode                    = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_DR_CAR_BUS_DISABLED: /* 0x0B 0xAC */
         {
            tSIRF_MSG_SSB_DR_CAR_BUS_DISABLED *msg = (tSIRF_MSG_SSB_DR_CAR_BUS_DISABLED*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length              = sizeof(*msg);
               msg->mode                    = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;

      case SIRF_MSG_SSB_DR_SENSOR_DATA: /* 0x19 0xAC */
      {
         tSIRF_MSG_SSB_DR_SENSOR_DATA *msg = (tSIRF_MSG_SSB_DR_SENSOR_DATA*) message_structure;

         *message_length = sizeof(*msg);
         msg->sensor_data_type  = SIRFBINARY_IMPORT_UINT8 (ptr);
         msg->num_of_data_sets  = SIRFBINARY_IMPORT_UINT8 (ptr);
         msg->reserved          = SIRFBINARY_IMPORT_UINT16(ptr);

         if ( payload_length < ( 2 * sizeof(tSIRF_UINT8) +
                                 (1 + (1+SIRF_MSG_SSB_DR_SENSOR_DATA_SET_MAX) * msg->num_of_data_sets) * sizeof(tSIRF_UINT16) +
                                 msg->num_of_data_sets * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else if (msg->num_of_data_sets > SIRF_MSG_SSB_DR_SENSOR_DATA_MAX)
         {
            tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
         }
         else
         {
            for( i = 0; i <(msg->num_of_data_sets); i++ )
            {
               msg->data_set[i].valid_data_indication   = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->data_set[i].data_set_time_tag       = SIRFBINARY_IMPORT_UINT32(ptr);

               for ( j=0; j<SIRF_MSG_SSB_DR_SENSOR_DATA_SET_MAX; j++ )
               {
                  msg->data_set[i].data[j]              = SIRFBINARY_IMPORT_SINT16(ptr);
               }
            }
         }
      }
      break;

         case SIRF_MSG_SSB_MMF_DATA: /* 0x50 0xAC */
         {
            tSIRF_MSG_SSB_MMF_DATA *msg = (tSIRF_MSG_SSB_MMF_DATA*) message_structure;

            if ( payload_length < (  2 * sizeof(tSIRF_UINT8) +
                                    (3 * 3) * sizeof(tSIRF_UINT16) +
                                    (1 + 3 * 4) * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->gps_tow_reference = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->num_of_data_sets  = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->control           = SIRFBINARY_IMPORT_UINT8 (ptr);
               for(i = 0; i <3; i++)
               {
                  msg->mmf_data[i].lat              = SIRFBINARY_IMPORT_SINT32(ptr);
                  msg->mmf_data[i].lon              = SIRFBINARY_IMPORT_SINT32(ptr);
                  msg->mmf_data[i].horiz_pos_uncert = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->mmf_data[i].alt_ellips       = SIRFBINARY_IMPORT_SINT32(ptr);
                  msg->mmf_data[i].vert_pos_uncert  = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->mmf_data[i].heading          = SIRFBINARY_IMPORT_UINT16(ptr);
                  msg->mmf_data[i].heading_uncert   = SIRFBINARY_IMPORT_UINT16(ptr);
                  msg->mmf_data[i].reserved         = SIRFBINARY_IMPORT_UINT16(ptr);
               }
            }
         }
         break;
         case SIRF_MSG_SSB_MMF_SET_MODE: /* 0x51 0xAC */
         {
            tSIRF_MSG_SSB_MMF_SET_MODE *msg = (tSIRF_MSG_SSB_MMF_SET_MODE*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length        = sizeof(*msg);
               msg->mode              = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;

      case SIRF_MSG_SSB_GPIO_WRITE: /* 0xB2 0x30 (178,48) */
      {
         tSIRF_MSG_SSB_GPIO_WRITE *msg = (tSIRF_MSG_SSB_GPIO_WRITE*)message_structure;

         if ( payload_length < (2 * sizeof(tSIRF_UINT16) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length    = sizeof(*msg);
            msg->gpio_to_write = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->gpio_state    = SIRFBINARY_IMPORT_UINT16(ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_GPIO_MODE_SET: /* 0xB2 0x31 (178,49) */
      {
         tSIRF_MSG_SSB_GPIO_MODE_SET *msg = (tSIRF_MSG_SSB_GPIO_MODE_SET*)message_structure;

         if ( payload_length < (3 * sizeof(tSIRF_UINT16) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length  = sizeof(*msg);
            msg->gpio_to_set = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->gpio_mode   = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->gpio_state  = SIRFBINARY_IMPORT_UINT16(ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_TRK_HW_CONFIG:   /* 0xCE */
      {
         tSIRF_MSG_SSB_TRK_HW_CONFIG *msg = (tSIRF_MSG_SSB_TRK_HW_CONFIG*) message_structure;

         if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length        = sizeof(*msg);
            msg->ExtLNAEnable      = SIRFBINARY_IMPORT_UINT8(ptr);  /* External LNA Enable 0=Ignore, 1=Internal, 2=External */
            /* Advancing the data pointer to make the size reconcile correctly, since this data won't be */
            /* utilized. DO NOT allow SSB to set the Reserved fields, since this is responsibility of    */
            /* the Host. Customers are NOT allowed to modify these Reserved fields.                      */
            ptr++;
            msg->Reserved = 0;
            for (i = 0; i < SIRF_MSG_SSB_TRK_HW_CONFIG_RF_OVRD_MSG_LENGTH; i++)
            {
               ptr++;
               msg->Reserved1[i] = 0;
            }
         }
      }
      break;

         case SIRF_MSG_SSB_TRKR_CUSTOMIO: /* 0x01 0xB2 */
         {
            tSIRF_UINT8 readType  = *(payload + 2);
            tSIRF_UINT8 readVer   = *(payload + 3);
            switch(readType)
            {
               case CUSTOMIO_SEL1:
               {
                  switch(readVer)
                  {
                     case 0:
                     {
                        tSIRF_MSG_SSB_TRKR_CUSTOMIO_SEL1* msg = (tSIRF_MSG_SSB_TRKR_CUSTOMIO_SEL1*) message_structure;
                        if ( payload_length < (SIRF_MSG_SSB_TRKR_CUSTOMIO_SEL1 + (tSIRF_UINT32)header_len) )
                        {
                           tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
                        }
                        else
                        {
                           *message_length   = sizeof(*msg);
                           msg->m_type         = SIRFBINARY_IMPORT_UINT8(ptr);
                           msg->m_ver          = SIRFBINARY_IMPORT_UINT8(ptr);
                           msg->m_pin_config   = SIRFBINARY_IMPORT_UINT8(ptr);
                        }
                     }
                     break;

                     default:
                        break;
                  }
               }
               break;

               default:
                  break;
            }
         }
         break;

         case SIRF_MSG_SSB_TRKR_CONFIG: /* 0x02 0xB2 */
         {
            tSIRF_INT32 i;
            tSIRF_MSG_SSB_TRKR_CONFIG* msg = (tSIRF_MSG_SSB_TRKR_CONFIG*) message_structure;

            if ( payload_length < (SIRF_MSG_SSB_TRKR_CONFIG_LENGTH + (tSIRF_UINT32)header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length        = sizeof(*msg);
               msg->ref_clk_frequency           = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->ref_clk_warmup_delay        = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->ref_clk_uncertainty         = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->ref_clk_offset              = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->ext_lna_enable              = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->io_pin_config_enable        = SIRFBINARY_IMPORT_UINT8(ptr);
               for(i=0; i<SIRFNAV_UI_CTRL_NUM_GPIO_PINS_CONFIG; i++)
               {
                  msg->io_pin_config[i]         = SIRFBINARY_IMPORT_UINT16(ptr);
               }
               msg->uart_max_preamble           = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->uart_idle_byte_wakeup_delay = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->uart_baud_rate              = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->uart_hw_fc                  = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->i2c_master_addr             = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->i2c_slave_addr              = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->i2c_rate                    = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->i2c_mode                    = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->i2c_max_msg_length          = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->pwr_ctrl_on_off             = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->backup_LDO_mode_enabled     = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_TRKR_PEEKPOKE_CMD: /* 0x03 0xB2 */
         {
            tSIRF_MSG_SSB_TRKR_PEEKPOKE_CMD *msg = (tSIRF_MSG_SSB_TRKR_PEEKPOKE_CMD*) message_structure;

            *message_length         = sizeof(*msg);
            msg->type               = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->access             = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->address            = SIRFBINARY_IMPORT_UINT32(ptr);

            if (msg->type == 0 || msg->type == 1)     /* 4-byte peek/poke cmd */
            {
               /* Simpler version of mei_reverseBytes() */
               for ( k = 3; k >= 0; k-- )
               {
                  msg->data[k]         = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            else if (msg->type == 2)                  /* n-byte peek cmd */
            {
               msg->numbytes           = SIRFBINARY_IMPORT_UINT16(ptr);
            }
            else if (msg->type == 3)                  /* n-byte poke cmd */
            {
               msg->numbytes           = SIRFBINARY_IMPORT_UINT16(ptr);
               if(msg->numbytes > MAX_DATA_SIZE)
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
               }
               else
               {
                  for ( k = 0; k < msg->numbytes; k++ )
                  {
                     msg->data[k]         = SIRFBINARY_IMPORT_UINT8(ptr);
                  }
               }
            }
            else
               return (SIRF_CODEC_ERROR_INVALID_PARAMETER);

         }
         break;

#ifdef PVT_BUILD
         case SIRF_MSG_SSB_TRKR_CONFIG_POLL: /* 0x09, 0xB2 */
         {
            /* Message has no data */
            memset( message_structure, 0, sizeof(tSIRF_MSG_SSB_TRKR_CONFIG_POLL) );
            *message_length = sizeof (tSIRF_MSG_SSB_TRKR_CONFIG_POLL);
         }
         break;

         case SIRF_MSG_SSB_CCK_POLL: /* 0x0B, 0xB2 */
         {
            /* Message has no data */
            memset( message_structure, 0, sizeof(tSIRF_MSG_SSB_CCK_POLL) );
            *message_length = sizeof (tSIRF_MSG_SSB_CCK_POLL);
         }
         break;

         case SIRF_MSG_SSB_PATCH_STORAGE_CONTROL: /* 0x14 0xB2 */
         {
            tSIRF_MSG_SSB_PATCH_STORAGE_CONTROL *msg = (tSIRF_MSG_SSB_PATCH_STORAGE_CONTROL*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->patch_storage_ctrl = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         } /* end of case SIRF_MSG_SSB_PATCH_STORAGE_CONTROL */
         break;

         case SIRF_MSG_SSB_SWTB_PMLOAD_IN:     /* 0x22 0xB2 */
         {
            tSIRF_MSG_SSB_SWTB_PMLOAD_IN *msg = (tSIRF_MSG_SSB_SWTB_PMLOAD_IN *)message_structure;

            if  ((payload_length < (1 * sizeof(tSIRF_UINT16) + header_len)) ||
                 (payload_length > (MAX_SWTB_PMLOAD_PDATA_SIZE + 4)))
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = payload_length;
               msg->seqno = SIRFBINARY_IMPORT_UINT16(ptr);
               for (i = 0; i < (payload_length - 4); i++)
               {
                  msg->PatchData[i] = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
         }
         break;

         case SIRF_MSG_SSB_SWTB_PMEXIT_IN:     /* 0x26 0xB2 */
         {
             *message_length = 0;
         }
         break;

         case SIRF_MSG_SSB_SWTB_PMSTART_IN:     /* 0x28 0xB2 */
         {
             *message_length = 0;
         }
         break;

         case SIRF_MSG_SSB_ENB_ALM2FLASH: /* 0x40 0xB2 */
         {
            tSIRF_MSG_SSB_ENB_ALM2FLASH *msg = (tSIRF_MSG_SSB_ENB_ALM2FLASH*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->enb_alm2flash = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         } /* end of case SIRF_MSG_SSB_ENB_ALM2FLASH */
         break;
#endif /* PVT_BUILD */

         case SIRF_MSG_SSB_SW_COMMANDED_OFF: /* 0x10, 0xCD */
         {
            /* Message has no data */
            memset( message_structure, 0, sizeof(tSIRF_MSG_SSB_SW_COMMANDED_OFF) );
            *message_length = sizeof (tSIRF_MSG_SSB_SW_COMMANDED_OFF);
         }
         break;
         case SIRF_MSG_SSB_TRK_HW_TEST_CONFIG: /* 0xCF */
         {
            tSIRF_MSG_SSB_TRK_HW_TEST_CONFIG *msg = (tSIRF_MSG_SSB_TRK_HW_TEST_CONFIG*) message_structure;

            if ( payload_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length        = sizeof(*msg);
               msg->RFTestPoint       = SIRFBINARY_IMPORT_UINT8(ptr);   /* RF Test Point  0=Ignore, 1=Enable, 2=Disable */
               msg->INTCpuPause       = SIRFBINARY_IMPORT_UINT8(ptr);   /* INT_CPUPause       0=Ignore, 1=Enable, 2=Disable */
            }
         }
         break;
         case SIRF_MSG_SSB_SET_IF_TESTPOINT: /* 0xD2 */
         {
            tSIRF_MSG_SSB_SET_IF_TESTPOINT *msg = (tSIRF_MSG_SSB_SET_IF_TESTPOINT*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length         = sizeof(*msg);
               msg->test_point_control = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_PWR_MODE_FPM_REQ:             /* 0x00 0xDA */
         {
            memset( message_structure, 0, sizeof(tSIRF_MSG_SSB_PWR_MODE_FPM_REQ) );
            *message_length = sizeof (tSIRF_MSG_SSB_PWR_MODE_FPM_REQ);
         }
         break;

         /* SIRF_MSG_SSB_PWR_MODE_APM_REQ 0x01 0xDA is decoded under 
         SIRF_CODEC_SSB_AGPS_Decode() */

         case SIRF_MSG_SSB_PWR_MODE_MP_REQ:              /* 0x02 0xDA */
         {
            tSIRF_MSG_SSB_PWR_MODE_MPM_REQ * msg = (tSIRF_MSG_SSB_PWR_MODE_MPM_REQ *) message_structure;
            memset( message_structure, 0, sizeof(tSIRF_MSG_SSB_PWR_MODE_MPM_REQ) );
            *message_length = sizeof (tSIRF_MSG_SSB_PWR_MODE_MPM_REQ);

            if (payload_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               msg->microPowerRequestTimeOut = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->microPowerRequestControl = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->reserved = SIRFBINARY_IMPORT_UINT16(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_PWR_MODE_TP_REQ:     /*  0x03 0xDA */
         {
            tSIRF_MSG_SSB_PWR_MODE_TP_REQ * msg = (tSIRF_MSG_SSB_PWR_MODE_TP_REQ *) message_structure;

            if ( payload_length < (sizeof(tSIRF_UINT16) + 
               3 * sizeof(tSIRF_UINT32) + 
               header_len ) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length    = sizeof(*msg);
               msg->dutyCycle     = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->on_time       = SIRFBINARY_IMPORT_UINT32(ptr);  
               msg->MaxOffTime    = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->MaxSearchTime = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_PWR_MODE_PTF_REQ:  /*  0xDA, 0x4 */
         {
            tSIRF_MSG_SSB_PWR_MODE_PTF_REQ * msg = (tSIRF_MSG_SSB_PWR_MODE_PTF_REQ *) message_structure;

            if ( payload_length < (3 * sizeof(tSIRF_UINT32) + header_len ) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length    = sizeof(*msg);
               msg->ptf_period    = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->MaxSearchTime = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->MaxOffTime    = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_CW_CONFIG: /* 0x01 0xDC */
         {
            tSIRF_MSG_SSB_CW_CONFIG *msg = message_structure;
            if (payload_length < (tSIRF_UINT32)SIRF_MSG_SSB_CW_CONFIG_LEN + header_len)
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->subId = SIRF_GET_SUB_ID(*message_id);
               msg->confMode = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_XO_OUTPUT_CONTROL_IN: /* 0x00, 0xDD */
         {
            tSIRF_MSG_SSB_XO_OUTPUT_CONTROL_IN * msg = (tSIRF_MSG_SSB_XO_OUTPUT_CONTROL_IN *) message_structure;

            if ( payload_length < (tSIRF_UINT32)(SIRF_MSG_SSB_XO_OUTPUT_CONTROL_IN_LEN + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->oneTimeList        = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->contList           = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->outputRequest      = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->spare              = SIRFBINARY_IMPORT_UINT16(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_XO_DEFAULTS_IN: /* 0x01, 0xDD */
         {
            tSIRF_MSG_SSB_XO_DEFAULTS_IN * msg = (tSIRF_MSG_SSB_XO_DEFAULTS_IN *) message_structure;

            if ( payload_length < (tSIRF_UINT32)(SIRF_MSG_SSB_XO_DEFAULTS_IN_LEN + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->source             = SIRFBINARY_IMPORT_UINT8 (ptr);    /** Bit Mask with the source of entries */
               msg->agingRateUnc       = SIRFBINARY_IMPORT_UINT8 (ptr);    /** aging rate uncertainty */
               msg->initialOffsetUnc   = SIRFBINARY_IMPORT_UINT8 (ptr);    /** initial offset uncertainty */
               msg->spare1             = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->clockDrift         = SIRFBINARY_IMPORT_UINT32(ptr);    /** clock drift */
               msg->tempUnc            = SIRFBINARY_IMPORT_UINT16(ptr);    /** temperature uncertainty */
               msg->mfgWeek            = SIRFBINARY_IMPORT_UINT16(ptr);    /** manufacturing wn for aging */
               msg->spare2             = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_TCXO_TABLE_IN: /* 0x02, 0xDD */
         {
            tSIRF_MSG_SSB_TCXO_TABLE_IN * msg = (tSIRF_MSG_SSB_TCXO_TABLE_IN *) message_structure;

            if ( payload_length < (tSIRF_UINT32)(SIRF_MSG_SSB_TCXO_TABLE_IN_LEN + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->spare1             = SIRFBINARY_IMPORT_UINT32(ptr);    /** not used */
               msg->offset             = SIRFBINARY_IMPORT_UINT16(ptr);    /** frequency offset bias from CD default LSB 1, ppb */
               msg->globalMin          = SIRFBINARY_IMPORT_UINT16(ptr);    /** minimum xo error observed LSB 1, ppb */
               msg->globalMax          = SIRFBINARY_IMPORT_UINT16(ptr);    /** maximum xo error observed LSB 1, ppb */
               msg->firstWeek          = SIRFBINARY_IMPORT_UINT16(ptr);    /** full gps week of first table update. LSB 1 */
               msg->lastWeek           = SIRFBINARY_IMPORT_UINT16(ptr);    /** full gps week of last table update LSB 1 */
               msg->lsb                = SIRFBINARY_IMPORT_UINT16(ptr);    /** array LSB of Min[] and Max[] LSB 1, ppb */
               msg->agingBin           = SIRFBINARY_IMPORT_UINT8 (ptr);    /** Bin of last aging update. */
               msg->agingUpcount       = SIRFBINARY_IMPORT_UINT8 (ptr);    /** Aging detection accumulator LSB 1 */
               msg->binCnt             = SIRFBINARY_IMPORT_UINT8 (ptr);    /** count of min bins filled */
               msg->spare2             = SIRFBINARY_IMPORT_UINT8 (ptr);    /** not used */

               for (i=0; i<SSB_XOT_TABLE_SIZE; i++)
               {
                  msg->min[i]          = SIRFBINARY_IMPORT_UINT8 (ptr);    /** Min XO error at each temperature LSB xoTable.lsb */
               }

               for (i=0; i<SSB_XOT_TABLE_SIZE; i++)
               {
                  msg->max[i]          = SIRFBINARY_IMPORT_UINT8 (ptr);    /** Max XO error at each temperature LSB xoTable.lsb */
               }
            }
         }
         break;

         case SIRF_MSG_SSB_XO_TEST_CONTROL_IN: /* 0x03, 0xDD */
         {
            tSIRF_MSG_SSB_XO_TEST_CONTROL_IN * msg = (tSIRF_MSG_SSB_XO_TEST_CONTROL_IN *) message_structure;

            if ( payload_length < (tSIRF_UINT32)(SIRF_MSG_SSB_XO_TEST_CONTROL_IN_LEN + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length   = sizeof(*msg);
               msg->mode         = SIRFBINARY_IMPORT_UINT8 (ptr);    /** Test Mode Control */
               msg->spare1       = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->spare2       = SIRFBINARY_IMPORT_UINT16 (ptr);
            }
         }
         break;

#ifdef XO_ENABLED
         case SIRF_MSG_SSB_XO_POLY_IN:   /* 0x10, 0xDD */
         {
            tSIRF_MSG_SSB_XO_POLY_IN *msg = (tSIRF_MSG_SSB_XO_POLY_IN *) message_structure;

            if ( payload_length < (tSIRF_UINT32)(SIRF_MSG_SSB_XO_POLY_IN_LEN + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->C[0]               = SIRFBINARY_IMPORT_UINT32(ptr);    /** polynomial co-efficients */
               msg->C[1]               = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->C[2]               = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->C[3]               = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->tempUnc            = SIRFBINARY_IMPORT_UINT16(ptr);    /** temperature uncertainty */
               msg->polySource         = SIRFBINARY_IMPORT_UINT8 (ptr);    /** Unused. Source of the initial Polynomial values */
               msg->spare1             = SIRFBINARY_IMPORT_UINT8 (ptr);
               msg->spare2             = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;
#endif /* XO_ENABLED */

         case SIRF_MSG_SSB_EE_SEA_PROVIDE_EPH:  /* 0x01 0xE8 */
         {
            tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH *msg = (tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH*) message_structure;

             /* payload length is compared with the size of the structure, which is determined by
              *  adding all the fields*sizeof(field) of the structure,
              *  alrenatively it could be compared with sizeof(structure) */
            if ( payload_length < ( (8 + 7 * SV_PER_PACKET) * sizeof(tSIRF_UINT8) +
                                    (1 + 11 * SV_PER_PACKET) * sizeof(tSIRF_UINT16) +
                                    (8 * SV_PER_PACKET) * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->week = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->svid_mask = SIRFBINARY_IMPORT_UINT32(ptr);
               for( i = 0; i < SV_PER_PACKET; i++)
               {
                  msg->extended_ephemeris[i].PRN = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->extended_ephemeris[i].ephemerisValidityFlag = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->extended_ephemeris[i].URA = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->extended_ephemeris[i].IODE = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->extended_ephemeris[i].Crs  = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->extended_ephemeris[i].deltaN = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->extended_ephemeris[i].M0 = SIRFBINARY_IMPORT_SINT32(ptr);
                  msg->extended_ephemeris[i].Cuc = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->extended_ephemeris[i].eccen = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->extended_ephemeris[i].Cus = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->extended_ephemeris[i].sqrtA = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->extended_ephemeris[i].toe = SIRFBINARY_IMPORT_UINT16(ptr);
                  msg->extended_ephemeris[i].Cic = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->extended_ephemeris[i].omega0 = SIRFBINARY_IMPORT_SINT32(ptr);
                  msg->extended_ephemeris[i].Cis = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->extended_ephemeris[i].i0 = SIRFBINARY_IMPORT_SINT32(ptr);
                  msg->extended_ephemeris[i].Crc = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->extended_ephemeris[i].w = SIRFBINARY_IMPORT_SINT32(ptr);
                  msg->extended_ephemeris[i].omegaDot = SIRFBINARY_IMPORT_SINT32(ptr);
                  msg->extended_ephemeris[i].iDot = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->extended_ephemeris[i].toc = SIRFBINARY_IMPORT_UINT16(ptr);
                  msg->extended_ephemeris[i].Tgd = SIRFBINARY_IMPORT_SINT8(ptr);
                  msg->extended_ephemeris[i].af2 = SIRFBINARY_IMPORT_SINT8(ptr);
                  msg->extended_ephemeris[i].af1 = SIRFBINARY_IMPORT_SINT16(ptr);
                  msg->extended_ephemeris[i].af0 = SIRFBINARY_IMPORT_SINT32(ptr);
                  msg->extended_ephemeris[i].age = SIRFBINARY_IMPORT_UINT8(ptr);
               }
               msg->extended_iono.alpha[0] = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->extended_iono.alpha[1] = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->extended_iono.alpha[2] = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->extended_iono.alpha[3] = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->extended_iono.beta[0] = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->extended_iono.beta[1] = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->extended_iono.beta[2] = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->extended_iono.beta[3] = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_EE_POLL_STATE: /* 0x02 0xE8 */
         {
            tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH *msg = (tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH*) message_structure;

            if ( payload_length < ( 1 * sizeof(tSIRF_UINT16) +
                                    1 * sizeof(tSIRF_UINT32) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->week      = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->svid_mask = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_EE_QUERY_AGE:     /* 0x11 0xE8 */
         {
         }
         break;

         case SIRF_MSG_SSB_EE_DEBUG:
         {
            tSIRF_MSG_SSB_EE_PROPRIETARY_DEBUG *msg = (tSIRF_MSG_SSB_EE_PROPRIETARY_DEBUG*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->debug_flags = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;
#ifdef EMB_SIF
         case SIRF_MSG_SSB_SIF_START_DLD:
         {
            if ( payload_length < header_len )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(tSIRF_MSG_SSB_SIF_START_DLD);
            }
         }
         break;
         case SIRF_MSG_SSB_SIF_GET_NVM_HEADER:
         {
            if ( payload_length < header_len )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(tSIRF_MSG_SSB_SIF_GET_NVM_HEADER);
            }
         }
         break;
         case SIRF_MSG_SSB_SIF_UPDATE_NVM_HEADER:
         {
            tSIRF_MSG_SSB_SIF_UPDATE_NVM_HEADER *msg = (tSIRF_MSG_SSB_SIF_UPDATE_NVM_HEADER*) message_structure;
            if ( payload_length < (1 * sizeof(tSIRF_UINT32) + 2 * sizeof(tSIRF_UINT16) + header_len ))
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               msg->seqNum    = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->size      = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->offset    = SIRFBINARY_IMPORT_UINT32(ptr);
               if( payload_length < (1 * sizeof(tSIRF_UINT32) + (2 * sizeof(tSIRF_UINT16) +
                                    msg->size * sizeof(tSIRF_UINT8)+ header_len )))
               {
                   tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  if (msg->size > SSB_SGEE_MAX_PKT_LEN)
                  {
                     tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
                  }
                  else
                  {
                     for(i=0;i<msg->size;i++)
                     {
                        msg->pktData[i] = SIRFBINARY_IMPORT_UINT8(ptr);
                     }
                     *message_length = sizeof(*msg);
                  }
               }
            }
         }
         break;
         case SIRF_MSG_SSB_SIF_EE_FILE_SIZE:
         {
            tSIRF_MSG_SSB_SIF_EE_FILE_SIZE *msg = (tSIRF_MSG_SSB_SIF_EE_FILE_SIZE*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->file_size   = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_SIF_PKT_DATA:
         {
            tSIRF_MSG_SSB_SIF_PKT_DATA *msg = (tSIRF_MSG_SSB_SIF_PKT_DATA*) message_structure;

            if ( payload_length < (2 * sizeof(tSIRF_UINT16) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               msg->pktSeqNo   = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->pktLength   = SIRFBINARY_IMPORT_UINT16(ptr);
               if ( payload_length < (msg->pktLength * sizeof(tSIRF_UINT8) +
                               2 * sizeof(tSIRF_UINT16) +
                               header_len) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else if (msg->pktLength > SSB_SGEE_MAX_PKT_LEN)
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  for(i=0;i<msg->pktLength;i++)
                  {
                     msg->pktData[i] = SIRFBINARY_IMPORT_UINT8(ptr);
                  }
               }
            }
         }
         break;
         case SIRF_MSG_SSB_SIF_GET_EE_AGE:
         {
            tSIRF_MSG_SSB_SIF_GET_EE_AGE *msg = (tSIRF_MSG_SSB_SIF_GET_EE_AGE*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               msg->numSAT   = SIRFBINARY_IMPORT_UINT8(ptr);
               if ( payload_length < ((3* sizeof(tSIRF_UINT8) + 6* sizeof(tSIRF_UINT16))* msg->numSAT  +
                                      1 * sizeof(tSIRF_UINT8) +
                                      header_len) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else if (msg->numSAT > SIRF_MAX_SVID_CNT)
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  for(i=0;i<msg->numSAT;i++)
                  {
                     msg->eeAgeStruct[i].prnNum = SIRFBINARY_IMPORT_UINT8(ptr);
                     msg->eeAgeStruct[i].ephPosFlag = SIRFBINARY_IMPORT_UINT8(ptr);
                     msg->eeAgeStruct[i].eePosAge = SIRFBINARY_IMPORT_UINT16(ptr);
                     msg->eeAgeStruct[i].cgeePosGPSWeek = SIRFBINARY_IMPORT_UINT16(ptr);
                     msg->eeAgeStruct[i].cgeePosTOE = SIRFBINARY_IMPORT_UINT16(ptr);
                     msg->eeAgeStruct[i].ephClkFlag = SIRFBINARY_IMPORT_UINT8(ptr);
                     msg->eeAgeStruct[i].eeClkAge = SIRFBINARY_IMPORT_UINT16(ptr);
                     msg->eeAgeStruct[i].cgeeClkGPSWeek = SIRFBINARY_IMPORT_UINT16(ptr);
                     msg->eeAgeStruct[i].cgeeClkTOE = SIRFBINARY_IMPORT_UINT16(ptr);
                  }
               }
            }
         }
         break;
         case SIRF_MSG_SSB_SIF_GET_SGEE_AGE:
         {
            tSIRF_MSG_SSB_SIF_GET_SGEE_AGE *msg = (tSIRF_MSG_SSB_SIF_GET_SGEE_AGE*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->satId   = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;
         case SIRF_MSG_SSB_HOST_RCV_PKT_DATA:
         {
            tSIRF_MSG_SSB_HOST_RCV_PKT_DATA *msg = (tSIRF_MSG_SSB_HOST_RCV_PKT_DATA*) message_structure;
            tSIRF_UINT16 testSize = 0;

            /* Find out received data length first to identify packet size*/

            if ( payload_length < ( 2 * sizeof(tSIRF_UINT8) + sizeof(tSIRF_UINT16) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               msg->seqNum     = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->NVMID      = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->numBlocks  = SIRFBINARY_IMPORT_UINT8(ptr);

               if ( payload_length < (msg->numBlocks * sizeof(tSIRF_UINT16)+ 2 * sizeof(tSIRF_UINT8) + sizeof(tSIRF_UINT16) + header_len) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else if (msg->numBlocks > MAX_RCV_BLOCKS)
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
               }
               else
               {
                  for(i=0; i<msg->numBlocks; i++)
                  {
                     msg->size[i]    = SIRFBINARY_IMPORT_UINT16(ptr);
                     msg->offset[i]  = SIRFBINARY_IMPORT_UINT32(ptr);
                     testSize       += msg->size[i];
                  }

                  if ( payload_length < (testSize * sizeof(tSIRF_UINT8) + msg->numBlocks * sizeof(tSIRF_UINT16)+ msg->numBlocks * sizeof(tSIRF_UINT32)
                                          + 2 * sizeof(tSIRF_UINT8) + sizeof(tSIRF_UINT16) + header_len) )
                  {
                     tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
                  }
                  else
                  {
                     *message_length = sizeof(*msg);
                     for(i=0;i<testSize;i++)
                     {
                        msg->pktData[i]     = SIRFBINARY_IMPORT_UINT8(ptr);
                     }
                  }
               }
            }
         }
         break;
         case SIRF_MSG_SSB_HOST_ACK_NACK:
         {
            tSIRF_MSG_SSB_HOST_ACK_NACK *msg = (tSIRF_MSG_SSB_HOST_ACK_NACK*) message_structure;

            if ( payload_length < (4 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               msg->ackMsgId =  SIRFBINARY_IMPORT_UINT8(ptr);
               msg->ackSid   =  SIRFBINARY_IMPORT_UINT8(ptr);
               msg->ackNack   =  SIRFBINARY_IMPORT_UINT8(ptr);
               msg->reason   =  SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;
#ifdef EE_I2C_FILE_TEST
         case SIRF_MSG_SSB_READ_I2CDATA:
         {
            tSIRF_MSG_SSB_READ_I2CData *msg = (tSIRF_MSG_SSB_READ_I2CData*) message_structure;

            if ( payload_length < (sizeof(tSIRF_UINT32) + sizeof(tSIRF_UINT16) + sizeof(tSIRF_UINT8) + header_len))
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               msg->offset   =  SIRFBINARY_IMPORT_UINT32(ptr);
               msg->length   =  SIRFBINARY_IMPORT_UINT16(ptr);
               msg->eeType   =  SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;
#endif
#endif /*EMB_SIF*/

#ifdef PVT_BUILD
         case SIRF_MSG_SSB_EE_STORAGE_CONTROL: /* 0xFD 0xE8 */
         {
            tSIRF_MSG_SSB_EE_STORAGE_CONTROL *msg = (tSIRF_MSG_SSB_EE_STORAGE_CONTROL*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->ee_storage_ctrl = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         } /* end of case SIRF_MSG_SSB_EE_STORAGE_CONTROL */
         break;
#endif /* #ifdef PVT_BUILD */

         case SIRF_MSG_SSB_EE_FILE_DOWNLOAD:
         {
            tSIRF_MSG_SSB_EE_FILE_DOWNLOAD *msg = (tSIRF_MSG_SSB_EE_FILE_DOWNLOAD*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->reserved   = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_SIF_SET_CONFIG:     /* 0xE8 0xFC */
         {
            tSIRF_MSG_SSB_SIF_SET_CONFIG *msg = (tSIRF_MSG_SSB_SIF_SET_CONFIG*) message_structure;

            if ( payload_length < ( 5 * sizeof(tSIRF_UINT8) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->operation_mode        = SIRFBINARY_IMPORT_UINT8(  ptr );
               msg->file_format           = SIRFBINARY_IMPORT_UINT8(  ptr );
               msg->ext_gps_time_src      = SIRFBINARY_IMPORT_UINT8(  ptr );
               msg->cgee_input_method     = SIRFBINARY_IMPORT_UINT8(  ptr );
               msg->sgee_input_method     = SIRFBINARY_IMPORT_UINT8(  ptr );
            }
         }
         break;

      case SIRF_MSG_SSB_EE_FILE_PART:     /* 0x12 0xE8*/
      {
         tSIRF_MSG_SSB_EE_FILE_PART *msg = (tSIRF_MSG_SSB_EE_FILE_PART*) message_structure;

         if ( payload_length < ( 4 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            msg->buffSize = SIRFBINARY_IMPORT_UINT32 (ptr);
            if (( payload_length < ( (4 + msg->buffSize) * sizeof(tSIRF_UINT8) +header_len) )
                    || (msg->buffSize > SSB_DLD_MAX_PKT_LEN))
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               for(i = 0; i < msg->buffSize; i++)
               {
                   msg->buff[i] = SIRFBINARY_IMPORT_UINT8 (ptr);
               }
            }
         }
      }
      break;

         case SIRF_MSG_SSB_EE_DISABLE_EE_SECS: /* 0xFE 0xE8 */
         {
            tSIRF_MSG_SSB_EE_DISABLE_EE_SECS *msg = (tSIRF_MSG_SSB_EE_DISABLE_EE_SECS*) message_structure;

            if ( payload_length < (1 * sizeof(tSIRF_UINT32) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->num_secs   = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;

#ifdef EMB_SIF
      case SIRF_MSG_SSB_SIF_DISABLE_AIDING: /* 0x20 0xE8 */
      {
         tSIRF_MSG_SSB_SIF_DISABLE_AIDING *msg = (tSIRF_MSG_SSB_SIF_DISABLE_AIDING*) message_structure;

         if ( payload_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            msg->sgeeDisable = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->cgeeDisable = SIRFBINARY_IMPORT_UINT8(ptr);
         }
      }
      break;

      case SIRF_MSG_SSB_SIF_GET_AIDING_STATUS: /* 0x21 0xE8 */
      {
         tSIRF_MSG_SSB_SIF_GET_AIDING_STATUS *msg = (tSIRF_MSG_SSB_SIF_GET_AIDING_STATUS*)message_structure;

         if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            msg->reserved = SIRFBINARY_IMPORT_UINT8(ptr);
         }
      }
      break;

#endif /*EMB_SIF*/

         case SIRF_MSG_SSB_SENSOR_CONFIG:    /* 0xEA 0x1 */
         {
            tSIRF_MSG_SSB_SENSOR_CONFIG *msg = (tSIRF_MSG_SSB_SENSOR_CONFIG*) message_structure;
            tSIRF_UINT32 length_requirement = (3 * sizeof(tSIRF_UINT8) + header_len);

            if ( payload_length < length_requirement )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->numSensors = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->i2cSpeed   = SIRFBINARY_IMPORT_UINT8(ptr);
               length_requirement += (( (14* sizeof(tSIRF_UINT8)) + (3* sizeof(tSIRF_UINT16)) ) * msg->numSensors);
               if ( payload_length < length_requirement )
               {
                   tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  for(i = 0; (i < msg->numSensors) && (i < SIRF_MSG_SSB_MAX_NUM_SENSORS); i++)
                  {
                      msg->Sensors[i].i2cAddress = SIRFBINARY_IMPORT_UINT16(ptr);
                      msg->Sensors[i].sensorType = SIRFBINARY_IMPORT_UINT8(ptr);
                      msg->Sensors[i].initTime = SIRFBINARY_IMPORT_UINT8(ptr);
                      msg->Sensors[i].nBytesResol = SIRFBINARY_IMPORT_UINT8(ptr);
                      msg->Sensors[i].sampRate = SIRFBINARY_IMPORT_UINT8(ptr);
                      msg->Sensors[i].sendRate = SIRFBINARY_IMPORT_UINT8(ptr);
                      msg->Sensors[i].decmMethod = SIRFBINARY_IMPORT_UINT8(ptr);
                      msg->Sensors[i].acqTime = SIRFBINARY_IMPORT_UINT8(ptr);
                      msg->Sensors[i].numReadReg = SIRFBINARY_IMPORT_UINT8(ptr);
                      msg->Sensors[i].measState = SIRFBINARY_IMPORT_UINT8(ptr);
                      if((msg->Sensors[i].numReadReg > SIRF_MSG_SSB_MAX_SENSOR_READ_REGS) ||
                         (0 == msg->Sensors[i].numReadReg)                                 )
                      {
                         tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
                         break;
                      }
                      else
                      {
                         length_requirement += (2 * msg->Sensors[i].numReadReg) * sizeof(tSIRF_UINT8);
                         if ( payload_length < length_requirement )
                         {
                            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
                            break;
                         }
                         else
                         {
                            for(j=0;j<msg->Sensors[i].numReadReg;j++)
                            {
                                msg->Sensors[i].sensorReadReg[j].readOprMethod = SIRFBINARY_IMPORT_UINT8(ptr);
                                msg->Sensors[i].sensorReadReg[j].dataReadReg = SIRFBINARY_IMPORT_UINT8(ptr);
                            }
                            msg->Sensors[i].pwrCtrlReg = SIRFBINARY_IMPORT_UINT8(ptr);
                            msg->Sensors[i].pwrOffSetting = SIRFBINARY_IMPORT_UINT8(ptr);
                            msg->Sensors[i].pwrOnSetting = SIRFBINARY_IMPORT_UINT8(ptr);
                            msg->Sensors[i].numInitReadReg = SIRFBINARY_IMPORT_UINT8(ptr);
                            if(msg->Sensors[i].numInitReadReg > SIRF_MSG_SSB_MAX_SENSOR_INIT_REGS)
                            {
                               tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
                               break;
                            }
                            else
                            {
                               length_requirement += (2* msg->Sensors[i].numInitReadReg) * sizeof(tSIRF_UINT8);
                               if ( payload_length < length_requirement )
                               {
                                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
                                  break;
                               }
                               else
                               {
                                  for(j=0;j<msg->Sensors[i].numInitReadReg;j++)
                                  {
                                     msg->Sensors[i].sensorInitReg[j].address = SIRFBINARY_IMPORT_UINT8(ptr);
                                     msg->Sensors[i].sensorInitReg[j].nBytes = SIRFBINARY_IMPORT_UINT8(ptr);
                                  }
                                  msg->Sensors[i].numCtrlReg = SIRFBINARY_IMPORT_UINT8(ptr);
                                  if(msg->Sensors[i].numCtrlReg > SIRF_MSG_SSB_MAX_SENSOR_CTRL_REGS)
                                  {
                                     tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
                                     break;
                                  }
                                  else
                                  {
                                     msg->Sensors[i].ctrlRegWriteDelay = SIRFBINARY_IMPORT_UINT8(ptr);
                                     length_requirement += ((2* msg->Sensors[i].numCtrlReg) *sizeof(tSIRF_UINT8));
                                     if ( payload_length < length_requirement )
                                     {
                                        tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
                                        break;
                                     }
                                     else
                                     {
                                        for(j=0;j<msg->Sensors[i].numCtrlReg;j++)
                                        {
                                           msg->Sensors[i].sensorCtrlReg[j].address = SIRFBINARY_IMPORT_UINT8(ptr);
                                           msg->Sensors[i].sensorCtrlReg[j].value = SIRFBINARY_IMPORT_UINT8(ptr);
                                        }
                                     } /* else */
                                  } /* else */
                               }/* else */
                            }/* else */
                         }/* else */
                      }/* else */
                  }/* for loop = numSensors */
                  if(SIRF_CODEC_ERROR_INVALID_MSG_LENGTH != tRet)
                  {
                     msg->processingRate = SIRFBINARY_IMPORT_UINT8(ptr);
                     for(i=0;i<msg->numSensors;i++)
                     {
                         msg->sensorScaleZeroPointVal[i].zeroPointVal = SIRFBINARY_IMPORT_UINT16(ptr);
                         msg->sensorScaleZeroPointVal[i].scaleFactor = SIRFBINARY_IMPORT_UINT16(ptr);
                     }
                  }
               } /* else */
            } /* else */
         } /* case */
         break;

         case SIRF_MSG_SSB_SENSOR_SWITCH:    /* 0xEA 0x2 */
          {
            tSIRF_MSG_SSB_SENSOR_SWITCH *msg = (tSIRF_MSG_SSB_SENSOR_SWITCH*) message_structure;

            if ( payload_length < (sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               tSIRF_UINT8 validMask = SENS_STATE_ENABLE | SENS_STATE_NOTIFY;

               *message_length = sizeof(*msg);
                msg->sensorSetState   = SIRFBINARY_IMPORT_UINT8(ptr);
                if((~validMask) & msg->sensorSetState)
                {
                   tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
                }
            }
         }
         break;

#ifdef SENS_SSB_DATA_INPUT_MODE
        case  SIRF_MSG_SSB_SENSOR_READINGS:             /*0x48, 0x1*/
        {
             tSIRF_MSG_SSB_SENSOR_READINGS *msg = (tSIRF_MSG_SSB_SENSOR_READINGS*) message_structure;

             tSIRF_UINT32 length_requirement = (3 * sizeof(tSIRF_UINT8)) + sizeof(tSIRF_UINT16) + header_len;

             if ( payload_length < length_requirement )
             {
                tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
             }
             else
             {
               *message_length = sizeof(*msg);
               msg->sensorID = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->dataLength   = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->numDataSet   = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->dataMode     = SIRFBINARY_IMPORT_UINT8(ptr);
               length_requirement += msg->numDataSet * ( sizeof(tSIRF_UINT32) + (msg->dataLength * sizeof(tSIRF_UINT8)) );

               if ( payload_length < length_requirement )
               {
                 tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                 for(i=0;i<msg->numDataSet;i++)
                 {
                   msg->dataSet[i].timeTag = SIRFBINARY_IMPORT_UINT32(ptr);
                   memcpy(msg->dataSet[i].data, ptr,msg->dataLength);
                   ptr += msg->dataLength;
                 }
               }
             }
         }
         break;
        case SIRF_MSG_SSB_RCVR_STATE:             /*0x48, 0x3*/
        {
            tSIRF_MSG_SBB_RCVR_STATE *msg = (tSIRF_MSG_SBB_RCVR_STATE*) message_structure;

            if ( payload_length < (sizeof(tSIRF_UINT32) + sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
                msg->timeStamp   = SIRFBINARY_IMPORT_UINT32(ptr);
                msg->rcvrPhysicalState   = SIRFBINARY_IMPORT_UINT8(ptr);
            }
        }
        break;

        case SIRF_MSG_SSB_POINT_N_TELL_OUTPUT:             /*0x48, 0x4*/
        {
            tSIRF_MSG_SSB_POINT_N_TELL_OUTPUT *msg = (tSIRF_MSG_SSB_POINT_N_TELL_OUTPUT*) message_structure;

            if ( payload_length < ( 3 * sizeof(tSIRF_UINT32) + 6 * sizeof(tSIRF_UINT16) + sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
                msg->timeStamp = SIRFBINARY_IMPORT_UINT32(ptr);
                msg->latitude = SIRFBINARY_IMPORT_SINT32(ptr);
                msg->longitude = SIRFBINARY_IMPORT_SINT32(ptr);
                msg->heading = SIRFBINARY_IMPORT_UINT16(ptr);
                msg->pitch = SIRFBINARY_IMPORT_SINT16(ptr);
                msg->roll = SIRFBINARY_IMPORT_SINT16(ptr);
                msg->headingUnc = SIRFBINARY_IMPORT_UINT16(ptr);
                msg->pitchUnc = SIRFBINARY_IMPORT_UINT16(ptr);
                msg->rollUnc = SIRFBINARY_IMPORT_UINT16(ptr);
                msg->status = SIRFBINARY_IMPORT_UINT8(ptr);
            }
        }
        break;

#endif /*SENS_SSB_DATA_INPUT_MODE*/

      case SIRF_MSG_SSB_SIRF_STATS:    /* 0xE1 0x06 */
         {
            tSIRF_MSG_SSB_SIRF_STATS *msg = (tSIRF_MSG_SSB_SIRF_STATS *) message_structure;
            tSIRF_UINT32 length_requirement = ( 4 * sizeof(tSIRF_UINT32) +
                                                6 * sizeof(tSIRF_UINT16) +
                                                9 * sizeof(tSIRF_UINT8) +
                                                header_len );

            if ( payload_length < length_requirement )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length = sizeof(*msg);
               msg->ttff_since_reset       = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->ttff_since_aiding      = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->ttff_first_nav         = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->pos_aiding_error_north = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->pos_aiding_error_east  = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->pos_aiding_error_down  = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->time_aiding_error      = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->freq_aiding_error      = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->hor_pos_uncertainty    = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->ver_pos_uncertainty    = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->time_uncertainty       = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->freq_uncertainty       = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->num_aided_ephemeris    = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->num_aided_acq_assist   = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->nav_mode               = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->pos_mode               = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->nav_status             = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->start_mode             = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->aiding_status          = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;

         case SIRF_MSG_SSB_TRKR_DBG:   /* 0x44 */
         case SIRF_MSG_SSB_TEXT:       /* 0xFF */
         {
            *message_length = payload_length - 1;
            memcpy(message_structure, ptr, payload_length - 1);
            *((char*) message_structure + payload_length - 1) = '\0';
            ptr += payload_length - 1;
         }
         break;

         default:
         {
#ifdef SIRF_AGPS
             tRet = SIRF_CODEC_SSB_AGPS_Decode(payload,payload_length,message_id,
                                                 message_structure,message_length );
             if ( SIRF_SUCCESS == tRet)
                {
                   ptr = payload + payload_length;
                }
#else
            if (( *message_id >= SIRF_MSG_SSB_PASSTHRU_INPUT_BEGIN   /* 0xB4 */
                 && *message_id <= SIRF_MSG_SSB_PASSTHRU_INPUT_END   /* 0xC7 */
                 && payload_length - 1 <= SIRF_MSG_SSB_MAX_MESSAGE_LEN
               && payload_length > 0)
             || (SIRF_MSG_SSB_SIRF_INTERNAL     == *message_id)  /* 0xE4 */
             || (SIRF_MSG_SSB_SIRF_INTERNAL_OUT == *message_id)) /* 0xE1 */
            { /* pass-thru */
               *message_length = payload_length - 1;
               memcpy(message_structure, ptr, payload_length - 1);
               ptr += payload_length - 1;
            }
            else
            { /* this message id is not imported */
               tRet = SIRF_CODEC_SSB_INVALID_MSG_ID;
            }
#endif /* SIRF_AGPS */
         }
         break;
      }

      /* check if length does not match */
      if ( (tRet == SIRF_SUCCESS) && (ptr != (payload + payload_length)))
      {
         *message_id = *message_length = 0;
         tRet = SIRF_CODEC_SSB_LENGTH_ERROR;
      }
   }
   else
   {
      tRet = SIRF_CODEC_SSB_NULL_POINTER;
   }

   *options = SIRF_CODEC_OPTIONS_MAKE_MSG_NUMBER(1,1); /* Message 1 of 1 */

   return tRet;

} /* SIRF_CODEC_SSB_Decode() */

/**
 * @}
 */
