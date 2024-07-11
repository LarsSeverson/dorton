#pragma once

#include "darray/darray.h"

#include "render/render_backend/render_backend_core.h"
#include "render/render_backend/render_backend_render_pass/render_backend_render_pass.h"

#include "./render_backend_framebuffer.h"

typedef struct FramebuffersInfo
{   // TODO: More customization
    RenderBackendRenderPass *render_pass;

} FramebuffersInfo;

typedef struct RenderBackendFramebuffers
{
    // RenderBackendFrameBuffer
    DArray framebuffers_inner;

    u32 size;

} RenderBackendFramebuffers;

struct RenderBackend;

DResult render_backend_create_framebuffers(struct RenderBackend *backend, RenderBackendFramebuffers *framebuffers, FramebuffersInfo *framebuffers_info);
DResult render_backend_destroy_framebuffers(struct RenderBackend *backend, RenderBackendFramebuffers *framebuffers);
DResult render_backend_recreate_framebuffers(struct RenderBackend *backend, RenderBackendFramebuffers *framebuffers, FramebuffersInfo *framebuffers_info);

RenderBackendFramebuffer *framebuffers_get(RenderBackendFramebuffers *framebuffers, u32 index);