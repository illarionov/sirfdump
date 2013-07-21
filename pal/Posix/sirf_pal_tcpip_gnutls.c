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
 * @file   sirf_pal_tcpip_gnutls.c
 *
 * @brief  SiRF PAL networking module.
 */

#include "sirf_pal.h"

#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <sys/socket.h>


#include <gnutls/gnutls.h>
#include <gnutls/extra.h>
#include <gnutls/x509.h>


/* ----------------------------------------------------------------------------
 *   Local Functions
 * ------------------------------------------------------------------------- */
#define SIRF_PAL_NET_SECURITY_X509_CERTIFICATES

#define MAX_GNUTLS_SOCKETS   (16)

#define KEYFILE         "x509-server-key.pem"
#define SERVER_CERTFILE "x509-server.pem"
#define CLIENT_CERTFILE "x509-client.pem"
#define CAFILE          "x509-ca.pem"
#define CRLFILE         "crl.pem"

#define DH_BITS 1024
#define MAX_SELECT_SOCKETS (32)

/* Socket structure with all the relevant informaiton for secure socket usage */
typedef struct tSIRF_SOCKET_GNUTLS_tag
{
   tSIRF_SOCKET socket;
   tSIRF_UINT32 security;
   /* the following items are used only if security is enabled */
   gnutls_session_t session;
   gnutls_anon_client_credentials_t anoncred;
   gnutls_certificate_credentials_t x509_cred;
   gnutls_priority_t priority_cache;
   gnutls_dh_params_t dh_params;
   tSIRF_MUTEX m_read_mutex;
   tSIRF_MUTEX m_write_mutex;
} tSIRF_SOCKET_GNUTLS;

static tSIRF_SOCKET_GNUTLS s_gnutls_sockets[MAX_GNUTLS_SOCKETS];
static tSIRF_MUTEX s_net_mutex;

/**
 * @brief Test if a socket handle is valid.
 * @param[in] socket                   Socket handle to test.
 * @return                             Non-zero if valid.
 */
static int IsValidSocket(int socket)
{
   return socket>=0;

} /* IsValidSocket() */

/**
 *  This function will check whether the given return code from
 * a gnutls function (recv/send), is an alert, and will print
 * that alert.
 * 
 * @param session the current GNU TLS session
 * @param ret     the alert to check
 */
static void check_alert (gnutls_session_t session, int ret)
{
   int last_alert;
   if (GNUTLS_E_WARNING_ALERT_RECEIVED == ret 
       || GNUTLS_E_FATAL_ALERT_RECEIVED == ret )
   {
      last_alert = gnutls_alert_get(session);
      /* The check for renegotiation is only useful if we are
       * a server, and we had requested a rehandshake.
       */
      if (GNUTLS_A_NO_RENEGOTIATION == last_alert &&
          GNUTLS_E_WARNING_ALERT_RECEIVED == ret)
      {
         DEBUGMSG(1,(DEBUGTXT("* Received NO_RENEGOTIATION alert. Client Does not support renegotiation.\n")));
      }
      else
      {
         DEBUGMSG(1,(DEBUGTXT("* Received alert '%d': %s.\n"), last_alert,
                 gnutls_alert_get_name(last_alert)));
      }
   }
}

/** 
 * Initialize the TLS session and initilize all security for a secure exchange.
 *
 * Customization of this function is useful for changing the cyphers used.
 *
 * Current implementation is to use x.509 certificates which require that
 * the certificates be stored on disk.
 * 
 * @param socket  pointer to the socket.
 * @param con_end type of connection either GNUTLS_SERVER or GNUTLS_CLIENT
 * 
 * @return SIRF_SUCCESS if there were no errors or the error code returned.
 */
