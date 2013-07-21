/**
 * @addtogroup platform_src_sirf_pal
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2005-2009 by SiRF Technology, Inc.  All rights reserved.
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
 * @file   sirf_pal_log.h
 *
 * @brief  SiRF PAL logging API
 */

#ifndef SIRF_PAL_LOG_H_INCLUDED
#define SIRF_PAL_LOG_H_INCLUDED

#include "sirf_errors.h"
#include "sirf_types.h"



/* ----------------------------------------------------------------------------
 *    Preprocessor Definitions
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 *    Function Prototypes
 * ------------------------------------------------------------------------- */

typedef tSIRF_HANDLE tSIRF_LOG_HANDLE;

/* Enter C naming convention */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/


/* Modes for SIRF_PAL_LOG_Open - 32 bit fieldmask */
/* bit0 - OVERWRITE or APPEND                     */
/* bit1 - Reserved for other modes                */
/* bit2 - Reserved for other modes                */
#define SIRF_PAL_LOG_MODE_OVERWRITE  0x0000
#define SIRF_PAL_LOG_MODE_APPEND     0x0001
#define SIRF_PAL_LOG_MODE_MASK       0x0007

tSIRF_RESULT SIRF_PAL_LOG_Open(  tSIRF_CHAR *filename, tSIRF_LOG_HANDLE *log, tSIRF_UINT32 mode );
tSIRF_RESULT SIRF_PAL_LOG_Close( tSIRF_LOG_HANDLE log );
tSIRF_RESULT SIRF_PAL_LOG_Write( tSIRF_LOG_HANDLE log, tSIRF_CHAR *text, tSIRF_UINT32 length );
tSIRF_RESULT SIRF_PAL_LOG_FileSize( tSIRF_LOG_HANDLE log, tSIRF_UINT32* fileSize  );

/* Leave C naming convention */
#ifdef __cplusplus
}
#endif /*__cplusplus*/



#endif /* !SIRF_PAL_LOG_H_INCLUDED */

/**
 * @}
 * End of file.
 */


