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
 * FILENAME:  sirf_codec_ai3.c
 *
 * DESCRIPTION: Routines to convert data to and from a byte stream defined
 *              by the F/AI3 protocol from and to the Tracker API 
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
#include "sirf_msg_ai3.h"
#include "sirf_codec.h"
#include "sirf_codec_ai3.h"

/***************************************************************************
 * External Prototype Definitions
 ***************************************************************************/

/***************************************************************************
 * File-Global Macro Definitions
 ***************************************************************************/

 #define AI3_DECODE_HEADER_SIZE (2)
 
/***************************************************************************
 * Packing tables for GPS_AI3 Messages
 ***************************************************************************/

/** 
 * Encodes a message in structure format into a stream of bytes to be 
 * transmitted across a uart.  This function does not place the header
 * or footer or run-length compress the message.  Data will be of the form
 * packet[0] = LOGICAL_CHANNEL
 * packet[1] = Message ID
 * packet[2..*packet_length] encoded message
 * 
 * @param message_id         [in] 32 bit message id including LC, SID, and ID
 * @param message_structure  [in] pointer to structure to encode
 * @param message_length     [in] Length of the message structure in bytes
 * @param packet             [out] pointer to place the resulting byte stream
 * @param packet_length      [in/out] Length of data written into packet
 *
 * @note This function does not check to make sure that there is enough room
 * in packet to place the full contents of the encoded message.  The caller,
 * should make sure that their message buffer is large enough.  Since the 
 * size of the encoded buffer is always <= message_length, the caller should
 * use a buffer of size message_length for packet.
 *
 * @note This function also does not place the Logical channel and message id
 * into the buffer. The reason for this is that most AI3 messages are run
 * length compressed, and each segment needs its own unique header which does
 * include the logical channel and message id.
 * 
 * @return SIRF_SUCCESS if the message was fully encoded, SIRF_FAILURE otherwise
 */
