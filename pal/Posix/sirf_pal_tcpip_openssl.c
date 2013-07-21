/**
*
*/

/**
* @file   sirf_pal_tcpip_openssl.c
*
* @brief  SiRF PAL networking module
*/
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include "sirf_pal.h"
#include "openssl/ssl.h"
#include "openssl/err.h"

#include "gps_logging.h"

#define MAX_OPENSSL_SOCKETS 8
#define AGPS_CA_PATH_LEN (32+1)

#define AGPS_GPRSIF_LEN 20 // GPRS interface like: ccinet0

/* cache invalid timeout in ms - currently set to 1 day */
#define CACHE_INVALIDATE_TIMEOUT (24 * 60 * 60 * 1000)

/* Socket structure with all the relevant informaiton for secure socket usage */
typedef struct tSIRF_SOCKET_OPENSSL_tag
{
    tSIRF_SOCKET socket;
    tSIRF_UINT32 security;
    /* the following items are used only if security is enabled */
    SSL_CTX *ctx;
    SSL *ssl;
} tSIRF_SOCKET_OPENSSL;

static tSIRF_SOCKET_OPENSSL s_openssl_sockets[MAX_OPENSSL_SOCKETS];
static tSIRF_MUTEX s_net_mutex;


extern char ca_path[256];

static tSIRF_BOOL is_cache_valid = 0;;
static char cached_server_name[128];
static tSIRF_UINT32 cached_server_ip;
static tSIRF_UINT32 cache_update_time;

static char gprs_interface[AGPS_GPRSIF_LEN];


void set_gprs_interface(char * s)
{
	gprs_interface[0] = '\0';

	if (s != NULL)
	{
		SIRF_LOGD("gprs_interface from Location Manager %s",s);
		strncpy(gprs_interface, s, AGPS_GPRSIF_LEN - 1);
		SIRF_LOGD("gprs_interface in PAL %s", gprs_interface);
	}
}


/**
* @brief Test if a socket handle is valid.
* @param[in] socket                   Socket handle to test.
* @return                             Non-zero if valid.
*/
static int IsValidSocket(int socket)
{
    return (socket >= 0);

} /* IsValidSocket() */

