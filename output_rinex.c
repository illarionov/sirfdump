
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sirfdump.h"
#include "sirf_msg.h"
#include "sirf_codec_ssb.h"

const char *MonthName[] = {"JAN","FEB","MAR","APR", "MAY","JUN","JUL","AUG", 
   "SEP", "OCO", "DEC"};

struct rinex_ctx_t {

   struct {
      char pgm[21];
      char run_by[21];
      char date[21];
   } file;

   char marker_name[21];
   char observer[21];
   char agency[21];

   struct {
      char no[21];
      char type[21];
      char version[21];
   } rec;

   struct {
      float x,y,z;
   } approx_pos;

   struct {
      char no[20];
      char type[20];
      float h, e, n;
   } antenna;

   struct {
      unsigned year, month, day, hour, min;
      float sec;
   } time_of_first_obs;

   int first_obs_found;
   int header_printed;

   unsigned gps_week;
   unsigned gps_tow;
   unsigned solution_svs;

   struct {
      unsigned sat_id;
      unsigned valid;
      double last_gps_sw_time;
      double pr;
      double cfreq;
      double phase;
      unsigned snr;
      unsigned last_phase_err_cnt;
   } ch[SIRF_NUM_CHANNELS];


};

static int handle_nl_meas_data_msg(struct rinex_ctx_t *ctx,
      tSIRF_MSG_SSB_NL_MEAS_DATA *msg);
static int handle_meas_nav_msg(struct rinex_ctx_t *ctx,
      tSIRF_MSG_SSB_MEASURED_NAVIGATION *msg);
static int handle_clock_status_msg(struct rinex_ctx_t *ctx,
      tSIRF_MSG_SSB_CLOCK_STATUS *msg);
static int hanble_50bpsdata_msg(struct rinex_ctx_t *ctx,
      tSIRF_MSG_SSB_50BPS_DATA *msg);
static int printf_obs_header(FILE *out_f, struct rinex_ctx_t *ctx);

void *new_rinex_ctx(int argc, char **argv)
{
   struct rinex_ctx_t *ctx;
   struct tm *tm;
   time_t clock;
   unsigned ch;

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

   ctx->marker_name[0]='\0';
   ctx->observer[0]='\0';
   ctx->agency[0]='\0';

   ctx->rec.no[0]='\0';
   ctx->rec.type[0]='\0';
   ctx->rec.version[0]='\0';

   ctx->approx_pos.x = ctx->approx_pos.y = ctx->approx_pos.z = 0;

   ctx->antenna.no[0]='\0';
   ctx->antenna.type[0]='\0';
   ctx->antenna.h = ctx->antenna.e = ctx->antenna.n = 0;

   ctx->time_of_first_obs.year = 0;
   ctx->time_of_first_obs.month = 0;
   ctx->time_of_first_obs.day = 0;
   ctx->time_of_first_obs.hour = 0;
   ctx->time_of_first_obs.min = 0;
   ctx->time_of_first_obs.sec = 0.0;

   ctx->first_obs_found = 0;
   ctx->header_printed = 0;

   ctx->gps_week=0;
   ctx->gps_tow=0;
   ctx->solution_svs=0;
   for (ch=0; ch < SIRF_NUM_CHANNELS; ch++) {
      ctx->ch[ch].valid = 0;
   }

   return ctx;
}

void free_rinex_ctx(void *ctx)
{
   struct rinex_ctx_t *c;
   c = (struct rinex_ctx_t *)ctx;
   free(c);
}


int output_rinex(struct transport_msg_t *msg, FILE *out_f, void *user_ctx)
{
   int err;
   struct rinex_ctx_t *ctx;
   tSIRF_UINT32 msg_id, msg_length;
   union {
      tSIRF_MSG_SSB_NL_MEAS_DATA nld;
      uint8_t u8[SIRF_MSG_SSB_MAX_MESSAGE_LEN];
   } m;
   char str[1024];

   assert(user_ctx);

   if (!msg || msg->payload_length < 1)
      return 1;

   ctx = (struct rinex_ctx_t *)user_ctx;

   err = SIRF_CODEC_SSB_Decode(msg->payload,
	 msg->payload_length,
	 &msg_id,
	 m.u8,
	 &msg_length);

   if (err)
      return err;

   str[0]='\0';

   switch (msg_id) {
      case SIRF_MSG_SSB_NL_MEAS_DATA:
	 fprintf(out_f, "msg 28. time_tag: %u chan: %02d sw_id: %03d gps_time: %11.2f\n",
	       m.nld.Timetag, m.nld.Chnl, m.nld.svid, m.nld.gps_sw_time);
	 handle_nl_meas_data_msg(ctx, &m.nld);
	 break;
      case SIRF_MSG_SSB_MEASURED_NAVIGATION:
	 handle_meas_nav_msg(ctx, (tSIRF_MSG_SSB_MEASURED_NAVIGATION *)&m);
	 break;
      case SIRF_MSG_SSB_CLOCK_STATUS:
	 handle_clock_status_msg(ctx, (tSIRF_MSG_SSB_CLOCK_STATUS *)&m);
	 break;
      case SIRF_MSG_SSB_50BPS_DATA:
	 hanble_50bpsdata_msg(ctx, (tSIRF_MSG_SSB_50BPS_DATA *)&m);
	 break;
      default:
	 break;
   }

   if (!ctx->header_printed && ctx->first_obs_found) {
      printf_obs_header(out_f, ctx);
      ctx->header_printed = 1;
   }

   return err;
}

