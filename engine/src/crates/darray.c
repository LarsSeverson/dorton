#include "darray.h"

#include "core/logger.h"

#include <stdlib.h>
#include <string.h>

DResult _darray_create(DArray *array, u64 length, u64 stride)
{
  void *raw_array = (void *)calloc(length, stride);

  array->size = 0;
  array->capacity = length;
  array->stride = stride;
  array->raw_array = raw_array;

  return D_SUCCESS;
}
DResult _darray_destroy(DArray array)
{
  if (array.raw_array)
  {
    free(array.raw_array);
    array.raw_array = NULL;
  }
  return D_SUCCESS;
}

DResult _darray_set(void *array, u64 field)
{
  return D_SUCCESS;
}

DResult _darray_resize(DArray *array, u64 size)
{
  void *new_raw_array = realloc(array->raw_array, array->stride * size);
  if (!new_raw_array)
  {
    return D_ERROR;
  }

  if (size > array->capacity)
  {
    memset((char *)new_raw_array + array->stride * array->capacity, 0, array->stride * (size - array->capacity));
  }

  array->raw_array = new_raw_array;
  array->capacity = size;

  return D_SUCCESS;
}

DResult _darray_push(DArray *array, const void *val)
{
  if (array->size == array->capacity) {
    DResult result = darray_resize(array, array->capacity * DARRAY_RESIZE_FACTOR);
    if (result != D_SUCCESS) {
      return result;
    }
  }

  memcpy((char *)array->raw_array + array->size * array->stride, val, array->stride);
  array->size++;
  return D_SUCCESS;
}

DResult _darray_pop(void *array, void *dest)
{
  return D_SUCCESS;
}

void *_darray_push_at(void *array, const void *val)
{
  return array;
}

DResult _darray_pop_at(void *array, void *dest)
{

  return D_SUCCESS;
}