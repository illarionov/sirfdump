/**
 * @addtogroup platform_src_sirf_pal_posix
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2005-2010 by SiRF Technology, a CSR plc Company.
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
 * @file   sirf_pal_os_thread.c
 *
 * @brief  SiRF PAL thread module.
 */

#include <pthread.h> 
#include <string.h>

#if defined(OS_QNX) || defined(OS_LINUX)
   #include <signal.h>
#else
   #include <sys/signal.h>
#endif

#include <errno.h>

#include "sirf_types.h"
#include "sirf_pal.h"

#include "sirf_threads.h"



/* ----------------------------------------------------------------------------
 *    Debugging Defines
 * ------------------------------------------------------------------------- */



/* Debugging definitions (addition to GPS_debug.h) */
/* We want to use our own version of VERIFY; do not change this */

#ifdef VERIFY
   #undef VERIFY
#endif /* VERIFY */

#ifdef DEBUG
   #define VERIFY(x) DEBUGCHK((x))
#else
   #define VERIFY(x) ((void)(x))
#endif /* DEBUG */

/* This purpose of this function is to interrupt a blocking system call */
extern tSIRF_VOID (*GPS_Syscall_Intr_Fn)(tSIRF_VOID *param);

/* ----------------------------------------------------------------------------
 *    Functions
 * ------------------------------------------------------------------------- */

static void sighandler (int signal)
{
   (void)signal;

   return;
}

static void SIRF_PAL_OS_THREAD_Send_Signal(void *param)
{
   pthread_t thread_handle = (pthread_t)param;

   pthread_kill(thread_handle, SIGUSR1);
   
   return;
}


/**
 * @brief Create a thread.
 * @param[in] thread_id                ID of the thread to create.
 * @param[in] function_ptr             Pointer to the thread entry rountine.
 * @param[out] thread_handle           Handle to the created thread.
 * @return                             Result code.
 */
tSIRF_UINT32 SIRF_PAL_OS_THREAD_Create( tSIRF_UINT32 thread_id, tSIRF_HANDLE function_ptr, tSIRF_THREAD *thread_handle )
{
   tSIRF_UINT32         max_threads;
   tSIRF_UINT32         i;
   int                  ret;
   pthread_attr_t       attr;
   struct sched_param   param;
   int                  policy;

   max_threads = SIRF_THREAD_MaxThreads();
   
   for( i=0; i< max_threads; i++ )
      if ( SIRF_THREAD_Table[i].thread_id == thread_id)
         break;

   if ( i == max_threads )
   {
      DEBUGCHK(0);
      return SIRF_PAL_OS_ERROR;         /* thread is not defined */
   }

   pthread_attr_init( &attr );

   if ( SIRF_THREAD_Table[i].thread_priority != 0 )
   {
      pthread_attr_getschedparam( &attr, &param);

      if (SIRF_THREAD_Table[i].thread_quantum!=0)
         policy = SIRF_THREAD_Table[i].thread_quantum;
      else
         policy = SCHED_RR;

      param.sched_priority = SIRF_THREAD_Table[i].thread_priority;

#ifdef OS_ANDROID
     if ( pthread_attr_setschedpolicy( &attr, policy ) != 0
         || pthread_attr_setschedparam( &attr, &param) != 0 )
#else
      /* Note: This may fail on some Posix implementations such as Cygwin. */
      if ( pthread_attr_setinheritsched( &attr, PTHREAD_EXPLICIT_SCHED ) != 0
         || pthread_attr_setschedpolicy( &attr, policy ) != 0
         || pthread_attr_setschedparam( &attr, &param) != 0 )
#endif
      {
         DEBUGMSG(1,(DEBUGTXT("OS_Thread_Create: could not set thread priority, using default\n")));
         /* return SIRF_PAL_OS_ERROR; */
      }
   }

   if ( SIRF_THREAD_Table[i].thread_stack_size != 0 )
   {
      if ( pthread_attr_setstacksize( &attr, SIRF_THREAD_Table[i].thread_stack_size ) != 0 )
         DEBUGMSG(1,(DEBUGTXT("OS_Thread_Create: could not set stack size, using default\n")));
   }

   ret = pthread_create((pthread_t *)thread_handle, &attr, function_ptr, NULL);

   VERIFY(*thread_handle);

   if ( ret != 0 )
   {
      DEBUGMSG(1, (DEBUGTXT("OS_Thread_Create: fatal error: could not create thread, ret=%d, errno=%d\n"), ret, errno));
      return SIRF_PAL_OS_ERROR;
   }

   return SIRF_SUCCESS ;

} /* SIRF_PAL_OS_THREAD_Create() */


