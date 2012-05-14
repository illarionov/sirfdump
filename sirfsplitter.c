
#define _GNU_SOURCE
#include <sys/types.h>

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sirfdump.h"
#include "sirf_msg.h"

const char *progname = "sirfsplitter";
const char *revision = "$Revision: 0.1 $";

struct opts_t {
   char *infile;
   char *dst_dir;
};

struct input_stream_t {
   int fd;
   uint8_t buf[1024];
   unsigned head, tail;
   int last_errno;
};

struct ctx_t {
   struct opts_t opts;
   struct input_stream_t in;

   unsigned gps_week;
   double gps_tow;
   struct gps_tm gps_time;

   char *out_fname;
   int outfd;

} ctx;


static void usage(void)
{
 fprintf(stdout, "\nUsage:\n    %s [-h] [options]\n"
       ,progname);
 return;
}

static void version(void)
{
 fprintf(stdout,"%s %s\n",progname,revision);
}

static void help(void)
{

 printf("%s - Sirf binary logger\t\t%s\n",
       progname, revision);
 usage();
 printf(
   "\nOptions:\n"
   "    -f, --infile                Input file, default: - (stdin)\n"
   "    -F, --dst_dir               Output directory, default: .\n"
   "    -h, --help                  Help\n"
   "    -v, --version               Show version\n"
   "\n"
 );
 return;
}

static struct ctx_t *init_ctx()
{
   Ctx.opts.infile = NULL;
   Ctx.opts.dst_dir = NULL;
   Ctx.in.fd = -1;
   Ctx.in.head = ctx->in.tail = 0;
   Ctx.in.last_errno = 0;

   Ctx.gps_week=0x400;
   Ctx.gps_tow=0;
   gpstime2tm0(Ctx.gps_week, Ctx.gps_tow, &Ctx.gps_time);
   Ctx.out_fname = NULL;
   Ctx.outfd = -1;

   return Ctx;
}

static void free_ctx(struct ctx_t *ctx)
{
   if (ctx == NULL)
      return;
   free(ctx->opts.infile);
   free(ctx->opts.outfile);
   if (ctx->in.fd > 0 && (ctx->in.fd != STDIN_FILENO))
      close(ctx->in.fd);
   if (ctx->outfd)
      close(ctx->outfh);
   free(ctx->out_fname);
}


static int set_file(char **dst, const char *optarg)
{
   assert(dst);
   assert(optarg);

   free(*dst);

   if (optarg[0] == '-' && (optarg[1] == '\0')) {
      *dst = NULL;
   }else {
      *dst = strdup(optarg);
      if (*dst == NULL) {
	 perror(NULL);
	 return 1;
      }
   }
   return 0;
}

static int read_data(struct input_stream_t *stream)
{
   ssize_t l;
   assert(
      !(stream->tail == sizeof(stream->buf)
	 && (stream->head == sizeof(stream->buf)))
      );

   if (stream->tail == sizeof(stream->buf)) {
      memmove(stream->buf, &stream->buf[stream->head],
	    stream->tail - stream->head);
      stream->tail = stream->tail - stream->head;
      stream->head=0;
   }

   l = read(stream->fd, &stream->buf[stream->tail], sizeof(stream->buf) - stream->tail);
   if (l<0)
      stream->last_errno = errno;
   else
      stream->tail = stream->tail+l;

   return l;
}

void *readpkt(struct input_stream_t *stream, struct transport_msg_t *res_msg)
{
   unsigned start_seq_found;
   unsigned payload_length;
   unsigned checksum;
   unsigned garbage_bytes;
   unsigned p0;
   size_t l;
   uint8_t *res;

   garbage_bytes = 0;
   start_seq_found=0;
   payload_length=0;

   for (;;) {
      while (stream->tail - stream->head < 8) {
	 l = read_data(stream);
	 if (l < 0)
	    return NULL;
      }

      /* search for start sequence  */
      while (stream->head < stream->tail-1) {
	 if ((stream->buf[stream->head] == 0xa0)
	       && (stream->buf[stream->head+1] == 0xa2)) {
	    start_seq_found=1;
	    break;
	 }else {
	    garbage_bytes++;
	    stream->head++;
	 }
      }
      if (!start_seq_found)
	 continue;

      while (stream->tail - stream->head < 6) {
	 l = read_data(stream);
	 if (l < 0)
	    return NULL;
      }

      /* get payload length  */
      payload_length = (0xff00 & (stream->buf[stream->head+2] << 8))
	 | (0xff & stream->buf[stream->head+3]);
      if (payload_length >= 1023) {
	 stream->head++;
	 continue;
      }

      /* load payload data */
      p0 = 2+2+payload_length;
      while (stream->tail - stream->head < p0+2+2) {
	 l = read_data(stream);
	 if (l < 0)
	    return NULL;
      }

      /* checksum  */
      checksum = (0xff00 & (stream->buf[stream->head+p0] << 8))
	 | (0xff & stream->buf[stream->head+p0+1]);

      /* end sequence  */
      if (stream->buf[stream->head+p0+2] != 0xb0
	    || (stream->buf[stream->head+p0+3] != 0xb3)) {
	 stream->head++;
	 continue;
      }
      break;
   } /* for(;;)  */

   res = &stream->buf[stream->head];
   stream->head = stream->head + 2+2+payload_length+2+2;
   if (stream->head == stream->tail) {
      stream->head = stream->tail = 0;
   }

   if (res_msg) {
      res_msg->payload = &res[4];
      res_msg->payload_length = payload_length;
      res_msg->checksum = checksum;
      res_msg->skipped_bytes = garbage_bytes;
   }

   return res;
}

