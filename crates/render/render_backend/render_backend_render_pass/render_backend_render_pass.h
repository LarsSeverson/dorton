#pragma once

#include "render/render_backend/render_backend_core.h"

typedef struct RenderBackendRenderPass
{
  VkRenderPass render_pass_inner;
} RenderBackendRenderPass;

struct RenderBackend;

DResult render_backend_create_render_pass(struct RenderBackend *backend);
DResult render_backend_destroy_render_pass(struct RenderBackend *backend);
