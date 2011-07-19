
#include <stdio.h>
#include "sirfdump.h"
#include "sirf_msg.h"
#include "sirf_codec_ssb.h"
#include "sirf_codec_ascii.h"

int output_dump(struct transport_msg_t *msg, FILE *out_f, void *user_ctx)
{
   int err;
   tSIRF_UINT32 msg_id, msg_length;
   tSIRF_UINT32 str_size;
   uint8_t msg_structure[SIRF_MSG_SSB_MAX_MESSAGE_LEN];
   char str[1024];

   if (!msg || msg->payload_length < 1)
      return 1;

   err = SIRF_CODEC_SSB_Decode(msg->payload,
	 msg->payload_length,
	 &msg_id,
	 msg_structure,
	 &msg_length);
   if (err)
      return err;

   str_size=sizeof(str);
   err = SIRF_CODEC_ASCII_Encode(msg_id,
	    msg_structure,
	    msg_length,
	    str,
	    &str_size);

   if (err == 0 && out_f)
      fputs(str, out_f);

   return err;
}


