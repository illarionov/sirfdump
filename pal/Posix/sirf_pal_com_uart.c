/**
 * @addtogroup platform_src_sirf_pal_posix
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2006-2010 by SiRF Technology, a CSR plc Companyc.
 *    All rights reserved.
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
 *
 *  Keywords for Perforce.  Do not modify.
 *
 *  $File: //customs/customer/Marvell-U1/sirf/Software/sirf/pal/Posix/sirf_pal_com_uart.c $
 *
 *  $DateTime: 2012/02/08 15:09:01 $
 *
 *  $Revision: #5 $
 */
/**
 * @file   sirf_pal_io_uart.c
 *
 * @brief  UART COM for the POSIX version of the SiRF PAL
 *
 * UART COM for the POSIX version of the SiRF PAL
 */

/* ----------------------------------------------------------------------------
 *   Included files
 * ------------------------------------------------------------------------- */
/* library includes */
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

/* public includes */
#include "sirf_pal.h"
#include "sirf_types.h"
#include "gps_logging.h"

/* private includes */

/* local includes */

/* ----------------------------------------------------------------------------
 *   Definitions
 * ------------------------------------------------------------------------- */
#define INVALID_FILE_DESCRIPTOR                  (-1)

#define PORT_IS_STARTED(_uart_settings)                                        \
         ((_uart_settings)->general_settings.port_opened != SIRF_FALSE)


/* ----------------------------------------------------------------------------
 *    Types, Enums, and Structs
 * ------------------------------------------------------------------------- */
typedef struct
{
   int          fd;
   tSIRF_MUTEX  read_mutex;
   tSIRF_MUTEX  write_mutex;
} platform_settings_type;

typedef struct
{
   /* general I/O settings */
   tSIRF_COM_COMMON_SETTINGS general_settings;

   /* uart-specific settings */
   tSIRF_BOOL             flow_control;
   tSIRF_UINT32           baud_rate;

   /* platform-specific (posix) settings */
   platform_settings_type platform_settings;
} com_uart_settings_type;


/* ----------------------------------------------------------------------------
 *    Global Variables
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 *    Local Variables
 * ------------------------------------------------------------------------- */
static com_uart_settings_type my_uart_settings[SIRF_COM_UART_MAX_INSTANCES];
static tSIRF_MUTEX create_delete_mutex;
static tSIRF_BOOL blocking_read = SIRF_FALSE; //TRUE;


/* ----------------------------------------------------------------------------
 *    Function Prototypes
 * ------------------------------------------------------------------------- */
static void init_port_settings(com_uart_settings_type * const uart_settings,
                               tSIRF_HANDLE               logical_handle);

static tSIRF_RESULT  set_baud_rate_and_flow_control(
                               com_uart_settings_type * const uart_settings);

/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------
 *
 *    Function:   init_port_settings
 *
 *    Note:       Initialize the UART settings structure for a given port.
 *
 * ------------------------------------------------------------------------- */
static void init_port_settings(com_uart_settings_type * const uart_settings,
                               tSIRF_HANDLE               logical_handle)
{
   memset(uart_settings, 0, sizeof(com_uart_settings_type));

   /* initialize the general settings */
   uart_settings->general_settings.logical_handle     = logical_handle;
   uart_settings->general_settings.read_byte_timeout  = 10;
   uart_settings->general_settings.read_total_timeout = 100;
   uart_settings->general_settings.read_wait_timeout  = 100;
   uart_settings->general_settings.port_opened        = SIRF_FALSE;

   /* initialize the uart-specific settings */
   uart_settings->baud_rate    = SIRF_COM_UART_BAUD_DEFAULT;
   uart_settings->flow_control = SIRF_COM_UART_FC_DEFAULT;

   /* initialize the platform-specific settings */
   uart_settings->platform_settings.fd          = INVALID_FILE_DESCRIPTOR;
   uart_settings->platform_settings.read_mutex  = NULL;
   uart_settings->platform_settings.write_mutex = NULL;

   return;
}  /* init_port_settings() */

/* ----------------------------------------------------------------------------
 *
 *    Function:   set_baud_rate_and_flow_control
 *
 *    Note:       Set the baud rate and flow control.
 *
 * ------------------------------------------------------------------------- */