/** 
* Find the next available socket in the s_gnutls_sockets structure
*
* NOTE: This function is NOT thread safe.  It needs to be protected by first
* grabbing the s_net_mutex
* 
* @return the location of the next socket that is not is use.  NULL if all are
* in use.
*/
static tSIRF_SOCKET_OPENSSL* find_next_avail_socket(tSIRF_VOID)
{
    int i;

    for (i = 0; i < MAX_OPENSSL_SOCKETS; i++)
    {
        if (SIRF_PAL_NET_INVALID_SOCKET == s_openssl_sockets[i].socket)
        {
            return (&s_openssl_sockets[i]);
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
    int i;
    tSIRF_RESULT result = SIRF_SUCCESS;

    /* Initialize the global socket structures */
    memset(s_openssl_sockets, 0, sizeof(s_openssl_sockets));
    for (i = 0; i < MAX_OPENSSL_SOCKETS; i++)
    {
        s_openssl_sockets[i].socket = SIRF_PAL_NET_INVALID_SOCKET;
    }

    /* Create the mutex for protecting the gnutls sockets */
    result = SIRF_PAL_OS_MUTEX_Create(&s_net_mutex);
    if (SIRF_SUCCESS != result)
    {
        SIRF_LOGE("Mutex creation failed with result %lu", result);
    }

    return result;
}

/** 
* Global shutdown of the network module *
* 
* @return SIRF_SUCCESS if successful.  Refer to error codes for failure
*/
tSIRF_RESULT SIRF_PAL_NET_Destroy(tSIRF_VOID)
{
    /* Delete the mutex.  The strucutres do not need to be modified as they are
    * simply static and the SIRF_PAL_NET_Init needs to be called again before
    * this module can be used again */
    SIRF_PAL_OS_MUTEX_Delete(s_net_mutex);
    s_net_mutex = NULL;

    return SIRF_SUCCESS;
}

#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netinet/in.h>


tSIRF_CHAR  DEFAULT_PDP_NAME[50];
tSIRF_CHAR  PREFERRED_PDP_NAME[50];


tSIRF_RESULT is_Default_PDP_NAME_PDP0(tSIRF_SOCKET socket_handle)
{

    struct ifconf ifcfg;
    struct ifreq *ifr = NULL;
    struct sockaddr_in *sin;
    struct sockaddr *sa;

    int numeqs = 30;
    int n = 0;
    char *ip_addr_notation;
    int result = SIRF_FAILURE;

    memset(&ifcfg, 0, sizeof(ifcfg));
    ifcfg.ifc_buf = NULL;
    ifcfg.ifc_len = sizeof(struct ifreq) * numeqs;
    ifcfg.ifc_buf = malloc(ifcfg.ifc_len);

    SIRF_LOGD("is_Default_PDP_NAME_PDP0");

    if(ioctl(socket_handle, SIOCGIFCONF, (char *)&ifcfg) < 0)
    {
        SIRF_LOGE("SIOCGIFCONF fail!!! Set default PDP as pdp0");
        return SIRF_TRUE;
    }

    ifr = ifcfg.ifc_req;

    for (n = 0; n < ifcfg.ifc_len; n+= sizeof(struct ifreq))
    {
        SIRF_LOGD("Netowrk Interface [%s]\n", ifr->ifr_name);

        sin = (struct sockaddr_in *)&ifr->ifr_addr;
        ip_addr_notation = inet_ntoa(sin->sin_addr);
        SIRF_LOGD("IP     %s %d\n", ip_addr_notation, (int)(sin->sin_addr.s_addr));
        if(strcmp (ifr->ifr_name, "pdp0") != 0)
        {
            ifr++; /*find next element */
            continue;
        }

        if ( (ntohl(sin->sin_addr.s_addr) == INADDR_LOOPBACK) || (ntohl(sin->sin_addr.s_addr) == INADDR_NONE))
        {
            SIRF_LOGD("Loop Back or ADDR NONE");
            result = SIRF_FAILURE;
            break;
        }
        else if (strcmp(ip_addr_notation, "0.0.0.0") == 0)
        {
            SIRF_LOGD("No Network Connection");
            result = SIRF_FAILURE;
            break;
        }
        else
        {
            SIRF_LOGD("Default PDP : pdp0 verified");
            result = SIRF_SUCCESS; 
            break;
        }

        break;

    }

    if(n >= ifcfg.ifc_len)
    {
        SIRF_LOGE("No match for default pdp(pdp0), rmnet will be used");
    }

    free(ifcfg.ifc_buf);

    return result;

}


tSIRF_RESULT SIRF_PAL_NET_Check_Multiple_PDP(tSIRF_SOCKET socket_handle)
{

    struct ifconf ifcfg;
    struct ifreq *ifr = NULL;
    struct sockaddr_in *sin;
    struct sockaddr *sa;

    int numeqs = 30;
    int n = 0;
    char *ip_addr_notation;
    int result = SIRF_FAILURE;

    memset(&ifcfg, 0, sizeof(ifcfg));
    ifcfg.ifc_buf = NULL;
    ifcfg.ifc_len = sizeof(struct ifreq) * numeqs;
    ifcfg.ifc_buf = malloc(ifcfg.ifc_len);

    SIRF_LOGD("SIRF_PAL_NET_Check_Multiple_PDP");

    if(ioctl(socket_handle, SIOCGIFCONF, (char *)&ifcfg) < 0)
    {
        SIRF_LOGE("SIOCGIFCONF fail!!! Use default PDP");
        return SIRF_FAILURE;
    }

    ifr = ifcfg.ifc_req;

    for (n = 0; n < ifcfg.ifc_len; n+= sizeof(struct ifreq))
    {
        SIRF_LOGD("Netowrk Interface [%s]\n", ifr->ifr_name);

        sin = (struct sockaddr_in *)&ifr->ifr_addr;
        ip_addr_notation = inet_ntoa(sin->sin_addr);
        SIRF_LOGD("IP     %s %d\n", ip_addr_notation, (int)(sin->sin_addr.s_addr));
        if(strcmp (ifr->ifr_name, PREFERRED_PDP_NAME) != 0)
        {
            ifr++; /*find next element */
            continue;
        }

        if ( (ntohl(sin->sin_addr.s_addr) == INADDR_LOOPBACK) || (ntohl(sin->sin_addr.s_addr) == INADDR_NONE))
        {
            SIRF_LOGD("Loop Back or ADDR NONE");
            result = SIRF_FAILURE;
            break;
        }
        else if (strcmp(ip_addr_notation, "0.0.0.0") == 0)
        {
            SIRF_LOGD("No Netowkr Connection");
            result = SIRF_FAILURE;
            break;
        }
        else
        {
            SIRF_LOGD("Multiple PDP : %s verified", PREFERRED_PDP_NAME);
            result = SIRF_SUCCESS; 
            break;
        }

        break;

    }

    if(n >= ifcfg.ifc_len)
    {
        SIRF_LOGD("No match for %s", PREFERRED_PDP_NAME);
    }

    free(ifcfg.ifc_buf);

    return result;

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
    static int default_pdp_check_completed = 0;
    int opt_c;
    int result = 0;
    int posix_socket;
    tSIRF_SOCKET_OPENSSL *ssl_socket = NULL;

    if(socket_handle == NULL)
    {
        SIRF_LOGE("%s: Invalid socket handle", __FUNCTION__); 
        return SIRF_PAL_API_ERROR;
    }

    *socket_handle = (tSIRF_SOCKET)SIRF_PAL_NET_INVALID_SOCKET;

    /* Find an unused internal socket */
    SIRF_PAL_OS_MUTEX_Enter(s_net_mutex);
    ssl_socket = find_next_avail_socket();
    if(NULL == ssl_socket)
    {
        SIRF_LOGD("No available free socket\n");
        SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);
        return SIRF_PAL_NET_MAX_SOCKETS_ALLOCATED;
    }

    posix_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (!IsValidSocket(posix_socket))
    {
        ssl_socket->socket = SIRF_PAL_NET_INVALID_SOCKET;
        SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);
        SIRF_LOGE("**** create socket failed! errno=%d ****", errno);
        return SIRF_PAL_NET_ERROR;
    }
    else
    {
        ssl_socket->socket = (tSIRF_SOCKET)posix_socket;
        SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);
    }

    /* Set all socket options here: */
    switch (socket_type)
    {
    case SIRF_PAL_NET_SOCKET_TYPE_BLOCKING:
        {
            //SIRF_LOGD("%s: socket type is BLOCKING", __FUNCTION__);
            /* Socket are blocking by default */
            break;
        }

    case SIRF_PAL_NET_SOCKET_TYPE_DEFAULT:
        {
            int flags;

            //SIRF_LOGD("%s: socket type is NONBLOCKING(DEFAULT)", __FUNCTION__);
            flags = fcntl(ssl_socket->socket, F_GETFL, 0);
            result = fcntl( ssl_socket->socket, F_SETFL, flags | O_NONBLOCK );

            if (0 < result) 
            {
                DEBUGMSG(1,(DEBUGTXT("fcntl() failed, errno=%d\n"), errno));
            }
            break;
        }

    default:
        return SIRF_PAL_NET_ERROR;
    }

    opt_c = 1;
    result = setsockopt(ssl_socket->socket, SOL_SOCKET, SO_REUSEADDR, &opt_c, sizeof(opt_c));
    if (result != 0)
    {
        SIRF_LOGE("****  setsockopt() failed, errno=%d ****", errno);
    }

    opt_c = 1;
    result = setsockopt(ssl_socket->socket, IPPROTO_TCP, TCP_NODELAY, &opt_c, sizeof(opt_c));
    if (result != 0)
    {
        SIRF_LOGE("****  setsockopt() failed, errno=%d ****", errno);
    }

    if (setsockopt(ssl_socket->socket, SOL_SOCKET, SO_BINDTODEVICE, gprs_interface, sizeof(gprs_interface)) < 0)
    {
        SIRF_LOGE("****  setsockopt() failed for %s errno=%d ****", gprs_interface, errno);
        close(ssl_socket->socket);
        return SIRF_PAL_NET_ERROR;
    }
    else 
    { 
        SIRF_LOGD("bind to %s success", gprs_interface);
    }    


    *socket_handle = (tSIRF_SOCKET)ssl_socket;

    //SIRF_LOGD("==== create socket return SIRF_SUCCESS, socket = %lu ====", ssl_socket->socket);

    return SIRF_SUCCESS;

} /* SIRF_PAL_NET_CreateSocket() */


