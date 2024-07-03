#pragma once

#include "darray/darray.h"

#include "render/render_backend/render_backend_core.h"

#include "./render_backend_semaphore.h"

typedef struct RenderBackendSemaphores
{
    // RenderBackendSemaphore
    DArray semaphores_inner; 

} RenderBackendSemaphores;

struct RenderBackend;

DResult render_backend_create_semaphores(struct RenderBackend *backend, RenderBackendSemaphores *semaphores, u32 size);
DResult render_backend_destroy_semaphores(struct RenderBackend *backend, RenderBackendSemaphores *semaphores);

RenderBackendSemaphore *semaphores_get(RenderBackendSemaphores *semaphores, u32 index);