static tSIRF_RESULT  set_baud_rate_and_flow_control(com_uart_settings_type * const uart_settings)
{
   /* local variables */
   struct termios   options;
   int              result;
   int              fd;
   int              brate = B115200;

   fd = uart_settings->platform_settings.fd;

   /* Get the current options for the port: */
   result = tcgetattr(fd, &options);
   if (0 != result)
   {
      SIRF_LOGE("error: could not get port attributes; errno=%d", errno);
      return SIRF_COM_RET_FAILURE_UART_ERROR;
   }

   switch (uart_settings->baud_rate)
   {
      case 921600: brate = B921600; break;
      case 460800: brate = B460800; break;
      case 230400: brate = B230400; break;
      case 115200: brate = B115200; break;
      case  57600: brate = B57600;  break;
      case  38400: brate = B38400;  break;
      case  19200: brate = B19200;  break;
      case   9600: brate = B9600;   break;
      case   4800: brate = B4800;   break;
      case   2400: brate = B2400;   break;
      case   1800: brate = B1800;   break;
      case   1200: brate = B1200;   break;
      case   600:  brate = B600;   break;
      case   300:  brate = B300;   break;
      case   150:  brate = B150;   break;
      case   134:  brate = B134;   break;
      case   110:  brate = B110;   break;
      case   75:   brate = B75;   break;
   }

   options.c_cflag = CLOCAL | CREAD | CS8;
   options.c_iflag = IGNPAR;
   options.c_oflag = 0;
   options.c_lflag = 0;

   if (uart_settings->flow_control)
   {
      /* Depending on the Posix/Unix/Linux flavor you may need one of these */
      /* options.c_cflag |= CNEW_RTSCTS; */
      /* Cygwin supports this one */
      options.c_cflag |= CRTSCTS;
   }
   else
   {
      options.c_cflag &= ~CRTSCTS;
   }

   result = cfsetispeed(&options, brate);
   if (0 != result)
   {
      SIRF_LOGE("error: setting input baud rate failed; errno=%d", errno);
      return SIRF_COM_RET_FAILURE_UART_ERROR;
   }

   result = cfsetospeed(&options, brate);
   if (0 != result)
   {
      SIRF_LOGE("error: setting output baud rate failed; errno=%d", errno);
      return SIRF_COM_RET_FAILURE_UART_ERROR;
   }

   options.c_cc[VTIME] = 0;   /* inter-character timer unused */
   options.c_cc[VMIN] = 1; /* blocking read until 1 char received */

   /* Set the new options for the port: */
   result = tcsetattr(fd, TCSANOW, &options);
   if ( 0 != result )
   {
      SIRF_LOGE("error: could not set port attributes; errno=%d", errno);
      return SIRF_COM_RET_FAILURE_UART_ERROR;
   }

   tcflush(fd, TCIOFLUSH);

   return SIRF_SUCCESS;
}


/* ----------------------------------------------------------------------------
 *
 *    Function:   SIRF_PAL_COM_UART_Init
 *
 *    Note:       Initialize UART port settings.
 *
 * ------------------------------------------------------------------------- */
void SIRF_PAL_COM_UART_Init(void)
{
   /* local variables */
   int port_counter;
   tSIRF_RESULT mutex_result;

   /* create the create/delete mutex */
   mutex_result = SIRF_PAL_OS_MUTEX_Create(&create_delete_mutex);
   if (SIRF_SUCCESS == mutex_result)
   {
      for(port_counter = 0; port_counter < SIRF_COM_UART_MAX_INSTANCES; port_counter++)
      {
         init_port_settings(&my_uart_settings[port_counter], SIRF_INVALID_HANDLE);
      }
   }
   else
   {
      SIRF_LOGE("%s: creation create_delete_mutex failed", __FUNCTION__);
   }

   return;

}  /* SIRF_PAL_COM_UART_Init() */

/* ----------------------------------------------------------------------------
 *
 *    Function:   SIRF_PAL_COM_UART_Close
 *
 *    Note:       This function should have the same return type and parameters
 *                as SIRF_PAL_COM_Close_Fcn().
 *
 * ------------------------------------------------------------------------- */
