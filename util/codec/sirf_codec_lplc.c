/**
 * @addtogroup lpl3.0_cmdr
 * @{
 */

/***************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. GPS Software                    *
 *                                                                         *
 *    Copyright (c) 2005 - 2010 by SiRF Technology, a CSR plc Company.     *
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
 * MODULE:  LPLCMDR
 *
 * FILENAME:  sirf_codec_lplc.c
 *
 * DESCRIPTION: Functions to encode and decode LPL Command messages from
 *              a byte stream to a C structure.
 *
 ***************************************************************************/
#include <string.h>

#include "sirf_msg.h"
#include "sirf_codec_lplc.h"
#include "sirf_codec.h"
#include "sirf_msg.h"

/***************************************************/
/* Definitions                                     */
/***************************************************/
#ifndef MIN
#define MIN(a,b)   (((a) < (b)) ? (a) : (b))
#endif

/**********************************************************************/
/* Global data variables                                              */
/**********************************************************************/
tSIRF_CHAR g_LPL_CMD_MSG_ICD_Version[20]="1.22";
tSIRF_CHAR g_LPL_CMD_MSG_ICD_Date[20]="8/13/2008";


/*****************/
/*****************/
/*****************/
/* CORE MESSAGES */
/*****************/
/*****************/
/*****************/

/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg001_30_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg001_30* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && (MID001_30_ENCODED_SIZE > size)))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT8 ( &(pMsgStruct->sessionType), &pData, ascii_packet );

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}

/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg001_30_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg001_30* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if (MID001_30_ENCODED_SIZE > *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT8 ( &(pMsgStruct->sessionType), &pData, *size - bytes, ascii_packet );

   *size = bytes;
   return SIRF_SUCCESS;
}


