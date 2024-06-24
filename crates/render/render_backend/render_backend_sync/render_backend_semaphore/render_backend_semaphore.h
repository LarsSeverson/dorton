#pragma once

#include "render/render_backend/render_backend.h"

// typedef enum SemaphoreState
// {
//     SEMAPHORE_STATE_SIGNALED = 0,
//     SEMAPHORE_STATE_NOT_SIGNALED
// } SemaphoreState;

typedef struct RenderBackendSemaphore
{
    VkSemaphore semaphore_inner;
    
} RenderBackendSemaphore;

DResult render_backend_create_semaphore(RenderBackend *backend, RenderBackendSemaphore *semaphore);
DResult render_backend_destroy_semaphore(RenderBackend *backend, RenderBackendSemaphore *semaphore);

dbool render_backend_wait_semaphore(RenderBackend *backend, RenderBackendSemaphore *semaphore, u64 timeout_ns);
DResult render_backend_reset_semaphore(RenderBackend *backend, RenderBackendSemaphore *semaphore);