tSIRF_RESULT SIRF_CODEC_AI3_Encode( tSIRF_UINT32 message_id,
                                    tSIRF_VOID *message_structure,
                                    tSIRF_UINT32 message_length,
                                    tSIRF_UINT8 *packet, 
                                    tSIRF_UINT32 *packet_length )
{
   tSIRF_RESULT tRet = SIRF_SUCCESS;
   
   /* Verify valid arguments. */
   if ( ((tSIRF_VOID *) NULL   != message_structure) &&
        ((tSIRF_UINT8 *) NULL  != packet) &&
        ((tSIRF_UINT32 *) NULL != packet_length))
   {
      tSIRF_INT32 ii;

      /* Get a pointer to the packet data. */
      tSIRF_UINT8 *ptr = packet;

      /* The fragment code inserts the logical channel and message Id */

      /* Switch on the type of message being encoded. */
      switch (message_id)
      {
      case SIRF_MSG_AI3_RSP:
      {
         /* Construct a pointer to the structure. */
         const tSIRF_MSG_AI3_RSP * msg = (const tSIRF_MSG_AI3_RSP *)message_structure;
  
         /**
          * Attention programmers: the code compares the count of data bytes to *packet_length:
          * it generates the count by scanning the data store and adding up all of the 
          * UINT8s then all of the UINT16s, then all of the UINT32s.
          */

         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( (32 + 3 * SIRF_MSG_AI3_RSP_NUM_SV_INFO) * sizeof(tSIRF_UINT8) + 
                                  (4 * SIRF_MSG_AI3_MEASUREMENT_DATA_MAX_SVS) * sizeof(tSIRF_UINT8) +
                                  (2 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) +
                                  (8 + 3 * SIRF_MSG_AI3_MEASUREMENT_DATA_MAX_SVS) * sizeof(tSIRF_UINT16) + 
                                  (1 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) +
                                  (7 * sizeof(tSIRF_UINT32) )) ))
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }
   
         SIRFBINARY_EXPORT8(msg->pos_results_flag,ptr);
         SIRFBINARY_EXPORT8(msg->pos_data.pos_err_status,ptr);
         SIRFBINARY_EXPORT8(msg->pos_data.qos_pass_flag,ptr);
         SIRFBINARY_EXPORT8(msg->pos_data.pos_type,ptr);
         SIRFBINARY_EXPORT8(msg->pos_data.dgps_cor,ptr); 
         SIRFBINARY_EXPORT16(msg->pos_data.gps_week,ptr);
         SIRFBINARY_EXPORT32(msg->pos_data.time_of_week,ptr);
         SIRFBINARY_EXPORT32(msg->pos_data.lat,ptr);
         SIRFBINARY_EXPORT32(msg->pos_data.lon,ptr);

         SIRFBINARY_EXPORT8(msg->pos_data.other_sections,ptr);
   
         /* Horizontal Error Section */
         SIRFBINARY_EXPORT8(msg->pos_data.hor_err_angle,ptr);
         SIRFBINARY_EXPORT8(msg->pos_data.hor_err_major,ptr);
         SIRFBINARY_EXPORT8(msg->pos_data.hor_err_minor,ptr);
   
         /* Vertical Position Section */
         SIRFBINARY_EXPORT16(msg->pos_data.vert_pos_height,ptr);
         SIRFBINARY_EXPORT8(msg->pos_data.vert_pos_std,ptr);
   
         /* Velocity Section */
         SIRFBINARY_EXPORT16(msg->pos_data.velocity_horizontal,ptr);
         SIRFBINARY_EXPORT16(msg->pos_data.velocity_heading,ptr);
         SIRFBINARY_EXPORT8(msg->pos_data.velocity_vertical,ptr);
         SIRFBINARY_EXPORT8(msg->pos_data.velocity_hor_err_ang,ptr);
         SIRFBINARY_EXPORT8(msg->pos_data.velocity_hor_err_maj_std,ptr);
         SIRFBINARY_EXPORT8(msg->pos_data.velocity_hor_err_min_std,ptr);
         SIRFBINARY_EXPORT8(msg->pos_data.velocity_ver_err,ptr);
   
         /* Clock Correction Section */
         SIRFBINARY_EXPORT8(msg->pos_data.clk_cor_time_ref,ptr);
         SIRFBINARY_EXPORT16(msg->pos_data.clk_cor_clk_bias,ptr);
         SIRFBINARY_EXPORT16(msg->pos_data.clk_cor_clk_drift,ptr);
         SIRFBINARY_EXPORT8(msg->pos_data.clk_cor_clk_std_err,ptr);
         SIRFBINARY_EXPORT8(msg->pos_data.clk_cor_utc_off,ptr);  
   
         /* Position Correction Section */
         SIRFBINARY_EXPORT8(msg->pos_data.pos_corr_num_sat,ptr);
         for (ii = 0; ii < SIRF_MSG_AI3_RSP_NUM_SV_INFO; ii++)
         {
            SIRFBINARY_EXPORT8(msg->pos_data.pos_corr_sv_info[ii].sv_prn,ptr);
            SIRFBINARY_EXPORT8(msg->pos_data.pos_corr_sv_info[ii].c_n0,ptr);
            SIRFBINARY_EXPORT8(msg->pos_data.pos_corr_sv_info[ii].inv_weights,ptr);
         }

         SIRFBINARY_EXPORT8(msg->alm_ref_date_flag,ptr);
         SIRFBINARY_EXPORT8(msg->alm_data_stat,ptr);
         SIRFBINARY_EXPORT16(msg->alm_week_num,ptr);
         SIRFBINARY_EXPORT8(msg->alm_toa,ptr);

         SIRFBINARY_EXPORT8(msg->gps_meas_flag,ptr);
         SIRFBINARY_EXPORT8(msg->gps_meas_data.meas_error_status,ptr);
         SIRFBINARY_EXPORT16(msg->gps_meas_data.meas_gps_week,ptr);
         SIRFBINARY_EXPORT32(msg->gps_meas_data.meas_gps_seconds,ptr);
         SIRFBINARY_EXPORT8(msg->gps_meas_data.time_accuracy,ptr);
         SIRFBINARY_EXPORT8(msg->gps_meas_data.num_sv,ptr);
         for (ii = 0; ii < SIRF_MSG_AI3_MEASUREMENT_DATA_MAX_SVS; ii++)
         {
            SIRFBINARY_EXPORT8(msg->gps_meas_data.meas_per_sv_data[ii].sv_prn,ptr);
            SIRFBINARY_EXPORT8(msg->gps_meas_data.meas_per_sv_data[ii].c_n0,ptr);
            SIRFBINARY_EXPORT16(msg->gps_meas_data.meas_per_sv_data[ii].sv_doppler,ptr);
            SIRFBINARY_EXPORT16(msg->gps_meas_data.meas_per_sv_data[ii].sv_code_phase_wh,ptr);
            SIRFBINARY_EXPORT16(msg->gps_meas_data.meas_per_sv_data[ii].sv_code_phase_fr,ptr);
            SIRFBINARY_EXPORT8(msg->gps_meas_data.meas_per_sv_data[ii].multipath_indicator,ptr);
            SIRFBINARY_EXPORT8(msg->gps_meas_data.meas_per_sv_data[ii].pseudorange_rms_error,ptr);
         }

         /* ICD 1.9 */
         SIRFBINARY_EXPORT8(msg->subalm_flag,ptr);
         SIRFBINARY_EXPORT8(msg->alm_data.data_stat,ptr);
         for (ii = 0; ii < SIRF_MAX_SVID_CNT; ii++)
         {         
            SIRFBINARY_EXPORT8(msg->alm_data.subalm_info[ii].subalm_sv_prn,ptr);
            SIRFBINARY_EXPORT16(msg->alm_data.subalm_info[ii].subalm_week_num,ptr);
            SIRFBINARY_EXPORT8(msg->alm_data.subalm_info[ii].subalm_toa,ptr);
         }

         SIRFBINARY_EXPORT8(msg->cp_valid_flag,ptr);
         SIRFBINARY_EXPORT8(msg->cp_data.cp_num_bytes,ptr);
         SIRFBINARY_EXPORT32(msg->cp_data.delta_lat,ptr);
         SIRFBINARY_EXPORT32(msg->cp_data.delta_lon,ptr);
         SIRFBINARY_EXPORT32(msg->cp_data.delta_alt,ptr);
         SIRFBINARY_EXPORT8(msg->cp_data.pr_err_th,ptr);

         /* ICD 2.0 */
         SIRFBINARY_EXPORT8(msg->pos_req_id,ptr);
      }
      break;
         
      case SIRF_MSG_AI3_ACK_NACK:
      {
         /* Construct a pointer to the structure. */
         const tSIRF_MSG_AI3_ACK_NACK * msg = (const tSIRF_MSG_AI3_ACK_NACK *)message_structure;
         
         if ( (sizeof(*msg) != message_length) || (*packet_length < 1 * sizeof(tSIRF_UINT8)) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

         /* Export members. */
         SIRFBINARY_EXPORT8(msg->acknak,ptr);
      }
      break;
         
      case SIRF_MSG_AI3_SLC_AIDING_REQ:
      {
         /* Construct a pointer to the structure. */
         const tSIRF_MSG_AI3_SLC_AIDING_REQ * msg = (const tSIRF_MSG_AI3_SLC_AIDING_REQ *)message_structure;
         
         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < (2 * sizeof(tSIRF_UINT8) + 
                                 1 * sizeof(tSIRF_UINT16) + 
                                 1 * sizeof(tSIRF_UINT32) )) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

         SIRFBINARY_EXPORT16(msg->section_validity_flag,ptr);
         SIRFBINARY_EXPORT8(msg->section_size,ptr);
         SIRFBINARY_EXPORT32(msg->sat_mask_nav_bit,ptr);
         SIRFBINARY_EXPORT8(msg->nav_bit_req_flag,ptr);
      }
      break;
      
      case SIRF_MSG_AI3_UNSOL_SLC_EPH_STATUS_RSP:
      case SIRF_MSG_AI3_SOL_SLC_EPH_STATUS_RSP:
      {
         /* Construct a pointer to the structure. */
         const tSIRF_MSG_AI3_SLC_EPH_STATUS_RSP * msg = 
            (const tSIRF_MSG_AI3_SLC_EPH_STATUS_RSP *)message_structure;
         
         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( (3 + 4 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) + 
                                  (3 + 3 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) + 
                                  1 * sizeof(tSIRF_UINT32) )) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

         /* Export members. */
         SIRFBINARY_EXPORT16(msg->agps_status_mask,ptr);
         SIRFBINARY_EXPORT8(msg->gps_time_mask,ptr);
         SIRFBINARY_EXPORT16(msg->gps_week,ptr);
         SIRFBINARY_EXPORT32(msg->gps_tow,ptr);
         SIRFBINARY_EXPORT16(msg->eph_status_section_size,ptr);
         SIRFBINARY_EXPORT8(msg->eph_aiding_type,ptr);
         SIRFBINARY_EXPORT8(msg->gps_time_tolerance_limit,ptr);

         for (ii = 0u; ii < SIRF_MAX_SVID_CNT; ++ii) 
         {
            const tSIRF_MSG_AI3_EPH_INFO* eph = (const tSIRF_MSG_AI3_EPH_INFO*)&msg->eph_info[ii];

            SIRFBINARY_EXPORT8(eph->svid,ptr);
            SIRFBINARY_EXPORT8(eph->sv_info_flag,ptr);
            SIRFBINARY_EXPORT16(eph->gps_week,ptr);
            SIRFBINARY_EXPORT16(eph->gps_toe,ptr);
            SIRFBINARY_EXPORT8(eph->iode,ptr);
            SIRFBINARY_EXPORT16(eph->azimuth,ptr);
            SIRFBINARY_EXPORT8(eph->elevation,ptr);
         }
      }
      break;
         
      case SIRF_MSG_AI3_SLC_CP_ACK_NACK:
      {
         /* Construct a pointer to the structure. */
         const tSIRF_MSG_AI3_SLC_CP_ACK_NACK * msg = (const tSIRF_MSG_AI3_SLC_CP_ACK_NACK *)message_structure;
         
         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( 2 * sizeof(tSIRF_UINT8) + 
                                  1 * sizeof(tSIRF_UINT16) )) )
         {          
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

         /* Export members. */
         SIRFBINARY_EXPORT8(msg->ack_nack_error,ptr);
         SIRFBINARY_EXPORT8(msg->slc_msg_id,ptr);
         SIRFBINARY_EXPORT16(msg->spare,ptr);
      }
      break;
         
      case SIRF_MSG_AI3_POLL_ALMANAC_RSP:
      {
         /* Construct a pointer to the structure. */
         const tSIRF_MSG_AI3_POLL_ALMANAC_RSP * msg = (const tSIRF_MSG_AI3_POLL_ALMANAC_RSP *)message_structure;
         
         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( (1 + 3 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) + 
                                  (1 + 6 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) + 
                                  (4 * 3 * SIRF_MAX_SVID_CNT) /* tSIRF_UINT24 */ + 
                                  1 * sizeof(tSIRF_UINT32) )) )
         {     
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

         /* Export members. */
         SIRFBINARY_EXPORT8(msg->data_flag,ptr);
         SIRFBINARY_EXPORT16(msg->extended_gps_week,ptr);
         SIRFBINARY_EXPORT32(msg->gpstow,ptr);
         for (ii = 0u; ii < SIRF_MAX_SVID_CNT; ++ii) 
         {
            const tSIRF_MSG_AI3_ALMANAC* alm = (const tSIRF_MSG_AI3_ALMANAC*) &msg->alm_per_sv_list[ii];
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
      break;
/* these are the reverse encoding used strictly for unit testing */
/* #ifdef CPPUNIT_HOST */
      case SIRF_MSG_AI3_REQ:
      {
         const tSIRF_MSG_AI3_REQ * msg = (const tSIRF_MSG_AI3_REQ*) message_structure;
         
         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( (26 * sizeof(tSIRF_UINT8) + 
                                  (6 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) +
                                  (3 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) +
                                  (7 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) +
                                   1 * sizeof(tSIRF_UINT16) + 
                                  (11 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) +
                                  (5 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) +
                                  (4 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) +
                                   4 * 3 * SIRF_MAX_SVID_CNT /* tSIRF_UINT24 */ +
                                   4 * sizeof(tSIRF_UINT32) +
                                  (8 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT32) ))))
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

         SIRFBINARY_EXPORT8(msg->icd_rev_num,ptr);
         SIRFBINARY_EXPORT8(msg->pos_req_flag,ptr);

         SIRFBINARY_EXPORT8(msg->pos_req_infol.num_fixes,ptr);
         SIRFBINARY_EXPORT8(msg->pos_req_infol.time_btw_fixes,ptr);
         SIRFBINARY_EXPORT8(msg->pos_req_infol.hori_error_max,ptr);
         SIRFBINARY_EXPORT8(msg->pos_req_infol.vert_error_max,ptr);
         SIRFBINARY_EXPORT8(msg->pos_req_infol.resp_time_max,ptr);
         SIRFBINARY_EXPORT8(msg->pos_req_infol.time_acc_priority,ptr);

         SIRFBINARY_EXPORT8(msg->location_method,ptr);
         SIRFBINARY_EXPORT8(msg->alm_req_flag,ptr);
         SIRFBINARY_EXPORT8(msg->iono_flag,ptr);

         SIRFBINARY_EXPORT8(msg->iono_data.alpha_0,ptr);
         SIRFBINARY_EXPORT8(msg->iono_data.alpha_1,ptr);
         SIRFBINARY_EXPORT8(msg->iono_data.alpha_2,ptr);
         SIRFBINARY_EXPORT8(msg->iono_data.alpha_3,ptr);
         SIRFBINARY_EXPORT8(msg->iono_data.beta_0,ptr);
         SIRFBINARY_EXPORT8(msg->iono_data.beta_1,ptr);
         SIRFBINARY_EXPORT8(msg->iono_data.beta_2,ptr);
         SIRFBINARY_EXPORT8(msg->iono_data.beta_3,ptr);

         for (ii = 0; ii < SIRF_MAX_SVID_CNT; ++ii) 
         {
            const tSIRF_MSG_AI3_EPHEMERIS_INFO_PER_SV* eph = &msg->eph_prms.sv_eph_prm[ii];

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
         }
         SIRFBINARY_EXPORT8(msg->alm_data_flag,ptr);
         SIRFBINARY_EXPORT16(msg->alm_data.alm_week_num,ptr);

         for (ii = 0; ii < SIRF_MAX_SVID_CNT; ++ii) 
         {
            const tSIRF_MSG_AI3_ALMANAC_PARAMETERS* alm = &msg->alm_data.alm_params[ii];
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
         SIRFBINARY_EXPORT8(msg->acq_assist_flag,ptr);
         SIRFBINARY_EXPORT32(msg->acq_info.reference_time,ptr);
         /* 32 instances of the following table:   */
         for (ii = 0; ii < SIRF_MAX_SVID_CNT; ++ii) 
         {
            const tSIRF_MSG_AI3_ACQUISITION_ASSISTANCE_DATA* acq = &msg->acq_info.acq_data[ii];

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

         SIRFBINARY_EXPORT8(msg->unhealthy_sat_flag,ptr);
         SIRFBINARY_EXPORT32(msg->unhealthy_sat_info,ptr);
         SIRFBINARY_EXPORT8(msg->pos_req_aux_flag,ptr);
         SIRFBINARY_EXPORT8(msg->pos_req_aux_num_bytes,ptr);
         SIRFBINARY_EXPORT8(msg->nw_enhance_type,ptr);
         SIRFBINARY_EXPORT32(msg->coarse_pos_ref_lat,ptr);
         SIRFBINARY_EXPORT32(msg->coarse_pos_ref_lon,ptr);
         SIRFBINARY_EXPORT8(msg->pos_req_id,ptr);
      }
      break;      
      case SIRF_MSG_AI3_NAV_SF_123_RSP:
      {
         const tSIRF_MSG_AI3_NAV_SF_123_RSP* msg = (const tSIRF_MSG_AI3_NAV_SF_123_RSP*)message_structure;
         
         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( (2 * SIRF_MSG_AI3_ANS123R_NUM_SF123) * sizeof(tSIRF_UINT8) +
                                   SIRF_MSG_AI3_ANS123R_NUM_SF123 * SIRF_MSG_AI3_ANS123R_NUM_BYTES_PER_SF123)) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }
         
         for (ii = 0; ii < SIRF_MSG_AI3_ANS123R_NUM_SF123; ++ii) 
         {
            const tSIRF_MSG_AI3_NAV_SF_123* sf123 = &msg->sf123[ii];
            SIRFBINARY_EXPORT8(sf123->sf_123_flag,ptr);
            SIRFBINARY_EXPORT8(sf123->sv_id,ptr);
            memcpy(ptr,sf123->sf123,SIRF_MSG_AI3_ANS123R_NUM_BYTES_PER_SF123);
            ptr += SIRF_MSG_AI3_ANS123R_NUM_BYTES_PER_SF123;
         }
      }
      break;
      case SIRF_MSG_AI3_NAV_SF_45_RSP:
      {
         /* Construct a pointer to the structure. */
         const tSIRF_MSG_AI3_NAV_SF_45_RSP * msg = (const tSIRF_MSG_AI3_NAV_SF_45_RSP *)message_structure;
         
         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( (SIRF_MSG_AI3_ANS45R_NUM_SF45) * sizeof(tSIRF_UINT8) +
                                   1 * sizeof(tSIRF_UINT32) +
                                   SIRF_MSG_AI3_ANS45R_NUM_SF45 * SIRF_MSG_AI3_ANS45R_NUM_BYTES_PER_SF45)) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

         /* Import members. */
         SIRFBINARY_EXPORT32(msg->sv_list,ptr);
         for (ii = 0u; ii < SIRF_MSG_AI3_ANS45R_NUM_SF45; ++ii) 
         {
            SIRFBINARY_EXPORT8(msg->sf45[ii].frame_num,ptr);
               
            /* Import the 75 bytes (600 bits) of subframes 4/5. */
            memcpy(ptr,msg->sf45[ii].SF45, SIRF_MSG_AI3_ANS45R_NUM_BYTES_PER_SF45);
            ptr += SIRF_MSG_AI3_ANS45R_NUM_BYTES_PER_SF45;
         }
      }
      break;
      case SIRF_MSG_AI3_SLC_EPH_STATUS_REQ:
      {
         /* there is nothing to export for this message */
         if (sizeof(tSIRF_MSG_AI3_SLC_EPH_STATUS_REQ) != message_length)
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }
      }
      break;
      case SIRF_MSG_AI3_POLL_ALMANAC_REQ:
      {
         /* there is nothing to export for this message */
         if (sizeof(tSIRF_MSG_AI3_POLL_ALMANAC_REQ) != message_length)
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }
      }
      break;
      case SIRF_MSG_AI3_CP_SEND_AUXILIARY_NAV:
      {
         const tSIRF_MSG_AI3_CP_SEND_AUXILIARY_NAV * msg = (const tSIRF_MSG_AI3_CP_SEND_AUXILIARY_NAV *)message_structure;

         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( (9 * sizeof(tSIRF_UINT8) + 
                                  (4 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) +
                                  (2 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) +
                                  (SIRF_MAX_SVID_CNT * SIRF_MSG_AI3_ANP_NUM_SF1_RESERVED) +
                                  (2 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) +
                                  (1 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) +
                                   2 * sizeof(tSIRF_UINT32) ))) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

         SIRFBINARY_EXPORT8(msg->aux_nav_model_valid_flag,ptr);

         /*The structure shall include 32 occurrences of     */
         /*auxiliary navigation model parameters             */
         for(ii = 0; ii < SIRF_MAX_SVID_CNT; ii++)
         {
            const tSIRF_MSG_AI3_AUXILIARY_NAV_PARAMS *aux_nav = &msg->aux_nav_params[ii];
            SIRFBINARY_EXPORT8(aux_nav->svid,ptr);
            SIRFBINARY_EXPORT16(aux_nav->toe,ptr);
            SIRFBINARY_EXPORT16(aux_nav->iodc,ptr);
            SIRFBINARY_EXPORT8(aux_nav->sf1_l2_info,ptr);
            SIRFBINARY_EXPORT8(aux_nav->sf1_sv_health,ptr);

            /* Import the 11 reserved bytes. */
            memcpy(ptr, 
                   &msg->aux_nav_params[ii].sf1_reserved, 
                   SIRF_MSG_AI3_ANP_NUM_SF1_RESERVED);
            ptr += SIRF_MSG_AI3_ANP_NUM_SF1_RESERVED;
            SIRFBINARY_EXPORT8(aux_nav->sf1_aodo_fit_interval,ptr);
         }
         /*Begin UTC Model                                   */
         SIRFBINARY_EXPORT8(msg->utc_model_valid_flag,ptr);
         {
            const tSIRF_MSG_AI3_AUXILIARY_NAV_UTC_MODEL *utc = &msg->utc_model;
            SIRFBINARY_EXPORT32(utc->a0,ptr);
            SIRFBINARY_EXPORT32(utc->a1,ptr);
            SIRFBINARY_EXPORT8(utc->delta_tls,ptr);
            SIRFBINARY_EXPORT8(utc->tot,ptr);
            SIRFBINARY_EXPORT8(utc->wnt,ptr);
            SIRFBINARY_EXPORT8(utc->wnlsf,ptr);
            SIRFBINARY_EXPORT8(utc->dn,ptr);
            SIRFBINARY_EXPORT8(utc->delta_t_lsf,ptr);
         }

         SIRFBINARY_EXPORT8(msg->gps_tow_assist_valid_flag,ptr);
         for(ii = 0; ii < SIRF_MAX_SVID_CNT; ii++)
         {
            const tSIRF_MSG_AI3_AUXILIARY_NAV_TOW_ASSIST *aux = &msg->aux_gps_tow_assist[ii];
            SIRFBINARY_EXPORT8(aux->svid,ptr);
            SIRFBINARY_EXPORT16(aux->tlm_msg,ptr);
            SIRFBINARY_EXPORT8(aux->tow_assist_info,ptr);
         }
      }
      break;

      case SIRF_MSG_AI3_BE_REQ:
      {
         /* Construct a pointer to the structure. */
         const tSIRF_MSG_AI3_BE_REQ * msg = (const tSIRF_MSG_AI3_BE_REQ *)message_structure;
         
         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( (2 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) +
                                  (1 + 2 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16))) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

         SIRFBINARY_EXPORT16(msg->eph_resp_trigger,ptr);

         for (ii = 0u; ii < SIRF_MAX_SVID_CNT; ++ii) 
         {
            const tSIRF_MSG_AI3_BE_REQ_PARAMS* eph_info = &msg->eph_params[ii];
 
            SIRFBINARY_EXPORT8( eph_info->eph_info_flag,ptr);
            SIRFBINARY_EXPORT8( eph_info->sv_prn_num,ptr);
            SIRFBINARY_EXPORT16(eph_info->gps_week,ptr);
            SIRFBINARY_EXPORT16(eph_info->toe,ptr);
         }/* ii=0 -> 32 */ 
      }
      break;

