#include "dset.h"

#include "../../logger/logger.h"

#include <stdlib.h>
#include <string.h>

i32 default_int_compare(const void *val1, const void *val2)
{
  const i32 int1 = *(const i32 *)val1;
  const i32 int2 = *(const i32 *)val2;

  return int1 - int2;
}

i32 default_str_compare(const void *val1, const void *val2)
{
  const char *str1 = (const char *)val1;
  const char *str2 = (const char *)val2;

  return strcmp(str1, str2);
}

DResult _dset_create(DSet *set, u64 length, u64 stride, DCompareFunc compare_func)
{
  void *raw_set = (void *)calloc(length, stride);

  set->size = 0;
  set->capacity = length;
  set->stride = stride;
  set->raw_set = raw_set;

  set->compare_func = compare_func;

  return D_SUCCESS;
}

DResult _dset_destroy(DSet *set)
{
  if (set->raw_set)
  {
    free(set->raw_set);
    set->raw_set = NULL;
  }
  return D_SUCCESS;
}

DResult _dset_resize(DSet *set, u64 size)
{
  void *new_raw_set = realloc(set->raw_set, set->stride * size);
  if (!new_raw_set)
  {
    return D_ERROR;
  }

  if (size > set->capacity)
  {
    memset((char *)new_raw_set + set->stride * set->capacity, 0, set->stride * (size - set->capacity));
  }

  set->raw_set = new_raw_set;
  set->capacity = size;

  return D_SUCCESS;
}

DResult _dset_push_at(DSet *set, const void *val, u64 index)
{
  if (set->size == set->capacity)
  {
    DResult result = dset_resize(set, set->size * DSET_RESIZE_FACTOR);
    if (result != D_SUCCESS)
    {
      return result;
    }
  }

  char *base = (char *)set->raw_set + set->stride * index;
  memmove(base + set->stride, base, set->stride * (set->size - index));
  memcpy(base, val, set->stride);

  set->size++;

  return D_SUCCESS;
}

DResult _dset_push(DSet *set, const void *val)
{
  for (u32 i = 0; i < set->size; ++i)
  {
    const void *curr_val = dset_get(set, i);
    i32 comp_result = set->compare_func(val, curr_val);

    if (comp_result == 0)
    {
      return D_IGNORED;
    }
    if (comp_result < 0)
    {
      return _dset_push_at(set, val, i);
    }
  }

  return _dset_push_at(set, val, set->size);
}

DResult _dset_pop(DSet *set)
{
  if (set->size == 0)
  {
    DERROR("Cannot pop a set that is empty.");
    return D_ERROR;
  }

  set->size--;
  if (set->size <= set->capacity / DSET_RESIZE_FACTOR)
  {
    dset_resize(set, set->size + 1);
  }

  return D_SUCCESS;
}

DResult _dset_pop_at(DSet *set, u64 index)
{
  if (set->size == 0)
  {
    DERROR("Cannot pop a set that is empty.");
    return D_ERROR;
  }
  if (set->size <= index)
  {
    DERROR("Cannot pop a set with a size{%i} less than index{%i}.", set->size, index);
    return D_ERROR;
  }

  char *base = (char *)set->raw_set + set->stride * index;

  memmove(base, base + set->stride, set->stride * (set->size - index - 1));
  set->size--;

  if (set->size <= set->capacity / DSET_RESIZE_FACTOR)
  {
    dset_resize(set, set->size + 1);
  }

  return D_SUCCESS;
}

DResult _dset_pop_val(DSet *set, const void *val)
{
  i32 index = dset_find(set, val);
  if (index != -1)
  {
    return dset_erase(set, index);
  }

  return D_IGNORED;
}

const void *_dset_get(DSet *set, u64 index)
{
  if (set->size <= index)
  {
    DERROR("Cannot get an element at index{%i} with set size{%i}.", index, set->size);
    return NULL;
  }

  const char *val = (const char *)set->raw_set + set->stride * index;
  return val;
}

i32 _dset_find(DSet *set, const void *val)
{
  for (u32 i = 0; i < set->size; ++i)
  {
    if (set->compare_func(val, dset_get(set, i)) == 0)
    {
      return i;
    }
  }

  return -1;
}

void *dset_data(DSet *set)
{
  return set->raw_set;
}

dbool dset_empty(DSet *set)
{
  return set->size == 0;
}

u64 dset_size(DSet *set)
{
  return set->size;
}