/**
* @brief Close a socket.
* @param[in] socket_handle            Handle to the socket to close.
* @return                             Result code.
*/
tSIRF_RESULT SIRF_PAL_NET_CloseSocket(tSIRF_SOCKET socket_handle)
{
    int result = 0;
    tSIRF_SOCKET_OPENSSL *ssl_socket = (tSIRF_SOCKET_OPENSSL *)socket_handle;

    if((SIRF_PAL_NET_INVALID_SOCKET == socket_handle) || (NULL == ssl_socket))
    {
        SIRF_LOGE("%s: Invalid socket handle", __FUNCTION__); 
        return SIRF_PAL_API_ERROR;
    }

    (void)SIRF_PAL_OS_MUTEX_Enter(s_net_mutex);
    if (IsValidSocket(ssl_socket->socket))
    {
        shutdown(ssl_socket->socket, SHUT_RDWR);
        usleep(500);
        result = close(ssl_socket->socket);       
        if(result < 0)
        {
            SIRF_LOGE("**** close socket failed, errno=%d ****", errno);
        }
        else
        {
            SIRF_LOGD("==== close socket was valid ====");
        }
    }
    else
    {
        SIRF_LOGE("===close socket was not valid");
    }

    if(SIRF_PAL_NET_SECURITY_NONE != ssl_socket->security)
    {
        if (NULL != ssl_socket->ssl)
        {
            SSL_free(ssl_socket->ssl);
            ssl_socket->ssl = NULL;
            SIRF_LOGD("===close socket ss1 freed");
        }

        if (NULL != ssl_socket->ctx)
        {
            SSL_CTX_free(ssl_socket->ctx);
            ssl_socket->ctx = NULL;
            SIRF_LOGD("===close socket ctx freed");
        }
    }

    ssl_socket->security = SIRF_PAL_NET_SECURITY_NONE;
    ssl_socket->socket = SIRF_PAL_NET_INVALID_SOCKET;
    (void)SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);

    return 0 == result ? SIRF_SUCCESS : SIRF_PAL_NET_ERROR;
} /* SIRF_PAL_NET_CloseSocket() */


