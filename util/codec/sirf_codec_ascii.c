/**
 * @addtogroup platform_src_sirf_util_codec
 * @{
 */

 /**************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. GPS Software                    *
 *                                                                         *
 *    Copyright (c) 2007-2010 by SiRF Technology, a CSR plc Company.       *
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
 * MODULE:  CODEC
 *
 * FILENAME:  sirf_codec_ascii.c
 *
 * DESCRIPTION:
 *
 ***************************************************************************/

/***************************************************************************
 * Include Fields
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "sirf_types.h"
#include "sirf_msg.h"
#include "sirf_msg_ssb.h"
#include "sirf_codec.h"
#include "sirf_codec_ascii.h"
#include "string_sif.h"

/***************************************************************************
 *   Macro Definitions
 ***************************************************************************/

/***************************************************************************
 * @brief:   Encode the error string for the specified error
 * @param:   pBuf - The buffer to write the data into
 * @param:   size - Maximum size of the buffer
 * @param:   pError - Pointer to the error structure
 * @return:  None
 ***************************************************************************/

static tSIRF_VOID PErrorFormat(tSIRF_CHAR *pBuf, tSIRF_UINT32 size, tSIRF_MSG_SSB_ERROR *pError)
{
   switch( pError->err_id )
   {
      case SIRF_MSG_SSB_ERRID_MI_BUFFERALLOCFAILURE:
         snprintf( pBuf, size, "UART buffer allocation error: %lu", pError->param[0] );
         break;
      case SIRF_MSG_SSB_ERRID_MI_UPDATETIMEFAILURE:
         /* "One second task not complete on entry #%ld (%ld tracker ms errors)";*/
         snprintf( pBuf, size, "OSOR #%ld (%ld ms)", pError->param[0], pError->param[1] );
         break;
      case SIRF_MSG_SSB_ERRID_MI_VCOCLOCKLOST:
         snprintf( pBuf, size, "VCO lock lost in %lu msec", pError->param[0] );
         break;
      case SIRF_MSG_SSB_ERRID_MI_MEMORYTESTFAILED:
         snprintf( pBuf, size, "Memory test failed at 0x%lx", pError->param[0] );
         break;
      case SIRF_MSG_SSB_ERRID_MI_FALSEACQRECEIVERRESET:
         snprintf( pBuf, size, "Assuming false acquisition and reseting receiver at %ld", pError->param[0] );
         break;
      case SIRF_MSG_SSB_ERRID_KFC_KILLCHANNEL:
         snprintf( pBuf, size, "Kalman Filter @%ld: Kill chan %02ld, range res=%ld",
                   pError->param[0], pError->param[1], pError->param[2] );
         break;
      case SIRF_MSG_SSB_ERRID_KFC_BACKUPFAILED_VELOCITY:
         snprintf( pBuf, size, "SRAM backup not done: Velocity sum exceeds maximum." );
         break;
      case SIRF_MSG_SSB_ERRID_KFC_BACKUPFAILED_NUMSV:
         snprintf( pBuf, size, "SRAM backup not done: Too few SVs used in a solution." );
         break;
      case SIRF_MSG_SSB_ERRID_KFS_BADALTITUDE:
         snprintf( pBuf, size, "KFDBG @%ld: Bad altitude = %ld", pError->param[0], pError->param[1] );
         break;
      case SIRF_MSG_SSB_ERRID_KRS_BADSOLUTION:
         snprintf( pBuf, size, "Krause solution poor:\n POS = %ld %ld %ld\n PR = %ld %ld",
                   pError->param[0], pError->param[1], pError->param[2], pError->param[3], pError->param[4] );
         break;
      case SIRF_MSG_SSB_ERRID_RMC_GETTINGPOSITION:
         snprintf( pBuf, size, "RxM error in getting SV %02ld's position", pError->param[0] );
         break;
      case SIRF_MSG_SSB_ERRID_RXM_TIMEEXCEEDED:
         snprintf( pBuf, size, "RxM: Pseudo range in seconds exceeds limit: %ld", pError->param[0] );
         break;
      case SIRF_MSG_SSB_ERRID_RXM_TDOPOVERFLOW:
         snprintf( pBuf, size, "RxM error: tdop=%ld overflow", pError->param[0] );
         break;
      case SIRF_MSG_SSB_ERRID_RXM_VALIDDURATIONEXCEEDED:
         snprintf( pBuf, size, "RxM: Ephemeris for SV %02ld exceeds valid time duration by %ld sec",
                   pError->param[0], pError->param[1] );
         break;
      case SIRF_MSG_SSB_ERRID_STRTP_SRAMCKSUM:
         snprintf( pBuf, size, "SRAM control flags contain bad checksum" );
         break;
      case SIRF_MSG_SSB_ERRID_STRTP_RTCTIMEINVALID:
         snprintf( pBuf, size, "RTC time or SRAM backup invalid, TOW:%ld, WNUM:%ld",
                   pError->param[0], pError->param[1] );
         break;
      case SIRF_MSG_SSB_ERRID_STRTP_BADPOSTION:
         snprintf( pBuf, size, "SRAM: Bad position during startup, x:%ld y:%ld z:%ld",
                   pError->param[0], pError->param[1], pError->param[2] );
         break;
      case SIRF_MSG_SSB_ERRID_CS_SVPARITY:
         snprintf( pBuf, size, "Bad parity from SV %02ld, subframe %ld", pError->param[0], pError->param[1] );
         break;
      default:
         snprintf( pBuf, size, "Error in unknown format: err_id=%lx params: %lx %lx %lx %lx",
                   pError->param[0], pError->param[1], pError->param[2], pError->param[3], pError->param[4] );
         break;
   }
}

/***************************************************************************
 * @brief:   Convert the specified SSB message structure into a text stream
 * @param[in]:     message_id - id of message to translate
 * @param[in]:     message_structure - data structure for that message
 * @param[in]:     message_length - pseudo-length of the data structure
 * @param[out]:    lpszText - output buffer to hold the translation
 * @param[in out]: lpszText_length - Input- size of lpszText buffer
                                     Output- resultant length of the output string
 * @return:   SIRF_SUCCESS, SIRF_CODEC_ERROR_INVALID_PARAMETER, or
 *            SIRF_CODEC_ERROR_INVALID_MSG_LENGTH
 ***************************************************************************/
