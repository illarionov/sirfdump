
#include <sys/uio.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "sirfdump.h"
#include "sirf_msg.h"
#include "sirf_codec_ssb.h"
#include "gpsd/crc24q.h"

struct epoch_t {
      /* mid-7 */
      unsigned gps_week;
      double gps_tow;
      unsigned solution_svs;
      unsigned clock_drift; /*  Clock bias change rate, Hz */
      unsigned clock_bias; /* ns */

      /* mid-28 */
      struct {
         unsigned valid;
	 unsigned sat_id;
         double gps_soft_time;
	 double pseudorange;
	 double carrier_freq;
	 double carrier_phase;
	 unsigned sync_flags;
	 double min_cno;
         unsigned phase_err_cnt;
         unsigned low_power_cnt;
      } ch[SIRF_NUM_CHANNELS];

      /* Epoch */
      unsigned valid_channels;
      double epoch_time;
};

struct rtcm_ctx_t {
   struct epoch_t epoch;
};

static int handle_nl_meas_data_msg(struct rtcm_ctx_t *ctx,
      tSIRF_MSG_SSB_NL_MEAS_DATA *msg);
static int handle_meas_nav_msg(struct rtcm_ctx_t *ctx,
      tSIRF_MSG_SSB_MEASURED_NAVIGATION *msg);
static int handle_clock_status_msg(struct rtcm_ctx_t *ctx,
      tSIRF_MSG_SSB_CLOCK_STATUS *msg, FILE *out_f);

static void epoch_clear (struct epoch_t *e);
static void epoch_close(struct epoch_t *e);
static int epoch_printf(FILE *out_f, struct epoch_t *e);
static int rtcm_transport_write(FILE *out_f, void *data, unsigned size);

void *new_rtcm_ctx(int argc, char **argv)
{
   struct rtcm_ctx_t *ctx;
   struct tm *tm;
   time_t clock;

   if (argc || argv) {};

   ctx = malloc(sizeof(*ctx));

   if (ctx == NULL)
      return NULL;

   epoch_clear(&ctx->epoch);

   return ctx;
}

void free_rtcm_ctx(void *ctx)
{
   free(ctx);
}

int output_rtcm(struct transport_msg_t *msg, FILE *out_f, void *user_ctx)
{
   int err;
   struct rtcm_ctx_t *ctx;
   tSIRF_UINT32 msg_id, msg_length;
   union {
      tSIRF_MSG_SSB_NL_MEAS_DATA nld;
      uint8_t u8[SIRF_MSG_SSB_MAX_MESSAGE_LEN];
   } m;
   char str[1024];

   assert(user_ctx);

   if (!msg || msg->payload_length < 1)
      return 1;

   ctx = (struct rtcm_ctx_t *)user_ctx;

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
	 handle_nl_meas_data_msg(ctx, &m.nld);
	 break;
      case SIRF_MSG_SSB_MEASURED_NAVIGATION:
	 handle_meas_nav_msg(ctx, (tSIRF_MSG_SSB_MEASURED_NAVIGATION *)&m);
	 break;
      case SIRF_MSG_SSB_CLOCK_STATUS:
	 handle_clock_status_msg(ctx, (tSIRF_MSG_SSB_CLOCK_STATUS *)&m, out_f);
	 break;
      default:
	 break;
   }


   return err;
}

static int handle_nl_meas_data_msg(struct rtcm_ctx_t *ctx,
      tSIRF_MSG_SSB_NL_MEAS_DATA *msg)
{
   unsigned i;
   assert(ctx);
   assert(msg);

   /*
   fprintf(stderr, "msg 28. ch: %d svid: %d gps_time: %f pr: %f freq: %f "
	 "phase: %f flags: 0x%x(%s) delta_rang_int: %d phase_err: %d\n", msg->Chnl, msg->svid,
	 msg->gps_sw_time, msg->pseudorange,  msg->carrier_freq,
	 msg->carrier_phase, msg->sync_flags,
	 msg->sync_flags & 0x02 ? "valid" : "invalid",
	 msg->delta_range_interval,
	 msg->phase_error_count);
   */

   if (msg->Chnl >= SIRF_NUM_CHANNELS) {
      fprintf(stderr, "skipped msg 28 record: wrong channel %u\n", msg->Chnl);
      return -1;
   }

   ctx->epoch.ch[msg->Chnl].valid = msg->sync_flags;
   ctx->epoch.ch[msg->Chnl].sat_id = msg->svid;
   ctx->epoch.ch[msg->Chnl].gps_soft_time = msg->gps_sw_time;
   ctx->epoch.ch[msg->Chnl].pseudorange = msg->pseudorange;
   ctx->epoch.ch[msg->Chnl].carrier_freq = msg->carrier_freq;
   ctx->epoch.ch[msg->Chnl].carrier_phase = msg->carrier_phase;
   ctx->epoch.ch[msg->Chnl].sync_flags = msg->sync_flags;
   ctx->epoch.ch[msg->Chnl].min_cno = msg->cton[0];
   for (i=1; i<SIRF_NUM_POINTS; i++) {
      if (ctx->epoch.ch[msg->Chnl].min_cno > msg->cton[i])
	 ctx->epoch.ch[msg->Chnl].min_cno = msg->cton[i];
   }
   ctx->epoch.ch[msg->Chnl].phase_err_cnt = msg->phase_error_count;
   ctx->epoch.ch[msg->Chnl].low_power_cnt = msg->low_power_count;

   return 1;
}

