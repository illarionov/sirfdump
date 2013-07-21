
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef WINCE
#include <Winbase.h>
#endif


#include "gps.h"
#include "sirfdump.h"
#include "sirf_msg.h"
#include "sirf_codec_ssb.h"
#include "nav.h"

extern const char *MonthName[];

struct rinex_nav_ctx_t {

   struct {
      char pgm[21];
      char run_by[21];
      char date[21];
   } file;

   fpos_t opt_header_pos;
   int opt_header_pos_valid;
   int header_printed;
   int opt_header_printed;
   unsigned gps_week;
   struct nav_data_t navdata;
};


static int print_nav_header(FILE *out_f, struct rinex_nav_ctx_t *ctx, const struct nav_sat_data_t *nav_data);
static int print_nav_optional_header(FILE *out_f, struct rinex_nav_ctx_t *ctx, const struct nav_sat_data_t *sat);
static int print_nav_data(FILE *out_f, struct rinex_nav_ctx_t *ctx, const struct nav_sat_data_t *nav_data);
static int handle_mid8_msg(struct rinex_nav_ctx_t *ctx,
      const tSIRF_MSG_SSB_50BPS_DATA *msg,
      FILE *out_f);
static double ura2meters(unsigned ura);

void *new_rinex_nav_ctx(int argc, char **argv)
{
   struct rinex_nav_ctx_t *ctx;

   if (argc || argv) {};

   ctx = malloc(sizeof(*ctx));

   if (ctx == NULL)
      return NULL;

   snprintf(ctx->file.pgm, sizeof(ctx->file.pgm), "sirfdump");
   ctx->file.run_by[0]='\0';

#ifndef WINCE
   {
       struct tm *tm;
       time_t clock;

       time(&clock);
       tm = gmtime(&clock);
       snprintf(ctx->file.date, sizeof(ctx->file.date), "%i-%3s-%02u %02i:%02i",
           tm->tm_mday, MonthName[tm->tm_mon], tm->tm_year % 100,
           tm->tm_hour, tm->tm_min);
   }
#else
   {
       SYSTEMTIME stm;
       GetSystemTime(&stm);
       snprintf(ctx->file.date, sizeof(ctx->file.date), "%i-%3s-%02u %02i:%02i",
           stm.wDay, MonthName[stm.wMonth-1], stm.wYear % 100,
           stm.wHour, stm.wMinute);
   }
#endif

   ctx->opt_header_pos_valid = 0;
   ctx->header_printed = ctx->opt_header_printed = 0;
   ctx->gps_week = 1024;

   init_nav_data(&ctx->navdata);

   return ctx;
}

void free_rinex_nav_ctx(void *ctx)
{
   struct rinex_nav_ctx_t *c;
   c = (struct rinex_nav_ctx_t *)ctx;
   free(c);
}

static void trimexp(char *buf, int length)
{
    if (length < 5) return;
    if (buf[length-5] == 'E'
        && ((buf[length-4] == '+') || (buf[length-4] == '-'))
        && (buf[length-3] == '0')
        && (isdigit(buf[length-2]))
        && (isdigit(buf[length-1]))) {
            buf[length-3] = buf[length-2];
            buf[length-2] = buf[length-1];
            buf[length-1] = '\0';
    }
}

static void outnavf(FILE *fp, double value)
{
    int length;
    char buf[30];
    length = _snprintf(buf, sizeof(buf), "%20.12E", value);
    if (length < 5) return;
    trimexp(buf, length);
    fprintf(fp, buf);
}

static void outnavhdrf(FILE *fp, double value)
{
    int length;
    char buf[30];
    length = _snprintf(buf, sizeof(buf), "%13.4E", value);
    if (length < 5) return;
    trimexp(buf, length);
    fprintf(fp, buf);
}

static void outnavrecf(FILE *fp, double v0, double v1, double v2, double v3)
{
    fputs("   ", fp);
    outnavf(fp, v0);
    outnavf(fp, v1);
    outnavf(fp, v2);
    outnavf(fp, v3);
    fputs("\n", fp);
}