/**
 * @brief Delete a thread.
 * @param[in] thread_handle            Handle to the thread to delete.
 * @return                             Result code.
 */
tSIRF_UINT32 SIRF_PAL_OS_THREAD_Delete( tSIRF_THREAD thread_handle )
{
   int result;

   DEBUGCHK(thread_handle);

   /* we allow all threads to gracefully exit by releasing their wait semaphores
      and setting shutdown flag */
   do
      result = pthread_join((pthread_t)thread_handle, NULL);
   while ( result==EINTR );

   if ( result != 0 )
   {
      DEBUGMSG(1, (DEBUGTXT("Could not join thread 0x%08X\n"), thread_handle));
      return SIRF_PAL_OS_ERROR;
   }

   return SIRF_SUCCESS;

} /* SIRF_PAL_OS_THREAD_Delete() */


/**
 * @brief Put the current thread to sleep.
 * @param[in] milliseconds             Milliseconds to sleep for.
 */
tSIRF_VOID SIRF_PAL_OS_THREAD_Sleep( tSIRF_UINT32 milliseconds )
{
   struct timespec time, remaining;
   int result;

   if ( !milliseconds )
      milliseconds = 1 ;

   remaining.tv_sec  =  milliseconds / 1000L;
   remaining.tv_nsec = (milliseconds % 1000L) * 1000000L;

   do
   {
      time = remaining;
      result = nanosleep( &time, &remaining ); 
   } while (result==-1 && errno==EINTR && (remaining.tv_sec>0 || remaining.tv_nsec>0) );
   
} /* SIRF_PAL_OS_THREAD_Sleep() */


/**
 * @brief Handler for thread startup.
 */
tSIRF_VOID SIRF_PAL_OS_THREAD_Entry( tSIRF_VOID )
{
   sigset_t set;
   int      ret;
   struct sigaction sigact;

   /* install signal handler for SIGUSR1 */
   memset(&sigact, 0, sizeof(sigact));
   sigact.sa_handler = sighandler;
   ret = sigaction(SIGUSR1, &sigact, NULL);
   if(ret < 0)
   {
      DEBUGMSG(1, (DEBUGTXT("ERROR: sigaction failed with errno %dn"), errno));
   }
   
   /* set signal mask for the thread */
   sigemptyset( &set );
   /* pthread_sigmask call on Android is returning EINVAL.
    * so, it has to replaced with sigprocmask */
#if defined(OS_ANDROID)
   ret = sigprocmask( SIG_SETMASK, &set, NULL);
#else
   ret = pthread_sigmask( SIG_SETMASK, &set, NULL );
#endif
   if ( ret!=0 )
   {
      DEBUGMSG(1, (DEBUGTXT("ERROR: could not set thread mask for current thread,\n")));
   }

   /* Set the System call interrupt function */
   GPS_Syscall_Intr_Fn = SIRF_PAL_OS_THREAD_Send_Signal;

} /* SIRF_PAL_OS_Thread_Start() */


/**
 * @brief Handler for thread shutdown.
 */
SIRF_PAL_OS_THREAD_RETURNTYPE SIRF_PAL_OS_THREAD_Return( tSIRF_VOID )
{
   pthread_exit(NULL);
#if defined(OS_QNX)
   return NULL;
#else
   return;
#endif
} /* SIRF_PAL_OS_Thread_Return() */



/**
 * @}
 * End of file.
 */


