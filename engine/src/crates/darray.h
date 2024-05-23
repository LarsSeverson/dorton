#pragma once

#include "defines.h"

typedef struct DArray
{
  void *raw_array;

  u64 capacity;
  u64 size;
  u64 stride;
} DArray;

DAPI DResult _darray_create(DArray *array, u64 length, u64 stride);
DAPI DResult _darray_destroy(DArray array);

DAPI DResult _darray_set(void *array, u64 field);

DAPI DResult _darray_resize(DArray *array, u64 size);

DAPI DResult _darray_push(DArray *array, const void *val);
DAPI DResult _darray_pop(void *array, void *dest);

DAPI void *_darray_push_at(void *array, const void *val);
DAPI DResult _darray_pop_at(void *array, void *dest);

#define DARRAY_DEFAULT_SIZE 1
#define DARRAY_RESIZE_FACTOR 2

#define darray_create(array, type) \
  _darray_create(array, DARRAY_DEFAULT_SIZE, sizeof(type))

#define darray_reserve(array, size, type) \
  _darray_create(array, size, sizeof(type))

#define darray_destroy(array) \
  _darray_destroy(array)

#define darray_resize(array, size) \
  _darray_resize(array, size)

#define darray_push(array, val) \
  {                             \
    typeof(val) temp = val;     \
    _darray_push(array, &temp);  \
  }