int output_rinex_nav(struct transport_msg_t *msg, FILE *out_f, void *user_ctx)
{
   int err;
   struct rinex_nav_ctx_t *ctx;
   tSIRF_UINT32 msg_id, msg_length;
   union {
      tSIRF_MSG_SSB_50BPS_DATA data_50bps;
      tSIRF_MSG_SSB_CLOCK_STATUS data_clock;
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
   else if (msg_id == SIRF_MSG_SSB_CLOCK_STATUS)
      ctx->gps_week = m.data_clock.gps_week;

   return err;
}


static int handle_mid8_msg(struct rinex_nav_ctx_t *ctx,
      const tSIRF_MSG_SSB_50BPS_DATA *msg,
      FILE *out_f)
{
   int data_changed;
   struct nav_sat_data_t *dst;

   assert(ctx);
   assert(msg);
   assert(out_f);

   data_changed = populate_navdata_from_mid8(msg, &ctx->navdata);

   if (data_changed <= 0)
      return data_changed;

   dst = get_navdata_p(&ctx->navdata, msg->svid);
   if (!dst)
      return -1;

   if (!ctx->header_printed)
      ctx->header_printed = print_nav_header(out_f, ctx, dst);
   else if (!ctx->opt_header_printed && (data_changed & 0x02))
      ctx->opt_header_printed = print_nav_optional_header(out_f, ctx, dst);
   else if (0 && (data_changed & 0x02)) /* iono data changed  */
      print_nav_optional_header(out_f, ctx, dst);

   if (data_changed & 0x01)
      print_nav_data(out_f, ctx, dst);

   return 0;
}

static int print_nav_header(FILE *out_f, struct rinex_nav_ctx_t *ctx, const struct nav_sat_data_t *sat)
{

   assert(ctx);
   assert(out_f);
   assert(sat);

   fprintf(out_f, "%9.2f%-11s%c%-19s%-20s%-20s\n",
	 2.10,
	 "",
	 'N',
	 ": GPS NAV DATA",
	 "",
	 "RINEX VERSION / TYPE");
   fprintf(out_f, "%-20s%-20s%-20s%-20s\n", ctx->file.pgm, ctx->file.run_by, ctx->file.date,
	 "PGM / RUN BY / DATE");

   if ((ctx->opt_header_printed = print_nav_optional_header(out_f, ctx, sat)) == 0) {
      ctx->opt_header_pos_valid = !fgetpos(out_f, &ctx->opt_header_pos);
      fprintf(out_f,
	    "%-60s%-20s\n"
	    "%-60s%-20s\n"
	    "%-60s%-20s\n"
	    "%-60s%-20s\n",
	    "", "COMMENT",
	    "", "COMMENT",
	    "", "COMMENT",
	    "", "COMMENT");
   }else
      ctx->opt_header_printed = 1;

   fprintf(out_f, "%-60s%-20s\n", "",
	 "END OF HEADER");

   return 1;
}

static int print_nav_optional_header(FILE *out_f, struct rinex_nav_ctx_t *ctx, const struct nav_sat_data_t *sat)
{
   unsigned wn;
   unsigned seek_to_header;

   if (ctx->navdata.sub4_18.is_active == 0)
      return 0;
   if (sat->is_sub1_active == 0)
      return 0;

   seek_to_header = ctx->header_printed
      && (!ctx->opt_header_printed)
      && (ctx->opt_header_pos_valid);

   if (seek_to_header) {
      if (fsetpos(out_f, &ctx->opt_header_pos) != 0)
	 seek_to_header = 0; /*  not a seekable stream  */
   }


   /* XXX */
   wn = (ctx->gps_week & 0xfc00)
      | (sat->sub1.sub1.WN & 0x300)
      | (ctx->navdata.sub4_18.WNt & 0xff);

   fputs("  ", out_f);
   outnavhdrf(out_f, ctx->navdata.sub4_18.ion_alpha[0]);
   outnavhdrf(out_f, ctx->navdata.sub4_18.ion_alpha[1]);
   outnavhdrf(out_f, ctx->navdata.sub4_18.ion_alpha[2]);
   outnavhdrf(out_f, ctx->navdata.sub4_18.ion_alpha[3]);
   fprintf(out_f, "%-10s%-20s\n", "", "ION ALPHA");

   fputs("  ", out_f);
   outnavhdrf(out_f, ctx->navdata.sub4_18.ion_beta[0]);
   outnavhdrf(out_f, ctx->navdata.sub4_18.ion_beta[1]);
   outnavhdrf(out_f, ctx->navdata.sub4_18.ion_beta[2]);
   outnavhdrf(out_f, ctx->navdata.sub4_18.ion_beta[3]);
   fprintf(out_f, "%-10s%-20s\n", "", "ION BETA");

   fputs("   ", out_f);
   outnavf(out_f, ctx->navdata.sub4_18.a0);
   outnavf(out_f, ctx->navdata.sub4_18.a1);

   fprintf(out_f,
	 "%9u%9u %-20s\n"
	 "%6u%-54s%-20s\n",
	 (unsigned)floor(ctx->navdata.sub4_18.tot),
	 wn,
	 "DELTA-UTC: A0,A1,T,W",

	 ctx->navdata.sub4_18.leap,
	 "",
	 "LEAP SECONDS"
	 );

   if (seek_to_header)
      fseek(out_f, 0L, SEEK_END);

   return 1;
}

static int print_nav_data(FILE *out_f, struct rinex_nav_ctx_t *ctx, const struct nav_sat_data_t *nav_data)
{
    unsigned wn;
    struct gps_tm toc_tm;

    assert(nav_data);

    if (!nav_data->is_sub1_active
        || !nav_data->is_sub2_active
        || !nav_data->is_sub3_active)
        return 0;

    assert((nav_data->sub1.sub1.IODC & 0xff) == nav_data->sub2.sub2.IODE);
    assert((nav_data->sub1.sub1.IODC & 0xff) == nav_data->sub3.sub3.IODE);

    /* XXX */
    wn = (ctx->gps_week & 0xfc00) | (nav_data->sub1.sub1.WN & 0x3ff);
    gpstime2tm0(wn, nav_data->sub1.sub1.l_toc, &toc_tm);

    fprintf(out_f,
        "%2u%3u%3u%3u%3u%3u%5.1f",
        nav_data->sub1.tSVID,
        toc_tm.year % 100,
        toc_tm.month,
        toc_tm.day,
        toc_tm.hour,
        toc_tm.min,
        toc_tm.sec
        );

    outnavf(out_f, nav_data->sub1.sub1.d_af0);
    outnavf(out_f, nav_data->sub1.sub1.d_af1);
    outnavf(out_f, nav_data->sub1.sub1.d_af2);
    fputs("\n", out_f);

    outnavrecf(out_f,
        (double)nav_data->sub2.sub2.IODE,
        nav_data->sub2.sub2.d_Crs,
        nav_data->sub2.sub2.d_deltan * (double)GPS_PI,
        nav_data->sub2.sub2.d_M0
        );

    outnavrecf(out_f,
        nav_data->sub2.sub2.d_Cuc,
        nav_data->sub2.sub2.d_eccentricity,
        nav_data->sub2.sub2.d_Cus,
        nav_data->sub2.sub2.d_sqrtA
        );

    outnavrecf(out_f,
        (double)nav_data->sub2.sub2.l_toe,
        nav_data->sub3.sub3.d_Cic,
        nav_data->sub3.sub3.d_Omega0 * (double)GPS_PI,
        nav_data->sub3.sub3.d_Cis
        );

    outnavrecf(out_f,
        nav_data->sub3.sub3.d_i0 * (double)GPS_PI,
        nav_data->sub3.sub3.d_Crc,
        nav_data->sub3.sub3.d_omega * (double)GPS_PI,
        nav_data->sub3.sub3.d_Omegad * (double)GPS_PI
        );

    outnavrecf(out_f,
        nav_data->sub3.sub3.d_IDOT * (double)GPS_PI,
        (double)nav_data->sub1.sub1.l2,
        (double)wn,
        (double)nav_data->sub1.sub1.l2p
        );

    outnavrecf(out_f,
        ura2meters(nav_data->sub1.sub1.ura),
        (double)nav_data->sub1.sub1.hlth,
        nav_data->sub1.sub1.d_Tgd,
        (double)nav_data->sub1.sub1.IODC
        );

    /* XXX  */
    fputs("   ", out_f);
    outnavf(out_f, (double)nav_data->sub1.l_TOW17);
    fputs("\n", out_f);

    return 1;
}

static double ura2meters(unsigned ura)
{

   switch (ura) {
      case 1: return 2.8;
      case 3: return 5.7;
      case 5: return 11.3;
      default:
	 break;
   }

   if (ura <= 6)
      return pow(2.0, 1.0+(double)ura/2.0);

   if (ura < 15)
      return pow(2.0, (double)ura - 2.0);

   return 6145.0;
}



