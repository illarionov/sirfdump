/**
 * @addtogroup lpl3.0_cmdr
 * @{
 */

/***************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. GPS Software                    *
 *                                                                         *
 *  Copyright (c) 2008-2009 by SiRF Technology, Inc.  All rights reserved. *
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
 * FILENAME:  sirf_codec_csv.c
 *
 * DESCRIPTION: Functions to encode and decode LPL Command messages to/from
 *              a CSV string and a C structure.
 *
 ***************************************************************************/
#include <string.h>
#include <stdio.h>

#include "sirf_codec_csv.h"
#include "string_sif.h"
#include "limits.h"

/***************************************************/
/* Definitions                                     */
/***************************************************/
#define MAX_LEN 80

#ifndef MIN
#define MIN(a,b)   (((a) < (b)) ? (a) : (b))
#endif

/***************************************************/
/* Import CSV helper functions                     */
/***************************************************/

/** 
 * Advance past the next whitespace or comma.  
 *
 * Specifically look for and advance until the next comma, space, carrage return
 * line feed, or the null char.
 *
 * useful if you need to advance past an omitted parameter such as ",,"
 * 
 * @param pSrc pointer to string pointer to advance
 * @param MaxLen length of the string pointed to by the string pointer
 * 
 * @return number of bytes passed looking for the comma or whitespace
 */
tSIRF_UINT32 AdvancePastNextComma( tSIRF_UINT8 **pSrc, 
                                   tSIRF_UINT32 MaxLen )
{
   tSIRF_UINT32 len = 0;
   char c  = (char)**pSrc;
   
   while ( (c != ',') && (c != ' ') && (c != '\n') && (c != '\r') && (c != '\0') && (len < MaxLen) )
   {
      *pSrc += 1;
      len   += 1;
      c = **pSrc;
   }

   if (',' ==  c)   /* Advance 1 beyond the comma */
   {
      *pSrc += 1;
      len   += 1;
   }
   return len;
}

/** 
 * Imports the next integer value from a string.
 *
 * This is the UINT8 flavor.  
 *
 * Note importing a value larger than a UINT8 will saturate the value to 2^8-1.
 * 
 * @param Dst pointer to where to place the imported value
 * @param pSrc pointer to string pointer to import the value from and advance
 * 
 * @return number of bytes advanced in the string
 */
tSIRF_UINT32 CSV_ImportUINT8( tSIRF_UINT8 *Dst, tSIRF_UINT8 **pSrc )
{
   unsigned int value;
   if (1 == sscanf((char*)*pSrc, "%15u", &value))
   {
      if (value > UCHAR_MAX)
      {
         value = UCHAR_MAX;
      }
      *Dst = (tSIRF_UINT8) value;
      return AdvancePastNextComma(pSrc, MAX_LEN);
   }
   else
   {
      return 0;
   }
}

/** 
 * Imports the next integer value from a string.
 *
 * This is the INT16 flavor.  
 *
 * Note importing a value larger than a INT16 will saturate the value to 
 * +2^15-1 or -2^15+1
 * 
 * @param Dst pointer to where to place the imported value
 * @param pSrc pointer to string pointer to import the value from and advance
 * 
 * @return number of bytes advanced in the string
 */
tSIRF_UINT32 CSV_ImportINT16( tSIRF_INT16 *Dst, tSIRF_UINT8 **pSrc )
{ 
   int value;
   if (1 == sscanf( (char *)*pSrc, "%15d", &value))
   {
      if (SHRT_MAX < value)
      {
         value = SHRT_MAX;
      }
      else if (SHRT_MIN > value)
      {
         value = SHRT_MIN;
      }
      *Dst = (tSIRF_INT16)value;
      return AdvancePastNextComma( pSrc, MAX_LEN);
   }
   else
   {
      return 0;
   }
}

/** 
 * Imports the next integer value from a string.
 *
 * This is the UINT16 flavor.  
 *
 * Note: importing values larger than UINT16 will saturate the value to 2^16-1.
 * 
 * @param Dst pointer to where to place the imported value
 * @param pSrc pointer to string pointer to import the value from and advance
 * 
 * @return number of bytes advanced in the string
 */
tSIRF_UINT32 CSV_ImportUINT16(tSIRF_UINT16 *Dst, tSIRF_UINT8 **pSrc )
{
   unsigned int value;
   if (1 == sscanf((char*)*pSrc, "%15u", &value))
   {
      if (USHRT_MAX < value)
      {
         value = USHRT_MAX;
      }

      *Dst = (tSIRF_UINT16) value;
      return AdvancePastNextComma(pSrc, MAX_LEN);
   }
   else
   {
      return 0;
   }
}