/**
* @brief Select on socket(s) to wait for activity.
* @param read[in]             Null-terminated array of sockets for reading.
* @param write[in]            Null-terminated array of sockets for writing. 
* @param excpt[in]            Null-terminated array of sockets for excpting.
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
    tSIRF_SOCKET_OPENSSL** ssl_read = NULL;
    tSIRF_SOCKET_OPENSSL** ssl_write = NULL;
    tSIRF_SOCKET_OPENSSL** ssl_excpt = NULL;

    if((NULL == read) && (NULL == write) && (NULL == excpt))
    {
        SIRF_LOGE("%s: all the parameters can't be NULL", __FUNCTION__);
        return SIRF_PAL_API_ERROR;
    }

    fdmax = 0;

    FD_ZERO(&r);
    FD_ZERO(&w);
    FD_ZERO(&e);

    (void)SIRF_PAL_OS_MUTEX_Enter(s_net_mutex);
    if (read)
    {
        ssl_read = (tSIRF_SOCKET_OPENSSL **)read;
        for (i = 0; SIRF_PAL_NET_INVALID_SOCKET != read[i]; i++)
        {
            if (SIRF_PAL_NET_INVALID_SOCKET != ssl_read[i]->socket)
            {
                FD_SET(ssl_read[i]->socket, &r);
                if (ssl_read[i]->socket > fdmax)
                {
                    fdmax = ssl_read[i]->socket;
                }
            }
            else
            {
                (void)SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);
                return SIRF_PAL_NET_ERROR;
            }
        }
    }

    if (write)
    {
        ssl_write = (tSIRF_SOCKET_OPENSSL **)write;
        for (i = 0; SIRF_PAL_NET_INVALID_SOCKET != write[i]; i++)
        {
            if (SIRF_PAL_NET_INVALID_SOCKET != ssl_write[i]->socket)
            {
                FD_SET(ssl_write[i]->socket, &w);
                if (ssl_write[i]->socket > fdmax)
                {
                    fdmax = ssl_write[i]->socket;
                }
            }
            else
            {
                (void)SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);
                return SIRF_PAL_NET_ERROR;
            }
        }
    }

    if (excpt)
    {
        ssl_excpt = (tSIRF_SOCKET_OPENSSL **)excpt;
        for (i = 0; SIRF_PAL_NET_INVALID_SOCKET != excpt[i]; i++)
        {
            if (SIRF_PAL_NET_INVALID_SOCKET != ssl_excpt[i]->socket)
            {
                FD_SET(ssl_excpt[i]->socket, &e);
                if (ssl_excpt[i]->socket > fdmax)
                {
                    fdmax = ssl_excpt[i]->socket;
                }
            }
            else
            {
                (void)SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);
                return SIRF_PAL_NET_ERROR;
            }
        }
    }
    (void)SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);

    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;


    result = select(fdmax+1, &r, &w, &e, &tv);

    if (0 < result)
    {
        (void)SIRF_PAL_OS_MUTEX_Enter(s_net_mutex);
        if (read)
        {
            for (i = 0; SIRF_PAL_NET_INVALID_SOCKET != read[i]; i++)
            {
                if ((SIRF_PAL_NET_INVALID_SOCKET == ssl_read[i]->socket) ||
                    (!FD_ISSET(ssl_read[i]->socket, &r)))
                {
                    read[i] = SIRF_PAL_NET_INVALID_SOCKET;
                }
            }
        }
        if (write)
        {
            for (i = 0; SIRF_PAL_NET_INVALID_SOCKET != write[i]; i++)
            {
                if ((SIRF_PAL_NET_INVALID_SOCKET == ssl_write[i]->socket) ||
                    (!FD_ISSET(ssl_write[i]->socket, &w)))
                {
                    write[i] = SIRF_PAL_NET_INVALID_SOCKET;
                }
            }
        }
        if (excpt)
        {
            for (i = 0; SIRF_PAL_NET_INVALID_SOCKET != excpt[i]; i++)
            {
                if ((SIRF_PAL_NET_INVALID_SOCKET == ssl_excpt[i]->socket) ||
                    (!FD_ISSET(ssl_excpt[i]->socket, &e)))
                {
                    excpt[i] = SIRF_PAL_NET_INVALID_SOCKET;
                }
            }
        }
        (void)SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);
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
        SIRF_LOGE("**** SIRF_PAL_NET_Select failed! errno=%d ****", errno);
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
    int result = 0;
    tSIRF_SOCKET_OPENSSL *ssl_socket = (tSIRF_SOCKET_OPENSSL *)socket_handle;

    if((SIRF_PAL_NET_INVALID_SOCKET == socket_handle) || (NULL == ssl_socket) ||
        (NULL == buffer) || (NULL == bytes_read))
    {
        if((SIRF_PAL_NET_INVALID_SOCKET == socket_handle) || (NULL == ssl_socket))
        {
            SIRF_LOGE("%s: Invalid socket handle", __FUNCTION__); 
        }
        if(NULL == buffer)
        {
            SIRF_LOGE("%s: Invalid buffer pointer", __FUNCTION__); 
        }
        if(NULL == bytes_read)
        {
            SIRF_LOGE("%s: Invalid bytes_read pointer", __FUNCTION__); 
        }
        return SIRF_PAL_API_ERROR;
    }

    *bytes_read = 0;

    (void)SIRF_PAL_OS_MUTEX_Enter(s_net_mutex);
    if(SIRF_PAL_NET_SECURITY_NONE != ssl_socket->security)
    {
        //SIRF_LOGD("==== ssl read: ssl is %p ====", ssl_socket->ssl); 
        result = SSL_read(ssl_socket->ssl, (char*)(buffer+*bytes_read), size-*bytes_read);
    }
    else
    {
        //SIRF_LOGD("==== read: socket is %ld ======\n", ssl_socket->socket); 
        result = recv(ssl_socket->socket, (char*)(buffer+*bytes_read), size-*bytes_read, 0);
    }
    (void)SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);

    if ( 0 > result )
    {
        DEBUGMSG(1,(DEBUGTXT("recv() failed, errno=%d, size=%d, *bytes_read=%d\n"), errno, size, *bytes_read));
        SIRF_LOGE("**** recv() failed, errno=%d ****", errno);
        SIRF_PAL_OS_THREAD_Sleep(100);
        return SIRF_PAL_NET_ERROR;
    }

    *bytes_read = result;

    //SIRF_LOGD("==== SIRF_PAL_NET_Read success, read %d bytes ====", (int)(*bytes_read));

    return 0 < result ? SIRF_SUCCESS : SIRF_PAL_NET_CONNECTION_CLOSED;

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
    int  result;
    tSIRF_SOCKET_OPENSSL *ssl_socket = (tSIRF_SOCKET_OPENSSL *)socket_handle;

    if((SIRF_PAL_NET_INVALID_SOCKET == socket_handle) || (NULL == ssl_socket) ||
        (NULL == packet) || (NULL == bytes_written))
    {
        if((SIRF_PAL_NET_INVALID_SOCKET == socket_handle) || (NULL == ssl_socket))
        {
            SIRF_LOGE("%s: Invalid socket handle", __FUNCTION__); 
        }
        if(NULL == packet)
        {
            SIRF_LOGE("%s: Invalid buffer pointer", __FUNCTION__); 
        }
        if(NULL == bytes_written)
        {
            SIRF_LOGE("%s: Invalid bytes_written pointer", __FUNCTION__); 
        }
        return SIRF_PAL_API_ERROR;
    }

    *bytes_written=0;

    (void)SIRF_PAL_OS_MUTEX_Enter(s_net_mutex);
    if(SIRF_PAL_NET_SECURITY_NONE != ssl_socket->security)
    {
        //SIRF_LOGD("==== ssl is %p ====", ssl_socket->ssl);
        result = SSL_write(ssl_socket->ssl, (char*)(packet+*bytes_written), length-*bytes_written);
    }
    else
    {
        //SIRF_LOGD("==== socket is %ld ====", ssl_socket->socket);
        result = send( ssl_socket->socket, (char*)(packet+*bytes_written), length-*bytes_written, 0);
    }
    (void)SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);

    if ( 0 >= result )
    {
        DEBUGMSG(1,(DEBUGTXT("send() failed, errno=%d\n"), errno));
        SIRF_LOGE("**** send() failed, errno=%d ****", errno);
        SIRF_PAL_OS_THREAD_Sleep(100);
        return SIRF_PAL_NET_ERROR;
    }


    *bytes_written = result;

    //SIRF_LOGD("==== SIRF_PAL_NET_Write success, write %d bytes ====", (int)(*bytes_written));

    return SIRF_SUCCESS;

} /* SIRF_PAL_NET_Write() */

