/**
 * @addtogroup platform_src_sirf_include
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
 */

#ifndef SIRF_MSG_GPRS_AT_COMMAND_H_INCLUDED
#define SIRF_MSG_GPRS_AT_COMMAND_H_INCLUDED

/* For more information on each of these message see 
   S000437e.pdf and 
   S000293I.pdf 
   from the MultiTech Systems documentation

   NOTE: All strings must be quoted, the codecs do not add the quotes.  The 
         reason for this is because unused optional parameters must be empty
         strings and not "".  For example for the +WIPBR,4,6,0,,, is a valid 
         command yet +WIPBR,4,6,0,"","","" is not
*/

/*-----------------------------------------------------------------------------
 *   Include Files
 *----------------------------------------------------------------------------*/

#include "sirf_msg.h"
#include "sirf_types.h"
#include "sirf_msg_ssb.h"

/*-----------------------------------------------------------------------------
 * Macro Definitions
 *----------------------------------------------------------------------------*/


/* The maximum message length that will ever be used in a message buffer.
   Same as SSB for now. */
#define SIRF_MSG_GPRS_AT_COMMAND_MAX_MESSAGE_LEN  SIRF_MSG_SSB_MAX_MESSAGE_LEN
#define SIRF_MSG_GPRS_AT_COMMAND_MAX_STRING (96)
#define SIRF_MSG_GPRS_AT_COMMAND_MAX_IP_STRING (128)

/*-----------------------------------------------------------------------------
 * Message definitions
 *----------------------------------------------------------------------------*/
/* Command messages starting with '+' are prefixed with P */
#define SIRF_MSG_GPRS_AT_COMMAND_PCGMI                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x01, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_PCGMM                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x02, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_PCMEE                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x03, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_PIFC                       SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x04, 0)

/* Network Commands */
#define SIRF_MSG_GPRS_AT_COMMAND_PWOPEN                     SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x10, 0)
/* +WIPCFG commands */
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_STOP_TCPIP         SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x11, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_START_TCPIP        SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x11, 1)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_CONFIGURE_TCPIP    SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x11, 2)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_TCPIP_VERSION      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x11, 3)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_STORE_TCPIP_CFG    SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x11, 4)
/* +WIPBR bearer commands */
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_CLOSE               SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x12, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_OPEN                SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x12, 1)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_SET                 SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x12, 2)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_GET                 SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x12, 3)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_START               SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x12, 4)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_STOP                SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x12, 5)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_CFG_MANAGEMENT      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x12, 6)
/* +WIPCREATE commands */
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_UDP             SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x13, 1)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_TCP_CLIENT      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x13, 2)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_TCP_SERVER      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x13, 3)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_FTP             SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x13, 4)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_HTTP_CLIENT     SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x13, 5)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_SMTP_CLIENT     SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x13, 6)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_POP3_CLIENT     SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x13, 7)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPDATA                   SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x14, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCLOSE                  SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x15, 0)

/* SMS Messaging */
#define SIRF_MSG_GPRS_AT_COMMAND_PCSMS                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x16, 0)

#define SIRF_MSG_GPRS_AT_COMMAND_PCMGF                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x16, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_PCPMS                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x17, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_PCNMI                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x19, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_PCNMA                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x1A, 0)

#define SIRF_MSG_GPRS_AT_COMMAND_PCMGS                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x1B, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_PCMGW                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x1C, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_PCMSS                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x1D, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_PCMGL                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x1E, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_PCMGR                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x1F, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_PCMGD                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x20, 0)

/* Specific AT Commands */
/* Cell Environment Description */
#define SIRF_MSG_GPRS_AT_COMMAND_PCCED                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x30, 0)
/* Get IMSI: International Mobile Subscriber Identity */
#define SIRF_MSG_GPRS_AT_COMMAND_PCIMI                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x31, 0)

/* Commands with no prefix */
#define SIRF_MSG_GPRS_AT_COMMAND_E                          SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x40, 0)

/* Responses */
#define SIRF_MSG_GPRS_AT_COMMAND_OK                         SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x80, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_STRING                     SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x81, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_ERROR                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x82, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_CONNECT                    SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x83, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_PCME_ERROR                 SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x84, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPREADY                  SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x85, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPPEERCLOSE              SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x86, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_PCCED_RESPONSE             SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x87, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_PCSQ                       SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x88, 0)

#define SIRF_MSG_GPRS_AT_COMMAND_PCMTI                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x90, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_PCMT                       SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x91, 0)

/* At command server specific messages, indication of state or errors */
#define SIRF_MSG_GPRS_AT_COMMAND_SERVER_ERROR               SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0xB0, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_SERVER_SET_TIMEOUT         SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0xB1, 0)
#define SIRF_MSG_GPRS_AT_COMMAND_SERVER_GET_TIMEOUT         SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0xB2, 0)


/*-----------------------------------------------------------------------------
 * SIRF_MSG_GPRS_AT_COMMAND_PCGMI (SIRF_LC_GPRS_AT_COMMAND, 0x01, 0)
 * Manufacturer Identification
 *----------------------------------------------------------------------------*/

/* The response to the +CGMI is a string and then OK */
#define SIRF_MSG_GPRS_AT_COMMAND_PCGMI_RESPONSES (2)

