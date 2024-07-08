#pragma once

#include "render/render_backend/render_backend_core.h"

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

struct RenderBackend;

DResult render_backend_create_fence(struct RenderBackend *backend, RenderBackendFence *fence);
DResult render_backend_destroy_fence(struct RenderBackend *backend, RenderBackendFence *fence);

dbool render_backend_wait_for_fence(struct RenderBackend *backend, RenderBackendFence *fence, u64 timeout_ns);
DResult render_backend_reset_fence(struct RenderBackend *backend, RenderBackendFence *fence);