static int handle_meas_nav_msg(struct rtcm_ctx_t *ctx,
      tSIRF_MSG_SSB_MEASURED_NAVIGATION *msg)
{
   assert(ctx);
   assert(msg);

   /*
   fprintf(stderr, "msg 2 (nav). week: %d tow: %lu sws: %d pmode: 0x%x\n",
	 msg->gps_week, msg->gps_tow, msg->sv_used_cnt, msg->nav_mode & 7);
	 */

   /* pmode  */
   if (((msg->nav_mode & 7) != 3) /* 3-SV solution (Kalman filter)*/
	 && ((msg->nav_mode & 7) != 4) /* >3-SV solution (Kalman filter) */
	 && ((msg->nav_mode & 7) != 5) /* 2-D point solution (least squares)  */
	 && ((msg->nav_mode & 7) != 6)) { /* 3-D point solution (least squares)  */
      /* no slution */
      /* XXX  */
   }

   ctx->epoch.gps_week = (ctx->epoch.gps_week & 0xfc00) | (msg->gps_week & 0x3ff);
   ctx->epoch.gps_tow = msg->gps_tow / 100.0;
   ctx->epoch.solution_svs = msg->sv_used_cnt;

   return 1;
}

static int handle_clock_status_msg(struct rtcm_ctx_t *ctx,
      tSIRF_MSG_SSB_CLOCK_STATUS *msg, FILE *out_f)
{
   assert(ctx);
   assert(msg);

   /*
   fprintf(stderr, "msg 7 (clock). week: %u tow: %f svs: %d drift_hz: %ld bias: %ld\n", msg->gps_week,
	 msg->gps_tow/100.0, msg->sv_used_cnt, msg->clk_offset, msg->clk_bias);
	 */

   /* Extended GPS week  */
   ctx->epoch.gps_week = msg->gps_week;
   ctx->epoch.gps_tow = msg->gps_tow / 100.0;
   ctx->epoch.solution_svs = msg->sv_used_cnt;
   ctx->epoch.clock_bias = msg->clk_bias;
   ctx->epoch.clock_drift = msg->clk_offset;

   epoch_close(&ctx->epoch);
   epoch_printf(out_f, &ctx->epoch);
   epoch_clear(&ctx->epoch);

   return 1;
}

static void epoch_clear (struct epoch_t *e)
{
   unsigned ch;
   assert(e);

   e->gps_week=0x400;
   e->gps_tow=0;
   e->solution_svs=0;

   for (ch=0; ch < SIRF_NUM_CHANNELS; ch++) {
      e->ch[ch].valid = 0;
   }

   e->valid_channels=0;
   e->epoch_time=0;
}

static int is_sat_in_epoch(const struct epoch_t *e, unsigned chan_id)
{
   return ( fabs(e->gps_tow + ((double)e->clock_bias / 1.0e9) - e->ch[chan_id].gps_soft_time) < 0.1);
}

static void epoch_close(struct epoch_t *e)
{
   unsigned chan_id;

   assert(e);

   e->valid_channels = 0;
   e->epoch_time = 0;

   for (chan_id=0; chan_id < SIRF_NUM_CHANNELS; chan_id++) {
      if (!e->ch[chan_id].valid)
	 continue;

      if (!is_sat_in_epoch(e, chan_id)) {
	 fprintf(stderr, "Found mid28 from different epoch. Current: %.5lf mid28: %.5lf\n",
	       (double)(e->gps_tow + (e->clock_bias / 1.0e9)),
	       (double)e->ch[chan_id].gps_soft_time);
	 e->ch[chan_id].valid = 0;
	 continue;
      }

      ++e->valid_channels;
      /* Use first valid channel time as epoch time   */
      if (e->epoch_time == 0)
	 e->epoch_time = (double)e->ch[chan_id].gps_soft_time - (double)e->clock_bias / 1.0e9;
   }
}

