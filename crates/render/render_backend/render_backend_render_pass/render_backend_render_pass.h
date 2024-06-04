#pragma once

#include "render/render_types.h"

// "../render_backend.h"
struct RenderBackend;

typedef struct RenderBackendRenderPass
{
  VkRenderPass render_pass_inner;
} RenderBackendRenderPass;

DResult render_backend_create_render_pass(struct RenderBackend *backend);
DResult render_backend_destroy_render_pass(struct RenderBackend *backend);