static int update_time(struct ctx_t *ctx, unsigned week, double tow)
{
   unsigned hour_changed;

   struct gps_tm new_gps_time;

   gpstime2tm0(week, tow, &new_gps_time);

   hour_changed = (ctx->gps_time.tm_hour != new_gps_time.tm_hour)
	 || (ctx->gps_time.tm_mday != new_gps_time.tm_mday)
	 || (ctx->gps_time.tm_mon != new_gps_time.tm_mon)
	 || (ctx->gps_time.tm_year != new_gps_time.tm_year)
	 || (ctx->out_fname == NULL);

   gpstime2tm0(week, tow, &ctx->gps_time);

   if (!hour_changed)
      return 0;


   
   return hour_changed;
}


int main(int argc, char *argv[])
{
   signed char c;
   uint8_t *pkt;
   struct ctx_t *ctx;
   int err;
   unsigned week;
   double tow;
   struct transport_msg_t msg;

   static struct option longopts[] = {
      {"version",     no_argument,       0, 'v'},
      {"help",        no_argument,       0, 'h'},
      {"infile",      required_argument, 0, 'f'},
      {"dst_dir",     required_argument, 0, 'd'},
      {0, 0, 0, 0}
   };

   ctx = init_ctx();
   assert(ctx);

   while ((c = getopt_long(argc, argv, "vh?f:d:",longopts,NULL)) != -1) {
      switch (c) {
	 case 'f':
	    if (set_file(&ctx->opts.infile, optarg) != 0) {
	       free_ctx(ctx);
	       return 1;
	    }
	    break;
	 case 'd':
	    if (set_file(&ctx->opts.dst_dir, optarg) != 0) {
	       free_ctx(ctx);
	       return 1;
	    }
	    break;
	 case 'v':
	    version();
	    free_ctx(ctx);
	    exit(0);
	    break;
	 default:
	    help();
	    free_ctx(ctx);
	    exit(0);
	    break;
      }
   }
   argc -= optind;
   argv += optind;

   /* infile  */
   if (ctx->opts.infile != NULL) {
      ctx->in.fd = open(ctx->opts.infile, O_RDONLY
#ifdef O_BINARY
	 | O_BINARY
#endif
      );
      if (ctx->in.fd < 0) {
	 perror(NULL);
	 free_ctx(ctx);
	 return 1;
      }
   }else
      ctx->in.fd = STDIN_FILENO;

   while ( (pkt = readpkt(&ctx->in, &msg)) != NULL ) {
      size_t r;
      tSIRF_UINT32 msg_id, msg_length;
      union {
	 tSIRF_MSG_SSB_NL_MEAS_DATA nld;
	 tSIRF_MSG_SSB_MEASURED_NAVIGATION mn;
	 tSIRF_MSG_SSB_CLOCK_STATUS clock;
	 uint8_t u8[SIRF_MSG_SSB_MAX_MESSAGE_LEN];
      } m;

      if ( SIRF_CODEC_SSB_Decode(msg->payload,
	    msg->payload_length,
	    &msg_id,
	    m.u8,
	    &msg_length) != 0) {
	 fprintf(stderr,"SIRF_CODEC_SSB_Decode() error\n");
      }else {
	 switch (msg_id) {
	    case SIRF_MSG_SSB_NL_MEAS_DATA:
	       /* XXX: stalled gps_week */
	       update_time(ctx, ctx->gps_week, m.nld.gps_sw_time);
	       break;
	    case SIRF_MSG_SSB_MEASURED_NAVIGATION:
	       gps_week = (ctx->gps_week & 0xfc00) | (m.mn.gps_week & 0x3ff);
	       gps_tow = m.mn.gps_tow / 100.0;
	       update_time(ctx, gps_week, gps_tow);
	       break;
	    case SIRF_MSG_SSB_CLOCK_STATUS:
	       update_time(ctx, m.clock.gps_week, m.clock,gps_tow);
	       break;
	    default:
	       break;
	 }
      }

      /* write packet */
      r = write(STDOUT_FILENO, pkt, ctx->in.payload_length+8);
      if (r < 0) {
	 ctx->in.last_errno = errno;
	 perror("stdout write() error");
	 break;
      }else if (r < ctx->in.payload_length+8) {
	 fprintf(stderr, "stdout write(): written less then requested (%i < %i)\n",
	       r, ctx->in.payload_length+8);
	 ctx->in.last_errno = 1;
	 break;
      }

      if (ctx->outfd > 0) {
	 r = write(ctx->outfd, pkt, ctx->in.payload_length+8);
	 if (r < 0) {
	    ctx->in.last_errno = errno;
	    perror("outfd write() error");
	    break;
	 }else if (r < ctx->in.payload_length+8) {
	    fprintf(stderr, "outfd write(): written less then requested (%i < %i)\n",
		  r, ctx->in.payload_length+8);
	    ctx->in.last_errno = 1;
	    break;
	 }
      } /* if */


   } /* while(pkt) */

   err = ctx->in.last_errno;
   free_ctx(ctx);
   return err;
}

