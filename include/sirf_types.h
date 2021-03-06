/**
 * @addtogroup platform_src_sirf_include
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
 *    This Software contains SiRF Technology Inc.�s confidential and
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this
 *    Software without SiRF Technology, Inc.�s  express written
 *    permission.   Use of any portion of the contents of this Software
 *    is subject to and restricted by your signed written agreement with
 *    SiRF Technology, Inc.
 *
 */

/**
 * @file   sirf_types.h
 *
 * @brief  SiRF standard types and constants.
 */

#ifndef SIRF_TYPES_H_INCLUDED
#define SIRF_TYPES_H_INCLUDED

#ifdef WINCE
#include "wince.h"
#else
#include <stdint.h>
#endif

#include "sirf_host_sdk_version.h"

/* ----------------------------------------------------------------------------
 *   Included files
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 *   SiRF definitions
 * ------------------------------------------------------------------------- */
#define SIRF_INVALID_HANDLE         ((void *)(-1))

#define SIRF_FALSE                  (0)
#define SIRF_TRUE                   (!SIRF_FALSE)

#define SIRF_TIMEOUT_INFINITE       (0xFFFFFFFF)

/* ----------------------------------------------------------------------------
 *   SiRF data types
 * ------------------------------------------------------------------------- */
#define tSIRF_VOID void

typedef unsigned long int       tSIRF_BOOL;
typedef unsigned char           tSIRF_UCHAR;

typedef char                    tSIRF_CHAR;

typedef int8_t                  tSIRF_INT8;
typedef uint8_t                 tSIRF_UINT8;

typedef int16_t                 tSIRF_INT16;
typedef uint16_t                tSIRF_UINT16;

typedef int32_t                 tSIRF_INT32;
typedef uint32_t                tSIRF_UINT32;

typedef double                  tSIRF_DOUBLE;
typedef float                   tSIRF_FLOAT;

typedef void *                  tSIRF_HANDLE;

#if defined(_MSC_VER) && (_MSC_VER < 1400)
#include <basetsd.h>
typedef INT64 tSIRF_INT64;
typedef UINT64 tSIRF_UINT64;
#else
typedef int64_t tSIRF_INT64;
typedef uint64_t tSIRF_UINT64;
#endif

#endif /* !SIRF_TYPES_H_INCLUDED */

/**
 * @}
 * End of file.
 */