static tSIRF_RESULT initialize_tls_session(tSIRF_SOCKET_GNUTLS *socket, 
                                           tSIRF_CHAR* hostname,
                                           gnutls_connection_end_t con_end)
{
   const char *err;

   int result;

#ifdef SIRF_PAL_NET_SECURITY_X509_CERTIFICATES
   result = gnutls_certificate_allocate_credentials(&socket->x509_cred);
   gnutls_certificate_set_verify_flags(socket->x509_cred,
                                       GNUTLS_VERIFY_ALLOW_X509_V1_CA_CRT);

   if (GNUTLS_E_SUCCESS != result)
   {
      DEBUGMSG(1,(DEBUGTXT("gnutls_certificate_allocate_credentials:%d"),
                  result));
      gnutls_perror(result);
      return SIRF_PAL_NET_SECURITY_CERTIFICATE_ERROR;
   }

   result = gnutls_certificate_set_x509_trust_file(socket->x509_cred, 
                                                   CAFILE,
                                                   GNUTLS_X509_FMT_PEM);
   if (0 > result)
   {
      DEBUGMSG(1,(DEBUGTXT("gnutls_certificate_allocate_credentials:%d"),
                  result));
      gnutls_perror(result);
      return SIRF_PAL_NET_SECURITY_CERTIFICATE_ERROR;
   }

   result = gnutls_init(&socket->session, con_end);
   if (GNUTLS_E_SUCCESS != result)
   {
      DEBUGMSG(1,(DEBUGTXT("gnutls_init:%d"),result));
      gnutls_perror(result);
      return SIRF_PAL_NET_SECURITY_ERROR;
   }

   if (GNUTLS_SERVER == con_end)
   {
      /* this is for servers only */
      result = gnutls_certificate_set_x509_crl_file(socket->x509_cred, CRLFILE,
                                                    GNUTLS_X509_FMT_PEM);
      if (0 > result)
      {
         DEBUGMSG(1,(DEBUGTXT("gnutls_certificate_set_x509_crl_file:%d"),
                     result));
         gnutls_perror(result);
         return SIRF_PAL_NET_SECURITY_CERTIFICATE_ERROR;
      }
      
      /* this is for servers only */
      result = gnutls_certificate_set_x509_key_file(socket->x509_cred, 
                                                    SERVER_CERTFILE, 
                                                    KEYFILE,
                                                    GNUTLS_X509_FMT_PEM);

      if (GNUTLS_E_SUCCESS != result)
      {
         DEBUGMSG(1,(DEBUGTXT("gnutls_certificate_set_x509_key_file:%d"),
                     result));
         gnutls_perror(result);
         return SIRF_PAL_NET_SECURITY_CERTIFICATE_ERROR;
      }

      /* prototype is int, but it doesn't return anything? */
      gnutls_dh_params_init(&socket->dh_params);
      /* prototype is int, but it doesn't return anything? */
      gnutls_dh_params_generate2(socket->dh_params, DH_BITS);

      result = gnutls_priority_init(&socket->priority_cache, "NORMAL", NULL);
      if (GNUTLS_E_SUCCESS != result)
      {
         DEBUGMSG(1,(DEBUGTXT("gnutls_priority_init:%d"),result));
         gnutls_perror(result);
         return SIRF_PAL_NET_SECURITY_ERROR;
      }
      
      gnutls_certificate_set_dh_params(socket->x509_cred, socket->dh_params);
      gnutls_priority_set(socket->session, socket->priority_cache);
      if (GNUTLS_E_SUCCESS != result)
      {
         DEBUGMSG(1,(DEBUGTXT("gnutls_priority_set:%d"),result));
         gnutls_perror(result);
         return SIRF_PAL_NET_SECURITY_ERROR;
      }
   }
   else if (GNUTLS_CLIENT == con_end)
   {
      result = gnutls_priority_set_direct(socket->session, "PERFORMANCE", &err);

      if (NULL != hostname)
      {
         gnutls_handshake_set_private_extensions (socket->session, 1);
         result = gnutls_server_name_set (socket->session, 
            GNUTLS_NAME_DNS, 
            hostname,
            strlen (hostname));
      }

      if (GNUTLS_E_SUCCESS != result)
      {
         DEBUGMSG(1,(DEBUGTXT("gnutls_priority_set:%d"),result));
         gnutls_perror(result);
         return SIRF_PAL_NET_SECURITY_ERROR;
      }
   }
   else
   {
      return SIRF_FAILURE;
   }

   gnutls_anon_allocate_client_credentials(&socket->anoncred);
   gnutls_credentials_set (socket->session, GNUTLS_CRD_ANON, socket->anoncred);

   /* prototype is int, but it doesn't return anything? */
   gnutls_credentials_set(socket->session, 
                          GNUTLS_CRD_CERTIFICATE, 
                          socket->x509_cred);

   if (GNUTLS_SERVER == con_end)
   {
      /* request client certificate if any. */
      gnutls_certificate_server_set_request(socket->session, 
                                            GNUTLS_CERT_REQUEST);
      /* Set maximum compatibility mode. This is only suggested on public 
       * webservers that need to trade security for compatibility
       */
      gnutls_session_enable_compatibility_mode(socket->session);
   }
#endif

   return SIRF_SUCCESS;
}

/* ----------------------------------------------------------------------------
 *   Functions
 * ------------------------------------------------------------------------- */


/** 
 * Find the next available socket in the s_gnutls_sockets structure
 *
 * NOTE: This function is NOT thread safe.  It needs to be protected by first
 * grabbing the s_net_mutex
 * 
 * @return the location of the next socket that is not is use.  NULL if all are
 * in use.
 */
static tSIRF_SOCKET_GNUTLS* find_next_avail_socket(tSIRF_VOID)
{
   int ii;
   for (ii = 0; ii < MAX_GNUTLS_SOCKETS; ii++)
   {
      if (SIRF_PAL_NET_INVALID_SOCKET == s_gnutls_sockets[ii].socket)
      {
         return &s_gnutls_sockets[ii];
      }
   }

   return NULL;
}

/** 
 * Global initialization of the Network Module
 * 
 * @return SIRF_SUCCESS if successful.  Refer to error codes for failure
 */