#define SIRF_MSG_GPRS_AT_COMMAND_PCGMI_FORMAT "+CGMI"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PCGMI_tag
{
   tSIRF_UINT32 reserved;
} tSIRF_MSG_GPRS_AT_COMMAND_PCGMI;

/*-----------------------------------------------------------------------------
 * SIRF_MSG_GPRS_AT_COMMAND_PCGMM (SIRF_LC_GPRS_AT_COMMAND, 0x01, 0)
 * Request Model Identification
 *----------------------------------------------------------------------------*/

/* The response to the +CGMM is a string and then OK */
#define SIRF_MSG_GPRS_AT_COMMAND_PCGMM_RESPONSES (2)

#define SIRF_MSG_GPRS_AT_COMMAND_PCGMM_FORMAT "+CGMM"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PCGMM_tag
{
   tSIRF_UINT32 reserved;
} tSIRF_MSG_GPRS_AT_COMMAND_PCGMM;

/*------------------------------------------------------------------------------
 * SIRF_MSG_GPRS_AT_COMMAND_PCMEE                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x03, 0)
 *----------------------------------------------------------------------------*/

/* The response to the +CGMM is a OK */
#define SIRF_MSG_GPRS_AT_COMMAND_PCMEE_RESPONSES (1)

#define SIRF_MSG_GPRS_AT_COMMAND_PCMEE_MODE_OFF (0)
#define SIRF_MSG_GPRS_AT_COMMAND_PCMEE_MODE_ON  (1)
#define SIRF_MSG_GPRS_AT_COMMAND_PCMEE_FORMAT "+CMEE=%d"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PCMEE_tag
{
   tSIRF_UINT32 mode;
} tSIRF_MSG_GPRS_AT_COMMAND_PCMEE;

/*------------------------------------------------------------------------------
 * SIRF_MSG_GPRS_AT_COMMAND_PIFC                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x04, 0)
 *----------------------------------------------------------------------------*/

/* The response to the +CGMM is a OK */
#define SIRF_MSG_GPRS_AT_COMMAND_PIFC_RESPONSES (1)

#define SIRF_MSG_GPRS_AT_COMMAND_PIFC_DCE_BY_DTE_NONE (0)
#define SIRF_MSG_GPRS_AT_COMMAND_PIFC_DCE_BY_DTE_RTS  (2)
#define SIRF_MSG_GPRS_AT_COMMAND_PIFC_DTE_BY_DCE_NONE (0)
#define SIRF_MSG_GPRS_AT_COMMAND_PIFC_DTE_BY_DCE_CTS  (2)

#define SIRF_MSG_GPRS_AT_COMMAND_PIFC_FORMAT "+IFC=%d,%d"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PIFC_tag
{
   tSIRF_UINT32 dce_by_dte;
   tSIRF_UINT32 dte_by_dce;
} tSIRF_MSG_GPRS_AT_COMMAND_PIFC;

/*------------------------------------------------------------------------------
 * SIRF_MSG_GPRS_AT_COMMAND_PWOPEN                     SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x10, 0)
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_PWOPEN_RESPONSES (1)
#define SIRF_MSG_GPRS_AT_COMMAND_PWOPEN_FORMAT "+WOPEN=%d"

/* Close the TCP/IP Stack */
#define SIRF_MSG_GPRS_AT_COMMAND_PWOPEN_STATE_CLOSE (0)
/* Open the TCP/IP Stack */
#define SIRF_MSG_GPRS_AT_COMMAND_PWOPEN_STATE_OPEN  (1)

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWOPEN_tag
{
   tSIRF_UINT32 state;
} tSIRF_MSG_GPRS_AT_COMMAND_PWOPEN;

/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_STOP_TCPIP         SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x11, 0)
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_STOP_TCPIP_RESPONSES (1)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_STOP_TCPIP_FORMAT "+WIPCFG=0 "

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_STOP_TCPIP_tag
{
   tSIRF_UINT32 reserved;
} tSIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_STOP_TCPIP;

/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_START_TCPIP        SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x11, 1)
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_START_TCPIP_RESPONSES (1)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_START_TCPIP_FORMAT "+WIPCFG=1"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_START_TCPIP_tag
{
   tSIRF_UINT32 reserved;
} tSIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_START_TCPIP;

/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_CONFIGURE_TCPIP    SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x11, 2)
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_CONFIGURE_TCPIP_RESPONSES (1)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_CONFIGURE_TCPIP_FORMAT "+WIPCFG=2,%d,%d"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_CONFIGURE_TCPIP_tag
{
   tSIRF_UINT32 opt_num;
   tSIRF_UINT32 value;
} tSIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_CONFIGURE_TCPIP;

/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_TCPIP_VERSION      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x11, 3)
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_TCPIP_VERSION_RESPONSES (3)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_TCPIP_VERSION_FORMAT "+WIPCFG=3"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_TCPIP_VERSION_tag
{
   tSIRF_UINT32 reserved;
} tSIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_TCPIP_VERSION;

/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_STORE_TCPIP_CFG    SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x11, 4)
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_STORE_TCPIP_CFG_RESPONSES (1)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_STORE_TCPIP_CFG_FORMAT "+WIPCFG=4,%d"

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_STORE_TCPIP_CFG_FREE  (0)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_STORE_TCPIP_CFG_STORE (1)

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_STORE_TCPIP_CFG_tag
{
   tSIRF_UINT32 mode;
} tSIRF_MSG_GPRS_AT_COMMAND_PWIPCFG_STORE_TCPIP_CFG;

