#pragma once

#include "defines.h"

typedef struct DString
{
  char *string;
  u64 length;
} DString;

DAPI DResult dstring_create(DString *dstring, const char *string);
DAPI DResult dstring_destroy(DString *string);

DAPI DResult dstring_dup(DString *src, DString *dest);
DAPI b8 dstring_equal(DString *str1, DString *str2);

DAPI char *dstring_data(DString *dstring);
DAPI u64 dstring_length(DString *dstring);