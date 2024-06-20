#pragma once

#include "darray/darray.h"

#include "render/render_backend/render_backend_core.h"

typedef struct RenderBackendSemaphores
{
    // RenderBackendSemaphore
    DArray image_available_semaphores;
    u32 image_available_semaphores_size;
    
    // RenderBackendSemaphore
    DArray render_finished_semaphores;
    u32 render_finished_semaphores_size;

} RenderBackendSemaphores;

struct RenderBackend;

DResult render_backend_create_semaphores(struct RenderBackend *backend);
DResult render_backend_destroy_semaphores(struct RenderBackend *backend);