/*==============================================================================
 * +WIPBR
 *============================================================================*/
/*----------------------------------------------------------------------------*/
/* +WIPBR BID values */
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_BID_UART1             (1)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_BID_UART2             (2)
/* 3 N/A */
/* 4 N/A */
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_BID_GSM               (5)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_BID_GPRS              (6)
/* 11..14 CMUX port over UART1 */
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_BID_CMUX_UART1_PORT1 (11)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_BID_CMUX_UART1_PORT2 (12)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_BID_CMUX_UART1_PORT3 (13)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_BID_CMUX_UART1_PORT4 (14)

/* 21..24 CMUX port over UART2 */
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_BID_CMUX_UART2_PORT1 (21)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_BID_CMUX_UART2_PORT2 (22)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_BID_CMUX_UART2_PORT3 (23)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_BID_CMUX_UART2_PORT4 (24)

/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_CLOSE               SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x12, 0)
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_CLOSE_RESPONSES (1)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_CLOSE_FORMAT "+WIPBR=0,%d"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_CLOSE_tag
{
   tSIRF_UINT32 bid;
} tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_CLOSE;

/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_OPEN                SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x12, 1)
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_OPEN_RESPONSES (1)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_OPEN_FORMAT "+WIPBR=1,%d"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_OPEN_tag
{
   tSIRF_UINT32 bid;
} tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_OPEN;

/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_SET                 SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x12, 2)
 *----------------------------------------------------------------------------*/

/* OPT NUM: Set the APN, string, max length 96 characters */
#define WIP_BOPT_GPRS_APN (11)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_SET_RESPONSES (1)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_SET_FORMAT "+WIPBR=2,%d,%d,%s"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_SET_tag
{
   tSIRF_UINT32 bid;
   tSIRF_UINT32 opt_num;
   tSIRF_CHAR   value[SIRF_MSG_GPRS_AT_COMMAND_MAX_STRING];
} tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_SET;

/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_GET                 SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x12, 3)
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_GET_RESPONSES (2)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_GET_FORMAT "+WIPBR=3,%d,%d"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_GET_tag
{
   tSIRF_UINT32 bid;
   tSIRF_UINT32 opt_num;
} tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_GET;

/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_START               SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x12, 4)
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_START_RESPONSES (1)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_START_FORMAT "+WIPBR=4,%d,%d,%s,%s,%s"

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_START_MODE_CLIENT (0)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_START_MODE_SERVER (1)


typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_START_tag
{
   tSIRF_UINT32 bid;
   tSIRF_UINT32 mode;
   tSIRF_CHAR   login[SIRF_MSG_GPRS_AT_COMMAND_MAX_STRING];
   tSIRF_CHAR   password[SIRF_MSG_GPRS_AT_COMMAND_MAX_STRING];
   tSIRF_CHAR   caller_identity[SIRF_MSG_GPRS_AT_COMMAND_MAX_STRING];
} tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_START;

/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_STOP                SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x12, 5)
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_STOP_RESPONSES (1)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_STOP_FORMAT "+WIPBR=5%d"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_STOP_tag
{
   tSIRF_UINT32 bid;
} tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_STOP;

/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_CFG_MANAGEMENT      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x12, 6)
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_CFG_MANAGEMENT_RESPONSES (1)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPBR_CFG_MANAGEMENT_FORMAT "+WIPBR=6,%d,%d"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_CFG_MANAGEMENT_tag
{
   tSIRF_UINT32 bid;
   tSIRF_UINT32 mode;
} tSIRF_MSG_GPRS_AT_COMMAND_PWIPBR_CFG_MANAGEMENT;

/*==============================================================================
 * +WIPCREATE
 *============================================================================*/
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_PEER_IP_MAX_LEN (32)

/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_UDP             SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x12, 1)
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_UDP_RESPONSES (1)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_UDP_FORMAT "+WIPCREATE=1,%d,%d,%s,%d"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_UDP_tag
{
   tSIRF_UINT32 communication_index;
   tSIRF_UINT32 local_port;
   tSIRF_CHAR   peer_ip[SIRF_MSG_GPRS_AT_COMMAND_MAX_IP_STRING];
   tSIRF_UINT32 peer_port;
} tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_UDP;

/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_TCP_CLIENT      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x12, 2)
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_TCP_CLIENT_RESPONSES (1)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_TCP_CLIENT_FORMAT "+WIPCREATE=2,%d,%s,%d"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_TCP_CLIENT_tag
{
   tSIRF_UINT32 communication_index;
   tSIRF_CHAR   peer_ip[SIRF_MSG_GPRS_AT_COMMAND_MAX_IP_STRING];
   tSIRF_UINT32 peer_port;
} tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_TCP_CLIENT;

/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_TCP_SERVER      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x12, 3)
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_TCP_SERVER_RESPONSES (1)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_TCP_SERVER_FORMAT "+WIPCREATE=3,%d,%d,%d,%d"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_TCP_SERVER_tag
{
   tSIRF_UINT32 server_index;
   tSIRF_UINT32 local_port;
   tSIRF_UINT32 idx_from;
   tSIRF_UINT32 idx_to;
} tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_TCP_SERVER;

