
/*******************************************************************************\
*
*   File Name:      gpiotool.h
*
*   Description:    Low-level primitive proto-types that access the GPS IO's
*
*   Create Date:    16/03/2009
*
*   Author:         Gal Savion
*
\*******************************************************************************/

#ifndef __gpiotool_H__
#define __gpiotool_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GPS_LDO_ON_OFF_EXIST_IN_PLATFORM 1


#if GPS_LDO_ON_OFF_EXIST_IN_PLATFORM
	void	gpiotool_GpsTurnOffLdo(void);
	void	gpiotool_GpsTurnOnLdo(void);
#endif /* GPS_LDO_ON_OFF_EXIST_IN_PLATFORM */

	int		gpiotool_InitGpioPorts(void);
	int		gpiotool_DeInitGpioPorts(void);
	void	gpiotool_ClearGpsResetPort(void);
	void	gpiotool_SetGpsResetPort(void);
	void	gpiotool_ClearGpsOnOffPort(void);
	void	gpiotool_SetGpsOnOffPort(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __gpiotool_H__ */
