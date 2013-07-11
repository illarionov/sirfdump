
#include <stdio.h>
#include "sirfdump.h"
#include "sirf_msg.h"
#include "sirf_codec_ssb.h"
#include "sirf_proto_nmea.h"

int output_nmea(struct transport_msg_t *msg, FILE *out_f, void *ctx)
{
   int err;
   tSIRF_UINT32 msg_id, msg_length;
   tSIRF_UINT32 options;
   tSIRF_UINT32 str_size;
   uint8_t msg_structure[SIRF_MSG_SSB_MAX_MESSAGE_LEN];
   char str[1024];

   if (!msg || msg->payload_length < 1)
      return 1;

   options = 0;
   err = SIRF_CODEC_SSB_Decode(msg->payload,
	 msg->payload_length,
	 &msg_id,
	 msg_structure,
	 &msg_length,
         &options);

   if (err)
      return err;

   str[0]='\0';
   str_size = sizeof(str);

   options = 0;

   if (SIRF_PROTO_NMEA_Encode(msg_id,
               msg_structure,
               msg_length,
               (tSIRF_UINT8 *)str,
               &str_size,
               &options) == SIRF_SUCCESS) {
       fputs(str, out_f);
   }

   return err;
}