/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_FTP             SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x12, 4)
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_FTP_RESPONSES (1)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_FTP_FORMAT "+WIPCREATE=4,%d,%s,%d,%s,%s,%s"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_FTP_tag
{
   tSIRF_UINT32 index;
   tSIRF_CHAR   server[SIRF_MSG_GPRS_AT_COMMAND_MAX_IP_STRING];
   tSIRF_UINT32 peer_port;
   tSIRF_CHAR   username[SIRF_MSG_GPRS_AT_COMMAND_MAX_STRING];
   tSIRF_CHAR   password[SIRF_MSG_GPRS_AT_COMMAND_MAX_STRING];
   tSIRF_CHAR   account[SIRF_MSG_GPRS_AT_COMMAND_MAX_STRING];
} tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_FTP;

/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_HTTP_CLIENT     SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x12, 5)
 *----------------------------------------------------------------------------*/
#define SIRF_MSG_GPRS_AT_COMMAND_MAX_HEADER_STRING (256)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_HTTP_CLIENT_RESPONSES (1)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_HTTP_CLIENT_FORMAT "+WIPCREATE=5,%d,%s,%d,%s,%s,%s"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_HTTP_CLIENT_tag
{
   tSIRF_UINT32 index;
   tSIRF_CHAR   server[SIRF_MSG_GPRS_AT_COMMAND_MAX_IP_STRING];
   tSIRF_UINT32 peer_port;
   tSIRF_CHAR   username[SIRF_MSG_GPRS_AT_COMMAND_MAX_STRING];
   tSIRF_CHAR   password[SIRF_MSG_GPRS_AT_COMMAND_MAX_STRING];
   tSIRF_CHAR   header_list[SIRF_MSG_GPRS_AT_COMMAND_MAX_HEADER_STRING];
} tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_HTTP_CLIENT;

/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_SMTP_CLIENT     SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x12, 6)
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_SMTP_CLIENT_RESPONSES (1)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_SMTP_CLIENT_FORMAT "+WIPCREATE=6,%d,%s,%d,%s,%s,"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_SMTP_CLIENT_tag
{
   tSIRF_UINT32 index;
   tSIRF_CHAR   server[SIRF_MSG_GPRS_AT_COMMAND_MAX_IP_STRING];
   tSIRF_UINT32 peer_port;
   tSIRF_CHAR   username[SIRF_MSG_GPRS_AT_COMMAND_MAX_STRING];
   tSIRF_CHAR   password[SIRF_MSG_GPRS_AT_COMMAND_MAX_STRING];
} tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_SMTP_CLIENT;

/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_POP3_CLIENT     SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x12, 7)
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_POP3_CLIENT_RESPONSES (1)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_POP3_CLIENT_FORMAT "+WIPCREATE=7,%d,%s,%d,%s,%s,"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_POP3_CLIENT_tag
{
   tSIRF_UINT32 index;
   tSIRF_CHAR   server[SIRF_MSG_GPRS_AT_COMMAND_MAX_IP_STRING];
   tSIRF_UINT32 peer_port;
   tSIRF_CHAR   username[SIRF_MSG_GPRS_AT_COMMAND_MAX_STRING];
   tSIRF_CHAR   password[SIRF_MSG_GPRS_AT_COMMAND_MAX_STRING];
} tSIRF_MSG_GPRS_AT_COMMAND_PWIPCREATE_POP3_CLIENT;


/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PWIPDATA                   SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x13, 0)
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPDATA_RESPONSES (1)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPDATA_PROTOCOL_UDP        (1)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPDATA_PROTOCOL_TCP_CLIENT (2)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPDATA_MODE_UNMAP                 (0)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPDATA_MODE_CONTINOUS             (1)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPDATA_MODE_CONTINOUS_TRANSPARENT (2)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPDATA_FORMAT "+WIPDATA=%d,%d,%d"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWIPDATA_tag
{
   tSIRF_UINT32 protocol;
   tSIRF_UINT32 idx; /* socket identifier */
   tSIRF_UINT32 mode;
} tSIRF_MSG_GPRS_AT_COMMAND_PWIPDATA;


/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PWIPCLOSE                  SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x15, 0)
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCLOSE_RESPONSES (1)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPCLOSE_FORMAT "+WIPCLOSE=%d,%d"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWIPCLOSE_tag
{
   tSIRF_UINT32 protocol;
   tSIRF_UINT32 idx;
} tSIRF_MSG_GPRS_AT_COMMAND_PWIPCLOSE;

/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PCSMS                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x16, 0)
 * Select Message Service
 *----------------------------------------------------------------------------*/
#define SIRF_MSG_GPRS_AT_COMMAND_PCSMS_RESPONSES (1)

#define SIRF_MSG_GPRS_AT_COMMAND_PCSMS_FORMAT "+CSMS=%d"

#define SIRF_MSG_GPRS_AT_COMMAND_PCSMS_SERVICE_PHASE2      0 /* SMS AT commands are compatible with GSM 07.05 Phase 2 version 4.7.0. */
#define SIRF_MSG_GPRS_AT_COMMAND_PCSMS_SERVICE_PHASE2_PLUS 1 /* SMS AT commands are compatible with GSM 07.05 Phase 2 + version . */

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PCSMS_tag
{
   tSIRF_UINT32 service; /* 0 or 1 */
} tSIRF_MSG_GPRS_AT_COMMAND_PCSMS;