/** 
 * Imports the next integer value from a string.
 *
 * This is the INT32 flavor.  
 *
 * @param Dst pointer to where to place the imported value
 * @param pSrc pointer to string pointer to import the value from and advance
 * 
 * @return number of bytes advanced in the string
 */
tSIRF_UINT32 CSV_ImportINT32( tSIRF_INT32 *Dst, tSIRF_UINT8 **pSrc )
{
   if (1 == sscanf( (char *)*pSrc, "%15ld", Dst))
   {
      return AdvancePastNextComma( pSrc, MAX_LEN);
   }
   else
   {
      return 0;
   }
}

/** 
 * Imports the next integer value from a string.
 *
 * This is the UINT32 flavor.  
 *
 * @param Dst pointer to where to place the imported value
 * @param pSrc pointer to string pointer to import the value from and advance
 * 
 * @return number of bytes advanced in the string
 */
tSIRF_UINT32 CSV_ImportUINT32( tSIRF_UINT32 *Dst, tSIRF_UINT8 **pSrc )
{
   if (1 == sscanf( (char *)*pSrc, "%15lu", Dst))
   {
      return AdvancePastNextComma( pSrc, MAX_LEN);
   }
   else
   {
      return 0;
   }
}

/** 
 * Imports the next integer value from a string.
 *
 * This is the UINT32 flavor that expects a hex number.  
 *
 * @param Dst pointer to where to place the imported value
 * @param pSrc pointer to string pointer to import the value from and advance
 * 
 * @return number of bytes advanced in the string
 */
tSIRF_UINT32 CSV_ImportUINT32Hex( tSIRF_UINT32 *Dst, tSIRF_UINT8 **pSrc )
{
   if (1 == sscanf( (char *)*pSrc, "%8lx", Dst))
   {
      return AdvancePastNextComma( pSrc, MAX_LEN);
   }
   else
   {
      return 0;
   }
}

/** 
 * Imports the next integer value from a string.
 *
 * This is the INT64 flavor.  
 *
 * @param Dst pointer to where to place the imported value
 * @param pSrc pointer to string pointer to import the value from and advance
 * 
 * @return number of bytes advanced in the string
 */
tSIRF_UINT32 CSV_ImportINT64( tSIRF_INT64 *Dst, tSIRF_UINT8 **pSrc )
{
   if (1 == sscanf( (char *)*pSrc, "%15lld", Dst))
   {
      return AdvancePastNextComma( pSrc, MAX_LEN);
   }
   else
   {
      return 0;
   }
}

/** 
 * Imports the next integer value from a string.
 *
 * This is the UINT64 flavor.  
 *
 * @param Dst pointer to where to place the imported value
 * @param pSrc pointer to string pointer to import the value from and advance
 * 
 * @return number of bytes advanced in the string
 */
tSIRF_UINT32 CSV_ImportUINT64( tSIRF_UINT64 *Dst, tSIRF_UINT8 **pSrc )
{
   if (1 == sscanf( (char *)*pSrc, "%15llu", Dst))
   {
      return AdvancePastNextComma( pSrc, MAX_LEN);
   }
   else
   {
      return 0;
   }
}

/** 
 * Imports the next float from a string.
 *
 * This is the FLOAT flavor.  
 *
 * @param Dst pointer to where to place the imported value
 * @param pSrc pointer to string pointer to import the value from and advance
 * 
 * @return number of bytes advanced in the string
 */
tSIRF_UINT32 CSV_ImportFLOAT( tSIRF_FLOAT *Dst, tSIRF_UINT8 **pSrc )
{
   /* IEEE Maxfloat is 3.40282347e38 add one for the decimal place and one
    * for the most significant digit */
   if (1 == sscanf( (char *)*pSrc, "%41f", Dst))
   {
      return AdvancePastNextComma( pSrc, MAX_LEN);
   }
   else
   {
      return 0;
   }
}

/** 
 * Imports the next double from a string.
 *
 * This is the DOUBLE flavor.  
 *
 * @param Dst pointer to where to place the imported value
 * @param pSrc pointer to string pointer to import the value from and advance
 * 
 * @return number of bytes advanced in the string
 */
tSIRF_UINT32 CSV_ImportDOUBLE( tSIRF_DOUBLE *Dst, tSIRF_UINT8 **pSrc )
{
   /* IEEE MaxDouble 1.7976931348623157e308 add one for the decimal place and 
    * one for the most significant digit */
   if (1 == sscanf( (char *)*pSrc, "%311lf", Dst))
   {
      return AdvancePastNextComma( pSrc, MAX_LEN);
   }
   else
   {
      return 0;
   }
}

