#pragma once

#include "darray/darray.h"

#include "render/render_backend/render_backend_core.h"

typedef struct RenderBackendFences
{
    // RenderBackendFence
    DArray active_buffer_fences;
} RenderBackendFences;

struct RenderBackend;

DResult render_backend_create_fences(struct RenderBackend *backend);
DResult render_backend_destroy_fences(struct RenderBackend *backend);