/*------------------------------------------------------------------------------
  * #define SIRF_MSG_GPRS_AT_COMMAND_PCNMA                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x17, 0)
  * New Message Acknowledgement
 *----------------------------------------------------------------------------*/
#define SIRF_MSG_GPRS_AT_COMMAND_PCNMA_RESPONSES (1)

#define SIRF_MSG_GPRS_AT_COMMAND_PCNMA_FORMAT "+CNMA"

/* The fields are unused in TEXT mode but are used in PDU mode */
typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PCNMA_tag
{
   tSIRF_UINT32 ack_nak; /* 0 or 1 */
   tSIRF_UINT32 length;
} tSIRF_MSG_GPRS_AT_COMMAND_PCNMA;

/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PCMGS                       SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x15, 0)
 *----------------------------------------------------------------------------*/
#define SIRF_MSG_GPRS_AT_COMMAND_PCMGS_RESPONSES (1)

/* Ctrl-z is ASCII 26 or ^Z */
#define SIRF_MSG_GPRS_AT_COMMAND_PCMGS_FORMAT "+CMGS=%s,%s,\n%s\x1a"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PCMGS_tag
{
   tSIRF_CHAR da;
   tSIRF_CHAR ioda;
   tSIRF_CHAR text_message;
} tSIRF_MSG_GPRS_AT_COMMAND_PCMGS;

/* SMS Messaging */
/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PCMGF                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x16, 0)
 *----------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PCPMS                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x17, 0)
 *----------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PCNMI                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x19, 0)
 *----------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PCMGS                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x1B, 0)
 *----------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PCMGW                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x1C, 0)
 *----------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PCMSS                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x1D, 0)
 *----------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PCMGL                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x1E, 0)
 *----------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PCMGR                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x1F, 0)
 *----------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PCMGD                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x20, 0)
 *----------------------------------------------------------------------------*/

/* Specific AT Commands */
/* Cell Environment Description */
/*-----------------------------------------------------------------------------
 * #define SIRF_MSG_GPRS_AT_COMMAND_PCCED                       SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x20, 0)
 * Cell Environment Description
 *----------------------------------------------------------------------------*/
#define SIRF_MSG_GPRS_AT_COMMAND_PCCED_FORMAT "+CCED=%d,%d,%s"

#define SIRF_MSG_GPRS_AT_COMMAND_PCCED_MODE_ONCE       (0) /* One shot requested */
#define SIRF_MSG_GPRS_AT_COMMAND_PCCED_MODE_AUTO       (1) /* Automatic shots requested */
#define SIRF_MSG_GPRS_AT_COMMAND_PCCED_MODE_AUTO_STOP  (2) /* Stop automatic shots */

#define SIRF_MSG_GPRS_AT_COMMAND_PCCED_DUMP_MAIN     (0x1) /* Main Cell: */
#define SIRF_MSG_GPRS_AT_COMMAND_PCCED_DUMP_NEIGHBOR (0x2) /* Neighbor1 to Neighbor6: */
#define SIRF_MSG_GPRS_AT_COMMAND_PCCED_DUMP_TIMING   (0x4) /* Main Cell Timing Advance */
#define SIRF_MSG_GPRS_AT_COMMAND_PCCED_DUMP_RSSI     (0x8) /* Main cell RSSI indications (RxLev) in a range from 0 to 31 */
#define SIRF_MSG_GPRS_AT_COMMAND_PCCED_DUMP_ALL     (0x10) /* <requested dump>=1 + <requested dump>=2 + <requested dump>=4 + Neighbor1 to */

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PCCED_tag
{
   tSIRF_UINT32 mode;
   tSIRF_UINT32 requested_dump;
   tSIRF_CHAR   csq_step[3]; /* Since this is an optional parameter a string
                              * must be used.  Two digit value 0-31 plus NULL */
} tSIRF_MSG_GPRS_AT_COMMAND_PCCED;

/* a +CCED: <value>...<value n> followed by OK or if the requested dump is 8
 * then the response is +csq:<rssi>,99 OK,
 * if the Requested dump is 0x10 then 3 responses follow, this needs to be
 * checked for independently.
 * Also if the mode is AUTO then simply an OK is expected, followed by
 * unsolicited responses about once per second. */
#define SIRF_MSG_GPRS_AT_COMMAND_PCCED_MIN_RESPONSES (2)

/*-----------------------------------------------------------------------------
 *#define SIRF_MSG_GPRS_AT_COMMAND_PCCED_RESPONSE             SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x20, 1)
 * Cell Environment Description Response messages
 *
 * Since this message response depends on the reqest, the parsing of this
 * message has to be done by the requester, so a string is passed in, but
 * the structures for the rest of the parts are defined here for convienence
 *----------------------------------------------------------------------------*/