/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg001_31_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg001_31* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && MID001_31_ENCODED_SIZE > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT32 ( &(pMsgStruct->GeoSession_id), &pData, ascii_packet );
   bytes += ImportUINT8  ( &(pMsgStruct->GeoSession_Type), &pData, ascii_packet );

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg001_31_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg001_31* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if (MID001_31_ENCODED_SIZE > *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT32 ( &(pMsgStruct->GeoSession_id), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT8  ( &(pMsgStruct->GeoSession_Type), &pData, *size - bytes, ascii_packet );

   *size = bytes;
   return SIRF_SUCCESS;
}


/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg129_30_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg129_30* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && MID129_30_ENCODED_SIZE > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT8 ( &(pMsgStruct->locationMethod), &pData, ascii_packet );

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg129_30_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg129_30* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if (MID129_30_ENCODED_SIZE > *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT8 ( &(pMsgStruct->locationMethod), &pData, *size - bytes, ascii_packet );

   *size = bytes;
   return SIRF_SUCCESS;
}


/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg129_31_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg129_31* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && MID129_31_ENCODED_SIZE > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT8 ( &(pMsgStruct->AI3_icd), &pData, ascii_packet );
   bytes += ImportUINT8 ( &(pMsgStruct->F_icd), &pData, ascii_packet );
   bytes += ImportUINT8 ( &(pMsgStruct->ICDsAutoDetect), &pData, ascii_packet );

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg129_31_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg129_31* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if (MID129_31_ENCODED_SIZE > *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT8 ( &(pMsgStruct->AI3_icd), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT8 ( &(pMsgStruct->F_icd), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT8 ( &(pMsgStruct->ICDsAutoDetect), &pData, *size - bytes, ascii_packet );

   *size = bytes;
   return SIRF_SUCCESS;
}


/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg129_32_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg129_32* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && MID129_32_ENCODED_SIZE > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT8 ( &(pMsgStruct->LPL_NAV_BITS_AIDING), &pData, ascii_packet );
   bytes += ImportUINT8 ( &(pMsgStruct->LPL_SGEE_AIDING), &pData, ascii_packet );
   bytes += ImportUINT8 ( &(pMsgStruct->LPL_CGEE_AIDING), &pData, ascii_packet );

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg129_32_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg129_32* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if (MID129_32_ENCODED_SIZE > *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT8 ( &(pMsgStruct->LPL_NAV_BITS_AIDING), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT8 ( &(pMsgStruct->LPL_SGEE_AIDING), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT8 ( &(pMsgStruct->LPL_CGEE_AIDING), &pData, *size - bytes, ascii_packet );

   *size = bytes;
   return SIRF_SUCCESS;
}


/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg129_33_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg129_33* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   tSIRF_UINT32 ii;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && MID129_33_ENCODED_SIZE_FIXED > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT16( &(pMsgStruct->port_number),&pData,ascii_packet);
   bytes += ImportUINT32( &(pMsgStruct->sz_ipaddress),&pData,ascii_packet);
   if ((sizeof(pMsgStruct->ip_address) >= pMsgStruct->sz_ipaddress) &&
       (pMsgStruct->sz_ipaddress <= size - bytes))
   {
      memset(pMsgStruct->ip_address,0,sizeof(pMsgStruct->ip_address));
      for (ii = 0; ii < pMsgStruct->sz_ipaddress; ii++)
      {
         bytes += ImportUINT8( (tSIRF_UINT8*)&(pMsgStruct->ip_address[ii]),&pData,ascii_packet);
      }
   }
   else
   {
      return SIRF_FAILURE;
   }
   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg129_33_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg129_33* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   tSIRF_UINT32 ii;

   if (((MID129_33_ENCODED_SIZE_FIXED + pMsgStruct->sz_ipaddress) > *size) ||
       (sizeof(pMsgStruct->ip_address) < pMsgStruct->sz_ipaddress))
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT16( &(pMsgStruct->port_number),&pData, *size - bytes,ascii_packet);
   bytes += ExportUINT32( &(pMsgStruct->sz_ipaddress),&pData, *size - bytes,ascii_packet);
   
   for (ii = 0; ii < pMsgStruct->sz_ipaddress; ii++)
   {
      bytes += ExportUINT8( (tSIRF_UINT8*)&(pMsgStruct->ip_address[ii]),&pData, *size - bytes,ascii_packet);
   }

   *size = bytes;
   return SIRF_SUCCESS;
}


/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg129_34_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg129_34* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && MID129_34_ENCODED_SIZE > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT8( &(pMsgStruct->use_security),&pData,ascii_packet);

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg129_34_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg129_34* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if (MID129_34_ENCODED_SIZE > *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT8( &(pMsgStruct->use_security),&pData, *size - bytes,ascii_packet); 

   *size = bytes;
   return SIRF_SUCCESS;
}


/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg130_30_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg130_30* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && MID130_30_ENCODED_SIZE > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT8 ( &(pMsgStruct->Port), &pData, ascii_packet );
   bytes += ImportUINT8 ( &(pMsgStruct->Enable), &pData, ascii_packet );

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg130_30_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg130_30* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if (MID130_30_ENCODED_SIZE > *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT8 ( &(pMsgStruct->Port), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT8 ( &(pMsgStruct->Enable), &pData, *size - bytes, ascii_packet );

   *size = bytes;
   return SIRF_SUCCESS;
}


/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg130_31_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg130_31* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && MID130_31_ENCODED_SIZE > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT32 ( &(pMsgStruct->GeoSession_ID), &pData, ascii_packet );
   bytes += ImportUINT8  ( &(pMsgStruct->GeoSession_Type), &pData, ascii_packet );

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg130_31_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg130_31* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if (MID130_31_ENCODED_SIZE > *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT32 ( &(pMsgStruct->GeoSession_ID), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT8  ( &(pMsgStruct->GeoSession_Type), &pData, *size - bytes, ascii_packet );

   *size = bytes;
   return SIRF_SUCCESS;
}


/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg130_32_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg130_32* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && MID130_32_ENCODED_SIZE > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT32 ( &(pMsgStruct->GeoSession_ID), &pData, ascii_packet );
   bytes += ImportUINT8  ( &(pMsgStruct->GeoSession_Type), &pData, ascii_packet );

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg130_32_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg130_32* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if (MID130_32_ENCODED_SIZE > *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT32 ( &(pMsgStruct->GeoSession_ID), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT8  ( &(pMsgStruct->GeoSession_Type), &pData, *size - bytes, ascii_packet );

   *size = bytes;
   return SIRF_SUCCESS;
}


/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg130_33_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg130_33* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && MID130_33_ENCODED_SIZE > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT32 ( &(pMsgStruct->GeoSession_ID), &pData, ascii_packet );
   bytes += ImportUINT8  ( &(pMsgStruct->GeoSession_Type), &pData, ascii_packet );

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg130_33_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg130_33* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if (MID130_33_ENCODED_SIZE > *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT32 ( &(pMsgStruct->GeoSession_ID), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT8  ( &(pMsgStruct->GeoSession_Type), &pData, *size - bytes, ascii_packet );

   *size = bytes;
   return SIRF_SUCCESS;
}


/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg130_34_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg130_34* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && 
        MID130_34_ENCODED_SIZE > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT32 ( &(pMsgStruct->GeoSession_ID), &pData, ascii_packet );
   bytes += ImportUINT8  ( &(pMsgStruct->GeoSession_Type), &pData, ascii_packet );
   bytes += ImportUINT32 ( &(pMsgStruct->AGPS_QOShorizontalAccuracy), &pData, ascii_packet );
   bytes += ImportUINT32 ( &(pMsgStruct->AGPS_QOSverticalAccuracy), &pData, ascii_packet );
   bytes += ImportUINT8  ( &(pMsgStruct->AGPS_QOSmaxResponseTime), &pData, ascii_packet );
   bytes += ImportUINT16 ( &(pMsgStruct->AGPS_QOS_maxLocationAge), &pData, ascii_packet );
   bytes += ImportUINT8  ( &(pMsgStruct->reqPositions), &pData, ascii_packet );
   bytes += ImportUINT8  ( &(pMsgStruct->timeBetweenFixes), &pData, ascii_packet );


   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg130_34_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg130_34* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if (MID130_34_ENCODED_SIZE > *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT32 ( &(pMsgStruct->GeoSession_ID), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT8  ( &(pMsgStruct->GeoSession_Type), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT32 ( &(pMsgStruct->AGPS_QOShorizontalAccuracy), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT32 ( &(pMsgStruct->AGPS_QOSverticalAccuracy), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT8  ( &(pMsgStruct->AGPS_QOSmaxResponseTime), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT16 ( &(pMsgStruct->AGPS_QOS_maxLocationAge), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT8  ( &(pMsgStruct->reqPositions), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT8  ( &(pMsgStruct->timeBetweenFixes), &pData, *size - bytes, ascii_packet );


   *size = bytes;
   return SIRF_SUCCESS;
}


/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg130_35_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg130_35* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && MID130_35_ENCODED_SIZE > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT8 ( &(pMsgStruct->resetType), &pData, ascii_packet );

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg130_35_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg130_35* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if (MID130_35_ENCODED_SIZE > *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT8 ( &(pMsgStruct->resetType), &pData, *size - bytes, ascii_packet );

   *size = bytes;
   return SIRF_SUCCESS;
}


/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg130_36_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg130_36* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && MID130_36_ENCODED_SIZE > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT32 ( &(pMsgStruct->GeoSession_ID), &pData, ascii_packet );
   bytes += ImportUINT8  ( &(pMsgStruct->GeoSession_Type), &pData, ascii_packet );

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg130_36_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg130_36* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if (MID130_36_ENCODED_SIZE > *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT32 ( &(pMsgStruct->GeoSession_ID), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT8  ( &(pMsgStruct->GeoSession_Type), &pData, *size - bytes, ascii_packet );

   *size = bytes;
   return SIRF_SUCCESS;
}


/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg130_37_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg130_37* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && MID130_37_ENCODED_SIZE > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT32 ( &(pMsgStruct->GeoSession_ID), &pData, ascii_packet );
   bytes += ImportUINT8  ( &(pMsgStruct->GeoSession_Type), &pData, ascii_packet );

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg130_37_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg130_37* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if (MID130_37_ENCODED_SIZE > *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT32 ( &(pMsgStruct->GeoSession_ID), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT8  ( &(pMsgStruct->GeoSession_Type), &pData, *size - bytes, ascii_packet );

   *size = bytes;
   return SIRF_SUCCESS;
}


/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg130_38_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg130_38* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && MID130_38_ENCODED_SIZE > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT32 ( &(pMsgStruct->GeoSession_ID), &pData, ascii_packet );
   bytes += ImportUINT8  ( &(pMsgStruct->GeoSession_Type), &pData, ascii_packet );

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg130_38_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg130_38* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if (MID130_38_ENCODED_SIZE > *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT32 ( &(pMsgStruct->GeoSession_ID), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT8  ( &(pMsgStruct->GeoSession_Type), &pData, *size - bytes, ascii_packet );

   *size = bytes;
   return SIRF_SUCCESS;
}


/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg133_30_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg133_30* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   
   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       /* At theis point make sure there is at least enough for the fixed part*/
       (!ascii_packet && MID133_30_ENCODED_SIZE_FIXED > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT8  ( &(pMsgStruct->LoggingType), &pData, ascii_packet );

   pMsgStruct->briefLogFileName_length = sizeof(pMsgStruct->briefLogFileName);
   bytes += ImportSTRING( (pMsgStruct->briefLogFileName),
                          &(pMsgStruct->briefLogFileName_length) ,
                          &pData,
                          size - bytes,
                          ascii_packet );

   pMsgStruct->detailedLogFileName_length = sizeof(pMsgStruct->detailedLogFileName);
   bytes += ImportSTRING( (pMsgStruct->detailedLogFileName),
                          &(pMsgStruct->detailedLogFileName_length) ,
                          &pData,
                          size - bytes,
                          ascii_packet );

   pMsgStruct->agpsLogFileName_length = sizeof(pMsgStruct->agpsLogFileName);
   bytes += ImportSTRING( (pMsgStruct->agpsLogFileName),
                          &(pMsgStruct->agpsLogFileName_length) ,
                          &pData,
                          size - bytes,
                          ascii_packet );

   pMsgStruct->previousLocationFileName_length = sizeof(pMsgStruct->previousLocationFileName);
   bytes += ImportSTRING( (pMsgStruct->previousLocationFileName),
                          &(pMsgStruct->previousLocationFileName_length) ,
                          &pData,
                          size - bytes,
                          ascii_packet );

   pMsgStruct->slcLogFileName_length = sizeof(pMsgStruct->slcLogFileName);
   bytes += ImportSTRING( (pMsgStruct->slcLogFileName),
                          &(pMsgStruct->slcLogFileName_length) ,
                          &pData,
                          size - bytes,
                          ascii_packet );

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg133_30_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg133_30* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if ((MID133_30_ENCODED_SIZE_FIXED 
      + pMsgStruct->briefLogFileName_length 
      + pMsgStruct->detailedLogFileName_length 
      + pMsgStruct->agpsLogFileName_length 
      + pMsgStruct->previousLocationFileName_length
      + pMsgStruct->slcLogFileName_length) > *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT8  ( &(pMsgStruct->LoggingType), &pData, *size - bytes, ascii_packet );

   bytes += ExportSTRING( (pMsgStruct->briefLogFileName),
                          pMsgStruct->briefLogFileName_length,
                          &pData,
                          *size - bytes,
                          ascii_packet );
   bytes += ExportSTRING( (pMsgStruct->detailedLogFileName),
                          pMsgStruct->detailedLogFileName_length, 
                          &pData, 
                          *size - bytes, 
                          ascii_packet );
   bytes += ExportSTRING( (pMsgStruct->agpsLogFileName),
                          (pMsgStruct->agpsLogFileName_length) ,
                          &pData,
                          *size - bytes,
                          ascii_packet );
   bytes += ExportSTRING( (pMsgStruct->previousLocationFileName),
                          pMsgStruct->previousLocationFileName_length,
                          &pData,
                          *size - bytes,
                          ascii_packet );
   bytes += ExportSTRING( (pMsgStruct->slcLogFileName),
                          pMsgStruct->slcLogFileName_length,
                          &pData,
                          *size - bytes,
                          ascii_packet );

   *size = bytes;
   return SIRF_SUCCESS;
}


/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg134_01_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg134_01* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && MID134_01_ENCODED_SIZE_FIXED > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT8  ( &(pMsgStruct->m_AppID), &pData, ascii_packet );
   pMsgStruct->m_TextLen = sizeof(pMsgStruct->m_StatusMessage);
   bytes += ImportSTRING( (pMsgStruct->m_StatusMessage),
                          &(pMsgStruct->m_TextLen),
                          &pData,
                          size - bytes,
                          ascii_packet );

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg134_01_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg134_01* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if (MID134_01_ENCODED_SIZE_FIXED > *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT8  ( &(pMsgStruct->m_AppID), &pData, *size - bytes, ascii_packet );
   bytes += ExportSTRING( (pMsgStruct->m_StatusMessage),
                          (pMsgStruct->m_TextLen),
                          &pData,
                          *size - bytes,
                          ascii_packet );

   *size = bytes;
   return SIRF_SUCCESS;
}


/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg134_02_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg134_02* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && MID134_02_ENCODED_SIZE_FIXED > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT8  ( &(pMsgStruct->m_AppID), &pData, ascii_packet );
   pMsgStruct->m_TextLen = sizeof(pMsgStruct->m_StatusMessage);
   bytes += ImportSTRING( (pMsgStruct->m_StatusMessage),
                          &(pMsgStruct->m_TextLen),
                          &pData,
                          size - bytes,
                          ascii_packet );

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg134_02_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg134_02* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if (MID134_02_ENCODED_SIZE_FIXED + pMsgStruct->m_TextLen> *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT8  ( &(pMsgStruct->m_AppID), &pData, *size - bytes, ascii_packet );
   bytes += ExportSTRING( (pMsgStruct->m_StatusMessage),
                          (pMsgStruct->m_TextLen),
                          &pData,
                          *size - bytes,
                          ascii_packet );

   *size = bytes;
   return SIRF_SUCCESS;
}


/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg135_01_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg135_01* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   (void)pData;
   (void)size;
   (void)ascii_packet;
   /* No payload besides the MID and SUBID */
   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if (sizeof(*pMsgStruct) > *struct_size)
   {
      return SIRF_FAILURE;
   }

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg135_01_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg135_01* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   (void)pData;
   (void)pMsgStruct;
   (void)ascii_packet;
   /* No payload besides the MID and SUBID */
   *size = 0;
   return SIRF_SUCCESS;
}


/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg135_02_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg135_02* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   tSIRF_UINT32 ii;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && MID135_02_ENCODED_SIZE_FIXED > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT16(&pMsgStruct->m_SWVerLen,&pData,ascii_packet);
   if (sizeof(pMsgStruct->m_SWVersion) < pMsgStruct->m_SWVerLen)
   {
      return SIRF_FAILURE;
   }

   for (ii = 0; ii < pMsgStruct->m_SWVerLen; ii++)
   {
      bytes += ImportUINT8(&pMsgStruct->m_SWVersion[ii],&pData,ascii_packet);
   }

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg135_02_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg135_02* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   if (pMsgStruct->m_SWVerLen + MID135_02_ENCODED_SIZE_FIXED > *size)
   {
      return SIRF_FAILURE;
   }

   *size = ExportSTRING( (pMsgStruct->m_SWVersion),
                         (pMsgStruct->m_SWVerLen),
                         &pData,
                         *size,
                         ascii_packet );
   return SIRF_SUCCESS;
}

/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg135_03_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg135_03* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   (void)pData;
   (void)size;
   (void)ascii_packet;

   /* No payload besides the MID and SUBID */
   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if (sizeof(*pMsgStruct) > *struct_size)
   {
      return SIRF_FAILURE;
   }

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg135_03_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg135_03* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   (void)pData;
   (void)pMsgStruct;
   (void)ascii_packet;
   /* No payload besides the MID and SUBID */
   *size = 0;
   return SIRF_SUCCESS;
}

/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg135_04_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg135_04* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   tSIRF_UINT32 ii;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && MID135_04_ENCODED_SIZE_FIXED > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT16(&pMsgStruct->m_ICDVerLen,&pData,ascii_packet);
   if (sizeof(pMsgStruct->m_ICDVersion) < pMsgStruct->m_ICDVerLen)
   {
      return SIRF_FAILURE;
   }

   for (ii = 0; ii < pMsgStruct->m_ICDVerLen; ii++)
   {
      bytes += ImportUINT8(&pMsgStruct->m_ICDVersion[ii],&pData,ascii_packet);
   }

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg135_04_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg135_04* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   if (pMsgStruct->m_ICDVerLen + MID135_04_ENCODED_SIZE_FIXED > *size)
   {
      return SIRF_FAILURE;
   }
   *size = ExportSTRING( (pMsgStruct->m_ICDVersion),
                         (pMsgStruct->m_ICDVerLen),
                         &pData,
                         *size,
                         ascii_packet );
   return SIRF_SUCCESS;
}


/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg137_30_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg137_30* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       /* This message may yeild a false positive as we don't know the size
        * until we've read in the length of the strings.  After each string
        * read a size check is needed */
       (!ascii_packet && MID137_30_ENCODED_SIZE_FIXED > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT8  ( &(pMsgStruct->gpsDevice_configType), &pData, ascii_packet );
   bytes += ImportUINT8  ( &(pMsgStruct->gpsDevice_SerialConfig_needToOpenComport), &pData, ascii_packet );
   pMsgStruct->gpsDevice_SerialConfig_serialport_length = sizeof(pMsgStruct->gpsDevice_SerialConfig_SerialPort);
   bytes += ImportSTRING( (tSIRF_UINT8 *)(pMsgStruct->gpsDevice_SerialConfig_SerialPort),
                          &(pMsgStruct->gpsDevice_SerialConfig_serialport_length) ,
                          &pData,
                          size - bytes,
                          ascii_packet );

   /* Size check */
   if (sizeof(tSIRF_UINT32) + sizeof(tSIRF_UINT8) > (size - bytes)) 
   {
      return SIRF_FAILURE;
   }
   bytes += ImportINT32  ( &(pMsgStruct->gpsDevice_SerialConfig_BaudRate), &pData, ascii_packet );
   bytes += ImportUINT8  ( &(pMsgStruct->gpsDevice_SerialConfig_needToOpenVirtualPort), &pData, ascii_packet );

   pMsgStruct->gpsDevice_SerialConfig_nmeaport_length = sizeof(pMsgStruct->gpsDevice_SerialConfig_NMEASerialPort);
   bytes += ImportSTRING( (tSIRF_UINT8 *)(pMsgStruct->gpsDevice_SerialConfig_NMEASerialPort),
                          &(pMsgStruct->gpsDevice_SerialConfig_nmeaport_length) ,
                          &pData,                          
                          size - bytes,
                          ascii_packet );
   /* size check for the rest of the message */
   if (sizeof(tSIRF_INT32) + 3*sizeof(tSIRF_UINT8) > (size - bytes)) 
   {
      return SIRF_FAILURE;
   }
   bytes += ImportUINT8(&(pMsgStruct->gpsDevice_SerialConfig_bUseNMEA), &pData, ascii_packet );
   bytes += ImportINT32(&(pMsgStruct->memConfig_memSize), &pData, ascii_packet );
   bytes += ImportUINT8(&(pMsgStruct->powerControl_cgpsCtrl), &pData, ascii_packet );
   bytes += ImportUINT8(&(pMsgStruct->powerControl_gpsPwrCtrlCap), &pData, ascii_packet );


   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg137_30_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg137_30* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if ((MID137_30_ENCODED_SIZE_FIXED 
      + pMsgStruct->gpsDevice_SerialConfig_serialport_length
      + pMsgStruct->gpsDevice_SerialConfig_nmeaport_length)> *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT8  ( &(pMsgStruct->gpsDevice_configType), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT8  ( &(pMsgStruct->gpsDevice_SerialConfig_needToOpenComport), &pData, *size - bytes, ascii_packet );
   bytes += ExportSTRING( (tSIRF_UINT8 *)(pMsgStruct->gpsDevice_SerialConfig_SerialPort),
                          (pMsgStruct->gpsDevice_SerialConfig_serialport_length) ,
                          &pData,
                          *size - bytes,
                          ascii_packet );
   bytes += ExportINT32  ( &(pMsgStruct->gpsDevice_SerialConfig_BaudRate), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT8  ( &(pMsgStruct->gpsDevice_SerialConfig_needToOpenVirtualPort), &pData, *size - bytes, ascii_packet );
   bytes += ExportSTRING( (tSIRF_UINT8 *)(pMsgStruct->gpsDevice_SerialConfig_NMEASerialPort),
                          (pMsgStruct->gpsDevice_SerialConfig_nmeaport_length),
                          &pData,
                          *size - bytes,
                          ascii_packet );
   bytes += ExportUINT8  ( &(pMsgStruct->gpsDevice_SerialConfig_bUseNMEA), &pData, *size - bytes, ascii_packet );
   bytes += ExportINT32  ( &(pMsgStruct->memConfig_memSize), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT8  ( &(pMsgStruct->powerControl_cgpsCtrl), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT8  ( &(pMsgStruct->powerControl_gpsPwrCtrlCap), &pData, *size - bytes, ascii_packet );


   *size = bytes;
   return SIRF_SUCCESS;
}


/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg137_31_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg137_31* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{ 
   (void)pData;
   (void)size;
   (void)ascii_packet;
   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if (sizeof(*pMsgStruct) > *struct_size) 
   {
      return SIRF_FAILURE;
   }

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg137_31_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg137_31* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   (void)pData;
   (void)pMsgStruct;
   (void)ascii_packet;
   *size = 0;
   return SIRF_SUCCESS;
}

/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg139_30_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg139_30* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{ 
   tSIRF_UINT32 bytes = 0;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && MID139_30_ENCODED_SIZE > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT32 ( &(pMsgStruct->FreqTxMetho), &pData, ascii_packet );

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}




/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg139_30_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg139_30* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if (MID139_30_ENCODED_SIZE > *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT32 ( &(pMsgStruct->FreqTxMetho), &pData, *size - bytes, ascii_packet );

   *size = bytes;
   return SIRF_SUCCESS;
}



/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg139_31_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg139_31* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{ 
   tSIRF_UINT32 bytes = 0;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && MID139_31_ENCODED_SIZE > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT32 ( &(pMsgStruct->MaxRequestTime), &pData, ascii_packet );
   bytes += ImportUINT32 ( &(pMsgStruct->RequestPeriod), &pData, ascii_packet );

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}




/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg139_31_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg139_31* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if (MID139_31_ENCODED_SIZE > *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT32 ( &(pMsgStruct->MaxRequestTime), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT32 ( &(pMsgStruct->RequestPeriod), &pData, *size - bytes, ascii_packet );

   *size = bytes;
   return SIRF_SUCCESS;
}



/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg139_32_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg139_32* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{ 
   tSIRF_UINT32 bytes = 0;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && MID139_32_ENCODED_SIZE > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportDOUBLE ( &(pMsgStruct->NominalFreq), &pData, ascii_packet );

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}




/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg139_32_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg139_32* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if (MID139_32_ENCODED_SIZE > *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportDOUBLE ( &(pMsgStruct->NominalFreq), &pData, *size - bytes, ascii_packet );

   *size = bytes;
   return SIRF_SUCCESS;
}




/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg139_33_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg139_33* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{ 
   tSIRF_UINT32 bytes = 0;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && MID139_33_ENCODED_SIZE > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT32 ( &(pMsgStruct->RequestID), &pData, ascii_packet );
   bytes += ImportUINT32 ( &(pMsgStruct->FrequencyDataType), &pData, ascii_packet );
   bytes += ImportDOUBLE ( &(pMsgStruct->frequency), &pData, ascii_packet );
   bytes += ImportUINT32 ( &(pMsgStruct->AccuracyDataType), &pData, ascii_packet );
   bytes += ImportDOUBLE ( &(pMsgStruct->Accuracy), &pData, ascii_packet );

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}




/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg139_33_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg139_33* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if (MID139_33_ENCODED_SIZE > *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT32 ( &(pMsgStruct->RequestID), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT32 ( &(pMsgStruct->FrequencyDataType), &pData, *size - bytes, ascii_packet );
   bytes += ExportDOUBLE ( &(pMsgStruct->frequency), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT32 ( &(pMsgStruct->AccuracyDataType), &pData, *size - bytes, ascii_packet );
   bytes += ExportDOUBLE ( &(pMsgStruct->Accuracy), &pData, *size - bytes, ascii_packet );

   *size = bytes;
   return SIRF_SUCCESS;
}


/** 
 * Decode function for this message.
 *
 * Converts a data stream into a structre.
 * 
 * @param pData[in] Data stream
 * @param size[in] size of the data stream.
 * @param pMsgStruct[out] structure to fill
 * @param struct_size[in/out]  size of pMsgStruct buffer.
 * @param ascii_packet[in] Type of data stream.  Ascii or Binary.
 * 
 * @return SIRF_SUCCESS if there is enough data to fill the structure size is
 *         sufficient.
 */
static tSIRF_RESULT LPLCmdrMsg255_01_Import(tSIRF_UINT8* pData, tSIRF_UINT32 size, tLPLCmdrMsg255_01* pMsgStruct, tSIRF_UINT32* struct_size, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;

   /* Not enough data or not enough room to fit the data
    * ascii_packets do their own error correction */
   if ((sizeof(*pMsgStruct) > *struct_size) ||
       (!ascii_packet && MID255_01_ENCODED_SIZE > size))
   {
      return SIRF_FAILURE;
   }

   bytes += ImportUINT8  ( &(pMsgStruct->m_ResponseToMID),   &pData, ascii_packet );
   bytes += ImportUINT8  ( &(pMsgStruct->m_ResponseToSUBID), &pData, ascii_packet );
   bytes += ImportUINT8  ( &(pMsgStruct->m_ResponseType),    &pData, ascii_packet );
   bytes += ImportUINT32 ( &(pMsgStruct->m_ResponseCode),    &pData, ascii_packet );

   *struct_size = sizeof(*pMsgStruct);
   return SIRF_SUCCESS;
}


/** 
 * Export this message.
 *
 * Convert a structure to the specified data stream.
 *
 * Note: The full data was exported if and only if SIRF_SUCCESS is returned
 * and the size returned is less than or equal to the input size.  Because
 * of string formatting it is impossible to tell if there is going to be 
 * enough room for the string until it is attempted to be written.
 * 
 * @param pData[out] data buffer to hold the stream.
 * @param size[in/out] Size of the data buffer on input, amout of data exported
 *                     on output.
 * @param pMsgStruct[out] Structure that needs exporting
 * @param ascii_packet[in] SIRF_TRUE for an ascii stream, SIRF_FALSE for a
 *                         byte stream.
 * 
 * @return SIRF_SUCCESS if there is enough room for a byte stream.  
 */
static tSIRF_RESULT LPLCmdrMsg255_01_Export(tSIRF_UINT8* pData, tSIRF_UINT32 *size, tLPLCmdrMsg255_01* pMsgStruct, tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   if (MID255_01_ENCODED_SIZE > *size)
   {
      return SIRF_FAILURE;
   }

   bytes += ExportUINT8  ( &(pMsgStruct->m_ResponseToMID),   &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT8  ( &(pMsgStruct->m_ResponseToSUBID), &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT8  ( &(pMsgStruct->m_ResponseType),    &pData, *size - bytes, ascii_packet );
   bytes += ExportUINT32 ( &(pMsgStruct->m_ResponseCode),    &pData, *size - bytes, ascii_packet );

   *size = bytes;
   return SIRF_SUCCESS;
}

/*****************************************************/
/*****************************************************/
/*****************************************************/
/* GENERIC COMPOSITE STRUCTURE FOR LPL CMDR MESSAGES */
/*****************************************************/
/*****************************************************/
/*****************************************************/

/** 
 * Decodes an ascii or byte packet into a message structure
 * 
 * @param packet[in]             pointer to data to decode
 * @param packet_length[in]      size of buffer at packet
 * @param message_id[out]        lc,mid,sid of the returned message
 * @param message_structure[out] stucture of the message
 * @param message_length[in/out] On input, available size of message_structre
 *                               On output, actual size used
 * @param ascii_packet[in]       true if an ascii packet is the input
 * 
 * @return SIRF_SUCCESS in the operation was performed correctly. SIRF_FAILURE
 *         otherwise.  SIRF_FAILURE can be returned if any of the pointers are
 *         NULL or too small to hold the message of interest.
 */
tSIRF_RESULT SIRF_CODEC_LPLC_Decode( tSIRF_UINT8* payload,
                                         tSIRF_UINT32 payload_length,
                                         tSIRF_UINT32 *message_id,
                                         tSIRF_VOID *message_structure,
                                         tSIRF_UINT32 *message_length,
                                         tSIRF_BOOL ascii_packet)
{
   tLPLCmdrMsgGeneric* pMsgStruct = (tLPLCmdrMsgGeneric*) message_structure;
   tSIRF_RESULT ret = SIRF_FAILURE;
   tSIRF_UINT8 lc, mid, sid;

   /* make sure inputs are not null and payload length is long enough to include
    * lc, mid, and sid */
   if ( ((tSIRF_UINT8 *) NULL ==  payload) ||
        ((tSIRF_UINT32 *) NULL == message_id) ||
        ((tSIRF_VOID *) NULL == message_structure) ||
        ((tSIRF_UINT32 *) NULL == message_length) ||
        (3*sizeof(tSIRF_UINT8) > payload_length)) 
   {
      return SIRF_FAILURE;
   }

   (void)ImportUINT8 ( &lc, &payload, ascii_packet );
   (void)ImportUINT8 ( &mid, &payload, ascii_packet );
   (void)ImportUINT8 ( &sid, &payload, ascii_packet );

   *message_id = SIRF_MAKE_MSG_ID(lc,mid,sid);
   payload_length -= SIRF_LPLC_MSG_HEADER_SIZE_BYTES; /* lc,mid,sid*/

   switch (*message_id)
   {
      case MID001_30:
         ret = LPLCmdrMsg001_30_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg001_30),
                                       message_length,
                                       ascii_packet);
         break;
      case MID001_31:
         ret = LPLCmdrMsg001_31_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg001_31),
                                       message_length,
                                       ascii_packet);
         break;
      case MID129_30:
         ret = LPLCmdrMsg129_30_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg129_30),
                                       message_length,
                                       ascii_packet);
         break;
      case MID129_31:
         ret = LPLCmdrMsg129_31_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg129_31),
                                       message_length,
                                       ascii_packet);
         break;
      case MID129_32:
         ret = LPLCmdrMsg129_32_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg129_32),
                                       message_length,
                                       ascii_packet);
         break;
      case MID129_33:
         ret = LPLCmdrMsg129_33_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg129_33),
                                       message_length,
                                       ascii_packet);
         break;
      case MID129_34:
         ret = LPLCmdrMsg129_34_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg129_34),
                                       message_length,
                                       ascii_packet);
         break;
      case MID130_30:
         ret = LPLCmdrMsg130_30_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg130_30),
                                       message_length,
                                       ascii_packet);
         break;
      case MID130_31:
         ret = LPLCmdrMsg130_31_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg130_31),
                                       message_length,
                                       ascii_packet);
         break;
      case MID130_32:
         ret = LPLCmdrMsg130_32_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg130_32),
                                       message_length,
                                       ascii_packet);
         break;
      case MID130_33:
         ret = LPLCmdrMsg130_33_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg130_33),
                                       message_length,
                                       ascii_packet);
         break;
      case MID130_34:
         ret = LPLCmdrMsg130_34_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg130_34),
                                       message_length,
                                       ascii_packet);
         break;
      case MID130_35:
         ret = LPLCmdrMsg130_35_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg130_35),
                                       message_length,
                                       ascii_packet);
         break;
      case MID130_36:
         ret = LPLCmdrMsg130_36_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg130_36),
                                       message_length,
                                       ascii_packet);
         break;
      case MID130_37:
         ret = LPLCmdrMsg130_37_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg130_37),
                                       message_length,
                                       ascii_packet);
         break;
      case MID130_38:
         ret = LPLCmdrMsg130_38_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg130_38),
                                       message_length,
                                       ascii_packet);
         break;
      case MID133_30:
         ret = LPLCmdrMsg133_30_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg133_30),
                                       message_length,
                                       ascii_packet);
         break;
      case MID134_01:
         ret = LPLCmdrMsg134_01_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg134_01),
                                       message_length,
                                       ascii_packet);
         break;
      case MID134_02:
         ret = LPLCmdrMsg134_02_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg134_02),
                                       message_length,
                                       ascii_packet);
         break;
      case MID135_01: 
         ret = LPLCmdrMsg135_01_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg135_01),
                                       message_length,
                                       ascii_packet );
         break;
      case MID135_02: 
         ret = LPLCmdrMsg135_02_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg135_02),
                                       message_length,
                                       ascii_packet );
         break;
      case MID135_03: 
         ret = LPLCmdrMsg135_03_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg135_03),
                                       message_length,
                                       ascii_packet );
         break;
      case MID135_04: 
         ret = LPLCmdrMsg135_04_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg135_04),
                                       message_length,
                                       ascii_packet );
         break;
      case MID137_30:
         ret = LPLCmdrMsg137_30_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg137_30),
                                       message_length,
                                       ascii_packet);
         break;
      case MID137_31:
         ret = LPLCmdrMsg137_31_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg137_31),
                                       message_length,
                                       ascii_packet);
         break;
      case MID139_30:
         ret = LPLCmdrMsg139_30_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg139_30),
                                       message_length,
                                       ascii_packet);
         break;
      case MID139_31:
         ret = LPLCmdrMsg139_31_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg139_31),
                                       message_length,
                                       ascii_packet);
         break;
      case MID139_32:
         ret = LPLCmdrMsg139_32_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg139_32),
                                       message_length,
                                       ascii_packet);
         break;
      case MID139_33:
         ret = LPLCmdrMsg139_33_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg139_33),
                                       message_length,
                                       ascii_packet);
         break;
      case MID255_01: 
         ret = LPLCmdrMsg255_01_Import(payload,
                                       payload_length,
                                       &(pMsgStruct->m_MsgPayload.Msg255_01),
                                       message_length,
                                       ascii_packet );
         break;
      default:
         ret = SIRF_FAILURE;
         *message_length = 0;
         break;
   }

   return ret;
}

