
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

   int first_record_found;
   int last_phase_err_cnt;
};

static int printf_obs_header(FILE *out_f, struct rinex_ctx_t *ctx);

void *new_rinex_ctx(int argc, char **argv)
{
   struct rinex_ctx_t *ctx;
   struct tm *tm;
   time_t clock;

   ctx = malloc(sizeof(*ctx));

   if (ctx == NULL)
      return NULL;

   snprintf(ctx->file.pgm, sizeof(ctx->file.pgm), "sirfdump");
   ctx->file.run_by[0]='\0';
   time(&clock);
   tm = gmtime(&clock);
   snprintf(ctx->file.date, sizeof(ctx->file.date), "%i-%3s-%2u %2i:%2i",
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

   ctx->first_record_found = 0;
   ctx->last_phase_err_cnt = 0;

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
      tSIRF_MSG_SSB_MEASURED_TRACKER mt;
      tSIRF_MSG_SSB_GEODETIC_NAVIGATION gn;
      uint8_t u8[SIRF_MSG_SSB_MAX_MESSAGE_LEN];
   } msg_structure;
   char str[1024];

   if (!msg || msg->payload_length < 1)
      return 1;

   ctx = (struct rinex_ctx_t *)user_ctx;

   err = SIRF_CODEC_SSB_Decode(msg->payload,
	 msg->payload_length,
	 &msg_id,
	 msg_structure.u8,
	 &msg_length);

   if (err)
      return err;

   str[0]='\0';

   if (ctx->first_record_found == 0) {
      printf_obs_header(out_f, ctx);
      ctx->first_record_found=1;
   }


   return err;
}

static int printf_obs_header(FILE *out_f, struct rinex_ctx_t *ctx)
{
   fprintf(out_f, "%9.2f %19s %19s %20s\n", 2.11, "OBSERVATION DATA", "G",
	 "RINEX VERSION / TYPE");
   fprintf(out_f, "%20s%20s%20s%20s", ctx->file.pgm, ctx->file.run_by, ctx->file.date,
	 "PGM / RUN BY / DATE");
   fprintf(out_f, "%60s%20s\n", ctx->marker_name,
	 "MARKER NAME");
   fprintf(out_f, "%20s%40s%20s\n", ctx->observer, ctx->agency,
	 "OBSERVER / AGENCY");
   fprintf(out_f, "%20s%20s%20s%20s\n", ctx->rec.no, ctx->rec.type, ctx->rec.version,
	 "REC # / TYPE / VERS");
   fprintf(out_f, "%20s%40s%20s\n", ctx->antenna.no, ctx->antenna.type,
	 "ANT # / TYPE");
   fprintf(out_f, "%14.4f%14.4f%14.4f%20s\n", ctx->approx_pos.x, ctx->approx_pos.y, ctx->approx_pos.x,
	 "APPROX POSITION XYZ");
   fprintf(out_f, "%14.4f%14.4f%14.4f%20s\n", ctx->antenna.h, ctx->antenna.e, ctx->antenna.n,
	 "ANTENNA: DELTA H/E/N");
   fprintf(out_f, "%6i%6i%48s%20s\n", 1, 0, "",
	 "WAVELENGTH FACT L1/2");
   fprintf(out_f, "%6i%54s%30s\n", 4, "C1    L1    S1    D1  ",
	 "# / TYPES OF OBSERV");
   fprintf(out_f, "%6d%6d%6d%6d%6d%13.7f%5s   %20s\n", ctx->time_of_first_obs.year,
	 ctx->time_of_first_obs.month, ctx->time_of_first_obs.day,
	 ctx->time_of_first_obs.hour, ctx->time_of_first_obs.min,
	 ctx->time_of_first_obs.sec, "GPS",
	 "TIME OF FIRST OBS");
   fprintf(out_f, "%60s%20s\n", "",
	 "END OF HEADER");

}