#define SIRF_MSG_GPRS_AT_COMMAND_PCCED_RESPONSE_FORMAT "+CCED: %s"
#define SIRF_MSG_GPRS_AT_COMMAND_PCCED_RESPONSE_MAX_STRING (512)

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PCCED_RESPONSE_MAIN_CELL_tag
{
   tSIRF_UINT32 mcc;
   tSIRF_UINT32 mnc;
   tSIRF_UINT32 lac;
   tSIRF_UINT32 ci; /* May not be available */
   tSIRF_UINT32 bsic;
   tSIRF_UINT32 bcch_freq;
   tSIRF_UINT32 rx_lev;
   tSIRF_UINT32 rx_lev_full;
   tSIRF_UINT32 rx_lev_sub;
   tSIRF_UINT32 rx_qual;
   tSIRF_UINT32 rx_qual_full;
   tSIRF_UINT32 rx_qual_sub;
   tSIRF_UINT32 idle_ts;

} tSIRF_MSG_GPRS_AT_COMMAND_PCCED_RESPONSE_MAIN_CELL;

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PCCED_RESPONSE_NEIGHBOR_tag
{
   tSIRF_UINT32 mcc;
   tSIRF_UINT32 mnc;
   tSIRF_UINT32 lac;
   tSIRF_UINT32 ci;
   tSIRF_UINT32 bsic;
   tSIRF_UINT32 bcch_freq;
   tSIRF_UINT32 rx_lev;
} tSIRF_MSG_GPRS_AT_COMMAND_PCCED_RESPONSE_NEIGHBOR;

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PCCED_RESPONSE_STRING_tag
{
   tSIRF_CHAR string[SIRF_MSG_GPRS_AT_COMMAND_PCCED_RESPONSE_MAX_STRING];
} tSIRF_MSG_GPRS_AT_COMMAND_PCCED_RESPONSE_STRING;

/*-----------------------------------------------------------------------------
 *SIRF_MSG_GPRS_AT_COMMAND_PCIMI SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x21, 1)
 * Get IMSI: International Mobile Subscriber Identity *
 *----------------------------------------------------------------------------*/
#define SIRF_MSG_GPRS_AT_COMMAND_PCIMI_FORMAT "+CIMI"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PCIMI_tag
{
   tSIRF_UINT32 reserved;
} tSIRF_MSG_GPRS_AT_COMMAND_PCIMI;

/* Response 1 is a string, response 2 is OK */
#define SIRF_MSG_GPRS_AT_COMMAND_PCIMI_RESPONSES (2)

/*-----------------------------------------------------------------------------
 * SIRF_MSG_GPRS_AT_COMMAND_E SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x40, 0)
 *
 * Echo Characters
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_E_FORMAT "E%d"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_E_tag
{
   tSIRF_UINT32 echo; /**< 0: Do not Echo charachers; 1: Echo characters */
} tSIRF_MSG_GPRS_AT_COMMAND_E;

/* The response to the ATE%d is OK */
#define SIRF_MSG_GPRS_AT_COMMAND_E_RESPONSES (1)

/*-----------------------------------------------------------------------------
 * SIRF_MSG_GPRS_AT_COMMAND_OK (SIRF_LC_GPRS_AT_COMMAND, 0x80, 0)
 * OK response
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_OK_FORMAT "OK"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_OK_tag
{
   tSIRF_UINT32 reserved;
} tSIRF_MSG_GPRS_AT_COMMAND_OK;

/*-----------------------------------------------------------------------------
 * SIRF_MSG_GPRS_AT_COMMAND_STRING (SIRF_LC_GPRS_AT_COMMAND, 0xFF, 0)
 * String response
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_STRING_FORMAT "%s"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_STRING_tag
{
   tSIRF_CHAR string[SIRF_MSG_GPRS_AT_COMMAND_MAX_MESSAGE_LEN];
} tSIRF_MSG_GPRS_AT_COMMAND_STRING;

/*------------------------------------------------------------------------------
 * SIRF_MSG_GPRS_AT_COMMAND_ERROR                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x82, 0)
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_ERROR_FORMAT "ERROR"
typedef struct tSIRF_MSG_GPRS_AT_COMMAND_ERROR_tag
{
   tSIRF_UINT32 reserved;
} tSIRF_MSG_GPRS_AT_COMMAND_ERROR;

/*------------------------------------------------------------------------------
 * SIRF_MSG_GPRS_AT_COMMAND_CONNECT                      SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x83, 0)
 *----------------------------------------------------------------------------*/

#define SIRF_MSG_GPRS_AT_COMMAND_CONNECT_FORMAT "CONNECT"
typedef struct tSIRF_MSG_GPRS_AT_COMMAND_CONNECT_tag
{
   tSIRF_UINT32 reserved;
} tSIRF_MSG_GPRS_AT_COMMAND_CONNECT;


/*------------------------------------------------------------------------------
 * SIRF_MSG_GPRS_AT_COMMAND_PCME_ERROR                  SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x84, 0)
 *----------------------------------------------------------------------------*/

