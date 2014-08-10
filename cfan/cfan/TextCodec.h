#ifndef _CF_TEXTCODEC_H_
#define _CF_TEXTCODEC_H_

#include "cfan/Endian.h"

CF_BEGIN

int utf8_to_unicode(uint8_t *in, uint16_t **out, int *outsize);
int unicode_to_utf8(uint16_t *in, int insize, uint8_t **out);

CF_END
#endif // _CF_TEXTCODEC_H_