tSIRF_RESULT SIRF_CODEC_ASCII_Encode( tSIRF_UINT32 message_id,
                                      tSIRF_VOID *message_structure,
                                      tSIRF_UINT32 message_length,
                                      tSIRF_UINT8  *lpszText,
                                      tSIRF_UINT32 *lpszText_length,
                                      tSIRF_UINT32 *options )
{
   tSIRF_RESULT tRet = SIRF_SUCCESS;

   (void)options;

   if ( ((tSIRF_VOID *) NULL != message_structure) &&
        ((tSIRF_UINT8 *) NULL != lpszText) &&
        ((tSIRF_UINT32 *) NULL != lpszText_length) )
   {
      tSIRF_UINT32 i, j;
      tSIRF_CHAR szFoo[250];
      tSIRF_CHAR szBuf[2 * SIRF_MSG_SSB_MAX_MESSAGE_LEN];

      switch ( message_id )
      {
         case SIRF_MSG_SSB_MEASURED_NAVIGATION: /* 0x02 (2) */
         {
            tSIRF_MSG_SSB_MEASURED_NAVIGATION *NavData = (tSIRF_MSG_SSB_MEASURED_NAVIGATION*)message_structure;
            snprintf((char*)szBuf, sizeof(szBuf), "2,%ld,%ld,%ld,%.3f,%.3f,%.3f,%u,%.1f,%u,%u,%lu,%u",
                     NavData->ecef_x,
                     NavData->ecef_y,
                     NavData->ecef_z,
                     NavData->ecef_vel_x * SIRF_MSG_SSB_VELOCITY_LSB,
                     NavData->ecef_vel_y * SIRF_MSG_SSB_VELOCITY_LSB,
                     NavData->ecef_vel_z * SIRF_MSG_SSB_VELOCITY_LSB,
                     NavData->nav_mode,
                     NavData->hdop * SIRF_MSG_SSB_DOP_LSB,
                     NavData->nav_mode2,
                     NavData->gps_week,
                     NavData->gps_tow,
                     NavData->sv_used_cnt );
            for( i = 0; i < SIRF_NUM_CHANNELS; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), ",%u", (int)NavData->sv_used[i] );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            break;
         }

         case SIRF_MSG_SSB_MEASURED_TRACKER:  /* 0x04 (4) */
         {
            tSIRF_MSG_SSB_MEASURED_TRACKER *TrkData = (tSIRF_MSG_SSB_MEASURED_TRACKER*)message_structure;
            snprintf( (char*)szBuf, sizeof(szBuf), "4,%d,%lu,%u",
                     TrkData->gps_week,
                     TrkData->gps_tow,
                     TrkData->chnl_cnt );
            for( i = 0; i < SIRF_NUM_CHANNELS; i++ )
            {
               snprintf( (char*)szFoo,  sizeof(szFoo),",%u,%u,%u,%u",
                        TrkData->chnl[i].svid,
                        (int) (TrkData->chnl[i].azimuth * SIRF_MSG_SSB_AZIMUTH_LSB),
                        (int) (TrkData->chnl[i].elevation * SIRF_MSG_SSB_ELEVATION_LSB),
                        TrkData->chnl[i].state );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
               for( j = 0; j < SIRF_NUM_POINTS; j++ )
               {
                  snprintf( szFoo,  sizeof(szFoo), ",%u", TrkData->chnl[i].cno[j] );
                  strlcat( szBuf, szFoo, sizeof(szBuf) );
               }
            }
            break;
         }

         case SIRF_MSG_SSB_RAW_TRACKER: /* 0x05 (5) */
         {
            tSIRF_MSG_SSB_RAW_TRACKER *RawTrk = (tSIRF_MSG_SSB_RAW_TRACKER*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "5,%ld,%d,%02x,%ld,%d,%d,%ld,%ld,%ld,%ld,%d",
                     RawTrk->channel,
                     RawTrk->svid,
                     RawTrk->state,
                     RawTrk->bit_number,
                     RawTrk->msec_number,
                     RawTrk->chip_number,
                     RawTrk->code_phase,
                     RawTrk->carrier_doppler,
                     RawTrk->measure_timetag,
                     RawTrk->delta_carrier_phase,
                     RawTrk->search_cnt );
            for( i = 0; i < SIRF_NUM_POINTS; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), ",%u", RawTrk->cno[i] );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            snprintf( szFoo, sizeof(szFoo), ",%u,%u,%d,%d",
                     RawTrk->power_bad_count,
                     RawTrk->phase_bad_count,
                     RawTrk->delta_car_interval,
                     RawTrk->correl_interval );
            strlcat( szBuf, szFoo, sizeof(szBuf) );
            break;
         }

         case SIRF_MSG_SSB_SW_VERSION: /* 0x06 (6) */
         {
            tSIRF_MSG_SSB_SW_VERSION *SwVersion = (tSIRF_MSG_SSB_SW_VERSION*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "SiRF Version: %s Customer Version %s", SwVersion->sirf_ver_str,SwVersion->cust_ver_str);
            break;
         }

         case SIRF_MSG_SSB_CLOCK_STATUS: /* 0x07 (7) */
         {
            tSIRF_MSG_SSB_CLOCK_STATUS *ClkStatus = (tSIRF_MSG_SSB_CLOCK_STATUS*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "Week:%u  TOW:%lu  EstGPSTime:%lu ms  SVCnt:%u  "
                               "Clock Drift:%lu Hz  Clock Bias:%lu ns",
                     ClkStatus->gps_week,
                     ClkStatus->gps_tow,
                     ClkStatus->est_gps_time,
                     ClkStatus->sv_used_cnt,
                     ClkStatus->clk_offset,
                     ClkStatus->clk_bias );
            break;
         }

         case SIRF_MSG_SSB_50BPS_DATA:             /* 0x08 */
         case SIRF_MSG_SSB_EE_50BPS_DATA_VERIFIED: /* 0x38, 0x05 */
         {
            tSIRF_MSG_SSB_50BPS_DATA *data50bps = (tSIRF_MSG_SSB_50BPS_DATA*)message_structure;

            if( SIRF_MSG_SSB_EE_50BPS_DATA_VERIFIED == message_id )
            {
               snprintf( szBuf, sizeof(szBuf), "56,5,%02u %02u ",
                               data50bps->chnl, data50bps->svid );
            }
            else
            {
               snprintf( szBuf, sizeof(szBuf), ": %02u %02u ",
                               data50bps->chnl, data50bps->svid );
            }

            for( i = 0; i < 10; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), "%08lx ", data50bps->word[i] );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            break;
         }

         case SIRF_MSG_SSB_THROUGHPUT: /* 0x09 (9) */
         {
            tSIRF_MSG_SSB_THROUGHPUT *Throughput = (tSIRF_MSG_SSB_THROUGHPUT*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "ThrPut(186=1ms): Latency:%03u SegStatMax:%03u "
                               "AveTrkTime:%03u Nav Complete:%04u ms",
                     Throughput->seg_stat_lat,
                     Throughput->seg_stat_max,
                     Throughput->avg_trk_time,
                     Throughput->last_ms );
            break;
         }

         case SIRF_MSG_SSB_ERROR: /* 0x0A (10) */
         {
            tSIRF_MSG_SSB_ERROR *pError = (tSIRF_MSG_SSB_ERROR *)message_structure;
            PErrorFormat( (tSIRF_CHAR *)szBuf, sizeof(szBuf), pError );
            break;
         }

         case SIRF_MSG_SSB_ACK: /* 0x0B (11) */
         {
            snprintf( szBuf, sizeof(szBuf), "Ack: 0x%02X 0x%02X", ((tSIRF_MSG_SSB_ACK*)message_structure)->msg_id,
                     ((tSIRF_MSG_SSB_ACK*)message_structure)->sub_id );
            break;
         }

         case SIRF_MSG_SSB_NAK: /* 0x0C (12) */
         {
            snprintf( szBuf, sizeof(szBuf), "Nak: 0x%02X 0x%02X", ((tSIRF_MSG_SSB_NAK*)message_structure)->msg_id,
                     ((tSIRF_MSG_SSB_NAK*)message_structure)->sub_id );
            break;
         }

         case SIRF_MSG_SSB_VISIBILITY_LIST: /* 0x0D (13) */
         {
            tSIRF_MSG_SSB_VISIBILITY_LIST *VisList = (tSIRF_MSG_SSB_VISIBILITY_LIST*)message_structure;

            if(VisList->svid_cnt > SIRF_MAX_SVID_CNT)
            {
               tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
            }
            else
            {
               snprintf( szBuf, sizeof(szBuf), "Visible List\n" );
               for( i = 0; i < VisList->svid_cnt; i++ )
               {
                  snprintf( szFoo, sizeof(szFoo), "SVID:%02u  Az:%03d  El:%03d\n",
                                  VisList->visible[i].svid,
                                  VisList->visible[i].azimuth,
                                  VisList->visible[i].elevation );
                  strlcat( szBuf, szFoo, sizeof(szBuf) );
               }
            }
            break;
         }

         case SIRF_MSG_SSB_ALMANAC: /* 0x0E (14) */
         {
            tSIRF_MSG_SSB_ALMANAC * Alm = (tSIRF_MSG_SSB_ALMANAC*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "ALMANAC SVID:%02u  ", Alm->svid );
            for( i = 0; i < SIRF_MSG_SSB_ALMANAC_ENTRIES; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), "%d, ", Alm->data[i] );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            break;
         }

         case SIRF_MSG_SSB_EPHEMERIS: /* 0x0F (15) */
         {
            tSIRF_MSG_SSB_EPHEMERIS * Eph = (tSIRF_MSG_SSB_EPHEMERIS*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "EPHEMERIS SVID:%02u  ", Eph->svid );
            for( i = 0; i < 3; i++ )
            {
               for ( j = 0; j < 15; j++ )
               {
                  snprintf( szFoo, sizeof(szFoo), "%u, ", Eph->subframe[i][j] );
                  strlcat( szBuf, szFoo, sizeof(szBuf) );
               }
            }
            break;
         }

         case SIRF_MSG_SSB_OK_TO_SEND: /* 0x12 (18) */
         {
            snprintf( szBuf, sizeof(szBuf), "OkToSend: %s",
               ((tSIRF_MSG_SSB_OK_TO_SEND*)message_structure)->input_enabled ? "1" : "0" );
            break;
         }

         case SIRF_MSG_SSB_RECEIVER_PARAMS: /* 0x13 (19) */
         {
            tSIRF_MSG_SSB_RECEIVER_PARAMS * RcvrParam = (tSIRF_MSG_SSB_RECEIVER_PARAMS*)message_structure;
            const char* strAltMode []={"auto","always","never use"};
            const char* strAltSrc  []={"last KF alt","fixed","dynamic"};
            const char* strDegMode []={"DThenT","TThenD","DirOnly","TimeOnly","Disabled"};
            const char* strDGPSMode[]={"auto","exclusive","never use","mixed"};
            const char* strDOPMode []={"auto","PDOP","HDOP","GDOP","disabled"};
            const char* strEnabled []={"disabled","enabled"};
            const char* strDGPSSrc []={"None","WAAS","Serial","Internal Beacon","Software"};
            float lp_duty_cycle;
            char  lp_str[300] = "";
            char  tmpStr[100] = "";

            if( (!RcvrParam->lp_on_time) || (!RcvrParam->lp_interval) )
            {
               lp_duty_cycle = 0.0;
            }
            else
            {
               lp_duty_cycle = (float) (100 * RcvrParam->lp_on_time)
                            / (float) (RcvrParam->lp_interval);
            }
            if( RcvrParam->lp_push_to_fix )
            {
               strlcat( lp_str, "Push-To-Fix enabled\n", sizeof(lp_str) );
            }
            else if( !RcvrParam->lp_pwr_cycling_enabled )
            {
               strlcat( lp_str, "Continuous power enabled\n", sizeof(lp_str) );
            }
            else
            {
               snprintf( tmpStr, sizeof(tmpStr), "TricklePower enabled with %ld ms on, %f%% duty cycle\n",
                        RcvrParam->lp_on_time, lp_duty_cycle );
               strlcat( lp_str, tmpStr, sizeof(lp_str) );
            }
            if( RcvrParam->lp_user_tasks_enabled )
            {
               snprintf( tmpStr, sizeof(tmpStr), "User tasks enabled, period = %ld\n",
                        RcvrParam->lp_user_task_interval );
               strlcat( lp_str, tmpStr, sizeof(lp_str) );
            }
            else
            {
               strlcat( lp_str, "User tasks disabled\n", sizeof(lp_str) );
            }
            snprintf( tmpStr, sizeof(tmpStr), "MaxAcqTime = %lu ms; MaxOffTime = %lu ms\n",
                     RcvrParam->lp_max_acq_time, RcvrParam->lp_max_off_time );
            strlcat( lp_str, tmpStr, sizeof(lp_str) );
            snprintf( szBuf, sizeof(szBuf), "Polling Navigation Parameters\n"
                     "AltMode: %s\nAltSource: %s\nAltitude: %d\n"
                     "DegradedMode: %s\nDegradedTimeout: %u s\nDRTimeout: %u s\n"
                     "TrkSmoothMode: %x\nStaticNav: %s\n3SV LSQ: %s\n"
                     "DOPMaskMode: %s\nElevMask:%.1f deg\nPwrMask: %u dBHz\n"
                     "DGPSSrc: %s\nDGPSMode: %s\nDGPSTimeout: %u s\n"
                     "%s",
                     strAltMode[RcvrParam->alt_mode],
                     strAltSrc[RcvrParam->alt_src],
                     RcvrParam->alt_input,
                     strDegMode[RcvrParam->degraded_mode],
                     RcvrParam->degraded_timeout,
                     RcvrParam->dr_timeout,
                     RcvrParam->trk_smooth,
                     strEnabled[RcvrParam->static_nav_mode],
                     strEnabled[RcvrParam->enable_3sv_lsq],
                     strDOPMode[RcvrParam->dop_mask_mode],
                     RcvrParam->nav_elev_mask / 10.0F,
                     RcvrParam->nav_pwr_mask,
                     strDGPSSrc[RcvrParam->dgps_src],
                     strDGPSMode[RcvrParam->dgps_mode],
                     RcvrParam->dgps_timeout,
                     lp_str );
            break;
         }

         case SIRF_MSG_SSB_TEST_MODE_DATA: /* 0x14 (20) */
         {
            tSIRF_MSG_SSB_TEST_MODE_DATA *msg = (tSIRF_MSG_SSB_TEST_MODE_DATA*)message_structure;

            if ( message_length != sizeof(*msg) )
            {
               return SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }

            snprintf( szBuf, sizeof(szBuf), "20,%u,%u,%u,%u,%u,%u,%u,%u,%u,%d,%d,%d,%ld,%d,%ld,%ld,%ld,%ld,%ld",
                                 msg->svid,
                                 msg->period,
                                 msg->bit_synch_time,
                                 msg->bit_count,
                                 msg->poor_status_count,
                                 msg->good_status_count,
                                 msg->parity_error_count,
                                 msg->lost_vco_count,
                                 msg->frame_synch_time,
                                 msg->cno_mean,
                                 msg->cno_sigma,
                                 msg->clock_drift,
                                 msg->clock_offset_mean,
                                 msg->bad_1khz_bit_count,
                                 msg->abs_i20ms,
                                 msg->abs_q1ms,
                                 msg->reserved[0],
                                 msg->reserved[1],
                                 msg->reserved[2] );
         }
         break;

         case SIRF_MSG_SSB_DGPS_STATUS: /* 0x1B (27) */
         {
            tSIRF_MSG_SSB_DGPS_STATUS *msg = (tSIRF_MSG_SSB_DGPS_STATUS*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "27,%u", msg->src );

            for( i = 0; i < SIRF_NUM_CHANNELS; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), ",%u", msg->cor_age[i] );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }

            snprintf( szFoo, sizeof(szFoo), ",%u,%u", msg->reserved[0], msg->reserved[1] );
            strlcat( szBuf, szFoo, sizeof(szBuf) );

            for( i = 0; i < SIRF_NUM_CHANNELS; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), ",%u,%d", msg->corrections[i].prn, msg->corrections[i].cor );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }

            break;
         }

         case SIRF_MSG_SSB_NL_MEAS_DATA: /* 0x1C (28) */
         {
            tSIRF_MSG_SSB_NL_MEAS_DATA *NlMeas = (tSIRF_MSG_SSB_NL_MEAS_DATA*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "28,%u,%lu,%u,%.10e,%.10e,%.10e,%.10e,%u,%u",
                     NlMeas->Chnl,
                     NlMeas->Timetag,
                     NlMeas->svid,
                     NlMeas->gps_sw_time,
                     NlMeas->pseudorange,
                     NlMeas->carrier_freq,
                     NlMeas->carrier_phase,
                     NlMeas->time_in_track,
                     NlMeas->sync_flags);
            for( i = 0; i < SIRF_NUM_POINTS; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), ",%u", NlMeas->cton[i] );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            snprintf( szFoo, sizeof(szFoo), ",%u,%u,%d,%u,%u",
                     NlMeas->delta_range_interval,
                     NlMeas->mean_delta_range_time,
                     NlMeas->extrapolation_time,
                     NlMeas->phase_error_count,
                     NlMeas->low_power_count );
            strlcat( szBuf, szFoo, sizeof(szBuf) );
            break;
         }

         case SIRF_MSG_SSB_NL_DGPS_DATA: /* 0x1D (29) */
         {
            tSIRF_MSG_SSB_NL_DGPS_DATA *NlDgps = (tSIRF_MSG_SSB_NL_DGPS_DATA*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "29,%d,%d,%u,%.10e,%.10e,%.10e,%.10e,%.10e",
                     NlDgps->svid,
                     NlDgps->iod,
                     NlDgps->source,
                     NlDgps->pr_correction,
                     NlDgps->prr_correction,
                     NlDgps->correction_age,
                     NlDgps->prc_variance,
                     NlDgps->prrc_variance );
            break;
         }

         case SIRF_MSG_SSB_NL_SV_STATE_DATA: /* 0x1E (30) */
         {
            tSIRF_MSG_SSB_NL_SV_STATE_DATA *NlSvState = (tSIRF_MSG_SSB_NL_SV_STATE_DATA*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "30,%u,%.11e",
                     NlSvState->svid,
                     NlSvState->time );
            for( i = 0; i < 3; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), ",%.10e", NlSvState->pos[i] );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            for( i = 0; i < 3; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), ",%.10e", NlSvState->vel[i] );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            snprintf( szFoo, sizeof(szFoo), ",%.10e,%.10e,%u,%.10e,%.10e,%.10e",
                     NlSvState->clk,
                     NlSvState->clf,
                     NlSvState->eph,
                     NlSvState->posvar,
                     NlSvState->clkvar,
                     NlSvState->iono );
            strlcat( szBuf, szFoo, sizeof(szBuf) );
            break;
         }

         case SIRF_MSG_SSB_NL_INIT_DATA: /* 0x1F (31) */
         {
            tSIRF_MSG_SSB_NL_INIT_DATA * NlInit = (tSIRF_MSG_SSB_NL_INIT_DATA*)message_structure;

            /* From NL_ControlBlock */
            snprintf( szBuf, sizeof(szBuf), "31,%u,%u,%u,%.10e,%u,%d,%d,%d,%u,%u,%d,%d,%d,%u,%d,%d"
                                 ",%d,%u,%d,%u,%d,%u,%d",
                     NlInit->enable_con_alt_mode, NlInit->alt_mode, NlInit->alt_source,
                     NlInit->altitude, NlInit->degraded_mode, NlInit->degraded_timeout,
                     NlInit->dr_timeout, NlInit->coast_timeout, NlInit->tracksmooth_mode,
                     NlInit->dop_selection, NlInit->hdop_thresh, NlInit->gdop_thresh,
                     NlInit->pdop_thresh, NlInit->dgps_selection, NlInit->dgps_timeout,
                     NlInit->elev_nav_mask, NlInit->pow_nav_mask, NlInit->editing_residual_mode,
                     NlInit->editing_residual_threshold, NlInit->ssd_mode, NlInit->ssd_threshold,
                     NlInit->static_nav_mode, NlInit->static_nav_threshold );

            /* From NL_PositionInitStruct */
            snprintf( szFoo, sizeof(szFoo), ",%.10e,%.10e,%.10e,%u",
                     NlInit->ecef_x, NlInit->ecef_y, NlInit->ecef_z, NlInit->position_init_source );
            strlcat( szBuf, szFoo, sizeof(szBuf) );

            /* From NL_TimeInitStruct */
            snprintf( szFoo, sizeof(szFoo), ",%.10e,%d,%u",
                     NlInit->gps_time, NlInit->gps_week, NlInit->time_init_source );
            strlcat( szBuf, szFoo, sizeof(szBuf) );

            /* from NL_ClockDriftInitStruct */
            snprintf( szFoo, sizeof(szFoo), ",%.10e,%u",
                     NlInit->clk_offset, NlInit->clk_offset_init_source );
            strlcat( szBuf, szFoo, sizeof(szBuf) );
            break;
         }

         case SIRF_MSG_SSB_NL_AUX_INIT_DATA: /* 0x40 0x01 */
         {
            tSIRF_MSG_SSB_NL_AUX_INIT_DATA * NlAuxInit = (tSIRF_MSG_SSB_NL_AUX_INIT_DATA*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "64,1,%lu,%u,%lu,%u,%u,%u,%u,%u,%lu,%lu,%lu,%lu",
                      NlAuxInit->time_init_unc,NlAuxInit->saved_pos_week,NlAuxInit->saved_pos_tow,
                      NlAuxInit->saved_pos_ehe,NlAuxInit->saved_pos_eve,NlAuxInit->sw_version,NlAuxInit->icd_version,
                      NlAuxInit->chip_version,NlAuxInit->acq_clk_speed,NlAuxInit->default_clock_offset,
                      NlAuxInit->tracker_status,NlAuxInit->reserved);
            break;
         }

         case SIRF_MSG_SSB_NL_AUX_MEAS_DATA: /* 0x40 0x02 */
         {
            tSIRF_MSG_SSB_NL_AUX_MEAS_DATA * NlAuxMeas = (tSIRF_MSG_SSB_NL_AUX_MEAS_DATA*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "64,2,%u,%u,%u,%u,%lu,%lu,%ld,%ld,%d,%d,%ld,%ld,%ld,%ld,"
                            "%d,%d,%d,%d,%u,%u,%ld,%d,%d,%u,%lu",
                      NlAuxMeas->sv_prn, NlAuxMeas->status, NlAuxMeas->extended_status,NlAuxMeas->bit_sync_qual,
                      NlAuxMeas->time_tag,NlAuxMeas->code_phase,NlAuxMeas->carrier_phase,NlAuxMeas->carrier_freq,
                      NlAuxMeas->carrier_accel,NlAuxMeas->ms_num,NlAuxMeas->bit_num,NlAuxMeas->code_correction,
                      NlAuxMeas->smooth_code,NlAuxMeas->code_offset,NlAuxMeas->pseudorange_noise,NlAuxMeas->delta_range_qual,
                      NlAuxMeas->phase_lock_qual,NlAuxMeas->ms_uncertainty,NlAuxMeas->sum_abs_I,NlAuxMeas->sum_abs_Q,
                      NlAuxMeas->sv_bit_num,NlAuxMeas->mp_los_det_value,NlAuxMeas->mp_only_det_value,
                      NlAuxMeas->recovery_status,NlAuxMeas->sw_time_uncertainty);
            break;
         }

         case SIRF_MSG_SSB_NL_AUX_AID_DATA: /* 0x40 0x03 */
         {
            tSIRF_MSG_SSB_NL_AUX_AID_DATA * NlAuxAid = (tSIRF_MSG_SSB_NL_AUX_AID_DATA*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "64,3,%ld,%ld,%ld,%lu,%u,%lu",
                            NlAuxAid->ecef_x,NlAuxAid->ecef_y,NlAuxAid->ecef_z,NlAuxAid->horiz_pos_unc,
                            NlAuxAid->alt_unc,NlAuxAid->sw_tow);
            break;
         }

         case SIRF_MSG_SSB_GEODETIC_NAVIGATION: /* 0x29 (41) */
         {
            tSIRF_MSG_SSB_GEODETIC_NAVIGATION *GeodNavState = (tSIRF_MSG_SSB_GEODETIC_NAVIGATION*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "41,%u,%u,%u,%lu,%u,%u,%u,%u,%u,%u,%lu,%ld,%ld,%ld,%ld,%u,%u,%u,"
                               "%d,%d,%d,%lu,%lu,%lu,%u,%ld,%lu,%ld,%lu,%lu,%u,%u,%u,%u,%u",
                     GeodNavState->nav_valid, GeodNavState->nav_mode,
                     GeodNavState->gps_week, GeodNavState->gps_tow, GeodNavState->utc_year,
                     GeodNavState->utc_month, GeodNavState->utc_day, GeodNavState->utc_hour,
                     GeodNavState->utc_min, GeodNavState->utc_sec, GeodNavState->sv_used,
                     GeodNavState->lat, GeodNavState->lon, GeodNavState->alt_ellips,
                     GeodNavState->alt_msl, GeodNavState->datum, GeodNavState->sog,
                     GeodNavState->hdg, GeodNavState->mag_var, GeodNavState->climb_rate,
                     GeodNavState->heading_rate, GeodNavState->ehpe, GeodNavState->evpe,
                     GeodNavState->ete, GeodNavState->ehve, GeodNavState->clk_bias,
                     GeodNavState->clk_bias_error, GeodNavState->clk_offset,
                     GeodNavState->clk_offset_error, GeodNavState->distance_travelled,
                     GeodNavState->distance_travelled_error, GeodNavState->heading_error, GeodNavState->sv_used_cnt,
                     GeodNavState->hdop, GeodNavState->additional_mode_info );
            break;
         }

         case SIRF_MSG_SSB_ADC_ODOMETER_DATA: /* 0x2D (45) */
         {
            if ( sizeof(tSIRF_MSG_SSB_ADC_ODOMETER_DATA) == message_length )
            {
               tSIRF_MSG_SSB_ADC_ODOMETER_DATA *ADCData10Hz = (tSIRF_MSG_SSB_ADC_ODOMETER_DATA*)message_structure;
               snprintf( szBuf, sizeof(szBuf), "45,%lu,%d,%d,%u,%u",
                  ADCData10Hz->current_time,
                  ADCData10Hz->adc2_avg,
                  ADCData10Hz->adc3_avg,
                  ADCData10Hz->odo_count,
                  ADCData10Hz->gpio_stat);
            }
            else if ( sizeof(tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ) == message_length )
            {
               tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ *ADCData1Hz = (tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ*)message_structure;
               snprintf( szBuf, sizeof(szBuf), "45" );
               for ( i=0; i<10; i++ )
               {
                  snprintf(szFoo, sizeof(szFoo), ",%lu,%d,%d,%u,%u",
                     ADCData1Hz->dataset[i].current_time,
                     ADCData1Hz->dataset[i].adc2_avg,
                     ADCData1Hz->dataset[i].adc3_avg,
                     ADCData1Hz->dataset[i].odo_count,
                     ADCData1Hz->dataset[i].gpio_stat);
                  strlcat( szBuf, szFoo, sizeof(szBuf) );
               }
            }
            break;
         }

         case SIRF_MSG_SSB_TEST_MODE_DATA_3: /* 0x2E (46) */
         {
            tSIRF_MSG_SSB_TEST_MODE_DATA_3 *msg = (tSIRF_MSG_SSB_TEST_MODE_DATA_3*)message_structure;

            if ( message_length != sizeof(*msg) )
            {
               return SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
            }

            snprintf( szBuf, sizeof(szBuf), "46,%u,%u,%u,%u,%u,%u,%u,%u,%u,%d,%d,%d,%ld,%d,%ld,%ld,%ld,%u,%u,%u,%u,%u",
                                 msg->svid,
                                 msg->period,
                                 msg->bit_synch_time,
                                 msg->bit_count,
                                 msg->poor_status_count,
                                 msg->good_status_count,
                                 msg->parity_error_count,
                                 msg->lost_vco_count,
                                 msg->frame_synch_time,
                                 msg->cno_mean,
                                 msg->cno_sigma,
                                 msg->clock_drift,
                                 msg->clock_offset,
                                 msg->bad_1khz_bit_count,
                                 msg->abs_i20ms,
                                 msg->abs_q20ms,
                                 msg->phase_lock,
                                 msg->rtc_frequency,
                                 msg->e_to_acq_ratio,
                                 msg->t_sync_agc_gain,
                                 msg->tm_5_ready,
                                 msg->ClkDriftUnc );
         }
         break;

         case SIRF_MSG_SSB_DR_NAV_STATUS: /* 0x0130 (48, 1) */
         {
            tSIRF_MSG_SSB_DR_NAV_STATUS * msg = (tSIRF_MSG_SSB_DR_NAV_STATUS*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "48,1,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",

                     msg->nav,
                     msg->data,
                     msg->cal_gb_cal,
                     msg->gsf_cal_ssf_cal,
                     msg->nav_across_reset_pos,
                     msg->hd,
                     msg->gyr_sub_op_spd_sub_op,
                     msg->nav_st_int_ran_z_gb_cal_upd,
                     msg->gbsf_cal_upd_spd_cal_upd_upd_nav_st,
                     msg->gps_upd_pos,
                     msg->gps_upd_hd,
                     msg->gps_pos_gps_vel,
                     msg->dws_hd_rt_s_f_cal_valid,
                     msg->dws_hd_rt_s_f_cal_upd,
                     msg->dws_spd_s_f_cal_valid,
                     msg->dws_spd_s_f_cal_upd );
            break;
         }

         case SIRF_MSG_SSB_DR_NAV_STATE: /* 0x0230 (48, 2) */
         {
            tSIRF_MSG_SSB_DR_NAV_STATE * msg = (tSIRF_MSG_SSB_DR_NAV_STATE*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "48,2,%u,%u,%d,%u,%d,%u,%d,%u,%d,%u,%lu,%u,%u,%u,%u, %u,%u,%d,%u,%d,%u,%d,%u,%d,%u,%d,%u,%d,%u,%u,%d,%u,%d,%u,%u,%d,%u",
                     msg->spd,
                     msg->spd_e,
                     msg->ssf,
                     msg->ssf_e,
                     msg->hd_rte,
                     msg->hd_rte_e,
                     msg->gb,
                     msg->gbE,
                     msg->gsf,
                     msg->gsf_e,
                     msg->tpe,
                     msg->the,
                     msg->nav_ctrl,
                     msg->reverse,
                     msg->hd,
                     msg->sensor_pkg,
                     msg->odo_spd,
                     msg->odo_spd_s_f,
                     msg->odo_spd_s_f_err,
                     msg->lf_wheel_spd_sf,
                     msg->lf_wheel_spd_sf_err,
                     msg->rf_wheel_spd_sf,
                     msg->rf_wheel_spd_sf_err,
                     msg->lr_wheel_spd_sf,
                     msg->lr_wheel_spd_sf_err,
                     msg->rr_wheel_spd_sf,
                     msg->rr_wheel_spd_sf_err,
                     msg->rear_axle_spd_delta,
                     msg->rear_axle_avg_spd,
                     msg->rear_axle_spd_err,
                     msg->rear_axle_hd_rt,
                     msg->rear_axle_hd_rt_err,
                     msg->front_axle_spd_delta,
                     msg->front_axle_avg_spd,
                     msg->front_axle_spd_err,
                     msg->front_axle_hd_rt,
                     msg->front_axle_hd_rt_err);
            break;
         }

         case SIRF_MSG_SSB_DR_NAV_SUBSYS: /* 0x0330 (48, 3) */
         {
            tSIRF_MSG_SSB_DR_NAV_SUBSYS * msg = (tSIRF_MSG_SSB_DR_NAV_SUBSYS*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "48,3,%d,%u,%u,%u,%u,%u,%lu,%d,%u,%u,%u,%u,%u,%lu,%u,%u",
                     msg->gps_hd_rte,
                     msg->gps_hd_rte_e,
                     msg->gps_hd,
                     msg->gps_hd_e,
                     msg->gps_spd,
                     msg->gps_spd_e,
                     msg->gps_pos_e,
                     msg->dr_hd_rte,
                     msg->dr_hd_rte_e,
                     msg->dr_hd,
                     msg->dr_hd_e,
                     msg->dr_spd,
                     msg->dr_spd_e,
                     msg->dr_pos_e,
                     msg->reserved[0],
                     msg->reserved[1]);
            break;
         }

         case SIRF_MSG_SSB_DR_SENS_PARAM: /* 0x00730 (48, 7) */
         {
            tSIRF_MSG_SSB_DR_SENS_PARAM *msg = (tSIRF_MSG_SSB_DR_SENS_PARAM*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "48,7,%u,%u,%u,",
                     msg->base_ssf,
                     msg->base_gb,
                     msg->base_gsf );
            break;
         }

         case SIRF_MSG_SSB_DR_DATA_BLK: /* 0x0830 (48, 8) */
         {
            tSIRF_MSG_SSB_DR_DATA_BLK * msg = (tSIRF_MSG_SSB_DR_DATA_BLK*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "48,8,%u,%u,%u,"
                               "%lu,%u,%d,%lu,%u,%d,%lu,%u,%d,%lu,%u,%d,%lu,%u,%d,"
                               "%lu,%u,%d,%lu,%u,%d,%lu,%u,%d,%lu,%u,%d,%lu,%u,%d",
              msg->meas_type,  msg->valid_cnt,  msg->bkup_flgs,
              msg->blk[0].tag, msg->blk[0].spd, msg->blk[0].hd_rte,
              msg->blk[1].tag, msg->blk[1].spd, msg->blk[1].hd_rte,
              msg->blk[2].tag, msg->blk[2].spd, msg->blk[2].hd_rte,
              msg->blk[3].tag, msg->blk[3].spd, msg->blk[3].hd_rte,
              msg->blk[4].tag, msg->blk[4].spd, msg->blk[4].hd_rte,
              msg->blk[5].tag, msg->blk[5].spd, msg->blk[5].hd_rte,
              msg->blk[6].tag, msg->blk[6].spd, msg->blk[6].hd_rte,
              msg->blk[7].tag, msg->blk[7].spd, msg->blk[7].hd_rte,
              msg->blk[8].tag, msg->blk[8].spd, msg->blk[8].hd_rte,
              msg->blk[9].tag, msg->blk[9].spd, msg->blk[9].hd_rte );
            break;
         }

         case SIRF_MSG_SSB_MMF_STATUS: /* 0x5030 (48, 80) */
         {
            tSIRF_MSG_SSB_MMF_STATUS *msg = (tSIRF_MSG_SSB_MMF_STATUS*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "48,80,%lu,%u,%ld,%ld,%ld,%lu,%u,%ld,%ld,%ld,%lu",
                  msg->status,
                  msg->sys_hdg,
                  msg->sys_lat,
                  msg->sys_lon,
                  msg->sys_alt,
                  msg->sys_tow,
                  msg->mmf_hdg,
                  msg->mmf_lat,
                  msg->mmf_lon,
                  msg->mmf_alt,
                  msg->mmf_tow );
            break;
         }

         case SIRF_MSG_SSB_SBAS_PARAM: /* 0x32 (50) */
         {
            tSIRF_MSG_SSB_SBAS_PARAM *msg = (tSIRF_MSG_SSB_SBAS_PARAM*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "50,%u,%u,%u,%u",
                  msg->prn,
                  msg->mode,
                  msg->timeout,
                  msg->flg_bits );
            break;
         }

         case SIRF_MSG_SSB_SIRFNAV_COMPLETE: /* 0x0133 (51, 1) */
         {
            tSIRF_MSG_SSB_SIRFNAV_COMPLETE *msg = (tSIRF_MSG_SSB_SIRFNAV_COMPLETE*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "51,1,%u",
                  msg->reserved);
            break;
         }

         case SIRF_MSG_SSB_SIRFNAV_TIME_TAGS: /* 0x0433 (51, 4) */
         {
            tSIRF_MSG_SSB_SIRFNAV_TIME_TAGS *msg = (tSIRF_MSG_SSB_SIRFNAV_TIME_TAGS*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "51,4,%lu,%u,%lu,%u,%u,%u,%u,%u,%u,%u,%lu",
                  msg->tracker_time_tag,
                  msg->gps_week,
                  msg->gps_tow,
                  msg->tracker_rtc_day,
                  msg->tracker_rtc_hour,
                  msg->tracker_rtc_min,
                  msg->tracker_rtc_sec,
                  msg->tracker_rtc_clkctr,
                  msg->tracker_rtc_ok,
                  msg->tracker_rtc_rollover_complete,
                  msg->tracker_rtc_rollover_seconds );
            break;
         }

         case SIRF_MSG_SSB_TRACKER_LOADER_STATE: // 0x0633 (51, 6)
         {
            tSIRF_MSG_SSB_TRACKER_LOADER_STATE *msg = (tSIRF_MSG_SSB_TRACKER_LOADER_STATE*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "51,6,%lu,%lu,%lu,%lu",
              msg->loader_state,
              msg->percentage_loaded,
              msg->loader_error,
              msg->time_tag );
            break;
         }

         case SIRF_MSG_SSB_SIRFNAV_START: /* 0x0733 (51, 7) */
         {
            tSIRF_MSG_SSB_SIRFNAV_START *msg = (tSIRF_MSG_SSB_SIRFNAV_START*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "51,7,%lu,%lu,%lu,%lu",
                  msg->start_mode,
                  msg->clock_offset,
                  msg->port_num,
                  msg->baud_rate );
            break;
         }
         case SIRF_MSG_SSB_EE_GPS_TIME_INFO: /* 0x0138 */
         {
            tSIRF_MSG_SSB_EE_GPS_TIME_INFO *msg = (tSIRF_MSG_SSB_EE_GPS_TIME_INFO*) message_structure;

            snprintf( szBuf, sizeof(szBuf), "56,1,%u,%u,%lu,%lu",
                  msg->time_valid_flag,
                  msg->gps_week,
                  msg->gps_tow,
                  msg->ephemeris_request_mask );
         }
         break;

         case SIRF_MSG_SSB_EE_ACK: /* 0xFF38 */
         {
            tSIRF_MSG_SSB_EE_ACK *msg = (tSIRF_MSG_SSB_EE_ACK*) message_structure;

            snprintf( szBuf, sizeof(szBuf), "56,255,%u,%u",
                  msg->acq_id,
                  msg->acq_sub_id );
         }
         break;

         case SIRF_MSG_SSB_SIRFNAV_STOP: /* 0x0833 (161, 8) */
         {
            tSIRF_MSG_SSB_SIRFNAV_STOP *msg = (tSIRF_MSG_SSB_SIRFNAV_STOP*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "51,8,%lu",
                  msg->stop_mode );
            break;
         }

         case SIRF_MSG_SSB_STARTUP_INFO: /* 0x0136 (54, 1) */
         {
            tSIRF_MSG_SSB_STARTUP_INFO *msg = (tSIRF_MSG_SSB_STARTUP_INFO*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "54,1,%u,%u,%u,"
                               "%lu,%u,%lu,%u,%u,%u,%u,%u,%u,"
                               "%ld,%ld,%ld,%u,"
                               "%lu,%lu,%u,%u,%ld,%lu",
              msg->mode,  msg->status,  msg->dr_status,
              msg->gps_off_time, msg->gps_week, msg->gps_tow,   msg->utc_year, msg->utc_month, msg->utc_day,   msg->utc_hour, msg->utc_min, msg->utc_sec,
              msg->lat, msg->lon, msg->alt_ellips, msg->heading,
              msg->valid_ephemeris, msg->collected_almanac, msg->collected_almanac_week, msg->factory_almanac_week,   msg->clk_offset, msg->reserved );
            break;
         }

         case SIRF_MSG_SSB_GPIO_READ: /* 0xC041 (65,192) */
         {
            tSIRF_MSG_SSB_GPIO_READ *msg = (tSIRF_MSG_SSB_GPIO_READ*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "65,192,%u", msg->gpio_state );
            break;
         }

         case SIRF_MSG_SSB_DOP_VALUES: /* 0x42 (66) */
         {
            tSIRF_MSG_SSB_DOP_VALUES *msg = (tSIRF_MSG_SSB_DOP_VALUES*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "66,%lu,%u,%u,%u,%u,%u",
              msg->gps_tow,
              msg->gdop, msg->pdop, msg->hdop, msg->vdop, msg->tdop );
            break;
         }

         case SIRF_MSG_SSB_TRKR_DBG: /* 0x44 (68) */
         {
            tSIRF_CHAR *msg = ((tSIRF_CHAR*)message_structure);

            if((msg[0] == (tSIRF_CHAR)SIRF_GET_MID(SIRF_MSG_SSB_SIRF_INTERNAL_OUT)) &&
               (msg[1] == 0)) /* Encrypted tracker output */
            {
               snprintf(szBuf, sizeof(szBuf), "68,%ld,%02X",
                        (unsigned long)SIRF_GET_MID(SIRF_MSG_SSB_SIRF_INTERNAL_OUT), msg[1]);
               for ( i = 2; i < message_length; i++ )
               {
                  snprintf( szFoo, sizeof(szFoo), ",%02X", ((tSIRF_UINT8*)message_structure)[i] );
                  strlcat( szBuf, szFoo, sizeof(szBuf) );
               }
            }
            else
            {
               snprintf(szBuf, sizeof(szBuf), "68,%u,%*s",
                        (tSIRF_UINT8)(*msg), (int)(message_length-1), msg+1 );
            }
            break;
         }

         case SIRF_MSG_SSB_DEMO_START_GPS_ENGINE: /* 0x05A1 (161, 5) */
         {
            tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE *msg = (tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "161,5,%lu,%lu,%lu,%lu",
                  msg->start_mode,
                  msg->clock_offset,
                  msg->port_num,
                  msg->baud_rate );
            break;
         }

         case SIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE: /* 0x06A1 (161, 6) */
         {
            tSIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE *msg = (tSIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "161,6,%lu",
                  msg->stop_mode );
            break;
         }

         case SIRF_MSG_SSB_SIRFNAV_STORE_NOW: /* 0x07A1 (161, 7) */
         {
            snprintf( szBuf, sizeof(szBuf), "161,7" );
            break;
         }

         case SIRF_MSG_SSB_DR_CAR_BUS_DATA: /* 0x09AC (172, 9) */
         {
            tSIRF_MSG_SSB_DR_CAR_BUS_DATA *msg = (tSIRF_MSG_SSB_DR_CAR_BUS_DATA*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "172,9,%u,%u,%u",
                  msg->sensor_data_type,
                  msg->num_of_data_sets,
                  msg->reverse_bitmap );
            for ( i = 0; i < 11; i++ )
            {
                  snprintf( szFoo, sizeof(szFoo), ",%u,%lu,%u,%d,%d,%d,%d,%u",
                     msg->data[i].valid_sensor_indication,
                     msg->data[i].data_set_time_tag,
                     msg->data[i].odometer_speed,
                     msg->data[i].data1,
                     msg->data[i].data2,
                     msg->data[i].data3,
                     msg->data[i].data4,
                     msg->data[i].reserved );
                  strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            break;
         }

         case SIRF_MSG_SSB_DR_CAR_BUS_ENABLED: /* 0x0AAC (172, 10) */
         {
            tSIRF_MSG_SSB_DR_CAR_BUS_ENABLED *msg = (tSIRF_MSG_SSB_DR_CAR_BUS_ENABLED*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "172,10,%lu",
                     msg->mode );
            break;
         }

         case SIRF_MSG_SSB_DR_CAR_BUS_DISABLED: /* 0x0BAC (172, 11) */
         {
            tSIRF_MSG_SSB_DR_CAR_BUS_DISABLED *msg = (tSIRF_MSG_SSB_DR_CAR_BUS_DISABLED*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "172,11,%lu",
                     msg->mode );
            break;
         }

         case SIRF_MSG_SSB_DR_SENSOR_DATA: /* 0x13AC (172, 19) */
         {
            tSIRF_MSG_SSB_DR_SENSOR_DATA *msg = (tSIRF_MSG_SSB_DR_SENSOR_DATA*)message_structure;

            if(msg->num_of_data_sets > SIRF_MSG_SSB_DR_SENSOR_DATA_MAX)
            {
               tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
            }
            else
            {
               snprintf( szBuf, sizeof(szBuf), "172,19,%u,%u,%u",
                     msg->sensor_data_type,
                     msg->num_of_data_sets,
                     msg->reserved );
               for ( i = 0; i < msg->num_of_data_sets; i++ )
               {
                  snprintf( szFoo, sizeof(szFoo), ",%u,%lu,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                     msg->data_set[i].valid_data_indication,
                     msg->data_set[i].data_set_time_tag,
                     msg->data_set[i].data[0],
                     msg->data_set[i].data[1],
                     msg->data_set[i].data[2],
                     msg->data_set[i].data[3],
                     msg->data_set[i].data[4],
                     msg->data_set[i].data[5],
                     msg->data_set[i].data[6],
                     msg->data_set[i].data[7],
                     msg->data_set[i].data[8] );
                  strlcat( szBuf, szFoo, sizeof(szBuf) );
               }
            }
            break;
         }


         case SIRF_MSG_SSB_MMF_DATA: /* 0x50AC (172, 80) */
         {
            tSIRF_MSG_SSB_MMF_DATA *msg = (tSIRF_MSG_SSB_MMF_DATA*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "172,80,%lu,%u,%u",
                  msg->gps_tow_reference,
                  msg->num_of_data_sets,
                  msg->control );
            for ( i = 0; i < 3; i++ )
            {
                  snprintf( szFoo, sizeof(szFoo), ",%ld,%ld,%lu,%ld,%lu,%u,%u,%u",
                     msg->mmf_data[i].lat,
                     msg->mmf_data[i].lon,
                     msg->mmf_data[i].horiz_pos_uncert,
                     msg->mmf_data[i].alt_ellips,
                     msg->mmf_data[i].vert_pos_uncert,
                     msg->mmf_data[i].heading,
                     msg->mmf_data[i].heading_uncert,
                     msg->mmf_data[i].reserved );
                  strlcat( szBuf, szFoo, sizeof(szBuf) );
            } /* for */
            break;
         }

         case SIRF_MSG_SSB_MMF_SET_MODE: /* 0x51AC (172, 81) */
         {
            tSIRF_MSG_SSB_MMF_SET_MODE *msg = (tSIRF_MSG_SSB_MMF_SET_MODE*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "172,81,%lu",
                     msg->mode );
            break;
         }

         case SIRF_MSG_SSB_GPIO_WRITE: /* 0xB2 0x30 (178,48) */
         {
            tSIRF_MSG_SSB_GPIO_WRITE *msg = (tSIRF_MSG_SSB_GPIO_WRITE*)message_structure;
            snprintf(szBuf, sizeof(szBuf), "178,48,%u,%u", msg->gpio_to_write, msg->gpio_state);
            break;
         }

         case SIRF_MSG_SSB_GPIO_MODE_SET: /* 0xB2 0x31 (178,49) */
         {
            tSIRF_MSG_SSB_GPIO_MODE_SET *msg = (tSIRF_MSG_SSB_GPIO_MODE_SET*)message_structure;
            snprintf(szBuf, sizeof(szBuf), "178,49,%u,%u,%u", msg->gpio_to_set, msg->gpio_mode, msg->gpio_state);
            break;
         }

         case SIRF_MSG_SSB_TRK_HW_TEST_CONFIG: /* 0xCF */
         {
            tSIRF_MSG_SSB_TRK_HW_TEST_CONFIG *msg = (tSIRF_MSG_SSB_TRK_HW_TEST_CONFIG*) message_structure;

            snprintf( szBuf, sizeof(szBuf), "207,%d,%d",
                     msg->RFTestPoint,
                     msg->INTCpuPause );
         }
         break;

         case SIRF_MSG_SSB_SET_IF_TESTPOINT: /* 0xD2 */
         {
            tSIRF_MSG_SSB_SET_IF_TESTPOINT *msg = (tSIRF_MSG_SSB_SET_IF_TESTPOINT*) message_structure;

            snprintf( szBuf, sizeof(szBuf), "210,%d",
                     msg->test_point_control );
         }
         break;

         case SIRF_MSG_SSB_EE_SEA_PROVIDE_EPH:  /* 0x01 0xE8 */
         {
            tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH *msg = (tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH*) message_structure;

            snprintf( szBuf, sizeof(szBuf), "232,1,%u,%lu",
               msg->week,
               msg->svid_mask );

            for( i = 0; i < SV_PER_PACKET; i++)
            {
               snprintf( szFoo, sizeof(szFoo), ",%u,%u,%u,%u,%d,%d,%ld,%d,%lu,%d,%lu,%u,%d,%ld,%d,%ld,%d,%ld,%ld,%d,%u,%d,%d,%d,%ld,%u",
                  msg->extended_ephemeris[i].PRN,
                  msg->extended_ephemeris[i].ephemerisValidityFlag,
                  msg->extended_ephemeris[i].URA,
                  msg->extended_ephemeris[i].IODE,
                  msg->extended_ephemeris[i].Crs,
                  msg->extended_ephemeris[i].deltaN,
                  msg->extended_ephemeris[i].M0,
                  msg->extended_ephemeris[i].Cuc,
                  msg->extended_ephemeris[i].eccen,
                  msg->extended_ephemeris[i].Cus,
                  msg->extended_ephemeris[i].sqrtA,
                  msg->extended_ephemeris[i].toe,
                  msg->extended_ephemeris[i].Cic,
                  msg->extended_ephemeris[i].omega0,
                  msg->extended_ephemeris[i].Cis,
                  msg->extended_ephemeris[i].i0,
                  msg->extended_ephemeris[i].Crc,
                  msg->extended_ephemeris[i].w,
                  msg->extended_ephemeris[i].omegaDot,
                  msg->extended_ephemeris[i].iDot,
                  msg->extended_ephemeris[i].toc,
                  msg->extended_ephemeris[i].Tgd,
                  msg->extended_ephemeris[i].af2,
                  msg->extended_ephemeris[i].af1,
                  msg->extended_ephemeris[i].af0,
                  msg->extended_ephemeris[i].age );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            } /* for */

            snprintf( szFoo, sizeof(szFoo), ",%u,%u,%u,%u,%u,%u,%u,%u",
               msg->extended_iono.alpha[0],
               msg->extended_iono.alpha[1],
               msg->extended_iono.alpha[2],
               msg->extended_iono.alpha[3],
               msg->extended_iono.beta[0],
               msg->extended_iono.beta[1],
               msg->extended_iono.beta[2],
               msg->extended_iono.beta[3] );
            strlcat( szBuf, szFoo, sizeof(szBuf) );
         }
         break;

         case SIRF_MSG_SSB_EE_FILE_DOWNLOAD:
         {
            tSIRF_MSG_SSB_EE_FILE_DOWNLOAD *msg = (tSIRF_MSG_SSB_EE_FILE_DOWNLOAD*) message_structure;

            snprintf( szBuf, sizeof(szBuf), "232,16,%lu",
                  msg->reserved );
            break;
         }

         case SIRF_MSG_SSB_SIF_SET_CONFIG: /* 0xFCE8 (232, 252) */
         {
            tSIRF_MSG_SSB_SIF_SET_CONFIG *msg = (tSIRF_MSG_SSB_SIF_SET_CONFIG*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "232,252,%u,%u,%u,%u,%u",
                  msg->operation_mode, msg->file_format, msg->ext_gps_time_src, msg->cgee_input_method, msg->sgee_input_method );
            break;
         }

         case SIRF_MSG_SSB_EE_FILE_PART: /* 0x12E8 (18, 252) */
         {
            tSIRF_MSG_SSB_EE_FILE_PART *msg = (tSIRF_MSG_SSB_EE_FILE_PART*)message_structure;
            tSIRF_UINT32 i;
            if(msg->buffSize > SSB_DLD_MAX_PKT_LEN)
            {
               tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
            }
            else
            {
               snprintf( szBuf, sizeof(szBuf), "232,18");

               for(i = 0; i < msg->buffSize; i++)
               {
                  snprintf(szFoo, sizeof(szFoo), ",%u",msg->buff[i]);
                  strlcat(szBuf, szFoo, sizeof(szBuf));
               }
            }
            break;
         }

         case SIRF_MSG_SSB_EE_DISABLE_EE_SECS: /* 0xFEE8 (232, 254) */
         {
            tSIRF_MSG_SSB_EE_DISABLE_EE_SECS *msg = (tSIRF_MSG_SSB_EE_DISABLE_EE_SECS*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "232,254,%lu",
                  msg->num_secs );
            break;
         }

#ifdef EMB_SIF
         case SIRF_MSG_SSB_SIF_DISABLE_AIDING: /* 0xFEE8 (232, 254) */
         {
            tSIRF_MSG_SSB_SIF_DISABLE_AIDING *msg = (tSIRF_MSG_SSB_SIF_DISABLE_AIDING*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "232,32,%u,%u",
                  msg->sgeeDisable,msg->cgeeDisable);
            break;
         }

         case SIRF_MSG_SSB_SIF_GET_AIDING_STATUS:/* 0xFEE8 (232, 33) */
         {
            tSIRF_MSG_SSB_SIF_GET_AIDING_STATUS *msg = (tSIRF_MSG_SSB_SIF_GET_AIDING_STATUS*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "232,33,%u",
                  msg->reserved);
            break;
         }
#endif /*EMB_SIF*/

         case SIRF_MSG_SSB_EE_DEBUG: /* 0xFFE8 (232, 255) */
         {
            tSIRF_MSG_SSB_EE_PROPRIETARY_DEBUG *msg = (tSIRF_MSG_SSB_EE_PROPRIETARY_DEBUG*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "232,255,%lu",
                  msg->debug_flags );
            break;
         }

         case SIRF_MSG_SSB_POS_RSP: /* 0x4501 (69, 1)*/
         {
            tSIRF_MSG_SSB_POSITION_RSP *msg = (tSIRF_MSG_SSB_POSITION_RSP*)message_structure;
            int i;

            snprintf(szBuf, sizeof(szBuf), "69,1,%u,%u,%u,%u,%u,%u,%u,%lu,%ld,%ld,%u,%u,%u,%u,%u,%u,%u,%u,%d,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",
               msg->pos_req_id, msg->pos_results_flag, msg->pos_err_status, msg->qos_pass_flag,
               msg->pos_type, msg->dgps_cor, msg->gps_week, msg->time_of_week,
               msg->lat, msg->lon, msg->other_sections, msg->hor_err_angle, msg->hor_err_major,
               msg->hor_err_minor, msg->vert_pos_height, msg->vert_pos_std, msg->velocity_horizontal,
               msg->velocity_heading, msg->velocity_vertical, msg->velocity_hor_err_ang,
               msg->velocity_hor_err_maj_std, msg->velocity_hor_err_min_std, msg->velocity_ver_err,
               msg->clk_cor_time_ref, msg->clk_cor_clk_bias, msg->clk_cor_clk_drift,
               msg->clk_cor_clk_std_err, msg->clk_cor_utc_off, msg->pos_corr_num_sat);
            for(i = 0; i < SIRF_MSG_SSB_RSP_NUM_SV_INFO; i++)
            {
               snprintf(szFoo, sizeof(szFoo), ",%u,%u,%u",
                  msg->pos_corr_sv_info[i].sv_prn,
                  msg->pos_corr_sv_info[i].c_n0,
                  msg->pos_corr_sv_info[i].inv_weights);
               strlcat(szBuf, szFoo, sizeof(szBuf));
            }
            break;
         }

         case SIRF_MSG_SSB_MEAS_RSP: /* 0x4502 (69, 2)*/
         {
            tSIRF_MSG_SSB_MEAS_RSP *msg = (tSIRF_MSG_SSB_MEAS_RSP*) message_structure;
            int i;

            if(msg->num_sv > SIRF_MSG_SSB_MEASUREMENT_DATA_MAX_SVS)
            {
               tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
            }
            else
            {
               snprintf(szBuf, sizeof(szBuf), "69,2,%u,%u,%u,%u,%lu,%u,%u",
                  msg->pos_req_id, msg->gps_meas_flag, msg->meas_error_status,
                  msg->meas_gps_week, msg->meas_gps_seconds,
                  msg->time_accuracy, msg->num_sv);
               for(i = 0; i < msg->num_sv; i++)
               {
                  snprintf(szFoo, sizeof(szFoo), ",%u,%u,%d,%u,%u,%u,%u",
                     msg->meas_per_sv_data[i].sv_prn,
                     msg->meas_per_sv_data[i].c_n0,
                     msg->meas_per_sv_data[i].sv_doppler,
                     msg->meas_per_sv_data[i].sv_code_phase_wh,
                     msg->meas_per_sv_data[i].sv_code_phase_fr,
                     msg->meas_per_sv_data[i].multipath_indicator,
                     msg->meas_per_sv_data[i].pseudorange_rms_error);
                  strlcat(szBuf, szFoo, sizeof(szBuf));
               }
            }
            break;
         }

         case SIRF_MSG_SSB_APRX_MS_POSITION_RSP: /* 0xD701 (215, 1) */
         {
            tSIRF_MSG_SSB_APRX_MS_POSITION_RSP *msg = (tSIRF_MSG_SSB_APRX_MS_POSITION_RSP *) message_structure;
            snprintf(szBuf, sizeof(szBuf), "215,1,%ld,%ld,%u,%u,%u,%u",
               msg->lat, msg->lon, msg->alt,
               msg->est_hor_err, msg->est_ver_err, msg->use_alt_aiding);
            break;
         }

         case SIRF_MSG_SSB_TIME_TX_RSP: /* 0xD702 (215, 2) */
         {
            tSIRF_MSG_SSB_TIME_TRANSFER_RSP *msg = (tSIRF_MSG_SSB_TIME_TRANSFER_RSP *) message_structure;
            snprintf(szBuf, sizeof(szBuf), "215,2,%u,%u,%u,%lu,%ld,%u",
               msg->tt_type, msg->week_num,
               msg->time_high, msg->time_low,
               msg->delta_utc, msg->time_accuracy);
            break;
         }

         case SIRF_MSG_SSB_FREQ_TX_RSP: /* 0xD703 (215, 3) */
         {
            tSIRF_MSG_SSB_FREQ_TRANSFER_RSP *msg = (tSIRF_MSG_SSB_FREQ_TRANSFER_RSP *) message_structure;
            snprintf(szBuf, sizeof(szBuf), "215,3,%d,%u,%lu,%u,%u,%lu",
               msg->freq_offset, msg->accuracy, msg->time_tag,
               msg->clock_ref, msg->nominal_freq_high, msg->nominal_freq_low);
            break;
         }

         case SIRF_MSG_SSB_SIRF_STATS:          /* 0xE106 (225, 6) */
         {
            tSIRF_MSG_SSB_SIRF_STATS *msg = (tSIRF_MSG_SSB_SIRF_STATS *) message_structure;
            snprintf(szBuf, sizeof(szBuf), "225,6,%u,%u,%u,%ld,%ld,%ld,%ld,%d,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",
               msg->ttff_since_reset, msg->ttff_since_aiding, msg->ttff_first_nav,
               msg->pos_aiding_error_north, msg->pos_aiding_error_east, msg->pos_aiding_error_down,
               msg->time_aiding_error, msg->freq_aiding_error, msg->hor_pos_uncertainty, msg->ver_pos_uncertainty,
               msg->time_uncertainty, msg->freq_uncertainty, msg->num_aided_ephemeris, msg->num_aided_acq_assist,
               msg->nav_mode, msg->pos_mode, msg->nav_status, msg->start_mode, msg->aiding_status);
            break;
         }

         case SIRF_MSG_SSB_SIRF_STATS_AGPS:     /* 0xE107 (225, 7) */
         {
            tSIRF_MSG_SSB_SIRF_STATS_AGPS *msg = (tSIRF_MSG_SSB_SIRF_STATS_AGPS *) message_structure;
            snprintf(szBuf, sizeof(szBuf), "225,7,%u,%u,%u,%ld,%ld,%ld,%ld,%d,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%lu,%lu",
               msg->stats.ttff_since_reset, msg->stats.ttff_since_aiding, msg->stats.ttff_first_nav,
               msg->stats.pos_aiding_error_north, msg->stats.pos_aiding_error_east, msg->stats.pos_aiding_error_down,
               msg->stats.time_aiding_error, msg->stats.freq_aiding_error,
               msg->stats.hor_pos_uncertainty, msg->stats.ver_pos_uncertainty,
               msg->stats.time_uncertainty, msg->stats.freq_uncertainty,
               msg->stats.num_aided_ephemeris, msg->stats.num_aided_acq_assist,
               msg->stats.nav_mode, msg->stats.pos_mode, msg->stats.nav_status,
               msg->stats.start_mode, msg->stats.aiding_status,
               msg->clock_drift, msg->reserved);
            break;
         }

         case SIRF_MSG_SSB_SENSOR_CONFIG:       /* 0xEA01 (234,1) */
         {
            tSIRF_MSG_SSB_SENSOR_CONFIG *msg = (tSIRF_MSG_SSB_SENSOR_CONFIG*) message_structure;
            tSIRF_UINT32 i=0, j=0;

            snprintf(szBuf, sizeof(szBuf), "234,1,%u,%u",
                                        msg->numSensors,
                                        msg->i2cSpeed);

            for(i = 0; (i < msg->numSensors) && (i < SIRF_MSG_SSB_MAX_NUM_SENSORS); i++)
            {
               snprintf(szFoo, sizeof(szFoo), ",%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",
                                      msg->Sensors[i].i2cAddress,msg->Sensors[i].sensorType,msg->Sensors[i].initTime,
                                      msg->Sensors[i].nBytesResol,msg->Sensors[i].sampRate,msg->Sensors[i].sendRate,msg->Sensors[i].decmMethod,
                                      msg->Sensors[i].acqTime,msg->Sensors[i].numReadReg,msg->Sensors[i].measState);
               strlcat(szBuf, szFoo, sizeof(szBuf));

               for(j=0;j<msg->Sensors[i].numReadReg;j++)
                 {
                    snprintf(szFoo, sizeof(szFoo), ",%u,%u", msg->Sensors[i].sensorReadReg[j].readOprMethod,
                                                                               msg->Sensors[i].sensorReadReg[j].dataReadReg);
                    strlcat(szBuf, szFoo, sizeof(szBuf));
                 }

               snprintf(szFoo, sizeof(szFoo), ",%u,%u,%u,%u", msg->Sensors[i].pwrCtrlReg,msg->Sensors[i].pwrOffSetting,
                                   msg->Sensors[i].pwrOnSetting,msg->Sensors[i].numInitReadReg);

               strlcat(szBuf, szFoo, sizeof(szBuf));

               for(j=0;j<msg->Sensors[i].numInitReadReg;j++)
               {
                    snprintf(szFoo, sizeof(szFoo), ",%u,%u", msg->Sensors[i].sensorInitReg[j].address,
                                                                               msg->Sensors[i].sensorInitReg[j].nBytes);
                    strlcat(szBuf, szFoo, sizeof(szBuf));
               }

               snprintf(szFoo, sizeof(szFoo), ",%u,%u", msg->Sensors[i].numCtrlReg,msg->Sensors[i].ctrlRegWriteDelay);
               strlcat(szBuf, szFoo, sizeof(szBuf));

               for(j=0;j<msg->Sensors[i].numCtrlReg;j++)
               {
                    snprintf(szFoo, sizeof(szFoo), ",%u,%u", msg->Sensors[i].sensorCtrlReg[j].address,
                                                                               msg->Sensors[i].sensorCtrlReg[j].value);
                    strlcat(szBuf, szFoo, sizeof(szBuf));
               }
            } /* for loop = numSensors */

            snprintf(szFoo, sizeof(szFoo), ",%u", msg->processingRate);
            strlcat(szBuf, szFoo, sizeof(szBuf));

            for(i=0;i<msg->numSensors;i++)
            {
                    snprintf(szFoo, sizeof(szFoo), ",%u,%u", msg->sensorScaleZeroPointVal[i].zeroPointVal,
                                                                              msg->sensorScaleZeroPointVal[i].scaleFactor);
                    strlcat(szBuf, szFoo, sizeof(szBuf));
            }
            break;
         }

         case SIRF_MSG_SSB_SENSOR_SWITCH:    /* 0xEA02 (234,2) */
         {
           tSIRF_MSG_SSB_SENSOR_SWITCH *msg = (tSIRF_MSG_SSB_SENSOR_SWITCH*) message_structure;

           snprintf(szBuf, sizeof(szBuf), "234,2,%u",
                                          msg->sensorSetState);
           break;
         }

         case SIRF_MSG_SSB_SENSOR_READINGS:     /* 0x4801, (72,1) */
         {
            tSIRF_MSG_SSB_SENSOR_READINGS *msg = (tSIRF_MSG_SSB_SENSOR_READINGS*) message_structure;
            tSIRF_UINT32 i = 0, j =0;

            snprintf(szBuf, sizeof(szBuf), "72,1,%u,%u,%u,%u",
                                        msg->sensorID,
                                        msg->dataLength,
                                        msg->numDataSet,
                                        msg->dataMode);

            for(i = 0; i < msg->numDataSet; i++)
              {
                 snprintf(szFoo, sizeof(szFoo), ",%lu", msg->dataSet[i].timeTag);
                 strlcat(szBuf, szFoo, sizeof(szBuf));
                   for(j = 0; j < msg->dataLength; j++)
                   {
                       snprintf(szFoo, sizeof(szFoo), ",%u", msg->dataSet[i].data[j]);
                       strlcat(szBuf, szFoo, sizeof(szBuf));
                   }
               }
            break;
         }

         case SIRF_MSG_SSB_SENSOR_FACTORY_STORED_PARAMS: /* 0x4802, (72,2) */
         {
            tSIRF_MSG_SSB_SENSOR_FACTORY_STORED_PARAMS *msg
                                = (tSIRF_MSG_SSB_SENSOR_FACTORY_STORED_PARAMS*) message_structure;
            tSIRF_UINT32 i=0, j=0;

            snprintf(szBuf, sizeof(szBuf), "72,2,%u,%u",
                                        msg->sensorID,
                                        msg->numInitReadReg);

            for(i=0;i<msg->numInitReadReg;i++)
              {
                 snprintf(szFoo, sizeof(szFoo), ",%u", msg->initData[i].nBytes);
                 strlcat(szBuf, szFoo, sizeof(szBuf));

                 for(j=0;j<msg->initData[i].nBytes;j++)
                  {
                    snprintf(szFoo, sizeof(szFoo), ",%u", msg->initData[i].data[j]);
                    strlcat(szBuf, szFoo, sizeof(szBuf));
                  }
               } /* for */
            break;
         }


         case SIRF_MSG_SSB_RCVR_STATE:    /* 0x4803, (72,3) */
         {
            tSIRF_MSG_SBB_RCVR_STATE *msg = (tSIRF_MSG_SBB_RCVR_STATE*) message_structure;

            snprintf(szBuf, sizeof(szBuf), "72,3,%lu,%u",
                                         msg->timeStamp,msg->rcvrPhysicalState);
            break;
         }

         case SIRF_MSG_SSB_POINT_N_TELL_OUTPUT:    /* 0x4804, (72,4) */
         {
            tSIRF_MSG_SSB_POINT_N_TELL_OUTPUT *msg = (tSIRF_MSG_SSB_POINT_N_TELL_OUTPUT*) message_structure;

            snprintf(szBuf, sizeof(szBuf), "72,4,%lu,%ld,%ld,%u,%hd,%hd,%u,%u,%u,%u",
                                         msg->timeStamp,msg->latitude,msg->longitude,msg->heading,msg->pitch,
                                         msg->roll,msg->headingUnc,msg->pitchUnc,msg->rollUnc,msg->status);
            break;
         }

         case SIRF_MSG_SSB_SENSOR_CALIBRATION_OUTPUT:    /* 0x4805, (72,5) */
         {
            tSIRF_MSG_SSB_SENSOR_CALIBRATION_OUTPUT *msg = (tSIRF_MSG_SSB_SENSOR_CALIBRATION_OUTPUT*) message_structure;
            tSIRF_UINT32 i=0;

            snprintf(szBuf, sizeof(szBuf), "72,5,%u,%u,%lu",
                                         msg->msgDescriptor,msg->sensorType,msg->timeStamp);

            for(i =0;i<6;i++)
            {
               snprintf(szFoo, sizeof(szFoo), ",%.10e", msg->CAL_FLD8[i]);
               strlcat(szBuf, szFoo, sizeof(szBuf));
            }

            for(i =0;i<24;i++)
            {
               snprintf(szFoo, sizeof(szFoo), ",%.10e", msg->CAL_FLD4[i]);
               strlcat(szBuf, szFoo, sizeof(szBuf));
            }
            break;
         }

         case SIRF_MSG_SSB_MAG_MODEL_PARAMS:    /* 0x4806, (72,6) */
         {
            tSIRF_MSG_SSB_MAG_MODEL_PARAMS *msg = (tSIRF_MSG_SSB_MAG_MODEL_PARAMS*) message_structure;

            snprintf(szBuf, sizeof(szBuf), "72,6,%u,%ld,%ld,%ld,%u,%u,%u,%.10e,%.10e,%.10e,%.10e,%.10e,%.10e,%.10e",
                                         msg->magModelParamSrc,msg->latitude,msg->longitude,
                                         msg->altitude,msg->year,msg->month,msg->day,msg->declination,
                                         msg->totalField,msg->horizonField,msg->inclination,msg->northComp,
                                         msg->eastComp,msg->downComp );
            break;
         }

         case SIRF_MSG_SSB_SESSION_CONTROL_REQ: /* 0xD500 (213, 0) */
         case SIRF_MSG_SSB_SESSION_OPEN_REQ:    /* 0xD501 (213, 1) */
         case SIRF_MSG_SSB_SESSION_CLOSE_REQ:   /* 0xD502 (213, 2) */

         case SIRF_MSG_SSB_SESSION_CONTROL_RSP: /* 0x4A00 (74, 0) */
         case SIRF_MSG_SSB_SESSION_OPEN_RSP:    /* 0x4A01 (74, 1) */
         case SIRF_MSG_SSB_SESSION_CLOSE_RSP:   /* 0x4A02 (74, 2) */

         case SIRF_MSG_SSB_AIDING_REQ:          /* 0x4900 (73, 0) */
         case SIRF_MSG_SSB_APROX_MS_POS_REQ:    /* 0x4901 (73, 1) */
         case SIRF_MSG_SSB_TIME_TX_REQ:         /* 0x4902 (73, 2) */
         case SIRF_MSG_SSB_FREQ_TX_REQ:         /* 0x4903 (73, 3) */
         case SIRF_MSG_SSB_NBA_REQ:             /* 0x4904 (73, 4) */

         case SIRF_MSG_SSB_AIDING_RSP:          /* 0xD700 (215, 0) */
         case SIRF_MSG_SSB_SET_NBA_SF1_2_3:     /* 0xD704 (215, 4) */
         case SIRF_MSG_SSB_SET_NBA_SF4_5:       /* 0xD705 (215, 5) */

         case SIRF_MSG_SSB_ACK_IN:              /* 0xD800 (216, 0) */
         case SIRF_MSG_SSB_ACK_NACK_ERROR_IN:   /* 0xD801 (216, 1) */
         case SIRF_MSG_SSB_REJECT_IN:           /* 0xD802 (216, 2) */

         case SIRF_MSG_SSB_ACK_OUT:             /* 0x4B00 (75, 0) */
         case SIRF_MSG_SSB_ACK_NACK_ERROR_OUT:  /* 0x4B01 (75, 1) */
         case SIRF_MSG_SSB_REJECT_OUT:          /* 0x4B02 (75, 2) */

         case SIRF_MSG_SSB_PWR_MODE_FPM_REQ:    /* 0xDA00 (218, 0) */
         case SIRF_MSG_SSB_PWR_MODE_APM_REQ:    /* 0xDA01 (218, 1) */
         case SIRF_MSG_SSB_PWR_MODE_MP_REQ:     /* 0xDA02 (218, 2) */
         case SIRF_MSG_SSB_PWR_MODE_TP_REQ:     /* 0xDA03 (218, 3) */
         case SIRF_MSG_SSB_PWR_MODE_PTF_REQ:    /* 0xDA04 (218, 4) */

         case SIRF_MSG_SSB_SIRF_INTERNAL_OUT:   /* 0xE100 (225, 0) */
         {
            if(message_length > SIRF_MSG_SSB_MAX_MESSAGE_LEN)
            {
               tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
            }
            else
            {
               snprintf( szBuf, sizeof(szBuf), "%ld,%02X",
                  (unsigned long)SIRF_GET_MID(message_id), SIRF_GET_SUB_ID(message_id));
               for ( i = 0; i <= message_length - 1; i++ )
               {
                  snprintf( szFoo, sizeof(szFoo), ",%02X", ((tSIRF_UINT8*)message_structure)[i] );
                  strlcat( szBuf, szFoo, sizeof(szBuf) );
               }
            }
            break;
         }

         case SIRF_MSG_SSB_TEXT: /* 0xFF (255) */
         {
            tSIRF_CHAR *anStoredDataPacket = ((tSIRF_NAV_TEXT*)message_structure)->msg_text;
            tSIRF_UINT32 cbStoredDataPacket = (tSIRF_UINT32)strlen( anStoredDataPacket );

            if(message_length > SIRF_MSG_SSB_MAX_MESSAGE_LEN)
            {
               tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
            }
            else
            {
               snprintf( szBuf, sizeof(szBuf), "%*s", (int)cbStoredDataPacket, anStoredDataPacket );
               /**
               * message may start with \r\n and end with \r\n
               * remove these if existing, replace with space ' '
               */
               for( i = 0; i < cbStoredDataPacket; i++ )
               {
                  if( ('\r' == szBuf[i]) || ('\n' == szBuf[i]) )
                  {
                     szBuf[i] = ' ';
                  }
               }
               szBuf[cbStoredDataPacket] = '\0';
            }
            break;
         }

         default: /* Other message: dump in hex */
         {
            if(message_length > SIRF_MSG_SSB_MAX_MESSAGE_LEN)
            {
               tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
            }
            else
            {
               snprintf( szBuf, sizeof(szBuf), "%ld",
                  (unsigned long)SIRF_GET_MID(message_id));
               for ( i = 0; i <= message_length - 1; i++ )
               {
                  snprintf( szFoo, sizeof(szFoo), ",%02X", ((tSIRF_UINT8*)message_structure)[i] );
                  strlcat( szBuf, szFoo, sizeof(szBuf) );
               }
            }
            break;
         }
      }

      if(SIRF_SUCCESS == tRet)
      {
         /* Check to make sure that the length of the buffer we are writing into is big enough.
         Account for the line terminator bytes. */
         if ( *lpszText_length < (strlen(szBuf) - 2) )
         {
            tRet = SIRF_CODEC_ERROR_INVALID_MSG_LENGTH;
         }
         else
         {
            /* Copy the data to our output buffer */
            strncpy( (char *)lpszText, (const char *)szBuf, *lpszText_length );

            /* Add line terminator */
            strlcat((char *)lpszText, "\x0D\x0A", *lpszText_length);

            /* Calculate string length */
            *lpszText_length = (tSIRF_UINT32)strlen((char*)lpszText);
         }
      }
      else
      {
         *lpszText_length = 0;
      }

   } /* if check on input parameters */
   else
   {
      tRet = SIRF_CODEC_ERROR_INVALID_PARAMETER;
   }

   return tRet;

} /* SIRF_CODEC_ASCII_Encode() */


