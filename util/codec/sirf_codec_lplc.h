/**
 * @addtogroup lpl3.0_cmdr
 * @{
 */

/***************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. GPS Software                    *
 *                                                                         *
 *  Copyright (c) 2005-2009 by SiRF Technology, Inc.  All rights reserved. *
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
 * MODULE:  LPLCMDR
 *
 * FILENAME:  sirf_codec_lplc.h
 *
 * DESCRIPTION: Functions to encode and decode LPL Command messages from
 *              a byte stream to a C structure.
 *
 ***************************************************************************/
#ifndef _LPL_CMDR_MSG_H
#define _LPL_CMDR_MSG_H

#include "sirf_types.h"
#include "sirf_errors.h"

/* Logical Channel, Message ID, Sub ID */
#define SIRF_LPLC_MSG_HEADER_SIZE_BYTES (3*sizeof(tSIRF_UINT8))

#define LPLC_LOC_API_SESSION_OPEN                        MID001_30   /* Ver 1.21 */
#define LPLC_LOC_API_SESSION_CLOSE                       MID001_31   /* Ver 1.21*/
#define LPLC_PLATFORM_CONFIG_SET_LOCATION_METHOD         MID129_30   /* Ver 1.21 */
#define LPLC_PLATFORM_CONFIG_SET_ICD                     MID129_31   /* Ver 1.21 */
#define LPLC_PLATFORM_CONFIG_SET_AIDING_CAPABILITIES     MID129_32   /* Ver 1.21 */
#define LPLC_PLATFORM_CONFIG_SET_AGPS_SERVER             MID129_33   /* Ver 1.22 */
#define LPLC_PLATFORM_CONFIG_SET_AGPS_SERVER_SECURITY    MID129_34   /* Ver 1.22 */
#define LPLC_GPS_CONFIG_SET_PORT                         MID130_30   /* Ver 1.21 */
#define LPLC_GPS_CONFIG_SET_DEBUG_LISTENER               MID130_31   /* Ver 1.21 */
#define LPLC_GPS_CONFIG_SET_SSB_LISTENER                 MID130_32   /* Ver 1.21 */
#define LPLC_GPS_CONFIG_SET_NMEA_LISTENER                MID130_33   /* Ver 1.21 */
#define LPLC_GPS_CONFIG_SET_LOCATION_LISTENER            MID130_34   /* Ver 1.21 */                                                           
#define LPLC_GPS_CONFIG_SET_RESET_TYPE                   MID130_35   /* Ver 1.21 */ 
#define LPLC_GPS_CONFIG_SET_MESSAGE_LISTENER             MID130_36   /* Ver 1.21 */ 
#define LPLC_GPS_CONFIG_SET_AI3_LISTENER                 MID130_37   /* Ver 1.21 */ 
#define LPLC_GPS_CONFIG_SET_F_LISTENER                   MID130_38   /* Ver 1.21 */ 
#define LPLC_GPS_CONFIG_SET_LOGGING_INFORMATION          MID133_30   /* Ver 1.21 */ 
#define LPLC_PLATFORM_DEBUG_OUTPUT_TEXT_MSG              MID134_01   /* Ver 1.12 */
#define LPLC_PLATFORM_DEBUG_INPUT_TEXT_MSG               MID134_02   /* Ver 1.19 */
#define LPLC_PLATFORM_VERSION_INFO_POLL_SW_VERSION       MID135_01   /* Ver 1.12 */
#define LPLC_PLATFORM_VERSION_INFO_POLL_SW_VERSION_RESP  MID135_02   /* Ver 1.12 */
#define LPLC_PLATFORM_VERSION_INFO_POLL_ICD_VERSION      MID135_03   /* Ver 1.15 */
#define LPLC_PLATFORM_VERSION_INFO_POLL_ICD_VERSION_RESP MID135_04   /* Ver 1.15 */
#define LPLC_PLATFORM_GM_INIT                            MID137_30   /* Ver 1.21 */
#define LPLC_PLATFORM_GM_RELEASE                         MID137_31   /* Ver 1.21 */
#define LPLC_GPS_CONFIG_SET_FREQ_TRANSFER_METHOD         MID139_30   /* Ver 1.21 */
#define LPLC_GPS_CONFIG_SET_FREQ_TRANSFER_PARAM          MID139_31   /* Ver 1.21 */
#define LPLC_GPS_CONFIG_SET_FREQ_NOMINAL                 MID139_32   /* Ver 1.21 */
#define LPLC_GPS_CONFIG_UPDATE_FREQ                      MID139_33   /* Ver 1.21 */
#define LPLC_GENERIC_RESPONSE_MESSAGE_ACK_NACK           MID255_01   /* Ver 1.12 */

