/**
 * @addtogroup platform_src_sirf_pal_common
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2005 - 2010 by SiRF Technology, a CSR plc Company.
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
 */

/**
 * @file   sirf_pal_storage_file.c
 *
 * @brief  SiRF PAL storage module using the filesystem.
 */

#include <stdio.h>

#include "sirf_types.h"
#include "sirf_pal.h"
#include "string_sif.h"

FILE *store_id[SIRF_PAL_STORAGE_MAX];

/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */



/**
 * @brief Close the storage device for a given storage ID.
 *        The storage ID is derived from sirf_pal_storage.h
 *        The storage ID is attached the NVM to denote the type
 * @param[in] storage_id               Storage ID.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_STORAGE_Open( tSIRF_UINT32 storage_id )
{
   tSIRF_RESULT result = SIRF_SUCCESS;
   char filename[18];
   FILE *fh;

   /* return if the ID is beyond the max possible */
   if (storage_id >= SIRF_PAL_STORAGE_MAX)
   {
      result = SIRF_FAILURE;
   }

   if (SIRF_SUCCESS == result)
   {
#ifdef OS_ANDROID
      snprintf(filename,sizeof(filename),"/data/NVM%d",(int)storage_id);
#else
      snprintf(filename,sizeof(filename),"NVM%d",(int)storage_id);
#endif

      /* check if file already exists */
      fh = fopen(filename,"r+b");

      if (fh == NULL)
      {
         fh = fopen(filename,"w+b");
         if (fh == NULL)
         {
            result = SIRF_FAILURE;
         }
      }

      if(result == SIRF_SUCCESS)
      {
         store_id[storage_id] = fh;
      }
   }
      
   return result;

} /* SIRF_PAL_STORAGE_Open() */


/**
 * @brief Close the storage device for a given storage ID.
 *        The storage ID is derived from sirf_pal_storage.h
 *        The storage ID is attached the NVM to denote the type
 * @param[in] storage_id               Storage ID.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_STORAGE_Close( tSIRF_UINT32 storage_id )
{
   tSIRF_RESULT result = SIRF_FAILURE;
   FILE *fh;

   /* return if the ID is beyond the max possible */
   if (storage_id < SIRF_PAL_STORAGE_MAX)
   {
      result = SIRF_SUCCESS;
   }

   if (SIRF_SUCCESS == result)
   {
      fh = store_id[storage_id];
      if (fh != NULL)
      {
         fclose(fh);
         store_id[storage_id] = NULL;
         result = SIRF_SUCCESS;
      }
      else
      {
         result = SIRF_FAILURE;
      }
   }
   return result;

} /* SIRF_PAL_STORAGE_Close() */


/**
 * @brief Write data to the storage device.
 * @param[in] storage_id               Storage ID - from sirf_pal_storage.h.
 * @param[in] offset                   Offset into the storage area.
 * @param[in] data                     Data to write.
 * @param[in] length                   Length of the data.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_STORAGE_Write( tSIRF_UINT32 storage_id, tSIRF_UINT32 offset, tSIRF_UINT8 *data, tSIRF_UINT32 length )
{
   tSIRF_UINT32 result = SIRF_FAILURE;
   FILE *fh;

   /* return if the ID is beyond the max possible */
   if (storage_id < SIRF_PAL_STORAGE_MAX)
   {
       result = SIRF_SUCCESS;
   }

   if (SIRF_SUCCESS == result)
   {
      fh = store_id[storage_id];

      /* check if the file exists and the data pointer is not NULL */
      if((fh != NULL) && (data != NULL))
      {
         if (fseek(fh, offset, SEEK_SET) == 0)/* check if we can go to the place we need get to */
         {
            if (1 == fwrite(data,length,1,fh))/* check if the write succeeded */
            {
               result = SIRF_SUCCESS;
            }
            else
            {
               result = SIRF_FAILURE;
            }
         }
         
         if (result == SIRF_SUCCESS)
         {
            tSIRF_UINT32 flush_result;

            flush_result = fflush(fh);/* flush the file */
            if (flush_result)
            {
               result = SIRF_FAILURE;
            }
         }
      }
      else
      {
         result = SIRF_FAILURE;
      }
   }
   return result;
} /* SIRF_PAL_STORAGE_Write() */


/**
 * @brief Read data from the storage device.
 * @param[in] storage_id               Storage ID.
 * @param[in] offset                   Offset into the storage area.
 * @param[in] data                     Buffer to hold data.
 * @param[in] length                   Length of the data to read.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_STORAGE_Read( tSIRF_UINT32 storage_id, tSIRF_UINT32 offset, tSIRF_UINT8 *data, tSIRF_UINT32 length )
{
   tSIRF_UINT32 result = SIRF_PAL_STORAGE_NOT_AVAILABLE;
   FILE *fh;

   /* return if the ID is beyond the max possible */
   if (storage_id >= SIRF_PAL_STORAGE_MAX)
   {
      return result;
   }
      
   fh = store_id[storage_id];

   /* check if the file exists and the data pointer is not NULL */
   if((fh != NULL) && (data != NULL))
   {
      if (0 == fseek(fh, offset, SEEK_SET))/* check if we can go to the place we need get to */
      {
         if (1 == fread(data,length,1,fh))/* check if the read succeeded */
         {
            result = SIRF_SUCCESS;
         }
         else
         {
            result = SIRF_PAL_STORAGE_EMPTY;
         }
      }
   }
      
   return result;

} /* SIRF_PAL_STORAGE_Read() */

/**
 * @}
 * End of file.
 */


