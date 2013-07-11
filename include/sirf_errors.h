/**
 * @addtogroup platform_src_sirf_include
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2009 - 2010 by SiRF Technology, a CSR plc Company
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
 *  $DateTime: 2011/10/17 14:33:40 $
 *
 *  $Revision: #2 $
 */

/**
 * @file   sirf_errors.h
 *
 * @brief  SiRF Errors.
 *
 */

#ifndef SIRF_ERRORS_H_INCLUDED
#define SIRF_ERRORS_H_INCLUDED


/* ----------------------------------------------------------------------------
 *   Included files
 * ------------------------------------------------------------------------- */
/* library includes */

/* public includes */
#include "sirf_types.h"

/* private includes */

/* local includes */

/* Enter C naming convention */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* ----------------------------------------------------------------------------
 *   Definitions
 * ------------------------------------------------------------------------- */


/* ----------------------------------------------------------------------------
 *    Types, Enums, and Structs
 * ------------------------------------------------------------------------- */

/**
 * @brief      typedef containing SiRF return values.
 *
 * long_description
 *             This typedef enumerates all possible return codes from public
 *             SiRF API's.
 *
 * @pre
 *             This typedef, tSIRF_RESULT is sized as a tSIRF_UINT32 to
 *             guarantee consistency across all platforms.  The enum is
 *             intentionally unnamed to prevent its usage in software (while
 *             providing a convenient mechanism for grouping and enumerating
 *             a list of return codes.)
 *
 * @warning
 *             There are no warnings. tSIRF_RESULT can safely be used across
 *             all platforms.
 *
 */
