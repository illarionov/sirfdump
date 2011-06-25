
#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/uio.h>

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sirf_msg.h"
#include "sirf_codec_ssb.h"
#include "sirf_codec_ascii.h"

const char *progname = "sirfdump";
const char *revision = "$Revision: 0.0 $";

struct opts_t {
   char *infile;
   char *outfile;
};

struct input_stream_t {
   int fd;
   uint8_t buf[1024];
   unsigned head, tail;
   int last_errno;
};

struct transport_msg_t {
   uint8_t *payload;
   unsigned payload_length;
   unsigned checksum;
   unsigned skipped_bytes;
};

struct ctx_t {
   struct opts_t opts;
   struct input_stream_t in;
   FILE *outfh;
};

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
   "    -F, --outfile               Output file, default: - (stdin)\n"
   "    -h, --help                  Help\n"
   "    -v, --version               Show version\n"
   "\n"
 );
 return;
}

static struct ctx_t *init_ctx()
{
   struct ctx_t *ctx;

   ctx = malloc(sizeof(*ctx));

   if (ctx == NULL) {
      perror(NULL);
      return NULL;
   }
   ctx->opts.infile = ctx->opts.outfile = NULL;
   ctx->in.fd = -1;
   ctx->in.head = ctx->in.tail = 0;
   ctx->in.last_errno = 0;
   ctx->outfh = NULL;

   return ctx;
}

static void free_ctx(struct ctx_t *ctx)
{
   if (ctx == NULL)
      return;
   free(ctx->opts.infile);
   free(ctx->opts.outfile);
   if (ctx->in.fd < 0 && (ctx->in.fd != STDIN_FILENO))
      close(ctx->in.fd);
   if (ctx->outfh && (ctx->outfh != stdout))
      fclose(ctx->outfh);
   free(ctx);
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
	 if (l <= 0)
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
	 if (l <= 0)
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
	 if (l <= 0)
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

int process(struct ctx_t *ctx)
{
   uint8_t *pkt;
   struct transport_msg_t msg;
   uint8_t msg_structure[SIRF_MSG_SSB_MAX_MESSAGE_LEN];
   tSIRF_UINT32 msg_id, msg_length;
   int err;

   while ( (pkt = readpkt(&ctx->in, &msg)) != NULL ) {
      msg_id = -1;
      msg_length = -1;
      err = SIRF_CODEC_SSB_Decode(msg.payload,
	    msg.payload_length,
	    &msg_id,
	    msg_structure,
	    &msg_length);
      if (err == 0) {
	 char str[1024];
	 tSIRF_UINT32 str_size = sizeof(str);
	 if (SIRF_CODEC_ASCII_Encode(msg_id,
	       msg_structure,
	       msg_length,
	       str,
	       &str_size) == 0) {
	    fputs(str, ctx->outfh);
	 }
      }
   }

   return ctx->in.last_errno;
}

int main(int argc, char *argv[])
{
   signed char c;
   struct ctx_t *ctx;

   static struct option longopts[] = {
      {"version",     no_argument,       0, 'v'},
      {"help",        no_argument,       0, 'h'},
      {"infile",      required_argument, 0, 'f'},
      {"outfile",     required_argument, 0, 'F'},
      {0, 0, 0, 0}
   };

   ctx = init_ctx();
   if (ctx == NULL)
      return 1;

   while ((c = getopt_long(argc, argv, "vh?f:F:",longopts,NULL)) != -1) {
      switch (c) {
	 case 'f':
	    if (set_file(&ctx->opts.infile, optarg) != 0) {
	       free_ctx(ctx);
	       return 1;
	    }
	    break;
	 case 'F':
	    if (set_file(&ctx->opts.outfile, optarg) != 0) {
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
      ctx->in.fd = open(ctx->opts.infile, O_RDONLY);
      if (ctx->in.fd < 0) {
	 perror(NULL);
	 free_ctx(ctx);
	 return 1;
      }
   }else
      ctx->in.fd = STDIN_FILENO;

   /* outfile  */
   if (ctx->opts.outfile != NULL) {
      ctx->outfh = fopen(ctx->opts.outfile, "a");
      if (ctx->outfh == NULL) {
	 perror(NULL);
	 free_ctx(ctx);
	 return 1;
      }
   }else
      ctx->outfh = stdout;

   process(ctx);

   free_ctx(ctx);
   return 0;
}


