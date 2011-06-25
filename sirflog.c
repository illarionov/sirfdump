
#define _GNU_SOURCE
#include <assert.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char *progname = "sirflog";
const char *revision = "$Revision: 0.0 $";

struct opts_t {
   char *infile;
   char *outfile;
};

struct ctx_t {
   struct opts_t opts;

   FILE *infh;
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
   ctx->infh = ctx->outfh = NULL;

   return ctx;
}

static void free_ctx(struct ctx_t *ctx)
{
   if (ctx == NULL)
      return;
   free(ctx->opts.infile);
   free(ctx->opts.outfile);
   if (ctx->infh && (ctx->infh != stdin))
      fclose(ctx->infh);
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

int process(struct ctx_t *ctx)
{
   int err;
   unsigned char buf[200];

   err = 0;
   for(;;){
      break;
   }

   return err;
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
      ctx->infh = fopen(ctx->opts.infile, "r");
      if (ctx->infh == NULL) {
	 perror(NULL);
	 free_ctx(ctx);
	 return 1;
      }
   }else
      ctx->infh = stdin;

   /* outfile  */
   if (ctx->opts.outfile != NULL) {
      ctx->outfh = fopen(ctx->opts.infile, "a");
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


