/**
 * @addtogroup platform_src_sirf_sirfnaviii
 * @{
 */

 /**************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. GPS Software                    *
 *                                                                         *
 *    Copyright (c) 2005 - 2010 by SiRF Technology, a CSR plc Company.     *
 *    All rights reserved.                                                 *
 *                                                                         *
 *    This Software is protected by United States copyright laws and       *
 *    international treaties.  You may not reverse engineer, decompile     *
 *    or disassemble this Software.                                        *
 *                                                                         *
 *    WARNING:                                                             *
 *    This Software contains SiRF Technology Inc.s confidential and       *
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,    *
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED      *
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this      *
 *    Software without SiRF Technology, Inc.s  express written            *
 *    permission.   Use of any portion of the contents of this Software    *
 *    is subject to and restricted by your signed written agreement with   *
 *    SiRF Technology, Inc.                                                *
 *                                                                         *
 ***************************************************************************
 *
 * FILE:  sirfnav_ui_ctrl.h
 *
 ***************************************************************************/

#ifndef __SIRFNAV_UI_CTRL_H__
#define __SIRFNAV_UI_CTRL_H__

#include "sirf_errors.h"
#include "sirf_types.h"

/* SiRFNavIII control interface ============================================ */

/***************************************************************************
*   Control Start Mode Bit Field Definitions - Refer to the ICD
***************************************************************************/
/* start mode */
#define SIRFNAV_UI_CTRL_MODE_AUTO                        0x00000000
#define SIRFNAV_UI_CTRL_MODE_HOT                         0x00000001
#define SIRFNAV_UI_CTRL_MODE_WARM                        0x00000002
#define SIRFNAV_UI_CTRL_MODE_COLD                        0x00000003
#define SIRFNAV_UI_CTRL_MODE_FACTORY                     0x00000004
#define SIRFNAV_UI_CTRL_MODE_TEST                        0x00000005
#define SIRFNAV_UI_CTRL_MODE_FACTORY_XO                  0x00000006

/* storage_mode */
#define SIRFNAV_UI_CTRL_MODE_STORAGE_NONE                0x00
#define SIRFNAV_UI_CTRL_MODE_STORAGE_PERIODIC_ALL        0x01
#define SIRFNAV_UI_CTRL_MODE_STORAGE_ON_DEMAND           0x02
#define SIRFNAV_UI_CTRL_MODE_STORAGE_ON_STOP             0x03

/* uart_hw_fc */
#define SIRFNAV_UI_CTRL_MODE_HW_FLOW_CTRL_OFF            0x00
#define SIRFNAV_UI_CTRL_MODE_HW_FLOW_CTRL_ON             0x01

/* lna_type */
#define SIRFNAV_UI_CTRL_MODE_EXT_LNA_OFF                 0x00
#define SIRFNAV_UI_CTRL_MODE_EXT_LNA_ON                  0x01

/* debug_settings */
#define SIRFNAV_UI_CTRL_MODE_NO_DEBUG                    0x00
#define SIRFNAV_UI_CTRL_MODE_TEXT_ENABLE                 0x01
#define SIRFNAV_UI_CTRL_MODE_RAW_MSG_ENABLE              0x02
#define SIRFNAV_UI_CTRL_MODE_ECHO_DISABLE                0x04
#define SIRFNAV_UI_CTRL_MODE_DISABLE_INSTANT_FIX         0x08  /* Disable SiRF Instant Fix feature */

/* tracker_port_select */
#define SIRFNAV_UI_CTRL_MODE_PORT_SEL_INVALID            0x00
#define SIRFNAV_UI_CTRL_MODE_PORT_SEL_UART               0x01  /* COM port for UART */
#define SIRFNAV_UI_CTRL_MODE_PORT_SEL_I2C                0x02  /* Aardvark for I2C */
#define SIRFNAV_UI_CTRL_MODE_PORT_SEL_SPI                0x03  /* Aardvark for SPI */

/* io_pin_configuration_mode */
#define SIRFNAV_UI_CTRL_MODE_IO_CONFIGURATION_DISABLE    0x00
#define SIRFNAV_UI_CTRL_MODE_IO_CONFIGURATION_ENABLE     0x01

