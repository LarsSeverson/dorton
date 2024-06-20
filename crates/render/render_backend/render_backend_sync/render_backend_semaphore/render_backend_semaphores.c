#include "render_backend_semaphores.h"
#include "render_backend_semaphore.h"

#include "logger.h"

#include "render/render_backend/render_backend.h"

DResult render_backend_create_semaphores(RenderBackend *backend)
{
    u8 size = backend->swap_chain.max_frames_in_flight;
    darray_reserve(&backend->semaphores.image_available_semaphores, RenderBackendSemaphore, size);
    darray_reserve(&backend->semaphores.render_finished_semaphores, RenderBackendSemaphore, size);

    for (u8 i = 0; i < size; ++i)
    {
        RenderBackendSemaphore *image_available_semaphore = (RenderBackendSemaphore *)darray_get(&backend->semaphores.image_available_semaphores, i);
        RenderBackendSemaphore *render_finished_semaphore = (RenderBackendSemaphore *)darray_get(&backend->semaphores.render_finished_semaphores, i);

        if (render_backend_create_semaphore(backend, image_available_semaphore) != D_SUCCESS)
        {
            DFATAL("Could not create image available semaphores.");
            return D_FATAL;
        }

        if (render_backend_create_semaphore(backend, render_finished_semaphore) != D_SUCCESS)
        {
            DFATAL("Could not create render finished semaphores.");
            return D_FATAL;
        }
    }

    backend->semaphores.image_available_semaphores_size = size;
    backend->semaphores.render_finished_semaphores_size = size;

    return D_SUCCESS;
}

DResult render_backend_destroy_semaphores(RenderBackend *backend)
{
    for (u32 i = 0; i < backend->semaphores.image_available_semaphores_size; ++i)
    {
        RenderBackendSemaphore *image_available_semaphore = (RenderBackendSemaphore *)darray_get(&backend->semaphores.image_available_semaphores, i);
        RenderBackendSemaphore *render_finished_semaphore = (RenderBackendSemaphore *)darray_get(&backend->semaphores.render_finished_semaphores, i);
        render_backend_destroy_semaphore(backend, image_available_semaphore);
        render_backend_destroy_semaphore(backend, render_finished_semaphore);
    }

    darray_destroy(&backend->semaphores.image_available_semaphores);
    darray_destroy(&backend->semaphores.render_finished_semaphores);

    return D_SUCCESS;
}