#define MID001_30 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x01, 0x1E )   /* Ver 1.21 */
#define MID001_31 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x01, 0x1F )   /* Ver 1.21 */
#define MID129_30 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x81, 0x1E )   /* Ver 1.21 */
#define MID129_31 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x81, 0x1F )   /* Ver 1.21 */
#define MID129_32 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x81, 0x20 )   /* Ver 1.21 */
#define MID129_33 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x81, 0x21 )   /* Ver 1.22 */
#define MID129_34 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x81, 0x22 )   /* Ver 1.22 */
#define MID130_30 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x82, 0x1E )   /* Ver 1.21 */
#define MID130_31 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x82, 0x1F )   /* Ver 1.21 */
#define MID130_32 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x82, 0x20 )   /* Ver 1.21 */
#define MID130_33 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x82, 0x21 )   /* Ver 1.21 */
#define MID130_34 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x82, 0x22 )   /* Ver 1.21 */
#define MID130_35 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x82, 0x23 )   /* Ver 1.21 */
#define MID130_36 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x82, 0x24 )   /* Ver 1.21 */
#define MID130_37 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x82, 0x25 )   /* Ver 1.21 */
#define MID130_38 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x82, 0x26 )   /* Ver 1.21 */
#define MID133_30 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x85, 0x1E )   /* Ver 1.21 */
#define MID134_01 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x86, 0x01 )   /* Ver 1.12 */
#define MID134_02 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x86, 0x02 )   /* Ver 1.19 */
#define MID135_01 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x87, 0x01 )   /* Ver 1.12 */
#define MID135_02 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x87, 0x02 )   /* Ver 1.12 */
#define MID135_03 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x87, 0x03 )   /* Ver 1.15 */
#define MID135_04 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x87, 0x04 )   /* Ver 1.15 */
#define MID137_30 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x89, 0x1E )   /* Ver 1.21 */
#define MID137_31 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x89, 0x1F )   /* Ver 1.21 */
#define MID139_30 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x8B, 0x1E )   /* Ver 1.21 */
#define MID139_31 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x8B, 0x1F )   /* Ver 1.21 */
#define MID139_32 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x8B, 0x20 )   /* Ver 1.21 */
#define MID139_33 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0x8B, 0x21 )   /* Ver 1.21 */
#define MID255_01 SIRF_MAKE_MSG_ID(SIRF_LC_LPLC, 0xFF, 0x01 )   /* Ver 1.12 */

/** The app Id of the LPL commander application */
#define LPLC_APP_ID (1)

extern tSIRF_CHAR g_LPL_CMD_MSG_ICD_Version[];
extern tSIRF_CHAR g_LPL_CMD_MSG_ICD_Date[];

#define STR_LEN_MAX 20
/*----------------------------------------------------------------------------*/
#define MID001_30_ENCODED_SIZE (sizeof(tSIRF_UINT8))
typedef struct LPLCmdrMsg001_30 /* LPLC_LOC_API_SESSION_OPEN */
{
   tSIRF_UINT8  sessionType;
} tLPLCmdrMsg001_30;

/*----------------------------------------------------------------------------*/
#define MID001_31_ENCODED_SIZE (sizeof(tSIRF_UINT32) + sizeof(tSIRF_UINT8))
typedef struct LPLCmdrMsg001_31 /* LPLC_LOC_API_SESSION_CLOSE */
{
   tSIRF_UINT32 GeoSession_id;
   tSIRF_UINT8  GeoSession_Type;
} tLPLCmdrMsg001_31;

/*----------------------------------------------------------------------------*/
#define MID129_30_ENCODED_SIZE (sizeof(tSIRF_UINT8))
typedef struct LPLCmdrMsg129_30 /* LPLC_PLATFORM_CONFIG_SET_LOCATION_METHOD */
{
   tSIRF_UINT8  locationMethod;
} tLPLCmdrMsg129_30;

