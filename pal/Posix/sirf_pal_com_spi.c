/**
* @addtogroup platform_src_sirf_pal_posix
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
*/

/**
* @file   sirf_pal_com_spi.c
*
* @brief  SiRF PAL SPI communications module.
*/

/* ----------------------------------------------------------------------------
*   Included Files
* ------------------------------------------------------------------------- */
/* library includes */

/* public includes */
#include "sirf_types.h"
#include "sirf_pal.h"

/***************************************************************************
*  Global Variables
***************************************************************************/

/* ----------------------------------------------------------------------------
*   Preprocessor Definitions
* ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
*   Types
* ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 *    Local Variables
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
*   Local Functions
* ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
*   Functions
* -------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 *
 *    Function:   SIRF_PAL_COM_SPI_Init
 *
 *    Note:       Initialize the SPI port settings.
 *
 * ------------------------------------------------------------------------- */
void SIRF_PAL_COM_SPI_Init(void)
{

   return;

}  /* SIRF_PAL_COM_SPI_Init() */

/* ----------------------------------------------------------------------------
 *
 *    Function:   SIRF_PAL_COM_SPI_Close
 *
 *    Note:       This function should have the same return type and parameters
 *                as SIRF_PAL_COM_Close_Fcn().
 *
 * ------------------------------------------------------------------------- */
tSIRF_RESULT SIRF_PAL_COM_SPI_Close(tSIRF_HANDLE  com_handle)
{

   (void)com_handle;

   return(SIRF_COM_RET_FAILURE_GENERIC);

}  /* SIRF_PAL_COM_SPI_Close() */

/* ----------------------------------------------------------------------------
 *
 *    Function:   SIRF_PAL_COM_SPI_Control
 *
 *    Note:       This function should have the same return type and parameters
 *                as SIRF_PAL_COM_Control_Fcn().
 *
 * ------------------------------------------------------------------------- */
tSIRF_RESULT SIRF_PAL_COM_SPI_Control(tSIRF_HANDLE              com_handle,
                                      tSIRF_COM_CONTROL         com_control,
                                      void              * const com_value)
{

   (void)com_handle;
   (void)com_control;
   (void)com_value;

   return(SIRF_COM_RET_FAILURE_GENERIC);

}  /* SIRF_PAL_COM_SPI_Control() */

/* ----------------------------------------------------------------------------
 *
 *    Function:   SIRF_PAL_COM_SPI_Create
 *
 *    Note:       This function should have the same return type and parameters
 *                as SIRF_PAL_COM_Create_Fcn().
 *
 *    Warning:    As written, this function is not thread-safe.
 *
 * ------------------------------------------------------------------------- */
tSIRF_RESULT SIRF_PAL_COM_SPI_Create(tSIRF_HANDLE * const com_handle)
{

   (void)com_handle;

   return(SIRF_COM_RET_FAILURE_GENERIC);

}  /* SIRF_PAL_COM_SPI_Create() */

/* ----------------------------------------------------------------------------
 *
 *    Function:   SIRF_PAL_COM_SPI_Delete
 *
 *    Note:       This function should have the same return type and parameters
 *                as SIRF_PAL_COM_Delete_Fcn().
 *
 * ------------------------------------------------------------------------- */
tSIRF_RESULT SIRF_PAL_COM_SPI_Delete(tSIRF_HANDLE * const com_handle)
{

   (void)com_handle;

   return(SIRF_COM_RET_FAILURE_GENERIC);

}  /* SIRF_PAL_COM_SPI_Delete() */

/* ----------------------------------------------------------------------------
 *
 *    Function:   SIRF_PAL_COM_SPI_Open
 *
 *    Note:       This function should have the same return type and parameters
 *                as SIRF_PAL_COM_Open_Fcn().
 *
 * ------------------------------------------------------------------------- */
tSIRF_RESULT SIRF_PAL_COM_SPI_Open(tSIRF_HANDLE               com_handle,
                                   char         const * const com_port)
{

   (void)com_handle;
   (void)com_port;

   return(SIRF_COM_RET_FAILURE_GENERIC);

}  /* SIRF_PAL_COM_SPI_Open() */


/* ----------------------------------------------------------------------------
*
*    Function:   SIRF_PAL_COM_SPI_Read
*
*    Note:       This function should have the same return type and parameters
*                as SIRF_PAL_COM_Read_Fcn().
*
* -------------------------------------------------------------------------- */
tSIRF_RESULT SIRF_PAL_COM_SPI_Read(tSIRF_HANDLE               com_handle,
                                   tSIRF_UINT8        * const read_ptr,
                                   tSIRF_UINT32               requested_bytes_to_read,
                                   tSIRF_UINT32       * const actual_bytes_read)
{

   (void)com_handle;
   (void)read_ptr;
   (void)requested_bytes_to_read;
   (void)actual_bytes_read;

   return(SIRF_COM_RET_FAILURE_GENERIC);

}  /* SIRF_PAL_COM_SPI_Read() */

/* ----------------------------------------------------------------------------
 *
 *    Function:   SIRF_PAL_COM_SPI_Write
 *
 *    Note:       This function should have the same return type and parameters
 *                as SIRF_PAL_COM_Write_Fcn().
 *
 * ------------------------------------------------------------------------- */
tSIRF_RESULT SIRF_PAL_COM_SPI_Write(tSIRF_HANDLE               com_handle,
                                    tSIRF_UINT8  const * const write_ptr,
                                    tSIRF_UINT32               requested_bytes_to_write,
                                    tSIRF_UINT32       * const actual_bytes_written)
{

   (void)com_handle;
   (void)write_ptr;
   (void)requested_bytes_to_write;
   (void)actual_bytes_written;

   return(SIRF_COM_RET_FAILURE_GENERIC);

}  /* SIRF_PAL_COM_SPI_Write() */

/**
* @}
*/
