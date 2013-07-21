/**
 * @addtogroup lpl3.0_cmdr
 * @{
 */

/***************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. GPS Software                    *
 *                                                                         *
 *  Copyright (c) 2009 by SiRF Technology, Inc.  All rights reserved.      *
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
 * FILENAME:  sirf_codec_csv.h
 *
 * DESCRIPTION: Functions to encode and decode LPL Command messages to/from
 *              a CSV string and a C structure.
 *
 ***************************************************************************/
#ifndef _LPL_CMDR_MSG_CSV_H
#define _LPL_CMDR_MSG_CSV_H

#include "sirf_types.h"

/***************************************************/
/* General CSV helper functions                    */
/***************************************************/
tSIRF_UINT32 AdvancePastNextComma( tSIRF_UINT8 **pSrc, tSIRF_UINT32 MaxLen );

/***************************************************/
/* Import CSV helper functions                     */
/***************************************************/
tSIRF_UINT32 CSV_ImportUINT8(     tSIRF_UINT8  *Dst, tSIRF_UINT8 **pSrc );
tSIRF_UINT32 CSV_ImportINT16(     tSIRF_INT16  *Dst, tSIRF_UINT8 **pSrc );
tSIRF_UINT32 CSV_ImportUINT16(    tSIRF_UINT16 *Dst, tSIRF_UINT8 **pSrc );
tSIRF_UINT32 CSV_ImportINT32(     tSIRF_INT32  *Dst, tSIRF_UINT8 **pSrc );
tSIRF_UINT32 CSV_ImportUINT32(    tSIRF_UINT32 *Dst, tSIRF_UINT8 **pSrc );
tSIRF_UINT32 CSV_ImportUINT32Hex( tSIRF_UINT32 *Dst, tSIRF_UINT8 **pSrc );
tSIRF_UINT32 CSV_ImportINT64(     tSIRF_INT64  *Dst, tSIRF_UINT8 **pSrc );
tSIRF_UINT32 CSV_ImportUINT64(    tSIRF_UINT64 *Dst, tSIRF_UINT8 **pSrc );
tSIRF_UINT32 CSV_ImportFLOAT(     tSIRF_FLOAT  *Dst, tSIRF_UINT8 **pSrc );
tSIRF_UINT32 CSV_ImportDOUBLE(    tSIRF_DOUBLE *Dst, tSIRF_UINT8 **pSrc );
tSIRF_UINT32 CSV_ImportSTRING(    tSIRF_UINT8 *Dst, 
                                  tSIRF_UINT16 *dst_size,
                                  tSIRF_UINT8 **pSrc, 
                                  tSIRF_UINT32 src_size);

/***************************************************/
/* Export CSV helper functions */
/***************************************************/
tSIRF_UINT32 CSV_ExportUINT8( tSIRF_UINT8 *Src, tSIRF_UINT8 **pDst, tSIRF_UINT32 size );
tSIRF_UINT32 CSV_ExportINT16( tSIRF_INT16 *Src, tSIRF_UINT8 **pDst, tSIRF_UINT32 size );
tSIRF_UINT32 CSV_ExportUINT16(tSIRF_UINT16 *Src, tSIRF_UINT8 **pDst, tSIRF_UINT32 size );
tSIRF_UINT32 CSV_ExportINT32( tSIRF_INT32 *Src, tSIRF_UINT8 **pDst, tSIRF_UINT32 size );
tSIRF_UINT32 CSV_ExportUINT32(tSIRF_UINT32 *Src, tSIRF_UINT8 **pDst, tSIRF_UINT32 size );
tSIRF_UINT32 CSV_ExportINT64( tSIRF_INT64 *Src, tSIRF_UINT8 **pDst, tSIRF_UINT32 size );
tSIRF_UINT32 CSV_ExportUINT64(tSIRF_UINT64 *Src, tSIRF_UINT8 **pDst, tSIRF_UINT32 size );
tSIRF_UINT32 CSV_ExportFLOAT( tSIRF_FLOAT *Src, tSIRF_UINT8 **pDst, tSIRF_UINT32 size );
tSIRF_UINT32 CSV_ExportDOUBLE(tSIRF_DOUBLE *Src, tSIRF_UINT8 **pDst, tSIRF_UINT32 size);
tSIRF_UINT32 CSV_ExportSTRING( tSIRF_UINT8 *Src, 
                      tSIRF_UINT16 src_size,
                      tSIRF_UINT8 **pDst, 
                      tSIRF_UINT32 dst_size);

#endif /* _LPL_CMDR_MSG_CSV_H */

/**
 * @}
 */

