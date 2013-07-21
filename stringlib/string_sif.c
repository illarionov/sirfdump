/**
 * @addtogroup string
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2006-2009 by SiRF Technology, Inc.  All rights reserved.
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
 * @file   string_sif.c
 *
 * @brief  String manipulation helper routines.
 *
 * This file contains standard implementations for the strlcpy and strlcat
 * buffer-safe string manipulation routines.
 */



/* ----------------------------------------------------------------------------
 *   Included files
 * ------------------------------------------------------------------------- */



#include "string_sif.h"



/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */

size_t strlcpy(
#if defined(TOOLCHAIN_VC8) && ( defined(OS_WINCE) || defined(OS_WIN32) || defined (OS_WIN32_MFC) )
         char        * __restrict dst,
         const char  * __restrict src,
#else
         char        *dst,
         const char  *src,
#endif
         size_t       size)
{
   size_t src_len, cpy_len;

   /* validate input args.  assuming the user follows truncation checking rules, this
      would at the very least signal truncation, and merit inspection. */
   if (dst == NULL || size == 0)
      return size;

   /* in the event of a NULL input string, the output destination gets and "empty 
      string" null-terminated, but we indicate to the caller truncation. */
   if (src == NULL)
   {
      dst[0] = '\0';
      return size;
   }

   /* Calculate the source string length. */
   src_len = strnlen(src, size);

   /* Calculate how many characters to copy. */
   cpy_len = (src_len <= size - 1) ? src_len : size - 1;

   /* Copy the contents. */
   memcpy(dst, src, cpy_len);
   dst[cpy_len] = '\0';

   /* Return the length of the source data. */
   return src_len;
}

size_t strlcat(
#if defined(TOOLCHAIN_VC8) && ( defined(OS_WINCE) || defined(OS_WIN32) || defined (OS_WIN32_MFC) )
         char        * __restrict dst,
         const char  * __restrict src,
#else
         char        *dst,
         const char  *src,
#endif
         size_t       size)
{
   size_t dst_len, src_len, end_len, write_len;

   /* validate input args.  assuming the user follows truncation checking rules, this
      would at the very least signal truncation, and merit inspection. */
   if (dst == NULL || size == 0)
      return size;

   /* Calculate the source and destination string lengths. */
   dst_len = strnlen(dst, size);

   if (src == NULL)
   {
      /* still check for NULL termination "guarantee" on the output buffer. */
      if (dst_len == size)
         dst[size-1] = '\0';

      /* indicate truncation occured */
      return size;
   }

   /* using size for src doesn't seem strictly correct, but it's safer than no limit. */
   src_len = strnlen(src, size);

   /* Calculate total output destination size  */
   end_len = src_len + dst_len;

   /* in the event the total combined size is longer than the destination buffer,
      we truncate the destination buffer. */
   write_len = (end_len <= size - 1) ? end_len : size - 1;

   /* Copy the contents.  There are 2 cases where this 'if' triggers.  If the
      dst_len == write_len, then we don't need to do any copy.
      If dst_len > write_len: this could only happen if dst is not NULL terminated
      and therefore dst_len == size and then write_len - dst_len == unsigned(-1).
      This would result in a huge buffer overrun. */
   if (dst_len < write_len)
      memcpy(dst + dst_len, src, write_len - dst_len);

   dst[write_len] = '\0';

   return end_len;
}

/**
 * @}
 */