/*----------------------------------------------------------------------------*/
#define MID129_31_ENCODED_SIZE (3*sizeof(tSIRF_UINT8))
typedef struct LPLCmdrMsg129_31 /* LPLC_PLATFORM_CONFIG_SET_ICD */
{
   tSIRF_UINT8 AI3_icd;
   tSIRF_UINT8 F_icd;
   tSIRF_UINT8 ICDsAutoDetect;
} tLPLCmdrMsg129_31;
/*----------------------------------------------------------------------------*/
#define MID129_32_ENCODED_SIZE (3*sizeof(tSIRF_UINT8))
typedef struct LPLCmdrMsg129_32 /* LPLC_PLATFORM_CONFIG_SET_AIDING_CAPABILITIES */
{
   tSIRF_UINT8  LPL_NAV_BITS_AIDING;
   tSIRF_UINT8  LPL_SGEE_AIDING;
   tSIRF_UINT8  LPL_CGEE_AIDING;
} tLPLCmdrMsg129_32;

#define LPLC_PLATFORM_CONFIG_SET_AGPS_SERVER_MAX_IP_ADDRESS (256)
/*----------------------------------------------------------------------------*/
#define MID129_33_ENCODED_SIZE_FIXED (sizeof(tSIRF_UINT16)+sizeof(tSIRF_UINT32))
typedef struct LPLCmdrMsg129_33 /* LPLC_PLATFORM_CONFIG_SET_AGPS_SERVER */
{
   tSIRF_UINT16 port_number;
   tSIRF_UINT32 sz_ipaddress;
   tSIRF_CHAR ip_address[LPLC_PLATFORM_CONFIG_SET_AGPS_SERVER_MAX_IP_ADDRESS];
} tLPLCmdrMsg129_33;
/*----------------------------------------------------------------------------*/
#define MID129_34_ENCODED_SIZE (sizeof(tSIRF_UINT8))
typedef struct LPLCmdrMsg129_34 /* LPLC_PLATFORM_CONFIG_SET_AGPS_SERVER_SECURITY */
{
   tSIRF_UINT8  use_security; /* Boolean value, 0 is false, nonzero is use security */
} tLPLCmdrMsg129_34;
/*----------------------------------------------------------------------------*/
#define MID130_30_ENCODED_SIZE (2*sizeof(tSIRF_UINT8))
typedef struct LPLCmdrMsg130_30  /* LPLC_GPS_CONFIG_SET_PORT */
{
   tSIRF_UINT8  Port;
   tSIRF_UINT8  Enable;
} tLPLCmdrMsg130_30;
/*----------------------------------------------------------------------------*/
#define MID130_31_ENCODED_SIZE (sizeof(tSIRF_UINT32) + sizeof(tSIRF_UINT8))
typedef struct LPLCmdrMsg130_31 /* LPLC_GPS_CONFIG_SET_DEBUG_LISTENER */
{
   tSIRF_UINT32 GeoSession_ID;
   tSIRF_UINT8  GeoSession_Type;
} tLPLCmdrMsg130_31;
/*----------------------------------------------------------------------------*/
#define MID130_32_ENCODED_SIZE (sizeof(tSIRF_UINT32) + sizeof(tSIRF_UINT8))
typedef struct LPLCmdrMsg130_32 /* LPLC_GPS_CONFIG_SET_SSB_LISTENER */
{
   tSIRF_UINT32 GeoSession_ID;
   tSIRF_UINT8  GeoSession_Type;
} tLPLCmdrMsg130_32;
/*----------------------------------------------------------------------------*/
#define MID130_33_ENCODED_SIZE (sizeof(tSIRF_UINT32) + sizeof(tSIRF_UINT8))
typedef struct LPLCmdrMsg130_33 /* LPLC_GPS_CONFIG_SET_NMEA_LISTENER */
{
   tSIRF_UINT32 GeoSession_ID;
   tSIRF_UINT8  GeoSession_Type;
} tLPLCmdrMsg130_33;
/*----------------------------------------------------------------------------*/
#define MID130_34_ENCODED_SIZE (4*sizeof(tSIRF_UINT8)+3*sizeof(tSIRF_UINT32)+sizeof(tSIRF_UINT16))
typedef struct LPLCmdrMsg130_34 /* LPLC_GPS_CONFIG_SET_LOCATION_LISTENER */
{
   tSIRF_UINT32 GeoSession_ID;
   tSIRF_UINT8  GeoSession_Type;
   tSIRF_UINT32 AGPS_QOShorizontalAccuracy;
   tSIRF_UINT32 AGPS_QOSverticalAccuracy;
   tSIRF_UINT8  AGPS_QOSmaxResponseTime;
   tSIRF_UINT16 AGPS_QOS_maxLocationAge;
   tSIRF_UINT8  reqPositions;
   tSIRF_UINT8  timeBetweenFixes;
} tLPLCmdrMsg130_34;
/*----------------------------------------------------------------------------*/
#define MID130_35_ENCODED_SIZE (sizeof(tSIRF_UINT8))
typedef struct LPLCmdrMsg130_35 /* LPLC_GPS_CONFIG_SET_RESET_TYPE */
{
   tSIRF_UINT8  resetType;
} tLPLCmdrMsg130_35;
/*----------------------------------------------------------------------------*/
#define MID130_36_ENCODED_SIZE (sizeof(tSIRF_UINT32) + sizeof(tSIRF_UINT8))
typedef struct LPLCmdrMsg130_36 /* LPLC_GPS_CONFIG_SET_MESSAGE_LISTENER */
{
   tSIRF_UINT32 GeoSession_ID;
   tSIRF_UINT8  GeoSession_Type;
} tLPLCmdrMsg130_36;
/*----------------------------------------------------------------------------*/
#define MID130_37_ENCODED_SIZE (sizeof(tSIRF_UINT32) + sizeof(tSIRF_UINT8))
typedef struct LPLCmdrMsg130_37 /* LPLC_GPS_CONFIG_SET_AI3_LISTENER */
{
   tSIRF_UINT32 GeoSession_ID;
   tSIRF_UINT8  GeoSession_Type;
} tLPLCmdrMsg130_37;
/*----------------------------------------------------------------------------*/
#define MID130_38_ENCODED_SIZE (sizeof(tSIRF_UINT32) + sizeof(tSIRF_UINT8))
typedef struct LPLCmdrMsg130_38 /* LPLC_GPS_CONFIG_SET_F_LISTENER */
{
   tSIRF_UINT32 GeoSession_ID;
   tSIRF_UINT8  GeoSession_Type;
} tLPLCmdrMsg130_38;
/*----------------------------------------------------------------------------*/
/* variable length message, so this is just the constant part */
#define MID133_30_ENCODED_SIZE_FIXED (6*sizeof(tSIRF_UINT8))