/** 
 * Imports a string from a CSV string
 * 
 * @param Dst pointer to where to store the imported string
 * @param dst_size size of the storage at Dst
 * @param pSrc pointer to string pointer which contains the string to extract
 * @param src_size size of the string pointed to by the string pointer pSrc
 * 
 * @return number of bytes in pSrc consumed
 */
tSIRF_UINT32 CSV_ImportSTRING( tSIRF_UINT8 *Dst, 
                      tSIRF_UINT16 *dst_size,
                      tSIRF_UINT8 **pSrc, 
                      tSIRF_UINT32 src_size )
{
   tSIRF_UINT32 matched;
   tSIRF_UINT32 len;
   tSIRF_UINT32 len_size;

   /* Get the string length */
   matched = sscanf( (char *)*pSrc, "%15lu,", &len);
   if (matched)
   {
      len_size = AdvancePastNextComma(pSrc,src_size);
      
      /* Figure out exactly how much we can copy in.  This is a 3 way min,
       * if it was done right the answer will be len */
      *dst_size = (tSIRF_UINT16)MIN(len,*dst_size); /* max write buffer */
      *dst_size = (tSIRF_UINT16)MIN(*dst_size,src_size-len_size); /* Max input buffer */
      /* no copy the actual string into the Dst */
      memcpy(Dst,*pSrc,*dst_size);
      len_size += AdvancePastNextComma(pSrc,src_size);
   }
   else
   {
      len_size = 0;
   }
   return len_size;
}


/***************************************************/
/* Export CSV helper functions */
/***************************************************/

/** 
 * Writes a UINT8 in CVS format to a string
 * 
 * @param Src pointer to the data to store
 * @param pDst pointer to the data pointer where to store the CSV string
 * @param size length of the data buffer
 * 
 * @return the number of bytes written to the pDst string
 */
tSIRF_UINT32 CSV_ExportUINT8( tSIRF_UINT8 *Src, tSIRF_UINT8 **pDst, tSIRF_UINT32 size )
{
   tSIRF_UINT32 len;
   snprintf((char*)*pDst,size, "%hhu,", *Src);
   len = strnlen((char*)*pDst,size);
   *pDst += len;
   return len;
}

/** 
 * Writes a UINT16 in CVS format to a string
 * 
 * @param Src pointer to the data to store
 * @param pDst pointer to the data pointer where to store the CSV string
 * @param size length of the data buffer
 * 
 * @return the number of bytes written to the pDst string
 */
tSIRF_UINT32 CSV_ExportINT16( tSIRF_INT16 *Src, tSIRF_UINT8 **pDst, tSIRF_UINT32 size )
{
   tSIRF_UINT32 len;
   snprintf((char*)*pDst,size, "%hd,", *Src);
   len = strnlen((char*)*pDst,size);
   *pDst += len;
   return len;
}

/** 
 * Writes a UINT16 in CVS format to a string
 * 
 * @param Src pointer to the data to store
 * @param pDst pointer to the data pointer where to store the CSV string
 * @param size length of the data buffer
 * 
 * @return the number of bytes written to the pDst string
 */
tSIRF_UINT32 CSV_ExportUINT16( tSIRF_UINT16 *Src, tSIRF_UINT8 **pDst, tSIRF_UINT32 size )
{
   tSIRF_UINT32 len;
   snprintf((char*)*pDst,size, "%hu,", *Src);
   len = strnlen((char*)*pDst,size);
   *pDst += len;
   return len;
}

/** 
 * Writes a UINT32 in CVS format to a string
 * 
 * @param Src pointer to the data to store
 * @param pDst pointer to the data pointer where to store the CSV string
 * @param size length of the data buffer
 * 
 * @return the number of bytes written to the pDst string
 */
tSIRF_UINT32 CSV_ExportINT32( tSIRF_INT32 *Src, tSIRF_UINT8 **pDst, tSIRF_UINT32 size )
{
   tSIRF_UINT32 len;
   snprintf((char*)*pDst,size, "%ld,", *Src);
   len = strnlen((char*)*pDst,size);
   *pDst += len;
   return len;
}

/** 
 * Writes a UINT32 in CVS format to a string
 * 
 * @param Src pointer to the data to store
 * @param pDst pointer to the data pointer where to store the CSV string
 * @param size length of the data buffer
 * 
 * @return the number of bytes written to the pDst string
 */
tSIRF_UINT32 CSV_ExportUINT32( tSIRF_UINT32 *Src, tSIRF_UINT8 **pDst, tSIRF_UINT32 size )
{
   tSIRF_UINT32 len;
   snprintf((char*)*pDst,size, "%lu,", *Src);
   len = strnlen((char*)*pDst,size);
   *pDst += len;
   return len;
}

/** 
 * Writes a INT64 in CVS format to a string
 * 
 * @param Src pointer to the data to store
 * @param pDst pointer to the data pointer where to store the CSV string
 * @param size length of the data buffer
 * 
 * @return the number of bytes written to the pDst string
 */
