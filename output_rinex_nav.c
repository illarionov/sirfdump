
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "gpsd/gps.h"
#include "sirfdump.h"
#include "sirf_msg.h"
#include "sirf_codec_ssb.h"

extern const char const *MonthName[];

#define MAX_GPS_PRN  32

struct tm0 {
   unsigned year, month, day, hour, min;
   float sec;
};

struct nav_data_t {
   unsigned is_printed;
   unsigned is_sub1_active;
   unsigned is_sub2_active;
   unsigned is_sub3_active;
   struct subframe_t sub1;
   struct subframe_t sub2;
   struct subframe_t sub3;
};

struct rinex_nav_ctx_t {

   struct {
      char pgm[21];
      char run_by[21];
      char date[21];
   } file;

   int header_printed;

   struct nav_data_t data[MAX_GPS_PRN];
};

int gpstime2tm0(unsigned gps_week, double gps_tow, struct tm0 *res);


static int print_nav_header(FILE *out_f, const struct rinex_nav_ctx_t *ctx);
static int print_nav_data(FILE *out_f, const struct nav_data_t *nav_data);
static int handle_mid8_msg(struct rinex_nav_ctx_t *ctx,
      tSIRF_MSG_SSB_50BPS_DATA *msg,
      FILE *out_f);

void *new_rinex_nav_ctx(int argc, char **argv)
{
   struct rinex_nav_ctx_t *ctx;
   struct tm *tm;
   unsigned i;
   time_t clock;

   if (argc || argv) {};

   ctx = malloc(sizeof(*ctx));

   if (ctx == NULL)
      return NULL;

   snprintf(ctx->file.pgm, sizeof(ctx->file.pgm), "sirfdump");
   ctx->file.run_by[0]='\0';
   time(&clock);
   tm = gmtime(&clock);
   snprintf(ctx->file.date, sizeof(ctx->file.date), "%i-%3s-%02u %02i:%02i",
	 tm->tm_mday, MonthName[tm->tm_mon], tm->tm_year % 100,
	 tm->tm_hour, tm->tm_min);

   ctx->header_printed = 0;

   for(i=0; i < MAX_GPS_PRN; i++) {
      ctx->data[i].is_sub1_active =
	 ctx->data[i].is_sub2_active =
	 ctx->data[i].is_sub3_active =
	 ctx->data[i].is_printed = 0;
   }

   return ctx;
}

void free_rinex_nav_ctx(void *ctx)
{
   struct rinex_nav_ctx_t *c;
   c = (struct rinex_nav_ctx_t *)ctx;
   free(c);
}


int output_rinex_nav(struct transport_msg_t *msg, FILE *out_f, void *user_ctx)
{
   int err;
   struct rinex_nav_ctx_t *ctx;
   tSIRF_UINT32 msg_id, msg_length;
   union {
      tSIRF_MSG_SSB_50BPS_DATA data_50bps;
      uint8_t u8[SIRF_MSG_SSB_MAX_MESSAGE_LEN];
   } m;
   char str[1024];

   assert(user_ctx);

   if (!msg || msg->payload_length < 1)
      return 1;

   ctx = (struct rinex_nav_ctx_t *)user_ctx;

   err = SIRF_CODEC_SSB_Decode(msg->payload,
	 msg->payload_length,
	 &msg_id,
	 m.u8,
	 &msg_length);

   if (err)
      return err;

   str[0]='\0';

   if (msg_id ==  SIRF_MSG_SSB_50BPS_DATA)
      handle_mid8_msg(ctx, &m.data_50bps, out_f);

   return err;
}


static int handle_mid8_msg(struct rinex_nav_ctx_t *ctx,
      tSIRF_MSG_SSB_50BPS_DATA *msg,
      FILE *out_f)
{
   unsigned i;
   struct subframe_t subp;
   struct subframe_t *dst_subp;
   struct nav_data_t *dst;
   uint32_t words[10];

   assert(ctx);
   assert(msg);
   assert(out_f);

   for(i=0; i<10; i++)
      words[i]=msg->word[i];

   if (gpsd_interpret_subframe_raw(&subp, msg->svid, words) <= 0)
      return -1;

   assert(subp.tSVID <= MAX_GPS_PRN);

   if (subp.subframe_num > 3)
      return 0;

   dst = &ctx->data[subp.tSVID-1];

   switch (subp.subframe_num) {
      case 1:
	 if (dst->is_sub1_active
	       && (dst->sub1.sub1.IODC == dst->sub1.sub1.IODC))
	    return 0;

	 memcpy(&dst->sub1, &subp, sizeof(subp));
	 dst->is_sub1_active = 1;
	 if (dst->is_sub2_active
	       && ((dst->sub1.sub1.IODC & 0xff) != dst->sub2.sub2.IODE))
	    dst->is_printed = dst->is_sub2_active = 0;
	 if (dst->is_sub3_active
	       && ((dst->sub1.sub1.IODC & 0xff) != dst->sub3.sub3.IODE))
	    dst->is_printed = dst->is_sub3_active = 0;
	 break;
      case 2:
	 if (dst->is_sub2_active
	       && (dst->sub2.sub2.IODE == subp.sub2.IODE))
	    /*  skip subframe */
	       break;

	 memcpy(&dst->sub2, &subp, sizeof(subp));
	 dst->is_sub2_active = 1;
	 if (dst->is_sub1_active
	       && ((dst->sub1.sub1.IODC & 0xff) != dst->sub2.sub2.IODE))
	    dst->is_printed = dst->is_sub1_active = 0;
	 if (dst->is_sub3_active
	       && (dst->sub3.sub3.IODE != dst->sub2.sub2.IODE))
	       dst->is_printed = dst->is_sub3_active = 0;
	 break;
      case 3:
	 if (dst->is_sub3_active
	       && (dst->sub3.sub3.IODE == subp.sub3.IODE))
	    /*  skip subframe */
	       break;

	 memcpy(&dst->sub3, &subp, sizeof(subp));
	 dst->is_sub3_active = 1;
	 if (dst->is_sub1_active
	       && ((dst->sub1.sub1.IODC & 0xff) != dst->sub3.sub3.IODE))
	    dst->is_printed = dst->is_sub1_active = 0;
	 if (dst->is_sub2_active
	       && (dst->sub2.sub2.IODE != dst->sub3.sub3.IODE))
	       dst->is_printed = dst->is_sub2_active = 0;
	 break;
      default:
	 return 0;
	 break;
   }

   if (!dst->is_printed
	 && dst->is_sub1_active
	 && dst->is_sub2_active
	 && dst->is_sub3_active
	 && ((dst->sub1.sub1.IODC & 0xff) == dst->sub2.sub2.IODE)
	 && ((dst->sub1.sub1.IODC & 0xff) == dst->sub3.sub3.IODE)
	 ) {

      if (!ctx->header_printed)
	 ctx->header_printed = print_nav_header(out_f, ctx);

      dst->is_printed =  print_nav_data(out_f, dst);
   }

   return 0;
}