/* i2c_mode */
/* SIRFNAV_UI_CTRL_MODE_I2C_MODE_MULTI_MASTER has to match I2C_DEFAULT_MODE */
#define SIRFNAV_UI_CTRL_MODE_I2C_MODE_MULTI_MASTER       0x01  /* Both Host and Tracker are masters */
#define SIRFNAV_UI_CTRL_MODE_I2C_MODE_HOST_MASTER        0x00  /* Host master, Tracker slave */

/* i2c_rate */
#define SIRFNAV_UI_CTRL_MODE_I2C_RATE_100_KBPS           0x00  /* Standard Mode */
#define SIRFNAV_UI_CTRL_MODE_I2C_RATE_400_KBPS           0x01  /* Fast Mode */
#define SIRFNAV_UI_CTRL_MODE_I2C_RATE_1_MBPS             0x02  /* Fast Mode Plus */
#define SIRFNAV_UI_CTRL_MODE_I2C_RATE_3P4_MBPS           0x03  /* High Speed Mode */

/* spi_rate */
#define SIRFNAV_UI_CTRL_MODE_SPI_RATE_125_KHZ            0x00  /* 125 kHz */
#define SIRFNAV_UI_CTRL_MODE_SPI_RATE_250_KHZ            0x01  /* 250 kHz */
#define SIRFNAV_UI_CTRL_MODE_SPI_RATE_500_KHZ            0x02  /* 500 kHz */
#define SIRFNAV_UI_CTRL_MODE_SPI_RATE_1_MHZ              0x03  /*   1 MHz */
#define SIRFNAV_UI_CTRL_MODE_SPI_RATE_2_MHZ              0x04  /*   2 MHz */
#define SIRFNAV_UI_CTRL_MODE_SPI_RATE_4_MHZ              0x05  /*   4 MHz */
#define SIRFNAV_UI_CTRL_MODE_SPI_RATE_6_MHZ              0x06  /*   6 MHz */
#define SIRFNAV_UI_CTRL_MODE_SPI_RATE_8_MHZ              0x07  /*   8 MHz */

/* On_Off_Control is bit mapped */
#define SIRFNAV_UI_CTRL_ON_OFF_DEFAULT                   0x00
#define SIRFNAV_UI_CTRL_ON_OFF_MAX                       0x3F

/* Bits [2:0] describes On Off Edge */
#define SIRFNAV_UI_CTRL_ON_OFF_EDGE_NONE                 0x00
#define SIRFNAV_UI_CTRL_ON_OFF_EDGE_FE                   0x01
#define SIRFNAV_UI_CTRL_ON_OFF_EDGE_RE                   0x02
#define SIRFNAV_UI_CTRL_ON_OFF_EDGE_RE_FE                0x03
#define SIRFNAV_UI_CTRL_ON_OFF_EDGE_FE_RE                0x04
#define SIRFNAV_UI_CTRL_ON_OFF_EDGE_MASK                 0x07

/* Bits [4:3] describes On Off Usage */
#define SIRFNAV_UI_CTRL_ON_OFF_USE_NONE                  0x00
#define SIRFNAV_UI_CTRL_ON_OFF_USE_GPIO                  0x08
#define SIRFNAV_UI_CTRL_ON_OFF_USE_UART_RX               0x10
#define SIRFNAV_UI_CTRL_ON_OFF_USE_UART_RTS              0x18
#define SIRFNAV_UI_CTRL_ON_OFF_USE_MASK                  0x18
#define SIRFNAV_UI_CTRL_ON_OFF_USE_SHIFT                 3

/* Bits [5] describes On Off Off Enabled */
#define SIRFNAV_UI_CTRL_ON_OFF_OFF_NONE                  0x00
#define SIRFNAV_UI_CTRL_ON_OFF_OFF_ENABLED               0x20

/* flash mode */
#define SIRFNAV_UI_CTRL_FLASH_MODE_DO_NOTHING            0x00
#define SIRFNAV_UI_CTRL_FLASH_MODE_ERASE                 0x01
#define SIRFNAV_UI_CTRL_FLASH_MODE_PROGRAM               0x02

/* preamble settings */
#define SIRFNAV_UI_CTRL_ENABLE_MEI_PREAMBLE              0x10
#define SIRFNAV_UI_CTRL_ENABLE_DRIVER_PREAMBLE           0x20
#define SIRFNAV_UI_CTRL_PREAMBLE_DELAY_MASK              0x0F