/* XXX  */
static unsigned set_ubits(uint8_t *buf, unsigned pos, int len, unsigned val)
{
   int i;

   pos = pos + len - 1;

   for (i=len-1; i>=0; --i) {
      if (val & 1)
	 buf[ pos/8 ] |= 1<<(7-(pos%8));
      else
	 buf[ pos/8 ] &= ~(1<<(7-(pos%8)));

      val >>= 1;
      pos -= 1;
   }

   return len;
}

/* XXX */
static inline unsigned set_sbits(uint8_t *buf, unsigned pos, unsigned len, int val)
{
   return set_ubits(buf, pos, len, (unsigned)val);
}

static int epoch_printf(FILE *out_f, struct epoch_t *e)
{
   unsigned pos;
   unsigned chan_id;
   uint8_t msg1002[8+(int)(9.25*SIRF_NUM_CHANNELS)+1];

   if (e->valid_channels == 0)
      return -1;

   pos = 0;

   /* MSG1002 Header  */

   /* DF002 uint12 message number */
   pos += set_ubits(msg1002, pos, 12, 1002);
   /* DF003 uint12 reference station id  */
   pos += set_ubits(msg1002, pos, 12, 0);
   /* DF004 uint30 TOW   */
   pos += set_ubits(msg1002, pos, 30, (unsigned)(e->epoch_time*1000.0)  & 0x3fffffff);
   /* DF005 bit(1) synchronous GNSS flag  */
   pos += set_ubits(msg1002, pos, 1, 0);
   /* DF006 uint5  no of GPS satellite signals processed */
   pos += set_ubits(msg1002, pos, 5, e->valid_channels);
   /* DF007 bit(1) divergence-free smoothind indicator   */
   pos += set_ubits(msg1002, pos, 1, 0);
   /* DF008 bit(3) GPS smooting interval   */
   pos += set_ubits(msg1002, pos, 3, 0);

   assert(pos == 64);

   /* Data  */
   for (chan_id=0; chan_id < SIRF_NUM_CHANNELS; ++chan_id) {
      double pr, phase;
      unsigned lock_time;

      if (!e->ch[chan_id].valid)
	 continue;

      pr = e->ch[chan_id].pseudorange - (SPEED_OF_LIGHT * (e->clock_bias / 1.0e9));
      phase = e->ch[chan_id].carrier_phase - e->ch[chan_id].pseudorange;
      /* XXX  */
      lock_time = 0;

      /* DF009 uint6 sattelite id  */
      pos += set_ubits(msg1002, pos, 6, e->ch[chan_id].sat_id);
      /* DF010 bit(1) L1 code indicator  */
      pos += set_ubits(msg1002, pos, 1, 0);
      /* DF011 uint24 pseudorange  */
      pos += set_ubits(msg1002, pos, 24, (unsigned)(fmod(pr, SPEED_OF_LIGHT / 1000.0) / 0.02));
      /* DF012 int20 PhaseRange-Pseudorange  */
      pos += set_sbits(msg1002, pos, 20, (int)(phase / 0.0005));
      /* DF013 uint7 L1 lock time indicator  */
      pos += set_ubits(msg1002, pos, 7, lock_time);
      /* DF014 uint8 L1 pseudorange modulus ambiguity  */
      pos += set_ubits(msg1002, pos, 8, (unsigned)(pr / (SPEED_OF_LIGHT / 1000.0)));
      /* DF015 uint8 L1 CNR  */
      pos += set_ubits(msg1002, pos, 8, (unsigned)(e->ch[chan_id].min_cno / 0.25));
   }

   return rtcm_transport_write(out_f, msg1002, (pos+7)/8);
}


static int rtcm_transport_write(FILE *out_f, void *data, unsigned size)
{
   unsigned crc24;
   uint8_t  header[3];
   uint8_t  footer[3];
   struct iovec crc_iovec[] = {
      {header, sizeof(header)},
      {data, (size_t)size},
   };

   assert(size <= 1023);

   header[0] = 0xd3;
   header[1] = (size >> 8) & 0x03;
   header[2] = size & 0xff;

   crc24 = crc24q_hashv(crc_iovec, sizeof(crc_iovec)/sizeof(crc_iovec[0]));
   footer[0] = (crc24 >> 16) & 0xff;
   footer[1] = (crc24 >> 8) & 0xff;
   footer[2] = crc24 & 0xff;

   if (fwrite(header, sizeof(header), 1, out_f) < 1)
      return -1;
   if (fwrite(data, size, 1, out_f) < 1)
      return -1;
   if (fwrite(footer, sizeof(footer), 1, out_f) < 1)
      return -1;

   return 0;
}

