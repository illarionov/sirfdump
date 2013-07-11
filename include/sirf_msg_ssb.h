/**
 * @addtogroup platform_src_sirf_include
 * @{
 */

/***************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. GPS Software                    *
 *                                                                         *
 *    Copyright (c) 2005-2011 by SiRF Technology, a CSR plc Company        *
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
 * MODULE:
 *
 * FILENAME: sirf_msg_ssb.h
 *
 * DESCRIPTION: Contains the GPS Protocol Definitions
 *
 * ***************************************************************************
 *
 *  These are keywords for Perforce.  Do not modify.
 *
 *  $File: //customs/customer/Marvell-U1/sirf/Software/sirf/include/sirf_msg_ssb.h $
 *
 *  $DateTime: 2011/10/17 14:33:40 $
 *
 *  $Revision: #2 $
 *
 ***************************************************************************/

#ifndef __SIRF_MSG_SSB__H__
#define __SIRF_MSG_SSB__H__

#include "sirfnav_ui_ctrl.h"

#include "sirf_errors.h"
#include "sirf_types.h"


#define SIRF_MSG_NMEA_PASS_THRU \
    (SIRF_MAKE_MSG_ID(SIRF_LC_NMEA,0,0))

/*******************************************************************************
 *   This file defines the messages in the following list:
 *      1. SSB Message IDs
 *   followed by:
 *      1. SSB Message Structures
 ******************************************************************************/

/*******************************************************************************
   Constants Used in Structure Definitions
*******************************************************************************/

#define SIRF_NUM_CHANNELS       ( 12 )
#define SIRF_NUM_POINTS         ( 10 )
#define SIRF_CW_PEAKS           (  8 )   /* This should match with NUM_CW_PEAKS */

/*******************************************************************************
   SSB Message IDs - Output
*******************************************************************************/

/* Messages from GPS (output):                               LC           MID   SID */
#define SIRF_MSG_SSB_MEASURED_NAVIGATION    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x02,    0)
#define SIRF_MSG_SSB_MEASURED_TRACKER       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x04,    0)
#define SIRF_MSG_SSB_RAW_TRACKER            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x05,    0)
#define SIRF_MSG_SSB_SW_VERSION             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x06,    0)
#define SIRF_MSG_SSB_CLOCK_STATUS           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x07,    0)
#define SIRF_MSG_SSB_50BPS_DATA             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x08,    0)
#define SIRF_MSG_SSB_THROUGHPUT             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x09,    0)
#define SIRF_MSG_SSB_ERROR                  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x0A,    0)
#define SIRF_MSG_SSB_ACK                    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x0B,    0)
#define SIRF_MSG_SSB_NAK                    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x0C,    0)
#define SIRF_MSG_SSB_VISIBILITY_LIST        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x0D,    0)
#define SIRF_MSG_SSB_ALMANAC                SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x0E,    0)
#define SIRF_MSG_SSB_EPHEMERIS              SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x0F,    0)
#define SIRF_MSG_SSB_RTCM                   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x11,    0)
#define SIRF_MSG_SSB_OK_TO_SEND             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x12,    0)
#define SIRF_MSG_SSB_RECEIVER_PARAMS        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x13,    0)
#define SIRF_MSG_SSB_TEST_MODE_DATA         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x14,    0)
#define SIRF_MSG_SSB_DGPS_STATUS            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x1B,    0)
#define SIRF_MSG_SSB_NL_MEAS_DATA           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x1C,    0)
#define SIRF_MSG_SSB_NL_DGPS_DATA           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x1D,    0)
#define SIRF_MSG_SSB_NL_SV_STATE_DATA       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x1E,    0)
#define SIRF_MSG_SSB_NL_INIT_DATA           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x1F,    0)
#define SIRF_MSG_SSB_GEODETIC_NAVIGATION    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x29,    0)
#define SIRF_MSG_SSB_QUEUE_CMD_PARAM        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B,    0)
    #define SIRF_MSG_SSB_QUEUE_CMD_NI       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x80)
    #define SIRF_MSG_SSB_QUEUE_CMD_DGPS_SRC SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x85)
    #define SIRF_MSG_SSB_QUEUE_CMD_SNM      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x88)
    #define SIRF_MSG_SSB_QUEUE_CMD_SDM      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x89)
    #define SIRF_MSG_SSB_QUEUE_CMD_SDGPSM   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x8A)
    #define SIRF_MSG_SSB_QUEUE_CMD_SEM      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x8B)
    #define SIRF_MSG_SSB_QUEUE_CMD_SPM      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x8C)
    #define SIRF_MSG_SSB_QUEUE_CMD_SSN      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x8F)
    #define SIRF_MSG_SSB_QUEUE_CMD_LP       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x97)
    #define SIRF_MSG_SSB_QUEUE_CMD_SSBAS    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0xAA)
#define SIRF_MSG_SSB_ADC_ODOMETER_DATA      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2D,    0)
#define SIRF_MSG_SSB_TEST_MODE_DATA_3       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2E,    0)
#define SIRF_MSG_SSB_DR_OUTPUT              SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x30,    0)
   #define SIRF_MSG_SSB_DR_NAV_STATUS       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x30, 0x01)
   #define SIRF_MSG_SSB_DR_NAV_STATE        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x30, 0x02)
   #define SIRF_MSG_SSB_DR_NAV_SUBSYS       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x30, 0x03)
   #define SIRF_MSG_SSB_DR_VALID            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x30, 0x05)
   #define SIRF_MSG_SSB_DR_GYR_FACT_CAL     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x30, 0x06)
   #define SIRF_MSG_SSB_DR_SENS_PARAM       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x30, 0x07)
   #define SIRF_MSG_SSB_DR_DATA_BLK         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x30, 0x08)
   #define SIRF_MSG_SSB_MMF_STATUS          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x30, 0x50)
#define SIRF_MSG_SSB_SBAS_PARAM             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x32,    0)
#define SIRF_MSG_SSB_SIRFNAV_NOTIFICATION   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x33,    0)
   #define SIRF_MSG_SSB_SIRFNAV_COMPLETE    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x33, 0x01)
   #define SIRF_MSG_SSB_SIRFNAV_TIMING      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x33, 0x02)
   #define SIRF_MSG_SSB_DEMO_TIMING         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x33, 0x03)
   #define SIRF_MSG_SSB_SIRFNAV_TIME_TAGS   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x33, 0x04)
   #define SIRF_MSG_SSB_TRACKER_LOADER_STATE SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x33, 0x06)
   #define SIRF_MSG_SSB_SIRFNAV_START       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x33, 0x07)
   #define SIRF_MSG_SSB_SIRFNAV_STOP        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x33, 0x08)
#define SIRF_MSG_SSB_EVENT                  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x36,    0)
   #define SIRF_MSG_SSB_STARTUP_INFO        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x36, 0x01)
#define SIRF_MSG_SSB_EE                     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38,    0)
   #define SIRF_MSG_SSB_EE_GPS_TIME_INFO    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38, 0x01)
   #define SIRF_MSG_SSB_EE_INTEGRITY        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38, 0x02)
   #define SIRF_MSG_SSB_EE_STATE            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38, 0x03)
   #define SIRF_MSG_SSB_EE_CLK_BIAS_ADJ     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38, 0x04)
   #define SIRF_MSG_SSB_EE_50BPS_DATA_VERIFIED SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38, 0x05)
   #define SIRF_MSG_SSB_EE_EPHEMERIS_AGE    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38, 0x11)
   #define SIRF_MSG_SSB_EE_STORE_NOW        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38, 0x12)
   #define SIRF_MSG_SSB_EE_FILE_PART_RESP   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38, 0x13)
   /* Start Embedded CLM */
    #define SIRF_MSG_SSB_SIF_ACK_NACK       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38, 0x20)
    #define SIRF_MSG_SSB_SIF_EE_AGE         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38, 0x21)
    #define SIRF_MSG_SSB_SIF_SGEE_AGE       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38, 0x22)
    #define SIRF_MSG_SSB_SIF_PKT_INVOKE_DLD SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38,0x23)
    #define SIRF_MSG_SSB_SIF_PKT_ERASE      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38,0x24)
    #define SIRF_MSG_SSB_SIF_STR_PKT_DATA   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38,0x25)
    #define SIRF_MSG_SSB_SIF_RCV_PKT_DATA   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38,0x26)
    #define SIRF_MSG_SSB_SIF_NVM_HEADER_DATA  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38,0x27)
    #define SIRF_MSG_SSB_SIF_GET_HOST_HEADER  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38,0x28)
    #define SIRF_MSG_SSB_SIF_AIDING_STATUS  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38,0x29)
   /* End Embedded CLM */
   #define SIRF_MSG_SSB_EE_ACK              SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38, 0xFF)
#define SIRF_MSG_SSB_TEST_MODE_DATA_ID      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x3F,    0)
#define SIRF_MSG_SSB_TEST_MODE_DATA_7       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x3F, 0x07)
#define SIRF_MSG_SSB_TEST_MODE_DATA_8       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x3F, 0x08)
#define SIRF_MSG_SSB_TEST_MODE_DATA_9       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x3F, 0x09)
#define SIRF_MSG_SSB_NL_AUX_DATA            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x40,    0)
    #define SIRF_MSG_SSB_NL_AUX_INIT_DATA   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x40, 0x01)
    #define SIRF_MSG_SSB_NL_AUX_MEAS_DATA   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x40, 0x02)
    #define SIRF_MSG_SSB_NL_AUX_AID_DATA    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x40, 0x03)
#define SIRF_MSG_SSB_IC_DATA                SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x41,    0)
   #define SIRF_MSG_SSB_GPIO_READ           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x41, 0xC0)
#define SIRF_MSG_SSB_DOP_VALUES             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x42,    0)
#define SIRF_MSG_SSB_TRKR_DBG               SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x44,    0)
/*  SIRF_AGPS Messages */
#define SIRF_MSG_SSB_POS_MEAS_RSP           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x45, 0)
    #define SIRF_MSG_SSB_POS_RSP            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x45, 0x1)
    #define SIRF_MSG_SSB_MEAS_RSP           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x45, 0x2)
    #define SIRF_MSG_SSB_PARTPOS_RSP        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x45, 0x3)
#define SIRF_MSG_SSB_STATUS_RESP            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x46, 0)
    #define SIRF_MSG_SSB_EPH_RSP            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x46, 0x1)
    #define SIRF_MSG_SSB_ALM_RSP            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x46, 0x2)
    #define SIRF_MSG_SSB_B_EPH_RSP          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x46, 0x3)
    #define SIRF_MSG_SSB_TIME_FREQ_APPROX_POS_RSP    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x46, 0x4)
    #define SIRF_MSG_SSB_CH_LOAD_RSP        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x46, 0x5)
    #define SIRF_MSG_SSB_CLIENT_STATUS_RSP  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x46, 0x6)
    #define SIRF_MSG_SSB_OSP_REV_RSP        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x46, 0x7)
    #define SIRF_MSG_SSB_SERIAL_SETTINGS_RSP SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x46, 0x8)
    #define SIRF_MSG_SSB_TX_BLANKING_RESP   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x46, 0x9)
#define SIRF_MSG_SSB_HW_CONFIG_REQ          SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x47, 0)

#define SIRF_MSG_SSB_SENSOR_DATA            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x48, 0)
    #define SIRF_MSG_SSB_SENSOR_READINGS    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x48, 0x1)
    #define SIRF_MSG_SSB_SENSOR_FACTORY_STORED_PARAMS        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x48, 0x2)
    #define SIRF_MSG_SSB_RCVR_STATE         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x48, 0x3)
    #define SIRF_MSG_SSB_POINT_N_TELL_OUTPUT        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x48, 0x4)
    #define SIRF_MSG_SSB_SENSOR_CALIBRATION_OUTPUT        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x48, 0x5)
    #define SIRF_MSG_SSB_MAG_MODEL_PARAMS        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x48, 0x6)
#define SIRF_MSG_SSB_AIDING_REQ             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x49, 0)
    #define SIRF_MSG_SSB_APROX_MS_POS_REQ   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x49, 0x1)
    #define SIRF_MSG_SSB_TIME_TX_REQ        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x49, 0x2)
    #define SIRF_MSG_SSB_FREQ_TX_REQ        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x49, 0x3)
    #define SIRF_MSG_SSB_NBA_REQ            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x49, 0x4)
#define SIRF_MSG_SSB_SESSION_CONTROL_RSP    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x4A, 0)
    #define SIRF_MSG_SSB_SESSION_OPEN_RSP   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x4A, 1)
    #define SIRF_MSG_SSB_SESSION_CLOSE_RSP  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x4A, 2)
#define SIRF_MSG_SSB_ACK_OUT                SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x4B, 0)
   #define SIRF_MSG_SSB_ACK_NACK_ERROR_OUT  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x4B, 0x1)
   #define SIRF_MSG_SSB_REJECT_OUT          SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x4B, 0x2)
#define SIRF_MSG_SSB_MPM_STATUS_OUT         SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x4D, 0)
   #define SIRF_MSG_SSB_MPM_STATUS_SVD      SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x4D, 0x1)
   #define SIRF_MSG_SSB_MPM_STATUS_EPH      SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x4D, 0x2)
   #define SIRF_MSG_SSB_MPM_STATUS_ALM      SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x4D, 0x3)
   #define SIRF_MSG_SSB_MPM_STATUS_UPD      SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x4D, 0x4)
   #define SIRF_MSG_SSB_MPM_STATUS_REC      SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x4D, 0x5)
#define SIRF_MSG_SSB_PWR_MODE_APM_RSP       SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x5A, 0x1)
/*  End of SIRF_AGPS Messages  */

#define SIRF_MSG_SSB_PWR_MODE_RSP           SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x5A, 0)
    #define SIRF_MSG_SSB_PWR_MODE_FPM_RSP   SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x5A, 0x0)
    #define SIRF_MSG_SSB_PWR_MODE_MPM_RSP   SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x5A, 0x2)
    #define SIRF_MSG_SSB_PWR_MODE_TP_RSP    SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x5A, 0x3)
    #define SIRF_MSG_SSB_PWR_MODE_PTF_RSP   SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x5A, 0x4)
    #define SIRF_MSG_SSB_PWR_MODE_ERR_RSP   SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x5A, 0x5)

#define SIRF_MSG_SSB_CW_OUTPUT              SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5C,    0)
    #define SIRF_MSG_SSB_CW_DATA            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5C, 0x01)
    #define SIRF_MSG_SSB_CW_FILTER          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5C, 0x02)

/* Xo Messages */
#define SIRF_MSG_SSB_XO_LEARNING_OUT        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D,    0)
    #define SIRF_MSG_SSB_XO_DEFAULTS_OUT    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x01)
    #define SIRF_MSG_SSB_TCXO_TABLE_OUT     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x02)
    #define SIRF_MSG_SSB_XO_TEMP_REC_OUT    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x04)
    #define SIRF_MSG_SSB_XO_EARC_OUT        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x05)
    #define SIRF_MSG_SSB_XO_RTC_ALARM_OUT   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x06)
    #define SIRF_MSG_SSB_XO_RTC_CAL_OUT     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x07)
    #define SIRF_MSG_SSB_XO_MPM_ACQ_OUT     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x09)
    #define SIRF_MSG_SSB_XO_MPM_PREPOS_OUT  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x0A)
    #define SIRF_MSG_SSB_XO_UNL_MEAS_OUT    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x0B)
    #define SIRF_MSG_SSB_XO_UNCERTAINTY_OUT SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x0C)
    #define SIRF_MSG_SSB_XO_SYS_TIME_OUT    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x0D)
    #define SIRF_MSG_SSB_XO_RTC_TABLE_OUT   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x0E)
/* For XO_ENABLED implementation */
    #define SIRF_MSG_SSB_XO_POLY_OUT        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x0F)
    #define SIRF_MSG_SSB_XO_TABLE_OUT       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x10)
    #define SIRF_MSG_SSB_XO_DENSITY_OUT     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x11)

#define SIRF_MSG_SSB_PASSTHRU_OUTPUT_BEGIN  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x61,    0)
#define SIRF_MSG_SSB_PASSTHRU_OUTPUT_END    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x7F,    0)
#define SIRF_MSG_SSB_TRKR_PEEKPOKE_RSP      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x04)
#define SIRF_MSG_SSB_TRKR_FLASHSTORE_RSP    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x05)
#define SIRF_MSG_SSB_TRKR_FLASHERASE_RSP    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x06)
#define SIRF_MSG_SSB_TRKR_CONFIG_RSP        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x07)
#define SIRF_MSG_SSB_TRKR_CUSTOMIO_RSP      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x08)
#ifdef PVT_BUILD
#define SIRF_MSG_SSB_TRKR_CONFIG_POLL_RSP   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x0A)
#define SIRF_MSG_SSB_CCK_POLL_RSP           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x0C)
#define SIRF_MSG_SSB_SWTB_PMPROMPT_OUT      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x90)
#define SIRF_MSG_SSB_SWTB_PMACK_OUT         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x91)
#endif /* PVT_BUILD */

#define SIRF_MSG_SSB_SIRF_INTERNAL_OUT      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE1,    0)
#define SIRF_MSG_SSB_SIRF_STATS             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE1,    6)
#define SIRF_MSG_SSB_SIRF_STATS_AGPS        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE1,    7)

#define SIRF_MSG_SSB_TEXT                   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xFF,    0)

/*******************************************************************************
   SSB Message IDs - Input
*******************************************************************************/

/* Messages to GPS (input):                                    LC           MID   SID */
#define SIRF_MSG_SSB_INITIALIZE               SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x80,    0)
#define SIRF_MSG_SSB_SET_NMEA_MODE            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x81,    0)
#define SIRF_MSG_SSB_SET_ALMANAC              SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x82,    0)
#define SIRF_MSG_SSB_POLL_SW_VERSION          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x84,    0)
#define SIRF_MSG_SSB_SET_DGPS_SOURCE          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x85,    0)
#define SIRF_MSG_SSB_SET_PORT_VALUES          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x86,    0) /**< @todo Needs actual assignment */
#define SIRF_MSG_SSB_SET_PORT_PROTOCOL        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x87,    0) /**< @todo temp */
#define SIRF_MSG_SSB_SET_NAV_MODE             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x88,    0)
#define SIRF_MSG_SSB_SET_DOP_MODE             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x89,    0)
#define SIRF_MSG_SSB_SET_DGPS_MODE            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x8A,    0)
#define SIRF_MSG_SSB_SET_ELEV_MASK            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x8B,    0)
#define SIRF_MSG_SSB_SET_POWER_MASK           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x8C,    0)
#define SIRF_MSG_SSB_SET_STAT_NAV             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x8F,    0)
#define SIRF_MSG_SSB_POLL_CLOCK_STATUS        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x90,    0)
#define SIRF_MSG_SSB_POLL_ALMANAC             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x92,    0)
#define SIRF_MSG_SSB_POLL_EPHEMERIS           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x93,    0)
#define SIRF_MSG_SSB_FLASH_UPDATE             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x94,    0)
#define SIRF_MSG_SSB_SET_EPHEMERIS            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x95,    0)
#define SIRF_MSG_SSB_SET_OP_MODE              SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x96,    0)
#define SIRF_MSG_SSB_SET_LOW_POWER            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x97,    0)
#define SIRF_MSG_SSB_POLL_RECEIVER_PARAMS     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x98,    0)
#define SIRF_MSG_SSB_SIRFNAV_COMMAND          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xA1,    0)
   #define SIRF_MSG_SSB_DEMO_SET_RESTART_MODE SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xA1, 0x01)
   #define SIRF_MSG_SSB_DEMO_TEST_CPU_STRESS  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xA1, 0x02)
   #define SIRF_MSG_SSB_DEMO_STOP_TEST_APP    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xA1, 0x03)
   #define SIRF_MSG_SSB_DEMO_START_GPS_ENGINE SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xA1, 0x05)
   #define SIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xA1, 0x06)
   #define SIRF_MSG_SSB_SIRFNAV_STORE_NOW     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xA1, 0x07)
   #define SIRF_MSG_SSB_DEMO_START_NAV_ENGINE SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xA1, 0x08)
#define SIRF_MSG_SSB_SET_MSG_RATE             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xA6,    0)
#define SIRF_MSG_SSB_SET_LOW_POWER_PARAMS     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xA7,    0)
#define SIRF_MSG_SSB_POLL_CMD_PARAM           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xA8,    0)
#define SIRF_MSG_SSB_SET_SBAS_PRN             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAA,    0)
#define SIRF_MSG_SSB_ADVANCED_NAV_INIT        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC,    0)
   #define SIRF_MSG_SSB_DR_SET_NAV_INIT       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x01)
   #define SIRF_MSG_SSB_DR_SET_NAV_MODE       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x02)
   #define SIRF_MSG_SSB_DR_SET_GYR_FACT_CAL   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x03)
   #define SIRF_MSG_SSB_DR_SET_SENS_PARAM     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x04)
   #define SIRF_MSG_SSB_DR_POLL_VALID         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x05)
   #define SIRF_MSG_SSB_DR_POLL_GYR_FACT_CAL  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x06)
   #define SIRF_MSG_SSB_DR_POLL_SENS_PARAM    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x07)
   #define SIRF_MSG_SSB_DR_CAR_BUS_DATA       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x09) /* This message is obsolete, use msg (AC,13) instead */
   #define SIRF_MSG_SSB_DR_CAR_BUS_ENABLED    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x0A)
   #define SIRF_MSG_SSB_DR_CAR_BUS_DISABLED   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x0B)
   #define SIRF_MSG_SSB_DR_SENSOR_DATA        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x13)
   #define SIRF_MSG_SSB_MMF_DATA              SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x50)
   #define SIRF_MSG_SSB_MMF_SET_MODE          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xAC, 0x51)
#define SIRF_MSG_SSB_IC_CONTROL               SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2,    0)
   #define SIRF_MSG_SSB_TRKR_CUSTOMIO         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x01)
   #define SIRF_MSG_SSB_TRKR_CONFIG           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x02)
   #define SIRF_MSG_SSB_TRKR_PEEKPOKE_CMD     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x03)
   #define SIRF_MSG_SSB_GPIO_WRITE            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x30)
   #define SIRF_MSG_SSB_GPIO_MODE_SET         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x31)
#ifdef PVT_BUILD
   #define SIRF_MSG_SSB_TRKR_CONFIG_POLL      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x09)
   #define SIRF_MSG_SSB_CCK_POLL              SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x0B)
   #define SIRF_MSG_SSB_PATCH_STORAGE_CONTROL SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x14)
   #define SIRF_MSG_SSB_SWTB_PMLOAD_IN        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x22)
   #define SIRF_MSG_SSB_SWTB_PMEXIT_IN        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x26)
   #define SIRF_MSG_SSB_SWTB_PMSTART_IN       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x28)
   #define SIRF_MSG_SSB_ENB_ALM2FLASH         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x40)
#endif /* PVT_BUILD */

#define SIRF_MSG_SSB_PASSTHRU_INPUT_BEGIN     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB4,    0)
#define SIRF_MSG_SSB_PASSTHRU_INPUT_END       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xC7,    0)
#define SIRF_MSG_SSB_SW_CONTROL               SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xCD,    0)
    #define SIRF_MSG_SSB_SW_COMMANDED_OFF     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xCD, 0x10)
#define SIRF_MSG_SSB_TRK_HW_CONFIG            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xCE,    0)
#define SIRF_MSG_SSB_TRK_HW_TEST_CONFIG       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xCF,    0)
#define SIRF_MSG_SSB_SET_IF_TESTPOINT         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD2,    0x80) /*! @todo resolve this with SIRF_MSG_SSB_POS_REQ */
/* SIRF_AGPS Messages */
#define SIRF_MSG_SSB_POS_REQ                  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD2, 0)
#define SIRF_MSG_SSB_SET_AIDING               SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD3, 0x0)
    #define SIRF_MSG_SSB_SET_IONO             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD3, 0x1)
    #define SIRF_MSG_SSB_SET_EPH_CLK          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD3, 0x2)
    #define SIRF_MSG_SSB_SET_ALM              SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD3, 0x3)
    #define SIRF_MSG_SSB_SET_ACQ_ASSIST       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD3, 0x4)
    #define SIRF_MSG_SSB_SET_RT_INTEG         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD3, 0x5)
    #define SIRF_MSG_SSB_SET_UTC_MODEL        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD3, 0x6)
    #define SIRF_MSG_SSB_SET_GPS_TOW_ASSIST   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD3, 0x7)
    #define SIRF_MSG_SSB_SET_AUX_NAV          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD3, 0x8)
    #define SIRF_MSG_SSB_SET_AIDING_AVAIL     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD3, 0x9)
#define SIRF_MSG_SSB_STATUS_REQ               SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD4, 0x0)
    #define SIRF_MSG_SSB_EPH_REQ              SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD4, 0x1)
    #define SIRF_MSG_SSB_ALM_REQ              SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD4, 0x2)
    #define SIRF_MSG_SSB_B_EPH_REQ            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD4, 0x3)
    #define SIRF_MSG_SSB_TIME_FREQ_APPROX_POS_REQ           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD4, 0x4)
    #define SIRF_MSG_SSB_CH_LOAD_REQ          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD4, 0x5)
    #define SIRF_MSG_SSB_CLIENT_STATUS_REQ    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD4, 0x6)
    #define SIRF_MSG_SSB_OSP_REV_REQ          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD4, 0x7)
    #define SIRF_MSG_SSB_SERIAL_SETTING_REQ   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD4, 0x8)
    #define SIRF_MSG_SSB_TX_BLANKING_REQ      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD4, 0x9)
#define SIRF_MSG_SSB_SESSION_CONTROL_REQ      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD5, 0)
    #define SIRF_MSG_SSB_SESSION_OPEN_REQ     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD5, 0x1)
    #define SIRF_MSG_SSB_SESSION_CLOSE_REQ    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD5, 0x2)
#define SIRF_MSG_SSB_HW_CONFIG_RSP            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD6, 0)
#define SIRF_MSG_SSB_AIDING_RSP               SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD7, 0)
    #define SIRF_MSG_SSB_APRX_MS_POSITION_RSP SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD7, 0x1)
    #define SIRF_MSG_SSB_TIME_TX_RSP          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD7, 0x2)
    #define SIRF_MSG_SSB_FREQ_TX_RSP          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD7, 0x3)
    #define SIRF_MSG_SSB_SET_NBA_SF1_2_3      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD7, 0x4)
    #define SIRF_MSG_SSB_SET_NBA_SF4_5        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD7, 0x5)
#define SIRF_MSG_SSB_ACK_IN                   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD8, 0x0)
    #define SIRF_MSG_SSB_ACK_NACK_ERROR_IN    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD8, 0x01)
    #define SIRF_MSG_SSB_REJECT_IN            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD8, 0x02)
#define SIRF_MSG_SSB_PWR_MODE_APM_REQ         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDA, 0x1)
/* End of SIRF_AGPS Messages */

#define SIRF_MSG_SSB_PWR_MODE_REQ             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDA, 0)
    #define SIRF_MSG_SSB_PWR_MODE_FPM_REQ     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDA, 0)
    #define SIRF_MSG_SSB_PWR_MODE_MP_REQ      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDA, 0x2)
    #define SIRF_MSG_SSB_PWR_MODE_TP_REQ      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDA, 0x3)
    #define SIRF_MSG_SSB_PWR_MODE_PTF_REQ     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDA, 0x4)

/* CW Input messages */
#define SIRF_MSG_SSB_CW_INPUT                 SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDC,    0)
    #define SIRF_MSG_SSB_CW_CONFIG            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDC, 0x01)

/* Xo Messages */
#define SIRF_MSG_SSB_XO_LEARNING_IN           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDD,    0)
    #define SIRF_MSG_SSB_XO_OUTPUT_CONTROL_IN SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDD, 0x00)
    #define SIRF_MSG_SSB_XO_DEFAULTS_IN       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDD, 0x01)
    #define SIRF_MSG_SSB_TCXO_TABLE_IN        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDD, 0x02)
    #define SIRF_MSG_SSB_XO_TEST_CONTROL_IN   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDD, 0x03)
/* For XO_ENABLED implementation */
    #define SIRF_MSG_SSB_XO_POLY_IN           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDD, 0x0F)

#define SIRF_MSG_SSB_SIRF_INTERNAL            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE4,    0)
#define SIRF_MSG_SSB_EE_INPUT                 SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8,    0)
    #define SIRF_MSG_SSB_EE_SEA_PROVIDE_EPH   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x01)
    #define SIRF_MSG_SSB_EE_POLL_STATE        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x02)
    #define SIRF_MSG_SSB_EE_FILE_DOWNLOAD     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x10)
    #define SIRF_MSG_SSB_EE_QUERY_AGE         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x11)
    #define SIRF_MSG_SSB_EE_FILE_PART         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x12)
    #define SIRF_MSG_SSB_EE_DOWNLOAD_TCP      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x13)
    #define SIRF_MSG_SSB_EE_SET_EPHEMERIS     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x14)
    #define SIRF_MSG_SSB_EE_FILE_STATUS       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x15)
    /* Start Embedded CLM */
    #define SIRF_MSG_SSB_SIF_START_DLD        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x16)
    #define SIRF_MSG_SSB_SIF_EE_FILE_SIZE     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x17)
    #define SIRF_MSG_SSB_SIF_PKT_DATA         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x18)
    #define SIRF_MSG_SSB_SIF_GET_EE_AGE       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x19)
    #define SIRF_MSG_SSB_SIF_GET_SGEE_AGE     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x1A)
    #define SIRF_MSG_SSB_HOST_RCV_PKT_DATA    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x1B)
    #define SIRF_MSG_SSB_HOST_ACK_NACK        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x1C)
    #define SIRF_MSG_SSB_SIF_GET_NVM_HEADER   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x1D)
    #define SIRF_MSG_SSB_SIF_UPDATE_NVM_HEADER  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x1E)
    #define SIRF_MSG_SSB_READ_I2CDATA           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x1F)
    #define SIRF_MSG_SSB_SIF_DISABLE_AIDING     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x20)
    #define SIRF_MSG_SSB_SIF_GET_AIDING_STATUS  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x21)

    #define SIRF_MSG_SSB_POS_FROMPART_REQ     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0xD0)
    #define SIRF_MSG_SSB_SIF_SET_CONFIG       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0xFC)
#ifdef PVT_BUILD
    #define SIRF_MSG_SSB_EE_STORAGE_CONTROL   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0xFD)
#endif /* #ifdef PVT_BUILD */
/* End Embedded CLM */
    #define SIRF_MSG_SSB_EE_DISABLE_EE_SECS   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0xFE)
    #define SIRF_MSG_SSB_EE_DEBUG             SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0xFF)

/* SENSOR INPUT Messages */
#define SIRF_MSG_SSB_SENSOR_CONTROL_INPUT     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xEA,    0)
    #define SIRF_MSG_SSB_SENSOR_CONFIG        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xEA, 0x01)
    #define SIRF_MSG_SSB_SENSOR_SWITCH        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xEA, 0x02)


#define SIRF_MSG_SSB_CW_CONFIG_LEN                 (1)

/* COMMON CONSTANTS ----------------------------------------------------------*/
/* Length of Xo messages, not including bytes for mid or sid */
#define SIRF_MSG_SSB_XO_DEFAULTS_OUT_LEN           (sizeof(tSIRF_MSG_SSB_XO_DEFAULTS_OUT))
#define SIRF_MSG_SSB_TCXO_TABLE_OUT_LEN            (sizeof(tSIRF_MSG_SSB_TCXO_TABLE_OUT))
#define SIRF_MSG_SSB_XO_TEMP_REC_OUT_LEN           (sizeof(tSIRF_MSG_SSB_XO_TEMP_REC_OUT))
#define SIRF_MSG_SSB_XO_EARC_OUT_LEN               (sizeof(tSIRF_MSG_SSB_XO_EARC_OUT))
#define SIRF_MSG_SSB_XO_RTC_ALARM_OUT_LEN          (sizeof(tSIRF_MSG_SSB_XO_RTC_ALARM_OUT))
#define SIRF_MSG_SSB_XO_RTC_CAL_OUT_LEN            (sizeof(tSIRF_MSG_SSB_XO_RTC_CAL_OUT))

#define SIRF_MSG_SSB_XO_RTC_TABLE_OUT_LEN          (sizeof(tSIRF_MSG_SSB_XO_RTC_TABLE_OUT))

/* For XO_ENABLED implementation */
#define SIRF_MSG_SSB_XO_POLY_OUT_LEN               (sizeof(tSIRF_MSG_SSB_XO_POLY_OUT))
#define SIRF_MSG_SSB_XO_TABLE_OUT_LEN              (sizeof(tSIRF_MSG_SSB_XO_TABLE_OUT))
#define SIRF_MSG_SSB_XO_DENSITY_OUT_LEN            (sizeof(tSIRF_MSG_SSB_XO_DENSITY_OUT))

/* Variable length records consist of header and N * record */
#define SIRF_MSG_SSB_XO_MPM_ACQ_OUT_LEN            (12)
#define SIRF_MSG_SSB_XO_MPM_ACQ_OUT_REC_LEN        (20)
#define SIRF_MSG_SSB_XO_MPM_PREPOS_OUT_LEN         (12)
#define SIRF_MSG_SSB_XO_MPM_PREPOS_OUT_REC_LEN     (8)
#define SIRF_MSG_SSB_XO_UNL_MEAS_OUT_LEN           (24)
#define SIRF_MSG_SSB_XO_UNL_MEAS_OUT_REC_LEN       (12)

#define SIRF_MSG_SSB_XO_UNCERTAINTY_OUT_LEN        (sizeof(tSIRF_MSG_SSB_XO_UNCERTAINTY_OUT))
#define SIRF_MSG_SSB_XO_SYS_TIME_OUT_LEN           (sizeof(tSIRF_MSG_SSB_XO_SYS_TIME_OUT))

#define SIRF_MSG_SSB_XO_OUTPUT_CONTROL_IN_LEN      (sizeof(tSIRF_MSG_SSB_XO_OUTPUT_CONTROL_IN))
#define SIRF_MSG_SSB_XO_DEFAULTS_IN_LEN            (sizeof(tSIRF_MSG_SSB_XO_DEFAULTS_IN))
#define SIRF_MSG_SSB_TCXO_TABLE_IN_LEN             (sizeof(tSIRF_MSG_SSB_TCXO_TABLE_IN))
#define SIRF_MSG_SSB_XO_TEST_CONTROL_IN_LEN        (sizeof(tSIRF_MSG_SSB_XO_TEST_CONTROL_IN))
/* For XO_ENABLED implementation */
#define SIRF_MSG_SSB_XO_POLY_IN_LEN                (sizeof(tSIRF_MSG_SSB_XO_POLY_IN))

/* XO Model Size for deviation and density tables */
#define SSB_XO_MODEL_SIZE                          (32)
#define SSB_XO_RTC_TABLE_SIZE                      (32)

/* XO Table min, max size */
#define SSB_XOT_TABLE_SIZE                            (64)

/*******************************************************************************
   The maximum message length that will ever be used in a message buffer.
   Currently the largest SSB message structure (tSIRF_MSG_SSB_BE_RSP) is
   2036 bytes, so set the max SSB message length to 2048.
*******************************************************************************/
#define SIRF_MSG_SSB_MAX_MESSAGE_LEN    ( 2048 )

#define SIRF_MAX_SVID_CNT               ( 32 )

/* If the EPE is exceeded, then a full fix will not be sent */
#define SIRF_UNKNOWN_EPE_EHE_DEFAULT      (-1.0f)  /* value has not been set */
#define SIRF_MAX_EPE_EHE_TIMEOUT          (5000)   /* timeout for no QOS check(ms)*/
#define SIRF_MAX_EPE_FOR_VALID_SSB_NO_QOS (200.0f)
#define SIRF_MAX_EPE_FOR_VALID_SSB        (4200.0f)
#define SIRF_MAX_ELLIPSE_ERROR_FOR_VALID_SSB (600.0f) /* equal to 4200m EHE */
#define SIRF_MAX_EPE_FOR_VALID_ABP_SSB    (16093.0f) /* For almanac-based positions,
                                                      use 10 mile threshold        */

/* SIRF_MSG_SSB_MEASURED_NAVIGATION -----------------------------------------------*/

#define SIRF_MSG_SSB_VELOCITY_LSB               ( 0.125F )
#define SIRF_MSG_SSB_DOP_LSB                    ( 0.2F )

#define SIRF_MSG_SSB_MODE_MASK                  ( 0x07 )
#define SIRF_MSG_SSB_MODE_DOP_MASK_EXCEED       ( 0x40 )
#define SIRF_MSG_SSB_MODE_DGPS_USED             ( 0x80 )

#define SIRF_MSG_SSB_MODE2_SOLUTION_VALIDATED   ( 0x02 )
#define SIRF_MSG_SSB_MODE2_DR_TIMED_OUT         ( 0x04 )
#define SIRF_MSG_SSB_MODE2_VELOCITY_INVALID     ( 0x10 )

/*******************************************************************************
   SSB Message Structures
*******************************************************************************/

/*******************************************************************************
   SIRF_MSG_SSB_MEASURED_NAVIGATION    ( 0x02 ) 2
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_MEASURED_NAVIGATION_tag
{
   tSIRF_INT32  ecef_x;       /**< estimated ECEF X position in meters */
   tSIRF_INT32  ecef_y;       /**< estimated ECEF Y position in meters */
   tSIRF_INT32  ecef_z;       /**< estimated ECEF Z position in meters */
   tSIRF_INT16  ecef_vel_x;   /**< estimated ECEF X velocity in 0.125 m/sec */
   tSIRF_INT16  ecef_vel_y;   /**< estimated ECEF Y velocity in 0.125 m/sec */
   tSIRF_INT16  ecef_vel_z;   /**< estimated ECEF Z velocity in 0.125 m/sec */
   tSIRF_UINT8   nav_mode;    /**< current mode of operation */
   tSIRF_UINT8   hdop;        /**< horizontal dilution of precision, LSB=0.2 */
   tSIRF_UINT8   nav_mode2;   /**< additional mode information */
   tSIRF_UINT16  gps_week;    /**< GPS week number in weeks */
   tSIRF_UINT32  gps_tow;     /**< GPS time of week in 10 ms, 0..60,479,900 ms */
   tSIRF_UINT8   sv_used_cnt; /**< number of SVs used, 0..12 */
   tSIRF_UINT8   sv_used[SIRF_NUM_CHANNELS];  /**< IDs of SVs used, 1..32 */

}  tSIRF_MSG_SSB_MEASURED_NAVIGATION;

/*******************************************************************************
   SIRF_MSG_SSB_MEASURED_TRACKER       ( 0x04 ) 4
*******************************************************************************/

#define SIRF_MSG_SSB_AZIMUTH_LSB                ( 1.5f )
#define SIRF_MSG_SSB_ELEVATION_LSB              ( 0.5f )
#define SIRF_CNO_MAX                    ( 60 )
#define SIRF_NUM_POINTS                 ( 10 )

typedef struct tSIRF_MSG_SSB_MEASURED_TRACKER_tag
{
   tSIRF_INT16 gps_week;               /**< GPS week number in weeks */
   tSIRF_UINT32 gps_tow;                /**< GPS time of week in 10 ms,
                                          0..60,479,900 */
   tSIRF_UINT8  chnl_cnt;               /**< ch count, 0..12 */
   struct tSIRF_SV_INFO_tag
   {
      tSIRF_UINT8  svid;                /**< ID of SV used */
      tSIRF_UINT8  azimuth;             /**< SV azimuth in 1.5 deg */
      tSIRF_UINT8  elevation;           /**< SV elevation in 0.5 deg */
      tSIRF_UINT16 state;               /**< tracking state, 1-locked,
                                          0-not locked, b0 @ t0 */
      tSIRF_UINT8  cno[SIRF_NUM_POINTS]; /**< CNo in dB-Hz, or CPower in dBW */
   } chnl[SIRF_NUM_CHANNELS];

}  tSIRF_MSG_SSB_MEASURED_TRACKER;

/*******************************************************************************
   SIRF_MSG_SSB_RAW_TRACKER              ( 0x05 ) 5
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_RAW_TRACKER_tag
{
   tSIRF_INT32 channel;             /**< Channel number in tracking [0 to 11] */
   tSIRF_INT16 svid;                /**< Satellite id  [1 to 32] */
   tSIRF_INT16 state;               /**< Status of the tracker channel
                                       (mask = 0x1FF) */
   tSIRF_INT32 bit_number;          /**< Bits at 50 bps = 20 ms */
   tSIRF_INT16 msec_number;         /**< Represents time in units of msec */
   tSIRF_INT16 chip_number;         /**< Represents time in units of CA chips */
   tSIRF_INT32 code_phase;          /**< Represents time in units of chips */
   tSIRF_INT32 carrier_doppler;     /**< Doppler frequency */
   tSIRF_INT32 measure_timetag;     /**< Measurement time tag */
   tSIRF_INT32 delta_carrier_phase; /**< Current carrier phase */
   tSIRF_INT16 search_cnt;          /**< How many times to search for a SV */
   tSIRF_UINT8  cno[SIRF_NUM_POINTS]; /**< C/No in dB-Hz */
   tSIRF_UINT8  power_bad_count;     /**< Count of Power in 20 ms below 31 dB-Hz */
   tSIRF_UINT8  phase_bad_count;     /**< Count of Power in 20 ms below 31 dB-Hz */
   tSIRF_INT16 delta_car_interval;  /**< Count of ms contained in delta_carrier
                                       phase */
   tSIRF_INT16 correl_interval;     /**< Correlation interval */

}  tSIRF_MSG_SSB_RAW_TRACKER;

/*******************************************************************************
   SIRF_MSG_SSB_SW_VERSION             ( 0x06 ) 6
*******************************************************************************/
#define MAX_VERSION_LENGTH 80
typedef struct  tSIRF_MSG_SSB_SW_VERSION_tag
{
   tSIRF_UINT8 sirf_ver_bytes;   /**< Number of characters in SiRF Version ID */
   tSIRF_UINT8 cust_ver_bytes;   /**< Number of characters in Customer Version ID */
   tSIRF_UINT8 sirf_ver_str[MAX_VERSION_LENGTH]; /**< SiRF Software Version ID */
   tSIRF_UINT8 cust_ver_str[MAX_VERSION_LENGTH]; /**< Customer Software Version ID */
}  tSIRF_MSG_SSB_SW_VERSION;

/*******************************************************************************
   SIRF_MSG_SSB_CLOCK_STATUS           ( 0x07 ) 7
**************************************************************************/

typedef struct
{
   tSIRF_UINT16 gps_week;        /**< GPS week number in weeks                   */
   tSIRF_UINT32 gps_tow;         /**< GPS time of week in 10 ms; 0..60,479,900 ms*/
   tSIRF_UINT8  sv_used_cnt;     /**< number of SVs used, 0..12                  */
   tSIRF_UINT32 clk_offset;      /**< clock Drift in Hz                          */
   tSIRF_UINT32 clk_bias;        /**< clock Bias in nanoseconds                  */
   tSIRF_UINT32 est_gps_time;    /**< estimated gps time in milliseconds         */

}  tSIRF_MSG_SSB_CLOCK_STATUS;

/*******************************************************************************
   SIRF_MSG_SSB_50BPS_DATA             ( 0x08 ) 8
   SIRF_MSG_SSB_EE_50BPS_DATA_VERIFIED ( 0x38, 5 )
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_50BPS_DATA_tag
{
   tSIRF_UINT8  chnl;
   tSIRF_UINT8  svid;
   tSIRF_UINT32 word[10];
}  tSIRF_MSG_SSB_50BPS_DATA;

/*******************************************************************************
   SIRF_MSG_SSB_THROUGHPUT             ( 0x09 ) 9
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_THROUGHPUT_tag
{
   tSIRF_UINT16 seg_stat_max;  /**< Maximum number at which Tracker finished
                                 processing */
   tSIRF_UINT16 seg_stat_lat;  /**< Maximum Tracker interrupt latency over 1
                                 second */
   tSIRF_UINT16 avg_trk_time;  /**< Average time spent in tracker
                                 SegStatTTL/TTLcnt */
   tSIRF_UINT16 last_ms;       /**< ms on which nav finished processing
                                 measurements on the PREVIOUS nav cycle.
                                 range 0-1000 */
}  tSIRF_MSG_SSB_THROUGHPUT;

/*******************************************************************************
   SIRF_MSG_SSB_ERROR                  ( 0x0A ) 10
*******************************************************************************/
#define SIRF_MSG_SSB_MAX_ERROR_PARAMS (6)

#define SIRF_MSG_SSB_ERRID(base,id) (base | id)

/*  err_id bases: */
#define SIRF_MSG_SSB_ERRCB_NOTIFY                    ( 0x0000 )
#define SIRF_MSG_SSB_ERRCB_WARNING                   ( 0x1000 )
#define SIRF_MSG_SSB_ERRCB_ALERT                     ( 0x2000 )

/* err_id: SIRF_MSG_SSB_ERRCB_NOTIFY */
#define SIRF_MSG_SSB_ERRID_NONE                      SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_NOTIFY,0x00)
#define SIRF_MSG_SSB_ERRID_SP_SERIALPARITY           SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_NOTIFY,0x01)
#define SIRF_MSG_SSB_ERRID_CS_SVPARITY               SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_NOTIFY,0x02)
#define SIRF_MSG_SSB_ERRID_KFS_BADALTITUDE           SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_NOTIFY,0x08)
#define SIRF_MSG_SSB_ERRID_RMC_GETTINGPOSITION       SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_NOTIFY,0x09)
#define SIRF_MSG_SSB_ERRID_RXM_TIMEEXCEEDED          SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_NOTIFY,0x0A)
#define SIRF_MSG_SSB_ERRID_RXM_TDOPOVERFLOW          SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_NOTIFY,0x0B)
#define SIRF_MSG_SSB_ERRID_RXM_VALIDDURATIONEXCEEDED SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_NOTIFY,0x0C)
#define SIRF_MSG_SSB_ERRID_STRTP_BADPOSTION          SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_NOTIFY,0x0D)

/* err_id: SIRF_MSG_SSB_ERRCB_WARNING */
#define SIRF_MSG_SSB_ERRID_MI_VCOCLOCKLOST           SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_WARNING,0x01)
#define SIRF_MSG_SSB_ERRID_MI_FALSEACQRECEIVERRESET  SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_WARNING,0x03)
#define SIRF_MSG_SSB_ERRID_KFC_KILLCHANNEL           SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_WARNING,0x04)
#define SIRF_MSG_SSB_ERRID_KRS_BADSOLUTION           SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_WARNING,0x07)
#define SIRF_MSG_SSB_ERRID_STRTP_SRAMCKSUM           SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_WARNING,0x08)
#define SIRF_MSG_SSB_ERRID_STRTP_RTCTIMEINVALID      SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_WARNING,0x09)
#define SIRF_MSG_SSB_ERRID_KFC_BACKUPFAILED_VELOCITY SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_WARNING,0x0a)
#define SIRF_MSG_SSB_ERRID_KFC_BACKUPFAILED_NUMSV    SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_WARNING,0x0b)

/* err_id: SIRF_MSG_SSB_ERRCB_ALERT */
#define SIRF_MSG_SSB_ERRID_MI_BUFFERALLOCFAILURE     SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_ALERT,0x01)
#define SIRF_MSG_SSB_ERRID_MI_UPDATETIMEFAILURE      SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_ALERT,0x02)
#define SIRF_MSG_SSB_ERRID_MI_MEMORYTESTFAILED       SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_ALERT,0x03)
#define SIRF_MSG_SSB_ERRID_MI_WDOREXCEPTIONCONDITION SIRF_MSG_SSB_ERRID(SIRF_MSG_SSB_ERRCB_ALERT,0x04)

typedef struct tSIRF_MSG_SSB_ERROR_tag
{
   tSIRF_UINT16 err_id;
   tSIRF_UINT16 param_cnt;
   tSIRF_UINT32 param[SIRF_MSG_SSB_MAX_ERROR_PARAMS];
}  tSIRF_MSG_SSB_ERROR;

/*******************************************************************************
   SIRF_MSG_SSB_ACK                    ( 0x0B ) 11
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_ACK_tag
{
   tSIRF_UINT8  msg_id;
   tSIRF_UINT8  sub_id;
}  tSIRF_MSG_SSB_ACK;

/*******************************************************************************
   SIRF_MSG_SSB_NAK                    ( 0x0C ) 12
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_NAK_tag
{
   tSIRF_UINT8  msg_id;
   tSIRF_UINT8  sub_id;
}  tSIRF_MSG_SSB_NAK;

/*******************************************************************************
   SIRF_MSG_SSB_VISIBILITY_LIST        ( 0x0D ) 13
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_VISIBILITY_LIST_tag
{
   tSIRF_UINT8  svid_cnt;        /**< number of valid entries in aVis[] element*/
   struct
   {
      tSIRF_UINT8  svid;         /**< sat PRN id */
      tSIRF_INT16 azimuth;
      tSIRF_INT16 elevation;
   } visible[SIRF_MAX_SVID_CNT]; /**< visible sat info */
}  tSIRF_MSG_SSB_VISIBILITY_LIST;

/*******************************************************************************
   SIRF_MSG_SSB_ALMANAC                ( 0x0E ) 14
*******************************************************************************/

#define SIRF_MSG_SSB_ALMANAC_GET_WEEK(week_and_status) \
    ((week_and_status & 0xFFC0) >> 6)
/* 1 = good; 0 = bad */
#define SIRF_MSG_SSB_ALMANAC_GET_STATUS(week_and_status) \
    ((week_and_status & 0x3F) >> 6)
#define SIRF_MSG_SSB_ALMANAC_MAKE_WEEK_AND_STATUS(week,status)\
    (((week & 0x3FF) << 6) | (status & 0x3F))

/* Careful This is NOT the same as SIRF_MSG_SSB_ALMANAC_DATA_ENTRY which is 14 */
#define SIRF_MSG_SSB_ALMANAC_ENTRIES (12)

typedef struct tSIRF_MSG_SSB_ALMANAC_tag
{
   tSIRF_UINT8  svid;
   tSIRF_UINT16 week_and_status;
   tSIRF_UINT16 data[SIRF_MSG_SSB_ALMANAC_ENTRIES];
   tSIRF_UINT16 almanac_checksum;
}  tSIRF_MSG_SSB_ALMANAC;

/*******************************************************************************
   SIRF_MSG_SSB_EPHEMERIS              ( 0x0F ) 15
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_EPHEMERIS_tag
{
   tSIRF_UINT8  svid;
   tSIRF_UINT16 subframe[3][15];
}  tSIRF_MSG_SSB_EPHEMERIS;

/*******************************************************************************
   SIRF_MSG_SSB_RTCM                   ( 0x11 ) 17
*******************************************************************************/
#define SIRF_MSG_SSB_MAX_RTCM_BUFFER (SIRF_MSG_SSB_MAX_MESSAGE_LEN - sizeof(tSIRF_UINT16))
typedef struct tSIRF_MSG_SSB_RTCM_tag
{
   tSIRF_UINT16      buffer_len;
   tSIRF_UINT8       buffer[SIRF_MSG_SSB_MAX_RTCM_BUFFER];
}tSIRF_MSG_SSB_RTCM;

/*******************************************************************************
   SIRF_MSG_SSB_OK_TO_SEND             ( 0x12 ) 18
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_OK_TO_SEND_tag
{
   tSIRF_UINT8 input_enabled;  /**< 1=receiver is not in sleep mode          */
                              /**< 0= receiver is about to be in sleep mode */
}  tSIRF_MSG_SSB_OK_TO_SEND;

/*******************************************************************************
   SIRF_MSG_SSB_RECEIVER_PARAMS        ( 0x13 ) 19
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_RECEIVER_PARAMS_tag
{
   tSIRF_UINT8  reserved_1a;
   tSIRF_UINT16 reserved_1b;
   tSIRF_UINT8  pos_mode_enable;
   tSIRF_UINT8  alt_mode;
   tSIRF_UINT8  alt_src;
   tSIRF_INT16  alt_input;
   tSIRF_UINT8  degraded_mode;
   tSIRF_UINT8  degraded_timeout;
   tSIRF_UINT8  dr_timeout;
   tSIRF_UINT8  trk_smooth;
   tSIRF_UINT8  static_nav_mode;
   tSIRF_UINT8  enable_3sv_lsq;
   tSIRF_UINT32 reserved_2;
   tSIRF_UINT8  dop_mask_mode;
   tSIRF_INT16  nav_elev_mask;
   tSIRF_UINT8  nav_pwr_mask;
   tSIRF_UINT32 reserved_3;
   tSIRF_UINT8  dgps_src;
   tSIRF_UINT8  dgps_mode;
   tSIRF_UINT8  dgps_timeout;
   tSIRF_UINT32 reserved_4;
   tSIRF_UINT8  lp_push_to_fix;
   tSIRF_INT32  lp_on_time;
   tSIRF_INT32  lp_interval;
   tSIRF_UINT8  lp_user_tasks_enabled;
   tSIRF_INT32  lp_user_task_interval;
   tSIRF_UINT8  lp_pwr_cycling_enabled;
   tSIRF_UINT32 lp_max_acq_time;
   tSIRF_UINT32 lp_max_off_time;
   tSIRF_UINT8  apm_enabled_power_duty_cycle;
   tSIRF_UINT16 number_of_fixes;
   tSIRF_UINT16 time_between_fixes;
   tSIRF_UINT8  horz_vert_error_max;
   tSIRF_UINT8  response_time_max;
   tSIRF_UINT8  time_accu_time_duty_cycle_priority;
}  tSIRF_MSG_SSB_RECEIVER_PARAMS;

/*******************************************************************************
   SIRF_MSG_SSB_TEST_MODE_DATA         ( 0x14 ) 20
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_TEST_MODE_DATA_tag
{
   tSIRF_UINT16 svid;                /**< fixed SVid to search on all channels*/
   tSIRF_UINT16 period;              /**< number of seconds statistics are
                                       accumulated over */
   tSIRF_UINT16 bit_synch_time;      /**< time to first bit synch              */
   tSIRF_UINT16 bit_count;           /**< Count of data bits came out during a
                                       period */
   tSIRF_UINT16 poor_status_count;   /**< Count of 100ms periods tracker spent in
                                       any status < 3F */
   tSIRF_UINT16 good_status_count;   /**< Count of 100ms periods tracker spent in
                                       status 3F */
   tSIRF_UINT16 parity_error_count;  /**< Number of word parity errors          */
   tSIRF_UINT16 lost_vco_count;      /**< number of msec VCO lock loss was
                                       detected */
   tSIRF_UINT16 frame_synch_time;    /**< time to first frame synch             */
   tSIRF_INT16 cno_mean;            /**< c/No mean in 0.1 dB-Hz                 */
   tSIRF_INT16 cno_sigma;           /**< c/No sigma in 0.1 dB                   */
   tSIRF_INT16 clock_drift;         /**< clock drift in 0.1 Hz                  */
   tSIRF_INT32 clock_offset_mean;   /**< average clock offset in 0.1 Hz         */

   /**<  For bit test at a high c/no - Test Mode 3 only */
   tSIRF_INT16 bad_1khz_bit_count;  /**< bad bit count out of 10,000
                                       (10 seconds * 1000 bits )     */
   tSIRF_INT32 abs_i20ms;           /**< phase noise estimate I20ms sum         */
   tSIRF_INT32 abs_q1ms;            /**< phase noise estimate Q1ms sum          */
   tSIRF_INT32 reserved[3];
}  tSIRF_MSG_SSB_TEST_MODE_DATA;

/*******************************************************************************
   SIRF_MSG_SSB_DGPS_STATUS            ( 0x1B ) 27
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_DGPS_STATUS_tag
{
   /* this message is not right */
   tSIRF_UINT8  src; /**< Reuse the SIRF_MSG_SSB_DGPS_SRC_<flag> defines */
   tSIRF_UINT8  cor_age[SIRF_NUM_CHANNELS];
   tSIRF_UINT8  reserved[2];
   struct
   {      tSIRF_UINT8  prn;
      tSIRF_INT16 cor;
   } corrections[SIRF_NUM_CHANNELS];
}  tSIRF_MSG_SSB_DGPS_STATUS;

/*******************************************************************************
   SIRF_MSG_SSB_NL_MEAS_DATA           ( 0x1C ) 28
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_NL_MEAS_DATA_tag
{
   tSIRF_UINT8  Chnl;                /**< Channel number which is tracking */
   tSIRF_UINT32 Timetag;             /**< time in ms of the measurement block in
                                       Rx SW time */
   tSIRF_UINT8  svid;                /**< Acquired satellite id */
   tSIRF_DOUBLE gps_sw_time;         /**< GPS Time estimated by software (ms)*/
   tSIRF_DOUBLE pseudorange;         /**< Pseudorange measurment in centimeters */
   tSIRF_FLOAT  carrier_freq;        /**< Either delta PR normalized or freq from
                                       AFC loop */
   tSIRF_DOUBLE carrier_phase;       /**< The integrated carrier phase in
                                       millimeters */
   tSIRF_UINT16 time_in_track;       /**< The count in ms of time in track for SV*/
   tSIRF_UINT8  sync_flags;          /**< 2 bits, bit 0: Coherent Integrat
                                       Interval, bit 1: Sync */
   tSIRF_UINT8  cton[SIRF_NUM_POINTS];/**< avg signal pwr of prev second in dB-Hz
                                       for each channel*/
   tSIRF_UINT16 delta_range_interval;/**< delta PR measurement interval for the
                                       preceding sec */
   tSIRF_UINT16 mean_delta_range_time;/**< mean time of delta PR interval in ms */
   tSIRF_INT16 extrapolation_time;  /**< PR extrap time in ms to reach common
                                       timetag value */
   tSIRF_UINT8  phase_error_count;   /**< cnt of phase errors > 60 deg measured in
                                       preceding sec */
   tSIRF_UINT8  low_power_count;     /**< cnt of power measurements < 28 dB-Hz in
                                       preceding sec */
}  tSIRF_MSG_SSB_NL_MEAS_DATA;

/*******************************************************************************
   SIRF_MSG_SSB_NL_DGPS_DATA           ( 0x1D ) 29
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_NL_DGPS_DATA_tag
{
   tSIRF_INT16 svid;
   tSIRF_INT16 iod;
   tSIRF_UINT8  source;
   tSIRF_FLOAT  pr_correction;   /**< (m)   */
   tSIRF_FLOAT  prr_correction;  /**< (m/s) */
   tSIRF_FLOAT  correction_age;  /**< (s)   */
   tSIRF_FLOAT  prc_variance;    /**< (m)   */
   tSIRF_FLOAT  prrc_variance;   /**< (m/s) */
}  tSIRF_MSG_SSB_NL_DGPS_DATA;

/*******************************************************************************
   SIRF_MSG_SSB_NL_SV_STATE_DATA       ( 0x1E ) 30
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_NL_SV_STATE_DATA_tag
{
   tSIRF_UINT8  svid;
   tSIRF_DOUBLE time;
   tSIRF_DOUBLE pos[3];
   tSIRF_DOUBLE vel[3];
   tSIRF_DOUBLE clk;
   tSIRF_FLOAT  clf;
   tSIRF_UINT8  eph;
   tSIRF_FLOAT  posvar;
   tSIRF_FLOAT  clkvar;
   tSIRF_FLOAT  iono;
}  tSIRF_MSG_SSB_NL_SV_STATE_DATA;

/*******************************************************************************
   SIRF_MSG_SSB_NL_INIT_DATA           ( 0x1F ) 31
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_NL_INIT_DATA_tag
{
   /**< From NL_ControlBlock */
   tSIRF_UINT8  enable_con_alt_mode;
   tSIRF_UINT8  alt_mode;
   tSIRF_UINT8  alt_source;
   tSIRF_FLOAT  altitude;
   tSIRF_UINT8  degraded_mode;
   tSIRF_INT16 degraded_timeout;
   tSIRF_INT16 dr_timeout;
   tSIRF_INT16 coast_timeout;
   tSIRF_UINT8  tracksmooth_mode;
   tSIRF_UINT8  dop_selection;
   tSIRF_INT16 hdop_thresh;
   tSIRF_INT16 gdop_thresh;
   tSIRF_INT16 pdop_thresh;
   tSIRF_UINT8  dgps_selection;
   tSIRF_INT16 dgps_timeout;
   tSIRF_INT16 elev_nav_mask;
   tSIRF_INT16 pow_nav_mask;
   tSIRF_UINT8  editing_residual_mode;
   tSIRF_INT16 editing_residual_threshold;
   tSIRF_UINT8  ssd_mode;
   tSIRF_INT16 ssd_threshold;
   tSIRF_UINT8  static_nav_mode;
   tSIRF_INT16 static_nav_threshold;

   /**< From NL_PositionInitStruct */
   tSIRF_DOUBLE ecef_x;
   tSIRF_DOUBLE ecef_y;
   tSIRF_DOUBLE ecef_z;
   tSIRF_UINT8  position_init_source;

   /**< From NL_TimeInitStruct */
   tSIRF_DOUBLE gps_time;
   tSIRF_INT16 gps_week;
   tSIRF_UINT8  time_init_source;

   /**< From NL_ClockDriftInitStruct */
   tSIRF_DOUBLE clk_offset;
   tSIRF_UINT8  clk_offset_init_source;
}  tSIRF_MSG_SSB_NL_INIT_DATA;


/*******************************************************************************
   SIRF_MSG_SSB_NL_AUX_INIT_DATA           0x40 0x01
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_NL_AUX_INIT_DATA_tag
{
   tSIRF_UINT32 time_init_unc;         /**< Initial time uncertainty (usec) */
   tSIRF_UINT16 saved_pos_week;        /**< Week of saved position */
   tSIRF_UINT32 saved_pos_tow;         /**< Time of week of saved position (sec) */
   tSIRF_UINT16 saved_pos_ehe;         /**< EHE of saved position (100m) */
   tSIRF_UINT16 saved_pos_eve;         /**< EVE of saved position (m) */
   tSIRF_UINT8  sw_version;            /**< Tracker SW version */
   tSIRF_UINT8  icd_version;           /**< Tracker ICD version */
   tSIRF_UINT16 chip_version;          /**< Tracker HW Chip version */
   tSIRF_UINT32 acq_clk_speed;         /**< Default rate of Tracker's internal clock (Hz) */
   tSIRF_UINT32 default_clock_offset;  /**< Default freq offset of Tracker's internal clock (Hz) */
   tSIRF_UINT32 tracker_status;        /**< Tracker Status:
                                          Bit 0: Status 0=good; 1=bad
                                          Bit 1: Cache  0=Disable; 1=Enable
                                          Bit 2: RTC    0=Invalid; 1=Valid
                                          Bit 3-31: reserved */
   tSIRF_UINT32 reserved;
}  tSIRF_MSG_SSB_NL_AUX_INIT_DATA;

/*******************************************************************************
   SIRF_MSG_SSB_NL_AUX_MEAS_DATA           0x40 0x02
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_NL_AUX_MEAS_DATA_tag
{
   tSIRF_UINT8  sv_prn;              /**< Satellite PRN number */
   tSIRF_UINT8  status;              /**< Tracker system status:
                                        0x01 Trickle Power Active
                                        0x02 Scalable Tracking Loop Active
                                        0x04 SCL_MEAS Active */
   tSIRF_UINT8  extended_status;     /**< Tracker channel status:
                                        0x02 Subframe sync verified
                                        0x04 Possible cycle slip
                                        0x08 Subframe sync lost
                                        0x10 Multipath detected
                                        0x20 Multipath-only detected
                                        0x40 Weak frame sync done */
   tSIRF_UINT8  bit_sync_qual;       /**< Confidence metric for bitsync */
   tSIRF_UINT32 time_tag;            /**< Measurement time tag (acqclk) */
   tSIRF_UINT32 code_phase;          /**< Code Phase (2^-11 chip) */
   tSIRF_INT32  carrier_phase;       /**< Carrier Phase (L1 cycle) */
   tSIRF_INT32  carrier_freq;        /**< Carrier Frequency (0.000476Hz) */
   tSIRF_INT16  carrier_accel;       /**< Doppler Rate (0.1m/s/s) */
   tSIRF_INT16  ms_num;              /**< Millisecond number (0 to 19) */
   tSIRF_INT32  bit_num;             /**< Bit number (0 to 30239999) */
   tSIRF_INT32  code_correction;     /**< For code smoothing (cycle) */
   tSIRF_INT32  smooth_code;         /**< For PR smoothing (2^-10 cycle) */
   tSIRF_INT32  code_offset;         /**< Code offset (2^-11 chip)*/
   tSIRF_INT16  pseudorange_noise;   /**< Pseudorange noise estimate */
   tSIRF_INT16  delta_range_qual;    /**< Deltarange accuracy estimate */
   tSIRF_INT16  phase_lock_qual;     /**< Phase lock accuracy estimate */
   tSIRF_INT16  ms_uncertainty;      /**< Millisecond uncertainty */
   tSIRF_UINT16 sum_abs_I;           /**< Sum |I| for this measurement */
   tSIRF_UINT16 sum_abs_Q;           /**< Sum |Q| for this measurement */
   tSIRF_INT32  sv_bit_num;          /**< Bit number of last SV bit */
   tSIRF_INT16  mp_los_det_value;    /**< Multipath line-of-sight detection value */
   tSIRF_INT16  mp_only_det_value;   /**< Multipath-only line-of-sight detection value */
   tSIRF_UINT8  recovery_status;     /**< Recovery status:
                                        0x01  WBS active
                                        0x02  False Lock
                                        0x04  Bad prepos, wrong BS
                                        0x08  Bad prepos, wrong FS
                                        0x10  Bad prepos, others */
   tSIRF_UINT32 sw_time_uncertainty; /**< SW time uncertainty (usec) */
}  tSIRF_MSG_SSB_NL_AUX_MEAS_DATA;

/*******************************************************************************
   SIRF_MSG_SSB_NL_AUX_AID_DATA           0x40 0x03
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_NL_AUX_AID_DATA_tag
{
   tSIRF_INT32  ecef_x;         /**< ECEF X position (m) */
   tSIRF_INT32  ecef_y;         /**< ECEF Y position (m) */
   tSIRF_INT32  ecef_z;         /**< ECEF Z position (m) */
   tSIRF_UINT32 horiz_pos_unc;  /**< Horizontal position uncertainty (m) */
   tSIRF_UINT16 alt_unc;        /**< Altitude uncertainty (m) */
   tSIRF_UINT32 sw_tow;         /**< Time of the week (msec) */
}  tSIRF_MSG_SSB_NL_AUX_AID_DATA;

/*******************************************************************************
   SIRF_MSG_SSB_GEODETIC_NAVIGATION    ( 0x29 ) 41
*******************************************************************************/

/** Nav Valid field */
#define SIRF_MSG_SSB_VALID_NO_TRACKER           ( 0x8000 )
#define SIRF_MSG_SSB_TRACKER_LOADING            ( 0x4000 )
#define SIRF_MSG_SSB_GPS_TEST_MODE              ( 0x2000 )
#define SIRF_MSG_SSB_GPS_FIX_INVALID            ( 0x0001 )
/* Bit 8: Almanac-based positioning, or not = not set */
#define SIRF_MSG_SSB_GN_ALMANAC_BASED_POSITIONING      (0x0100)
/* Bit 9: WiFi-tagged coarse position, or not = not set */
#define SIRF_MSG_SSB_GN_WIFI_COARSE_POSITION           (0x0200)
/* Bit 10: APM QoP spec met but not QoP posn request, or else = not set */
#define SIRF_MSG_SSB_GN_APM_QOP_POSN_REQ_NOT_MET       (0x0400)
/* Bit 11: Reverse EE processing allowed, or not = not set */
#define SIRF_MSG_SSB_GN_REVERSE_EE_ALLOWED             (0x0800)

/** Additional mode information */

/* Bit 0: Feedback enabled,  or not = not set */
#define SIRF_MSG_SSB_GN_MAP_MATCHING_FEEDBACK_ENABLED  (0x01)
/* Bit 1: Feedback received, or not = not set */
#define SIRF_MSG_SSB_GN_MAP_MATCHING_FEEDBACK_RECEIVED (0x02)
/* Bit 2: Map Matching used in position compution, or not = not set */
#define SIRF_MSG_SSB_GN_MAP_MATCHING_USED_IN_POSITION  (0x04)
/* Bit 3: GPS time and week set, or not = not set */
#define SIRF_MSG_SSB_GN_GPS_TIME_AND_WEEK_SET          (0x08)
/* Bit 4: Verified by satellite, or not = not set */
#define SIRF_MSG_SSB_GN_UTC_OFFSET_VERIFIED            (0x10)
/* Bit 5: Indicate SBAS range measurement used in Nav solution */
#define SIRF_MSG_SSB_GN_SBAS_RANGING                   (0x20)
/* Bit 6: Car Bus signal enabled , or not = not set */
#define SIRF_MSG_SSB_GN_CAR_BUS_SIGNAL_ENABLED         (0x40)
/* Bit 7: DR direction = reverse, or forward = not set */
#define SIRF_MSG_SSB_GN_DR_DIRECTION_REVERSE           (0x80)

typedef struct tSIRF_MSG_SSB_GEODETIC_NAVIGATION_tag
{
   tSIRF_UINT16 nav_valid;  /**< GPS validity bits; 0 is all valid               */
   tSIRF_UINT16 nav_mode;   /**< GPS mode flags                                  */
   tSIRF_UINT16 gps_week;   /**< GPS week number in weeks                        */
   tSIRF_UINT32 gps_tow;    /**< GPS time of week in 1 ms, 0..604,799,000 ms     */
   tSIRF_UINT16 utc_year;
   tSIRF_UINT8  utc_month;
   tSIRF_UINT8  utc_day;
   tSIRF_UINT8  utc_hour;
   tSIRF_UINT8  utc_min;
   tSIRF_UINT16 utc_sec;
   tSIRF_UINT32 sv_used;    /**< SVs used in fix (bitmap) */
   tSIRF_INT32 lat;        /**< Latitude in 1e-7 degrees,
                              -900,000,000..900,000,000 */
   tSIRF_INT32 lon;        /**< Longitude in 1e-7 degrees,
                              -1,800,000,000..1,800,000,000 */
   tSIRF_INT32 alt_ellips; /**< Altitude from Ellipsoid in 0.01 meters,
                              -200,000..10,000,000 */
   tSIRF_INT32 alt_msl;    /**< Altitude from Mean Sea Level in 0.01 meters,
                              -200,000..10,000,000 */
   tSIRF_UINT8  datum;
   tSIRF_UINT16 sog;        /**< Speed Over Ground in 0.01 meters/sec, 0..65535  */
   tSIRF_UINT16 hdg;        /**< Heading, from True North in 0.01 degrees,
                              0..36,000 */
   tSIRF_INT16 mag_var;
   tSIRF_INT16 climb_rate;
   tSIRF_INT16 heading_rate;
   tSIRF_UINT32 ehpe;
   tSIRF_UINT32 evpe;
   tSIRF_UINT32 ete;
   tSIRF_UINT16 ehve;
   tSIRF_INT32 clk_bias;
   tSIRF_UINT32 clk_bias_error;
   tSIRF_INT32 clk_offset;
   tSIRF_UINT32 clk_offset_error;
   tSIRF_UINT32 distance_travelled;
   tSIRF_UINT16 distance_travelled_error;
   tSIRF_UINT16 heading_error;
   tSIRF_UINT8  sv_used_cnt;/**< Number of SVs used in fix                       */
   tSIRF_UINT8  hdop;       /**< Horizontal Dillution Of Precision               */
   tSIRF_UINT8  additional_mode_info;  /**<  Additional mode information         */
}  tSIRF_MSG_SSB_GEODETIC_NAVIGATION;
/*******************************************************************************
 SIRF_MSG_SSB_QUEUE_CMD_PARAM        ( 0x2B ) 0x43
 ******************************************************************************/
/* This message contains the Polled Msg ID as the message Sub ID below */
/*******************************************************************************
     SIRF_MSG_SSB_QUEUE_CMD_NI       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0X80)
 ******************************************************************************/
#define tSIRF_MSG_SSB_QUEUE_CMD_NI tSIRF_MSG_SSB_INITIALIZE
/*******************************************************************************
     SIRF_MSG_SSB_QUEUE_CMD_DGPS_SRC SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0X85)
 ******************************************************************************/
#define tSIRF_MSG_SSB_QUEUE_CMD_DGPS_SRC tSIRF_MSG_SSB_SET_DGPS_SOURCE
/*******************************************************************************
     SIRF_MSG_SSB_QUEUE_CMD_SNM      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 088)
 ******************************************************************************/
#define tSIRF_MSG_SSB_QUEUE_CMD_SNM tSIRF_MSG_SSB_SET_NAV_MODE
/*******************************************************************************
     SIRF_MSG_SSB_QUEUE_CMD_SDM      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 089)
 ******************************************************************************/
#define tSIRF_MSG_SSB_QUEUE_CMD_SDM tSIRF_MSG_SSB_SET_DOP_MODE
/*******************************************************************************
     SIRF_MSG_SSB_QUEUE_CMD_SDGPSM   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0X8A)
 ******************************************************************************/
#define tSIRF_MSG_SSB_QUEUE_CMD_SDGPSM tSIRF_MSG_SSB_SET_DGPS_MODE
/*******************************************************************************
     SIRF_MSG_SSB_QUEUE_CMD_SEM      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x8B)
 ******************************************************************************/
#define tSIRF_MSG_SSB_QUEUE_CMD_SEM tSIRF_MSG_SSB_SET_ELEV_MASK
/*******************************************************************************
     SIRF_MSG_SSB_QUEUE_CMD_SPM      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x8C)
 ******************************************************************************/
#define tSIRF_MSG_SSB_QUEUE_CMD_SPM tSIRF_MSG_SSB_SET_POWER_MASK
/*******************************************************************************
     SIRF_MSG_SSB_QUEUE_CMD_SSN      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x8F)
 ******************************************************************************/
#define tSIRF_MSG_SSB_QUEUE_CMD_SSN tSIRF_MSG_SSB_SET_STAT_NAV
/*******************************************************************************
     SIRF_MSG_SSB_QUEUE_CMD_LP       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0x97)
 ******************************************************************************/
#define tSIRF_MSG_SSB_QUEUE_CMD_LP tSIRF_MSG_SSB_SET_LOW_POWER
/*******************************************************************************
     SIRF_MSG_SSB_QUEUE_CMD_SSBAS    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2B, 0AA)
 ******************************************************************************/
#define tSIRF_MSG_SSB_QUEUE_CMD_SSBAS tSIRF_MSG_SSB_SET_SBAS_PRN

/*******************************************************************************
   SIRF_MSG_SSB_ADC_ODOMETER_DATA          ( 0x2D ) 45
*******************************************************************************/

#define SIRF_MSG_SSB_DR_DATASET_LENGTH 10

typedef struct tSIRF_MSG_SSB_ADC_ODOMETER_DATA_tag /* 10Hz */
{
   tSIRF_UINT32 current_time;     /**< Tracker Time, millisecond counts */
   tSIRF_INT16 adc2_avg;         /**< Averaged measurement from ADC[2] input */
   tSIRF_INT16 adc3_avg;         /**< Averaged measurement from ADC[3] input */
   tSIRF_UINT16 odo_count;        /**< Odometer counter measurement
                                    at the most recent 100ms tracker input */
   tSIRF_UINT8  gpio_stat;        /**< GPIO input states
                                    at the most recent 100ms tracker input:
                                    bit 0: GPIO[0]  input
                                    bit 1: GPIO[2]  input
                                    bit 2: GPIO[3]  input
                                    bit 3: GPIO[4]  input
                                    bit 4: GPIO[5]  input
                                    bit 5: GPIO[7]  input
                                    bit 6: GPIO[8]  input
                                    bit 7: GPIO[12] input */
}  tSIRF_MSG_SSB_ADC_ODOMETER_DATA;

typedef struct tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ_tag /**< 1Hz-10 measurements per sec*/
{
   tSIRF_MSG_SSB_ADC_ODOMETER_DATA dataset[SIRF_MSG_SSB_DR_DATASET_LENGTH];
}  tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ;
/*******************************************************************************
  SIRF_MSG_SSB_TEST_MODE_DATA_3       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x2E,    0)
*******************************************************************************/
#define SIRF_MSG_SSB_TEST_MODE_PHASE_LOCK_SCALE        (0.001)
typedef struct tSIRF_MSG_SSB_TEST_MODE_DATA_3_tag
{
   tSIRF_UINT16 svid;                /**< fixed SVid to search on all channels*/
   tSIRF_UINT16 period;              /**< number of seconds statistics are
                                       accumulated over */
   tSIRF_UINT16 bit_synch_time;      /**< time to first bit synch              */
   tSIRF_UINT16 bit_count;           /**< Count of data bits came out during a
                                       period */
   tSIRF_UINT16 poor_status_count;   /**< Count of 100ms periods tracker spent in
                                       any status < 3F */
   tSIRF_UINT16 good_status_count;   /**< Count of 100ms periods tracker spent in
                                       status 3F */
   tSIRF_UINT16 parity_error_count;  /**< Number of word parity errors          */
   tSIRF_UINT16 lost_vco_count;      /**< number of msec VCO lock loss was
                                       detected */
   tSIRF_UINT16 frame_synch_time;    /**< time to first frame synch             */
   tSIRF_INT16 cno_mean;            /**< c/No mean in 0.1 dB-Hz                 */
   tSIRF_INT16 cno_sigma;           /**< c/No sigma in 0.1 dB                   */
   tSIRF_INT16 clock_drift;         /**< clock drift in 0.1 Hz                  */
   tSIRF_INT32 clock_offset;        /**< clock offset in 0.1 Hz         */

   /*  For bit test at a high c/no - Test Mode 3 only */
   tSIRF_INT16 bad_1khz_bit_count;  /**< bad bit count out of 10,000
                                       (10 seconds * 1000 bits )     */
   tSIRF_INT32 abs_i20ms;           /**< phase noise estimate I20ms sum         */
   tSIRF_INT32 abs_q20ms;           /**< phase noise estimate Q20ms sum          */

   tSIRF_INT32 phase_lock;          /**< phase lock indicator. LSB = 0.001 */
   tSIRF_UINT16 rtc_frequency;      /**<  RTC Frequency. Unit: Hz */
   tSIRF_UINT16 e_to_acq_ratio;     /**<  ECLK to ACQ Clock ratio  */
   tSIRF_UINT8  t_sync_agc_gain;    /**<  Tsync and AGC Gain value */
   tSIRF_UINT8  tm_5_ready;         /**<  Ready for TM5 switch? >= 0x80
                                       Yes, else No */
   tSIRF_UINT16 ClkDriftUnc;        /**< Clock Drift (Frequency) Uncertainty in Hz */

} tSIRF_MSG_SSB_TEST_MODE_DATA_3;

/*******************************************************************************
   SIRF_MSG_SSB_DR_OUTPUT              ( 0x30 ) 48
*******************************************************************************/

/*******************************************************************************
       SIRF_MSG_SSB_DR_NAV_STATUS         ( 0x0130 ) 48 SID 1
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_NAV_STATUS_tag
{
   tSIRF_UINT8  nav;
   tSIRF_UINT16 data;
   tSIRF_UINT8  cal_gb_cal;
   tSIRF_UINT8  gsf_cal_ssf_cal;
   tSIRF_UINT8  nav_across_reset_pos;
   tSIRF_UINT8  hd;
   tSIRF_UINT8  gyr_sub_op_spd_sub_op;
   tSIRF_UINT8  nav_st_int_ran_z_gb_cal_upd;
   tSIRF_UINT8  gbsf_cal_upd_spd_cal_upd_upd_nav_st;
   tSIRF_UINT8  gps_upd_pos;
   tSIRF_UINT8  gps_upd_hd;
   tSIRF_UINT8  gps_pos_gps_vel;

   /* Where did these fields come from? */
   tSIRF_UINT8  dws_hd_rt_s_f_cal_valid;
   tSIRF_UINT8  dws_hd_rt_s_f_cal_upd;
   tSIRF_UINT16 dws_spd_s_f_cal_valid;
   tSIRF_UINT8  dws_spd_s_f_cal_upd;
}  tSIRF_MSG_SSB_DR_NAV_STATUS;

/*******************************************************************************
       SIRF_MSG_SSB_DR_NAV_STATE          ( 0x0230 ) 48 SID 2
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_NAV_STATE_tag
{
   tSIRF_UINT16 spd;
   tSIRF_UINT16 spd_e;
   tSIRF_INT16 ssf;
   tSIRF_UINT16 ssf_e;
   tSIRF_INT16 hd_rte;
   tSIRF_UINT16 hd_rte_e;
   tSIRF_INT16 gb;
   tSIRF_UINT16 gbE;
   tSIRF_INT16 gsf;
   tSIRF_UINT16 gsf_e;
   tSIRF_UINT32 tpe;
   tSIRF_UINT16 the;
   tSIRF_UINT8  nav_ctrl;
   tSIRF_UINT8  reverse;
   tSIRF_UINT16 hd;
   /* where did these fields come from */
   tSIRF_UINT8  sensor_pkg; /**< Identify which sensor package used:
                           *   0 = Gyro and Odo
                           *   1 = Wheel Speed and Odo
                           */
   tSIRF_UINT16 odo_spd;
   tSIRF_INT16 odo_spd_s_f;
   tSIRF_UINT16 odo_spd_s_f_err;
   tSIRF_INT16 lf_wheel_spd_sf;
   tSIRF_UINT16 lf_wheel_spd_sf_err;
   tSIRF_INT16 rf_wheel_spd_sf;
   tSIRF_UINT16 rf_wheel_spd_sf_err;
   tSIRF_INT16 lr_wheel_spd_sf;
   tSIRF_UINT16 lr_wheel_spd_sf_err;
   tSIRF_INT16 rr_wheel_spd_sf;
   tSIRF_UINT16 rr_wheel_spd_sf_err;
   tSIRF_INT16 rear_axle_spd_delta;
   tSIRF_UINT16 rear_axle_avg_spd;
   tSIRF_UINT16 rear_axle_spd_err;
   tSIRF_INT16 rear_axle_hd_rt;
   tSIRF_UINT16 rear_axle_hd_rt_err;
   tSIRF_INT16 front_axle_spd_delta;
   tSIRF_UINT16 front_axle_avg_spd;
   tSIRF_UINT16 front_axle_spd_err;
   tSIRF_INT16 front_axle_hd_rt;
   tSIRF_UINT16 front_axle_hd_rt_err;
}  tSIRF_MSG_SSB_DR_NAV_STATE;

/*******************************************************************************
       SIRF_MSG_SSB_DR_NAV_SUBSYS         ( 0x0330 ) 48 SID 3
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_NAV_SUBSYS_tag
{
   tSIRF_INT16 gps_hd_rte;
   tSIRF_UINT16 gps_hd_rte_e;
   tSIRF_UINT16 gps_hd;
   tSIRF_UINT16 gps_hd_e;
   tSIRF_UINT16 gps_spd;
   tSIRF_UINT16 gps_spd_e;
   tSIRF_UINT32 gps_pos_e;
   tSIRF_INT16 dr_hd_rte;
   tSIRF_UINT16 dr_hd_rte_e;
   tSIRF_UINT16 dr_hd;
   tSIRF_UINT16 dr_hd_e;
   tSIRF_UINT16 dr_spd;
   tSIRF_UINT16 dr_spd_e;
   tSIRF_UINT32 dr_pos_e;
   tSIRF_UINT8  reserved[2];
}  tSIRF_MSG_SSB_DR_NAV_SUBSYS;

/*******************************************************************************
       SIRF_MSG_SSB_DR_VALID              ( 0x0530 ) 48 SID 5
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_VALID_tag
{
   tSIRF_UINT32 valid;  /**< bit  0: invalid position */
                       /**< bit  1: invalid position error */
                       /**< bit  2: invalid heading */
                       /**< bit  3: invalid heading error */
                       /**< bit  4: invalid speed scale factor */
                       /**< bit  5: invalid speed scale factor error */
                       /**< bit  6: invalid gyro bias */
                       /**< bit  7: invalid gyro bias error */
                       /**< bit  8: invalid gyro scale factor */
                       /**< bit  9: invalid gyro scale factor error */
                       /**< bit 10: invalid baseline speed scale factor */
                       /**< bit 11: invalid baseline gyro bias */
                       /**< bit 12: invalid baseline gyro scale factor */
                       /**< bit 13 - 31: reserved */
   tSIRF_UINT32 reserved;
}  tSIRF_MSG_SSB_DR_VALID;

/*******************************************************************************
       SIRF_MSG_SSB_DR_GYR_FACT_CAL       ( 0x0630 ) 48 SID 6
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_GYR_FACT_CAL_tag
{
   tSIRF_UINT8  cal;  /**< bit 0: start gyro bias calibration */
                     /**< bit 1: start gyro scale factor calibration */
   tSIRF_UINT8  reserved;
}  tSIRF_MSG_SSB_DR_GYR_FACT_CAL;

/*******************************************************************************
       SIRF_MSG_SSB_DR_SENS_PARAM         ( 0x0730 ) 48 SID 7
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_SENS_PARAM_tag
{
   tSIRF_UINT8  base_ssf;  /**< in ticks/m */
   tSIRF_UINT16 base_gb;   /**< in zero rate volts */
   tSIRF_UINT16 base_gsf;  /**< in mV / (deg/s) */
}  tSIRF_MSG_SSB_DR_SENS_PARAM;

/*******************************************************************************
       SIRF_MSG_SSB_DR_DATA_BLK           ( 0x0830 ) 48 SID 8
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_DATA_BLK_tag
{
   tSIRF_UINT8  meas_type;
   tSIRF_UINT8  valid_cnt;
   tSIRF_UINT16 bkup_flgs;
   struct tSIRF_DR_DATA_tag
   {
      tSIRF_UINT32 tag;
      tSIRF_UINT16 spd;
      tSIRF_INT16 hd_rte;
   } blk[SIRF_MSG_SSB_DR_DATASET_LENGTH];
}  tSIRF_MSG_SSB_DR_DATA_BLK;

/*******************************************************************************
       SIRF_MSG_SSB_MMF_STATUS            ( 0x5030 ) 48 SID 80
*******************************************************************************/
/* Map matching is enabled */
#define SIRF_MSG_SSB_STATUS_MMF_ENABLED                  0x80000000
/* Map matching calibration is enabled. */
#define SIRF_MSG_SSB_STATUS_MMF_CALIBRATION_ENABLED      0x40000000
/* Map matching retroloop is enabled. */
#define SIRF_MSG_SSB_STATUS_MMF_RETROLOOP_ENABLED        0x20000000
/* We received a MMF packet */
#define SIRF_MSG_SSB_STATUS_GOT_DATA                     0x10000000
/* Altitude updated with MMF data */
#define SIRF_MSG_SSB_STATUS_SYSTEM_ALTITUDE_VALID        0x08000000
/* Heading updated with MMF data */
#define SIRF_MSG_SSB_STATUS_SYSTEM_HEADING_VALID         0x04000000
/* Position updated with MMF data */
#define SIRF_MSG_SSB_STATUS_SYSTEM_POSITION_VALID        0x02000000
/* Incorrect number of data sets inside MMF packet. */
#define SIRF_MSG_SSB_STATUS_INVALID_DATA_SIZE            0x01000000
#define SIRF_MSG_SSB_STATUS_HEADING_OUT_OF_RANGE         0x00800000
#define SIRF_MSG_SSB_STATUS_HEADING_DELTA_TOO_BIG        0x00400000
#define SIRF_MSG_SSB_STATUS_POSITION_DRIFT_TOO_BIG       0x00200000
/* Indicates that newer MMF packet(s) has overwritten older one before data
   was processed */
#define SIRF_MSG_SSB_STATUS_DATA_OVERFLOW                0x00100000
/* Indicates that received data was too old for processing */
#define SIRF_MSG_SSB_STATUS_DATA_TOO_OLD                 0x00080000
/* Nav was updated with MMF feedback */
#define SIRF_MSG_SSB_STATUS_NAV_UPDATED                  0x00040000
/* Nav is valid */
#define SIRF_MSG_SSB_STATUS_NAV_VALID                    0x00020000
/* Copies of MMF data packet control register */
#define SIRF_MSG_SSB_DATA_ALTITUDE_VALID                 0x00000020
#define SIRF_MSG_SSB_DATA_HEADING_VALID                  0x00000010
#define SIRF_MSG_SSB_DATA_POSITION_VALID                 0x00000008
#define SIRF_MSG_SSB_DATA_ALTITUDE_FORCED                0x00000004
#define SIRF_MSG_SSB_DATA_HEADING_FORCED                 0x00000002
#define SIRF_MSG_SSB_DATA_POSITION_FORCED                0x00000001

typedef struct tSIRF_MSG_SSB_MMF_STATUS_tag
{
   tSIRF_UINT32   status;     /**< Status see above for bit definitions */
   tSIRF_UINT16   sys_hdg;    /**< For the last received MMF packet this was the
                              * system's heading, LLA and TOW */
   tSIRF_INT32   sys_lat;
   tSIRF_INT32   sys_lon;
   tSIRF_INT32   sys_alt;
   tSIRF_UINT32   sys_tow;
   tSIRF_UINT16   mmf_hdg;    /**< For the last received MMF packet this was the
                              * MMF's heading, LLA and TOW */
   tSIRF_INT32   mmf_lat;
   tSIRF_INT32   mmf_lon;
   tSIRF_INT32   mmf_alt;
   tSIRF_UINT32   mmf_tow;
}  tSIRF_MSG_SSB_MMF_STATUS;

/*******************************************************************************
   SIRF_MSG_SSB_CW_DATA           0x5C 0x01
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_CW_DATA_tag
{
   tSIRF_INT32 freq[SIRF_CW_PEAKS];
   tSIRF_UINT16 cno[SIRF_CW_PEAKS];
}  tSIRF_MSG_SSB_CW_DATA;

/*******************************************************************************
   SIRF_MSG_SSB_CW_FILTER         0x5C 0x02
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_CW_FILTER_tag
{
   tSIRF_UINT8 sampling;
   tSIRF_UINT8 adc;
   tSIRF_INT8  freqbin[SIRF_CW_PEAKS];
   tSIRF_UINT8 nbin[SIRF_CW_PEAKS];
}  tSIRF_MSG_SSB_CW_FILTER;

/*******************************************************************************
   SIRF_MSG_SSB_SBAS_PARAM                 ( 0x32 ) 50
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SBAS_PARAM_tag
{
   tSIRF_UINT8  prn;      /**< PRN                                               */
   tSIRF_UINT8  mode;     /**< 0=TESTING, 1=INTEGRITY                            */
   tSIRF_UINT8  timeout;  /**< 1 - 255 seconds;
                               zero will bring the timeout value to default      */
   tSIRF_UINT8  flg_bits; /**< bit0: Timeout;     0=default; 1=user              */
                          /**< bit1: Hlth;        0=ON; 1=OFF                    */
                          /**< bit2: Corrections; 0=ON; 1=OFF                    */
                          /**< bit3: SBAS PRN;    0=AutoScan; 1=Manual (user)    */
   tSIRF_UINT8  spare[8]; /**< ignore these bits */
}  tSIRF_MSG_SSB_SBAS_PARAM;

/*******************************************************************************
   SIRF_MSG_SSB_SIRFNAV_NOTIFICATION   ( 0x33 ) 51
      SIRF_MSG_SSB_SIRFNAV_COMPLETE      ( 0x0133 )
      SIRF_MSG_SSB_SIRFNAV_TIMING        ( 0x0233 )
      SIRF_MSG_SSB_DEMO_TIMING           ( 0x0333 )
      SIRF_MSG_SSB_SIRFNAV_TIME_TAGS     ( 0x0433 )
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SIRFNAV_COMPLETE_tag
{
   tSIRF_UINT8  reserved;
}  tSIRF_MSG_SSB_SIRFNAV_COMPLETE;

typedef struct tSIRF_MSG_SSB_SIRFNAV_TIME_TAGS_tag
{
   tSIRF_UINT32 tracker_time_tag;  /**< Tracker's time tag in [ms]*/
   tSIRF_UINT16 gps_week;          /**< GPS week number in weeks */
   tSIRF_UINT32 gps_tow;           /**< GPS time of week in 1 ms,
                                     0..604,799,000 ms */
   tSIRF_UINT8  tracker_rtc_day;
   tSIRF_UINT8  tracker_rtc_hour;
   tSIRF_UINT8  tracker_rtc_min;
   tSIRF_UINT8  tracker_rtc_sec;
   tSIRF_UINT16 tracker_rtc_clkctr;
   tSIRF_UINT8  tracker_rtc_ok;
   tSIRF_UINT8  tracker_rtc_rollover_complete;
   tSIRF_UINT32 tracker_rtc_rollover_seconds;
} tSIRF_MSG_SSB_SIRFNAV_TIME_TAGS;


#define SIRF_MSG_SSB_TRACKER_LOADER_STATE_START                           0
#define SIRF_MSG_SSB_TRACKER_LOADER_STATE_RESET                           1
#define SIRF_MSG_SSB_TRACKER_LOADER_STATE_PRE_LOAD_CRC_CHECK_TO_START     2
#define SIRF_MSG_SSB_TRACKER_LOADER_STATE_PRE_LOAD_CRC_CHECK_FINISHED   101
#define SIRF_MSG_SSB_TRACKER_LOADER_STATE_CRC_CHECK_PASSED              102
#define SIRF_MSG_SSB_TRACKER_LOADER_STATE_CRC_CHECK_FAILED              103
#define SIRF_MSG_SSB_TRACKER_LOADER_STATE_LOAD_TO_START                   3
#define SIRF_MSG_SSB_TRACKER_LOADER_STATE_LOAD_IN_PROGRESS                4
#define SIRF_MSG_SSB_TRACKER_LOADER_STATE_LOAD_FINISHED                 106
#define SIRF_MSG_SSB_TRACKER_LOADER_STATE_POST_LOAD_CRC_CHECK_TO_START  107
#define SIRF_MSG_SSB_TRACKER_LOADER_STATE_POST_LOAD_CRC_CHECK_FINISHED  108
#define SIRF_MSG_SSB_TRACKER_LOADER_STATE_PROGRAM_TO_START                5
#define SIRF_MSG_SSB_TRACKER_LOADER_STATE_TARGET_TO_SLEEP               110
#define SIRF_MSG_SSB_TRACKER_LOADER_STATE_FINISHED                        6
#define SIRF_MSG_SSB_TRACKER_LOADER_STATE_FAILED                          7

typedef struct tSIRF_MSG_SSB_TRACKER_LOADER_STATE_tag
{
   tSIRF_UINT32 loader_state;
   tSIRF_UINT32 percentage_loaded;
   tSIRF_UINT32 loader_error;
   tSIRF_UINT32 time_tag;

} tSIRF_MSG_SSB_TRACKER_LOADER_STATE;


typedef struct tSIRF_MSG_SSB_SIRFNAV_START_tag
{
   tSIRF_UINT32 start_mode;
   tSIRF_UINT32 clock_offset;
   tSIRF_UINT32 port_num;
   tSIRF_UINT32 baud_rate;
}  tSIRF_MSG_SSB_SIRFNAV_START;


typedef struct tSIRF_MSG_SSB_SIRFNAV_STOP_tag
{
   tSIRF_UINT32 stop_mode;
}  tSIRF_MSG_SSB_SIRFNAV_STOP;


/*******************************************************************************
      SIRF_MSG_SSB_STARTUP_INFO      ( 0x0136 ) 54 SID 1
*******************************************************************************/

/* GPS start mode */
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_SNAP             (0x00)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_HOT              (0x01)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_WARM             (0x02)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_WARM_WITH_INIT   (0x03)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_COLD             (0x04)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_FACTORY          (0x05)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_FAST             (0x06)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_INVALID          (0x0F)

/* start reason */
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_EXTERNAL         (0x00)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_INTERNAL         (0x01 << 4)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_PTF              (0x02 << 4)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_APM              (0x03 << 4)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_INITIAL          (0x04 << 4)

/* restart type */
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_GPS_ONLY_RESTART (0x00)
#define SIRF_MSG_SSB_STARTUP_MSG_MODE_SYSTEM_RESTART   (0x01 << 7)

/* rtc data validity */
#define SIRF_MSG_SSB_STARTUP_MSG_STATUS_RTC_DATA_INVALID     (0x00)
#define SIRF_MSG_SSB_STARTUP_MSG_STATUS_RTC_DATA_VALID       (0x01)

/* storage data validity */
#define SIRF_MSG_SSB_STARTUP_MSG_STATUS_STORAGE_DATA_INVALID (0x00)
#define SIRF_MSG_SSB_STARTUP_MSG_STATUS_STORAGE_DATA_VALID   (0x01 << 1)

/* flash data validity */
#define SIRF_MSG_SSB_STARTUP_MSG_STATUS_FLASH_DATA_INVALID   (0x00)
#define SIRF_MSG_SSB_STARTUP_MSG_STATUS_FLASH_DATA_VALID     (0x01 << 2)

/* dr status validity */
#define SIRF_MSG_SSB_STARTUP_MSG_STATUS_DR_STATUS_INVALID (0x00)
#define SIRF_MSG_SSB_STARTUP_MSG_STATUS_DR_STATUS_VALID   (0x01)

typedef struct tSIRF_MSG_SSB_STARTUP_INFO_tag
{
   tSIRF_UINT8  mode;          /**< Bit field:*/
                              /**< Bits 3..0 - GPS start mode*/
                              /**<         0 - snap*/
                              /**<         1 - hot*/
                              /**<         2 - warm*/
                              /**<         3 - warm with initialization*/
                              /**<         4 - cold*/
                              /**<         5 - factory*/
                              /**<         6 - fast*/
                              /**< Bits 6..4 - start reason*/
                              /**<         0 - commanded externally*/
                              /**<         1 - internal reason (recovery mode)*/
                              /**<         2 - Push-To-Fix commanded*/
                              /**<         3 - Advanced-Power-Mode commanded*/
                              /**<         4 - initial*/
                              /**< Bit 7     - restart type*/
                              /**<         0 - GPS only restart*/
                              /**<         1 - system restart*/
   tSIRF_UINT8  status;        /**< Bit field:*/
                              /**< Bit 0     - RTC status*/
                              /**<         0 - RTC data invalid*/
                              /**<         1 - RTC data valid*/
                              /**< Bit 1     - storage status*/
                              /**<         0 - data in storage is not valid*/
                              /**<         1 - data in storage is valid*/
                              /**< Bit 2     - Flash storage status*/
                              /**<         0 - data in storage is not valid*/
                              /**<         1 - data in storage is valid*/
   tSIRF_UINT8  dr_status;     /**< Bit field:*/
                              /**< Bit 0     - DR status*/
                              /**<         0 - sensor data invalid*/
                              /**<         1 - sensor data valid*/
   tSIRF_UINT32 gps_off_time;  /**< Number of seconds GPS was off*/
   tSIRF_UINT16 gps_week;      /**< GPS week number in weeks                     */
   tSIRF_UINT32 gps_tow;       /**< GPS time of week in 1 ms, 0..604,799,000 ms  */
   tSIRF_UINT16 utc_year;
   tSIRF_UINT8  utc_month;
   tSIRF_UINT8  utc_day;
   tSIRF_UINT8  utc_hour;
   tSIRF_UINT8  utc_min;
   tSIRF_UINT16 utc_sec;
   tSIRF_INT32 lat;           /**< Latitude in 1e-7 degrees,
                                 -900,000,000..900,000,000 */
   tSIRF_INT32 lon;           /**< Longitude in 1e-7 degrees,
                                 -1,800,000,000..1,800,000,000 */
   tSIRF_INT32 alt_ellips;    /**< Altitude from Ellipsoid in 0.01 meters,
                                 -200,000..10,000,000 */
   tSIRF_UINT16 heading;       /**< Heading, from True North in 0.01 degrees,
                                 0..36,000    */
   tSIRF_UINT32 valid_ephemeris;        /**< SV with valid ephemeris data. Bitmap.
                                          Bit 0: SV1, bit 1: SV2, ... */
   tSIRF_UINT32 collected_almanac;      /**< SV with collected almanac data,
                                          i.e. with almanac info newer than
                                          factory almanac. Bitmap.
                                          Bit 0: SV1, bit 1: SV2, ... */
   tSIRF_UINT16 collected_almanac_week; /**< week of the oldest collected almanac
                                        * entry */
   tSIRF_UINT16 factory_almanac_week;   /**<week of the factory collected almanac*/
   tSIRF_INT32 clk_offset;
   tSIRF_UINT32 reserved;
} tSIRF_MSG_SSB_STARTUP_INFO;

/*******************************************************************************
   SIRF_MSG_SSB_EE_GPS_TIME_INFO          ( 0x0138 ) 56 SID 1
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_EE_GPS_TIME_INFO_tag
{
   tSIRF_UINT8  time_valid_flag;
   tSIRF_UINT16 gps_week;
   tSIRF_UINT32 gps_tow;
   tSIRF_UINT32 ephemeris_request_mask; /**< bitfield for each sat */
}tSIRF_MSG_SSB_EE_GPS_TIME_INFO;

/*******************************************************************************
   SIRF_MSG_SSB_EE_INTEGRITY         ( 0x0238 ) 56 SID 2
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_EE_INTEGRITY_tag
{
   tSIRF_UINT32 sat_pos_validity_mask;
   tSIRF_UINT32 sat_clk_validity_mask;
   tSIRF_UINT32 sat_health_mask;
}tSIRF_MSG_SSB_EE_INTEGRITY;

/*******************************************************************************
   SIRF_MSG_SSB_EE_STATE         ( 0x0338 ) 56 SID 3
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_EE_STATE_tag
{
   tSIRF_UINT8 svid;
   tSIRF_UINT8 eph_source_flag;
   tSIRF_UINT16 week_no;
   tSIRF_UINT16 toe;              /**< in seconds */
   tSIRF_UINT8 eph_integrity;
   tSIRF_UINT8 eph_age;           /**< in days    */
}tSIRF_MSG_SSB_EE_STATE;

/*******************************************************************************
   SIRF_MSG_SSB_EE_CLK_BIAS_ADJ         ( 0x0438 ) 56 SID 4
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_EE_CLK_BIAS_ADJ_tag
{
   tSIRF_UINT8  svid;
   tSIRF_UINT16 toe;
   tSIRF_INT32  clk_bias_adjust;
}tSIRF_MSG_SSB_EE_CLK_BIAS_ADJ;

/*******************************************************************************
   SIRF_MSG_SSB_EE_STORE_NOW         ( 0x1238 ) 56 SID 18
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_EE_STORE_NOW_tag
{
   tSIRF_UINT8  reserved;
}tSIRF_MSG_SSB_EE_STORE_NOW;


/*******************************************************************************
   SIRF_MSG_SSB_EE_FILE_PART_RESP         ( 0x1338 ) 56 SID 19
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_EE_FILE_PART_RESP_tag
{
   tSIRF_UINT32  response;
}tSIRF_MSG_SSB_EE_FILE_PART_RESP;

/*******************************************************************************
#define SIRF_MSG_SSB_SIF_ACK_NACK        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38, 0x20)
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SIF_ACK_NACK_tag
{
   tSIRF_UINT8  ackMsgId;   /**< mid of msg acked */
   tSIRF_UINT8  ackSid;     /**< sid of msg acked */
   tSIRF_UINT8  ackNack;    /**< ack = 0 , nack = 1 */
   tSIRF_UINT8  reason;     /**< reason for ack */
}tSIRF_MSG_SSB_SIF_ACK_NACK;

/*******************************************************************************
#define SIRF_MSG_SSB_SIF_EE_AGE        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38, 0x21)
*******************************************************************************/


/*******************************************************************************
#define SIRF_MSG_SSB_SIF_SGEE_AGE        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38, 0x22)
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SIF_SGEE_AGE_tag
{
   tSIRF_UINT32  sgeeAge;              /**< sgee Age of satellite */
   tSIRF_UINT32  predictionInterval;   /**< prediction Interval */
}tSIRF_MSG_SSB_SIF_SGEE_AGE;


/*******************************************************************************
 #define SIRF_MSG_SSB_SIF_PKT_INVOKE_DLD      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38,0x23)
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SIF_PKT_INVOKE_DLD_tag
{
   tSIRF_UINT8  start;                      /**< Start = 1, Stop = 0 */
   tSIRF_UINT32 waitTime;                   /**< Time to wait before requesting again */
}tSIRF_MSG_SSB_SIF_PKT_INVOKE_DLD;


/*******************************************************************************
 #define SIRF_MSG_SSB_SIF_PKT_ERASE       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38,0x24)
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SIF_PKT_ERASE_tag
{
   tSIRF_UINT8   NVMID;                   /**< Storage Type */
}tSIRF_MSG_SSB_SIF_PKT_ERASE;


/*******************************************************************************
#define SIRF_MSG_SSB_SIF_STR_PKT_DATA   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38,0x25)
*******************************************************************************/
#define SIF_MAX_PKT_DATA 2000
#define SSB_SGEE_MAX_PKT_LEN 400
#define MAX_RCV_BLOCKS 32
#define BBRAM_SGEE_HEADER_SIZE_DEFINE 600
#define SSB_DLD_MAX_PKT_LEN 512

typedef struct tSIRF_MSG_SSB_SIF_STR_PKT_DATA_tag
{
   tSIRF_UINT8 NVMID;                   /**< Storage ID */
   tSIRF_UINT16 size;                   /**< Data size */
   tSIRF_UINT32 offset;                 /**< Offset in Storage */
   tSIRF_UINT16 seqNum;                 /**< Packet sequence number */
   tSIRF_UINT8  data[SIF_MAX_PKT_DATA]; /**< Received data */
}tSIRF_MSG_SSB_SIF_STR_PKT_DATA;


/*******************************************************************************
#define SIRF_MSG_SSB_SIF_RCV_PKT_DATA   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38,0x26)
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SIF_RCV_PKT_DATA_tag
{
   tSIRF_UINT16 seqNum;                 /**< Packet sequence number */
   tSIRF_UINT8 NVMID;                   /**< Storage ID */
   tSIRF_UINT8 numBlocks;               /**< Number of Blocks to read */
   tSIRF_UINT16 size[MAX_RCV_BLOCKS];   /**< Data size */
   tSIRF_UINT32 offset[MAX_RCV_BLOCKS]; /**< Offset in Storage */
}tSIRF_MSG_SSB_SIF_RCV_PKT_DATA;

/*******************************************************************************
#define SIRF_MSG_SSB_SIF_NVM_HEADER_DATA   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38,0x27)
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SIF_NVM_HEADER_DATA_tag
{
   tSIRF_UINT16 offset;                                    /**< BBRAM header offset */
   tSIRF_UINT16 pktSize;                                   /**< BBRAM header size */
   tSIRF_UINT8  pktData[BBRAM_SGEE_HEADER_SIZE_DEFINE];    /**< BBRAM data */
}tSIRF_MSG_SSB_SIF_NVM_HEADER_DATA;

/*******************************************************************************
#define SIRF_MSG_SSB_SIF_AIDING_STATUS  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x38,0x28)
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_SIF_AIDING_STATUS_tag
{
   tSIRF_UINT8   sgeeDisable;                                 /**< SGEE Disable/Enable */
   tSIRF_UINT8   cgeeDisable;                                 /**< CGEE Disable/Enable */
   tSIRF_UINT32  CGEEDisableSeconds;                          /**< CGEE Prediction stop time */
   tSIRF_UINT32  CurrentReceiverSeconds;                      /**< Receiver current time */
}tSIRF_MSG_SSB_SIF_AIDING_STATUS;

/*******************************************************************************
   SIRF_MSG_SSB_EE_ACK               ( 0xFF38 ) 56 SID 255
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_EE_ACK_tag
{
   tSIRF_UINT8 acq_id;
   tSIRF_UINT8 acq_sub_id;
}tSIRF_MSG_SSB_EE_ACK;

/*******************************************************************************
   SIRF_MSG_SSB_TEST_MODE_DATA_7    ( 0x073F ) 63 SID 7
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_TEST_MODE_DATA_7_tag
{
   tSIRF_UINT8  test_status;        /**< 0 = in progress, 1 = complete */
   tSIRF_UINT32 spur1_frequency;    /**< first spur frequency */
   tSIRF_UINT16 spur1_sig_to_noise; /**< first spur power */
   tSIRF_UINT32 spur2_frequency;    /**< second spur frequency */
   tSIRF_UINT16 spur2_sig_to_noise; /**< second spur power */
   tSIRF_UINT32 spur3_frequency;    /**< third spur frequency */
   tSIRF_UINT16 spur3_sig_to_noise; /**< third spur power */
   tSIRF_UINT32 spur4_frequency;    /**< fourth spur frequency */
   tSIRF_UINT16 spur4_sig_to_noise; /**< fourth spur power */
}tSIRF_MSG_SSB_TEST_MODE_DATA_7;

/*******************************************************************************
   SIRF_MSG_SSB_TEST_MODE_DATA_8    ( 0x083F ) 63 SID 8
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_TEST_MODE_DATA_8_tag
{
   tSIRF_INT32   spurDoppler;      /**< Spur doppler in HW units */
   tSIRF_UINT16  spurCN0;          /**< Spur signal strength in 0.1 dBHz */
   tSIRF_UINT16  jitter;           /**< Osc Phase jitter in 0.01 deg/sqrt(Hz)*/
   tSIRF_UINT16  rtcFrequency;     /**< RTC Frequency. Unit: Hz  */
   tSIRF_UINT16  etoAcqRatio;      /**< ECLK to ACQ Clock ratio  */
   tSIRF_UINT8   tSyncAGCGain;     /**< Tsync and AGC Gain value */

}tSIRF_MSG_SSB_TEST_MODE_DATA_8;

/*******************************************************************************
   SIRF_MSG_SSB_TEST_MODE_DATA_9    ( 0x093F ) 63 SID 9
*******************************************************************************/

#define SIRF_MSG_SSB_TEST_MODE_DATA_9_PKT_LEN     512
typedef struct tSIRF_MSG_SSB_TEST_MODE_DATA_9_tag
{
   tSIRF_UINT8  iqbuf_seq_num;       /* sequence number of IQ buffer, 0 to 15 */
   tSIRF_UINT8  packet_seq_num;      /* packet number of current IQ buffer */
   tSIRF_UINT8  iqsample[SIRF_MSG_SSB_TEST_MODE_DATA_9_PKT_LEN];
                                     /* IQ samples in 2-bit format */
}tSIRF_MSG_SSB_TEST_MODE_DATA_9;

typedef struct tSIRF_MSG_SSB_TRACKER_DATA_GPIO_STATE_tag
{
   tSIRF_UINT16 gpio_state; /* GPIO0 state is on bit 0, GPIO1 on bit 1, etc. */

} tSIRF_MSG_SSB_TRACKER_DATA_GPIO_STATE;

/*******************************************************************************
SIRF_MSG_SSB_GPIO_READ              SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x41, 0xC0)
*******************************************************************************/

#define SIRF_MSG_SSB_GPIO_MASK( gpio )  ( 0x0001 << (gpio) )

typedef struct tSIRF_MSG_SSB_GPIO_READ_tag
{
   tSIRF_UINT16 gpio_state;        /* Read state of all GPIOs (bit 0 = GPIO 0, 
                                    * bit 1 = GPIO 1, etc.) */
} tSIRF_MSG_SSB_GPIO_READ;

/*******************************************************************************
   SIRF_MSG_SSB_SIRF_STATS    ( 0x06E1 ) 225 SID 6
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_SIRF_STATS_tag
{
   tSIRF_UINT16 ttff_since_reset;
   tSIRF_UINT16 ttff_since_aiding;
   tSIRF_UINT16 ttff_first_nav;
   tSIRF_INT32  pos_aiding_error_north;
   tSIRF_INT32  pos_aiding_error_east;
   tSIRF_INT32  pos_aiding_error_down;
   tSIRF_INT32  time_aiding_error;
   tSIRF_INT16  freq_aiding_error;
   tSIRF_UINT8  hor_pos_uncertainty;
   tSIRF_UINT16 ver_pos_uncertainty;
   tSIRF_UINT8  time_uncertainty;
   tSIRF_UINT8  freq_uncertainty;
   tSIRF_UINT8  num_aided_ephemeris;
   tSIRF_UINT8  num_aided_acq_assist;
   tSIRF_UINT8  nav_mode;
   tSIRF_UINT8  pos_mode;
   tSIRF_UINT16 nav_status;
   tSIRF_UINT8  start_mode;
   tSIRF_UINT8  aiding_status;
}tSIRF_MSG_SSB_SIRF_STATS;

/*******************************************************************************
   SIRF_MSG_SSB_SIRF_STATS_AGPS    ( 0x07E1 ) 225 SID 7
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_SIRF_STATS_AGPS_tag
{
   tSIRF_MSG_SSB_SIRF_STATS stats;
   tSIRF_UINT32 clock_drift;
   tSIRF_UINT32 reserved;
}tSIRF_MSG_SSB_SIRF_STATS_AGPS;


/*******************************************************************************
   SIRF_MSG_SSB_DOP_VALUES             ( 0x42 ) 66
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_DOP_VALUES_tag
{
   tSIRF_UINT32  gps_tow;   /**< GPS time of week in 1 ms, 0..604,799,000 ms*/
   tSIRF_UINT16  gdop;      /**< scaled by 10*/
   tSIRF_UINT16  pdop;      /**< scaled by 10*/
   tSIRF_UINT16  hdop;      /**< scaled by 10*/
   tSIRF_UINT16  vdop;      /**< scaled by 10*/
   tSIRF_UINT16  tdop;      /**< scaled by 10*/
}  tSIRF_MSG_SSB_DOP_VALUES;

/*******************************************************************************
   SIRF_MSG_SSB_INITIALIZE             ( 0x80 ) 128
*******************************************************************************/

/* configuration flags:*/
#define SIRF_MSG_SSB_RESTART_HOT                   0x00
#define SIRF_MSG_SSB_RESTART_WARM_NOINIT           0x02
#define SIRF_MSG_SSB_RESTART_WARM_INIT             0x03
#define SIRF_MSG_SSB_RESTART_COLD                  0x04
#define SIRF_MSG_SSB_RESTART_FACTORY               0x08

/* additional flags:*/
#define SIRF_MSG_SSB_RESTART_RAW_TRK_OUT           0x10
#define SIRF_MSG_SSB_RESTART_TEXTOUT               0x20  /**< Enable debug data output */

typedef struct tSIRF_MSG_SSB_INITIALIZE_tag
{
   tSIRF_INT32 ecef_x;        /**< Position - X in meters */
   tSIRF_INT32 ecef_y;        /**< Position - Y in meters */
   tSIRF_INT32 ecef_z;        /**< Position - Z in meters */
   tSIRF_INT32 clk_offset;    /**< Clock Offset in Hz */
   tSIRF_UINT32 gps_tow;       /**< GPS Time of Week, in 10ms */
   tSIRF_UINT16 gps_week;      /**< Week number */
   tSIRF_UINT8  chnl_cnt;      /**< Number of channels */
   tSIRF_UINT8  restart_flags; /**< Restart Configuration */
} tSIRF_MSG_SSB_INITIALIZE;

/*******************************************************************************
   SIRF_MSG_SSB_SET_NMEA_MODE          ( 0x81 ) 129
*******************************************************************************/
#define SIRF_MSG_SSB_MAX_NUM_NMEA_MSG ( 10 )

typedef struct tSIRF_MSG_SSB_NMEA_CFG_tag
{
   tSIRF_UINT8  rate;
   tSIRF_UINT8  cksum;
} tSIRF_MSG_SSB_NMEA_CFG;

typedef struct tSIRF_MSG_SSB_SET_NMEA_MODE_tag
{
   tSIRF_UINT8        mode;
   tSIRF_MSG_SSB_NMEA_CFG nmea_cfg[SIRF_MSG_SSB_MAX_NUM_NMEA_MSG];
   tSIRF_UINT16       baud_rate;
}  tSIRF_MSG_SSB_SET_NMEA_MODE;

/*******************************************************************************
   SIRF_MSG_SSB_SET_ALMANAC            ( 0x82 ) 130
*******************************************************************************/

#define SIRF_MSG_SSB_ALMANAC_DATA_ENTRY      ( 14 )
#define SIRF_MSG_SSB_ALMANAC_SIZE       (SIRF_MAX_SVID_CNT*SIRF_MSG_SSB_ALMANAC_DATA_ENTRY)

typedef struct tSIRF_MSG_SSB_ALM_DATA_tag
{
   tSIRF_INT16 data[SIRF_MSG_SSB_ALMANAC_DATA_ENTRY];
} tSIRF_MSG_SSB_ALM_DATA;

typedef struct tSIRF_MSG_SSB_SET_ALMANAC_tag
{
   tSIRF_MSG_SSB_ALM_DATA almanac[SIRF_MAX_SVID_CNT];
}  tSIRF_MSG_SSB_SET_ALMANAC;

/*******************************************************************************
   SIRF_MSG_SSB_POLL_SW_VERSION        ( 0x84 ) 132
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_POLL_SW_VERSION_tag
{
   tSIRF_UINT8  reserved; /**< ignored */
}  tSIRF_MSG_SSB_POLL_SW_VERSION;

/*******************************************************************************
   SIRF_MSG_SSB_SET_DGPS_SOURCE        ( 0x85 ) 133
*******************************************************************************/

#define SIRF_MSG_SSB_DGPS_SRC_NONE         0  /**< Use no corrections       */
#define SIRF_MSG_SSB_DGPS_SRC_SBAS         1  /**< Use SBAS channel         */
#define SIRF_MSG_SSB_DGPS_SRC_USER         2  /**< Use external receiver    */
#define SIRF_MSG_SSB_DGPS_SRC_INTERNAL     3  /**< Use internal DGPS Beacon */
#define SIRF_MSG_SSB_DGPS_SRC_SOFTWARE     4  /**< Corrections provided using a module
                                        interface routine in a custon user
                                        application    */

typedef struct tSIRF_MSG_SSB_SET_DGPS_SOURCE_tag
{
   tSIRF_UINT8  src;
   tSIRF_UINT32 freq;           /**< unused */
   tSIRF_UINT8  bit_rate;       /**< unused */
}  tSIRF_MSG_SSB_SET_DGPS_SOURCE;

/*******************************************************************************
   SIRF_MSG_SSB_SET_PORT_VALUES  ( 0x86 )
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_SET_PORT_VALUES_tag
{
   tSIRF_UINT32 baud_rate;
   tSIRF_UINT8  data_bits;
   tSIRF_UINT8  stop_bits;
   tSIRF_UINT8  parity;
   tSIRF_UINT8  reserved;
} tSIRF_MSG_SSB_SET_PORT_VALUES;

/*******************************************************************************
 SIRF_MSG_SSB_SET_PORT_PROTOCOL ( 0x87 )
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_SET_PORT_PROTOCOL_tag
{
   tSIRF_UINT8  protocol;   // 1 = SSB, 2 = NMEA
} tSIRF_MSG_SSB_SET_PORT_PROTOCOL;

/*******************************************************************************
   SIRF_MSG_SSB_SET_NAV_MODE           ( 0x88 ) 136
*******************************************************************************/

/* Bit definitions for "pos_mode_enable" field: */
/* Bit 0: Almanac-Based-Position (ABP), 0=disabled 1=enabled */
#define SIRF_MSG_SSB_POS_MODE_ABP           (0x01)
/* Bit 1: Reserved EE, 0=disabled 1=enabled */
#define SIRF_MSG_SSB_POS_MODE_REVEE         (0x02)
/* Bit 2: 5Hz Nav, 0=disabled 1=enabled */
#define SIRF_MSG_SSB_POS_MODE_5HZNAV        (0x04)
/* Bit 3: SBAS Ranging Control, 0=disabled 1=enabled */
#define SIRF_MSG_SSB_POS_MODE_SBAS_RANGING  (0x08)

typedef struct tSIRF_MSG_SSB_SET_NAV_MODE_tag
{
   tSIRF_UINT8  enable_3d;        /**< TRUE/FALSE                */
   tSIRF_UINT8  enable_con_alt;   /**< TRUE/FALSE                */
   tSIRF_UINT8  degraded_mode;    /**< 0=d_t, 1=t_d, 2=d_only,
                                    3=t_only, 4=disabled*/
   tSIRF_UINT8  pos_mode_enable;  /**< bit0: ABP(0=disabled,1=enabled),
                                       bit1: RevEE(0=disabled,1=enabled)
                                       bit2: 5Hz Nav(0=disabled,1=enabled)
                                       bit3: SBAS Ranging (0=disabled,1=enabled) */
   tSIRF_UINT8  dr_enable;        /**< TRUE/FALSE                */
   tSIRF_INT16  alt_input;        /**< -1000..10000 m            */
   tSIRF_UINT8  alt_mode;         /**< 0=auto, 1=always, 2=never */
   tSIRF_UINT8  alt_src;          /**< 0=last, 1=fixed, 2=dynamic*/
   tSIRF_UINT8  coast_timeout;    /**< 0 to 120 sec, def 2  sec  */
   tSIRF_UINT8  degraded_timeout; /**< 1 to 120 sec, def 20 sec  */
   tSIRF_UINT8  dr_timeout;       /**< 1 to 120 sec, def 5  sec  */
   tSIRF_UINT8  trk_smooth;       /**< Track Smooth Algorithm enable/disable */
}  tSIRF_MSG_SSB_SET_NAV_MODE;

/*******************************************************************************
   SIRF_MSG_SSB_SET_DOP_MODE           ( 0x89 ) 137
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_DOP_MODE_tag
{
   tSIRF_UINT8  mode;     /**< 0=auto P/H, 1=PDOP ,2=HDOP, 3=GDOP, 4=never */
   tSIRF_UINT8  gdop_th;  /**< 1 to 50, def 8                     */
   tSIRF_UINT8  pdop_th;  /**< 1 to 50, def 8                     */
   tSIRF_UINT8  hdop_th;  /**< 1 to 50, def 8                     */
}  tSIRF_MSG_SSB_SET_DOP_MODE;

/*******************************************************************************
   SIRF_MSG_SSB_SET_DGPS_MODE          ( 0x8A ) 138
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_DGPS_MODE_tag
{
   tSIRF_UINT8  mode;
   tSIRF_UINT8  timeout;
}  tSIRF_MSG_SSB_SET_DGPS_MODE;

/*******************************************************************************
   SIRF_MSG_SSB_SET_ELEV_MASK          ( 0x8B ) 139
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_ELEV_MASK_tag
{
   tSIRF_INT16  trk;  /**< -200 to 900 1/10 deg, def 50 */
   tSIRF_INT16  nav;  /**< -200 to 900 1/10 deg, def 75 */
}  tSIRF_MSG_SSB_SET_ELEV_MASK;

/*******************************************************************************
   SIRF_MSG_SSB_SET_POWER_MASK         ( 0x8C ) 140
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_POWER_MASK_tag
{
   tSIRF_UINT8  trk;  /**< 20 to 50 dBHz, def 28 */
   tSIRF_UINT8  nav;  /**< 20 to 50 dBHz, def 30 */
}  tSIRF_MSG_SSB_SET_POWER_MASK;

/*******************************************************************************
   SIRF_MSG_SSB_SET_STAT_NAV           ( 0x8F ) 143
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_STAT_NAV_tag
{
   tSIRF_UINT8  staticnav;
}  tSIRF_MSG_SSB_SET_STAT_NAV;

/*******************************************************************************
   SIRF_MSG_SSB_POLL_CLOCK_STATUS      ( 0x90 ) 144
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_POLL_CLOCK_STATUS_tag
{
   tSIRF_UINT8  reserved; /**< ignored */
}  tSIRF_MSG_SSB_POLL_CLOCK_STATUS;

/*******************************************************************************
   SIRF_MSG_SSB_POLL_ALMANAC           ( 0x92 ) 146
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_POLL_ALMANAC_tag
{
   tSIRF_UINT8  reserved;
}  tSIRF_MSG_SSB_POLL_ALMANAC;

/*******************************************************************************
   SIRF_MSG_SSB_POLL_EPHEMERIS         ( 0x93 ) 147
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_POLL_EPHEMERIS_tag
{
   tSIRF_UINT8  svid; /**< 0 = send all Ephemeris */
   tSIRF_UINT8  reserved;
}  tSIRF_MSG_SSB_POLL_EPHEMERIS;

/*******************************************************************************
   SIRF_MSG_SSB_SET_EPHEMERIS          ( 0x95 ) 149
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_EPHEMERIS_tag
{
   tSIRF_UINT16 subframe[3][15];
}  tSIRF_MSG_SSB_SET_EPHEMERIS;

/*******************************************************************************
   SIRF_MSG_SSB_SET_OP_MODE            ( 0x96 ) 150
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_OP_MODE_tag
{
   tSIRF_UINT16 mode;         /**< Op Mode:  0 = Normal, 0x1E51 = Test,
                                             0x1e52 - test2, . . . */
   tSIRF_UINT16 svid;         /**< SVid to search for (in test mode)   */
   tSIRF_UINT16 period;       /**< output message period (test mode)   */
   tSIRF_UINT8  tm5Cmd;       /**< Testmode 5 Low Power Command 0=No 1=Yes */
}  tSIRF_MSG_SSB_SET_OP_MODE;

/*******************************************************************************
   SIRF_MSG_SSB_SET_LOW_POWER          ( 0x97 ) 151
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_LOW_POWER_tag
{
   tSIRF_UINT16 push_to_fix;
   tSIRF_UINT16 duty_cycle;
   tSIRF_UINT32 on_time;
}  tSIRF_MSG_SSB_SET_LOW_POWER;

/*******************************************************************************
   SIRF_MSG_SSB_POLL_RECEIVER_PARAMS   ( 0x98 ) 152
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_NAV_POLL_RECEIVER_PARAMS_tag
{
   tSIRF_UINT8  reserved;
}  tSIRF_MSG_SSB_NAV_POLL_RECEIVER_PARAMS;

/*******************************************************************************
   SIRF_MSG_SSB_SIRFNAV_COMMAND        ( 0xA1 ) 161
*******************************************************************************/

#define SIRF_MSG_SSB_RESTART_MODE_REINITIALIZE       ( 0x00 )
#define SIRF_MSG_SSB_RESTART_MODE_START_STOP         ( 0x01 )

typedef struct tSIRF_MSG_SSB_DEMO_SET_RESTART_MODE_tag
{
   tSIRF_UINT8  control;
}  tSIRF_MSG_SSB_DEMO_SET_RESTART_MODE;

typedef struct tSIRF_MSG_SSB_DEMO_TEST_CPU_STRESS_tag
{
   tSIRF_UINT16 TaskID ;
   tSIRF_UINT32 PeriodOn ;
   tSIRF_UINT32 PeriodOff ;
   tSIRF_UINT32 LoadDuration ;
   tSIRF_UINT32 FullLoadDuration ;
   tSIRF_UINT32 OffsetTime ;
}  tSIRF_MSG_SSB_DEMO_TEST_CPU_STRESS;

typedef struct tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE_tag
{
   tSIRF_UINT32 start_mode;
   tSIRF_UINT32 clock_offset;
   tSIRF_UINT32 port_num;
   tSIRF_UINT32 baud_rate;
}  tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE;

/* Typically SIRF_CONFIG_SIZE should be size of tSIRF_CONFIG str
 * But, in the current implementation, it does not include code_load_baud_rate.
 * With code_load_baud_rate, the size would be 102 bytes.
 * This was not included in order to have only one SiRFLive
 * and be able to run Natural Hot Start tests with both 4.0.0
 * 4.0.1 versions of 4t. */
#define SIRF_CONFIG_SIZE  (98)
typedef struct tSIRF_MSG_SSB_DEMO_START_NAV_ENGINE_tag
{
   tSIRF_CONFIG config;
}  tSIRF_MSG_SSB_DEMO_START_NAV_ENGINE;

typedef struct tSIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE_tag
{
   tSIRF_UINT32 stop_mode;
}  tSIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE;

typedef struct tSIRF_MSG_SSB_DEMO_STOP_TEST_APP_tag
{
   tSIRF_UINT8 reserved;
}  tSIRF_MSG_SSB_DEMO_STOP_TEST_APP;

typedef struct tSIRF_MSG_SSB_SIRFNAV_STORE_NOW_tag
{
   tSIRF_UINT8 reserved;
} tSIRF_MSG_SSB_SIRFNAV_STORE_NOW;

typedef struct tSIRF_MSG_SSB_READ_I2CData_tag
{
   tSIRF_UINT32 offset;
   tSIRF_UINT32 length;
   tSIRF_UINT8  eeType;
}  tSIRF_MSG_SSB_READ_I2CData;

typedef struct tSIRF_MSG_SSB_EXT_UART_OPEN_PORT_tag
{
   tSIRF_CHAR   port_name[32];
   tSIRF_UINT32 baud_rate;
   tSIRF_UINT32 flow_control;
}  tSIRF_MSG_SSB_EXT_UART_OPEN_PORT;

/*******************************************************************************
   SIRF_MSG_SSB_SET_MSG_RATE           ( 0xA6 ) 166
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_MSG_RATE_tag
{
   tSIRF_UINT8  mode;        /**< mode: 0= set rate, 1 = query */
   tSIRF_UINT8  msg_id;      /**< msg id to control */
   tSIRF_UINT8  rate;        /**< rate of xmission (0: disable) */
   tSIRF_UINT8  sub_id;      /**< sub id to control */
   tSIRF_UINT8  reserved[3]; /**< reserved */
}  tSIRF_MSG_SSB_SET_MSG_RATE;

/*******************************************************************************
   SIRF_MSG_SSB_SET_LOW_POWER_PARAMS   ( 0xA7 ) 167
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_LOW_POWER_PARAMS_tag
{
   tSIRF_UINT32 MaxOffTime;
   tSIRF_UINT32 MaxAcqTime;
   tSIRF_UINT32 push_to_fix_period;
   tSIRF_UINT16 adaptive_tp;
}  tSIRF_MSG_SSB_SET_LOW_POWER_PARAMS;


/*******************************************************************************
   SIRF_MSG_SSB_POLL_CMD_PARAM   ( 0xA8 ) 168
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_POLL_CMD_PARAM_tag
{
   tSIRF_UINT8 req_msg_id;
} tSIRF_MSG_SSB_POLL_CMD_PARAM;

/*******************************************************************************
   SIRF_MSG_SSB_SET_SBAS_PRN           ( 0xAA ) 170
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_SBAS_PRN_tag
{
   tSIRF_UINT8  prnOrRegion;
   tSIRF_UINT8  mode;
   tSIRF_INT8   flagBits;
   tSIRF_INT8   region;
   tSIRF_UINT8  regionPRN;
}  tSIRF_MSG_SSB_SET_SBAS_PRN;


/*******************************************************************************
      SIRF_MSG_SSB_ADVANCED_NAV_INIT ( 0x00AC ) 172  SID 0
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_ADVANCED_NAV_INIT_tag
{
   struct
   {
      tSIRF_UINT32 timeMode     : 3;  /**< see resetInitModeEnum */
      tSIRF_UINT32 posMode      : 3;  /**< see resetInitModeEnum */
      tSIRF_UINT32 clkMode      : 3;  /**< see resetInitModeEnum */
      tSIRF_UINT32 resetEphem   : 1;  /**< 1 to clear ephemeris */
      tSIRF_UINT32 fullReset    : 1;  /**< 1 for full reset, 0 for gps reset only */
   } control;
   tSIRF_INT32  timeSkew;     /**< time skew in microseconds */
   tSIRF_UINT32 timeUncert;   /**< time uncertainty in microseconds */
   tSIRF_INT32  pos[3];       /**< position x,y,z in m */
   tSIRF_UINT16 horzUncert;   /**< horizontal position uncertainty in km */
   tSIRF_UINT16 vertUncert;   /**< vertical position uncertainty in km */
   tSIRF_INT32  clkSkew;      /**< clock skew in Hz */
   tSIRF_UINT32 clkUncert;    /**< clock uncertainty in Hz */
   /** Note that this message actually contains 6 more reserved bytes */
} tSIRF_MSG_SSB_ADVANCED_NAV_INIT;

/*******************************************************************************
      SIRF_MSG_SSB_DR_SET_NAV_INIT       ( 0x01AC )
      SIRF_MSG_SSB_DR_SET_NAV_MODE       ( 0x02AC )
      SIRF_MSG_SSB_DR_SET_GYR_FACT_CAL   ( 0x03AC )
      SIRF_MSG_SSB_DR_SET_SENS_PARAM     ( 0x04AC )
      SIRF_MSG_SSB_DR_POLL_VALID         ( 0x05AC )
      SIRF_MSG_SSB_DR_POLL_GYR_FACT_CAL  ( 0x06AC )
      SIRF_MSG_SSB_DR_POLL_SENS_PARAM    ( 0x07AC )
      SIRF_MSG_SSB_DR_CAR_BUS_DATA       ( 0x09AC )
      SIRF_MSG_SSB_DR_CAR_BUS_ENABLED    ( 0x0AAC )
      SIRF_MSG_SSB_DR_CAR_BUS_DISABLED   ( 0x0BAC )
      SIRF_MSG_SSB_MMF_DATA              ( 0x50AC )
      SIRF_MSG_SSB_MMF_SET_MODE          ( 0x51AC )
*******************************************************************************/

/*******************************************************************************
      SIRF_MSG_SSB_DR_SET_NAV_INIT       ( 0x01AC ) 172 SID 1
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_DR_SET_NAV_INIT_tag
{
   tSIRF_INT32 lat;          /**< in deg                                 */
   tSIRF_INT32 lon;          /**< in deg                                 */
   tSIRF_INT32 alt_ellips;   /**< in m; from ellipsoid                   */
   tSIRF_UINT16 heading;      /**< in deg; true                           */
   tSIRF_INT32 clk_offset;   /**< Clock Offset in Hz                     */
   tSIRF_UINT32 gps_tow;      /**< GPS Time of Week, in seconds           */
   tSIRF_UINT16 gps_week;     /**< Week number                            */
   tSIRF_UINT8  chnl_cnt;     /**< Number of channels                     */
   tSIRF_UINT8  restart_flags;/**< Reset Configuration                    */
                             /**<   Bit 0: Init Data Valid Bit, 1-valid  */
                             /**<   Bit 1: Clear Ephemeris Flag, 1-clear */
                             /**<   Bit 2: Clear Memory Flag, 1-clear    */
}  tSIRF_MSG_SSB_DR_SET_NAV_INIT;

/*******************************************************************************
      SIRF_MSG_SSB_DR_SET_NAV_MODE       ( 0x02AC ) 172 SID 2
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_DR_SET_NAV_MODE_tag
{
   tSIRF_UINT8  mode;    /**< bit 0: GPS nav only                                */
                        /**< bit 1: DR nav okay with stored/default calibration */
                        /**< bit 2: DR nav okay with current GPS calibration    */
}  tSIRF_MSG_SSB_DR_SET_NAV_MODE;

/*******************************************************************************
      SIRF_MSG_SSB_DR_SET_GYR_FACT_CAL   ( 0x03AC ) 172 SID 3
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_SET_GYR_FACT_CAL_tag
{
   tSIRF_UINT8  cal;     /**< bit 0: start gyro bias calibration         */
                        /**< bit 1: start gyro scale factor calibration */
   tSIRF_UINT8  reserved;
}  tSIRF_MSG_SSB_DR_SET_GYR_FACT_CAL;

/*******************************************************************************
      SIRF_MSG_SSB_DR_SET_SENS_PARAM     ( 0x04AC ) 172 SID 4
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_SET_SENS_PARAM_tag
{
   tSIRF_UINT8  base_ssf;  /**< in ticks/m                    */
   tSIRF_UINT16 base_gb;   /**< in zero rate volts            */
   tSIRF_UINT16 base_gsf;  /**< in mV / (deg/s)               */
}  tSIRF_MSG_SSB_DR_SET_SENS_PARAM;

/*******************************************************************************
      SIRF_MSG_SSB_DR_POLL_VALID         ( 0x05AC ) 172 SID 5
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_POLL_VALID_tag
{
   tSIRF_UINT8  reserved;
}  tSIRF_MSG_SSB_DR_POLL_VALID;

/*******************************************************************************
      SIRF_MSG_SSB_DR_POLL_GYR_FACT_CAL  ( 0x06AC ) 172 SID 6
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_POLL_GYR_FACT_CAL_tag
{
   tSIRF_UINT8  reserved;
}  tSIRF_MSG_SSB_DR_POLL_GYR_FACT_CAL;

/*******************************************************************************
      SIRF_MSG_SSB_DR_POLL_SENS_PARAM    ( 0x07AC ) 172 SID 7
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_DR_POLL_SENS_PARAM_tag
{
   tSIRF_UINT8  reserved;
}  tSIRF_MSG_SSB_DR_POLL_SENS_PARAM;

/*******************************************************************************
      SIRF_MSG_SSB_DR_CAR_BUS_DATA       ( 0x09AC ) 172 SID 9
*******************************************************************************/
typedef struct
{
   tSIRF_UINT8      valid_sensor_indication;
   tSIRF_UINT32     data_set_time_tag;
   tSIRF_UINT16     odometer_speed;
   tSIRF_INT16     data1;
   tSIRF_INT16     data2;
   tSIRF_INT16     data3;
   tSIRF_INT16     data4;
   tSIRF_UINT8      reserved;
} tSIRF_MSG_SSB_DR_CAR_BUS_DATA_SET;

#define MAX_DR_CAR_BUS_DATA_SETS 11
typedef struct
{
   tSIRF_UINT8                 sensor_data_type;
   tSIRF_UINT8                 num_of_data_sets;
   tSIRF_UINT16                reverse_bitmap;
   tSIRF_MSG_SSB_DR_CAR_BUS_DATA_SET   data[MAX_DR_CAR_BUS_DATA_SETS];
} tSIRF_MSG_SSB_DR_CAR_BUS_DATA;

/*******************************************************************************
      SIRF_MSG_SSB_DR_CAR_BUS_ENABLED    ( 0x0AAC ) 172 SID 10
*******************************************************************************/
typedef struct
{
   tSIRF_UINT32  mode;
} tSIRF_MSG_SSB_DR_CAR_BUS_ENABLED;

/*******************************************************************************
      SIRF_MSG_SSB_DR_CAR_BUS_DISABLED   ( 0x0BAC ) 172 SID 11
*******************************************************************************/
typedef struct
{
   tSIRF_UINT32  mode;
} tSIRF_MSG_SSB_DR_CAR_BUS_DISABLED;


/*******************************************************************************
      SIRF_MSG_SSB_DR_SENSOR_DATA       ( 0x13AC ) 172 SID 19
*******************************************************************************/

#define SIRF_MSG_SSB_DR_SENSOR_DATA_SET_MAX  9
#define SIRF_MSG_SSB_DR_SENSOR_DATA_MAX     11

#define SIRF_MSG_SSB_DR_SENSOR_ODO_GYRO_REV      1
#define SIRF_MSG_SSB_DR_SENSOR_DIF_PULSES_REV    2
#define SIRF_MSG_SSB_DR_SENSOR_DIF_SPEEDS_REV    3
#define SIRF_MSG_SSB_DR_SENSOR_DIF_ANGLRT_REV    4
#define SIRF_MSG_SSB_DR_SENSOR_ODO_GYRO_NOREV    5
#define SIRF_MSG_SSB_DR_SENSOR_DIF_PULSES_NOREV  6
#define SIRF_MSG_SSB_DR_SENSOR_DIF_SPEEDS_NOREV  7
#define SIRF_MSG_SSB_DR_SENSOR_DIF_ANGLRT_NOREV  8
                                                       //  AUTO      |    PED
#define SIRF_MSG_SSB_DR_SENSOR_VALID_TIME_TAG  0x0001
#define SIRF_MSG_SSB_DR_SENSOR_VALID_DATA0     0x0002  // yaw gyro   |  yaw gyro
#define SIRF_MSG_SSB_DR_SENSOR_VALID_DATA1     0x0004  // odo speed  |  roll gyro
#define SIRF_MSG_SSB_DR_SENSOR_VALID_DATA2     0x0008  // reverse    |  pitch gyro
#define SIRF_MSG_SSB_DR_SENSOR_VALID_DATA3     0x0010  // RF wheel   |  X accel
#define SIRF_MSG_SSB_DR_SENSOR_VALID_DATA4     0x0020  // LF wheel   |  Y accel
#define SIRF_MSG_SSB_DR_SENSOR_VALID_DATA5     0x0040  // RR wheel   |  Z accel
#define SIRF_MSG_SSB_DR_SENSOR_VALID_DATA6     0x0080  // LR wheel   |  compass
#define SIRF_MSG_SSB_DR_SENSOR_VALID_DATA7     0x0100  // spare      |  altimeter
#define SIRF_MSG_SSB_DR_SENSOR_VALID_DATA8     0x0200  // spare      |  spare

typedef struct
{
   tSIRF_UINT16    valid_data_indication; // bitmap
   tSIRF_UINT32    data_set_time_tag;     // time in [ms]
   tSIRF_INT16     data[ SIRF_MSG_SSB_DR_SENSOR_DATA_SET_MAX ];
} tSIRF_MSG_SSB_DR_SENSOR_DATA_SET;

typedef struct
{
   tSIRF_UINT8                        sensor_data_type;
   tSIRF_UINT8                        num_of_data_sets;
   tSIRF_UINT16                       reserved;
   tSIRF_MSG_SSB_DR_SENSOR_DATA_SET   data_set[ SIRF_MSG_SSB_DR_SENSOR_DATA_MAX ];
} tSIRF_MSG_SSB_DR_SENSOR_DATA;


/*******************************************************************************
      SIRF_MSG_SSB_MMF_DATA              ( 0x50AC ) 172 SID 80
*******************************************************************************/
typedef struct
{
   tSIRF_INT32     lat;                /**< deg; scaling 1e-7*/
   tSIRF_INT32     lon;                /**< deg; scaling 1e-7*/
   tSIRF_UINT32     horiz_pos_uncert;   /**< m;   scaling 0.01*/
   tSIRF_INT32     alt_ellips;         /**< m;   scaling 0.1*/
   tSIRF_UINT32     vert_pos_uncert;    /**< m;   scaling 0.1*/
   tSIRF_UINT16     heading;            /**< deg; scaling 0.01*/
   tSIRF_UINT16     heading_uncert;     /**< deg; scaling 0.01*/
   tSIRF_UINT16     reserved;
} tSIRF_MSG_SSB_MMF_DATA_SET;

typedef struct
{
   tSIRF_UINT32     gps_tow_reference;  /**< GPS TOW (sec; scaling 1e-3) for the */
                                       /**< original NAV data from which*/
                                       /**<   the MMF data is derived*/
   tSIRF_UINT8      num_of_data_sets;   /**< 1 to 3*/
   tSIRF_UINT8      control;            /**< Bit 0 = 1: Position Forced*/
                                       /**< Bit 1 = 1: Heading Forced*/
                                       /**< Bit 2 = 1: Altitude Forced*/
                                       /**< Bit 3 = 1: Position is Valid*/
                                       /**< Bit 4 = 1: Heading is Valid*/
                                       /**< Bit 5 = 1: Altitude is Valid*/
                                       /**< Bits 6-7: reserved*/
   tSIRF_MSG_SSB_MMF_DATA_SET  mmf_data[3];
} tSIRF_MSG_SSB_MMF_DATA;

/*******************************************************************************
      SIRF_MSG_SSB_MMF_SET_MODE          ( 0x51AC ) 172 SID 81
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_MMF_SET_MODE_tag
{
   tSIRF_UINT32  mode;   /**< Bit  0 = 1: Enable map matching*/
                        /**< Bit  1 = 1: Enable map matching callibration*/
                        /**< Bit  2 = 1: Enable map matching retro loop*/
                        /**< Bit 16 = 1: Disable map matching*/
                        /**< Bit 17 = 1: Disable map matching callibration*/
                        /**< Bit 18 = 1: Disable map matching retro loop*/
}  tSIRF_MSG_SSB_MMF_SET_MODE;

/*******************************************************************************
      SIRF_MSG_SSB_TRK_HW_CONFIG          ( 0xCE ) 206
*******************************************************************************/

#define SIRF_MSG_SSB_TRK_HW_CONFIG_LNA_IGNORE   (0)
#define SIRF_MSG_SSB_TRK_HW_CONFIG_LNA_INTERNAL (1)
#define SIRF_MSG_SSB_TRK_HW_CONFIG_LNA_EXTERNAL (2)

#define SIRF_MSG_SSB_TRK_HW_CONFIG_RF_OVRD_MSG_LENGTH (16)

typedef struct tSIRF_MSG_SSB_TRK_HW_CONFIG_tag
{
   /* External/Internal Enable: 0=Ignore, 1=Internal, 2=External */
   tSIRF_UINT8 ExtLNAEnable;
   /* SSB not allowed to change these Reserved fields */
   tSIRF_UINT8 Reserved;
   tSIRF_UINT8 Reserved1[SIRF_MSG_SSB_TRK_HW_CONFIG_RF_OVRD_MSG_LENGTH];
} tSIRF_MSG_SSB_TRK_HW_CONFIG;

/*******************************************************************************
   SIRF_MSG_SSB_TRKR_CUSTOMIO                ( 0x01B2 ) 178 SID 1
*******************************************************************************/
#define SIRF_MSG_SSB_TRKR_CUSTOMIO_SEL1   ( 3 )
#define CUSTOMIO_SEL1                     ( 1 )

typedef struct tSIRF_MSG_SSB_TRKR_CUSTOMIO_SEL1_tag
{
   tSIRF_UINT8 m_type;          /**< CUSTOMIO_SEL1 I/O type */
   tSIRF_UINT8 m_ver;           /**< CUSTOMIO_SEL1 I/O version */
   tSIRF_UINT8 m_pin_config;    /**< CUSTOMIO_SEL1 Pin Config 1 to 7 */
}  tSIRF_MSG_SSB_TRKR_CUSTOMIO_SEL1;

/*******************************************************************************
   SIRF_MSG_SSB_TRKR_CONFIG                 ( 0x02B2 ) 178 SID 2
*******************************************************************************/
#define SIRF_MSG_SSB_TRKR_CONFIG_LENGTH   ( 55 )

typedef struct tSIRF_MSG_SSB_MEI_TRKR_CONFIG_tag
{
   tSIRF_UINT32 ref_clk_frequency;             /**< The unit is in Hz */
   tSIRF_UINT16 ref_clk_warmup_delay;          /**< The unit is in RTC cycles. */
   tSIRF_UINT32 ref_clk_uncertainty;           /**< The unit is in ppb */
   tSIRF_INT32 ref_clk_offset;                 /**< The unit is in Hz */
   tSIRF_UINT8 ext_lna_enable;                 /**< 0=Internal, 1=External */
   tSIRF_UINT8 io_pin_config_enable;           /**< 0= ignore IoPinConfig field, 1=apply IoPinConfig field */
   tSIRF_UINT16 io_pin_config[SIRFNAV_UI_CTRL_NUM_GPIO_PINS_CONFIG];
                                               /**< I/O Pin setting */
   tSIRF_UINT32 uart_baud_rate;                /**< UART baud */
   tSIRF_UINT8 uart_hw_fc;                     /**< 0= HW flow control OFF, 1= ON */
   tSIRF_UINT8 uart_idle_byte_wakeup_delay;    /**< Number of bytes worth of delay between preamble transmissions */
   tSIRF_UINT8 uart_max_preamble;              /**< The unit is in number of message transmissions */
   tSIRF_UINT16 i2c_master_addr;               /**< I2C Master (host) address */
   tSIRF_UINT16 i2c_slave_addr;                /**< I2C Slave (Tracker) address */
   tSIRF_UINT8 i2c_rate;                       /**< 0=100 kbps,1=400 kbps, 2=1000 kbps, 3= 3400 kbps  */
   tSIRF_UINT8 i2c_mode;                       /**< 0=Slave, 1=Multi-master */
   tSIRF_UINT16 i2c_max_msg_length;            /**< Maximum message length of I2C Msg */
   tSIRF_UINT8 pwr_ctrl_on_off;                /**< Power Control On Off Control Byte */
   tSIRF_UINT8 backup_LDO_mode_enabled;        /**< 0=Switcher mode, 1=Internal backup LDO mode */
}  tSIRF_MSG_SSB_TRKR_CONFIG;

/*******************************************************************************
  SIRF_MSG_SSB_TRKR_PEEKPOKE_CMD         ( 0x03B2 ) 178 SID 3
*******************************************************************************/
#define MAX_DATA_SIZE   1000 /**< number of bytes */
typedef struct tSIRF_MSG_SSB_TRKR_PEEKPOKE_CMD_tag
{
   tSIRF_UINT8 type;       /**< same as 'cmd' in tSIRF_MSG_SSB_DATA_PEEKPOKE_tag */
   tSIRF_UINT8 access;     /**< same as 'mode' in tSIRF_MSG_SSB_DATA_PEEKPOKE_tag */
   tSIRF_UINT32 address;
   tSIRF_UINT16 numbytes;
   tSIRF_UINT8 data[MAX_DATA_SIZE];
} tSIRF_MSG_SSB_TRKR_PEEKPOKE_CMD;

#ifdef PVT_BUILD
/*******************************************************************************
  SIRF_MSG_SSB_PATCH_STORAGE_CONTROL         ( 0x14B2 ) 178 SID 20
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_PATCH_STORAGE_CONTROL_tag
{
   tSIRF_UINT8 patch_storage_ctrl;  /**< 00 = Don't store patch data to serial flash
                                         01 = Store patch data to serial flash (default) */
} tSIRF_MSG_SSB_PATCH_STORAGE_CONTROL;
#endif

/*******************************************************************************
  SIRF_MSG_SSB_TRKR_FLASHSTORE_RSP         ( 0x05B2 ) 178 SID 5
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_TRKR_FLASHSTORE_CMD_tag
{
   tSIRF_RESULT ack_result;        /**< 0=success, otherwise a failure */
} tSIRF_MSG_SSB_TRKR_FLASHSTORE_CMD;

/*******************************************************************************
  SIRF_MSG_SSB_TRKR_FLASHERASE_RSP         ( 0x06B2 ) 178 SID 6
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_TRKR_FLASHERASE_CMD_tag
{
   tSIRF_RESULT ack_result;        /**< 0=success, otherwise a failure */
} tSIRF_MSG_SSB_TRKR_FLASHERASE_CMD;

/*******************************************************************************
  SIRF_MSG_SSB_TRKR_CONFIG_RSP           ( 0x07B2 ) 178 SID 7
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_TRKR_CONFIG_CMD_tag
{
   tSIRF_UINT8 ack_result;        /**< 0=success, otherwise a failure */
} tSIRF_MSG_SSB_TRKR_CONFIG_CMD;

/*******************************************************************************
  SIRF_MSG_SSB_TRKR_CUSTOMIO_RSP           ( 0x08B2 ) 178 SID 8
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_TRKR_CUSTOMIO_CMD_tag
{
   /* This message will only be sent in success case.  If no ACK received
      from tracker, then this message will not be sent at all to user.   */
   tSIRF_UINT8 reserved;        /**< This message sends no payload; only the ID
                                   is sent.                                   */
} tSIRF_MSG_SSB_TRKR_CUSTOMIO_CMD;

/*******************************************************************************
SIRF_MSG_SSB_GPIO_WRITE           (0xB2, 0x30)
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_GPIO_WRITE_tag
{
   tSIRF_UINT16 gpio_to_write;  /* GPIO0 - bit 0, GPIO1 - bit 1,... ;
                                 * 0 => don't alter state, 1 => update the state */
   tSIRF_UINT16 gpio_state;     /* GPIO0 - bit 0, GPIO1 - bit 1,... ;
                                 * 0 => make GPIO low, 1 => make GPIO high */
} tSIRF_MSG_SSB_GPIO_WRITE;

/*******************************************************************************
SIRF_MSG_SSB_GPIO_MODE_SET        (0xB2, 0x31)
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_GPIO_MODE_SET_tag
{
   tSIRF_UINT16 gpio_to_set;    /* GPIO0 - bit 0, GPIO1 - bit 1,... ;
                                 * 0 => don't alter mode, 1 => update the mode */
   tSIRF_UINT16 gpio_mode;      /* GPIO0 - bit 0, GPIO1 - bit 1,... ;
                                 * 0 => make GPIO as input or restore GPIO 
                                 * original mode (see gpio_state),
                                 * 1 => make GPIO as output */
   tSIRF_UINT16 gpio_state;     /* GPIO0 - bit 0, GPIO1 - bit 1,... ;
                                 * Meaning depends on gpio_mode:
                                 * If gpio_mode == 0
                                 * 0 => make GPIO as input,
                                 * 1 => restore GPIO original mode
                                 * If gpio_mode == 1
                                 * 0 => make GPIO output low,
                                 * 1 => make GPIO output high */
} tSIRF_MSG_SSB_GPIO_MODE_SET;

#ifdef PVT_BUILD
/*******************************************************************************
  SIRF_MSG_SSB_TRKR_CONFIG_POLL              ( 0x09B2 ) 178 SID 9
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_TRKR_CONFIG_POLL_tag
{
   tSIRF_UINT8 reserved; /**< No field for this message */
} tSIRF_MSG_SSB_TRKR_CONFIG_POLL;

/*******************************************************************************
  SIRF_MSG_SSB_TRKR_CONFIG_POLL_RSP          ( 0x0AB2 ) 178 SID 10
*******************************************************************************/
/* This message uses same structure as 0xB2 0x02 (tSIRF_MSG_SSB_TRKR_CONFIG) */

/*******************************************************************************
  SIRF_MSG_SSB_CCK_POLL                      ( 0x0BB2 ) 178 SID 11
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_CCK_POLL_tag
{
   tSIRF_UINT8 reserved; /**< No field for this message */
} tSIRF_MSG_SSB_CCK_POLL;

/*******************************************************************************
  SIRF_MSG_SSB_ENB_ALM2FLASH                 ( 0x40B2 ) 178 SID 64
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_ENB_ALM2FLASH_tag
{
   tSIRF_UINT8 enb_alm2flash;    /**< 00 = Almanac storage on flash disabled
                                      01 = Almanac storage on flash enabled */
}tSIRF_MSG_SSB_ENB_ALM2FLASH;
#endif /* PVT_BUILD */

/*******************************************************************************
SIRF_MSG_SSB_SW_COMMANDED_OFF SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xCD, 0x10)
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_SW_COMMANDED_OFF_tag
{
   tSIRF_UINT8 reserved; /**< No field for this message */
} tSIRF_MSG_SSB_SW_COMMANDED_OFF;

/*******************************************************************************
      SIRF_MSG_SSB_TRK_HW_TEST_CONFIG          ( 0xCF ) 207
*******************************************************************************/

#define SIRF_MSG_SSB_TRK_HW_TEST_CONFIG_RF_TP_IGNORE               (0)
#define SIRF_MSG_SSB_TRK_HW_TEST_CONFIG_RF_TP_ENABLE               (1)
#define SIRF_MSG_SSB_TRK_HW_TEST_CONFIG_RF_TP_DISABLE              (2)

#define SIRF_MSG_SSB_TRK_HW_TEST_CONFIG_INT_CPU_PAUSE_IGNORE       (0)
#define SIRF_MSG_SSB_TRK_HW_TEST_CONFIG_INT_CPU_PAUSE_ENABLE       (1)
#define SIRF_MSG_SSB_TRK_HW_TEST_CONFIG_INT_CPU_PAUSE_DISABLE      (2)

typedef struct tSIRF_MSG_SSB_TRK_HW_TEST_CONFIG_tag
{
   tSIRF_UINT8 RFTestPoint;   /* RF Test Point   0=Ignore, 1=Enable, 2=Disable */
   tSIRF_UINT8 INTCpuPause;   /* INT_CPUPause      0=Ignore, 1=Enable, 2=Disable */

} tSIRF_MSG_SSB_TRK_HW_TEST_CONFIG;

/*******************************************************************************
      SIRF_MSG_SSB_SET_IF_TESTPOINT          ( 0xD2 ) 210
*******************************************************************************/

#define SET_IF_TP_OFF                          0
#define GRF3IPLUS_SET_IF_TP_V_TUNE             1
#define GRF3IPLUS_SET_IF_TP_IF_FLTR_INPUT      2
#define GRF3IPLUS_SET_IF_TP_IF_FLTR_OUTPUT     3
#define GRF3IPLUS_SET_IF_TP_BANDGAP_REF        4
#define GRF3IPLUS_SET_IF_TP_SIGN_BIT           7
#define GRF3IPLUS_SET_IF_TP_MAG_BIT           15

typedef struct tSIRF_MSG_SSB_SET_IF_TESTPOINT_tag
{
   tSIRF_UINT8 test_point_control;
} tSIRF_MSG_SSB_SET_IF_TESTPOINT;

/*******************************************************************************
#define GPS_SSB_SET_IF_TESTPOINT    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xXX, 0x02)
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_SET_LNA_tag
{
   tSIRF_UINT8 internal_external; /**< 0 = External, 1 = Internal */
} tSIRF_MSG_SSB_SET_LNA;

#ifdef PVT_BUILD

/*******************************************************************************
#define SIRF_MSG_SSB_SWTB_PMLOAD_IN  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x22)
*******************************************************************************/
/* Maximum number of bytes in patch message */
#define PATCH_MAX_BUFFER_SIZE   1024

/* Fixed length size in patch load message */
/* Contains 1 byte for MID, 1 byte for SID, and 2 bytes for SeqNo */
#define PATCH_LOAD_FIXED_SIZE  4

/* Patch load message envelope size */
/* 0xA0, 0xA2, 2 bytes Payload Length, 2 byte for Checksum, 0xB0, and 0xB3 */
#define PATCH_LOAD_ENVELOPE_SIZE  8

/* Patch load payload size */
/* 1024 - 4 - 8 */
#define MAX_SWTB_PMLOAD_PDATA_SIZE  \
(PATCH_MAX_BUFFER_SIZE - PATCH_LOAD_FIXED_SIZE - PATCH_LOAD_ENVELOPE_SIZE)

typedef struct tSIRF_MSG_SSB_SWTB_PMLOAD_IN_tag
{
   tSIRF_UINT16 seqno;
   tSIRF_UINT8  PatchData[MAX_SWTB_PMLOAD_PDATA_SIZE];
} tSIRF_MSG_SSB_SWTB_PMLOAD_IN;

#endif /* PVT_BUILD */

/*******************************************************************************
   SIRF_MSG_SSB_TEXT                   ( 0xFF ) 255
*******************************************************************************/

#define SIRF_MSG_SSB_MAX_TEXT_LEN 250

typedef struct tSIRF_MSG_SSB_COMM_MESSAGE_TEXT_tag
{
   tSIRF_CHAR msg_text[SIRF_MSG_SSB_MAX_TEXT_LEN];
}  tSIRF_MSG_SSB_COMM_MESSAGE_TEXT;

typedef tSIRF_MSG_SSB_COMM_MESSAGE_TEXT  tSIRF_NAV_TEXT;
typedef tSIRF_MSG_SSB_COMM_MESSAGE_TEXT  tSIRF_COMM_MESSAGE_TR_TEXT;
/*    Later Additions  *********************/

/* Msg ID 0xAB --> MID_SiRFLocIn */
typedef struct tSIRF_MSG_SSB_SIRFLOC_IN_tag
{
   tSIRF_UINT8 sub_id;
   tSIRF_UINT16 entry;
} tSIRF_MSG_SSB_SIRFLOC_IN;

/*Msg ID 0xE0 */

typedef struct tSIRF_MSG_SSB_DATA_PEEKPOKE_tag
{
   tSIRF_UINT8 cmd;
   tSIRF_UINT8 mode;
   tSIRF_UINT32 address;
   tSIRF_UINT32 data;
} tSIRF_MSG_SSB_DATA_PEEKPOKE;

/*******************************************************************************
   SIRF_MSG_SSB_EE_INPUT                   ( 0xE8 ) 232
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_EE_INPUT_tag
{
   tSIRF_UINT8 data[500];
}tSIRF_MSG_SSB_EE_INPUT;

typedef struct tSIRF_MSG_SSB_EXTENDED_EPHEMERIS_tag
{
   tSIRF_UINT8  PRN;
   tSIRF_UINT8  ephemerisValidityFlag;
   tSIRF_UINT8  URA;
   tSIRF_UINT8  IODE;
   tSIRF_INT16  Crs;
   tSIRF_INT16  deltaN;
   tSIRF_INT32  M0;
   tSIRF_INT16  Cuc;
   tSIRF_UINT32 eccen;
   tSIRF_INT16  Cus;
   tSIRF_UINT32 sqrtA;
   tSIRF_UINT16 toe;
   tSIRF_INT16  Cic;
   tSIRF_INT32  omega0;
   tSIRF_INT16  Cis;
   tSIRF_INT32  i0;
   tSIRF_INT16  Crc;
   tSIRF_INT32  w;
   tSIRF_INT32  omegaDot;
   tSIRF_INT16  iDot;
   tSIRF_UINT16 toc;
   tSIRF_INT8   Tgd;
   tSIRF_INT8   af2;
   tSIRF_INT16  af1;
   tSIRF_INT32  af0;
   tSIRF_UINT8  age;
} tSIRF_MSG_SSB_EXTENDED_EPHEMERIS;

typedef struct tSIRF_MSG_SSB_BROADCAST_EPHEMERIS_tag
{
   tSIRF_UINT8  eph_flag;
   tSIRF_UINT8  sv_prn_num;
   tSIRF_UINT16 gps_week;
   tSIRF_UINT8  URA;
   tSIRF_UINT8  IODE;
   tSIRF_INT16  Crs;
   tSIRF_INT16  deltaN;
   tSIRF_INT32  M0;
   tSIRF_INT16  Cuc;
   tSIRF_UINT32 eccen;
   tSIRF_INT16  Cus;
   tSIRF_UINT32 sqrtA;
   tSIRF_UINT16 toe;
   tSIRF_INT16  Cic;
   tSIRF_INT32  omega0;
   tSIRF_INT16  Cis;
   tSIRF_INT32  i0;
   tSIRF_INT16  Crc;
   tSIRF_INT32  w;
   tSIRF_INT32  omegaDot;
   tSIRF_INT16  iDot;
   tSIRF_UINT16 toc;
   tSIRF_INT8   Tgd;
   tSIRF_INT8   af2;
   tSIRF_INT16  af1;
   tSIRF_INT32  af0;
   tSIRF_UINT8  age;
} tSIRF_MSG_SSB_BROADCAST_EPHEMERIS;

typedef struct tSIRF_MSG_SSB_EXTENDED_IONO_tag
{
   tSIRF_INT8  alpha[4];
   tSIRF_INT8  beta[4];
} tSIRF_MSG_SSB_EXTENDED_IONO;

#define SV_PER_PACKET                          ( 8 )

#ifdef LPL_CLM
#define SIRF_MSG_SSB_EE_AIDED_TIME_FLAG_VALID    1
#define SIRF_MSG_SSB_EE_AIDED_IONO_FLAG_VALID    1
#endif


typedef struct tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH_tag
{
   tSIRF_UINT16                       week;
   tSIRF_UINT32                       svid_mask;
   tSIRF_MSG_SSB_EXTENDED_EPHEMERIS   extended_ephemeris[SV_PER_PACKET];
   tSIRF_MSG_SSB_EXTENDED_IONO        extended_iono;
} tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH;

typedef struct tSIRF_MSG_SSB_EE_PROPRIETARY_DEBUG_tag
{
   tSIRF_UINT32 debug_flags;
}tSIRF_MSG_SSB_EE_PROPRIETARY_DEBUG;

/*******************************************************************************
#define SIRF_MSG_SSB_SIF_SET_CONFIG SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0xFC)
*******************************************************************************/
/* operation mode defines for tSIRF_MSG_SSB_SIF_SET_CONFIG */
#define SIRF_INSTANT_FIX_MODE_SGEE_CGEE_MIXED 0
#define SIRF_INSTANT_FIX_MODE_SGEE            1
#define SIRF_INSTANT_FIX_MODE_CGEE            2

#define SIRF_INSTANT_FIX_FILE_FORMAT_FF4      0
#define SIRF_INSTANT_FIX_FILE_FORMAT_FF1      1
#define SIRF_INSTANT_FIX_FILE_FORMAT_FF3      2

#define SIRF_INSTANT_FIX_CGEE_INPUT_SF        0
#define SIRF_INSTANT_FIX_CGEE_INPUT_BE        1

#define SIRF_INSTANT_FIX_EXT_GPSTIME_AVAIL    0
#define SIRF_INSTANT_FIX_EXT_GPSTIME_NONE     1

#define SIRF_INSTANT_FIX_SGEE_INPUT_SIRFNAV   0
#define SIRF_INSTANT_FIX_SGEE_INPUT_EXTAPP    1

typedef struct tSIRF_MSG_SSB_SIF_SET_CONFIG_tag
{
   /* SIF Operation Mode
      0       : SGEE-CGEE-MIXED mode : Default,
      1       : SGEE mode,
      2       : CGEE mode */
   tSIRF_UINT8 operation_mode;

   /*SIF storage file format:*/
   /* 0: FF4 Storage : Default,
      1: FF1 Storage,
      2: FF3 Storage */
   tSIRF_UINT8 file_format;

   /*Input GPS Time from external Src or from SSB Msg7,
     This is used when input cgee method is SubFrame Data*/
   /* 0: GPS Time Source Available : Default ( using SSB Msg7 to get GPS Time ),
      1: No Other GPS Time Source Available */
   tSIRF_UINT8 ext_gps_time_src;

   /*Input method for CGEE generation */
   /* 0: Sub frame data : Default,
      1: BE and Ionosphere data */
   tSIRF_UINT8 cgee_input_method;

   /*Input method for SGEE Data */
   /* 0: SGEE File retrieval using Builtin SiRFNAV Scheduler,
         SGEE thread will be used by SiRFNAV.               : Default
      1: SGEE File retrieval using External SGEE File Downloader application,
         It may be builtin with Navigation APP and SIF SGEE thread can be used by APP */
   tSIRF_UINT8 sgee_input_method;

}tSIRF_MSG_SSB_SIF_SET_CONFIG;

/*******************************************************************************
#define SIRF_MSG_SSB_EE_FILE_PART      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x12)
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_EE_FILE_PART_tag
{
   tSIRF_UINT32 buffSize;
   tSIRF_UINT8 buff[SSB_DLD_MAX_PKT_LEN];/* Correct this buffer size should match with downloader buffer*/
}tSIRF_MSG_SSB_EE_FILE_PART;

/*******************************************************************************
#define SIRF_MSG_SSB_SIF_DISABLE_AIDING   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x20)
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_SIF_DISABLE_AIDING_tag
{
   tSIRF_UINT8 sgeeDisable;
   tSIRF_UINT8 cgeeDisable;
}tSIRF_MSG_SSB_SIF_DISABLE_AIDING;

/*******************************************************************************
#define SIRF_MSG_SSB_SIF_GET_AIDING_STATUS   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x21)
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_SIF_GET_AIDING_STATUS_tag
{
   tSIRF_UINT8 reserved;
}tSIRF_MSG_SSB_SIF_GET_AIDING_STATUS;

#ifdef PVT_BUILD
/*******************************************************************************
#define SIRF_MSG_SSB_EE_STORAGE_CONTROL SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0xFD)
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_EE_STORAGE_CONTROL_tag
{
   tSIRF_UINT8 ee_storage_ctrl;  /**< 00 = storage available on host (default)
                                      01 = I2C EEPROM provided for GSD4e access
                                      10 = store to the parallel flash
                                      11 = no storage, [7:2] = reserved */
}tSIRF_MSG_SSB_EE_STORAGE_CONTROL;
#endif /* #ifdef PVT_BUILD */

/*******************************************************************************
#define SIRF_MSG_SSB_EE_DISABLE_EE_SECS SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0xFE)
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_EE_DISABLE_EE_SECS_tag
{
   tSIRF_UINT32 num_secs;  /**< Number of seconds to wait before disabling CGEE generation */
}tSIRF_MSG_SSB_EE_DISABLE_EE_SECS;

#ifdef LPL_CLM
typedef struct tSIRF_MSG_SSB_EE_SEND_BROADCAST_EPH_tag
{
   tSIRF_UINT8                         iono_flag;
   tSIRF_MSG_SSB_EXTENDED_IONO         extended_iono;
   tSIRF_UINT8                         time_flag;
   tSIRF_UINT16                        extd_gps_week;
   tSIRF_UINT32                        gps_tow;
   tSIRF_UINT8                         num_eph;
   tSIRF_MSG_SSB_BROADCAST_EPHEMERIS   eph[SIRF_MAX_SVID_CNT];
} tSIRF_MSG_SSB_EE_SEND_BROADCAST_EPH;
#endif /* LPL_CLM */

/*******************************************************************************
 #define SIRF_MSG_SSB_EE_FILE_DOWNLOAD       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x10)
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_EE_FILE_DOWNLOAD_tag
{
   tSIRF_UINT32 reserved;                         /**< empty message to trigger sgee data download */
}tSIRF_MSG_SSB_EE_FILE_DOWNLOAD;

/* START EMBEDDED CLM */
/*******************************************************************************
 #define SIRF_MSG_SSB_SIF_START_DLD       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x16)
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_SIF_START_DLD_tag
{
   tSIRF_UINT8 reserved;                         /**< empty message to indicate start of sgee data download */
}tSIRF_MSG_SSB_SIF_START_DLD;

/*******************************************************************************
 #define SIRF_MSG_SSB_SIF_EE_FILE_SIZE       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x17)
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_SIF_EE_FILE_SIZE_tag
{
   tSIRF_UINT32 file_size;                       /**< sgee file size to be downloaded */
}tSIRF_MSG_SSB_SIF_EE_FILE_SIZE;

/*******************************************************************************
 #define SIRF_MSG_SSB_SIF_PKT_DATA       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x18)
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_SIF_PKT_DATA_tag
{
   tSIRF_UINT16 pktSeqNo;                        /**< pkt Seq Number , starts from 1 */
   tSIRF_UINT16 pktLength;                       /**< length of the data following this field */
   tSIRF_UINT8  pktData[SSB_SGEE_MAX_PKT_LEN];   /**< sgee data */
}tSIRF_MSG_SSB_SIF_PKT_DATA;

/*******************************************************************************
 #define SIRF_MSG_SSB_SIF_GET_EE_AGE       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x19)
*******************************************************************************/
typedef struct {

    tSIRF_UINT8 prnNum;            /**< PRN number of satellite for which age is indicated in other fields.*/

    tSIRF_UINT8 ephPosFlag;        /**< Ephemeris flag to indicate the type of ephemeris available for the satellite:(Position Age)*0: Invalid ephemeris, not available, 1: BE, 2: SGEE, 3: CGEE*/

    tSIRF_UINT16 eePosAge;         /**< Age of EE in 0.01 days (Position Age)*/

    tSIRF_UINT16 cgeePosGPSWeek;   /**< GPS week of BE used in the CGEE generation; 0 if ephPosFlag is not set to 3 or set to 0.(Position Age)*/

    tSIRF_UINT16 cgeePosTOE;       /**< TOE of BE used in the CGEE generation; 0 if ephPosFlag is not set to 3.or set to 0(Position Age)*/

    tSIRF_UINT8 ephClkFlag;        /**< 0: Invalid ephemeris, not available, 1: BE, 2: SGEE, 3: CGEE
                                    * Ephemeris flag to indicate the type of ephemeris available for the satellite:(Clock Age)
                                    */
    tSIRF_UINT16 eeClkAge;         /**<  Age of EE in 0.01 days(Clock Age) */

    tSIRF_UINT16 cgeeClkGPSWeek;   /**< GPS week of BE used in the CGEE generation; 0 if ephClkFlag is not set to 3 or set to 0.(Clock Age)*/

    tSIRF_UINT16 cgeeClkTOE;       /**< TOE of BE used in the CGEE generation; 0 if ephClkFlag is not set to 3.or set to 0(Clock Age)*/
}tSIRF_MSG_SSB_CLM_EE_AGE;

#define SSB_MAX_NUM_EE_AGE 1

typedef struct tSIRF_MSG_SSB_SIF_GET_EE_AGE_tag
{
   tSIRF_UINT8 numSAT;                                        /**< number of satellites for which EE age is requested, repetition of following structure */
   tSIRF_MSG_SSB_CLM_EE_AGE eeAgeStruct[SSB_MAX_NUM_EE_AGE];  /**< repetition of structure for EE Age */
}tSIRF_MSG_SSB_SIF_GET_EE_AGE;

/*******************************************************************************
 #define SIRF_MSG_SSB_SIF_GET_SGEE_AGE       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x1A)
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_SIF_GET_SGEE_AGE_tag
{
   tSIRF_UINT8 satId;                            /**< satellite id for which sgee age is requested */
}tSIRF_MSG_SSB_SIF_GET_SGEE_AGE;

/*******************************************************************************
#define SIRF_MSG_SSB_HOST_RCV_PKT_DATA  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x1B)
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_HOST_RCV_PKT_DATA_tag
{
   tSIRF_UINT16 seqNum;                          /**< Packet sequence number */
   tSIRF_UINT16 NVMID;                           /**< Packet sequence number */
   tSIRF_UINT8  numBlocks;                       /**< Number of Blocks to read */
   tSIRF_UINT16 size[MAX_RCV_BLOCKS];            /**< Data size */
   tSIRF_UINT32 offset[MAX_RCV_BLOCKS];          /**< Data size */
   tSIRF_UINT8  pktData[SSB_SGEE_MAX_PKT_LEN];   /**< ee data */
}tSIRF_MSG_SSB_HOST_RCV_PKT_DATA;


/*******************************************************************************
#define SIRF_MSG_SSB_HOST_ACK_NACK      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x1C)
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_HOST_ACK_NACK_tag
{
   tSIRF_UINT8  ackMsgId;   /**< mid of msg acked */
   tSIRF_UINT8  ackSid;     /**< sid of msg acked */
   tSIRF_UINT8  ackNack;    /**< ack = 0 , nack = 1 */
   tSIRF_UINT8  reason;     /**< reason for ack */
}tSIRF_MSG_SSB_HOST_ACK_NACK;

/*******************************************************************************
 #define SIRF_MSG_SSB_SIF_GET_NVM_HEADER       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x1D)
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_SIF_GET_NVM_HEADER_tag
{
   tSIRF_UINT8 reserved;                         /**< empty message to indicate get BBRAM Header request */
}tSIRF_MSG_SSB_SIF_GET_NVM_HEADER;

/*******************************************************************************
 #define SIRF_MSG_SSB_SIF_UPDATE_NVM_HEADER       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xE8, 0x1E)
*******************************************************************************/
typedef struct tSIRF_MSG_SSB_SIF_UPDATE_NVM_HEADER_tag
{
   tSIRF_UINT16 seqNum;                          /**< Packet sequence number */
   tSIRF_UINT16 size;                            /**< Data size */
   tSIRF_UINT32 offset;                          /**< Data size */
   tSIRF_UINT8  pktData[SSB_SGEE_MAX_PKT_LEN];   /**< Header data */
}tSIRF_MSG_SSB_SIF_UPDATE_NVM_HEADER;

/* End EMBEDDED CLM */

/*    Later Additions  *********************/
/*******************************************************************************
#define SIRF_MSG_SSB_SET_REGISTER    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xXX, 0x03)
*******************************************************************************/

typedef struct tSIRF_MSG_SSB_SET_REGISTER_tag
{
   tSIRF_UINT32 reg;   /**< 32 bit address of the register */
   tSIRF_UINT32 value; /**< Value to set the register with */
} tSIRF_MSG_SSB_SET_REGISTER;

/*******************************************************************************
 * These are message structs left over from the gps_messages.h file.
 * @todo these all should be removed
 ******************************************************************************/
/******************************************************************************/
/*
 * for output message QueueError
 */
typedef struct tSIRF_MSG_SSB_QUEUE_ERROR_tag
{
   tSIRF_UINT16 ErrId;
   tSIRF_UINT16 count;
   tSIRF_UINT32* pPARAMS;
}tSIRF_MSG_SSB_QUEUE_ERROR;

/* for message QueueAlm (SSBOUT.C ) : indirectly related to message Id 146(0x92)Poll Almanac */


typedef struct tSIRF_MSG_SSB_QUEUE_ALM_ALL_tag
{
   /* This struct contains entire internal format almanac, size=32x14 */
   tSIRF_INT16      data[SIRF_MSG_SSB_ALMANAC_SIZE];

}tSIRF_MSG_SSB_QUEUE_ALM_ALL;

typedef struct tSIRF_MSG_SSB_QUEUE_ALM_RECORD_tag
{
   tSIRF_UINT8      svID;
   /* Data is 2 bytes=week&status then 12 bytes Alm Data = 14 */
   tSIRF_INT16      data[SIRF_MSG_SSB_ALMANAC_DATA_ENTRY];
}tSIRF_MSG_SSB_QUEUE_ALM_RECORD;

typedef struct tSIRF_MSG_SSB_SET_LOWPOWER_tag /* Added in order to accomodate the dutycycle field */
{
   tSIRF_INT16 DutyCycle;
   tSIRF_MSG_SSB_SET_LOW_POWER LowPower;
}tSIRF_MSG_SSB_SET_LOWPOWER;

typedef struct tSIRF_MSG_SSB_USER_PARAMS_tag
{
   tSIRF_UINT32  UserTasksEnabled;
   tSIRF_INT32  UserTaskInterval;  /**< in milliseconds */
} tSIRF_MSG_SSB_USER_PARAMS;

 typedef struct tSIRF_MSG_SSB_DATA_RECEIVER_PARAMS_tag
{
   tSIRF_MSG_SSB_SET_NAV_MODE                RP_NavModeMask;
   tSIRF_MSG_SSB_SET_DOP_MODE                RP_DOP_Mask;
   tSIRF_MSG_SSB_SET_ELEV_MASK               RP_ElevMask;
   tSIRF_MSG_SSB_SET_POWER_MASK              RP_PwrMask;
   tSIRF_MSG_SSB_SET_DGPS_MODE               RP_DGPS_Mode;
   tSIRF_MSG_SSB_SET_LOW_POWER               RP_LP_Param;
   tSIRF_MSG_SSB_USER_PARAMS                     RP_userParam;
   tSIRF_MSG_SSB_SET_LOW_POWER_PARAMS        RP_LP_AcqParam;
   tSIRF_UINT8                           RP_staticNav;
   tSIRF_INT32                          RP_correctionSrc;
   tSIRF_UINT8                           RP_enable3SVLSQ;
}tSIRF_MSG_SSB_DATA_RECEIVER_PARAMS;

/*
 * for input message HandleSetAdvNavInit
 */

/*******************************************************************************
   Ephemeris age, sent by CLM
*******************************************************************************/
typedef struct tSIRF_MSG_EE_AGE_tag
{
   tSIRF_UINT16 gps_week;     /**< GPS week number in weeks */
   tSIRF_UINT32 gps_tow;      /**< GPS time of week in seconds */
   tSIRF_UINT8  indicator;    /**< 0 means age is valid */
   tSIRF_FLOAT  age;
   tSIRF_UINT8  age_in_days;
} tSIRF_MSG_EE_AGE;


/*******************************************************************************
   Extended Ephemeris input file information. The input file is provided
   from downloader when CLM operates in SGEE mode. The information contains the
   received buffer and the size of the extended ephemeris data.
*******************************************************************************/
typedef struct tSIRF_SSB_EE_FILE_PART_INFO_tag
{
   tSIRF_UINT32   file_size;    /**< extended ephemeris data size */
   tSIRF_HANDLE   file_addr;    /**< address of buffer containing EE data*/
}tSIRF_SSB_EE_FILE_PART_INFO;

/*
** SIRF_MSG_SSB_EE_FILE_PART_STATUS - Status of Extended Ephemeris buffer
*/
#define SIRF_MSG_SSB_EE_FILE_PART_STATUS_SUCCESS     ( 0x0 )
#define SIRF_MSG_SSB_EE_FILE_PART_STATUS_FAILED      ( 0x1 )

/*******************************************************************************
   Extended Ephemeris file processing status.
   The status contains information about the file/buffer status processed by CLM.
*******************************************************************************/
typedef struct tSIRF_SSB_EE_FILE_PART_STATUS_tag
{
   tSIRF_UINT8   status;        /**< EE file part status/acknowledgement*/
}tSIRF_SSB_EE_FILE_PART_STATUS;


/* SIRF_AGPS Message Structures */

#define SIRF_MSG_SSB_RSP_NUM_SV_INFO (16)
#define SIRF_MSG_SSB_MEASUREMENT_DATA_MAX_SVS (16)
#define SIRF_MSG_SSB_CLK_BIAS_ADJUST_MAX_SVS (16)
#define SIRF_MSG_SSB_ANP_NUM_SF1_RESERVED (11)
#define SIRF_MSG_SSB_ANS123R_NUM_BYTES_PER_SF123 (113)  /* 904 bits */
#define SIRF_MSG_SSB_ANS123R_NUM_SF123 (16)
#define SIRF_MSG_SSB_ANS45R_NUM_BYTES_PER_SF45 (75)     /* 600 bits */
#define SIRF_MSG_SSB_ANS45R_NUM_SF45 (25)


/*******************************************************************************
  SIRF_MSG_SSB_POS_RSP            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x45, 0x1)
*******************************************************************************/
/* Position Error Status Field Values*/
#define SIRF_MSG_SSB_VALID_POS                         ( 0x00 )   /* Valid Position */
#define SIRF_MSG_SSB_NOT_ENOUGH_SAT_TRACKED            ( 0x01 )   /* Not Enough satellites tracked */
#define SIRF_MSG_SSB_GPS_AIDING_DATA_MISING            ( 0x02 )   /* GPS Aiding data missing (not supported) */
#define SIRF_MSG_SSB_NEED_MORE_TIME                    ( 0x03 )   /* Need more time */
#define SIRF_MSG_SSB_NO_FIX_AVAIL_AFTER_FULL_SEARCH    ( 0x04 )   /* No fix available after full search */
#define SIRF_MSG_SSB_POS_REPORTING_DISABLED            ( 0x06 )   /* Position Reporting Disabled */

/* POS_TYPE Field Specification */
#define SIRF_MSG_SSB_POS_2D                            ( 0xFC )   /* Position Type 2D */
#define SIRF_MSG_SSB_POS_3D                            ( 0xFE )   /* Position Type 3D */
#define SIRF_MSG_SSB_POS_NOT_TRKL_POWR_SLN             ( 0xDF )   /* Not a trickle power solution */
#define SIRF_MSG_SSB_POS_TRKL_POWR_SLN                 ( 0xFF )   /* Trickle power Solution */

/* Dgps Correction DGPS_COR Field Specification */
#define SIRF_MSG_SSB_NO_DGPS_CORRECTION                ( 0x00 )   /* No DGPS correction */
#define SIRF_MSG_SSB_LOCAL_DGPS_CORRECTION             ( 0x01 )   /* Local DGPS correction */
#define SIRF_MSG_SSB_WAAS_CORRECTION                   ( 0x02 )   /* WAAS correction */

/* Other Sections */
#define SIRF_MSG_SSB_HZ_ERROR_SECTION                  ( 0x01 )
#define SIRF_MSG_SSB_VRT_POS_SECTION                   ( 0x02 )
#define SIRF_MSG_SSB_VELOCITY_SECTION                  ( 0x04 )
#define SIRF_MSG_SSB_CLOCK_CORERCTION_SECTION          ( 0x08 )
#define SIRF_MSG_SSB_POS_CORERCTION_SECTION            ( 0x10 )

/*  starting at SV_PRN */
typedef struct tSIRF_MSG_SSB_NB_SV_DATA_tag
{
   tSIRF_UINT8 sv_prn;           /**< Satellite PRN number  */

   tSIRF_UINT8 c_n0;             /**< Satellite C/N0, units 1 dB-Hz, range from 0 to 60 */

   tSIRF_UINT8 inv_weights;      /**< Inverse of Weighting Factor in position computation, range 0.125 to 3968m */

} tSIRF_MSG_SSB_NB_SV_DATA;

/* starting at POSITION_ERROR_STATUS */
typedef struct tSIRF_MSG_SSB_POSITION_RSP_tag
{
   tSIRF_UINT8    pos_req_id;     /**< POS_REQ_ID that the returned position/measurements
                                     are associated with */

   tSIRF_UINT8    pos_results_flag;
                                  /**< If set to 1,some fields in the position result section
                                     are valid, else invalid */

   tSIRF_UINT8    pos_err_status; /**< position results block validity */

   tSIRF_UINT8    qos_pass_flag;  /**< POS_ACC_MET, Position Accuracy Flag */

   tSIRF_UINT8    pos_type;       /**< Bit Mask for Pos Type field */

   tSIRF_UINT8    dgps_cor;       /**< DGPS correction type */

   tSIRF_UINT16   gps_week;       /**< Extended GPS week number */

   tSIRF_UINT32   time_of_week;   /**< GPS time in the week when the position was computed.
                                    UNITS 1/1000 seconds, Range 0s to 604,799.999 seconds. */

   tSIRF_INT32    lat;            /**< Measured Latitude,units of 180/232 degrees,
                                    range from -90 degrees to +90x(1-2-31) degrees */

   tSIRF_INT32    lon;            /**< Measured Longitude,units of 360/232 degrees,
                                    range from -180 degrees to +180x(1-2-31) degrees */

   tSIRF_UINT8    other_sections; /**< Indicates the validity status of other sections */

   tSIRF_UINT8    hor_err_angle;  /**< Error Ellipse major axis angle with respect to True North in WGS84.
                                    Units of 180/28degrees, Range from 0 to +180x(1-2-7) degrees */

   tSIRF_UINT8    hor_err_major;  /**< Major Axis Standard Deviation Error */

   tSIRF_UINT8    hor_err_minor;  /**< Minor Axis Standard Deviation Error */

   tSIRF_UINT16   vert_pos_height;/**< Height , Units 0.1 m, range of -500 m  to +6053.5 m
                                     with respect to WGS84 reference ellipsoid */

   tSIRF_UINT8    vert_pos_std;   /**< Height Standard Deviation Error */

   tSIRF_UINT16   velocity_horizontal;
                                  /**< Horizontal Velocity, units of 0.0625 meters/second,
                                     range from 0 to 4095 m/s */

   tSIRF_UINT16   velocity_heading;
                                  /**< Units of 360/216 degrees, Range from 0 to 360x(1-2-16) degrees.*/

   tSIRF_INT8     velocity_vertical;
                                  /**< two's complement value of Vertical Velocity,
                                     units of 0.5m/s in the range from -64m/s to +63.5 m/s.*/

   tSIRF_UINT8    velocity_hor_err_ang;
                                  /**< Error Ellipse major axis angle with respect to True North in WGS84.
                                     Units 0.75 degrees, Range from 0 to +180x(1-2-7) degrees */

   tSIRF_UINT8    velocity_hor_err_maj_std;
                                  /**< Major Axis Standard Deviation Error */

   tSIRF_UINT8    velocity_hor_err_min_std;
                                  /**< Minor Axis Standard Deviation Error */

   tSIRF_UINT8    velocity_ver_err;
                                  /**< Height Standard Deviation Error */

   tSIRF_UINT8    clk_cor_time_ref;
                                  /**< '0' to indicate the tie reference is the local clock. '1' value is reserved.*/

   tSIRF_UINT16   clk_cor_clk_bias;
                                  /**< clock bias, Range from -429.287 seconds to +429.287 seconds with a minimum
                                     non-zero value of 100ns */

   tSIRF_UINT16   clk_cor_clk_drift;
                                  /**< clock drift in the range of -327.52ppm (or us/s) to +327.52 ppm, with a
                                     minimum non-zero value of 0.0025ppm. */

   tSIRF_UINT8    clk_cor_clk_std_err;
                                  /**< Estimated Time Accuracy */

   tSIRF_UINT8    clk_cor_utc_off;
                                  /**< The offset between GPS time and UTC time in units of seconds
                                     range of 0-255 seconds */
   tSIRF_UINT8    pos_corr_num_sat;
                                  /**< Number of Satellite Vehicles Currently Tracked range from 1 to 10 */

   tSIRF_MSG_SSB_NB_SV_DATA    pos_corr_sv_info[SIRF_MSG_SSB_RSP_NUM_SV_INFO];

}tSIRF_MSG_SSB_POSITION_RSP;

/*******************************************************************************
    SIRF_MSG_SSB_MEAS_RSP           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x45, 0x2)
 *******************************************************************************/
/* Measurement Error Status */
#define SIRF_MSG_SSB_MEAS_VALID_GPS_MEAS                   ( 0x00 )
#define SIRF_MSG_SSB_MEAS_NOT_ENOUGH_SAT_TRACKED           ( 0x01 )
#define SIRF_MSG_SSB_MEAS_GPS_AIDING_DATA_MISSING          ( 0x02 )
#define SIRF_MSG_SSB_MEAS_NEED_MORE_TIME                   ( 0x03 )
#define SIRF_MSG_SSB_MEAS_REQ_LOCN_METHOD_NOT_SUPPORTED    ( 0xFF )

/* Multi Path Indicator */
#define SIRF_MSG_SSB_MEAS_MPI_NOT_MEASURED                  ( 0x00 )
#define SIRF_MSG_SSB_MEAS_MPI_LOW_ERROR                     ( 0x01 )  /* Error< = 5 meters */
#define SIRF_MSG_SSB_MEAS_MPI_MEDIUM                        ( 0x02 )  /* 5<Error < 43 meters */
#define SIRF_MSG_SSB_MEAS_MPI_HIGH                          ( 0x03 )  /* Error > 43 meters */


typedef struct tSIRF_MSG_SSB_MEAS_PER_SV_DATA_tag
{
   tSIRF_UINT8  sv_prn;           /**< Satellite PRN number  */

   tSIRF_UINT8  c_n0;             /**< Satellite C/N0 units of 1 dB-Hz in the range from 0 to 60 */

   tSIRF_INT16 sv_doppler;        /**< Satellite Doppler Measurement */

   tSIRF_UINT16 sv_code_phase_wh; /**< Satellite Code Phase Measurement - Whole Chips. units of 1 C/A
                                    code chip, in the range from 0 to 1022 chips */

   tSIRF_UINT16 sv_code_phase_fr; /**< Satellite Code Phase Measurement - Fractional Chips
                                    units of 2-10 of C/A code chips, in the range from 0 to (2-10-1)/ 2-10 chips */

   tSIRF_UINT8  multipath_indicator;
                                  /**<Multipath Indicator, set the values as described in macros for multi path */

   tSIRF_UINT8  pseudorange_rms_error;
                                  /**< pseudorange RMS error range from 0.5m to 112m */

} tSIRF_MSG_SSB_MEAS_PER_SV_DATA;


typedef struct tSIRF_MSG_SSB_MEAS_RSP_tag
{
   tSIRF_UINT8   pos_req_id;      /**< Position/measurement response identifier */

   tSIRF_UINT8  gps_meas_flag;    /**< If set to 0x00, all fields of the GPS measurement section are invalid
                                    If set to 0x01, some fields in the GPS measurement section are valid */

   tSIRF_UINT8  meas_error_status;/**< If set to 0x00, GPS measurement information is delivered and valid.
                                     If set to any other value, the MEASUREMENT SECTION is invalid */

   tSIRF_UINT16 meas_gps_week;    /**< Extended GPS week number since the beginning of the GPS reference, in number of weeks */

   tSIRF_UINT32 meas_gps_seconds; /**< GPS time in the week when the position was computed.UNITS of 1/1000 seconds,
                                     Range from 0s to 604,799.999 seconds. */

   tSIRF_UINT8  time_accuracy;    /**< Accuracy of GPS Measurement Time Tag */

   tSIRF_UINT8  num_sv;           /**< Number of Satellite Measurements  */

   tSIRF_MSG_SSB_MEAS_PER_SV_DATA meas_per_sv_data [SIRF_MSG_SSB_MEASUREMENT_DATA_MAX_SVS];

} tSIRF_MSG_SSB_MEAS_RSP;

/*******************************************************************************
    SIRF_MSG_SSB_PARTPOS_RSP        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x45, 0x3)
 *******************************************************************************/
typedef struct
{
   tSIRF_UINT8  svid;                /**< Acquired satellite id */
   tSIRF_DOUBLE pseudorange;         /**< Pseudorange measurment */
   tSIRF_FLOAT  carrier_freq;        /**< Either delta PR normalized or freq from AFC loop */
   tSIRF_UINT8  sync_flags;          /**< 2 bits, bit 0: Coherent Integrat
                                          Interval, bit 1: Sync */
   tSIRF_UINT8  ctoN;                /**< signal pwr of bin[5] prev second in dB-Hz */
} tSIRF_MSG_SSB_PARTPOS_SVMEAS;

typedef struct
{
   tSIRF_UINT8  svid;                /**< Acquired satellite id */
   tSIRF_DOUBLE pos[3];              /**< Satellite position vector */
   tSIRF_DOUBLE vel[3];              /**< Satellite velocity vector */
   tSIRF_DOUBLE clk_bias;            /**< Clock Bias */
   tSIRF_FLOAT  clk_drift;           /**< Clock Drift */
   tSIRF_FLOAT  pos_var;             /**< Position Variance */
   tSIRF_FLOAT  clk_var;             /**< Clock Variance */
   tSIRF_FLOAT  iono;                /**< Ionospheric error */
   tSIRF_UINT8  status;              /**< Status 0 - SVSTATE_NO_DATA
                                                 1 - SVSTATE_FROM_EPH,
                                                 2 - SVSTATE_FROM_ALM,
                                                 3 - SVSTATE_FROM_ACQ_ASSIST,
                                                 4 - SVSTATE_FROM_CURRENT_ALM,
                                                 5 - SVSTATE_FROM_EPH_EXTENSION_SERVER,
                                                 6 - SVSTATE_FROM_EPH_EXTENSION_CLIENT,
                                                 7 - SVSTATE_FROM_REV_EPH_EXT_CLIENT */
}tSIRF_MSG_SSB_PARTPOS_SVSTATE;

typedef struct tSIRF_MSG_SSB_PARTPOS_RSP_tag
{
   tSIRF_UINT8    pos_req_id;           /**< POS_REQ_ID that the returned position/measurements
                                             are associated with */
   tSIRF_UINT8    pos_results_flag;     /**< If set to 1,some fields in the position result section
                                             are valid, else invalid */
   tSIRF_UINT8    pos_err_status;       /**< position results block validity */
   tSIRF_INT16    qos_value;            /**< QoS value */
   tSIRF_UINT8    pos_type;             /**< Bit Mask for Pos Type field */
   tSIRF_UINT8    dgps_cor;             /**< DGPS correction type */
   tSIRF_UINT16   gps_week;             /**< Extended GPS week number */
   tSIRF_INT32    lat;                  /**< Measured Latitude,units of 180/232 degrees,
                                             range from -90 degrees to +90x(1-2-31) degrees */
   tSIRF_INT32    lon;                  /**< Measured Longitude,units of 360/232 degrees,
                                             range from -180 degrees to +180x(1-2-31) degrees */
   tSIRF_UINT16   vert_pos_height;      /**< Height , Units 0.1 m, range of -500 m  to +6053.5 m
                                             with respect to WGS84 reference ellipsoid */
   tSIRF_UINT16   velocity_horizontal;  /**< Horizontal Velocity, units of 0.0625 meters/second,
                                             range from 0 to 4095 m/s */
   tSIRF_UINT16   velocity_heading;     /**< Units of 360/216 degrees, Range from 0 to 360x(1-2-16) degrees.*/
   tSIRF_INT8     velocity_vertical;    /**< two's complement value of Vertical Velocity,
                                             units of 0.5m/s in the range from -64m/s to +63.5 m/s.*/
   tSIRF_DOUBLE   meas_time;            /**< GPS Time for measurements estimated by software */
   tSIRF_UINT8    num_sv_meas_cnt;      /**< Number of measurments available */
   tSIRF_MSG_SSB_PARTPOS_SVMEAS sat_meas[SIRF_NUM_CHANNELS];
   tSIRF_DOUBLE   state_time;           /**< GPS Time SV States estimated by software */
   tSIRF_UINT8    num_sv_state_cnt;     /**< Number of Sat States available */
   tSIRF_MSG_SSB_PARTPOS_SVSTATE sat_state[SIRF_NUM_CHANNELS];
} tSIRF_MSG_SSB_PARTPOS_RSP;

/********************************************************************************
    SIRF_MSG_SSB_EPH_RSP            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x46, 0x1)
 *******************************************************************************/
/* GPS TIME FLAG */
#define SIRF_MSG_SSB_EXT_GPS_WEEK_TOW_NOT_VALID           ( 0x00 )
#define SIRF_MSG_SSB_EXT_GPS_WEEK_VALID_TOW_NOT_VALID     ( 0x01 )
#define SIRF_MSG_SSB_EXT_GPS_WEEK_NOT_VALID_TOW_VALID     ( 0x02 )
#define SIRF_MSG_SSB_EXT_GPS_WEEK_VALID_TOW_VALID         ( 0x03 )

/* Starting at SATID */
typedef struct tSIRF_MSG_SSB_EPH_INFO_tag
{
   tSIRF_UINT8  svid;             /**< The satellite ID (PRN number) */

   tSIRF_UINT8  sv_info_flag;     /**< The satellite info flag */

   tSIRF_UINT16 gps_week;         /**< The GPS week of the ephemeris in SLC for SATID. Value={0-1023} */

   tSIRF_UINT16 gps_toe;          /**< GPS time of ephemeris in hours of the latest ephemeris set */

   tSIRF_UINT8  iode;             /**< Issue of Data Ephemeris for SATID */

   tSIRF_UINT16 azimuth;          /**< Azimuth angle of the GPS satellite units of 1 degree. The valid value is from 0 to 359 degrees */

   tSIRF_INT8   elevation;        /**< Elevation angle of the GPS satellite units of 1 degree. The valid value is form -90 to 90 degrees */

}tSIRF_MSG_SSB_EPH_INFO;


typedef struct tSIRF_MSG_SSB_SLC_EPH_STATUS_RSP
{
   tSIRF_UINT8  gps_time_mask;    /**< GPS_TIME_FLAG Flag for the GPS time section */

   tSIRF_UINT16 gps_week;         /**< Extended GPS week number This field is only valid if bit 0 of GPS_TIME_FLAG is 1 */

   tSIRF_UINT32 gps_tow;          /**< GPS time of week in the unit of 0.1 seconds. This field is only valid if bit 1 of GPS_TIME_FLAG is 1 */

   tSIRF_UINT8  eph_status_type;  /**< The type of ephemeris status report */

   tSIRF_UINT8  gps_t_toe_limit;  /**< Tolerance of the TOE age.valid range is from 0 to 10. This parameter is currently set to 2 */

   tSIRF_UINT8  num_sv;           /**< number of satellites for which ephemeris status parameters are given */

   tSIRF_MSG_SSB_EPH_INFO eph_info[SIRF_MAX_SVID_CNT]; /**< structure that contains satellite ephemeris status information */

}tSIRF_MSG_SSB_SLC_EPH_STATUS_RSP;

/********************************************************************************
    SIRF_MSG_SSB_ACK_NACK_ERROR_OUT  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x4B, 0x1)
 *******************************************************************************/


typedef struct tSIRF_MSG_SSB_ACKNACK_ERROR_tag
{
   tSIRF_UINT8 msg_id;            /**<  msg id of the acked/nacked message  */

   tSIRF_UINT8 sub_id;            /**<  sub id of the acked/nacked message  */

   tSIRF_UINT8 session_error_reason;
                                  /**< ack/nack value as described */

   tSIRF_UINT16 reserved;         /**< reserved bytes */

}  tSIRF_MSG_SSB_ACKNACK_ERROR;


#define SIRF_MSG_SSB_ACK_VAL                         ( 0x00 )
#define SIRF_MSG_SSB_MESS_ID_NOT_RECOGNIZED          ( 0xFA )
#define SIRF_MSG_SSB_PARAMETER_NOT_RECOGNIZED        ( 0xFB )
#define SIRF_MSG_SSB_OSP_VERSION_NOT_SUPPORTED       ( 0xFC )
#define SIRF_MSG_SSB_CP_NAV_BIT_AIDING_NOT_SUPPORTED ( 0xFD )
#define SIRF_MSG_SSB_CP_EPH_STATUS_RSP_NOT_ACCEPTED  ( 0xFE )
#define SIRF_MSG_SSB_NACK_VAL                        ( 0xFF )

/*******************************************************************************
    SIRF_MSG_SSB_ALM_RSP            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x46, 0x2)
 *******************************************************************************/

/* starting at ALM_VALID_FLAG */
typedef struct tSIRF_MSG_SSB_OS_ALMANAC_tag
{
   tSIRF_UINT8  valid_flag;

   tSIRF_UINT8  svprnNum;         /**< Satellite PRN number.range from 1 to 32 */

   tSIRF_UINT16 week_num;

   tSIRF_UINT16 eccentricity;     /**< Scale Factor: 2 pow(-21) Unit: dimensionless */

   tSIRF_UINT8  toa;              /**< Scale Factor 2 pow(12)  Unit Seconds */

   tSIRF_INT16 delta_incl;        /**< Scale Factor:2 pow(-19) Unit:semi-circles */

   tSIRF_INT16 omegadot;          /**< Scale Factor:2 pow(-38) Unit:semi-circles/sec. */

   tSIRF_UINT32 a_sqrt;           /**<  note: 24 bits Scale Factor:2 pow(-11) Unit:meters1/2 */

   tSIRF_INT32 omega_0;           /**<  note: 24 bits Scale Factor:2 pow(-23) Unit:semi-circles */

   tSIRF_INT32 omega;             /**<  note: 24 bits Scale Factor:2 pow(-23) Unit:semi-circles */

   tSIRF_INT32 m0;                /**<  note: 24 bits Scale Factor:2 pow(-23) Unit:semi-circles */

   tSIRF_INT16 af0;               /**<  Scale Factor:2 pow(-20) Unit:Seconds */

   tSIRF_INT16 af1;               /**<  Scale Factor:2 pow(-38) Unit:sec/sec */

} tSIRF_MSG_SSB_OS_ALMANAC;


typedef struct tSIRF_MSG_SSB_ALMANAC_RSP_tag
{
   tSIRF_UINT8   data_flag;       /**<  bit field. Flag for each data section  */

   tSIRF_UINT16  extended_gps_week;
                                  /**< Extended GPS week number  */

   tSIRF_UINT32  gpstow;          /**< current GPS time of week in the unit of 0.1 seconds */

   tSIRF_UINT8   num_sv;          /**< number of satellites for which almanac information is being given */

   tSIRF_MSG_SSB_OS_ALMANAC alm_per_sv_list[SIRF_MAX_SVID_CNT];
                                  /**<  The structure of almanac parameters */

} tSIRF_MSG_SSB_ALMANAC_RSP;

/*******************************************************************************
     SIRF_MSG_SSB_B_EPH_RSP          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x46, 0x3)
 *******************************************************************************/
/* Support definitions for Broadcast Ephemeris Message */
#define SIRF_MSG_SSB_BEREQ_EPH_INFO_STOP_OUTPUT           (0x00)
#define SIRF_MSG_SSB_BEREQ_EPH_INFO_IMMEDIATE_IF_NEWER    (0x01)
#define SIRF_MSG_SSB_BEREQ_EPH_INFO_WHEN_UPDATED          (0x02)
#define SIRF_MSG_SSB_BEREQ_EPH_INFO_WHEN_CHANGED          (0x04)
#define SIRF_MSG_SSB_BEREQ_EPH_INFO_MASK                  (0x07)
#define SIRF_MSG_SSB_BEREQ_EPH_TOE_SCALEFACTOR            (16)
#define ALL_SVS                                           (0xFFFFFFFF)

/*   starting after EPH_FLAG */
typedef struct tSIRF_MSG_SSB_EPHEMERIS_PARAMETERS_tag
{
   tSIRF_UINT8  sv_prn_num;       /**< Satellite PRN number */

   tSIRF_UINT8  ura_ind;          /**< User range accuracy index */

   tSIRF_UINT8  iode;             /**< Issue of data ephemeris */

   tSIRF_INT16  c_rs;             /**< Amplitude of the sine harmonic correction term to the orbit radius
                                 Scale Factor: 2 pow(-5) Units:Meters  */

   tSIRF_INT16  delta_n;          /**< Mean motion difference from the computed value Scale Factor: 2 pow(-43)
                                 Units:semi-circles/sec */

   tSIRF_INT32  m0;               /**< Mean anomaly at the reference time Scale Factor: 2 pow(-31)
                                 Units: semi-circles */

   tSIRF_INT16  c_uc;             /**< Amplitude of the cosine harmonic correction term to the argument of latitude
                                 Scale Factor: 2 pow(-29) Units: Radians */

   tSIRF_UINT32 eccentricity;     /**< Scale Factor: 2 pow(-33) Units: N/A */

   tSIRF_INT16  c_us;             /**< Amplitude of the sine harmonic correction term to the argument of latitude
                                 Scale Factor: 2 pow(-29) Units: Radians */

   tSIRF_UINT32 a_sqrt;           /**< Square root of the semi-major axis Scale Factor: 2 pow(-19)
                                 Units: sqrt(meter) */

   tSIRF_UINT16 toe;              /**< Ephemeris reference time.Scale Factor: 2 pow(4) Units: seconds */

   tSIRF_INT16  c_ic;             /**< Amplitude of the cosine harmonic correction term to the angle of inclination
                                  Scale Factor: 2 pow(-29) Units: radians  */

   tSIRF_INT32  omega_0;          /**< Longitude of ascending node of orbit plane at weekly epoch
                                  Scale Factor: 2 pow(-31) Units: semi-circles */

   tSIRF_INT16  c_is;             /**< Amplitude of the sine harmonic correction term to the angle of inclination
                                  Scale Factor: 2 pow(-29) Units: Radians*/

   tSIRF_INT32  i0;               /**< Inclination angle at reference time.Scale Factor: 2 pow(-31)
                                  Units: semi-circles */

   tSIRF_INT16  c_rc;             /**< Amplitude of the cosine harmonic correction term to the orbit radius
                                  Scale Factor: 2 pow(-5) Units: meters */

   tSIRF_INT32  omega;            /**< Argument of perigee Scale Factor: 2 pow(-31) Units: semi-circles */

   tSIRF_INT32  omegadot;         /**< Rate of right ascension Scale Factor: 2 pow(-43) Units: semi-circles/sec */

   tSIRF_INT16  idot;             /**< Rate of inclination angle Scale Factor: 2 pow(-43) Units: semi-circles/sec */

   tSIRF_UINT16 toc;              /**< Clock data reference time Scale Factor: 2 pow(4) Units: seconds */

   tSIRF_INT8   t_gd;             /**< L1 and L2 correction term Scale Factor: 2 pow(-31) Units: seconds */

   tSIRF_INT8   af2;              /**< Apparent satellite clock correction Scale Factor: 2 pow(-55) Units: sec/sec2 */

   tSIRF_INT16  af1;              /**< Apparent satellite clock correction. Scale Factor: 2 pow(-43) Units: sec/sec1*/

   tSIRF_INT32  af0;              /**< Apparent satellite clock correction. Scale Factor: 2 pow(-31) Units: seconds */

} tSIRF_MSG_SSB_EPHEMERIS_PARAMETERS;

typedef struct tSIRF_MSG_SSB_BE_tag
{
   tSIRF_UINT8  eph_flag;         /**< ephemeris parameter validity flag */

   tSIRF_UINT8  health;           /**< Broadcast Ephemeris Health.0 means the satellite is healthy,*/

   tSIRF_UINT16 gps_week;         /**< gps week number */

   tSIRF_MSG_SSB_EPHEMERIS_PARAMETERS eph;
                                 /**< structure for ephemeris parameters */

} tSIRF_MSG_SSB_BE;


/* Starting at ALPHA_0 */
typedef struct tSIRF_MSG_SSB_SET_AIDING_IONO_DATA_tag
{

   tSIRF_INT8 alpha_0;            /**< Unit: Seconds  Scale Factor: 2 pow(-30) */

   tSIRF_INT8 alpha_1;            /**< Unit: sec/semi-circles Scale Factor: 2 pow(-27) */

   tSIRF_INT8 alpha_2;            /**< Unit: sec/(semi-circles)2 Scale Factor: 2 pow(-24) */

   tSIRF_INT8 alpha_3;            /**< Unit: sec/(semi-circles)3 Scale Factor: 2 pow(-24) */

   tSIRF_INT8 beta_0;             /**< Unit: Seconds Scale Factor: 2 pow(11) */

   tSIRF_INT8 beta_1;             /**< Unit: sec/semi-circles Scale Factor: 2 pow(14) */

   tSIRF_INT8 beta_2;             /**< Unit: sec/(semi-circles)2 Scale Factor: 2 pow(16) */

   tSIRF_INT8 beta_3;             /**< Unit: sec/(semi-circles)3 Scale Factor: 2 pow(16) */

} tSIRF_MSG_SSB_SET_IONO_DATA;


typedef struct tSIRF_MSG_SSB_BE_RSP_tag
{
   tSIRF_UINT8 reserved;          /**< reserved */

   tSIRF_UINT8 iono_flag;         /**< iono parameters validity flag */

   tSIRF_MSG_SSB_SET_IONO_DATA iono_data;
                                  /**< structure for iono data */

   tSIRF_UINT8  time_flag;        /**< Time parameter validity flag */

   tSIRF_UINT16 extd_gps_week;    /**< Extended GPS week number */

   tSIRF_UINT32 gps_tow;          /**< This is the time of week in unit of 0.1 seconds */

   tSIRF_UINT8  num_sv;           /**< number of satellites for which broadcast ephemeris is being given */

   tSIRF_MSG_SSB_BE be[SIRF_MAX_SVID_CNT];
                                  /**< fields are repeated a number of times indicated by num_sv  */

} tSIRF_MSG_SSB_BE_RSP;

/*******************************************************************************
   SIRF_MSG_SSB_SESSION_OPEN_RSP   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x4A, 1)
 *******************************************************************************/


typedef struct tSIRF_MSG_SSB_SESSION_OPEN_RSP_tag
{
   tSIRF_UINT8 ses_open_not;      /**< session open status */

}  tSIRF_MSG_SSB_SESSION_OPEN_RSP;


#define SIRF_MSG_SSB_SON_OPENING_SUCCEEDED          ( 0x00 )
#define SIRF_MSG_SSB_SON_OPENING_FAILED             ( 0x01 )
#define SIRF_MSG_SSB_SON_RESUME_SUCCEEDED           ( 0x80 )
#define SIRF_MSG_SSB_SON_RESUME_FAILED              ( 0x81 )

/*******************************************************************************
     SIRF_MSG_SSB_CLIENT_STATUS_RSP  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x46, 0x6)
 *******************************************************************************/

typedef struct tSIRF_MSG_SSB_SLC_STATUS_tag
{
   tSIRF_UINT8 slc_status;        /**< Client Status */

}  tSIRF_MSG_SSB_SLC_STATUS;


#define SIRF_MSG_SSB_SLCS_NO_FIX_AFTER_FULL_SEARCH    ( 0x01 )
#define SIRF_MSG_SSB_SLCS_OK_TO_SEND                  ( 0x04 )
#define SIRF_MSG_SSB_SLCS_NOT_OK_TO_SEND              ( 0x02 )
#define SIRF_MSG_SSB_SLCS_EXTENSION                   ( 0x80 )


#define SIRF_MSG_SSB_SLC_BITMASK    (0x7F)
                                  /*  Mask to extract the status bits  */

#define SIRF_MSG_SSB_OK_BITMASK     (0x06)
                                  /* Mask to extract the OK/Not OK bits */

/*******************************************************************************
    SIRF_MSG_SSB_SESSION_CLOSE_RSP  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x4A, 2)
 *******************************************************************************/


typedef struct tSIRF_MSG_SSB_SESSION_CLOSE_RSP_tag
{
   tSIRF_UINT8 ses_close_not;     /**< Session closing status. */

}  tSIRF_MSG_SSB_SESSION_CLOSE_RSP;


#define SIRF_MSG_SSB_SCN_SESSION_CLOSED             ( 0x00 )
#define SIRF_MSG_SSB_SCN_SESSION_CLOSING_FAILED     ( 0x01 )
#define SIRF_MSG_SSB_SCN_SESSION_SUSPENDED          ( 0x80 )
#define SIRF_MSG_SSB_SCN_SESSION_SUSPENSION_FAILED  ( 0x81 )

/*******************************************************************************
    SIRF_MSG_SSB_HW_CONFIG_REQ          SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x47, 0)
 *******************************************************************************/


typedef struct tSIRF_MSG_SSB_HW_CONFIG_REQ_tag
{
   tSIRF_UINT8 reserved;
} tSIRF_MSG_SSB_HW_CONFIG_REQ;


/*******************************************************************************
    SIRF_MSG_SSB_APROX_MS_POS_REQ   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x49, 0x1)
*******************************************************************************/


typedef struct  tSIRF_MSG_SSB_APRX_MS_POSITION_REQ_tag
{
   tSIRF_UINT8 reserved;
} tSIRF_MSG_SSB_APRX_MS_POSITION_REQ;

/*******************************************************************************
     SIRF_MSG_SSB_TIME_TX_REQ        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x49, 0x2)
 *******************************************************************************/


typedef struct tSIRF_MSG_SSB_TIME_TRANSFER_REQ_tag
{
   tSIRF_UINT8 reserved;
} tSIRF_MSG_SSB_TIME_TRANSFER_REQ;

/*******************************************************************************
    SIRF_MSG_SSB_FREQ_TX_REQ        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x49, 0x3)
 *******************************************************************************/


typedef struct  tSIRF_MSG_SSB_FREQ_TRANSFER_REQ_tag
{
   tSIRF_UINT8  req_info;         /**< Information field about frequency request */

}  tSIRF_MSG_SSB_FREQ_TRANSFER_REQ;


#define SIRF_MSG_SSB_FTRQ_MULTIPLE_REQ                ( 0x01 )
                                  /**< "0": single request , "1": multiple request */

#define SIRF_MSG_SSB_FTRQ_TRANSFER_MODE_ON            ( 0x02 )
                                  /**< Valid only if Bit 1 is "1":
                                     If "1", periodic "Frequency Transfer Response" mode is turned ON
                                     If "0", periodic "Frequency Transfer Response" mode is stopped */

#define SIRF_MSG_SSB_FTRQ_TURN_OFF_REFERENCE_CLOCK    ( 0x04 )
                                 /**< 0' = Don't turn off reference clock '1' = Turn off reference clock */


/*******************************************************************************
   SIRF_MSG_SSB_TIME_FREQ_APPROX_POS_RSP    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x46, 0x4)
 *******************************************************************************/


typedef struct  tSIRF_MSG_SSB_TIME_FREQ_APROX_POS_RSP_tag
{
   tSIRF_UINT8  status_resp_mask; /**< status response mask */

   tSIRF_UINT16 gps_week;         /**< extended GPS week number */

   tSIRF_UINT32 gps_tow;          /**< SLC GPS TOW, to the nearest second */

   tSIRF_UINT8  time_acc_scale;   /**< scale factor for the time accuracy status */

   tSIRF_UINT8  time_accuracy;    /**< time accuracy status with above scale */

   tSIRF_UINT8  freq_acc_scale;   /**< scale factor of the frequency accuracy */

   tSIRF_UINT8  freq_accuracy;    /**< scaled frequency accuracy status */

   tSIRF_INT16 scaled_freq_offset;/**< frequency offset measured by the SLC */

   tSIRF_UINT32 freq_time_tag;    /**< time when frequency measurement taken */

   tSIRF_UINT32 slc_hor_unc;      /**< horizontal uncertaingy in meters */

   tSIRF_UINT16 slc_ver_unc;      /**< vertical uncertaingy in 0.1 meters,
                                     range of 0 meters to 6553.5 meters */

   tSIRF_UINT32 spare03;          /**< spare */

   tSIRF_UINT32 spare47;          /**< spare */

}  tSIRF_MSG_SSB_TIME_FREQ_APROX_POS_RSP;

#define SIRF_MSG_SSB_TFAP_RSP_GPS_WEEK_IS_VALID                   ( 0x01 )
#define SIRF_MSG_SSB_TFAP_RSP_GPS_TOW_IS_VALID                    ( 0x02 )
#define SIRF_MSG_SSB_TFAP_RSP_STATUS_TIME_IS_VALID                ( 0x04 )
#define SIRF_MSG_SSB_TFAP_RSP_STATUS_FREQ_IS_VALID                ( 0x08 )
#define SIRF_MSG_SSB_TFAP_RSP_SLC_HOR_UNC_IS_VALID                ( 0x10 )
#define SIRF_MSG_SSB_TFAP_RSP_SLC_VER_UNC_IS_VALID                ( 0x20 )

/*******************************************************************************
     SIRF_MSG_SSB_REJECT_OUT          SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x4B, 0x2)
 *******************************************************************************/


typedef struct  tSIRF_MSG_SSB_REJECT_tag
{
   tSIRF_UINT8  rej_msgid;        /**< Message ID of Rejected Message */

   tSIRF_UINT8  rej_msgsubid;     /**< Message Sub ID of Rejected Message */

   tSIRF_UINT8  reason;           /**< the reason of the reject */

}  tSIRF_MSG_SSB_REJECT_CP, tSIRF_MSG_SSB_REJECT_SLC;


#define SIRF_MSG_SSB_REJ_NOT_SUPPORTED                   ( 0x01 )
#define SIRF_MSG_SSB_REJ_NOT_READY                       ( 0x02 )
#define SIRF_MSG_SSB_REJ_NOT_AVAILABLE                   ( 0x04 )
#define SIRF_MSG_SSB_REJ_WRONGLY_FORMATTED_MESSAGE       ( 0x08 )
#define SIRF_MSG_SSB_REJ_NO_TIME_PULSE_DURING_PRECISE_TT ( 0x10 )

/*******************************************************************************
    SIRF_MSG_SSB_SERIAL_SETTINGS_RSP SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x46, 0x8)
 *******************************************************************************/


typedef struct  tSIRF_MSG_SSB_SERIAL_SETTING_RSP_tag
{
   tSIRF_UINT8  port;             /**< Serial Port A or B*/

   tSIRF_UINT32 baud;             /**< desired baud rate.*/

   tSIRF_UINT8  ack_number;       /**< Acknowledge Number */

}  tSIRF_MSG_SSB_SERIAL_SETTING_RSP;

/*******************************************************************************
    SIRF_MSG_SSB_CH_LOAD_RSP        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x46, 0x5)
 *******************************************************************************/

typedef struct  tSIRF_MSG_SSB_CHANNEL_LOAD_RSP_ONE_CHANNEL_tag
{
   tSIRF_UINT8 load;               /**< Bandwidth, from 0 - 100 */
}  tSIRF_MSG_SSB_CHANNEL_LOAD_RSP_ONE_CHANNEL;


typedef struct  tSIRF_MSG_SSB_CHANNEL_LOAD_RSP_tag
{
   tSIRF_UINT8 port;              /**< 0 = Port A, 1 = Port B */

   tSIRF_UINT8 total_load;        /**< Bandwidth, from 0 - 100 */

   tSIRF_UINT8 number_of_channels;/**< count of folowing struct */

   tSIRF_MSG_SSB_CHANNEL_LOAD_RSP_ONE_CHANNEL load_entry[SIRF_NUM_CHANNELS];

}  tSIRF_MSG_SSB_CHANNEL_LOAD_RSP;

/*******************************************************************************
    SIRF_MSG_SSB_TX_BLANKING_RESP   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x46, 0x9)
 *******************************************************************************/


typedef struct  tSIRF_MSG_SSB_TX_BLANKING_RSP_tag
{
   tSIRF_UINT8 ack_nack;          /**< 0 ack, 1 nack */
   tSIRF_UINT8 spare;
}  tSIRF_MSG_SSB_TX_BLANKING_RSP;

/*******************************************************************************
    7.5.22.2 Test Mode Configuration Response
 *******************************************************************************/

typedef struct  tSIRF_MSG_SSB_TEST_MODE_CONFIG_RSP_tag
{
   tSIRF_UINT8 reserved;
}  tSIRF_MSG_SSB_TEST_MODE_CONFIG_RSP;

/*******************************************************************************
    SIRF_MSG_SSB_OSP_REV_RSP        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x46, 0x7)
 *******************************************************************************/


#define OSP_REVISION (0x1)

typedef struct tSIRF_MSG_SSB_OSP_REV_RSP_tag
{
   tSIRF_UINT8 osp_revision;      /**< range of 1.0-25.5, value 10 in this field translates to OSP rev 1.0 */
}  tSIRF_MSG_SSB_OSP_REV_RSP;


/*******************************************************************************
    SIRF_MSG_SSB_NBA_REQ            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x49, 0x4)
 *******************************************************************************/


#define SIRF_MSG_SSB_ASAR_NBRF_SF_123_REQUESTED (0x01)
#define SIRF_MSG_SSB_ASAR_NBRF_SF_45_REQUESTED  (0x02)


typedef struct tSIRF_MSG_SSB_SLC_AIDING_REQ_tag
{
   tSIRF_UINT16 section_validity_flag;
                                  /**< 0 = NAVBIT section is NOT valid 1 = is valid */

   tSIRF_UINT8  section_size;     /**< The size of this section in bytes.*/

   tSIRF_UINT32 sat_mask_nav_bit; /**< Bitmap representing the satellites for which NavBits are requested */

   tSIRF_UINT8  nav_bit_req_flag; /**< Bit 0:Subframe 1,2,and 3 Bit 1 Subframe 4 and 5 Bit 2-7: Reserved */

}tSIRF_MSG_SSB_SLC_AIDING_REQ;

/*******************************************************************************
    SIRF_MSG_SSB_MPM_STATUS_SVD         SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x4D, 1)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_MPM_STATUS_SVD_tag
{
   tSIRF_UINT32 gpsTow;
   tSIRF_UINT16 timeinFPMode;
   tSIRF_UINT16 rtcUncAtWakeUp;
   tSIRF_UINT16 totalRtcCorr;
   tSIRF_UINT8  unusedTokens;
   tSIRF_UINT8  trTemp;
}  tSIRF_MSG_SSB_MPM_STATUS_SVD;

/*******************************************************************************
    SIRF_MSG_SSB_MPM_STATUS_EPH         SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x4D, 2)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_MPM_STATUS_EPH_tag
{
   tSIRF_UINT32 gpsTow;
   tSIRF_UINT32 svBitMaskBefore;
   tSIRF_UINT32 svBitMaskAfter;
   tSIRF_UINT16 timeinFPMode;
   tSIRF_UINT16 rtcUncAtWakeUp;
   tSIRF_UINT16 totalRtcCorr;
   tSIRF_UINT8  navSuccess;
   tSIRF_UINT8  unusedTokens;
   tSIRF_UINT8  trTemp;
}  tSIRF_MSG_SSB_MPM_STATUS_EPH;

/*******************************************************************************
    SIRF_MSG_SSB_MPM_STATUS_ALM         SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x4D, 3)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_MPM_STATUS_ALM_tag
{
   tSIRF_UINT32 gpsTow;
   tSIRF_UINT16 timeinFPMode;
   tSIRF_UINT16 rtcUncAtWakeUp;
   tSIRF_UINT16 totalRtcCorr;
   tSIRF_UINT8  idAlmCollected;
   tSIRF_UINT8  collectionSuccess;
   tSIRF_UINT8  navSuccess;
   tSIRF_UINT8  unusedTokens;
   tSIRF_UINT8  trTemp;
}  tSIRF_MSG_SSB_MPM_STATUS_ALM;

/*******************************************************************************
    SIRF_MSG_SSB_MPM_STATUS_UPD         SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x4D, 4)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_MPM_STATUS_UPD_tag
{
   tSIRF_UINT32 gpsTow;
   tSIRF_INT32  meanCodeRes;
   tSIRF_INT32  stdPR;
   tSIRF_INT32  meanDoppRes;
   tSIRF_INT32  stdDR;
   tSIRF_UINT16 rtcUncAtWakeUp;
   tSIRF_UINT16 totalRtcCorr;
   tSIRF_INT16  uNavTimeCorr;
   tSIRF_UINT8  unusedTokens;
   tSIRF_UINT8  trTemp;
   tSIRF_UINT8  uNavStatus;
   tSIRF_UINT8  numBESVs;
   tSIRF_UINT8  numEESVs;
   tSIRF_UINT8  numAlmSVs;
}  tSIRF_MSG_SSB_MPM_STATUS_UPD;

/*******************************************************************************
    SIRF_MSG_SSB_MPM_STATUS_REC         SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x4D, 5)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_MPM_STATUS_REC_tag
{
   tSIRF_UINT32 gpsTow;
   tSIRF_INT32  totalTimeCorr;
   tSIRF_UINT16 timeinFPMode;
   tSIRF_UINT16 rtcUncAtWakeUp;
   tSIRF_UINT16 totalRtcCorr;
   tSIRF_UINT8  bitSyncDone;
   tSIRF_UINT8  frameSyncDone;
   tSIRF_UINT8  unusedTokens;
   tSIRF_UINT8  trTemp;
}  tSIRF_MSG_SSB_MPM_STATUS_REC;

/*******************************************************************************
    SIRF_MSG_SSB_PWR_MODE_RSP           SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x5A, 0)
 *******************************************************************************/

typedef struct tSIRF_MSG_SSB_PWR_MODE_RSP_tag
{
   tSIRF_UINT8  reserved;
}tSIRF_MSG_SSB_PWR_MODE_RSP;

/*******************************************************************************
    SIRF_MSG_SSB_PWR_MODE_MPM_RSP       SIRF_MAKE_MSG_ID(SIRF_LC_SSB,0x5A, 0x02)
 *******************************************************************************/

typedef struct tSIRF_MSG_SSB_PWR_MODE_MPM_RSP_tag
{
   tSIRF_UINT16  errorCode;
   tSIRF_UINT16  reserved;
}tSIRF_MSG_SSB_PWR_MODE_MPM_RSP;

/*******************************************************************************
    XO MESSAGES OUT
 *******************************************************************************/
/*******************************************************************************
    SIRF_MSG_SSB_XO_DEFAULTS_OUT     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x01)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_XO_DEFAULTS_OUT_tag
{
   tSIRF_UINT8    source;         /**< Bit Mask with the source of entries */

   tSIRF_UINT8    agingRateUnc;   /**< aging rate uncertainty,
                                   * LSB 0.1,  ppm/year  */

   tSIRF_UINT8    initialOffsetUnc;/**< initial offset uncertainty,
                                   * LSB 0.1,  ppm  */
   tSIRF_UINT8    spare1;

   tSIRF_INT32    clockDrift;     /**< clock drift,
                                   * LSB 1,    ppb */

   tSIRF_UINT16   tempUnc;        /**< temperature uncertainty,
                                   * LSB 0.01, ppm  */

   tSIRF_UINT16   mfgWeek;        /**< manufacturing wn for aging,
                                   * LSB 1,    wn  */
   tSIRF_UINT32   spare2;
} tSIRF_MSG_SSB_XO_DEFAULTS_OUT;

/*******************************************************************************
 SIRF_MSG_SSB_TCXO_TABLE_OUT       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x02)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_TCXO_TABLE_OUT_tag
{
   tSIRF_UINT32   ctr;             /**< Counter incremented with each output */
   tSIRF_INT16    offset;          /**< frequency offset bias from CD default
                                    * LSB 1, ppb */
   tSIRF_INT16    globalMin;       /**< minimum xo error observed
                                    * LSB 1, ppb */
   tSIRF_INT16    globalMax;       /**< maximum xo error observed
                                    * LSB 1, ppb */
   tSIRF_UINT16   firstWeek;       /**< full gps week of first table update.
                                    * LSB 1 */
   tSIRF_UINT16   lastWeek;        /**< full gps week of last table update
                                    * LSB 1 */
   tSIRF_UINT16   lsb;             /**< array LSB of Min[] and Max[]
                                    * LSB 1, ppb */
   tSIRF_UINT8    agingBin;        /**< Bin of last aging update. */
   tSIRF_INT8     agingUpcount;    /**< Aging detection accumulator
                                    * LSB 1 */
   tSIRF_UINT8    binCnt;          /**< count of min bins filled */
   tSIRF_UINT8    spare;           /**< not used */
   tSIRF_INT8     min[SSB_XOT_TABLE_SIZE];
                                   /**< Min XO error at each temperature
                                    * LSB xoTable.lsb */
   tSIRF_INT8     max[SSB_XOT_TABLE_SIZE];
                                   /**< Max XO error at each temperature
                                    * LSB xoTable.lsb */
} tSIRF_MSG_SSB_TCXO_TABLE_OUT;

/*******************************************************************************
 #define SIRF_MSG_SSB_XO_TEMP_REC_OUT    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x04)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_XO_TEMP_REC_OUT_tag
{
   tSIRF_UINT32   currentTimeCount;/**< time since power on, ms */
   tSIRF_UINT16   trTime;          /**< Latest TRec temperature. */
   tSIRF_UINT8    trTemp;          /**< Latest TRec temperature. */
   tSIRF_UINT8    nCnt;            /**< Rec N Count. */
   tSIRF_UINT8    totCnt;          /**< Total Rec Count. */
   tSIRF_UINT8    status;          /**< Echo Test Control mode */
   tSIRF_UINT16   ctr;             /**< counter for output messages.
                                    *  uninitialized and wraparound counter. */
} tSIRF_MSG_SSB_XO_TEMP_REC_OUT;

/*******************************************************************************
 #define SIRF_MSG_SSB_XO_EARC_OUT        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x05)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_XO_EARC_OUT_tag
{
   tSIRF_UINT32   currentTimeCount;          /**< time since power on, ms */
   tSIRF_UINT32   acqclkLsw;                 /**< acqclk, lsw */
   tSIRF_UINT32   rtcWclkSec;                /**< rtc wclk secs */
   tSIRF_UINT16   rtcWclkCtr;                /**< rtc wclk ctr */
   tSIRF_UINT16   EARC_r0;                   /**< earc r0 */
   tSIRF_UINT16   EARC_r1;                   /**< earc r1 */
   tSIRF_UINT16   spare;
} tSIRF_MSG_SSB_XO_EARC_OUT;

/*******************************************************************************
 #define SIRF_MSG_SSB_XO_RTC_ALARM_OUT   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x06)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_XO_RTC_ALARM_OUT_tag
{
   tSIRF_UINT32   currentTimeCount;          /**< time since power on, ms */
   tSIRF_UINT32   acqclkLsw;                 /**< acqclk, lsw */
   tSIRF_UINT32   rtcWclkSec;                /**< rtc wclk secs */
   tSIRF_UINT16   rtcWclkCtr;                /**< rtc wclk ctr */
   tSIRF_UINT16   spare;

} tSIRF_MSG_SSB_XO_RTC_ALARM_OUT;

/*******************************************************************************
 #define SIRF_MSG_SSB_XO_RTC_CAL_OUT     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x07)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_XO_RTC_CAL_OUT_tag
{
   tSIRF_UINT32   currentTimeCount;          /**< time since power on, ms */
   tSIRF_UINT32   acqclkLsw;                 /**< acqclk, lsw */
   tSIRF_UINT32   gpsTimeSec;                /**< gps time, secs */
   tSIRF_UINT32   gpsTimeFrac;               /**< gps time, fractional nsecs */
   tSIRF_UINT32   rtcWclkSec;                /**< rtc time, seconds */
   tSIRF_UINT16   rtcWclkCtr;                /**< rtc time, counter */
   tSIRF_UINT16   rtcFreqUnc;                /**< rtc freq unc, ppb */
   tSIRF_INT32    rtcDriftInt;               /**< rtc drift ratio, integer */
   tSIRF_UINT32   rtcDriftFrac;              /**< rtc drift ratio, frac */
   tSIRF_UINT32   rtcTimeUnc;                /**< rtc time unc, usec */
   tSIRF_INT32    rtcFreqOffset;             /**< rtc freq offset, ppb*/
   tSIRF_INT32    xoFreqOffset;              /**< rtc freq offset, ppb */
   tSIRF_UINT16   gpsWeek;                   /**< gps week */
   tSIRF_UINT16   spare;

} tSIRF_MSG_SSB_XO_RTC_CAL_OUT;

/*******************************************************************************
 #define SIRF_MSG_SSB_XO_MPM_ACQ_OUT     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x09)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_XO_MPM_ACQ_OUT_REC_tag
{
   tSIRF_UINT32   codePhase;                 /**< code Phase */
   tSIRF_INT32    doppler;                   /**< doppler */
   tSIRF_UINT32   codeOffset;                /**< code Offset */
   tSIRF_UINT32   peakMag;                   /**< peak Magnitude */
   tSIRF_UINT16   status;                    /**< status */
   tSIRF_UINT8    prn;                       /**< svid */
   tSIRF_UINT8    spare;
} tSIRF_MSG_SSB_XO_MPM_ACQ_OUT_REC;

typedef struct tSIRF_MSG_SSB_XO_MPM_ACQ_OUT_tag
{
   tSIRF_UINT8    numRecs;                   /**< number of records */
   tSIRF_UINT8    spare1;
   tSIRF_UINT16   spare2;
   tSIRF_UINT32   currentTimeCount;          /**< time since power on, ms */
   tSIRF_UINT32   acqclkLsw;                 /**< acqclk, lsw */
   tSIRF_MSG_SSB_XO_MPM_ACQ_OUT_REC *pRec;   /**< following fields are based on numRecs */
} tSIRF_MSG_SSB_XO_MPM_ACQ_OUT;

/*******************************************************************************
 #define SIRF_MSG_SSB_XO_MPM_PREPOS_OUT  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x0A)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_XO_MPM_PREPOS_OUT_REC_tag
{
   tSIRF_UINT32   pr;                        /**< psuedoRange, 0.1 m */
   tSIRF_INT16    prr;                       /**< psuedoRange Rate, m/s  */
   tSIRF_UINT8    prn;                       /**< svid */
   tSIRF_UINT8    spare;

} tSIRF_MSG_SSB_XO_MPM_PREPOS_OUT_REC;

typedef struct tSIRF_MSG_SSB_XO_MPM_PREPOS_OUT_tag
{
   tSIRF_UINT8    numRecs;                   /**< number of records */
   tSIRF_UINT8    spare1;
   tSIRF_UINT16   spare2;
   tSIRF_UINT32   currentTimeCount;          /**< time since power on, ms */
   tSIRF_UINT32   acqclkLsw;                 /**< acqclk, lsw */
   tSIRF_MSG_SSB_XO_MPM_PREPOS_OUT_REC *pRec;/**< following fields are based on numRecs */

} tSIRF_MSG_SSB_XO_MPM_PREPOS_OUT;

/*******************************************************************************
 #define SIRF_MSG_SSB_XO_UNL_MEAS_OUT    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x0B)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_XO_UNL_MEAS_OUT_REC_tag
{
   tSIRF_UINT32   pr;                        /**< psuedoRange, 0.1 m */
   tSIRF_INT16    prr;                       /**< psuedoRange Rate, m/s  */
   tSIRF_UINT16   cno;                       /**< CNo, 0.1 dbHz */
   tSIRF_UINT8    prn;                       /**< svid */
   tSIRF_UINT8    spare1;
   tSIRF_UINT16   spare2;
} tSIRF_MSG_SSB_XO_UNL_MEAS_OUT_REC;

typedef struct tSIRF_MSG_SSB_XO_UNL_MEAS_OUT_tag
{
   tSIRF_UINT8    numRecs;                   /**< number of records */
   tSIRF_UINT8    mode;
   tSIRF_UINT16   spare;
   tSIRF_UINT32   currentTimeCount;          /**< time since power on, ms */
   tSIRF_UINT32   acqclkLsw;                 /**< acqclk, lsw */
   tSIRF_INT32    timeCorr;                  /**< time correction, ms */
   tSIRF_UINT32   timeCorrUnc;               /**< time correction unc, */
   tSIRF_INT16    freqCorr;                  /**< freqeuncy correction, */
   tSIRF_UINT16   freqCorrUnc;               /**< freqeuncy correction unc, */
   tSIRF_MSG_SSB_XO_UNL_MEAS_OUT_REC *pRec;  /**< number of records of following fields */

} tSIRF_MSG_SSB_XO_UNL_MEAS_OUT;

/*******************************************************************************
 #define SIRF_MSG_SSB_XO_UNCERTAINTY_OUT SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x0C)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_XO_UNCERTAINTY_OUT_tag
{
   tSIRF_UINT32   currentTimeCount;          /**< time since power on, ms */
   tSIRF_UINT32   acqclkLsw;                 /**< acqclk, lsw */
   tSIRF_INT32    freq;                      /**< freq, ppb */
   tSIRF_UINT16   freqUncNom;                /**< freq unc nominal, ppb */
   tSIRF_UINT16   freqUncFull;               /**< freq unc full, ppb */
   tSIRF_UINT16   tempUncNom;                /**< temperature unc nominal, ppb */
   tSIRF_UINT16   tempUncFull;               /**< temperature unc full, ppb */
   tSIRF_UINT16   ageUncNom;                 /**< age unc nominal, ppb */
   tSIRF_UINT16   ageUncFull;                /**< age unc full, ppb */
   tSIRF_UINT16   measUncNom;                /**< meas unc nominal, ppb */
   tSIRF_UINT16   measUncFull;               /**< meas unc full, ppb */
   tSIRF_UINT16   gpsWeek;                   /**< gps week */
   tSIRF_UINT8    trTemp;                    /**< Latest TRec temperature. */
   tSIRF_UINT8   spare;

} tSIRF_MSG_SSB_XO_UNCERTAINTY_OUT;

/*******************************************************************************
 #define SIRF_MSG_SSB_XO_SYS_TIME_OUT    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x0D)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_XO_SYS_TIME_OUT_tag
{
   tSIRF_UINT32   currentTimeCount;          /**< time since power on, ms */
   tSIRF_UINT32   acqclkMsw;                 /**< acqclk, msw */
   tSIRF_UINT32   acqclkLsw;                 /**< acqclk, lsw */
   tSIRF_UINT32   gpsTimeSec;                /**< gps time, secs */
   tSIRF_UINT32   gpsTimeFrac;               /**< gps time, fractional nsecs */
   tSIRF_UINT32   rtcWclkSec;                /**< rtc wclk secs */
   tSIRF_UINT16   rtcWclkCtr;                /**< rtc wclk ctr */
   tSIRF_INT16    clockBias;                 /**< clock bias, m */
   tSIRF_INT16    clockDrift;                /**< clock drift, m/s */
   tSIRF_UINT16   spare;

} tSIRF_MSG_SSB_XO_SYS_TIME_OUT;


typedef struct tSIRF_MSG_SSB_XO_RTC_TABLE_OUT_tag
{
   tSIRF_UINT16   val[SSB_XO_RTC_TABLE_SIZE];/** Offset from Max value in scale units (3.05 ppb)
                                               *  Def = 65535 */
   tSIRF_INT16    fMax;                      /** Max value = (1 - f) * 1e-6 */
   tSIRF_UINT16   spare;

} tSIRF_MSG_SSB_XO_RTC_TABLE_OUT;

/*******************************************************************************
 * For XO_ENABLED
 #define SIRF_MSG_SSB_XO_POLY_OUT           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x0F)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_XO_POLY_OUT_tag
{
   tSIRF_UINT32 C[3];                     /** polynomial co-efficients */
                                          /** C0: frequency error at inflection temperature c2, in 0.01 ppb */
                                          /** C1: K factor with range from -2 to 8, in 0.0001 */
                                          /** C2: crystal inflection temperature usually near 25, in 0.0001 C */

   tSIRF_UINT16   tempUnc;                /** temperature uncertainty, copied to xoDefaults
                                            * LSB 0.01, ppm  */

   tSIRF_UINT8    polySource;             /** Source of the initial Polynomial values */
   tSIRF_INT8     spare1;
   tSIRF_UINT32   spare2;

} tSIRF_MSG_SSB_XO_POLY_OUT;

/*******************************************************************************
 * For XO_ENABLED
 #define SIRF_MSG_SSB_XO_TABLE_OUT           SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x10)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_XO_TABLE_OUT_tag
{
   tSIRF_UINT8    calT;                   /** T for XO update */

   tSIRF_UINT8    xoActive;               /** Active susbsystem, T = XO, F= TCXO. */
   tSIRF_INT8     xoActiveSource;         /** Source of XoActive */

   tSIRF_UINT8    updMode;                /** Update Mode
                                           * 0 = no updates
                                           * 1 = update C0 term when 1 bin filled
                                           * 2 = update C0, C1 term when 2-4 bins filled
                                           * 3 = update all C terms when 5 or more bins filled */

   tSIRF_INT16    fMean[SSB_XO_MODEL_SIZE];     /** Average frequency for the bin, in ppb  */
   tSIRF_INT16    tMean[SSB_XO_MODEL_SIZE];     /** Average Temperature for the bin, 1/256 T  */

   tSIRF_UINT32   spare;

} tSIRF_MSG_SSB_XO_TABLE_OUT;

/*******************************************************************************
 * For XO_ENABLED
 #define SIRF_MSG_SSB_XO_DENSITY_OUT         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x5D, 0x11)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_XO_DENSITY_OUT_tag
{
   tSIRF_UINT8    bin[SSB_XO_MODEL_SIZE]; /** update count, each bin = 8T, range 0 - 255 */
} tSIRF_MSG_SSB_XO_DENSITY_OUT;

#ifdef PVT_BUILD

/*******************************************************************************
 #define SIRF_MSG_SSB_SWTB_PMPROMPT_OUT SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x90)
 *******************************************************************************/
/* PM Prompt notifies SSB app of chip id, silicon, ROM, and patch versions */
typedef struct tSIRF_MSG_SSB_PM_PROMPT_OUT_tag
{
   tSIRF_UINT16 ChipId;
   tSIRF_UINT16 SiliconVersion;
   tSIRF_UINT16 ROMVersionCode;
   tSIRF_UINT16 PatchVersionCode;
} tSIRF_MSG_SSB_PM_PROMPT_OUT;

/*******************************************************************************
 #define SIRF_MSG_SSB_SWTB_PMACK_OUT SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xB2, 0x91)
 *******************************************************************************/
/* PM Acknowledgement has msg seq no, msg id of orig message, and result */
typedef struct tSIRF_MSG_SSB_PM_ACK_OUT_tag
{
   tSIRF_UINT16 MessageSeqNo;        /* Message Seq No Acknowledged */
   tSIRF_UINT8  MessageIdAck;        /* Id of original message */
   tSIRF_UINT8  AckStatus;           /* Result code */
} tSIRF_MSG_SSB_PM_ACK_OUT;

#endif /* PVT_BUILD */

/*******************************************************************************
    SSB Extended Input Message Structures
 *******************************************************************************/


/*******************************************************************************
    SIRF_MSG_SSB_POS_REQ                  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD2, 0)
 *******************************************************************************/



typedef struct tSIRF_MSG_SSB_POS_REQ_tag
{
   tSIRF_UINT8 pos_req_id;      /**< Position request identifier 0-255*/
   tSIRF_UINT8 num_fixes;       /**< Number of requested MS position (fixes) */
   tSIRF_UINT8 time_btw_fixes;  /**< Time elapsed between fixes */
   tSIRF_UINT8 hori_error_max;  /**< Maximum requested horizontal error */
   tSIRF_UINT8 vert_error_max;  /**< Maximum requested vertical error */
   tSIRF_UINT8 resp_time_max;   /**< Maximum response time */
   tSIRF_UINT8 time_acc_priority; /**< Time/accuracy priority */
   tSIRF_UINT8 location_method;   /**< GPS Location Method, as below
                                        xxxx 0000   MS Assisted
                                        xxxx 0001   MS Based
                                        xxxx 0010   MS Based is preferred, but MS Assisted is allowed
                                        xxxx 0011   MS Assisted is preferred, but MS Based is allowed
                                        xxxx 0100   Simultaneous MS Based and MS Assisted
                                        xx00 xxxx   No change to default ABP processing setting
                                        xx01 xxxx   Enable ABP processing with override of default setting
                                        xx10 xxxx   Disable ABP processing with override of default setting
                                        00xx xxxx   No change to default Reverse EE processing setting
                                        01xx xxxx   Enable Reverse EE processing with override of default setting
                                        10xx xxxx   Disable Reverse EE processing with override of default setting
                                        */
} tSIRF_MSG_SSB_POS_REQ;


#define SIRF_MSG_SSB_REQ_VERT_ERROR_1M      (0x00)
#define SIRF_MSG_SSB_REQ_VERT_ERROR_5M      (0x01)
#define SIRF_MSG_SSB_REQ_VERT_ERROR_10M     (0x02)
#define SIRF_MSG_SSB_REQ_VERT_ERROR_20M     (0x03)
#define SIRF_MSG_SSB_REQ_VERT_ERROR_40M     (0x04)
#define SIRF_MSG_SSB_REQ_VERT_ERROR_80M     (0x05)
#define SIRF_MSG_SSB_REQ_VERT_ERROR_160M    (0x06)
#define SIRF_MSG_SSB_REQ_VERT_ERROR_NO_MAX  (0x07)

#define SIRF_MSG_SSB_REQ_HORIZ_ERROR_NO_MAX (0x00)

#define SIRF_MSG_SSB_REQ_NO_PRIORITY        (0x00)
#define SIRF_MSG_SSB_REQ_TIME_PRIORITY      (0x01)
#define SIRF_MSG_SSB_REQ_ACC_PRIORITY       (0x02)
#define SIRF_MSG_SSB_USE_ENTIRE_RESP_TIME   (0x03)

#define SIRF_MSG_SSB_REQ_LOCMETH_MSA               (0x00)  /* MS Assisted */
#define SIRF_MSG_SSB_REQ_LOCMETH_MSB               (0x01)   /* MS Based */
#define SIRF_MSG_SSB_REQ_LOCMETH_MSB_PREFERRED     (0x02)  /* MS Based is preferred, but MS Assisted is allowed */
#define SIRF_MSG_SSB_REQ_LOCMETH_MSA_PREFERRED     (0x03)  /* MS Assisted is preferred, but MS Based is allowed */
#define SIRF_MSG_SSB_REQ_LOCMETH_SIMUL_MSB_MSA     (0x04)  /* Simultaneous MS Based and MS Assisted */


/*******************************************************************************
    SIRF_MSG_SSB_SET_EPH_CLK          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD3, 0x2)
 *******************************************************************************/


#define SIRF_MSG_SSB_EP_URA_INDEX_0002_40 (0x00)
#define SIRF_MSG_SSB_EP_URA_INDEX_0003_40 (0x01)
#define SIRF_MSG_SSB_EP_URA_INDEX_0004_85 (0x02)
#define SIRF_MSG_SSB_EP_URA_INDEX_0006_85 (0x03)
#define SIRF_MSG_SSB_EP_URA_INDEX_0009_65 (0x04)
#define SIRF_MSG_SSB_EP_URA_INDEX_0013_65 (0x05)
#define SIRF_MSG_SSB_EP_URA_INDEX_0024_00 (0x06)
#define SIRF_MSG_SSB_EP_URA_INDEX_0048_00 (0x07)
#define SIRF_MSG_SSB_EP_URA_INDEX_0096_00 (0x08)
#define SIRF_MSG_SSB_EP_URA_INDEX_0192_00 (0x09)
#define SIRF_MSG_SSB_EP_URA_INDEX_0384_00 (0x0a)
#define SIRF_MSG_SSB_EP_URA_INDEX_0768_00 (0x0b)
#define SIRF_MSG_SSB_EP_URA_INDEX_1536_00 (0x0c)
#define SIRF_MSG_SSB_EP_URA_INDEX_3072_00 (0x0d)
#define SIRF_MSG_SSB_EP_URA_INDEX_6144_00 (0x0e)
#define SIRF_MSG_SSB_EP_URA_INDEX_NONE    (0x0f)




typedef struct tSIRF_MSG_SSB_EPHEMERIS_INFO_PER_SV_tag
{
   tSIRF_UINT8  eph_flag; /**< Ephemeris parameter validity flag */
   tSIRF_MSG_SSB_EPHEMERIS_PARAMETERS eph_data;
}tSIRF_MSG_SSB_EPHEMERIS_INFO_PER_SV;


typedef struct tSIRF_MSG_SSB_SET_EPH_CLK_tag
{
   tSIRF_UINT8 num_sv;  /**< Number of satellites */
   tSIRF_MSG_SSB_EPHEMERIS_INFO_PER_SV sv_eph_prm[SIRF_MAX_SVID_CNT];
} tSIRF_MSG_SSB_SET_EPH_CLK;

/*******************************************************************************
    SIRF_MSG_SSB_SET_ALM              SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD3, 0x3)
 *******************************************************************************/


typedef struct tSIRF_MSG_SSB_ALMANAC_PARAMETERS_tag
{
   tSIRF_UINT8  alm_valid_flag;   /**< Almanac validity flag */
   tSIRF_UINT8  alm_sv_prn_num;   /**< The satellite PRN number */
   tSIRF_UINT16 alm_eccentricity; /**< eccentricity. Scale: 2 pow(-21), dimensionless */
   tSIRF_UINT8  alm_toa;          /**< The reference time of the almanac.Scale: 2 pow(12),seconds */
   tSIRF_INT16 alm_delta_incl;    /**< Delta inclination Scale: 2 pow(-19) semi-circles*/
   tSIRF_INT16 alm_omegadot;      /**< Rate of right ascension  Scale: 2 pow(-38) semi-circles/sec*/
   tSIRF_UINT32 alm_a_sqrt;       /**< Square root of the semi-major axis Scale: 2 pow(-11) sqrt(meter)*/
   tSIRF_INT32 alm_omega_0;       /**< Longitude of ascending node of orbit plane at weekly epoch Scale: 2 pow(-23) semi-circles*/
   tSIRF_INT32 alm_omega;         /**< Argument of perigee Scale: 2 pow(-23)semi-circles */
   tSIRF_INT32 alm_m0;            /**< Mean anomaly at reference time Scale: 2 pow(-23) semi-circles*/
   tSIRF_INT16 alm_af0;           /**< Apparent satellite clock correction af0 Scale: 2 pow(-20) seconds*/
   tSIRF_INT16 alm_af1;           /**< Apparent satellite clock correction af1 Scale: 2 pow(-38) sec/sec*/
} tSIRF_MSG_SSB_ALMANAC_PARAMETERS;


typedef struct tSIRF_MSG_SSB_SET_ALMANAC_ASSIST_DATA_tag
{
   tSIRF_UINT16 alm_week_num; /**< The GPS week number of the almanac */
   tSIRF_UINT8  num_sv;       /**< Number of satellites */
   tSIRF_MSG_SSB_ALMANAC_PARAMETERS alm_params[SIRF_MAX_SVID_CNT];
} tSIRF_MSG_SSB_SET_ALMANAC_ASSIST_DATA;

/*******************************************************************************
    SIRF_MSG_SSB_SET_ACQ_ASSIST       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD3, 0x4)
 *******************************************************************************/

#define SIRF_MSG_SSB_AAD_DU_200HZ    (0x00)
#define SIRF_MSG_SSB_AAD_DU_100HZ    (0x01)
#define SIRF_MSG_SSB_AAD_DU_50HZ     (0x02)
#define SIRF_MSG_SSB_AAD_DU_25HZ     (0x03)
#define SIRF_MSG_SSB_AAD_DU_12_5HZ   (0x04)
#define SIRF_MSG_SSB_AAD_DU_UNKNOWN  (0xFF)


typedef struct tSIRF_MSG_SSB_ACQUISITION_ASSISTANCE_DATA_tag
{
   tSIRF_UINT8  acq_assist_valid_flag;  /**< Acquisition Assistance Data Validity Flag */

   tSIRF_UINT8  sv_prn_number;          /**< Satellite PRN Number */

   tSIRF_INT16  doppler0;               /**< The 0th Order Doppler Units Hz */

   tSIRF_INT8   doppler1;               /**< The 1st Order Doppler Units Hz/sec */

   tSIRF_UINT8  doppler_uncertainty;    /**< The Doppler Uncertainty */

   tSIRF_UINT16 sv_code_phase;          /**< code phase range is from 0 to 1022 Chips */

   tSIRF_UINT8  sv_code_phase_int;      /**< C/A Code Periods that have Elapsed Since The Latest GPS Bit Boundary Units Milliseconds */

   tSIRF_UINT8  gps_bit_num;            /**< Two Least Significant Bits of The Bit Number */

   tSIRF_UINT16 code_phase_uncertainty; /**< Code Phase Uncertainty Units Chips */

   tSIRF_UINT16 azimuth;                /**< Code Phase Uncertainty Units Degrees */

   tSIRF_INT8   elevation;              /**< Elevation Angle of the GPS Satellite Units Degrees */

} tSIRF_MSG_SSB_ACQUISITION_ASSISTANCE_DATA;


typedef struct tSIRF_MSG_SSB_SET_ACQ_ASSIST_DATA_tag
{
   tSIRF_UINT32 reference_time;         /**< GPS Time Reference for Acquisition Assistance Data */

   tSIRF_UINT8   num_sv;                /**< Number of satellites */

   tSIRF_MSG_SSB_ACQUISITION_ASSISTANCE_DATA acq_data[SIRF_MAX_SVID_CNT];
} tSIRF_MSG_SSB_SET_ACQ_ASSIST_DATA;


/*******************************************************************************
   SIRF_MSG_SSB_SET_RT_INTEG         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD3, 0x5)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_SET_RT_INTEGRITY_tag
{
   tSIRF_UINT32 unhealthy_sat_info;     /**< Information on unhealthy satellite */
} tSIRF_MSG_SSB_SET_RT_INTEGRITY;


/*******************************************************************************
    SIRF_MSG_SSB_EPH_REQ              SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD4, 0x1)
 *******************************************************************************/

typedef struct tSIRF_MSG_SSB_SLC_EPH_STATUS_REQ_tag
{
   tSIRF_UINT8 reserved;
} tSIRF_MSG_SSB_SLC_EPH_STATUS_REQ;

/*******************************************************************************
    SIRF_MSG_SSB_ALM_REQ              SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD4, 0x2)
 *******************************************************************************/

typedef struct tSIRF_MSG_SSB_ALMANAC_REQ_tag
{
   tSIRF_UINT8 reserved;
} tSIRF_MSG_SSB_ALMANAC_REQ;


/*******************************************************************************
    SIRF_MSG_SSB_SET_UTC_MODEL        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD3, 0x6)
 *******************************************************************************/

/*starting at R_A0 */
typedef struct tSIRF_MSG_SSB_SET_UTC_MODEL_tag
{
   tSIRF_UINT32        a0;       /**< Constant term of polynomial (raw) Units: seconds */

   tSIRF_UINT32        a1;       /**< The first order term of polynomial (raw) Units: sec/sec */

   tSIRF_UINT8         delta_tls;/**< Delta time due to leap seconds (raw)Units: seconds */

   tSIRF_UINT8         tot;      /**< Reference time for UTC Data (raw) Units: seconds */

   tSIRF_UINT8         wnt;      /**< UTC reference week number (raw)Units: weeks */

   tSIRF_UINT8         wnlsf;    /**< Week number at which leap second becomes effective (raw) Units: weeks */

   tSIRF_UINT8         dn;       /**< Day number at the end of which leap second becomes effective (raw) Units: days */

   tSIRF_UINT8         delta_t_lsf;/**< Delta time due to the scheduled future or recent past leap second (raw) Units: seconds */

} tSIRF_MSG_SSB_SET_UTC_MODEL;


/*******************************************************************************
    SIRF_MSG_SSB_SET_GPS_TOW_ASSIST   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD3, 0x7)
 *******************************************************************************/

/*Starting at TOW_ASSIST_SV_PRN_NNM */
typedef struct tSIRF_MSG_SSB_AUXILIARY_NAV_TOW_ASSIST_tag
{
   tSIRF_UINT8          svid;           /**< Satellite PRN Number */

   tSIRF_UINT16         tlm_msg;        /**< Telemetry work */

   tSIRF_UINT8          tow_assist_info;/**< Additional TOW Assist Information */

} tSIRF_MSG_SSB_AUXILIARY_NAV_TOW_ASSIST;

typedef struct tSIRF_MSG_SSB_SET_GPS_TOW_ASSIST_tag
{
tSIRF_UINT8 num_sv; /**< Number of satellites */
tSIRF_MSG_SSB_AUXILIARY_NAV_TOW_ASSIST set_gps_tow_assist[SIRF_MAX_SVID_CNT];
} tSIRF_MSG_SSB_SET_GPS_TOW_ASSIST;


/*******************************************************************************
    SIRF_MSG_SSB_SET_AUX_NAV          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD3, 0x8)
 *******************************************************************************/

/* Starting at NAVMODEL_SV_PRN */
typedef struct tSIRF_MSG_SSB_AUXILIARY_NAV_PARAMS_tag
{
   tSIRF_UINT8          svid;     /**< Satellite ID number for the NAVMODEL */

   tSIRF_UINT16         toe;      /**< Time of Ephemeris of the NAVMODEL */

   tSIRF_UINT16         iodc;     /**< Issue of Data, Clock of the NAVMODEL */

   tSIRF_UINT8          sf1_l2_info;
                                  /**<  subframe 1 C/A or L2 P info */

   tSIRF_UINT8          sf1_sv_health;
                                  /**< Bits 5 to 0 (LSB) correspond to the 6 bit "SV Health" found in subframe 1 */

   tSIRF_UINT8          sf1_reserved[SIRF_MSG_SSB_ANP_NUM_SF1_RESERVED];
                                  /**< subframe 1 bits */

   tSIRF_UINT8          sf1_aodo_fit_interval;
                                  /**< Bit 5 to 1 bit "Fit Interval Flag" found subframe 2
                                     Bits 4 to 0 (LSB) correspond to the 5 bit "AODO" found subframe 2.*/

} tSIRF_MSG_SSB_AUXILIARY_NAV_PARAMS;


typedef struct tSIRF_MSG_SSB_SET_AUXILIARY_NAV_PARAMS_tag
{
   tSIRF_UINT8 num_sv;  /**< Number of satellites */

   tSIRF_MSG_SSB_AUXILIARY_NAV_PARAMS set_aux_nav_model_params[SIRF_MAX_SVID_CNT];

} tSIRF_MSG_SSB_SET_AUXILIARY_NAV_PARAMS;

/*******************************************************************************
    SIRF_MSG_SSB_B_EPH_REQ            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD4, 0x3)
 *******************************************************************************/


/* Broadcast Ephemeris Request Message, starting at EPH_INFO_FLAG */
typedef struct tSIRF_MSG_SSB_BE_REQ_PARAMS_tag{
   tSIRF_UINT8  eph_info_flag;    /**< Broadcast Ephemeris Information Validity Flag */

   tSIRF_UINT8  sv_prn_num;       /**< satellite id */

   tSIRF_UINT16 gps_week;         /**< Broadcast Ephemeris Reference Week */

   tSIRF_UINT16 toe;              /**< Broadcast Ephemeris Reference Time Scale 16 Units:Seconds */

} tSIRF_MSG_SSB_BE_REQ_PARAMS;

/* Broadcast Ephemeris Request Message Format  */
typedef struct tSIRF_MSG_SSB_BE_REQ_tag
{
   tSIRF_UINT16 eph_resp_trigger; /**< Broadcast Ephemeris Response Message Trigger */

   tSIRF_UINT8   num_sv;          /**< Number of satellites */

   tSIRF_MSG_SSB_BE_REQ_PARAMS eph_params[SIRF_MAX_SVID_CNT];

} tSIRF_MSG_SSB_BE_REQ;

/*******************************************************************************
    SIRF_MSG_SSB_SESSION_OPEN_REQ     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD5, 0x1)
 *******************************************************************************/


typedef struct tSIRF_MSG_SSB_SESSION_OPEN_REQ_tag
{
   tSIRF_UINT8 ses_open_req;      /**< Session open request information */
}  tSIRF_MSG_SSB_SESSION_OPEN_REQ;


#define SIRF_MSG_SOR_OPEN                        ( 0x71 )
#define SIRF_MSG_SOR_RESUME                     ( 0x80 )

/*******************************************************************************
    SIRF_MSG_SSB_CLIENT_STATUS_REQ    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD4, 0x6)
 *******************************************************************************/


typedef struct tSIRF_MSG_SSB_SLC_STATUS_REQ_tag
{
   tSIRF_UINT8 reserved;
}  tSIRF_MSG_SSB_SLC_STATUS_REQ;

/*******************************************************************************
    7.5.1.1 Test Mode Config Request
 *******************************************************************************/


typedef struct tSIRF_MSG_SSB_TEST_MODE_CONFIG_REQ_tag
{
   tSIRF_UINT8 config_1;
   tSIRF_UINT8 config_reserved[3];
}  tSIRF_MSG_SSB_TEST_MODE_CONFIG_REQ;

#define SIRF_MSG_BCAST_EPH_ALLOWED       (0x1)
#define SIRF_MSG_BCAST_EPH_NOT_ALLOWED   (0x2)

/*******************************************************************************
    SIRF_MSG_SSB_TX_BLANKING_REQ      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD4, 0x9)
 *******************************************************************************/


typedef struct  tSIRF_MSG_SSB_TX_BLANK_REQ_tag
{
   tSIRF_UINT8 command;           /**< 0 start, 1 stop */
   tSIRF_UINT8 air_interface;     /**< 0 GSM air if, others invalid */
   tSIRF_UINT8 mode;              /**< 00 = 1 slot, 01 = 2 slot others reserved */
   tSIRF_UINT32 spare;
}  tSIRF_MSG_SSB_TX_BLANK_REQ;


#define SIRF_MSG_SSB_SET_BLANK_ENABLE                (0x00)
#define SIRF_MSG_SSB_SET_BLANK_DISABLE               (0x01)
#define SIRF_MSG_SSB_SET_BLANK_MODE_2_SLOT_BLANKING  (0x01)
#define SIRF_MSG_SSB_SET_BLANK_MODE_4_SLOT_BLANKING  (0x02)



/*******************************************************************************
     SIRF_MSG_SSB_CH_LOAD_REQ          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD4, 0x5)
 *******************************************************************************/


typedef struct  tSIRF_MSG_SSB_CHANNEL_LOAD_QUERY_tag
{
   tSIRF_UINT8 port;              /**< 0 = Port A, 1 = Port B*/
   tSIRF_UINT8 mode;              /**< Response Mode */
}  tSIRF_MSG_SSB_CHANNEL_LOAD_QUERY;


#define SIRF_MSG_SSB_CLQ_MODE_TURN_OFF_PERIODIC_MESSAGE                ( 0x00 )
#define SIRF_MSG_SSB_CLQ_MODE_TURN_ON_PERIODIC_MESSAGE                 ( 0x01 )
#define SIRF_MSG_SSB_CLQ_MODE_SEND_MESSAGE_JUST_ONCE                   ( 0x02 )

/*******************************************************************************
   SIRF_MSG_SSB_SERIAL_SETTING_REQ   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD4, 0x8)
 *******************************************************************************/

typedef struct tSIRF_MSG_SSB_SERIAL_PORT_SETTING_REQ_tag
{
   tSIRF_UINT32 baud;       /**< desired baud rate */
   tSIRF_UINT8 data_bits;   /**< data bits used per character */
   tSIRF_UINT8 stop_bits;   /**< Stop bit length */
   tSIRF_UINT8 parity;      /**< None = 0, Odd = 1, Even = 2 */
   tSIRF_UINT8 port;        /**< Serial Port A or B */
   tSIRF_UINT8 reserved;
}  tSIRF_MSG_SSB_SERIAL_PORT_SETTING_REQ;



/*******************************************************************************
    7.5.15.1 Set Low Power Mode
 *******************************************************************************/


typedef struct  tSIRF_MSG_SSB_SET_LPM_REQ_tag
{
   tSIRF_UINT8 lp_enable;          /**< 1=enable low power */
   tSIRF_UINT8 power_duty_cycle;   /**< 1-20 in units of 5% */
   tSIRF_UINT8 time_duty_priority; /**< 1=time>power, 2=else */
   tSIRF_UINT8 max_on_time_indicator;
   tSIRF_UINT8 spare1;
}  tSIRF_MSG_SSB_SET_LPM_REQ;


/*******************************************************************************
 SIRF_MSG_SSB_TIME_FREQ_APPROX_POS_REQ  SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD4, 0x4)
 *******************************************************************************/


typedef struct tSIRF_MSG_SSB_TIME_FREQ_APROX_POS_REQ_tag
{
  tSIRF_UINT8 req_mask; /**< Request mask */
} tSIRF_MSG_SSB_TIME_FREQ_APROX_POS_REQ;

#define SIRF_MSG_SSB_TFAP_REQ_GPS_TOW_REQUESTED     ( 0x01 )
#define SIRF_MSG_SSB_TFAP_REQ_TIME_ACCURACY         ( 0x02 )
#define SIRF_MSG_SSB_TFAP_REQ_FREQ_STATUS           ( 0x04 )
#define SIRF_MSG_SSB_TFAP_REQ_APPROXIMATE_POSITION  ( 0x08 )

/* Delete the following if unused macro */
#define SIRF_MSG_SSB_TFAP_TIME_ACC_SCALE_FINE       ( 0x01 )
#define SIRF_MSG_SSB_TFAP_TIME_ACC_SCALE_COARSE     ( 0x00 )
#define SIRF_MSG_SSB_TFAP_FREQ_ACC_SCALE            ( 0x00 )
#define SIRF_MSG_SSB_TFAP_MAX_TIME_MICROSEC_ACC     ( 7680 )

#define SIRF_MSG_SSB_TFAP_GPS_WEEK_VALID            ( 0x01 )
#define SIRF_MSG_SSB_TFAP_GPS_TOW_VALID             ( 0x02 )
#define SIRF_MSG_SSB_TFAP_GPS_TIME_ACC_VALID        ( 0x04 )
#define SIRF_MSG_SSB_TFAP_GPS_FREQ_ACC_VALID        ( 0x08 )
#define SIRF_MSG_SSB_TFAP_GPS_HOR_UNC               ( 0x10 )
#define SIRF_MSG_SSB_TFAP_GPS_VER_UNC               ( 0x20 )


/*******************************************************************************
    SIRF_MSG_SSB_APRX_MS_POSITION_RSP SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD7, 0x1)
 *******************************************************************************/


typedef struct  tSIRF_MSG_SSB_APRX_MS_POSITION_RSP_tag
{
   tSIRF_INT32 lat;             /**< appoximate ms lattitude */
   tSIRF_INT32 lon;             /**< appoximate ms longitude*/
   tSIRF_UINT16 alt;            /**< appoximate ms altitude */
   tSIRF_UINT8  est_hor_err;    /**< estimated horizontal error */
   tSIRF_UINT16 est_ver_err;    /**< estimated vertical error */
   tSIRF_UINT8  use_alt_aiding; /**< 0x01 ON means use altitude aiding */
}  tSIRF_MSG_SSB_APRX_MS_POSITION_RSP;


/*******************************************************************************
    SIRF_MSG_SSB_FREQ_TX_RSP          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD7, 0x3)
 *******************************************************************************/


typedef struct  tSIRF_MSG_SSB_FREQ_TRANSFER_RSP_tag
{
   tSIRF_INT16  freq_offset;                 /**< Hz */
   tSIRF_UINT8  accuracy;                    /**< +/- this * 1575.42 MHz */
   tSIRF_UINT32 time_tag;                    /**< milliseconds tow */
   tSIRF_UINT8  clock_ref;                   /**< SIRF_MSG_F_TRSP_RCI_<flag> */
   tSIRF_UINT8  nominal_freq_high;           /**< high frequency bits */
   tSIRF_UINT32 nominal_freq_low;            /**< low frequency bits */
}  tSIRF_MSG_SSB_FREQ_TRANSFER_RSP;

#define SIRF_MSG_SSB_FTRSP_RCI_RELATED_TO_SLC_CLOCK                ( 0x01 )
#define SIRF_MSG_SSB_FTRSP_RCI_REFERENCE_CLOCK_IS_OFF              ( 0x02 )
#define SIRF_MSG_SSB_FTRSP_RCI_PLEASE_TURN_OFF_REFERENCE_CLOCK     ( 0x04 )
#define SIRF_MSG_SSB_FTRSP_RCI_NOMINAL_FREQ_INCLUDED_HERE          ( 0x08 )
#define SIRF_MSG_SSB_FTRSP_RCI_USE_TRANSFER_DATA                   ( 0x80 )

/*******************************************************************************
     SIRF_MSG_SSB_TIME_TX_RSP          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD7, 0x2)
 *******************************************************************************/

typedef struct tSIRF_MSG_SSB_TIME_TRANSFER_RSP_tag
{
   tSIRF_UINT8  tt_type;           /**< Time Transfer Type 00=Coarse, FF=Precise */
   tSIRF_UINT16 week_num;          /**< GPS Week Number modulo 1024 */
   tSIRF_UINT8  time_high;         /**< Bits [39..32] */
   tSIRF_UINT32 time_low;          /**< Bits [31..0] microseconds 0-604800 */
   tSIRF_INT32 delta_utc;          /**< GPS Time to UTC Time Correction milliseconds -8388.608 < x < 8388.608 */
   tSIRF_UINT8  time_accuracy;     /**< Time Transfer Accuracy coarse=milliseconds, precise=microseconds*/
}  tSIRF_MSG_SSB_TIME_TRANSFER_RSP;

/*******************************************************************************
    SIRF_MSG_SSB_SET_AIDING_AVAIL     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD3, 0x9)
 *******************************************************************************/


typedef struct tSIRF_MSG_SSB_PUSH_AIDING_AVAILABILITY_REQ_tag
{
   tSIRF_UINT8  aiding_availability_mask; /**< Mask to indicate the type of aiding available */
   tSIRF_UINT8  forced_aiding_req_mask;   /**< Mask to indicate the type of aiding that the CP would like to force the SLC to re-request */
   tSIRF_UINT8  est_hor_err;              /**< horizontal error */
   tSIRF_UINT16 est_ver_err;              /**< vertical error */
   tSIRF_UINT8  rel_freq_acc;             /**< relative frequency accuracy */
   tSIRF_UINT8  time_accuracy_scale;      /**< time accuracy scale */
   tSIRF_UINT8  time_accuracy;            /**< time accuracy */
   tSIRF_UINT16 spare;                    /**< spare */
} tSIRF_MSG_SSB_PUSH_AIDING_AVAILABILITY_REQ;

/* aiding_availability_mask */
#define SIRF_MSG_SSB_PAA_AAM_HOR_VER_ER_VALID           ( 0x01 )
#define SIRF_MSG_SSB_PAA_AAM_REL_FREQ_ACC_VALID         ( 0x02 )
#define SIRF_MSG_SSB_PAA_AAM_TIME_ACCURACY_VALID        ( 0x04 )
/* forced_aiding_req_mask */
#define SIRF_MSG_SSB_PAA_FAR_POSITION_AIDING            ( 0x01 )
#define SIRF_MSG_SSB_PAA_FAR_FREQ_AIDING                ( 0x02 )
#define SIRF_MSG_SSB_PAA_FAR_TIME_AIDING                ( 0x04 )


/*******************************************************************************
    SIRF_MSG_SSB_HW_CONFIG_RSP            SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD6, 0)
 *******************************************************************************/

#define SIRF_MSG_SSB_HWC_PRECISE_TIME_AVAILABLE      ( 0x01 )
#define SIRF_MSG_SSB_HWC_PRECISE_TIME_DIRECTION_BOTH ( 0x02 )
#define SIRF_MSG_SSB_HWC_FREQ_TRANSFER               ( 0x04 )
#define SIRF_MSG_SSB_HWC_FREQ_TRANSFER_NO_COUNTER    ( 0x08 )
#define SIRF_MSG_SSB_HWC_RTC_AVAILABLE               ( 0x10 )
#define SIRF_MSG_SSB_HWC_RTC_FOR_GPS_INTERNAL        ( 0x20 )
#define SIRF_MSG_SSB_HWC_COARSE_TIME_AVAILABLE       ( 0x40 )
#define SIRF_MSG_SSB_HWC_REFERENCE_CLOCK_OFF         ( 0x80 )


typedef struct tSIRF_MSG_SSB_HW_CONFIG_RSP_tag
{
   tSIRF_UINT8  hw_config_msg;          /**< Hardware configuration information */
   tSIRF_UINT8  nominal_freq_high;      /**< Nominal CP Frequency  [39:32] */
   tSIRF_UINT32 nominal_freq_low;       /**< [31:00] */
   tSIRF_UINT8  nw_enhance_type;        /**< Network Enhancement Type */
}  tSIRF_MSG_SSB_HW_CONFIG_RSP;

/* Position request auxiliary flag validity */
/*
#define SIRF_MSG_SSB_REQ_POSREQ_AUXVALID_NW_ENHANCE           (0x01)
#define SIRF_MSG_SSB_REQ_POSREQ_AUXVALID_NW_ENHANCE_AND_COLOC (0x03)
*/


#define SIRF_MSG_SSB_REQ_NW_ET_NONE                      (0x00)
#if 0
#define SIRF_MSG_SSB_REQ_NW_ET_COURSE_POSITION_ENABLED   (0x02)
#endif
#define SIRF_MSG_SSB_REQ_NW_ET_AUX_NAVMODEL_SUPPORTED    (0x04)
#define SIRF_MSG_SSB_REQ_NW_ET_NAVBIT_123_SUPPORTED      (0x08)
#define SIRF_MSG_SSB_REQ_NW_ET_NAVBIT_45_SUPPORTED       (0x10)

/*******************************************************************************
    SIRF_MSG_SSB_SESSION_OPEN_REQ     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD5, 0x1)
 *******************************************************************************/


typedef struct tSIRF_MSG_SSB_SESSION_CLOSE_REQ_tag
{
   tSIRF_UINT8 ses_close_req; /**< Session closing request information */
}  tSIRF_MSG_SSB_SESSION_CLOSE_REQ;


#define SIRF_MSG_SSB_SESSION_CLOSING_REQ              ( 0x00 )
#define SIRF_MSG_SSB_SESSION_SUSPEND_REQ              ( 0x80 )

/*******************************************************************************
     SIRF_MSG_SSB_OSP_REV_REQ          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD4, 0x7)
 *******************************************************************************/

typedef struct tSIRF_MSG_SSB_OSP_REV_REQ_tag
{
   tSIRF_UINT8 reserved;
}  tSIRF_MSG_SSB_OSP_REV_REQ;

/*******************************************************************************
     SIRF_MSG_SSB_SET_NBA_SF1_2_3      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD7, 0x4)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_NAV_SF_123_tag
{
   tSIRF_UINT8 sf_123_flag; /**< Subframe 1, 2, and 3 Flag */
   tSIRF_UINT8 sv_id;       /**< Satellite PRN number */
   tSIRF_UINT8 sf123[SIRF_MSG_SSB_ANS123R_NUM_BYTES_PER_SF123]; /**< Subframe 1, 2 and 3 */
}tSIRF_MSG_SSB_NAV_SF_123;


typedef struct tSIRF_SSB_NAV_SF_123_RSP_tag
{
   tSIRF_UINT8  num_sv; /**< Number of satellites */
   tSIRF_MSG_SSB_NAV_SF_123  sf123[SIRF_MSG_SSB_ANS123R_NUM_SF123];
} tSIRF_MSG_SSB_NAV_SF_123_RSP;

/*******************************************************************************
    SIRF_MSG_SSB_SET_NBA_SF4_5        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xD7, 0x5)
 *******************************************************************************/

typedef struct
{
   tSIRF_UINT8 frame_num;/**< frame number for which the data in SUBF_4_5 is valid for */
   tSIRF_UINT8 SF45[SIRF_MSG_SSB_ANS45R_NUM_BYTES_PER_SF45]; /**< Subframe 4 and 5 , 600 bits */
}tSIRF_MSG_SSB_NAV_SF_45;


typedef struct tSIRF_MSG_SSB_NAV_SF_45_RSP_tag
{
   tSIRF_UINT32            sv_list; /**< Satellite List */
   tSIRF_MSG_SSB_NAV_SF_45 sf45[SIRF_MSG_SSB_ANS45R_NUM_SF45];
}tSIRF_MSG_SSB_NAV_SF_45_RSP;


/*******************************************************************************
     SIRF_MSG_SSB_PWR_MODE_FPM_REQ         SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDA, 0)
 *******************************************************************************/
#define SIRF_MSG_SSB_FULL_POWER_ONTIME   (1000)
#define SIRF_MSG_SSB_FULL_POWER_INTERVAL (1000)
typedef struct tSIRF_MSG_SSB_PWR_MODE_FPM_REQ_tag
{
   tSIRF_UINT8 reserved;
}tSIRF_MSG_SSB_PWR_MODE_FPM_REQ;

/*******************************************************************************
     SIRF_MSG_SSB_PWR_MODE_APM_REQ     SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDA, 0x1)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_PWR_MODE_APM_REQ_tag
{
   tSIRF_UINT8 num_fixes;       /**< Number of requested APM cycles Valid range is 0-255*/
   tSIRF_UINT8 tbf;             /**< Time between fixes, Valid range 10-255s */
   tSIRF_UINT8 duty_cycle;      /**< Duty cycle, ranges from 1-20 in 5% increments */
   tSIRF_UINT8 max_horz_error;  /**< Maximum requested horizontal error */
   tSIRF_UINT8 max_vert_error;  /**< Maximum requested vertical error */
   tSIRF_UINT8 priority;        /**< Specifies if time or power duty has priority */
   tSIRF_UINT32 max_off_time;   /**< Maximum time for sleep mode */
   tSIRF_UINT32 max_search_time;/**< Maximum satellite search time */
   tSIRF_UINT8 time_accuracy_priority;/**< Time vs accuracy priority setting */
   tSIRF_UINT8 reserved;
}tSIRF_MSG_SSB_PWR_MODE_APM_REQ;

/*******************************************************************************
     SIRF_MSG_SSB_PWR_MODE_MPM_REQ       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDA, 0x2)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_PWR_MODE_MPM_REQ_tag
{
   tSIRF_UINT8 microPowerRequestTimeOut;
   tSIRF_UINT8 microPowerRequestControl;
   tSIRF_UINT16 reserved;
}tSIRF_MSG_SSB_PWR_MODE_MPM_REQ;

/*******************************************************************************
     SIRF_MSG_SSB_PWR_MODE_TP_REQ       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDA, 0x3)
 *******************************************************************************/
#define SIRF_MSG_SSB_DUTY_CYCLE_100PERCENT     (1000) /* units: 0.1 precent */
#define SIRF_MSG_SSB_DEFAULT_MAX_OFF_TIME     (30000) /* ms, = 30s */
#define SIRF_MSG_SSB_DEFAULT_MAX_SEARCH_TIME (120000) /* ms, = 120s */
typedef struct tSIRF_MSG_SSB_PWR_MODE_TP_REQ_tag
{
   tSIRF_UINT16 dutyCycle;        /**< Percent time on */
   tSIRF_UINT32 on_time;          /**< Duration of each tracking period, Range is 200 - 900 msec */
   tSIRF_UINT32 MaxOffTime;       /**< Maximum time for sleep mode */
   tSIRF_UINT32 MaxSearchTime;    /**< Maximum satellite search time */
}tSIRF_MSG_SSB_PWR_MODE_TP_REQ;

/*******************************************************************************
     SIRF_MSG_SSB_PWR_MODE_PTF_REQ      SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDA, 0x4)
 *******************************************************************************/
#define SIRF_MSG_SSB_DEFAULT_PTF_PERIOD (1800) /**< seconds */
typedef struct tSIRF_MSG_SSB_PWR_MODE_PTF_REQ_tag
{
   tSIRF_UINT32 ptf_period;     /**< PTF Period */
   tSIRF_UINT32 MaxSearchTime;  /**< Maximum satellite search time */
   tSIRF_UINT32 MaxOffTime;     /**< Hibernate period if search time exceeded */
}tSIRF_MSG_SSB_PWR_MODE_PTF_REQ;


/* SIRF_AGPS */

/* CW INPUT MESSAGES */
typedef struct tSIRF_MSG_SSB_CW_CONFIG_tag
{
   tSIRF_UINT8 subId;
   tSIRF_UINT8 confMode;
}  tSIRF_MSG_SSB_CW_CONFIG;

/*******************************************************************************
    XO MESSAGES IN
 *******************************************************************************/
/*******************************************************************************
    #define SIRF_MSG_SSB_XO_OUT_CONTROL_IN    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDD, 0x00)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_XO_OUTPUT_CONTROL_IN_tag
{
   /** The following fields are Bit Masks for message 0x5D output enabling.
    *  The bit position corresponds to the sID for 0x5D
    *  where bit 0 = sID 0
    *  if the sID is not defined it is ignored.
    *  All output can be disabled by setting both lists to 0. */

   tSIRF_UINT16   oneTimeList;           /**< One Time request List. */
   tSIRF_UINT16   contList;              /**< Continuous request List. */
   tSIRF_UINT16   outputRequest;         /**< Requested control for output sID's */
   tSIRF_UINT16   spare;

} tSIRF_MSG_SSB_XO_OUTPUT_CONTROL_IN;

/*******************************************************************************
    #define SIRF_MSG_SSB_XO_DEFAULTS_IN       SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDD, 0x01)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_XO_DEFAULTS_IN_tag
{
   tSIRF_UINT8    source;                 /**< Bit Mask with the source of entries */

   tSIRF_UINT8    agingRateUnc;           /**< aging rate uncertainty,
                                           * LSB 0.1,  ppm/year  */

   tSIRF_UINT8    initialOffsetUnc;       /**< initial offset uncertainty,
                                           * LSB 0.1,  ppm  */
   tSIRF_UINT8    spare1;

   tSIRF_INT32   clockDrift;              /**< clock drift,
                                           * LSB 1,    ppb */

   tSIRF_UINT16   tempUnc;                /**< temperature uncertainty,
                                           * LSB 0.01, ppm  */

   tSIRF_UINT16   mfgWeek;                /**< manufacturing wn for aging,
                                           * LSB 1,    wn  */
   tSIRF_UINT32   spare2;
} tSIRF_MSG_SSB_XO_DEFAULTS_IN;

/*******************************************************************************
    #define SIRF_MSG_SSB_TCXO_TABLE_IN          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDD, 0x02)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_TCXO_TABLE_IN_tag
{
   tSIRF_INT32    spare1;
   tSIRF_INT16    offset;                    /**< frequency offset bias from CD default
                                              * LSB 1, ppb */
   tSIRF_INT16    globalMin;                 /**< minimum xo error observed
                                              * LSB 1, ppb */
   tSIRF_INT16    globalMax;                 /**< maximum xo error observed
                                              * LSB 1, ppb */
   tSIRF_UINT16   firstWeek;                 /**< full gps week of first table update.
                                              * LSB 1 */
   tSIRF_UINT16   lastWeek;                  /**< full gps week of last table update
                                              * LSB 1 */
   tSIRF_UINT16   lsb;                       /**< array LSB of Min[] and Max[]
                                              * LSB 1, ppb */
   tSIRF_UINT8    agingBin;                  /**< Bin of last aging update. */
   tSIRF_INT8     agingUpcount;              /**< Aging detection accumulator
                                              * LSB 1 */
   tSIRF_UINT8    binCnt;                    /**< count of min bins filled */
   tSIRF_UINT8    spare2;                    /**< not used */
   tSIRF_INT8     min[SSB_XOT_TABLE_SIZE];   /**< Min XO error at each temperature
                                              * LSB xoTable.lsb */
   tSIRF_INT8     max[SSB_XOT_TABLE_SIZE];   /**< Max XO error at each temperature
                                              * LSB xoTable.lsb */
} tSIRF_MSG_SSB_TCXO_TABLE_IN;

/*******************************************************************************
    #define SIRF_MSG_SSB_XO_TEST_CONTROL_IN   SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDD, 0x03)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_XO_TEST_CONTROL_IN_tag
{
   tSIRF_UINT8    mode;                  /**< Test Mode Control for MPM
                                          *  Bit 0:
                                          *    0 = uses host updates
                                          *    1 = ignores host updates
                                          *  Bit 1:
                                          *    0 = Trec updates temp table
                                          *    1 = Block updates to temp table
                                          * */
   tSIRF_UINT8    spare1;
   tSIRF_UINT16   spare2;

} tSIRF_MSG_SSB_XO_TEST_CONTROL_IN;

/*******************************************************************************
 * For XO_ENABLED
    #define SIRF_MSG_SSB_XO_POLY_IN          SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xDD, 0x10)
 *******************************************************************************/
typedef struct tSIRF_MSG_SSB_XO_POLY_IN_tag
{
   tSIRF_INT32 C[4];                      /** polynomial co-efficients */

   tSIRF_UINT16   tempUnc;                /** temperature uncertainty, copied to xoDefaults
                                            * LSB 0.01, ppm  */

   tSIRF_UINT8    polySource;             /** Source of the initial Polynomial values */
   tSIRF_INT8     spare1;
   tSIRF_UINT32   spare2;

} tSIRF_MSG_SSB_XO_POLY_IN;


/*******************************************************************************
 #define SIRF_MSG_SSB_SENSOR_CONFIG        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xEA, 0x01)
 *******************************************************************************/
/** Sensor Configuration Message **/
#define SIRF_MSG_SSB_MAX_SENSOR_INIT_REGS (12)
#define SIRF_MSG_SSB_MAX_SENSOR_READ_REGS (12)
#define SIRF_MSG_SSB_MAX_SENSOR_CTRL_REGS (32)
#define SIRF_MSG_SSB_MAX_NUM_SENSORS  (4)

typedef struct tSIRF_MSG_SSB_SENSOR_INIT_REG_tag  /** Initialization Data Read Register **/
{
   tSIRF_UINT8 address;  /** Register address **/
   tSIRF_UINT8 nBytes;   /** Number of bytes to be read from this register at initialization time **/
} tSIRF_MSG_SSB_SENSOR_INIT_REG;

typedef struct tSIRF_MSG_SSB_SENSOR_READ_REG_tag /** Sensor Read Register **/
{
   tSIRF_UINT8  readOprMethod;   /** Read operation method: 0- read only, others- write with repeated start read **/
   tSIRF_UINT8  dataReadReg;     /** Register address from where to read the sensor data **/
} tSIRF_MSG_SSB_SENSOR_READ_REG;


typedef struct tSIRF_MSG_SSB_SENSOR_CTRL_REG_tag /** Sensor Control Register **/
{
   tSIRF_UINT8 address;  /** Register address **/
   tSIRF_UINT8 value;    /** Data to write, 0xFF means address is to be used as write command only **/
} tSIRF_MSG_SSB_SENSOR_CTRL_REG;

typedef struct tSIRF_MSG_SSB_SENSOR_tag
{
   tSIRF_UINT16 i2cAddress;        /** Slave device address for Sensor **/
   tSIRF_UINT8  sensorType;        /** Sensor Type for sensor **/
   tSIRF_UINT8  initTime;          /** Initialization period after power up for Sensor, unit: 10ms **/
   tSIRF_UINT8  nBytesResol;       /** Data type (bit 0), Number of bytes to read from each register (bits 1-3) and Data Resolution (bits 4-7) **/
   tSIRF_UINT8  sampRate;          /** Sample rate: 1- 0.5hz, 2- 1hz, 3- 2hz, 4- 5hz, 5- 10hz, 6- 25hz, 7- 50hz **/
   tSIRF_UINT8  sendRate;          /** Send Rate: 1- 0.5hz, 2- 1hz, 3- 2hz, 4- 5hz, 5- 10hz, 6- 25hz, 7- 50hz **/
   tSIRF_UINT8  decmMethod;        /** Data decimation method: 0- raw, 1- averaging, 2- sliding median **/
   tSIRF_UINT8  acqTime;           /** Acquisition time delay, unit: 10us **/
   tSIRF_UINT8  numReadReg;        /** Number of sensor read registers **/
   tSIRF_UINT8  measState;         /** Measurement State: 0 - auto (sensor configured) 1 - forced (sw controlling) **/
   tSIRF_MSG_SSB_SENSOR_READ_REG sensorReadReg[SIRF_MSG_SSB_MAX_SENSOR_READ_REGS];

   tSIRF_UINT8  pwrCtrlReg;        /** Register to controls the sensor power states **/
   tSIRF_UINT8  pwrOffSetting;     /** Setting for Low Power (OFF) mode **/
   tSIRF_UINT8  pwrOnSetting;      /** Setting for Normal Power (ON) mode **/


   tSIRF_UINT8  numInitReadReg;    /** Number of Initialization Registers to be read **/
   tSIRF_MSG_SSB_SENSOR_INIT_REG sensorInitReg[SIRF_MSG_SSB_MAX_SENSOR_INIT_REGS];

   tSIRF_UINT8  numCtrlReg;        /** Number of sensor control registers **/
   tSIRF_UINT8  ctrlRegWriteDelay; /** Time delay between two consecutive register writes, unit: ms **/
   tSIRF_MSG_SSB_SENSOR_CTRL_REG sensorCtrlReg[SIRF_MSG_SSB_MAX_SENSOR_CTRL_REGS];

} tSIRF_MSG_SSB_SENSOR;

/* Definition for sensor types */
#define SENS_TYPE_ACCEL            1
#define SENS_TYPE_MAGN             2
#define SENS_TYPE_PRESS            3
#define SENS_TYPE_GYRO             4
#define SENS_TYPE_ACCEL_GYRO       5
#define SENS_TYPE_ACCEL_MAGN       6
#define SENS_TYPE_GYRO_MAGN        7
#define SENS_TYPE_ACCEL_MAGN_GYRO  8
#define MAX_SENS_TYPE              8

typedef struct tSIRF_MSG_SSB_SCALE_ZERO_POINT_tag
{
   tSIRF_UINT16 zeroPointVal; /** Zero Point Value for sensor **/
   tSIRF_UINT16 scaleFactor;  /** Scale Factor (sensitivity) for sensor **/
}tSIRF_MSG_SSB_SCALE_ZERO_POINT;

typedef struct tSIRF_MSG_SSB_SENSOR_CONFIG_tag
{
   tSIRF_UINT8 numSensors;      /** Number of sensors, range: 0 - 4 **/
   tSIRF_UINT8 i2cSpeed;        /** I2C bus speed setting: 0: Low, 1: Standard, 2: Fast, 3: Fast plus, 4: High **/
   tSIRF_MSG_SSB_SENSOR Sensors[SIRF_MSG_SSB_MAX_NUM_SENSORS]; /** Pointers to sensors **/
   tSIRF_UINT8 processingRate;     /** Sensor Data Processing Rate: set default to 1 Hz**/
   tSIRF_MSG_SSB_SCALE_ZERO_POINT sensorScaleZeroPointVal[SIRF_MSG_SSB_MAX_NUM_SENSORS];
} tSIRF_MSG_SSB_SENSOR_CONFIG;


/*******************************************************************************
 #define SIRF_MSG_SSB_SENSOR_SWITCH        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0xEA, 0x02)
 *******************************************************************************/
/* Note:If the following structure is modified then tLOCM_SensorSwitch must also be modified. */
typedef struct tSIRF_MSG_SSB_SENSOR_SWITCH_tag
{
   tSIRF_UINT8 sensorSetState;   /** Bit 0:  0 - turn sensor set OFF
                                             1 - turn sensor set ON 
                                     Bit 1:  0 - turn the receiver state change notifications OFF 
                                             1 - turn the receiver state change notifications ON
                                     Bits 2-7 - Reserved.
 **/
} tSIRF_MSG_SSB_SENSOR_SWITCH;

/* Constants for the bit-field sensorSetState */
#define SENS_STATE_ENABLE        0x01   /* Bit 0 - Sensors Enable/Disable */
#define SENS_STATE_NOTIFY        0x02   /* Bit 1 - Rcvr State Notification Enable/Disable */

/*******************************************************************************
#define SIRF_MSG_SSB_SENSOR_READINGS    SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x48, 0x1)
 *******************************************************************************/

/** Sensor Data Message **/
#define SIRF_MSG_SSB_MAX_SENSOR_DATA_LEN   (6)

/* Must reflect the maximum number of points used in the decimation methods */
#define SENS_MAX_DECIM_FACTOR_SIZE          30 /* 30 to 1 is the current max decimation ratio */

/* 100 Hz / 1 Hz  + 1 full decimation set*/
#define SIRF_MSG_SSB_MAX_SENSOR_DATA_SETS  (100 + SENS_MAX_DECIM_FACTOR_SIZE)

typedef struct tSIRF_MSG_SSB_SENSOR_DATA_tag
{
   tSIRF_UINT32 timeTag;  /** Time stamp for data set **/
   tSIRF_UINT8  data[SIRF_MSG_SSB_MAX_SENSOR_DATA_LEN]; /** Data **/
} tSIRF_MSG_SSB_SENSOR_DATA;


typedef struct tSIRF_MSG_SSB_SENSOR_READINGS_tag
{
   tSIRF_UINT16 sensorID;     /** Identification for sensor **/
   tSIRF_UINT8  dataLength;   /** Number of bytes on a single sensor data set, range:  1-6 **/
   tSIRF_UINT8  numDataSet;   /** Number of data sets in the message, range:  1-100 **/
   tSIRF_UINT8  dataMode;     /** Data mode: 0- raw, 1- averaging, 2- sliding median **/
   tSIRF_MSG_SSB_SENSOR_DATA dataSet[SIRF_MSG_SSB_MAX_SENSOR_DATA_SETS];
} tSIRF_MSG_SSB_SENSOR_READINGS;

/*******************************************************************************
 #define SIRF_MSG_SSB_SENSOR_FACTORY_STORED_PARAMS        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x48, 0x2)
 *******************************************************************************/
/** Sensor Initialization Registers Data Message **/
#define SIRF_MSG_SSB_MAX_SENSOR_INIT_REG_DATA_LEN   (20)

typedef struct tSIRF_MSG_SBB_SENSOR_INIT_REG_DATA_tag
{
   tSIRF_UINT8  nBytes;  /**  Number of bytes read from this register location, range:  1-20 **/
   tSIRF_UINT8  data[SIRF_MSG_SSB_MAX_SENSOR_INIT_REG_DATA_LEN]; /** Data **/
} tSIRF_MSG_SBB_SENSOR_INIT_REG_DATA;


typedef struct tSIRF_MSG_SSB_SENSOR_FACTORY_STORED_PARAMS_tag
{
   tSIRF_UINT16 sensorID;       /** Identification for sensor **/
   tSIRF_UINT8  numInitReadReg; /** Number of registers data sets:  1-6 **/
   tSIRF_MSG_SBB_SENSOR_INIT_REG_DATA initData[SIRF_MSG_SSB_MAX_SENSOR_INIT_REGS];
} tSIRF_MSG_SSB_SENSOR_FACTORY_STORED_PARAMS;


/*******************************************************************************
 #define SIRF_MSG_SSB_RCVR_STATE        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x48, 0x3)
 *******************************************************************************/

typedef struct tSIRF_MSG_SBB_RCVR_STATE_tag
{
   tSIRF_UINT32 timeStamp;  /**  This can be Acq Clk **/
   tSIRF_UINT8  rcvrPhysicalState;  /**  State of the receiver, range: 0-2 **/
} tSIRF_MSG_SBB_RCVR_STATE;

/* Definition for rcvrPhysicalState values */
#define SENS_RCVR_STATE_UNKNOWN          0
#define SENS_RCVR_STATE_STATIONARY       1
#define SENS_RCVR_STATE_MOVING           2

/*******************************************************************************
 #define SIRF_MSG_SSB_POINT_N_TELL_OUTPUT        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x48, 0x4)
 *******************************************************************************/

typedef struct tSIRF_MSG_SSB_POINT_N_TELL_OUTPUT_tag
{
   tSIRF_UINT32 timeStamp;  /**  This can be Acq Clk /Tow **/
   tSIRF_INT32 latitude;  /**  In degrees (+ = North) x 10 ^ 7  **/
   tSIRF_INT32 longitude;  /**  In degrees (+ = East) x 10 ^ 7.  **/
   tSIRF_UINT16 heading;  /**  In degrees x 10 ^ 2  **/
   tSIRF_INT16 pitch;  /**  In degrees x 10 ^ 2  **/
   tSIRF_INT16 roll;  /**  In degrees x 10 ^ 2  **/
   tSIRF_UINT16 headingUnc;  /**  In degrees x 10 ^ 2  **/
   tSIRF_UINT16 pitchUnc;  /**  In degrees x 10 ^ 2  **/
   tSIRF_UINT16 rollUnc;  /**  In degrees x 10 ^ 2  **/
   tSIRF_UINT8 status;  /**  Magnetic sensor calibration status  **/
} tSIRF_MSG_SSB_POINT_N_TELL_OUTPUT;

/* Definition for magnetic sensor status */
#define COMPASS_CALIB_UNKNOWN          0
#define COMPASS_CALIBRATED                  1
#define COMPASS_CALIB_REQUIRED          2
#define COMPASS_MAG_DISTURBED           3  /* Magnetic disturbance */

/* Definition for Accelerometer  status */
#define ACCEL_CALIB_UNKNOWN               0
#define ACCEL_CALIBRATED                       1
#define ACCEL_CALIB_REQUIRED               2


/*******************************************************************************
 #define SIRF_MSG_SSB_SENSOR_CALIBRATION_OUTPUT        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x48, 0x5)
 *******************************************************************************/

typedef struct tSIRF_MSG_SSB_SENSOR_CALIBRATION_OUTPUT_tag
{
   tSIRF_UINT16 msgDescriptor; /** Message Descriptor. This field describes the content 
                                                      of this message. The Lower 8 bits are used as follows:
                                                      Bit0 – ‘0’ indicates that Calibration values are picked 
                                                      from NVM or are init time values. ‘1’ indicates that the 
                                                      calibration values are as a result of calibration performed 
                                                      using run time data.
                                                      Bit1 – ‘0’ indicates that this message is meant to output 
                                                      only offset and scale values. ‘1’ indicates that this message 
                                                      contains the all calibration parameter for this sensor.

                                                      The Upper 8bits contain number of calibration fields which 
                                                      are to be used in this message. **/

   tSIRF_UINT8 sensorType;  /**  Sensor ID  Identifies the sensor.1 - Accelerometer,2 - Magnetic sensor,
                                              , 3 - Gyroscope 4 - Pressure sensor**/
   tSIRF_UINT32 timeStamp;  /**  This can be Acq Clk **/
   tSIRF_DOUBLE CAL_FLD8[6];  /** Array for Offset value (Double). Default value is 0. **/
   tSIRF_FLOAT CAL_FLD4[24];  /** Element of P error covariance matrix (Float). **/
} tSIRF_MSG_SSB_SENSOR_CALIBRATION_OUTPUT;

/*******************************************************************************
 #define SIRF_MSG_SSB_MAG_MODEL_PARAMS        SIRF_MAKE_MSG_ID(SIRF_LC_SSB, 0x48, 0x6)
 *******************************************************************************/

typedef struct tSIRF_MSG_SSB_MAG_MODEL_PARAMS_tag
{
   tSIRF_UINT8 magModelParamSrc;  /**  This field describes whether the input parameters to the IGRF 
   model are default, from NVM or as a result of GPS fix.Default - 0.NVM -1.GPS Fix -2. **/
   tSIRF_INT32 latitude;  /**  In degrees (+ = North) x 10 ^ 7  **/
   tSIRF_INT32 longitude;  /**  In degrees (+ = East) x 10 ^ 7.  **/
   tSIRF_INT32 altitude;  /**  In meters x 10 ^ 2.  **/
   tSIRF_UINT16 year; /**< Year part of the date for when to compute the values. */
   tSIRF_UINT8 month; /**< Month part of the date for when to compute the values. */
   tSIRF_UINT8 day; /**< Day part of the date for when to compute the values. */
   tSIRF_FLOAT declination; /**< d:  Magnetic Declination. (Decimal Degrees) */
   tSIRF_FLOAT totalField; /**< f: Total Magnetic Field Strength.     (nT) */
   tSIRF_FLOAT horizonField; /**< h:  Magnetic Field HorizontalStrength. (nT) */
   tSIRF_FLOAT inclination; /**< i:  Magnetic Inclination. (Decimal Degrees)  */
   tSIRF_FLOAT northComp; /**< x:  Magnetic Field, North Component.   (nT) */
   tSIRF_FLOAT eastComp; /**< y:  Magnetic Field, East Component.    (nT) */
   tSIRF_FLOAT downComp; /**< z:  Magnetic Field, Down Component.    (nT)*/
} tSIRF_MSG_SSB_MAG_MODEL_PARAMS;

/* Definition for Magnetic model params     */
#define MAG_MODEL_PARAMS_DEFAULT                    0
#define MAG_MODEL_PARAMS_FROM_NVM                 1
#define MAG_MODEL_PARAMS_FROM_GPS                 2

/* Definition used for calibration messages */
#define SENS_SSB_CAL_INIT_OR_NVM_PARAMS        0
#define SENS_SSB_CAL_RUNTIME_PARAMS                1

#define SENS_SSB_CAL_OFFSETSCALE_PARAMS        0
#define SENS_SSB_CAL_PROCESS_PARAMS                2

#define SENS_SSB_CAL_OFFSETSCALE_COUNT          6
#define SENS_SSB_CAL_PROCESS_ACCEL_COUNT      30
#define SENS_SSB_CAL_PROCESS_MAG_COUNT         4

#ifdef NOT_WORKING_FOR_NOW
typedef union tSIRF_MSG_SSB_tag
{
   tSIRF_MSG_SSB_MEASURED_NAVIGATION sirf_msg_ssb_measured_navigation;
   tSIRF_MSG_SSB_MEASURED_TRACKER sirf_msg_ssb_measured_tracker;
   tSIRF_MSG_SSB_RAW_TRACKER sirf_msg_ssb_raw_tracker;
   tSIRF_MSG_SSB_50BPS_DATA sirf_msg_ssb_50bps_data;
   tSIRF_MSG_SSB_THROUGHPUT sirf_msg_ssb_throughput;
   tSIRF_MSG_SSB_ERROR sirf_msg_ssb_error;
   tSIRF_MSG_SSB_ACK sirf_msg_ssb_ack;
   tSIRF_MSG_SSB_NAK sirf_msg_ssb_nak;
   tSIRF_MSG_SSB_VISIBILITY_LIST sirf_msg_ssb_visibility_list;
   tSIRF_MSG_SSB_ALMANAC sirf_msg_ssb_almanac;
   tSIRF_MSG_SSB_EPHEMERIS sirf_msg_ssb_ephemeris;
   tSIRF_MSG_SSB_OK_TO_SEND sirf_msg_ssb_ok_to_send;
   tSIRF_MSG_SSB_RECEIVER_PARAMS sirf_msg_ssb_receiver_params;
   tSIRF_MSG_SSB_TEST_MODE_DATA sirf_msg_ssb_test_mode_data;
   tSIRF_MSG_SSB_DGPS_STATUS sirf_msg_ssb_dgps_status;
   tSIRF_MSG_SSB_NL_MEAS_DATA sirf_msg_ssb_nl_meas_data;
   tSIRF_MSG_SSB_NL_DGPS_DATA sirf_msg_ssb_nl_dgps_data;
   tSIRF_MSG_SSB_NL_SV_STATE_DATA sirf_msg_ssb_nl_sv_state_data;
   tSIRF_MSG_SSB_NL_INIT_DATA sirf_msg_ssb_nl_init_data;
   tSIRF_MSG_SSB_NL_AUX_INIT_DATA sirf_msg_ssb_nl_aux_init_data;
   tSIRF_MSG_SSB_NL_AUX_MEAS_DATA sirf_msg_ssb_nl_aux_meas_data;
   tSIRF_MSG_SSB_NL_AUX_AID_DATA sirf_msg_ssb_nl_aux_aid_data;
   tSIRF_MSG_SSB_GEODETIC_NAVIGATION sirf_msg_ssb_geodetic_navigation;
   tSIRF_MSG_SSB_ADC_ODOMETER_DATA sirf_msg_ssb_adc_odometer_data;
   tSIRF_MSG_SSB_ADC_ODOMETER_DATA_1HZ sirf_msg_ssb_adc_odometer_data_1hz;
   tSIRF_MSG_SSB_TEST_MODE_DATA_3 sirf_msg_ssb_test_mode_data_3;
   tSIRF_MSG_SSB_DR_NAV_STATUS sirf_msg_ssb_dr_nav_status;
   tSIRF_MSG_SSB_DR_NAV_STATE sirf_msg_ssb_dr_nav_state;
   tSIRF_MSG_SSB_DR_NAV_SUBSYS sirf_msg_ssb_dr_nav_subsys;
   tSIRF_MSG_SSB_DR_VALID sirf_msg_ssb_dr_valid;
   tSIRF_MSG_SSB_DR_GYR_FACT_CAL sirf_msg_ssb_dr_gyr_fact_cal;
   tSIRF_MSG_SSB_DR_SENS_PARAM sirf_msg_ssb_dr_sens_param;
   tSIRF_MSG_SSB_DR_DATA_BLK sirf_msg_ssb_dr_data_blk;
   tSIRF_MSG_SSB_MMF_STATUS sirf_msg_ssb_mmf_status;
   tSIRF_MSG_SSB_CW_DATA sirf_msg_ssb_cw_data;
   tSIRF_MSG_SSB_CW_FILTER sirf_msg_ssb_cw_filter;
   tSIRF_MSG_SSB_SBAS_PARAM sirf_msg_ssb_sbas_param;
   tSIRF_MSG_SSB_SIRFNAV_COMPLETE sirf_msg_ssb_sirfnav_complete;
   tSIRF_MSG_SSB_SIRFNAV_TIME_TAGS sirf_msg_ssb_sirfnav_time_tags;
   tSIRF_MSG_SSB_TRACKER_LOADER_STATE sirf_msg_ssb_tracker_loader_state;
   tSIRF_MSG_SSB_SIRFNAV_START sirf_msg_ssb_sirfnav_start;
   tSIRF_MSG_SSB_SIRFNAV_STOP sirf_msg_ssb_sirfnav_stop;
   tSIRF_MSG_SSB_STARTUP_INFO sirf_msg_ssb_startup_info;
   tSIRF_MSG_SSB_EE_GPS_TIME_INFO sirf_msg_ssb_ee_gps_time_info;
   tSIRF_MSG_SSB_EE_INTEGRITY sirf_msg_ssb_ee_integrity;
   tSIRF_MSG_SSB_EE_STATE sirf_msg_ssb_ee_state;
   tSIRF_MSG_SSB_EE_CLK_BIAS_ADJ sirf_msg_ssb_ee_clk_bias_adj;
   tSIRF_MSG_SSB_EE_STORE_NOW sirf_msg_ssb_ee_store_now;
   tSIRF_MSG_SSB_EE_FILE_PART_RESP sirf_msg_ssb_ee_file_part_resp;
   tSIRF_MSG_SSB_SIF_ACK_NACK sirf_msg_ssb_sif_ack_nack;
   tSIRF_MSG_SSB_SIF_SGEE_AGE sirf_msg_ssb_sif_sgee_age;
   tSIRF_MSG_SSB_SIF_PKT_INVOKE_DLD sirf_msg_ssb_sif_pkt_invoke_dld;
   tSIRF_MSG_SSB_SIF_PKT_ERASE sirf_msg_ssb_sif_pkt_erase;
   tSIRF_MSG_SSB_SIF_STR_PKT_DATA sirf_msg_ssb_sif_str_pkt_data;
   tSIRF_MSG_SSB_SIF_RCV_PKT_DATA sirf_msg_ssb_sif_rcv_pkt_data;
   tSIRF_MSG_SSB_SIF_NVM_HEADER_DATA sirf_msg_ssb_sif_nvm_header_data;
   tSIRF_MSG_SSB_SIF_AIDING_STATUS sirf_msg_ssb_sif_aiding_status;
   tSIRF_MSG_SSB_EE_ACK sirf_msg_ssb_ee_ack;
   tSIRF_MSG_SSB_TEST_MODE_DATA_7 sirf_msg_ssb_test_mode_data_7;
   tSIRF_MSG_SSB_TEST_MODE_DATA_8 sirf_msg_ssb_test_mode_data_8;
   tSIRF_MSG_SSB_TEST_MODE_DATA_9 sirf_msg_ssb_test_mode_data_9;
   tSIRF_MSG_SSB_TRACKER_DATA_GPIO_STATE sirf_msg_ssb_tracker_data_gpio_state;
   tSIRF_MSG_SSB_GPIO_READ sirf_msg_ssb_gpio_read;
   tSIRF_MSG_SSB_SIRF_STATS sirf_msg_ssb_sirf_stats;
   tSIRF_MSG_SSB_SIRF_STATS_AGPS sirf_msg_ssb_sirf_stats_agps;
   tSIRF_MSG_SSB_DOP_VALUES sirf_msg_ssb_dop_values;
   tSIRF_MSG_SSB_INITIALIZE sirf_msg_ssb_initialize;
   tSIRF_MSG_SSB_NMEA_CFG sirf_msg_ssb_nmea_cfg;
   tSIRF_MSG_SSB_SET_NMEA_MODE sirf_msg_ssb_set_nmea_mode;
   tSIRF_MSG_SSB_ALM_DATA sirf_msg_ssb_alm_data;
   tSIRF_MSG_SSB_SET_ALMANAC sirf_msg_ssb_set_almanac;
   tSIRF_MSG_SSB_POLL_SW_VERSION sirf_msg_ssb_poll_sw_version;
   tSIRF_MSG_SSB_SET_DGPS_SOURCE sirf_msg_ssb_set_dgps_source;
   tSIRF_MSG_SSB_SET_PORT_VALUES sirf_msg_ssb_set_port_values;
   tSIRF_MSG_SSB_SET_PORT_PROTOCOL sirf_msg_ssb_set_port_protocol;
   tSIRF_MSG_SSB_SET_NAV_MODE sirf_msg_ssb_set_nav_mode;
   tSIRF_MSG_SSB_SET_DOP_MODE sirf_msg_ssb_set_dop_mode;
   tSIRF_MSG_SSB_SET_DGPS_MODE sirf_msg_ssb_set_dgps_mode;
   tSIRF_MSG_SSB_SET_ELEV_MASK sirf_msg_ssb_set_elev_mask;
   tSIRF_MSG_SSB_SET_POWER_MASK sirf_msg_ssb_set_power_mask;
   tSIRF_MSG_SSB_SET_STAT_NAV sirf_msg_ssb_set_stat_nav;
   tSIRF_MSG_SSB_POLL_CLOCK_STATUS sirf_msg_ssb_poll_clock_status;
   tSIRF_MSG_SSB_POLL_ALMANAC sirf_msg_ssb_poll_almanac;
   tSIRF_MSG_SSB_POLL_EPHEMERIS sirf_msg_ssb_poll_ephemeris;
   tSIRF_MSG_SSB_SET_EPHEMERIS sirf_msg_ssb_set_ephemeris;
   tSIRF_MSG_SSB_SET_OP_MODE sirf_msg_ssb_set_op_mode;
   tSIRF_MSG_SSB_SET_LOW_POWER sirf_msg_ssb_set_low_power;
   tSIRF_MSG_SSB_NAV_POLL_RECEIVER_PARAMS sirf_msg_ssb_nav_poll_receiver_params;
   tSIRF_MSG_SSB_DEMO_SET_RESTART_MODE sirf_msg_ssb_demo_set_restart_mode;
   tSIRF_MSG_SSB_DEMO_TEST_CPU_STRESS sirf_msg_ssb_demo_test_cpu_stress;
   tSIRF_MSG_SSB_DEMO_START_GPS_ENGINE sirf_msg_ssb_demo_start_gps_engine;
   tSIRF_MSG_SSB_DEMO_START_NAV_ENGINE sirf_msg_ssb_demo_start_nav_engine;
   tSIRF_MSG_SSB_DEMO_STOP_GPS_ENGINE sirf_msg_ssb_demo_stop_gps_engine;
   tSIRF_MSG_SSB_DEMO_STOP_TEST_APP sirf_msg_ssb_demo_stop_test_app;
   tSIRF_MSG_SSB_SIRFNAV_STORE_NOW sirf_msg_ssb_sirfnav_store_now;
   tSIRF_MSG_SSB_READ_I2CData sirf_msg_ssb_read_i2cdata;
   tSIRF_MSG_SSB_EXT_UART_OPEN_PORT sirf_msg_ssb_ext_uart_open_port;
   tSIRF_MSG_SSB_SET_MSG_RATE sirf_msg_ssb_set_msg_rate;
   tSIRF_MSG_SSB_SET_LOW_POWER_PARAMS sirf_msg_ssb_set_low_power_params;
   tSIRF_MSG_SSB_POLL_CMD_PARAM sirf_msg_ssb_poll_cmd_param;
   tSIRF_MSG_SSB_SET_SBAS_PRN sirf_msg_ssb_set_sbas_prn;
   tSIRF_MSG_SSB_ADVANCED_NAV_INIT sirf_msg_ssb_advanced_nav_init;
   tSIRF_MSG_SSB_DR_SET_NAV_INIT sirf_msg_ssb_dr_set_nav_init;
   tSIRF_MSG_SSB_DR_SET_NAV_MODE sirf_msg_ssb_dr_set_nav_mode;
   tSIRF_MSG_SSB_DR_SET_GYR_FACT_CAL sirf_msg_ssb_dr_set_gyr_fact_cal;
   tSIRF_MSG_SSB_DR_SET_SENS_PARAM sirf_msg_ssb_dr_set_sens_param;
   tSIRF_MSG_SSB_DR_POLL_VALID sirf_msg_ssb_dr_poll_valid;
   tSIRF_MSG_SSB_DR_POLL_GYR_FACT_CAL sirf_msg_ssb_dr_poll_gyr_fact_cal;
   tSIRF_MSG_SSB_DR_POLL_SENS_PARAM sirf_msg_ssb_dr_poll_sens_param;
   tSIRF_MSG_SSB_MMF_SET_MODE sirf_msg_ssb_mmf_set_mode;
   tSIRF_MSG_SSB_TRK_HW_CONFIG sirf_msg_ssb_trk_hw_config;
   tSIRF_MSG_SSB_TRKR_CUSTOMIO_SEL1 sirf_msg_ssb_trkr_customio_sel1;
   tSIRF_MSG_SSB_TRKR_CONFIG sirf_msg_ssb_trkr_config;
   tSIRF_MSG_SSB_TRKR_PEEKPOKE_CMD sirf_msg_ssb_trkr_peekpoke_cmd;
#ifdef PVT_BUILD
   tSIRF_MSG_SSB_PATCH_STORAGE_CONTROL sirf_msg_ssb_patch_storage_control;
#endif /* PVT_BUILD */
   tSIRF_MSG_SSB_TRKR_FLASHSTORE_CMD sirf_msg_ssb_trkr_flashstore_cmd;
   tSIRF_MSG_SSB_TRKR_FLASHERASE_CMD sirf_msg_ssb_trkr_flasherase_cmd;
   tSIRF_MSG_SSB_TRKR_CONFIG_CMD sirf_msg_ssb_trkr_config_cmd;
   tSIRF_MSG_SSB_TRKR_CUSTOMIO_CMD sirf_msg_ssb_trkr_customio_cmd;
   tSIRF_MSG_SSB_GPIO_WRITE sirf_msg_ssb_gpio_write;
   tSIRF_MSG_SSB_GPIO_MODE_SET sirf_msg_ssb_gpio_mode_set;
#ifdef PVT_BUILD
   tSIRF_MSG_SSB_TRKR_CONFIG_POLL sirf_msg_ssb_trkr_config_poll;
   tSIRF_MSG_SSB_CCK_POLL sirf_msg_ssb_cck_poll;
   tSIRF_MSG_SSB_ENB_ALM2FLASH sirf_msg_ssb_enb_alm2flash;
#endif /* PVT_BUILD */
   tSIRF_MSG_SSB_SW_COMMANDED_OFF sirf_msg_ssb_sw_commanded_off;
   tSIRF_MSG_SSB_TRK_HW_TEST_CONFIG sirf_msg_ssb_trk_hw_test_config;
   tSIRF_MSG_SSB_SET_IF_TESTPOINT sirf_msg_ssb_set_if_testpoint;
   tSIRF_MSG_SSB_SET_LNA sirf_msg_ssb_set_lna;
#ifdef PVT_BUILD
   tSIRF_MSG_SSB_SWTB_PMLOAD_IN sirf_msg_ssb_swtb_pmload_in;
#endif /* PVT_BUILD */
   tSIRF_MSG_SSB_COMM_MESSAGE_TEXT sirf_msg_ssb_comm_message_text;
   tSIRF_MSG_SSB_SIRFLOC_IN sirf_msg_ssb_sirfloc_in;
   tSIRF_MSG_SSB_DATA_PEEKPOKE sirf_msg_ssb_data_peekpoke;
   tSIRF_MSG_SSB_EE_INPUT sirf_msg_ssb_ee_input;
   tSIRF_MSG_SSB_EXTENDED_EPHEMERIS sirf_msg_ssb_extended_ephemeris;
   tSIRF_MSG_SSB_BROADCAST_EPHEMERIS sirf_msg_ssb_broadcast_ephemeris;
   tSIRF_MSG_SSB_EXTENDED_IONO sirf_msg_ssb_extended_iono;
   tSIRF_MSG_SSB_EE_SEA_PROVIDE_EPH sirf_msg_ssb_ee_sea_provide_eph;
   tSIRF_MSG_SSB_EE_PROPRIETARY_DEBUG sirf_msg_ssb_ee_proprietary_debug;
   tSIRF_MSG_SSB_SIF_SET_CONFIG sirf_msg_ssb_sif_set_config;
   tSIRF_MSG_SSB_EE_FILE_PART sirf_msg_ssb_ee_file_part;
   tSIRF_MSG_SSB_SIF_DISABLE_AIDING sirf_msg_ssb_sif_disable_aiding;
   tSIRF_MSG_SSB_SIF_GET_AIDING_STATUS sirf_msg_ssb_sif_get_aiding_status;
#ifdef PVT_BUILD
   tSIRF_MSG_SSB_EE_STORAGE_CONTROL sirf_msg_ssb_ee_storage_control;
#endif /* PVT_BUILD */
   tSIRF_MSG_SSB_EE_DISABLE_EE_SECS sirf_msg_ssb_ee_disable_ee_secs;
#ifdef LPL_CLM
   tSIRF_MSG_SSB_EE_SEND_BROADCAST_EPH sirf_msg_ssb_ee_send_broadcast_eph;
#endif /* LPL_CLM */
   tSIRF_MSG_SSB_EE_FILE_DOWNLOAD sirf_msg_ssb_ee_file_download;
   tSIRF_MSG_SSB_SIF_START_DLD sirf_msg_ssb_sif_start_dld;
   tSIRF_MSG_SSB_SIF_EE_FILE_SIZE sirf_msg_ssb_sif_ee_file_size;
   tSIRF_MSG_SSB_SIF_PKT_DATA sirf_msg_ssb_sif_pkt_data;
   tSIRF_MSG_SSB_SIF_GET_EE_AGE sirf_msg_ssb_sif_get_ee_age;
   tSIRF_MSG_SSB_SIF_GET_SGEE_AGE sirf_msg_ssb_sif_get_sgee_age;
   tSIRF_MSG_SSB_HOST_RCV_PKT_DATA sirf_msg_ssb_host_rcv_pkt_data;
   tSIRF_MSG_SSB_HOST_ACK_NACK sirf_msg_ssb_host_ack_nack;
   tSIRF_MSG_SSB_SIF_GET_NVM_HEADER sirf_msg_ssb_sif_get_nvm_header;
   tSIRF_MSG_SSB_SIF_UPDATE_NVM_HEADER sirf_msg_ssb_sif_update_nvm_header;
   tSIRF_MSG_SSB_SET_REGISTER sirf_msg_ssb_set_register;
   tSIRF_MSG_SSB_QUEUE_ERROR sirf_msg_ssb_queue_error;
   tSIRF_MSG_SSB_QUEUE_ALM_ALL sirf_msg_ssb_queue_alm_all;
   tSIRF_MSG_SSB_QUEUE_ALM_RECORD sirf_msg_ssb_queue_alm_record;
   tSIRF_MSG_SSB_SET_LOWPOWER sirf_msg_ssb_set_lowpower;
   tSIRF_MSG_SSB_USER_PARAMS sirf_msg_ssb_user_params;
   tSIRF_MSG_SSB_DATA_RECEIVER_PARAMS sirf_msg_ssb_data_receiver_params;
   tSIRF_MSG_EE_AGE sirf_msg_ee_age;
   tSIRF_SSB_EE_FILE_PART_INFO sirf_ssb_ee_file_part_info;
   tSIRF_SSB_EE_FILE_PART_STATUS sirf_ssb_ee_file_part_status;
   tSIRF_MSG_SSB_NB_SV_DATA sirf_msg_ssb_nb_sv_data;
   tSIRF_MSG_SSB_POSITION_RSP sirf_msg_ssb_position_rsp;
   tSIRF_MSG_SSB_MEAS_PER_SV_DATA sirf_msg_ssb_meas_per_sv_data;
   tSIRF_MSG_SSB_MEAS_RSP sirf_msg_ssb_meas_rsp;
   tSIRF_MSG_SSB_PARTPOS_RSP sirf_msg_ssb_partpos_rsp;
   tSIRF_MSG_SSB_EPH_INFO sirf_msg_ssb_eph_info;
   tSIRF_MSG_SSB_ACKNACK_ERROR sirf_msg_ssb_acknack_error;
   tSIRF_MSG_SSB_OS_ALMANAC sirf_msg_ssb_os_almanac;
   tSIRF_MSG_SSB_ALMANAC_RSP sirf_msg_ssb_almanac_rsp;
   tSIRF_MSG_SSB_EPHEMERIS_PARAMETERS sirf_msg_ssb_ephemeris_parameters;
   tSIRF_MSG_SSB_BE sirf_msg_ssb_be;
   tSIRF_MSG_SSB_SET_IONO_DATA sirf_msg_ssb_set_iono_data;
   tSIRF_MSG_SSB_BE_RSP sirf_msg_ssb_be_rsp;
   tSIRF_MSG_SSB_SESSION_OPEN_RSP sirf_msg_ssb_session_open_rsp;
   tSIRF_MSG_SSB_SLC_STATUS sirf_msg_ssb_slc_status;
   tSIRF_MSG_SSB_SESSION_CLOSE_RSP sirf_msg_ssb_session_close_rsp;
   tSIRF_MSG_SSB_HW_CONFIG_REQ sirf_msg_ssb_hw_config_req;
   tSIRF_MSG_SSB_TIME_TRANSFER_REQ sirf_msg_ssb_time_transfer_req;
   tSIRF_MSG_SSB_OSP_REV_RSP sirf_msg_ssb_osp_rev_rsp;
   tSIRF_MSG_SSB_SLC_AIDING_REQ sirf_msg_ssb_slc_aiding_req;
   tSIRF_MSG_SSB_MPM_STATUS_SVD sirf_msg_ssb_mpm_status_svd;
   tSIRF_MSG_SSB_MPM_STATUS_EPH sirf_msg_ssb_mpm_status_eph;
   tSIRF_MSG_SSB_MPM_STATUS_ALM sirf_msg_ssb_mpm_status_alm;
   tSIRF_MSG_SSB_MPM_STATUS_UPD sirf_msg_ssb_mpm_status_upd;
   tSIRF_MSG_SSB_MPM_STATUS_REC sirf_msg_ssb_mpm_status_rec;
   tSIRF_MSG_SSB_PWR_MODE_RSP sirf_msg_ssb_pwr_mode_rsp;
   tSIRF_MSG_SSB_PWR_MODE_MPM_RSP sirf_msg_ssb_pwr_mode_mpm_rsp;
   tSIRF_MSG_SSB_XO_DEFAULTS_OUT sirf_msg_ssb_xo_defaults_out;
   tSIRF_MSG_SSB_TCXO_TABLE_OUT sirf_msg_ssb_tcxo_table_out;
   tSIRF_MSG_SSB_XO_TEMP_REC_OUT sirf_msg_ssb_xo_temp_rec_out;
   tSIRF_MSG_SSB_XO_EARC_OUT sirf_msg_ssb_xo_earc_out;
   tSIRF_MSG_SSB_XO_RTC_ALARM_OUT sirf_msg_ssb_xo_rtc_alarm_out;
   tSIRF_MSG_SSB_XO_RTC_CAL_OUT sirf_msg_ssb_xo_rtc_cal_out;
   tSIRF_MSG_SSB_XO_MPM_ACQ_OUT_REC sirf_msg_ssb_xo_mpm_acq_out_rec;
   tSIRF_MSG_SSB_XO_MPM_ACQ_OUT sirf_msg_ssb_xo_mpm_acq_out;
   tSIRF_MSG_SSB_XO_MPM_PREPOS_OUT_REC sirf_msg_ssb_xo_mpm_prepos_out_rec;
   tSIRF_MSG_SSB_XO_MPM_PREPOS_OUT sirf_msg_ssb_xo_mpm_prepos_out;
   tSIRF_MSG_SSB_XO_UNL_MEAS_OUT_REC sirf_msg_ssb_xo_unl_meas_out_rec;
   tSIRF_MSG_SSB_XO_UNL_MEAS_OUT sirf_msg_ssb_xo_unl_meas_out;
   tSIRF_MSG_SSB_XO_UNCERTAINTY_OUT sirf_msg_ssb_xo_uncertainty_out;
   tSIRF_MSG_SSB_XO_SYS_TIME_OUT sirf_msg_ssb_xo_sys_time_out;
   tSIRF_MSG_SSB_XO_RTC_TABLE_OUT sirf_msg_ssb_xo_rtc_table_out;
   tSIRF_MSG_SSB_XO_POLY_OUT sirf_msg_ssb_xo_poly_out;
   tSIRF_MSG_SSB_XO_TABLE_OUT sirf_msg_ssb_xo_table_out;
   tSIRF_MSG_SSB_XO_DENSITY_OUT sirf_msg_ssb_xo_density_out;
#ifdef PVT_BUILD
   tSIRF_MSG_SSB_PM_PROMPT_OUT sirf_msg_ssb_pm_prompt_out;
   tSIRF_MSG_SSB_PM_ACK_OUT sirf_msg_ssb_pm_ack_out;
#endif /* PVT_BUILD */
   tSIRF_MSG_SSB_POS_REQ sirf_msg_ssb_pos_req;
   tSIRF_MSG_SSB_EPHEMERIS_INFO_PER_SV sirf_msg_ssb_ephemeris_info_per_sv;
   tSIRF_MSG_SSB_SET_EPH_CLK sirf_msg_ssb_set_eph_clk;
   tSIRF_MSG_SSB_ALMANAC_PARAMETERS sirf_msg_ssb_almanac_parameters;
   tSIRF_MSG_SSB_SET_ALMANAC_ASSIST_DATA sirf_msg_ssb_set_almanac_assist_data;
   tSIRF_MSG_SSB_ACQUISITION_ASSISTANCE_DATA sirf_msg_ssb_acquisition_assistance_data;
   tSIRF_MSG_SSB_SET_ACQ_ASSIST_DATA sirf_msg_ssb_set_acq_assist_data;
   tSIRF_MSG_SSB_SET_RT_INTEGRITY sirf_msg_ssb_set_rt_integrity;
   tSIRF_MSG_SSB_SLC_EPH_STATUS_REQ sirf_msg_ssb_slc_eph_status_req;
   tSIRF_MSG_SSB_ALMANAC_REQ sirf_msg_ssb_almanac_req;
   tSIRF_MSG_SSB_UTC_MODEL sirf_msg_ssb_utc_model;
   tSIRF_MSG_SSB_AUXILIARY_NAV_TOW_ASSIST sirf_msg_ssb_auxiliary_nav_tow_assist;
   tSIRF_MSG_SSB_SET_GPS_TOW_ASSIST sirf_msg_ssb_set_gps_tow_assist;
   tSIRF_MSG_SSB_AUXILIARY_NAV_PARAMS sirf_msg_ssb_auxiliary_nav_params;
   tSIRF_MSG_SSB_SET_AUXILIARY_NAV_PARAMS sirf_msg_ssb_set_auxiliary_nav_params;
   tSIRF_MSG_SSB_BE_REQ_PARAMS sirf_msg_ssb_be_req_params;
   tSIRF_MSG_SSB_BE_REQ sirf_msg_ssb_be_req;
   tSIRF_MSG_SSB_SESSION_OPEN_REQ sirf_msg_ssb_session_open_req;
   tSIRF_MSG_SSB_SLC_STATUS_REQ sirf_msg_ssb_slc_status_req;
   tSIRF_MSG_SSB_TEST_MODE_CONFIG_REQ sirf_msg_ssb_test_mode_config_req;
   tSIRF_MSG_SSB_SERIAL_PORT_SETTING_REQ sirf_msg_ssb_serial_port_setting_req;
   tSIRF_MSG_SSB_TIME_FREQ_APROX_POS_REQ sirf_msg_ssb_time_freq_aprox_pos_req;
   tSIRF_MSG_SSB_TIME_TRANSFER_RSP sirf_msg_ssb_time_transfer_rsp;
   tSIRF_MSG_SSB_PUSH_AIDING_AVAILABILITY_REQ sirf_msg_ssb_push_aiding_availability_req;
   tSIRF_MSG_SSB_HW_CONFIG_RSP sirf_msg_ssb_hw_config_rsp;
   tSIRF_MSG_SSB_SESSION_CLOSE_REQ sirf_msg_ssb_session_close_req;
   tSIRF_MSG_SSB_OSP_REV_REQ sirf_msg_ssb_osp_rev_req;
   tSIRF_MSG_SSB_NAV_SF_123 sirf_msg_ssb_nav_sf_123;
   tSIRF_MSG_SSB_NAV_SF_123_RSP sirf_msg_ssb_nav_sf_123_rsp;
   tSIRF_MSG_SSB_NAV_SF_45_RSP sirf_msg_ssb_nav_sf_45_rsp;
   tSIRF_MSG_SSB_PWR_MODE_FPM_REQ sirf_msg_ssb_pwr_mode_fpm_req;
   tSIRF_MSG_SSB_PWR_MODE_APM_REQ sirf_msg_ssb_pwr_mode_apm_req;
   tSIRF_MSG_SSB_PWR_MODE_MPM_REQ sirf_msg_ssb_pwr_mode_mpm_req;
   tSIRF_MSG_SSB_PWR_MODE_TP_REQ sirf_msg_ssb_pwr_mode_tp_req;
   tSIRF_MSG_SSB_PWR_MODE_PTF_REQ sirf_msg_ssb_pwr_mode_ptf_req;
   tSIRF_MSG_SSB_CW_CONFIG sirf_msg_ssb_cw_config;
   tSIRF_MSG_SSB_XO_OUTPUT_CONTROL_IN sirf_msg_ssb_xo_output_control_in;
   tSIRF_MSG_SSB_XO_DEFAULTS_IN sirf_msg_ssb_xo_defaults_in;
   tSIRF_MSG_SSB_TCXO_TABLE_IN sirf_msg_ssb_tcxo_table_in;
   tSIRF_MSG_SSB_XO_TEST_CONTROL_IN sirf_msg_ssb_xo_test_control_in;
   tSIRF_MSG_SSB_XO_POLY_IN sirf_msg_ssb_xo_poly_in;
   tSIRF_MSG_SSB_SENSOR_INIT_REG sirf_msg_ssb_sensor_init_reg;
   tSIRF_MSG_SSB_SENSOR_READ_REG sirf_msg_ssb_sensor_read_reg;
   tSIRF_MSG_SSB_SENSOR_CTRL_REG sirf_msg_ssb_sensor_ctrl_reg;
   tSIRF_MSG_SSB_SENSOR sirf_msg_ssb_sensor;
   tSIRF_MSG_SSB_SCALE_ZERO_POINT sirf_msg_ssb_scale_zero_point;
   tSIRF_MSG_SSB_SENSOR_CONFIG sirf_msg_ssb_sensor_config;
   tSIRF_MSG_SSB_SENSOR_SWITCH sirf_msg_ssb_sensor_switch;
   tSIRF_MSG_SSB_SENSOR_DATA sirf_msg_ssb_sensor_data;
   tSIRF_MSG_SSB_SENSOR_READINGS sirf_msg_ssb_sensor_readings;
   tSIRF_MSG_SBB_SENSOR_INIT_REG_DATA sirf_msg_sbb_sensor_init_reg_data;
   tSIRF_MSG_SSB_SENSOR_FACTORY_STORED_PARAMS sirf_msg_ssb_sensor_factory_stored_params;
   tSIRF_MSG_SBB_RCVR_STATE sirf_msg_sbb_rcvr_state;
   tSIRF_MSG_SSB_POINT_N_TELL_OUTPUT sirf_msg_ssb_point_n_tell_output;
   tSIRF_MSG_SSB_SENSOR_CALIBRATION_OUTPUT sirf_msg_ssb_sensor_calibration_output;
   tSIRF_MSG_SSB_MAG_MODEL_PARAMS sirf_msg_ssb_mag_model_params;
} tSIRF_MSG_SSB;

/*******************************************************************************
   The maximum message length that will ever be used in a message buffer.
   This relies on the largest possible message being contained
   in the tSIRF_MSG_SSB union.  The compiler may pad this union to be larger
   than the maximum message size, but it cannot be smaller.
*******************************************************************************/
#define SIRF_MSG_SSB_MAX_MESSAGE_LEN    ( sizeof(tSIRF_MSG_SSB) )

/* Special case: make the differential corrections message as large as possible based on
 * other message sizes
 */

/*******************************************************************************
   SIRF_MSG_SSB_RTCM                   ( 0x11 ) 17
*******************************************************************************/
#define SIRF_MSG_SSB_MAX_RTCM_BUFFER (SIRF_MSG_SSB_MAX_MESSAGE_LEN - sizeof(tSIRF_UINT16))
typedef struct tSIRF_MSG_SSB_RTCM_tag
{
   tSIRF_UINT16      buffer_len;
   tSIRF_UINT8       buffer[SIRF_MSG_SSB_MAX_RTCM_BUFFER];
}tSIRF_MSG_SSB_RTCM;

#endif /* NOT_WORKING_FOR_NOW */

#endif /* __SIRF_MSG_SSB__H__ */


/**
 * @}
 */