tSIRF_RESULT SIRF_PAL_NET_Init(tSIRF_VOID)
{
   int ii;
   const tSIRF_CHAR* version;
   tSIRF_RESULT result = SIRF_SUCCESS;

   /* Initialize the global socket structures */
   memset(s_gnutls_sockets,0,sizeof(s_gnutls_sockets));
   for (ii = 0; ii < MAX_GNUTLS_SOCKETS; ii++)
   {
      s_gnutls_sockets[ii].socket = SIRF_PAL_NET_INVALID_SOCKET;
   }

   /* Create the mutex for protecting the gnutls sockets */
   result = SIRF_PAL_OS_MUTEX_Create(&s_net_mutex);
   if (SIRF_SUCCESS != result)
   {
      return result;
   }

   /* Global initialization of the GnuTLS library */
   result = gnutls_global_init();
   if (GNUTLS_E_SUCCESS != result)
   {
      return SIRF_PAL_NET_ERROR;
   }

   /* If any extra library features are to be used, call this */
   /* gnutls_global_init_extra(); */
   /* Check to make sure we are using the version we thing we should be using */
   /* @todo figure out the actual version string to pass in to check */
   /* see LIBGNUTLS_VERSION vor a sutable version string */
   /* Function: const char * gnutls_check_version (const char * req_version) */
   version = gnutls_check_version(NULL);
   if (NULL == version)
   {
      return SIRF_PAL_NET_SECURITY_VERSION_ERROR;
   }

   DEBUGMSG(1,(DEBUGTXT("GnuTLS Version: %s\n"),version));

#if (__BUILD_TYPE == __DEBUG_BUILD)
   /* Use these to set debugging functionality, these require stderr and 
    * stdout are mapped appropriately */
   /* void gnutls_global_set_log_level (int level) */
   /* level is 0-9, or 10 to enable all */
   /* gnutls_global_set_log_level(); */

   /* void gnutls_global_set_log_function (gnutls_log_func log_func) */
   /* void (*gnutls_log_func)( int level, const char*);  */
   /* gnutls_global_set_log_function(); */
#endif /* #if (__BUILD_TYPE == __DEBUG_BUILD) */

   return result;
}
/** 
 * Global shutdown of the network module *
 * 
 * @return SIRF_SUCCESS if successful.  Refer to error codes for failure
 */
tSIRF_RESULT SIRF_PAL_NET_Destroy(tSIRF_VOID)
{
   /* Shutdown the GnuTLS library */
   gnutls_global_deinit();

   /* Delete the mutex.  The strucutres do not need to be modified as they are
    * simply static and the SIRF_PAL_NET_Init needs to be called again before
    * this module can be used again */
   SIRF_PAL_OS_MUTEX_Delete(s_net_mutex);
   s_net_mutex = NULL;
   return SIRF_SUCCESS;
}

/**
 * @brief Create a socket of the specified type.
 * @param socket_handle[out]           Handle to the socket.
 * @param socket_type[in]              Type of socket to create.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_NET_CreateSocket( tSIRF_SOCKET *socket_handle, 
                                        tSIRF_UINT32 socket_type )
{
   tSIRF_RESULT   res;
   char           opt_c;
   int            result = 0;
   tSIRF_SOCKET_GNUTLS *gnutls_handle = NULL;

   DEBUGCHK(NULL != socket_handle);

   *socket_handle = (tSIRF_SOCKET)SIRF_PAL_NET_INVALID_SOCKET;

   /* Find an unused internal socket */
   /* We must keep this mutex locked until we have a valid socket assinged
    * to gnutls->socket */
   /*------------------------------------------------------------ */
   SIRF_PAL_OS_MUTEX_Enter(s_net_mutex);
   gnutls_handle = find_next_avail_socket();

   /* Check to make sure that there was an availale socket */
   if (NULL == gnutls_handle)
   {
      SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);
      return SIRF_PAL_NET_MAX_SOCKETS_ALLOCATED;
   }

   gnutls_handle->socket = (tSIRF_SOCKET)socket(AF_INET, SOCK_STREAM, 0);

   if (!IsValidSocket(gnutls_handle->socket))
   {
      gnutls_handle->socket = SIRF_PAL_NET_INVALID_SOCKET;
      SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);

      DEBUGMSG(1,(DEBUGTXT("socket() failed, errno=%d\n"), errno));
      return SIRF_PAL_NET_ERROR;
   }
   SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);
   /*------------------------------------------------------------ */

   /* Create the mutex's */
   res = SIRF_PAL_OS_MUTEX_Create(&gnutls_handle->m_read_mutex);
   if (SIRF_SUCCESS != res)
   {
      (void)close(gnutls_handle->socket);
      return res;
   }
   res = SIRF_PAL_OS_MUTEX_Create(&gnutls_handle->m_write_mutex);
   if (SIRF_SUCCESS != res)
   {
      (void)SIRF_PAL_OS_MUTEX_Delete(gnutls_handle->m_read_mutex);
      (void)close(gnutls_handle->socket);
      return res;
   }

   switch (socket_type)
   {
   case SIRF_PAL_NET_SOCKET_TYPE_BLOCKING:
      /* Sockets are blocking by default */
      break;
   case SIRF_PAL_NET_SOCKET_TYPE_DEFAULT:
      do 
      {
         result = fcntl( gnutls_handle->socket, F_SETFL, O_NONBLOCK );
      } while (result!=0 && errno==EINTR);
      if (0!=result) 
      {
         DEBUGMSG(1,(DEBUGTXT("fcntl() failed, errno=%d\n"), errno));
         SIRF_PAL_NET_CloseSocket((tSIRF_SOCKET)gnutls_handle);
         return SIRF_PAL_NET_ERROR;
      }
      break;
   default:
      return SIRF_PAL_API_ERROR;
   }

   opt_c = 1;
   result = setsockopt(gnutls_handle->socket, 
                       SOL_SOCKET, 
                       SO_REUSEADDR, 
                       (char*)&opt_c, 
                       sizeof(opt_c));
   if (0!=result) 
   {
      DEBUGMSG(1,(DEBUGTXT("setsockopt() failed, errno=%d\n"), errno));
      /* All objects have been created so CloseSocket will succeded
       * and is the easiest way to undo everything that was done */
      SIRF_PAL_NET_CloseSocket((tSIRF_SOCKET)gnutls_handle);
      return SIRF_PAL_NET_ERROR;
   }

   opt_c = 1;
   result = setsockopt(gnutls_handle->socket, 
                       SOL_SOCKET, 
                       SO_DONTLINGER, 
                       (char*)&opt_c, 
                       sizeof(opt_c));
   if (0!=result) 
   {
      DEBUGMSG(1,(DEBUGTXT("setsockopt() failed, errno=%d\n"), errno));
      SIRF_PAL_NET_CloseSocket((tSIRF_SOCKET)gnutls_handle);
      return SIRF_PAL_NET_ERROR;
   }

   opt_c = 1;
   result = setsockopt(gnutls_handle->socket, 
                       IPPROTO_TCP, 
                       TCP_NODELAY, 
                       (char*)&opt_c, 
                       sizeof(opt_c));
   if (0!=result) 
   {
      DEBUGMSG(1,(DEBUGTXT("setsockopt() failed, errno=%d\n"), errno));
      SIRF_PAL_NET_CloseSocket((tSIRF_SOCKET)gnutls_handle);
      return SIRF_PAL_NET_ERROR;
   }

   *socket_handle = (tSIRF_SOCKET)gnutls_handle;

   return SIRF_SUCCESS;

} /* SIRF_PAL_NET_CreateSocket() */


