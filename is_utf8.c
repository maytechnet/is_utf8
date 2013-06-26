#include "is_utf8.h"

/*
  Check if the given unsigned char * is a valid utf-8 sequence.

  Return value :
  If the string is valid utf-8, 0 is returned.
  Else the position, starting from 1, is returned.

  Valid utf-8 sequences look like this :
  0xxxxxxx
  110xxxxx 10xxxxxx
  1110xxxx 10xxxxxx 10xxxxxx
  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
*/
int is_utf8(unsigned char *str, size_t len)
{
    size_t i = 0;
    size_t continuation_bytes = 0;
    size_t last = 0;

    while (i < len)
    {
        if (str[i] <= 0x7F)
            continuation_bytes = 0;
        else if (str[i] >= 0xC0 /*11000000*/ && str[i] <= 0xDF /*11011111*/) {
            continuation_bytes = 1;
            last = i;
        } else if (str[i] >= 0xE0 /*11100000*/ && str[i] <= 0xEF /*11101111*/) {
            continuation_bytes = 2;
            last = i;
        } else if (str[i] >= 0xF0 /*11110000*/ && str[i] <= 0xF4 /* Cause of RFC 3629 */) {
            continuation_bytes = 3;
            last = i;
        } else
            return i + 1;
        i += 1;
        while (i < len && continuation_bytes > 0
               && str[i] >= 0x80
               && str[i] <= 0xBF) {
            i += 1;
            continuation_bytes -= 1;
        }
        if (continuation_bytes != 0)
            return last + 1;
    }
    return 0;
}

