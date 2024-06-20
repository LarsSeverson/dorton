#pragma once

#include "darray/darray.h"

#include "render/render_backend/render_backend_core.h"

typedef struct RenderBackendFramebuffers
{
    // RenderBackendFrameBuffer 
    DArray framebuffers_inner;

    u32 framebuffers_size;
} RenderBackendFramebuffers;

struct RenderBackend;

DResult render_backend_create_framebuffers(struct RenderBackend *backend);
DResult render_backend_destroy_framebuffers(struct RenderBackend *backend);