typedef tSIRF_UINT32 tSIRF_RESULT; enum
{
   SIRF_SUCCESS                              = 0,

   SIRF_FAILURE                              = 1,

   /* -------------------------------------------------------------------------
    *   CLM Errors                           = 0x1xxx
    * ---------------------------------------------------------------------- */
   /* CLM Control Errors                     = 0x10xx */
   CLM_CTRL_ALREADY_STARTED                  = 0x1000,
   CLM_CTRL_ALREADY_STOPPED                  = 0x1001,
   CLM_CTRL_INVALID_MODE                     = 0x1002,
   CLM_CTRL_ERROR_OPENING_PORT               = 0x1003,
   CLM_CTRL_RTOS_ERROR                       = 0x1004,
   CLM_CTRL_MODE_NOT_SPECIFIED               = 0x1005,

   /* SEA File Errors                        = 0x11xx */
   SEA_FILE_OK                               = 0x1100,
   SEA_FILE_ERROR_DECODING                   = 0x1101,
   SEA_FILE_ERROR_ZERO_FILE                  = 0x1102,
   SEA_FILE_ERROR_OVERFLOW                   = 0x1103,
   SEA_FILE_ERROR_IDS_EXCEPTION_CAUGHT       = 0x1104,
   SEA_FILE_ERROR_DOWNLOAD_VERIFY            = 0x1105,
   SEA_FILE_ERROR_REG_DENY                   = 0x1106,
   SEA_FILE_ERROR_DOWNLOAD_DENY              = 0x1107,

   /* -------------------------------------------------------------------------
    *   PAL Errors                           = 0x2xxx,
    * ---------------------------------------------------------------------- */
   /* General PAL Errors                     = 0x20xx */
   SIRF_PAL_OS_ERROR                         = 0x2000,
   SIRF_PAL_OS_SEMAPHORE_WAIT_TIMEOUT        = 0x2001,   /* Return code indicating timeout   */
                                                         /* waiting for semaphore.           */
   SIRF_PAL_API_ERROR                        = 0x2002,   /* API usage error code.  This is   */
                                                         /* returned if a NULL pointer is    */
                                                         /* passed an invalid parameter.     */

   /* General COM Errors                     = 0x21xx */
   SIRF_COM_RET_FAILURE_GENERIC              = 0x2100,   /* generic unknown failure          */
   SIRF_COM_RET_FAILURE_GENERIC_OPEN         = 0x2101,   /* generic open failure             */
   SIRF_COM_RET_FAILURE_GENERIC_READ         = 0x2102,   /* generic read failure             */
   SIRF_COM_RET_FAILURE_GENERIC_WRITE        = 0x2103,   /* generic write failure            */
   SIRF_COM_RET_FAILURE_MUTEX_FAILED         = 0x2104,   /* could not lock/obtain mutex      */
   SIRF_COM_RET_FAILURE_TIMEOUT              = 0x2105,   /* the operation timed out          */
   SIRF_COM_RET_FAILURE_FIFO_FULL            = 0x2106,   /* generic FIFO full failure        */

   /* I2C-Specific Errors                    = 0x22xx */
   SIRF_COM_RET_FAILURE_I2C_ERROR            = 0x2200,
   SIRF_COM_RET_FAILURE_I2C_NO_DEVICES       = 0x2201,
   SIRF_COM_RET_FAILURE_I2C_PORT_BUSY        = 0x2202,
   SIRF_COM_RET_FAILURE_I2C_MASTER_BITRATE   = 0x2203,
   SIRF_COM_RET_FAILURE_I2C_SLAVE_BITRATE    = 0x2204,
   SIRF_COM_RET_FAILURE_I2C_MASTER_CONFIG    = 0x2205,
   SIRF_COM_RET_FAILURE_I2C_SLAVE_CONFIG     = 0x2206,
   SIRF_COM_RET_FAILURE_I2C_MASTER_PULLUP    = 0x2207,
   SIRF_COM_RET_FAILURE_I2C_SLAVE_PULLUP     = 0x2208,
   SIRF_COM_RET_FAILURE_I2C_MASTER_PWR       = 0x2209,
   SIRF_COM_RET_FAILURE_I2C_SLAVE_PWR        = 0x220A,
   SIRF_COM_RET_FAILURE_I2C_SLAVE_ENABLE     = 0x220B,
   SIRF_COM_RET_FAILURE_I2C_TIMEOUT          = 0x220C,
   SIRF_COM_RET_FAILURE_I2C_READ             = 0x220D,
   SIRF_COM_RET_FAILURE_I2C_WRITE            = 0x220E,

   /* SPI-Specific Failures                  = 0x23xx */
   SIRF_COM_RET_FAILURE_SPI_ERROR            = 0x2300,
   SIRF_COM_RET_FAILURE_SPI_ABORT_FAILED     = 0x2301,
   SIRF_COM_RET_FAILURE_SPI_NO_DEVICES       = 0x2302,
   SIRF_COM_RET_FAILURE_SPI_PORT_BUSY        = 0x2303,
   SIRF_COM_RET_FAILURE_SPI_BITRATE          = 0x2304,
   SIRF_COM_RET_FAILURE_SPI_THREAD           = 0x2305,

   /* UART-Specific Failures                 = 0x24xx */
   SIRF_COM_RET_FAILURE_UART_ERROR           = 0x2400,
   SIRF_COM_RET_FAILURE_UART_ABORT_FAILED    = 0x2401,

   /* PAL Frequency Aiding Errors            = 0x25xx */
   SIRF_PAL_HW_FREQ_UPDATE_REQUEST_PENDING   = 0x2500,  /* When request is made and   */
                                                        /* response will be received  */
                                                        /* with call back             */
   SIRF_PAL_HW_FREQ_UPDATE_REQUEST_INVALID_ID= 0x2501,  /* Invalid Frequency transfer */
                                                        /* update Request             */

   /* PAL Logging Errors                     = 0x26xx */
   SIRF_PAL_LOG_ALREADY_OPEN                 = 0x2600,
   SIRF_PAL_LOG_ALREADY_CLOSED               = 0x2601,
   SIRF_PAL_LOG_OPEN_ERROR                   = 0x2602,
   SIRF_PAL_LOG_NOT_OPEN                     = 0x2603,
   SIRF_PAL_LOG_NULL_PARAMETER               = 0x2604,

   /* PAL Network Errors                     = 0x27xx */
   SIRF_PAL_NET_ERROR                        = 0x2700,
   SIRF_PAL_NET_TIMEOUT                      = 0x2701,
   SIRF_PAL_NET_CONNECTION_CLOSED            = 0x2702,
   SIRF_PAL_NET_MAX_SOCKETS_ALLOCATED        = 0x2703,
   SIRF_PAL_NET_SECURITY_NOT_SUPPORTED       = 0x2704,
   SIRF_PAL_NET_SECURITY_VERSION_ERROR       = 0x2705,
   SIRF_PAL_NET_SECURITY_PEER_REFUSED        = 0x2706,
   SIRF_PAL_NET_SECURITY_CERTIFICATE_ERROR   = 0x2707,
   SIRF_PAL_NET_SECURITY_ERROR               = 0x2708,
   SIRF_PAL_NET_CONNECT_INPROGRESS           = 0x2709,
   SIRF_PAL_NET_CONNECT_WOULD_BLOCK          = 0x270A,
   SIRF_PAL_NET_CONNECT_ALREADY              = 0x270B,

   /* PAL Storage Errors                     = 0x28xx */
   SIRF_PAL_STORAGE_NOT_AVAILABLE            = 0x2800,
   SIRF_PAL_STORAGE_FULL                     = 0x2801,
   SIRF_PAL_STORAGE_EXISTS                   = 0x2802,
   SIRF_PAL_STORAGE_ERROR                    = 0x2803,
   SIRF_PAL_STORAGE_LENGTH_TOO_BIG           = 0x2804,
   SIRF_PAL_STORAGE_EMPTY                    = 0x2805,

   /* -------------------------------------------------------------------------
    *   SiRFNav Errors                       = 0x3xxx
    * ---------------------------------------------------------------------- */
   /* SiRFNav_Start(), SiRFNav_Stop() Errors = 0x30xx */
   SIRFNAV_UI_CTRL_ALREADY_STARTED           = 0x3000,
   SIRFNAV_UI_CTRL_ALREADY_STOPPED           = 0x3001,
   SIRFNAV_UI_CTRL_INVALID_MODE              = 0x3002,
   SIRFNAV_UI_CTRL_ERROR_OPENING_PORT        = 0x3003,
   SIRFNAV_UI_CTRL_OS_ERROR                  = 0x3004,
   SIRFNAV_UI_CTRL_INVALID_STORAGE_MODE      = 0x3005,
   SIRFNAV_UI_CTRL_INVALID_PORT_BAUD_RATE    = 0x3006,
   SIRFNAV_UI_CTRL_INVALID_ON_OFF_PORT_NUM   = 0x3007,
   SIRFNAV_UI_CTRL_INVALID_ON_PORT           = 0x3008,
   SIRFNAV_UI_CTRL_INVALID_RESET_PORT        = 0x3009,
   SIRFNAV_UI_CTRL_INVALID_LNA_TYPE          = 0x300A,
   SIRFNAV_UI_CTRL_INVALID_DEBUG_SETTING     = 0x300B,
   SIRFNAV_UI_CTRL_INVALID_RFOVRD_SETTING    = 0x300C,
   SIRFNAV_UI_CTRL_INVALID_IO_PINCONFIG_MODE = 0x300D,
   SIRFNAV_UI_CTRL_INVALID_TRKR_PORT_NUM     = 0x300E,
   SIRFNAV_UI_CTRL_INVALID_TRKR_PORT         = 0x300F,
   SIRFNAV_UI_CTRL_INVALID_HW_FC_SETTING     = 0x3010,
   SIRFNAV_UI_CTRL_INVALID_I2C_ADDRESSES     = 0x3011,
   SIRFNAV_UI_CTRL_INVALID_I2C_BIT_RATE      = 0x3012,
   SIRFNAV_UI_CTRL_INVALID_I2C_MODE          = 0x3013,
   SIRFNAV_UI_CTRL_ERROR_CREATING_COM_THREAD = 0x3014,
   SIRFNAV_UI_CTRL_ERROR_CREATING_MAIN_THREAD= 0x3015,
   SIRFNAV_UI_CTRL_INVALID_SPI_BIT_RATE      = 0x3016,
   SIRFNAV_UI_CTRL_INVALID_ON_OFF_CONTROL    = 0x3017,
   SIRFNAV_UI_CTRL_INVALID_BACKUP_LDO_MODE   = 0x3018,
   SIRFNAV_UI_CTRL_INVALID_PREAMBLE_ENABLE   = 0x3019,
   SIRFNAV_UI_CTRL_INVALID_DRIVER_PREAMBLE   = 0x301A,
   SIRFNAV_UI_CTRL_INVALID_TRACKER_IMAGE     = 0x301B,
   SIRFNAV_UI_CTRL_ERROR_OPENING_TRK_PORT    = 0x301C,
   SIRFNAV_UI_CTRL_ERROR_OPENING_RESET_PORT  = 0x301D,
   SIRFNAV_UI_CTRL_ERROR_OPENING_ON_OFF_PORT = 0x301E,
   SIRFNAV_UI_CTRL_ERROR_INIT_NAV_QUEUE      = 0x301F,
   SIRFNAV_UI_CTRL_ERROR_INIT_MEI_QUEUE      = 0x3020,

   /* SiRFNav_Input() Errors                 = 0x31xx */
   SIRFNAV_UI_IO_ERROR_INVALID_MSG_LENGTH    = 0x3100,
   SIRFNAV_UI_IO_ERROR_QUEUE_ERROR           = 0x3101,
   SIRFNAV_UI_IO_ERROR_NULL_POINTER          = 0x3102,
   SIRFNAV_UI_ERROR_NOT_STARTED              = 0x3103,

   /* -------------------------------------------------------------------------
    *   Utility Errors                       = 0x4xxx
    * ---------------------------------------------------------------------- */
   /* ASCII CODEC Errors                     = 0x40xx */
   SIRF_CODEC_ASCII_UNKNOWN_MESSAGE_ID       = 0x4000,
   SIRF_CODEC_ASCII_BAD_FORMAT               = 0x4001,
   SIRF_CODEC_ASCII_NO_NUMBER                = 0x4002,
   SIRF_CODEC_ASCII_NO_COMMA                 = 0x4003,
   SIRF_CODEC_ASCII_LENGTH_ERROR             = 0x4004,

   /* General CODEC Errors                   = 0x41xx */
   SIRF_CODEC_ERROR_INVALID_MSG_ID           = 0x4100,
   SIRF_CODEC_ERROR_INVALID_MSG_LENGTH       = 0x4101,
   SIRF_CODEC_ERROR_INVALID_PARAMETER        = 0x4102,

   /* SSB CODEC Errors                       = 0x42xx */
   SIRF_CODEC_SSB_NULL_POINTER               = 0x4200,
   SIRF_CODEC_SSB_INVALID_MSG_ID             = 0x4201,
   SIRF_CODEC_SSB_LENGTH_ERROR               = 0x4202,

   /* EXT Aux Errors                         = 0x43xx */
   SIRF_EXT_AUX_ALREADY_RUNNING              = 0x4300,
   SIRF_EXT_AUX_NOT_RUNNING                  = 0x4301,

   /* EXT Log Errors                         = 0x44xx */
   SIRF_EXT_LOG_ALREADY_OPEN                 = 0x4400,
   SIRF_EXT_LOG_ALREADY_CLOSED               = 0x4401,

   /* EXT UART Errors                        = 0x45xx */
   SIRF_EXT_UART_ALREADY_CREATED             = 0x4500,
   SIRF_EXT_UART_NOT_CREATED                 = 0x4501,
   SIRF_EXT_UART_ALREADY_RUNNING             = 0x4502,
   SIRF_EXT_UART_NOT_RUNNING                 = 0x4503,
   SIRF_EXT_UART_UNKNOWN_PROTOCOL            = 0x4504,
   SIRF_EXT_UART_NO_MORE_PORTS_AVAILABLE     = 0x4505,
   SIRF_EXT_UART_PROTOCOL_NOT_SET            = 0x4506,
   SIRF_EXT_UART_SEND_ERROR                  = 0x4507,

   /* EXT TCP Errors                         = 0x46xx */

   /* PROTO General Errors                   = 0x48xx */
   SIRF_PROTO_BUFFER_ERROR                   = 0x4800,
   SIRF_PROTO_NOT_ENOUGH_DATA_YET            = 0x4801,
   SIRF_PROTO_FRAME_ERROR                    = 0x4802,
   SIRF_PROTO_FRAME_CHECKSUM_ERROR           = 0x4803,
   
}  /* tSIRF_RESULT */;

/* ----------------------------------------------------------------------------
 *    Global Variables
 * ------------------------------------------------------------------------- */


/* ----------------------------------------------------------------------------
 *    Function Prototypes
 * ------------------------------------------------------------------------- */


/* Leave C naming convention */
#ifdef __cplusplus
}
#endif /*__cplusplus*/


#endif /* SIRF_ERRORS_H_INCLUDED */

/**
 * @}
 * End of file.
 */