/** 
 * Encodes a message structer into either an ascii or byte packet
 * 
 * @param message_id[in]        lc,mid,sid of the message
 * @param message_structure[in] stucture of the message
 * @param message_length[in]    size of the data pointed to by message_structure
 * @param packet[out]           pointer to data to encode
 * @param packet_length[out]    size of buffer at packet
 * @param ascii_packet[in]      true if an ascii packet should be retured
 * 
 * @return SIRF_SUCCESS in the operation was performed correctly. SIRF_FAILURE
 *         otherwise.  SIRF_FAILURE can be returned if any of the pointers are
 *         NULL or too small to hold the message of interest.
 */
tSIRF_RESULT SIRF_CODEC_LPLC_Encode( tSIRF_UINT32 message_id,
                                         tSIRF_VOID *message_structure,
                                         tSIRF_UINT32 message_length,
                                         tSIRF_UINT8 *packet,
                                         tSIRF_UINT32 *packet_length,
                                         tSIRF_BOOL ascii_packet)
{
   tSIRF_UINT32 bytes = 0;
   tLPLCmdrMsgGeneric* pMsgStruct = (tLPLCmdrMsgGeneric*)message_structure;
   tSIRF_UINT32 buffer_left = *packet_length; /* amount of buffer left in packet */
   tSIRF_RESULT ret = SIRF_SUCCESS;
   tSIRF_UINT8 mid = SIRF_GET_MID(message_id);
   tSIRF_UINT8 lc  = SIRF_GET_LC(message_id);
   tSIRF_UINT8 sid = SIRF_GET_SUB_ID(message_id);

   /* First make sure the function was called right */
   if ( ((tSIRF_VOID *) NULL == message_structure) ||
        ((tSIRF_UINT8 *) NULL ==  packet) ||
        ((tSIRF_UINT32 *) NULL == packet_length) ||
        (3*sizeof(tSIRF_UINT8) > buffer_left) || /* room for lc,mid,sid */
        (lc != SIRF_LC_LPLC))
   {
      return SIRF_CODEC_ERROR_INVALID_PARAMETER;
   }

   /* Encode the lc,mid, and sid */
   bytes += ExportUINT8 ( &lc, &packet, *packet_length - bytes, ascii_packet );
   bytes += ExportUINT8 ( &mid, &packet, *packet_length - bytes, ascii_packet );
   /* LPLC everything has a SID. */
   bytes += ExportUINT8 ( &sid, &packet, *packet_length - bytes, ascii_packet );

   buffer_left -= SIRF_LPLC_MSG_HEADER_SIZE_BYTES; /* lc,mid,sid*/

   switch (message_id)
   {
      case MID001_30:
         if (sizeof(pMsgStruct->m_MsgPayload.Msg001_30) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg001_30_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg001_30),
                                       ascii_packet);
         break;
      case MID001_31:
         if (sizeof(pMsgStruct->m_MsgPayload.Msg001_31) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg001_31_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg001_31),
                                       ascii_packet);
         break;
      case MID129_30:
         if (sizeof(pMsgStruct->m_MsgPayload.Msg129_30) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg129_30_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg129_30),
                                       ascii_packet);
         break;
      case MID129_31:
         if (sizeof(pMsgStruct->m_MsgPayload.Msg129_31) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg129_31_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg129_31),
                                       ascii_packet);
         break;
      case MID129_32:
         if (sizeof(pMsgStruct->m_MsgPayload.Msg129_32) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg129_32_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg129_32),
                                       ascii_packet);
         break;
      case MID129_33:
         if (sizeof(pMsgStruct->m_MsgPayload.Msg129_33) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg129_33_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg129_33),
                                       ascii_packet);
         break;
      case MID129_34:
         if (sizeof(pMsgStruct->m_MsgPayload.Msg129_34) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg129_34_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg129_34),
                                       ascii_packet);
         break;
      case MID130_30:
         if (sizeof(pMsgStruct->m_MsgPayload.Msg130_30) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg130_30_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg130_30),
                                       ascii_packet);
         break;
      case MID130_31:
         if (sizeof(pMsgStruct->m_MsgPayload.Msg130_31) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg130_31_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg130_31),
                                       ascii_packet);
         break;
      case MID130_32:
         if (sizeof(pMsgStruct->m_MsgPayload.Msg130_32) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg130_32_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg130_32),
                                       ascii_packet);
         break;
      case MID130_33:
         if (sizeof(pMsgStruct->m_MsgPayload.Msg130_33) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg130_33_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg130_33),
                                       ascii_packet);
         break;
      case MID130_34:
         if (sizeof(pMsgStruct->m_MsgPayload.Msg130_34) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg130_34_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg130_34),
                                       ascii_packet);
         break;
      case MID130_35:
         if (sizeof(pMsgStruct->m_MsgPayload.Msg130_35) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg130_35_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg130_35),
                                       ascii_packet);
         break;
      case MID130_36:
         if (sizeof(pMsgStruct->m_MsgPayload.Msg130_36) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg130_36_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg130_36),
                                       ascii_packet);
         break;
      case MID130_37:
         if (sizeof(pMsgStruct->m_MsgPayload.Msg130_37) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg130_37_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg130_37),
                                       ascii_packet);
         break;
      case MID130_38:
         if (sizeof(pMsgStruct->m_MsgPayload.Msg130_38) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg130_38_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg130_38),
                                       ascii_packet);
         break;
      case MID133_30:
         if (sizeof(pMsgStruct->m_MsgPayload.Msg133_30) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg133_30_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg133_30),
                                       ascii_packet);
         break;
      case MID134_01: 
         if (sizeof(pMsgStruct->m_MsgPayload.Msg134_01) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg134_01_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg134_01),
                                       ascii_packet );
         break;
      case MID134_02: 
         if (sizeof(pMsgStruct->m_MsgPayload.Msg134_02) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg134_02_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg134_02),
                                       ascii_packet );
         break;
      case MID135_01: 
         if (sizeof(pMsgStruct->m_MsgPayload.Msg135_01) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg135_01_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg135_01),
                                       ascii_packet );
         break;
      case MID135_02: 
         if (sizeof(pMsgStruct->m_MsgPayload.Msg135_02) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg135_02_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg135_02),
                                       ascii_packet );
         break;
      case MID135_03: 
         if (sizeof(pMsgStruct->m_MsgPayload.Msg135_03) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg135_03_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg135_03),
                                       ascii_packet );
         break;
      case MID135_04: 
         if (sizeof(pMsgStruct->m_MsgPayload.Msg135_04) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg135_04_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg135_04),
                                       ascii_packet );
         break;
      case MID137_30:
         if (sizeof(pMsgStruct->m_MsgPayload.Msg137_30) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg137_30_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg137_30),
                                       ascii_packet);
         break;
      case MID137_31:
         if (sizeof(pMsgStruct->m_MsgPayload.Msg137_31) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg137_31_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg137_31),
                                       ascii_packet);
         break;
      case MID139_30:
         if (sizeof(pMsgStruct->m_MsgPayload.Msg139_30) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg139_30_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg139_30),
                                       ascii_packet);
         break;
      case MID139_31:
         if (sizeof(pMsgStruct->m_MsgPayload.Msg139_31) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg139_31_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg139_31),
                                       ascii_packet);
         break;
      case MID139_32:
         if (sizeof(pMsgStruct->m_MsgPayload.Msg139_32) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg139_32_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg139_32),
                                       ascii_packet);
         break;
      case MID139_33:
         if (sizeof(pMsgStruct->m_MsgPayload.Msg139_33) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg139_33_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg139_33),
                                       ascii_packet);
         break;
      case MID255_01: 
         if (sizeof(pMsgStruct->m_MsgPayload.Msg255_01) != message_length)
         {
            return SIRF_FAILURE;
         }
         ret = LPLCmdrMsg255_01_Export(packet,
                                       &buffer_left,
                                       &(pMsgStruct->m_MsgPayload.Msg255_01),
                                       ascii_packet );
         break;
      default:
         ret = SIRF_FAILURE;
         *packet_length = 0;
         break;
   }
   *packet_length = buffer_left+SIRF_LPLC_MSG_HEADER_SIZE_BYTES;
   return ret;
}

