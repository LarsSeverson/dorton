#include "darray.h"

#include "../../logger/logger.h"

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

DResult _darray_destroy(DArray *array)
{
  if (array->raw_array)
  {
    free(array->raw_array);
    array->raw_array = NULL;
  }
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
  if (array->size == array->capacity)
  {
    DResult result = darray_resize(array, array->capacity * DARRAY_RESIZE_FACTOR);
    if (result != D_SUCCESS)
    {
      return result;
    }
  }

  memcpy((char *)array->raw_array + array->stride * array->size, val, array->stride);
  array->size++;
  return D_SUCCESS;
}

DResult _darray_push_at(DArray *array, const void *val, u64 index)
{
  if (array->size <= index)
  {
    DERROR("Cannot push to an array with a size{%i} less than index{%i}.", array->size, index);
    return D_ERROR;
  }

  if (array->size == array->capacity)
  {
    DResult result = darray_resize(array, array->size * DARRAY_RESIZE_FACTOR);
    if (result != D_SUCCESS)
    {
      return result;
    }
  }

  char *base = (char *)array->raw_array + array->stride * index;
  memmove(base + array->stride, base, array->stride * (array->size - index));
  memcpy(base, val, array->stride);

  array->size++;

  return D_SUCCESS;
}

DResult _darray_pop(DArray *array)
{
  if (array->size == 0)
  {
    DERROR("Cannot pop an array that is empty.");
    return D_ERROR;
  }
  array->size--;
  if (array->size <= array->capacity / DARRAY_RESIZE_FACTOR)
  {
    darray_resize(array, array->size + 1);
  }

  return D_SUCCESS;
}

DResult _darray_pop_at(DArray *array, u64 index)
{
  if (array->size == 0)
  {
    DERROR("Cannot pop an array that is empty.");
    return D_ERROR;
  }
  if (array->size <= index)
  {
    DERROR("Cannot pop an array with a size{%i} less than index{%i}.", array->size, index);
    return D_ERROR;
  }

  char *base = (char *)array->raw_array + array->stride * index;

  memmove(base, base + array->stride, array->stride * (array->size - index - 1));
  array->size--;

  if (array->size <= array->capacity / DARRAY_RESIZE_FACTOR)
  {
    darray_resize(array, array->size + 1);
  }

  return D_SUCCESS;
}

void *_darray_get(DArray *array, u64 index)
{
  if (array->size <= index)
  {
    DERROR("Cannot get an element at index{%i} with array size{%i}.", index, array->size);
    return NULL;
  }

  char *val = (char *)array->raw_array + array->stride * index;
  return val;
}

DResult _darray_set(DArray *array, const void *val, u64 index)
{
  if (array->size <= index)
  {
    DERROR("Cannot set an element at index{%i} with array size{%i}.", index, array->size);
    return D_ERROR;
  }

  char *element_at_index = (char *)array->raw_array + array->stride * index;
  memcpy(element_at_index, val, array->stride);
  return D_SUCCESS;
}

void *_darray_data(DArray *array)
{
  return array->raw_array;
}

u64 darray_size(DArray *array)
{
  return array->size;
}
