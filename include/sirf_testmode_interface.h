/***************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. GPS Software                    *
 *                                                                         *
 *    Copyright (c) 2010 by SiRF Technology, a CSR plc Company.            *
 *    All rights reserved.                                                 *
 *                                                                         *
 *    This Software is protected by United States copyright laws and       *
 *    international treaties.  You may not reverse engineer, decompile     *
 *    or disassemble this Software.                                        *
 *                                                                         *
 *    WARNING:                                                             *
 *    This Software contains SiRF Technology Inc.s confidential and        *
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,    *
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED      *
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this      *
 *    Software without SiRF Technology, Inc.s  express written             *
 *    permission.   Use of any portion of the contents of this Software    *
 *    is subject to and restricted by your signed written agreement with   *
 *    SiRF Technology, Inc.                                                *
 *                                                                         *
 **************************************************************************/

 #define TEST_MODE_5 0
 #define TEST_MODE_7 1

 typedef struct
 {
        int               CompleteFlag;
        float             CN0;
        float             gps_tow;         /* gps time of week */
        tSIRF_UINT16      bit_synch_time;  /**< time to first bit synch	*/
        float             cno_mean;      /**< c/No mean in 0.1 dB-Hz */
        float             cno_sigma;     /**< c/No sigma in 0.1 dB */
        float             clock_drift;	 /**< clock drift in 0.1 Hz  */
        float             clock_offset;	 /**< clock offset in 0.1 Hz */
	 
        tSIRF_INT32       abs_i20ms;     /**< phase noise estimate I20ms sum */
        tSIRF_INT32       abs_q20ms;     /**< phase noise estimate Q20ms sum */
	 
        float        phase_lock;    /**< phase lock indicator. LSB = 0.001 */
        tSIRF_UINT16 rtc_frequency; 	 /**<  RTC Frequency. Unit: Hz */
        tSIRF_UINT16 e_to_acq_ratio;	 /**<  ECLK to ACQ Clock ratio	*/
        tSIRF_UINT8  t_sync_agc_gain;	 /**<  Tsync and AGC Gain value */
        tSIRF_UINT8  tm_5_ready;         /**<  Ready for TM5 switch? >= 0x80 */
 
 } SiRFGpsTestModeResult;


 typedef void (* gps_testmode_result_func)(SiRFGpsTestModeResult *);
 
 /** Callback structure for the TestMode interface. */
 typedef struct {
		 gps_testmode_result_func testmode_result_cb;
 } SiRFGpsTestModeResultCallbacks;
 

 typedef struct {
	 /**
	  * Opens the TestMode interface and provides the callback routines
	  * to the implemenation of this interface.
	  */
	 int  (*init)( SiRFGpsTestModeResultCallbacks* callbacks );
	 /* Test Mode Start of TestmodeSharedLib. */
	 int  (*start)(int mode, int sv_num, int time_interval);

	 int  (*stop)(void);
	 
	 int  (*deinit)( void );
	 

 } SiRFGpsTestModeInterface;