tSIRF_UINT32 CSV_ExportINT64( tSIRF_INT64 *Src, tSIRF_UINT8 **pDst, tSIRF_UINT32 size )
{
   tSIRF_UINT32 len;
   snprintf((char*)*pDst,size, "%lld,", *Src);
   len = strnlen((char*)*pDst,size);
   *pDst += len;
   return len;
}

/** 
 * Writes a UINT64 in CVS format to a string
 * 
 * @param Src pointer to the data to store
 * @param pDst pointer to the data pointer where to store the CSV string
 * @param size length of the data buffer
 * 
 * @return the number of bytes written to the pDst string
 */
tSIRF_UINT32 CSV_ExportUINT64( tSIRF_UINT64 *Src, tSIRF_UINT8 **pDst, tSIRF_UINT32 size )
{
   tSIRF_UINT32 len;
   snprintf((char*)*pDst,size, "%llu,", *Src);
   len = strnlen((char*)*pDst,size);
   *pDst += len;
   return len;
}

/** 
 * Writes a FLOAT in CVS format to a string
 * 
 * @param Src pointer to the data to store
 * @param pDst pointer to the data pointer where to store the CSV string
 * @param size length of the data buffer
 * 
 * @return the number of bytes written to the pDst string
 */
tSIRF_UINT32 CSV_ExportFLOAT( tSIRF_FLOAT *Src, tSIRF_UINT8 **pDst, tSIRF_UINT32 size )
{
   tSIRF_UINT32 len;
   snprintf((char*)*pDst,size, "%f,", *Src);
   len = strnlen((char*)*pDst,size);
   *pDst += len;
   return len;
}

/** 
 * Writes a DOUBLE in CVS format to a string
 * 
 * @param Src pointer to the data to store
 * @param pDst pointer to the data pointer where to store the CSV string
 * @param size length of the data buffer
 * 
 * @return the number of bytes written to the pDst string
 */
tSIRF_UINT32 CSV_ExportDOUBLE( tSIRF_DOUBLE *Src, tSIRF_UINT8 **pDst, tSIRF_UINT32 size )
{
   tSIRF_UINT32 len;
   snprintf((char*)*pDst,size, "%lf,", *Src);
   len = strnlen((char*)*pDst,size);
   *pDst += len;
   return len;
}


/** 
 * String format is a 16bit integer followed by a string followed by a comma
 * 
 * @param Src       String to export
 * @param src_size  size of string to export
 * @param pDst      buffer to export into
 * @param dst_size  sizeof pDst buffer
 * 
 * @return nuber of bytes writeen into pDst and by how much pDst was advanced
 */
tSIRF_UINT32 CSV_ExportSTRING( tSIRF_UINT8 *Src, 
                      tSIRF_UINT16 src_size,
                      tSIRF_UINT8 **pDst, 
                      tSIRF_UINT32 dst_size)
{
   tSIRF_UINT32 bytes=0;
   tSIRF_UINT32 len;

   /* First figure out how long the lenght string will be */
   snprintf((char*)*pDst,dst_size,"%u,",src_size);
   bytes = strnlen((char*)*pDst,dst_size);
   /* Now figure out how long the total string will be, +1 for the comma */
   len = bytes + src_size + 1;
   if ( len < dst_size )
   {
      snprintf((char*)(*pDst)+bytes, dst_size - bytes,"%s,", Src);
      bytes = len;
      *pDst += bytes;
   }
   else
   {
      tSIRF_UINT32 len_size = 0;
      tSIRF_UINT32 final_size;
      /* this loop will run at most 3 times, first with len_size = 0
         next with len_size = to the string length of the final size
         and the finally if necessary len_size will be one more because
         there was a didget lost.  For example take a dst_size of 100.  The
         the loops would be:
         Iteratioin 1:
            final_size = 100;
            len_size = 4;
         Iteration 2:
            final_size = 96;
            len_size = 3;
         Iteration 3 (final):
            Final_size = 97;
            len_size = 3;
      */
         
      do
      {
         final_size = dst_size - len_size;
         /* now the hard part, figure out how much will fit and re-do it */
         snprintf((char*)*pDst,dst_size,"%lu,",final_size);
         len_size = strnlen((char*)*pDst,dst_size);
      } while (final_size + len_size != dst_size);
      /* Now print what we can */
      snprintf((char*)(*pDst)+len_size, dst_size - len_size - 1,"%s", Src);
      snprintf((char*)(*pDst)+dst_size -1,1,",");
      bytes = dst_size;
      *pDst += dst_size;
   }

   return bytes;
}

/**
 * @}
 */
