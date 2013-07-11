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
 * @file   strnlen_sif.c
 *
 * @brief  Safe strlen implmentation.
 */



/* ----------------------------------------------------------------------------
 *   Included files
 * ------------------------------------------------------------------------- */



#include "string_sif.h"



/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */


size_t strnlen(
         const char   * str,
         size_t         size)
{
   size_t len;

   if (str == NULL) return 0;

   for (len = 0; len < size; len++)
   {
      if ('\0' == str[len]) 
      {
         break;
      }
   }

   return len;
}
/**
 * @}
 */