/**
 * @brief Close a socket.
 * @param[in] socket_handle            Handle to the socket to close.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_NET_CloseSocket(tSIRF_SOCKET socket_handle)
{
   int result;
   tSIRF_SOCKET_GNUTLS *gnutls_socket = (tSIRF_SOCKET_GNUTLS*) socket_handle;

   DEBUGCHK(SIRF_PAL_NET_INVALID_SOCKET != socket_handle);

   if (SIRF_PAL_NET_SECURITY_NONE != gnutls_socket->security)
   {
      gnutls_bye(gnutls_socket->session,GNUTLS_SHUT_WR);
   }

   do
   {
      shutdown(gnutls_socket->socket, 2);
      result = close(gnutls_socket->socket);
   } while (0 > result && EINTR == errno);

   if (SIRF_PAL_NET_SECURITY_NONE != gnutls_socket->security)
   {
      gnutls_certificate_free_credentials(gnutls_socket->x509_cred);
      gnutls_priority_deinit(gnutls_socket->priority_cache);
   }

   /* Grab all the active mutexes before setting the socket to invalid 
    * Anybody doing anything should wake up and release their mutex now
    * that the socket has been closed */
   (void)SIRF_PAL_OS_MUTEX_Enter(gnutls_socket->m_write_mutex);
   (void)SIRF_PAL_OS_MUTEX_Enter(gnutls_socket->m_read_mutex);

   /* Grab the global mutexes to make sure we aren't selecting on multiple
    * and that as soon as the socket is set invalid it can be created
    * again */
   (void)SIRF_PAL_OS_MUTEX_Enter(s_net_mutex);

   /* Release the socket */
   gnutls_socket->socket = SIRF_PAL_NET_INVALID_SOCKET;

   (void)SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);

   (void)SIRF_PAL_OS_MUTEX_Exit(gnutls_socket->m_read_mutex);
   (void)SIRF_PAL_OS_MUTEX_Exit(gnutls_socket->m_write_mutex);

   (void)SIRF_PAL_OS_MUTEX_Delete(gnutls_socket->m_write_mutex);
   (void)SIRF_PAL_OS_MUTEX_Delete(gnutls_socket->m_read_mutex);

   return 0 == result ? SIRF_SUCCESS : SIRF_PAL_NET_ERROR;

} /* SIRF_PAL_NET_CloseSocket() */


/**
 * @brief Select on socket(s) to wait for activity.
 * @param read[in]             Null-terminated array of sockets for reading.
 * @param write[in]            Null-terminated array of sockets for writing. 
 * @param excpt[in]           Null-terminated array of sockets for excpting.
 * @param timeout[in]          Timeout in milliseconds.
 *
 * The arrays are modified on exit to contain only those sockets that were 
 * triggered.
 *
 * @return                             Success code.
 */
