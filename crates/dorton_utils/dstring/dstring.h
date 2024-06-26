#pragma once

#include "defines.h"

#include <stdarg.h>

typedef struct DString
{
  char *string;
  u64 length;
} DString;

DAPI DResult dstring_create(DString *dstring, const char *string);
DAPI DResult dstring_destroy(DString *string);

DAPI DResult dstring_reserve(DString *dstring, u32 size);

DAPI DResult dstring_dup(DString *src, DString *dest);

DAPI dbool dstring_equal(DString *str1, DString *str2);
DAPI dbool dstring_equal_raw(const char *str1, const char *str2);

DAPI char *dstring_data(DString *dstring);
DAPI u64 dstring_length(DString *dstring);

DAPI DResult dstring_format(DString *dest, const char *format, ...);