tSIRF_RESULT SIRF_PAL_COM_UART_Close(tSIRF_HANDLE com_handle)
{
   /* local variables */
   tSIRF_RESULT           return_value = SIRF_COM_RET_FAILURE_GENERIC;
   com_uart_settings_type *uart_settings;
   int result;
   //SIRF_LOGD("SIRF_PAL_COM_UART_Close Called");

   /* Validate input parameters */
   if((SIRF_INVALID_HANDLE != com_handle) &&
      (SIRF_COM_UART_MAX_INSTANCES > (int)com_handle))
   {
      /* initialize local variables */
      uart_settings = &my_uart_settings[(int)com_handle];

      result = close(uart_settings->platform_settings.fd);
      //SIRF_LOGD("UART Port Close result =%d", result);
      if(result == 0)
      {
         return_value = SIRF_SUCCESS;
      }
      else
      {
         SIRF_LOGE("UART Port Close Fail!!!");
      }

      uart_settings->platform_settings.fd         = INVALID_FILE_DESCRIPTOR;
      uart_settings->general_settings.port_opened = SIRF_FALSE;
   }
   SIRF_LOGD("rtn V C %d", (int)return_value);
   return(return_value);

}  /* SIRF_PAL_COM_UART_Close() */

/* ----------------------------------------------------------------------------
 *
 *    Function:   SIRF_PAL_COM_UART_Control
 *
 *    Note:       This function should have the same return type and parameters
 *                as SIRF_PAL_COM_Control_Fcn().
 *
 * ------------------------------------------------------------------------- */
tSIRF_RESULT SIRF_PAL_COM_UART_Control(
      tSIRF_HANDLE               com_handle,
      tSIRF_COM_CONTROL          com_control,
      void               * const com_value)
{
   /* local variables */
   tSIRF_RESULT               return_value = SIRF_COM_RET_FAILURE_GENERIC;
   com_uart_settings_type     *uart_settings;

   /* Validate input parameters */
   if((SIRF_INVALID_HANDLE != com_handle) &&
      (SIRF_COM_UART_MAX_INSTANCES > (int)com_handle))
   {
      uart_settings = &my_uart_settings[(int)com_handle];

      switch(com_control)
      {
         /* ----------------------------------------------------------------------
          *   General Settings
          * ------------------------------------------------------------------- */
         case SIRF_COM_CTRL_READ_BYTE_TIMEOUT:
            uart_settings->general_settings.read_byte_timeout = (tSIRF_INT32)com_value;
            return_value = SIRF_SUCCESS;
            break;

         case SIRF_COM_CTRL_READ_TOTAL_TIMEOUT:
            uart_settings->general_settings.read_total_timeout = (tSIRF_INT32)com_value;
            return_value = SIRF_SUCCESS;
            break;

         case SIRF_COM_CTRL_READ_WAIT_TIMEOUT:
            if(blocking_read)
            {
               uart_settings->general_settings.read_wait_timeout = SIRF_TIMEOUT_INFINITE;
            }
            else
            {
               uart_settings->general_settings.read_wait_timeout = (tSIRF_INT32)com_value;
            }
            return_value = SIRF_SUCCESS;
            break;

         /* ----------------------------------------------------------------------
          *   UART-Specific Settings
          * ------------------------------------------------------------------- */
         case SIRF_COM_CTRL_UART_BAUD_RATE:
            uart_settings->baud_rate = (tSIRF_UINT32)com_value;
            if(PORT_IS_STARTED(uart_settings))
            {
               return_value = set_baud_rate_and_flow_control(uart_settings);
            }
            else
            {
               return_value = SIRF_SUCCESS;
            }
            break;

         case SIRF_COM_CTRL_UART_FLOW_CONTROL:
            uart_settings->flow_control = (tSIRF_UINT32)com_value;
            if(PORT_IS_STARTED(uart_settings))
            {
               return_value = set_baud_rate_and_flow_control(uart_settings);
            }
            else
            {
               return_value = SIRF_SUCCESS;
            }
            break;

         case SIRF_COM_CTRL_READ_BLOCKING:
            blocking_read = (tSIRF_BOOL)com_value;
            break;

         default:
            /* an unhandled/unknown setting was commanded.  do nothing */
            DEBUGMSG(1, (DEBUGTXT("Unhandled UART control setting: %d\n"), (tSIRF_UINT32)com_control));
            break;
      }
   }

   return(return_value);

}  /* SIRF_PAL_COM_UART_Control() */

/* ----------------------------------------------------------------------------
 *
 *    Function:   SIRF_PAL_COM_UART_Create
 *
 *    Note:       This function should have the same return type and parameters
 *                as SIRF_PAL_COM_Create_Fcn().
 *
 *    Warning:    As written, this function is not thread-safe.
 *
 * ------------------------------------------------------------------------- */