tSIRF_RESULT SIRF_PAL_NET_Select( tSIRF_SOCKET *read, 
                                  tSIRF_SOCKET *write, 
                                  tSIRF_SOCKET *excpt, 
                                  tSIRF_UINT32 timeout)
{
   struct timeval tv;
   fd_set r, w, e;
   int result, i;
   tSIRF_SOCKET fdmax;

   tSIRF_SOCKET_GNUTLS **gnutls_read = (tSIRF_SOCKET_GNUTLS**) read;
   tSIRF_SOCKET_GNUTLS **gnutls_write = (tSIRF_SOCKET_GNUTLS**) write;
   tSIRF_SOCKET_GNUTLS **gnutls_excpt = (tSIRF_SOCKET_GNUTLS**) excpt;

   /* These are needed to record the actual berkley socket that might
    * have been closed */
   tSIRF_SOCKET read_2[MAX_SELECT_SOCKETS];
   tSIRF_SOCKET write_2[MAX_SELECT_SOCKETS];
   tSIRF_SOCKET excpt_2[MAX_SELECT_SOCKETS];

   fdmax = 0;

   FD_ZERO(&r);
   FD_ZERO(&w);
   FD_ZERO(&e);

   /* Now is the tricky part.  We need to record the actual sockets without
    * them getting set invalid underneith us thus this is a critical section
    * to "remember" what we were told to select on.  The problem is that
    * read[i] cannot be overritten, but read[i]->socket can and will when close
    * gets called */

   (void)SIRF_PAL_OS_MUTEX_Enter(s_net_mutex);

   if (read)
   {
      for (i=0; 
           SIRF_PAL_NET_INVALID_SOCKET != read[i] && i < MAX_SELECT_SOCKETS;
           i++)
      {
         if (SIRF_PAL_NET_INVALID_SOCKET != gnutls_read[i]->socket)
         {
            /* record the socket */
            read_2[i] = (tSIRF_SOCKET)gnutls_read[i]->socket; 
            FD_SET((gnutls_read[i]->socket), &r); /* Add it to the set */
            if (gnutls_read[i]->socket>fdmax)
            { 
               fdmax=gnutls_read[i]->socket;
            }
         }
         else
         {
            /* Sometime between the calling of this function and entering the
             * mutex, this particular socket was closed. */
            SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);
            return SIRF_PAL_NET_ERROR;
         }
      }
      if (i < MAX_SELECT_SOCKETS)
      {
         read_2[i] = SIRF_PAL_NET_INVALID_SOCKET;
      }
      else
      {
         read_2[i-1] = SIRF_PAL_NET_INVALID_SOCKET;
      }
   }

   if (write)
   {
      for (i=0; 
           SIRF_PAL_NET_INVALID_SOCKET != write[i] && i < MAX_SELECT_SOCKETS;
           i++)
      {
         if (SIRF_PAL_NET_INVALID_SOCKET != gnutls_write[i]->socket)
         {
            /* record the socket */
            write_2[i] = (tSIRF_SOCKET)gnutls_write[i]->socket; 
            FD_SET((gnutls_write[i]->socket), &w); /* Add it to the set */
            if (gnutls_write[i]->socket>fdmax)
            { 
               fdmax=gnutls_write[i]->socket;
            }
         }
         else
         {
            /* Sometime between the calling of this function and entering the
             * mutex, this particular socket was closed. */
            SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);
            return SIRF_PAL_NET_ERROR;
         }
      }
      if (i < MAX_SELECT_SOCKETS)
      {
         write_2[i] = SIRF_PAL_NET_INVALID_SOCKET;
      }
      else
      {
         write_2[i-1] = SIRF_PAL_NET_INVALID_SOCKET;
      }
   }

   if (excpt)
   {
      for (i=0; 
           SIRF_PAL_NET_INVALID_SOCKET != excpt[i] && i < MAX_SELECT_SOCKETS;
           i++)
      {
         if (SIRF_PAL_NET_INVALID_SOCKET != gnutls_excpt[i]->socket)
         {
            /* record the socket */
            excpt_2[i] = (tSIRF_SOCKET)gnutls_excpt[i]->socket; 
            FD_SET((gnutls_excpt[i]->socket), &e); /* Add it to the set */
            if (gnutls_excpt[i]->socket>fdmax)
            { 
               fdmax=gnutls_excpt[i]->socket;
            }
         }
         else
         {
            /* Sometime between the calling of this function and entering the
             * mutex, this particular socket was closed. */
            SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);
            return SIRF_PAL_NET_ERROR;
         }
      }
      if (i < MAX_SELECT_SOCKETS)
      {
         excpt_2[i] = SIRF_PAL_NET_INVALID_SOCKET;
      }
      else
      {
         excpt_2[i-1] = SIRF_PAL_NET_INVALID_SOCKET;
      }
   }

   (void)SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);

   tv.tv_sec = timeout / 1000;
   tv.tv_usec = (timeout % 1000) * 1000;

   do
   {
      result = select(fdmax+1, &r, &w, &e, &tv);
   }
   while ( 0 > result && EINTR == errno );

   if (0<result)
   {
      if (read)
      {
         for (i=0; SIRF_PAL_NET_INVALID_SOCKET != read_2[i]; i++)
         {
            if (!FD_ISSET(read_2[i], &r))
            {
               read[i] = SIRF_PAL_NET_INVALID_SOCKET;
            }
         }
      }

      if (write)
      {
         for (i=0; SIRF_PAL_NET_INVALID_SOCKET != write_2[i]; i++)
         {
            if (!FD_ISSET(write_2[i], &w))
            {
               write[i] = SIRF_PAL_NET_INVALID_SOCKET;
            }
         }
      }

      if (excpt)
      {
         for (i=0; SIRF_PAL_NET_INVALID_SOCKET != excpt_2[i]; i++)
         {
            if (!FD_ISSET(excpt_2[i], &e))
            {
               excpt[i] = SIRF_PAL_NET_INVALID_SOCKET;
            }
         }
      }
   }

   if (0 < result)
   {
      return SIRF_SUCCESS;
   }
   else if (0 == result)
   {
      return SIRF_PAL_NET_TIMEOUT;
   }
   else
   {
      return SIRF_PAL_NET_ERROR;
   }
} /* SIRF_PAL_NET_Select() */


