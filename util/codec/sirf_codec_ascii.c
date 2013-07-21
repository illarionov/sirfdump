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
         snprintf( pBuf, size, "UART buffer allocation error: %lu", (unsigned long)pError->param[0] );
         break;
      case SIRF_MSG_SSB_ERRID_MI_UPDATETIMEFAILURE:
         /* "One second task not complete on entry #%ld (%ld tracker ms errors)";*/
         snprintf( pBuf, size, "OSOR #%ld (%ld ms)", (long)pError->param[0], (long)pError->param[1] );
         break;
      case SIRF_MSG_SSB_ERRID_MI_VCOCLOCKLOST:
         snprintf( pBuf, size, "VCO lock lost in %lu msec",(unsigned long) pError->param[0] );
         break;
      case SIRF_MSG_SSB_ERRID_MI_MEMORYTESTFAILED:
         snprintf( pBuf, size, "Memory test failed at 0x%lx", (unsigned long)pError->param[0] );
         break;
      case SIRF_MSG_SSB_ERRID_MI_FALSEACQRECEIVERRESET:
         snprintf( pBuf, size, "Assuming false acquisition and reseting receiver at %ld", (long)pError->param[0] );
         break;
      case SIRF_MSG_SSB_ERRID_KFC_KILLCHANNEL:
         snprintf( pBuf, size, "Kalman Filter @%ld: Kill chan %02ld, range res=%ld",
                   (long)pError->param[0], (long)pError->param[1], (long)pError->param[2] );
         break;
      case SIRF_MSG_SSB_ERRID_KFC_BACKUPFAILED_VELOCITY:
         snprintf( pBuf, size, "SRAM backup not done: Velocity sum exceeds maximum." );
         break;
      case SIRF_MSG_SSB_ERRID_KFC_BACKUPFAILED_NUMSV:
         snprintf( pBuf, size, "SRAM backup not done: Too few SVs used in a solution." );
         break;
      case SIRF_MSG_SSB_ERRID_KFS_BADALTITUDE:
         snprintf( pBuf, size, "KFDBG @%ld: Bad altitude = %ld", (long)pError->param[0], (long)pError->param[1] );
         break;
      case SIRF_MSG_SSB_ERRID_KRS_BADSOLUTION:
         snprintf( pBuf, size, "Krause solution poor:\n POS = %ld %ld %ld\n PR = %ld %ld",
                   (long)pError->param[0], (long)pError->param[1], (long)pError->param[2], (long)pError->param[3], (long)pError->param[4] );
         break;
      case SIRF_MSG_SSB_ERRID_RMC_GETTINGPOSITION:
         snprintf( pBuf, size, "RxM error in getting SV %02ld's position", (long)pError->param[0] );
         break;
      case SIRF_MSG_SSB_ERRID_RXM_TIMEEXCEEDED:
         snprintf( pBuf, size, "RxM: Pseudo range in seconds exceeds limit: %ld", (long)pError->param[0] );
         break;
      case SIRF_MSG_SSB_ERRID_RXM_TDOPOVERFLOW:
         snprintf( pBuf, size, "RxM error: tdop=%ld overflow", (long)pError->param[0] );
         break;
      case SIRF_MSG_SSB_ERRID_RXM_VALIDDURATIONEXCEEDED:
         snprintf( pBuf, size, "RxM: Ephemeris for SV %02ld exceeds valid time duration by %ld sec",
                   (long)pError->param[0], (long)pError->param[1] );
         break;
      case SIRF_MSG_SSB_ERRID_STRTP_SRAMCKSUM:
         snprintf( pBuf, size, "SRAM control flags contain bad checksum" );
         break;
      case SIRF_MSG_SSB_ERRID_STRTP_RTCTIMEINVALID:
         snprintf( pBuf, size, "RTC time or SRAM backup invalid, TOW:%ld, WNUM:%ld",
                   (long)pError->param[0], (long)pError->param[1] );
         break;
      case SIRF_MSG_SSB_ERRID_STRTP_BADPOSTION:
         snprintf( pBuf, size, "SRAM: Bad position during startup, x:%ld y:%ld z:%ld",
                   (long)pError->param[0], (long)pError->param[1], (long)pError->param[2] );
         break;
      case SIRF_MSG_SSB_ERRID_CS_SVPARITY:
         snprintf( pBuf, size, "Bad parity from SV %02ld, subframe %ld", (long)pError->param[0], (long)pError->param[1] );
         break;
      default:
         snprintf( pBuf, size, "Error in unknown format: err_id=%lx params: %lx %lx %lx %lx",
                   (unsigned long)pError->param[0], (unsigned long)pError->param[1], (unsigned long)pError->param[2], (unsigned long)pError->param[3], (unsigned long)pError->param[4] );
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
                     (long)NavData->ecef_x,
                     (long)NavData->ecef_y,
                     (long)NavData->ecef_z,
                     NavData->ecef_vel_x * SIRF_MSG_SSB_VELOCITY_LSB,
                     NavData->ecef_vel_y * SIRF_MSG_SSB_VELOCITY_LSB,
                     NavData->ecef_vel_z * SIRF_MSG_SSB_VELOCITY_LSB,
                     (unsigned)NavData->nav_mode,
                     NavData->hdop * SIRF_MSG_SSB_DOP_LSB,
                     (unsigned)NavData->nav_mode2,
                     (unsigned)NavData->gps_week,
                     (unsigned long)NavData->gps_tow,
                     (unsigned)NavData->sv_used_cnt );
            for( i = 0; i < SIRF_NUM_CHANNELS; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), ",%u", (unsigned)NavData->sv_used[i] );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            break;
         }

         case SIRF_MSG_SSB_MEASURED_TRACKER:  /* 0x04 (4) */
         {
            tSIRF_MSG_SSB_MEASURED_TRACKER *TrkData = (tSIRF_MSG_SSB_MEASURED_TRACKER*)message_structure;
            snprintf( (char*)szBuf, sizeof(szBuf), "4,%d,%lu,%u",
                     (int)TrkData->gps_week,
                     (unsigned long)TrkData->gps_tow,
                     (unsigned)TrkData->chnl_cnt );
            for( i = 0; i < SIRF_NUM_CHANNELS; i++ )
            {
               snprintf( (char*)szFoo,  sizeof(szFoo),",%u,%u,%u,%u",
                        (unsigned)TrkData->chnl[i].svid,
                        (unsigned) (TrkData->chnl[i].azimuth * SIRF_MSG_SSB_AZIMUTH_LSB),
                        (unsigned) (TrkData->chnl[i].elevation * SIRF_MSG_SSB_ELEVATION_LSB),
                        (unsigned)TrkData->chnl[i].state );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
               for( j = 0; j < SIRF_NUM_POINTS; j++ )
               {
                  snprintf( szFoo,  sizeof(szFoo), ",%u", (unsigned)TrkData->chnl[i].cno[j] );
                  strlcat( szBuf, szFoo, sizeof(szBuf) );
               }
            }
            break;
         }

         case SIRF_MSG_SSB_RAW_TRACKER: /* 0x05 (5) */
         {
            tSIRF_MSG_SSB_RAW_TRACKER *RawTrk = (tSIRF_MSG_SSB_RAW_TRACKER*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "5,%ld,%d,%02x,%ld,%d,%d,%ld,%ld,%ld,%ld,%d",
                     (long)RawTrk->channel,
                     (int)RawTrk->svid,
                     (unsigned)RawTrk->state,
                     (unsigned long)RawTrk->bit_number,
                     (int)RawTrk->msec_number,
                     (int)RawTrk->chip_number,
                     (long)RawTrk->code_phase,
                     (long)RawTrk->carrier_doppler,
                     (long)RawTrk->measure_timetag,
                     (long)RawTrk->delta_carrier_phase,
                     (int)RawTrk->search_cnt );
            for( i = 0; i < SIRF_NUM_POINTS; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), ",%u", (unsigned)RawTrk->cno[i] );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            snprintf( szFoo, sizeof(szFoo), ",%u,%u,%d,%d",
                     (unsigned)RawTrk->power_bad_count,
                     (unsigned)RawTrk->phase_bad_count,
                     (int)RawTrk->delta_car_interval,
                     (int)RawTrk->correl_interval );
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
                     (unsigned)ClkStatus->gps_week,
                     (unsigned long)ClkStatus->gps_tow,
                     (unsigned long)ClkStatus->est_gps_time,
                     (unsigned)ClkStatus->sv_used_cnt,
                     (unsigned long)ClkStatus->clk_offset,
                     (unsigned long)ClkStatus->clk_bias );
            break;
         }

         case SIRF_MSG_SSB_50BPS_DATA:             /* 0x08 */
         case SIRF_MSG_SSB_EE_50BPS_DATA_VERIFIED: /* 0x38, 0x05 */
         {
            tSIRF_MSG_SSB_50BPS_DATA *data50bps = (tSIRF_MSG_SSB_50BPS_DATA*)message_structure;

            if( SIRF_MSG_SSB_EE_50BPS_DATA_VERIFIED == message_id )
            {
               snprintf( szBuf, sizeof(szBuf), "56,5,%02u %02u ",
                               (unsigned)data50bps->chnl, (unsigned)data50bps->svid );
            }
            else
            {
               snprintf( szBuf, sizeof(szBuf), ": %02u %02u ",
                               (unsigned)data50bps->chnl, (unsigned)data50bps->svid );
            }

            for( i = 0; i < 10; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), "%08lx ", (unsigned long)data50bps->word[i] );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            break;
         }

         case SIRF_MSG_SSB_THROUGHPUT: /* 0x09 (9) */
         {
            tSIRF_MSG_SSB_THROUGHPUT *Throughput = (tSIRF_MSG_SSB_THROUGHPUT*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "ThrPut(186=1ms): Latency:%03u SegStatMax:%03u "
                               "AveTrkTime:%03u Nav Complete:%04u ms",
                     (unsigned)Throughput->seg_stat_lat,
                     (unsigned)Throughput->seg_stat_max,
                     (unsigned)Throughput->avg_trk_time,
                     (unsigned)Throughput->last_ms );
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
            snprintf( szBuf, sizeof(szBuf), "Ack: 0x%02X 0x%02X", (unsigned)(((tSIRF_MSG_SSB_ACK*)message_structure)->msg_id),
                     (unsigned)(((tSIRF_MSG_SSB_ACK*)message_structure)->sub_id ));
            break;
         }

         case SIRF_MSG_SSB_NAK: /* 0x0C (12) */
         {
            snprintf( szBuf, sizeof(szBuf), "Nak: 0x%02X 0x%02X", (unsigned)(((tSIRF_MSG_SSB_NAK*)message_structure)->msg_id),
                     (unsigned)(((tSIRF_MSG_SSB_NAK*)message_structure)->sub_id ));
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
                                  (unsigned)VisList->visible[i].svid,
                                  (int)VisList->visible[i].azimuth,
                                  (int)VisList->visible[i].elevation );
                  strlcat( szBuf, szFoo, sizeof(szBuf) );
               }
            }
            break;
         }

         case SIRF_MSG_SSB_ALMANAC: /* 0x0E (14) */
         {
            tSIRF_MSG_SSB_ALMANAC * Alm = (tSIRF_MSG_SSB_ALMANAC*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "ALMANAC SVID:%02u  ", (unsigned)Alm->svid );
            for( i = 0; i < SIRF_MSG_SSB_ALMANAC_ENTRIES; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), "%d, ", (int)Alm->data[i] );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            break;
         }

         case SIRF_MSG_SSB_EPHEMERIS: /* 0x0F (15) */
         {
            tSIRF_MSG_SSB_EPHEMERIS * Eph = (tSIRF_MSG_SSB_EPHEMERIS*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "EPHEMERIS SVID:%02u  ", (unsigned)Eph->svid );
            for( i = 0; i < 3; i++ )
            {
               for ( j = 0; j < 15; j++ )
               {
                  snprintf( szFoo, sizeof(szFoo), "%u, ", (unsigned)Eph->subframe[i][j] );
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
                        (long)RcvrParam->lp_on_time, lp_duty_cycle );
               strlcat( lp_str, tmpStr, sizeof(lp_str) );
            }
            if( RcvrParam->lp_user_tasks_enabled )
            {
               snprintf( tmpStr, sizeof(tmpStr), "User tasks enabled, period = %ld\n",
                        (long)RcvrParam->lp_user_task_interval );
               strlcat( lp_str, tmpStr, sizeof(lp_str) );
            }
            else
            {
               strlcat( lp_str, "User tasks disabled\n", sizeof(lp_str) );
            }
            snprintf( tmpStr, sizeof(tmpStr), "MaxAcqTime = %lu ms; MaxOffTime = %lu ms\n",
                     (unsigned long)RcvrParam->lp_max_acq_time, (unsigned long)RcvrParam->lp_max_off_time );
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
                     (int)RcvrParam->alt_input,
                     strDegMode[RcvrParam->degraded_mode],
                     (unsigned)RcvrParam->degraded_timeout,
                     (unsigned)RcvrParam->dr_timeout,
                     (unsigned)RcvrParam->trk_smooth,
                     strEnabled[RcvrParam->static_nav_mode],
                     strEnabled[RcvrParam->enable_3sv_lsq],
                     strDOPMode[RcvrParam->dop_mask_mode],
                     RcvrParam->nav_elev_mask / 10.0F,
                     (unsigned)RcvrParam->nav_pwr_mask,
                     strDGPSSrc[RcvrParam->dgps_src],
                     strDGPSMode[RcvrParam->dgps_mode],
                     (unsigned)RcvrParam->dgps_timeout,
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
                                 (unsigned)msg->svid,
                                 (unsigned)msg->period,
                                 (unsigned)msg->bit_synch_time,
                                 (unsigned)msg->bit_count,
                                 (unsigned)msg->poor_status_count,
                                 (unsigned)msg->good_status_count,
                                 (unsigned)msg->parity_error_count,
                                 (unsigned)msg->lost_vco_count,
                                 (unsigned)msg->frame_synch_time,
                                 (int)msg->cno_mean,
                                 (int)msg->cno_sigma,
                                 (int)msg->clock_drift,
                                 (long)msg->clock_offset_mean,
                                 (int)msg->bad_1khz_bit_count,
                                 (long)msg->abs_i20ms,
                                 (long)msg->abs_q1ms,
                                 (long)msg->reserved[0],
                                 (long)msg->reserved[1],
                                 (long)msg->reserved[2] );
         }
         break;

         case SIRF_MSG_SSB_DGPS_STATUS: /* 0x1B (27) */
         {
            tSIRF_MSG_SSB_DGPS_STATUS *msg = (tSIRF_MSG_SSB_DGPS_STATUS*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "27,%u", (unsigned)msg->src );

            for( i = 0; i < SIRF_NUM_CHANNELS; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), ",%u", (unsigned)msg->cor_age[i] );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }

            snprintf( szFoo, sizeof(szFoo), ",%u,%u", (unsigned)msg->reserved[0], (unsigned)msg->reserved[1] );
            strlcat( szBuf, szFoo, sizeof(szBuf) );

            for( i = 0; i < SIRF_NUM_CHANNELS; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), ",%u,%d", (unsigned)msg->corrections[i].prn, (unsigned)msg->corrections[i].cor );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }

            break;
         }

         case SIRF_MSG_SSB_NL_MEAS_DATA: /* 0x1C (28) */
         {
            tSIRF_MSG_SSB_NL_MEAS_DATA *NlMeas = (tSIRF_MSG_SSB_NL_MEAS_DATA*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "28,%u,%lu,%u,%.10e,%.10e,%.10e,%.10e,%u,%u",
                     (unsigned)NlMeas->Chnl,
                     (unsigned long)NlMeas->Timetag,
                     (unsigned)NlMeas->svid,
                     NlMeas->gps_sw_time,
                     NlMeas->pseudorange,
                     NlMeas->carrier_freq,
                     NlMeas->carrier_phase,
                     (unsigned)NlMeas->time_in_track,
                     (unsigned)NlMeas->sync_flags);
            for( i = 0; i < SIRF_NUM_POINTS; i++ )
            {
               snprintf( szFoo, sizeof(szFoo), ",%u", (unsigned)NlMeas->cton[i] );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            snprintf( szFoo, sizeof(szFoo), ",%u,%u,%d,%u,%u",
                     (unsigned)NlMeas->delta_range_interval,
                     (unsigned)NlMeas->mean_delta_range_time,
                     (int)NlMeas->extrapolation_time,
                     (unsigned)NlMeas->phase_error_count,
                     (unsigned)NlMeas->low_power_count );
            strlcat( szBuf, szFoo, sizeof(szBuf) );
            break;
         }

         case SIRF_MSG_SSB_NL_DGPS_DATA: /* 0x1D (29) */
         {
            tSIRF_MSG_SSB_NL_DGPS_DATA *NlDgps = (tSIRF_MSG_SSB_NL_DGPS_DATA*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "29,%d,%d,%u,%.10e,%.10e,%.10e,%.10e,%.10e",
                     (int)NlDgps->svid,
                     (int)NlDgps->iod,
                     (unsigned)NlDgps->source,
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
                     (unsigned)NlSvState->svid,
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
                     (unsigned)NlInit->enable_con_alt_mode, (unsigned)NlInit->alt_mode, (unsigned)NlInit->alt_source,
                     NlInit->altitude, (unsigned)NlInit->degraded_mode, (int)NlInit->degraded_timeout,
                     (int)NlInit->dr_timeout, (int)NlInit->coast_timeout, (unsigned)NlInit->tracksmooth_mode,
                     (unsigned)NlInit->dop_selection, (int)NlInit->hdop_thresh, (int)NlInit->gdop_thresh,
                     (int)NlInit->pdop_thresh, (unsigned)NlInit->dgps_selection, (int)NlInit->dgps_timeout,
                     (int)NlInit->elev_nav_mask, (int)NlInit->pow_nav_mask, (unsigned)NlInit->editing_residual_mode,
                     (int)NlInit->editing_residual_threshold, (unsigned)NlInit->ssd_mode, (int)NlInit->ssd_threshold,
                     (unsigned)NlInit->static_nav_mode, (int)NlInit->static_nav_threshold );

            /* From NL_PositionInitStruct */
            snprintf( szFoo, sizeof(szFoo), ",%.10e,%.10e,%.10e,%u",
                     NlInit->ecef_x, NlInit->ecef_y, NlInit->ecef_z, (unsigned)NlInit->position_init_source );
            strlcat( szBuf, szFoo, sizeof(szBuf) );

            /* From NL_TimeInitStruct */
            snprintf( szFoo, sizeof(szFoo), ",%.10e,%d,%u",
                     NlInit->gps_time, (int)NlInit->gps_week, (unsigned)NlInit->time_init_source );
            strlcat( szBuf, szFoo, sizeof(szBuf) );

            /* from NL_ClockDriftInitStruct */
            snprintf( szFoo, sizeof(szFoo), ",%.10e,%u",
                     NlInit->clk_offset, (unsigned)NlInit->clk_offset_init_source );
            strlcat( szBuf, szFoo, sizeof(szBuf) );
            break;
         }

         case SIRF_MSG_SSB_NL_AUX_INIT_DATA: /* 0x40 0x01 */
         {
            tSIRF_MSG_SSB_NL_AUX_INIT_DATA * NlAuxInit = (tSIRF_MSG_SSB_NL_AUX_INIT_DATA*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "64,1,%lu,%u,%lu,%u,%u,%u,%u,%u,%lu,%lu,%lu,%lu",
                      (unsigned long)NlAuxInit->time_init_unc,(unsigned)NlAuxInit->saved_pos_week,(unsigned long)NlAuxInit->saved_pos_tow,
                      (unsigned)NlAuxInit->saved_pos_ehe,(unsigned)NlAuxInit->saved_pos_eve,(unsigned)NlAuxInit->sw_version,(unsigned)NlAuxInit->icd_version,
                      (unsigned)NlAuxInit->chip_version,(unsigned long)NlAuxInit->acq_clk_speed,(unsigned long)NlAuxInit->default_clock_offset,
                      (unsigned long)NlAuxInit->tracker_status,(unsigned long)NlAuxInit->reserved);
            break;
         }

         case SIRF_MSG_SSB_NL_AUX_MEAS_DATA: /* 0x40 0x02 */
         {
            tSIRF_MSG_SSB_NL_AUX_MEAS_DATA * NlAuxMeas = (tSIRF_MSG_SSB_NL_AUX_MEAS_DATA*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "64,2,%u,%u,%u,%u,%lu,%lu,%ld,%ld,%d,%d,%ld,%ld,%ld,%ld,"
                            "%d,%d,%d,%d,%u,%u,%ld,%d,%d,%u,%lu",
                      (unsigned)NlAuxMeas->sv_prn, (unsigned)NlAuxMeas->status, (unsigned)NlAuxMeas->extended_status,(unsigned)NlAuxMeas->bit_sync_qual,
                      (unsigned long)NlAuxMeas->time_tag,(unsigned long)NlAuxMeas->code_phase,(long)NlAuxMeas->carrier_phase,(long)NlAuxMeas->carrier_freq,
                      (int)NlAuxMeas->carrier_accel,(int)NlAuxMeas->ms_num,(long)NlAuxMeas->bit_num,(long)NlAuxMeas->code_correction,
                      (long)NlAuxMeas->smooth_code,(long)NlAuxMeas->code_offset,(int)NlAuxMeas->pseudorange_noise,(int)NlAuxMeas->delta_range_qual,
                      (int)NlAuxMeas->phase_lock_qual,(int)NlAuxMeas->ms_uncertainty,(unsigned)NlAuxMeas->sum_abs_I,(unsigned)NlAuxMeas->sum_abs_Q,
                      (long)NlAuxMeas->sv_bit_num,(int)NlAuxMeas->mp_los_det_value,(int)NlAuxMeas->mp_only_det_value,
                      (unsigned)NlAuxMeas->recovery_status,(unsigned long)NlAuxMeas->sw_time_uncertainty);
            break;
         }

         case SIRF_MSG_SSB_NL_AUX_AID_DATA: /* 0x40 0x03 */
         {
            tSIRF_MSG_SSB_NL_AUX_AID_DATA * NlAuxAid = (tSIRF_MSG_SSB_NL_AUX_AID_DATA*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "64,3,%ld,%ld,%ld,%lu,%u,%lu",
                            (long)NlAuxAid->ecef_x,(long)NlAuxAid->ecef_y,(long)NlAuxAid->ecef_z,(unsigned long)NlAuxAid->horiz_pos_unc,
                            (unsigned)NlAuxAid->alt_unc,(unsigned long)NlAuxAid->sw_tow);
            break;
         }

         case SIRF_MSG_SSB_GEODETIC_NAVIGATION: /* 0x29 (41) */
         {
            tSIRF_MSG_SSB_GEODETIC_NAVIGATION *GeodNavState = (tSIRF_MSG_SSB_GEODETIC_NAVIGATION*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "41,%u,%u,%u,%lu,%u,%u,%u,%u,%u,%u,%lu,%ld,%ld,%ld,%ld,%u,%u,%u,"
                               "%d,%d,%d,%lu,%lu,%lu,%u,%ld,%lu,%ld,%lu,%lu,%u,%u,%u,%u,%u",
                     (unsigned)GeodNavState->nav_valid, (unsigned)GeodNavState->nav_mode,
                     (unsigned)GeodNavState->gps_week, (unsigned long)GeodNavState->gps_tow, (unsigned)GeodNavState->utc_year,
                     (unsigned)GeodNavState->utc_month, (unsigned)GeodNavState->utc_day, (unsigned)GeodNavState->utc_hour,
                     (unsigned)GeodNavState->utc_min, (unsigned)GeodNavState->utc_sec, (unsigned long)GeodNavState->sv_used,
                     (long)GeodNavState->lat, (long)GeodNavState->lon, (long)GeodNavState->alt_ellips,
                     (long)GeodNavState->alt_msl, (unsigned)GeodNavState->datum, (unsigned)GeodNavState->sog,
                     (unsigned)GeodNavState->hdg, (int)GeodNavState->mag_var, (int)GeodNavState->climb_rate,
                     (int)GeodNavState->heading_rate, (unsigned long)GeodNavState->ehpe, (unsigned long)GeodNavState->evpe,
                     (unsigned long)GeodNavState->ete, (unsigned)GeodNavState->ehve, (unsigned long)GeodNavState->clk_bias,
                     (unsigned long)GeodNavState->clk_bias_error, (long)GeodNavState->clk_offset,
                     (unsigned long)GeodNavState->clk_offset_error, (unsigned long)GeodNavState->distance_travelled,
                     (unsigned)GeodNavState->distance_travelled_error, (unsigned)GeodNavState->heading_error, (unsigned)GeodNavState->sv_used_cnt,
                     (unsigned)GeodNavState->hdop, (unsigned)GeodNavState->additional_mode_info );
            break;
         }

         case SIRF_MSG_SSB_ADC_ODOMETER_DATA: /* 0x2D (45) */
         {
            if ( sizeof(tSIRF_MSG_SSB_ADC_ODOMETER_DATA) == message_length )
            {
               tSIRF_MSG_SSB_ADC_ODOMETER_DATA *ADCData10Hz = (tSIRF_MSG_SSB_ADC_ODOMETER_DATA*)message_structure;
               snprintf( szBuf, sizeof(szBuf), "45,%lu,%d,%d,%u,%u",
                  (unsigned long)ADCData10Hz->current_time,
                  (int)ADCData10Hz->adc2_avg,
                  (int)ADCData10Hz->adc3_avg,
                  (unsigned)ADCData10Hz->odo_count,
                  (unsigned)ADCData10Hz->gpio_stat);
            }
            else if ( sizeof(tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ) == message_length )
            {
               tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ *ADCData1Hz = (tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ*)message_structure;
               snprintf( szBuf, sizeof(szBuf), "45" );
               for ( i=0; i<10; i++ )
               {
                  snprintf(szFoo, sizeof(szFoo), ",%lu,%d,%d,%u,%u",
                     (unsigned long)ADCData1Hz->dataset[i].current_time,
                     (int)ADCData1Hz->dataset[i].adc2_avg,
                     (int)ADCData1Hz->dataset[i].adc3_avg,
                     (unsigned)ADCData1Hz->dataset[i].odo_count,
                     (unsigned)ADCData1Hz->dataset[i].gpio_stat);
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
                                 (unsigned)msg->svid,
                                 (unsigned)msg->period,
                                 (unsigned)msg->bit_synch_time,
                                 (unsigned)msg->bit_count,
                                 (unsigned)msg->poor_status_count,
                                 (unsigned)msg->good_status_count,
                                 (unsigned)msg->parity_error_count,
                                 (unsigned)msg->lost_vco_count,
                                 (unsigned)msg->frame_synch_time,
                                 (int)msg->cno_mean,
                                 (int)msg->cno_sigma,
                                 (int)msg->clock_drift,
                                 (long)msg->clock_offset,
                                 (int)msg->bad_1khz_bit_count,
                                 (long)msg->abs_i20ms,
                                 (long)msg->abs_q20ms,
                                 (long)msg->phase_lock,
                                 (unsigned)msg->rtc_frequency,
                                 (unsigned)msg->e_to_acq_ratio,
                                 (unsigned)msg->t_sync_agc_gain,
                                 (unsigned)msg->tm_5_ready,
                                 (unsigned)msg->ClkDriftUnc );
         }
         break;

         case SIRF_MSG_SSB_DR_NAV_STATUS: /* 0x0130 (48, 1) */
         {
            tSIRF_MSG_SSB_DR_NAV_STATUS * msg = (tSIRF_MSG_SSB_DR_NAV_STATUS*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "48,1,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",

                     (unsigned)msg->nav,
                     (unsigned)msg->data,
                     (unsigned)msg->cal_gb_cal,
                     (unsigned)msg->gsf_cal_ssf_cal,
                     (unsigned)msg->nav_across_reset_pos,
                     (unsigned)msg->hd,
                     (unsigned)msg->gyr_sub_op_spd_sub_op,
                     (unsigned)msg->nav_st_int_ran_z_gb_cal_upd,
                     (unsigned)msg->gbsf_cal_upd_spd_cal_upd_upd_nav_st,
                     (unsigned)msg->gps_upd_pos,
                     (unsigned)msg->gps_upd_hd,
                     (unsigned)msg->gps_pos_gps_vel,
                     (unsigned)msg->dws_hd_rt_s_f_cal_valid,
                     (unsigned)msg->dws_hd_rt_s_f_cal_upd,
                     (unsigned)msg->dws_spd_s_f_cal_valid,
                     (unsigned)msg->dws_spd_s_f_cal_upd );
            break;
         }

         case SIRF_MSG_SSB_DR_NAV_STATE: /* 0x0230 (48, 2) */
         {
            tSIRF_MSG_SSB_DR_NAV_STATE * msg = (tSIRF_MSG_SSB_DR_NAV_STATE*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "48,2,%u,%u,%d,%u,%d,%u,%d,%u,%d,%u,%lu,%u,%u,%u,%u, %u,%u,%d,%u,%d,%u,%d,%u,%d,%u,%d,%u,%d,%u,%u,%d,%u,%d,%u,%u,%d,%u",
                     (unsigned)msg->spd,
                     (unsigned)msg->spd_e,
                     (int)msg->ssf,
                     (unsigned)msg->ssf_e,
                     (int)msg->hd_rte,
                     (unsigned)msg->hd_rte_e,
                     (int)msg->gb,
                     (unsigned)msg->gbE,
                     (int)msg->gsf,
                     (unsigned)msg->gsf_e,
                     (unsigned long)msg->tpe,
                     (unsigned)msg->the,
                     (unsigned)msg->nav_ctrl,
                     (unsigned)msg->reverse,
                     (unsigned)msg->hd,
                     (unsigned)msg->sensor_pkg,
                     (unsigned)msg->odo_spd,
                     (int)msg->odo_spd_s_f,
                     (unsigned)msg->odo_spd_s_f_err,
                     (int)msg->lf_wheel_spd_sf,
                     (unsigned)msg->lf_wheel_spd_sf_err,
                     (int)msg->rf_wheel_spd_sf,
                     (unsigned)msg->rf_wheel_spd_sf_err,
                     (int)msg->lr_wheel_spd_sf,
                     (unsigned)msg->lr_wheel_spd_sf_err,
                     (int)msg->rr_wheel_spd_sf,
                     (unsigned)msg->rr_wheel_spd_sf_err,
                     (int)msg->rear_axle_spd_delta,
                     (unsigned)msg->rear_axle_avg_spd,
                     (unsigned)msg->rear_axle_spd_err,
                     (int)msg->rear_axle_hd_rt,
                     (unsigned)msg->rear_axle_hd_rt_err,
                     (int)msg->front_axle_spd_delta,
                     (unsigned)msg->front_axle_avg_spd,
                     (unsigned)msg->front_axle_spd_err,
                     (int)msg->front_axle_hd_rt,
                     (unsigned)msg->front_axle_hd_rt_err);
            break;
         }

         case SIRF_MSG_SSB_DR_NAV_SUBSYS: /* 0x0330 (48, 3) */
         {
            tSIRF_MSG_SSB_DR_NAV_SUBSYS * msg = (tSIRF_MSG_SSB_DR_NAV_SUBSYS*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "48,3,%d,%u,%u,%u,%u,%u,%lu,%d,%u,%u,%u,%u,%u,%lu,%u,%u",
                     (int)msg->gps_hd_rte,
                     (unsigned)msg->gps_hd_rte_e,
                     (unsigned)msg->gps_hd,
                     (unsigned)msg->gps_hd_e,
                     (unsigned)msg->gps_spd,
                     (unsigned)msg->gps_spd_e,
                     (unsigned long)msg->gps_pos_e,
                     (int)msg->dr_hd_rte,
                     (unsigned)msg->dr_hd_rte_e,
                     (unsigned)msg->dr_hd,
                     (unsigned)msg->dr_hd_e,
                     (unsigned)msg->dr_spd,
                     (unsigned)msg->dr_spd_e,
                     (unsigned long)msg->dr_pos_e,
                     (unsigned)msg->reserved[0],
                     (unsigned)msg->reserved[1]);
            break;
         }

         case SIRF_MSG_SSB_DR_SENS_PARAM: /* 0x00730 (48, 7) */
         {
            tSIRF_MSG_SSB_DR_SENS_PARAM *msg = (tSIRF_MSG_SSB_DR_SENS_PARAM*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "48,7,%u,%u,%u,",
                     (unsigned)msg->base_ssf,
                     (unsigned)msg->base_gb,
                     (unsigned)msg->base_gsf );
            break;
         }

         case SIRF_MSG_SSB_DR_DATA_BLK: /* 0x0830 (48, 8) */
         {
            tSIRF_MSG_SSB_DR_DATA_BLK * msg = (tSIRF_MSG_SSB_DR_DATA_BLK*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "48,8,%u,%u,%u,"
                               "%lu,%u,%d,%lu,%u,%d,%lu,%u,%d,%lu,%u,%d,%lu,%u,%d,"
                               "%lu,%u,%d,%lu,%u,%d,%lu,%u,%d,%lu,%u,%d,%lu,%u,%d",
              (unsigned)msg->meas_type,  (unsigned)msg->valid_cnt,  (unsigned)msg->bkup_flgs,
              (unsigned long)msg->blk[0].tag, (unsigned)msg->blk[0].spd, (int)msg->blk[0].hd_rte,
              (unsigned long)msg->blk[1].tag, (unsigned)msg->blk[1].spd, (int)msg->blk[1].hd_rte,
              (unsigned long)msg->blk[2].tag, (unsigned)msg->blk[2].spd, (int)msg->blk[2].hd_rte,
              (unsigned long)msg->blk[3].tag, (unsigned)msg->blk[3].spd, (int)msg->blk[3].hd_rte,
              (unsigned long)msg->blk[4].tag, (unsigned)msg->blk[4].spd, (int)msg->blk[4].hd_rte,
              (unsigned long)msg->blk[5].tag, (unsigned)msg->blk[5].spd, (int)msg->blk[5].hd_rte,
              (unsigned long)msg->blk[6].tag, (unsigned)msg->blk[6].spd, (int)msg->blk[6].hd_rte,
              (unsigned long)msg->blk[7].tag, (unsigned)msg->blk[7].spd, (int)msg->blk[7].hd_rte,
              (unsigned long)msg->blk[8].tag, (unsigned)msg->blk[8].spd, (int)msg->blk[8].hd_rte,
              (unsigned long)msg->blk[9].tag, (unsigned)msg->blk[9].spd, (int)msg->blk[9].hd_rte );
            break;
         }

         case SIRF_MSG_SSB_MMF_STATUS: /* 0x5030 (48, 80) */
         {
            tSIRF_MSG_SSB_MMF_STATUS *msg = (tSIRF_MSG_SSB_MMF_STATUS*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "48,80,%lu,%u,%ld,%ld,%ld,%lu,%u,%ld,%ld,%ld,%lu",
                  (unsigned long)msg->status,
                  (unsigned)msg->sys_hdg,
                  (long)msg->sys_lat,
                  (long)msg->sys_lon,
                  (long)msg->sys_alt,
                  (unsigned long)msg->sys_tow,
                  (unsigned)msg->mmf_hdg,
                  (long)msg->mmf_lat,
                  (long)msg->mmf_lon,
                  (long)msg->mmf_alt,
                  (unsigned long)msg->mmf_tow );
            break;
         }

         case SIRF_MSG_SSB_SBAS_PARAM: /* 0x32 (50) */
         {
            tSIRF_MSG_SSB_SBAS_PARAM *msg = (tSIRF_MSG_SSB_SBAS_PARAM*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "50,%u,%u,%u,%u",
                  (unsigned)msg->prn,
                  (unsigned)msg->mode,
                  (unsigned)msg->timeout,
                  (unsigned)msg->flg_bits );
            break;
         }

         case SIRF_MSG_SSB_SIRFNAV_COMPLETE: /* 0x0133 (51, 1) */
         {
            tSIRF_MSG_SSB_SIRFNAV_COMPLETE *msg = (tSIRF_MSG_SSB_SIRFNAV_COMPLETE*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "51,1,%u",
                  (unsigned)msg->reserved);
            break;
         }

         case SIRF_MSG_SSB_SIRFNAV_TIME_TAGS: /* 0x0433 (51, 4) */
         {
            tSIRF_MSG_SSB_SIRFNAV_TIME_TAGS *msg = (tSIRF_MSG_SSB_SIRFNAV_TIME_TAGS*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "51,4,%lu,%u,%lu,%u,%u,%u,%u,%u,%u,%u,%lu",
                  (unsigned long)msg->tracker_time_tag,
                  (unsigned)msg->gps_week,
                  (unsigned long)msg->gps_tow,
                  (unsigned)msg->tracker_rtc_day,
                  (unsigned)msg->tracker_rtc_hour,
                  (unsigned)msg->tracker_rtc_min,
                  (unsigned)msg->tracker_rtc_sec,
                  (unsigned)msg->tracker_rtc_clkctr,
                  (unsigned)msg->tracker_rtc_ok,
                  (unsigned)msg->tracker_rtc_rollover_complete,
                  (unsigned long)msg->tracker_rtc_rollover_seconds );
            break;
         }

         case SIRF_MSG_SSB_TRACKER_LOADER_STATE: // 0x0633 (51, 6)
         {
            tSIRF_MSG_SSB_TRACKER_LOADER_STATE *msg = (tSIRF_MSG_SSB_TRACKER_LOADER_STATE*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "51,6,%lu,%lu,%lu,%lu",
              (unsigned long)msg->loader_state,
              (unsigned long)msg->percentage_loaded,
              (unsigned long)msg->loader_error,
              (unsigned long)msg->time_tag );
            break;
         }

         case SIRF_MSG_SSB_SIRFNAV_START: /* 0x0733 (51, 7) */
         {
            tSIRF_MSG_SSB_SIRFNAV_START *msg = (tSIRF_MSG_SSB_SIRFNAV_START*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "51,7,%lu,%lu,%lu,%lu",
                  (unsigned long)msg->start_mode,
                  (unsigned long)msg->clock_offset,
                  (unsigned long)msg->port_num,
                  (unsigned long)msg->baud_rate );
            break;
         }
         case SIRF_MSG_SSB_EE_GPS_TIME_INFO: /* 0x0138 */
         {
            tSIRF_MSG_SSB_EE_GPS_TIME_INFO *msg = (tSIRF_MSG_SSB_EE_GPS_TIME_INFO*) message_structure;

            snprintf( szBuf, sizeof(szBuf), "56,1,%u,%u,%lu,%lu",
                  (unsigned)msg->time_valid_flag,
                  (unsigned)msg->gps_week,
                  (unsigned long)msg->gps_tow,
                  (unsigned long)msg->ephemeris_request_mask );
         }
         break;

         case SIRF_MSG_SSB_EE_ACK: /* 0xFF38 */
         {
            tSIRF_MSG_SSB_EE_ACK *msg = (tSIRF_MSG_SSB_EE_ACK*) message_structure;

            snprintf( szBuf, sizeof(szBuf), "56,255,%u,%u",
                  (unsigned)msg->acq_id,
                  (unsigned)msg->acq_sub_id );
         }
         break;

         case SIRF_MSG_SSB_SIRFNAV_STOP: /* 0x0833 (161, 8) */
         {
            tSIRF_MSG_SSB_SIRFNAV_STOP *msg = (tSIRF_MSG_SSB_SIRFNAV_STOP*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "51,8,%lu",
                  (unsigned long)msg->stop_mode );
            break;
         }

         case SIRF_MSG_SSB_STARTUP_INFO: /* 0x0136 (54, 1) */
         {
            tSIRF_MSG_SSB_STARTUP_INFO *msg = (tSIRF_MSG_SSB_STARTUP_INFO*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "54,1,%u,%u,%u,"
                               "%lu,%u,%lu,%u,%u,%u,%u,%u,%u,"
                               "%ld,%ld,%ld,%u,"
                               "%lu,%lu,%u,%u,%ld,%lu",
              (unsigned)msg->mode,  (unsigned)msg->status,  (unsigned)msg->dr_status,
              (unsigned long)msg->gps_off_time, (unsigned)msg->gps_week, (unsigned long)msg->gps_tow,   (unsigned)msg->utc_year, (unsigned)msg->utc_month, (unsigned)msg->utc_day,   (unsigned)msg->utc_hour, (unsigned)msg->utc_min, (unsigned)msg->utc_sec,
              (long)msg->lat, (long)msg->lon, (long)msg->alt_ellips, (unsigned)msg->heading,
              (unsigned long)msg->valid_ephemeris, (unsigned long)msg->collected_almanac, (unsigned)msg->collected_almanac_week, (unsigned)msg->factory_almanac_week,   (long)msg->clk_offset, (unsigned long)msg->reserved );
            break;
         }

         case SIRF_MSG_SSB_GPIO_READ: /* 0xC041 (65,192) */
         {
            tSIRF_MSG_SSB_GPIO_READ *msg = (tSIRF_MSG_SSB_GPIO_READ*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "65,192,%u", (unsigned)msg->gpio_state );
            break;
         }

         case SIRF_MSG_SSB_DOP_VALUES: /* 0x42 (66) */
         {
            tSIRF_MSG_SSB_DOP_VALUES *msg = (tSIRF_MSG_SSB_DOP_VALUES*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "66,%lu,%u,%u,%u,%u,%u",
              (unsigned long)msg->gps_tow,
              (unsigned)msg->gdop, (unsigned)msg->pdop, (unsigned)msg->hdop, (unsigned)msg->vdop, (unsigned)msg->tdop );
            break;
         }

         case SIRF_MSG_SSB_TRKR_DBG: /* 0x44 (68) */
         {
            tSIRF_CHAR *msg = ((tSIRF_CHAR*)message_structure);

            if((msg[0] == (tSIRF_CHAR)SIRF_GET_MID(SIRF_MSG_SSB_SIRF_INTERNAL_OUT)) &&
               (msg[1] == 0)) /* Encrypted tracker output */
            {
               snprintf(szBuf, sizeof(szBuf), "68,%ld,%02X",
                        (unsigned long)SIRF_GET_MID(SIRF_MSG_SSB_SIRF_INTERNAL_OUT), (unsigned)msg[1]);
               for ( i = 2; i < message_length; i++ )
               {
                  snprintf( szFoo, sizeof(szFoo), ",%02X", (unsigned)(((tSIRF_UINT8*)message_structure)[i]) );
                  strlcat( szBuf, szFoo, sizeof(szBuf) );
               }
            }
            else
            {
               snprintf(szBuf, sizeof(szBuf), "68,%u,%*s",
                        (unsigned)((tSIRF_UINT8)(*msg)), (int)(message_length-1), msg+1 );
            }
            break;
         }

         case SIRF_MSG_SSB_DEMO_START_GPS_ENGINE: /* 0x05A1 (161, 5) */
         {
            tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE *msg = (tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "161,5,%lu,%lu,%lu,%lu",
                  (unsigned long)msg->start_mode,
                  (unsigned long)msg->clock_offset,
                  (unsigned long)msg->port_num,
                  (unsigned long)msg->baud_rate );
            break;
         }

         case SIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE: /* 0x06A1 (161, 6) */
         {
            tSIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE *msg = (tSIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "161,6,%lu",
                  (unsigned long)msg->stop_mode );
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
                  (unsigned)msg->sensor_data_type,
                  (unsigned)msg->num_of_data_sets,
                  (unsigned)msg->reverse_bitmap );
            for ( i = 0; i < 11; i++ )
            {
                  snprintf( szFoo, sizeof(szFoo), ",%u,%lu,%u,%d,%d,%d,%d,%u",
                     (unsigned)msg->data[i].valid_sensor_indication,
                     (unsigned long)msg->data[i].data_set_time_tag,
                     (unsigned)msg->data[i].odometer_speed,
                     (int)msg->data[i].data1,
                     (int)msg->data[i].data2,
                     (int)msg->data[i].data3,
                     (int)msg->data[i].data4,
                     (unsigned)msg->data[i].reserved );
                  strlcat( szBuf, szFoo, sizeof(szBuf) );
            }
            break;
         }

         case SIRF_MSG_SSB_DR_CAR_BUS_ENABLED: /* 0x0AAC (172, 10) */
         {
            tSIRF_MSG_SSB_DR_CAR_BUS_ENABLED *msg = (tSIRF_MSG_SSB_DR_CAR_BUS_ENABLED*)message_structure;
            snprintf( szBuf, sizeof(szBuf), "172,10,%lu",
                     (unsigned long)msg->mode );
            break;
         }

         case SIRF_MSG_SSB_DR_CAR_BUS_DISABLED: /* 0x0BAC (172, 11) */
         {
            tSIRF_MSG_SSB_DR_CAR_BUS_DISABLED *msg = (tSIRF_MSG_SSB_DR_CAR_BUS_DISABLED*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "172,11,%lu",
                     (unsigned long)msg->mode );
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
                     (unsigned)msg->sensor_data_type,
                     (unsigned)msg->num_of_data_sets,
                     (unsigned)msg->reserved );
               for ( i = 0; i < msg->num_of_data_sets; i++ )
               {
                  snprintf( szFoo, sizeof(szFoo), ",%u,%lu,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                     (unsigned)msg->data_set[i].valid_data_indication,
                     (unsigned long)msg->data_set[i].data_set_time_tag,
                     (int)msg->data_set[i].data[0],
                     (int)msg->data_set[i].data[1],
                     (int)msg->data_set[i].data[2],
                     (int)msg->data_set[i].data[3],
                     (int)msg->data_set[i].data[4],
                     (int)msg->data_set[i].data[5],
                     (int)msg->data_set[i].data[6],
                     (int)msg->data_set[i].data[7],
                     (int)msg->data_set[i].data[8] );
                  strlcat( szBuf, szFoo, sizeof(szBuf) );
               }
            }
            break;
         }


         case SIRF_MSG_SSB_MMF_DATA: /* 0x50AC (172, 80) */
         {
            tSIRF_MSG_SSB_MMF_DATA *msg = (tSIRF_MSG_SSB_MMF_DATA*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "172,80,%lu,%u,%u",
                  (unsigned long)msg->gps_tow_reference,
                  (unsigned)msg->num_of_data_sets,
                  (unsigned)msg->control );
            for ( i = 0; i < 3; i++ )
            {
                  snprintf( szFoo, sizeof(szFoo), ",%ld,%ld,%lu,%ld,%lu,%u,%u,%u",
                     (long)msg->mmf_data[i].lat,
                     (long)msg->mmf_data[i].lon,
                     (unsigned long)msg->mmf_data[i].horiz_pos_uncert,
                     (long)msg->mmf_data[i].alt_ellips,
                     (unsigned long)msg->mmf_data[i].vert_pos_uncert,
                     (unsigned)msg->mmf_data[i].heading,
                     (unsigned)msg->mmf_data[i].heading_uncert,
                     (unsigned)msg->mmf_data[i].reserved );
                  strlcat( szBuf, szFoo, sizeof(szBuf) );
            } /* for */
            break;
         }

         case SIRF_MSG_SSB_MMF_SET_MODE: /* 0x51AC (172, 81) */
         {
            tSIRF_MSG_SSB_MMF_SET_MODE *msg = (tSIRF_MSG_SSB_MMF_SET_MODE*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "172,81,%lu",
                     (unsigned long)msg->mode );
            break;
         }

         case SIRF_MSG_SSB_GPIO_WRITE: /* 0xB2 0x30 (178,48) */
         {
            tSIRF_MSG_SSB_GPIO_WRITE *msg = (tSIRF_MSG_SSB_GPIO_WRITE*)message_structure;
            snprintf(szBuf, sizeof(szBuf), "178,48,%u,%u", (unsigned)msg->gpio_to_write, (unsigned)msg->gpio_state);
            break;
         }

         case SIRF_MSG_SSB_GPIO_MODE_SET: /* 0xB2 0x31 (178,49) */
         {
            tSIRF_MSG_SSB_GPIO_MODE_SET *msg = (tSIRF_MSG_SSB_GPIO_MODE_SET*)message_structure;
            snprintf(szBuf, sizeof(szBuf), "178,49,%u,%u,%u", (unsigned)msg->gpio_to_set, (unsigned)msg->gpio_mode, (unsigned)msg->gpio_state);
            break;
         }

         case SIRF_MSG_SSB_TRK_HW_TEST_CONFIG: /* 0xCF */
         {
            tSIRF_MSG_SSB_TRK_HW_TEST_CONFIG *msg = (tSIRF_MSG_SSB_TRK_HW_TEST_CONFIG*) message_structure;

            snprintf( szBuf, sizeof(szBuf), "207,%d,%d",
                     (int)msg->RFTestPoint,
                    (int)msg->INTCpuPause );
         }
         break;

         case SIRF_MSG_SSB_SET_IF_TESTPOINT: /* 0xD2 */
         {
            tSIRF_MSG_SSB_SET_IF_TESTPOINT *msg = (tSIRF_MSG_SSB_SET_IF_TESTPOINT*) message_structure;

            snprintf( szBuf, sizeof(szBuf), "210,%d",
                     (int)msg->test_point_control );
         }
         break;

         case SIRF_MSG_SSB_EE_SEA_PROVIDE_EPH:  /* 0x01 0xE8 */
         {
            tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH *msg = (tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH*) message_structure;

            snprintf( szBuf, sizeof(szBuf), "232,1,%u,%lu",
               (unsigned)msg->week,
               (unsigned long)msg->svid_mask );

            for( i = 0; i < SV_PER_PACKET; i++)
            {
               snprintf( szFoo, sizeof(szFoo), ",%u,%u,%u,%u,%d,%d,%ld,%d,%lu,%d,%lu,%u,%d,%ld,%d,%ld,%d,%ld,%ld,%d,%u,%d,%d,%d,%ld,%u",
                  (unsigned)msg->extended_ephemeris[i].PRN,
                  (unsigned)msg->extended_ephemeris[i].ephemerisValidityFlag,
                  (unsigned)msg->extended_ephemeris[i].URA,
                  (unsigned)msg->extended_ephemeris[i].IODE,
                  (int)msg->extended_ephemeris[i].Crs,
                  (int)msg->extended_ephemeris[i].deltaN,
                  (long int)msg->extended_ephemeris[i].M0,
                  (int)msg->extended_ephemeris[i].Cuc,
                  (unsigned long)msg->extended_ephemeris[i].eccen,
                  (int)msg->extended_ephemeris[i].Cus,
                  (unsigned long)msg->extended_ephemeris[i].sqrtA,
                  (unsigned)msg->extended_ephemeris[i].toe,
                  (int)msg->extended_ephemeris[i].Cic,
                  (long int)msg->extended_ephemeris[i].omega0,
                  (int)msg->extended_ephemeris[i].Cis,
                  (long int)msg->extended_ephemeris[i].i0,
                  (int)msg->extended_ephemeris[i].Crc,
                  (long int)msg->extended_ephemeris[i].w,
                  (long int)msg->extended_ephemeris[i].omegaDot,
                  (int)msg->extended_ephemeris[i].iDot,
                  (unsigned)msg->extended_ephemeris[i].toc,
                  (int)msg->extended_ephemeris[i].Tgd,
                  (int)msg->extended_ephemeris[i].af2,
                  (int)msg->extended_ephemeris[i].af1,
                  (long int)msg->extended_ephemeris[i].af0,
                  (unsigned)msg->extended_ephemeris[i].age );
               strlcat( szBuf, szFoo, sizeof(szBuf) );
            } /* for */

            snprintf( szFoo, sizeof(szFoo), ",%u,%u,%u,%u,%u,%u,%u,%u",
               (unsigned)msg->extended_iono.alpha[0],
               (unsigned)msg->extended_iono.alpha[1],
               (unsigned)msg->extended_iono.alpha[2],
               (unsigned)msg->extended_iono.alpha[3],
               (unsigned)msg->extended_iono.beta[0],
               (unsigned)msg->extended_iono.beta[1],
               (unsigned)msg->extended_iono.beta[2],
               (unsigned)msg->extended_iono.beta[3] );
            strlcat( szBuf, szFoo, sizeof(szBuf) );
         }
         break;

         case SIRF_MSG_SSB_EE_FILE_DOWNLOAD:
         {
            tSIRF_MSG_SSB_EE_FILE_DOWNLOAD *msg = (tSIRF_MSG_SSB_EE_FILE_DOWNLOAD*) message_structure;

            snprintf( szBuf, sizeof(szBuf), "232,16,%lu",
                  (unsigned long)msg->reserved );
            break;
         }

         case SIRF_MSG_SSB_SIF_SET_CONFIG: /* 0xFCE8 (232, 252) */
         {
            tSIRF_MSG_SSB_SIF_SET_CONFIG *msg = (tSIRF_MSG_SSB_SIF_SET_CONFIG*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "232,252,%u,%u,%u,%u,%u",
                  (unsigned)msg->operation_mode, (unsigned)msg->file_format, (unsigned)msg->ext_gps_time_src, (unsigned)msg->cgee_input_method, (unsigned)msg->sgee_input_method );
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
                  snprintf(szFoo, sizeof(szFoo), ",%u",(unsigned)msg->buff[i]);
                  strlcat(szBuf, szFoo, sizeof(szBuf));
               }
            }
            break;
         }

         case SIRF_MSG_SSB_EE_DISABLE_EE_SECS: /* 0xFEE8 (232, 254) */
         {
            tSIRF_MSG_SSB_EE_DISABLE_EE_SECS *msg = (tSIRF_MSG_SSB_EE_DISABLE_EE_SECS*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "232,254,%lu",
                  (unsigned long)msg->num_secs );
            break;
         }