/* Default defines */
#define SIRFNAV_UI_CTRL_DEFAULT_BAUD_RATE                115200
#define SIRFNAV_UI_CTRL_DEFAULT_FLOW_CONTROL             SIRFNAV_UI_CTRL_MODE_HW_FLOW_CTRL_OFF
#define SIRFNAV_UI_CTRL_DEFAULT_TRACKER_PORT             "\\\\.\\COM1"
#define SIRFNAV_UI_CTRL_DISABLE_IO_PIN_CONFIG            0x0000
#define SIRFNAV_UI_CTRL_NUM_GPIO_PINS_CONFIG             11         /* Total number of pins in Trkr Config Message */
#define SIRFNAV_UI_CTRL_DEFAULT_PIN0_CONFIG              0x0000     /* pin disabled */
#define SIRFNAV_UI_CTRL_DEFAULT_PIN1_CONFIG              0x0000     /* pin disabled */
#define SIRFNAV_UI_CTRL_DEFAULT_PIN2_CONFIG              0x0004     /* Tsync enabled*/
#define SIRFNAV_UI_CTRL_DEFAULT_PIN3_CONFIG              0x0000     /* pin disabled */
#define SIRFNAV_UI_CTRL_DEFAULT_PIN4_CONFIG              0x0000     /* pin disabled */
#define SIRFNAV_UI_CTRL_DEFAULT_PIN5_CONFIG              0x007C     /* Time mark out enabled with rising edge active*/
#define SIRFNAV_UI_CTRL_DEFAULT_PIN6_CONFIG              0x0000     /* Host port unknown */
#define SIRFNAV_UI_CTRL_DEFAULT_PIN7_CONFIG              0x0000     /* Host port unknown */
#define SIRFNAV_UI_CTRL_DEFAULT_PIN8_CONFIG              0x0000     /* Host port unknown */
#define SIRFNAV_UI_CTRL_DEFAULT_PIN9_CONFIG              0x0000     /* Host port unknown */
#define SIRFNAV_UI_CTRL_DEFAULT_PIN10_CONFIG             0x0000     /* Pin is not available on 4t */
#define SIRFNAV_UI_CTRL_DEFAULT_IDLE_BYTE_WAKEUP_DELAY   0
#define SIRFNAV_UI_CTRL_DEFAULT_MAX_PREAMBLE             0

#define SIRFNAV_UI_CTRL_DEFAULT_REF_CLK_FREQ             16369000   /**< units: Hz */
#define SIRFNAV_UI_CTRL_DEFAULT_REF_CLK_WARMUP_DELAY     1023       /**< units: RTC tick */
#define SIRFNAV_UI_CTRL_DEFAULT_REF_CLK_OFFSET           96250      /**< units: Hz  */
#define SIRFNAV_UI_CTRL_DEFAULT_REF_CLK_UNC              3000       /**< units: ppb */
#define SIRFNAV_UI_CTRL_UNKNOWN_REF_CLK_OFFSET           0x7FFFFFFF /* Clock offset unknown */
#define SIRFNAV_UI_CTRL_UNKNOWN_REF_CLK_UNC              0xFFFFFFFF /* Clock uncertainty unknown */

#define SIRFNAV_UI_CTRL_DEFAULT_I2C_HOST_ADDRESS         0x62
#define SIRFNAV_UI_CTRL_DEFAULT_I2C_TRACKER_ADDRESS      0x60

#define SIRFNAV_UI_CTRL_MODE_BACKUP_LDO_ENABLE           0x01
#define SIRFNAV_UI_CTRL_MODE_BACKUP_LDO_DISABLE          0x00

#define MAX_PORT_NUM_STRING_LENGTH                       128 /* number of chars */

/* IMPORTANT NOTE: If any parameter is added or deleted from this structure then 
   SIRF_CONFIG_SIZE in sirf_msg_ssb.h needs to be updated accordingly */