typedef struct LPLCmdrMsg133_30 /* LPLC_GPS_CONFIG_SET_LOGGING_INFORMATION */
{
   tSIRF_UINT8  LoggingType;
   tSIRF_UINT16 briefLogFileName_length;
   tSIRF_UINT8  briefLogFileName[STR_LEN_MAX];
   tSIRF_UINT16 detailedLogFileName_length;
   tSIRF_UINT8  detailedLogFileName[STR_LEN_MAX];
   tSIRF_UINT16 agpsLogFileName_length;
   tSIRF_UINT8  agpsLogFileName[STR_LEN_MAX];
   tSIRF_UINT16 previousLocationFileName_length;
   tSIRF_UINT8  previousLocationFileName[STR_LEN_MAX];
   tSIRF_UINT16 slcLogFileName_length;
   tSIRF_UINT8  slcLogFileName[STR_LEN_MAX];
} tLPLCmdrMsg133_30;
/*----------------------------------------------------------------------------*/
#define MID134_01_ENCODED_SIZE_FIXED (2*sizeof(tSIRF_UINT8) + sizeof(tSIRF_UINT16))
#define MID134_01_TEXT_LEN_MAX (200)
typedef struct LPLCmdrMsg134_01 
{
   tSIRF_UINT8  m_AppID;
   tSIRF_UINT16 m_TextLen;
   tSIRF_UINT8  m_StatusMessage[MID134_01_TEXT_LEN_MAX];
} tLPLCmdrMsg134_01;
/*----------------------------------------------------------------------------*/
#define MID134_02_ENCODED_SIZE_FIXED (sizeof(tSIRF_UINT8) + sizeof(tSIRF_UINT16))
#define MID134_02_TEXT_LEN_MAX (200)
typedef struct LPLCmdrMsg134_02 
{
   tSIRF_UINT8  m_AppID;
   tSIRF_UINT16 m_TextLen;
   tSIRF_UINT8  m_StatusMessage[MID134_02_TEXT_LEN_MAX];
} tLPLCmdrMsg134_02;
/*----------------------------------------------------------------------------*/
#define MID135_01_ENCODED_SIZE (0)
typedef struct LPLCmdrMsg135_01 
{
   tSIRF_UINT8 m_PLACE_HOLDER; /* No payload besides the MID and SUBID */
} tLPLCmdrMsg135_01;
/*----------------------------------------------------------------------------*/
#define MID135_02_ENCODED_SIZE_FIXED (sizeof(tSIRF_UINT8))
#define MID135_02_SW_VERSION_LENGTH_MAX (200)
typedef struct LPLCmdrMsg135_02 
{
   tSIRF_UINT16 m_SWVerLen;
   tSIRF_UINT8  m_SWVersion[MID135_02_SW_VERSION_LENGTH_MAX];
} tLPLCmdrMsg135_02;
/*----------------------------------------------------------------------------*/
#define MID135_03_ENCODED_SIZE (0)
typedef struct LPLCmdrMsg135_03 
{
   tSIRF_UINT8 m_PLACE_HOLDER; /* No payload besides the MID and SUBID */
} tLPLCmdrMsg135_03;
/*----------------------------------------------------------------------------*/
#define MID135_04_ENCODED_SIZE_FIXED (sizeof(tSIRF_UINT8))
#define MID135_04_ICD_VERSION_LENGTH_MAX (200)
typedef struct LPLCmdrMsg135_04 
{
   tSIRF_UINT16 m_ICDVerLen;
   tSIRF_UINT8  m_ICDVersion[MID135_04_ICD_VERSION_LENGTH_MAX];
} tLPLCmdrMsg135_04;
/*----------------------------------------------------------------------------*/
#define MID137_30_ENCODED_SIZE_FIXED (6*sizeof(tSIRF_UINT8)+2*sizeof(tSIRF_UINT16)+sizeof(tSIRF_UINT32))
typedef struct LPLCmdrMsg137_30 /* LPLC_PLATFORM_GM_INIT */
{
   tSIRF_UINT8  gpsDevice_configType;
   tSIRF_UINT8  gpsDevice_SerialConfig_needToOpenComport;
   tSIRF_UINT16 gpsDevice_SerialConfig_serialport_length; 
   tSIRF_INT8   gpsDevice_SerialConfig_SerialPort[STR_LEN_MAX];
   tSIRF_INT32  gpsDevice_SerialConfig_BaudRate;
   tSIRF_UINT8  gpsDevice_SerialConfig_needToOpenVirtualPort;
   tSIRF_UINT16 gpsDevice_SerialConfig_nmeaport_length; 
   tSIRF_INT8   gpsDevice_SerialConfig_NMEASerialPort[STR_LEN_MAX]; 
   tSIRF_UINT8  gpsDevice_SerialConfig_bUseNMEA;
   tSIRF_INT32  memConfig_memSize;
   tSIRF_UINT8  powerControl_cgpsCtrl;
   tSIRF_UINT8  powerControl_gpsPwrCtrlCap;
} tLPLCmdrMsg137_30;
/*----------------------------------------------------------------------------*/
#define MID137_31_ENCODED_SIZE (0)
typedef struct LPLCmdrMsg137_31 /* LPLC_PLATFORM_GM_RELEASE */
{
   tSIRF_UINT8 dummy; /* Added because it doesn't compile without it */
} tLPLCmdrMsg137_31;
/*----------------------------------------------------------------------------*/
#define MID139_30_ENCODED_SIZE (sizeof(tSIRF_UINT32))
typedef struct LPLCmdrMsg139_30 /* LPLC_GPS_CONFIG_SET_FREQ_TRANSFER_METHOD */
{
  tSIRF_UINT32 FreqTxMetho ; 
} tLPLCmdrMsg139_30;

