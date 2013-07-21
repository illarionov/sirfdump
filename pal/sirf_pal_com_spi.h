/**
 * @addtogroup src_sirf_pal
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2009 by SiRF Technology, Inc.  All rights reserved.
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
 *  $File: //customs/customer/Marvell-U1/sirf/Software/sirf/pal/sirf_pal_com_spi.h $
 *
 *  $DateTime: 2011/07/29 13:26:23 $
 *
 *  $Revision: #1 $
 */

/**
 * @file   sirf_pal_com_spi.h
 *
 * @brief  SPI I/O communication for the SiRF PAL
 *
 * SPI I/O for the SiRF PAL.  For more information on the COM methods, see
 * sirf_pal_com.h
 *
 */

#ifndef SIRF_PAL_COM_SPI_H_INCLUDED
#define SIRF_PAL_COM_SPI_H_INCLUDED


/* ----------------------------------------------------------------------------
 *   Included files
 * ------------------------------------------------------------------------- */
/* library includes */

/* public includes */
#include "sirf_pal_com.h"
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

/* ----------------------------------------------------------------------------
 *    Types, Enums, and Structs
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 *    Global Variables
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 *    Function Prototypes
 * ------------------------------------------------------------------------- */

/**
 *    Note:       Initialize the SPI port settings.
 */
void SIRF_PAL_COM_SPI_Init(void);

tSIRF_RESULT SIRF_PAL_COM_SPI_Close(
      tSIRF_HANDLE               com_handle);

tSIRF_RESULT SIRF_PAL_COM_SPI_Control(
      tSIRF_HANDLE               com_handle,
      tSIRF_COM_CONTROL          com_control,
      void               * const com_value);

tSIRF_RESULT SIRF_PAL_COM_SPI_Create(
      tSIRF_HANDLE       * const com_handle);

tSIRF_RESULT SIRF_PAL_COM_SPI_Delete(
      tSIRF_HANDLE       * const com_handle);

tSIRF_RESULT SIRF_PAL_COM_SPI_Open(
      tSIRF_HANDLE               com_handle,
      char         const * const com_port);

tSIRF_RESULT SIRF_PAL_COM_SPI_Read(
      tSIRF_HANDLE               com_handle,
      tSIRF_UINT8        * const read_ptr,
      tSIRF_UINT32               requested_bytes_to_read,
      tSIRF_UINT32       * const actual_bytes_read);

tSIRF_RESULT SIRF_PAL_COM_SPI_Write(
      tSIRF_HANDLE               com_handle,
      tSIRF_UINT8  const * const write_ptr,
      tSIRF_UINT32               requested_bytes_to_write,
      tSIRF_UINT32       * const actual_bytes_written);

/* Leave C naming convention */
#ifdef __cplusplus
}
#endif /*__cplusplus*/


#endif /* SIRF_PAL_COM_SPI_H_INCLUDED */

/**
 * @}
 * End of file.
 */