tSIRF_RESULT SIRF_PAL_COM_UART_Create(
      tSIRF_HANDLE       * const com_handle)
{
   /* local variables */
   tSIRF_RESULT return_value = SIRF_COM_RET_FAILURE_GENERIC;
   int          port_counter;
   tSIRF_RESULT mutex_result;

   if(NULL == com_handle)
   {
      DEBUGMSG(1, (DEBUGTXT("%s: invalid input parameters\n"), __FUNCTION__));
      return(SIRF_COM_RET_FAILURE_GENERIC);
   }

   *com_handle = SIRF_INVALID_HANDLE;

   /* ----------------------------------------------------------------------
    *   1. lock the create/delete mutex
    * ------------------------------------------------------------------- */
   mutex_result = SIRF_PAL_OS_MUTEX_Enter(create_delete_mutex);
   if(SIRF_SUCCESS != mutex_result)
   {
      DEBUGMSG(1, (DEBUGTXT("Uart Create Mutex Error on line %d\n"), __LINE__));
      return_value = SIRF_COM_RET_FAILURE_MUTEX_FAILED;
   }
   else
   {
      /* ----------------------------------------------------------------------
       *   2. search for an unused port
       * ------------------------------------------------------------------- */
      for(port_counter = 0; port_counter < SIRF_COM_UART_MAX_INSTANCES; port_counter++)
      {
         if(SIRF_INVALID_HANDLE == my_uart_settings[port_counter].general_settings.logical_handle)
         {
            /* we found an unused port, reserve it */
            init_port_settings(&my_uart_settings[port_counter], (tSIRF_HANDLE)port_counter);

            /* create the read and write mutexes */
            mutex_result = SIRF_PAL_OS_MUTEX_Create(&my_uart_settings[port_counter].platform_settings.read_mutex);

            if(SIRF_SUCCESS == mutex_result)
            {
               mutex_result = SIRF_PAL_OS_MUTEX_Create(&my_uart_settings[port_counter].platform_settings.write_mutex);

               if(SIRF_SUCCESS == mutex_result)
               {
                  *com_handle  = my_uart_settings[port_counter].general_settings.logical_handle;
                  return_value = SIRF_SUCCESS;
               }
               else
               {
                  /* creating write mutex failed, delete the read */
                  (void)SIRF_PAL_OS_MUTEX_Delete(&my_uart_settings[port_counter].platform_settings.read_mutex);
               }
            }

            break;
         }
      } /* for */

      /* ----------------------------------------------------------------------
       *   3. unlock the create/delete mutex
       * ------------------------------------------------------------------- */
      (void)SIRF_PAL_OS_MUTEX_Exit(create_delete_mutex);
   } /* else */

   return(return_value);

}  /* SIRF_PAL_COM_UART_Create() */

/* ----------------------------------------------------------------------------
 *
 *    Function:   SIRF_PAL_COM_UART_Delete
 *
 *    Note:       This function should have the same return type and parameters
 *                as SIRF_PAL_COM_Delete_Fcn().
 *
 * ------------------------------------------------------------------------- */
tSIRF_RESULT SIRF_PAL_COM_UART_Delete(
      tSIRF_HANDLE       * const com_handle)
{
   /* local variables */
   tSIRF_RESULT            return_value = SIRF_COM_RET_FAILURE_GENERIC;
   com_uart_settings_type  *uart_settings;
   tSIRF_RESULT            result_read;
   tSIRF_RESULT            result_write;
   tSIRF_RESULT            mutex_result;

   /* Validate input parameters */
   if((NULL != com_handle) &&
      (SIRF_INVALID_HANDLE != *com_handle) &&
      (SIRF_COM_UART_MAX_INSTANCES > (int)*com_handle))
   {
      uart_settings = &my_uart_settings[(int)*com_handle];

      /* ----------------------------------------------------------------------
       *   1. lock the create/delete mutex
       * ------------------------------------------------------------------- */
      mutex_result = SIRF_PAL_OS_MUTEX_Enter(create_delete_mutex);
      if(SIRF_SUCCESS != mutex_result)
      {
         DEBUGMSG(1, (DEBUGTXT("Uart Create Mutex Error on line %d\n"), __LINE__));
         return_value = SIRF_COM_RET_FAILURE_MUTEX_FAILED;
      }
      else
      {
         /* ----------------------------------------------------------------------
          *   2. delete the read/write mutexes
          * ------------------------------------------------------------------- */
         result_read = SIRF_PAL_OS_MUTEX_Delete(uart_settings->platform_settings.read_mutex);
         result_write = SIRF_PAL_OS_MUTEX_Delete(uart_settings->platform_settings.write_mutex);

         /* invalidate the platform settings */
         init_port_settings(uart_settings, SIRF_INVALID_HANDLE);
         *com_handle = SIRF_INVALID_HANDLE;

         if ((SIRF_SUCCESS == result_read) && (SIRF_SUCCESS == result_write))
         {
            return_value = SIRF_SUCCESS;
         }

         /* ----------------------------------------------------------------------
          *   3. unlock the create/delete mutex
          * ------------------------------------------------------------------- */
         (void)SIRF_PAL_OS_MUTEX_Exit(create_delete_mutex);
      }
   }

   return(return_value);

}  /* SIRF_PAL_COM_UART_Delete() */

