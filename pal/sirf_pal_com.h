/**
 * @addtogroup src_sirf_pal
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2006-2010 by SiRF Technology, a CSR plc Companyc.
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
 *
 *  Keywords for Perforce.  Do not modify.
 *
 *  $File: //customs/customer/Marvell-U1/sirf/Software/sirf/pal/sirf_pal_com.h $
 *
 *  $DateTime: 2011/07/29 13:26:23 $
 *
 *  $Revision: #1 $
 */

/**
 * @file   sirf_pal_com.h
 *
 * @brief  Generic I/O communication for the SiRF PAL
 *
 * This file defines the typedefs, enums, and functions utilized by the
 * various COM devices supported by the SiRFNav application.
 *
 * SiRFNav will prepare a device for communication by utilizing the following
 * pseudocode initialization sequence:
 *
 *    SIRF_PAL_COM_Create_Fcn()
 *
 *    while(control_settings_remain)
 *    {
 *       SIRF_PAL_COM_Control_Fcn()
 *    }
 *
 *    SIRF_PAL_COM_Open_Fcn()
 *
 *    At this point, SiRFNav will expect the device to be open for
 *    communication.  SiRFNav will begin utilizing Read() and Write().
 *
 */

#ifndef SIRF_PAL_COM_H_INCLUDED
#define SIRF_PAL_COM_H_INCLUDED


/* ----------------------------------------------------------------------------
 *   Included files
 * ------------------------------------------------------------------------- */
/* library includes */

/* public includes */
#include "sirf_errors.h"
#include "sirf_types.h"
/* private includes */

/* local includes */

/* Enter C naming convention */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* ----------------------------------------------------------------------------
 *   Definitions
 * ------------------------------------------------------------------------- */
#define SIRF_COM_FILE_MAX_INSTANCES       (0)
#define SIRF_COM_I2C_MAX_INSTANCES        (2)
#define SIRF_COM_SPI_MAX_INSTANCES        (1)
#define SIRF_COM_UART_MAX_INSTANCES       (4)

#define SIRF_COM_MAX_INSTANCES            (SIRF_COM_FILE_MAX_INSTANCES + \
                                           SIRF_COM_I2C_MAX_INSTANCES  + \
                                           SIRF_COM_SPI_MAX_INSTANCES  + \
                                           SIRF_COM_UART_MAX_INSTANCES)

#define SIRF_COM_DFLT_READ_WAIT_TIMEOUT   (SIRF_TIMEOUT_INFINITE)
#define SIRF_COM_DFLT_READ_TOTAL_TIMEOUT  (1000) /* in milliseconds */
#define SIRF_COM_DFLT_READ_BYTE_TIMEOUT   (SIRF_TIMEOUT_INFINITE)

/* ----------------------------------------------------------------------------
 *    Types, Enums, and Structs
 * ------------------------------------------------------------------------- */

/**
 *
 * @brief      structure for common com settings.
 *
 * long_description
 *             This typedef contains COM settings that are common across all
 *             platforms and ports.  Its use is entirely OPTIONAL; it exists
 *             to encourage simplicity and commonality across the various COM
 *             implementations.  If it is utilized, it should be declared 
 *             locally (static) to the driver in which it is used.
 *
 * @pre
 *             none.
 *
 * @warning
 *             Usage of this structure is OPTIONAL.
 */
typedef struct
{
   tSIRF_HANDLE         logical_handle;
   tSIRF_INT32          read_byte_timeout;
   tSIRF_INT32          read_total_timeout;
   tSIRF_INT32          read_wait_timeout;
   tSIRF_BOOL           port_opened;
} tSIRF_COM_COMMON_SETTINGS;

/**
 * @brief      typedef to set/get settings in SiRF PAL I/O drivers.
 *
 * long_description
 *             This typedef is utilized to set/get settings in SiRF PAL I/O
 *             drivers.
 *
 * @pre
 *             This typedef is set to a UINT32 to guarantee a consistent size
 *             across all platforms.
 *
 * @warning
 *             no warnings.
 */
typedef tSIRF_UINT32 tSIRF_COM_CONTROL; enum
{
   /* command                             parameter type                      */

   /* -------------------------------------------------------------------------
    *   General Settings
    * ---------------------------------------------------------------------- */
   SIRF_COM_CTRL_READ_BYTE_TIMEOUT = 0,/* tSIRF_INT32 - timeout in milliseconds */
   SIRF_COM_CTRL_READ_WAIT_TIMEOUT,    /* tSIRF_INT32 - timeout in milliseconds */
   SIRF_COM_CTRL_READ_TOTAL_TIMEOUT,   /* tSIRF_INT32 - timeout in milliseconds */
   SIRF_COM_CTRL_READ_BLOCKING,        /* specifies whether tracker is in bootloader state */

   /* -------------------------------------------------------------------------
    *   File-Specific Settings
    * ---------------------------------------------------------------------- */

   /* -------------------------------------------------------------------------
    *   I2C-Specific Settings
    * ---------------------------------------------------------------------- */
   SIRF_COM_CTRL_I2C_HOST_ADDR,
   SIRF_COM_CTRL_I2C_ME_ADDR,
   SIRF_COM_CTRL_I2C_RATE,
   SIRF_COM_CTRL_I2C_MODE,

   /* -------------------------------------------------------------------------
    *   SPI-Specific Settings
    * ---------------------------------------------------------------------- */
   SIRF_COM_CTRL_SPI_RATE,             /* tSIRF_UINT8                         */
   SIRF_COM_CTRL_SPI_SET_IDLE_TX,      /* tSIRF_BOOL - enable SPI idle TX     */
   SIRF_COM_CTRL_SPI_SET_WATERMARK,    /* tSIRF_BOOL - enable SPI water mark  */

   /* -------------------------------------------------------------------------
    *   UART-Specific Settings
    * ---------------------------------------------------------------------- */
   SIRF_COM_CTRL_UART_BAUD_RATE,       /* tSIRF_UINT32                        */
   SIRF_COM_CTRL_UART_FLOW_CONTROL,    /* tSIRF_BOOL                          */
   SIRF_COM_CTRL_UART_SET_DTR,         /* tSIRF_BOOL; TRUE = SET; FALSE = CLR */
   SIRF_COM_CTRL_UART_SET_RTS,         /* tSIRF_BOOL; TRUE = SET; FALSE = CLR */

   SIRF_COM_CTRL_COUNT
}  /* tSIRF_COM_CONTROL */;

/* ----------------------------------------------------------------------------
 *    Global Variables
 * ------------------------------------------------------------------------- */


/* ----------------------------------------------------------------------------
 *    Function Prototypes
 * ------------------------------------------------------------------------- */


/* Leave C naming convention */
#ifdef __cplusplus
}
#endif /*__cplusplus*/


#endif /* SIRF_PAL_COM_H_INCLUDED */

/**
 * @}
 * End of file.
 */
