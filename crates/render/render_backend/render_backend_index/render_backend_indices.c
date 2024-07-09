#include "render_backend_indices.h"

#include "logger.h"

DResult render_backend_create_indices(RenderBackendIndices *indices, RenderBackendIndex *indices_data, u32 indices_data_size)
{
    if (indices_data == NULL || indices == NULL)
    {
        DERROR("Could not create indices.");
        return D_ERROR;
    }

    darray_reserve(&indices->indices_inner, RenderBackendIndex, indices_data_size);
    RenderBackendIndex *indices_raw_data = (RenderBackendIndex *)darray_data(&indices->indices_inner);

    for (u32 i = 0; i < indices_data_size; ++i)
    {
        indices_raw_data[i] = indices_data[i];
    }

    indices->size = indices_data_size;

    return D_SUCCESS;
}

DResult render_backend_destroy_indices(RenderBackendIndices *indices)
{
    darray_destroy(&indices->indices_inner);
    *indices = (RenderBackendIndices){0};

    return D_SUCCESS;
}

DResult render_backend_indices_push(RenderBackendIndices *indices, RenderBackendIndex index)
{
    darray_push(&indices->indices_inner, index);
    indices->size = (u32)darray_size(&indices->indices_inner);

    return D_SUCCESS;
}

RenderBackendIndex render_backend_indices_get(RenderBackendIndices *indices, u32 index)
{
    return *(RenderBackendIndex *)darray_get(&indices->indices_inner, index);
}

RenderBackendIndex *render_backend_indices_data(RenderBackendIndices *indices)
{
    return (RenderBackendIndex *)darray_data(&indices->indices_inner);
}

u32 render_backend_indices_size(RenderBackendIndices *indices)
{
    return indices->size;
}