/* ----------------------------------------------------------------------------
 *
 *    Function:   SIRF_PAL_COM_UART_Open
 *
 *    Note:       This function should have the same return type and parameters
 *                as SIRF_PAL_COM_Open_Fcn().
 *
 * ------------------------------------------------------------------------- */
tSIRF_RESULT SIRF_PAL_COM_UART_Open(
      tSIRF_HANDLE               com_handle,
      char         const * const com_port)
{
  /* local variables */
   tSIRF_RESULT            return_value = SIRF_COM_RET_FAILURE_GENERIC;
   com_uart_settings_type  *uart_settings;
   int                     fd;

   //SIRF_LOGD("SIRF_PAL_COM_UART_Open Called : %s    \n", com_port);
   if((SIRF_INVALID_HANDLE != com_handle) &&
      (SIRF_COM_UART_MAX_INSTANCES > (int)com_handle))
   {
      uart_settings = &my_uart_settings[(int)com_handle];

      if(PORT_IS_STARTED(uart_settings))
      {
         DEBUGMSG(1, (DEBUGTXT("%s: port is already started\n"), __FUNCTION__));
         return_value = SIRF_COM_RET_FAILURE_GENERIC_OPEN;
         SIRF_LOGD("UART port is already started");
      }
      else
      {
         /* open the port: */
         SIRF_LOGE("UART PORT:%s\n", com_port);
         fd = open( com_port, O_RDWR | O_NOCTTY );
         if ( fd < 0 )
         {
            DEBUGMSG(1, (DEBUGTXT("error: could not open port; errno=%d\n"), errno));
            SIRF_LOGE("UART Port Open Fail!!!!");
            return_value = SIRF_COM_RET_FAILURE_GENERIC_OPEN;
         }
         else
         {
            uart_settings->platform_settings.fd = fd;
            return_value = set_baud_rate_and_flow_control (uart_settings);
            if (SIRF_SUCCESS == return_value)
            {
               uart_settings->general_settings.port_opened = SIRF_TRUE;
               //SIRF_LOGD("UART Port Open Success fd = %d!!!!", fd);
            }
         }
      }
   }
   SIRF_LOGD("rtn V O %d", (int)return_value);

   return(return_value);

}  /* SIRF_PAL_COM_UART_Open() */

/* ----------------------------------------------------------------------------
 *
 *    Function:   SIRF_PAL_COM_UART_Read
 *
 *    Note:       This function should have the same return type and parameters
 *                as SIRF_PAL_COM_Read_Fcn().
 *
 * ------------------------------------------------------------------------- */