/**
 * @brief Read data from a socket.
 * @param socket_handle[in]            Handle to the socket to read from.
 * @param buffer[out]                  Buffer to read data into.
 * @param size[in]                     Size of the data buffer.
 * @param bytes_read[out]              Number of bytes read.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_NET_Read( tSIRF_SOCKET socket_handle, 
                                tSIRF_UINT8 *buffer, 
                                tSIRF_UINT32 size, 
                                tSIRF_UINT32 *bytes_read )
{
   tSIRF_RESULT ret = SIRF_SUCCESS;
   int result = 0;
   int handshake = 0;
   tSIRF_UINT32 security;
   tSIRF_SOCKET_GNUTLS *gnutls_socket = (tSIRF_SOCKET_GNUTLS*) socket_handle;

   DEBUGCHK(SIRF_PAL_NET_INVALID_SOCKET != socket_handle);
   DEBUGCHK(bytes_read);

   *bytes_read = 0;

   (void)SIRF_PAL_OS_MUTEX_Enter(gnutls_socket->m_read_mutex);
   security = gnutls_socket->security;
   do 
   {
      if (SIRF_PAL_NET_SECURITY_NONE == gnutls_socket->security)
      {
         result = recv(gnutls_socket->socket, 
                       (char*)(buffer+*bytes_read), 
                       size-*bytes_read, 
                       0);
         if (0 > result)
         {
            if ( ECONNRESET == errno || ENOTCONN == errno )
            {
               ret = SIRF_PAL_NET_CONNECTION_CLOSED;
            }
         }
      } /* SIRF_PAL_NET_SECURITY_NONE */ 
      else
      {
         result = gnutls_record_recv(gnutls_socket->session, 
                                     (char*)(buffer+*bytes_read), 
                                     size-*bytes_read);
         if (GNUTLS_E_INTERRUPTED == result || GNUTLS_E_AGAIN == result)
         {
            errno = EINTR;
         }
         else if (GNUTLS_E_REHANDSHAKE == result)
         {
            do 
            {
               handshake = gnutls_handshake(gnutls_socket->session);
            } while ((handshake == GNUTLS_E_AGAIN) || 
                     (handshake == GNUTLS_E_INTERRUPTED));
            if (0 > handshake)
            {
               gnutls_perror(handshake);
               check_alert(gnutls_socket->session,handshake);
               ret = SIRF_PAL_NET_SECURITY_PEER_REFUSED;
            }
            else
            {
               /* The result is less than zero, try the receive again */
               errno = EINTR;
            }
         }
         else if (GNUTLS_E_EXPIRED               == result || 
                  GNUTLS_E_FATAL_ALERT_RECEIVED  == result || 
                  GNUTLS_E_INVALID_SESSION       == result)
         {
            gnutls_perror(result);
            ret = SIRF_PAL_NET_CONNECTION_CLOSED;
         }
      }
   } while ( 0 > result && EINTR == errno );

   (void)SIRF_PAL_OS_MUTEX_Exit(gnutls_socket->m_read_mutex);

   if ( 0 > result )
   {
      if (SIRF_PAL_NET_SECURITY_NONE != security)
      {
         gnutls_perror(result);
      }
      DEBUGMSG(1,(DEBUGTXT("recv() failed, errno=%d\n"), errno));
      SIRF_PAL_OS_THREAD_Sleep(100); /* prevent spinning */
      ret = SIRF_PAL_NET_ERROR;
   }   
   else
   {
      *bytes_read = result;
   }

   return ret;

} /* SIRF_PAL_NET_Read() */


/**
 * @brief Write data to a socket.
 * @param[in] socket_handle            Handle to the socket to write into.
 * @param[in] packet                   Data to write.
 * @param[in] length                   Length of the data to write.
 * @param[out] bytes_written           Number of bytes written.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_NET_Write( tSIRF_SOCKET socket_handle, 
                                 tSIRF_UINT8 *packet, 
                                 tSIRF_UINT32 length, 
                                 tSIRF_UINT32 *bytes_written )
{
   tSIRF_RESULT ret = SIRF_SUCCESS;
   int  result;
   int handshake;
   tSIRF_UINT32 security;
   tSIRF_SOCKET_GNUTLS *gnutls_socket = (tSIRF_SOCKET_GNUTLS*) socket_handle;

   DEBUGCHK(SIRF_PAL_NET_INVALID_SOCKET != socket_handle);
   DEBUGCHK(bytes_written);

   *bytes_written=0;

   (void)SIRF_PAL_OS_MUTEX_Enter(gnutls_socket->m_write_mutex);
   security = gnutls_socket->security;
   do
   {
      if (SIRF_PAL_NET_SECURITY_NONE == security)
      {
         result = send( gnutls_socket->socket, 
                        (char*)(packet+*bytes_written), 
                        length-*bytes_written, 
                        0);
         if (0 > result)
         {
            if ( ENOTCONN == errno || ECONNRESET == errno )
            {
               ret = SIRF_PAL_NET_CONNECTION_CLOSED;
            }
         }
      }
      else
      {
         result = gnutls_record_send( gnutls_socket->session, 
                                      (char*)(packet+*bytes_written), 
                                      length-*bytes_written);
         if (GNUTLS_E_INTERRUPTED == result || GNUTLS_E_AGAIN == result)
         {
            errno = EINTR;
         }
         else if (GNUTLS_E_REHANDSHAKE == result)
         {
            do 
            {
               handshake = gnutls_handshake(gnutls_socket->session);
            } while ((GNUTLS_E_AGAIN       == handshake) || 
                     (GNUTLS_E_INTERRUPTED == handshake));

            if (0 > handshake)
            {
               gnutls_perror(handshake);
               check_alert(gnutls_socket->session,handshake);
               ret = SIRF_PAL_NET_SECURITY_PEER_REFUSED;
            }
            else
            {
               /* The result is less than zero, try the receive again */
               errno = EINTR;
            }
         }
         else if (GNUTLS_E_EXPIRED              == result || 
                  GNUTLS_E_FATAL_ALERT_RECEIVED == result || 
                  GNUTLS_E_INVALID_SESSION      == result)
         {
            gnutls_perror(result);
            ret = SIRF_PAL_NET_CONNECTION_CLOSED;
         }
      }
   } while ( 0 > result && EINTR == errno );

   (void)SIRF_PAL_OS_MUTEX_Exit(gnutls_socket->m_write_mutex);

   if ( 0 >= result )
   {
      if (SIRF_PAL_NET_SECURITY_NONE != security)
      {
         gnutls_perror(result);
      }
      DEBUGMSG(1,(DEBUGTXT("send() failed, errno=%d\n"), errno));
      SIRF_PAL_OS_THREAD_Sleep(100);
      return SIRF_PAL_NET_ERROR;
   }
   else
   {
      *bytes_written = result;
   }

   return ret;

} /* SIRF_PAL_NET_Write() */