static int print_nav_header(FILE *out_f, const struct rinex_nav_ctx_t *ctx)
{

   assert(ctx);
   assert(out_f);

   fprintf(out_f, "%9.2f%-11s%-20s%-20s%-20s\r\n",
	 2.10,
	 "",
	 "NAVIGATION DATA",
	 "",
	 "RINEX VERSION / TYPE");
   fprintf(out_f, "%-20s%-20s%-20s%-20s\r\n", ctx->file.pgm, ctx->file.run_by, ctx->file.date,
	 "PGM / RUN BY / DATE");

   fprintf(out_f, "%-60s%-20s\r\n", "",
	 "END OF HEADER");

   return 1;
}


static int print_nav_data(FILE *out_f, const struct nav_data_t *nav_data)
{
   unsigned wn;
   struct tm0 epoch;

   assert(nav_data);

   /* XXX */
   wn = nav_data->sub1.sub1.WN+1024;
   gpstime2tm0(wn, nav_data->sub1.sub1.l_toc, &epoch);

   fprintf(out_f,
	 "%2u%3u%3u%3u%3u%3u%5.1f%19.12lf%19.12lf%19.12lf\r\n",
	 nav_data->sub1.tSVID,
	 epoch.year % 100,
	 epoch.month,
	 epoch.day,
	 epoch.hour,
	 epoch.min,
	 epoch.sec,
	 /*  XXX */
	 nav_data->sub1.sub1.d_af0,
	 nav_data->sub1.sub1.d_af1,
	 nav_data->sub1.sub1.d_af2);

   fprintf(out_f,
	 "   %19.12lE%19.12lE%19.12lE%19.12lE\r\n"
	 "   %19.12lE%19.12lE%19.12lE%19.12lE\r\n"
	 "   %19.12lE%19.12lE%19.12lE%19.12lE\r\n"
	 "   %19.12lE%19.12lE%19.12lE%19.12lE\r\n"
	 "   %19.12lE%19.12lE%19.12lE%19.12lE\r\n"
	 "   %19.12lE%19.12lE%19.12lE%19.12lE\r\n"
	 "   %19.12lE%19.12lE\r\n",
	 (double)nav_data->sub2.sub2.IODE,
	 nav_data->sub2.sub2.d_Crs,
	 nav_data->sub2.sub2.d_deltan * (double)GPS_PI,
	 /* XXX  */
	 nav_data->sub2.sub2.d_M0,

	 nav_data->sub2.sub2.d_Cuc,
	 nav_data->sub2.sub2.d_eccentricity,
	 nav_data->sub2.sub2.d_Cus,
	 nav_data->sub2.sub2.d_sqrtA,

	 (double)nav_data->sub2.sub2.l_toe,
	 nav_data->sub3.sub3.d_Cic,
	 nav_data->sub3.sub3.d_Omega0 * (double)GPS_PI,
	 nav_data->sub3.sub3.d_Cis,

	 nav_data->sub3.sub3.d_i0 * (double)GPS_PI,
	 nav_data->sub3.sub3.d_Crc,
	 nav_data->sub3.sub3.d_omega * (double)GPS_PI,
	 nav_data->sub3.sub3.d_Omegad * (double)GPS_PI,

	 nav_data->sub3.sub3.d_IDOT * (double)GPS_PI,
	 (double)nav_data->sub1.sub1.l2,
	 (double)wn,
	 (double)nav_data->sub1.sub1.l2p,

	 (double)nav_data->sub1.sub1.ura,
	 (double)(nav_data->sub1.sub1.hlth & 0x40),
	 nav_data->sub1.sub1.d_Tgd,
	 (double)nav_data->sub1.sub1.IODC,

	 /* XXX  */
	 (double)nav_data->sub1.sub1.l_toc,
	 0.0
	 );

   return 1;
}



