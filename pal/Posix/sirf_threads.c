/**
 * @addtogroup app
 * @{
 */

 /***************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. GPS Software                    *
 *                                                                         *
 *    Copyright (c) 2009 by SiRF Technology, Inc.  All rights reserved.    *
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
 ***************************************************************************/

/**
 * Thread definition table ----------------------------------------------------
 * Lower-numbered priorities are higher in importance.
 * Same priority must not be assigned to two or more tasks because the priority is a handle.
 * All priorities must be different. This is a constraint by the OS.
 * All of the task priorities are relative to the max assigned in the OS.
 * Leave room at bottom for OS-defined tasks: OS_LOWEST_PRIO - 2 -> OS_LOWEST_PRIO 
 */

#include "sirf_pal_config.h"
#include "sirf_threads.h"
#include "sirfnav_demo_config.h"

tSIRF_THREAD_TABLE   SIRF_THREAD_Table[] =
{
   /**
    * THREAD ID                      PRIORITY                      QUANTUM   STACK SIZE
    */
#ifdef SIRF_LPL
   { SIRFLPL_THREAD_SLC,             31,     0,  0},
   { SIRFLPL_THREAD_NET,             31,     0,  0 },
   { SIRFLPL_THREAD_CP,              30,     0,  0  },
   { SIRFLPL_THREAD_TIMER,           30,     0,  0},   // all changed from 31 to 30
#endif

#ifdef SIRF_HOST
   { SIRFNAV_THREAD_NAV,             28,     0,  0    },  // changing 30 to 29
   { SIRFNAV_THREAD_TRK_COM,         25,     0,  0  },
#endif

#ifdef SIRF_EXT_AUX
   { SIRF_EXT_AUX_THREAD_1,          30,     0,  0  },
#endif
   { SIRFNAV_DEMO_THREAD_EXT1,       30,     0,  0  },      
#if 0
   { SIRFNAV_DEMO_THREAD_EXT_UART1,          30,     0,  0   },
   { SIRFNAV_DEMO_THREAD_EXT_UART2,          30,     0,  0  },
   { SIRFNAV_DEMO_THREAD_EXT_UART3,          30,     0,  0   },

   { SIRFLPL_THREAD_GPRS, 30,     0,  0  },
#endif

#if defined( SIRF_CLM )
   { SIRFINSTANTFIX_THREAD_CGEE,    0,    0,      0 }, /* CGEE thread */
   { SIRFINSTANTFIX_THREAD_SGEE,    0,    0,      0 }, /* SGEE thread */
#endif

   {0,0,0,0}
};



/***************************************************************************
* @brief   Return the size of the thread table
* @return  Size of the thread table
***************************************************************************/
tSIRF_UINT32 SIRF_THREAD_MaxThreads( tSIRF_VOID )
{
   return sizeof(SIRF_THREAD_Table) / sizeof(tSIRF_THREAD_TABLE);
}

/**
 * @}
 */
