
#include <stdio.h>
#include "sirfdump.h"
#include "sirf_msg.h"
#include "sirf_codec_ssb.h"
#include "sirf_codec_ascii.h"

/*  XXX  */
extern unsigned output_dump_use_gsw230_byte_order;

int output_dump(struct transport_msg_t *msg, FILE *out_f, void *user_ctx)
{
   int err;
   tSIRF_UINT32 msg_id, msg_length;
   tSIRF_UINT32 str_size;
   tSIRF_UINT32 options;
   uint8_t msg_structure[SIRF_MSG_SSB_MAX_MESSAGE_LEN];
   char str[1024];

   if (!msg || msg->payload_length < 1)
      return 1;

   options = output_dump_use_gsw230_byte_order;
   err = SIRF_CODEC_SSB_Decode(msg->payload,
	 msg->payload_length,
	 &msg_id,
	 msg_structure,
	 &msg_length,
         &options);
   if (err)
      return err;

   str_size=sizeof(str);
   err = SIRF_CODEC_ASCII_Encode(msg_id,
	    msg_structure,
	    msg_length,
	    (tSIRF_UINT8 *)str,
	    &str_size,
            &options);

   if (err == 0 && out_f) {
      fputs(str, out_f);
   }

   return err;
}


