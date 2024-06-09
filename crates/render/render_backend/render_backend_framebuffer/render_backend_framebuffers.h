#pragma once

#include "render/render_types.h"

#include "darray/darray.h"

typedef struct RenderBackendFramebuffers
{
    // RenderBackendFrameBuffer 
    DArray framebuffers_inner;

    u32 framebuffers_size;
} RenderBackendFramebuffers;

struct RenderBackend;

DResult render_backend_create_framebuffers(struct RenderBackend *backend);
DResult render_backend_destroy_framebuffers(struct RenderBackend *backend);