/* #endif */ /* CPPUNIT_TEST */

      case SIRF_MSG_AI3_BE_RSP: /* Ai3 Broadcast Ephemeris Response message */
      {
         /* Construct a pointer to the structure. */
         const tSIRF_MSG_AI3_BE_RSP * msg = (const tSIRF_MSG_AI3_BE_RSP *)message_structure;

         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( (11 * sizeof(tSIRF_UINT8) + 
                                  (7 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) +
                                   1 * sizeof(tSIRF_UINT16) + 
                                  (12 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) +
                                   1 * sizeof(tSIRF_UINT32) + 
                                  (8 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT32) ))) )
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

         for (ii = 0; ii < SIRF_MAX_SVID_CNT; ++ii) 
         {
            const tSIRF_MSG_AI3_BE* eph = &msg->be[ii];

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
            SIRFBINARY_EXPORT8 (eph->eph.t_gd,         ptr);
            SIRFBINARY_EXPORT8 (eph->eph.af2,          ptr);
            SIRFBINARY_EXPORT16(eph->eph.af1,         ptr);
            SIRFBINARY_EXPORT32(eph->eph.af0,         ptr);
         } /* ii=0 -> 32 */
      }
      break;

      case SIRF_MSG_AI3_EE_INTEGRITY_WARNING:
      {
         /* Construct a pointer to the structure. */
         const tSIRF_MSG_AI3_EE_INTEGRITY_WARNING * msg = (const tSIRF_MSG_AI3_EE_INTEGRITY_WARNING *)message_structure;

         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( 1 * sizeof(tSIRF_UINT8) +
                                  3 * sizeof(tSIRF_UINT32))) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

         SIRFBINARY_EXPORT8( msg->icd_rev_num,             ptr);
         SIRFBINARY_EXPORT32(msg->sat_pos_validity_mask,   ptr);
         SIRFBINARY_EXPORT32(msg->sat_clock_validity_mask, ptr);
         SIRFBINARY_EXPORT32(msg->sat_health_indicator,    ptr);
      }
      break;

      case SIRF_MSG_AI3_EE_CLKBIAS_ADJUST:
      {
         /* Construct a pointer to the structure. */
         const tSIRF_MSG_AI3_EE_CLKBIAS_ADJUST * msg = (const tSIRF_MSG_AI3_EE_CLKBIAS_ADJUST *)message_structure;

         if ( (sizeof(*msg) != message_length) || 
              (*packet_length < ( (1 + SIRF_MSG_AI3_CLK_BIAS_ADJUST_MAX_SVS) * sizeof(tSIRF_UINT8) +
                                  SIRF_MSG_AI3_CLK_BIAS_ADJUST_MAX_SVS * sizeof(tSIRF_UINT16) +
                                  SIRF_MSG_AI3_CLK_BIAS_ADJUST_MAX_SVS * sizeof(tSIRF_UINT32))) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            break;
         }

         SIRFBINARY_EXPORT8(msg->icd_rev_num, ptr);

         for (ii = 0u; ii < SIRF_MSG_AI3_CLK_BIAS_ADJUST_MAX_SVS; ++ii) 
         {
            const tSIRF_MSG_AI3_EE_CLKBIAS_ADJUST_DATA* data_rec = &msg->data[ii];
 
            SIRFBINARY_EXPORT8( data_rec->sv_id,            ptr);
            SIRFBINARY_EXPORT16(data_rec->ee_toe,           ptr);
            SIRFBINARY_EXPORT32(data_rec->clock_bias_adjust,ptr);
         }/* ii=0 -> 12 */ 
      }
      break;

      default:
         tRet = SIRF_CODEC_ERROR_INVALID_MSG_ID;
         break;
      }
      if (SIRF_SUCCESS == tRet)
      {
         *packet_length = ptr-packet;
      } 
      else 
      {
         *packet_length = 0;
      }
   }
   else 
   {
      tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
   }
   return tRet;
}