#define PCME_ERROR_INVALID_OPTION               (800) /* invalid option */
#define PCME_ERROR_INVALID_OPTION_VALUE         (801) /* invalid option value */
#define PCME_ERROR_NOT_ENOUGH_MEMORY            (802) /* not enough memory */
#define PCME_ERROR_WIP_BAD_STATE                (803) /* operation not allowed in the current WIP stack state */
#define PCME_ERROR_DEVICE_ALREADY_OPEN          (804) /* device already open */
#define PCME_ERROR_NETWORK_IF_NA                (805) /* network interface not available */
#define PCME_ERROR_NOT_ALLOED                   (806) /* operation not allowed on the considered bearer */
#define PCME_ERROR_BEARER_LINE_BUSY             (807) /* bearer connection failure : line busy */
#define PCME_ERROR_BEARER_NO_ANSER              (808) /* bearer connection failure : no answer */
#define PCME_ERROR_BEARER_NO_CARRIER            (809) /* bearer connection failure : no carrier */
#define PCME_ERROR_BEARER_NO_SIM                (810) /* bearer connection failure : no SIM card present */
#define PCME_ERROR_BEARER_SIM_NOT_READY         (811) /* bearer connection failure : SIM not ready (no pin code entered, …) */
#define PCME_ERROR_BEARER_GPRS_NETWORK_FAILURE  (812) /* bearer connection failure : GPRS network failure */
#define PCME_ERROR_BEARER_PPP_LCP_NEG_FAILURE   (813) /* bearer connection failure : PPP LCP negotiation failed */
#define PCME_ERROR_BEARER_PPP_AUTH_FAILED       (814) /* bearer connection failure : PPP authentication failed */
#define PCME_ERROR_BEARER_PPP_IPCP_NEG_FAILURE  (815) /* bearer connection failure : PPP IPCP negotiation failed */
#define PCME_ERROR_BEARER_PPP_PEER_TERMINATED   (816) /* bearer connection failure : PPP peer terminates session */
#define PCME_ERROR_BEARER_PPP_PEER_NO_ANSER     (817) /* bearer connection failure : PPP peer does not answer to echo request */
#define PCME_ERROR_INCOMING_CALL_REFUSED        (818) /* incoming call refused */
#define PCME_ERROR_ON_PING_CHANNEL              (819) /* error on Ping channel */
#define PCME_ERROR_WRITING_FLASH                (820) /* error writing configuration in FLASH memory */
#define PCME_ERROR_READING_FLASH                (821) /* error reading configuration in FLASH memory */
#define PCME_ERROR_RESERVED_BLOCK1_BEGIN        (822) /* 822-829 reserved for future use */
#define PCME_ERROR_RESERVED_BLOCK1_END          (829) /* 822-829 reserved for future use */
#define PCME_ERROR_BAD_INDEX                    (830) /* bad index */
#define PCME_ERROR_BAD_STATE                    (831) /* bad state */
#define PCME_ERROR_BAD_PORT                     (832) /* bad port number */
#define PCME_ERROR_BAD_PORT_STATE               (833) /* bad port state */
#define PCME_ERROR_NOT_IMPLEMENTED              (834) /* not implemented */
#define PCME_ERROR_OPTION_NOT_SUPPORTED         (835) /* option not supported */
#define PCME_ERROR_MEMORY_ALLOCAITON_ERROR      (836) /* memory allocation error */
#define PCME_ERROR_BAD_PROTOCOL                 (837) /* bad protocol */
#define PCME_ERROR_NO_MORE_FREE_SOCKETS         (838) /* no more free socket */
#define PCME_ERROR_DURING_CHANNEL_CREATION      (839) /* error during channel creation */
#define PCME_ERROR_ALREADY_ACTIVE               (840) /* UDP/TCP socket or FTP/HTTP/SMTP/POP3 session is already active */
#define PCME_ERROR_PEER_CLOSED                  (841) /* peer closed */
#define PCME_ERROR_DEST_HOST_UNREACHABLE        (842) /* destination host unreachable ( whether host unreachable, Network unreachable, response timeout) */
#define PCME_ERROR_CONNECTION_RESET_BY_PEER     (843) /* connection reset by peer */
#define PCME_ERROR_STACK_ALREADY_IN_USE         (844) /* stack already in use */
#define PCME_ERROR_ALREADY_RESERVED             (845) /* attempt is made to reserve/create a client socket which is already reserved/opened by TCP server/client */
#define PCME_ERROR_FCM_SUBSCRIPTION_FAILURE     (846) /* internal error: FCM subscription failure */
#define PCME_ERROR_RESERVED_BLOCK2_START        (847) /*-849 reserved for future use */
#define PCME_ERROR_RESERVED_BLOCK2_END          (849) /*-849 reserved for future use */
#define PCME_ERROR_UNKNOWN_REASON               (850) /* unknown reason */
#define PCME_ERROR_RESERVED_BLOCK3_START        (851) /* 851-859 reserved for future use */
#define PCME_ERROR_RESERVED_BLOCK3_END          (859) /* 851-859 reserved for future use */
#define PCME_ERROR_PROTOCOL_UNDEFINED           (860) /* protocol undefined or internal error */
#define PCME_ERROR_USERNAME_REJECTED            (861) /* username rejected by server */
#define PCME_ERROR_PASSWORD_REJECTED            (862) /* password rejected by server */
#define PCME_ERROR_DELETE_ERROR                 (863) /* delete error */
#define PCME_ERROR_LIST                         (864) /* list error */
#define PCME_ERROR_AUTHENTICATION               (865) /* authentication error */
#define PCME_ERROR_SERVER_NOT_READY             (866) /* server not ready error */
#define PCME_ERROR_POP3_EMAIL_RETREIVE          (867) /* POP3 email retrieving error */
#define PCME_ERROR_POP3_EMAIL_SIZE              (868) /* POP3 email size error */
#define PCME_ERROR_RESERVED_BLOCK4_START        (869) /* 869-879 reserved for future use */
#define PCME_ERROR_RESERVED_BLOCK4_END          (879) /* 869-879 reserved for future use */
#define PCME_ERROR_SMTP_SENDER_EMAIL_REJECTED   (880) /* SMTP sender email address rejected by server */
#define PCME_ERROR_SMTP_RECIPIENT_EMAIL_REJECTED (881) /* SMTP recipient email address rejected by server */
#define PCME_ERROR_SMTP_CC_RECIPIENT_EMAIL_REJECTED (882) /* SMTP CC recipient email address rejected by server */
#define PCME_ERROR_SMTP_BCC_RECIPIENT_EMAIL_REJECTED (883) /* SMTP BCC recipient email address rejected by server */
#define PCME_ERROR_SMTP_EMAIL_BODY_REJECTED     (884) /* SMTP email body send request rejected by server */


