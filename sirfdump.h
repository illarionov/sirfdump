
#ifndef SIRFDUMP_H
#define SIRFDUMP_H

#include <stdint.h>
#include <stdio.h>

struct transport_msg_t {
   uint8_t *payload;
   unsigned payload_length;
   unsigned checksum;
   unsigned skipped_bytes;
};

typedef int (dumpf_t)(struct transport_msg_t *msg, FILE *out_f, void *user_ctx);

int output_dump(struct transport_msg_t *msg, FILE *out_f, void *user_ctx);
int output_nmea(struct transport_msg_t *msg, FILE *out_f, void *user_ctx);

void *new_rinex_ctx(int argc, char **argv);
void free_rinex_ctx(void *ctx);
int output_rinex(struct transport_msg_t *msg, FILE *out_f, void *user_ctx);

void *new_rinex_nav_ctx(int argc, char **argv);
void free_rinex_nav_ctx(void *ctx);
int output_rinex_nav(struct transport_msg_t *msg, FILE *out_f, void *user_ctx);



#endif /* SIRFDUMP_H */


