#pragma once

#include "darray/darray.h"
 
#include "./render_backend_index.h"

typedef struct RenderBackendIndices
{
    // RenderBackendIndex
    DArray indices_inner;
    u32 size;
    
} RenderBackendIndices;

DResult render_backend_create_indices(RenderBackendIndices *indices, RenderBackendIndex *indices_data, u32 indices_data_size);
DResult render_backend_destroy_indices(RenderBackendIndices *indices);

DResult render_backend_indices_push(RenderBackendIndices *indices, RenderBackendIndex index);

RenderBackendIndex render_backend_indices_get(RenderBackendIndices *indices, u32 index);
RenderBackendIndex *render_backend_indices_data(RenderBackendIndices *indices);

u32 render_backend_indices_size(RenderBackendIndices *indices);