/** 
 * Decodes a message from a serial byte stream into a message strucutre.
 * the serial byte stream should be of the format:
 * packet[0] = LOGICAL_CHANNEL
 * packet[1] = Message ID
 * packet[2..*packet_length] encoded message
 * where the encoded message is the data that will be placed into the message
 * structure.
 * 
 * @param payload            [in] pointer to place the message to decode
 * @param payload_length     [in] length of the message at payload
 * @param message_id         [out] 32 bit message id including LC, SID, and ID 
 * @param message_structure  [out] pointer to structure decoded
 * @param message_length     [out] Length of the message structure in bytes 
 * 
 * @return SIRF_SUCCESS if the message was fully encoded, SIRF_FAILURE otherwise
 */
tSIRF_RESULT SIRF_CODEC_AI3_Decode( tSIRF_UINT8 *payload, 
                                    tSIRF_UINT32 payload_length,
                                    tSIRF_UINT32 *message_id, 
                                    tSIRF_VOID *message_structure, 
                                    tSIRF_UINT32 *message_length )
{
   tSIRF_RESULT tRet = SIRF_SUCCESS;
   int lc;
   int msg_id;
   
   /* Verify valid arguments. */
   if ( ((tSIRF_UINT8 *) NULL  != payload) && 
        ((tSIRF_UINT32 *) NULL != message_id) &&  
        ((tSIRF_VOID *) NULL   != message_structure) &&
        ((tSIRF_UINT32*)NULL   != message_length))
   {
      tSIRF_UINT32 ii;

      /* Get a pointer to the packet data. */
      const tSIRF_UINT8 *ptr = payload;
      
      /* At a minimum, make sure we have room for the header bytes */
      if ( payload_length < AI3_DECODE_HEADER_SIZE )
      {
         return SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
      }
      
      /* Read the logical channel. */
      lc     = SIRFBINARY_IMPORT_UINT8(ptr);
      /* Read the message ID. */
      msg_id     = SIRFBINARY_IMPORT_UINT8(ptr);

      *message_id = SIRF_MAKE_MSG_ID(lc,msg_id,0);
      /* Switch on the type of message being decoded. */
      switch (*message_id)
      {
      case SIRF_MSG_AI3_REQ:
      {
         tSIRF_MSG_AI3_REQ * msg = (tSIRF_MSG_AI3_REQ*) message_structure;
         
         if ( payload_length < ( (26 * sizeof(tSIRF_UINT8) + 
                                 (6 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) +
                                 (3 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) +
                                 (7 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) +
                                  1 * sizeof(tSIRF_UINT16) + 
                                 (11 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) +
                                 (5 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) +
                                 (4 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) +
                                  4 * 3 * SIRF_MAX_SVID_CNT /* tSIRF_UINT24 */ +
                                  4 * sizeof(tSIRF_UINT32) +
                                 (8 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT32) + 
                                 AI3_DECODE_HEADER_SIZE) ))
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
   
            msg->icd_rev_num                           = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->pos_req_flag                          = SIRFBINARY_IMPORT_UINT8(ptr);
   
            msg->pos_req_infol.num_fixes               = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->pos_req_infol.time_btw_fixes          = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->pos_req_infol.hori_error_max          = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->pos_req_infol.vert_error_max          = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->pos_req_infol.resp_time_max           = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->pos_req_infol.time_acc_priority       = SIRFBINARY_IMPORT_UINT8(ptr);
   
            msg->location_method                       = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->alm_req_flag                          = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->iono_flag                             = SIRFBINARY_IMPORT_UINT8(ptr);
   
            msg->iono_data.alpha_0                     = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->iono_data.alpha_1                     = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->iono_data.alpha_2                     = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->iono_data.alpha_3                     = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->iono_data.beta_0                      = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->iono_data.beta_1                      = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->iono_data.beta_2                      = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->iono_data.beta_3                      = SIRFBINARY_IMPORT_SINT8(ptr);
   
            for (ii = 0; ii < SIRF_MAX_SVID_CNT; ++ii) 
            {
               tSIRF_MSG_AI3_EPHEMERIS_INFO_PER_SV* eph = &msg->eph_prms.sv_eph_prm[ii];
   
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
            msg->alm_data_flag                         = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->alm_data.alm_week_num                 = SIRFBINARY_IMPORT_UINT16(ptr);
   
            for (ii = 0; ii < SIRF_MAX_SVID_CNT; ++ii) 
            {
               tSIRF_MSG_AI3_ALMANAC_PARAMETERS* alm = &msg->alm_data.alm_params[ii];
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
            msg->acq_assist_flag                       = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->acq_info.reference_time               = SIRFBINARY_IMPORT_UINT32(ptr);
            /* 32 instances of the following table:   */
            for (ii = 0; ii < SIRF_MAX_SVID_CNT; ++ii) 
            {
               tSIRF_MSG_AI3_ACQUISITION_ASSISTANCE_DATA* acq = &msg->acq_info.acq_data[ii];
   
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
   
            msg->unhealthy_sat_flag                    = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->unhealthy_sat_info                    = SIRFBINARY_IMPORT_UINT32(ptr);
            msg->pos_req_aux_flag                      = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->pos_req_aux_num_bytes                 = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->nw_enhance_type                       = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->coarse_pos_ref_lat                    = SIRFBINARY_IMPORT_SINT32(ptr);
            msg->coarse_pos_ref_lon                    = SIRFBINARY_IMPORT_SINT32(ptr);
            msg->pos_req_id                            = SIRFBINARY_IMPORT_UINT8(ptr);
         }
      }
      break;      
         
      case SIRF_MSG_AI3_ACK_NACK:
      {
         /* Construct a pointer to the structure. */
         tSIRF_MSG_AI3_ACK_NACK * msg = (tSIRF_MSG_AI3_ACK_NACK *)message_structure;
         
         if ( payload_length < (1 * sizeof(tSIRF_UINT8) + AI3_DECODE_HEADER_SIZE) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
            
            /* Import members. */
            msg->acknak                                = SIRFBINARY_IMPORT_UINT8(ptr);
         }
      }
      break;
      
      case SIRF_MSG_AI3_NAV_SF_123_RSP:
      {
         /* Construct a pointer to the structure. */
         tSIRF_MSG_AI3_NAV_SF_123_RSP * msg = (tSIRF_MSG_AI3_NAV_SF_123_RSP *)message_structure;
         
         if ( payload_length < (2 * SIRF_MSG_AI3_ANS123R_NUM_SF123 * sizeof(tSIRF_UINT8) + 
                                SIRF_MSG_AI3_ANS123R_NUM_SF123 * SIRF_MSG_AI3_ANS123R_NUM_BYTES_PER_SF123 +
                                AI3_DECODE_HEADER_SIZE) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
   
            /* Import members. */
            for (ii = 0; ii < SIRF_MSG_AI3_ANS123R_NUM_SF123; ++ii) 
            {
               tSIRF_MSG_AI3_NAV_SF_123* sf123 = (tSIRF_MSG_AI3_NAV_SF_123*)&msg->sf123[ii];
               sf123->sf_123_flag                      = SIRFBINARY_IMPORT_UINT8(ptr);
               sf123->sv_id                            = SIRFBINARY_IMPORT_UINT8(ptr);
               memcpy(sf123->sf123,ptr,SIRF_MSG_AI3_ANS123R_NUM_BYTES_PER_SF123);
               ptr += SIRF_MSG_AI3_ANS123R_NUM_BYTES_PER_SF123;
            }
         }
      }
      break;
      
      case SIRF_MSG_AI3_NAV_SF_45_RSP:
      {
         /* Construct a pointer to the structure. */
         tSIRF_MSG_AI3_NAV_SF_45_RSP * msg = (tSIRF_MSG_AI3_NAV_SF_45_RSP *)message_structure;
         
         if ( payload_length < (SIRF_MSG_AI3_ANS45R_NUM_SF45 * sizeof(tSIRF_UINT8) + 
                                SIRF_MSG_AI3_ANS45R_NUM_SF45 * SIRF_MSG_AI3_ANS45R_NUM_BYTES_PER_SF45 +
                                1 * sizeof(tSIRF_UINT32) +
                                AI3_DECODE_HEADER_SIZE) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
   
            /* Import members. */
            msg->sv_list                               = SIRFBINARY_IMPORT_UINT32(ptr);
            for (ii = 0u; ii < SIRF_MSG_AI3_ANS45R_NUM_SF45; ++ii) 
            {
               msg->sf45[ii].frame_num                 = SIRFBINARY_IMPORT_UINT8(ptr);
                  
               /* Import the 75 bytes (600 bits) of subframes 4/5. */
               memcpy(msg->sf45[ii].SF45, ptr, SIRF_MSG_AI3_ANS45R_NUM_BYTES_PER_SF45);
               ptr += SIRF_MSG_AI3_ANS45R_NUM_BYTES_PER_SF45;
            }
         }
      }
      break;
         
      case SIRF_MSG_AI3_SLC_CP_ACK_NACK:
      {
         /* Construct a pointer to the structure. */
         tSIRF_MSG_AI3_SLC_CP_ACK_NACK * msg = (tSIRF_MSG_AI3_SLC_CP_ACK_NACK *)message_structure;
         
         if ( payload_length < (2 * sizeof(tSIRF_UINT8) + 
                                1 * sizeof(tSIRF_UINT16) +
                                AI3_DECODE_HEADER_SIZE) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
   
            /* Import members. */
            msg->ack_nack_error                        = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->slc_msg_id                            = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->spare                                 = SIRFBINARY_IMPORT_UINT16(ptr);
         }
      }
      break;
         
      case SIRF_MSG_AI3_SLC_EPH_STATUS_REQ:
         /* No members. */
         memset(message_structure,0,sizeof(tSIRF_MSG_AI3_SLC_EPH_STATUS_REQ));
         *message_length = sizeof(tSIRF_MSG_AI3_SLC_EPH_STATUS_REQ);
         break;
         
      case SIRF_MSG_AI3_POLL_ALMANAC_REQ:
         /* No members. */
         memset(message_structure,0,sizeof(tSIRF_MSG_AI3_POLL_ALMANAC_REQ));
         *message_length = sizeof(tSIRF_MSG_AI3_POLL_ALMANAC_REQ);
         break;
         
      case SIRF_MSG_AI3_CP_SEND_AUXILIARY_NAV:
      {
         tSIRF_MSG_AI3_CP_SEND_AUXILIARY_NAV * msg = (tSIRF_MSG_AI3_CP_SEND_AUXILIARY_NAV *)message_structure;
         
         if ( payload_length < (9 * sizeof(tSIRF_UINT8) + 
                               (4 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) +
                               (2 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) +
                               (SIRF_MAX_SVID_CNT * SIRF_MSG_AI3_ANP_NUM_SF1_RESERVED) +
                               (2 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) +
                               (1 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) +
                                2 * sizeof(tSIRF_UINT32) +
                                AI3_DECODE_HEADER_SIZE) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
   
            msg->aux_nav_model_valid_flag              = SIRFBINARY_IMPORT_UINT8(ptr);
   
            /*The structure shall include 32 occurrences of     */
            /*auxiliary navigation model parameters             */
            for(ii = 0; ii < SIRF_MAX_SVID_CNT; ii++)
            {
               tSIRF_MSG_AI3_AUXILIARY_NAV_PARAMS *aux_nav = &msg->aux_nav_params[ii];
               aux_nav->svid                           = SIRFBINARY_IMPORT_UINT8(ptr);
               aux_nav->toe                            = SIRFBINARY_IMPORT_UINT16(ptr);
               aux_nav->iodc                           = SIRFBINARY_IMPORT_UINT16(ptr);
               aux_nav->sf1_l2_info                    = SIRFBINARY_IMPORT_UINT8(ptr);
               aux_nav->sf1_sv_health                  = SIRFBINARY_IMPORT_UINT8(ptr);
   
               /* Import the 11 reserved bytes. */
               memcpy(&msg->aux_nav_params[ii].sf1_reserved, 
                      ptr, 
                      SIRF_MSG_AI3_ANP_NUM_SF1_RESERVED);
               ptr += SIRF_MSG_AI3_ANP_NUM_SF1_RESERVED;
               aux_nav->sf1_aodo_fit_interval          = SIRFBINARY_IMPORT_UINT8(ptr);
            }
            /*Begin UTC Model                                   */
            msg->utc_model_valid_flag                  = SIRFBINARY_IMPORT_UINT8(ptr);
            {
               tSIRF_MSG_AI3_AUXILIARY_NAV_UTC_MODEL *utc = &msg->utc_model;
               utc->a0                                 = SIRFBINARY_IMPORT_UINT32(ptr);
               utc->a1                                 = SIRFBINARY_IMPORT_UINT32(ptr);
               utc->delta_tls                          = SIRFBINARY_IMPORT_UINT8(ptr);
               utc->tot                                = SIRFBINARY_IMPORT_UINT8(ptr);
               utc->wnt                                = SIRFBINARY_IMPORT_UINT8(ptr);
               utc->wnlsf                              = SIRFBINARY_IMPORT_UINT8(ptr);
               utc->dn                                 = SIRFBINARY_IMPORT_UINT8(ptr);
               utc->delta_t_lsf                        = SIRFBINARY_IMPORT_UINT8(ptr);
            }
   
            msg->gps_tow_assist_valid_flag             = SIRFBINARY_IMPORT_UINT8(ptr);
            for(ii = 0; ii < SIRF_MAX_SVID_CNT; ii++)
            {
               tSIRF_MSG_AI3_AUXILIARY_NAV_TOW_ASSIST *aux = &msg->aux_gps_tow_assist[ii];
               aux->svid                               = SIRFBINARY_IMPORT_UINT8(ptr);
               aux->tlm_msg                            = SIRFBINARY_IMPORT_UINT16(ptr);
               aux->tow_assist_info                    = SIRFBINARY_IMPORT_UINT8(ptr);
            }
         }
      }
      break;

      case SIRF_MSG_AI3_BE_REQ:
      {    
         /* Construct a pointer to the structure. */
         tSIRF_MSG_AI3_BE_REQ * msg = ( tSIRF_MSG_AI3_BE_REQ *)message_structure;
         
         if ( payload_length < ( (2 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) +
                                 (1 + 2 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) + 
                                 AI3_DECODE_HEADER_SIZE) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
   
            /* Export members. */
            msg->eph_resp_trigger                      = SIRFBINARY_IMPORT_UINT16(ptr);
            for (ii = 0u; ii < SIRF_MAX_SVID_CNT; ++ii) 
            {
               tSIRF_MSG_AI3_BE_REQ_PARAMS* eph_info = &msg->eph_params[ii];
               eph_info->eph_info_flag                 = SIRFBINARY_IMPORT_UINT8(ptr);
               eph_info->sv_prn_num                    = SIRFBINARY_IMPORT_UINT8(ptr);
               eph_info->gps_week                      = SIRFBINARY_IMPORT_UINT16(ptr);
               eph_info->toe                           = SIRFBINARY_IMPORT_UINT16(ptr);
            }
         }
      }
      break;


/* #ifdef CPPUNIT_HOST */ /* following only for unit testing */
      case SIRF_MSG_AI3_RSP:
      {
         /* Construct a pointer to the structure. */
         tSIRF_MSG_AI3_RSP * msg = (tSIRF_MSG_AI3_RSP *)message_structure;
         
         if ( payload_length < ( (32 + 3 * SIRF_MSG_AI3_RSP_NUM_SV_INFO) * sizeof(tSIRF_UINT8) + 
                                 (4 * SIRF_MSG_AI3_MEASUREMENT_DATA_MAX_SVS) * sizeof(tSIRF_UINT8) +
                                 (2 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) +
                                 (8 + 3 * SIRF_MSG_AI3_MEASUREMENT_DATA_MAX_SVS) * sizeof(tSIRF_UINT16) + 
                                 (1 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) +
                                 (7 * sizeof(tSIRF_UINT32) +
                                 AI3_DECODE_HEADER_SIZE) ))
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
   
            msg->pos_results_flag                      = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->pos_data.pos_err_status               = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->pos_data.qos_pass_flag                = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->pos_data.pos_type                     = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->pos_data.dgps_cor                     = SIRFBINARY_IMPORT_UINT8(ptr); 
            msg->pos_data.gps_week                     = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->pos_data.time_of_week                 = SIRFBINARY_IMPORT_UINT32(ptr);
            msg->pos_data.lat                          = SIRFBINARY_IMPORT_SINT32(ptr);
            msg->pos_data.lon                          = SIRFBINARY_IMPORT_SINT32(ptr);
   
            msg->pos_data.other_sections               = SIRFBINARY_IMPORT_UINT8(ptr);
      
            /* Horizontal Error Section */
            msg->pos_data.hor_err_angle                = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->pos_data.hor_err_major                = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->pos_data.hor_err_minor                = SIRFBINARY_IMPORT_UINT8(ptr);
      
            /* Vertical Position Section */
            msg->pos_data.vert_pos_height              = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->pos_data.vert_pos_std                 = SIRFBINARY_IMPORT_UINT8(ptr);
      
            /* Velocity Section */
            msg->pos_data.velocity_horizontal          = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->pos_data.velocity_heading             = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->pos_data.velocity_vertical            = SIRFBINARY_IMPORT_SINT8(ptr);
            msg->pos_data.velocity_hor_err_ang         = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->pos_data.velocity_hor_err_maj_std     = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->pos_data.velocity_hor_err_min_std     = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->pos_data.velocity_ver_err             = SIRFBINARY_IMPORT_UINT8(ptr);
      
            /* Clock Correction Section */
            msg->pos_data.clk_cor_time_ref             = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->pos_data.clk_cor_clk_bias             = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->pos_data.clk_cor_clk_drift            = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->pos_data.clk_cor_clk_std_err          = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->pos_data.clk_cor_utc_off              = SIRFBINARY_IMPORT_UINT8(ptr);  
      
            /* Position Correction Section */
            msg->pos_data.pos_corr_num_sat             = SIRFBINARY_IMPORT_UINT8(ptr);
            for (ii = 0; ii < SIRF_MSG_AI3_RSP_NUM_SV_INFO; ii++)
            {
               tSIRF_MSG_AI3_NB_SV_DATA *sv_data = &msg->pos_data.pos_corr_sv_info[ii];
               sv_data->sv_prn                         = SIRFBINARY_IMPORT_UINT8(ptr);
               sv_data->c_n0                           = SIRFBINARY_IMPORT_UINT8(ptr);
               sv_data->inv_weights                    = SIRFBINARY_IMPORT_UINT8(ptr);
            }
   
            msg->alm_ref_date_flag                     = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->alm_data_stat                         = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->alm_week_num                          = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->alm_toa                               = SIRFBINARY_IMPORT_UINT8(ptr);
   
            msg->gps_meas_flag                         = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->gps_meas_data.meas_error_status       = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->gps_meas_data.meas_gps_week           = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->gps_meas_data.meas_gps_seconds        = SIRFBINARY_IMPORT_UINT32(ptr);
            msg->gps_meas_data.time_accuracy           = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->gps_meas_data.num_sv                  = SIRFBINARY_IMPORT_UINT8(ptr);
            for (ii = 0; ii < SIRF_MSG_AI3_MEASUREMENT_DATA_MAX_SVS; ii++)
            {
               tSIRF_MSG_AI3_MEAS_PER_SV_DATA *meas = &msg->gps_meas_data.meas_per_sv_data[ii];
               meas->sv_prn                            = SIRFBINARY_IMPORT_UINT8(ptr);
               meas->c_n0                              = SIRFBINARY_IMPORT_UINT8(ptr);
               meas->sv_doppler                        = SIRFBINARY_IMPORT_SINT16(ptr);
               meas->sv_code_phase_wh                  = SIRFBINARY_IMPORT_UINT16(ptr);
               meas->sv_code_phase_fr                  = SIRFBINARY_IMPORT_UINT16(ptr);
               meas->multipath_indicator               = SIRFBINARY_IMPORT_UINT8(ptr);
               meas->pseudorange_rms_error             = SIRFBINARY_IMPORT_UINT8(ptr);
            }
   
            /* ICD 1.9 */
            msg->subalm_flag                           = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->alm_data.data_stat                    = SIRFBINARY_IMPORT_UINT8(ptr);
            for (ii = 0; ii < SIRF_MAX_SVID_CNT; ii++)
            {
               tSIRF_MSG_AI3_SUBALM_INFO *subalm = &msg->alm_data.subalm_info[ii];
               subalm->subalm_sv_prn                   = SIRFBINARY_IMPORT_UINT8(ptr);
               subalm->subalm_week_num                 = SIRFBINARY_IMPORT_UINT16(ptr);
               subalm->subalm_toa                      = SIRFBINARY_IMPORT_UINT8(ptr);
            }
   
            msg->cp_valid_flag                         = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->cp_data.cp_num_bytes                  = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->cp_data.delta_lat                     = SIRFBINARY_IMPORT_SINT32(ptr);
            msg->cp_data.delta_lon                     = SIRFBINARY_IMPORT_SINT32(ptr);
            msg->cp_data.delta_alt                     = SIRFBINARY_IMPORT_SINT32(ptr);
            msg->cp_data.pr_err_th                     = SIRFBINARY_IMPORT_UINT8(ptr);
   
            /* ICD 2.0 */
            msg->pos_req_id                            = SIRFBINARY_IMPORT_UINT8(ptr);
         }
      }
      break;
      case SIRF_MSG_AI3_SLC_AIDING_REQ:
      {
         /* Construct a pointer to the structure. */
         tSIRF_MSG_AI3_SLC_AIDING_REQ * msg = ( tSIRF_MSG_AI3_SLC_AIDING_REQ *)message_structure;
         
         if ( payload_length < ( 2 * sizeof(tSIRF_UINT8) + 
                                 1 * sizeof(tSIRF_UINT16) + 
                                 1 * sizeof(tSIRF_UINT32) + 
                                 AI3_DECODE_HEADER_SIZE) )
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
      
      case SIRF_MSG_AI3_UNSOL_SLC_EPH_STATUS_RSP:
      case SIRF_MSG_AI3_SOL_SLC_EPH_STATUS_RSP:
      {
         /* Construct a pointer to the structure. */
         tSIRF_MSG_AI3_SLC_EPH_STATUS_RSP * msg = ( tSIRF_MSG_AI3_SLC_EPH_STATUS_RSP *)message_structure;
         
         if ( payload_length < ( (3 + 4 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) + 
                                 (3 + 3 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) + 
                                  1 * sizeof(tSIRF_UINT32) + 
                                  AI3_DECODE_HEADER_SIZE) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            *message_length = sizeof(*msg);
   
            /* Export members. */
            msg->agps_status_mask                      = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->gps_time_mask                         = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->gps_week                              = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->gps_tow                               = SIRFBINARY_IMPORT_UINT32(ptr);
            msg->eph_status_section_size               = SIRFBINARY_IMPORT_UINT16(ptr);
            msg->eph_aiding_type                       = SIRFBINARY_IMPORT_UINT8(ptr);
            msg->gps_time_tolerance_limit              = SIRFBINARY_IMPORT_UINT8(ptr);
   
            for (ii = 0u; ii < SIRF_MAX_SVID_CNT; ++ii) 
            {
               tSIRF_MSG_AI3_EPH_INFO* eph = &msg->eph_info[ii];
   
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
      break;
         
      case SIRF_MSG_AI3_POLL_ALMANAC_RSP:
      {
         /* Construct a pointer to the structure. */
         tSIRF_MSG_AI3_POLL_ALMANAC_RSP * msg = ( tSIRF_MSG_AI3_POLL_ALMANAC_RSP *)message_structure;
         
         if ( payload_length < ( (1 + 3 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) + 
                                 (1 + 6 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) + 
                                 (4 * 3 * SIRF_MAX_SVID_CNT) /* tSIRF_UINT24 */ + 
                                  1 * sizeof(tSIRF_UINT32) + 
                                  AI3_DECODE_HEADER_SIZE) )
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
            for (ii = 0u; ii < SIRF_MAX_SVID_CNT; ++ii) 
            {
               tSIRF_MSG_AI3_ALMANAC* alm = &msg->alm_per_sv_list[ii];
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
      break;


      case SIRF_MSG_AI3_BE_RSP: /* Ai3 Broadcast Ephemeris Response message */
      {
         /* Construct a pointer to the structure. */
         tSIRF_MSG_AI3_BE_RSP * msg = ( tSIRF_MSG_AI3_BE_RSP *)message_structure;
         
         if ( payload_length < ( (11 * sizeof(tSIRF_UINT8) + 
                                 (7 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT8) +
                                  1 * sizeof(tSIRF_UINT16) + 
                                 (12 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT16) +
                                  1 * sizeof(tSIRF_UINT32) + 
                                 (8 * SIRF_MAX_SVID_CNT) * sizeof(tSIRF_UINT32) + 
                                 AI3_DECODE_HEADER_SIZE) ))
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
   
            for (ii = 0; ii < SIRF_MAX_SVID_CNT; ++ii) 
            {
               tSIRF_MSG_AI3_BE* eph = &msg->be[ii];
   
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
            } /* ii=0 -> 32 */
         }
      }

      break;
/* #endif */ /* CPPUNIT_TESTS */

      default:
         /* Test for message passthru. */
         if ( *message_id >= SIRF_MSG_SSB_PASSTHRU_INPUT_BEGIN && /* 0xB4 */
              *message_id <= SIRF_MSG_SSB_PASSTHRU_INPUT_END &&   /* 0xC7 */
              payload_length - 1 <= SIRF_MSG_AI3_MAX_MESSAGE_LEN &&
              payload_length > 0)
         { 
            /* pass-thru: message. */
            *message_length = payload_length - 1;
            memcpy(message_structure, ptr, payload_length - 1);
            ptr += payload_length - 1;
         }
         else
         {
            /* This message id is not imported. */
            *message_id = 0;
            *message_length = 0;
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_ID;
         }
         break;
      }

      /* Check to see if the length does not match */
      if ( (SIRF_SUCCESS == tRet) && (ptr != (payload + payload_length)))
      {
         *message_id = 0;
         *message_length = 0;
         tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
      }
   }
   else
   {
      tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
   }

   return tRet;
}

/**
 * @}
 */