tSIRF_RESULT SIRF_PAL_COM_UART_Read(
      tSIRF_HANDLE               com_handle,
      tSIRF_UINT8        * const read_ptr,
      tSIRF_UINT32               requested_bytes_to_read,
      tSIRF_UINT32       * const actual_bytes_read)
{
   /* local variables */
   com_uart_settings_type  * uart_settings;
   tSIRF_RESULT            return_value = SIRF_COM_RET_FAILURE_GENERIC;
   tSIRF_RESULT            pal_result;
   fd_set                  read_set;
   struct timeval          read_timeout;
   int                     fd;
   int                     os_result;

   /* Validate input parameters */
   if((SIRF_INVALID_HANDLE != com_handle) &&
      (SIRF_COM_UART_MAX_INSTANCES > (int)com_handle) &&
      (NULL != read_ptr) &&
      (0 < requested_bytes_to_read) &&
      (NULL != actual_bytes_read))
   {
      uart_settings    = &my_uart_settings[(int)com_handle];

      /* -------------------------------------------------------------------------
       *   1.  ensure that the port has been started
       * ---------------------------------------------------------------------- */
      if(!(PORT_IS_STARTED(uart_settings)))
      {
         return_value = SIRF_COM_RET_FAILURE_GENERIC_OPEN;
      }
      else
      {
         /* ----------------------------------------------------------------------
          *   2.  lock the read mutex
          * ------------------------------------------------------------------- */
         pal_result = SIRF_PAL_OS_MUTEX_Enter(uart_settings->platform_settings.read_mutex);
         if(SIRF_SUCCESS != pal_result)
         {
            DEBUGMSG(1, (DEBUGTXT("Uart Read Mutex Error on line %d\n"), __LINE__));
            return_value = SIRF_COM_RET_FAILURE_MUTEX_FAILED;
            SIRF_LOGE("Failed to take mutex in UART read\n" );
         }
         else
         {
            /* -------------------------------------------------------------------
             *   3.  wait for a byte to be received,
             *       or wait timeout exceeded,
             *       or the port to be closed
             * ---------------------------------------------------------------- */
            fd = uart_settings->platform_settings.fd;
            if(0 > fd)
            {
               return_value = SIRF_COM_RET_FAILURE_GENERIC;
            }
            else
            {
               FD_ZERO(&read_set);
               FD_SET(fd, &read_set);

               if(uart_settings->general_settings.read_wait_timeout == (tSIRF_INT32)SIRF_TIMEOUT_INFINITE)
               {
                  os_result = select(fd + 1, &read_set, NULL, NULL, NULL);
               }
               else
               {
                  read_timeout.tv_sec  = (uart_settings->general_settings.read_wait_timeout / 1000);
                  read_timeout.tv_usec = (uart_settings->general_settings.read_wait_timeout % 1000) * 1000;
                  os_result = select(fd + 1, &read_set, NULL, NULL, &read_timeout);
               }
               if (0 > os_result)
               {
                  DEBUGMSG(1,(DEBUGTXT("SIRF_PAL_COM_UART_Read: error in select; errno=%d\n"), errno));
                  return_value = SIRF_COM_RET_FAILURE_UART_ERROR;
                  SIRF_LOGE("select fail - errno = %d\n", errno);
               }
               else if (0 == os_result)
               {
                  return_value = SIRF_COM_RET_FAILURE_TIMEOUT;
               }
               else
               {
                  os_result = read( fd, read_ptr, requested_bytes_to_read );
                  if(0 > os_result)
                  {
                     DEBUGMSG(1, (DEBUGTXT("%s: read failed with errno %d\n"), __FUNCTION__, errno));
                     return_value = SIRF_COM_RET_FAILURE_UART_ERROR;
                     *actual_bytes_read = 0;
                     SIRF_LOGE("failed to read bytes from UART %d", os_result );
                  }
                  else
                  {
                     return_value = SIRF_SUCCESS;
                     *actual_bytes_read = os_result;
                     //SIRF_LOGD("UART Read Bytes = %d", os_result);
                  }
               }

               /* -------------------------------------------------------------------
                *   5.  unlock the mutex
                * ---------------------------------------------------------------- */
               (void)SIRF_PAL_OS_MUTEX_Exit(uart_settings->platform_settings.read_mutex);
            }
         }
      }
   }

   return(return_value);

}  /* SIRF_PAL_COM_UART_Read() */

/* ----------------------------------------------------------------------------
 *
 *    Function:   SIRF_PAL_COM_UART_Write
 *
 *    Note:       This function should have the same return type and parameters
 *                as SIRF_PAL_COM_Write_Fcn().
 *
 * ------------------------------------------------------------------------- */