/**
 * @brief Set the socket to run in listening mode.
 * @param[in] socket_handle            Handle to the socket.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_NET_Listen(tSIRF_SOCKET socket_handle)
{
   int result;
   tSIRF_SOCKET_GNUTLS *gnutls_socket = (tSIRF_SOCKET_GNUTLS*)socket_handle;

   DEBUGCHK(SIRF_PAL_NET_INVALID_SOCKET != socket_handle);

   do
   {
      result = listen(gnutls_socket->socket, SOMAXCONN);
   } while (0 != result && EINTR == errno);

   return 0 == result ? SIRF_SUCCESS : SIRF_PAL_NET_ERROR;

} /* SIRF_PAL_NET_Listen() */


/**
 * @brief Bind a socket to the specified address.
 * @param[in] socket_handle            Handle to the socket.
 * @param[in] name                     Socket address to bind to.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_NET_Bind( tSIRF_SOCKET socket_handle, 
                                tSIRF_SOCKADDR *name )
{
   int result;
   tSIRF_SOCKET_GNUTLS *gnutls_socket = (tSIRF_SOCKET_GNUTLS*)socket_handle;
   DEBUGCHK((SIRF_PAL_NET_INVALID_SOCKET != socket_handle) && (NULL != name));
   DEBUGCHK(SIRF_PAL_NET_INVALID_SOCKET != gnutls_socket->socket);

   name->reserved_0 = AF_INET;
   result = bind(gnutls_socket->socket, 
                 (struct sockaddr*)name, 
                 sizeof(tSIRF_SOCKADDR));
   if (0 != result)
   {
      DEBUGMSG(1,(DEBUGTXT("bind() failed, errno=%d\n"), errno));
   }

   return 0 == result ? SIRF_SUCCESS : SIRF_PAL_NET_ERROR;

} /* SIRF_PAL_NET_Bind() */


/**
 * @brief Accept an incoming connection on a listening socket.
 * @param[in] listener_handle          Listening socket.
 * @param[out] socket_handle           Received socket connection.
 * @param[in] name                     Address of received socket.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_NET_Accept( tSIRF_SOCKET listener_handle, 
                                  tSIRF_SOCKET *socket_handle, 
                                  tSIRF_SOCKADDR *name,
                                  tSIRF_UINT32 security)
{
   int result;
   int namelen = sizeof(tSIRF_SOCKADDR);
   tSIRF_SOCKET_GNUTLS *gnutls_socket = (tSIRF_SOCKET_GNUTLS*)listener_handle;

   DEBUGCHK(SIRF_PAL_NET_INVALID_SOCKET != listener_handle);
   DEBUGCHK( socket_handle );
   *socket_handle = SIRF_PAL_NET_INVALID_SOCKET;

   do
   {
      result = accept(gnutls_socket->socket, 
                      (struct sockaddr*)name, 
                      (name ? &namelen : NULL) );
   } while ( 0 > result && EINTR == errno );

   /* Validate the socket and perform the handshake if secure */
   if (IsValidSocket(result))
   {
      SIRF_PAL_OS_MUTEX_Enter(s_net_mutex);
      gnutls_socket = find_next_avail_socket();
      if (NULL == gnutls_socket){
         shutdown(result, 2);
         result = close(result);
         SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);
         return SIRF_PAL_NET_ERROR;
      }
      gnutls_socket->socket   = result;
      SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);

      gnutls_socket->security = security;
      if (SIRF_PAL_NET_SECURITY_NONE != security)
      {
         if (SIRF_SUCCESS == initialize_tls_session(gnutls_socket,NULL,GNUTLS_SERVER))
         {
            gnutls_transport_set_ptr(gnutls_socket->session, 
                                     (gnutls_transport_ptr_t) gnutls_socket->socket);
            do 
            {
               result = gnutls_handshake(gnutls_socket->session);
            } while ((GNUTLS_E_AGAIN       == result) || 
                     (GNUTLS_E_INTERRUPTED == result));

            if (0 > result)
            {
               gnutls_perror(result);
               SIRF_PAL_NET_CloseSocket((tSIRF_SOCKET)gnutls_socket);
               return SIRF_PAL_NET_SECURITY_PEER_REFUSED;
            }
         }
         else
         {
            SIRF_PAL_NET_CloseSocket((tSIRF_SOCKET)gnutls_socket);
            return SIRF_PAL_NET_SECURITY_PEER_REFUSED;
         }
      }
      *socket_handle = (tSIRF_SOCKET)gnutls_socket;
      return SIRF_SUCCESS;
   }

   return SIRF_PAL_NET_ERROR;

} /* SIRF_PAL_NET_Accept() */


