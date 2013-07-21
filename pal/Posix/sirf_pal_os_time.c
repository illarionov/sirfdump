/**
 * @addtogroup platform_src_sirf_pal_posix
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
 * @file   sirf_pal_os_time.c
 *
 * @brief  SiRF PAL time module.
 */

#include <stdio.h>
#include <time.h>

#include "sirf_types.h"
#include "sirf_pal.h"
#include "gps_logging.h"
#include "CpaClient.h"

/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */



/**
 * @brief Get the current time from the system in milliseconds.
 * @return                    Relative milliseconds counter.
 */
tSIRF_UINT32 SIRF_PAL_OS_TIME_SystemTime( tSIRF_VOID )
{
   struct timespec time;

   if ( clock_gettime( CLOCK_MONOTONIC, &time ) != 0 )
      return 0;

   return ( time.tv_sec * 1000L + time.tv_nsec / 1000000L );

} /* SIRF_PAL_OS_TIME_SystemTime() */


/**
 * @brief Get the real-time clock value.
 * @param[out] date_time      Structure to populate with the RTC values.
 * @return                    Result code.
 */
tSIRF_RESULT SIRF_PAL_OS_TIME_DateTime( tSIRF_DATE_TIME *date_time )
{
   struct tm local;
   struct timespec ts;

   DEBUGCHK(date_time);

   if ( clock_gettime( CLOCK_REALTIME, &ts ) != 0 )
      return SIRF_FAILURE;
   
   local = *localtime(&ts.tv_sec);
   
   date_time->year         = 1900+local.tm_year;
   date_time->month        = 1+local.tm_mon;
   date_time->day          = local.tm_mday;
   date_time->minute       = local.tm_min;
   date_time->hour         = local.tm_hour;
   date_time->second       = local.tm_sec;
   date_time->milliseconds = ts.tv_nsec / 1000000L;

   return SIRF_SUCCESS;

} /* SIRF_PAL_OS_TIME_DateTime() */

tSIRF_RESULT SIRF_PAL_OS_TIME_UTCDateTime( tSIRF_DATE_TIME *date_time )
{
  time_t rawtime = {0};
  struct tm* utctime = 0;

  time ( &rawtime );

  utctime = gmtime(&rawtime);

  date_time->year        = (tSIRF_UINT16) utctime->tm_year + 1900;
  date_time->month       = (tSIRF_UINT8)  utctime->tm_mon + 1;
  date_time->day         = (tSIRF_UINT8)  utctime->tm_mday;
  date_time->minute      = (tSIRF_UINT8)  utctime->tm_min;
  date_time->hour        = (tSIRF_UINT8)  utctime->tm_hour;
  date_time->second      = (tSIRF_UINT8)  utctime->tm_sec;
  date_time->milliseconds = (tSIRF_UINT16)0;
  	

  return SIRF_SUCCESS;
}

#define GPS_MINUS_UTC 15  /* UTC Offset as of Dec.29 2008 */

static void convertUtcToGpsTime(tSIRF_DATE_TIME   *utcTime, 
                             tSIRF_UINT16      *weekno,
                             tSIRF_INT32      *timeOfWeek)
{
   tSIRF_UINT32 jy = utcTime->year;
   tSIRF_UINT32 jm = utcTime->month;
   tSIRF_UINT32 JD = 0;

   *weekno     = 0;
   *timeOfWeek = 0;

   /* Check added since some PAL implementaion were not returning the correct values */
   if ( (utcTime->month >= 1 && utcTime->month <= 12) &&
        (utcTime->year >= 1970) &&
        (utcTime->day >= 1 && utcTime->day <= 31) && 
        (utcTime->hour <= 23) &&
        (utcTime->minute <= 59) &&
        (utcTime->second <= 59) )
   {
      if (utcTime->month <= 2)
      {
         jy = utcTime->year - 1;
         jm = utcTime->month + 12;
      }
      /* Calculate Number of days since 6jan1980 (Total seconds since 6Jan0000 - Total seconds until 6Jan1980) ?????*/
      JD = (tSIRF_UINT32) ((tSIRF_UINT32)(365.25 * jy) + (tSIRF_UINT32)(30.6001 * (jm + 1)) + utcTime->day - 723263);
      /* Calculate GPS Week Number */
      *weekno     = (tSIRF_UINT16) (JD / 7.0);
      /* Calculate total seconds since 6Jan1980 ?????*/
      *timeOfWeek = (JD - (*weekno * 7)) * (60 * 60 * 24) + 
      3600 * utcTime->hour+ 60 * utcTime->minute + utcTime->second + GPS_MINUS_UTC;
   }
}

tSIRF_RESULT SIRF_PAL_OS_TIME_RTCRead( tSIRF_UINT16 *weekno, tSIRF_INT32 *timeOfWeek )
{
   (void)weekno;
   (void)timeOfWeek;
   AGPS_NITZ_STATUS_MSG nitzStatusMsg;
   tSIRF_RESULT rc = SIRF_FAILURE;
   int ret = 0;
   char sign;
    tSIRF_DATE_TIME utcTime;

   nitzStatusMsg.ret_code = 0;

   if ((ret == 0) &&
	   (nitzStatusMsg.ret_code == 0))
   {
      rc = SIRF_SUCCESS;

	  if (nitzStatusMsg.time_zone)
	  {
		  sign = '+';
	  }
	  
	  else
	  {
		  sign = '-';
	  }

	  SIRF_LOGD("[%s]: Received NITZ info: %02d/%02d/%02d %02d:%02d:%02d UTC. Local TZ=UTC%c%02d", __FUNCTION__,
          (int) nitzStatusMsg.year,
          (int) nitzStatusMsg.month,
          (int) nitzStatusMsg.day,
          (int) nitzStatusMsg.hour,
          (int) nitzStatusMsg.minute,
          (int) nitzStatusMsg.sec,
	   	 sign,
          (int) (nitzStatusMsg.time_zone / 4));

	  SIRF_LOGD("[%s]: NITZ Uncertainty: %d sec", __FUNCTION__, (int) nitzStatusMsg.uncertainty);


      utcTime.year = nitzStatusMsg.year + 2000;
      utcTime.month = nitzStatusMsg.month;
      utcTime.day = nitzStatusMsg.day;
      utcTime.hour = nitzStatusMsg.hour + (nitzStatusMsg.time_zone / 4);
      utcTime.minute = nitzStatusMsg.minute;
      utcTime.second = nitzStatusMsg.sec;
      utcTime.milliseconds = 0;
            
      convertUtcToGpsTime(&utcTime, weekno, timeOfWeek);
      
      SIRF_LOGD("[%s]: GpsWeek %d, GpsTimeOWeek: %d", __FUNCTION__, *weekno, (int) *timeOfWeek);

      rc = SIRF_SUCCESS;
   }

   return rc;
}


/**
 * @}
 * End of file.
 */


