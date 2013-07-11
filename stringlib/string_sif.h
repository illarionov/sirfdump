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
 * @file   string_sif.h
 *
 * @brief  String manipulation helper routines.
 *
 * This file contains standard implementations for the strlcpy and strlcat
 * buffer-safe string manipulation routines.
 */

#ifndef STRING_SIF_H_INCLUDED
#define STRING_SIF_H_INCLUDED



/* ----------------------------------------------------------------------------
 *   Included files
 * ------------------------------------------------------------------------- */



#include <string.h>
#include <stdio.h>  /* Needed for snprintf according to the C99 standandard */

#if defined TOOLCHAIN_VC8 || defined TOOLCHAIN_VC6
#define snprintf _snprintf
#endif

#if defined NBA_TEST
extern int nba_truth_enabled;
extern char nba_filename[256];
#endif
/* ----------------------------------------------------------------------------
 *    Function Prototypes
 * ------------------------------------------------------------------------- */



/* Enter C naming convention */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/



/**
 * Returns the length of the input string not including the null termination
 * character '\0'.  Allows for a maximum sized string length for fixed buffers
 * in the event the input string is not null terminated.  This is a "safer"
 * method.
 *
 * @param str Input string pointer.
 * @param size Maximum length of fixed buffer.
 *
 * @return size_t Length of the input string not including NULL terminating
 *    character.
 */
size_t strnlen(
         const char   * str,
         size_t         size);

/**
 * @brief Copy source string to destination.
 *
 * This function copies a string from the source to destination
 * and guarantees:
 *  - The destination string will be null-terminated.
 *  - The destination buffer will not be overrun.
 *
 * This safe function also performs truncation when the source string is longer
 * than the destination buffer.  Truncation is indicated when the return size
 * is larger than the input size - 1.  It is expected that strings to be copied
 * are at most 1 character less than the buffer size, as the last character is
 * reserved for the null termination.  In the event the return length is equal to
 * the input size, this indicates the input string was not null-terminated.
 * Keep in mind this might not be 'strictly' true that the input string is not
 * really terminated, but the check for the null character only goes as far as
 * the input size.
 *
 * To check for truncation:
 * @code
 * if (strlcpy(dst, src, sizeof(dst)) >= sizeof(dst))
 *    printf("Attempted overflow of dst buffer.\n");
 * @endcode
 * Note the use of >= in the comparator above.  The current implementation will
 * not return > size, but this is a useful convention to follow for other
 * implementations and possible future revisions.
 *
 * @param[out] dst      Destination buffer to copy into.
 * @param[in]  src      Source string to copy.
 * @param[in]  size     Size of the destination buffer.
 * @return              Length of string attempted to build.
 *
 */
size_t strlcpy(
#if defined(TOOLCHAIN_VC8) && ( defined(OS_WINCE) || defined(OS_WIN32) || defined (OS_WIN32_MFC) )
         char        * __restrict dst,
         const char  * __restrict src,
#else
         char        *dst,
         const char  *src,
#endif
         size_t       size);

/**
 * @brief Concatenate source string to destination.
 *
 * This function concatenates a string from the source to destination
 * and guarantees:
 *  - The destination string will be null-terminated.
 *  - The destination buffer will not be overrun.
 *
 * Note, this implementation deviates slightly from other common forms in the
 * "guarantee" of null-termination of the destination buffer.  The common
 * implementation does not null_terminate a previously non terminated dst
 * buffer.  That is, if the strnlen function returns a string length of the dst
 * buffer equal to the input size, this function will terminate the buffer.
 * This only happens if dst was not previously terminated.  Thus we truly
 * 'guarantee' that the return buffer is null-terminated.
 *
 * Detecting an attempted overrun may be performed by:
 * @code
 * if (strlcat(dst, src, sizeof(dst)) >= sizeof(dst))
 *    printf("Attempted overflow of dst buffer.\n");
 * @endcode
 *
 * @param[out] dst      Destination buffer to concatenate onto.
 * @param[in]  src      Source string to concatenate.
 * @param[in]  size     Size of the destination buffer.
 * @return              Length of string attempted to build.  In the event a
 *    truncation occurs, i.e. the combined write length is longer than the
 *    destination buffer, the total size is still returned.  Thus the caller can
 *    and should identify a truncation by comparing the returned size to the
 *    input dst buffer size.
 */
size_t strlcat(
#if defined(TOOLCHAIN_VC8) && ( defined(OS_WINCE) || defined(OS_WIN32) || defined (OS_WIN32_MFC) )
         char        * __restrict dst,
         const char  * __restrict src,
#else
         char        *dst,
         const char  *src,
#endif
         size_t       size);



/* Leave C naming convention */
#ifdef __cplusplus
}
#endif /*__cplusplus*/


#endif /* !STRING_SIF_H_INCLUDED */

/**
 * @}
 */