/**
* @brief Set the socket to run in listening mode.
* @param[in] socket_handle            Handle to the socket.
* @return                             Result code.
*/
tSIRF_RESULT SIRF_PAL_NET_Listen(tSIRF_SOCKET socket_handle)
{
    int result;
    tSIRF_SOCKET_OPENSSL *ssl_socket = (tSIRF_SOCKET_OPENSSL *)socket_handle;

    if((SIRF_PAL_NET_INVALID_SOCKET == socket_handle) || (NULL == ssl_socket))
    {
        SIRF_LOGE("%s: Invalid socket handle", __FUNCTION__);
        return SIRF_PAL_API_ERROR;
    }

    (void)SIRF_PAL_OS_MUTEX_Enter(s_net_mutex);
    result = listen(ssl_socket->socket, SOMAXCONN);
    (void)SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);

    return 0==result ? SIRF_SUCCESS : SIRF_PAL_NET_ERROR;

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
    tSIRF_SOCKET_OPENSSL *ssl_socket = (tSIRF_SOCKET_OPENSSL *)socket_handle;

    if((SIRF_PAL_NET_INVALID_SOCKET == socket_handle) || (NULL == ssl_socket))
    {
        SIRF_LOGE("%s: Invalid socket handle", __FUNCTION__);
        return SIRF_PAL_API_ERROR;
    }

    name->reserved_0 = AF_INET;
    (void)SIRF_PAL_OS_MUTEX_Enter(s_net_mutex);
    result = bind(ssl_socket->socket, (struct sockaddr*)name, sizeof(tSIRF_SOCKADDR));
    if (0 != result)
    {
        DEBUGMSG(1,(DEBUGTXT("bind() failed, errno=%d\n"), errno));
        SIRF_LOGE("**** net bind failed, errno=%d ****", errno);
    }
    (void)SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);

    return 0==result ? SIRF_SUCCESS : SIRF_PAL_NET_ERROR;
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
    tSIRF_SOCKET_OPENSSL *old_ssl_socket = (tSIRF_SOCKET_OPENSSL *)listener_handle;
    tSIRF_SOCKET_OPENSSL *new_ssl_socket = NULL;

    if((SIRF_PAL_NET_INVALID_SOCKET == listener_handle) || (NULL == old_ssl_socket) || (NULL == socket_handle))
    {
        if((SIRF_PAL_NET_INVALID_SOCKET == listener_handle) || (NULL == old_ssl_socket))
        {
            SIRF_LOGE("%s: Invalid listener handle", __FUNCTION__);
        }
        if(NULL == socket_handle)
        {
            SIRF_LOGE("%s: Invalid socket handle", __FUNCTION__);
        }
        return SIRF_PAL_API_ERROR;
    }

    *socket_handle = SIRF_PAL_NET_INVALID_SOCKET;

    if (SIRF_PAL_NET_SECURITY_NONE != security)
    {
        return SIRF_PAL_NET_SECURITY_NOT_SUPPORTED;
    }

    do
    {
        result = accept(old_ssl_socket->socket, (struct sockaddr*)name, (name ? &namelen : NULL) );
    } while ( 0 > result && EINTR == errno );

    if (!IsValidSocket(result))
    {
        SIRF_LOGE("**** accept failed! errno=%d ****", errno);
        return SIRF_PAL_NET_ERROR;
    }

    /* Find an unused internal socket */
    SIRF_PAL_OS_MUTEX_Enter(s_net_mutex);
    new_ssl_socket = find_next_avail_socket();
    if(NULL == new_ssl_socket)
    {
        SIRF_LOGE("No available free socket\n");
        close(result);
        SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);
        return SIRF_PAL_NET_MAX_SOCKETS_ALLOCATED;
    }

    new_ssl_socket->socket = (tSIRF_SOCKET)result;
    new_ssl_socket->security = SIRF_PAL_NET_SECURITY_NONE;
    SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);
    *socket_handle = (tSIRF_SOCKET)new_ssl_socket;

    return 0 < result ? SIRF_SUCCESS : SIRF_PAL_NET_ERROR;

} /* SIRF_PAL_NET_Accept() */

