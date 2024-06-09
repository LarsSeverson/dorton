#pragma once

#include "render/render_backend/render_backend.h"

typedef enum FenceState
{
    FENCE_STATE_SIGNALED = 0,
    FENCE_STATE_NOT_SIGNALED
} FenceState;

typedef struct RenderBackendFence
{
    VkFence fence_inner;

    FenceState fence_state;
} RenderBackendFence;

DResult render_backend_create_fence(RenderBackend *backend, RenderBackendFence *fence);
DResult render_backend_destroy_fence(RenderBackend *backend, RenderBackendFence *fence);

b8 render_backend_wait_fence(RenderBackend *backend, RenderBackendFence *fence, u64 timeout_ns);
DResult render_backend_reset_fence(RenderBackend *backend, RenderBackendFence *fence);