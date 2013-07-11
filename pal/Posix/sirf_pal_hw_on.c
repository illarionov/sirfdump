/**
 * @addtogroup sirf_pal_posix
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
 * @file   sirf_pal_hw_reset.c
 *
 * @brief  Tracker reset control module.
 */
#include <stddef.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#if defined(OS_LINUX) && defined(PHYTEC_LPC3180)
#include <linux/gpio.h>
#endif

#include "sirf_types.h"
#include "sirf_pal.h"
#include "sirf_pal_hw_on.h"
#include "sirf_threads.h"/*add by weixing*/

#ifdef OS_ANDROID
#define  LOG_TAG  "gps_sirf"
#include <cutils/log.h>
#endif

#if defined(OS_ANDROID) && defined(NEO_FREERUNNER)
#define ON_OFF_GPIO "/sys/bus/platform/devices/neo1973-pm-gps.0/ss0_power_on"
#endif

#if defined(OS_LINUX) && defined(PHYTEC_LPC3250)
#define GPIO_ONOFF 14

struct gpio_control
{
   unsigned int pin;
   unsigned int arg;
};
#endif

/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */
/* A global handler for on/off port to control ON_OFF pin */
tSIRF_HANDLE OnOffPortHandle = NULL;
extern tSIRF_HANDLE ResetPortHandle;

/**
 * @brief Open and Init the ON_OFF port handle
 *
 * @param[in] level           Initial line value
 * @return                    SIRF_FAILURE or SIRF_SUCCESS.
 */
tSIRF_RESULT SIRF_PAL_HW_OpenON_OFF( tSIRF_UINT32 level )
{
   tSIRF_RESULT result = SIRF_FAILURE;

   OnOffPortHandle = NULL;
    LOGE("gps Open ON_OFF level: %d", level);

   /* Call appropriate function based on how the ON_OFF line is configured */
   switch ( g_userPalConfig.on_off_line_usage )
   {
      case UI_CTRL_MODE_HW_ON_UART_RTS:
         OnOffPortHandle = ResetPortHandle;
         result = SIRF_PAL_HW_WriteON_OFF( level );
         break;

      case UI_CTRL_MODE_HW_ON_NONE:
      case UI_CTRL_MODE_HW_ON_GPIO:
      case UI_CTRL_MODE_HW_ON_UART_RX:
         result = SIRF_PAL_HW_WriteON_OFF(level);
         result = SIRF_SUCCESS;
         break;

      default:
         result = SIRF_FAILURE;
         break;
   }

   return result;
}

/**
 * @brief Close the ON_OFF port handle
 *
 * @return                    SIRF_FAILURE or SIRF_SUCCESS.
 */
tSIRF_RESULT SIRF_PAL_HW_CloseON_OFF( void )
{
   /* This just returns a SUCCESS as RESET and ON_OFF lines use the same COM port */
   /* Do not need to check line usage as we are returning SUCCESS */
   return SIRF_SUCCESS;
}

/**
 * @brief Set the ON_OFF line
 * @param[in] level              SIRF_PAL_HW_ON_HIGH or SIRF_PAL_HW_ON_LOW
 * @return                       SIRF_SUCCESS or SIRF_FAILURE
 */
tSIRF_RESULT SIRF_PAL_HW_WriteON_OFF( tSIRF_UINT32 level )
{
   tSIRF_RESULT retVal = SIRF_FAILURE;

      LOGE("gps WriteON ON_OFF level: %d", level);
   /* Call appropriate function based on how the ON_OFF line is configured */
   switch ( g_userPalConfig.on_off_line_usage )
   {
   case UI_CTRL_MODE_HW_ON_UART_RTS:
      {
         if ( SIRF_PAL_HW_ON_HIGH == level )
         {
            retVal = SIRF_PAL_COM_UART_SetRTS( OnOffPortHandle );
         }
         else
         {
            retVal = SIRF_PAL_COM_UART_ClrRTS( OnOffPortHandle );
         }
         break;
      }

   case UI_CTRL_MODE_HW_ON_GPIO:
      {
		#ifdef NS115
		int fd_gpio;
                char on_buff[2] = {'1','0'};
                //fd_gpio = open("sys/class/gpio/gpio18/value", O_RDWR);
                fd_gpio = open("sys/class/gpio/gpio59/value", O_RDWR);
                LOGE("returning fd from GPIO 0x%08x\n", fd_gpio);
                LOGE("GPS \n");
                LOGE("GPS \n");
                LOGE("GPS \n");
                LOGE("GPS \n");
                //LOGE("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC \n");
                if(fd_gpio >= 0)
                {   
    
    
                        if(level == SIRF_PAL_HW_ON_HIGH)
                        {   
    
                                //SIRF_PAL_OS_THREAD_Sleep(10);
                                //cmd |= 0x40;
                                write(fd_gpio,&on_buff[0],1);
                                SIRF_PAL_OS_THREAD_Sleep(10);
                                LOGE("GPS ON IS HIGH\n");
                        }   
                        else
                        {   
                                //cmd &= ~0x40;
                                write(fd_gpio,&on_buff[1],1);
                                LOGE("GPS ON IS LOW\n");
                        }   
                        close(fd_gpio);
                }   
                else
                {   
                        LOGE("%s: could not open gpio for GPS on", __FUNCTION__);
                } 

		#else
                int fd_gpio;
                int ret;
                unsigned char cmd = 0;
                struct RWREGS_PARAM parm;
                parm.start_reg = 0xFC23;
                parm.byte_cnt = 1;
                parm.buf = &cmd;


                fd_gpio = open("/dev/io373x", O_RDWR);
                LOGE("returning fd from GPIO 0x%08x\n", fd_gpio);
                //LOGE("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD \n");
                if(fd_gpio >= 0)
                {
                        cmd = 0;
                        parm.start_reg = 0xFC23;
                        ret = ioctl(fd_gpio, ENEEC_IOC_READ_REGS, &parm);
                        if(ret < 0){
                                LOGE("Read EC 0xFC23 out state error: %d\n", ret);
                        }
                        //LOGE("FC25: 0x%02x\n", (0x10 & cmd));
                        if(level == SIRF_PAL_HW_ON_HIGH)
                        {
                                cmd |= 0x20;
                                LOGE("GPS ON\n");
                        } else
                        {
                                SIRF_PAL_OS_THREAD_Sleep((unsigned long)20);
                                cmd &= ~0x20;
                                LOGE("GPS OFF\n");
                        }
                        ret = ioctl(fd_gpio, ENEEC_IOC_WRITE_REGS, &parm);
                        if(ret < 0)
                        {
                                LOGE("write EC 0xFC23 error: %d\n", ret);
                        } else
                        {
                                retVal = SIRF_SUCCESS;
                        }

                close(fd_gpio);
                        LOGE("closed GPIO for on/off\n");
                }
                else
                {
                        LOGE("%s: could not open EC for GPS on/off", __FUNCTION__);
                }
               #endif


         break;
      }

   case UI_CTRL_MODE_HW_ON_NONE:
      retVal = SIRF_SUCCESS;
      break;

   case UI_CTRL_MODE_HW_ON_UART_RX:
   default:
      LOGE("gps ON_OFF reture FAILURE");
      retVal = SIRF_FAILURE;
      break;
   }

   return retVal;
} /* SIRF_PAL_HW_WriteON_OFF() */


/**
 * @}
 * End of file.
 */


