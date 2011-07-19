
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

#endif /* SIRFDUMP_H */


