/**
 * @addtogroup platform_src_sirf_pal
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2006- 2011 by SiRF Technology, a CSR plc Company.
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
 * @file   sirf_pal_storage.h
 *
 * @brief  SiRF PAL storage API.
 */

#ifndef SIRF_PAL_STORAGE_H_INCLUDED
#define SIRF_PAL_STORAGE_H_INCLUDED

#include "sirf_errors.h"
#include "sirf_types.h"
#ifdef EMB_SIF
#include <stddef.h>
#include "clm_storage.h"
#endif

/* ----------------------------------------------------------------------------
 *    Preprocessor Definitions
 * ------------------------------------------------------------------------- */

#ifdef EMB_SIF
/* SiRF PAL storage return codes. */
#define SIRF_PAL_STORAGE_NOT_AVAILABLE   0x2210
#define SIRF_PAL_STORAGE_FULL            0x2211
#define SIRF_PAL_STORAGE_EXISTS          0x2212
#define SIRF_PAL_STORAGE_ERROR           0x2213
#define SIRF_PAL_STORAGE_LENGTH_TOO_BIG  0x2214

#define BBRAM_SGEE_HEADER_SIZE           (offsetof(tCLM_STORAGE_HEADER, epochFF4Header) + offsetof(tCLM_FF4_EPOCH_HDR,initEEToe) + 2)//360
#endif
typedef enum
{
   SIRF_PAL_STORAGE_BBRAM,  /* for BBRAM */
   SIRF_PAL_STORAGE_FLASH,  /* for FLASH */
   SIRF_PAL_STORAGE_PATCH,  /* for Host PM */
#ifdef EMB_SIF
   SIRF_PAL_STORAGE_CLM_SGEE, /* for SGEE */
   SIRF_PAL_STORAGE_CLM_SGEE_CLOCK, /* for SGEE Clock*/
   SIRF_PAL_STORAGE_CLM_CGEE, /* for CGEE */
   SIRF_PAL_STORAGE_CLM_BE,   /* for BE */
   SIRF_PAL_STORAGE_CLM_HEADER,/* for CLM */
   SIRF_PAL_STORAGE_LOCAL_EE_FILE,    /* for LOCAL EE FILE used by EE Downloader */
#else   
   SIRF_PAL_STORAGE_CLM,    /* for CLM */
   SIRF_PAL_STORAGE_LOCAL_EE_FILE, /* for Local CLM File */
   SIRF_PAL_STORAGE_LPL,    /* for LPL */
   SIRF_PAL_STORAGE_FIRMWARE, /* for tracker image file */
   SIRF_PAL_STORAGE_CLM_SGEE, /* for SGEE */
   SIRF_PAL_STORAGE_CLM_SGEE_CLOCK, /* for SGEE Clock*/
   SIRF_PAL_STORAGE_CLM_CGEE, /* for CGEE */
   SIRF_PAL_STORAGE_CLM_BE,   /* for BE */
   SIRF_PAL_STORAGE_CLM_HEADER,    /* for CLM */
   SIRF_PAL_ECLM_CONFIG_FILE,    /* for Config File used by ECLM Host App */
#endif
#ifdef SIRF_HOST
   SIRF_PAL_STORAGE_SFSTATE,     /* SVD SF State collection structure */
#endif /* SIRF_HOST */
   SIRF_PAL_STORAGE_MAX
}SirfPalStorageEnum;

#ifdef EMB_SIF
typedef struct
{
   tSIRF_UINT8 sgee_header[BBRAM_SGEE_HEADER_SIZE];
}tBBRAM_sgee_header;
#endif

/* ----------------------------------------------------------------------------
 *    Function Prototypes
 * ------------------------------------------------------------------------- */



/* Enter C naming convention */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

tSIRF_RESULT SIRF_PAL_STORAGE_Open(  tSIRF_UINT32 storage_id );
tSIRF_RESULT SIRF_PAL_STORAGE_Close( tSIRF_UINT32 storage_id );

tSIRF_RESULT SIRF_PAL_STORAGE_Write( tSIRF_UINT32 storage_id, tSIRF_UINT32 offset, tSIRF_UINT8 *data, tSIRF_UINT32 length );
tSIRF_RESULT SIRF_PAL_STORAGE_Read(  tSIRF_UINT32 storage_id, tSIRF_UINT32 offset, tSIRF_UINT8 *data, tSIRF_UINT32 length );
/* Leave C naming convention */
#ifdef __cplusplus
}
#endif /*__cplusplus*/



#endif /* !SIRF_PAL_STORAGE_H_INCLUDED */

/**
 * @}
 * End of file.
 */


