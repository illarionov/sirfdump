/**
 * @addtogroup platform_src_sirf_pal
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2007 - 2010 by SiRF Technology, a CSR plc Company, Inc.       
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
 *
 *  Keywords for Perforce.  Do not modify.
 *
 *  $File: //customs/customer/Marvell-U1/sirf/Software/sirf/pal/sirf_pal.h $
 *
 *  $DateTime: 2011/07/29 13:26:23 $
 *
 *  $Revision: #1 $
 */

/**
 * @file   sirf_pal.h
 *
 * @brief  SiRF PAL API.
 */

/* Protect against multiple inclusions. */
#ifndef SIRF_PAL_H_INCLUDED
#define SIRF_PAL_H_INCLUDED

typedef struct RWREGS_PARAM
{
        unsigned short start_reg;
        unsigned int byte_cnt;
        unsigned char *buf;
} RWREGS_PARAM; 
                
#define ENEEC_IOC_MAGIC         ('e' + 0x80)
                
#define ENEEC_IOC_READ_REGS     _IOWR(ENEEC_IOC_MAGIC, 13, struct RWREGS_PARAM)
#define ENEEC_IOC_WRITE_REGS    _IOW (ENEEC_IOC_MAGIC, 14, struct RWREGS_PARAM)
                
 
/* ----------------------------------------------------------------------------
 *   Included Files
 * ------------------------------------------------------------------------- */

/* Include the platform specific PAL header file. */
/* Thread declarations --------------------------------------------------------*/
#ifndef PVT_BUILD
#if defined ( OS_WIN32 ) || defined ( OS_WIN32_MFC) || defined ( OS_WINCE )
   #include "win32/sirf_pal_config.h"
#elif defined ( OS_QNX ) || defined ( OS_LINUX ) || defined ( OS_POSIX )
   #include "Posix/sirf_pal_config.h"
#elif defined ( OS_UCOSII )
   #include "ucos/sirf_pal_config.h"
#else
   #error No OS specified
#endif
#endif /* #ifndef PVT_BUILD */

/* Include the individual PAL header files. */
#include "sirf_errors.h"
#include "sirf_pal_os_thread.h"
#include "sirf_pal_os_mem.h"
#include "sirf_pal_os_mutex.h"
#include "sirf_pal_os_semaphore.h"
#include "sirf_pal_os_time.h"
#include "sirf_pal_com.h"
#include "sirf_pal_com_i2c.h"
#include "sirf_pal_com_spi.h"
#include "sirf_pal_com_uart.h"
#include "sirf_pal_storage.h"
#include "sirf_pal_hw_reset.h"
#include "sirf_pal_hw_on.h"
#include "sirf_pal_debug.h"
#include "sirf_pal_log.h"
#include "sirf_pal_tcpip.h"
#include "sirf_threads.h"
#include "sirf_pal_hw_frequency_aiding.h"
#include "sirf_types.h"

/* ----------------------------------------------------------------------------
 *   Platform Definitions
 * ------------------------------------------------------------------------- */

/* Includes and Configuration for SIRFNAV */
#if defined (SIRF_HOST) || defined (SIRFNAV_DEMO ) || defined (SIRF_COMPACT_DEMO)

/* ext_sreset_n_line_usage */
/* Note: The reset port is always expected between the host and 
   the tracker in real-world scenario. However, this option is 
   provided to enable 'soft-reset' option for system test use only. */
#define UI_CTRL_MODE_HW_RESET_NONE               0x00
#define UI_CTRL_MODE_HW_RESET_GPIO               0x01
#define UI_CTRL_MODE_HW_RESET_UART_DTR           0x02
/* on_off_line_usage */
#define UI_CTRL_MODE_HW_ON_NONE                  0x00
#define UI_CTRL_MODE_HW_ON_GPIO                  0x01
#define UI_CTRL_MODE_HW_ON_UART_RX               0x02
#define UI_CTRL_MODE_HW_ON_UART_RTS              0x03
#define UI_CTRL_DEFAULT_ON_OFF_PORT              "\\\\.\\COM2"
#define UI_CTRL_DEFAULT_RESET_PORT               "\\\\.\\COM2"
#define MAX_PORT_NUM_STRING_LENGTH               128

typedef struct tPAL_CONFIG_tag
{
   /* Parameter used by PAL */
   /* on_off_line_usage */
   tSIRF_UINT8       on_off_line_usage;

   /* ext_sreset_n_line_usage */
   tSIRF_UINT8       ext_sreset_n_line_usage;

   /* on_off_port */
   tSIRF_UINT8       on_off_port[MAX_PORT_NUM_STRING_LENGTH];

   /* reset_port */
   tSIRF_UINT8       reset_port[MAX_PORT_NUM_STRING_LENGTH];

} tPAL_CONFIG;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Upon initialization of the Host, all tPAL_CONFIG parameters will be filled with their default values. */
/* The user system must populate g_userPalConfig prior to the first call to SiRFNav_Start() */
extern tPAL_CONFIG g_userPalConfig;

extern tSIRF_UINT8 PAL_ssl_version;
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif 

/* ----------------------------------------------------------------------------
 *   Preprocessor Definitions
 * ------------------------------------------------------------------------- */

/* Enter C naming convention */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * @brief Initialize all PAL subsystems
 *
 * @return SIRF_SUCCESS if successful, SIRF_FAILURE otherwise.
 */
tSIRF_RESULT SIRF_PAL_Init(void);

/**
 * @brief Perform teardown (if necessary) on all PAL subsystems
 *
 * @return SIRF_SUCESS if successful, SIRF_FAILURE otherwise
 */
tSIRF_RESULT SIRF_PAL_Destroy(void);

/* Leave C naming convention */
#ifdef __cplusplus
}
#endif /*__cplusplus*/


#endif /* !SIRF_PAL_H_INCLUDED */

/**
 * @}
 * End of file.
 */
 