/*******************************************/
/*******************************************/
/*******************************************/
/* Helper functions                        */
/*******************************************/
/*******************************************/
/*******************************************/

/***************************************************************************
 *  @func:     SwapBytes
 *
 *  @param:    tSIRF_UINT16        inBytes
 *
 *  @return:   tSIRF_UINT16        outBytes
 *
 *  @comm:     Swap the lowest 2 bytes in a number, eg 0x0000ABCD becomes 0x0000CDAB
 ***************************************************************************/
tSIRF_UINT16 SwapBytes(tSIRF_UINT16 inBytes)
{
   tSIRF_UINT8 byte1 = inBytes & 0x000000FF;
   tSIRF_UINT8 byte2 = (inBytes & 0x0000FF00) >> 8;
   tSIRF_UINT16 shifted_byte1 = ((tSIRF_UINT16)byte1 << 8);
   tSIRF_UINT16 return_value = shifted_byte1 + (tSIRF_UINT16)byte2;
   return return_value;
}

/***************************************************************************
 *  @func:     AddChannelID
 *
 *  @param:    tSIRF_UINT32        inBytes
 *
 *  @return:   tSIRF_UINT32        outBytes
 *
 *  @comm:     Shifts the inBytes up 1 byte and adds the channel ID 0xB1 to the
 *             lower byte.  For example 0xAB would become 0xABB1
 ***************************************************************************/