/*=============================================================================

  -----------------------------------------------------------------------------*/
#if 0
tSIRF_RESULT SIRF_CODEC_ASCII_Decode( tSIRF_UINT8  *linestr,
                                      tSIRF_UINT32  line_length,
                                      tSIRF_UINT32 *message_id,
                                      tSIRF_VOID   *message_structure,
                                      tSIRF_UINT32 *message_length,
                                      tSIRF_UINT32 *options )
{
   tSIRF_CHAR   *ptr = linestr;
   tSIRF_CHAR   *c;
   tSIRF_UINT32  items_read;

   *message_length = 0;

   /* Decode message id */
   c = strchr( ptr, ',' );
   if ( c )
   {
      while ( ptr != c )
      {
         if ( isdigit( *ptr ) )
         {
            ptr++;
         }
         else
         {
            return SIRF_FAILURE; /* Characters before comma are not number */
         }
      }
   }
   else
   {
      return SIRF_FAILURE; /* Comma has not been found */
   }

   items_read = sscanf( linestr, "%15lu,", message_id );
   if ( items_read != 1 )
   {
      return SIRF_FAILURE;
   }

   /* Decode message */
   switch( *message_id )
   {
      case SIRF_GET_MID( SIRF_MSG_SSB_SIRFNAV_NOTIFICATION ):
      {
         tSIRF_UINT32 sub_id = 0;

         items_read = sscanf( linestr, "%15lu,%15lu", message_id, &sub_id );
         if ( items_read != 2 )
         {
            return SIRF_FAILURE;
         }

         *message_id |= sub_id << 8;

         switch ( sub_id )
         {
            case SIRF_GET_SUB_ID(SIRF_MSG_SSB_SIRFNAV_START):
            {
               tSIRF_MSG_SSB_SIRFNAV_START *msg = (tSIRF_MSG_SSB_SIRFNAV_START*)message_structure;
               *message_length = sizeof(tSIRF_MSG_SSB_SIRFNAV_START);

               memset( msg, 0, sizeof(tSIRF_MSG_SSB_SIRFNAV_START) );

               items_read = sscanf( (const char *)linestr, "51,7,%15lu,%15lu,%15lu,%15lu",
                                     &msg->start_mode, &msg->clock_offset, &msg->port_num, &msg->baud_rate);
               if ( items_read != 4 )
               {
                  return SIRF_FAILURE;
               }
               break;
            }

            default:
               return SIRF_FAILURE;
         } /* switch ( sub_id ) */
         break;
      }

      case SIRF_GET_MID(SIRF_MSG_SSB_DEMO_START_GPS_ENGINE):
      {
         tSIRF_UINT32 sub_id = 0;

         items_read = sscanf( linestr, "%15lu,%15lu", message_id, &sub_id );
         if ( items_read != 2 )
         {
            return SIRF_FAILURE;
         }

         *message_id |= sub_id << 8;

         switch ( sub_id )
         {
            case SIRF_GET_SUB_ID(SIRF_MSG_SSB_DEMO_START_GPS_ENGINE):
            {
               tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE *msg = (tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE*)message_structure;
               *message_length = sizeof(tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE);

               memset( msg, 0, sizeof(tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE) );

               items_read = sscanf( (const char *)linestr, "161,5,%15lu,%15lu,%15lu,%15lu",
                                    &msg->start_mode, &msg->clock_offset, &msg->port_num, &msg->baud_rate);
               if ( items_read != 4 )
               {
                  return SIRF_FAILURE;
               }
               break;
            }

            default:
               return SIRF_FAILURE;
         } /* switch ( sub_id ) */
         break;
      }
   } /* switch ( message_id ) */

   *message_id |= SIRF_LC_SSB << 16;

   return SIRF_SUCCESS;

} /* SIRF_CODEC_ASCII_Decode() */
#endif

/**
 * @}
 */

