/**
 * @addtogroup platform_src_sirf_util_codec
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2005-2010 by SiRF Technology, a CSR plc Company
 *    All rights reserved.
 *
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

#ifndef SIRF_AGPS
#error SIRF_AGPS is not defined when compiling this file
#endif

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
tSIRF_RESULT SIRF_CODEC_SSB_AGPS_Encode( tSIRF_UINT32 message_id,
                                    tSIRF_VOID *message_structure,
                                    tSIRF_UINT32 message_length,
                                    tSIRF_UINT8 *packet,
                                    tSIRF_UINT32 *packet_length )
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
      tSIRF_UINT8 i;
      tSIRF_UINT8 *ptr = packet;
      tSIRF_UINT8 header_len = 0;

      if (SIRF_GET_SUB_ID(message_id))
      {
         header_len++; /* start with sid byte count */
      }
      header_len += 1 * sizeof(tSIRF_UINT8); /* mid byte only */
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
       case  SIRF_MSG_SSB_POS_RSP: /* 0x45 ,0x01 */
       {
         /* Construct a pointer to the structure. */
         const tSIRF_MSG_SSB_POSITION_RSP* msg = (const tSIRF_MSG_SSB_POSITION_RSP *)message_structure;
  
         /**
          * Attention programmers: the code compares the count of data bytes to *packet_length:
          * it generates the count by scanning the data store and adding up all of the 
          * UINT8s then all of the UINT16s, then all of the UINT32s.
          */

         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( (20 + 3 * SIRF_MSG_SSB_RSP_NUM_SV_INFO) * sizeof(tSIRF_UINT8) + 
                                 (6* sizeof(tSIRF_UINT16) + 3 * sizeof(tSIRF_UINT32) ) +
                                    header_len) ))
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }
            
         SIRFBINARY_EXPORT8(msg->pos_req_id,ptr);
         SIRFBINARY_EXPORT8(msg->pos_results_flag,ptr);
         SIRFBINARY_EXPORT8(msg->pos_err_status,ptr);
         SIRFBINARY_EXPORT8(msg->qos_pass_flag,ptr);
         SIRFBINARY_EXPORT8(msg->pos_type,ptr);
         SIRFBINARY_EXPORT8(msg->dgps_cor,ptr); 
         SIRFBINARY_EXPORT16(msg->gps_week,ptr);
         SIRFBINARY_EXPORT32(msg->time_of_week,ptr);
         SIRFBINARY_EXPORT32(msg->lat,ptr);
         SIRFBINARY_EXPORT32(msg->lon,ptr);

         SIRFBINARY_EXPORT8(msg->other_sections,ptr);
   
         /* Horizontal Error Section */
         SIRFBINARY_EXPORT8(msg->hor_err_angle,ptr);
         SIRFBINARY_EXPORT8(msg->hor_err_major,ptr);
         SIRFBINARY_EXPORT8(msg->hor_err_minor,ptr);
   
         /* Vertical Position Section */
         SIRFBINARY_EXPORT16(msg->vert_pos_height,ptr);
         SIRFBINARY_EXPORT8(msg->vert_pos_std,ptr);
   
         /* Velocity Section */
         SIRFBINARY_EXPORT16(msg->velocity_horizontal,ptr);
         SIRFBINARY_EXPORT16(msg->velocity_heading,ptr);
         SIRFBINARY_EXPORT8(msg->velocity_vertical,ptr);
         SIRFBINARY_EXPORT8(msg->velocity_hor_err_ang,ptr);
         SIRFBINARY_EXPORT8(msg->velocity_hor_err_maj_std,ptr);
         SIRFBINARY_EXPORT8(msg->velocity_hor_err_min_std,ptr);
         SIRFBINARY_EXPORT8(msg->velocity_ver_err,ptr);
   
         /* Clock Correction Section */
         SIRFBINARY_EXPORT8(msg->clk_cor_time_ref,ptr);
         SIRFBINARY_EXPORT16(msg->clk_cor_clk_bias,ptr);
         SIRFBINARY_EXPORT16(msg->clk_cor_clk_drift,ptr);
         SIRFBINARY_EXPORT8(msg->clk_cor_clk_std_err,ptr);
         SIRFBINARY_EXPORT8(msg->clk_cor_utc_off,ptr);  
   
         /* Position Correction Section */
         SIRFBINARY_EXPORT8(msg->pos_corr_num_sat,ptr);
         for (i = 0; i < SIRF_MSG_SSB_RSP_NUM_SV_INFO; i++)
         {
            SIRFBINARY_EXPORT8(msg->pos_corr_sv_info[i].sv_prn,ptr);
            SIRFBINARY_EXPORT8(msg->pos_corr_sv_info[i].c_n0,ptr);
            SIRFBINARY_EXPORT8(msg->pos_corr_sv_info[i].inv_weights,ptr);
         }

      }
      break;
    case  SIRF_MSG_SSB_MEAS_RSP: /* 0x45 ,0x02 */
      {
         /* Construct a pointer to the structure. */
         const tSIRF_MSG_SSB_MEAS_RSP* msg = (const tSIRF_MSG_SSB_MEAS_RSP *)message_structure;
  
         /**
          * Attention programmers: the code compares the count of data bytes to *packet_length:
          * it generates the count by scanning the data store and adding up all of the 
          * UINT8s then all of the UINT16s, then all of the UINT32s.
          */
         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < (  5 * sizeof(tSIRF_UINT8) + 
                                   1 * sizeof(tSIRF_UINT16) + 
                                   1 * sizeof(tSIRF_UINT32) +
                                    header_len) ))
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

            /* ICD 2.0 */
         SIRFBINARY_EXPORT8(msg->pos_req_id,ptr);
         SIRFBINARY_EXPORT8(msg->gps_meas_flag,ptr);
         SIRFBINARY_EXPORT8(msg->meas_error_status,ptr);
         SIRFBINARY_EXPORT16(msg->meas_gps_week,ptr);
         SIRFBINARY_EXPORT32(msg->meas_gps_seconds,ptr);
         SIRFBINARY_EXPORT8(msg->time_accuracy,ptr);
         SIRFBINARY_EXPORT8(msg->num_sv,ptr);
         if ( ( *packet_length <  ( 5 * sizeof(tSIRF_UINT8) + 
                                 ( 4 * msg->num_sv) * sizeof(tSIRF_UINT8) +
                                   1 * sizeof(tSIRF_UINT16) + 
                                 ( 3 * msg->num_sv) * sizeof(tSIRF_UINT16) + 
                                   1 * sizeof(tSIRF_UINT32) +
                                 header_len ) ) ||
              ( msg->num_sv > SIRF_MSG_SSB_MEASUREMENT_DATA_MAX_SVS ) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }
         else
         {            
            for (i = 0; i < msg->num_sv; i++)
            {
               SIRFBINARY_EXPORT8(msg->meas_per_sv_data[i].sv_prn,ptr);
               SIRFBINARY_EXPORT8(msg->meas_per_sv_data[i].c_n0,ptr);
               SIRFBINARY_EXPORT16(msg->meas_per_sv_data[i].sv_doppler,ptr);
               SIRFBINARY_EXPORT16(msg->meas_per_sv_data[i].sv_code_phase_wh,ptr);
               SIRFBINARY_EXPORT16(msg->meas_per_sv_data[i].sv_code_phase_fr,ptr);
               SIRFBINARY_EXPORT8(msg->meas_per_sv_data[i].multipath_indicator,ptr);
               SIRFBINARY_EXPORT8(msg->meas_per_sv_data[i].pseudorange_rms_error,ptr);
            } /* i=0 -> msg->num_sv */ 
         }
      }
    break;
    case  SIRF_MSG_SSB_PARTPOS_RSP: /* 0x45, 0x03 */
      {
         /* Construct a pointer to the structure. */
         const tSIRF_MSG_SSB_PARTPOS_RSP* msg = (const tSIRF_MSG_SSB_PARTPOS_RSP *)message_structure;

         SIRFBINARY_EXPORT8(msg->pos_req_id,               ptr);
         SIRFBINARY_EXPORT8(msg->pos_results_flag,         ptr);
         SIRFBINARY_EXPORT8(msg->pos_err_status,           ptr);
         SIRFBINARY_EXPORT16(msg->qos_value,               ptr);
         SIRFBINARY_EXPORT8(msg->pos_type,                 ptr);
         SIRFBINARY_EXPORT8(msg->dgps_cor,                 ptr);
         SIRFBINARY_EXPORT16(msg->gps_week,                ptr);
         SIRFBINARY_EXPORT32(msg->lat,                     ptr);
         SIRFBINARY_EXPORT32(msg->lon,                     ptr);
         SIRFBINARY_EXPORT16(msg->vert_pos_height,         ptr);
         /* Velocity Section */
         SIRFBINARY_EXPORT16(msg->velocity_horizontal,     ptr);
         SIRFBINARY_EXPORT16(msg->velocity_heading,        ptr);
         SIRFBINARY_EXPORT8(msg->velocity_vertical,        ptr);
         /* Measurements Section */
         SIRFBINARY_EXPORT_DOUBLE(msg->meas_time,          ptr);
         SIRFBINARY_EXPORT8 (msg->num_sv_meas_cnt,         ptr);
         for (i = 0; i < msg->num_sv_meas_cnt; i++)
         {
            SIRFBINARY_EXPORT8(msg->sat_meas[i].svid,               ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->sat_meas[i].pseudorange,  ptr);
            SIRFBINARY_EXPORT32(msg->sat_meas[i].carrier_freq,      ptr);
            SIRFBINARY_EXPORT8(msg->sat_meas[i].sync_flags,         ptr);
            SIRFBINARY_EXPORT8(msg->sat_meas[i].ctoN,               ptr);
         }
         /* Sat States Section */
         SIRFBINARY_EXPORT_DOUBLE(msg->state_time,         ptr);
         SIRFBINARY_EXPORT8(msg->num_sv_state_cnt,         ptr);
         for (i = 0; i < msg->num_sv_state_cnt; i++)
         {
            SIRFBINARY_EXPORT8(msg->sat_state[i].svid,                 ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->sat_state[i].pos[0],         ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->sat_state[i].pos[1],         ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->sat_state[i].pos[2],         ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->sat_state[i].vel[0],         ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->sat_state[i].vel[1],         ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->sat_state[i].vel[2],         ptr);
            SIRFBINARY_EXPORT_DOUBLE(msg->sat_state[i].clk_bias,       ptr);
            SIRFBINARY_EXPORT32(msg->sat_state[i].clk_drift,           ptr);
            SIRFBINARY_EXPORT32(msg->sat_state[i].pos_var,             ptr);
            SIRFBINARY_EXPORT32(msg->sat_state[i].clk_var,             ptr);
            SIRFBINARY_EXPORT32(msg->sat_state[i].iono,                ptr);
            SIRFBINARY_EXPORT8(msg->sat_state[i].status,               ptr);
         }
      }
    break;
    case  SIRF_MSG_SSB_EPH_RSP:  /* 0x46 ,0x01 */
      {
         /* Construct a pointer to the structure. */
         const tSIRF_MSG_SSB_SLC_EPH_STATUS_RSP * msg = 
            (const tSIRF_MSG_SSB_SLC_EPH_STATUS_RSP *)message_structure;
         
         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < (  4 * sizeof(tSIRF_UINT8) + 
                                   1 * sizeof(tSIRF_UINT16) + 
                                   1 * sizeof(tSIRF_UINT32) +
                                    header_len ) ) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

         /* Export members. */
         SIRFBINARY_EXPORT8(msg->gps_time_mask,ptr);
         SIRFBINARY_EXPORT16(msg->gps_week,ptr);
         SIRFBINARY_EXPORT32(msg->gps_tow,ptr);
         SIRFBINARY_EXPORT8(msg->eph_status_type,ptr);
         SIRFBINARY_EXPORT8(msg->gps_t_toe_limit,ptr);
         SIRFBINARY_EXPORT8(msg->num_sv,ptr);
         if ( ( *packet_length < ( 4 * sizeof(tSIRF_UINT8) + 
                                 ( 4 * msg->num_sv ) * sizeof(tSIRF_UINT8) +
                                   1 * sizeof(tSIRF_UINT16) + 
                                 ( 3 * msg->num_sv ) * sizeof(tSIRF_UINT16) +
                                   1 * sizeof(tSIRF_UINT32) + 
                                 header_len ) ) ||
              ( msg->num_sv > SIRF_MAX_SVID_CNT ) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }
         else
         {
            for (i = 0u; i < msg->num_sv; ++i) 
            {
               const tSIRF_MSG_SSB_EPH_INFO* eph = (const tSIRF_MSG_SSB_EPH_INFO*)&msg->eph_info[i];

               SIRFBINARY_EXPORT8(eph->svid,ptr);
               SIRFBINARY_EXPORT8(eph->sv_info_flag,ptr);
               SIRFBINARY_EXPORT16(eph->gps_week,ptr);
               SIRFBINARY_EXPORT16(eph->gps_toe,ptr);
               SIRFBINARY_EXPORT8(eph->iode,ptr);
               SIRFBINARY_EXPORT16(eph->azimuth,ptr);
               SIRFBINARY_EXPORT8(eph->elevation,ptr);
            } /* i=0 -> msg->num_sv */ 
         }
      }
      break;
         
    case  SIRF_MSG_SSB_ALM_RSP:  /* 0x46 ,0x02 */
      {
         /* Construct a pointer to the structure. */
         const tSIRF_MSG_SSB_ALMANAC_RSP * msg = (const tSIRF_MSG_SSB_ALMANAC_RSP *)message_structure;
         
         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( (2 + 3 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) + 
                                 (1 + 6 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) + 
                                 (4 * 3 * SIRF_MAX_SVID_CNT) /* tSIRF_UINT24 */ + 
                                  1 * sizeof(tSIRF_UINT32) +
                                    header_len)) )
         {     
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

         /* Export members. */
         SIRFBINARY_EXPORT8(msg->data_flag,ptr);
         SIRFBINARY_EXPORT16(msg->extended_gps_week,ptr);
         SIRFBINARY_EXPORT32(msg->gpstow,ptr);
         SIRFBINARY_EXPORT8(msg->num_sv,ptr);
         if( msg->num_sv > SIRF_MAX_SVID_CNT )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
         }
         else
         {
            for (i = 0u; i < msg->num_sv; ++i) 
            {
               const tSIRF_MSG_SSB_OS_ALMANAC* alm = (const tSIRF_MSG_SSB_OS_ALMANAC*) &msg->alm_per_sv_list[i];
               SIRFBINARY_EXPORT8(alm->valid_flag,ptr);
               SIRFBINARY_EXPORT8(alm->svprnNum,ptr);
               SIRFBINARY_EXPORT16(alm->week_num,ptr);
               SIRFBINARY_EXPORT16(alm->eccentricity,ptr);
               SIRFBINARY_EXPORT8(alm->toa,ptr);
               SIRFBINARY_EXPORT16(alm->delta_incl,ptr);
               SIRFBINARY_EXPORT16(alm->omegadot,ptr);
               SIRFBINARY_EXPORT24(alm->a_sqrt,ptr);
               SIRFBINARY_EXPORT24(alm->omega_0,ptr);
               SIRFBINARY_EXPORT24(alm->omega,ptr);
               SIRFBINARY_EXPORT24(alm->m0,ptr);
               SIRFBINARY_EXPORT16(alm->af0,ptr);
               SIRFBINARY_EXPORT16(alm->af1,ptr);
            }
         }
      }
      break;
    case  SIRF_MSG_SSB_B_EPH_RSP: /* 0x46 ,0x03 */
      {
         /* Construct a pointer to the structure. */
         const tSIRF_MSG_SSB_BE_RSP * msg = (const tSIRF_MSG_SSB_BE_RSP *)message_structure;

         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( (12 * sizeof(tSIRF_UINT8) + 
                                    1 * sizeof(tSIRF_UINT16) + 
                                    1 * sizeof(tSIRF_UINT32) + 
                                    header_len))) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

         SIRFBINARY_EXPORT8(msg->reserved,         ptr);
         SIRFBINARY_EXPORT8(msg->iono_flag,        ptr);
         SIRFBINARY_EXPORT8(msg->iono_data.alpha_0,ptr);
         SIRFBINARY_EXPORT8(msg->iono_data.alpha_1,ptr);
         SIRFBINARY_EXPORT8(msg->iono_data.alpha_2,ptr);
         SIRFBINARY_EXPORT8(msg->iono_data.alpha_3,ptr);
         SIRFBINARY_EXPORT8(msg->iono_data.beta_0, ptr);
         SIRFBINARY_EXPORT8(msg->iono_data.beta_1, ptr);
         SIRFBINARY_EXPORT8(msg->iono_data.beta_2, ptr);
         SIRFBINARY_EXPORT8(msg->iono_data.beta_3, ptr);
         SIRFBINARY_EXPORT8(msg->time_flag,        ptr);
         SIRFBINARY_EXPORT16(msg->extd_gps_week,   ptr);
         SIRFBINARY_EXPORT32(msg->gps_tow,         ptr);         
         SIRFBINARY_EXPORT8(msg->num_sv,        ptr);
         if ( ( *packet_length < (  12 * sizeof(tSIRF_UINT8) + 
                                   ( 7 * msg->num_sv) * sizeof(tSIRF_UINT8) +
                                     1 * sizeof(tSIRF_UINT16) + 
                                   (12 * msg->num_sv) * sizeof(tSIRF_UINT16) +
                                     1 * sizeof(tSIRF_UINT32) + 
                                   ( 8 * msg->num_sv) * sizeof(tSIRF_UINT32) + 
                                    header_len ) ) ||
              ( msg->num_sv > SIRF_MAX_SVID_CNT ) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }
         else
         {
            for (i = 0; i < msg->num_sv; ++i) 
            {
               const tSIRF_MSG_SSB_BE* eph = &msg->be[i];

               SIRFBINARY_EXPORT8 (eph->eph_flag,        ptr);
               SIRFBINARY_EXPORT8 (eph->health,          ptr);
               SIRFBINARY_EXPORT16(eph->gps_week,        ptr);
               SIRFBINARY_EXPORT8 (eph->eph.sv_prn_num,  ptr);
               SIRFBINARY_EXPORT8 (eph->eph.ura_ind,     ptr);
               SIRFBINARY_EXPORT8 (eph->eph.iode,        ptr);
               SIRFBINARY_EXPORT16(eph->eph.c_rs,        ptr);
               SIRFBINARY_EXPORT16(eph->eph.delta_n,     ptr);
               SIRFBINARY_EXPORT32(eph->eph.m0,          ptr);
               SIRFBINARY_EXPORT16(eph->eph.c_uc,        ptr);
               SIRFBINARY_EXPORT32(eph->eph.eccentricity,ptr);
               SIRFBINARY_EXPORT16(eph->eph.c_us,        ptr);
               SIRFBINARY_EXPORT32(eph->eph.a_sqrt,      ptr);
               SIRFBINARY_EXPORT16(eph->eph.toe,         ptr);
               SIRFBINARY_EXPORT16(eph->eph.c_ic,        ptr);
               SIRFBINARY_EXPORT32(eph->eph.omega_0,     ptr);
               SIRFBINARY_EXPORT16(eph->eph.c_is,        ptr);
               SIRFBINARY_EXPORT32(eph->eph.i0,          ptr);
               SIRFBINARY_EXPORT16(eph->eph.c_rc,        ptr);
               SIRFBINARY_EXPORT32(eph->eph.omega,       ptr);
               SIRFBINARY_EXPORT32(eph->eph.omegadot,    ptr);
               SIRFBINARY_EXPORT16(eph->eph.idot,        ptr);
               SIRFBINARY_EXPORT16(eph->eph.toc,         ptr);
               SIRFBINARY_EXPORT8 (eph->eph.t_gd,        ptr);
               SIRFBINARY_EXPORT8 (eph->eph.af2,         ptr);
               SIRFBINARY_EXPORT16(eph->eph.af1,         ptr);
               SIRFBINARY_EXPORT32(eph->eph.af0,         ptr);
            } /* i=0 to msg->num_sv; */
         }
      }
      break;
    case  SIRF_MSG_SSB_TIME_FREQ_APPROX_POS_RSP: /* 0x46 ,0x04 */
         {
            tSIRF_MSG_SSB_TIME_FREQ_APROX_POS_RSP * msg = (tSIRF_MSG_SSB_TIME_FREQ_APROX_POS_RSP *) message_structure;
            
            if ( *packet_length < ( 5 * sizeof(tSIRF_UINT8) +
                                    3 * sizeof(tSIRF_UINT16) +
                                    5 * sizeof(tSIRF_UINT32) +
                                    header_len) )
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
    case  SIRF_MSG_SSB_CH_LOAD_RSP: /* 0x46 ,0x05 */
         {
            tSIRF_MSG_SSB_CHANNEL_LOAD_RSP * msg = (tSIRF_MSG_SSB_CHANNEL_LOAD_RSP *) message_structure;
           if ( *packet_length < ( (3 + 1 * SIRF_NUM_CHANNELS) * sizeof(tSIRF_UINT8) + 
                                        header_len ) )                                              
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->port,               ptr);
               SIRFBINARY_EXPORT8(msg->total_load,         ptr);
               SIRFBINARY_EXPORT8(msg->number_of_channels, ptr);
               if(msg->number_of_channels > SIRF_NUM_CHANNELS)
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
               }
               else
               {
                  for (i = 0; i < msg->number_of_channels; i++)
                  {
                     tSIRF_MSG_SSB_CHANNEL_LOAD_RSP_ONE_CHANNEL * pMsg = 
                        (tSIRF_MSG_SSB_CHANNEL_LOAD_RSP_ONE_CHANNEL *) &msg->load_entry[i]; 
                     SIRFBINARY_EXPORT8(pMsg->load,         ptr);
                  }
               }
            }
         }
         break;
    case  SIRF_MSG_SSB_CLIENT_STATUS_RSP:  /* 0x46 ,0x06 */
         {
            tSIRF_MSG_SSB_SLC_STATUS * msg = (tSIRF_MSG_SSB_SLC_STATUS *) message_structure;
            
            if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->slc_status, ptr);
            }
         }
         break;
    case  SIRF_MSG_SSB_OSP_REV_RSP:  /* 0x46 ,0x07 */
         {
            tSIRF_MSG_SSB_OSP_REV_RSP* msg = (tSIRF_MSG_SSB_OSP_REV_RSP *) message_structure;
            
            if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->osp_revision, ptr);
            }
         }
         break;
    case  SIRF_MSG_SSB_SERIAL_SETTINGS_RSP:  /* 0x46 ,0x8 */
         {
            tSIRF_MSG_SSB_SERIAL_SETTING_RSP* msg = (tSIRF_MSG_SSB_SERIAL_SETTING_RSP *) message_structure;
            
            if ( *packet_length < ((2 * sizeof(tSIRF_UINT8) + 
                                                1 * sizeof(tSIRF_UINT32) + 
                                                header_len )) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8 (msg->port,       ptr);
               SIRFBINARY_EXPORT32(msg->baud,  ptr);
               SIRFBINARY_EXPORT8 (msg->ack_number, ptr);
            }
         }
         break;
    case  SIRF_MSG_SSB_TX_BLANKING_RESP:  /* 0x46 ,0x9 */
         {
            tSIRF_MSG_SSB_TX_BLANKING_RSP* msg = (tSIRF_MSG_SSB_TX_BLANKING_RSP *) message_structure;
               
            if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8 (msg->ack_nack,       ptr);
               SIRFBINARY_EXPORT8 (msg->spare, ptr);
            }
         }
         break;
    case  SIRF_MSG_SSB_HW_CONFIG_REQ: /* 0x47 */
         break;


    case  SIRF_MSG_SSB_APROX_MS_POS_REQ: /* 0x49 ,0x01 */
         break;

    case  SIRF_MSG_SSB_TIME_TX_REQ:   /* 0x49 ,0x02 */
         break;
    case  SIRF_MSG_SSB_FREQ_TX_REQ:
         {
            tSIRF_MSG_SSB_FREQ_TRANSFER_REQ * msg = (tSIRF_MSG_SSB_FREQ_TRANSFER_REQ *) message_structure;
            
            if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->req_info, ptr);
            }
         }
         break;
    case  SIRF_MSG_SSB_NBA_REQ:  /* 0x49 ,0x04 */
         {
            tSIRF_MSG_SSB_SLC_AIDING_REQ * msg = (tSIRF_MSG_SSB_SLC_AIDING_REQ *) message_structure;
            
            if ( *packet_length < ( 2 * sizeof(tSIRF_UINT8) + 
                                 1 * sizeof(tSIRF_UINT16) + 
                                 1 * sizeof(tSIRF_UINT32) + 
                                 header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT16(msg->section_validity_flag, ptr);
               SIRFBINARY_EXPORT8(msg->section_size, ptr);
               SIRFBINARY_EXPORT32(msg->sat_mask_nav_bit, ptr);
               SIRFBINARY_EXPORT8(msg->nav_bit_req_flag, ptr);
            }
         }
         break;

    case  SIRF_MSG_SSB_SESSION_OPEN_RSP:  /* 0x4A ,0x01 */
         {
            tSIRF_MSG_SSB_SESSION_OPEN_RSP * msg 
               = (tSIRF_MSG_SSB_SESSION_OPEN_RSP *) message_structure;
            if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->ses_open_not, ptr);
            }
         }
         break;
    case  SIRF_MSG_SSB_SESSION_CLOSE_RSP: /* 0x4A ,0x02 */
         {
            tSIRF_MSG_SSB_SESSION_CLOSE_RSP * msg = 
               (tSIRF_MSG_SSB_SESSION_CLOSE_RSP  *) message_structure;
               
            if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->ses_close_not, ptr);
            }
         }
         break;
   case  SIRF_MSG_SSB_ACK_NACK_ERROR_OUT: /* 0x4B ,0x01 */
         {
            tSIRF_MSG_SSB_ACKNACK_ERROR* msg = (tSIRF_MSG_SSB_ACKNACK_ERROR *) message_structure;

            if ( *packet_length < ((3 * sizeof(tSIRF_UINT8) + 
                                                    1 * sizeof(tSIRF_UINT16) +
                                                    header_len ) ))
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->msg_id, ptr);
               SIRFBINARY_EXPORT8(msg->sub_id, ptr);
               SIRFBINARY_EXPORT8(msg->session_error_reason, ptr);
               SIRFBINARY_EXPORT16(msg->reserved, ptr);
            }
         }
         break;
   case  SIRF_MSG_SSB_REJECT_OUT:  /* 0x4B ,0x02 */
         {
            tSIRF_MSG_SSB_REJECT_SLC* msg = (tSIRF_MSG_SSB_REJECT_SLC *) message_structure;

            if ( *packet_length < (3 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->rej_msgid, ptr);
               SIRFBINARY_EXPORT8(msg->rej_msgsubid, ptr);
               SIRFBINARY_EXPORT8(msg->reason, ptr);
            }
         }
         break;
         
   case  SIRF_MSG_SSB_PWR_MODE_MPM_RSP:   /* 0x5A, 0x2 */
      {
         const tSIRF_MSG_SSB_PWR_MODE_MPM_RSP *msg = (const tSIRF_MSG_SSB_PWR_MODE_MPM_RSP*) message_structure;
         if(sizeof(tSIRF_MSG_SSB_PWR_MODE_MPM_RSP) != message_length ||
               (*packet_length <( 2 * sizeof(tSIRF_UINT16) + header_len))) 
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }
         SIRFBINARY_EXPORT16(msg->errorCode,ptr);
         SIRFBINARY_EXPORT16(msg->reserved,ptr);
         break;
      }

   case  SIRF_MSG_SSB_PWR_MODE_APM_RSP:   /* 0x5A, 0x1 */
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
    
   case  SIRF_MSG_SSB_POS_REQ:  /* 0xD2 */
         {
         const tSIRF_MSG_SSB_POS_REQ * msg = (const tSIRF_MSG_SSB_POS_REQ*) message_structure;
         
         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( 8 * sizeof(tSIRF_UINT8) + header_len)))
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

         SIRFBINARY_EXPORT8(msg->pos_req_id,ptr);

         SIRFBINARY_EXPORT8(msg->num_fixes,ptr);
         SIRFBINARY_EXPORT8(msg->time_btw_fixes,ptr);
         SIRFBINARY_EXPORT8(msg->hori_error_max,ptr);
         SIRFBINARY_EXPORT8(msg->vert_error_max,ptr);
         SIRFBINARY_EXPORT8(msg->resp_time_max,ptr);
         SIRFBINARY_EXPORT8(msg->time_acc_priority,ptr);
         SIRFBINARY_EXPORT8(msg->location_method,ptr);
      }
    break;
    case  SIRF_MSG_SSB_SET_IONO: /* 0xD3 ,0x01 */
    {
         const tSIRF_MSG_SSB_SET_IONO_DATA* msg = (const tSIRF_MSG_SSB_SET_IONO_DATA*) message_structure;
         
         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( ( 8 * sizeof(tSIRF_UINT8) + header_len))))
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }


         SIRFBINARY_EXPORT8(msg->alpha_0,ptr);
         SIRFBINARY_EXPORT8(msg->alpha_1,ptr);
         SIRFBINARY_EXPORT8(msg->alpha_2,ptr);
         SIRFBINARY_EXPORT8(msg->alpha_3,ptr);
         SIRFBINARY_EXPORT8(msg->beta_0,ptr);
         SIRFBINARY_EXPORT8(msg->beta_1,ptr);
         SIRFBINARY_EXPORT8(msg->beta_2,ptr);
         SIRFBINARY_EXPORT8(msg->beta_3,ptr);
      }
      break;
    case  SIRF_MSG_SSB_SET_EPH_CLK: /* 0xD3 ,0x02 */
     {
         const tSIRF_MSG_SSB_SET_EPH_CLK* msg = (const tSIRF_MSG_SSB_SET_EPH_CLK*) message_structure;
         
         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( 1 * sizeof(tSIRF_UINT8) + 
                                  header_len ) ) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

         SIRFBINARY_EXPORT8(msg->num_sv,ptr);
         if ( ( *packet_length < ( 1 * sizeof(tSIRF_UINT8) + 
                                 ( 6 * msg->num_sv) * sizeof(tSIRF_UINT8) +
                                 (11 * msg->num_sv) * sizeof(tSIRF_UINT16) +
                                 ( 8 * msg->num_sv) * sizeof(tSIRF_UINT32) + 
                                 header_len ) ) ||
              ( msg->num_sv > SIRF_MAX_SVID_CNT ) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }
         else
         {
            for (i = 0; i < msg->num_sv; ++i) 
            {
               const tSIRF_MSG_SSB_EPHEMERIS_INFO_PER_SV* eph = &msg->sv_eph_prm[i];

               SIRFBINARY_EXPORT8(eph->eph_flag,ptr);
               SIRFBINARY_EXPORT8(eph->eph_data.sv_prn_num,ptr);
               SIRFBINARY_EXPORT8(eph->eph_data.ura_ind,ptr);
               SIRFBINARY_EXPORT8(eph->eph_data.iode,ptr);
               SIRFBINARY_EXPORT16(eph->eph_data.c_rs,ptr);
               SIRFBINARY_EXPORT16(eph->eph_data.delta_n,ptr);
               SIRFBINARY_EXPORT32(eph->eph_data.m0,ptr);
               SIRFBINARY_EXPORT16(eph->eph_data.c_uc,ptr);
               SIRFBINARY_EXPORT32(eph->eph_data.eccentricity,ptr);
               SIRFBINARY_EXPORT16(eph->eph_data.c_us,ptr);
               SIRFBINARY_EXPORT32(eph->eph_data.a_sqrt,ptr);
               SIRFBINARY_EXPORT16(eph->eph_data.toe,ptr);
               SIRFBINARY_EXPORT16(eph->eph_data.c_ic,ptr);
               SIRFBINARY_EXPORT32(eph->eph_data.omega_0,ptr);
               SIRFBINARY_EXPORT16(eph->eph_data.c_is,ptr);
               SIRFBINARY_EXPORT32(eph->eph_data.i0,ptr);
               SIRFBINARY_EXPORT16(eph->eph_data.c_rc,ptr);
               SIRFBINARY_EXPORT32(eph->eph_data.omega,ptr);
               SIRFBINARY_EXPORT32(eph->eph_data.omegadot,ptr);
               SIRFBINARY_EXPORT16(eph->eph_data.idot,ptr);
               SIRFBINARY_EXPORT16(eph->eph_data.toc,ptr);
               SIRFBINARY_EXPORT8(eph->eph_data.t_gd,ptr);
               SIRFBINARY_EXPORT8(eph->eph_data.af2,ptr);
               SIRFBINARY_EXPORT16(eph->eph_data.af1,ptr);
               SIRFBINARY_EXPORT32(eph->eph_data.af0,ptr);
            }  /* i=0 -> msg->num_sv */ 
         }
     }
    break;
    case  SIRF_MSG_SSB_SET_ALM: /* 0xD3 ,0x03 */
       {
         const tSIRF_MSG_SSB_SET_ALMANAC_ASSIST_DATA* msg = (const tSIRF_MSG_SSB_SET_ALMANAC_ASSIST_DATA*) message_structure;
         
         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( (1* sizeof(tSIRF_UINT8) + 
                                  1 * sizeof(tSIRF_UINT16) + 
                                 (3 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) +
                                 (5 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) +
                                 (4 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT32) + 
                                 header_len))))
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }


         SIRFBINARY_EXPORT16(msg->alm_week_num,ptr);
         SIRFBINARY_EXPORT8(msg->num_sv,ptr);
         if (msg->num_sv > SIRF_MAX_SVID_CNT)
         {
            tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
         }
         else
         {
            for (i = 0; i < msg->num_sv; ++i) 
            {
               const tSIRF_MSG_SSB_ALMANAC_PARAMETERS* alm = &msg->alm_params[i];

               SIRFBINARY_EXPORT8(alm->alm_valid_flag,ptr);
               SIRFBINARY_EXPORT8(alm->alm_sv_prn_num,ptr);
               SIRFBINARY_EXPORT16(alm->alm_eccentricity,ptr);
               SIRFBINARY_EXPORT8(alm->alm_toa,ptr);
               SIRFBINARY_EXPORT16(alm->alm_delta_incl,ptr);
               SIRFBINARY_EXPORT16(alm->alm_omegadot,ptr);
               SIRFBINARY_EXPORT24(alm->alm_a_sqrt,ptr);
               SIRFBINARY_EXPORT24(alm->alm_omega_0,ptr);
               SIRFBINARY_EXPORT24(alm->alm_omega,ptr);
               SIRFBINARY_EXPORT24(alm->alm_m0,ptr);
               SIRFBINARY_EXPORT16(alm->alm_af0,ptr);
               SIRFBINARY_EXPORT16(alm->alm_af1,ptr);
            }
         }
      }
    break;
    case  SIRF_MSG_SSB_SET_ACQ_ASSIST: /* 0xD3 ,0x04 */
    {
         const tSIRF_MSG_SSB_SET_ACQ_ASSIST_DATA* msg = (const tSIRF_MSG_SSB_SET_ACQ_ASSIST_DATA*) message_structure;
         
         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( (1 * sizeof(tSIRF_UINT8) + 
                                  1* sizeof(tSIRF_UINT32) +
                                 (7 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) +
                                 (4 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) +
                                 header_len))))
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }


         SIRFBINARY_EXPORT32(msg->reference_time,ptr);       
         SIRFBINARY_EXPORT8(msg->num_sv,ptr);

         if(msg->num_sv > SIRF_MAX_SVID_CNT)
         {
            tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
         }
         else
         {
            /* 32 instances of the following table:   */
            for (i = 0; i < msg->num_sv; ++i) 
            {
               const tSIRF_MSG_SSB_ACQUISITION_ASSISTANCE_DATA* acq = &msg->acq_data[i];

               SIRFBINARY_EXPORT8(acq->acq_assist_valid_flag,ptr);
               SIRFBINARY_EXPORT8(acq->sv_prn_number,ptr);
               SIRFBINARY_EXPORT16(acq->doppler0,ptr);
               SIRFBINARY_EXPORT8(acq->doppler1,ptr);
               SIRFBINARY_EXPORT8(acq->doppler_uncertainty,ptr);
               SIRFBINARY_EXPORT16(acq->sv_code_phase,ptr);
               SIRFBINARY_EXPORT8(acq->sv_code_phase_int,ptr);
               SIRFBINARY_EXPORT8(acq->gps_bit_num,ptr);
               SIRFBINARY_EXPORT16(acq->code_phase_uncertainty,ptr);
               SIRFBINARY_EXPORT16(acq->azimuth,ptr);
               SIRFBINARY_EXPORT8(acq->elevation,ptr);
            }
         }
      }
    break;
    case  SIRF_MSG_SSB_SET_RT_INTEG: /* 0xD3 ,0x05 */
       {
         const tSIRF_MSG_SSB_SET_RT_INTEGRITY* msg = (const tSIRF_MSG_SSB_SET_RT_INTEGRITY*) message_structure;
         
         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( 1 * sizeof(tSIRF_UINT8) + header_len)))
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

         SIRFBINARY_EXPORT32(msg->unhealthy_sat_info,ptr);
      }
    break;
    case  SIRF_MSG_SSB_SET_UTC_MODEL: /* 0xD3 ,0x06 */
     {
         const tSIRF_MSG_SSB_SET_UTC_MODEL* msg = (const tSIRF_MSG_SSB_SET_UTC_MODEL *)message_structure;

         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( (6 * sizeof(tSIRF_UINT8) + 
                                2 * sizeof(tSIRF_UINT32) +
                                header_len))) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

            SIRFBINARY_EXPORT32(msg->a0,ptr);
            SIRFBINARY_EXPORT32(msg->a1,ptr);
            SIRFBINARY_EXPORT8(msg->delta_tls,ptr);
            SIRFBINARY_EXPORT8(msg->tot,ptr);
            SIRFBINARY_EXPORT8(msg->wnt,ptr);
            SIRFBINARY_EXPORT8(msg->wnlsf,ptr);
            SIRFBINARY_EXPORT8(msg->dn,ptr);
            SIRFBINARY_EXPORT8(msg->delta_t_lsf,ptr);
      }
     break;
    case  SIRF_MSG_SSB_SET_GPS_TOW_ASSIST: /* 0xD3 ,0x07 */
         {
         const tSIRF_MSG_SSB_SET_GPS_TOW_ASSIST* msg = (const tSIRF_MSG_SSB_SET_GPS_TOW_ASSIST *)message_structure;

         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( (1 * sizeof(tSIRF_UINT8) + 
                               (2 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) +
                               (1 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) +
                                header_len))) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

         SIRFBINARY_EXPORT8(msg->num_sv,ptr);
         if(msg->num_sv > SIRF_MAX_SVID_CNT)
         {
            tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
         }
         else
         {
            for(i = 0; i < msg->num_sv; i++)
            {
               const tSIRF_MSG_SSB_AUXILIARY_NAV_TOW_ASSIST *aux = &msg->set_gps_tow_assist[i];
               SIRFBINARY_EXPORT8(aux->svid,ptr);
               SIRFBINARY_EXPORT16(aux->tlm_msg,ptr);
               SIRFBINARY_EXPORT8(aux->tow_assist_info,ptr);
            }
         }
      }
      break;
    case  SIRF_MSG_SSB_SET_AUX_NAV: /* 0xD3 ,0x08 */
    {
         const tSIRF_MSG_SSB_SET_AUXILIARY_NAV_PARAMS* msg = (const tSIRF_MSG_SSB_SET_AUXILIARY_NAV_PARAMS *)message_structure;

         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( (1 * sizeof(tSIRF_UINT8) + 
                               (5 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) + 
                               (2 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) +
                                 header_len))) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

         SIRFBINARY_EXPORT8(msg->num_sv,ptr);
         if(msg->num_sv > SIRF_MAX_SVID_CNT)
         {
            tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
         }
         else
         {
            /*The structure shall include 32 occurrences of     */
            /*auxiliary navigation model parameters             */
            for(i = 0; i < msg->num_sv; i++)
            {
               const tSIRF_MSG_SSB_AUXILIARY_NAV_PARAMS *aux_nav = &msg->set_aux_nav_model_params[i];
               SIRFBINARY_EXPORT8(aux_nav->svid,ptr);
               SIRFBINARY_EXPORT16(aux_nav->toe,ptr);
               SIRFBINARY_EXPORT16(aux_nav->iodc,ptr);
               SIRFBINARY_EXPORT8(aux_nav->sf1_l2_info,ptr);
               SIRFBINARY_EXPORT8(aux_nav->sf1_sv_health,ptr);

               /* Import the 11 reserved bytes. */
               memcpy(ptr, 
                      &msg->set_aux_nav_model_params[i].sf1_reserved, 
                      SIRF_MSG_SSB_ANP_NUM_SF1_RESERVED);
               ptr += SIRF_MSG_SSB_ANP_NUM_SF1_RESERVED;
               SIRFBINARY_EXPORT8(aux_nav->sf1_aodo_fit_interval,ptr);
            }
         }
     }
    break;
    case  SIRF_MSG_SSB_SET_AIDING_AVAIL: /* 0xD3 ,0x09 */
         {
            tSIRF_MSG_SSB_PUSH_AIDING_AVAILABILITY_REQ * msg = 
                (tSIRF_MSG_SSB_PUSH_AIDING_AVAILABILITY_REQ *) message_structure;
            
            if ( *packet_length < ((6 * sizeof(tSIRF_UINT8) + 
                                      2 * sizeof(tSIRF_UINT16) +
                                      header_len )) )
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
    case  SIRF_MSG_SSB_EPH_REQ: /* 0xD4 ,0x01 */
      {
         /* there is nothing to export for this message */
         if (sizeof(tSIRF_MSG_SSB_SLC_EPH_STATUS_REQ) != message_length)
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }
      }
      break;
    case  SIRF_MSG_SSB_ALM_REQ:  /* 0xD4 ,0x02 */
      {
         /* there is nothing to export for this message */
         if (sizeof(tSIRF_MSG_SSB_ALMANAC_REQ) != message_length)
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }
      }
      break;
    case  SIRF_MSG_SSB_B_EPH_REQ: /* 0xD4 ,0x03 */
      {
         /* Construct a pointer to the structure. */
         const tSIRF_MSG_SSB_BE_REQ * msg = (const tSIRF_MSG_SSB_BE_REQ *)message_structure;
         
         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( ( 1 * sizeof(tSIRF_UINT8) +
                                    1 * sizeof(tSIRF_UINT16) + 
                                    header_len ) ) ) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

         SIRFBINARY_EXPORT16(msg->eph_resp_trigger,ptr);
         SIRFBINARY_EXPORT8(msg->num_sv,ptr);
         if ( ( *packet_length < ( ( 1 * sizeof(tSIRF_UINT8) +
                                   ( 2 * msg->num_sv ) * sizeof(tSIRF_UINT8) +
                                     1 * sizeof(tSIRF_UINT16) + 
                                   ( 2 * msg->num_sv ) * sizeof(tSIRF_UINT16) +
                                   header_len) ) ) ||
              ( msg->num_sv > SIRF_MAX_SVID_CNT ) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }
         else
         {
            for (i = 0u; i < msg->num_sv; ++i) 
            {
               const tSIRF_MSG_SSB_BE_REQ_PARAMS* eph_info = &msg->eph_params[i];
 
               SIRFBINARY_EXPORT8( eph_info->eph_info_flag,ptr);
               SIRFBINARY_EXPORT8( eph_info->sv_prn_num,ptr);
               SIRFBINARY_EXPORT16(eph_info->gps_week,ptr);
               SIRFBINARY_EXPORT16(eph_info->toe,ptr);
            } /* i=0 -> msg->num_sv */ 
         }
      }
      break;
    case  SIRF_MSG_SSB_TIME_FREQ_APPROX_POS_REQ: /* 0xD4 ,0x04 */
      {
         const tSIRF_MSG_SSB_TIME_FREQ_APROX_POS_REQ* msg = (const tSIRF_MSG_SSB_TIME_FREQ_APROX_POS_REQ *)message_structure;

         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( (1 * sizeof(tSIRF_UINT8) + header_len ))) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

         SIRFBINARY_EXPORT8(msg->req_mask,ptr);
         }
    break;
    case  SIRF_MSG_SSB_CH_LOAD_REQ: /* 0xD4 ,0x05 */
         {
            tSIRF_MSG_SSB_CHANNEL_LOAD_QUERY* msg = (tSIRF_MSG_SSB_CHANNEL_LOAD_QUERY *) message_structure;
            
            if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + header_len ) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->port,         ptr);
               SIRFBINARY_EXPORT8(msg->mode, ptr);
            }
         }
         break;
    case  SIRF_MSG_SSB_CLIENT_STATUS_REQ: /* 0xD4 ,0x06 */
         {
            tSIRF_MSG_SSB_SLC_STATUS * msg = (tSIRF_MSG_SSB_SLC_STATUS *) message_structure;
            
            if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->slc_status, ptr);
            }
         }
         break;
    case  SIRF_MSG_SSB_OSP_REV_REQ: /* 0xD4 ,0x07 */
         {
            /* Unused: tSIRF_MSG_SSB_OSP_REV_REQ * msg = (tSIRF_MSG_SSB_OSP_REV_REQ *) message_structure; */
            
            if ( *packet_length < header_len )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
         }
         break;
    case  SIRF_MSG_SSB_SERIAL_SETTING_REQ:  /* 0xD4 ,0x08 */
         {
            tSIRF_MSG_SSB_SERIAL_PORT_SETTING_REQ* msg = (tSIRF_MSG_SSB_SERIAL_PORT_SETTING_REQ *) message_structure;
            
            if ( *packet_length < (5* sizeof(tSIRF_UINT8) + 
                                                1 * sizeof(tSIRF_UINT32) +
                                                header_len ) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT32(msg->baud, ptr);
               SIRFBINARY_EXPORT8(msg->data_bits, ptr);
               SIRFBINARY_EXPORT8(msg->stop_bits, ptr);
               SIRFBINARY_EXPORT8(msg->parity, ptr);
               SIRFBINARY_EXPORT8(msg->port, ptr);
               SIRFBINARY_EXPORT8(msg->reserved, ptr);
            }
         }
         break;
    case  SIRF_MSG_SSB_TX_BLANKING_REQ: /* 0xD4 ,0x09 */
         {
            tSIRF_MSG_SSB_TX_BLANK_REQ * msg = (tSIRF_MSG_SSB_TX_BLANK_REQ *) message_structure;
               
            if ( *packet_length < (3 * sizeof(tSIRF_UINT8) + 
                                                1 * sizeof(tSIRF_UINT32) +
                                                header_len ) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8 (msg->command,       ptr);
               SIRFBINARY_EXPORT8 (msg->air_interface, ptr);
               SIRFBINARY_EXPORT8 (msg->mode,          ptr);
               SIRFBINARY_EXPORT32 (msg->spare,         ptr);
            }
         }
         break;
    case  SIRF_MSG_SSB_SESSION_OPEN_REQ: /* 0xD5 ,0x01 */
         {
            tSIRF_MSG_SSB_SESSION_OPEN_REQ * msg = (tSIRF_MSG_SSB_SESSION_OPEN_REQ *) message_structure;

            /* Check to make sure that the length of the buffer we are writing into is big enough */
            if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->ses_open_req, ptr);
            }
         }
         break;
    case  SIRF_MSG_SSB_SESSION_CLOSE_REQ:  /* 0xD5 ,0x02 */
         {
            tSIRF_MSG_SSB_SESSION_CLOSE_REQ * msg = (tSIRF_MSG_SSB_SESSION_CLOSE_REQ  *) message_structure;
            
            if ( *packet_length < (1 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->ses_close_req, ptr);
            }
         }
         break;

    case  SIRF_MSG_SSB_HW_CONFIG_RSP: /* 0xD6 */
         {
            tSIRF_MSG_SSB_HW_CONFIG_RSP * msg = 
               (tSIRF_MSG_SSB_HW_CONFIG_RSP *) message_structure;
            
            if ( *packet_length < (3 * sizeof(tSIRF_UINT8) + 1 * sizeof(tSIRF_UINT32) +
                                   header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8 (msg->hw_config_msg,     ptr);
               SIRFBINARY_EXPORT8 (msg->nominal_freq_high, ptr);
               SIRFBINARY_EXPORT32(msg->nominal_freq_low,  ptr);
               SIRFBINARY_EXPORT8(msg->nw_enhance_type,  ptr);
            }
         }
         break;
    case  SIRF_MSG_SSB_APRX_MS_POSITION_RSP:  /* 0xD7 ,0x01 */
         {
            tSIRF_MSG_SSB_APRX_MS_POSITION_RSP * msg = (tSIRF_MSG_SSB_APRX_MS_POSITION_RSP *) message_structure;
            
            if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + 
                                   2 * sizeof(tSIRF_UINT16) +
                                   2 * sizeof(tSIRF_UINT32) +
                                   header_len) )
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
    case  SIRF_MSG_SSB_TIME_TX_RSP:  /* 0xD7 ,0x02 */
         {
            tSIRF_MSG_SSB_TIME_TRANSFER_RSP * msg = (tSIRF_MSG_SSB_TIME_TRANSFER_RSP *) message_structure;
            
            if ( *packet_length < (3 * sizeof(tSIRF_UINT8) + 
                                      1 * sizeof(tSIRF_UINT16) +
                                      3 /* 1 * sizeof(tSIRF_INT24) */ + 
                                      1 * sizeof(tSIRF_UINT32) +
                                      header_len ) )
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
    case  SIRF_MSG_SSB_FREQ_TX_RSP:  /* 0xD7 ,0x03 */
         {
            tSIRF_MSG_SSB_FREQ_TRANSFER_RSP * msg = (tSIRF_MSG_SSB_FREQ_TRANSFER_RSP *) message_structure;
            
            if ( *packet_length < (2 * sizeof(tSIRF_UINT8) + 
                                      1 * sizeof(tSIRF_UINT16) +
                                      1 * sizeof(tSIRF_UINT32) +
                                      header_len ) )       
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
    case  SIRF_MSG_SSB_SET_NBA_SF1_2_3:  /* 0xD7 ,0x04 */
      {
         const tSIRF_MSG_SSB_NAV_SF_123_RSP* msg = (const tSIRF_MSG_SSB_NAV_SF_123_RSP*)message_structure;
         
         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < (1*sizeof(tSIRF_UINT8) + 
                                2 * SIRF_MSG_SSB_ANS123R_NUM_SF123 * sizeof(tSIRF_UINT8) + 
                                SIRF_MSG_SSB_ANS123R_NUM_SF123 * SIRF_MSG_SSB_ANS123R_NUM_BYTES_PER_SF123 +
                                header_len)) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }
         
         for (i = 0; i < SIRF_MSG_SSB_ANS123R_NUM_SF123; ++i) 
         {
            const tSIRF_MSG_SSB_NAV_SF_123* sf123 = (const tSIRF_MSG_SSB_NAV_SF_123*)&msg->sf123[i];
            SIRFBINARY_EXPORT8(sf123->sf_123_flag,ptr);
            SIRFBINARY_EXPORT8(sf123->sv_id,ptr);
            memcpy(ptr,sf123->sf123,SIRF_MSG_SSB_ANS123R_NUM_BYTES_PER_SF123);
            ptr += SIRF_MSG_SSB_ANS123R_NUM_BYTES_PER_SF123;
         }
      }
      break;
    case  SIRF_MSG_SSB_SET_NBA_SF4_5:  /* 0xD7 ,0x05 */
      {
         /* Construct a pointer to the structure. */
         const tSIRF_MSG_SSB_NAV_SF_45_RSP * msg = (const tSIRF_MSG_SSB_NAV_SF_45_RSP *)message_structure;
         
         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < (SIRF_MSG_SSB_ANS45R_NUM_SF45 * sizeof(tSIRF_UINT8) + 
                                SIRF_MSG_SSB_ANS45R_NUM_SF45 * SIRF_MSG_SSB_ANS45R_NUM_BYTES_PER_SF45 +
                                1 * sizeof(tSIRF_UINT32) +
                                header_len)) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

         /* Import members. */
         SIRFBINARY_EXPORT32(msg->sv_list,ptr);
         for (i = 0u; i < SIRF_MSG_SSB_ANS45R_NUM_SF45; ++i) 
         {
            SIRFBINARY_EXPORT8(msg->sf45[i].frame_num,ptr);
               
            /* Import the 75 bytes (600 bits) of subframes 4/5. */
            memcpy(ptr,msg->sf45[i].SF45, SIRF_MSG_SSB_ANS45R_NUM_BYTES_PER_SF45);
            ptr += SIRF_MSG_SSB_ANS45R_NUM_BYTES_PER_SF45;
         }
      }
      break;
    case  SIRF_MSG_SSB_ACK_NACK_ERROR_IN: /* 0xD8 ,0x01 */
         {
            tSIRF_MSG_SSB_ACKNACK_ERROR* msg = (tSIRF_MSG_SSB_ACKNACK_ERROR *) message_structure;
            
            if ( *packet_length < (3 * sizeof(tSIRF_UINT8) + 
                                                   1 * sizeof(tSIRF_UINT16) + 
                                                   header_len ) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->msg_id, ptr);
               SIRFBINARY_EXPORT8(msg->sub_id, ptr);
               SIRFBINARY_EXPORT8(msg->session_error_reason,ptr);
               SIRFBINARY_EXPORT8(msg->reserved,ptr);
            }
         }
         break;
    case  SIRF_MSG_SSB_REJECT_IN: /* 0xD8 ,0x02 */
         {
            tSIRF_MSG_SSB_REJECT_CP * msg = (tSIRF_MSG_SSB_REJECT_CP *) message_structure;
            
            if ( *packet_length < (3 * sizeof(tSIRF_UINT8) + header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT8(msg->rej_msgid, ptr);
               SIRFBINARY_EXPORT8(msg->rej_msgsubid, ptr);
               SIRFBINARY_EXPORT8(msg->reason,    ptr);
            }
         }
         break;

    case SIRF_MSG_SSB_SIRF_STATS_AGPS:
         {
            tSIRF_MSG_SSB_SIRF_STATS_AGPS *msg = (tSIRF_MSG_SSB_SIRF_STATS_AGPS *) message_structure;

            if ( *packet_length < ( 6 * sizeof(tSIRF_UINT32) + 
                                    6 * sizeof(tSIRF_UINT16) + 
                                    9 * sizeof(tSIRF_UINT8) + 
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               SIRFBINARY_EXPORT16(msg->stats.ttff_since_reset,       ptr);
               SIRFBINARY_EXPORT16(msg->stats.ttff_since_aiding,      ptr);
               SIRFBINARY_EXPORT16(msg->stats.ttff_first_nav,         ptr);
               SIRFBINARY_EXPORT32(msg->stats.pos_aiding_error_north, ptr);
               SIRFBINARY_EXPORT32(msg->stats.pos_aiding_error_east,  ptr);
               SIRFBINARY_EXPORT32(msg->stats.pos_aiding_error_down,  ptr);
               SIRFBINARY_EXPORT32(msg->stats.time_aiding_error,      ptr);
               SIRFBINARY_EXPORT16(msg->stats.freq_aiding_error,      ptr);
               SIRFBINARY_EXPORT8(msg->stats.hor_pos_uncertainty,     ptr);
               SIRFBINARY_EXPORT16(msg->stats.ver_pos_uncertainty,    ptr);
               SIRFBINARY_EXPORT8(msg->stats.time_uncertainty,        ptr);
               SIRFBINARY_EXPORT8(msg->stats.freq_uncertainty,        ptr);
               SIRFBINARY_EXPORT8(msg->stats.num_aided_ephemeris,     ptr);
               SIRFBINARY_EXPORT8(msg->stats.num_aided_acq_assist,    ptr);
               SIRFBINARY_EXPORT8(msg->stats.nav_mode,                ptr);
               SIRFBINARY_EXPORT8(msg->stats.pos_mode,                ptr);
               SIRFBINARY_EXPORT16(msg->stats.nav_status,             ptr);
               SIRFBINARY_EXPORT8(msg->stats.start_mode,              ptr);
               SIRFBINARY_EXPORT8(msg->stats.aiding_status,           ptr);
               SIRFBINARY_EXPORT32(msg->clock_drift,                  ptr);
               SIRFBINARY_EXPORT32(msg->reserved,                     ptr);
            }
         }
         break;

    default:
         /* Pass thru messages 0x61 to 0x7F */
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
         break;

      } /* switch */

      if (SIRF_SUCCESS == tRet)
      {
         *packet_length = ptr - packet;
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
tSIRF_RESULT SIRF_CODEC_SSB_AGPS_Decode( tSIRF_UINT8 *payload,
                                    tSIRF_UINT32 payload_length,
                                    tSIRF_UINT32 *message_id,
                                    tSIRF_VOID   *message_structure,
                                    tSIRF_UINT32 *message_length )
{
   tSIRF_RESULT tRet = SIRF_SUCCESS;

   if ( ((tSIRF_UINT8 *) NULL !=  payload) &&
        ((tSIRF_UINT32 *) NULL != message_id) &&
        ((tSIRF_VOID *) NULL != message_structure) &&
        ((tSIRF_UINT32 *) NULL != message_length) )
   {
      tSIRF_UINT8 *ptr = payload;
      tSIRF_UINT32 i;
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

      /* add the sub-id to the message id */
      if (
         SIRF_MSG_SSB_PWR_MODE_REQ == *message_id ||
         SIRF_MSG_SSB_SET_AIDING == *message_id ||
         SIRF_MSG_SSB_STATUS_REQ == *message_id ||
         SIRF_MSG_SSB_SESSION_CONTROL_REQ == *message_id ||
         SIRF_MSG_SSB_AIDING_RSP== *message_id ||
         SIRF_MSG_SSB_ACK_IN == *message_id ||
         SIRF_MSG_SSB_POS_MEAS_RSP== *message_id ||
         SIRF_MSG_SSB_STATUS_RESP == *message_id ||
         SIRF_MSG_SSB_AIDING_REQ == *message_id ||
         SIRF_MSG_SSB_SESSION_CONTROL_RSP == *message_id ||
         SIRF_MSG_SSB_ACK_OUT == *message_id ||
         SIRF_MSG_SSB_PWR_MODE_RSP == *message_id ||
         SIRF_MSG_SSB_SIRF_INTERNAL_OUT == *message_id ||
         SIRF_MSG_SSB_EE_INPUT == *message_id
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

   /* Output Messages */
    case SIRF_MSG_SSB_POS_RSP:   /* 0x45 , 0x01 */
      {
         /* Construct a pointer to the structure. */
         tSIRF_MSG_SSB_POSITION_RSP * msg = (tSIRF_MSG_SSB_POSITION_RSP *)message_structure;
         
         if ( payload_length < ( (20 + 3 * SIRF_MSG_SSB_RSP_NUM_SV_INFO) * sizeof(tSIRF_UINT8) + 
                                 (6* sizeof(tSIRF_UINT16) + 3 * sizeof(tSIRF_UINT32) + header_len ) ))
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
            msg->pos_req_id                            = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->pos_results_flag                      = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->pos_err_status               = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->qos_pass_flag                = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->pos_type                     = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->dgps_cor                     = SIRFBINARY_IMPORT_UINT8(ptr); 
            msg->gps_week                     = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->time_of_week                 = SIRFBINARY_IMPORT_UINT32(ptr);
            msg->lat                          = SIRFBINARY_IMPORT_SINT32(ptr);
            msg->lon                          = SIRFBINARY_IMPORT_SINT32(ptr);
   
            msg->other_sections               = SIRFBINARY_IMPORT_UINT8(ptr);
      
            /* Horizontal Error Section */
            msg->hor_err_angle                = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->hor_err_major                = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->hor_err_minor                = SIRFBINARY_IMPORT_UINT8(ptr);
      
            /* Vertical Position Section */
            msg->vert_pos_height              = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->vert_pos_std                 = SIRFBINARY_IMPORT_UINT8(ptr);
      
            /* Velocity Section */
            msg->velocity_horizontal          = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->velocity_heading             = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->velocity_vertical            = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->velocity_hor_err_ang         = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->velocity_hor_err_maj_std     = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->velocity_hor_err_min_std     = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->velocity_ver_err             = SIRFBINARY_IMPORT_UINT8(ptr);
      
            /* Clock Correction Section */
            msg->clk_cor_time_ref             = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->clk_cor_clk_bias             = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->clk_cor_clk_drift            = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->clk_cor_clk_std_err          = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->clk_cor_utc_off              = SIRFBINARY_IMPORT_UINT8(ptr);  
      
            /* Position Correction Section */
            msg->pos_corr_num_sat             = SIRFBINARY_IMPORT_UINT8(ptr);
            for (i = 0; i < SIRF_MSG_SSB_RSP_NUM_SV_INFO; i++)
            {
               tSIRF_MSG_SSB_NB_SV_DATA *sv_data = &msg->pos_corr_sv_info[i];
               sv_data->sv_prn                         = SIRFBINARY_IMPORT_UINT8(ptr);
               sv_data->c_n0                           = SIRFBINARY_IMPORT_UINT8(ptr);
               sv_data->inv_weights                    = SIRFBINARY_IMPORT_UINT8(ptr);
            }

         }
      }
      break;
    case SIRF_MSG_SSB_MEAS_RSP: /* 0x45 , 0x02 */
      {
         /* Construct a pointer to the structure. */
         tSIRF_MSG_SSB_MEAS_RSP* msg = (tSIRF_MSG_SSB_MEAS_RSP *)message_structure;

         if ( payload_length < ( 5 * sizeof(tSIRF_UINT8) +
                                 1 * sizeof(tSIRF_UINT16) + 
                                 1 * sizeof(tSIRF_UINT32) +
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
            /* ICD 2.0 */
            msg->pos_req_id                            = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->gps_meas_flag                         = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->meas_error_status       = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->meas_gps_week           = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->meas_gps_seconds        = SIRFBINARY_IMPORT_UINT32(ptr);
            msg->time_accuracy           = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->num_sv                  = SIRFBINARY_IMPORT_UINT8(ptr);
            if ( ( payload_length < ( (5 + 4 * msg->num_sv) * sizeof(tSIRF_UINT8) +
                                      (1 + 3 * msg->num_sv) * sizeof(tSIRF_UINT16) + 
                                      (1 * sizeof(tSIRF_UINT32) +
                                       header_len) )) ||
                 ( msg->num_sv > SIRF_MSG_SSB_MEASUREMENT_DATA_MAX_SVS ) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               for (i = 0; i < msg->num_sv; i++)
               {
                  tSIRF_MSG_SSB_MEAS_PER_SV_DATA *meas = &msg->meas_per_sv_data[i];
                  meas->sv_prn                            = SIRFBINARY_IMPORT_UINT8(ptr);
                  meas->c_n0                              = SIRFBINARY_IMPORT_UINT8(ptr);
                  meas->sv_doppler                        = SIRFBINARY_IMPORT_SINT16(ptr);
                  meas->sv_code_phase_wh                  = SIRFBINARY_IMPORT_UINT16(ptr);
                  meas->sv_code_phase_fr                  = SIRFBINARY_IMPORT_UINT16(ptr);
                  meas->multipath_indicator               = SIRFBINARY_IMPORT_UINT8(ptr);
                  meas->pseudorange_rms_error             = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }   
         }
      }
      break;
    case SIRF_MSG_SSB_EPH_RSP: /* 0x46 , 0x01 */
      {
         /* Construct a pointer to the structure. */
         tSIRF_MSG_SSB_SLC_EPH_STATUS_RSP * msg = ( tSIRF_MSG_SSB_SLC_EPH_STATUS_RSP *)message_structure;

         if ( payload_length < ( 4  * sizeof(tSIRF_UINT8) + 
                                 1  * sizeof(tSIRF_UINT16) + 
                                 1  * sizeof(tSIRF_UINT32) + 
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
   
            /* Export members. */
            msg->gps_time_mask                         = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->gps_week                              = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->gps_tow                               = SIRFBINARY_IMPORT_UINT32(ptr);
            msg->eph_status_type                       = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->gps_t_toe_limit                       = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->num_sv                                = SIRFBINARY_IMPORT_UINT8(ptr);   
            if ( payload_length < ( (4 + 4 * msg->num_sv) * sizeof(tSIRF_UINT8) + 
                                    (1 + 3 * msg->num_sv) * sizeof(tSIRF_UINT16) + 
                                         1 * sizeof(tSIRF_UINT32) + 
                                         header_len))
            {
                tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else if (msg->num_sv > SIRF_MAX_SVID_CNT)
            {
               tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
            }
            else
            {
               for (i = 0; i < msg->num_sv ; ++i) 
               {
                  tSIRF_MSG_SSB_EPH_INFO* eph = &msg->eph_info[i];
      
                  eph->svid                               = SIRFBINARY_IMPORT_UINT8(ptr);
                  eph->sv_info_flag                       = SIRFBINARY_IMPORT_UINT8(ptr);
                  eph->gps_week                           = SIRFBINARY_IMPORT_UINT16(ptr);
                  eph->gps_toe                            = SIRFBINARY_IMPORT_UINT16(ptr);
                  eph->iode                               = SIRFBINARY_IMPORT_UINT8(ptr);
                  eph->azimuth                            = SIRFBINARY_IMPORT_UINT16(ptr);
                  eph->elevation                          = SIRFBINARY_IMPORT_SINT8(ptr);
               }
            }
         }
      }
      break;
    case SIRF_MSG_SSB_ALM_RSP: /* 0x46 , 0x02 */
      {
         /* Construct a pointer to the structure. */
         tSIRF_MSG_SSB_ALMANAC_RSP * msg = ( tSIRF_MSG_SSB_ALMANAC_RSP *)message_structure;

         if ( payload_length < ( 2  * sizeof(tSIRF_UINT8) + 
                                 1  * sizeof(tSIRF_UINT16) + 
                                 1 * sizeof(tSIRF_UINT32) + 
                                  header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
   
            /* Export members. */
            msg->data_flag                             = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->extended_gps_week                     = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->gpstow                                = SIRFBINARY_IMPORT_UINT32(ptr);
            msg->num_sv                              = SIRFBINARY_IMPORT_UINT8(ptr);
            if ( payload_length < ( (2 + 3 * msg->num_sv) * sizeof(tSIRF_UINT8) + 
                                    (1 + 6 * msg->num_sv) * sizeof(tSIRF_UINT16) + 
                                    (4 * 3 * msg->num_sv) /* tSIRF_UINT24 */ + 
                                     1 * sizeof(tSIRF_UINT32) + 
                                     header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else if (msg->num_sv > SIRF_MAX_SVID_CNT)
            {
               tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
            }
            else
            {
               for (i = 0; i < msg->num_sv    ; ++i) 
               {
                  tSIRF_MSG_SSB_OS_ALMANAC* alm = &msg->alm_per_sv_list[i];
                  alm->valid_flag                         = SIRFBINARY_IMPORT_UINT8(ptr);
                  alm->svprnNum                           = SIRFBINARY_IMPORT_UINT8(ptr);
                  alm->week_num                           = SIRFBINARY_IMPORT_UINT16(ptr);
                  alm->eccentricity                       = SIRFBINARY_IMPORT_UINT16(ptr);
                  alm->toa                                = SIRFBINARY_IMPORT_UINT8(ptr);
                  alm->delta_incl                         = SIRFBINARY_IMPORT_SINT16(ptr);
                  alm->omegadot                           = SIRFBINARY_IMPORT_SINT16(ptr);
                  alm->a_sqrt                             = SIRFBINARY_IMPORT_UINT24(ptr);
                  alm->omega_0                            = SIRFBINARY_IMPORT_SINT24(ptr);
                  alm->omega                              = SIRFBINARY_IMPORT_SINT24(ptr);
                  alm->m0                                 = SIRFBINARY_IMPORT_SINT24(ptr);
                  alm->af0                                = SIRFBINARY_IMPORT_SINT16(ptr);
                  alm->af1                                = SIRFBINARY_IMPORT_SINT16(ptr);
               }
            }
         }
      }
      break;
    case SIRF_MSG_SSB_B_EPH_RSP: /* 0x46 , 0x03 */
      {
         /* Construct a pointer to the structure. */
         tSIRF_MSG_SSB_BE_RSP * msg = ( tSIRF_MSG_SSB_BE_RSP *)message_structure;

         if ( payload_length < ( (12 * sizeof(tSIRF_UINT8) + 
                                  1 * sizeof(tSIRF_UINT16) + 
                                  1 * sizeof(tSIRF_UINT32) + 
                                 header_len) ))
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }

         else
         {
            *message_length = sizeof(*msg);
   
            msg->reserved            = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->iono_flag           = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->iono_data.alpha_0   = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->iono_data.alpha_1   = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->iono_data.alpha_2   = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->iono_data.alpha_3   = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->iono_data.beta_0    = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->iono_data.beta_1    = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->iono_data.beta_2    = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->iono_data.beta_3    = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->time_flag           = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->extd_gps_week       = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->gps_tow             = SIRFBINARY_IMPORT_UINT32(ptr);         
            msg->num_sv             = SIRFBINARY_IMPORT_UINT8(ptr);          
            if ( ( payload_length < ( 12 * sizeof(tSIRF_UINT8) + 
                                     ( 7 * msg->num_sv ) * sizeof(tSIRF_UINT8) +
                                       1 * sizeof(tSIRF_UINT16) + 
                                     (12 * msg->num_sv ) * sizeof(tSIRF_UINT16) +
                                       1 * sizeof(tSIRF_UINT32) + 
                                     ( 8 * msg->num_sv ) * sizeof(tSIRF_UINT32) + 
                                      header_len ) ) ||
                 ( msg->num_sv > SIRF_MAX_SVID_CNT ) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               for (i = 0; i < msg->num_sv; ++i) 
               {
                  tSIRF_MSG_SSB_BE* eph = &msg->be[i];
      
                  eph->eph_flag         = SIRFBINARY_IMPORT_UINT8 (ptr);
                  eph->health           = SIRFBINARY_IMPORT_UINT8 (ptr);
                  eph->gps_week         = SIRFBINARY_IMPORT_UINT16(ptr);
                  eph->eph.sv_prn_num   = SIRFBINARY_IMPORT_UINT8 (ptr);
                  eph->eph.ura_ind      = SIRFBINARY_IMPORT_UINT8 (ptr);
                  eph->eph.iode         = SIRFBINARY_IMPORT_UINT8 (ptr);
                  eph->eph.c_rs         = SIRFBINARY_IMPORT_SINT16(ptr);
                  eph->eph.delta_n      = SIRFBINARY_IMPORT_SINT16(ptr);
                  eph->eph.m0           = SIRFBINARY_IMPORT_SINT32(ptr);
                  eph->eph.c_uc         = SIRFBINARY_IMPORT_SINT16(ptr);
                  eph->eph.eccentricity = SIRFBINARY_IMPORT_UINT32(ptr);
                  eph->eph.c_us         = SIRFBINARY_IMPORT_SINT16(ptr);
                  eph->eph.a_sqrt       = SIRFBINARY_IMPORT_UINT32(ptr);
                  eph->eph.toe          = SIRFBINARY_IMPORT_UINT16(ptr);
                  eph->eph.c_ic         = SIRFBINARY_IMPORT_SINT16(ptr);
                  eph->eph.omega_0      = SIRFBINARY_IMPORT_SINT32(ptr);
                  eph->eph.c_is         = SIRFBINARY_IMPORT_SINT16(ptr);
                  eph->eph.i0           = SIRFBINARY_IMPORT_SINT32(ptr);
                  eph->eph.c_rc         = SIRFBINARY_IMPORT_SINT16(ptr);
                  eph->eph.omega        = SIRFBINARY_IMPORT_SINT32(ptr);
                  eph->eph.omegadot     = SIRFBINARY_IMPORT_SINT32(ptr);
                  eph->eph.idot         = SIRFBINARY_IMPORT_SINT16(ptr);
                  eph->eph.toc          = SIRFBINARY_IMPORT_UINT16(ptr);
                  eph->eph.t_gd         = SIRFBINARY_IMPORT_SINT8 (ptr);
                  eph->eph.af2          = SIRFBINARY_IMPORT_SINT8 (ptr);
                  eph->eph.af1          = SIRFBINARY_IMPORT_SINT16(ptr);
                  eph->eph.af0          = SIRFBINARY_IMPORT_SINT32(ptr);
               } /* i=0 -> msg->num_sv */
            }
         }
      }

      break;
    case SIRF_MSG_SSB_TIME_FREQ_APPROX_POS_RSP: /* 0x46 , 0x04 */
            {
               tSIRF_MSG_SSB_TIME_FREQ_APROX_POS_RSP * msg = (tSIRF_MSG_SSB_TIME_FREQ_APROX_POS_RSP *) message_structure;
               
               if ( payload_length < (5 * sizeof(tSIRF_UINT8) + 
                                      3 * sizeof(tSIRF_UINT16) +
                                      5 * sizeof(tSIRF_UINT32) +
                                      header_len ) )
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
    case SIRF_MSG_SSB_CH_LOAD_RSP: /* 0x46 , 0x05 */
            {
               tSIRF_MSG_SSB_CHANNEL_LOAD_RSP * msg = (tSIRF_MSG_SSB_CHANNEL_LOAD_RSP *) message_structure;
               
               if ( payload_length < ( (3 + 1 * SIRF_NUM_CHANNELS) * sizeof(tSIRF_UINT8) + 
                                        header_len ) )               
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->port = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->total_load = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->number_of_channels = SIRFBINARY_IMPORT_UINT8(ptr);
                  if(msg->number_of_channels > SIRF_NUM_CHANNELS)
                  {
                     tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
                  }
                  else
                  {
                     for (i = 0; i < msg->number_of_channels; i++)
                     {
                        tSIRF_MSG_SSB_CHANNEL_LOAD_RSP_ONE_CHANNEL * pMsg = 
                           (tSIRF_MSG_SSB_CHANNEL_LOAD_RSP_ONE_CHANNEL *) &msg->load_entry[i];
                        pMsg->load = SIRFBINARY_IMPORT_UINT8(ptr);
                     }
                  }
               }
            }
            break;
    case SIRF_MSG_SSB_CLIENT_STATUS_RSP: /* 0x46 , 0x06 */
            {
               tSIRF_MSG_SSB_SLC_STATUS * msg = (tSIRF_MSG_SSB_SLC_STATUS *) message_structure;
               
               if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len ) )
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
    case SIRF_MSG_SSB_OSP_REV_RSP:  /* 0x46 , 0x07 */
            {
               tSIRF_MSG_SSB_OSP_REV_RSP* msg = 
                  (tSIRF_MSG_SSB_OSP_REV_RSP *) message_structure;
               
               if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len ) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->osp_revision= SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;
    case SIRF_MSG_SSB_SERIAL_SETTINGS_RSP: /* 0x46 , 0x08 */
            {
               tSIRF_MSG_SSB_SERIAL_SETTING_RSP* msg = 
                  (tSIRF_MSG_SSB_SERIAL_SETTING_RSP *) message_structure;
               
               if ( payload_length < (2 * sizeof(tSIRF_UINT8) + 
                                                      1 * sizeof(tSIRF_UINT32) + 
                                                     header_len ) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->port = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->baud = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->ack_number = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;
    case SIRF_MSG_SSB_TX_BLANKING_RESP: /* 0x46 , 0x09 */
            {
               tSIRF_MSG_SSB_TX_BLANKING_RSP* msg = 
                  (tSIRF_MSG_SSB_TX_BLANKING_RSP *) message_structure;
               
               if ( payload_length < (2 * sizeof(tSIRF_UINT8) + header_len ) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->ack_nack= SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->spare = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;
    case SIRF_MSG_SSB_HW_CONFIG_REQ: /* 0x47  */
           {
            memset( message_structure, 0, sizeof(tSIRF_MSG_SSB_HW_CONFIG_REQ) );
            *message_length = sizeof (tSIRF_MSG_SSB_HW_CONFIG_REQ);
           }
           break;

    case SIRF_MSG_SSB_APROX_MS_POS_REQ: /* 0x49 , 0x01 */
           {
            memset( message_structure, 0, sizeof(tSIRF_MSG_SSB_APRX_MS_POSITION_REQ) );
            *message_length = sizeof (tSIRF_MSG_SSB_APRX_MS_POSITION_REQ);
           }
           break;
    case SIRF_MSG_SSB_TIME_TX_REQ:   /* 0x49 , 0x02 */
           {
            memset( message_structure, 0, sizeof(tSIRF_MSG_SSB_TIME_TRANSFER_REQ) );
            *message_length = sizeof (tSIRF_MSG_SSB_TIME_TRANSFER_REQ);
           }
           break;
    case SIRF_MSG_SSB_FREQ_TX_REQ:  /* 0x49 , 0x03 */
            {
               tSIRF_MSG_SSB_FREQ_TRANSFER_REQ * msg = (tSIRF_MSG_SSB_FREQ_TRANSFER_REQ *) message_structure;
              
               if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len ) )
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
    case SIRF_MSG_SSB_NBA_REQ:  /* 0x49 , 0x04 */
      {
         /* Construct a pointer to the structure. */
         tSIRF_MSG_SSB_SLC_AIDING_REQ * msg = ( tSIRF_MSG_SSB_SLC_AIDING_REQ *)message_structure;
         
         if ( payload_length < ( 2 * sizeof(tSIRF_UINT8) + 
                                 1 * sizeof(tSIRF_UINT16) + 
                                 1 * sizeof(tSIRF_UINT32) + 
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
            msg->section_validity_flag                 = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->section_size                          = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->sat_mask_nav_bit                      = SIRFBINARY_IMPORT_UINT32(ptr);
            msg->nav_bit_req_flag                      = SIRFBINARY_IMPORT_UINT8(ptr);
         }
      }
      break;


    case SIRF_MSG_SSB_SESSION_OPEN_RSP:  /* 0x4A , 0x01 */
            {
               tSIRF_MSG_SSB_SESSION_OPEN_RSP * msg = 
                  (tSIRF_MSG_SSB_SESSION_OPEN_RSP *) message_structure;
                  
               if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len ) )
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
    case SIRF_MSG_SSB_SESSION_CLOSE_RSP: /* 0x4A , 0x02 */
            {
               tSIRF_MSG_SSB_SESSION_CLOSE_RSP * msg = 
                  (tSIRF_MSG_SSB_SESSION_CLOSE_RSP *) message_structure;
                  
               if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len ) )
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

   case SIRF_MSG_SSB_ACK_NACK_ERROR_OUT: /* 0x4B , 0x01 */
            {
               tSIRF_MSG_SSB_ACKNACK_ERROR* msg = (tSIRF_MSG_SSB_ACKNACK_ERROR *) message_structure;
               
               if ( payload_length < (3 * sizeof(tSIRF_UINT8) + 
                                                    1 * sizeof(tSIRF_UINT16) +
                                                    header_len ) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->msg_id = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->sub_id = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->session_error_reason = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->reserved = SIRFBINARY_IMPORT_UINT16(ptr);
               }
            }
            break;

   case SIRF_MSG_SSB_REJECT_OUT: /* 0x4B , 0x02 */
            {
               tSIRF_MSG_SSB_REJECT_SLC * msg = (tSIRF_MSG_SSB_REJECT_SLC *) message_structure;
               
               if ( payload_length < (3 * sizeof(tSIRF_UINT8) + header_len ) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->rej_msgid = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->rej_msgsubid= SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->reason = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

    case SIRF_MSG_SSB_POS_REQ: /* 0xD2  */
      {
         tSIRF_MSG_SSB_POS_REQ * msg = (tSIRF_MSG_SSB_POS_REQ*) message_structure;
         
         if ( payload_length < ( 8 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
            msg->pos_req_id                            = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->num_fixes               = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->time_btw_fixes          = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->hori_error_max          = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->vert_error_max          = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->resp_time_max           = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->time_acc_priority       = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->location_method                       = SIRFBINARY_IMPORT_UINT8(ptr);
         }
      }
    break;
    case SIRF_MSG_SSB_SET_IONO:  /* 0xD3 , 0x01 */
       {
         tSIRF_MSG_SSB_SET_IONO_DATA* msg = (tSIRF_MSG_SSB_SET_IONO_DATA*) message_structure;
         
         if ( payload_length < ( 8 * sizeof(tSIRF_UINT8) + header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
            msg->alpha_0                     = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->alpha_1                     = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->alpha_2                     = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->alpha_3                     = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->beta_0                      = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->beta_1                      = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->beta_2                      = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->beta_3                      = SIRFBINARY_IMPORT_SINT8(ptr);
         }
      }
     break;
    case SIRF_MSG_SSB_SET_EPH_CLK: /* 0xD3 , 0x02 */
      {
         tSIRF_MSG_SSB_SET_EPH_CLK* msg = (tSIRF_MSG_SSB_SET_EPH_CLK*) message_structure;

         /* This message is variable length, but data gets placed in an array of 
            size  SIRF_MAX_SVID_CNT. Need to ensure that data from array[num_Sv] to 
            array[SIRF_MAX_SVID_CNT] is initialized to zero after populating 
            array[0] through array[num_Sv-1]
         */
         memset( &msg->sv_eph_prm, 0,(sizeof(tSIRF_MSG_SSB_EPHEMERIS_INFO_PER_SV)*SIRF_MAX_SVID_CNT));

         if ( payload_length < (1 * sizeof(tSIRF_UINT8) + 
                                 header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
   
            msg->num_sv= SIRFBINARY_IMPORT_UINT8(ptr);
            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + 
                                 (6 * msg->num_sv) * sizeof(tSIRF_UINT8) +
                                 (11 * msg->num_sv) * sizeof(tSIRF_UINT16) +
                                 (8 * msg->num_sv) * sizeof(tSIRF_UINT32) + 
                                 header_len))
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else if (msg->num_sv > SIRF_MAX_SVID_CNT)
            {
               tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
            }
            else
            {
               for (i = 0; i < msg->num_sv; ++i) 
               {
                  tSIRF_MSG_SSB_EPHEMERIS_INFO_PER_SV* eph = &msg->sv_eph_prm[i];
      
                  eph->eph_flag                           = SIRFBINARY_IMPORT_UINT8(ptr);
                  eph->eph_data.sv_prn_num                = SIRFBINARY_IMPORT_UINT8(ptr);
                  eph->eph_data.ura_ind                   = SIRFBINARY_IMPORT_UINT8(ptr);
                  eph->eph_data.iode                      = SIRFBINARY_IMPORT_UINT8(ptr);
                  eph->eph_data.c_rs                      = SIRFBINARY_IMPORT_SINT16(ptr);
                  eph->eph_data.delta_n                   = SIRFBINARY_IMPORT_SINT16(ptr);
                  eph->eph_data.m0                        = SIRFBINARY_IMPORT_SINT32(ptr);
                  eph->eph_data.c_uc                      = SIRFBINARY_IMPORT_SINT16(ptr);
                  eph->eph_data.eccentricity              = SIRFBINARY_IMPORT_UINT32(ptr);
                  eph->eph_data.c_us                      = SIRFBINARY_IMPORT_SINT16(ptr);
                  eph->eph_data.a_sqrt                    = SIRFBINARY_IMPORT_UINT32(ptr);
                  eph->eph_data.toe                       = SIRFBINARY_IMPORT_UINT16(ptr);
                  eph->eph_data.c_ic                      = SIRFBINARY_IMPORT_SINT16(ptr);
                  eph->eph_data.omega_0                   = SIRFBINARY_IMPORT_SINT32(ptr);
                  eph->eph_data.c_is                      = SIRFBINARY_IMPORT_SINT16(ptr);
                  eph->eph_data.i0                        = SIRFBINARY_IMPORT_SINT32(ptr);
                  eph->eph_data.c_rc                      = SIRFBINARY_IMPORT_SINT16(ptr);
                  eph->eph_data.omega                     = SIRFBINARY_IMPORT_SINT32(ptr);
                  eph->eph_data.omegadot                  = SIRFBINARY_IMPORT_SINT32(ptr);
                  eph->eph_data.idot                      = SIRFBINARY_IMPORT_SINT16(ptr);
                  eph->eph_data.toc                       = SIRFBINARY_IMPORT_UINT16(ptr);
                  eph->eph_data.t_gd                      = SIRFBINARY_IMPORT_SINT8(ptr);
                  eph->eph_data.af2                       = SIRFBINARY_IMPORT_SINT8(ptr);
                  eph->eph_data.af1                       = SIRFBINARY_IMPORT_SINT16(ptr);
                  eph->eph_data.af0                       = SIRFBINARY_IMPORT_SINT32(ptr);
               }
            }
         }
      }
    break;
    case SIRF_MSG_SSB_SET_ALM:  /* 0xD3 , 0x03 */
       {
         tSIRF_MSG_SSB_SET_ALMANAC_ASSIST_DATA* msg = (tSIRF_MSG_SSB_SET_ALMANAC_ASSIST_DATA*) message_structure;
         if ( payload_length <  ((1* sizeof(tSIRF_UINT8) + 
                                  1 * sizeof(tSIRF_UINT16) + 
                                 header_len) ))
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
            msg->alm_week_num                 = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->num_sv                            = SIRFBINARY_IMPORT_UINT8(ptr);

            if ( payload_length <  ((1* sizeof(tSIRF_UINT8) + 
                                  1 * sizeof(tSIRF_UINT16) + 
                                 (3 * msg->num_sv) * sizeof(tSIRF_UINT8) +
                                 (5 * msg->num_sv) * sizeof(tSIRF_UINT16) +
                                 (4 * msg->num_sv) * sizeof(tSIRF_UINT32) + 
                                 header_len) ))
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else if(msg->num_sv > SIRF_MAX_SVID_CNT)
            {
               tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
            }
            else
            {
               for (i = 0; i < msg->num_sv; ++i) 
               {
                  tSIRF_MSG_SSB_ALMANAC_PARAMETERS* alm = &msg->alm_params[i];
                  alm->alm_valid_flag                     = SIRFBINARY_IMPORT_UINT8(ptr);
                  alm->alm_sv_prn_num                     = SIRFBINARY_IMPORT_UINT8(ptr);
                  alm->alm_eccentricity                   = SIRFBINARY_IMPORT_UINT16(ptr);
                  alm->alm_toa                            = SIRFBINARY_IMPORT_UINT8(ptr);
                  alm->alm_delta_incl                     = SIRFBINARY_IMPORT_SINT16(ptr);
                  alm->alm_omegadot                       = SIRFBINARY_IMPORT_SINT16(ptr);
                  alm->alm_a_sqrt                         = SIRFBINARY_IMPORT_UINT24(ptr);
                  alm->alm_omega_0                        = SIRFBINARY_IMPORT_SINT24(ptr);
                  alm->alm_omega                          = SIRFBINARY_IMPORT_SINT24(ptr);
                  alm->alm_m0                             = SIRFBINARY_IMPORT_SINT24(ptr);
                  alm->alm_af0                            = SIRFBINARY_IMPORT_SINT16(ptr);
                  alm->alm_af1                            = SIRFBINARY_IMPORT_SINT16(ptr);
               }
            }
         }
      }
     break;
    case SIRF_MSG_SSB_SET_ACQ_ASSIST:  /* 0xD3 , 0x04 */
    {
         tSIRF_MSG_SSB_SET_ACQ_ASSIST_DATA * msg = (tSIRF_MSG_SSB_SET_ACQ_ASSIST_DATA*) message_structure;
         if ( payload_length < ( (1 * sizeof(tSIRF_UINT8) + 
                                  1 * sizeof(tSIRF_UINT32) +
                                 header_len) ))
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);

            msg->reference_time               = SIRFBINARY_IMPORT_UINT32(ptr);   
            msg->num_sv                       = SIRFBINARY_IMPORT_UINT8(ptr);
            if ( payload_length < ( (1 * sizeof(tSIRF_UINT8) + 
                                     1* sizeof(tSIRF_UINT32) +
                                    (7 * msg->num_sv) * sizeof(tSIRF_UINT8) +
                                    (4 * msg->num_sv) * sizeof(tSIRF_UINT16) +
                                    header_len)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else if (msg->num_sv > SIRF_MAX_SVID_CNT)
            {
               tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
            }
            else
            {
               /* 32 instances of the following table:   */
               for (i = 0; i < msg->num_sv; ++i) 
               {
                  tSIRF_MSG_SSB_ACQUISITION_ASSISTANCE_DATA* acq = &msg->acq_data[i];
      
                  acq->acq_assist_valid_flag              = SIRFBINARY_IMPORT_UINT8(ptr);
                  acq->sv_prn_number                      = SIRFBINARY_IMPORT_UINT8(ptr);
                  acq->doppler0                           = SIRFBINARY_IMPORT_SINT16(ptr);
                  acq->doppler1                           = SIRFBINARY_IMPORT_SINT8(ptr);
                  acq->doppler_uncertainty                = SIRFBINARY_IMPORT_UINT8(ptr);
                  acq->sv_code_phase                      = SIRFBINARY_IMPORT_UINT16(ptr);
                  acq->sv_code_phase_int                  = SIRFBINARY_IMPORT_UINT8(ptr);
                  acq->gps_bit_num                        = SIRFBINARY_IMPORT_UINT8(ptr);
                  acq->code_phase_uncertainty             = SIRFBINARY_IMPORT_UINT16(ptr);
                  acq->azimuth                            = SIRFBINARY_IMPORT_UINT16(ptr);
                  acq->elevation                          = SIRFBINARY_IMPORT_SINT8(ptr);
               }
            }
         }
      }
     break;
    case SIRF_MSG_SSB_SET_RT_INTEG:  /* 0xD3 , 0x05 */
      {
         tSIRF_MSG_SSB_SET_RT_INTEGRITY* msg = (tSIRF_MSG_SSB_SET_RT_INTEGRITY*) message_structure;
         
         if ( payload_length < ( (1 * sizeof(tSIRF_UINT32) +
                                                  header_len) ))
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg); 
            msg->unhealthy_sat_info                    = SIRFBINARY_IMPORT_UINT32(ptr);
         }
      }
     break;
    case SIRF_MSG_SSB_SET_UTC_MODEL: /* 0xD3 , 0x06 */
{
         tSIRF_MSG_SSB_SET_UTC_MODEL* msg = (tSIRF_MSG_SSB_SET_UTC_MODEL *)message_structure;
         
         if ( payload_length < (6 * sizeof(tSIRF_UINT8) + 
                                2 * sizeof(tSIRF_UINT32) +
                                header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
               msg->a0                                 = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->a1                                 = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->delta_tls                          = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->tot                                = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->wnt                                = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->wnlsf                              = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->dn                                 = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->delta_t_lsf                        = SIRFBINARY_IMPORT_UINT8(ptr);

         }
      }
    break;
    case SIRF_MSG_SSB_SET_GPS_TOW_ASSIST:  /* 0xD3 , 0x07 */
      {
         tSIRF_MSG_SSB_SET_GPS_TOW_ASSIST* msg = (tSIRF_MSG_SSB_SET_GPS_TOW_ASSIST *)message_structure;
         if (payload_length < (1 * sizeof(tSIRF_UINT8) + 
                                header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
   
            msg->num_sv= SIRFBINARY_IMPORT_UINT8(ptr);
            if ( payload_length < ((1 * sizeof(tSIRF_UINT8) + 
                                  (2 * msg->num_sv) * sizeof(tSIRF_UINT8) +
                                  (1 * msg->num_sv) * sizeof(tSIRF_UINT16) +
                                   header_len)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else if (msg->num_sv > SIRF_MAX_SVID_CNT)
            {
               tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
            }
            else
            {
               for(i = 0; i < msg->num_sv; i++)
               {
                  tSIRF_MSG_SSB_AUXILIARY_NAV_TOW_ASSIST *aux = &msg->set_gps_tow_assist[i];
                  aux->svid                               = SIRFBINARY_IMPORT_UINT8(ptr);
                  aux->tlm_msg                            = SIRFBINARY_IMPORT_UINT16(ptr);
                  aux->tow_assist_info                    = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
         }
      }
     break;
    case SIRF_MSG_SSB_SET_AUX_NAV:  /* 0xD3 , 0x08 */
      {
         tSIRF_MSG_SSB_SET_AUXILIARY_NAV_PARAMS* msg = (tSIRF_MSG_SSB_SET_AUXILIARY_NAV_PARAMS *)message_structure;
         if ( payload_length < (1 * sizeof(tSIRF_UINT8) + 
                                             header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
   
             msg->num_sv = SIRFBINARY_IMPORT_UINT8(ptr);
            /*The structure shall include 32 occurrences of     */
            /*auxiliary navigation model parameters             */
            if ( payload_length < (1 * sizeof(tSIRF_UINT8) + 
                                  (5 * msg->num_sv) * sizeof(tSIRF_UINT8) + 
                                  (2 * msg->num_sv) * sizeof(tSIRF_UINT16) +
                                    header_len) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else if (msg->num_sv > SIRF_MAX_SVID_CNT)
            {
               tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
            }
            else
            {
               for(i = 0; i < msg->num_sv ; i++)
               {
                  tSIRF_MSG_SSB_AUXILIARY_NAV_PARAMS *aux_nav = &msg->set_aux_nav_model_params[i];
                  aux_nav->svid                           = SIRFBINARY_IMPORT_UINT8(ptr);
                  aux_nav->toe                            = SIRFBINARY_IMPORT_UINT16(ptr);
                  aux_nav->iodc                           = SIRFBINARY_IMPORT_UINT16(ptr);
                  aux_nav->sf1_l2_info                    = SIRFBINARY_IMPORT_UINT8(ptr);
                  aux_nav->sf1_sv_health                  = SIRFBINARY_IMPORT_UINT8(ptr);
      
                  /* Import the 11 reserved bytes. */
                  memcpy(&msg->set_aux_nav_model_params[i].sf1_reserved, 
                         ptr, 
                         SIRF_MSG_SSB_ANP_NUM_SF1_RESERVED);
                  ptr += SIRF_MSG_SSB_ANP_NUM_SF1_RESERVED;
                  aux_nav->sf1_aodo_fit_interval          = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
         }
      }
      break;
    case SIRF_MSG_SSB_SET_AIDING_AVAIL:  /* 0xD3 , 0x09 */
           {
               tSIRF_MSG_SSB_PUSH_AIDING_AVAILABILITY_REQ * msg = 
                  (tSIRF_MSG_SSB_PUSH_AIDING_AVAILABILITY_REQ *) message_structure;
                  
               if ( payload_length < ((6 * sizeof(tSIRF_UINT8) + 
                                      2 * sizeof(tSIRF_UINT16) +
                                      header_len ) ))
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

    case SIRF_MSG_SSB_EPH_REQ:  /* 0xD4 , 0x01 */
          {
         /* No members. */
         memset(message_structure,0,sizeof(tSIRF_MSG_SSB_SLC_EPH_STATUS_REQ));
         *message_length = sizeof(tSIRF_MSG_SSB_SLC_EPH_STATUS_REQ);
        }
       break;
    case SIRF_MSG_SSB_ALM_REQ:  /* 0xD4 , 0x02 */
          {
         /* No members. */
         memset(message_structure,0,sizeof(tSIRF_MSG_SSB_ALMANAC_REQ));
         *message_length = sizeof(tSIRF_MSG_SSB_ALMANAC_REQ);
          }
       break;
    case SIRF_MSG_SSB_B_EPH_REQ: /* 0xD4 , 0x03 */
        {    
         /* Construct a pointer to the structure. */
         tSIRF_MSG_SSB_BE_REQ * msg = ( tSIRF_MSG_SSB_BE_REQ *)message_structure;
         if ( payload_length < (( 1 * sizeof(tSIRF_UINT8) +
                                 1 * sizeof(tSIRF_UINT16) + 
                                 header_len)) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
   
            /* Export members. */
            msg->eph_resp_trigger                      = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->num_sv                                   = SIRFBINARY_IMPORT_UINT8(ptr);
            if ( payload_length < (( (1+ 2 * msg->num_sv) * sizeof(tSIRF_UINT8) +
                                    (1 + 2 * msg->num_sv) * sizeof(tSIRF_UINT16) + 
                                    header_len)) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else if (msg->num_sv > SIRF_MAX_SVID_CNT)
            {
               tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
            }
            else
            {
               for (i = 0; i < msg->num_sv; ++i) 
               {
                  tSIRF_MSG_SSB_BE_REQ_PARAMS* eph_info = &msg->eph_params[i];
                  eph_info->eph_info_flag                 = SIRFBINARY_IMPORT_UINT8(ptr);
                  eph_info->sv_prn_num                    = SIRFBINARY_IMPORT_UINT8(ptr);
                  eph_info->gps_week                      = SIRFBINARY_IMPORT_UINT16(ptr);
                  eph_info->toe                           = SIRFBINARY_IMPORT_UINT16(ptr);
               }
            }
         }
      }
      break;
    case SIRF_MSG_SSB_TIME_FREQ_APPROX_POS_REQ:  /* 0xD4 , 0x04 */
            {
               tSIRF_MSG_SSB_TIME_FREQ_APROX_POS_REQ * msg = 
                  (tSIRF_MSG_SSB_TIME_FREQ_APROX_POS_REQ *) message_structure;
                  
               if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len ) )
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
    case SIRF_MSG_SSB_CH_LOAD_REQ:  /* 0xD4 , 0x05 */
           {
               tSIRF_MSG_SSB_CHANNEL_LOAD_QUERY * msg = (tSIRF_MSG_SSB_CHANNEL_LOAD_QUERY *) message_structure;
               
               if ( payload_length < (2 * sizeof(tSIRF_UINT8) + header_len ) )
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
    case SIRF_MSG_SSB_CLIENT_STATUS_REQ: /* 0xD4 , 0x06 */
           {
            memset( message_structure, 0, sizeof(tSIRF_MSG_SSB_SLC_STATUS_REQ) );
            *message_length = sizeof (tSIRF_MSG_SSB_SLC_STATUS_REQ);
           }
         break;
    case SIRF_MSG_SSB_OSP_REV_REQ: /* 0xD4 , 0x07 */
           {
            memset( message_structure, 0, sizeof(tSIRF_MSG_SSB_OSP_REV_REQ) );
            *message_length = sizeof (tSIRF_MSG_SSB_OSP_REV_REQ);
           }
         break;
    case SIRF_MSG_SSB_SERIAL_SETTING_REQ: /* 0xD4 , 0x08 */
            {
               tSIRF_MSG_SSB_SERIAL_PORT_SETTING_REQ * msg = 
                  (tSIRF_MSG_SSB_SERIAL_PORT_SETTING_REQ *) message_structure;
               
               if ( payload_length < (5* sizeof(tSIRF_UINT8) + 
                                                    1 * sizeof(tSIRF_UINT32) +
                                                    header_len ) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->baud = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->data_bits = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->stop_bits = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->parity = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->port = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->reserved = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;
    case SIRF_MSG_SSB_TX_BLANKING_REQ: /* 0xD4 , 0x09 */
            {
               tSIRF_MSG_SSB_TX_BLANK_REQ* msg = (tSIRF_MSG_SSB_TX_BLANK_REQ *) message_structure;
               
               if ( payload_length < (3 * sizeof(tSIRF_UINT8) + 
                                                   1 * sizeof(tSIRF_UINT32) +
                                                   header_len ) )
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
    case SIRF_MSG_SSB_SESSION_OPEN_REQ:  /* 0xD5 , 0x01 */
         {
               tSIRF_MSG_SSB_SESSION_OPEN_REQ * msg = (tSIRF_MSG_SSB_SESSION_OPEN_REQ *) message_structure;
               
               /* Check to make sure that the buffer we are reading from is big enough */
               if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len ) )
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
    case SIRF_MSG_SSB_SESSION_CLOSE_REQ: /* 0xD5 , 0x02 */
           {
               tSIRF_MSG_SSB_SESSION_CLOSE_REQ * msg = (tSIRF_MSG_SSB_SESSION_CLOSE_REQ *) message_structure;
               
               if ( payload_length < (1 * sizeof(tSIRF_UINT8) + header_len ) )
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

    case SIRF_MSG_SSB_HW_CONFIG_RSP: /* 0xD6 */
            {
               tSIRF_MSG_SSB_HW_CONFIG_RSP * msg = 
                  (tSIRF_MSG_SSB_HW_CONFIG_RSP *) message_structure;
                  
               if ( payload_length < (3 * sizeof(tSIRF_UINT8) + 
                                      1 * sizeof(tSIRF_UINT32) +
                                      header_len ) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->hw_config_msg = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->nominal_freq_high = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->nominal_freq_low = SIRFBINARY_IMPORT_UINT32(ptr);
                  msg->nw_enhance_type= SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;
    case SIRF_MSG_SSB_APRX_MS_POSITION_RSP:  /* 0xD7 , 0x01 */
           {
               tSIRF_MSG_SSB_APRX_MS_POSITION_RSP * msg = (tSIRF_MSG_SSB_APRX_MS_POSITION_RSP *) message_structure;
               
               if ( payload_length < (2 * sizeof(tSIRF_UINT8) + 
                                      2 * sizeof(tSIRF_UINT16) +
                                      2 * sizeof(tSIRF_UINT32) +
                                      header_len ) )
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
    case SIRF_MSG_SSB_TIME_TX_RSP:  /* 0xD7 , 0x02 */
            {
               tSIRF_MSG_SSB_TIME_TRANSFER_RSP * msg = (tSIRF_MSG_SSB_TIME_TRANSFER_RSP *) message_structure;
               
               if ( payload_length < (3 * sizeof(tSIRF_UINT8) + 
                                      1 * sizeof(tSIRF_UINT16) +
                                      3 /* 1 * sizeof(tSIRF_INT24) */ + 
                                      1 * sizeof(tSIRF_UINT32) +
                                      header_len ) )
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
    case SIRF_MSG_SSB_FREQ_TX_RSP:  /* 0xD7 , 0x03 */
            {
               tSIRF_MSG_SSB_FREQ_TRANSFER_RSP * msg = (tSIRF_MSG_SSB_FREQ_TRANSFER_RSP *) message_structure;
               /* Minimum size is 8 bytes + LC + MID, maximum is 13 + LC + MID */
               if ( payload_length < (2 * sizeof(tSIRF_UINT8) + 
                                      1 * sizeof(tSIRF_UINT16) +
                                      1 * sizeof(tSIRF_UINT32) +
                                      header_len ) )
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
                  if( (msg->clock_ref & SIRF_MSG_SSB_FTRSP_RCI_NOMINAL_FREQ_INCLUDED_HERE) )
                  { 
                     if( payload_length < (3 * sizeof(tSIRF_UINT8)  + 
                                            1 * sizeof(tSIRF_UINT16) +
                                            2 * sizeof(tSIRF_UINT32) +
                                            header_len ) )
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
    case SIRF_MSG_SSB_SET_NBA_SF1_2_3:  /* 0xD7 , 0x04 */
      {
         /* Construct a pointer to the structure. */
         tSIRF_MSG_SSB_NAV_SF_123_RSP * msg = (tSIRF_MSG_SSB_NAV_SF_123_RSP *)message_structure;
         
         if ( payload_length < (1*sizeof(tSIRF_UINT8) + 
                                2 * SIRF_MSG_SSB_ANS123R_NUM_SF123 * sizeof(tSIRF_UINT8) + 
                                SIRF_MSG_SSB_ANS123R_NUM_SF123 * SIRF_MSG_SSB_ANS123R_NUM_BYTES_PER_SF123 +
                                header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
   
            /* Import members. */
            for (i = 0; i < SIRF_MSG_SSB_ANS123R_NUM_SF123; ++i) 
            {
               tSIRF_MSG_SSB_NAV_SF_123* sf123 = (tSIRF_MSG_SSB_NAV_SF_123*)&msg->sf123[i];
               sf123->sf_123_flag                      = SIRFBINARY_IMPORT_UINT8(ptr);
               sf123->sv_id                            = SIRFBINARY_IMPORT_UINT8(ptr);
               memcpy(sf123->sf123,ptr,SIRF_MSG_SSB_ANS123R_NUM_BYTES_PER_SF123);
               ptr += SIRF_MSG_SSB_ANS123R_NUM_BYTES_PER_SF123;
            }
         }
      }
      break;
    case SIRF_MSG_SSB_SET_NBA_SF4_5:  /* 0xD7 , 0x05 */
      {
         /* Construct a pointer to the structure. */
         tSIRF_MSG_SSB_NAV_SF_45_RSP * msg = (tSIRF_MSG_SSB_NAV_SF_45_RSP *)message_structure;
         
         if ( payload_length < (SIRF_MSG_SSB_ANS45R_NUM_SF45 * sizeof(tSIRF_UINT8) + 
                                SIRF_MSG_SSB_ANS45R_NUM_SF45 * SIRF_MSG_SSB_ANS45R_NUM_BYTES_PER_SF45 +
                                1 * sizeof(tSIRF_UINT32) +
                                header_len) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
   
            /* Import members. */
            msg->sv_list                               = SIRFBINARY_IMPORT_UINT32(ptr);
            for (i = 0u; i < SIRF_MSG_SSB_ANS45R_NUM_SF45; ++i) 
            {
               msg->sf45[i].frame_num                 = SIRFBINARY_IMPORT_UINT8(ptr);
                  
               /* Import the 75 bytes (600 bits) of subframes 4/5. */
               memcpy(msg->sf45[i].SF45, ptr, SIRF_MSG_SSB_ANS45R_NUM_BYTES_PER_SF45);
               ptr += SIRF_MSG_SSB_ANS45R_NUM_BYTES_PER_SF45;
            }
         }
      }
      break;
    case SIRF_MSG_SSB_ACK_NACK_ERROR_IN:  /* 0xD8 , 0x01 */
            {
               tSIRF_MSG_SSB_ACKNACK_ERROR* msg = (tSIRF_MSG_SSB_ACKNACK_ERROR *) message_structure;
               
               if ( payload_length < ((3 * sizeof(tSIRF_UINT8) + 
                                                   1 * sizeof(tSIRF_UINT16) + 
                                                   header_len ) ))
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length = sizeof(*msg);
                  msg->msg_id = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->sub_id= SIRFBINARY_IMPORT_UINT8(ptr);  
                  msg->session_error_reason= SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->reserved= SIRFBINARY_IMPORT_UINT16(ptr);
               }
            }
            break;
    case SIRF_MSG_SSB_REJECT_IN:  /* 0xD8 , 0x02 */
            {
               tSIRF_MSG_SSB_REJECT_CP * msg = (tSIRF_MSG_SSB_REJECT_CP *) message_structure;
               
               if ( payload_length < (3 * sizeof(tSIRF_UINT8) + header_len ) )
               {
                  tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
               }
               else
               {
                  *message_length   = sizeof(*msg);
                  msg->rej_msgid    = SIRFBINARY_IMPORT_UINT8(ptr);
                  msg->rej_msgsubid = SIRFBINARY_IMPORT_UINT8(ptr);  
                  msg->reason       = SIRFBINARY_IMPORT_UINT8(ptr);
               }
            }
            break;

    case SIRF_MSG_SSB_PWR_MODE_APM_REQ:            /* 0xDA, 0x1*/
         {
            tSIRF_MSG_SSB_PWR_MODE_APM_REQ * msg = (tSIRF_MSG_SSB_PWR_MODE_APM_REQ *) message_structure;
               
            if ( payload_length < (8 * sizeof(tSIRF_UINT8) + 
                                   2 * sizeof(tSIRF_UINT32) +
                                   header_len ) )
            {
               tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }
            else
            {
               *message_length             = sizeof(*msg);
               msg->num_fixes              = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->tbf                    = SIRFBINARY_IMPORT_UINT8(ptr); 
               msg->duty_cycle             = SIRFBINARY_IMPORT_UINT8(ptr); 
               msg->max_horz_error         = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->max_vert_error         = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->priority               = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->max_off_time           = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->max_search_time        = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->time_accuracy_priority = SIRFBINARY_IMPORT_UINT8(ptr); 
               msg->reserved               = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;

    case SIRF_MSG_SSB_SIRF_STATS_AGPS:    /* 0xE1 0x07 */
         {
            tSIRF_MSG_SSB_SIRF_STATS_AGPS *msg = (tSIRF_MSG_SSB_SIRF_STATS_AGPS *) message_structure;
            tSIRF_UINT32 length_requirement = ( 6 * sizeof(tSIRF_UINT32) + 
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
               msg->stats.ttff_since_reset       = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->stats.ttff_since_aiding      = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->stats.ttff_first_nav         = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->stats.pos_aiding_error_north = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->stats.pos_aiding_error_east  = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->stats.pos_aiding_error_down  = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->stats.time_aiding_error      = SIRFBINARY_IMPORT_SINT32(ptr);
               msg->stats.freq_aiding_error      = SIRFBINARY_IMPORT_SINT16(ptr);
               msg->stats.hor_pos_uncertainty    = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->stats.ver_pos_uncertainty    = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->stats.time_uncertainty       = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->stats.freq_uncertainty       = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->stats.num_aided_ephemeris    = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->stats.num_aided_acq_assist   = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->stats.nav_mode               = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->stats.pos_mode               = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->stats.nav_status             = SIRFBINARY_IMPORT_UINT16(ptr);
               msg->stats.start_mode             = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->stats.aiding_status          = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->clock_drift                  = SIRFBINARY_IMPORT_UINT32(ptr);
               msg->reserved                     = SIRFBINARY_IMPORT_UINT32(ptr);
            }
         }
         break;

    case SIRF_MSG_SSB_POS_FROMPART_REQ: /* 0xE8, 0xD0 */
         {
            /* Construct a pointer to the structure. */
            tSIRF_MSG_SSB_PARTPOS_RSP* msg = (tSIRF_MSG_SSB_PARTPOS_RSP *)message_structure;

            *message_length = sizeof(*msg);
            msg->pos_req_id                = SIRFBINARY_IMPORT_UINT8 (ptr);
            msg->pos_results_flag          = SIRFBINARY_IMPORT_UINT8 (ptr);
            msg->pos_err_status            = SIRFBINARY_IMPORT_UINT8 (ptr);
            msg->qos_value                 = SIRFBINARY_IMPORT_SINT16(ptr);
            msg->pos_type                  = SIRFBINARY_IMPORT_UINT8 (ptr);
            msg->dgps_cor                  = SIRFBINARY_IMPORT_UINT8 (ptr);
            msg->gps_week                  = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->lat                       = SIRFBINARY_IMPORT_SINT32(ptr);
            msg->lon                       = SIRFBINARY_IMPORT_SINT32(ptr);
            msg->vert_pos_height           = SIRFBINARY_IMPORT_UINT16(ptr);
            /* Velocity Section */
            msg->velocity_horizontal       = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->velocity_heading          = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->velocity_vertical         = SIRFBINARY_IMPORT_SINT8(ptr);
            /* Measurements Section */
            SIRFBINARY_IMPORT_DOUBLE(msg->meas_time,       ptr);
            msg->num_sv_meas_cnt = SIRFBINARY_IMPORT_UINT8(ptr);
            for (i = 0; i < msg->num_sv_meas_cnt; i++)
            {
               msg->sat_meas[i].svid          = SIRFBINARY_IMPORT_UINT8(ptr);
               SIRFBINARY_IMPORT_DOUBLE(msg->sat_meas[i].pseudorange,   ptr);
               SIRFBINARY_IMPORT_FLOAT(msg->sat_meas[i].carrier_freq,   ptr);
               msg->sat_meas[i].sync_flags    = SIRFBINARY_IMPORT_UINT8(ptr);
               msg->sat_meas[i].ctoN          = SIRFBINARY_IMPORT_UINT8(ptr);
            }
            /* Sat States Section */
            SIRFBINARY_IMPORT_DOUBLE(msg->state_time,       ptr);
            msg->num_sv_state_cnt = SIRFBINARY_IMPORT_UINT8(ptr);
            for (i = 0; i < msg->num_sv_state_cnt; i++)
            {
               msg->sat_state[i].svid    = SIRFBINARY_IMPORT_UINT8(ptr);
               SIRFBINARY_IMPORT_DOUBLE(msg->sat_state[i].pos[0],  ptr);
               SIRFBINARY_IMPORT_DOUBLE(msg->sat_state[i].pos[1],  ptr);
               SIRFBINARY_IMPORT_DOUBLE(msg->sat_state[i].pos[2],  ptr);
               SIRFBINARY_IMPORT_DOUBLE(msg->sat_state[i].vel[0],  ptr);
               SIRFBINARY_IMPORT_DOUBLE(msg->sat_state[i].vel[1],  ptr);
               SIRFBINARY_IMPORT_DOUBLE(msg->sat_state[i].vel[2],  ptr);
               SIRFBINARY_IMPORT_DOUBLE(msg->sat_state[i].clk_bias,ptr);
               SIRFBINARY_IMPORT_FLOAT(msg->sat_state[i].clk_drift,ptr);
               SIRFBINARY_IMPORT_FLOAT(msg->sat_state[i].pos_var,  ptr);
               SIRFBINARY_IMPORT_FLOAT(msg->sat_state[i].clk_var,  ptr);
               SIRFBINARY_IMPORT_FLOAT(msg->sat_state[i].iono,     ptr);
               msg->sat_state[i].status  = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
         break;

         default:
         {
            if (( *message_id >= SIRF_MSG_SSB_PASSTHRU_INPUT_BEGIN   /* 0xB4 */
                 && *message_id <= SIRF_MSG_SSB_PASSTHRU_INPUT_END   /* 0xC7 */
                 && payload_length - 1 <= SIRF_MSG_SSB_MAX_MESSAGE_LEN
                 && payload_length > 0) || (SIRF_MSG_SSB_SIRF_INTERNAL == *message_id)) /* 0xE4 */
            { /* pass-thru */
               *message_length = payload_length - 1;
               memcpy(message_structure, ptr, payload_length - 1);
               ptr += payload_length - 1;
            }
            else
            { /* this message id is not imported */
               tRet = SIRF_CODEC_SSB_INVALID_MSG_ID;
            }
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

   return tRet;

} /* SIRF_CODEC_SSB_AGPS_Decode() */

/**
 * @}
 */
