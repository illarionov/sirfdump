
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "gpsd/gps.h"
#include "sirfdump.h"
#include "sirf_msg.h"
#include "sirf_codec_ssb.h"

extern const char const *MonthName[];

#define L1_CARRIER_FREQ 1575420000.0
#define SPEED_OF_LIGHT 299792458.0
#define GPS_EPOCH  315964800 /*  GPS epoch in Unix time */

struct tm0 {
   unsigned year, month, day, hour, min;
   float sec;
};

struct rinex_nav_ctx_t {

   struct {
      char pgm[21];
      char run_by[21];
      char date[21];
   } file;

   int header_printed;

};

static int print_obs_header(FILE *out_f, struct rinex_nav_ctx_t *ctx);
int gpstime2tm0(unsigned gps_week, double gps_tow, struct tm0 *res);
static int handle_mid8_msg(struct rinex_nav_ctx_t *ctx,
      tSIRF_MSG_SSB_50BPS_DATA *msg,
      FILE *out_f);

void *new_rinex_nav_ctx(int argc, char **argv)
{
   struct rinex_nav_ctx_t *ctx;
   struct tm *tm;
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
   uint32_t words[10];

   assert(ctx);
   assert(msg);
   assert(out_f);

   for(i=0; i<10; i++)
      words[i]=msg->word[i];

   /*
   fprintf(stderr, "msg 8 (50 BPS data). sv_id: %2d data: "
	 "%08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx\n"
	 ,
	 (unsigned)msg->svid,
	 msg->word[0], msg->word[1], msg->word[2], msg->word[3],
	 msg->word[4], msg->word[5], msg->word[6], msg->word[7],
	 msg->word[8], msg->word[9]
	 );
	 */
   if (gpsd_interpret_subframe_raw(&subp, msg->svid, words) <= 0)
      return -1;

   if (!ctx->header_printed)
      ctx->header_printed = print_obs_header(out_f, ctx);

   return 0;
}

static int print_obs_header(FILE *out_f, struct rinex_nav_ctx_t *ctx)
{

   assert(ctx);
   assert(out_f);

   fprintf(out_f, "%9.2f%-11s%-20s%-20s%-20s\r\n",
	 2.11,
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