/*----------------------------------------------------------------------------*/
#define MID139_31_ENCODED_SIZE (2*sizeof(tSIRF_UINT32))
typedef struct LPLCmdrMsg139_31 /* LPLC_GPS_CONFIG_SET_FREQ_TRANSFER_PARAM */
{
   tSIRF_UINT32 MaxRequestTime;
   tSIRF_UINT32 RequestPeriod;
} tLPLCmdrMsg139_31;

/*----------------------------------------------------------------------------*/
#define MID139_32_ENCODED_SIZE (sizeof(tSIRF_DOUBLE))
typedef struct LPLCmdrMsg139_32 /* LPLC_GPS_CONFIG_SET_FREQ_NOMINAL */
{
    tSIRF_DOUBLE NominalFreq;
} tLPLCmdrMsg139_32;

/*----------------------------------------------------------------------------*/
#define MID139_33_ENCODED_SIZE (3*sizeof(tSIRF_UINT32)+2*sizeof(tSIRF_DOUBLE))
typedef struct LPLCmdrMsg139_33 /* LPLC_GPS_CONFIG_UPDATE_FREQ */
{
    tSIRF_UINT32 RequestID;
    tSIRF_UINT32 FrequencyDataType;
    tSIRF_DOUBLE  frequency;
    tSIRF_UINT32 AccuracyDataType;
    tSIRF_DOUBLE Accuracy;
} tLPLCmdrMsg139_33;
/*----------------------------------------------------------------------------*/
#define MID255_01_ENCODED_SIZE (3*sizeof(tSIRF_UINT8)+sizeof(tSIRF_UINT32))
#define MID255_01_RESPONSE_TYPE_ACK (1)
#define MID255_01_RESPONSE_TYPE_NAK (2)
typedef struct LPLCmdrMsg255_01 
{
   tSIRF_UINT8  m_ResponseToMID;
   tSIRF_UINT8  m_ResponseToSUBID;
   tSIRF_UINT8  m_ResponseType;
   tSIRF_UINT32 m_ResponseCode;
} tLPLCmdrMsg255_01;