static int verify_callback(int preverify_ok, X509_STORE_CTX *ctx)
{
    SIRF_LOGD("==== verify_callback is called, preverify_ok is %d ====", preverify_ok);
    return preverify_ok;
}

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
    tSIRF_SOCKADDR       sockaddr;
    struct hostent *     host_addrs;
    tSIRF_UINT32         current_time;
    tSIRF_SOCKET_OPENSSL *ssl_socket = (tSIRF_SOCKET_OPENSSL *)socket_handle;
    int retry_count;

    if((SIRF_PAL_NET_INVALID_SOCKET == socket_handle) || (NULL == ssl_socket) || (NULL == addr))
    {
        if((SIRF_PAL_NET_INVALID_SOCKET == socket_handle) || (NULL == ssl_socket))
        {
            SIRF_LOGE("%s: Invalid socket handle", __FUNCTION__);
        }
        if(NULL == addr)
        {
            SIRF_LOGE("%s: Invalid addr", __FUNCTION__);
        }

        return SIRF_PAL_API_ERROR;
    }

    SIRF_LOGD("==== %s: socket is %ld, addr is :%s, port is %d, security is %ld ====",
        __FUNCTION__, ssl_socket->socket, addr, port, security);
    current_time = SIRF_PAL_OS_TIME_SystemTime();

    /* check for valid cached entry first */
    /* cache is considered valid if all the following conditions are met
    * 1. If cache entry is valid
    * 2. provided server name is the same as cached
    * 3. cache entry hasn't timeedout.
    */
    if(is_cache_valid &&
        (strcmp(addr, cached_server_name) == 0) &&
        ((current_time - cache_update_time) < CACHE_INVALIDATE_TIMEOUT))
    {
        SIRF_LOGD("INFO: Using cached IP address");
        sockaddr.reserved_0 = AF_INET;
        sockaddr.sin_addr   = cached_server_ip;
        sockaddr.sin_port   = htons(port);
    }
    else
    {
        retry_count = 0;

        /* mark cached entry as invalid */
        is_cache_valid = 0;
        SIRF_LOGD("INFO: Performing DNS lookup");
        do
        {
            host_addrs = gethostbyname( addr );
            retry_count++;
        } while(NULL == host_addrs && TRY_AGAIN == h_errno && retry_count < 1);
        if ((NULL == host_addrs) || (retry_count > 3)) 
        {
            SIRF_LOGE("====GetHostbyname failed : %d", retry_count);
            return SIRF_PAL_NET_ERROR;
        }
        else
        {
            p_addr = (unsigned long **)host_addrs->h_addr_list;

            sockaddr.reserved_0 = AF_INET;
            sockaddr.sin_addr   = **p_addr;
            sockaddr.sin_port   = htons(port);
            /* DNS lookup successful, update cache with this entry */
            is_cache_valid = 1;
            strncpy(cached_server_name, addr, sizeof(cached_server_name));
            cached_server_ip = sockaddr.sin_addr;
            cache_update_time = SIRF_PAL_OS_TIME_SystemTime();
        }
    }

    SIRF_LOGD("sin_addr = %u, sin_port = %d", (unsigned int)sockaddr.sin_addr, (unsigned int)sockaddr.sin_port);
    SIRF_LOGD("==== Going to connect to ip %08x ====", (unsigned int) sockaddr.sin_addr);

    retry_count = 0;
    do
    {
        result = connect(ssl_socket->socket, (struct sockaddr*)&sockaddr, sizeof(tSIRF_SOCKADDR));
        SIRF_LOGD("==== retry_count=%d, result=%d ====", retry_count, result);            
        if(result != 0)
        {
            SIRF_LOGE("==== result=%d, %s ====", result, strerror(errno));
            /* break if the system call was interrupted */
            if(EINTR == errno)
            {            
                break;
            }
            else
            {
                usleep(200000); // 200 mS
            }
        }
        else
        {
            SIRF_LOGD("==== connected=%d ====", result);
        }
    } while((result != 0) && (++retry_count < 20));

    if (0 != result)
    {
        SIRF_LOGE("ERROR: connect() to IP  failed, errno=%d", errno);
        return SIRF_PAL_NET_ERROR;
    }
    else
    {
        SIRF_LOGD("connect successful");
    }

    (void)SIRF_PAL_OS_MUTEX_Enter(s_net_mutex);
    ssl_socket->security = security;
    if (SIRF_PAL_NET_SECURITY_NONE != security)
    {
        unsigned long SSL_error;
        char SSL_err_buf[128];
        const SSL_METHOD *method;
        X509 *server_cert;
        char *strSubject;
        char *strIssuer;
        int err;

        SSL_load_error_strings();

        if (! SSL_library_init())
        {
            SSL_error = ERR_get_error();
            SIRF_LOGE("%s, **** SSL library init failed! ****", ERR_error_string(SSL_error, SSL_err_buf));
            (void)SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);
            return SIRF_PAL_NET_ERROR;
        }
        SIRF_LOGD("==== SSL library init succeed! ====");

        if( PAL_ssl_version == 0)
        {
            method = TLSv1_client_method();
        }
        else if( PAL_ssl_version == 1)
        {
            method = SSLv3_client_method();
        }
        else
        {
            method = SSLv23_client_method();
        }    
        //    method = SSLv23_client_method();
        SIRF_LOGD("==== method is %p, ====", method);
        //ssl_socket->ctx = SSL_CTX_new(SSLv23_client_method());
        ssl_socket->ctx = SSL_CTX_new(method);
        SIRF_LOGD("==== ctx is %p, ====", ssl_socket->ctx);
        if (NULL == ssl_socket->ctx)
        {
            SIRF_LOGE( "==== SSL_CTX_new return NULL ====");
            SSL_error = ERR_get_error();
            SIRF_LOGE("%s, **** Error new SSL CTX! ****", ERR_error_string(SSL_error, SSL_err_buf));
            (void)SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);
            return SIRF_PAL_NET_ERROR;
        }
        SIRF_LOGD("==== new SSL CTX succeed ====");
        if(1 != SSL_CTX_load_verify_locations(ssl_socket->ctx, ca_path, NULL))
        {
            SSL_error = ERR_get_error();
            SIRF_LOGE("%s, **** Error loading trust store! ****", ERR_error_string(SSL_error, SSL_err_buf));
            if (NULL != ssl_socket->ctx)
            {
                SSL_CTX_free(ssl_socket->ctx);
                ssl_socket->ctx = NULL;
            }
            (void)SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);
            return SIRF_PAL_NET_ERROR;
        }
        SIRF_LOGD("==== load certification succeed ====");

        //SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
        //SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, verify_callback);
        SSL_CTX_set_verify(ssl_socket->ctx, SSL_VERIFY_NONE, verify_callback);
        SIRF_LOGD("==== SSL_VERIFY_NONE ===");

        ssl_socket->ssl = SSL_new(ssl_socket->ctx);
        if(ssl_socket->ssl == NULL)
        {
            SSL_error = ERR_get_error();
            SIRF_LOGE("%s, **** SSL_new Error! ****", ERR_error_string(SSL_error, SSL_err_buf));
            if (NULL != ssl_socket->ctx)
            {
                SSL_CTX_free(ssl_socket->ctx);
                ssl_socket->ctx = NULL;
            }
            (void)SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);
            return SIRF_PAL_NET_ERROR;
        }
        SIRF_LOGD( "==== SSL_new succeed ====");

        SSL_set_fd(ssl_socket->ssl, ssl_socket->socket);
        err = SSL_connect(ssl_socket->ssl);
        if (err < 1)
        {
            SSL_error = ERR_get_error();
            err = SSL_get_error(ssl_socket->ssl, err);
            SIRF_LOGE("%s, **** SSL connect error #%d in accept, program terminated ****", ERR_error_string(SSL_error, SSL_err_buf), err);

            if(err == 5)
            {
                SIRF_LOGE("**** sockerrno is:%d ****", errno);
            }

            if (NULL != ssl_socket->ssl)
            {
                SSL_free(ssl_socket->ssl);
                ssl_socket->ssl = NULL;
            }

            if (NULL != ssl_socket->ctx)
            {
                SSL_CTX_free(ssl_socket->ctx);
                ssl_socket->ctx = NULL;
            }

            SIRF_LOGW("*** Need to shutdown and close socket");
            (void)SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);
            return SIRF_PAL_NET_ERROR;
        }
        SIRF_LOGD("==== SSL connection on socket %x, Version: %s, Cipher: %s ====",
            (unsigned int)ssl_socket->socket, SSL_get_version(ssl_socket->ssl), SSL_get_cipher(ssl_socket->ssl));

        server_cert = SSL_get_peer_certificate(ssl_socket->ssl);
        if (NULL == server_cert)
        {
            SSL_error = ERR_get_error();
            SIRF_LOGE("%s, **** get peer certificate failed ****", ERR_error_string(SSL_error, SSL_err_buf));
        }

        SIRF_LOGD("==== get peer certificate succeed  ====");

        strSubject = X509_NAME_oneline(X509_get_subject_name(server_cert),0,0);
        if (NULL == strSubject)
        {
            SSL_error = ERR_get_error();
            SIRF_LOGE("%s, **** get subject name failed ****", ERR_error_string(SSL_error, SSL_err_buf));
        }
        SIRF_LOGD("==== get subject name : %s ====", strSubject);

        strIssuer = X509_NAME_oneline(X509_get_issuer_name(server_cert),0,0);
        if (NULL == strIssuer)
        {
            SSL_error = ERR_get_error();
            SIRF_LOGE("%s, **** get issuer name failed ****", ERR_error_string(SSL_error, SSL_err_buf));
            //SIRF_LOGD("%s, **** get issuer name failed ****", ERR_error_string(SSL_error, SSL_err_buf));
        }
        SIRF_LOGD("==== get issuer name : %s ====", strIssuer);

        X509_free(server_cert);
        SIRF_LOGD("==== Establish SSL succeed ====");

        /*
        int verify_rst = SSL_get_verify_result(ssl);
        if(verify_rst != X509_V_OK)
        {
        DEBUGMSG(1,(DEBUGTXT("Check the certificate failed!\n")));
        g_logger(LAPI_LOG_INF, "**** Check the certificate failed, result is %d ****", verify_rst);
        SSL_CTX_free(ctx);
        return SIRF_PAL_NET_SECURITY_PEER_REFUSED;
        }
        g_logger(LAPI_LOG_INF, "==== check the certificate succeed ====");
        */
    }
    (void)SIRF_PAL_OS_MUTEX_Exit(s_net_mutex);

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
    tSIRF_SOCKET_OPENSSL *ssl_socket = (tSIRF_SOCKET_OPENSSL *)socket_handle;

    if((SIRF_PAL_NET_INVALID_SOCKET == socket_handle) || (ssl_socket == NULL))
    {
        SIRF_LOGE("%s: Invalid socket handle", __FUNCTION__);
        return SIRF_PAL_API_ERROR;
    }

    len = sizeof(tSIRF_SOCKADDR);
    result = getpeername( ssl_socket->socket, 
        (struct sockaddr*)(name?name:&tempname), 
        &len );
    if(result < 0)
    {
        SIRF_LOGE("**** SIRF_PAL_NET_GetPeerName failed! errno=%d ****", errno);
    }

    return 0 == result ? SIRF_SUCCESS : SIRF_PAL_NET_ERROR;

} /* SIRF_PAL_NET_GetPeerName() */