static int handle_nl_meas_data_msg(struct rinex_ctx_t *ctx,
      tSIRF_MSG_SSB_NL_MEAS_DATA *msg)
{
   assert(ctx);
   assert(msg);

   return 1;
}

static int handle_meas_nav_msg(struct rinex_ctx_t *ctx,
      tSIRF_MSG_SSB_MEASURED_NAVIGATION *msg)
{
   assert(ctx);
   assert(msg);

   /* pmode  */
   if (((msg->nav_mode & 7) != 3) /* 3-SV solution (Kalman filter)*/
	 && ((msg->nav_mode & 7) != 4) /* >3-SV solution (Kalman filter) */
	 && ((msg->nav_mode & 7) != 5) /* 2-D point solution (least squares)  */
	 && ((msg->nav_mode & 7) != 6)) { /* 3-D point solution (least squares)  */
      fprintf(stderr, "msgid 2: no solution. week: %d tow: %d sws: %d pmode = 0x%x \n",
	    msg->gps_week, msg->gps_tow, msg->sv_used_cnt, msg->nav_mode & 7);
      return 1;
   }

   ctx->approx_pos.x = msg->ecef_x;
   ctx->approx_pos.y = msg->ecef_y;
   ctx->approx_pos.z = msg->ecef_z;

   ctx->gps_week = msg->gps_week;
   ctx->gps_tow = msg->gps_tow;
   ctx->solution_svs = msg->sv_used_cnt;

   ctx->first_obs_found=1;


   return 1;
}

static int handle_clock_status_msg(struct rinex_ctx_t *ctx,
      tSIRF_MSG_SSB_CLOCK_STATUS *msg)
{
   assert(ctx);
   assert(msg);

   return 1;
}

static int hanble_50bpsdata_msg(struct rinex_ctx_t *ctx,
      tSIRF_MSG_SSB_50BPS_DATA *msg)
{
   assert(ctx);
   assert(msg);

   return 1;
}



static int printf_obs_header(FILE *out_f, struct rinex_ctx_t *ctx)
{

   assert(ctx);
   assert(out_f);

   fprintf(out_f, "%9.2f%-11s%-20s%-20s%-20s\n", 2.11, "", "OBSERVATION DATA", "G",
	 "RINEX VERSION / TYPE");
   fprintf(out_f, "%-20s%-20s%-20s%-20s\n", ctx->file.pgm, ctx->file.run_by, ctx->file.date,
	 "PGM / RUN BY / DATE");
   fprintf(out_f, "%-60s%-20s\n", ctx->marker_name,
	 "MARKER NAME");
   fprintf(out_f, "%-20s%-40s%-20s\n", ctx->observer, ctx->agency,
	 "OBSERVER / AGENCY");
   fprintf(out_f, "%-20s%-20s%-20s%-20s\n", ctx->rec.no, ctx->rec.type, ctx->rec.version,
	 "REC # / TYPE / VERS");
   fprintf(out_f, "%-20s%-40s%-20s\n", ctx->antenna.no, ctx->antenna.type,
	 "ANT # / TYPE");
   fprintf(out_f, "%14.4f%14.4f%14.4f%18s%-20s\n", ctx->approx_pos.x, ctx->approx_pos.y, ctx->approx_pos.x, "",
	 "APPROX POSITION XYZ");
   fprintf(out_f, "%14.4f%14.4f%14.4f%18s%-20s\n", ctx->antenna.h, ctx->antenna.e, ctx->antenna.n, "",
	 "ANTENNA: DELTA H/E/N");
   fprintf(out_f, "%6i%6i%-48s%-20s\n", 1, 0, "",
	 "WAVELENGTH FACT L1/2");
   fprintf(out_f, "%6i    %-50s%-20s\n", 4, "C1    L1    S1    D1  ",
	 "# / TYPES OF OBSERV");
   fprintf(out_f, "%6d%6d%6d%6d%6d%13.7f%-5s%12s%-20s\n", ctx->time_of_first_obs.year,
	 ctx->time_of_first_obs.month, ctx->time_of_first_obs.day,
	 ctx->time_of_first_obs.hour, ctx->time_of_first_obs.min,
	 ctx->time_of_first_obs.sec, "", "",
	 "TIME OF FIRST OBS");
   fprintf(out_f, "%-60s%-20s\n", "",
	 "END OF HEADER");

   return 1;
}


