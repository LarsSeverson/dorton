#pragma once

#include "render/render_backend/render_backend.h"

typedef struct FramebufferInfo
{
    u32 width;
    u32 height;
    u32 attachment_count;

    VkImageView *attachments;
    VkRenderPass render_pass;
} FramebufferInfo;

typedef struct RenderBackendFramebuffer
{
    VkFramebuffer framebuffer_inner;

    FramebufferInfo framebuffer_info;
} RenderBackendFramebuffer;

DResult render_backend_create_framebuffer(RenderBackend *backend, RenderBackendFramebuffer *framebuffer, FramebufferInfo *framebuffer_info);
DResult render_backend_destroy_framebuffer(RenderBackend *backend, RenderBackendFramebuffer *framebuffer);