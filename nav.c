
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "gpsd/gps.h"
#include "sirfdump.h"
#include "nav.h"
#include "sirf_msg.h"
#include "sirf_codec_ssb.h"

void init_nav_data(struct nav_data_t *data)
{
   unsigned i;

   assert(data);

   for(i=0; i < sizeof(data->prn)/sizeof(data->prn[0]); ++i) {
      data->prn[i].is_sub1_active =
	 data->prn[i].is_sub2_active =
	 data->prn[i].is_sub3_active =
	 data->prn[i].is_printed = 0;
   }
}

struct nav_sat_data_t *get_navdata_p(struct nav_data_t *data, unsigned prn)
{
   assert(data);

   if (prn > MAX_GPS_PRN)
      return NULL;

   return &data->prn[prn-1];
}

int populate_navdata_from_mid8(
      const tSIRF_MSG_SSB_50BPS_DATA *msg,
      struct nav_data_t *data
      )
{
   int data_changed;
   unsigned i;
   struct subframe_t subp;
   struct nav_sat_data_t *dst;
   uint32_t words[10];

   assert(msg);
   assert(data);

   for(i=0; i<10; i++)
      words[i]=msg->word[i];

   if (gpsd_interpret_subframe_raw(&subp, msg->svid, words) <= 0)
      return -1;

   data_changed = 0;
   if (subp.subframe_num > 3)
      return data_changed;

   dst = get_navdata_p(data, subp.tSVID);
   /* XXX  */
   if (dst == NULL)
      return -2;

   switch (subp.subframe_num) {
      case 1:
	 if (dst->is_sub1_active
	       && (dst->sub1.sub1.IODC == dst->sub1.sub1.IODC))
	    /*  skip subframe */
	    break;

	 memcpy(&dst->sub1, &subp, sizeof(subp));
	 dst->is_sub1_active = 1;
	 data_changed = 1;
	 if (dst->is_sub2_active
	       && ((dst->sub1.sub1.IODC & 0xff) != dst->sub2.sub2.IODE))
	    dst->is_sub2_active = 0;
	 if (dst->is_sub3_active
	       && ((dst->sub1.sub1.IODC & 0xff) != dst->sub3.sub3.IODE))
	    dst->is_sub3_active = 0;
	 break;
      case 2:
	 if (dst->is_sub2_active
	       && (dst->sub2.sub2.IODE == subp.sub2.IODE))
	    /*  skip subframe */
	    break;

	 memcpy(&dst->sub2, &subp, sizeof(subp));
	 data_changed = 1;
	 dst->is_sub2_active = 1;
	 if (dst->is_sub1_active
	       && ((dst->sub1.sub1.IODC & 0xff) != dst->sub2.sub2.IODE))
	    dst->is_sub1_active = 0;
	 if (dst->is_sub3_active
	       && (dst->sub3.sub3.IODE != dst->sub2.sub2.IODE))
	    dst->is_sub3_active = 0;
	 break;
      case 3:
	 if (dst->is_sub3_active
	       && (dst->sub3.sub3.IODE == subp.sub3.IODE))
	    /*  skip subframe */
	    break;

	 memcpy(&dst->sub3, &subp, sizeof(subp));
	 data_changed = 1;
	 dst->is_sub3_active = 1;
	 if (dst->is_sub1_active
	       && ((dst->sub1.sub1.IODC & 0xff) != dst->sub3.sub3.IODE))
	    dst->is_sub1_active = 0;
	 if (dst->is_sub2_active
	       && (dst->sub2.sub2.IODE != dst->sub3.sub3.IODE))
	    dst->is_sub2_active = 0;
	 break;
      default:
	 break;
   }

   return data_changed;
}