#ifdef EMB_SIF
         case SIRF_MSG_SSB_SIF_DISABLE_AIDING: /* 0xFEE8 (232, 254) */
         {
            tSIRF_MSG_SSB_SIF_DISABLE_AIDING *msg = (tSIRF_MSG_SSB_SIF_DISABLE_AIDING*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "232,32,%u,%u",
                  (unsigned)msg->sgeeDisable,(unsigned)msg->cgeeDisable);
            break;
         }

         case SIRF_MSG_SSB_SIF_GET_AIDING_STATUS:/* 0xFEE8 (232, 33) */
         {
            tSIRF_MSG_SSB_SIF_GET_AIDING_STATUS *msg = (tSIRF_MSG_SSB_SIF_GET_AIDING_STATUS*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "232,33,%u",
                  (unsigned)msg->reserved);
            break;
         }
#endif /*EMB_SIF*/

         case SIRF_MSG_SSB_EE_DEBUG: /* 0xFFE8 (232, 255) */
         {
            tSIRF_MSG_SSB_EE_PROPRIETARY_DEBUG *msg = (tSIRF_MSG_SSB_EE_PROPRIETARY_DEBUG*)message_structure;

            snprintf( szBuf, sizeof(szBuf), "232,255,%lu",
                  (unsigned long)msg->debug_flags );
            break;
         }

         case SIRF_MSG_SSB_POS_RSP: /* 0x4501 (69, 1)*/
         {
            tSIRF_MSG_SSB_POSITION_RSP *msg = (tSIRF_MSG_SSB_POSITION_RSP*)message_structure;
            int i;

            snprintf(szBuf, sizeof(szBuf), "69,1,%u,%u,%u,%u,%u,%u,%u,%lu,%ld,%ld,%u,%u,%u,%u,%u,%u,%u,%u,%d,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",
               (unsigned)msg->pos_req_id, (unsigned)msg->pos_results_flag, (unsigned)msg->pos_err_status, (unsigned)msg->qos_pass_flag,
               (unsigned)msg->pos_type, (unsigned)msg->dgps_cor, (unsigned)msg->gps_week, (unsigned long)msg->time_of_week,
               (long)msg->lat, (long)msg->lon, (unsigned)msg->other_sections, (unsigned)msg->hor_err_angle, (unsigned)msg->hor_err_major,
               (unsigned)msg->hor_err_minor, (unsigned)msg->vert_pos_height, (unsigned)msg->vert_pos_std, (unsigned)msg->velocity_horizontal,
               (unsigned)msg->velocity_heading, (int)msg->velocity_vertical, (unsigned)msg->velocity_hor_err_ang,
               (unsigned)msg->velocity_hor_err_maj_std, (unsigned)msg->velocity_hor_err_min_std, (unsigned)msg->velocity_ver_err,
               (unsigned)msg->clk_cor_time_ref, (unsigned)msg->clk_cor_clk_bias, (unsigned)msg->clk_cor_clk_drift,
               (unsigned)msg->clk_cor_clk_std_err, (unsigned)msg->clk_cor_utc_off, (unsigned)msg->pos_corr_num_sat);
            for(i = 0; i < SIRF_MSG_SSB_RSP_NUM_SV_INFO; i++)
            {
               snprintf(szFoo, sizeof(szFoo), ",%u,%u,%u",
                  (unsigned)msg->pos_corr_sv_info[i].sv_prn,
                  (unsigned)msg->pos_corr_sv_info[i].c_n0,
                  (unsigned)msg->pos_corr_sv_info[i].inv_weights);
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
                  (unsigned)msg->pos_req_id, (unsigned)msg->gps_meas_flag, (unsigned)msg->meas_error_status,
                  (unsigned)msg->meas_gps_week, (unsigned long)msg->meas_gps_seconds,
                  (unsigned)msg->time_accuracy, (unsigned)msg->num_sv);
               for(i = 0; i < msg->num_sv; i++)
               {
                  snprintf(szFoo, sizeof(szFoo), ",%u,%u,%d,%u,%u,%u,%u",
                     (unsigned)msg->meas_per_sv_data[i].sv_prn,
                     (unsigned)msg->meas_per_sv_data[i].c_n0,
                     (int)msg->meas_per_sv_data[i].sv_doppler,
                     (unsigned)msg->meas_per_sv_data[i].sv_code_phase_wh,
                     (unsigned)msg->meas_per_sv_data[i].sv_code_phase_fr,
                     (unsigned)msg->meas_per_sv_data[i].multipath_indicator,
                     (unsigned)msg->meas_per_sv_data[i].pseudorange_rms_error);
                  strlcat(szBuf, szFoo, sizeof(szBuf));
               }
            }
            break;
         }

         case SIRF_MSG_SSB_APRX_MS_POSITION_RSP: /* 0xD701 (215, 1) */
         {
            tSIRF_MSG_SSB_APRX_MS_POSITION_RSP *msg = (tSIRF_MSG_SSB_APRX_MS_POSITION_RSP *) message_structure;
            snprintf(szBuf, sizeof(szBuf), "215,1,%ld,%ld,%u,%u,%u,%u",
               (long)msg->lat, (long)msg->lon, (unsigned)msg->alt,
               (unsigned)msg->est_hor_err, (unsigned)msg->est_ver_err, (unsigned)msg->use_alt_aiding);
            break;
         }

         case SIRF_MSG_SSB_TIME_TX_RSP: /* 0xD702 (215, 2) */
         {
            tSIRF_MSG_SSB_TIME_TRANSFER_RSP *msg = (tSIRF_MSG_SSB_TIME_TRANSFER_RSP *) message_structure;
            snprintf(szBuf, sizeof(szBuf), "215,2,%u,%u,%u,%lu,%ld,%u",
               (unsigned)msg->tt_type, (unsigned)msg->week_num,
               (unsigned)msg->time_high, (unsigned long)msg->time_low,
               (long)msg->delta_utc, (unsigned)msg->time_accuracy);
            break;
         }

         case SIRF_MSG_SSB_FREQ_TX_RSP: /* 0xD703 (215, 3) */
         {
            tSIRF_MSG_SSB_FREQ_TRANSFER_RSP *msg = (tSIRF_MSG_SSB_FREQ_TRANSFER_RSP *) message_structure;
            snprintf(szBuf, sizeof(szBuf), "215,3,%d,%u,%lu,%u,%u,%lu",
               (int)msg->freq_offset, (unsigned)msg->accuracy, (unsigned long)msg->time_tag,
               (unsigned)msg->clock_ref, (unsigned)msg->nominal_freq_high, (unsigned long)msg->nominal_freq_low);
            break;
         }

         case SIRF_MSG_SSB_SIRF_STATS:          /* 0xE106 (225, 6) */
         {
            tSIRF_MSG_SSB_SIRF_STATS *msg = (tSIRF_MSG_SSB_SIRF_STATS *) message_structure;
            snprintf(szBuf, sizeof(szBuf), "225,6,%u,%u,%u,%ld,%ld,%ld,%ld,%d,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",
               (unsigned)msg->ttff_since_reset, (unsigned)msg->ttff_since_aiding, (unsigned)msg->ttff_first_nav,
               (long)msg->pos_aiding_error_north, (long)msg->pos_aiding_error_east, (long)msg->pos_aiding_error_down,
               (long)msg->time_aiding_error, (int)msg->freq_aiding_error, (unsigned)msg->hor_pos_uncertainty, (unsigned)msg->ver_pos_uncertainty,
               (unsigned)msg->time_uncertainty, (unsigned)msg->freq_uncertainty, (unsigned)msg->num_aided_ephemeris, (unsigned)msg->num_aided_acq_assist,
               (unsigned)msg->nav_mode, (unsigned)msg->pos_mode, (unsigned)msg->nav_status, (unsigned)msg->start_mode, (unsigned)msg->aiding_status);
            break;
         }

         case SIRF_MSG_SSB_SIRF_STATS_AGPS:     /* 0xE107 (225, 7) */
         {
            tSIRF_MSG_SSB_SIRF_STATS_AGPS *msg = (tSIRF_MSG_SSB_SIRF_STATS_AGPS *) message_structure;
            snprintf(szBuf, sizeof(szBuf), "225,7,%u,%u,%u,%ld,%ld,%ld,%ld,%d,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%lu,%lu",
               (unsigned)msg->stats.ttff_since_reset, (unsigned)msg->stats.ttff_since_aiding, (unsigned)msg->stats.ttff_first_nav,
               (long)msg->stats.pos_aiding_error_north, (long)msg->stats.pos_aiding_error_east, (long)msg->stats.pos_aiding_error_down,
               (long)msg->stats.time_aiding_error, (int)msg->stats.freq_aiding_error,
               (unsigned)msg->stats.hor_pos_uncertainty, (unsigned)msg->stats.ver_pos_uncertainty,
               (unsigned)msg->stats.time_uncertainty, (unsigned)msg->stats.freq_uncertainty,
               (unsigned)msg->stats.num_aided_ephemeris, (unsigned)msg->stats.num_aided_acq_assist,
               (unsigned)msg->stats.nav_mode, (unsigned)msg->stats.pos_mode, (unsigned)msg->stats.nav_status,
               (unsigned)msg->stats.start_mode, (unsigned)msg->stats.aiding_status,
               (unsigned long)msg->clock_drift, (unsigned long)msg->reserved);
            break;
         }

         case SIRF_MSG_SSB_SENSOR_CONFIG:       /* 0xEA01 (234,1) */
         {
            tSIRF_MSG_SSB_SENSOR_CONFIG *msg = (tSIRF_MSG_SSB_SENSOR_CONFIG*) message_structure;
            tSIRF_UINT32 i=0, j=0;

            snprintf(szBuf, sizeof(szBuf), "234,1,%u,%u",
                                        (unsigned)msg->numSensors,
                                        (unsigned)msg->i2cSpeed);

            for(i = 0; (i < msg->numSensors) && (i < SIRF_MSG_SSB_MAX_NUM_SENSORS); i++)
            {
               snprintf(szFoo, sizeof(szFoo), ",%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",
                                      (unsigned)msg->Sensors[i].i2cAddress,(unsigned)msg->Sensors[i].sensorType,msg->Sensors[i].initTime,
                                      (unsigned)msg->Sensors[i].nBytesResol,(unsigned)msg->Sensors[i].sampRate,msg->Sensors[i].sendRate,(unsigned)msg->Sensors[i].decmMethod,
                                      (unsigned)msg->Sensors[i].acqTime,(unsigned)msg->Sensors[i].numReadReg,(unsigned)msg->Sensors[i].measState);
               strlcat(szBuf, szFoo, sizeof(szBuf));

               for(j=0;j<msg->Sensors[i].numReadReg;j++)
                 {
                    snprintf(szFoo, sizeof(szFoo), ",%u,%u", (unsigned)msg->Sensors[i].sensorReadReg[j].readOprMethod,
                                                                               (unsigned)msg->Sensors[i].sensorReadReg[j].dataReadReg);
                    strlcat(szBuf, szFoo, sizeof(szBuf));
                 }

               snprintf(szFoo, sizeof(szFoo), ",%u,%u,%u,%u", (unsigned)msg->Sensors[i].pwrCtrlReg,msg->Sensors[i].pwrOffSetting,
                                   (unsigned)msg->Sensors[i].pwrOnSetting,(unsigned)msg->Sensors[i].numInitReadReg);

               strlcat(szBuf, szFoo, sizeof(szBuf));

               for(j=0;j<msg->Sensors[i].numInitReadReg;j++)
               {
                    snprintf(szFoo, sizeof(szFoo), ",%u,%u", (unsigned)msg->Sensors[i].sensorInitReg[j].address,
                                                                               (unsigned)msg->Sensors[i].sensorInitReg[j].nBytes);
                    strlcat(szBuf, szFoo, sizeof(szBuf));
               }

               snprintf(szFoo, sizeof(szFoo), ",%u,%u", (unsigned)msg->Sensors[i].numCtrlReg,(unsigned)msg->Sensors[i].ctrlRegWriteDelay);
               strlcat(szBuf, szFoo, sizeof(szBuf));

               for(j=0;j<msg->Sensors[i].numCtrlReg;j++)
               {
                    snprintf(szFoo, sizeof(szFoo), ",%u,%u", (unsigned)msg->Sensors[i].sensorCtrlReg[j].address,
                                                                               (unsigned)msg->Sensors[i].sensorCtrlReg[j].value);
                    strlcat(szBuf, szFoo, sizeof(szBuf));
               }
            } /* for loop = numSensors */

            snprintf(szFoo, sizeof(szFoo), ",%u", (unsigned)msg->processingRate);
            strlcat(szBuf, szFoo, sizeof(szBuf));

            for(i=0;i<msg->numSensors;i++)
            {
                    snprintf(szFoo, sizeof(szFoo), ",%u,%u", (unsigned)msg->sensorScaleZeroPointVal[i].zeroPointVal,
                                                                              (unsigned)msg->sensorScaleZeroPointVal[i].scaleFactor);
                    strlcat(szBuf, szFoo, sizeof(szBuf));
            }
            break;
         }

         case SIRF_MSG_SSB_SENSOR_SWITCH:    /* 0xEA02 (234,2) */
         {
           tSIRF_MSG_SSB_SENSOR_SWITCH *msg = (tSIRF_MSG_SSB_SENSOR_SWITCH*) message_structure;

           snprintf(szBuf, sizeof(szBuf), "234,2,%u",
                                          (unsigned)msg->sensorSetState);
           break;
         }

         case SIRF_MSG_SSB_SENSOR_READINGS:     /* 0x4801, (72,1) */
         {
            tSIRF_MSG_SSB_SENSOR_READINGS *msg = (tSIRF_MSG_SSB_SENSOR_READINGS*) message_structure;
            tSIRF_UINT32 i = 0, j =0;

            snprintf(szBuf, sizeof(szBuf), "72,1,%u,%u,%u,%u",
                                        (unsigned)msg->sensorID,
                                        (unsigned)msg->dataLength,
                                        (unsigned)msg->numDataSet,
                                        (unsigned)msg->dataMode);

            for(i = 0; i < msg->numDataSet; i++)
              {
                 snprintf(szFoo, sizeof(szFoo), ",%lu", (unsigned long)msg->dataSet[i].timeTag);
                 strlcat(szBuf, szFoo, sizeof(szBuf));
                   for(j = 0; j < msg->dataLength; j++)
                   {
                       snprintf(szFoo, sizeof(szFoo), ",%u", (unsigned)msg->dataSet[i].data[j]);
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
                                        (unsigned)msg->sensorID,
                                        (unsigned)msg->numInitReadReg);

            for(i=0;i<msg->numInitReadReg;i++)
              {
                 snprintf(szFoo, sizeof(szFoo), ",%u", (unsigned)msg->initData[i].nBytes);
                 strlcat(szBuf, szFoo, sizeof(szBuf));

                 for(j=0;j<msg->initData[i].nBytes;j++)
                  {
                    snprintf(szFoo, sizeof(szFoo), ",%u", (unsigned)msg->initData[i].data[j]);
                    strlcat(szBuf, szFoo, sizeof(szBuf));
                  }
               } /* for */
            break;
         }


         case SIRF_MSG_SSB_RCVR_STATE:    /* 0x4803, (72,3) */
         {
            tSIRF_MSG_SBB_RCVR_STATE *msg = (tSIRF_MSG_SBB_RCVR_STATE*) message_structure;

            snprintf(szBuf, sizeof(szBuf), "72,3,%lu,%u",
                                         (unsigned long)msg->timeStamp,(unsigned)msg->rcvrPhysicalState);
            break;
         }

         case SIRF_MSG_SSB_POINT_N_TELL_OUTPUT:    /* 0x4804, (72,4) */
         {
            tSIRF_MSG_SSB_POINT_N_TELL_OUTPUT *msg = (tSIRF_MSG_SSB_POINT_N_TELL_OUTPUT*) message_structure;

            snprintf(szBuf, sizeof(szBuf), "72,4,%lu,%ld,%ld,%u,%hd,%hd,%u,%u,%u,%u",
                                         (unsigned long)msg->timeStamp,(long)msg->latitude,(long)msg->longitude,(unsigned)msg->heading,(short)msg->pitch,
                                         (short)msg->roll,(unsigned)msg->headingUnc,(unsigned)msg->pitchUnc,(unsigned)msg->rollUnc,(unsigned)msg->status);
            break;
         }

         case SIRF_MSG_SSB_SENSOR_CALIBRATION_OUTPUT:    /* 0x4805, (72,5) */
         {
            tSIRF_MSG_SSB_SENSOR_CALIBRATION_OUTPUT *msg = (tSIRF_MSG_SSB_SENSOR_CALIBRATION_OUTPUT*) message_structure;
            tSIRF_UINT32 i=0;

            snprintf(szBuf, sizeof(szBuf), "72,5,%u,%u,%lu",
                                         (unsigned)msg->msgDescriptor,(unsigned)msg->sensorType,(unsigned long)msg->timeStamp);

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
                                         (unsigned)msg->magModelParamSrc,(long)msg->latitude,(long)msg->longitude,
                                         (long)msg->altitude,(unsigned)msg->year,(unsigned)msg->month,(unsigned)msg->day,msg->declination,
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
                  (unsigned long)SIRF_GET_MID(message_id), (unsigned)SIRF_GET_SUB_ID(message_id));
               for ( i = 0; i <= message_length - 1; i++ )
               {
                  snprintf( szFoo, sizeof(szFoo), ",%02X", (unsigned)(((tSIRF_UINT8*)message_structure)[i]) );
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
                  snprintf( szFoo, sizeof(szFoo), ",%02X", (unsigned)(((tSIRF_UINT8*)message_structure)[i]) );
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