/* GENERIC COMPOSITE typedef structure FOR LPL CMDR MESSAGES */
typedef struct LPLCmdrMsgGeneric
{
   union
   {
      /* CORE MESSAGES */
      tLPLCmdrMsg001_30 Msg001_30;
      tLPLCmdrMsg001_31 Msg001_31;
      tLPLCmdrMsg129_30 Msg129_30;
      tLPLCmdrMsg129_31 Msg129_31;
      tLPLCmdrMsg129_32 Msg129_32; 
      tLPLCmdrMsg129_33 Msg129_33; 
      tLPLCmdrMsg129_34 Msg129_34; 
      tLPLCmdrMsg130_30 Msg130_30;
      tLPLCmdrMsg130_31 Msg130_31;
      tLPLCmdrMsg130_32 Msg130_32;
      tLPLCmdrMsg130_33 Msg130_33;
      tLPLCmdrMsg130_34 Msg130_34;
      tLPLCmdrMsg130_35 Msg130_35;
      tLPLCmdrMsg130_36 Msg130_36;
      tLPLCmdrMsg130_37 Msg130_37;
      tLPLCmdrMsg130_38 Msg130_38;
      tLPLCmdrMsg133_30 Msg133_30;
      tLPLCmdrMsg134_01 Msg134_01;
      tLPLCmdrMsg134_02 Msg134_02;
      tLPLCmdrMsg135_01 Msg135_01;
      tLPLCmdrMsg135_02 Msg135_02;
      tLPLCmdrMsg135_03 Msg135_03;
      tLPLCmdrMsg135_04 Msg135_04;
      tLPLCmdrMsg137_30 Msg137_30;
      tLPLCmdrMsg137_31 Msg137_31;
      tLPLCmdrMsg139_30 Msg139_30;
      tLPLCmdrMsg139_31 Msg139_31;
      tLPLCmdrMsg139_32 Msg139_32;
      tLPLCmdrMsg139_33 Msg139_33;
      tLPLCmdrMsg255_01 Msg255_01;
   } m_MsgPayload;
} tLPLCmdrMsgGeneric;

#ifdef __cplusplus
extern "C" {
#endif

tSIRF_RESULT SIRF_CODEC_LPLC_Encode( tSIRF_UINT32 message_id,
                                         tSIRF_VOID *message_structure,
                                         tSIRF_UINT32 message_length,
                                         tSIRF_UINT8 *packet,
                                         tSIRF_UINT32 *packet_length,
                                         tSIRF_BOOL ascii_packet);

tSIRF_RESULT SIRF_CODEC_LPLC_Decode( tSIRF_UINT8* payload,
                                         tSIRF_UINT32 payload_length,
                                         tSIRF_UINT32 *message_id,
                                         tSIRF_VOID *message_structure,
                                         tSIRF_UINT32 *message_length,
                                         tSIRF_BOOL ascii_packet);

tSIRF_UINT16 SwapBytes(tSIRF_UINT16 inBytes);
tSIRF_UINT32 AddChannelID(tSIRF_UINT32 inBytes);
tSIRF_UINT8 GetMID(tSIRF_UINT16 inBytes);
tSIRF_UINT8 GetSUBID(tSIRF_UINT16 inBytes);

#ifdef __cplusplus
}
#endif

#endif /* _LPL_CMDR_MSG_H */

/**
 * @}
 */
