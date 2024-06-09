#pragma once

#include "render/render_types.h"

#include "darray/darray.h"

typedef struct RenderBackendFences
{
    // RenderBackendFence
    DArray active_buffer_fences;
} RenderBackendFences;

struct RenderBackend;

DResult render_backend_create_fences(struct RenderBackend *backend);
DResult render_backend_destroy_fences(struct RenderBackend *backend);