tSIRF_RESULT SIRF_PAL_COM_UART_Write(
      tSIRF_HANDLE               com_handle,
      tSIRF_UINT8  const * const write_ptr,
      tSIRF_UINT32               requested_bytes_to_write,
      tSIRF_UINT32       * const actual_bytes_written)
{
   /* local variables */
   com_uart_settings_type  *uart_settings;
   tSIRF_RESULT            return_value = SIRF_COM_RET_FAILURE_GENERIC;
   tSIRF_UINT32            bytes_written = 0;
   tSIRF_RESULT            pal_result;
   int                     os_result;
   int                     fd;

   /* Validate input paramters */
   if((SIRF_INVALID_HANDLE != com_handle) &&
      (SIRF_COM_UART_MAX_INSTANCES > (int)com_handle) &&
      (NULL != write_ptr) &&
      (0 < requested_bytes_to_write) &&
      (NULL != actual_bytes_written))
   {
      uart_settings    = &my_uart_settings[(int)com_handle];

      /* ----------------------------------------------------------------------
       *   1.  ensure that the port has been started
       * ------------------------------------------------------------------- */
      if(!(PORT_IS_STARTED(uart_settings)))
      {
         return_value = SIRF_COM_RET_FAILURE_GENERIC_OPEN;
      }
      else
      {
         /* ----------------------------------------------------------------------
          *   2.  lock the write mutex
          * ------------------------------------------------------------------- */
         pal_result = SIRF_PAL_OS_MUTEX_Enter(uart_settings->platform_settings.write_mutex);
         if(SIRF_SUCCESS != pal_result)
         {
            DEBUGMSG(1, (DEBUGTXT("Uart Write Mutex Error on line %d\n"), __LINE__));
            return_value = SIRF_COM_RET_FAILURE_MUTEX_FAILED;
         }
         else
         {
            fd = uart_settings->platform_settings.fd;
            if(0 > fd)
            {
               return_value = SIRF_COM_RET_FAILURE_GENERIC;
            }
            else
            {
               do
               {
                  os_result = write(fd, write_ptr+bytes_written, requested_bytes_to_write-bytes_written);
                  if(0 > os_result)
                  {
                     DEBUGMSG(1, (DEBUGTXT("%s: write failed with errno %d\n"), __FUNCTION__, errno));
                     SIRF_LOGE("UART Write Failed with error %d", errno);
                     return_value = SIRF_COM_RET_FAILURE_GENERIC_WRITE;
                     break;
                  }
                  else
                  {
                     fsync(fd);
                     bytes_written += os_result;
                     //SIRF_LOGD("UART Write Success : %d bytes", (int)bytes_written);
                  }
               }while ( bytes_written < requested_bytes_to_write && PORT_IS_STARTED(uart_settings) );

               if(bytes_written == requested_bytes_to_write)
               {
                  return_value = SIRF_SUCCESS;
               }
            }

            /* -------------------------------------------------------------------
             *   5.  unlock the mutex
             * ---------------------------------------------------------------- */
            (void)SIRF_PAL_OS_MUTEX_Exit(uart_settings->platform_settings.write_mutex);
         }
      }
   }

   *actual_bytes_written = bytes_written;

   return(return_value);

}  /* SIRF_PAL_COM_UART_Write() */

/* ============================================================================
 * Functions below are used to reset a GSD4t tracker only.
 * They are not used for any communication handshake. */

/**
 * @brief Clear serial port RTS.
 * @param[in] port_handle              Serial port handle to drive.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_COM_UART_ClrRTS( tSIRF_HANDLE com_handle )
{
   int fd;
   int sercmd, serstat;
   int result;
   com_uart_settings_type *uart_settings;
   tSIRF_RESULT return_value = SIRF_COM_RET_FAILURE_GENERIC;

   if((SIRF_INVALID_HANDLE != com_handle) &&
      (SIRF_COM_UART_MAX_INSTANCES > (int)com_handle))
   {
      uart_settings = &my_uart_settings[(int)com_handle];
      fd = uart_settings->platform_settings.fd;

      if (0 > fd)
      {
         return SIRF_COM_RET_FAILURE_GENERIC;
      }
      sercmd = TIOCM_RTS;

      result = ioctl(fd, TIOCMBIC, &sercmd); // Reset the RTS pin
      if(0 > result)
      {
         return SIRF_COM_RET_FAILURE_UART_ERROR;
      }
      /* Read the RTS pin status. */
      result = ioctl(fd, TIOCMGET, &serstat);
      if(0 > result)
      {
         return SIRF_COM_RET_FAILURE_UART_ERROR;
      }

      if (!(serstat & TIOCM_RTS) )
      {
         return_value =  SIRF_SUCCESS;
      }
      else
      {
         return_value = SIRF_COM_RET_FAILURE_UART_ERROR;
      }
   }

   return return_value;
} /* SIRF_PAL_COM_UART_ClrRTS() */