/**
 * @brief Connect to the specified address/port.
 * @param[in] socket_handle            Socket handle.
 * @param[in] addr                     Address to connect to.
 * @param[in] port                     Port to connect to.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_NET_Connect( tSIRF_SOCKET socket_handle, 
                                   tSIRF_CHAR *addr, 
                                   tSIRF_UINT16 port, 
                                   tSIRF_UINT32 security )
{
   unsigned long **     p_addr;
   int                  result;
   tSIRF_SOCKADDR        sockaddr;
   struct hostent *     host_addrs;
   tSIRF_SOCKET_GNUTLS  *gnutls_socket = (tSIRF_SOCKET_GNUTLS*) socket_handle;

   DEBUGCHK((SIRF_PAL_NET_INVALID_SOCKET != socket_handle) && addr);

   /* Set the security so we know which function to use for the send */
   gnutls_socket->security = security;

   /* convert from dotted notation to sockaddr: */
   { 
      /* Use TCP if DNS lookup needed */
      sethostent(1);
      host_addrs = gethostbyname( addr );
      endhostent();

      if (!host_addrs) return SIRF_PAL_NET_ERROR;
   }

   p_addr = (unsigned long **)host_addrs->h_addr_list;
   do
   {
      sockaddr.reserved_0 = AF_INET;
      sockaddr.sin_addr   = **p_addr++;
      sockaddr.sin_port   = htons(port);

      do
      {
         result = connect(gnutls_socket->socket, 
                          (struct sockaddr*)&sockaddr, 
                          sizeof(tSIRF_SOCKADDR));

         if (0 != result)
         {
            if ((EINPROGRESS == errno) ||
                (EALREADY == errno) ||
                (EWOULDBLOCK == errno))
            {
               /* This means that the connect call would block, but that the
                * request was successful.  Use select to make sure it actually
                * connected before using */
               result = 0;
               break;
            }

            if ( EINTR == errno )
            {
               continue;
            }
            else
            {
               DEBUGMSG(1,(DEBUGTXT("connect() to IP %s failed, errno=%d trying next\n"),
                           inet_ntoa(sockaddr), errno));
               break;
            }
         }
      }
      while ( 0 != result );
   }
   while ((0 != *p_addr) && (0 != result)); 

   if (SIRF_PAL_NET_SECURITY_NONE != security)
   {
      /* We have to block here until the socket has successfully connected
       * before the handshake can be done */
      do
      {
         tSIRF_SOCKET write[2];
         write[0] = (tSIRF_SOCKET)gnutls_socket;
         write[1] = SIRF_PAL_NET_INVALID_SOCKET;
         /* 10 ms is arbitrary */
         result = SIRF_PAL_NET_Select(NULL, write, NULL,10);
      } while (SIRF_PAL_NET_TIMEOUT == result);

      /* If the connection is truely successful, do the handshake */
      if (SIRF_SUCCESS == result)
      {
         if (SIRF_SUCCESS == initialize_tls_session(gnutls_socket,addr,GNUTLS_CLIENT))
         {
            gnutls_transport_set_ptr(gnutls_socket->session, 
                                     (gnutls_transport_ptr_t)gnutls_socket->socket);

            do 
            {
               result = gnutls_handshake(gnutls_socket->session);
            } while ((GNUTLS_E_AGAIN == result)||(GNUTLS_E_INTERRUPTED == result));
         
            /* Check for successful handshake */
            if (0 > result)
            {
               gnutls_perror(result);
               check_alert(gnutls_socket->session,result);
               shutdown(gnutls_socket->socket, 2);
               return SIRF_PAL_NET_SECURITY_PEER_REFUSED;
            }
         }
         else
         {
            shutdown(gnutls_socket->socket, 2);
            return SIRF_PAL_NET_SECURITY_PEER_REFUSED;
         }
      }
      else 
      {
         return SIRF_PAL_NET_ERROR;
      }
   }


   return SIRF_SUCCESS;

} /* SIRF_PAL_NET_Connect() */


/**
 * @brief Get the socket peer name.
 * @param socket_handle[in]            Socket handle.
 * @param name[in]                     Name of the peer.
 * @return                             Result code.
 */
tSIRF_RESULT SIRF_PAL_NET_GetPeerName( tSIRF_SOCKET socket_handle, 
                                       tSIRF_SOCKADDR *name )
{
   int result, len;
   tSIRF_SOCKADDR tempname;

   len = sizeof(tSIRF_SOCKADDR);
   result = getpeername( socket_handle, 
                         (struct sockaddr*)(name?name:&tempname), 
                         &len );

   return 0 == result ? SIRF_SUCCESS : SIRF_PAL_NET_ERROR;

} /* SIRF_PAL_NET_GetPeerName() */

/**
 * @}
 * End of file.
 */


