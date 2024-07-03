#include "render_backend_semaphores.h"

#include "logger.h"

#include "render/render_backend/render_backend.h"

DResult render_backend_create_semaphores(RenderBackend *backend, RenderBackendSemaphores *semaphores, u32 size)
{
    darray_reserve(&semaphores->semaphores_inner, RenderBackendSemaphore, size);

    for (u32 i = 0; i < size; ++i)
    {
        RenderBackendSemaphore *semaphore = (RenderBackendSemaphore *)darray_get(&semaphores->semaphores_inner, i);

        if (render_backend_create_semaphore(backend, semaphore) != D_SUCCESS)
        {
            DERROR("Could not create semaphores.");
            return D_ERROR;
        }
    }

    return D_SUCCESS;
}

DResult render_backend_destroy_semaphores(RenderBackend *backend, RenderBackendSemaphores *semaphores)
{
    u32 size = (u32)darray_size(&semaphores->semaphores_inner);
    for (u32 i = 0; i < size; ++i)
    {
        RenderBackendSemaphore *semaphore = (RenderBackendSemaphore *)darray_get(&semaphores->semaphores_inner, i);
        render_backend_destroy_semaphore(backend, semaphore);
    }

    darray_destroy(&semaphores->semaphores_inner);

    return D_SUCCESS;
}

RenderBackendSemaphore *semaphores_get(RenderBackendSemaphores *semaphores, u32 index)
{
    RenderBackendSemaphore *semaphore = (RenderBackendSemaphore *)darray_get(&semaphores->semaphores_inner, index);
    return semaphore;
}