typedef struct tSIRF_CONFIG_tag
{
   /* Parameters used by Host and Tracker */

   /* MAPPING:  For SiRFNavIII, was start_mode.start_type (bits 3 - 0) (Start Type) */
   /* start_mode */
   tSIRF_UINT32   start_mode;

   /* MAPPING:  For SiRFNavIII, start_mode bit 6 was reserved */

   /* MAPPING:  For SiRFNavIII, was start_mode.me_preamble (bit 7) (Preamble Enable) */

   /* uart_max_preamble_transmissions */
   /* Integer range:  0 through 255 */
   /* This variable indicates the maximum number of MEI tracker preamble wakeup       */
   /* messages to be sent by the tracker.  If zero, then the tracker preamble wakeup  */
   /* is continuously transmitted until acknowledged.  This feature is enabled by the */
   /* uart_idle_byte_wakeup_delay field described below.                             */
   tSIRF_UINT8       uart_max_preamble;  /* Tracker to Host */

   /* uart_idle_byte_wakeup_delay */
   /* Integer range:  0 through 255 */
   /* This variable enables the preamble feature.  Bits 0:3 indicate either the     */
   /* number of idle bytes betweeen MEI level preamble transmissions, or the number */
   /* of milliseconds worth of 0xFF byte transmission preambles to be sent based on */
   /* which type of wakeup is selected in Bits 4:7.  If bit 4 is set, the tracker   */
   /* will transmit MEI protocol level preamble messages.  Bit 5 enables the UART   */
   /* driver level wakeup, where instead of MEI messages, 0xFF is transmitted prior */
   /* to valid message data being transmitted.  An error code will be returned if   */
   /* both bits 4 and 5 are set.                                                    */
   tSIRF_UINT8       uart_idle_byte_wakeup_delay;  /* Tracker to Host */

   /* MAPPING:  For SiRFNavIII, was start_mode bit 8 (Force Clock Offset) but unused in the code */
   /* MAPPING:  For SiRFNavIII, was clock_offset (second parameter in SiRFNav_Start()) */
   /* ref_clk_offset */

   /* Zero means no clock offset (same as ignore clock offset) */
   /* 0xFFFFFFFF means invalid clock offset (clock offset unknown) */
   /* All other values allowed.  Units:  Hz */
   tSIRF_INT32      ref_clk_offset;

   /* MAPPING:  For SiRFNavIII, was start_mode bit 9 (Use Tracker RTC) but always used */

   /* MAPPING:  For SiRFNavIII, was start_mode.me_hw_fc (bit 13) (Hardware Flow Control) */
   /* uart_hw_fc */
   /* When on, both the Host and Tracker must use flow control (4-wire UART) */
   tSIRF_UINT8       uart_hw_fc;

   /* MAPPING:  For SiRFNavIII, was start_mode.lna_type (bit 14) (LNA Type) */
   /* lna_type */
   /* External LNA off automatically implies internal LNA on and visa versa */
   tSIRF_UINT8       lna_type;

   /* MAPPING:  For SiRFNavIII, was start_mode.debugSettings (bit 15) (Text Enable) */
   /* MAPPING:  For SiRFNavIII, was start_mode.debugSettings (bit 16) (Raw Data Enable) */
   /* MAPPING:  For SiRFNavIII, was start_mode.debugSettings (bit 17) (Echo Disable) */

   /* debug_settings */
   /* Note the values are not mutually exclusive hence a bit pattern is used */
   tSIRF_UINT8       debug_settings;

   /* MAPPING:  For SiRFNavIII, was start_mode.tcxo_warmup_delay_in_100ms (bits 22 - 18) (TCXO Warm-up Delay) */
   /* ref_clk_warmup_delay */
   /* All values allowed.  Units:  RTC clock cycles */
   tSIRF_UINT16      ref_clk_warmup_delay;

   /* ref_clk_frequency */
   tSIRF_UINT32      ref_clk_frequency;

   /* MAPPING:  For SiRFNavIII, start_mode bit 23 was reserved */

   /* MAPPING:  For SiRFNavIII, was start_mode bits 29 - 24 (Initial Clock Uncertainty) but not a variable */
   /* MAPPING:  For SiRFNavIII, was start_mode bits 31 - 30 (Initial Clock Uncertainty Units) but not a variable */
   /* ref_clk_uncertainty */
   /* Units:  ppb, so remember 1 ppm is 1000 ppb */
   tSIRF_UINT32      ref_clk_uncertainty;

   /* MAPPING:  For SiRFNavIII, was baud_rate (fourth parameter in SiRFNav_Start()) */
   /* baud_rate */
   /* This variable contains the actual baud rate (e.g. 115200) */
   tSIRF_UINT32      uart_baud_rate;
   tSIRF_UINT32      code_load_baud_rate;

   /* MAPPING:  These are new for SiRFNavIV */

   /* io_pin_configuration_mode */
   tSIRF_UINT8       io_pin_configuration_mode;

   /* io_pin_configuration */
   /* This variable specifies the Tracker GPIO settings */
   tSIRF_UINT16      io_pin_configuration[SIRFNAV_UI_CTRL_NUM_GPIO_PINS_CONFIG];

   /* i2c_host_address */
   tSIRF_UINT16      i2c_host_address;

   /* i2c_tracker_address */
   tSIRF_UINT16      i2c_tracker_address;

   /* i2c_mode */
   tSIRF_UINT8       i2c_mode;

   /* i2c_rate */
   tSIRF_UINT8       i2c_rate;

   /* spi rate */
   tSIRF_UINT8       spi_rate;

   /* On_Off_Control */
   tSIRF_UINT8       on_off_control;

   /* flash_mode */
   tSIRF_UINT8       flash_mode;

   /* MAPPING:  For SiRFNavIII, was start_mode bits 12 - 10 (Storage Mode) but not a variable */
   /* storage_mode */
   tSIRF_UINT8       storage_mode;

   /* MAPPING:  For SiRFNavIII, was port_num (third parameter in SiRFNav_Start()) */
   /* tracker_port */
   /* This string only has meaning if UART selected */
   tSIRF_UINT8       tracker_port[MAX_PORT_NUM_STRING_LENGTH];

   /* tracker_port_select */
   tSIRF_UINT8       tracker_port_select;

   /* enable nav bit aiding or not */
   tSIRF_BOOL        weak_signal_enabled;

   /* enable internal backup LDO mode                                                 */
   /* If enabled, our software will command the internal switcher back to Linear      */
   /* mode under all current draw conditions. The benefit of this mode is no          */
   /* external BOM increase, other than the tank inductor and cap, and full           */
   /* performance with lowest noise floor. Note that if this mode is used, the        */
   /* tank must be placed. The internal hardware FSM (finite state machine) will      */
   /* power up the 4t in switcher mode, after which software takes control and sets   */
   /* it back to linear mode. This mode has no impact to hibernate or standby current */ 
   /* compared to switched mode, but carries an overhead of around 25% in tracking    */
   /* or acquisition mode */
   tSIRF_UINT8      backup_LDO_mode_enabled;

} tSIRF_CONFIG;

