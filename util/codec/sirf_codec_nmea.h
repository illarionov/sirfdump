/**
 * @addtogroup platform_src_sirf_util_codec
 * @{
 */

 /**************************************************************************
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
 * FILENAME:  sirf_codec_nmea.h
 *
 * DESCRIPTION: Routine prototypes and symbol definitions
 *
 ***************************************************************************/

#ifndef __SIRF_CODEC_NMEA_H__
#define __SIRF_CODEC_NMEA_H__

#include "sirf_types.h"
#include "sirf_msg.h"

/***************************************************************************
   Macro Definitions
***************************************************************************/


#define SIRF_CODEC_NMEA_GGA  1
#define SIRF_CODEC_NMEA_RMC  2
#define SIRF_CODEC_NMEA_GLL  3
#define SIRF_CODEC_NMEA_GSA  4
#define SIRF_CODEC_NMEA_VTG  5
#define SIRF_CODEC_NMEA_GSV  6

/* GGA, RMC, GLL, GSA, and VTG can be made from GEODETIC */
#define SIRF_CODEC_NMEA_MAX_MSG_FROM_GEODETIC         (5)
#define SIRF_CODEC_NMEA_MAX_MSG_FROM_MEASURED_TRACKER (1)

#define SIRF_CODEC_NMEA_PSRF108         (0)
#define SIRF_CODEC_NMEA_PSRF107_ONE_SAT (1)
#define SIRF_CODEC_NMEA_PSRF107_TWO_SAT (2)

#define SIRF_CODEC_NMEA_MAX_MSG_FROM_EE_SEA_PROVIDE_EPH (3)

/* NMEA Message IDs. These are translated to GPS Message IDs defined in gps_messages.h */

#define SIRF_CODEC_NMEA_PSRF100_SET_SERIAL_PORT           100
#define SIRF_CODEC_NMEA_PSRF101_NAVIGATION_INITIALIZATION 101

#define NMEA_EE_REQUEST_EPHEMERIS        ( 151 )
#define NMEA_EE_INTEGRITY_WARNING        ( 152 )
#define NMEA_EE_MSG_ACK                  ( 154 )
#define NMEA_EE_PROVIDE_EPHEMERIS        ( 107 )
#define NMEA_EE_PROVIDE_IONO_PARAMS      ( 108 )
#define NMEA_EE_DEBUG_MSG                ( 110 )
#define NMEA_EE_CLK_BIAS_ADJUSTMENT      ( 155 )
#define NMEA_EE_SET_MSG_RATE             ( 130 )      
#define NMEA_EE_SUBFRAME_NAV_BITS_OUTPUT ( 140 )

/* Option values for SIRF_CODEC_NMEA_PSRF100_SET_SERIAL_PORT */
#define SIRF_CODEC_NMEA_MAX_MSG_FROM_PSRF100 (2)
#define SIRF_CODEC_NMEA_SSB_SET_PORT_VALUES  (1)
#define SIRF_CODEC_NMEA_SSB_SET_PROTOCOL     (2)

/** Maximum number of channels in the NMEA message for GSA and GSV */
#define SIRF_CODEC_NMEA_MAX_CHANNELS (12)


#define SIRF_MSG_SSB_DOP_LSB ( 0.2F )
#define   CR         (0x0D)       /* End of Sentence indicator */
#define   LF         (0x0A)       /* End of sentence indicator */

/***************************************************************************
   Prototype Definitions
***************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

tSIRF_RESULT SIRF_CODEC_NMEA_Encode( 
   tSIRF_UINT32               message_id, 
   tSIRF_VOID   const * const message_structure,
   tSIRF_UINT32               message_length,
   tSIRF_UINT8        * const payload, 
   tSIRF_UINT32       * const payload_length,
   tSIRF_UINT32       * const options );

tSIRF_RESULT SIRF_CODEC_NMEA_Decode( 
   tSIRF_UINT8  const * const payload, 
   tSIRF_UINT32               payload_length,
   tSIRF_UINT32       * const message_id, 
   tSIRF_VOID         * const message_structure,
   tSIRF_UINT32       * const message_length,
   tSIRF_UINT32       * const options );

#ifdef __cplusplus
}
#endif

#endif /*__SIRF_CODEC_NMEA_H__ */

/**
 * @}
 * End of file.
 */

