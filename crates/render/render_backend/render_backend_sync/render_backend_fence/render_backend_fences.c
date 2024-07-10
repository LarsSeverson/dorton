#include "render_backend_fences.h"
#include "render_backend_fence.h"

#include "logger.h"

#include "render/render_backend/render_backend.h"

DResult render_backend_create_fences(RenderBackend *backend, RenderBackendFences *fences, u32 size)
{
    darray_reserve(&fences->fences_inner, RenderBackendFence, size);

    for (u32 i = 0; i < size; ++i)
    {
        RenderBackendFence *fence = (RenderBackendFence *)darray_get(&fences->fences_inner, i);
        if (render_backend_create_fence(backend, fence) != D_SUCCESS)
        {
            DFATAL("Could not create fences.");
            return D_FATAL;
        }
    }

    return D_SUCCESS;
}

DResult render_backend_destroy_fences(RenderBackend *backend, RenderBackendFences *fences)
{
    u32 size = (u32)darray_size(&fences->fences_inner);

    for (u32 i = 0; i < size; ++i)
    {
        RenderBackendFence *fence = (RenderBackendFence *)darray_get(&fences->fences_inner, i);
        if (render_backend_destroy_fence(backend, fence) != D_SUCCESS)
        {
            return D_ERROR;
        }
    }

    darray_destroy(&fences->fences_inner);

    return D_SUCCESS;
}

DResult render_backend_wait_for_fences(RenderBackend *backend, RenderBackendFences *fences, u32 index, u32 count, u64 timeout_ns)
{
    u32 fences_size = darray_size(&fences->fences_inner);
    if (index >= fences_size || index + count > fences_size)
    {
        DERROR("Index out of range for fences size: %d.", fences_size);
        return D_ERROR;
    }

    for (u32 i = index; i < index + count; ++i)
    {
        RenderBackendFence *fence = (RenderBackendFence *)darray_get(&fences->fences_inner, i);
        while(render_backend_wait_for_fence(backend, fence, timeout_ns) == false);
    }

    return D_SUCCESS;
}

DResult render_backend_reset_fences(RenderBackend *backend, RenderBackendFences *fences, u32 index, u32 count)
{
    u32 fences_size = darray_size(&fences->fences_inner);
    if (index >= fences_size || index + count > fences_size)
    {
        DERROR("Index out of range for fences size: %d.", fences_size);
        return D_ERROR;
    }

    for (u32 i = index; i < index + count; ++i)
    {
        RenderBackendFence *fence = (RenderBackendFence *)darray_get(&fences->fences_inner, i);
        if (render_backend_reset_fence(backend, fence) == D_ERROR)
        {
            DERROR("Could not reset for fences.");
            return D_ERROR;
        }
    }

    return D_SUCCESS;
}

RenderBackendFence *fences_get(RenderBackendFences *fences, u32 index)
{
    RenderBackendFence *fence = (RenderBackendFence *)darray_get(&fences->fences_inner, index);

    return fence;
}