#define SIRF_MSG_GPRS_AT_COMMAND_PCME_ERROR_FORMAT "+CME ERROR: %d"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PCME_ERROR_tag
{
   tSIRF_UINT32 error;
} tSIRF_MSG_GPRS_AT_COMMAND_PCME_ERROR;

/*-----------------------------------------------------------------------------
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPREADY                  SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x85, 0)
 *----------------------------------------------------------------------------*/
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPREADY_MODE_UDP         (1)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPREADY_MODE_TCP_CLIENT  (2)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPREADY_MODE_TCP_SERVER  (3)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPREADY_MODE_FTP         (4)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPREADY_MODE_HTTP_CLIENT (5)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPREADY_MODE_SMTP_CLIENT (6)
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPREADY_MODE_POP3_CLIENT (7)

#define SIRF_MSG_GPRS_AT_COMMAND_PWIPREADY_FORMAT "+WIPREADY: %d,%d"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWIPREADY_tag
{
   tSIRF_UINT32 mode; /**< Same mode as in the +WIPCREATE message */
   tSIRF_UINT32 communication_index;
} tSIRF_MSG_GPRS_AT_COMMAND_PWIPREADY;

/*-----------------------------------------------------------------------------
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPPEERCLOSE              SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x85, 0)
*-----------------------------------------------------------------------------*/
#define SIRF_MSG_GPRS_AT_COMMAND_PWIPPEERCLOSE_FORMAT "+WIPPEERCLOSE: %d,%d"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PWIPPEERCLOSE_tag
{
   tSIRF_UINT32 mode; /**< Same mode in the +WIPCREATE and +WIPREADY message,
                       * Use SIRF_MSG_GPRS_AT_COMMAND_PWIPREADY_MODE_XXX */
   tSIRF_UINT32 communication_index;
} tSIRF_MSG_GPRS_AT_COMMAND_PWIPPEERCLOSE;

/*-----------------------------------------------------------------------------
#define SIRF_MSG_GPRS_AT_COMMAND_PCSQ                       SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0x88, 0)
 *----------------------------------------------------------------------------*/
#define SIRF_MSG_GPRS_AT_COMMAND_PCSQ_FORMAT "+CSQ: %d,%d"

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_PCSQ_tag
{
   tSIRF_UINT32 rssi; /**< Same mode in the +WIPCREATE and +WIPREADY message,
                       * Use SIRF_MSG_GPRS_AT_COMMAND_PWIPREADY_MODE_XXX */
   tSIRF_UINT32 ber;  /** For this GPRS modem ber will always be 99 */
} tSIRF_MSG_GPRS_AT_COMMAND_PCSQ;

/*-----------------------------------------------------------------------------
 * SIRF_MSG_GPRS_AT_COMMAND_SERVER_ERROR               SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0xB0, 0) 
 *----------------------------------------------------------------------------*/
#define SIRF_MSG_GPRS_AT_COMMAND_SERVER_ERROR_MAX_DATA_SIZE (31)

typedef struct tSIRF_MSG_GPRS_AT_COMMAND_SERVER_ERROR_tag
{
   tSIRF_RESULT result;
   tSIRF_UINT32 data[SIRF_MSG_GPRS_AT_COMMAND_SERVER_ERROR_MAX_DATA_SIZE];
} tSIRF_MSG_GPRS_AT_COMMAND_SERVER_ERROR;

/*-----------------------------------------------------------------------------
 * SIRF_MSG_GPRS_AT_COMMAND_SERVER_SET_TIMEOUT         SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0xB1, 0)
 *----------------------------------------------------------------------------*/
typedef struct tSIRF_MSG_GPRS_AT_COMMAND_SERVER_SET_TIMEOUT_tag
{
   tSIRF_UINT32 timeout;
} tSIRF_MSG_GPRS_AT_COMMAND_SERVER_SET_TIMEOUT;

/*-----------------------------------------------------------------------------
 * SIRF_MSG_GPRS_AT_COMMAND_SERVER_GET_TIMEOUT         SIRF_MAKE_MSG_ID(SIRF_LC_GPRS_AT_COMMAND, 0xB2, 0)
 *----------------------------------------------------------------------------*/
typedef struct tSIRF_MSG_GPRS_AT_COMMAND_SERVER_GET_TIMEOUT_tag
{
   tSIRF_UINT32 timeout;
} tSIRF_MSG_GPRS_AT_COMMAND_SERVER_GET_TIMEOUT;




#endif /* SIRF_MSG_GPRS_AT_COMMAND_H_INCLUDED */

/**
 * @}
 */

