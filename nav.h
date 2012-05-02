#ifndef NAV_H
#define NAV_H

#include "gpsd/gps.h"
#include "sirf_msg.h"

struct nav_data_t {
   struct nav_sat_data_t {
      unsigned is_printed;
      unsigned is_sub1_active;
      unsigned is_sub2_active;
      unsigned is_sub3_active;
      struct subframe_t sub1;
      struct subframe_t sub2;
      struct subframe_t sub3;
   } prn[MAX_GPS_PRN+1];
};

void init_nav_data(struct nav_data_t *data);
struct nav_sat_data_t *get_navdata_p(struct nav_data_t *data, unsigned prn);
int populate_navdata_from_mid8(
      const tSIRF_MSG_SSB_50BPS_DATA *msg,
      struct nav_data_t *data
      );


#endif /* NAV_H */