/***************************************************************************
 *   SiRFNav_Stop() stop_mode values
***************************************************************************/

#define SIRFNAV_UI_CTRL_STOP_MODE_TERMINATE   (0)  /* All GPS threads will be deleted and     */
                                                   /* communication interface closed.         */
#define SIRFNAV_UI_CTRL_STOP_MODE_SUSPEND     (1)  /* GPS activity will be suspended but GPS  */
                                                   /* threads will not be deleted; tracker    */
                                                   /* communication interface will not be     */
                                                   /* closed                                  */
#define SIRFNAV_UI_CTRL_STOP_MODE_FREEZE      (2)  /* GPS will be notified to make a restart  */
                                                   /* as soon as the CPU is back available.   */
                                                   /* Not any OS system function call nor GPS */
                                                   /* data storage call is made during this   */
                                                   /* SiRFNav_Stop execution                  */

/* UART configuration shared by application and host */
typedef struct tSIRF_PORTCF_tag
{
   tSIRF_UINT32 baud_rate;
   tSIRF_UINT8  flow_control;
}tSIRF_PORTCF;

/* SiRFTrack_Notify opcode values */
#define SIRFTRK_NOTIFY_PORTCF                 (1)

/* SiRFTrack_Control opcode values */
#define SIRFTRK_CTRL_PORTCF_GET               (1)
#define SIRFTRK_CTRL_PORTCF_SET               (2)


/* Prototypes -------------------------------------------------------------- */
#ifdef __cplusplus
extern "C" {
#endif


tSIRF_RESULT SiRFNav_Start(tSIRF_CONFIG *config);

tSIRF_RESULT SiRFNav_Stop(tSIRF_UINT32 stop_mode);

/* Optional API for Sensor support */
/* Unlike the tSIRF_CONFIG parameters, there is no default for sensor information (other than numSensors is zero). */
/* Thus, this routine is essentially a Set routine and is only required if sensors are attached to the Tracker.    */
/* While this routine is from the Tracker, the Host will have something equivalent.  This is just a placeholder at */
/* this time, and the Host naming will have to follow the standard API formats.                                    */
/* EXTR SENS_Config(UINT8 numSensors, tSensorConfigData *configData, UINT8 i2cSpeed); */


#ifdef __cplusplus
}
#endif

#endif /* __SIRFNAV_UI_CTRL_H__ */

/**
 * @}
 */