/**
 * @brief Set serial port RTS.
 * @param[in] port_handle              Serial port handle to drive.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_COM_UART_SetRTS( tSIRF_HANDLE com_handle )
{
   int fd;
   int sercmd, serstat;
   int retval;
   com_uart_settings_type *uart_settings;
   tSIRF_RESULT result = SIRF_COM_RET_FAILURE_GENERIC;

   if((SIRF_INVALID_HANDLE != com_handle) &&
      (SIRF_COM_UART_MAX_INSTANCES > (int)com_handle))
   {
      uart_settings = &my_uart_settings[(int)com_handle];
      fd = uart_settings->platform_settings.fd;

      if (0 > fd)
      {
         return SIRF_COM_RET_FAILURE_GENERIC;
      }
      sercmd = TIOCM_RTS;

      retval = ioctl(fd, TIOCMBIS, &sercmd);
      if(0 > retval)
      {
         return SIRF_COM_RET_FAILURE_UART_ERROR;
      }
      /* Read the RTS pin status. */
      retval = ioctl(fd, TIOCMGET, &serstat);
      if(0 > retval)
      {
         return SIRF_COM_RET_FAILURE_UART_ERROR;
      }

      if (serstat & TIOCM_RTS)
      {
         result = SIRF_SUCCESS;
      }
      else
      {
         result = SIRF_COM_RET_FAILURE_UART_ERROR;
      }
   }

   return result;
} /* SIRF_PAL_COM_UART_SetRTS() */

/**
 * @brief Clear serial port DTR.
 * @param[in] port_handle              Serial port handle to drive.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_COM_UART_ClrDTR( tSIRF_HANDLE com_handle )
{

   int fd;
   int sercmd, serstat;
   int retval;
   com_uart_settings_type *uart_settings;
   tSIRF_RESULT result = SIRF_COM_RET_FAILURE_GENERIC;

   if((SIRF_INVALID_HANDLE != com_handle) &&
      (SIRF_COM_UART_MAX_INSTANCES > (int)com_handle))
   {
      uart_settings = &my_uart_settings[(int)com_handle];
      fd = uart_settings->platform_settings.fd;
      if (0 > fd)
      {
         return SIRF_COM_RET_FAILURE_GENERIC;
      }

      sercmd = TIOCM_DTR;
      retval = ioctl(fd, TIOCMBIC, &sercmd); // Reset the RTS pin
      if(0 > retval)
      {
         return SIRF_COM_RET_FAILURE_UART_ERROR;
      }

      /* Read the DTR pin status. */
      retval = ioctl(fd, TIOCMGET, &serstat);
      if(0 > retval)
      {
         return SIRF_COM_RET_FAILURE_UART_ERROR;
      }

      if (!(serstat & TIOCM_DTR) )
      {
         result = SIRF_SUCCESS;
      }
      else
      {
         result = SIRF_COM_RET_FAILURE_UART_ERROR;
      }
   }

   return result;
} /* SIRF_PAL_COM_UART_ClrDTR() */

/**
 * @brief Set serial port DTR.
 * @param[in] port_handle              Serial port handle to drive.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_COM_UART_SetDTR( tSIRF_HANDLE com_handle )
{

   int fd;
   int sercmd, serstat;
   int retval;
   com_uart_settings_type *uart_settings;
   tSIRF_RESULT result = SIRF_COM_RET_FAILURE_GENERIC;

   if((SIRF_INVALID_HANDLE != com_handle) &&
      (SIRF_COM_UART_MAX_INSTANCES > (int)com_handle))
   {
      uart_settings = &my_uart_settings[(int)com_handle];
      fd = uart_settings->platform_settings.fd;
      if (0 > fd)
      {
         return SIRF_COM_RET_FAILURE_GENERIC;
      }

      sercmd = TIOCM_DTR;
      retval = ioctl(fd, TIOCMBIS, &sercmd);
      if(0 > retval)
      {
         return SIRF_COM_RET_FAILURE_UART_ERROR;
      }

      /* Read the DTR pin status. */
      retval = ioctl(fd, TIOCMGET, &serstat);
      if(0 > retval)
      {
         return SIRF_COM_RET_FAILURE_UART_ERROR;
      }

      if (serstat & TIOCM_DTR)
      {
         result = SIRF_SUCCESS;
      }
      else
      {
         result = SIRF_COM_RET_FAILURE_UART_ERROR;
      }
   }

   return result;
} /* SIRF_PAL_COM_UART_SetDTR() */

/**
 * @}
 * End of file.
 */
