#include "dstring.h"

#include "../../logger/logger.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

DResult dstring_create(DString *dstring, const char *string)
{
  if (string == NULL)
  {
    DERROR("dstring_create string is NULL.");
    return D_ERROR;
  }

  dstring->length = strlen(string);
  dstring->string = (char *)calloc((dstring->length + 1), sizeof(char));

  if (dstring->string)
  {
    strcpy(dstring->string, string);
  }
  else
  {
    dstring->length = 0;
    DERROR("Could not create dstring.");
    return D_ERROR;
  }

  return D_SUCCESS;
}

DResult dstring_destroy(DString *dstring)
{
  if (dstring->string)
  {
    free(dstring->string);
    dstring->string = NULL;
    dstring->length = 0;
  }
  else
  {
    DERROR("Cannot destroy a NULL dstring.");
    return D_ERROR;
  }
  return D_SUCCESS;
}

DResult dstring_dup(DString *src, DString *dest)
{
  if (src == NULL || dest == NULL)
  {
    DERROR("Could not dup dstrings.");
    return D_ERROR;
  }

  dest->length = src->length;
  dest->string = (char *)calloc(src->length + 1, sizeof(char));

  if (dest->string == NULL)
  {
    DERROR("Could not dup strings. dest->string was NULL after calloc.");
    return D_ERROR;
  }

  memcpy(dest->string, src->string, src->length + 1);

  return D_SUCCESS;
}

b8 dstring_equal(DString *str1, DString *str2)
{
  if (str1 == NULL || str2 == NULL)
  {
    return false;
  }

  return strcmp(str1->string, str2->string) == 0;
}

char *dstring_data(DString *dstring)
{
  return dstring->string;
}

u64 dstring_length(DString *dstring)
{
  return dstring->length;
}