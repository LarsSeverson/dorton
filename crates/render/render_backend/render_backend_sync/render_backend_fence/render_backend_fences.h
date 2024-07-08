#pragma once

#include "darray/darray.h"

#include "render/render_backend/render_backend_core.h"
#include "./render_backend_fence.h"

typedef struct RenderBackendFences
{
    // RenderBackendFence
    DArray fences_inner;
} RenderBackendFences;

struct RenderBackend;

DResult render_backend_create_fences(struct RenderBackend *backend, RenderBackendFences *fences, u32 size);
DResult render_backend_destroy_fences(struct RenderBackend *backend, RenderBackendFences *fences);

DResult render_backend_wait_for_fences(struct RenderBackend *backend, RenderBackendFences *fences, u32 index, u32 count, u64 timeout_ns);
DResult render_backend_reset_fences(struct RenderBackend *backend, RenderBackendFences *fences, u32 index, u32 count);

RenderBackendFence *fences_get(RenderBackendFences *fences, u32 index);