tSIRF_UINT32 AddChannelID(tSIRF_UINT32 inBytes)
{
   /* The channel ID goes into the lowest bits - yes crazy but that's what 
      the SIRFNAV_DEMO_SendSSB() function sends first...! */
   return (inBytes << 8) + SIRF_LC_LPLC;
}

/***************************************************************************
 *  @func:     GetMID
 *
 *  @param:    tSIRF_UINT16    inBytes
 *
 *  @return:   tSIRF_UINT8     outByte
 *
 *  @comm:     Get the message ID when it's the second lowest byte in the number.
 *             Logical AND with 0xFF00 and shift the result right 1 byte.
 *             For exmaple 0xABCD as input would return 0xAB
 ***************************************************************************/
tSIRF_UINT8 GetMID(tSIRF_UINT16 inBytes)
{
   return (inBytes & 0xFF00) >> 8;
}

/***************************************************************************
 *  @func:     GetSUBID
 *
 *  @param:    tSIRF_UINT16   inBytes
 *
 *  @return:   tSIRF_UINT8    outByte
 *
 *  @comm:     Get the message sub ID when it's the lowest byte in the number.
 *             Logical AND with 0x00FF.
 *             For exmaple 0xABCD as input would return 0xCD
 ***************************************************************************/
tSIRF_UINT8 GetSUBID(tSIRF_UINT16 inBytes)
{
   return inBytes & 0x00FF;
}

/**
 * @}
 */

