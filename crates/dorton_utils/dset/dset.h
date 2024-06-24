#pragma once

#include "defines.h"

typedef i32 (*DCompareFunc)(const void *val1, const void *val2);

DAPI i32 default_int_compare(const void *val1, const void *val2);
DAPI i32 default_str_compare(const void *val1, const void *val2);

#define DSET_DEFAULT_INT_COMP_FUNC default_int_compare
#define DSET_DEFAULT_STR_COMP_FUNC default_str_compare

typedef struct DSet
{
  void *raw_set;

  u64 capacity;
  u64 size;
  u64 stride;

  DCompareFunc compare_func;
} DSet;

DAPI DResult _dset_create(DSet *set, u64 length, u64 stride, DCompareFunc compare_func);
DAPI DResult _dset_destroy(DSet *set);

DAPI DResult _dset_resize(DSet *set, u64 size);

DAPI DResult _dset_push(DSet *set, const void *val);
DAPI DResult _dset_pop(DSet *set);

DAPI DResult _dset_pop_at(DSet *set, u64 index);
DAPI DResult _dset_pop_val(DSet *set, const void *val);

DAPI const void *_dset_get(DSet *set, u64 index);
DAPI i32 _dset_find(DSet *set, const void *val);

DAPI void *dset_data(DSet *set);
DAPI dbool dset_empty(DSet *set);

DAPI u64 dset_size(DSet *set);

#define DSET_DEFAULT_SIZE 1
#define DSET_RESIZE_FACTOR 2

#define dset_create(set, type, comp_func) \
  _dset_create(set, DSET_DEFAULT_SIZE, sizeof(type), comp_func)

#define dset_reserve(set, type, size) \
  _dset_create(set, size, sizeof(type))

#define dset_destroy(set) \
  _dset_destroy(set)

#define dset_resize(set, size) \
  _dset_resize(set, size)

#define dset_push(set, val) \
  {                         \
    typeof(val) temp = val; \
    _dset_push(set, &temp); \
  }

#define dset_pop_back(set) \
  _dset_pop(set)

#define dset_erase(set, index) \
  _dset_pop_at(set, index)

#define dset_erase_val(set, val) \
  _dset_pop_val(set, val)

#define dset_get(set, index) \
  _dset_get(set, index)

#define dset_find(set, val) \
  _dset_find(set, val)
