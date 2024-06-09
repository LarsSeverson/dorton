#include "render_backend_fences.h"
#include "render_backend_fence.h"
#include "render/render_backend/render_backend.h"

#include "logger.h"

DResult render_backend_create_fences(RenderBackend *backend)
{
    u8 size = backend->swap_chain.max_frames_in_flight;

    darray_reserve(&backend->fences.active_buffer_fences, RenderBackendFence, size);

    for (u8 i = 0; i < size; ++i)
    {
        RenderBackendFence *fence = (RenderBackendFence *)darray_get(&backend->fences.active_buffer_fences, i);
        if (render_backend_create_fence(backend, fence) != D_SUCCESS)
        {
            DFATAL("Could not create backend active buffer fences.");
            return D_FATAL;
        }
    }

    return D_SUCCESS;
}

DResult render_backend_destroy_fences(RenderBackend *backend)
{
    u8 size = (u8)darray_size(&backend->fences.active_buffer_fences);

    for (u8 i = 0; i < size; ++i)
    {
        RenderBackendFence *fence = (RenderBackendFence *)darray_get(&backend->fences.active_buffer_fences, i);
        if (render_backend_destroy_fence(backend, fence) != D_SUCCESS)
        {
            return D_FATAL;
        }
    }

    darray_destroy(&backend->fences.active_buffer_fences);

    return D_SUCCESS;
}