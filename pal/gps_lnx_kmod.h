/* ===========================================================================
File        : gps_lnx_kmod.h

Notes       : 
=========================================================================== */


/************************************************************************/
#ifndef _gps_lnx_kmod_h_
#define _gps_lnx_kmod_h_
/* Header files includes */
#include <linux/ioctl.h>


/*************************************************************************
 * Constants
 *************************************************************************/

/* user level ioctl commands for accessing APIs */

#define GPS_SIRF_IOC_MAGIC					'G'
#define IOCTL_GPS_RESET_N_CLEAR				_IOW(GPS_SIRF_IOC_MAGIC, 0, int)
#define IOCTL_GPS_RESET_N_SET				_IOW(GPS_SIRF_IOC_MAGIC, 1, int)
#define IOCTL_GPS_ON_OFF_CLEAR				_IOW(GPS_SIRF_IOC_MAGIC, 2, int)
#define IOCTL_GPS_ON_OFF_SET				_IOW(GPS_SIRF_IOC_MAGIC, 3, int)
#define IOCTL_GPS_LDO_ON_OFF_CLEAR			_IOW(GPS_SIRF_IOC_MAGIC, 4, int)
#define IOCTL_GPS_LDO_ON_OFF_SET			_IOW(GPS_SIRF_IOC_MAGIC, 5, int)


#endif /* _GPS_LINUX_KERNEL_MODULE_H_ */

